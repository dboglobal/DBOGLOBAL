#include "stdafx.h"
#include "Npc.h"
#include "ObjectManager.h"
#include "NtlRandom.h"
#include "BotAiAction_NavMove.h"
#include "BotAiState.h"
#include "GameServer.h"


CBotBreakUp::CBotBreakUp()
{
	m_pOwnerChar = NULL;
	m_byOldMaxMobCount = 12;
	m_byCurMobCount = 12;
	m_fRed = 0.0f;
	Init();
}

CBotBreakUp::~CBotBreakUp()
{
	Destroy();
}


bool CBotBreakUp::Create(CCharacter* pChar)
{
	m_pOwnerChar = pChar;
	Reset();
	InitRadian();
	return true;
}

void CBotBreakUp::Destroy()
{
	m_pOwnerChar = NULL;
}

void CBotBreakUp::Init()
{
	m_nBreakUpSlot = -1;
	for (int j = 0; j < 16; j++)
		m_ahBreakup[j] = -1;
}

void CBotBreakUp::Reset()
{
	m_vecBotSave.clear();
	m_vecRadian.clear();
	m_vecBotSave.reserve(m_byOldMaxMobCount);
	m_vecRadian.reserve(m_byOldMaxMobCount);
	for (int j = 0; j < m_byOldMaxMobCount; j++)
	{
		m_vecBotSave.push_back(INVALID_HOBJECT);
		m_vecRadian.push_back(0.0f);
	}
}

bool CBotBreakUp::Reserve(BYTE byCurBotCount)
{
	if (m_vecBotSave.size() < INVALID_BYTE)
	{
		if (byCurBotCount == m_byOldMaxMobCount)
			return true;

		m_vecBotSave.reserve(byCurBotCount);
		m_vecRadian.reserve(byCurBotCount);

		BYTE byNewBotCount = byCurBotCount - m_byOldMaxMobCount;
		m_byOldMaxMobCount = byCurBotCount;

		for (int j = 0; byNewBotCount > j; j++)
		{
			m_vecBotSave.push_back(INVALID_HOBJECT);
			m_vecRadian.push_back(0.0f);
		}

		return true;
	}
	
	//this error happen when too many objects follow one target.
	//ERR_LOG(LOG_USER, "[Handle : %u] fail : INVALID_BYTE <= BYTE( m_vecBotSave.size() %u ), pSpawnObject->GetCurLoc() = (%f, %f, %f), World ID = %u pSpawnObject->GetObjType() = %u",
	//	m_pOwnerChar->GetID(), m_vecBotSave.size(), m_pOwnerChar->GetCurLoc().x, m_pOwnerChar->GetCurLoc().y, m_pOwnerChar->GetCurLoc().z, m_pOwnerChar->GetWorldID(), m_pOwnerChar->GetObjType());

	printf("INVALID_BYTE <= BYTE( m_vecBotSave.size() %u ) \n", (BYTE)m_vecBotSave.size());

	return false;
}


bool CBotBreakUp::AddNavMove(CNpc *pBotNpc, float fAttackRange, BYTE byBotCount, CBotBreakUp::eBREAKUP_MOVETYPE eMoveType)
{
	switch (eMoveType)
	{
		case BREAKUP_MOVETYPE_LEVEL1:
		{
			return AddMove_Level1(pBotNpc, fAttackRange, byBotCount);
		}
		break;
		case BREAKUP_MOVETYPE_LEVEL2:
		{
			return AddMove_Level2(pBotNpc, fAttackRange, byBotCount);
		}
		break;
		case BREAKUP_MOVETYPE_LEVEL3:
		{
			return AddMove_Level3(pBotNpc, fAttackRange, byBotCount);
		}
		break;
		case BREAKUP_MOVETYPE_LEVEL4:
		{
			return AddMove_Level4(pBotNpc, fAttackRange, byBotCount);
		}
		break;
		case BREAKUP_MOVETYPE_CHASE:
		{
			return AddMove_Chase(pBotNpc, fAttackRange, byBotCount);
		}
		break;

		default:{}
	}

	return false;
}


