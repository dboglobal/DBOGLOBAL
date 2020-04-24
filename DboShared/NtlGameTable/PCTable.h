#pragma once

#include "Table.h"
#include "NtlCharacter.h"

const DWORD		DBO_MAX_LENGTH_MODEL_NAME = 32;

// 모든 캐릭터들의 공통 속성 데이타
#pragma pack(push, 4)
struct sCHAR_TBLDAT : public sTBLDAT
{
public:
	int				dwBasic_LP;
	WORD			wBasic_EP;
	WORD			wBasic_Physical_Defence;
	WORD			wBasic_Energy_Defence;

	WORD			wBasicStr;
	WORD			wBasicCon;
	WORD			wBasicFoc;
	WORD			wBasicDex;
	WORD			wBasicSol;
	WORD			wBasicEng;

	float			fScale;
	WORD			wAttack_Speed_Rate; // 맨손 공격시의 애니메이션 속도에 대한 %
	BYTE			byAttack_Type;
	float			fAttack_Range;
	WORD			wAttack_Rate;
	WORD			wDodge_Rate;
	WORD			wBlock_Rate;
	WORD			wCurse_Success_Rate;
	WORD			wCurse_Tolerance_Rate;
	float			fRadius;
	WORD			wBasic_Aggro_Point;
};
#pragma pack(pop)

// PC
#pragma pack(push, 4)
struct sPC_TBLDAT : public sCHAR_TBLDAT
{
public:
	BYTE			byRace;
	BYTE			byGender;
	BYTE			byClass;
	TBLIDX			prior_Class_Tblidx;
	DWORD			dwClass_Bit_Flag;
	char			szModel_Child[ NTL_MAX_LENGTH_MODEL_NAME + 1 ];
	char			szModel_Adult[ NTL_MAX_LENGTH_MODEL_NAME + 1 ];

	float			fChild_Run_Speed_Origin;
	float			fChild_Run_Speed;
	float			fAdult_Run_Speed_Origin;
	float			fAdult_Run_Speed;
	float			fChild_Fly_Speed_Origin;
	float			fChild_Fly_Speed;
	float			fAdult_Fly_Speed_Origin;
	float			fAdult_Fly_Speed;
	float			fChild_Dash_Speed_Origin;
	float			fChild_Dash_Speed;
	float			fAdult_Dash_Speed_Origin;
	float			fAdult_Dash_Speed;
	float			fChild_Accel_Speed_Origin;
	float			fChild_Accel_Speed;
	float			fAdult_Accel_Speed_Origin;
	float			fAdult_Accel_Speed;

	WORD			wBasic_RP;
	WORD			wBasic_Physical_Offence;
	WORD			wBasic_Energy_Offence;

	DWORD			dwWeightLimit;

	BYTE			byLevel_Up_LP;
	BYTE			byLevel_Up_EP;
	BYTE			byLevel_Up_RP;
	BYTE			byLevel_Up_Physical_Offence;
	BYTE			byLevel_Up_Physical_Defence;
	BYTE			byLevel_Up_Energy_Offence;
	BYTE			byLevel_Up_Energy_Defence;

	float			fLevel_Up_Str;
	float			fLevel_Up_Con;
	float			fLevel_Up_Foc;
	float			fLevel_Up_Dex;
	float			fLevel_Up_Sol;
	float			fLevel_Up_Eng;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CPCTable : public CTable
{
public:
	CPCTable(void);

	virtual ~CPCTable(void);


public:

	bool						Create(DWORD dwCodePage);

	void						Destroy();


protected:

	void						Init();


public:

	sTBLDAT *					FindData(TBLIDX tblidx); 

	sTBLDAT *					GetPcTbldat(BYTE byRace, BYTE byClass, BYTE byGender);

	bool						SetPcTbldat(BYTE byRace, BYTE byClass, BYTE byGender, sTBLDAT * pTbldat);

	

protected:

	WCHAR**						GetSheetListInWChar() { return &(CPCTable::m_pwszSheetList[0]); }

	void*						AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);

	bool						DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);

	bool						AddTable(void * pvTable, bool bReload, bool bUpdate);

	bool						SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


protected:

	sTBLDAT *					m_aPcTbldat[RACE_COUNT][PC_CLASS_COUNT][GENDER_COUNT];


private:

	static WCHAR* m_pwszSheetList[];

};
