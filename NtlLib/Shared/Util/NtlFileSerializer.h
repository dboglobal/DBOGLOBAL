/*****************************************************************************
 *
 * File			: NtlFileSerializer.h
 * Author		: 
 * Copyright	: (аж)NTL
 * Date			: 2007. 02. 06
 * Abstract		: 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include "NtlSerializer.h"

class CNtlFileSerializer : public CNtlSerializer
{
public:

	CNtlFileSerializer();
	CNtlFileSerializer(int iBufferSize, int iGlowSize);

	CNtlFileSerializer(char* pszFullPath);
	CNtlFileSerializer(WCHAR* pwszFullPath);

	virtual ~CNtlFileSerializer();

public:
	bool SaveFile(char* pszFullPathFileName, bool bCrypt = FALSE, char* szCryptPassword = NULL);
	bool SaveFile(WCHAR* pwszFullPathFileName, bool bCrypt = FALSE, WCHAR* szCryptPassword = NULL);

	bool LoadFile(char* pszFullPathFileName, bool bCrypt = FALSE, char* szCryptPassword = NULL);
	bool LoadFile(char* pszBuffer, int nSize, bool bCrypt = FALSE, char* szCryptPassword = NULL);

	bool LoadFile(WCHAR* pwszFullPathFileName, bool bCrypt = FALSE, WCHAR* szCryptPassword = NULL);
};