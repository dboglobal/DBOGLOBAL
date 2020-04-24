//***********************************************************************************
//
//	File		:	GraphicDataMapTable.h
//
//	Begin		:	2006-12-12 first written
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hong Ho Dong   ( battery@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

/**
 * World의 Center(0, 0)는 World의 중앙이다.
 * World 좌표계에서 x,z은 right x--, bottom z--, left x++, top z++ 이다.
 * World는 항상 가로, 세로 Size가 같다.
 * 첫번째 Field는 right, bottom에서 시작 left++, top++
 * 첫번째 Tile은 right, bottom에서 시작 left++, top++
 * Filed 의 Sector의 갯수는 4개
 */
#pragma once
#include <map>
#include "WorldTable.h"

#define NTL_SIZE_FIELD				256				// Field Size(m)
#define NTL_SIZE_TILE				4				// Tile Size(m)

//#define NTL_NUM_WIDTH_SECTOR_TILE			32				// Sector의 Tile 갯수
#define NTL_NUM_FIELD_TILE			4096			// Field의 Tile 갯수
#define NTL_NUM_WIDTH_FIELD_TILE	64				// Field의 Tile의 가로 갯수

class CDboWeInfo
{
public:
	CDboWeInfo();
	~CDboWeInfo();

private:
	bool		m_bTileDataLoaded;
	bool		m_bFieldDataLoaded;

	int			m_nAllFieldNum;
	int			m_nWidthFieldNum;
	int			m_nHeightFieldNum;
	int			m_nWidthHalfSize;
	int			m_nHeightHalfSize;
	
	DWORD		*m_pTileInfo;

	TBLIDX		*m_pFieldTextAll;
	ZONEID		*m_pFieldZone;
	
public:
	bool		Create(const char * pszTileFullPathFileName, const char * pszFieldFullPathFileName);
	void		Destroy();
	
	inline bool	IsTileDataLoaded() { return m_bTileDataLoaded; }
	inline bool	IsFieldDataLoaded() { return m_bFieldDataLoaded; }

	DWORD		GetAttribute(float x, float z);
	TBLIDX		GetTextAllIndex(float x, float z);
	ZONEID		GetZoneIndex(float x, float z);

	bool		CanSee(float curX, float curY, float curZ, float desX, float desY, float desZ);
};

class CDboWeInfoGroup
{
public:
	typedef std::map<TBLIDX, CDboWeInfo *> TABLE;
	typedef TABLE::iterator TABLEIT;
	typedef TABLE::value_type TABLEVAL;

public:
	CDboWeInfoGroup();
	~CDboWeInfoGroup();
private:
	TABLE	m_mapTableList;

public:
	bool	Create(const char * pszTilePath, const char * pszFieldPath, CWorldTable *pWorldTable);
	void	Destroy();

	DWORD	GetAttribute(unsigned int uiWorldIndex, float x, float z);
	TBLIDX	GetTextAllIndex(const TBLIDX worldTblidx, const float x, const float z);
	ZONEID	GetZoneIndex(TBLIDX worldTblidx, float x, float z);
	
	bool	CanSee(unsigned int uiWordIndex, float curX, float curY, float curZ, float desX, float desY, float desZ);

};



