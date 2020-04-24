#pragma once
//created 1.12.2014
#include "Table.h"

#include "NtlCharTitle.h"

const DWORD		DBO_MAX_LENGTH_TITLE_BONE_NAME = 0x200;
const DWORD		DBO_MAX_LENGTH_TITLE_EFFECT_NAME = 0x200;
const DWORD		DBO_MAX_LENGTH_TITLE_SOUND_NAME = 0x200;

#pragma pack(push, 4)
struct sCHARTITLE_TBLDAT : public sTBLDAT
{

public:

	TBLIDX		tblNameIndex;
	BYTE		byContentsType;
	BYTE		byRepresentationType;

	WCHAR		wszBoneName[ DBO_MAX_LENGTH_TITLE_BONE_NAME + 1 ];
	WCHAR		wszEffectName[ DBO_MAX_LENGTH_TITLE_EFFECT_NAME + 1 ];
	WCHAR		wszEffectSound[ DBO_MAX_LENGTH_TITLE_SOUND_NAME + 1 ];

	TBLIDX			atblSystem_Effect_Index[NTL_MAX_CHAR_TITLE_EFFECT];
	BYTE			abySystem_Effect_Type[NTL_MAX_CHAR_TITLE_EFFECT]; 
	double			abySystem_Effect_Value[NTL_MAX_CHAR_TITLE_EFFECT];

	
protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CCharTitleTable : public CTable
{
public:
	CCharTitleTable(void);
	virtual ~CCharTitleTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CCharTitleTable::m_pwszSheetList[0]); }
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