#include "stdafx.h"
#include "WayPointList.h"
#include "TableContainerManager.h"


CWayPointList::CWayPointList()
{
	m_pPathData = NULL;
	m_byCurStep = 0;
}

CWayPointList::~CWayPointList()
{
	Destroy();
}


bool CWayPointList::Create(sPATH_LIST* pPathData)
{
	if (pPathData)
	{
		m_pPathData = pPathData;
		m_byCurStep = 0;
		return true;
	}

	return false;
}

bool CWayPointList::Create(TBLIDX worldTblidx, TBLIDX pathTblidx)
{
	sPATH_LIST* pPathData = (sPATH_LIST*)g_pTableContainer->GetWorldPathTable()->FindPathList(worldTblidx, pathTblidx);
	if (pPathData)
		return Create(pPathData);

	//ERR_LOG(LOG_BOTAI, "The path data couldn't be found., worldTblidx = %u, pathTblidx = %u", worldTblidx, pathTblidx);

	return false;
}

void CWayPointList::Destroy()
{
}

bool CWayPointList::GetCurWayPoint(CNtlVector& rLoc)
{
	if (m_pPathData)
	{
		if (m_byCurStep < m_pPathData->_NumPathPoint)
		{
			rLoc = m_pPathData->_vecPathPoint[m_byCurStep]._Pos;
			return true;
		}
	}

	return false;
}

bool CWayPointList::GetNextWayPoint(CNtlVector& rLoc)
{
	if (m_pPathData)
	{
		if (++m_byCurStep < m_pPathData->_NumPathPoint)
		{
			rLoc = m_pPathData->_vecPathPoint[m_byCurStep]._Pos;
			return true;
		}

		m_byCurStep = 0;
	}

	return false;
}

bool CWayPointList::GetSecondNextWayPoint(CNtlVector& rLoc)
{
	if (m_pPathData)
	{
		BYTE byStep = m_byCurStep + 1;
		if (byStep < m_pPathData->_NumPathPoint)
		{
			rLoc = m_pPathData->_vecPathPoint[byStep]._Pos;
			return true;
		}
	}

	return false;
}

float CWayPointList::GetWayPointRadius()
{
	if (m_pPathData)
	{
		return m_pPathData->_fRadius;
	}

	return 0.0f;
}

bool CWayPointList::IsEmpty()
{
	return m_pPathData == NULL;
}

void CWayPointList::InitStep()
{
	this->m_byCurStep = 0;
}


void CWayPointList::Init()
{
	this->m_pPathData = NULL;
	this->m_byCurStep = 0;
}