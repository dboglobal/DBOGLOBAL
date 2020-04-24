#include "precomp_ntlpresentation.h"
#include "NtlPLElapsedController.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEntity.h"


CNtlElapsedController::CNtlElapsedController()
{
	m_fCurrTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_fWeightValue = 1.0f;
	
	m_pPLEntity = NULL;
	m_bFinish = FALSE;
}

CNtlElapsedController::~CNtlElapsedController()
{
}

void CNtlElapsedController::Enter(CNtlPLEntity *pPLEntity, RwReal fLifeTime, RwReal fWeightValue)
{
	NTL_PRE("CNtlElapsedController::Enter => pPLEntity is null ");

	m_pPLEntity = pPLEntity;
	m_fLifeTime = fLifeTime;
	m_fWeightValue = fWeightValue;
	
	RwReal fPLWeightValue = m_pPLEntity->GetWeightElapsedTime();
	m_pPLEntity->SetWeightElapsedTime(fPLWeightValue*fWeightValue);
}

void CNtlElapsedController::Update(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;
	if(m_fCurrTime >= m_fLifeTime)
		m_bFinish = TRUE;
}

void CNtlElapsedController::Exit(void)
{
	RwReal fPLWeightValue = m_pPLEntity->GetWeightElapsedTime();
	m_pPLEntity->SetWeightElapsedTime(fPLWeightValue/m_fWeightValue);
}

RwBool CNtlElapsedController::IsFinish(void)
{
	return m_bFinish;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlElapsedControllerGroup::CNtlElapsedControllerGroup()
{
	m_pPLEntity = NULL;
	memset(m_pController, 0, sizeof(CNtlElapsedController*)*MAX_ELAP_CONTROLLER);
	m_chRef = 0;
	m_bFinish = FALSE;
}

CNtlElapsedControllerGroup::~CNtlElapsedControllerGroup()
{
	if(m_chRef > 0)
	{
		for(RwInt32 i = 0; i < MAX_ELAP_CONTROLLER; ++i)
		{
			if(m_pController[i])
			{
				NTL_DELETE(m_pController[i]);
			}
		}
	}

	m_pPLEntity->EnableWeightElapsedTime(FALSE);
	m_pPLEntity->SetWeightElapsedTime(1.0f);
}

RwInt32 CNtlElapsedControllerGroup::DeleteLongTimeController(void)
{
	RwInt32 iIdx = 0;
	RwReal fLongTime = 0.0f;

	for(RwInt32 i = 0; i < MAX_ELAP_CONTROLLER; ++i)
	{
		if(m_pController[i])
		{
			if(m_pController[i]->m_fCurrTime >= fLongTime)
				iIdx = i;
		}
	}

	m_pController[iIdx]->Exit();
	NTL_DELETE(m_pController[iIdx]);

	m_chRef--;

	return iIdx;
}

void CNtlElapsedControllerGroup::CreateController(RwReal fLifeTime, RwReal fWeightValue)
{
	RwInt32 iIdx = 0;

	if(m_chRef >= MAX_ELAP_CONTROLLER)
		iIdx = DeleteLongTimeController();
	else
	{
		for(RwInt32 i = 0; i < MAX_ELAP_CONTROLLER; ++i)
		{
			if(m_pController[i] == NULL)
			{
				iIdx = i;
				break;
			}
		}
	}

	m_pController[iIdx] = NTL_NEW CNtlElapsedController;
	m_pController[iIdx]->Enter(m_pPLEntity, fLifeTime, fWeightValue);

	if(m_chRef == 0)
		m_pPLEntity->EnableWeightElapsedTime(TRUE);

	m_chRef++;
}

void CNtlElapsedControllerGroup::Update(RwReal fElapsed)
{
	for(RwInt32 i = 0; i < MAX_ELAP_CONTROLLER; ++i)
	{
		if(m_pController[i])
		{
			m_pController[i]->Update(fElapsed);
			if(m_pController[i]->IsFinish())
			{
				m_pController[i]->Exit();
				NTL_DELETE(m_pController[i]);

				m_chRef--;
			}
		}
	}
}

void CNtlElapsedControllerGroup::SetControlEntity(CNtlPLEntity *pPLEntity)
{
	m_pPLEntity = pPLEntity;
}

RwBool CNtlElapsedControllerGroup::IsFinish(void)
{
	if(m_chRef <= 0)
		return TRUE;

	return FALSE;
}