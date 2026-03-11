#ifndef __MANAGE_REGISTRY__
#define __MANAGE_REGISTRY__

#ifdef _WIN32
#include <string>
#include <tchar.h>
#include <Shlwapi.h>


typedef std::basic_string<TCHAR> T2String;

class CRegStdBase
{
public:

	/*
	 * Removes the whole registry key including all values. So if you set the registry
	 * entry to be HKCU\Software\Company\Product\key\value there will only be
	 * HKCU\Software\Company\Product key in the registry.
	 * \return ERROR_SUCCESS or an nonzero errorcode. Use FormatMessage() to get an error description.
	 */
	DWORD removeKey() { RegOpenKeyEx(m_base, m_path.c_str(), 0, KEY_WRITE, &m_hKey); return SHDeleteKey(m_base, m_path.c_str()); }

	/*
	 * Removes the value of the registry object. If you set the registry entry to
	 * be HKCU\Software\Company\Product\key\value there will only be
	 * HKCU\Software\Company\Product\key\ in the registry.
	 * \return ERROR_SUCCESS or an nonzero errorcode. Use FormatMessage() to get an error description.
	 */
	LONG removeValue() { RegOpenKeyEx(m_base, m_path.c_str(), 0, KEY_WRITE, &m_hKey); return RegDeleteValue(m_hKey, m_key.c_str()); }

public:
	//handle to the registry base
	HKEY m_base;

	//handle to the open registry key
	HKEY m_hKey;

	//the name of the value
	T2String m_key;

	//the path to the key
	T2String m_path;
};
class CRegString : public CRegStdBase
{
public:
	/*
	 * Constructor.
	 * \param key the path to the key, including the key. example: "Software\\Company\\SubKey\\MyValue"
	 * \param def the default value used when the key does not exist or a read error occured
	 * \param force set to TRUE if no cache should be used, i.e. always read and write directly from/to registry
	 * \param base a predefined base key like HKEY_LOCAL_MACHINE. see the SDK documentation for more information.
	 */
	CRegString(T2String key, T2String def = _T(""), BOOL force = FALSE, HKEY base = HKEY_CURRENT_USER);
	~CRegString(void);

public:
	T2String read();//reads the value from the registry
	void write();//writes the value to the registry

public:
	operator T2String();
	CRegString& operator=(T2String s);
	CRegString& operator+=(T2String s) { return *this = (T2String)*this + s; }
	operator LPCTSTR();

protected:
	//indicates if the value has already been read from the registry
	BOOL m_read;

	//indicates if no cache should be used, i.e. always read and write directly from registry
	BOOL m_force;

	//the cached value of the registry
	T2String m_value;

	//the default value to use
	T2String m_defaultvalue;
};
class CRegWORD : public CRegStdBase
{
public:
	/*
	 * Constructor.
	 * \param key the path to the key, including the key. example: "Software\\Company\\SubKey\\MyValue"
	 * \param def the default value used when the key does not exist or a read error occured
	 * \param force set to TRUE if no cache should be used, i.e. always read and write directly from/to registry
	 * \param base a predefined base key like HKEY_LOCAL_MACHINE. see the SDK documentation for more information.
	 */

	CRegWORD(T2String key, DWORD def = 0, BOOL force = FALSE, HKEY base = HKEY_CURRENT_USER);
	~CRegWORD(void);

public:
	DWORD read();//reads the value from the registry
	void write();//writes the value to the registry

public:
	operator DWORD();

	CRegWORD& operator=(DWORD d);
	CRegWORD& operator+=(DWORD d) { return *this = *this + d;}
	CRegWORD& operator-=(DWORD d) { return *this = *this - d;}
	CRegWORD& operator*=(DWORD d) { return *this = *this * d;}
	CRegWORD& operator/=(DWORD d) { return *this = *this / d;}
	CRegWORD& operator%=(DWORD d) { return *this = *this % d;}
	CRegWORD& operator<<=(DWORD d) { return *this = *this << d;}
	CRegWORD& operator>>=(DWORD d) { return *this = *this >> d;}
	CRegWORD& operator&=(DWORD d) { return *this = *this & d;}
	CRegWORD& operator|=(DWORD d) { return *this = *this | d;}
	CRegWORD& operator^=(DWORD d) { return *this = *this ^ d;}

protected:
	//indicates if the value has already been read from the registry
	BOOL m_read;

	//indicates if no cache should be used, i.e. always read and write directly from registry
	BOOL m_force;

	//the cached value of the registry
	DWORD m_value;

	//the default value to use
	DWORD m_defaultvalue;
};



#endif
#endif
