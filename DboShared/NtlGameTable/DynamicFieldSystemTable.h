#pragma once
//created 17.10.2016
#include "Table.h"
#include "NtlEventDef.h"

#pragma pack(push, 4)
struct sDYNAMIC_FIELD_SYSTEM_TBLDAT : public sTBLDAT
{

public:

	WCHAR				wszName[32 + 1];
	bool				bOnOff;
	BYTE				byServerFarm;
	DWORD				dwConnectionTime;
	BYTE				byType;
	TBLIDX				tIndex;
	DWORD				dwContentRestrictionBitFlag;
	DWORD				adwSetting[3];
	float				fRate;
	BYTE				byAction;
	TBLIDX				aIndex;
	BYTE				byGroup;
	DWORD				dwVolume;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)


typedef std::multimap<BYTE, sDYNAMIC_FIELD_SYSTEM_TBLDAT*> DYNAMIC_FIELD_SYSTEM_MAP;
typedef DYNAMIC_FIELD_SYSTEM_MAP::value_type DYNAMIC_FIELD_SYSTEM_VAL;


class CDynamicFieldSystemTable : public CTable
{
public:

	CDynamicFieldSystemTable(void);
	virtual ~CDynamicFieldSystemTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CDynamicFieldSystemTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


public:

	bool								IsOn(eEVENT_SYSTEM_TYPE eEventSystemType);
	DYNAMIC_FIELD_SYSTEM_MAP::iterator	DynamicFieldBegin();
	DYNAMIC_FIELD_SYSTEM_MAP::iterator	DynamicFieldEnd();
	bool								IsThisDynamicFieldMob(TBLIDX tblidx);


private:

	bool						m_abOnOff[EVENT_SYSTEM_TYPE_COUNT];

	DYNAMIC_FIELD_SYSTEM_MAP	m_mapDynamicFieldSystem;



private:

	static WCHAR* m_pwszSheetList[];


};
