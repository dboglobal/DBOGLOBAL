#include "precomp_ntlcore.h"
#include "NtlStringUtil.h"


RwUInt32 GetByte(const wchar_t* pwcText)
{
	if( wcscmp(pwcText, L"") == 0 )
		return 0;

	char *pcFirst, *pcNext;
	static char acBuffer[256] = "";

	memset(acBuffer, 0, 256);
	WideCharToMultiByte(GetACP(), 0, pwcText, -1, acBuffer, 256, NULL, NULL);

	pcFirst = pcNext = acBuffer;

	while( strcmp(pcNext, "") != 0 )
	{
		pcNext = CharNextExA(GetACP(), pcNext, 0);
	}

	return (RwUInt32)(pcNext - pcFirst);
}


RwInt32 GetNumberLength(RwInt32 iNumber)
{
	char acBuffer[20] = "";

	_itoa_s(iNumber, acBuffer, 10);

	return strlen(acBuffer);
}

RwInt32 GetNumberLength(RwUInt32 uiNumber)
{
	char acBuffer[20] = "";

	_ultoa_s(uiNumber, acBuffer, 10);

	return strlen(acBuffer);
}

char* MakeStr(char* pcText, ...)
{
	static char acBuffer[1024];

	ZeroMemory(acBuffer, 1024);

	va_list va;
	va_start(va, pcText);

	vsprintf_s(acBuffer, pcText, va);

	va_end(va);

	return acBuffer;
}

void StringTokenize(std::vector<std::string>& vecstrOutToken, const std::string& strText, const std::string& strDelimiters)
{
	std::string::size_type lastPos	= strText.find_first_not_of(strDelimiters, 0);
	std::string::size_type pos		= strText.find_first_of(strDelimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		vecstrOutToken.push_back(strText.substr(lastPos, pos - lastPos));
		
		lastPos	= strText.find_first_not_of(strDelimiters, pos);
		pos		= strText.find_first_of(strDelimiters, lastPos);
	}
}

std::string StringReplace(const std::string& strText, const std::string& strDest, const std::string& strReplace)
{
	std::string				strResult	= strText;
	std::string::size_type	pos			= 0;
	std::string::size_type	offset		= 0;
	std::string::size_type	lenDest		= strDest.size();
	std::string::size_type	lenReplace	= strReplace.size();

	while ((pos = strResult.find(strDest, offset)) != std::string::npos)
	{
		strResult.replace(strResult.begin() + pos, strResult.begin() + pos + lenDest, strReplace);
		offset = pos + lenReplace;
	}
	return strResult;
}




