//***********************************************************************************
//	File		:	StringManager.h
//	Desc		:	
//	Begin		:	2005. 1.20
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "rwcore.h"
#include <string>
#include "NtlDebug.h"

// 게임 문자열 종류
#define STRING_DATA(p)		p,

enum STRING_TYPE
{
#include "stringType.txt"
};

enum STRING_NATIONAL
{
	STRING_NATIONAL_KOREA = 0x00,
	STRING_NATIONAL_USA,

	STRING_NATIONAL_MAX_COUNT
};


class CStringManager
{
public:
	RwInt32					m_nNational;

	std::string				m_strData[STRING_NATIONAL_MAX_COUNT][STRING_MAX_COUNT];


public:
	CStringManager(void);
	~CStringManager(void);

	static	CStringManager& GetInstance();

	RwBool	Delete();
	RwBool	Create(const RwChar* strFileName);

	RwBool	LoadString(std::ifstream& inFile, std::string& strData, RwChar cDeli = '^');

public:
	const RwChar*	GetString(int type)
	{
		NTL_ASSERTE(!m_strData[m_nNational][type].empty());
		return m_strData[m_nNational][type].c_str();
	}
};

