#include "StdAfx.h"
#include "NtlFileSerializer.h"
#include "NtlStringHandler.h"
#include "NtlCipher.h"
#include <atlbase.h>

CNtlFileSerializer::CNtlFileSerializer() :
							CNtlSerializer()
{
}

CNtlFileSerializer::CNtlFileSerializer(int iBufferSize, int iGlowSize) :
							CNtlSerializer(iBufferSize, iGlowSize)
{
}

CNtlFileSerializer::CNtlFileSerializer(char* pszFullPath) :
							CNtlSerializer()
{
	LoadFile(pszFullPath);
}

CNtlFileSerializer::CNtlFileSerializer(WCHAR* pwszFullPath) :
							CNtlSerializer()
{
	LoadFile(pwszFullPath);
}

CNtlFileSerializer::~CNtlFileSerializer()
{
}

bool CNtlFileSerializer::SaveFile(char* pszFullPathFileName, bool bCrypt /* = FALSE */, char* szCryptPassword /* = NULL */)
{
	if(!pszFullPathFileName)
		return false;

	DWORD dwFileAttribute = ::GetFileAttributes(pszFullPathFileName);
	if (INVALID_FILE_ATTRIBUTES != dwFileAttribute)
	{
		if (dwFileAttribute & FILE_ATTRIBUTE_READONLY)
		{
			dwFileAttribute &= ~FILE_ATTRIBUTE_READONLY;

			::SetFileAttributes(pszFullPathFileName, dwFileAttribute);
		}
	}

	FILE *pFile = NULL;

	if (0 != fopen_s(&pFile, pszFullPathFileName, "wb"))
	{
		return false;
	}

	if(bCrypt)
	{
		int nBufSize = GetDataSize() + 256;	
		char* buf = new char[nBufSize];		
		ZeroMemory(buf, nBufSize);

		CNtlCipher cipher;
		int nSetKey = cipher.SetKey(DES_CIPHER, szCryptPassword, (int)strlen(szCryptPassword));
		int nEncSize = cipher.Encrypt(m_pBuffer, GetDataSize(), buf, nBufSize);

		if(nEncSize <= 0)
		{
			delete[] buf;
			fclose(pFile);
			return false;
		}

		fwrite(buf, nEncSize, 1, pFile);
		delete[] buf;
	}
	else
	{
		fwrite(GetData(), GetDataSize(), 1, pFile);
	}

	fclose(pFile);

	return true;
}

bool CNtlFileSerializer::SaveFile(WCHAR* pwszFullPathFileName, bool bCrypt /* = FALSE */, WCHAR* szCryptPassword /* = NULL */)
{
	USES_CONVERSION;

	return SaveFile(W2A(pwszFullPathFileName), bCrypt, W2A(szCryptPassword));
}

bool CNtlFileSerializer::LoadFile(char* pszFullPathFileName, bool bCrypt /* = FALSE */, char* szCryptPassword /* = NULL */)
{
	if(!pszFullPathFileName)
		return false;

	FILE *pFile = NULL;

	if (0 != fopen_s(&pFile, pszFullPathFileName, "rb"))
	{
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	if (0 == lSize)
	{
		fclose(pFile);
		return false;
	}

	Refresh();
	CheckInBuffer(lSize);

	if(bCrypt)
	{
		char* buf = new char[lSize];
		ZeroMemory(buf, lSize);

		fread((void*)buf, lSize, 1, pFile);

		CNtlCipher cipher;
		cipher.SetKey(DES_CIPHER, szCryptPassword, (int)strlen(szCryptPassword));
		int nDecSize = cipher.Decrypt(buf, lSize, m_pBuffer, lSize);
		if(nDecSize <= 0)
		{
			delete[] buf;
			fclose(pFile);
			return false;
		}

		IncrementEndPointer(nDecSize);
		delete[] buf;
	}
	else
	{
		fread((void*)(GetData()), lSize, 1, pFile);
		IncrementEndPointer(lSize);
	}

	fclose(pFile);

	return true;
}

bool CNtlFileSerializer::LoadFile(char* pszBuffer, int nSize, bool bCrypt /*= FALSE*/, char* szCryptPassword /*= NULL*/)
{
	if ( NULL == pszBuffer )
	{
		return false;
	}

	if ( nSize == 0 )
	{
		return false;
	}

	Refresh();
	CheckInBuffer( nSize );

	if ( bCrypt )
	{
		CNtlCipher cipher;
		cipher.SetKey(DES_CIPHER, szCryptPassword, (int)strlen(szCryptPassword) );

		int nDecSize = cipher.Decrypt( pszBuffer, nSize, m_pBuffer, nSize );

		if ( nDecSize <= 0 )
		{
			return false;
		}

		IncrementEndPointer( nDecSize );
	}
	else
	{
		memcpy( (void*)(GetData()), pszBuffer, nSize );

		IncrementEndPointer( nSize );
	}

	return true;
}

bool CNtlFileSerializer::LoadFile(WCHAR* pwszFullPathFileName, bool bCrypt /* = FALSE */, WCHAR* szCryptPassword /* = NULL */)
{
	USES_CONVERSION;

	return LoadFile(W2A(pwszFullPathFileName), bCrypt, W2A(szCryptPassword));
}