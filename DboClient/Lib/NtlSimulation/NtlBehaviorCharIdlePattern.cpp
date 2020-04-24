#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharIdlePattern.h"

//shared
#include "NtlActionPattern.h"
#include "MobMovePatternTable.h"
#include "GraphicDataTable.h"

// simulation 
#include "NtlSobActor.h"
#include "NtlSobCharProxy.h"
#include "NtlSobMonsterAttr.h"

CNtlBehaviorCharIdlePattern::CNtlBehaviorCharIdlePattern()
{
	m_eCurrPattern	= IDLE_PATTERN_NORMAL_IDLE;
	m_byCurrTime	= 0.0f;
	m_byIdleTime	= 0.0f;
	m_pActor		= NULL;
}

CNtlBehaviorCharIdlePattern::~CNtlBehaviorCharIdlePattern()
{
}

RwBool CNtlBehaviorCharIdlePattern::IsVaildIncrementPattern(void)
{
	CNtlSobMonsterAttr *pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pActor->GetSobAttr() );
	RwUInt8 byPatternId = pSobMobAttr->GetMovePatternId();

	byPatternId++;

	sMOVE_PATTERN_TBLDAT *pMovePatternTblData = pSobMobAttr->GetMovePatternTbl();

	RwUInt8 byPatternValue = pMovePatternTblData->abyPattern[byPatternId];
	if(byPatternValue == (RwUInt8)INVALID_ACTIONPATTERNID)
		return FALSE;

	if(byPatternValue == (RwUInt8)BOTAP_MOVE)
		return FALSE;

	return TRUE;
}

void CNtlBehaviorCharIdlePattern::IncrementTablePatternId(void)
{
	// move pattern id를 증가시킨다.
	CNtlSobMonsterAttr *pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pActor->GetSobAttr() );
	RwUInt8 byPatternId = pSobMobAttr->GetMovePatternId();

	byPatternId++;

	sMOVE_PATTERN_TBLDAT *pMovePatternTblData = pSobMobAttr->GetMovePatternTbl();

	if(pMovePatternTblData->abyPattern[byPatternId] == (RwUInt8)INVALID_ACTIONPATTERNID)
		return;

	pSobMobAttr->SetMovePatternId(byPatternId);
}

RwUInt8	CNtlBehaviorCharIdlePattern::GetTablePatternId(void)
{
	CNtlSobMonsterAttr *pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pActor->GetSobAttr() );
	RwUInt8 byPatternId = pSobMobAttr->GetMovePatternId();

	sMOVE_PATTERN_TBLDAT *pMovePatternTblData = pSobMobAttr->GetMovePatternTbl();

	return pMovePatternTblData->abyPattern[byPatternId];
}


RwUInt8 CNtlBehaviorCharIdlePattern::GetCurrentUnitId(void)
{
	// move pattern id를 증가시킨다.
	CNtlSobMonsterAttr *pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pActor->GetSobAttr() );
	return pSobMobAttr->GetMovePatternId();
}

void CNtlBehaviorCharIdlePattern::MoveProc(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	if(uiCurrAnimKey != NML_IDLE_LOOP)
	{
		if (m_pActor->IsAirMode())
			pSobProxy->SetBaseAnimation(FLYING_STAND_IDLE);
		else
			pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);
	}

	m_eCurrPattern = IDLE_PATTERN_MOVE;
}

void CNtlBehaviorCharIdlePattern::NormalIdleProc(RwReal fLifeTime)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	if(uiCurrAnimKey != NML_IDLE_LOOP)
	{
	    pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);
	}

	m_byCurrTime = 0.0f;
	m_byIdleTime = fLifeTime;
	m_eCurrPattern = IDLE_PATTERN_NORMAL_IDLE;
}

void CNtlBehaviorCharIdlePattern::ExceptIdleProc(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	if(uiCurrAnimKey != NML_IDLE_LOOP)
	{
	    pSobProxy->SetBaseAnimation(NML_IDLE_LOOP);
	}

	m_byCurrTime = 0.0f;
	m_byIdleTime = 10000.0f;
	m_eCurrPattern = IDLE_PATTERN_NORMAL_IDLE;
}

void CNtlBehaviorCharIdlePattern::NormalSpecialIdleProc(ECharIdlePattern ePattern)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	
	if(ePattern == IDLE_PATTERN_SPECIAL_IDLE1)
	{
		if(uiCurrAnimKey != NML_IDLE_01)
		{
			pSobProxy->SetBaseAnimation(NML_IDLE_01, FALSE);
		}

		m_eCurrPattern = IDLE_PATTERN_SPECIAL_IDLE1;
	}
	else
	{
		if(uiCurrAnimKey != NML_IDLE_02)
		{
			pSobProxy->SetBaseAnimation(NML_IDLE_02, FALSE);
		}

		m_eCurrPattern = IDLE_PATTERN_SPECIAL_IDLE2;
	}
	   
	m_byCurrTime = 0.0f;
	m_byIdleTime = 0.0f;
}

void CNtlBehaviorCharIdlePattern::CurrentUnitProc(void)
{
	RwUInt8 byPatternId = GetTablePatternId();

	if(byPatternId == (RwUInt8)BOTAP_MOVE)
	{
		MoveProc();
	}

	else if(byPatternId == (RwUInt8)BOTAP_IDLE1)
	{
		NormalIdleProc( (RwReal)BOTAP_IDLE1_SECOND/1000.0f );
	}

	else if(byPatternId == (RwUInt8)BOTAP_IDLE2)
	{
		NormalIdleProc( (RwReal)BOTAP_IDLE2_SECOND/1000.0f );
	}

	else if(byPatternId == (RwUInt8)BOTAP_MIDLE1)
	{
		NormalSpecialIdleProc(IDLE_PATTERN_SPECIAL_IDLE1);
	}
	
	else if(byPatternId == (RwUInt8)BOTAP_MIDLE2)
	{
		NormalSpecialIdleProc(IDLE_PATTERN_SPECIAL_IDLE2);
	}
}


void CNtlBehaviorCharIdlePattern::SetActor(CNtlSobActor *pActor)
{
	m_pActor = pActor;

	ExceptIdleProc();
	/*
	RwUInt8 byPatternId = GetTablePatternId();

	if(byPatternId == (RwUInt8)BOTAP_MOVE)
	{
		NextUnit();
	}
	else
	{
		CurrentUnitProc();
	}
	*/
}

void CNtlBehaviorCharIdlePattern::Update(RwReal fElapsed)
{
	if(m_eCurrPattern == IDLE_PATTERN_NORMAL_IDLE)
	{
		m_byCurrTime += fElapsed;

		if(m_byCurrTime >= m_byIdleTime)
		{
			NextUnit();
		}
	}
}

void CNtlBehaviorCharIdlePattern::NextUnit(void)
{
	if(IsVaildIncrementPattern())
	{
		IncrementTablePatternId();
		CurrentUnitProc();
	}
	else
	{
		ExceptIdleProc();
	}
}

void CNtlBehaviorCharIdlePattern::SyncUnit(void)
{
	CurrentUnitProc();
}