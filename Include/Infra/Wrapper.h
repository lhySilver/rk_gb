#ifndef __INFRA_WRAPPER_H__
#define __INFRA_WRAPPER_H__
#include <assert.h>
#ifndef NULL
#define NULL   0
#endif

template<class T>
class TWrapper
{
public:
	/// 构造函数，创建T对象
	TWrapper(T *p = NULL)
	{
		m_t = p;
		if (!p)
		{
			m_t = T::create();
		}
	}

	/// 析构函数，销毁T对象
	virtual ~TWrapper()
	{
		delete m_t;
	}

	/// 返回T对象指针
	T* operator->() 
	{
		return m_t; 
	}

	/// 返回T对象指针
	T* operator &() 
	{
		return m_t; 
	}

private:
	T *m_t;
};

template<class T>
class TStrWrapper
{
public:
	/// 构造函数，创建T对象
	TStrWrapper()
	{
		m_t = new T;
		if (m_t == NULL)
		{
			assert(0);
		}
	}

	/// 析构函数，销毁T对象
	virtual ~TStrWrapper()
	{
		delete m_t;
	}

	/// 返回T对象指针
	T* operator->() 
	{
		return m_t; 
	}

	/// 返回T对象指针
	T* operator &() 
	{
		return m_t; 
	}

	T & operator *() 
	{
		return *m_t; 
	}

private:
	T *m_t;
};

#endif
