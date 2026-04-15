#include <list>
#include <deque>
#include <vector>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <ctype.h>

#include "Inifile.h"
#include "Common.h"
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'

CInifile::CInifile()
{

	memset(m_buf,0x00,MAX_FILE_SIZE);
	m_FileName = "";
	m_pFile = NULL;
	m_Filesize = 0;

}


CInifile::~CInifile()
{



}

//if load ini file error, return -1; else if success, return 0;
int CInifile::load_ini_file(const char *file, char *buf,int *file_size)
{
//	core_assert(file !=0);
//	core_assert(buf !=0);

	if (NULL == file || NULL == buf)
	{
		//_LogError<< "The file may not exist or buf is NULL! "<< _LogErrorEnd;
		AppErr("The file may not exist or buf is NULL! \n");
		return -1;
	}	
	if (m_FileName != file)
	{
		if(m_pFile != NULL)
		{
			fclose(m_pFile);
			m_pFile = NULL;
			//m_FileName = CData("");
			m_FileName = "";
		}
		*file_size =0;

		//"r" Opens for reading. 
		//If the file does not exist or cannot be found, the fopen call fails.
		m_pFile = fopen(file,"r");
		if( NULL == m_pFile) 
		{
			//_LogError<< "Fail to open file: " << file << _LogErrorEnd;
			AppWarning("Fail to open file%s\n",file);
			return -1;
		}
		m_FileName = file;
// 		int i = 0;
// 		buf[i]=fgetc(m_pFile);
// 		
// 		//load initialization file
// 		while( buf[i]!= (char)EOF) 
// 		{
// 			i++;
// 			//core_assert( i < MAX_FILE_SIZE); //file too big
// 			if ( i >= MAX_FILE_SIZE )
// 			{
// 				close_ini_file();
// 				return -1;
// 			}
// 
// 			buf[i]=fgetc(m_pFile);
// 		}
// 		buf[i]='\0';
// 		*file_size = i;
		*file_size =  fread(buf,1,MAX_FILE_SIZE,m_pFile);
		buf[*file_size]='\0';
	}
	
	return 0;
}
//if return 0, close file success; esle the pointer to file is NULL, return -1
int CInifile::close_ini_file()
{
	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
		//m_FileName = CData("");
		m_FileName ="";

		return 0;
	}

	return -1;
}


int CInifile::isnewline(char c)
{
	return ('\n' == c ||  '\r' == c )? 1 : 0;
}


int CInifile::isend(char c)
{
	return '\0'==c? 1 : 0;
}


int CInifile::isleftbarce(char c)
{
	return LEFT_BRACE == c? 1 : 0;
}


int CInifile::isrightbrace(char c )
{
	return RIGHT_BRACE == c? 1 : 0;
}

//if return 0, parse file success; 
//esle if parameters is NULL or cannot find the section and the key, return -1
int CInifile::parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
					  int *key_s,int *key_e, int *value_s, int *value_e)
{
	const char *p = buf;
	int i=0;

// 	core_assert(buf!=0);
// 	core_assert(section != 0 && strlen(section));
// 	core_assert(key != 0 && strlen(key));
	
	if ( NULL == section || NULL == key || NULL == buf )
	{
		return -1;
	}
	else if ( 0 == strlen(section) || 0 == strlen(key) )
	{
		return -1;
	}

	*sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

	while( !isend(p[i]) ) 
	{
		//find the section
		if( ( 0==i ||  isnewline(p[i-1]) ) && isleftbarce(p[i]) )
		{
			int section_start=i+1;

			//find the ']'
			do {
				i++;
			} while( !isrightbrace(p[i]) && !isend(p[i]));

			if( 0 == strncmp(p+section_start,section, i-section_start)&&(i-section_start)==strlen(section)) 
			{
				int newline_start=0;

				i++;

				//Skip over space char after ']'
				while(isspace(p[i])) {
					i++;
				}

				//find the section
				*sec_s = section_start;
				*sec_e = i;

				while( ! (isnewline(p[i-1]) && isleftbarce(p[i])) 
				&& !isend(p[i]) ) 
				{
					int j=0;
					//get a new line
					newline_start = i;

					while( !isnewline(p[i]) &&  !isend(p[i]) ) 
					{
						i++;
					}
					
					//now i  is equal to end of the line
					j = newline_start;

					if(';' != p[j]) //skip over comment
					{
						while(j < i && p[j]!='=') 
						{
							j++;
							if('=' == p[j])
							{
								if(strncmp(key,p+newline_start,j-newline_start)==0&&(j-newline_start)==strlen(key))
								{
									//find the key ok
									*key_s = newline_start;
									*key_e = j-1;

									*value_s = j+1;
									*value_e = i;

									return 0;
								}
							}
						}
					}

					i++;
				}
			}
		}
		else
		{
			i++;
		}
	}
	return -1;
}

