#include "StreamRequestManager.h"
#include "SipDefs.h"

CStreamRequestManager::CStreamRequestManager()
{

}

void CStreamRequestManager::InsertStreamHandle(StreamHandle handle)
{
      m_lock.Lock();
      m_set_handle.insert(handle);
      m_lock.Unlock();
}

bool CStreamRequestManager::DeleteStreamHandle(StreamHandle handle)
{
    bool flag = false;
    m_lock.Lock();
    std::set<StreamHandle>::iterator iter = m_set_handle.find(handle);
    if( iter != m_set_handle.end() ){
         m_set_handle.erase(handle);
         flag = true;
    }
    m_lock.Unlock();
         return flag;
}

bool CStreamRequestManager::FindStreamHandle(StreamHandle handle)
{
    bool flag = false;
    m_lock.Lock();
    std::set<StreamHandle>::iterator iter = m_set_handle.find(handle);
    if( iter != m_set_handle.end() ){
         flag = true;
    }
    m_lock.Unlock();
        return flag;
}

StreamHandle CStreamRequestManager::DeleteStreamHandle(int call_id , int dialog_id)
{
    StreamHandle handle = NULL;
    m_lock.Lock();
    std::set<StreamHandle>::iterator iter = m_set_handle.begin();
    while( iter != m_set_handle.end() ) {

         SipDialogKey* key = ( SipDialogKey*)(*iter);
         if(  key->CallId == call_id && key->DialogId == dialog_id  ){
             handle = (StreamHandle)*iter;
			 m_set_handle.erase(iter);
			 free(key);
             break;
         }
              iter++;
    }
    m_lock.Unlock();
         return handle;
}

StreamHandle CStreamRequestManager::FindStreamHandle(int call_id , int dialog_id)
{
    StreamHandle handle = NULL;
    m_lock.Lock();
    std::set<StreamHandle>::iterator iter = m_set_handle.begin();
    while( iter != m_set_handle.end() ) {

         SipDialogKey* key = (SipDialogKey*)(*iter);
         if(  key->CallId == call_id && key->DialogId == dialog_id  ){
             handle = *iter;
             break;
         }
              iter++;
    }
    m_lock.Unlock();
         return handle;
}


