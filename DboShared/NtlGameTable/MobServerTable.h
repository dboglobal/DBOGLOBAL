#pragma once
//created 04.03.2014
#include "Table.h"


#pragma pack(push, 4)
struct sMOB_SERVER_TBLDAT : public sTBLDAT
{

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

class CMobServerTable : public CTable
{
public:

	CMobServerTable(void);
	virtual ~CMobServerTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CMobServerTable::m_pwszSheetList[0]); }
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