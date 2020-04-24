//***********************************************************************************
//
//	File		:	ModelToolObjDataTable.h
//
//	Begin		:	2007-04-12
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hong Ho Dong   ( battery@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************
#pragma once

#include <map>
#include <string>
#include "GraphicDataTable.h"
#include "NtlVector.h"

const int NTL_MAX_SIZE_OBJ_ANIM	= (OBJECT_ANIMATION_END - OBJECT_ANIMATION_START);

struct sOBJ_DATA_INFO
{
	float	fDurationTime;			// Animation 전체 시간
	bool	bDataLoaded;			// Data가 Load가 되는지

	sOBJ_DATA_INFO() : fDurationTime(0), bDataLoaded(false) { }
};

struct sMODELTOOL_OBJ_TBLDAT
{
private:
	sOBJ_DATA_INFO	m_aObjTbldat[ NTL_MAX_SIZE_OBJ_ANIM ];
public:
	CNtlVector		m_vMin;
	CNtlVector		m_vMax;
	float			m_fScale;

protected:
	int GetRemapAnimKeyID( int nAnimKeyID);

public:
	bool Add(int nAnimKeyID, float fDurationTime);
	sOBJ_DATA_INFO *GetAnim(int nAnimKeyID);
	float	GetAnimDurationTime(int nAnimKeyID);
public:
	//wrapping function은 여기에 추가 해서 쓰세요 (by HoDong)
};


class CModelToolObjDataTable
{

public:
	typedef	std::map<std::string, sMODELTOOL_OBJ_TBLDAT *> TABLE;
	typedef TABLE::iterator TABLEIT;
	typedef TABLE::value_type TABLEVAL;
	
	CModelToolObjDataTable();
	~CModelToolObjDataTable();

private:
	TABLE	m_mapTableList;

protected:
	bool	LoadPropertyList(const char *lpszPath, const char * lpszFileName);
	bool	LoadProperty(const char * lpszFullPathFileName);
	bool	Add(const std::string &strModelName, sMODELTOOL_OBJ_TBLDAT *pModelToolObjData);

public:
	bool	Create(const char *lpszPath);
	void	Destroy();

	sMODELTOOL_OBJ_TBLDAT *FindData(const std::string &strModelName);

};
