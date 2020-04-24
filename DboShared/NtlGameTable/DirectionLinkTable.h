#pragma once
//last update 1.12.2014
#include "table.h"
#include "NtlSharedDef.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const unsigned int DBO_MAX_LENGTH_DIRECTION_FUNCTION_NAME = 256;

const unsigned int DBO_MAX_LENGTH_DIRECTION_NOTE = 32;
//-----------------------------------------------------------------------------------

enum eDIRECTION_FUNC_FLAG
{
	DIRECTION_FUNC_FLAG_LOOP		= 0x01, // 애니메이션의 루프 여부 ( Client상에서는 계속 반복된다 )
	DIRECTION_FUNC_FLAG_TIMEOUT		= 0x02, // 애니메이션의 시간만큼 대기한다 
	DIRECTION_FUNC_FLAG_KEEPUP		= 0x04, // 애니메이션의 시간에 관계없이 연출상태를 무한히 지속한다
	DIRECTION_FUNC_FLAG_CANCELABLE	= 0x08, // Client Input/TS Event/피격 등에 의해 취소된다
};

enum eDIRECTION_LINK_TYPE
{
	DIRECTION_LINK_TYPE_LUA_SCRIPT = 0,		
	DIRECTION_LINK_TYPE_ANIMATION_NUMBER,
	DIRECTION_LINK_TYPE_FLASH,

	DIRECTION_LINK_TYPE_UNKNOWN = 0xFF,

	DIRECTION_LINK_TYPE_FIRST = DIRECTION_LINK_TYPE_LUA_SCRIPT,
	DIRECTION_LINK_TYPE_LAST = DIRECTION_LINK_TYPE_ANIMATION_NUMBER
};

enum eDIRECTION_LINK_LOOP_ENABLE
{
	DIRECTION_LINK_LOOP_ENABLE_FLOW = 0,		
	DIRECTION_LINK_LOOP_ENABLE_LOOP,			

	DIRECTION_LINK_LOOP_ENABLE_UNKNOWN = 0xFF,

	DIRECTION_LINK_LOOP_ENABLE_FIRST = DIRECTION_LINK_LOOP_ENABLE_FLOW,
	DIRECTION_LINK_LOOP_ENABLE_LAST = DIRECTION_LINK_LOOP_ENABLE_LOOP
};
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sDIRECTION_LINK_TBLDAT : public sTBLDAT
{
	sDIRECTION_LINK_TBLDAT()
	{
		ZeroMemory( szFunctionName, sizeof( szFunctionName ) );
		ZeroMemory( szNote, sizeof( szNote ) );
	}

	char			szFunctionName[ DBO_MAX_LENGTH_DIRECTION_FUNCTION_NAME + 1 ];
	char			szNote[ DBO_MAX_LENGTH_DIRECTION_NOTE + 1 ];
	BYTE			byType;
	DWORD			dwAnimationID;
	BYTE			byFuncFlag;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CDirectionLinkTable : public CTable
{
public:

	CDirectionLinkTable( void );

	virtual ~CDirectionLinkTable( void );

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx); 

	bool					CheckData(sDIRECTION_LINK_TBLDAT * pTbldat);

protected:

	void					Init( void );

	WCHAR**					GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }

	void*					AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );

	bool					DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );

	bool					AddTable( void* pvTable , bool bReload, bool bUpdate );

	bool					SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );


protected:

	static WCHAR*			m_pwszSheetList[];

};