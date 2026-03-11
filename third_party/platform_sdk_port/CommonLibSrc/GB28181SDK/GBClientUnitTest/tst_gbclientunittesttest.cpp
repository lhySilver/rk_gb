#include <QString>
#include <QtTest>
#include "GB28181ClientSDK.h"
#include <unistd.h>
#include <iostream>


class GBClientUnitTestTest;
GB28181ClientSDK*  g_client  = NULL;
StreamHandle g_handle = NULL;
ResponseHandle  g_catalog = NULL;
ResponseHandle  g_record  = NULL;

class ReceiverImpl : public GBClientReceiver
{
public:

    virtual bool OnStreamRequest(StreamHandle handle, const char* gb_code, StreamRequestType type, const MediaInfo* input  )
    {

        if(  type == kLiveStream) {
            printf("\nstream handle = %d",handle);
            g_handle = handle;
            return true;
        }

           if(  type == kStopStream) {

                  printf("\nStopRequest\n");
              return true;
           }

    }

     virtual bool OnPlayControl(StreamHandle handle , const PlayCtrlCmd* cmd)
    {
         printf("\nplayback....................\n");
        return true;
    }

     virtual bool OnQuery( ResponseHandle  handle, const QueryParam* param )
    {
        if(  param->type == kDeviceCatalogQuery){
            printf("\ncatalog query\n");
            g_catalog = handle;
        }

        if(  param->type == kDeviceRecordQuery){
            printf("\nrecord query\n");
            RecordIndex index;
            memset(&index,0,sizeof(index));
            index.Num = 3;
            std::string code =  "44030500042030900002";
            index.record_list = (RecordParam*)malloc(sizeof(RecordParam)*3);
            memcpy( index.GBCode,  code.c_str(), code.length());
            int i=0;
            for(;i< 3;i++) {
                memcpy( index.record_list[i].DeviceID,  code.c_str(), code.length());
            }

            g_client->ResponseRecordIndex(handle, &index);

        }

       return true;
    }


public:
    GBClientUnitTestTest* test;
};

class GBClientUnitTestTest :  public QObject
{
    Q_OBJECT

public:
    GBClientUnitTestTest();
    GB28181ClientSDK  client;
    ReceiverImpl  receiver;
private Q_SLOTS:
    void testCaseStart();
    void testCaseForRegister();
    void testCaseForReponseMedia();
    void testCaseForHeartbeat();
    void testCaseForCatalog();

};




GBClientUnitTestTest::GBClientUnitTestTest()
{

}

void GBClientUnitTestTest::testCaseStart()
{
    GB28181Version version  = kGB2016Version;
    TransPorco proco = kSipProcoOverUDP;
    const char* localGbCode = "44030500042030900002";
    const char* ip="192.168.52.187";
    uint16_t port = 5601;
    g_client = &client;
    int res = client.Start(version,  proco ,  localGbCode, ip ,  port);
    QVERIFY2( res==true, "Failure");
}

void GBClientUnitTestTest::testCaseForRegister()
{

    GBRegistParam  gb_param;
    ConnectParam   param;
    memset(&gb_param,0,sizeof(gb_param));
    memset(&param,0,sizeof(param));
    gb_param.expires = 3600;
    std::string code =  "44030500042030900002";
    std::string server_code = "44030500042030900003";
    std::string ip =  "192.168.52.187";
    memcpy( gb_param.password,  code.c_str(), code.length());
    memcpy(  gb_param.username,  code.c_str(), code.length());
    memcpy(  param.GBCode ,  server_code.c_str(), server_code.length());
    memcpy( param.ip,  ip.c_str(), ip.length());
    param.port = 5607;
    client.SetReceiver( &receiver  );
    receiver.test = this;
    int res = client.Register( &gb_param , &param  );
    QVERIFY2( res=true, "Failure");

}

void GBClientUnitTestTest::testCaseForReponseMedia()
{
    while(g_handle== NULL){
        usleep(1000);
    }
    printf("StreamRequest\n");

    MediaInfo info;
    memset(&info,0,sizeof(info));
    info.RtpType = kRtpOverTcp;
    info.RtpDescri.type = kVideo;
    std::string code =  "44030500042030900012";
    info.EndTime = 232223;
    info.Port =1233;
    info.RequestType = kLiveStream;
    memcpy( info.IP , "192.168.52.134",IP_LEN);
    memcpy( info.DeviceID, code.c_str(),GB_ID_LEN);
    int res = client.ResponseMediaPlayInfo(g_handle,&info );
        QVERIFY2( res= true, "Failure");


}


 void GBClientUnitTestTest::testCaseForCatalog()
 {
     std::string device_id = "44030500042030900003";
     while (g_catalog== NULL) {
             usleep(100);
     }
     printf("reponse catalog\n");
     getchar();
     DeviceCatalogList list;
     std::string code = "44030500042030900003";
     memcpy( list.GBCode, code.c_str(),code.size());
     list.sum = 20;
     list.catalog =  (CatalogInfo*)malloc(sizeof(CatalogInfo)*20);
     int i = 0;
     for(;i<20;i++) {
         memcpy(list.catalog[i].DeviceID,  device_id.c_str(), device_id.size());
         memcpy(list.catalog[i].CivilCode,  device_id.c_str(), device_id.size());
         memcpy(list.catalog[i].ParentID,  device_id.c_str(), device_id.size());
         list.catalog[i].Manufacturer[0] ='1';
     }
     int res = client.ResponseDeviceCatalog(g_catalog,&list);
     QVERIFY2( res= true, "Failure");
 }

void GBClientUnitTestTest::testCaseForHeartbeat()
{
    printf("heartbeat \n");
    getchar();
    std::string GBCode = "44030500042030900003";
    KeepaliveInfo list;
    list.Num  = 3;
    list.fault_device = (FaultDevice*)malloc(  sizeof(FaultDevice) * 3);
    memcpy( list.fault_device[0].DeviceID, GBCode.c_str(),GB_ID_LEN);
    memcpy( list.fault_device[1].DeviceID, GBCode.c_str(),GB_ID_LEN );
    memcpy( list.fault_device[2].DeviceID,GBCode.c_str(),GB_ID_LEN );
    std::cout<<"Keepalive"<<std::endl;
    getchar();
    int res = client.SendHeartbeat( &list );
    QVERIFY2( res= true, "Failure");
}

QTEST_APPLESS_MAIN(GBClientUnitTestTest)

#include "tst_gbclientunittesttest.moc"
