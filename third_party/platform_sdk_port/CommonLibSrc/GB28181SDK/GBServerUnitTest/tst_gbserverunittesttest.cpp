#include <QString>
#include <QtTest>
#include "GB28181ServerSDK.h"
#include <unistd.h>
#include <iostream>

SessionHandle g_session_handle = NULL;

class ReceiverImpl : public GBServerReceiver
{

   virtual bool OnRegist(SessionHandle handle, const GBRegistParam* param, const ConnectParam* connect )
    {

          printf("GBcode : %s\n", connect->GBCode  );
          printf("ip : %s\n", connect->ip  );
          printf("port : %d\n", connect->port  );

          printf("password : %s\n", param->password  );
          printf("expires : %d\n", param->expires  );
          printf("username : %s\n", param->username  );
          g_session_handle = handle;
          return true;
    }

    virtual bool OnQueryResponse( QueryType type, const char* gb_code, void* msg )
    {
         printf("query response");
        printf("type : %d\n", type  );
        printf("code : %s\n", gb_code  );
        DeviceCatalogList*list = (DeviceCatalogList*)msg;
        int i = 0;
         printf("sum : %d\n", list->sum  );

        for(;i< list->sum; i++ ) {

            printf("deviceid : %s\n", list->catalog[i].DeviceID  );
            printf("civicode : %s\n", list->catalog[i].CivilCode  );
            printf("parant id : %s\n", list->catalog[i].ParentID  );
            printf("\n");
        }

    }


     virtual bool OnNotify(NotifyType type,  const char* gb_code,  void* info  )
    {
        printf("type : %d\n", type  );
        printf("code : %s\n", gb_code  );
        KeepaliveInfo* keep = (KeepaliveInfo*)info;
        printf("error code : %s\n",keep->fault_device[0].DeviceID);
        return true;
    }
};


class GBServerUnitTestTest : public QObject
{
    Q_OBJECT

public:
    GBServerUnitTestTest();
     GB28181ServerSDK   server;
     ReceiverImpl     receiver;
private Q_SLOTS:
    void testCaseForStart();
    void  testCaseForStartAndStopStream();
    void testCaseForRecordQuery();
    void  testCaseForQuery();
};

GBServerUnitTestTest::GBServerUnitTestTest()
{
}

 void GBServerUnitTestTest::testCaseForRecordQuery()
 {
     std::cout<<"record query"<<std::endl;
     getchar();
     RecordParam param;
     memset(&param,0,sizeof(param));
     std::string code = "44030500042030900002";
     std::string Time = "2019:12.23.1";
     memcpy(param.DeviceID, code.c_str(), code.size());
     memcpy(param.EndTime, Time.c_str(), Time.size());
     memcpy(param.StartTime, Time.c_str(), Time.size());
     RecordIndex* result= NULL;
    int res =  server.QueryDeviceRecordIndex(g_session_handle, &param, 10000, &result   );
    if(result != NULL) {
         printf("record success");
    }
    QVERIFY2(res == true, "Failure");

 }


void GBServerUnitTestTest::testCaseForQuery()
{
    while(g_session_handle == NULL){
        usleep(10000);
    }
    std::cout<<"catalog query"<<std::endl;
    getchar();
    CatalogQueryParam param;
    memset(&param,0,sizeof(param));
    std::string code = "44030500042030900003";
    std::string Time = "2019:12.23.1";
    memcpy(param.DeviceID, code.c_str(), code.size());
    memcpy(param.time.EndTime, Time.c_str(), Time.size());
    memcpy(param.time.StartTime, Time.c_str(), Time.size());
    int res = server.QueryCatalog( g_session_handle,&param  );
     server.SetGBReceiver(&receiver);
      usleep(100000000000);
    QVERIFY2(res == true, "Failure");
}


void GBServerUnitTestTest::testCaseForStart()
{
    GB28181Version version  = kGB2016Version;
    TransPorco proco = kSipProcoOverUDP;
    const char* localGbCode = "44030500042030900003";
    const char* ip="192.168.52.187";
    uint16_t port = 5607;

    int res = server.Start(version,  proco ,  localGbCode, ip ,  port);
     server.SetGBReceiver(&receiver);
    QVERIFY2(res == true, "Failure");
}

void GBServerUnitTestTest::testCaseForStartAndStopStream()
{
    int res;
    while(g_session_handle == NULL){
        usleep(10000);
    }
    std::cout<<"start stream"<<std::endl;
    getchar();

    MediaInfo* output = (MediaInfo*)malloc(sizeof(MediaInfo));
    memset(output,0,sizeof(MediaInfo));
    StreamHandle  stream_handle;

    MediaInfo info;
    memset(&info,0,sizeof(info));
    info.RtpType = kRtpOverTcp;
    std::string code =  "44030500042030900012";
    info.EndTime = 232223;
    info.Port =1233;
    info.RequestType = kLiveStream;
    info.RtpDescri.type = kVideo;
    info.RtpDescri.DescriNum = 1;
    info.RtpDescri.mapDescri = (RtpMap*)malloc(sizeof(RtpMap));
    info.RtpDescri.mapDescri->MediaFormat = 96;
    memcpy( info.RtpDescri.mapDescri->MimeType,"PS", strlen("PS"));
    info.RtpDescri.mapDescri->SampleRate = 9000;
    memcpy( info.IP , "192.168.52.134",IP_LEN);
    memcpy( info.DeviceID, code.c_str(),GB_ID_LEN);
    res = server.StartLiveStreamRequest( g_session_handle,  &info,  output,  &stream_handle  );
    if(res != true) {
          QVERIFY2(res == true, "Failure");
    }
    std::cout<<"play back"<<std::endl;
    getchar();
    PlayCtrlCmd cmd;
    memset(&cmd,0,sizeof(cmd));
    cmd.Type = kPlayStart;
    memcpy(cmd.GBCode,code.c_str(),code.size());
    res = server.PlaybackControl( g_session_handle,  stream_handle,&cmd  );
    if(res != true) {
          QVERIFY2(res == true, "Failure");
    }
    std::cout<<"stop stream"<<std::endl;
    getchar();
    res = server.StopStreamRequest (g_session_handle, stream_handle  );

    QVERIFY2(res == true, "Failure");

}

QTEST_APPLESS_MAIN(GBServerUnitTestTest)

#include "tst_gbserverunittesttest.moc"
