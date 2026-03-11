////////////////////Singleton.h
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "ShareSDK.h"
/************************************************************************
如果想要是单件           不是线程安全的
************************************************************************/
template <class T> 
class CSingleton
{
public:
	static T* GetInstance()
	{ 
		if ( NULL == m_instance )
		{
			ASSERT(m_Count == 0);
			m_Count++;
			m_instance = new T;
			CREATE_SINGLETON(m_instance,T);
		}

		return m_instance;
	}

	static void ReleaseInstance()
	{
		if ( NULL != m_instance )
		{
			ASSERT(m_Count == 1);
			m_Count--;
			DESTROY_SINGLETON(m_instance);
			delete m_instance;
			m_instance = NULL;
		}
	}

protected:
	CSingleton() {}
	virtual ~CSingleton() {}

private:
	CSingleton(const CSingleton&){}
	CSingleton& operator=(const CSingleton&){}

private:
	static T* m_instance;
	static int m_Count;//计数，程序在整个运行过程中只能创建和销毁一次
	//主要是为了解决Quit函数和释放内存顺序问题
};

template <class T>
T* CSingleton<T>::m_instance = NULL;

template <class T>
int CSingleton<T>::m_Count = 0;

#endif