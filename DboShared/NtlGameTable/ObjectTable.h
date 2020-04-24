#pragma once
#include "table.h"
#include "NtlVector.h"
#include "NtlQuest.h"
//updated 1.12.2014
#include "NtlSharedDef.h"

const DWORD		DBO_MAX_LENGTH_CLICK_SOUND_NAME = 64;
// Object
#pragma pack(push, 4)
struct sOBJECT_TBLDAT : public sTBLDAT
{
public:

	sOBJECT_TBLDAT( void )
	{
		byStateType = INVALID_TOBJECT_STATE_TYPE;
		byDefMainState = TOBJECT_STATE_NONE;
		byDefSubState = TOBJECT_SUBSTATE_SHOW;
		memset( abyState, 0xff, sizeof( abyState ) );
		contentsTblidx = INVALID_TBLIDX;
	}

public:

	DWORD			dwName;
	DWORD			dwFunction;

	CNtlVector		vMin;
	CNtlVector		vMax;
	CNtlVector		vLoc;
	CNtlVector		vDir;

	BYTE			byStateType;
	BYTE			byDefMainState;
	BYTE			byDefSubState;
	BYTE			abyState[DBO_MAX_OBJECT_STATE][DBO_MAX_OBJECT_STATE_ANIMATION];
	char			achClickSound[DBO_MAX_OBJECT_STATE][DBO_MAX_LENGTH_CLICK_SOUND_NAME];

	char			szModelName[ NTL_MAX_LENGTH_MODEL_NAME + 1 ];

	float			fRadius;
	DWORD			dwSequence; // Order to calculate the value at the time of loading (using the handle value of the shared client and server)

	TBLIDX			contentsTblidx;
	TBLIDX			objectDirectionIndex;
	BYTE			byBoundaryDistance;

	QUESTID			minQuestId;
	QUESTID			maxQuestId;

	BYTE			byWarfog;
	TBLIDX			ZoneId; 
	TBLIDX			WorldId; 


public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CObjectTable : public CTable
{
// Member variables
protected:
	static WCHAR* m_pwszSheetList[];

// Constructions and Destructions
public:
	CObjectTable( void );
	virtual ~CObjectTable( void );

// Methods
public:
	bool			Create( DWORD dwCodePage );
	void			Destroy( void );

	sTBLDAT*		FindData(TBLIDX tblidx); 

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

private:

	DWORD			m_dwObjectSequence;
};