#include "stdafx.h"
#include "BotPathFinder.h"
#include "char.h"
#include "ObjectManager.h"
#include "NtlNavi.h"


CBotPathFinder::CBotPathFinder()
{
	Init();
}

CBotPathFinder::~CBotPathFinder()
{
	Destroy();
}


bool CBotPathFinder::Create(CCharacter* pBot, bool bUsePathFinding)
{
	if (pBot)
	{
		m_pOwnerRef = pBot;
		m_bUsePathFinding = bUsePathFinding;
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pBot == NULL");
	return false;
}


void CBotPathFinder::Destroy()
{
}

void CBotPathFinder::Reset()
{
	m_hTarget = INVALID_HOBJECT;
	m_vDestLoc.Reset();
	m_bCachedResult = false;
	m_vCachedLoc.x = INVALID_FLOAT;
	m_vCachedLoc.y = INVALID_FLOAT;
	m_vCachedLoc.z = INVALID_FLOAT;
	m_hChachedTarget = INVALID_HOBJECT;
	m_dwCurNavStep = INVALID_DWORD;
}

void CBotPathFinder::SetDestLoc(CNtlVector& rLoc)
{
	m_vDestLoc = rLoc;
	m_hTarget = INVALID_HOBJECT;
}

void CBotPathFinder::SetTarget(CCharacter* pBot)
{
	if (pBot)
	{
		m_hTarget = pBot->GetID();
		m_vDestLoc = pBot->GetCurLoc();
	}
	else
		ERR_LOG(LOG_BOTAI, "pBot == NULL");
}

void CBotPathFinder::SetWayPointData(sPATH_LIST* pPathData)
{
	m_wayPointList.Create(pPathData);
}

bool CBotPathFinder::IsArrived()
{
	return m_vDestLoc == m_pOwnerRef->GetCurLoc();
}

bool CBotPathFinder::GetCurWayPoint(CNtlVector& rLoc)
{
	return m_wayPointList.GetCurWayPoint(rLoc);
}

bool CBotPathFinder::GetNextWayPoint(CNtlVector& rLoc)
{
	return m_wayPointList.GetNextWayPoint(rLoc);
}

bool CBotPathFinder::GetCurNavLoc(CNtlVector& rLoc)
{
	if (m_bUsePathFinding)
	{
		if (m_dwCurNavStep < m_vecPosList.size())
		{
			sNAVI_POS& svPos = m_vecPosList[m_dwCurNavStep];
			rLoc.x = svPos.x;
			rLoc.y = svPos.y;
			rLoc.z = svPos.z;
			return true;
		}
	}
	else
	{
		rLoc = m_vDestLoc;
		return true;
	}

	return false;
}

bool CBotPathFinder::GetNextNavLoc(CNtlVector& rLoc)
{
	if (m_bUsePathFinding)
	{
		if (m_dwCurNavStep++ < m_vecPosList.size())
		{
			sNAVI_POS& svPos = m_vecPosList[m_dwCurNavStep];
			rLoc.x = svPos.x;
			rLoc.y = svPos.y;
			rLoc.z = svPos.z;
			return true;
		}
		else
		{
			m_dwCurNavStep = (DWORD)m_vecPosList.size();
			return false;
		}
	}
	else
	{
		rLoc = m_vDestLoc;
		return true;
	}
	
	return false;
}

BYTE CBotPathFinder::GetAllNextNavLoc(sVECTOR3* pavLoc)
{
	if (pavLoc)
	{
		if (m_bUsePathFinding)
		{
			if (m_vecPosList.size() > m_dwCurNavStep)
			{
				BYTE byLocations = 0;
				for (DWORD _Pos = m_dwCurNavStep; ; _Pos++)
				{
					if (_Pos >= m_vecPosList.size())
						break;

					sNAVI_POS& loc = m_vecPosList[_Pos];

					pavLoc[byLocations].x = loc.x;
					pavLoc[byLocations].y = loc.y;
					pavLoc[byLocations++].z = loc.z;

					if (byLocations >= DBO_MAX_NEXT_DEST_LOC_COUNT)
						break;
				}
				return byLocations;
			}
		}
		else
		{
			m_vDestLoc.CopyTo(pavLoc[0]);
			return 1;
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "pavLoc == invalid");
	}

	return 0;
}

CBotPathFinder::ePATH_FINDING_RESULT CBotPathFinder::PathFind()
{
	if (m_bUsePathFinding == false)
	{
		return PATH_FINDING_RESULT_CACHE_HIT;
	}
		
	if (m_bCachedResult && m_vCachedLoc == m_vDestLoc)
	{
		return PATH_FINDING_RESULT_CACHE_HIT;
	}

	if (m_hTarget != INVALID_HOBJECT)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
		if (pTarget == NULL || !pTarget->IsInitialized())
			return PATH_FINDING_RESULT_FAILED;

		if (m_hChachedTarget == m_hTarget && pTarget->IsInRange(m_vCachedLoc, m_pOwnerRef->GetAttackFollowRange()))
		{
			return PATH_FINDING_RESULT_CACHE_HIT;
		}
	}

	m_hChachedTarget = m_hTarget;
	m_vCachedLoc = m_vDestLoc;
	m_bCachedResult = false;

	if (m_pOwnerRef->GetCurWorld())
	{
		sNAVI_POS vParentCurLoc(m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z);
		sNAVI_POS dest(m_vDestLoc.x, m_vDestLoc.y, m_vDestLoc.z);

		float fSubjectAgentRadius = Dbo_ConvertToAgentRadius(m_pOwnerRef->GetObjectRadius());

		if (GetNaviEngine()->IsPathDataLoaded(m_pOwnerRef->GetCurWorld()->GetTbldat()->dwWorldResourceID))
		{
			sNAVI_POS sFirstCollision;

			eCOL_TEST_RESULT eResult = GetNaviEngine()->FastFirstCollisionTest(m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, vParentCurLoc, dest, sFirstCollision);
			switch (eResult)
			{
				case eCOL_TEST_RESULT_INVALID_DEST_POS:
				case eCOL_TEST_RESULT_COL:
				{
					if (GetNaviEngine()->FastFindPath(m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, vParentCurLoc, dest, m_vecPosList) == false)
					{
						return PATH_FINDING_RESULT_FAILED;
					}
				}
				break;

				case eCOL_TEST_RESULT_NO_COL:
				{
					m_vecPosList.clear();
					m_vecPosList.push_back(vParentCurLoc);
					m_vecPosList.push_back(dest);
				}
				break;

				default: return PATH_FINDING_RESULT_FAILED; break;
			}
		}
		else
		{
			m_vecPosList.clear();
			m_vecPosList.push_back(vParentCurLoc);
			m_vecPosList.push_back(dest);
		}

		if (m_vecPosList.size() >= 2)
		{
			m_bCachedResult = true;
			m_dwCurNavStep = 1;
			return PATH_FINDING_RESULT_NEW_RESULT;
		}
		else
		{
			m_vecPosList.clear();
		}
	}

	return PATH_FINDING_RESULT_FAILED;
}

