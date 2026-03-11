#include "ShareSDK.h"

#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

namespace {

class ShimThread {
public:
    ShimThread(LPTHREAD_ROUTINE routine, LPVOID parameter, tuint32 sleep_ms, bool suspended)
        : m_routine(routine),
          m_parameter(parameter),
          m_sleep_ms(sleep_ms),
          m_suspended(suspended),
          m_destroying(false),
          m_started(false) {
    }

    bool Start() {
        const int ret = pthread_create(&m_thread, NULL, &ShimThread::ThreadMain, this);
        return ret == 0;
    }

    void Resume() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_suspended = false;
            m_started = true;
        }
        m_cv.notify_all();
    }

    void Destroy() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_destroying = true;
            m_suspended = false;
            m_started = true;
        }
        m_cv.notify_all();
        pthread_join(m_thread, NULL);
    }

    tuint_ptr ThreadId() const {
        return static_cast<tuint_ptr>(m_thread);
    }

private:
    static void* ThreadMain(void* opaque) {
        ShimThread* self = static_cast<ShimThread*>(opaque);
        self->Run();
        return NULL;
    }

    void Run() {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (m_suspended && !m_destroying) {
                m_cv.wait(lock);
            }
            if (m_destroying) {
                return;
            }
        }

        if (m_sleep_ms > 0) {
            usleep(static_cast<useconds_t>(m_sleep_ms) * 1000);
        }

        if (m_routine != NULL) {
            m_routine(m_parameter);
        }
    }

private:
    pthread_t m_thread;
    LPTHREAD_ROUTINE m_routine;
    LPVOID m_parameter;
    tuint32 m_sleep_ms;
    bool m_suspended;
    bool m_destroying;
    bool m_started;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

struct TimerState {
    tuint32 id;
    tuint32 elapse;
    TimerCBFun* callback;
    LPVOID parameter;
    std::mutex mutex;
    std::condition_variable cv;
    bool cancelled;
    std::thread worker;

    TimerState()
        : id(0), elapse(0), callback(NULL), parameter(NULL), cancelled(false) {
    }
};

std::atomic<int> g_log_level(CLogWrapper::LEVEL_INFO);
std::atomic<tuint32> g_next_timer_id(1);
std::mutex g_timer_mutex;
std::map<tuint32, std::shared_ptr<TimerState> > g_timers;

void ShimLog(const char* level, const char* message, const char* file, tuint32 line) {
    if (message == NULL) {
        message = "";
    }
    if (file == NULL) {
        file = "";
    }
    fprintf(stderr, "[GBSDK][%s] %s (%s:%u)\n", level, message, file, static_cast<unsigned>(line));
}

void TimerThreadMain(std::shared_ptr<TimerState> state) {
    {
        std::unique_lock<std::mutex> lock(state->mutex);
        if (!state->cv.wait_for(lock,
                                std::chrono::milliseconds(state->elapse),
                                [state]() { return state->cancelled; })) {
            if (state->callback != NULL) {
                state->callback(state->id, state->elapse, state->parameter);
            }
        }
    }

    std::lock_guard<std::mutex> lock(g_timer_mutex);
    std::map<tuint32, std::shared_ptr<TimerState> >::iterator it = g_timers.find(state->id);
    if (it != g_timers.end() && it->second.get() == state.get()) {
        g_timers.erase(it);
    }
}

}  // namespace


extern "C" void uuid_generate(unsigned char* out) {
    if (out == NULL) {
        return;
    }
    FILE* fp = fopen("/dev/urandom", "rb");
    if (fp != NULL) {
        size_t n = fread(out, 1, 16, fp);
        fclose(fp);
        if (n == 16) {
            out[6] = static_cast<unsigned char>((out[6] & 0x0F) | 0x40);
            out[8] = static_cast<unsigned char>((out[8] & 0x3F) | 0x80);
            return;
        }
    }
    const unsigned int seed = static_cast<unsigned int>(time(NULL) ^ getpid());
    srand(seed);
    for (int i = 0; i < 16; ++i) {
        out[i] = static_cast<unsigned char>(rand() & 0xFF);
    }
    out[6] = static_cast<unsigned char>((out[6] & 0x0F) | 0x40);
    out[8] = static_cast<unsigned char>((out[8] & 0x3F) | 0x80);
}

