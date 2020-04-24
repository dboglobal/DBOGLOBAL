//***********************************************************************************
//
//	File		:	ActionTable.h
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

const DWORD		DBO_MAX_LENGTH_ACTION_ICON_NAME = 32;

#pragma pack(push, 4)
struct sACTION_TBLDAT : public sTBLDAT
{
	sACTION_TBLDAT()
	{
		bValidity_Able = true;
	};

public:
	bool			bValidity_Able;
	BYTE			byAction_Type;
	TBLIDX			Action_Name;
	char			szIcon_Name[ DBO_MAX_LENGTH_ACTION_ICON_NAME + 1 ];
	TBLIDX			Note;
	TBLIDX			chat_Command_Index;
	BYTE			byETC_Action_Type;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

enum eACTION_TYPE
{
	ACTION_TYPE_NORMAL_ACTION = 1,
	ACTION_TYPE_SOCIAL_ACTION,

	ACTION_TYPE_COUNT,

	ACTION_TYPE_UNKNOWN = 0xFF,

	ACTION_TYPE_FIRST = ACTION_TYPE_NORMAL_ACTION,
	ACTION_TYPE_LAST = ACTION_TYPE_SOCIAL_ACTION
};
//-----------------------------------------------------------------------------------
// ACTION FUNC BIT FLAG
//-----------------------------------------------------------------------------------
enum eACTION_FUNC_TYPE
{
	ACTION_FUNC_TYPE_SCOUTER_USE = 0,
	ACTION_FUNC_TYPE_PC_SHOP_OPEN,
	ACTION_FUNC_TYPE_NORMAL_DICE_OPEN,
	ACTION_FUNC_TYPE_BASIC_ATTACK,
	ACTION_FUNC_TYPE_ASSIST,
	ACTION_FUNC_TYPE_GET_ITEM,
	ACTION_FUNC_TYPE_RELAX_ACTION,
};	

enum eACTION_FUNC_TYPE_BIT_FLAG
{
	ACTION_FUNC_TYPE_BIT_FLAG_SCOUTER_USE			= 0x01 << ACTION_FUNC_TYPE_SCOUTER_USE,
	ACTION_FUNC_TYPE_BIT_FLAG_PC_SHOP_OPEN			= 0x01 << ACTION_FUNC_TYPE_PC_SHOP_OPEN,
	ACTION_FUNC_TYPE_BIT_FLAG_NORMAL_DICE_OPEN		= 0x01 << ACTION_FUNC_TYPE_NORMAL_DICE_OPEN,
	ACTION_FUNC_TYPE_BIT_FLAG_BASIC_ATTACK			= 0x01 << ACTION_FUNC_TYPE_BASIC_ATTACK,
	ACTION_FUNC_TYPE_BIT_FLAG_ASSIST				= 0x01 << ACTION_FUNC_TYPE_ASSIST,
	ACTION_FUNC_TYPE_BIT_FLAG_GET_ITEM				= 0x01 << ACTION_FUNC_TYPE_GET_ITEM,
	ACTION_FUNC_TYPE_BIT_FLAG_RELAX_ACTION			= 0x01 << ACTION_FUNC_TYPE_RELAX_ACTION,
};

class CActionTable : public CTable
{
public:

	CActionTable(void);
	virtual ~CActionTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CActionTable::m_pwszSheetList[0]); }
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