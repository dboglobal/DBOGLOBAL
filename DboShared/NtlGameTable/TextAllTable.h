//***********************************************************************************
//
//	File		:	TextAll.h
//
//	Begin		:	2006-12-26
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Sang Jun, Pee ( peessi@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"

#pragma pack(push, 4)
struct sTEXT_TBLDAT : public sTBLDAT
{
	sTEXT_TBLDAT()
	{
		wstrText = L"";
	}

	std::wstring wstrText;
	
};
#pragma pack(pop)

class CTextTable : public CTable
{
public:
	enum TYPE { INDEX, TEXT };

	CTextTable(VOID);
	virtual ~CTextTable(VOID);

	BOOL	Create( DWORD dwCodePage, INT nField, INT nType );
	VOID	Destroy(VOID);

protected:
	VOID	Init(VOID);

public:
	sTBLDAT*		FindData( TBLIDX tblidx );
	std::wstring&	GetText( TBLIDX tblidx );
	BOOL			GetText( TBLIDX tblidx, std::wstring* pwstr );

	bool			AddTable(VOID* pvTable, bool bReload, bool bUpdate);

protected:
	WCHAR**	GetSheetListInWChar(VOID) { return &(CTextTable::m_pwszSheetList[0]); }
	VOID*	AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );
	bool	DeallocNewTable( VOID* pvTable, WCHAR* pwszSheetName );
	bool	SetTableData( VOID* pvTable, WCHAR* pwszSheetName, TYPE eType, BSTR bstrData );
	bool	SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	
	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


protected:

	virtual bool				InitializeFromXmlDoc(CNtlXMLDoc* pXmlDoc, WCHAR* pwszFileName, bool bReload, bool bUpdate);


private:
	static	WCHAR* m_pwszSheetList[];

	INT		m_nField;
	INT		m_nType;
};

class CMapNameTextTable : public CTextTable
{
public:
	BOOL	GetZoneName( TBLIDX tblidx, std::wstring* pwstr );
	BOOL	GetSectorName( TBLIDX tblidx, std::wstring* pwstr );
	BOOL	GetAreaName( TBLIDX tblidx, std::wstring* pwstr );
	std::wstring GetZoneName( TBLIDX tblidx );
	std::wstring GetSectorName( TBLIDX tblidx );
	std::wstring GetAreaName( TBLIDX tblidx );

	VOID GetErrorText( TBLIDX tblidx, std::wstring* pwString );
};

class CTextAllTable
{
public:
	enum TABLETYPE
	{
		ACTION_DATA,
		CHAT_COMMMAND_DATA,
		HTB_SET_DATA,
		ITEM_DATA,
		MERCHANT_DATA,
		MOB_DATA,
		NPC_DATA,
		SKILL_DATA,
		SYSTEM_EFFECT,
		USE_ITEM_DATA,
		MAP_NAME,
		OBJECT_DATA,
		QUEST_ITEM_DATA,
		ETC_TEXT,
		HELP_DATA,
		HELP_POPOHINT_DATA,
		DRAGON_BALL,
		DB_REWARD,
		TMQ_DATA,
		CS_TEXT,
		MILEPOST,
		FILTERING,
		NPC_DIALOG,
		GM_TOOL,
		DBO_TIP,

		DWC_DATA,
		CHAR_TITLE_DATA,
		PARTY_DUNGEON_DATA,

		TABLE_COUNT
	};

	CTextAllTable(VOID);
	~CTextAllTable(VOID);

	BOOL	Create( DWORD dwCodePage );
	VOID	Destroy(VOID);
	
	CTextTable*	GetTextTbl( TABLETYPE eType ) { return m_pTextTable[eType]; }

	CTextTable* GetActionTbl(VOID) { return m_pTextTable[ACTION_DATA]; }
	CTextTable* GetChatCommandTbl(VOID) { return m_pTextTable[CHAT_COMMMAND_DATA]; }
	CTextTable* GetHTBSetTbl(VOID) { return m_pTextTable[HTB_SET_DATA]; }
	CTextTable* GetItemTbl(VOID) { return m_pTextTable[ITEM_DATA]; }
	CTextTable* GetMerchantTbl(VOID) { return m_pTextTable[MERCHANT_DATA]; }
	CTextTable* GetMobTbl(VOID) { return m_pTextTable[MOB_DATA]; }
	CTextTable* GetNPCTbl(VOID) { return m_pTextTable[NPC_DATA]; }
	CTextTable* GetSkillTbl(VOID) { return m_pTextTable[SKILL_DATA]; }
	CTextTable* GetSystemEffectTbl(VOID) { return m_pTextTable[SYSTEM_EFFECT]; }
	CTextTable* GetUseItemTbl(VOID) { return m_pTextTable[USE_ITEM_DATA]; }
	CMapNameTextTable* GetMapNameTbl(VOID) { return reinterpret_cast<CMapNameTextTable*>( m_pTextTable[MAP_NAME] ); }
	CTextTable* GetObjectTbl(VOID) { return m_pTextTable[OBJECT_DATA]; }
	CTextTable* GetQuestItemTbl(VOID) { return m_pTextTable[QUEST_ITEM_DATA]; }
	CTextTable* GetETCTbl(VOID) { return m_pTextTable[ETC_TEXT]; }
	CTextTable* GetHelpTbl(VOID) { return m_pTextTable[HELP_DATA]; }
	CTextTable* GetHelpPopoTbl(VOID) { return m_pTextTable[HELP_POPOHINT_DATA]; }
	CTextTable* GetDragonTbl(VOID) { return m_pTextTable[DRAGON_BALL]; }
	CTextTable* GetDBRewardTbl(VOID) { return m_pTextTable[DB_REWARD]; }
	CTextTable* GetTMQTbl(VOID) { return m_pTextTable[TMQ_DATA]; }
	CTextTable* GetCSTextTble(VOID) { return m_pTextTable[CS_TEXT];}
	CTextTable* GetMilePostTable(VOID) { return m_pTextTable[MILEPOST]; }
	CTextTable* GetFilteringTable(VOID) { return m_pTextTable[FILTERING]; }
	CTextTable* GetNpcDialogTbl(VOID) { return m_pTextTable[NPC_DIALOG]; }
	CTextTable* GetGMToolTbl(VOID) { return m_pTextTable[GM_TOOL]; }
	CTextTable* GetDBOTipTbl(VOID) { return m_pTextTable[DBO_TIP]; }
	//new
	CTextTable* GetDWCTbl(VOID) { return m_pTextTable[DWC_DATA]; }
	CTextTable* GetCharTitleTbl(VOID) { return m_pTextTable[CHAR_TITLE_DATA]; }
	CTextTable* GetPartyDungeonTbl(VOID) { return m_pTextTable[PARTY_DUNGEON_DATA]; }

public:

	void						Reset();

	bool						LoadFromXml(char* pszFileName);

	bool						LoadFromXml(WCHAR* pwszFileName);

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	INT							TypeToField( INT eType );

	CTextTable*					m_pTextTable[TABLE_COUNT];

	DWORD						m_dwCodePage;

	eTABLE_LOCAL_LOAD_TYPE		m_eTableLoadType;


public:

	eTABLE_LOCAL_LOAD_TYPE		GetTableLoadType() { return m_eTableLoadType; }
	void						SetTableLoadType(eTABLE_LOCAL_LOAD_TYPE eTableLoadType) { m_eTableLoadType = eTableLoadType; }

};