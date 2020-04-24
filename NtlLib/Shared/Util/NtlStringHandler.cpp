#include "StdAfx.h"
#include "NtlStringHandler.h"


const DWORD NTL_MAX_LENGTH_OF_FORMAT_STRING_RESULT = 1024;

std::wstring s2ws(const std::string& s)
{
#if(_MSC_PLATFORM_TOOLSET > 100)

	std::string convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(s);
#else

	WCHAR* pTemp = Ntl_MB2WC(s.c_str());

	std::wstring wstrResult = pTemp;

	Ntl_CleanUpHeapStringW(pTemp);

	return wstrResult;

#endif
}

std::string ws2s(const std::wstring& wstr)
{
#if(_MSC_PLATFORM_TOOLSET > 100)

	auto convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);

#else

	char* pTemp = Ntl_WC2MB(wstr.c_str());

	std::string strResult = pTemp;

	Ntl_CleanUpHeapString(pTemp);

	return strResult;

#endif
}

WCHAR* Ntl_MB2WC(const char* pszOriginalString)
{
	if (NULL == pszOriginalString)
	{
		return NULL;
	}

	int iRequiredChars = ::MultiByteToWideChar(::GetACP(), 0, pszOriginalString, -1, NULL, 0);
	WCHAR* pwszResultString = new WCHAR[iRequiredChars];
	if (NULL == pwszResultString)
	{
		return NULL;
	}

	::MultiByteToWideChar(::GetACP(), 0, pszOriginalString, -1, pwszResultString, iRequiredChars);

	return pwszResultString;
}

char* Ntl_WC2MB(const WCHAR* pwszOriginalString)
{
	if (NULL == pwszOriginalString)
	{
		return NULL;
	}

	int iRequiredChars = ::WideCharToMultiByte(::GetACP(), 0, pwszOriginalString, -1, NULL, 0, NULL, NULL);
	char* pszResultString = new char[iRequiredChars];
	if (NULL == pszResultString)
	{
		return NULL;
	}

	::WideCharToMultiByte(::GetACP(), 0, pwszOriginalString, -1, pszResultString, iRequiredChars, NULL, NULL);

	return pszResultString;
}

void Ntl_CleanUpHeapString(char* pszString)
{
	if (NULL != pszString)
	{
		delete [] pszString;
	}
}

void Ntl_CleanUpHeapStringW(WCHAR* pwszString)
{
	if (NULL != pwszString)
	{
		delete [] pwszString;
	}
}

void Ntl_CleanUpHeapStringW(const WCHAR* pwszString)
{
	if (NULL != pwszString)
	{
		delete[] pwszString;
	}
}

int Ntl_GenerateFormattedString(std::string& rstrResult, char* pszFormat, ...)
{
	char szResult[NTL_MAX_LENGTH_OF_FORMAT_STRING_RESULT + 1];
	::ZeroMemory(szResult, _countof(szResult));

	va_list valist;
	va_start(valist, pszFormat);

	int nWrittenBytes = vsprintf_s(szResult, _countof(szResult), pszFormat, valist);

	va_end(valist);

	if (0 < nWrittenBytes)
	{
		rstrResult.assign(szResult);
	}
	else
	{
		rstrResult.clear();
	}

	return nWrittenBytes;
}

int Ntl_GenerateFormattedStringW(std::wstring& rwstrResult, WCHAR* pwszFormat, ...)
{
	WCHAR wszResult[NTL_MAX_LENGTH_OF_FORMAT_STRING_RESULT + 1];
	::ZeroMemory(wszResult, _countof(wszResult));

	va_list valist;
	va_start(valist, pwszFormat);

	int nWrittenBytes = vswprintf_s(wszResult, _countof(wszResult), pwszFormat, valist);

	va_end(valist);

	if (0 < nWrittenBytes)
	{
		rwstrResult.assign(wszResult);
	}
	else
	{
		rwstrResult.clear();
	}

	return nWrittenBytes;
}