//***********************************************************************************
//
//	File		:	TableFileNameList.h
//
//	Begin		:	2007-02-02
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "TableContainer.h"

class CTableFileNameList
{
public:
	CTableFileNameList(void);

	virtual ~CTableFileNameList(void);


public:

	bool							Create();

	void							Destroy();


protected:

	void							Init();

public:

	char*							GetFileName(CTableContainer::eTABLE eTable);

	WCHAR*							GetFileNameW(CTableContainer::eTABLE eTable);

	bool							SetFileName(CTableContainer::eTABLE eTable, char* pszFileNameWithoutExtension);

	bool							SetFileName(CTableContainer::eTABLE eTable, WCHAR* pwszFileNameWithoutExtension);


private:

	// We use a file name without a file extension.
	// 화일 확장자가 포함되지 않은 화일 이름을 사용한다.
	// by YOSHIKI(2007-02-02)
	std::string							m_astrFileName[CTableContainer::TABLE_COUNT];

	std::wstring						m_awstrFileName[CTableContainer::TABLE_COUNT];
};