bool CBotPathFinder::PathDirectFind()
{
	return true;
}

CBotPathFinder::ePATH_FINDING_RESULT CBotPathFinder::WanderPathFind()
{
	if (m_bUsePathFinding == false)
	{
		return PATH_FINDING_RESULT_CACHE_HIT;
	}

	if (m_bCachedResult && m_vCachedLoc == m_vDestLoc)
	{
		return PATH_FINDING_RESULT_CACHE_HIT;
	}

	if (m_hTarget != INVALID_HOBJECT)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
		if (pTarget == NULL || !pTarget->IsInitialized())
			return PATH_FINDING_RESULT_FAILED;

		if (m_hChachedTarget == m_hTarget && pTarget->IsInRange(m_vCachedLoc, m_pOwnerRef->GetAttackFollowRange()))
			return PATH_FINDING_RESULT_CACHE_HIT;
	}

	m_hChachedTarget = m_hTarget;
	m_vCachedLoc = m_vDestLoc;
	m_bCachedResult = false;

	if (m_pOwnerRef->GetCurWorld())
	{
		sNAVI_POS src(m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z);
		sNAVI_POS dest(m_vDestLoc.x, m_vDestLoc.y, m_vDestLoc.z);

		float fSubjectAgentRadius = Dbo_ConvertToAgentRadius(m_pOwnerRef->GetObjectRadius());

		if (GetNaviEngine()->IsPathDataLoaded(m_pOwnerRef->GetCurWorld()->GetTbldat()->dwWorldResourceID))
		{
			sNAVI_POS sFirstCollision;

			eCOL_TEST_RESULT eResult = GetNaviEngine()->FastFirstCollisionTest(m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, src, dest, sFirstCollision);
			switch (eResult)
			{
				case eCOL_TEST_RESULT_INVALID_SRC_POS:
				{
					return PATH_FINDING_RESULT_SRC_FAILED;
				}
				break;

				case eCOL_TEST_RESULT_COL:
				{
					m_vecPosList.clear();
					m_vecPosList.push_back(src);
					m_vecPosList.push_back(sFirstCollision);
				}
				break;

				case eCOL_TEST_RESULT_NO_COL:
				{
					float fNewY = 0.0f;

					long lLong = GetNaviEngine()->GetFastHeight(m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle(), dest.x, dest.y, dest.z, fNewY, 5000);

					if(lLong != 0xFFFFFFFF)
					{
						dest.y = fNewY;

						m_vecPosList.clear();
						m_vecPosList.push_back(src);
						m_vecPosList.push_back(dest);
					}
					else
					{
						ERR_LOG(LOG_SYSTEM, "can not find Navi Heigth. tblidx[%u] ResourceId[%u] x:y:z[%0.3f:%0.3f:%0.3f]",
							m_pOwnerRef->GetCurWorld()->GetIdx(), m_pOwnerRef->GetCurWorld()->GetTbldat()->dwWorldResourceID, dest.x, dest.y, dest.z);
					}
				}
				break;

				default: return PATH_FINDING_RESULT_FAILED; break;
			}
		}
		else
		{
			m_vecPosList.clear();
			m_vecPosList.push_back(src);
			m_vecPosList.push_back(dest);
		}

		if (m_vecPosList.size() >= 2)
		{
			m_bCachedResult = true;
			m_dwCurNavStep = 1;
			return PATH_FINDING_RESULT_NEW_RESULT;
		}
		else
		{
			m_vecPosList.clear();
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "The bot doesn't belong to any world.(NULL == m_pOwnerRef->GetSectree())");
	}

	return PATH_FINDING_RESULT_FAILED;
}

