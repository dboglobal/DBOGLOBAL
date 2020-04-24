//***********************************************************************************
//
//	File		:	CDboNavi.h
//
//	Begin		:	2007-07-27
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hong Ho Dong   ( battery@ntl-inc.com )
//
//	Desc		:	Path & Field(Zone) & Tile Attribute
//
//***********************************************************************************
#pragma once

#include "WorldTable.h"
#include "DboWeInfoGroup.h"
#include "GraphicDataTable.h"

#include "NtlNavi\NtlNaviDefine.h"



class CDboNavi
{
public:
	static DWORD CREATE_INVALID;
	static DWORD CREATE_TILE_INFO;				// Tile Info Create
	static DWORD CREATE_FIELD_INFO;				// Field Info Create
	static DWORD CREATE_PATH_ENGINE_INFO;		// Path Engine Info Create

	struct SCreateParam
	{
		const char *pszWeTileInfoPath;			// TileInfo File Path
		const char *pszWeFieldInfoPath;			// FieldInfo File Path		
		const char *pszPathEnginePath;			// PathEngine File Path
		CWorldTable	*pWorldTable;				// Pointer WorldTable

		DWORD dwCreateInfoFlag;					// CreateFlag

		SCreateParam(): pszWeTileInfoPath(NULL), 
					    pszWeFieldInfoPath(NULL),
					    pszPathEnginePath(NULL),
						pWorldTable(NULL) { dwCreateInfoFlag = CREATE_INVALID; }
	};

public:
	CDboNavi();
	~CDboNavi();

protected:
	CDboWeInfoGroup	m_DboWeInfoGroup;

public:
	bool	Create(SCreateParam *pCreateParam);
	void	Destroy();

protected:
	void	Init();

public:
	DWORD	GetAttribute(unsigned int uiWorldIndex, float x, float z);
	bool	IsBasicAttributeSet(eDBO_WORLD_ATTR_BASIC eAttrBasic, unsigned int uiWorldIndex, float x, float z);
	
	TBLIDX	GetTextAllIndex(const TBLIDX worldTblidx, const float x, const float z);
	ZONEID	GetZoneIndex(TBLIDX worldTblidx, float x, float z);

	// 아직 구현 안 되었음
	bool	CanSee(unsigned int uiWorldIndex, float curX, float curY, float curZ, float desX, float desY, float desZ);
	

protected:
	void	RegisterTerrainAttributeGroup(eDBO_WORLD_ATTR_GROUP eAttrGroup, eDBO_WORLD_ATTR_BASIC eAttrBasic);

private:
	DWORD	m_adwBasicAttributeCheckFlag[DBO_WORLD_ATTR_BASIC_COUNT];
};