void CBotBreakUp::ResetPosition()
{
	if (m_pOwnerChar)
	{
		if (m_pOwnerChar->GetMeHaterManager()->GetContainerMeHater()->GetMeHaterCount() > 1)
		{
			struct sFOREACH
			{
				CBotBreakUp*	pBotBreakUp;
				CNtlVector		vTemp;

				void operator () (CCharacterObject* pBot)
				{
					if (pBot)
						pBotBreakUp->GetPosition((CNpc*)pBot, vTemp, pBotBreakUp->m_byCurMobCount);
				}
			};

			AllUnsetBreakUp();

			sFOREACH sForEach;
			sForEach.pBotBreakUp = this;

			m_pOwnerChar->GetMeHaterManager()->ForEachNearRange(sForEach);
		}
	}
}


void CBotBreakUp::UnsetBreakUp(HOBJECT hEnemy)
{
	if (hEnemy != INVALID_HOBJECT)
	{
		for (int j = 0; j < m_vecBotSave.size(); j++)
		{
			if (m_vecBotSave[j] == hEnemy)
			{
				m_vecBotSave[j] = INVALID_HOBJECT;
				return;
			}
		}
	}
}


bool CBotBreakUp::GetPosition(CNpc *pBotNpc, std::vector<CNtlVector>& rDir, BYTE *byDirCount, const BYTE byBotCount)
{
	if (pBotNpc && m_pOwnerChar)
	{
		UnsetBreakUp(pBotNpc->GetID());
		Reserve(byBotCount);
		CBotBreakUp::eDIR eDir;

		BYTE byPos = GetArrayPosition(pBotNpc, eDir);
		if (byPos == INVALID_BYTE)
		{
			return false;
		}
		else if (IsNullPos(byPos))
		{
			SetBreakUp(pBotNpc->GetID(), byPos);
			rDir[0].operator=(GetRadianToCoordinates(rDir[0], eDir, m_vecRadian[byPos]));

			*byDirCount = 1;
			return true;
		}
		else
		{
			int nPosIdx = byPos;
			int nLeftTimePosIdx = (m_byCurMobCount >> 1) + 1;
			int v43 = 1;
			int v46 = 0;
			int v47 = 0;

			for (int j = 1; j < nLeftTimePosIdx; j++)
			{
				if (j % 2)
					v43 = j;
				else
					v43 = -j;

				nPosIdx += v43;

				if (v43 <= 0)
				{
					if (nPosIdx < 0)
						nPosIdx += m_byCurMobCount;

					v47 = nPosIdx;
				}
				else
				{
					if (m_byCurMobCount <= nPosIdx)
						nPosIdx -= m_byCurMobCount;
					v46 = nPosIdx;
				}

				if (IsNullPos(nPosIdx) == true)
				{
					SetBreakUp(pBotNpc->GetID(), nPosIdx);
					rDir[0].operator=(GetRadianToCoordinates(rDir[0], GetCoordinates(nPosIdx), m_vecRadian[nPosIdx]));
					*byDirCount = 1;
					return true;
				}
			}

			int v49 = -1;
			int v50 = 0;
			int v51 = v47 - 1;

			int nLoopCount = 0;


			while (v46 != v51)
			{
				++nLoopCount;
				if (nLoopCount > 5000)
				{
					ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
				}

				if (v51 < 0)
				{
					v51 = m_byCurMobCount - 1;
					if (v46 == v51)
					{
						if (IsNullPos(v51) == 1)
							v49 = v51;
						break;
					}
				}
				if (IsNullPos(v51) == 1)
				{
					v49 = v51;
					break;
				}
				--v51;
				++v50;
			}

			nLoopCount = 0; //reset loop count

			if (v49 >= 0)
			{
				int v52 = -1;
				int v53 = 0;
				int v54 = v46 + 1;
				while (v47 != v54)
				{
					++nLoopCount;
					if (nLoopCount > 5000)
					{
						ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
					}

					if (m_byCurMobCount <= v54)
					{
						v54 = 0;
						if (!v47)
						{
							if (IsNullPos(v54) == 1)
								v52 = v54;
							break;
						}
					}
					if (IsNullPos(v54) == 1)
					{
						v52 = v54;
						break;
					}
					++v54;
					++v53;
				}
				if (v50 <= v53)
				{
					if (v50 >= v53)
					{
						SetBreakUp(pBotNpc->GetID(), v49);
						rDir[0].operator=(GetRadianToCoordinates(rDir[0], GetCoordinates(v49), m_vecRadian[v49]));
						rDir[1].operator=(GetRadianToCoordinates(rDir[1], GetCoordinates(v47), m_vecRadian[v47]));

						*byDirCount = 2;
						return true;
					}
					else
					{
						SetBreakUp(pBotNpc->GetID(), v49);
						rDir[0].operator=(GetRadianToCoordinates(rDir[0], GetCoordinates(v49), m_vecRadian[v49]));
						rDir[1].operator=(GetRadianToCoordinates(rDir[1], GetCoordinates(v47), m_vecRadian[v47]));

						*byDirCount = 2;
						return true;
					}
				}
				else
				{
					SetBreakUp(pBotNpc->GetID(), v52);
					rDir[0].operator=(GetRadianToCoordinates(rDir[0], GetCoordinates(v52), m_vecRadian[v52]));
					rDir[1].operator=(GetRadianToCoordinates(rDir[1], GetCoordinates(v46), m_vecRadian[v46]));

					*byDirCount = 2;
					return true;
				}
			}

		}
	}

	return false;
}

