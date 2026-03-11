
#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <list>
#include "Lock.h"
//不带同步功能的观察者
//////////声明类原形
template <class T> class CObservable;

template <class TYPE_1>
class CObserver
{
public:
	CObserver(void);
public:
	virtual ~CObserver(void);


	virtual void Update(TYPE_1 DataInfo, CObservable<TYPE_1> *pObservable = NULL);
};

template <class TYPE_1>
CObserver<TYPE_1>::CObserver(void)
{
}

template <class TYPE_1>
CObserver<TYPE_1>::~CObserver(void)
{
}

template <class TYPE_1>
void CObserver<TYPE_1>::Update(TYPE_1 DataInfo, CObservable<TYPE_1> *pObservable )
{

}

//带有同步功能的观察者
//////////声明类原形
template <class T> class CSyncObservable;

template <class TYPE_1>
class CSyncObserver
{
public:
	CSyncObserver(void);
public:
	virtual ~CSyncObserver(void);


	virtual void Update(TYPE_1 DataInfo, CSyncObservable<TYPE_1> *pObservable = NULL);
protected:
	friend class CSyncObservable<TYPE_1>;
	virtual void AddObservable(CSyncObservable<TYPE_1> *pObServable);
	virtual void DelObservable(CSyncObservable<TYPE_1> *pDelObserver);
private:
	std::list<CSyncObservable<TYPE_1> *>   m_ObservableList;
	CLock									  m_ObservableListLock;
};

template <class TYPE_1>
CSyncObserver<TYPE_1>::CSyncObserver(void)
{
}

template <class TYPE_1>
CSyncObserver<TYPE_1>::~CSyncObserver(void)
{
	//在自己析构时将自己从被观察者中移除,没必要也不能加同步锁，否则会出现死锁
	while(!m_ObservableList.empty())
	{
		CSyncObservable<TYPE_1> *pObServable=m_ObservableList.front();
		m_ObservableList.pop_front();
		pObServable->DeleteObserver(this);
	}
}

template <class TYPE_1>
void CSyncObserver<TYPE_1>::Update(TYPE_1 DataInfo, CSyncObservable<TYPE_1> *pObservable )
{

}

template <class TYPE_1>
void CSyncObserver<TYPE_1>::AddObservable(CSyncObservable<TYPE_1> *pObServable)
{
	m_ObservableListLock.Lock();
    typename std::list< CSyncObservable< TYPE_1 > * >::iterator iter = m_ObservableList.begin();
    while ( iter != m_ObservableList.end() )
    {
        if(*iter == pObServable)
        {
			m_ObservableListLock.Unlock();
			return;
		}		
		iter++;
	}

	m_ObservableList.push_back(pObServable);
	m_ObservableListLock.Unlock();
}

template <class TYPE_1>
void CSyncObserver<TYPE_1>::DelObservable(CSyncObservable<TYPE_1> *pDelObserver)
{
	m_ObservableListLock.Lock();
	typename std::list<CSyncObservable<TYPE_1> *>::iterator iter = m_ObservableList.begin();
	while (iter != m_ObservableList.end())
	{
		CSyncObservable<TYPE_1> *pObservable =*iter;
		if(pObservable == pDelObserver)
		{
			m_ObservableList.erase(iter);
			break;
		}		
		iter++;
	}		
	m_ObservableListLock.Unlock();
}

#endif
