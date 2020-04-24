#include "precomp_ntlcore.h"
#include "registry.h"


CRegistry::CRegistry(void)
{
}

CRegistry::~CRegistry(void)
{
}

void CRegistry::SetRegistry(const string& SubKey, const string& ValueName, const string& Value)
{
	HKEY hkResult; 
	HKEY hKey = HKEY_LOCAL_MACHINE; 
	LONG ret;
	DWORD dwState;

	ret = RegCreateKeyEx(hKey, SubKey.c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwState);
	if(ret != ERROR_SUCCESS)
		return;

	ret = RegSetValueEx(hkResult, ValueName.c_str(), NULL, REG_SZ, (const BYTE*)(Value.c_str()), static_cast<DWORD>(Value.length()));
	
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(hkResult);
		return;
	}

	RegCloseKey(hkResult);
}

void CRegistry::GetRegistry(const string& SubKey, const string& ValueName, string& Value)
{
	HKEY hkResult; 
	HKEY hKey = HKEY_LOCAL_MACHINE; 
	LONG ret;
	DWORD dwByte, dwType;	
	static char szBuff[80];

	ZeroMemory(szBuff, sizeof(char) * 80);

	ret = RegOpenKeyEx(hKey, SubKey.c_str(), 0, KEY_ALL_ACCESS, &hkResult);
	if(ret != ERROR_SUCCESS)
		Value = "ERROR";

	ret = RegQueryValueEx(hkResult, ValueName.c_str(), 0, &dwType, (unsigned char*)szBuff, &dwByte);
	
	if(ret != ERROR_SUCCESS)
	{
		RegCloseKey(hkResult);
		Value = "ERROR";
		return;
	}

	RegCloseKey(hkResult);

	Value = szBuff;
}

void CRegistry::DelRegistry(const string& SubKey, const string& ValueName)
{
	HKEY hkResult; 
	HKEY hKey = HKEY_LOCAL_MACHINE; 
	LONG ret;
		
	ret = RegOpenKeyEx(hKey, SubKey.c_str(), 0, KEY_ALL_ACCESS, &hkResult );
	if( ret != ERROR_SUCCESS )
	{
		return;
	}

	RegDeleteValue(hkResult, ValueName.c_str());
	RegCloseKey(hkResult);
}