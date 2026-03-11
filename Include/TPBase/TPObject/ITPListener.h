
#ifndef _ITPLISTENER_H_
#define _ITPLISTENER_H_

//class ITPListener
//{
//public:    
//	virtual void onData(int engineId, int connId, char* pData, int nLen) = 0;
//	virtual void onClose(int engineId, int connId) = 0;
//
//    //返回值为0表示接受此连接，返回值为1表示拒绝接受
//	virtual void onConnect(int engineId, int connId, char* pIp, int nPort) = 0;
//
//	//发送时数据放在队列中,当心跳heartbeat时才实际发送,发送成功时将之前
//	//的ID确认给调用者,用于清除数据区指针.发送直接删除指针会引起错误.
//	//* 注意：onSendDataAck中不允许再调用任何传输层的方法，比如SendData()等，
//	//* 否则会引起死锁和传输层队列错误。
//	virtual void onSendDataAck(int engineId, int connId, char *pData, int nID) = 0;
//};
#endif	//_ITPLISTENER_H_

