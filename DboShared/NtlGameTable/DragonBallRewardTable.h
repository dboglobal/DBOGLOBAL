#pragma once
//last update 02.03.2014
#include "Table.h"
#include "NtlItem.h"

#pragma pack(push, 4)
struct sDRAGONBALL_REWARD_TBLDAT : public sTBLDAT
{
public:
	BYTE		byBallType;
	BYTE		byRewardCategoryDepth;
	TBLIDX		rewardCategoryName;
	TBLIDX		rewardCategoryDialog;
	BYTE		byRewardType;
	TBLIDX		rewardName;
	TBLIDX		rewardLinkTblidx;
	DWORD		dwRewardZenny;
	TBLIDX		rewardDialog1;
	TBLIDX		rewardDialog2;
	DWORD		dwClassBit;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CDragonBallRewardTable : public CTable
{
public:
	CDragonBallRewardTable(void);
	virtual ~CDragonBallRewardTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);

protected:
	WCHAR** GetSheetListInWChar() { return &(CDragonBallRewardTable::m_pwszSheetList[0]); }
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