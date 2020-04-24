#include "precomp_ntlsimulation.h"
#include "DboTLConditionCheckState.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlCameraManager.h"
#include "NtlSob.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"


//////////////////////////////////////////////////////////////////////////
//	CDboConditionCheck_MouseDrag
//////////////////////////////////////////////////////////////////////////

VOID CDboConditionCheck_MouseDrag::Update(RwReal fElapsed)
{
	RwReal fCurYaw = GetNtlGameCameraManager()->GetYaw();
	RwReal fDiff = fabs(m_fYaw) - fabs(fCurYaw);

	if( fabs(fDiff) > 90.f )
	{
		m_fYaw = fCurYaw;
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_MOUSE_DRAG);
	}
}


//////////////////////////////////////////////////////////////////////////
//	CDboConditionCheck_Mob_Target
//////////////////////////////////////////////////////////////////////////

VOID CDboConditionCheck_CheckPoint::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CDboConditionCheck_CheckPoint::HandleEvents");

	if(pMsg.Id == g_EventDirectionNfy)
	{
		SNtlEventDirectionNfy* pEvent = reinterpret_cast<SNtlEventDirectionNfy*>( pMsg.pData );

		if( !pEvent->bCreate )
			Logic_SendTutorialCondition(ETL_CONDITION_TYPE_IN_CHECK_POINT);
	}

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
//	CDboConditionCheck_Mob_Target
//////////////////////////////////////////////////////////////////////////

VOID CDboConditionCheck_Mob_Target::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CDboConditionCheck_MouseDrag::HandleEvents");

	if(pMsg.Id == g_EventSobTargetInfoUpdate)
	{
		SNtlEventSobTargetInfoUpdate *pTartgetUpdate = reinterpret_cast<SNtlEventSobTargetInfoUpdate*>(pMsg.pData); 

		if(pTartgetUpdate->bShow)
		{
			if( INVALID_SERIAL_ID == pTartgetUpdate->hSerialId )
				NTL_RETURNVOID();

			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pTartgetUpdate->hSerialId ); 
			if(pSobObj)
			{
				if( pSobObj->GetClassID() == SLCLASS_MONSTER )
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_MOB_TARGET);
			}
		}
	}

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
//	CDboConditionCheck_Mob_NormalAttack
//////////////////////////////////////////////////////////////////////////

VOID CDboConditionCheck_Mob_NormalAttack::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CDboConditionCheck_Mob_NormalAttack::HandleEvents");

	if(pMsg.Id == g_EventSobAttack)
	{
		SNtlEventSobAttack* pEvent = reinterpret_cast<SNtlEventSobAttack*>( pMsg.pData );

		if( pEvent->hAttackerSerialId == Logic_GetAvatarHandle() )
		{
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEvent->hDefenderSerialId );
			if(pSobObj)
			{
				if( pSobObj->GetClassID() == SLCLASS_MONSTER )
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_MOB_NORMAL_ATTACK);
			}			
		}
	}

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
//	CDboConditionCheck_Mob_SkillAttack
//////////////////////////////////////////////////////////////////////////

VOID CDboConditionCheck_Mob_SkillAttack::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CDboConditionCheck_Mob_SkillAttack::HandleEvents");

	if(pMsg.Id == g_EventSobSkillAction)
	{
		SNtlEventSobSkillAction* pEvent = reinterpret_cast<SNtlEventSobSkillAction*>( pMsg.pData );

		if( pEvent->hSerialId == Logic_GetAvatarHandle() )
		{
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pEvent->hTargetSerialId );
			if(pSobObj)
			{
				if( pSobObj->GetClassID() == SLCLASS_MONSTER )
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_MOB_SKILL_ATTACK);
			}			
		}		
	}

	NTL_RETURNVOID();
}