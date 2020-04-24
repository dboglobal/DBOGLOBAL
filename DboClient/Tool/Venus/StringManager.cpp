//***********************************************************************************
//	File		:	StringManager.cpp
//	Desc		:	
//	Begin		:	2005. 1.20
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\stringmanager.h"

#include <iostream>
#include <fstream>


CStringManager::CStringManager(void)
{
	m_nNational	= STRING_NATIONAL_USA;
}

CStringManager::~CStringManager(void)
{
}

CStringManager& CStringManager::GetInstance()
{
	static CStringManager manager;
	return manager;
}


//---------------------------------------------------------------------------
//Name		: DeleteData
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CStringManager::Delete()
{
	for (int i = 0; i < STRING_NATIONAL_MAX_COUNT; ++ i)
	{
		for (int j = 0; j < STRING_MAX_COUNT; ++ j)
		{
			m_strData[i][j].erase();
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
//Name		: Load
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CStringManager::Create(const RwChar* strFileName)
{
	std::ifstream file(strFileName, std::ios::in | std::ios::binary);
	if(!file.is_open())
	{
		return FALSE;
	}

	for (int i = 0; i < STRING_MAX_COUNT; ++ i)
	{
		for (int j = 0; j < STRING_NATIONAL_MAX_COUNT; ++ j)
		{
			LoadString(file, m_strData[j][i]);
		}
	}

	file.close();

	return TRUE;
}

//---------------------------------------------------------------------------
//Name		: LoadString
//Desc		: 
//Return	: 
//Parameter	: 
//---------------------------------------------------------------------------
RwBool CStringManager::LoadString(std::ifstream& inFile, std::string& strData, RwChar cDeli)
{
	strData.erase();

	char c;
	if( !( inFile >> c ) ) return FALSE;

	if( '@' == c ) return TRUE;

	//; cDeli 로 시작해야 한다
	if( c != cDeli ) return FALSE;

	//; 문자를 읽어 들인다
	while( inFile.get( c ) )
	{
		//; cDeli로 끝나야한다
		if( cDeli == c ) return TRUE;
		strData += c;
	}

	return FALSE;
}