#ifndef __LOWER_GAT_1400_OBSERVER_H_
#define __LOWER_GAT_1400_OBSERVER_H_

#include "CMS1400Struct.h"

class CLower1400SubscribeObserver
{
public:
	CLower1400SubscribeObserver(){}
	virtual ~CLower1400SubscribeObserver(){}
	virtual void AddSubscribe(const GAT_1400_Subscribe& subscribe) = 0;   //添加订阅
	virtual void UpdateSubscribe(const GAT_1400_Subscribe& subscribe) = 0;//修改订阅
	virtual void DelSubscribe(const std::string& subscribeId) = 0;   //删除订阅
};

enum regist_state{
	EM_REGIST_ON,
	EM_REGIST_OFF
};

class CLower1400RegistStatusObserver
{
public:
	CLower1400RegistStatusObserver(){}
	virtual ~CLower1400RegistStatusObserver(){}
	virtual void UpdateRegistStatus(enum regist_state state) = 0;//注册状态
};

#endif