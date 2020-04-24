#include "precomp_ntlsimulation.h"
#include "NtlBehaviorData.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLSceneManager.h"

// simulation
#include "NtlSobActor.h"
#include "NtlBehaviorDef.h"
#include "NtlSLLogic.h"
#include "NtlSobProxy.h"
#include "NtlSLEvent.h"


 DEFINITION_MEMORY_POOL(SHitStuff)


CNtlTimeSequence::CNtlTimeSequence()
{
	m_pPrev = NULL;
	m_pNext = NULL;
    m_fTime = 0.0f;
}

CNtlTimeSequence::~CNtlTimeSequence()
{
	Remove(this);
}

void CNtlTimeSequence::Add(CNtlTimeSequence *pSeq)
{
	CNtlTimeSequence *pNextSeq = GetNext();
	if(pNextSeq)
	{
		pSeq->SetNext(pNextSeq);
		pNextSeq->SetPrev(pSeq);
	}

	SetNext(pSeq);
	pSeq->SetPrev(this);
}

void CNtlTimeSequence::Remove(CNtlTimeSequence *pSeq)
{
	CNtlTimeSequence *pPrevSeq = pSeq->GetPrev();
	CNtlTimeSequence *pNextSeq = pSeq->GetNext();

	if(pPrevSeq)
	{
		pPrevSeq->SetNext(pNextSeq);
	}

	if(pNextSeq)
	{
		pNextSeq->SetPrev(pPrevSeq);
	}
}

void CNtlTimeSequence::SetPrev(CNtlTimeSequence *pSeq)
{
	m_pPrev = reinterpret_cast<CNtlTimeSequence*>(pSeq);
}

CNtlTimeSequence* CNtlTimeSequence::GetPrev(void) const
{
	return m_pPrev;
}

void CNtlTimeSequence::SetNext(CNtlTimeSequence *pSeq)
{
	m_pNext = reinterpret_cast<CNtlTimeSequence*>(pSeq);
}

CNtlTimeSequence* CNtlTimeSequence::GetNext(void) const
{
	return m_pNext;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDistTimeSequence)

RwReal CNtlDistTimeSequence::m_fCurrDist = 0.0f;
RwReal CNtlDistTimeSequence::m_fOldDist = 0.0f;
RwReal CNtlDistTimeSequence::m_fDeltaDist = 0.0f;
RwBool CNtlDistTimeSequence::m_bCalcOld = FALSE;
RwInt32 CNtlDistTimeSequence::m_iOldTimeNode = 0;

CNtlDistTimeSequence::CNtlDistTimeSequence()
{
	m_fDist = 0.0f;
	m_fTime = 0.0f;
}

CNtlDistTimeSequence::~CNtlDistTimeSequence()
{
}

void CNtlDistTimeSequence::SetDistance(RwReal fDist)
{
	m_fDist = fDist;
}

RwReal CNtlDistTimeSequence::GetDistance(void) const
{
	return m_fDist;
}

RwReal CNtlDistTimeSequence::RecursiveData(RwReal fTime)
{
	CNtlDistTimeSequence *pNext = reinterpret_cast<CNtlDistTimeSequence*>(m_pNext);

	if(pNext == NULL && fTime >= m_fTime)
	{
		return m_fDist;
	}
	else
	{
		if(fTime > m_fTime)
		{
			return pNext->RecursiveData(fTime);
		}
		else
		{
			CNtlDistTimeSequence *pPrev = reinterpret_cast<CNtlDistTimeSequence*>(m_pPrev);
			RwReal fPrevDist = 0.0f;
			RwReal fPrevTime = 0.0f;

			if(pPrev)
			{
				fPrevDist = pPrev->GetDistance();
				fPrevTime = pPrev->GetTime();
			}

			RwReal fDeltaDist = m_fDist - fPrevDist;
			RwReal fDeltaTime = m_fTime - fPrevTime;

			return fPrevDist + (fDeltaDist/fDeltaTime)*(fTime-fPrevTime);
		}
	}
}