bool CBotBreakUp::GetPosition(CNpc *pBotNpc, CNtlVector& rDir, const BYTE byBotCount)
{
	if (pBotNpc && m_pOwnerChar)
	{
		UnsetBreakUp(pBotNpc->GetID());
		Reserve(byBotCount);
		CBotBreakUp::eDIR eDir;

		BYTE byPos = GetArrayPosition(pBotNpc, eDir);
		if (byPos == INVALID_BYTE)
		{
			return false;
		}
		else if (IsNullPos(byPos))
		{
			SetBreakUp(pBotNpc->GetID(), byPos);
			rDir = GetRadianToCoordinates(rDir, eDir, m_vecRadian[byPos]);

			return true;
		}
		else
		{
			int nPosOffset = 1;
			int nPosIdx = byPos;

			for (int j = 1; byBotCount > j; j++)
			{
				if (j % 2)
					nPosOffset = j;
				else
					nPosOffset = -j;

				nPosIdx += nPosOffset;

				if (nPosOffset <= 0)
				{
					if (nPosIdx < 0)
						nPosIdx += m_byCurMobCount;
				}
				else if (m_byCurMobCount <= nPosIdx)
				{
					nPosIdx -= m_byCurMobCount;
				}
				if (IsNullPos(nPosIdx) == true)
				{
					SetBreakUp(pBotNpc->GetID(), nPosIdx);
					rDir = GetRadianToCoordinates(rDir, GetCoordinates(nPosIdx), m_vecRadian[nPosIdx]);
					return true;
				}
			}
		}
	}

	return false;
}


CBotBreakUp::eDIR CBotBreakUp::GetCoordinates(int nPosIdx)
{
	return (CBotBreakUp::eDIR)(int)(nPosIdx / ((float)m_byCurMobCount / 4.0f));
}


