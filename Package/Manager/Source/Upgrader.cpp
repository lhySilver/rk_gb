#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Infra/Packet.h"
#include "Infra/File.h"
#include "Manager/Console.h"
#include "Manager/EventManager.h"
#include "MagicBox.h"
#include "Log/Log.h"
#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/ExchangeKind.h"
#include "Zlib/zziplib.h"
#include "PAL/Flash.h"
#include "Upgrader.h"
#include "MagicBox.h"
#include "Log/DebugDef.h"

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX(a,b)				(((a) > (b)) ? (a) : (b))
#define MIN(a,b)				(((a) < (b)) ? (a) : (b))

///////////////////////////////////////////////////////////////////////////////
////////////////// plugin of zzip for memory block vector access
// mbv stand for memory block vector
static const int maxMbvFds = 10;
int count = 0;

struct MemBlockVectorHandle mbvFds[maxMbvFds] = {{0}};

static int mbv_open(zzip_char_t* name, int flags, ...)
{
	int i;
	int fd = -1;
	struct MemBlock* mb;

	// i 不能从0开始，可能是zzip库的bug
	for( i = 1; i < maxMbvFds; i++)
	{
		if(mbvFds[i].start == NULL)
		{
			fd = i;
			break;
		}
	}

	if(i == maxMbvFds)
	{
		return fd;
	}

	struct MemBlockVectorHandle* fp = &mbvFds[fd];

	fp->start = (struct MemBlock*) name;
	fp->current = fp->start;
	fp->pos = 0;
	fp->size = 0;

	mb = fp->start;
	while(mb)
	{
		mb->offset = fp->size;
		fp->size += mb->size;
		mb = mb->next;
	}

	return fd;
}

static int mbv_close(int fd)
{
	assert((fd < maxMbvFds) && (fd >= 0));

	struct MemBlockVectorHandle* fp = &mbvFds[fd];

	fp->start = NULL;

	return 0;
}

static zzip_ssize_t mbv_read(int fd, void* buf, zzip_size_t len)
{
	assert((fd < maxMbvFds) && (fd >= 0));

	struct MemBlockVectorHandle* fp = &mbvFds[fd];
	zzip_size_t remain = (len < (zzip_size_t)(fp->size - fp->pos)
		? len : (zzip_size_t)(fp->size - fp->pos));
	uchar *addr = (uchar *)buf;
	struct MemBlock* mb = fp->current;

	while(mb)
	{
		if(remain == 0)
		{
			break;
		}

		int min = (remain < (mb->offset + mb->size - fp->pos)
			? remain : (mb->offset + mb->size - fp->pos));

		memcpy(addr, mb->base + fp->pos - mb->offset, min);

		if(remain >= mb->offset + mb->size - fp->pos)
		{
			mb = mb->next;
		}

		remain -= min;
		addr += min;
		fp->pos += min;
	}

	fp->current = mb;

	return (len - remain);
}

static zzip_off_t mbv_seek(int fd, zzip_off_t offset, int whence)
{
	assert((fd < maxMbvFds) && (fd >= 0));

	struct MemBlockVectorHandle* fp = &mbvFds[fd];

	/* Change position pointer */
	switch (whence) {
	case SEEK_SET:
		fp->pos = offset;
		break;
	case SEEK_CUR:
		fp->pos += offset;
		break;
	case SEEK_END:
	fp->pos = fp->size;
	break;
	}

	/* Limit position to the last byte of the file */
	if (fp->pos > fp->size) {
		fp->pos = fp->size;
	}

	struct MemBlock* mb = fp->start;
	while(mb)
	{
		if(fp->pos < (zzip_off_t)(mb->offset + mb->size))
		{
			fp->current = mb;
			break;
		};
		mb = mb->next;
	}

	/* Return current position */ 
	return(fp->pos);	
}

static zzip_off_t mbv_filesize(int fd)
{
	assert((fd < maxMbvFds) && (fd >= 0));

	struct MemBlockVectorHandle* fp = &mbvFds[fd];

	return fp->size;
}

static struct zzip_plugin_io mbv_io = 
{
	mbv_open,
	mbv_close,
	mbv_read,
	mbv_seek,
	mbv_filesize,
	0
};

