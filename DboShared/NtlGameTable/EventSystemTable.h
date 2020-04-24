#pragma once
//created 17.10.2016
#include "Table.h"
#include "NtlEventDef.h"


#pragma pack(push, 4)
struct sEVENT_SYSTEM_TBLDAT : public sTBLDAT
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
	DWORD				dwUnknown;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)


typedef std::multimap<BYTE, sEVENT_SYSTEM_TBLDAT*> EVENT_SYSTEM_MAP;
typedef EVENT_SYSTEM_MAP::value_type EVENT_SYSTEM_VAL;


class CEventSystemTable : public CTable
{
public:

	CEventSystemTable(void);
	virtual ~CEventSystemTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CEventSystemTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


public:

	bool						IsOn(eEVENT_SYSTEM_TYPE eEventSystemType);
	EVENT_SYSTEM_MAP::iterator	EventBegin();
	EVENT_SYSTEM_MAP::iterator	EventEnd();
	bool						IsThisEventMob(TBLIDX tblidx);


private:

	bool						m_abOnOff[EVENT_SYSTEM_TYPE_COUNT];

	EVENT_SYSTEM_MAP			m_mapEventSystem;



private:

	static WCHAR* m_pwszSheetList[];


};
