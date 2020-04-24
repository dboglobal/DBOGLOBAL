#include "StdAfx.h"
#include "WorldPathTable.h"
#include "NtlSerializer.h"
#include <string>


CWorldPathTable::CWorldPathTable(void)
{
}

CWorldPathTable::~CWorldPathTable(void)
{
	m_mapWorldPathTable.clear();
}

bool CWorldPathTable::Create(const char * lpszDataPath)
{
	m_strDataPath = lpszDataPath;

	return true;
}

bool CWorldPathTable::CreateWorldPath(TBLIDX _TblIdx, LPCSTR _pWorldName)
{
	std::string strTheFormer(".\\");
	std::string strFileName(_pWorldName);
	std::string strTheLatter("_PATH.pth");
	std::string strPathFileName;

	strPathFileName = m_strDataPath + strTheFormer + strFileName + strTheLatter;

	FILE* pFile = NULL;
	if(::fopen_s(&pFile, strPathFileName.c_str(), "rb") != 0)
	{
	//	printf("CANT OPEN FILE %s \n", strPathFileName);
		return false;
	}

	sPATH_TABLE sCurPathTable;
	TBLIDX		CurTblIdx;

	::fread(&sCurPathTable._NumPathList, 1, sizeof(unsigned int), pFile);

	for(unsigned int i = 0; i < sCurPathTable._NumPathList; ++i)
	{
		sPATH_POINT sCurPathPoint;
		sPATH_LIST	sCurPathList;

		::fread(&CurTblIdx, 1, sizeof(TBLIDX), pFile);
		::fread(&sCurPathList._fRadius, 1, sizeof(float), pFile);
		::fread(&sCurPathList._NumPathPoint, 1, sizeof(BYTE), pFile);

		sCurPathList._vecPathPoint.reserve( sCurPathList._NumPathPoint );

		for(BYTE j = 0; j < sCurPathList._NumPathPoint; ++j)
		{
			::fread(&sCurPathPoint._Pos.x, sizeof(float), 1, pFile);
			::fread(&sCurPathPoint._Pos.y, sizeof(float), 1, pFile);
			::fread(&sCurPathPoint._Pos.z, sizeof(float), 1, pFile);	
			::fread(&sCurPathPoint._PrevDist, sizeof(float), 1, pFile);

			sCurPathList._vecPathPoint.push_back(sCurPathPoint);
		}

		if(sCurPathTable._mapPathList.insert(sPATH_TABLE::tTABLEVAL(CurTblIdx, sCurPathList)).second == false)
		{
			return false;
		}
	}

	if(m_mapWorldPathTable.insert(CWorldPathTable::tTABLEVAL(_TblIdx, sCurPathTable)).second == false)
	{
		return false;
	}

	::fclose(pFile);

	return true;
}

sPATH_TABLE* CWorldPathTable::FindData(TBLIDX _TblIdx)
{
	if(0 == _TblIdx)
	{
		return NULL;
	}

	tTABLEIT The = m_mapWorldPathTable.find(_TblIdx);
	if(The == m_mapWorldPathTable.end())
	{
		return NULL;
	}

	return static_cast<sPATH_TABLE*>(&The->second); 
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//  [8/21/2007 zeroera]
//-----------------------------------------------------------------------------------
sPATH_LIST * CWorldPathTable::FindPathList(TBLIDX worldTblidx, TBLIDX pathTblidx)
{
	sPATH_TABLE * pPathTable = FindData( worldTblidx );
	if( NULL == pPathTable )
	{
		return NULL;
	}

	return pPathTable->FindData( pathTblidx );
}


unsigned int CWorldPathTable::GetNumPathTable()
{
	return static_cast<unsigned int>(m_mapWorldPathTable.size());
}