//////////////////////////////////////////////////////////////////////////
/// CUpgrade
/// 升级文件名的头64个字节内容是大端的，需要转换成小端
#define swap_long(x) (((((ulong)(x)) & 0xff000000) >> 24) | \
	((((ulong)(x)) & 0x00ff0000) >> 8) | \
	((((ulong)(x)) & 0x0000ff00) << 8) | \
	((((ulong)(x)) & 0x000000ff) << 24)\
	)

PATTERN_SINGLETON_IMPLEMENT(CUpgrader);

CUpgrader::CUpgrader() : m_mutexUpgrader(MUTEX_RECURSIVE), m_upgrader("Upgrader") 
{
	m_filePath = "";
	m_totalSize = 0;
	m_burnedSize = 0;
	m_started = false;
}

CUpgrader::~CUpgrader()
{
}

bool CUpgrader::init()
{
	// 有可能出现网络超时导致升级失败，这时下次在升级时需要释放上次的packet
	while (!m_buffer.empty())
	{
		UChar packet = m_buffer.front();
		if (packet.Buffer)
		{
			delete packet.Buffer;
			packet.len = 0;
		}
		m_buffer.pop_front();
	}
    m_filePath.clear();
	return true;
}

bool CUpgrader::putBuffer(uchar *buf, int len)
{
	CGuard guard(m_mutexUpgrader);
	UChar packet;
	if (!m_buffer.empty())
	{
		// 将最后一个packet给填满
		packet = m_buffer.back();
		//printf("------------->len[%d],[%d]\n", len, packet.len);
		while (len > 0 && packet.len < perPacketSize)
		{
			int buflen = MIN((uint)len, (perPacketSize - packet.len));
			memcpy(packet.Buffer+ packet.len, buf, buflen);
			packet.len += buflen;
			len -= buflen;
			buf += buflen;
			m_buffer.pop_back();
			m_buffer.push_back(packet);
		}
	}
	while (len > 0)
	{
		//CPacket *packet = g_PacketManager.GetPacket(perPacketSize);
		m_Buffer[count].Buffer = new char[perPacketSize];
		m_Buffer[count].len = 0;
		int buflen = MIN((uint)len, perPacketSize);
		CTime::sleep(1);
		if (!m_Buffer[count].Buffer)
		{
			return false;
		}
		memcpy(m_Buffer[count].Buffer + m_Buffer[count].len, buf, buflen);
		len -= buflen;
		buf += buflen;
		m_Buffer[count].len += buflen;
		m_buffer.push_back(m_Buffer[count]);
		//printf("=======--->size : [%d],[%d]\n\n", m_buffer.size(),m_Buffer[count].len);
		count++;
	}
	return true;
}

bool CUpgrader::start(bool sync)
{
	if (!m_started)
	{
		m_started = true;
		FlashInit();
		if (sync)
		{
			onUpgrade(0);
		}
		else
		{
			m_upgrader.Start(CTimer::Proc(&CUpgrader::onUpgrade, this), 0, 0);
		}
		return true;
	}
	return false;
}

bool CUpgrader::abort()
{
	cleanup();
	m_upgrader.Stop();
	// 升级异常后系统可能会因为取不到packet而丢失几秒钟的录像(1S左右)
	// 之后便会自动恢复,故记录日志
//<shang>	ILog::instance()->append("Upgrade", "Exception");
	return true;
}

bool CUpgrader::started() const
{
	return m_started;
}

int CUpgrader::getProgress()
{
	return m_burnedSize * 100 / m_totalSize;
}