RwReal CNtlDistTimeSequence::RecursiveDeltaData(RwReal fOldTime, RwReal fCurrTime)
{
	CNtlDistTimeSequence *pNext = reinterpret_cast<CNtlDistTimeSequence*>(m_pNext);
	CNtlDistTimeSequence *pPrev = reinterpret_cast<CNtlDistTimeSequence*>(m_pPrev);
	
	RwReal fPrevDist = 0.0f;
	RwReal fPrevTime = 0.0f;

	if(pPrev)
	{
		fPrevDist = pPrev->GetDistance();
		fPrevTime = pPrev->GetTime();
	}

	RwReal fDeltaDist = m_fDist - fPrevDist;
	RwReal fDeltaTime = m_fTime - fPrevTime;
	
	if(fOldTime <= m_fTime)
	{
		if(!m_bCalcOld)
		{
			m_fOldDist = (fDeltaDist/fDeltaTime)*(fOldTime-fPrevTime);
			m_bCalcOld = TRUE;
		}
	}

	if(pNext == NULL && fCurrTime >= m_fTime)
	{
		if(m_bCalcOld)
		{
			if(m_iOldTimeNode == 0)
				m_fDeltaDist += fDeltaDist - m_fOldDist;
			else
				m_fDeltaDist += fDeltaDist;
		}
		else
		{
			return 0.0f;
		}
	}
	else
	{
		if(fCurrTime > m_fTime)
		{
			if(m_bCalcOld)
			{
				if(m_iOldTimeNode == 0)
					m_fDeltaDist += fDeltaDist - m_fOldDist;
				else
					m_fDeltaDist += fDeltaDist;

				m_iOldTimeNode++;
			}
		
			return pNext->RecursiveDeltaData(fOldTime, fCurrTime);
		}
		else
		{
			RwReal fCurrDist = (fDeltaDist/fDeltaTime)*(fCurrTime-fPrevTime);
			if(m_iOldTimeNode == 0)
				m_fDeltaDist += fCurrDist - m_fOldDist;
			else
				m_fDeltaDist += fCurrDist;

			return m_fDeltaDist;
		}
	}

	return m_fDeltaDist;
}

void* CNtlDistTimeSequence::GetData(RwReal fTime)
{
	m_fCurrDist = RecursiveData(fTime);

	return &m_fCurrDist;
}

void* CNtlDistTimeSequence::GetDeltaData(RwReal fPrevTime, RwReal fCurrTime)
{
	m_fDeltaDist = 0.0f;
	m_bCalcOld = FALSE;
	m_iOldTimeNode = 0;
	m_fDeltaDist = RecursiveDeltaData(fPrevTime, fCurrTime);

	return &m_fDeltaDist;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtl2DTimeSequence)

CNtl2DTimeSequence::S2DDist CNtl2DTimeSequence::m_sCurrDist;

CNtl2DTimeSequence::CNtl2DTimeSequence()
{
	m_sDist.fX = 0.0f;
	m_sDist.fY = 0.0f;
	m_fTime = 0.0f;
}

CNtl2DTimeSequence::~CNtl2DTimeSequence()
{
}

CNtl2DTimeSequence::S2DDist CNtl2DTimeSequence::RecursiveData(RwReal fTime)
{
	CNtl2DTimeSequence *pNext = reinterpret_cast<CNtl2DTimeSequence*>(m_pNext);

	if(pNext == NULL && fTime >= m_fTime)
	{
		return m_sDist;
	}
	else
	{
		if(fTime > m_fTime)
		{
			return pNext->RecursiveData(fTime);
		}
		else
		{
			CNtl2DTimeSequence *pPrev = reinterpret_cast<CNtl2DTimeSequence*>(m_pPrev);
			S2DDist sPrev2dDist = { 0.0f, 0.0f };
			RwReal fPrevTime = 0.0f;

			if(pPrev)
			{
				sPrev2dDist = pPrev->GetDistance();
				fPrevTime = pPrev->GetTime();
			}

			S2DDist s2dDeltaDist;
			s2dDeltaDist.fX = m_sDist.fX - sPrev2dDist.fX;
			s2dDeltaDist.fY = m_sDist.fY - sPrev2dDist.fY;
			RwReal fDeltaTime = m_fTime - fPrevTime;

			s2dDeltaDist.fX = sPrev2dDist.fX + (s2dDeltaDist.fX/fDeltaTime)*(fTime-fPrevTime);
			s2dDeltaDist.fY = sPrev2dDist.fY + (s2dDeltaDist.fY/fDeltaTime)*(fTime-fPrevTime);

			return s2dDeltaDist;
		}
	}
}

void CNtl2DTimeSequence::SetDistance(RwReal fDistX, RwReal fDistY)
{
	m_sDist.fX = fDistX;
	m_sDist.fY = fDistY;
}

CNtl2DTimeSequence::S2DDist CNtl2DTimeSequence::GetDistance(void) const
{
	return m_sDist;
}

void* CNtl2DTimeSequence::GetData(RwReal fTime)
{
	m_sCurrDist = RecursiveData(fTime);
	return &m_sCurrDist;
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtl3DTimeSequence)

