#pragma once
//created 04.03.2014
#include "Table.h"


#pragma pack(push, 4)
struct sNPC_SERVER_TBLDAT : public sTBLDAT
{
//	sNPC_SERVER_TBLDAT()
//	{
//		bValidity_Able = true;
//	};

public:

	bool	bValidity_Able;
	DWORD	dwServerBitFlag;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CNpcServerTable : public CTable
{
public:

	CNpcServerTable(void);
	virtual ~CNpcServerTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CNpcServerTable::m_pwszSheetList[0]); }
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