#ifndef __INTER_MANAGER_CONFIGMANAGER_H__
#define __INTER_MANAGER_CONFIGMANAGER_H__

#include "Manager/ConfigManager.h"
#include "Infra/Singleton.h"
#include "Infra/File.h"
#include "Zlib/zlib.h"
#include "Infra/Signals.h"
#include "Infra/Timer.h"
//#include <Json/value.h>
#include <Json/json.h>
#include "Event.h"
#include <map>


class CConfigManager : public IConfigManager
{
	PATTERN_SINGLETON_DECLARE(CConfigManager);

public:

	///< 初始化，启动配置管理
	void start();


	static void mkdirConfigPath(const char *dir);

	/// 配置接口
	/// \param [in] firstDir 主配置文件的存放目录
	/// \param [in] secondDir 备份配置文件的存放目录
	/// \param [in] customDir 用户自定义配置文件的存放目录
	static void config(const char *firstDir, const char *secondDir, const char *customDir);

	/// 删除配置文件，需要重启，重启后所有默认被恢复成默认配置
	/// \param user 用户名
	void deleteFile(const char* user = NULL);

	// 得到最新的配置结构
	/// \param [in] name 配置名称
	/// \param [out] table 配置表
	/// \note name遵循如下语法规则
	/// 单一个字符串将取整个配置
	/// 字符串后跟[%d], .key代表要访问相应的配置项
	/// - "." => 根节点，和name和name.意义一样
	/// - ".key" => 访问配置内名为key的配置项
	/// - ".[n]" => 访问撇之内第n+1个配置项
	/// 以上过程可以递归
	/// 比如访问录像配置name为getConfigName(CFG_RECORD)
	/// 访问第一通道的录像配置name为"Record.[0]"
	/// 访问第一通道录像配置的时间段的配置为"Record.[0].TimeSection"
	bool getConfig(const char* name, Json::Value& table);

	/// 提交配置，含合法性检查，调用saveFile函数保存文件。
	/// \param name 配置名称
	/// \param table 新的配置
	/// \param index 配置序号
	/// \param user 用户名
	/// \param applyOptions 返回值初始值，取ApplyOptions位与值。
	/// \param notifyParent 回调时通知上层节点的回调函数
	/// \return 配置应用的结果，取ApplyOptions位与值，applyOptions作为返回值的初始值。
	/// \note name的语法参见getConfig()
	int setConfig(const char* name, const Json::Value& table, const char* user = 0, int applyOptions = applyDelaySave);

	/// 获得最新配置的引用, 该函数只能在配置校验的回调函数里使用
	/// \param [in] name 要取的配置的路径
	const Json::Value& resolveConfig(const char *name);

	// 得到默认的配置表
	/// \param [in] name 配置名称
	/// \param [out] 配置表
	bool getDefault(const char* name, Json::Value& table);

	/// 设置默认配置表，一般对每个配置在初始化时调用一次即可，必须在配置使用之前调用
	/// 此函数仅供CConfigManager调用。
	/// \param [in] name 配置名称
	/// \param table 默认配置表
	void setDefault(const char* name, const Json::Value& table, bool bProduceConfig = false);

	/// 注册配置变化观察者函数。
	/// \param name 配置名称
	/// \param proc 观察者函数
	/// \retval >0 当前已经注册的观察者数目。
	/// \return <0 TSignalN::ErrorCode类型的值。
	int attach(const char* name, Proc proc);

	/// 注销配置变化观察者函数。
	/// \param name 配置名称
	/// \param proc 观察者函数
	/// \retval >=0 当前已经注册的观察者数目。
	/// \return <0 TSignalN::ErrorCode类型的值。
	int detach(const char* name, Proc proc);

	/// 注册配置校验观察者函数, 必须配置使用前调用。
	/// \param name 配置名称
	/// \param proc 观察者函数
	/// \retval >0 当前已经注册的观察者数目。
	/// \return <0 TSignalN::ErrorCode类型的值。
	int attachVerify(const char* name, Proc proc);

	/// 注销配置校验观察者函数。
	/// \param name 配置名称
	/// \param proc 观察者函数
	/// \retval >=0 当前已经注册的观察者数目。
	/// \return  <0 TSignalN::ErrorCode类型的值。
	int detachVerify(const char* name, Proc proc);

	/// 注册钩子函数
	/// \param [in] name 要观察的配置名称
	/// \param [in] pObj 钩子对象指针
	/// \param [in] proc 钩子函数指针
	/// \retval >= 0 当前已经注册的观察者函数
	/// \retval < 0 TSignalN::ErrorCode类型的值
	int hooks(const char *name, HookProc proc);

	/// 恢复到上一次正确的配置
	void restore();

	// 保存配置文件
	void saveFile();

	/// 控制台调试接口
	int onConsole(int argc, char **argv);
private:

	/// 替换dest中所有src中存在的项
	/// \param fillEmptyOnly true-只替换dest中为空，src中不为空的项，false-替换所有项
	void replaceConfig(Json::Value& dest, const Json::Value& src, bool fillEmptyOnly = false);

	/// 读取配置
	bool readConfig(const char* chPath, std::string& input);

	/// 定时器回调函数，保存配置
	void onTimer(uint arg);
	
	/// 事件回调函数
	void onAppEvent(std::string code, int index, appEventAction action, const EventHandler *param, Json::Value* data);

	int setConfigInter(const char* name, const Json::Value& table, const char* user = 0, int applyOptions = applyDelaySave);
	
	bool getConfigInter(const char* name, Json::Value& table);

	bool getDefaultInter(const char* name, Json::Value& table);

	void makeNewConfig(const Json::Value &defaultConfig, const Json::Value &nowConfig, Json::Value &newConfig);

private:
	Json::Value m_configAll;				/// 配置总表
	Json::Value m_configDefault;			///< Json默认配置表
	Json::Value m_configCustom;				///< Json用户自定义配置表
	std::map<std::string, bool>	m_changed;	/// 配置表变化了
	gzFile	m_fileConfig;					/// 配置文件

	CTimer		m_cfgTimer;
	std::string m_stream;	// 字符串流
	CMutex		m_mutex;

	typedef std::map<std::string, Signal*> SignalMap;
	typedef std::map<std::string, std::string> ValidMap;
	typedef std::map<std::string, HookSignal*> HookSignalMap;

	SignalMap m_applyMap;
	SignalMap m_verifyMap;      // 校验函数map
	ValidMap m_validMap;        // 有效的map,即已验证过的
	HookSignalMap m_hookMap;

	static std::string sm_firstFileDir;		///< 配置文件路径1
	static std::string sm_secondFileDir;	///< 配置文件路径2
	static std::string sm_customFileDir;	///< 用户自定义配置文件
};

#endif //__CONFIG_MANAGER__
