#pragma once
//last update 1.12.2014
#include "Table.h"
#include "NtlSkill.h"

const DWORD		DBO_MAX_LENGTH_HTB_SET_NAME_TEXT = 40;
const DWORD		DBO_MAX_LENGTH_HTB_SET_ICON_NAME = 32;

// HTB 동작 정보
#pragma pack(push, 4)
struct sHTB_ACTION
{
	BYTE				bySkillType;
	TBLIDX				skillTblidx;
};
#pragma pack(pop)

// HTB 스킬모음
#pragma pack(push, 4)
struct sHTB_SET_TBLDAT : public sTBLDAT
{
	sHTB_SET_TBLDAT()
	{
		bValidity_Able = true;
	};

public:
	WCHAR				wszNameText[ DBO_MAX_LENGTH_HTB_SET_NAME_TEXT + 1 ];
	TBLIDX				HTB_Skill_Name;
	bool				bValidity_Able;
	DWORD				dwPC_Class_Bit_Flag;// // enum ePC_CLASS_FLAG
	BYTE				bySlot_Index;
	BYTE				bySkill_Grade;
	char				szIcon_Name[ DBO_MAX_LENGTH_HTB_SET_ICON_NAME + 1 ];
	WORD				wNeed_EP;
	BYTE				byRequire_Train_Level;
	DWORD				dwRequire_Zenny;
	WORD				wNext_Skill_Train_Exp;
	WORD				wCool_Time;
	DWORD				dwCoolTimeInMilliSecs;
	TBLIDX				Note;
	BYTE				bySetCount;
	BYTE				byStop_Point;

	sHTB_ACTION			aHTBAction[ NTL_HTB_MAX_SKILL_COUNT_IN_SET ];
	WORD				wRequireSP;
protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CHTBSetTable : public CTable
{
public:

	CHTBSetTable(void);
	virtual ~CHTBSetTable(void);

/*
public:

	BOOL				Check() { return TRUE; }*/

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CHTBSetTable::m_pwszSheetList[0]); }
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
