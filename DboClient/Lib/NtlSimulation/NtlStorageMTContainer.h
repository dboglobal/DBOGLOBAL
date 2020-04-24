/*****************************************************************************
* File			: NtlStorageMTContainer.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 2. 5	
* Abstract		: NtlStorage의 Key와 관련 정보들을 저장하는 매핑 테이블
*****************************************************************************
* Desc			: 
*
* 만약 MappingTable을 수정하기 위해서는 NtlStorageDefine.h 에 있는 Define값을 수정하고
* 현재 클래스에 있는 매핑테이블 데이터를 채워준다. Table 배열의 끝에 삽입은 할 수 있으나 수정/삭제는 절대로
* 하지 않는다. 만약 폐기해야할 KEY가 있다면 정의된 KEY는 그대로 놔두고 그냥 사용하지 않는다.
*
* Mapping Table은 static 배열로 구성되며 절대로 중간 삽입/삭제는 해서는 안된다.
* ( Mapping table is composed of static array, and you should not insert/delete it from the midterm )
*****************************************************************************/

#ifndef __NTL_STORAGE_MT_CONTAINER_H__
#define __NTL_STORAGE_MT_CONTAINER_H__

#include "NtlStorageDefine.h"

#include <map>
#include <string>

#pragma once

class CNtlStorageMTContainer
{
public:
	CNtlStorageMTContainer(void);
	virtual ~CNtlStorageMTContainer(void);

	const SNtlStorageMappingTableData* GetMappingTable( unsigned int uiKey );

	bool					SortTable();
	bool					SetUnitCheckFunc( unsigned int uiKey, NtlStorageUnitCheckFunc funcApply);
	NtlStorageUnitCheckFunc		GetUnitCheckFunc( unsigned int uiKey );

	unsigned int				GetCount();
	eNTL_STORAGE_VARIABLE_TYPE	GetVariableType( unsigned int uiKey );
	eNTL_STORAGE_TYPE			GetStorageType( unsigned int uiKey );
	const char*					GetString( unsigned int uiKey );
	const char*					GetDefaultString( unsigned int uiKey );
	bool						GetDefaultBool( unsigned int uiKey );
	int							GetDefaultInt( unsigned int	uiKey );
	float						GetDefaultFloat( unsigned int uiKey );
	bool						GetKeyFromString( std::string& strKeyName, unsigned int& uiKey );

	static CNtlStorageMTContainer* GetInstance()
	{
		static CNtlStorageMTContainer instance;
		return &instance;
	}

protected:
	static SNtlStorageMappingTableData m_StorageMappingTable[];

	// Type 별로 멀티맵에 포인터를 보관하고 있는다.
	typedef std::multimap	<unsigned char, SNtlStorageMappingTableData*> SORTMMAP;
	SORTMMAP		m_mmapCategory;

	// KEY ID 순서대로 MAP에 보관하고 있는다.
	typedef std::map	<unsigned int, SNtlStorageMappingTableData* > TABLEMAP;
	TABLEMAP		m_mapTable;

	typedef std::map	<std::string, SNtlStorageMappingTableData* > STRINGMAP;
	STRINGMAP		m_mapString;


	bool			m_bSorted;			///< 테이블을 정렬해서 가지고 있는지 확인
	unsigned int	m_uiCount;			///< 테이블 원소 갯수
};

static CNtlStorageMTContainer* GetNtlStorageMTContainer()
{
	return CNtlStorageMTContainer::GetInstance();
}

#endif