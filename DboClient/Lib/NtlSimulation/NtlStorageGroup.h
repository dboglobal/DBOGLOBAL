/*****************************************************************************
* File			: NtlStorageGroup.h
* Author		: HaeSung, Cho
* Copyright	: (주)NTL
* Date			: 2009. 2. 5	
* Abstract		: Ntl Storage Group base
*****************************************************************************
* Desc          : Unit들을 Group으로 가지는 StorageGroup의 Base class
* 만약 NtlStorageGroup을 상속 받는 클래스의 특수한 작동의 Load/Save 를 사용하려고
* 한다면 Load와 Save를 짝을 맞춰서 작성해줘야 한다.
*
* [Category]
* Key = Value;
*
* 형식의 데이터를 사용하려고 한다면 Base의 Load/Save 함수를 그대로 사용해도 상관없다.
*****************************************************************************/

#if !defined(_CNTLSTORAGEGROUP_H)
#define _CNTLSTORAGEGROUP_H

#include "NtlStorageDefine.h"
#include "NtlStorageUnit.h"
#include "NtlStorageSerializer.h"

#include <map>
#include <sstream>

class CNtlStorageGroup
{
public:
	CNtlStorageGroup();
	virtual ~CNtlStorageGroup();

	virtual bool Create();
	virtual void Destroy();

	virtual bool Load(CNtlStorageSerializer* pS);
	virtual bool Save(CNtlStorageSerializer* pS);
	virtual bool Reset();
	
	// Set
	bool SetData(unsigned int uiKey, int nData);
	bool SetData(unsigned int uiKey, float fData);
	bool SetData(unsigned int uiKey, std::string strData);
	bool SetData(unsigned int uiKey, const char* acData);
	bool SetData(unsigned int uiKey, bool bData);

	// Get
	bool GetIntData(unsigned int uiKey, int& nData);
	bool GetStringData(unsigned int uiKey, std::string& strData);
	bool GetBoolData(unsigned int uiKey, bool& bData);
	bool GetFloatData(unsigned int uiKey, float& fData);

	int			GetIntData(unsigned int uiKey);
	std::string GetStringData(unsigned int uiKey);
	bool		GetBoolData(unsigned int uiKey);
	float		GetFloatData(unsigned int uiKey);

	void		UnitCheck();

	void					SetStorageGroupType(eNTL_STORAGE_GROUP_TYPE eType);
	eNTL_STORAGE_GROUP_TYPE	GetStorageGroupType();

	bool					IsHave( eNTL_STORAGE_TYPE eType );
	CNtlStorageUnit*		GetStorageUnit( eNTL_STORAGE_TYPE eType );

	bool					SetUnitApplyFunc( NtlStorageUnitApplyFunc func );

protected:
	eNTL_STORAGE_GROUP_TYPE						m_eGroupType;

	typedef std::map<eNTL_STORAGE_TYPE, CNtlStorageUnit*>		UNITMAP;
	UNITMAP	m_mapStorageUnit;
};

#endif  //_CNTLSTORAGEGROUP_H
