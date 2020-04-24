#include "stdafx.h"
#include "ContainerActionPatternSet.h"
#include "TableContainerManager.h"
#include "MobMovePatternTable.h"
#include "FactoryActionPatternUnit.h"


CContainerActionPatternSet::CContainerActionPatternSet()
{
	m_bIsDestroy = false;
	m_mapPattern.clear();
	m_mapActionPattern.clear();
}

CContainerActionPatternSet::~CContainerActionPatternSet()
{
	if (!m_bIsDestroy)
		Destroy();
}


bool CContainerActionPatternSet::Create()
{
	return true;
}

void CContainerActionPatternSet::Destroy()
{
	m_bIsDestroy = true;
	Clear();
}

bool CContainerActionPatternSet::Add(CCharacter* pBot, TBLIDX actionPatternTblidx)
{
	if (pBot)
	{
		if (actionPatternTblidx == INVALID_ACTIONPATTERNID)
			return false;
		else
		{
			if (Find(pBot->GetTblidx(), actionPatternTblidx))
				return true;
			else
			{
				sMOVE_PATTERN_TBLDAT* movePatternTbldat = (sMOVE_PATTERN_TBLDAT*)g_pTableContainer->GetMobMovePatternTable()->FindData(actionPatternTblidx);
				if (movePatternTbldat)
				{
					ACTIONPATTERN_FIELDSIZE fieldSize = 0;

					for (; fieldSize < MAX_SAVECOUNT_ACTIONPATTERN_FIELD && movePatternTbldat->abyPattern[fieldSize] != INVALID_BYTE; fieldSize++)
						;
					--fieldSize; //decrease by one because in loop it gets increased by one which makes fieldSize invalid

					if (fieldSize && fieldSize < MAX_SAVECOUNT_ACTIONPATTERN_FIELD)
					{
					//	ERR_LOG(LOG_GENERAL, "fieldSize %u eACTIONPATTERNID %u \n", fieldSize, movePatternTbldat->abyPattern[fieldSize]);
						CContainerActionPatternUnit* pUnit = new CContainerActionPatternUnit;
						if (pUnit->Create(actionPatternTblidx, fieldSize))
						{
							for (int k = 0; ; k++)
							{
								if (fieldSize <= k)
								{
									sPATTERN_DATA pattern;
									pattern.mobTblidx = pBot->GetTblidx();
									pattern.actionePatternTblidx = actionPatternTblidx;
									pattern.pPatternUnit = pUnit;
									m_mapActionPattern.insert(std::make_pair(pattern.mobTblidx, pattern));

									return true;
								}
							//	ERR_LOG(LOG_GENERAL, "abyPattern %u \n", movePatternTbldat->abyPattern[k]);
								CActionPatternUnit* pActionUnit = CFactoryActionPatternUnit::CreateActionPattern((eACTIONPATTERNID)movePatternTbldat->abyPattern[k]);
								if (pActionUnit == NULL)
								{
									ERR_LOG(LOG_BOTAI, "fail : NULL == pActionUnit");
									return false;
								}

								if (!pActionUnit->Create(pBot))
									break;

								if (!pUnit->AddActionPattern(pActionUnit))
									return false;
							}

							ERR_LOG(LOG_BOTAI, "fail : false == pUnit->Create( pBot )");
						}
					}
					else
					{
						ERR_LOG(LOG_BOTAI, "fail : 0 == fieldSize");
					}
				}
				else
				{
					ERR_LOG(LOG_BOTAI, "can not find action pattern table. tblidx[%u]", actionPatternTblidx);
				}
			}
		}
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : pBot == NULL");
	}

	return false;
}

void CContainerActionPatternSet::Clear()
{
	for (ACTIONPATTERN_MAP::iterator it = m_mapActionPattern.begin(); it != m_mapActionPattern.end(); it++)
	{
		SAFE_DELETE(it->second.pPatternUnit);
	}

	m_mapActionPattern.clear();
}

