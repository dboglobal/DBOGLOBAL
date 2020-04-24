#pragma once
//last update 1.12.2014
#include "Table.h"

#include "NtlVector.h"
#include "NtlSkill.h"

#include <vector>

const DWORD		DBO_MAX_LENGTH_SKILL_NAME_TEXT = 40;
const DWORD		DBO_MAX_LENGTH_SKILL_ICON_NAME = 32;

#pragma pack(push, 4)
struct sSKILL_TBLDAT : public sTBLDAT
{

public:
	TBLIDX			Skill_Name;
	WCHAR			wszNameText[ DBO_MAX_LENGTH_SKILL_NAME_TEXT + 1 ];
	bool			bValidity_Able;

	DWORD			dwPC_Class_Bit_Flag;		// ePC_CLASS_FLAG

	BYTE			byClass_Type;

	BYTE			bySkill_Class;				// eSKILL_CLASS
	BYTE			bySkill_Type;				// eSKILL_TYPE
	BYTE			bySkill_Active_Type;		// eSKILL_ACTIVE_TYPE
	BYTE			byBuff_Group;
	BYTE			bySlot_Index;
	BYTE			bySkill_Grade;
	DWORD			dwFunction_Bit_Flag;

	BYTE			byAppoint_Target;
	BYTE			byApply_Target;
	BYTE			byApply_Target_Max;
	BYTE			byApply_Range;
	BYTE			byApply_Area_Size_1;
	BYTE			byApply_Area_Size_2;

	TBLIDX			skill_Effect[NTL_MAX_EFFECT_IN_SKILL];
	BYTE			bySkill_Effect_Type[NTL_MAX_EFFECT_IN_SKILL];
	double			aSkill_Effect_Value[NTL_MAX_EFFECT_IN_SKILL];
	DWORD			dwAdditional_Aggro_Point;

	BYTE			abyRpEffect[DBO_MAX_RP_BONUS_COUNT_PER_SKILL];		// eDBO_RP_BONUS_SLOT
	float			afRpEffectValue[DBO_MAX_RP_BONUS_COUNT_PER_SKILL];

	BYTE			byRequire_Train_Level;
	DWORD			dwRequire_Zenny;
	WORD			wNext_Skill_Train_Exp;
	WORD			wRequireSP;
	bool			bSelfTrain;
	TBLIDX			uiRequire_Skill_Tblidx_Min_1;
	TBLIDX			uiRequire_Skill_Tblidx_Max_1;
	TBLIDX			uiRequire_Skill_Tblidx_Min_2;
	TBLIDX			uiRequire_Skill_Tblidx_Max_2;
	TBLIDX			Root_Skill;

	BYTE			byRequire_Epuip_Slot_Type;
	BYTE			byRequire_Item_Type;
	char			szIcon_Name[ DBO_MAX_LENGTH_SKILL_ICON_NAME + 1 ];
	DWORD			dwRequire_LP;
	WORD			wRequire_EP;
	BYTE			byRequire_RP_Ball;
	float			fCasting_Time;
	DWORD			dwCastingTimeInMilliSecs;
	WORD			wCool_Time;
	DWORD			dwCoolTimeInMilliSecs;
	WORD			wKeep_Time;
	DWORD			dwKeepTimeInMilliSecs;
	bool			bKeep_Effect;
	BYTE			byUse_Range_Min;
	float			fUse_Range_Min;
	BYTE			byUse_Range_Max;
	float			fUse_Range_Max;
	TBLIDX			Note;

	DWORD			dwNextSkillTblidx;
	bool			bDefaultDisplayOff;

	DWORD			dwAnimation_Time;

	ANIMATIONID		wCasting_Animation_Start;
	ANIMATIONID		wCasting_Animation_Loop;
	ANIMATIONID		wAction_Animation_Index;
	ANIMATIONID		wAction_Loop_Animation_Index;
	ANIMATIONID		wAction_End_Animation_Index;
	bool			bDash_Able;

	DWORD			dwTransform_Use_Info_Bit_Flag;
	float			fSuccess_Rate;
	BYTE			byPC_Class_Change;
	BYTE			byUse_Type; 

	BYTE			bySkill_Group;
	DWORD			dwRequire_VP;
	DWORD			dwUse_Restriction_Rule_Bit_Flag;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)


class CSkillTable : public CTable
{
public:

	CSkillTable(void);
	virtual ~CSkillTable(void);

	/*
	public:

	BOOL				Check() { return TRUE; }*/
public:

	typedef std::map<TBLIDX, TBLIDX> PRE_TABLE;
	typedef PRE_TABLE::iterator PRE_TABLE_IT;
	typedef PRE_TABLE::value_type PRE_TABLE_VAL;

public:
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	TBLIDX 				FindPreSkillTblidx(TBLIDX tblidx); 
	TBLIDX 				FindBasicSkillTblidx(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CSkillTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);

protected:

	PRE_TABLE					m_mapPreTableList;

private:
	static WCHAR* m_pwszSheetList[];

	std::vector<sSKILL_TBLDAT*> m_mapBasicMascotSkills;

public:

	std::vector<sSKILL_TBLDAT*>::iterator GetMascotSkillsVectorBegin() { return m_mapBasicMascotSkills.begin(); }
	std::vector<sSKILL_TBLDAT*>::iterator GetMascotSkillsVectorEnd() { return m_mapBasicMascotSkills.end(); }

	std::vector<sSKILL_TBLDAT*> GetMascotSkillsVector() { return m_mapBasicMascotSkills; }
};
