#include "Registry.h"
#ifdef _WIN32

/*
 * Constructor.
 * @param key the path to the key, including the key. example: "Software\\Company\\SubKey\\MyValue"
 * @param def the default value used when the key does not exist or a read error occured
 * @param force set to TRUE if no cache should be used, i.e. always read and write directly from/to registry
 * @param base a predefined base key like HKEY_LOCAL_MACHINE. see the SDK documentation for more information.
 */
CRegString::CRegString(T2String key, T2String def, BOOL force, HKEY base)
{
	m_value = _T("");
	m_defaultvalue = def;
	m_force = force;
	m_base = base;
	m_read = FALSE;

	T2String::size_type pos = key.find_last_of(_T('\\'));
	m_path = key.substr(0, pos);
	m_key = key.substr(pos + 1);
	read();
}

CRegString::~CRegString(void)
{

}

T2String CRegString::read()
{
	if (RegOpenKeyEx(m_base, m_path.c_str(), 0, KEY_EXECUTE, &m_hKey)==ERROR_SUCCESS){
		int size = 0;
		DWORD type;
		RegQueryValueEx(m_hKey, m_key.c_str(), NULL, &type, NULL, (LPDWORD) &size);
		TCHAR* pStr = new TCHAR[size];
		if (RegQueryValueEx(m_hKey, m_key.c_str(), NULL, &type, (BYTE*) pStr,(LPDWORD) &size)==ERROR_SUCCESS){
			m_value.assign(pStr);
			delete [] pStr;
			m_read = TRUE;
			RegCloseKey(m_hKey);
			return m_value;
		}
		else{
			delete [] pStr;
			RegCloseKey(m_hKey);
			return m_defaultvalue;
		}
	}
	return m_defaultvalue;
}

void CRegString::write()
{
	DWORD disp;
	if (RegCreateKeyEx(m_base, m_path.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &m_hKey, &disp)!=ERROR_SUCCESS){
		return;
	}
	if (RegSetValueEx(m_hKey, m_key.c_str(), 0, REG_SZ, (BYTE *)m_value.c_str(), (DWORD)m_value.size()+1)==ERROR_SUCCESS){
		m_read = TRUE;
	}
	RegCloseKey(m_hKey);
}

CRegString::operator LPCTSTR()
{
	if (m_read && !m_force){
		return m_value.c_str();
	}
	else{
		return read().c_str();
	}
}

CRegString::operator T2String()
{
	if (m_read && !m_force){
		return m_value;
	}
	else{
		return read();
	}
}

CRegString& CRegString::operator =(T2String s)
{
	if (s == m_value && !m_force){
		return *this;
	}
	m_value = s;
	write();
	return *this;
}


/*
 * Constructor.
 * @param key the path to the key, including the key. example: "Software\\Company\\SubKey\\MyValue"
 * @param def the default value used when the key does not exist or a read error occured
 * @param force set to TRUE if no cache should be used, i.e. always read and write directly from/to registry
 * @param base a predefined base key like HKEY_LOCAL_MACHINE. see the SDK documentation for more information.
 */
CRegWORD::CRegWORD(T2String key, DWORD def, BOOL force, HKEY base)
{
	m_value = 0;
	m_defaultvalue = def;
	m_force = force;
	m_base = base;
	m_read = FALSE;

	T2String::size_type pos = key.find_last_of(_T('\\'));
	m_path = key.substr(0, pos);
	m_key = key.substr(pos + 1);
	read();
}

CRegWORD::~CRegWORD(void)
{
}

DWORD CRegWORD::read()
{
	if (RegOpenKeyEx(m_base, m_path.c_str(), 0, KEY_EXECUTE, &m_hKey) == ERROR_SUCCESS){
		int size = sizeof(m_value);
		DWORD type;
		if (RegQueryValueEx(m_hKey, m_key.c_str(), NULL, &type, (BYTE*) &m_value,(LPDWORD) &size) == ERROR_SUCCESS){
			m_read = TRUE;
			RegCloseKey(m_hKey);
			return m_value;
		}
		else{
			RegCloseKey(m_hKey);
			return m_defaultvalue;
		}
	}
	return m_defaultvalue;
}

void CRegWORD::write()
{
	DWORD disp;
	if (RegCreateKeyEx(m_base, m_path.c_str(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &m_hKey, &disp) != ERROR_SUCCESS){
		return;
	}
	if (RegSetValueEx(m_hKey, m_key.c_str(), 0, REG_DWORD,(const BYTE*) &m_value, sizeof(m_value)) == ERROR_SUCCESS){
		m_read = TRUE;
	}
	RegCloseKey(m_hKey);
}

CRegWORD::operator DWORD()
{
	if (m_read && !m_force){
		return m_value;
	}
	else{
		return read();
	}
}

#endif