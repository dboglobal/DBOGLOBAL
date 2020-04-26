#pragma once
//updated 1.12.2014
#include "Table.h"
#include "NtlItem.h"

// Hard Coding
#define	HOIPOISTONE_ITEM_INDEX	19954


#pragma pack(push, 4)
struct sITEM_TBLDAT : public sTBLDAT
{

public:
	bool			bValidity_Able;
	TBLIDX			Name;
	WCHAR			wszNameText[ DBO_MAX_LENGTH_ITEM_NAME_TEXT + 1 - 32]; // -32 because in table we only store 32 + 1
	char			szIcon_Name[ DBO_MAX_LENGTH_ITEM_ICON_NAME + 1 ];
	BYTE			byModel_Type;
	char			szModel[ DBO_MAX_LENGTH_ITEM_MODEL_NAME + 1 ];
	char			szSub_Weapon_Act_Model[ DBO_MAX_LENGTH_ITEM_SUBWEAPON_ACT_MODEL_NAME + 1 ];
	BYTE			byItem_Type;
	BYTE			byEquip_Type;
	DWORD			dwEquip_Slot_Type_Bit_Flag;
	WORD			wFunction_Bit_Flag;	//eITEM_FUNC_BIT_FLAG
	BYTE			byMax_Stack;
	BYTE			byRank;
	DWORD			dwWeight; //new
	DWORD			dwCost;
	DWORD			dwSell_Price; //new
	BYTE			byDurability;
	BYTE			byDurability_Count;
	BYTE			byBattle_Attribute;

	WORD			wPhysical_Offence;
	WORD			wEnergy_Offence;
	WORD			wPhysical_Defence;
	WORD			wEnergy_Defence;

	float			fAttack_Range_Bonus;
	WORD			wAttack_Speed_Rate;

	BYTE			byNeed_Min_Level; //new
	BYTE			byNeed_Max_Level; //new

	DWORD			dwNeed_Class_Bit_Flag;

	DWORD			dwNeed_Gender_Bit_Flag; //new

	BYTE			byClass_Special;
	BYTE			byRace_Special;
	WORD			wNeed_Str;
	WORD			wNeed_Con;
	WORD			wNeed_Foc;
	WORD			wNeed_Dex;
	WORD			wNeed_Sol;
	WORD			wNeed_Eng;
	TBLIDX			set_Item_Tblidx;
	TBLIDX			Note;
	BYTE			byBag_Size;
	WORD			wScouter_Watt;	
	DWORD			dwScouter_MaxPower;
	BYTE			byScouter_Parts_Type1;
	BYTE			byScouter_Parts_Type2;
	BYTE			byScouter_Parts_Type3;
	BYTE			byScouter_Parts_Type4;


	TBLIDX			Use_Item_Tblidx;
	bool			bIsCanHaveOption; //new
	TBLIDX			Item_Option_Tblidx;

	BYTE			byItemGroup;
	TBLIDX			Charm_Tblidx;
	WORD			wCostumeHideBitFlag;
	TBLIDX			NeedItemTblidx;
	DWORD			CommonPoint;
	BYTE			byCommonPointType;
	BYTE			byNeedFunction;
	DWORD			dwUseDurationMax;
	BYTE			byDurationType;		//eDURATIONTYPE
	TBLIDX			contentsTblidx;
	DWORD			dwDurationGroup;
	BYTE			byDropLevel;
	
	TBLIDX			enchantRateTblidx;
	TBLIDX			excellentTblidx;
	TBLIDX			rareTblidx;
	TBLIDX			legendaryTblidx;

	bool			bCreateSuperiorAble;
	bool			bCreateExcellentAble;
	bool			bCreateRareAble;
	bool			bCreateLegendaryAble;
	BYTE			byRestrictType;			//eITEM_RESTRICT_TYPE

	float			fAttack_Physical_Revision;
	float			fAttack_Energy_Revision;
	float			fDefence_Physical_Revision;
	float			fDefence_Energy_Revision;

	BYTE			byTmpTabType;
	bool			bIsCanRenewal;
	WORD			wDisassemble_Bit_Flag;

	BYTE			byDisassembleNormalMin;
	BYTE			byDisassembleNormalMax;
	BYTE			byDisassembleUpperMin;
	BYTE			byDisassembleUpperMax;
	BYTE			byDropVisual;

	BYTE			byUseDisassemble;
	


	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemTable : public CTable
{
public:

	CItemTable(void);
	virtual ~CItemTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

	TBLIDX				GetRandomTblidx(TBLIDX minItemTblidx, TBLIDX maxItemTblidx, BYTE byMinLevel, BYTE byMaxLevel);
	TBLIDX				FindDisassembleData(int nMaterialType, BYTE byRank, BYTE byStep);
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CItemTable::m_pwszSheetList[0]); }
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