bool CBotPathFinder::IsValidPos(CNtlVector& rLoc)
{
	if (m_pOwnerRef->GetCurWorld())
	{
		if (GetNaviEngine()->IsPathDataLoaded(m_pOwnerRef->GetCurWorld()->GetTbldat()->dwWorldResourceID))
		{
			sNAVI_POS sNaviPos(m_pOwnerRef->GetCurLoc().x, m_pOwnerRef->GetCurLoc().y, m_pOwnerRef->GetCurLoc().z);

			return GetNaviEngine()->IsValidPos(m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle(), sNaviPos);
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool CBotPathFinder::GetNearestPos(CNtlVector& rLoc)
{
	if (m_pOwnerRef->GetCurWorld())
	{
		if (GetNaviEngine()->IsPathDataLoaded(m_pOwnerRef->GetCurWorld()->GetTbldat()->dwWorldResourceID))
		{
			float fSubjectAgentRadius = Dbo_ConvertToAgentRadius(m_pOwnerRef->GetObjectRadius());
			int nClosestRange = (int)(fSubjectAgentRadius * 100);
			sNAVI_POS src(rLoc.x, rLoc.y, rLoc.z);

			return GetNaviEngine()->FastFindNearestPos(m_pOwnerRef->GetCurWorld()->GetNaviInstanceHandle(), fSubjectAgentRadius, src, 1, nClosestRange);
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool CBotPathFinder::IsCachedMoveNearestDest(CCharacter* pBot)
{
	return false;
}



void CBotPathFinder::Init()
{
	m_pOwnerRef = NULL;

	Reset();

	m_sCachedMoveNearestDest.bCached = false;
	m_sCachedMoveNearestDest.worldTblidx = INVALID_TBLIDX;
	m_sCachedMoveNearestDest.sSrcPos.x = 0.0f;
	m_sCachedMoveNearestDest.sSrcPos.y = 0.0f;
	m_sCachedMoveNearestDest.sSrcPos.z = 0.0f;

	m_sCachedMoveNearestDest.hTarget = INVALID_HOBJECT;

	m_sCachedMoveNearestDest.sDestPos.x = 0.0f;
	m_sCachedMoveNearestDest.sDestPos.y = 0.0f;
	m_sCachedMoveNearestDest.sDestPos.z = 0.0f;

	m_sCachedMoveNearestDest.sNearestDest.x = 0.0f;
	m_sCachedMoveNearestDest.sNearestDest.y = 0.0f;
	m_sCachedMoveNearestDest.sNearestDest.z = 0.0f;
}

