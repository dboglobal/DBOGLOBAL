//***********************************************************************************
//
//	File		:	NtlNaviWorldAttribute.cpp
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
#include "StdAfx.h"
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include "DboWeInfoGroup.h"


CDboWeInfo::CDboWeInfo() : m_bTileDataLoaded(false),
						   m_bFieldDataLoaded(false),
						   m_pTileInfo(NULL),
						   m_pFieldTextAll(NULL),
						   m_pFieldZone(NULL),
				           m_nAllFieldNum(0),
				           m_nWidthFieldNum(0),
				           m_nHeightFieldNum(0),
				           m_nWidthHalfSize(0),
				           m_nHeightHalfSize(0)
{
}


CDboWeInfo::~CDboWeInfo()
{
	Destroy();
}


bool CDboWeInfo::Create(const char * pszTileFullPathFileName, const char * pszFieldFullPathFileName)
{
	if( IsTileDataLoaded() || IsFieldDataLoaded() )
		Destroy();

	FILE *fp = NULL;

	if(pszTileFullPathFileName != NULL)
	{
		if(fopen_s(&fp, pszTileFullPathFileName, "rb") != 0)
		{
			printf("%s: File not load\n", pszTileFullPathFileName);
			return false;
		}
		
		fread(&m_nAllFieldNum, sizeof(m_nAllFieldNum), 1, fp);
		
		m_nWidthFieldNum	= (int)sqrt((float)m_nAllFieldNum);
		m_nHeightFieldNum	= m_nWidthFieldNum;

		m_nWidthHalfSize	= m_nWidthFieldNum * NTL_SIZE_FIELD / 2;
		m_nHeightHalfSize	= m_nHeightFieldNum * NTL_SIZE_FIELD / 2;

		if(m_nAllFieldNum != 0)
			m_pTileInfo = new DWORD[m_nAllFieldNum * NTL_NUM_FIELD_TILE];
		else
		{
			_ASSERT(0);
			
			if( fp != NULL)
			{
				fclose(fp);
			}

			return false;
		}

		for(int i = 0; i < m_nAllFieldNum; i++)
		{
			if( fread(&m_pTileInfo[i*NTL_NUM_FIELD_TILE], NTL_NUM_FIELD_TILE * sizeof(DWORD), 1, fp) != 1)
			{
				_ASSERT(0);
				break;
			}
		}

		if(fp != NULL)
		{
			fclose(fp);
		}

	//	sprintf_s(buffer, "%s: File load\n", pszTileFullPathFileName); 
	//	OutputDebugString(buffer);
		m_bTileDataLoaded = true;
	}

	if(pszFieldFullPathFileName != NULL)
	{
		if(fopen_s(&fp, pszFieldFullPathFileName, "rb") != 0)
		{
			printf("%s: File not load\n", pszFieldFullPathFileName);
			return false;
		}
		
		fread(&m_nAllFieldNum, sizeof(m_nAllFieldNum), 1, fp);
		m_nWidthFieldNum	= (int)sqrt((float)m_nAllFieldNum);
		m_nHeightFieldNum	= m_nWidthFieldNum;

		m_nWidthHalfSize	= m_nWidthFieldNum * NTL_SIZE_FIELD / 2;
		m_nHeightHalfSize	= m_nHeightFieldNum * NTL_SIZE_FIELD / 2;
		
		if(m_nAllFieldNum != 0)
		{
			m_pFieldTextAll = new TBLIDX[m_nAllFieldNum];
			m_pFieldZone = new ZONEID[m_nAllFieldNum];
		}
		else
		{
			_ASSERT(0);
			
			if( fp != NULL)
			{
				fclose(fp);
			}

			return false;
		}

		for(int i = 0; i < m_nAllFieldNum; i++)
		{
			if( fread(&m_pFieldTextAll[i], sizeof(TBLIDX), 1, fp) != 1)
			{
				_ASSERT(0);
				break;
			}

			if (0 == m_pFieldTextAll[i])
			{
				m_pFieldTextAll[i] = INVALID_TBLIDX;
			}

			if (INVALID_TBLIDX == m_pFieldTextAll[i])
			{
				m_pFieldZone[i] = INVALID_ZONEID;
			}
			else
			{
				m_pFieldZone[i] = m_pFieldTextAll[i] / 1000;
			}
		}

		if(fp != NULL)
		{
			fclose(fp);
		}

	//	sprintf_s(buffer, "%s: File load\n", pszFieldFullPathFileName); 
	//	OutputDebugString(buffer);

		m_bFieldDataLoaded = true;
	}

	return true;
}


void CDboWeInfo::Destroy()
{
	if(m_pTileInfo)
	{
		delete m_pTileInfo;
		m_pTileInfo = NULL;
	}

	if(m_pFieldTextAll)
	{
		delete m_pFieldTextAll;
		m_pFieldTextAll = NULL;
	}

	if(m_pFieldZone)
	{
		delete m_pFieldZone;
		m_pFieldZone = NULL;
	}

	m_bTileDataLoaded = false;
	m_bFieldDataLoaded = false;
}

DWORD CDboWeInfo::GetAttribute(float x, float z)
{
	if(!IsTileDataLoaded())
		return 0;
	
	//Tile 좌표계로 변환
	float fTileCoordX = x + m_nWidthHalfSize;
	float fTileCoordZ = z + m_nHeightHalfSize;
	
	int nFieldNum = int(fTileCoordX) / NTL_SIZE_FIELD + 
		            int(fTileCoordZ) / NTL_SIZE_FIELD * m_nWidthFieldNum;

	int nFieldInTileX = ((int(fTileCoordX) % NTL_SIZE_FIELD) / NTL_SIZE_TILE);
	int nFieldInTileZ = ((int(fTileCoordZ) % NTL_SIZE_FIELD) / NTL_SIZE_TILE);

	int nTileNum = nFieldInTileZ * NTL_NUM_WIDTH_FIELD_TILE + nFieldInTileX;
		           
	return m_pTileInfo[nFieldNum * NTL_NUM_FIELD_TILE + nTileNum];
}