CLogWrapper::CLogWrapper() {
}

CLogWrapper::~CLogWrapper() {
}

void CLogWrapper::Log(const LOG_LEVEL level, const char* logInfo, const char* file, const char* fun, int line) {
    (void)fun;
    const char* level_name = "INFO";
    switch (level) {
    case LEVEL_DEBUG:
        level_name = "DEBUG";
        break;
    case LEVEL_INFO:
        level_name = "INFO";
        break;
    case LEVEL_WARN:
        level_name = "WARN";
        break;
    case LEVEL_ERROR:
        level_name = "ERROR";
        break;
    case LEVEL_FATAL:
        level_name = "FATAL";
        break;
    default:
        break;
    }
    ShimLog(level_name, logInfo, file, static_cast<tuint32>(line < 0 ? 0 : line));
}

void CLogWrapper::Debug(const char* logInfo) {
    Log(LEVEL_DEBUG, logInfo, NULL, NULL, 0);
}

void CLogWrapper::Info(const char* logInfo) {
    Log(LEVEL_INFO, logInfo, NULL, NULL, 0);
}

void CLogWrapper::Warn(const char* logInfo) {
    Log(LEVEL_WARN, logInfo, NULL, NULL, 0);
}

void CLogWrapper::Error(const char* logInfo) {
    Log(LEVEL_ERROR, logInfo, NULL, NULL, 0);
}

void CLogWrapper::Fatal(const char* logInfo) {
    Log(LEVEL_FATAL, logInfo, NULL, NULL, 0);
}

int CLogWrapper::SetLevel(LOG_LEVEL level) {
    g_log_level.store(static_cast<int>(level));
    return 0;
}

const CLogWrapper::LOG_LEVEL CLogWrapper::GetLevel() {
    return static_cast<CLogWrapper::LOG_LEVEL>(g_log_level.load());
}

void CLogWrapper::Init(const char* exeName, const char* dirPath) {
    (void)exeName;
    (void)dirPath;
}

void CLogWrapper::DeInit() {
}

void SHARESDK_AbnormalOutput(const char* logInfo, const char* fileName, tuint32 line) {
    ShimLog("ABNORMAL", logInfo, fileName, line);
}

void SHARESDK_NormalOutput(const char* logInfo, const char* fileName, tuint32 line) {
    ShimLog("NORMAL", logInfo, fileName, line);
}

void SHARESDK_DebugOutput(const char* logInfo, const char* fileName, tuint32 line) {
    ShimLog("DEBUG", logInfo, fileName, line);
}

void SHARESDK_WarnPrintf(const char ch, const char* logInfo, const char* fileName, tuint32 line) {
    (void)ch;
    ShimLog("WARN", logInfo, fileName, line);
}

void SHARESDK_RunPrintf(const char ch, const char* logInfo, const char* fileName, tuint32 line) {
    (void)ch;
    ShimLog("RUN", logInfo, fileName, line);
}

void SHARESDK_DebugPrintf(const char ch, const char* logInfo, const char* fileName, tuint32 line) {
    (void)ch;
    ShimLog("DEBUG", logInfo, fileName, line);
}

void SHARESDK_ErrorPrintf(const char ch, const char* logInfo, const char* fileName, tuint32 line) {
    (void)ch;
    ShimLog("ERROR", logInfo, fileName, line);
}

void SHARESDK_AddOutputObserver(COutputBase* pOutObject) {
    (void)pOutObject;
}

void SHARESDK_DelOutputObserver(COutputBase* pOutObject) {
    (void)pOutObject;
}

HANDLE SHARESDK_CreateThread(LPTHREAD_ROUTINE lpStartAddress,
                             LPVOID lpParameter,
                             tuint32 sleepTime,
                             bool bCreatSuspended,
                             const char* fileName,
                             int lineNum,
                             const char* description) {
    (void)fileName;
    (void)lineNum;
    (void)description;
    std::unique_ptr<ShimThread> holder(new ShimThread(lpStartAddress, lpParameter, sleepTime, bCreatSuspended));
    if (!holder->Start()) {
        return 0;
    }
    return static_cast<HANDLE>(reinterpret_cast<tuint_ptr>(holder.release()));
}

