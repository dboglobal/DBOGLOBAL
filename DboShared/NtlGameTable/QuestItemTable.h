#pragma once
#include "table.h"

const DWORD		DBO_MAX_LENGTH_QUEST_ITEM_ICON_NAME = 32;

//-----------------------------------------------------------------------------------
// Quest item Function Type
//-----------------------------------------------------------------------------------
enum eQUEST_ITEM_FUNC_TYPE
{
	QUEST_ITEM_FUNC_TYPE_DEL_FORBIDDEN,
};


// Object
#pragma pack(push, 4)
struct sQUESTITEM_TBLDAT : public sTBLDAT
{
public:
	TBLIDX			ItemName;		// 게임상에 표시될 Item의 이름
	char			szIconName[ DBO_MAX_LENGTH_QUEST_ITEM_ICON_NAME + 1 ];		// Icon의 File 이름
	TBLIDX			Note;		// Quest item의 설명
	BYTE			byFunctionBitFlag;
protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestItemTable : public CTable
{
// Member variables
protected:
	static WCHAR* m_pwszSheetList[];

// Constructions and Destructions
public:
	CQuestItemTable( void );
	virtual ~CQuestItemTable( void );

// Methods
public:
	bool			Create( DWORD dwCodePage );
	void			Destroy( void );

	sTBLDAT*		FindData( TBLIDX tblidx ); 

// Implementations
protected:
	void			Init( void );
	WCHAR**			GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }
	void*			AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );
	bool			DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );
	bool			AddTable( void* pvTable, bool bReload, bool bUpdate );
	bool			SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);
};
