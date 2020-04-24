#pragma once
//***********************************************************************************
//
//	File		:	NewbieTableCheck.h
//
//	Begin		:	2006-03-30
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//************************************************************************************

#include "CheckTable.h"
#include "NtlVector.h"
#include "NtlCharacter.h"

struct sNEWBIE_CHECK_TBLDAT : public sTBLDATCHECK
{
public:
	
	BYTE			byOnly_N;
	BYTE			byInvalid_Check;
	BYTE			byN_From_N;
	int				nMin;
	int				nMax;
	BYTE			byN_Above;
	int				nAbove;
	BYTE			byN_Below;
	int				nBelow;
	BYTE			byString_Below;
	int				nString;
	BYTE			byIn_Tblidx;
	std::wstring	wstrTable;
	BYTE			byLook_Up_Level;
	std::wstring	wstrTable_Level;
	BYTE			byLook_Up_Field;
	std::wstring    wstrField;
	std::wstring    wstrTable_Field;
	
};


class CNewbieTableCheck :
	public CCheckTable
{
public:
	CNewbieTableCheck(void);
public:
	virtual ~CNewbieTableCheck(void);

	bool Create(WCHAR* pwszFileName, DWORD dwCodePage);
	bool Create(char* pszFileName, DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDATCHECK *					FindData(std::wstring wstrField); 

	
protected:
	WCHAR** GetSheetListInWChar() { return &(CNewbieTableCheck::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void* pvTable, WCHAR* pwszSheetName);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

protected:
	
private:
	static WCHAR* m_pwszSheetList[];
};
