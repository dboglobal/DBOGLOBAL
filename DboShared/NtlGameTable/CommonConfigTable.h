#pragma once
//created 5.5.2015
#include "Table.h"

#define COMMONCONFIG_MAX_TBLDAT_VALUE_COUNT	10
const BYTE NON_NPC_MERCHANT_IDX_COUNT = 6;
const BYTE NON_NPC_MERCHANT_BUYSELL_COUNT = 6;

#pragma pack(push, 4)

struct sNON_NPC_MERCHANT_DATA
{
	TBLIDX	m_aTblIdxMerchant[NON_NPC_MERCHANT_IDX_COUNT];
	BYTE	m_abyBuySellRate[NON_NPC_MERCHANT_BUYSELL_COUNT];
};

struct sCOMMONCONFIG_VALUE_DATA
{
	DWORD	adwValue[COMMONCONFIG_MAX_TBLDAT_VALUE_COUNT];
};

struct sCOMMONCONFIG_TBLDAT : public sTBLDAT
{

public:

	std::wstring				wstrName;
	std::wstring				wstrValue[COMMONCONFIG_MAX_TBLDAT_VALUE_COUNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CCommonConfigTable : public CTable
{
public:

	CCommonConfigTable(void);
	virtual ~CCommonConfigTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *					FindData(TBLIDX tblidx); 
	sCOMMONCONFIG_VALUE_DATA*	FindCommonConfig(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CCommonConfigTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


	bool			ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue);

	bool			ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue);

	bool			ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx);

	bool			ReadFLOAT(float & rDest, std::wstring & wstrValue, float fMaxValue);

	bool			ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc);

	bool			GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);



private:
	static WCHAR* m_pwszSheetList[];

	/*sNON_NPC_MERCHANT_DATA m_aNonNPCMerchant[3];
	DWORD	m_dwMaterialDecompositionConstant;
	BYTE	m_byAdmissionDayMax[2];

	DWORD	m_dwOverweightEffectCount;
	DWORD	m_adwOverweightEffectRangeBase[10];
	TBLIDX	m_aOverweightEffectSkillTblidx[10][8];

	TBLIDX	m_aRevivalAftereffectSkillTblidx[5];
	TBLIDX	m_aRevivalAftereffectSkillTblidx_PCBang[5];
	DWORD	m_dwRevivalAftereffectValue[4];
	DWORD	m_dwRevivalAftereffectValue_PCBang[4];

	BYTE	m_aClassRoleplay[21][3];
	BYTE	m_aDungeonTypeMaxRole[4][3];
	*/

	std::map<TBLIDX, sCOMMONCONFIG_VALUE_DATA*> m_mapCommonConfigValueList;

};