void CUpgrader::onUpgrade(uint arg/*== force*/)
{
	char buf[1024] = {0};
	CConfigReader reader;
	Json::Value table;
	ZZIP_DIR *dir = NULL;
	ZZIP_FILE *zipfile;
	zzip_ssize_t len;
	int state = stateOK;
	MemBlock* blocks = NULL;
	ZZIP_DIRENT *entry;

	m_totalSize = 0;
	m_burnedSize = 0;
	m_upgradeInfo["FileName"] = "";
	m_upgradeInfo["State"] = "UpgradeStart";
	m_upgradeInfo["Process"] = 0;
	IEventManager::instance()->notify("Upgrade", 0, appEventPulse, NULL, NULL, &m_upgradeInfo);
	// 读取zip目录
	if(m_buffer.size()) // 从内存块升级
	{
		blocks = new MemBlock[m_buffer.size()];
		printf("-------------onUpgrade111111[%d]>> \n\n\n", m_buffer.size());
		for(size_t b = 0; b < m_buffer.size(); b++)
		{
			blocks[b].base = (uchar*)(m_buffer[b].Buffer);
			blocks[b].size =m_buffer[b].len;
			blocks[b].next = (b == m_buffer.size() - 1) ? NULL : (&blocks[b] + 1);
		}
		dir = zzip_opendir_ext_io((const zzip_char_t*)blocks, 0, 0, &mbv_io);
	}
	else // 从文件升级
	{
		dir = zzip_opendir(m_filePath.c_str());
	}

	if(!dir)
	{
		ManagerErr("CUpgrader::upgrade open zip dir failed.\n");
		state = stateInvalidFomat;
		goto end;
	}
	// 打印升级文件列表
	entry = zzip_readdir(dir);
	ManagerInfo("CUpgrader::upgrade file list:\n");
	while(entry)
	{
		m_totalSize += entry->st_size;
		entry = zzip_readdir(dir);
	}
	// 读取升级控制文件InstallDesc
	zipfile = zzip_file_open(dir, "InstallDesc", O_RDONLY);
	if(!zipfile)
	{
		ManagerErr("CUpgrader::upgrade open install script failed.\n");
		state = stateInvalidFomat; // 没找到升级控制文件
		goto end;
	}

	len = zzip_read(zipfile, buf, 1024);
	m_burnedSize += len;
	m_upgradeInfo["State"] = "UpgradeProcess";
	m_upgradeInfo["Process"] = getProgress();
	//IEventManager::instance()->notify("Upgrade", 0, appEventPulse, NULL, NULL, &m_upgradeInfo);
	buf[len] = '\0';
	zzip_file_close(zipfile);

	if(!reader.parse(buf, table))
	{
		ManagerErr("CUpgrader::upgrade parse install script failed: \n %s\n", buf);
		state = stateInvalidFomat;
		goto end;
	}
	#if 0
	if (!arg)
	{
		const Json::Value &productDefinition = IMagicBox::instance()->getProductDefinition();

		ManagerInfo("%s\n", productDefinition.toStyledString().c_str());
		ManagerInfo("%s\n", table.toStyledString().c_str());
		// SkipCheck 直接跳过升级包检验
		if (productDefinition["Vendor"].asString() != "SkipCheck" && table["Vendor"].asString() != "SkipCheck")
		{
			// 升级包校验, 硬件标示，厂商不同时不可升级
			if (productDefinition["Hardware"].asString() != table["Hardware"].asString()
				|| productDefinition["Vendor"].asString() != table["Vendor"].asString())
			{
				ManagerErr("Upgrade check failed!\n \tHardware:%s %s\n\tVendor:%s %s\n", 
					productDefinition["Hardware"].asCString(),
					table["Hardware"].asCString(),
					productDefinition["Vendor"].asCString(),
					table["Vendor"].asCString());
				state = stateInvalidFomat;
				goto end;
			}

			if((productDefinition["Version"].asString() != "" || table["Version"].asString() != "") && (table["Version"].asString() != productDefinition["Version"].asString()))
			{
				ManagerErr("Version check failed!\n Version:%s %s\n",productDefinition["Version"].asCString(), table["Version"].asCString());
				state = stateInvalidFomat;
				goto end;
			}
		}
	}
	else
	{
		ManagerInfo("upgrader force upgrade!\n");
	}
	#endif
	
	// 执行命令列表
	{
		Json::Value& cmdTable = table["UpgradeCommand"];

		for (size_t i = 0; i < cmdTable.size(); i++)
		{
			if (cmdTable[i]["Command"].asString() == "Burn")
			{
//<shang>				ILog::instance()->append("Upgrade", cmdTable[i]["FileName"].asCString());
				m_upgradeInfo["FileName"] = cmdTable[i]["FileName"].asCString();
				if(!updatePart(dir, cmdTable[i]["FileName"].asCString()))
				{
					state = stateUpdatePartFail;
					goto end;
				}
			}
			else if (cmdTable[i]["Command"].asString() == "RemoveFile")
			{
				CFile::Remove(cmdTable[i]["FileName"].asCString());
			}
			else if (cmdTable[i]["Command"].asString() == "CryptoIdWrite")
			{
				int len = strlen(cmdTable[i]["HWID"].asCString());
				uint id[CMagicBox::magicMemoryLen/sizeof(int)];
				const char *pbuf = cmdTable[i]["HWID"].asCString();

				if (len/8*4 != CMagicBox::magicMemoryLen)
				{
					ManagerErr("ID len:%d, cryptoLen:%d :%s Error!\n", len, CMagicBox::magicMemoryLen, cmdTable[i]["HWID"].asCString());
					continue;
				}
				memset(id, 0, sizeof(id));
				for(int i = 0; i < len/8; i++)
				{
					sscanf(pbuf,"%08x", &id[i]);
					pbuf = pbuf + 8;
				}
				CMagicBox::instance()->setHWID((const uchar *)id, CMagicBox::magicMemoryLen);
			}
			else if (cmdTable[i]["Command"].asString() == "Shell")
			{
				system(cmdTable[i]["Script"].asCString());
			}
			else
			{
				ManagerErr("CUpgrader::upgrade unknown command '%s!'\n", cmdTable[i].toStyledString().c_str());
			}
			ManagerInfo("\n");
		}
	}
end:

	if(dir)
	{
		zzip_closedir(dir);
	}

	if(blocks)
	{
		delete []blocks;
	}

	cleanup();
	if (state != stateOK)
	{
		m_upgradeInfo["State"] = "UpgradeError";
	}
	else
	{
		m_upgradeInfo["State"] = "UpgradeFinish";
	}
	m_started = false;
	IEventManager::instance()->notify("Upgrade", 0, appEventPulse, NULL, NULL, &m_upgradeInfo);
}

