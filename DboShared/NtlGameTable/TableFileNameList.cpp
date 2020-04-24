//***********************************************************************************
//
//	File		:	TableFileNameList.cpp
//
//	Begin		:	2007-02-02
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "TableFileNameList.h"

#include "NtlStringHandler.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableFileNameList::CTableFileNameList(void)
{
	Init();	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTableFileNameList::~CTableFileNameList(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableFileNameList::Create()
{
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CTableFileNameList::Destroy()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CTableFileNameList::Init()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
char* CTableFileNameList::GetFileName(CTableContainer::eTABLE eTable)
{
	if (CTableContainer::TABLE_COUNT <= eTable)
	{
		//- yoshiki : To log system!
		return NULL;
	}

	return (char*)(m_astrFileName[eTable].c_str());
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WCHAR* CTableFileNameList::GetFileNameW(CTableContainer::eTABLE eTable)
{
	if (CTableContainer::TABLE_COUNT <= eTable)
	{
		//- yoshiki : To log system!
		return NULL;
	}

	return (WCHAR*)(m_awstrFileName[eTable].c_str());
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableFileNameList::SetFileName(CTableContainer::eTABLE eTable, char* pszFileNameWithoutExtension)
{
	if (CTableContainer::TABLE_COUNT <= eTable || NULL == pszFileNameWithoutExtension)
	{
		//- yoshiki : To log system!
		return false;
	}

	WCHAR* pwszFileNameWithoutExtension = Ntl_MB2WC(pszFileNameWithoutExtension);
	if (NULL == pwszFileNameWithoutExtension)
	{
		return false;
	}

	m_astrFileName[eTable] = pszFileNameWithoutExtension;
	m_awstrFileName[eTable] = pwszFileNameWithoutExtension;

	Ntl_CleanUpHeapStringW(pwszFileNameWithoutExtension);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTableFileNameList::SetFileName(CTableContainer::eTABLE eTable, WCHAR* pwszFileNameWithoutExtension)
{
	if (CTableContainer::TABLE_COUNT <= eTable || NULL == pwszFileNameWithoutExtension)
	{
		//- yoshiki : To log system!
		return false;
	}

	char* pszFileNameWithoutExtension = Ntl_WC2MB(pwszFileNameWithoutExtension);
	if (NULL == pszFileNameWithoutExtension)
	{
		return false;
	}

	m_astrFileName[eTable] = pszFileNameWithoutExtension;
	m_awstrFileName[eTable] = pwszFileNameWithoutExtension;

	Ntl_CleanUpHeapString(pszFileNameWithoutExtension);

	return true;
}