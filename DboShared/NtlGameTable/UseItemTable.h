#pragma once
//last update 1.12.2014
#include "Table.h"
#include "NtlItem.h"

const DWORD		DBO_MAX_LENGTH_USE_ITEM_CASTING_EFFECT_NAME = 32;
const DWORD		DBO_MAX_LENGTH_USE_ITEM_ACTION_EFFECT_NAME = 32;

enum eUSE_ITEM_FUNCTION
{
	USE_ITEM_FUNCTION_NON_CONSUMABLE = 0,
	USE_ITEM_FUNCTION_SUB_BUFF_SYSTEM,
	USE_ITEM_FUNCTION_IRREMOVABLE_BUFF,
	USE_ITEM_FUNCTION_INFINITE_DURATION,
	USE_ITEM_FUNCTION_INVISIBLE_ICON,
	USE_ITEM_FUNCTION_ALWAYS_SUCCESS,
	USE_ITEM_FUNCTION_LOCK_NPC_USING_QUESTITEM,
	USE_ITEM_FUNCTION_COUNT,

	USE_ITEM_FUNCTION_FIRST = USE_ITEM_FUNCTION_NON_CONSUMABLE,
	USE_ITEM_FUNCTION_LAST = USE_ITEM_FUNCTION_LOCK_NPC_USING_QUESTITEM
};

#pragma pack(push, 4)
struct sUSE_ITEM_TBLDAT : public sTBLDAT
{
public:
		
	BYTE			byUse_Item_Active_Type;
	BYTE			byBuff_Group;
	BYTE			byBuffKeepType; //new
	DWORD			dwCool_Time_Bit_Flag;
	WORD			wFunction_Bit_Flag;
	DWORD			dwUse_Restriction_Rule_Bit_Flag; //new
	DWORD			dwUse_Allow_Rule_Bit_Flag; //new
	BYTE			byAppoint_Target;
	BYTE			byApply_Target;
	DWORD			dwApply_Target_Index; //new
	BYTE			byApply_Target_Max;
	BYTE			byApply_Range;
	BYTE			byApply_Area_Size_1; //new
	BYTE			byApply_Area_Size_2; //new
	WORD			wNeed_State_Bit_Flag;
	TBLIDX			aSystem_Effect[NTL_MAX_EFFECT_IN_ITEM];
	BYTE			abySystem_Effect_Type[NTL_MAX_EFFECT_IN_ITEM];
	double			aSystem_Effect_Value[NTL_MAX_EFFECT_IN_ITEM];//
	DWORD			dwRequire_LP; //
	WORD			wRequire_EP;
	BYTE			byRequire_RP_Ball;
	float			fCasting_Time;
	DWORD			dwCastingTimeInMilliSecs;
	DWORD			dwCool_Time; //
	DWORD			dwCoolTimeInMilliSecs;
	DWORD			dwKeep_Time; //
	DWORD			dwKeepTimeInMilliSecs;
	bool			bKeep_Effect;
	BYTE			byUse_Range_Min;
	float			fUse_Range_Min;
	BYTE			byUse_Range_Max;
	float			fUse_Range_Max;
	TBLIDX			Use_Info_Text;
	char			szCasting_Effect[ DBO_MAX_LENGTH_USE_ITEM_CASTING_EFFECT_NAME + 1 ];
	char			szAction_Effect[ DBO_MAX_LENGTH_USE_ITEM_ACTION_EFFECT_NAME + 1 ];
	
	ANIMATIONID		wCasting_Animation_Start;
	ANIMATIONID		wCasting_Animation_Loop;
	ANIMATIONID		wAction_Animation_Index;
	ANIMATIONID		wAction_Loop_Animation_Index;
	ANIMATIONID		wAction_End_Animation_Index;

	BYTE			byCastingEffectPosition;
	BYTE			byActionEffectPosition;

	TBLIDX			useWorldTblidx; //new

	float			fUseLoc_X;
	float			fUseLoc_Z;
	float			fUseLoc_Radius;
	QUESTID			RequiredQuestID;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)


class CUseItemTable : public CTable
{
public:

	CUseItemTable(void);
	virtual ~CUseItemTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CUseItemTable::m_pwszSheetList[0]); }
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