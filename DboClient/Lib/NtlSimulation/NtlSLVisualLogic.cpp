#include "precomp_ntlsimulation.h"
#include "NtlSLVisualLogic.h"

// simulation
#include "NtlSLVisualDeclear.h"
#include "NtlSobActor.h"
#include "NtlSobSkill.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSobManager.h"



void Logic_SkillUseVisualEffect(CNtlSobActor *pActor, SERIAL_HANDLE hSkillSerialId)
{
	// skill 사용 effect 출력
	CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
	RwV3d vPos = pActor->GetPosition();
	vPos.y += pSobProxy->GetPLEntityHeight() * 0.9f;

	if(Logic_IsMasterSkill(hSkillSerialId))
		pSobProxy->CreatePLEffect(NTL_VID_SPECIAL_SKILL_USE, &vPos);
	else
		pSobProxy->CreatePLEffect(NTL_VID_NORMAL_SKILL_USE, &vPos);
}

void Logic_VisualAttackSelect(SERIAL_HANDLE hSerialId)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hSerialId) );
	if(pActor == NULL)
		return;

	SNtlEventSobAttackSelect sSobAttackSelect;
	sSobAttackSelect.hSerialId = hSerialId;
	sSobAttackSelect.pSobActor = pActor;

	RWS::CMsg msg;
	msg.Id = g_EventSobAttackSelect;
	msg.pData = reinterpret_cast<void*>(&sSobAttackSelect);

	CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
	pSobProxy->HandleEvents(msg);
}