void CUpgrader::cleanup()
{
	CGuard guard(m_mutexUpgrader);

	FlashDestroy();
	// 释放所有占用的packet
	while (!m_buffer.empty())
	{
		UChar packet = m_buffer.front();
		if (packet.Buffer)
		{
			delete packet.Buffer;
			packet.len = 0;
			//packet->Release();
		}
		m_buffer.pop_front();
	}
    m_filePath.clear();
}

bool CUpgrader::updatePart(ZZIP_DIR *dir, const char* filename)
{
	ulong flashSectorSize;
	char *fileBuffer;
	char *rawFlashBuffer;
	ulong addr;
	ulong readsize;
	bool ret = true;
	ImageHeader header;
	ulong start, end;
	ZZIP_FILE *zipfile = 0;
	ZZIP_STAT stat;

	if (FlashGetSectorSize(&flashSectorSize) < 0)
	{
		return false;
	}
	fileBuffer = new char[flashSectorSize];
	rawFlashBuffer = new char[flashSectorSize];

	zipfile = zzip_file_open(dir, filename, O_RDONLY);
	if(!zipfile)
	{
		ManagerErr("CUpgrader::burnPartion '%s' open file failed.\n", filename);
		ret = false;
		goto end;
	}

	// 从64字节头取升级包信息
	zzip_read(zipfile, (char *)&header, sizeof(ImageHeader));
	m_burnedSize += sizeof(ImageHeader);
	m_upgradeInfo["Process"] = getProgress();
	IEventManager::instance()->notify("Upgrade", 0, appEventPulse, NULL, NULL, &m_upgradeInfo);

	//start = swap_long(header.ih_load);
	//end = swap_long(header.ih_load) + swap_long(header.ih_size);
	
	start = header.ih_load;
	end = header.ih_load +header.ih_size;
	stat.st_size = 0;
	zzip_dir_stat(dir, filename, &stat, 0);

	if(end - start < (uint)stat.st_size - sizeof(ImageHeader))
	{
		ManagerErr("CUpgrader::burnPartion '%s' invalid size.\n", filename);
		ret = false;
		goto end;
	}
	else
	{
		ManagerInfo("CUpgrader::burnPartion '%s' to [%08x, %08x]\n", filename, (int)start, (int)end);
	}

	addr = start;
	while (addr < end)
	{
		if ((readsize = zzip_read(zipfile, fileBuffer, flashSectorSize)) > 0)
		{
			if (FlashRead((uchar *)rawFlashBuffer, addr, flashSectorSize) < 0)
			{
				ret = false;
				goto end;
			}

			// 比较数据，提高速度
			if (memcmp(rawFlashBuffer, fileBuffer, readsize))
			{
				FlashErase(addr);
				if (FlashWrite((uchar*)fileBuffer, addr, readsize) < 0)
				{
					ManagerErr("CUpgrader::burnPartion write flash failed.\n");
					ret = false;
					goto end;
				}
			}
			else 
			{
				ManagerInfo("CUpgrader::memcmp section data is same with img file '%s'\n", filename);
			}
			
			addr += readsize;
			m_burnedSize += readsize;
			
			m_upgradeInfo["Process"] = getProgress();
			IEventManager::instance()->notify("Upgrade", 0, appEventPulse, NULL, NULL, &m_upgradeInfo);
		}
		//else if (readsize = 0)
		else if (readsize == 0)			// zzb 2013.9.3 modify
		{
			// 文件已经读完
			ManagerInfo("img write flash sus\n");
			break;
		}
		else
		{
			ManagerErr("CUpgrader::burnPartion read zip file failed.\n");
			ret = false;
			goto end;
		}
	}

end:
	if(zipfile)
	{
		zzip_file_close(zipfile);
	}

	if(fileBuffer)
	{
		delete []fileBuffer;
	}
	if (rawFlashBuffer)
	{
		delete []rawFlashBuffer;
	}
	return ret;
}

