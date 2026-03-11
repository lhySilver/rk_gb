#include "Infra/Referable.h"
#include "CAS.h"

//#pragma warning(disable 4311);

IReferable::IReferable()
{
	CAtomicCount *pAtomic  = new CAtomicCount;
	m_handle = reinterpret_cast<void*>(pAtomic);
}

IReferable::~IReferable()
{
	CAtomicCount *pAtomic = (CAtomicCount*)m_handle;

	delete pAtomic;
}

void IReferable::AddRef()
{
	CAtomicCount *pAtomic = (CAtomicCount*)m_handle;

	pAtomic->AddRef();
}

void IReferable::Release()
{
	CAtomicCount *pAtomic = (CAtomicCount*)m_handle;

	if (pAtomic->Release())
	{ 
		delete this;
	}
}

/*/////////////////////////////////////////////////////////////////////
CReferablePtr
use this class to use class IReferable, simlar as std::auto_ptr
you can use the class as a pointer of class IReferable usually.
CBaseReferablePtr is used to be a friend class of IReferable

though there are class derieved from CReferablePtr, the methods
needn't be virtual. because the derieved class is a simple wrapper
/////////////////////////////////////////////////////////////////////*/


CBaseReferablePtr::CBaseReferablePtr( IReferable* RefPtr) : mRefPtr( RefPtr )
{
	if( mRefPtr ) mRefPtr->AddRef();
}

//auto matically release the pointer
//care that it's not virtual, so the derived class shouldn't have destructor
CBaseReferablePtr::~CBaseReferablePtr()
{
	if( mRefPtr ) mRefPtr->Release();
}

CBaseReferablePtr::CBaseReferablePtr( const CBaseReferablePtr& RefPtr )
: mRefPtr( RefPtr.mRefPtr )
{
	if( mRefPtr ) mRefPtr->AddRef();
}

CBaseReferablePtr& CBaseReferablePtr::operator =( IReferable* RefPtr )
{
	if(mRefPtr!=RefPtr){
		if(mRefPtr) mRefPtr->Release();
		if((mRefPtr=RefPtr)!=0) mRefPtr->AddRef();
	}
	return *this;
}

CBaseReferablePtr& CBaseReferablePtr::operator=( const CBaseReferablePtr& RefPtr )
{
	return operator=(RefPtr.mRefPtr);
}

bool CBaseReferablePtr::IsEmpty() const
{
	return mRefPtr == 0;
}
