/*
 * Copyright (c) 2023 DGIOT
 *
 * NOTICE: ALL RIGHTS RESERVED.
 *
 * This source code file is the exclusive property of DGIOT.
 * It is confidential and proprietary information.
 *
 * No part of this file may be copied, reproduced, transmitted, distributed,
 * sold, disclosed, or otherwise used in any form or by any means without
 * the express written permission of DGIOT.
 *
 * Violation of this copyright notice may result in severe legal consequences.
 * By accessing this source code, you agree to abide by the terms of this
 * copyright notice.
 */
#ifndef TEST_ASK_BASE_H
#define TEST_ASK_BASE_H
#include "ITestModuleV2.h"
#include <semaphore.h>
constexpr unsigned int ASK_NEVER_TIMEOUT = 0;
constexpr unsigned int DEFAULT_ASK_TIMEOUT = 3 * 1000;
enum class TestAskBlock
{
    BLOCK = 0,
    NOT_BLOCK,
    UNRELATED,
    END
};
enum class TestAskReply
{
    NEED_REPLY = 0,
    NEED_NOT_REPLY,
    END
};
class TestAskBase : virtual public VTestAsk
{
public:
    TestAskBase(const TestAskBlock &isBlock, const TestAskReply &needReply,
                const unsigned int &timeoutMs = DEFAULT_ASK_TIMEOUT);
    virtual ~TestAskBase() = default;
    TestCode Blocking(void) override;
    bool NeedReply(void) override;
    void ReplyFinished(const TestCode &code) override;
    bool IfTimeout(const unsigned int &integrationTimeMs) override;

protected:
    sem_t mSem;
    TestAskBlock mIsBlock;
    TestAskReply mNeedReply;
    TestCode mReplyOK;
    unsigned int mTimeout;
    unsigned int mWaitingTimeMs;
};
#endif