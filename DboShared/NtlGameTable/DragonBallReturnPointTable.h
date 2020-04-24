#pragma once
//created 5.5.2015
#include "Table.h"


#pragma pack(push, 4)
struct sDRAGONBALL_RETURN_POINT_TBLDAT : public sTBLDAT
{

public:

	BYTE	byScatterPoint;
	float	fField_X;
	float	fField_Y;
	float	fField_Z;


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CDragonBallReturnPointTable : public CTable
{
public:

	CDragonBallReturnPointTable(void);
	virtual ~CDragonBallReturnPointTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CDragonBallReturnPointTable::m_pwszSheetList[0]); }
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