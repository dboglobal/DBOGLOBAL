//***********************************************************************************
//
//	File		:	StatusTransformTable.h
//
//	Begin		:	2006-12-26
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"

#pragma pack(push, 4)
struct sSTATUS_TRANSFORM_TBLDAT : public sTBLDAT
{
public:
	float			fLP_Transform;
	float			fEP_Transform;
	float			fPhysical_Offence_Transform;
	float			fEnergy_Offence_Transform;
	float			fPhysical_Defence_Transform;
	float			fEnergy_Defence_Transform;
	float			fRun_Speed_Transform;
	float			fAttack_Speed_Transform;
	float			fAttack_Rate_Transform;
	float			fDodge_Rate_Transform;
	float			fBlock_Rate_Transform;
	float			fCurse_Success_Transform;
	float			fCurse_Tolerance_Transform;
	float			fAttack_Range_Change;

	float			fLP_Consume_Rate;
	float			fEP_Consume_Rate;
	DWORD			dwDuration;
	DWORD			dwDurationInMilliSecs;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CStatusTransformTable : public CTable
{
public:
	CStatusTransformTable(void);

	virtual ~CStatusTransformTable(void);


public:

	bool						Create(DWORD dwCodePage);

	void						Destroy();


protected:

	void						Init();


public:

	sTBLDAT *					FindData(TBLIDX tblidx); 


protected:

	WCHAR**						GetSheetListInWChar() { return &(CStatusTransformTable::m_pwszSheetList[0]); }

	void*						AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);

	bool						DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);

	bool						AddTable(void * pvTable, bool bReload, bool bUpdate);

	bool						SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:

	static WCHAR*				m_pwszSheetList[];
};