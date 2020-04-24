//***********************************************************************************
//
//	File		:	WorldZoneTable.h
//
//	Begin		:	2007-08-14
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_WORLD_ZONE_NAME = 40;

#pragma pack(push, 4)
struct sWORLD_ZONE_TBLDAT : public sTBLDAT
{
public:
	WORD			wFunctionBitFlag;
	TBLIDX			worldTblidx;
	TBLIDX			nameTblidx;
	WCHAR			wszName_Text[ DBO_MAX_LENGTH_WORLD_ZONE_NAME + 1 ];
	bool			bForbidden_Vehicle;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CWorldZoneTable : public CTable
{
public:

	CWorldZoneTable(void);

	virtual ~CWorldZoneTable(void);


public:

	bool						Create(DWORD dwCodePage);

	void						Destroy();


protected:

	void						Init();


public:

	sTBLDAT*					FindData(TBLIDX tblidx); 


protected:

	WCHAR**						GetSheetListInWChar() { return &(CWorldZoneTable::m_pwszSheetList[0]); }

	void*						AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);

	bool						DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);

	bool						AddTable(void * pvTable, bool bReload, bool bUpdate);

	bool						SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:

	static WCHAR*				m_pwszSheetList[];
};