CNtlVector& CBotBreakUp::GetRadianToCoordinates(CNtlVector& pRes, CBotBreakUp::eDIR eDir, float fRadian)
{
	CNtlVector vDir;

	switch (eDir)
	{
		case DIR_1:
		{
			vDir.x = cos(fRadian);
			vDir.z = sin(fRadian);
		}
		break;
		case DIR_2:
		{
			vDir.x = sin(fRadian - 1.5707964f) * -1.0f;
			vDir.z = cos(fRadian - 1.5707964f);
		}
		break;
		case DIR_3:
		{
			vDir.x = (float)(cos(fRadian - 3.1415927) * -1.0f);
			vDir.z = (float)(sin(fRadian - 3.1415927) * -1.0f);
		}
		break;
		case DIR_4:
		{
			vDir.x = (float)(sin(fRadian - 4.712389f));
			vDir.z = (float)(cos(fRadian - 4.712389f) * -1.0);
		}
		break;
		default: break;
	}

	vDir.x = vDir.x * -1.0f;
	vDir.y = 0.0f;

	pRes = vDir;

	return pRes;
}

BYTE CBotBreakUp::GetArrayPosition(CNpc *pBotNpc, CBotBreakUp::eDIR& rDir)
{
	if (pBotNpc && m_pOwnerChar)
	{
		CNtlVector vDiff(pBotNpc->GetCurLoc() - m_pOwnerChar->GetCurLoc());
		float fRadian = (float)(sqrt((vDiff.x * (-1.0)) * (vDiff.x * (-1.0)) + (vDiff.z * vDiff.z)));
		float fRed = vDiff.z / fRadian;

		float fX = vDiff.x * (-1.0f);

		if (fRed < 0.0f)
			fRed *= -1.0f;
		
		if (fX < 0.0f || vDiff.z < 0.0f)
		{
			if (fX  > 0.0f || vDiff.z < 0.0f)
			{
				if (fX  > 0.0f || vDiff.z > 0.0f)
				{
					rDir = DIR_4;
					fRed = 1.0f - fRed;
					fRed += 3.0f;
				}
				else
				{
					rDir = DIR_3;
					fRed += 2.0;
				}
			}
			else
			{
				rDir = DIR_2;
				fRed = 1.0f - fRed;
				fRed += 1.0f;
			}
		}
		else
		{
			rDir = DIR_1;
		}

		BYTE byIndex = BYTE(rDir / (4.0f / (float)m_byCurMobCount));
		if (m_vecBotSave.size() <= byIndex)
			byIndex = (BYTE)(m_vecBotSave.size() - 1);

		return byIndex;
	}

	return INVALID_BYTE;
}

bool CBotBreakUp::IsNullPos(BYTE byArrayPos)
{
	return m_vecBotSave[byArrayPos] == INVALID_HOBJECT;
}


void CBotBreakUp::SetBreakUp(HOBJECT hEnemy, BYTE byArrayPos)
{
	m_vecBotSave[byArrayPos] = hEnemy;
}


void CBotBreakUp::AllUnsetBreakUp()
{
	for (int j = 0; m_byCurMobCount > j; j++)
		m_vecBotSave[j] = INVALID_HOBJECT;
}


void CBotBreakUp::InitRadian()
{
	m_fRed = (360.0f / (float)m_byCurMobCount) * 0.017453292f;
	float fTemp = ((360.0f / (float)m_byCurMobCount) / 2.0f) * 0.017453292f;

	for (int j = 0; m_byCurMobCount > j; j++)
	{
		m_vecRadian[j] = fTemp;
		fTemp += m_fRed;
	}
}


int CBotBreakUp::GetChaseSlot()
{
	return m_nBreakUpSlot;
}