CContainerActionPatternUnit* CContainerActionPatternSet::Find(TBLIDX mobTblidx, TBLIDX actionPatternTblidx)
{
	ACTIONPATTERN_MAP::iterator lb = m_mapActionPattern.lower_bound(mobTblidx);
	ACTIONPATTERN_MAP::iterator ub = m_mapActionPattern.upper_bound(mobTblidx);

	for (; lb != ub; lb++)
	{
		if (lb->second.actionePatternTblidx == actionPatternTblidx)
			return lb->second.pPatternUnit;
	}

	return NULL;
}

bool CContainerActionPatternSet::AddPattern(CCharacter* pBot)
{
	if (pBot)
	{
		if (FindPattern(pBot->GetTblidx()))
			return true;
		else
		{
			CContainerActionPatternUnit* pActionPattern = new CContainerActionPatternUnit;
			BYTE fildSize = INVALID_BYTE;
			TBLIDX actionpatternTblID = INVALID_TBLIDX;

			if (g_pTableContainer->GetMobMovePatternTable()->GetNumberOfTables() <= ((CNpc*)pBot)->GetActionPattern()->GetTableIdx())
				actionpatternTblID = 0;
			else
			{
				actionpatternTblID = ((CNpc*)pBot)->GetActionPattern()->GetTableIdx();
			}

			if (sMOVE_PATTERN_TBLDAT* movePatternTbldat = (sMOVE_PATTERN_TBLDAT*)g_pTableContainer->GetMobMovePatternTable()->FindData(actionpatternTblID))
			{
				for (; fildSize < MAX_SAVECOUNT_ACTIONPATTERN_FIELD && movePatternTbldat->abyPattern[fildSize] != INVALID_BYTE; fildSize++)
					;
				if (fildSize)
				{
					--fildSize;
					if (fildSize == INVALID_BYTE)
					{
						ERR_LOG(LOG_BOTAI, "fail : INVALID_ACTIONPATTERN_FIELD == fildSize");
					}
					else if (pActionPattern->Create(actionpatternTblID, fildSize))
					{
						for (BYTE m = 0; fildSize > m; m++)
						{
							CActionPatternUnit* pActionUnit = CFactoryActionPatternUnit::CreateActionPattern((eACTIONPATTERNID)movePatternTbldat->abyPattern[m]);
							if (pActionUnit == NULL)
							{
								ERR_LOG(LOG_BOTAI, "fail : NULL == pActionUnit");
								return false;
							}

							if (!pActionUnit->Create(pBot))
							{
								ERR_LOG(LOG_BOTAI, "fail : false == pUnit->Create( pBot )");
								return false;
							}

							if (!pActionPattern->AddActionPattern(pActionUnit))
								return false;
						}

						m_mapPattern.insert(std::make_pair(pBot->GetTblidx(), pActionPattern));
						return true;
					}
				}
				else ERR_LOG(LOG_BOTAI, "fail : 0 == fildSize");
			}
		}
	}
	else ERR_LOG(LOG_BOTAI, "fail : pBot == NULL");

	return false;
}

CContainerActionPatternUnit* CContainerActionPatternSet::FindPattern(TBLIDX tblidxBot)
{
	PATTERN_MAP::iterator it = m_mapPattern.find(tblidxBot);
	if (it != m_mapPattern.end())
		return it->second;

	return NULL;
}

bool CContainerActionPatternSet::DeletePattern(TBLIDX tblidxBot)
{
	if (tblidxBot == INVALID_TBLIDX)
	{
		ERR_LOG(LOG_BOTAI, "fail : INVALID_TBLIDX == tblidxBot");
		return false;
	}

	PATTERN_MAP::iterator it = m_mapPattern.find(tblidxBot);
	if (it == m_mapPattern.end())
		return false;

	CContainerActionPatternUnit* pUnit = it->second;
	m_mapPattern.erase(tblidxBot);
	
	SAFE_DELETE(pUnit);

	return true;
}

void CContainerActionPatternSet::AllDeletePattern()
{
	PATTERN_MAP::iterator it = m_mapPattern.begin();
	for (; it != m_mapPattern.end(); it++)
	{
		delete (CContainerActionPatternUnit*)it->second;
	}

	m_mapPattern.clear();
}