TBLIDX CDboWeInfo::GetTextAllIndex(float x, float z)
{
	TBLIDX textindex = INVALID_TBLIDX;

	if(!IsFieldDataLoaded())
		return textindex;

	int fTileCoordX = (int)x + m_nWidthHalfSize;
	int fTileCoordZ = (int)z + m_nHeightHalfSize;
	
	int nFieldNum = (fTileCoordX / NTL_SIZE_FIELD) + (fTileCoordZ / NTL_SIZE_FIELD) * m_nWidthFieldNum;

	return m_pFieldTextAll[nFieldNum];
}

ZONEID CDboWeInfo::GetZoneIndex(float x, float z)
{
	ZONEID zoneid = INVALID_ZONEID;

	if(!IsFieldDataLoaded())
		return zoneid;

	int fTileCoordX = (int)x + m_nWidthHalfSize;
	int fTileCoordZ = (int)z + m_nHeightHalfSize;
	
	int nFieldNum = (fTileCoordX / NTL_SIZE_FIELD) + (fTileCoordZ / NTL_SIZE_FIELD) * m_nWidthFieldNum;

	return m_pFieldZone[nFieldNum];
}

bool CDboWeInfo::CanSee(float curX, float curY, float curZ, float desX, float desY, float desZ)
{
	if(!IsTileDataLoaded())
	{
		return true;
	}

	float a,b,c,d,e,f;
	a = curX;
	b = curY;
	c = curZ;
	d = desX;
	e = desY;
	f = desZ;

	return true;
}

CDboWeInfoGroup::CDboWeInfoGroup()
{

}

CDboWeInfoGroup::~CDboWeInfoGroup()
{
	Destroy();
}

bool CDboWeInfoGroup::Create(const char * pszTilePath, const char * pszFieldPath, CWorldTable *pWorldTable)
{
	char szBuffer1[1024];
	char szBuffer2[1024];
	char* pTileFileName = NULL;
	char* pFieldFileName = NULL;

	sWORLD_TBLDAT *pWorldData = NULL;

	for (CTable::TABLEIT iterTable = pWorldTable->Begin() ; iterTable != pWorldTable->End() ; iterTable++)
	{
		pWorldData = (sWORLD_TBLDAT*)(iterTable->second);
		if(pWorldData == NULL)
		{
			_ASSERT(0);
			return false;
		}

		if(m_mapTableList.find( pWorldData->tblidx ) != m_mapTableList.end())
		{
			_ASSERT(0);
			return false;
		}

		if (NULL == pszTilePath)
		{
			pTileFileName = NULL;
		}
		else
		{
			sprintf_s(szBuffer1,"%s\\%s", pszTilePath, pWorldData->szName);
			pTileFileName = szBuffer1;
		}

		if (NULL == pszFieldPath)
		{
			pFieldFileName = NULL;
		}
		else
		{
			sprintf_s(szBuffer2,"%s\\%s", pszFieldPath, pWorldData->szName);
			pFieldFileName = szBuffer2;
		}

		CDboWeInfo *pDboWeInfo = new CDboWeInfo;
		if(pDboWeInfo->Create(pTileFileName, pFieldFileName) == true)
		{
			m_mapTableList[pWorldData->tblidx] = pDboWeInfo;
		}
		else
		{
			delete pDboWeInfo;
			pDboWeInfo = NULL;
		}
	}
	return true;
}


void CDboWeInfoGroup::Destroy()
{
	CDboWeInfoGroup::TABLEIT stMapIT  = m_mapTableList.begin();
	CDboWeInfoGroup::TABLEIT endMapIT = m_mapTableList.end();

	for(;stMapIT != endMapIT;)
	{
		CDboWeInfo *pData = stMapIT->second;
		if(pData != NULL)
		{
			delete pData;
			pData = NULL;
		}
		++stMapIT;
	}
	m_mapTableList.clear();
}

DWORD CDboWeInfoGroup::GetAttribute(unsigned int uiWorldIndex, float x, float z)
{
	if (m_mapTableList.find(uiWorldIndex) == m_mapTableList.end())
	{
		printf("GetAttribute: world tblidx %d not found \n", uiWorldIndex);
		return 0;
	}

	return m_mapTableList[uiWorldIndex]->GetAttribute(x, z);
}

TBLIDX CDboWeInfoGroup::GetTextAllIndex(const TBLIDX worldTblidx, const float x, const float z)
{
	TABLE::iterator it = m_mapTableList.find(worldTblidx);
	if (it == m_mapTableList.end())
		return INVALID_TBLIDX;
	else
		return it->second->GetTextAllIndex(x, z);

	return INVALID_TBLIDX;
}

ZONEID CDboWeInfoGroup::GetZoneIndex(TBLIDX worldTblidx, float x, float z)
{
	TABLE::iterator it = m_mapTableList.find(worldTblidx);
	if (it == m_mapTableList.end())
		return INVALID_TBLIDX;
	else
		return it->second->GetZoneIndex(x, z);
}

bool CDboWeInfoGroup::CanSee(unsigned int uiWorldIndex, float curX, float curY, float curZ, float desX, float desY, float desZ)
{
	TABLE::iterator it = m_mapTableList.find(uiWorldIndex);
	if (it == m_mapTableList.end())
		return false;
	else
		return it->second->CanSee(curX, curY, curZ, desX, desY, desZ);
}