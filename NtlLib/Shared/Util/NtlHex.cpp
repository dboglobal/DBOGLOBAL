//***********************************************************************************
//
//	File		:	Utils.cpp
//
//	Begin		:	2006-03-24
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include <stdlib.h>
#include "NtlHex.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool WideCharToHex(WCHAR* pwszString, DWORD dwBufferSize, char* pszBuffer)
{
	if (NULL == pwszString || 3 > dwBufferSize)
		return false;
	if (FALSE != IsBadWritePtr(pszBuffer, dwBufferSize))
		return false;

	BYTE* pbyOriginalPosition = (BYTE*)pwszString;
	DWORD dwCurrentIndex = 0;
	DWORD dwBufferIndex = 0;

	strcpy_s(pszBuffer, dwBufferSize, "0x");
	dwBufferIndex += 2;

	while (dwBufferIndex + 4 <= dwBufferSize - 1)
	{
		BYTE byPart1 = *(pbyOriginalPosition + dwCurrentIndex) >> 4;
		BYTE byPart2 = *(pbyOriginalPosition + dwCurrentIndex) & 0x0F;
		BYTE byPart3 = *(pbyOriginalPosition + dwCurrentIndex + 1) >> 4;
		BYTE byPart4 = *(pbyOriginalPosition + dwCurrentIndex + 1) & 0x0F;

		*(pszBuffer + dwBufferIndex) = ValueToCharacter(byPart1);
		*(pszBuffer + dwBufferIndex + 1) = ValueToCharacter(byPart2);
		*(pszBuffer + dwBufferIndex + 2) = ValueToCharacter(byPart3);
		*(pszBuffer + dwBufferIndex + 3) = ValueToCharacter(byPart4);

		dwCurrentIndex += 2;
		dwBufferIndex += 4;

		if (0 == byPart1 && 0 == byPart2 && 0 == byPart3 && 0 == byPart4)
			break;
	}

	*(pszBuffer + dwBufferIndex) = '\0';

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool HexToWideChar(char* pszHex, DWORD dwBufferSize, WCHAR* pwszBuffer)
{
	if (NULL == pszHex || 0 == dwBufferSize)
		return false;
	if (FALSE != IsBadWritePtr(pwszBuffer, dwBufferSize * sizeof(WCHAR)))
		return false;

	if (0 != strncmp(pszHex, "0x", 2))
		return false;

	BYTE* pbyOriginalBufferPosition = (BYTE*)pwszBuffer;
	DWORD dwCurrentIndex = 2;
	DWORD dwBufferIndex = 0;
	DWORD dwBufferSizeInBytes = dwBufferSize * 2;

	while (dwBufferIndex + 2 <= dwBufferSizeInBytes - 2)
	{
		BYTE byPart1 = CharacterToValue(*(pszHex + dwCurrentIndex));
		BYTE byPart2 = CharacterToValue(*(pszHex + dwCurrentIndex + 1));
		BYTE byPart3 = CharacterToValue(*(pszHex + dwCurrentIndex + 2));
		BYTE byPart4 = CharacterToValue(*(pszHex + dwCurrentIndex + 3));

		*(pbyOriginalBufferPosition + dwBufferIndex) = byPart1 << 4 | (byPart2 & 0x0F);
		*(pbyOriginalBufferPosition + dwBufferIndex + 1) = byPart3 << 4 | (byPart4 & 0x0F);

		dwCurrentIndex += 4;
		dwBufferIndex += 2;

		if (0 == byPart1 && 0 == byPart2 && 0 == byPart3 && 0 == byPart4)
			break;
	}

	*(pbyOriginalBufferPosition + dwBufferIndex) = '\0';
	*(pbyOriginalBufferPosition + dwBufferIndex + 1) = '\0';

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool StreamToHex(BYTE* pbyStream, DWORD dwStreamLength, char* pszBuffer, DWORD dwBufferSize)
{
	if (NULL == pbyStream || NULL == pszBuffer)
	{
		return false;
	}
	if (FALSE != IsBadWritePtr(pszBuffer, dwBufferSize * sizeof(char)))
	{
		return false;
	}

	DWORD dwRequiredBufferSize = dwStreamLength * 2 + 2 + 1;
	if (dwBufferSize < dwRequiredBufferSize)
	{
		return false;
	}

	DWORD dwCurrentIndex = 0;
	DWORD dwBufferIndex = 0;

	strcpy_s(pszBuffer, dwBufferSize, "0x");
	dwBufferIndex += 2;

	while (dwCurrentIndex < dwStreamLength)
	{
		BYTE byPart1 = *(pbyStream + dwCurrentIndex) >> 4;
		BYTE byPart2 = *(pbyStream + dwCurrentIndex) & 0x0F;

		*(pszBuffer + dwBufferIndex) = ValueToCharacter(byPart1);
		*(pszBuffer + dwBufferIndex + 1) = ValueToCharacter(byPart2);

		dwCurrentIndex += 1;
		dwBufferIndex += 2;
	}

	*(pszBuffer + dwBufferIndex) = '\0';

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool HexToStream(char* pszHex, BYTE* pbyBuffer, DWORD dwBufferSize)
{
	if (NULL == pszHex || 0 == dwBufferSize)
	{
		return false;
	}
	if (FALSE != IsBadWritePtr(pbyBuffer, dwBufferSize * sizeof(BYTE)))
	{
		return false;
	}

	if (0 != strncmp(pszHex, "0x", 2))
	{
		return false;
	}

	size_t sizeHex = strlen(pszHex);
	if (0 != sizeHex % 2)
	{
		return false;
	}

	DWORD dwRequiredBufferSize = ((DWORD)(strlen(pszHex)) - 2) / 2;
	if (dwBufferSize < dwRequiredBufferSize)
	{
		return false;
	}

	DWORD dwCurrentIndex = 2;
	DWORD dwBufferIndex = 0;

	while (dwCurrentIndex + 2 <= sizeHex)
	{
		BYTE byPart1 = CharacterToValue(*(pszHex + dwCurrentIndex));
		BYTE byPart2 = CharacterToValue(*(pszHex + dwCurrentIndex + 1));

		*(pbyBuffer + dwBufferIndex) = (byPart1 << 4) | (byPart2 & 0x0F);

		dwCurrentIndex += 2;
		dwBufferIndex += 1;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
char ValueToCharacter(BYTE byValue)
{
	if (0 <= byValue && 9 >= byValue)
	{
		return '0' + byValue;
	}
	else if (10 <= byValue && 15 >= byValue)
	{
		return 'a' + (byValue - 10);
	}

	return '\0';
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BYTE CharacterToValue(char chCharacter)
{
	if ('0' <= chCharacter && '9' >= chCharacter)
	{
		return chCharacter - '0';
	}
	else if ('a' <= chCharacter && 'f' >= chCharacter)
	{
		return chCharacter - 'a' + 10;
	}

	return 0;
}