HANDLE SHARESDK_CreateRunOneThread(LPTHREAD_ROUTINE lpStartAddress,
                                   LPVOID lpParameter,
                                   bool bCreatSuspended,
                                   const char* fileName,
                                   int lineNum,
                                   const char* description) {
    return SHARESDK_CreateThread(lpStartAddress, lpParameter, 0, bCreatSuspended, fileName, lineNum, description);
}

bool SHARESDK_RunOneThread(LPTHREAD_ROUTINE lpStartAddress,
                           LPVOID lpParameter,
                           const char* fileName,
                           const char* funName,
                           int lineNum,
                           const char* description) {
    (void)fileName;
    (void)funName;
    (void)lineNum;
    (void)description;
    pthread_t thread;
    struct DetachedContext {
        LPTHREAD_ROUTINE routine;
        LPVOID parameter;
    };
    DetachedContext* context = new DetachedContext();
    context->routine = lpStartAddress;
    context->parameter = lpParameter;
    int ret = pthread_create(
        &thread,
        NULL,
        [](void* opaque) -> void* {
            std::unique_ptr<DetachedContext> context(static_cast<DetachedContext*>(opaque));
            if (context->routine != NULL) {
                context->routine(context->parameter);
            }
            return NULL;
        },
        context);
    if (ret != 0) {
        delete context;
        return false;
    }
    pthread_detach(thread);
    return true;
}

void SHARESDK_DestroyThread(HANDLE threadHandle, const char* fileName, int lineNum) {
    (void)fileName;
    (void)lineNum;
    ShimThread* thread = reinterpret_cast<ShimThread*>(static_cast<tuint_ptr>(threadHandle));
    if (thread == NULL) {
        return;
    }
    thread->Destroy();
    delete thread;
}

bool SHARESDK_AttachThreadToCPU(HANDLE threadHandle, int cpuindex) {
    (void)threadHandle;
    (void)cpuindex;
    return true;
}

bool SHARESDK_SetThreadRunProperty(const char* pBufForXMLData) {
    (void)pBufForXMLData;
    return true;
}

tuint_ptr SHARESDK_GetThreadID(HANDLE threadHandle) {
    ShimThread* thread = reinterpret_cast<ShimThread*>(static_cast<tuint_ptr>(threadHandle));
    if (thread == NULL) {
        return static_cast<tuint_ptr>(pthread_self());
    }
    return thread->ThreadId();
}

bool SHARESDK_ResumeThread(HANDLE threadHandle) {
    ShimThread* thread = reinterpret_cast<ShimThread*>(static_cast<tuint_ptr>(threadHandle));
    if (thread == NULL) {
        return false;
    }
    thread->Resume();
    return true;
}

void SHARESDK_CreateSingleton(LPVOID pSingleTon, const char* name) {
    (void)pSingleTon;
    (void)name;
}

void SHARESDK_DestroySingleton(LPVOID pSingleTon) {
    (void)pSingleTon;
}

tuint32 SHARESDK_SetTimer(tuint32 dwElapse, TimerCBFun* pCBFun, LPVOID lpParameter) {
    std::shared_ptr<TimerState> state(new TimerState());
    state->id = g_next_timer_id.fetch_add(1);
    state->elapse = dwElapse;
    state->callback = pCBFun;
    state->parameter = lpParameter;
    {
        std::lock_guard<std::mutex> lock(g_timer_mutex);
        g_timers[state->id] = state;
    }
    state->worker = std::thread(TimerThreadMain, state);
    state->worker.detach();
    return state->id;
}

bool SHARESDK_KillTimer(tuint32 dwTimerID) {
    std::shared_ptr<TimerState> state;
    {
        std::lock_guard<std::mutex> lock(g_timer_mutex);
        std::map<tuint32, std::shared_ptr<TimerState> >::iterator it = g_timers.find(dwTimerID);
        if (it == g_timers.end()) {
            return false;
        }
        state = it->second;
        g_timers.erase(it);
    }
    {
        std::lock_guard<std::mutex> lock(state->mutex);
        state->cancelled = true;
    }
    state->cv.notify_all();
    return true;
}
