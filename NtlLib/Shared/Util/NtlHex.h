#pragma once
//***********************************************************************************
//
//	File		:	Utils.h
//
//	Begin		:	2006-03-24
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:
//
//	Desc		:	
//
//***********************************************************************************

bool				WideCharToHex(WCHAR* pwszString, DWORD dwBufferSize, char* pszBuffer);

bool				HexToWideChar(char* pszHex, DWORD dwBufferSize, WCHAR* pwszBuffer);

// --------------------------------------------------------------------------------
// StreamToHex
//
// 결과에 terminating null('\0')을 붙입니다.
// A terminating null character('\0') will be added to the result.
// --------------------------------------------------------------------------------
// HexToStream
//
// Stream에는 terminating null의 개념이 없으므로 결과에 '\0'을 붙이지 않습니다.
// '\0' will NOT be added to the result since stream isn't supposed to have terminating null.
// --------------------------------------------------------------------------------
// by YOSHIKI(2007-02-28)
// --------------------------------------------------------------------------------
bool				StreamToHex(BYTE* pbyStream, DWORD dwStreamLength, char* pszBuffer, DWORD dwBufferSize);

bool				HexToStream(char* pszHex, BYTE* pbyBuffer, DWORD dwBufferSize);


char				ValueToCharacter(BYTE byValue);

BYTE				CharacterToValue(char chCharacter);