void CBotBreakUp::SetChaseSlot(int nBreakUpSlot)
{
	this->m_nBreakUpSlot = nBreakUpSlot;
}
//NEED EDIT.. NOT USED
int CBotBreakUp::CalcBreakUpSlot(CCharacter* pChar)
{
	CNtlVector vAttackerDirection(pChar->GetCurLoc().operator-(m_pOwnerChar->GetCurLoc()));
	int nAngle = (int)CalcDboAngle(&vAttackerDirection, &CNtlVector::UNIT_Z);
	nAngle = nAngle % 360;
	int nCheckSlot = (int)(2 * (nAngle / 45.0f));

	int nSlot = -1;

	if (nCheckSlot < 16)
	{
		for (int j = 0; j < 4; j++)
		{
			nSlot = (nCheckSlot + 2 * j) % -16;

			/*if (*((_DWORD *)v25 + nSlot + 26) == -1)
				return nSlot;

			if (pChar->GetID() == *((_DWORD *)v25 + nSlot + 26))
				return nSlot;

			nSlot = (nCheckSlot - 2 - 2 * j + 16) % -16;

			if (*((_DWORD *)v25 + v16 + 26) != -1)
			{
				if (pChar->GetID() != *((_DWORD *)v25 + nSlot + 26))
					continue;
			}*/
			return nSlot;
		}

		for (int k = 0; k < 4; k++)
		{
			/*v16 = (v15 + 2 * k + 1) % -16;
			if (*((_DWORD *)v25 + v16 + 26) == -1)
				return (unsigned int)v16;
			v10 = CGameObject::GetHandle((CGameObject *)&v26->vfptr);
			if (v10 == *((_DWORD *)v25 + v16 + 26))
				return (unsigned int)v16;
			v16 = (v15 - 1 - 2 * k + 16) % -16;
			if (*((_DWORD *)v25 + v16 + 26) != -1)
			{
				v11 = CGameObject::GetHandle((CGameObject *)&v26->vfptr);
				if (v11 != *((_DWORD *)v25 + v16 + 26))
					continue;
			}
			return (unsigned int)v16;*/
		}
	}

	return -1;
}

void CBotBreakUp::SetBreakUpSlot(int nSlot, CCharacter* pChar)
{
	int nOldSlot = pChar->GetBotBreakUp()->GetChaseSlot();
	if (nOldSlot != -1)
	{
		if (pChar->GetID() == m_ahBreakup[nOldSlot])
			m_ahBreakup[nOldSlot] = -1;
	}

	m_ahBreakup[nSlot] = pChar->GetID();
	pChar->GetBotBreakUp()->SetChaseSlot(nSlot);
}

CNtlVector CBotBreakUp::GetBreakUpLocation(CNtlVector& rVecRes, int nBreakUpSlot, CCharacter* pAttacker)
{
	float fAttackRange = pAttacker->GetObjectRadius() + ((pAttacker->CCharacterObject::GetAttackRange() * 70.0f) / 100.0f) + m_pOwnerChar->GetObjectRadius();
	float fRadian = (((float)nBreakUpSlot * 360.0f) / 16.0f) * 0.017453292f;

	CNtlVector vNewLocation;
	CNtlVector vTemp(0.0f, 0.0f, fAttackRange);
		
	RotateVector(vNewLocation, &vTemp, fRadian * 0.017453292f);

	return m_pOwnerChar->GetCurLoc() + vNewLocation;
}


bool CBotBreakUp::AddMove_Level1(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount)
{
	return true;
}

bool CBotBreakUp::AddMove_Level2(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount)
{
	CGameServer* app = (CGameServer*)g_pApp;

	printf("AddMove_Level2 not sure if works correct \n");
	CBotAiState* pCurState = pBotNpc->GetBotController()->GetCurrentState();
	if (pCurState)
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(pBotNpc->GetTargetHandle());
		if (pTarget && pTarget->IsInitialized())
		{
			float fRange = pBotNpc->GetAttackRange(pTarget);
			if (fRange > 2.0f)
				fRange /= 2.5f;

			float fDestX = RandomRangeF(pTarget->GetCurLoc().x - fRange, pTarget->GetCurLoc().x + fRange);
			float fDestZ = RandomRangeF(pTarget->GetCurLoc().z - fRange, pTarget->GetCurLoc().z + fRange);

			CNtlVector vDir(pBotNpc->GetCurLoc());
			vDir.x = fDestX;
			vDir.z = fDestZ;
			vDir.operator-=(pTarget->GetCurLoc());
			vDir.y = 0.0f;

			if (!vDir.SafeNormalize())
				return true;

			if (app->m_config.m_bEnableNavigator)
			{
				pBotNpc->GetPathFinder()->Reset();
			}

			float fOffset = fRange * 70.0f;

			CNtlVector rDestLoc(pTarget->GetCurLoc().operator+(vDir.operator*(fRange + pBotNpc->GetObjectRadius() + pTarget->GetObjectRadius())));

			CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(pBotNpc, rDestLoc, true, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
			if (pCurState->AddSubControlQueue(pNavMove, true))
				return true;
		}
	}

	return false;
}

