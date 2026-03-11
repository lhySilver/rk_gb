
#ifndef OBSERVABLE_H
#define OBSERVABLE_H

//////////包含头文件 
#include <list>
#include "Lock.h"
#include "Observer.h"
#include "MySingleLock.h"

//////////声明类原形
template <class T> class CObserver;

template<class TYPE>
class CObservable
{
public:
	CObservable(void);
public:
	virtual ~CObservable(void);

	//void SetChanged();
	/////////////加入需要观察的对象
	void AddObserver(CObserver<TYPE> *pObServer);

	/////////////移除对象，并不销毁对象
	void DeleteObserver(CObserver<TYPE> *pDelObserver);
	
	/////////通知对象
	void NotifyObserver(TYPE dataInfo);

	/////////移除全部的观察者
	void ClearAllOberver();

private:
	//BOOL  m_bChanged;

	//////////////通知对象的列表
    CLock   m_obsLock;
	std::list<CObserver<TYPE> *>   m_ObserverList;
};


template<class TYPE>
CObservable<TYPE>::CObservable(void)
{
}

template<class TYPE>
CObservable<TYPE>::~CObservable(void)
{
}

template<class TYPE>
void CObservable<TYPE>::ClearAllOberver()
{
    CMySingleLock lock(m_obsLock);
	m_ObserverList.clear();
}


template<class TYPE>
void CObservable<TYPE>::AddObserver(CObserver<TYPE> *pServer)
{
	if(NULL == pServer)
	{
		return ;
	}

	/////////防止重复加入的对象被重复通知
	DeleteObserver(pServer);	
    {
        CMySingleLock lock(m_obsLock);
        m_ObserverList.push_back(pServer);
    }
}

template<class TYPE>
void CObservable<TYPE>::NotifyObserver(TYPE dataInfo)
{
    CMySingleLock lock(m_obsLock);
	typename std::list<CObserver<TYPE> *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CObserver<TYPE> *pObserver =*iter;
		iter++;
		pObserver->Update(dataInfo, this);
	}
}

//void CObservable::SetChanged()
//{
//
//}

template<class TYPE>
void CObservable<TYPE>::DeleteObserver(CObserver<TYPE> *pDelObserver)
{
    CMySingleLock lock(m_obsLock);
	typename std::list<CObserver<TYPE> *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CObserver<TYPE> *pObserver = *iter;
		if(pObserver == pDelObserver)
		{
			m_ObserverList.erase(iter);
			break;
		}		
		iter++;
	}		
}


//////////声明类原形
template <class T> class CSyncObserver;

template<class TYPE>
class CSyncObservable
{
public:
	CSyncObservable(void);
public:
	virtual ~CSyncObservable(void);

	//void SetChanged();
	/////////////加入需要观察的对象
	bool AddObserver(CSyncObserver<TYPE> *pServer);

	/////////////移除对象，并不销毁对象
	void DeleteObserver(CSyncObserver<TYPE> *pDelObserver);
	
	/////////通知对象
	void NotifyObserver(TYPE dataInfo);

	/////////移除全部的观察者
	void ClearAllOberver();

private:
	//BOOL  m_bChanged;

	//////////////通知对象的列表
	std::list<CSyncObserver<TYPE> *>   m_ObserverList;
	CLock									  m_ObserverListLock;
};


template<class TYPE>
CSyncObservable<TYPE>::CSyncObservable(void)
{
}

template<class TYPE>
CSyncObservable<TYPE>::~CSyncObservable(void)
{
	ClearAllOberver();		
}

template<class TYPE>
void CSyncObservable<TYPE>::ClearAllOberver()
{
	//没必要也不能加同步锁，否则会出现死锁
	while(!m_ObserverList.empty())
	{
		CSyncObserver<TYPE> *pObServer=m_ObserverList.front();
		m_ObserverList.pop_front();
		pObServer->DelObservable(this);
	}
}


template<class TYPE>
bool CSyncObservable<TYPE>::AddObserver(CSyncObserver<TYPE> *pServer)
{
	if(NULL == pServer)
	{
		return false;
	}
	pServer->AddObservable(this);

	m_ObserverListLock.Lock();
	/////////防止重复加入的对象被重复通知
	//DeleteObserver(pServer);	//和pServer->AddObservable(this)矛盾，所以去掉

	// 防止重复添加对象到观察者
    typename std::list<CSyncObserver<TYPE> *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end()){
		if(*iter == pServer){
			m_ObserverListLock.Unlock();
			return true;
		}
		++iter;
	}

	m_ObserverList.push_back(pServer);
	m_ObserverListLock.Unlock();
	return true;
}

template<class TYPE>
void CSyncObservable<TYPE>::NotifyObserver(TYPE dataInfo)
{
	m_ObserverListLock.Lock();
	typename std::list<CSyncObserver<TYPE> *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CSyncObserver<TYPE> *pObserver = *iter;
		iter++;
		pObserver->Update(dataInfo, this);
	}
	m_ObserverListLock.Unlock();
}

template<class TYPE>
void CSyncObservable<TYPE>::DeleteObserver(CSyncObserver<TYPE> *pDelObserver)
{
	pDelObserver->DelObservable(this);
	m_ObserverListLock.Lock();
	typename std::list<CSyncObserver<TYPE> *>::iterator iter = m_ObserverList.begin();
	while (iter != m_ObserverList.end())
	{
		CSyncObserver<TYPE> *pObserver =*iter;
		if(pObserver == pDelObserver)
		{
			m_ObserverList.erase(iter);
			break;
		}		
		iter++;
	}		
	m_ObserverListLock.Unlock();
}

#endif 
