
#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_VEHICLE_MODEL_NAME = 32;

#pragma pack(push, 4)
struct sVEHICLE_TBLDAT : public sTBLDAT
{
	sVEHICLE_TBLDAT()
	{
	};

public:
	char			szModelName[ DBO_MAX_LENGTH_VEHICLE_MODEL_NAME + 1 ];
	BYTE			bySRPType;
	BYTE			bySpeed;
	BYTE			byVehicleType;
	WORD			wRunHeight;
	BYTE			byPersonnel;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CVehicleTable : public CTable
{
public:
	CVehicleTable(void);
	virtual	~CVehicleTable(void);

	bool			Create(DWORD dwCodePage);
	void			Destroy();

protected:
	void			Init();

public:
	sTBLDAT*		FindData(TBLIDX tblidx); 

protected:
	WCHAR**			GetSheetListInWChar() { return &(CVehicleTable::m_pwszSheetList[0]); }
	void*			AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool			DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool			AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool			SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:
	virtual bool	LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);
	virtual bool	SaveToBinary(CNtlSerializer& serializer);

private:
	static WCHAR*	m_pwszSheetList[];
};