bool CUpgrader::start(bool sync, bool force)
{
	if (!m_started)
	{
		m_started = true;
		FlashInit();
		if (sync)
		{
			onUpgrade(force);
		}
		else
		{
			m_upgrader.Start(CTimer::Proc(&CUpgrader::onUpgrade, this), 0, 0, force);
		}
		return true;
	}
	return false;
}

bool CUpgrader::setFilePath(const char* path)
{
    if (!m_started)
    {
        m_filePath = path;
        return true;
    }
    return false;
}

int CUpgrader::onConsole(int argc, char **argv)
{
	if (argc < 2)
	{
help:
		trace("upgrade -u file				upgrade use 'file'!\n");
		trace("upgrade -f file				dump buffer data to 'file'!\n");
		trace("upgrade -d file				dump default InstallDesc file to 'file'!\n");
		return 0;
	}
	switch (argv[1][1])
	{
	case 'u':
		{
			if (argc < 3)
			{
				goto help;
			}
			uchar *buf = new uchar[32*1024];
			CFile file;

			if (file.Open(argv[2], CFile::modeRead))
			{
				int len = 0;

				while ((len = file.Read(buf, 32*1024)) > 0)
				{
					putBuffer(buf, len);
				}
				file.Close();
				start(true, true);
			}
			delete [] buf;
		}
		break;

	case 'f':
		{
			if (argc < 3)
			{
				goto help;
			}
			CGuard guard(m_mutexUpgrader);
			CFile file;

			if (file.Open(argv[2], CFile::modeReadWrite|CFile::modeCreate))
			{
				trace("dump default InstallDesc file to :%s\n", argv[2]);
				for (std::deque<UChar>::iterator pi = m_buffer.begin(); pi != m_buffer.end(); pi++)
				{	
					//UChar *packet = (*pi);
					if (pi->Buffer)
					{
						file.Write(pi->Buffer, strlen(pi->Buffer));
					}
						
				}
				file.Close();
			}
		}
		break;

	case 'd':
		{
			Json::Value table;
			Json::Value& cmdTable = table["UpgradeCommand"];
			CFile file;

			cmdTable[0u]["Command"] = "Burn";
			cmdTable[0u]["FileName"] = "uboot.img";
			cmdTable[1u]["Command"] = "Burn";
			cmdTable[1u]["FileName"] = "kernel.img";
			cmdTable[2u]["Command"] = "Burn";
			cmdTable[2u]["FileName"] = "rootfs.img";
			
			if (file.Open(argv[2], CFile::modeReadWrite|CFile::modeCreate))
			{
				std::string stream;

				#ifdef _DEBUG
					CConfigWriter writer(stream);
				#else
					CConfigWriter writer;
				#endif
				trace("upgrade buffer dump to file:%s\n", argv[2]);
				stream = writer.write(table);
				std::cout << "demo write json ==============\n";
    			std::cout << stream << std::endl;
				file.Write((void *)stream.c_str(), stream.size());
				file.Flush();
				file.Close();
			}
		}
		break;

	default:
		goto help;
	}
	return 0;
}

IUpgrader *IUpgrader::instance()
{
	return CUpgrader::instance();
}

IUpgrader::~IUpgrader()
{
}
