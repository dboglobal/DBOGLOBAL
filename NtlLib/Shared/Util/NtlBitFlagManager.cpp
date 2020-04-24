//***********************************************************************************
//
//	File		:	NtlBitFlagManager.cpp
//
//	Begin		:	2007-01-22
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlBitFlagManager.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlBitFlagManager::CNtlBitFlagManager(void)
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlBitFlagManager::CNtlBitFlagManager(CNtlBitFlagManager *bitFlag)
{
	Init();

	DWORD dwRequiredBytes = (bitFlag->m_dwMaxFlagCount - 1) / 8 + 1;

	if (bitFlag->m_pabyFlag)
	{
		BYTE* pabyFlag = new BYTE[dwRequiredBytes];
		if (NULL == pabyFlag)
		{
			//- yoshiki : To log system!
			return;
		}
		::CopyMemory(pabyFlag, bitFlag->m_pabyFlag, dwRequiredBytes * sizeof(BYTE));

		if (NULL != m_pabyFlag)
		{
			delete[] m_pabyFlag;
			m_pabyFlag = NULL;
		}

		m_pabyFlag = pabyFlag;
		m_dwBytesUsed = dwRequiredBytes;
		m_dwMaxFlagCount = bitFlag->m_dwMaxFlagCount;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlBitFlagManager::~CNtlBitFlagManager(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlBitFlagManager::Create(DWORD dwMaxFlagCount)
{
	return Create(NULL, dwMaxFlagCount);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlBitFlagManager::Create(void* pvFlag, DWORD dwMaxFlagCount)
{
	if (NULL != m_pabyFlag)
	{
		//- yoshiki : To log system!
		return false;
	}

	DWORD dwRequiredBytes = (dwMaxFlagCount - 1) / 8 + 1;

	m_pabyFlag = new BYTE[dwRequiredBytes];
	if (NULL == m_pabyFlag)
	{
		//- yoshiki : To log system!
		return false;
	}

	if (NULL == pvFlag)
	{
		::ZeroMemory(m_pabyFlag, dwRequiredBytes * sizeof(BYTE));
	}
	else
	{
		::CopyMemory(m_pabyFlag, pvFlag, dwRequiredBytes * sizeof(BYTE));
	}
	m_dwBytesUsed = dwRequiredBytes;

	m_dwMaxFlagCount = dwMaxFlagCount;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlBitFlagManager::Destroy()
{
	if (m_pabyFlag) 
	{ 
		delete[] m_pabyFlag;
		m_pabyFlag = NULL;
	}

	m_dwBytesUsed = 0;

	m_dwMaxFlagCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlBitFlagManager::Init()
{
	m_pabyFlag = NULL;
	m_dwBytesUsed = 0;

	m_dwMaxFlagCount = 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlBitFlagManager::Set(DWORD dwIndex)
{
	if (m_dwMaxFlagCount <= dwIndex)
	{
		return false;
	}

	DWORD dwByteIndex = dwIndex / 8;
	BYTE byMask = 0x01ui8 << (BYTE)(dwIndex % 8);

	m_pabyFlag[dwByteIndex] |= byMask;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlBitFlagManager::Unset(DWORD dwIndex)
{
	if (m_dwMaxFlagCount <= dwIndex)
	{
		return false;
	}

	DWORD dwByteIndex = dwIndex / 8;
	BYTE byMask = 0x01ui8 << (BYTE)(dwIndex % 8);

	m_pabyFlag[dwByteIndex] &= ~byMask;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlBitFlagManager::IsSet(DWORD dwIndex)
{
	if (m_dwMaxFlagCount <= dwIndex)
	{
		return false;
	}

	DWORD dwByteIndex = dwIndex / 8;
	BYTE byMask = 0x01ui8 << (BYTE)(dwIndex % 8);

	if (0x00ui8 == (m_pabyFlag[dwByteIndex] & byMask))
	{
		return false;
	}
	else
	{
		return true;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlBitFlagManager::Not()
{
	for (DWORD dwByteIndex = 0 ; dwByteIndex < m_dwBytesUsed ; dwByteIndex++)
	{
		m_pabyFlag[dwByteIndex] = ~m_pabyFlag[dwByteIndex];
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlBitFlagManager::Replace(void* pvNewFlag, DWORD dwMaxFlagCount)
{
	DWORD dwRequiredBytes = (dwMaxFlagCount - 1) / 8 + 1;

#ifdef _DEVEL
	if (FALSE != IsBadReadPtr(pvNewFlag, dwRequiredBytes))
#else
	if (NULL == pvNewFlag)
#endif
	{
		//- yoshiki : To log system!
		return false;
	}

	BYTE* pabyFlag = new BYTE[dwRequiredBytes];
	if (NULL == pabyFlag)
	{
		//- yoshiki : To log system!
		return false;
	}
	::CopyMemory(pabyFlag, pvNewFlag, dwRequiredBytes * sizeof(BYTE));

	if (NULL != m_pabyFlag)
	{
		delete [] m_pabyFlag;
		m_pabyFlag = NULL;
	}

	m_pabyFlag = pabyFlag;
	m_dwBytesUsed = dwRequiredBytes;

	m_dwMaxFlagCount = dwMaxFlagCount;

	return true;
}
