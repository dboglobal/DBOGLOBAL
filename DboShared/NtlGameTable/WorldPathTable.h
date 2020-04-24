//***********************************************************************************
//	File		:	WorldPathTable.h
//
//	Begin		:	2007-06-01
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Shin Woo Teck   ( woody1019@ntl-inc.com )
//
//	Desc		:	
//***********************************************************************************


#pragma once


#include "NtlVector.h"
#include "NtlWorld.h"
#include <map>
#include <vector>


struct sPATH_POINT
{
	CNtlVector	_Pos;
	float		_PrevDist;
};

struct sPATH_LIST
{
	float						_fRadius;
	BYTE						_NumPathPoint;
	std::vector<sPATH_POINT>	_vecPathPoint;

	sPATH_LIST()
	{
		_fRadius		= 0.0f;
		_NumPathPoint	= 0;
	}

	~sPATH_LIST()
	{
		_vecPathPoint.clear();
	}

	sPATH_POINT* sPATH_LIST::GetAt(BYTE _PathPointIdx)
	{
		if(_PathPointIdx >= _NumPathPoint)
		{
			return NULL;
		}

		return &_vecPathPoint[_PathPointIdx];
	}
};

struct sPATH_TABLE
{
	typedef	std::map<TBLIDX, sPATH_LIST>	tTABLE;
	typedef tTABLE::iterator				tTABLEIT;
	typedef tTABLE::value_type				tTABLEVAL;

	unsigned int	_NumPathList;
	tTABLE			_mapPathList;	

	sPATH_TABLE()
	{
		_NumPathList = 0;
	}

	~sPATH_TABLE()
	{
		_mapPathList.clear();
	}

	sPATH_LIST* FindData(TBLIDX _TblIdx)
	{
		if(0 == _TblIdx)
		{
			return NULL;
		}

		tTABLEIT The = _mapPathList.find(_TblIdx);
		if(The == _mapPathList.end())
		{
			return NULL;
		}

		return static_cast<sPATH_LIST*>(&The->second); 
	}
};


class CWorldPathTable
{
public:
	CWorldPathTable(void);
	virtual ~CWorldPathTable(void);

protected:
	typedef	std::map<TBLIDX, sPATH_TABLE>	tTABLE;
	typedef tTABLE::iterator				tTABLEIT;
	typedef tTABLE::value_type				tTABLEVAL;

	unsigned int	m_NumPathTable;
	tTABLE			m_mapWorldPathTable;
	std::string		m_strDataPath;

public:

	bool			Create(const char * lpszDataPath);

	bool			CreateWorldPath(TBLIDX _TblIdx, LPCSTR _pWorldName);

public:

	sPATH_TABLE*	FindData(TBLIDX _TblIdx);
	sPATH_LIST *	FindPathList(TBLIDX worldTblidx, TBLIDX pathTblidx);
	unsigned int	GetNumPathTable();
};
