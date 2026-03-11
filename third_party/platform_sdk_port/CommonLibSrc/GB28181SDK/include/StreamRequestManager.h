#ifndef CSTREAMREQUESTMANAGER_H
#define CSTREAMREQUESTMANAGER_H
#include "GB28181Defs.h"
#include <set>
#include "Lock.h"


class CStreamRequestManager
{
public:
    CStreamRequestManager();

    void InsertStreamHandle(StreamHandle handle);

    bool DeleteStreamHandle(StreamHandle handle);

    bool FindStreamHandle(StreamHandle handle);

   StreamHandle  DeleteStreamHandle(int call_id , int dialog_id);

   StreamHandle FindStreamHandle(int call_id , int dialog_id);

private:
    CLock                          m_lock;
    std::set<StreamHandle> m_set_handle;
};

#endif // CSTREAMREQUESTMANAGER_H
