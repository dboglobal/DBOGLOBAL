#pragma once
//created 11.12.2014
#include "Table.h"


#pragma pack(push, 4)
struct sAIR_COSTUME_TBLDAT : public sTBLDAT
{

public:

	WORD		wUnknown;//always 91

	BYTE		byUnknown2;//always 57
	BYTE		byUnknown3;//always 101
	BYTE		byUnknown4;//always 93

	WORD		wUnknown5;
	WORD		wUnknown6;
	WORD		wUnknown7;

	WCHAR		wszUnknown8[ 24 + 1 ];

	BYTE		byUnknown9;

	WCHAR		wszUnknown10[ 30 + 1 ];
	WCHAR		wszUnknown11[ 30 + 1 ];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CAirCostumeTable : public CTable
{
public:

	CAirCostumeTable(void);
	virtual ~CAirCostumeTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CAirCostumeTable::m_pwszSheetList[0]); }
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