/**************************************************************************
* 函数名称： read_profile_string( const char *section, const char *key,
                                  char *value, int size, const char *file)
* 功能描述： 从INI格式文件file中，读取并返回[section]中key的值value
* 访问的表：
* 修改的表：
* 输入参数：@file:    INI格式文件
			@section: INI文件中，用"[]"括起来的部分
			@key:     INI文件中，"="的前面部分的关键字表示为key
			@size:    输出参数value变量的总长度        
* 输出参数：@value:   返回输出值，"="的后面部分，表示对应key的值
* 返 回 值：if return 0, read file success; esle return -1
* 其它说明：
* 修改日期 版本号 修改人 修改内容
* 
* -----------------------------------------------
* 2008.08.22 V1.0 XXXX XXXX
**************************************************************************/
//if return 0, read file success; esle return -1
int CInifile::read_profile_string( const char *section, const char *key,char *value, 
		 int size, const char *file)
{
	
	int sec_s,sec_e,key_s,key_e, value_s, value_e;

// 	//check parameters
// 	core_assert(section != 0 && strlen(section));
// 	core_assert(key != 0 && strlen(key));
// 	core_assert(value != 0);
// 	core_assert(size > 0);
// 	core_assert(file !=0 &&strlen(file));

	if ( NULL == section || NULL == key || NULL == value || size <= 0 || NULL == file )
	{
		//_LogError<< "One of parameters of read_profile_string() is NULL!! "<< _LogErrorEnd;
		AppErr("One of parameters of read_profile_string() is NULL!! \n");
		return -1;
	}
	else if (0 == strlen(section) || 0 == strlen(key) || 0 == strlen(file) )
	{
		//_LogError<< "One of parameters of read_profile_string() is NULL String!! "<< _LogErrorEnd;
		AppErr("One of parameters of read_profile_string() is NULL String!! \n");
		return -1;
	}
	// Lock
	//FastMutex::ScopedLock lock(_mutex);
	
	if ( -1 == load_ini_file(file,m_buf,&m_Filesize) )
	{
		//_LogError<< "Load ini file failed! "<< _LogErrorEnd;
		AppWarning("Load ini file[%s] failed! \n", file);
		return -1;
	}
	
	if(-1 == parse_file(section,key,m_buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
	{
		//_LogError<< "parse ini file failed! " <<section<< _LogErrorEnd;
		AppErr("parse ini file[%s] failed! section[%s] key[%s]\n", file, section, key);
		close_ini_file();
		return -1;  
	}
	else
	{
		int cpcount = value_e -value_s;

		if( size-1 < cpcount)
		{
			cpcount =  size-1;
		}
	
		memset(value, 0, size);
		memcpy(value,m_buf+value_s, cpcount );
		value[cpcount] = '\0';

		close_ini_file();
		return 0;
	}
}

/**************************************************************************
* 函数名称： write_profile_string(const char *section, const char *key,
					              const char *value, const char *file)
* 功能描述： 向INI格式文件file，把[section]中关键字为Key的值改写为value
* 访问的表：
* 修改的表：
* 输入参数：@file:    INI格式文件
			@section: INI文件中，用"[]"括起来的部分
			@key:     INI文件中，"="的前面部分的关键字表示为key
* 输出参数：@value:   返回输出值，"="的后面部分，表示对应key的值
* 返 回 值：if return 0, write file success; esle return -1
* 其它说明：
* 修改日期 版本号 修改人 修改内容
* 
* -----------------------------------------------
* 2008.08.22 V1.0 XXXX XXXX
**************************************************************************/
//if return 0, write file success; esle return -1
int CInifile::write_profile_string(const char *section, const char *key,
					const char *value, const char *file)
{
	char w_buf[MAX_FILE_SIZE]={0};
	int sec_s,sec_e,key_s,key_e, value_s, value_e;
	sec_s=sec_e=key_s=key_e=value_s=value_e = 0;
	int value_len = (int)strlen(value);

// 	//check parameters
// 	assert(section != NULL && strlen(section));
// 	assert(key != NULL && strlen(key));
// 	assert(value != NULL);
// 	assert(file !=NULL &&strlen(file));

	if ( NULL == section || NULL == key || NULL == value || NULL == file )
	{
		//_LogError<< "One of parameters of write_profile_string() is NULL!! "<< _LogErrorEnd;
		AppErr("One of parameters of write_profile_string() is NULL!! \n");
		return -1;
	}
	else if (0 == strlen(section) || 0 == strlen(key) || 0 == strlen(file) )
	{
		//_LogError<< "One of parameters of write_profile_string() is NULL String!! "<< _LogErrorEnd;
		AppErr("One of parameters of write_profile_string() is NULL String!! \n");
		return -1;
	}
	// Lock
	//FastMutex::ScopedLock lock(_mutex);
	
	if(-1 == load_ini_file(file,m_buf,&m_Filesize))
	{
		//_LogError<< "Load ini file failed! "<< _LogErrorEnd;
		AppErr("Load ini file failed! \n");
		return -1;
// 		sec_s = -1;
	}
	else
	{
		parse_file(section,key,m_buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e);
	}

	if( -1 == sec_s)
	{
		
		if(0==m_Filesize)
		{
			sprintf(w_buf+m_Filesize,"[%s]\n%s=%s\n",section,key,value);
		}
		else
		{
			//not find the section, then add the new section at end of the file
			memcpy(w_buf,m_buf,m_Filesize);
			sprintf(w_buf+m_Filesize,"\n[%s]\n%s=%s\n",section,key,value);
		}
	}
	else if(-1 == key_s)
	{
		//not find the key, then add the new key & value at end of the section
		memcpy(w_buf,m_buf,sec_e);
		sprintf(w_buf+sec_e,"%s=%s\n",key,value);
		sprintf(w_buf+sec_e+strlen(key)+strlen(value)+2,m_buf+sec_e, m_Filesize - sec_e);
	}
	else
	{
		//update value with new value
		memcpy(w_buf,m_buf,value_s);
		memcpy(w_buf+value_s,value, value_len);
		if (value_e < m_Filesize)
		{
			memcpy(w_buf+value_s+value_len, m_buf+value_e, m_Filesize - value_e);
		}
		else
		{
			memcpy(w_buf+value_s+value_len, m_buf+value_e, m_Filesize+1 - value_e);
		}
	}
	
	close_ini_file();
	//"w+"Opens an empty file for both reading and writing. 
	//If the given file exists, its contents are destroyed.
	m_pFile = fopen(file,"w+"); 
	if( NULL == m_pFile) 
	{
		//_LogError<< "Fail to open file ! "<< _LogErrorEnd;
		AppErr("Fail to open file ! \n");
		return -1;
	}
	m_FileName = file;
	
	if(-1 == fputs(w_buf,m_pFile) )
	{
// 		fclose(m_pFile);
// 		m_pFile = NULL;
		//_LogError<< "Fail to save file ! "<< _LogErrorEnd;
		AppErr("Fail to save file ! \n");
		close_ini_file();
		return -1;
	}
	memset(m_buf,0x00,MAX_FILE_SIZE);
	memcpy(m_buf,w_buf,strlen(w_buf));
	m_Filesize = strlen(m_buf);

	close_ini_file();

	return 0;
}
