#include "ResponseWaiter.h"
#include "GB28181Defs.h"

CGbResponseWaiter::CGbResponseWaiter(std::string cmd,std::string identifiler)
                        :m_identifiler(identifiler),m_cmd(cmd)
{
     m_currentNum = 0;
     m_buffer = NULL;
     m_recv_flag.SetValue(0);
     m_reference_count.SetValue(0);
}

void FreeDeviceStatus(DeviceStatus* src)
{
      if(  !src ){
          return;
      }
      if( src->Alarm_Status){
          free(src->Alarm_Status);
      }
      free(src);
}

void FreeDeviceRecord(RecordIndex* src)
{
    if(  !src ){
        return;
    }

    if( src->record_list){
        free(src->record_list);
    }

    free(src);
}

void FreeDeviceCatalog(DeviceCatalogList* src)
{
    if(  !src ){
        return;
    }
    if( src->catalog){
        free(src->catalog);
    }
    free(src);
}


CGbResponseWaiter::~CGbResponseWaiter()
{
    if( m_cmd == "DeviceStatus")  {
           FreeDeviceStatus((DeviceStatus*)m_buffer);
    }

    if( m_cmd == "RecordInfo")  {
           FreeDeviceRecord((RecordIndex*)m_buffer);
    }

    if( m_cmd == "Catalog")  {
           FreeDeviceCatalog((DeviceCatalogList*)m_buffer);
    }
}

void CGbResponseWaiter::AddRef()
{
    m_reference_count.Increment();
}

void CGbResponseWaiter::UnRef()
{
     if(m_reference_count == 0 ){
         delete this;
     }
     m_reference_count .Decrement();
}


void CopyDevicePreset(PresetInfo* src,  PresetInfo* dest)
{
    memcpy(dest,  src, sizeof(*src) );
    dest->PresetList = (PresetParam *)malloc(  sizeof(PresetParam)* src->PersetListNum  );
    memcpy(dest->PresetList,  src->PresetList, sizeof(PresetParam)* src->PersetListNum );
}



void CopyDeviceInfo(DeviceInfo* src,  DeviceInfo* dest)
{
    memcpy(dest,  src, sizeof(*src) );
}


 void CopyDeviceStatus( DeviceStatus* src,  DeviceStatus* dest )
{
    memcpy(dest,  src, sizeof(*src) );
    dest->Alarm_Status = (AlarmStatus*)malloc(  sizeof(AlarmStatus)* src->AlarmNum  );
    memcpy(dest->Alarm_Status,  src->Alarm_Status, sizeof(sizeof(AlarmStatus)* src->AlarmNum) );
}

 void CopyDeviceCatalog( DeviceCatalogList* src,  DeviceCatalogList* dest )
{
    memcpy(dest,  src, sizeof(*src) );
	if (src->sum == 0) {
		return;
	}
    dest->catalog = (CatalogInfo*)malloc(  sizeof(CatalogInfo)* src->sum  );
    memcpy( dest->catalog,  src->catalog, (sizeof(CatalogInfo)* src->sum) );
}

 void CopyDeviceRecord( RecordIndex* src,  RecordIndex* dest )
{
    memcpy(dest,  src, sizeof(*src) );
	if (src->Num == 0) {
		return;
	}
    dest->record_list = (RecordParam*)malloc(  sizeof(RecordParam)* src->Num  );
    memcpy(dest->record_list,  src->record_list, (sizeof(RecordParam)* src->Num) );
}


void CGbResponseWaiter::OnDeviceStatus(void* msg ,  uint32_t sum)
{
    if( !m_buffer)  {
           DeviceStatus* status = (DeviceStatus*)msg;
           DeviceStatus* temp = (DeviceStatus*)malloc(sizeof(DeviceStatus));
           CopyDeviceStatus(temp ,status);
           m_buffer = temp;
    }
}

bool CGbResponseWaiter::OnDeviceCatalog(void* msg ,  uint32_t sum)
{
	
	DeviceCatalogList*  src_catalog = (DeviceCatalogList*)msg;
	DeviceCatalogList* catalog_buffer = NULL;

     if(sum == 0){
		if (!m_buffer) {
			 catalog_buffer = (DeviceCatalogList*)malloc(sizeof(DeviceCatalogList));
			 memcpy(catalog_buffer, src_catalog, sizeof(DeviceCatalogList));
			 catalog_buffer->catalog = NULL;;
			 catalog_buffer->sum = sum;
			 m_buffer = catalog_buffer;
		}
        return true;
     }

     if( m_currentNum >= sum) {
         return true;
     }

   
    if( !m_buffer)  {
          catalog_buffer  = (DeviceCatalogList*)malloc(sizeof(DeviceCatalogList));
          memcpy(catalog_buffer, src_catalog,  sizeof(DeviceCatalogList));
          catalog_buffer->catalog = (CatalogInfo*)malloc(sizeof(CatalogInfo)*sum);
          memset(catalog_buffer->catalog,0, (sizeof(CatalogInfo)*sum));
          catalog_buffer->sum = sum;
          m_buffer =  catalog_buffer;
    }
    catalog_buffer = (DeviceCatalogList*)m_buffer;
    int  i = m_currentNum;
    int  j = 0;
    int  add = m_currentNum + src_catalog->sum;

    if( add > (int)sum ) {
        add = sum;
    }

    for( ;i < add ;i++)  {
        CatalogInfo* dest = &(catalog_buffer->catalog[i]);
        CatalogInfo* src = &(src_catalog->catalog[j]);
        memcpy(dest , src ,  sizeof(CatalogInfo));
        j++;
    }

    if( add == (int)sum) {
        return true;
    }
        m_currentNum = add;
        return  false;
}

