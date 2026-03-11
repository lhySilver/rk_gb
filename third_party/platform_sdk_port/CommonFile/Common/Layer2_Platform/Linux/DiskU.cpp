#include "DiskU.h"
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

bool isFolderExists(const string& strFolder)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
	DIR *pDir = opendir(strFolder.c_str());
	if(pDir){
		closedir(pDir);
		return true;
	}
#endif
	return false;
}

bool isRemoveAble(const string& strNode)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
    long nTotal = 0;
    char cData[2048] = {0};
    char cStram[2048] = {0};
    sprintf(cStram, "cat /sys/block/%s/removable", strNode.c_str());
    FILE *fstream = popen(cStram, "r");
    if(fstream){
        if(fgets(cData, sizeof(cData), fstream)){
            int nLen = strlen(cData);
            cData[nLen -1] = 0x00;
            nTotal = strtol(cData, NULL, 10);
        }
        pclose(fstream);
    }
    if(nTotal > 0){
        return true;
    }
#endif
    return false;
}
bool isParentNode(const string& strNode)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
	long nTotal = 0;
	char cData[2048] = {0};
	char cStram[2048] = {0};
	sprintf(cStram, "lsblk -m /dev/%s", strNode.c_str());
	FILE *fstream = popen(cStram, "r");
	if(fstream){
		while(fgets(cData, sizeof(cData), fstream)){
			nTotal++;
		}
		pclose(fstream);
	}
	if(nTotal > 2){
		return true;
	}
#endif
	return false;
}
void GetUDiskPath(const string& strUDiskAllData, string& strDiskMountPoint)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
    size_t nPos = strUDiskAllData.find("part");
    if(nPos != string::npos){
        strDiskMountPoint = strUDiskAllData.substr(nPos + strlen("part") + 1, strUDiskAllData.size());
    }
    else{
        nPos = strUDiskAllData.find("disk");
        if(nPos != string::npos){
            strDiskMountPoint = strUDiskAllData.substr(nPos + strlen("disk") + 1, strUDiskAllData.size());
        }
    }
#endif
}

void MountDiskU()
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
	char cData[4096] = {0};
	char cStram[2048] = {0};
    sprintf(cStram, "lsblk");

    bool bFirst = true;
    bool bExistsParentNode = false;
    string strParentNode = "";
	FILE *fstream = popen(cStram, "r");
	if(fstream){
		while(fgets(cData, sizeof(cData), fstream)){
            if(bFirst){//过滤第一行标题
                bFirst = false;
                continue;
            }

			long nLen = strlen(cData);
			if(nLen > 0){
				cData[nLen - 1] = 0x00;

				string strDiskID = "";
				string strDiskMountPoint= "";
				string strData = cData;
				long nPos1 = strData.find(" ");
				if(nPos1 != string::npos){
					strDiskID = strData.substr(0, nPos1);
					if(strDiskID.find("└─") != string::npos){
						strDiskID = strDiskID.substr(strlen("└─"), strDiskID.size());
					}
				}

                bool bNeedCheckRemoveAble = true;
                if(bExistsParentNode){
                    if(strDiskID.find(strParentNode) != string::npos){
                        bNeedCheckRemoveAble = false;
                    }
                }

				if(strDiskID.empty()){
					continue;
				}
                if(strDiskID.find("loop") != string::npos){
                    continue;
                }
                if(bNeedCheckRemoveAble && !isRemoveAble(strDiskID)){
                    continue;
                }
                if(isParentNode(strDiskID)){
                    strParentNode = strDiskID;
                    bExistsParentNode = true;
                    continue;
                }
                strParentNode = "";
                bExistsParentNode = false;

                GetUDiskPath(cData, strDiskMountPoint);

				if(strDiskMountPoint.size() > 0){}
				else{
					time_t tmNow = time(NULL);
					struct tm *timeCurrent = localtime(&tmNow);

					DIR *pDir = opendir("/opt/DiskU/");
					if(pDir){
						closedir(pDir);
					}
					else{
						system("mkdir -p /opt/DiskU");
					}

					char cPath[2048] = {0};
					sprintf(cPath, "/opt/DiskU/%s_%04d_%02d_%02d_%02d_%02d_%02d", strDiskID.c_str(), 1900 + timeCurrent->tm_year, timeCurrent->tm_mon + 1, timeCurrent->tm_mday, timeCurrent->tm_hour, timeCurrent->tm_min, timeCurrent->tm_sec);
					if(!isFolderExists(cPath)){
						mkdir(cPath, S_IRWXU | S_IRWXG | S_IRWXO);
					}

					char cCommand[2048] = {0};
					sprintf(cCommand, "mount -o iocharset=utf8 /dev/%s %s", strDiskID.c_str(), cPath);
					system(cCommand);
				}
			}
		}
		pclose(fstream);
	}
#endif
}

bool CheckDiskU(string& strPath, string& strID)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
	char cData[4096] = {0};
	char cStram[2048] = {0};
    sprintf(cStram, "lsblk");

    bool bFirst = true;
    bool bExistsParentNode = false;
    string strParentNode = "";

	FILE *fstream = popen(cStram, "r");
	if(fstream){
		while(fgets(cData, sizeof(cData), fstream)){
            if(bFirst){//过滤第一行标题
                bFirst = false;
                continue;
            }

            long nLen = strlen(cData);
            if(nLen > 0){
                cData[nLen - 1] = 0x00;

                string strDiskID = "";
                string strDiskMountPoint= "";
                string strData = cData;
                long nPos1 = strData.find(" ");
                if(nPos1 != string::npos){
                    strDiskID = strData.substr(0, nPos1);
                    if(strDiskID.find("└─") != string::npos){
                        strDiskID = strDiskID.substr(strlen("└─"), strDiskID.size());
                    }
                }

                bool bNeedCheckRemoveAble = true;
                if(bExistsParentNode){
                    if(strDiskID.find(strParentNode) != string::npos){
                        bNeedCheckRemoveAble = false;
                    }
                }

                if(strDiskID.empty()){
                    continue;
                }
                if(strDiskID.find("loop") != string::npos){
                    continue;
                }
                if(bNeedCheckRemoveAble && !isRemoveAble(strDiskID)){
                    continue;
                }
                if(isParentNode(strDiskID)){
                    strParentNode = strDiskID;
                    bExistsParentNode = true;
                    continue;
                }
                strParentNode = "";
                bExistsParentNode = false;

                GetUDiskPath(cData, strDiskMountPoint);

				if(strDiskMountPoint.size() > 0){
					strID = strDiskID;
					strPath = strDiskMountPoint;
					pclose(fstream);
					return true;
				}
			}
		}
		pclose(fstream);
	}
#endif
	return false;
}


bool FindDiskU(string& strPath, string& strID)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
    MountDiskU();

    if(CheckDiskU(strPath, strID)){
		return true;
	}

#endif
	return false;
}

void RepairDiskU(const string& strPath, const string& strID)
{
#if (!defined _WIN32) && (!defined __ENVIRONMENT_MAC__)
	char cCommand[2048] = {0};
	sprintf(cCommand, "umount %s", strPath.c_str());
	system(cCommand);

	memset(cCommand, 0, 2048);
	sprintf(cCommand, "fsck -y %s", strID.c_str());
	system(cCommand);
#endif
}
