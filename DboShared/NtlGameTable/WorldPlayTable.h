//***********************************************************************************
//
//	File		:	WorldPlayTable.h
//
//	Begin		:	2008-03-03
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Lee Ju-hyung
//
//	Desc		:	
//
//***********************************************************************************

#pragma once
#include "Table.h"
#include "NtlWorld.h"

//const DWORD NTL_MAX_LENGTH_WORLD_PLAY_DESC = 255;

#pragma pack(push, 4)
struct sWORLDPLAY_TBLDAT : public sTBLDAT
{
public:
	
	DWORD					dwGroup;
	BYTE					byExecuterType;		// eWORLDPLAY_EXECUTER_TYPE
	BYTE					byShareType;		// eWORLDPLAY_SHARE_TYPE
	DWORD					dwShareLimitTime;	// Share Limit Time



protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CWorldPlayTable : public CTable
{
public:
	CWorldPlayTable(void);
	virtual ~CWorldPlayTable(void);

public:

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 


protected:
	WCHAR** GetSheetListInWChar() { return &(CWorldPlayTable::m_pwszSheetList[0]); }
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