bool CBotBreakUp::AddMove_Level3(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount)
{
	printf("CBotBreakUp::AddMove_Level3 TODO \n");
	if (pBotNpc->GetBotController()->GetCurrentState())
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(pBotNpc->GetTargetHandle());
		if (pTarget && pTarget->IsInitialized())
		{

		}
	}

	return false;
}

bool CBotBreakUp::AddMove_Level4(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount)
{
	printf("CBotBreakUp::AddMove_Level4 TODO \n");
	if (pBotNpc->GetBotController()->GetCurrentState())
	{
		CCharacter* pTarget = g_pObjectManager->GetChar(pBotNpc->GetTargetHandle());
		if (pTarget && pTarget->IsInitialized())
		{

		}
	}

	return false;
}

bool CBotBreakUp::AddMove_Chase(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount)
{
	if (pBotNpc)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		if (pBotNpc->GetBotController()->GetCurrentState())
		{
			if (byBotCount >= 12)
			{
				if (byBotCount == INVALID_BYTE)
					byBotCount = m_byCurMobCount;
				else
					m_byCurMobCount = byBotCount;
			}
			else
			{
				m_byCurMobCount = 12;
			}

			CNtlVector vDir;
			if (GetPosition(pBotNpc, vDir, m_byCurMobCount))
			{
				vDir.y = 0.0f;
				if (vDir.SafeNormalize())
				{
					if (app->m_config.m_bEnableNavigator)
					{
						pBotNpc->GetPathFinder()->Reset();
					}

					float fDistance = (fAttackRange * 70.0f) / 100.0f;
					CNtlVector rDestLoc(m_pOwnerChar->GetCurLoc().operator+(vDir.operator*(fDistance)));

					return Following(pBotNpc, rDestLoc);
				}
				else
				{
					return true;
				}
			}
			else
			{
				return AddMove_Level2(pBotNpc, fAttackRange, byBotCount);
			}
		}
	}

	return false;
}

bool CBotBreakUp::Following(CNpc* pBotNpc, CNtlVector& rDestLoc)
{
	pBotNpc->GetPathFinder()->SetDestLoc(rDestLoc);

	CBotPathFinder::ePATH_FINDING_RESULT nPathFindingResult = pBotNpc->GetPathFinder()->PathFind();

	if (nPathFindingResult == CBotPathFinder::PATH_FINDING_RESULT_FAILED)
		return false;

	BYTE byNextLocCount = 0;
	sVECTOR3 avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
	if (nPathFindingResult == CBotPathFinder::PATH_FINDING_RESULT_CACHE_HIT || nPathFindingResult == CBotPathFinder::PATH_FINDING_RESULT_NEW_RESULT)
	{
		byNextLocCount = pBotNpc->GetPathFinder()->GetAllNextNavLoc(avDestLoc);
		//printf("CBotBreakUp::Following \n");
		//pBotNpc->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, INVALID_BYTE, byNextLocCount, avDestLoc, true);

		pBotNpc->SendCharStateFollowing(pBotNpc->GetTargetHandle(), pBotNpc->GetAttackFollowRange(), DBO_MOVE_FOLLOW_AUTO_ATTACK, avDestLoc[0], true);

		return true;
	}

	ERR_LOG(LOG_SYSTEM, "The value of \"nPathFindingResult\" is not allowed here., nPathFindingResult = %d", nPathFindingResult);
	return false;
}









