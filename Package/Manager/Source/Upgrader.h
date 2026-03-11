#ifndef __INTER_MANAGER_UPGRADER_H__
#define __INTER_MANAGER_UPGRADER_H__

#include "Infra/Singleton.h"
#include "Infra/Timer.h"
#include "Infra/Packet.h"
#include "Infra/Mutex.h"
#include "Manager/Upgrader.h"
#include <Zlib/zziplib.h>
#include <string>
#include <deque>

///////////////////////////////////////////////////////////////////////////////
////////////////// Image Header Definition
#define	IH_MAGIC	0x27051956	/* Image Magic Number		*/
#define	IH_NMLEN		32	/* Image Name Length		*/
#define IH_OS_LINUX		5	/* Linux	*/
#define IH_CPU_ARM		2	/* ARM		*/

/*
* all data in network byte order (aka natural aka bigendian)
*/
struct ImageHeader
{
	uint	ih_magic;	/* Image Header Magic Number	*/
	uint	ih_hcrc;	/* Image Header CRC Checksum	*/
	uint	ih_time;	/* Image Creation Timestamp	*/
	uint	ih_size;	/* Image Data Size		*/
	uint	ih_load;	/* Data  Load  Address		*/
	uint	ih_ep;		/* Entry Point Address		*/
	uint	ih_dcrc;	/* Image Data CRC Checksum	*/
	uchar	ih_os;		/* Operating System		*/
	uchar	ih_arch;	/* CPU architecture		*/
	uchar	ih_type;	/* Image Type			*/
	uchar	ih_comp;	/* Compression Type		*/
	uchar	ih_name[IH_NMLEN];	/* Image Name		*/
} ;

///////////////////////////////////////////////////////////////////////////////
////////////////// plugin of zzip for memory block vector access
/// 内存快结构
struct MemBlock
{
	uchar *base;	// 内存块地址
	uint size;		// 内存块大小
	uint offset;	// 内存块在内存块序列中的偏移，mbv_open时自动生成
	MemBlock* next; // 下一个内存块指针，0表示结束
};

struct UChar
{
	char* Buffer;
	int len;
};

/// 内存块数组文件描述符
struct MemBlockVectorHandle
{
	struct MemBlock* start;
	struct MemBlock* current;
	zzip_off_t pos;
	zzip_off_t size;
};

/// \brief 升级管理器
/// 
class CUpgrader : public IUpgrader
{
	PATTERN_SINGLETON_DECLARE(CUpgrader);

public:
	enum 
	{
		perPacketSize = 32*1024,	///< 升级时每个packet数据包的大小
	};

	/// 升级初始化
	bool init();

	/// 追加升级数据和setFilePath只能使用其中一个
	bool putBuffer(uchar *buf, int len);

	/// 开始应用升级数据
	bool start(bool sync = false);

	/// 中止升级
	bool abort();

	/// 当前是否已经开始升级
	bool started() const;

private:

	void onUpgrade(uint arg);

	void cleanup();

	bool updatePart(ZZIP_DIR *dir, const char* filename);

	int getProgress();

	int onConsole(int argc, char **argv);
	
	/// 开始应用升级数据
	bool start(bool sync, bool force);

	bool setFilePath(const char* path);
private:
	Json::Value m_upgradeInfo;
	CMutex m_mutexUpgrader;
	CTimer m_upgrader;
	std::deque<UChar > m_buffer;		// 升级缓冲数据
	std::string m_filePath;
	int m_totalSize;
	int m_burnedSize;
	bool	m_started;
	UChar m_Buffer[400];
};

#endif
