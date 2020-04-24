#pragma once
//last update 1.12.2014
#include "table.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sDUNGEON_TBLDAT : public sTBLDAT
{
public:

	sDUNGEON_TBLDAT( void ) { }

public:

	BYTE			byDungeonType;			// 1:normal 2:hard
	BYTE			byMaxMember;
	TBLIDX			linkWorld;
	BYTE			byMinLevel;
	BYTE			byMaxLevel;
	TBLIDX			needItemTblidx;
	DWORD			dwHonorPoint;
	TBLIDX			wpsTblidx;
	TBLIDX			openCine;

	GROUPID			groupIdx;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CDungeonTable : public CTable
{
public:

	CDungeonTable( void );

	virtual ~CDungeonTable( void );

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx); 

	sDUNGEON_TBLDAT*		FindDataByGroupIdx(GROUPID groupIdx, BYTE byDifficulty);

protected:

	void					Init( void );

	WCHAR**					GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }

	void*					AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );

	bool					DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );

	bool					AddTable( void* pvTable, bool bReload, bool bUpdate );

	bool					SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );


protected:

	static WCHAR*			m_pwszSheetList[];

};
