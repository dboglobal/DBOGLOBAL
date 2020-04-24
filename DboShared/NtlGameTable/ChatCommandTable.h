//***********************************************************************************
//
//	File		:	CChatCommandTable.h
//
//	Begin		:	2006-08-31
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"

const DWORD NTL_MAX_CHAT_COMMAND = 8;

#pragma pack(push, 4)
struct sCHAT_COMMAND_TBLDAT : public sTBLDAT
{
	sCHAT_COMMAND_TBLDAT()
	{
		bValidity_Able = true;
	};

public:
	bool			bValidity_Able;
	WORD			wAction_Animation_Index;
	TBLIDX			aChat_Command[NTL_MAX_CHAT_COMMAND];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CChatCommandTable : public CTable
{
public:

	CChatCommandTable(void);
	virtual ~CChatCommandTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CChatCommandTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];
};