RwV3d CNtl3DTimeSequence::m_svCurrData = ZeroAxis;

CNtl3DTimeSequence::CNtl3DTimeSequence() 
{
    m_vData = ZeroAxis;
}

CNtl3DTimeSequence::~CNtl3DTimeSequence() 
{

}

void* CNtl3DTimeSequence::GetData( RwReal fTime ) 
{
    m_svCurrData = RecursiveData(fTime);
    return &m_svCurrData;
}

RwV3d CNtl3DTimeSequence::RecursiveData( RwReal fTime ) 
{
    CNtl3DTimeSequence *pNext = reinterpret_cast<CNtl3DTimeSequence*>(m_pNext);

    if(pNext == NULL && fTime >= m_fTime)
    {
        return m_vData;
    }
    else
    {
        if(fTime > m_fTime)
        {
            return pNext->RecursiveData(fTime);
        }
        else
        {
            CNtl3DTimeSequence *pPrev = reinterpret_cast<CNtl3DTimeSequence*>(m_pPrev);
            RwV3d vPrevData = ZeroAxis;            
            RwReal fPrevTime = 0.0f;

            if(pPrev)
            {
                vPrevData = pPrev->GetHaveData();
                fPrevTime = pPrev->GetTime();
            }

            RwReal fDeltaTime = (fTime - fPrevTime) / (m_fTime - fPrevTime);
            return CNtlMath::Interpolation(vPrevData, m_vData, fDeltaTime);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlAttackData)
DEFINITION_MEMORY_POOL(CNtlBeCharData)






////////////////////////////////////////////////////////////////////////////
//  des : // actor A function related to coordinate correction
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : actor
//  \param2 : 보정할 위치
////////////////////////////////////////////////////////////////////////////

void Behavior_AdjustPositionJumping(CNtlSobActor *pActor, const RwV3d *pTarLoc)
{
	/*
	RwV3d vPos, vTarPos, vSub;

	vPos = pActor->GetPosition();
	vPos.y = 0.0f;
	CNtlMath::MathRwV3dAssign(&vTarPos, pTarLoc->x, 0.0f, pTarLoc->z);

	RwV3dSubMacro(&vSub, &vTarPos, &vPos);
	RwReal fLen = RwV3dLength(&vSub);
	if(fLen >= NTL_POSITION_JUMPING_LIMIT)
	{
		GetSceneManager()->GetWorldHeight(&vTarPos, vTarPos.y);
		pActor->SetPosition(&vTarPos);
	}
	*/

	RwV3d vPos;
	RwV3dAssignMacro(&vPos, pTarLoc);

	if ( pActor->GetClassID() == SLCLASS_VEHICLE )
	{
		RwV3d vCurDir = pActor->GetDirection();
		RwV3d vCurPos = pActor->GetPosition();

		RwV3d vResultDir, vResultPos;
		Logic_GetWorldSlop( pActor, vCurPos, vCurDir, pActor->GetSobProxy()->GetPLEntityDepth() / 2.f, vResultPos, vResultDir, 10.f, 10.f );

		pActor->SetDirection( &vCurDir );
		pActor->SetPosition( &vCurPos );
	}
	else
	{
		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(pActor, &vPos, sHStuff);

		if (pActor->IsAirMode() == FALSE)
		{
			vPos.y = sHStuff.fFinialHeight;
		}

		pActor->SetPosition(&vPos);
	}
}


SMoveSyncStuff::~SMoveSyncStuff() 
{
    Clear();
}

void SMoveSyncStuff::Clear() 
{
    NTL_DELETE(m_pMoveSyncCurr);    

    for each(SNtlEventSobMoveSync* pEvent in m_MoveSyncQ)
    {
        NTL_DELETE(pEvent);
    }
    m_MoveSyncQ.clear();
}

SNtlEventSobMoveSync* SMoveSyncStuff::Next() 
{
    NTL_DELETE(m_pMoveSyncCurr);    

    if(!m_MoveSyncQ.empty())
    {
        m_pMoveSyncCurr = Pop();                
    }
    else
    {
        m_pMoveSyncCurr = NULL;        
    } 

    return m_pMoveSyncCurr;
}

SNtlEventSobMoveSync* SMoveSyncStuff::Pop() 
{
    if(m_MoveSyncQ.empty())
        return NULL;

    SNtlEventSobMoveSync* pPopEvent = m_MoveSyncQ.front();
    m_MoveSyncQ.pop_front();

    return pPopEvent;
}