bool CGbResponseWaiter::OnDeviceRecord(void* msg ,  uint32_t sum)
{
	RecordIndex*  src_index = (RecordIndex*)msg;
	RecordIndex*  index_buffer = NULL;

    if(sum == 0) {
		if (!m_buffer) {
			index_buffer = (RecordIndex*)malloc(sizeof(RecordIndex));
			memcpy(index_buffer, src_index, sizeof(RecordIndex));
			index_buffer->record_list = NULL;
			index_buffer->Num = 0;
			m_buffer = index_buffer;
		}
        return true;
    }

    if( m_currentNum >= sum) {
        return true;
    }

   if( !m_buffer)  {
         index_buffer  = (RecordIndex*)malloc(sizeof(RecordIndex));
         memcpy(index_buffer, src_index,  sizeof(RecordIndex));
         index_buffer->record_list = (RecordParam  *)malloc(sizeof(RecordParam)*sum);
         memset(index_buffer->record_list,0, (sizeof(RecordParam)*sum));
         index_buffer->Num = sum;
         m_buffer =  index_buffer;
   }

   index_buffer = (RecordIndex* )m_buffer;
   int  i = m_currentNum;
   int  j = 0;
   int  add = m_currentNum + src_index->Num;

   if( add > (int)sum ) {
       add = sum;
   }

   for( ;i < add ;i++)  {
       memcpy( &(index_buffer->record_list[i] ),  &(src_index->record_list[j]),  sizeof(RecordParam));
       j++;
   }

   if( add == (int)sum) {
       return true;
   }
       m_currentNum = add;
       return  false;
}

void CGbResponseWaiter::OnDeviceInfo(void* msg ,  uint32_t sum)
{
    if( !m_buffer)  {
           DeviceInfo* status = (DeviceInfo*)msg;
           DeviceInfo* temp = (DeviceInfo*)malloc(sizeof(DeviceInfo));
           memcpy( temp, status, sizeof(DeviceInfo));
           m_buffer = temp;
     }
}



void CGbResponseWaiter::OnDevicePreset(void* msg ,  uint32_t sum)
{
    if( !m_buffer)  {
           PresetInfo* status = (PresetInfo*)msg;
           PresetInfo* temp = (PresetInfo*)malloc(sizeof(PresetInfo));
           memset(temp,0, sizeof(PresetInfo));
           CopyDevicePreset(status,temp);
           m_buffer = temp;
     }
}

void CGbResponseWaiter::Wakeup( void* msg ,  uint32_t sum )
{

        if( m_cmd == "DeviceStatus")  {
               OnDeviceStatus(msg ,   sum);
        }

        if( m_cmd == "DeviceInfo")  {
               OnDeviceInfo(msg ,   sum);
        }

        if( m_cmd == "PresetQuery")  {
               OnDevicePreset(msg ,   sum);
        }


        if( m_cmd == "Catalog")  {
            if(!OnDeviceCatalog(msg , sum)) {
                return;
            }
        }

        if( m_cmd == "RecordInfo")  {
            if(!OnDeviceRecord(msg , sum)){
                return;
            }
        }
        m_recv_flag.SetValue(1);
        m_wait_event.SetEvent();
}

bool CGbResponseWaiter::WaitTimeout(void *result, int timeout )
{
        m_wait_event.WaitEvent(timeout);
        if(  m_recv_flag   ) {
            if(m_cmd == "DeviceStatus") {
                CopyDeviceStatus((DeviceStatus*)m_buffer, (DeviceStatus*)result);
            }

            if(m_cmd == "Catalog") {
                CopyDeviceCatalog((DeviceCatalogList*)m_buffer, (DeviceCatalogList*)result);
            }

            if(m_cmd == "RecordInfo") {
                CopyDeviceRecord((RecordIndex*)m_buffer, (RecordIndex*)result);
            }

            if(m_cmd == "PresetQuery") {
                CopyDevicePreset((PresetInfo*)m_buffer, (PresetInfo*)result);
            }

            if(m_cmd == "DeviceInfo") {
                CopyDeviceInfo((DeviceInfo*)m_buffer, (DeviceInfo*)result);
            }

            return true;
        }
            return false;
}

