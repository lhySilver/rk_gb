#ifndef __INIFILE_H__
#define __INIFILE_H__

#define MAX_FILE_SIZE (5 * 1024)

#include <string>
using namespace std;

/// Class Inifile
/// Reads and writes ini file
class CInifile
{
public:
	
	CInifile();
	
	virtual ~CInifile();
public:
		/// Reads "key" from ini file
	 int read_profile_string( const char *section, const char *key,char *value, 
		 int size,const char *file);

	 	/// Write "key" to ini file
	 int write_profile_string(const char *section, const char *key,
					const char *value, const char *file);

// 		/// Singleton
// 	static Inifile& defaultInifile();
	
private:
	int load_ini_file(const char *file, char *buf,int *file_size);
//	int load_ini_file_w(const char *file, char *buf,int *file_size);
	int close_ini_file();
	
	int isnewline(char c);
	
	int isend(char c);
	
	int isleftbarce(char c);
	
	int isrightbrace(char c );
	
	 int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
					  int *key_s,int *key_e, int *value_s, int *value_e);

private:
	//mutable FastMutex  _mutex;
	string m_FileName;
	char m_buf[MAX_FILE_SIZE];
	FILE *m_pFile;
	int m_Filesize;
};

#endif
