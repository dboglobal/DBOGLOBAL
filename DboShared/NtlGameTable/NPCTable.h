#pragma once
#include "table.h"
#include "PCTable.h"

const DWORD		DBO_MAX_LENGTH_BOT_NAME_TEXT = 40;
const DWORD		DBO_MAX_LENGTH_BOT_CAMERA_BONE_NAME = 32;
const DWORD		DBO_MAX_LENGTH_BOT_ILLUST_NAME = 32;
// NPC/MOB/PET의 공통 속성 데이타
#pragma pack(push, 4)
struct sBOT_TBLDAT : public sCHAR_TBLDAT
{
//	sBOT_TBLDAT()
//	{
//		bValidity_Able = true;
//	};

public:
	bool			bValidity_Able;
	TBLIDX			Name;
	WCHAR			wszNameText[ DBO_MAX_LENGTH_BOT_NAME_TEXT + 1 ];
	char			szModel[ NTL_MAX_LENGTH_MODEL_NAME + 1 ];
	BYTE			byLevel;
	BYTE			byGrade;

	DWORD			dwAi_Bit_Flag;

	WORD			wLP_Regeneration;
	WORD			wEP_Regeneration;
	BYTE			byAttack_Animation_Quantity;
	BYTE			byBattle_Attribute;

	WORD			wBasic_Physical_Offence;
	WORD			wBasic_Energy_Offence;

	float			fWalk_Speed_Origin;
	float			fWalk_Speed;
	float			fRun_Speed_Origin;
	float			fRun_Speed;
	float			fRadius_X;
	float			fRadius_Z;

	WORD			wSight_Range;
	WORD			wScan_Range;

	WORD			wUse_Skill_Time[NTL_MAX_NPC_HAVE_SKILL];
	TBLIDX			use_Skill_Tblidx[NTL_MAX_NPC_HAVE_SKILL];
	BYTE			byUse_Skill_Basis[NTL_MAX_NPC_HAVE_SKILL];
	WORD			wUse_Skill_LP[NTL_MAX_NPC_HAVE_SKILL];

	BYTE			byVisible_Sight_Range;

	char			szCamera_Bone_Name[ DBO_MAX_LENGTH_BOT_CAMERA_BONE_NAME + 1 ];

	WORD			wAttackCoolTime; 
	float			fFly_Height;

	char			szNameText[ DBO_MAX_LENGTH_BOT_NAME_TEXT + 1 ];

	bool			bSpawn_Animation;
	DWORD			dwDialogGroup;
	char			szILLust[ DBO_MAX_LENGTH_BOT_ILLUST_NAME + 1 ];

	DWORD			dwAllianceIdx; 
	WORD			wAggroMaxCount;

	DWORD			dwNpcAttributeFlag;
	WORD			wStomachacheDefence;
	WORD			wPoisonDefence;
	WORD			wBleedDefence;
	WORD			wBurnDefence;

};
#pragma pack(pop)

// NPC
#pragma pack(push, 4)
struct sNPC_TBLDAT : public sBOT_TBLDAT
{
public:
	BYTE			byNpcType;
	BYTE			byJob;
	DWORD			dwFunc_Bit_Flag;
	TBLIDX			Dialog_Script_Index;

	TBLIDX			amerchant_Tblidx[NTL_MAX_MERCHANT_TAB_COUNT];

	TBLIDX			statusTransformTblidx;
	TBLIDX			contentsTblidx;

	WORD			wUnknown;
	DWORD			dwUnknown2;
	DWORD			dwUnknown3;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CNPCTable : public CTable
{
public:
	CNPCTable(void);
public:
	virtual ~CNPCTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	static TBLIDX FindMerchantItem(sNPC_TBLDAT* psTbldat, BYTE byIndex);
protected:
	WCHAR** GetSheetListInWChar() { return &(CNPCTable::m_pwszSheetList[0]); }
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
