#include "precomp_ntlpresentation.h"

#include "NtlDebug.h"

#include "NtlPLDojoContainer.h"

#include "NtlPLDojo.h"

CNtlPLObjectController::CNtlPLObjectController()
{
	m_pNtlPLObject = NULL;
}

CNtlPLObjectController::~CNtlPLObjectController()
{
}

void CNtlPLObjectController::Init(CNtlPLObject* pNtlPLOjbect)
{
	Free();

	m_pNtlPLObject = pNtlPLOjbect;
}

RwBool CNtlPLObjectController::Update(RwReal fElapsed)
{
	return FALSE;
}

void CNtlPLObjectController::Free()
{
	m_pNtlPLObject = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLObjectControllerFade::CNtlPLObjectControllerFade()
{
	m_fTime		= 0.0f;
	m_fTimeCur	= 0.0f;
	m_fColorS	= 1.0f;
	m_fColorE	= 1.0f;
}

CNtlPLObjectControllerFade::~CNtlPLObjectControllerFade()
{
}

RwBool CNtlPLObjectControllerFade::Update(RwReal fElapsed)
{
	if (m_fTimeCur > 0.0f)
	{
		m_fTimeCur -= fElapsed;		
		if (m_pNtlPLObject)
		{
			RwReal fAlpha = (((m_fTime - m_fTimeCur) / m_fTime) * (m_fColorE - m_fColorS) + m_fColorS);
			CLAMP(fAlpha, 0.0f, 1.0f);
			m_pNtlPLObject->SetAlpha(static_cast<RwUInt8>(fAlpha * 255.0f));
		}
		return TRUE;
	}
	else if (m_bAutoDelete && m_pNtlPLObject)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObject);
		m_pNtlPLObject = NULL;
	}
// 	else if (m_pNtlPLObject)
// 	{
// 		RwReal fAlpha = (((m_fTime - m_fTimeCur) / m_fTime) * (m_fColorE - m_fColorS) + m_fColorS);
// 		CLAMP(fAlpha, 0.0f, 1.0f);
// 		m_pNtlPLObject->SetAlpha(static_cast<RwUInt8>(fAlpha * 255.0f));
// 	}
	return FALSE;
}

void CNtlPLObjectControllerFade::Free()
{
	if (m_bAutoDelete && m_pNtlPLObject)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObject);
	}
	CNtlPLObjectController::Free();
}

void CNtlPLObjectControllerFade::SetParam(RwReal fTime, RwReal fColorS, RwReal fColorE, RwBool bAutoDelete)
{
	m_bAutoDelete = bAutoDelete;
		 
	m_fTime			= fTime;
	m_fTimeCur		= m_fTime;
	m_fColorS		= fColorS;
	m_fColorE		= fColorE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLDojo::CNtlPLDojo()
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_ATTACH);
	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_DOJO);

	m_pDojoObject	= NULL;

	m_vPos.x		= 0.0f;
	m_vPos.y		= 0.0f;
	m_vPos.z		= 0.0f;

	m_vRot.x		= 0.0f;
	m_vRot.y		= 0.0f;
	m_vRot.z		= 0.0f;

	m_vScale.x		= 1.0f;
	m_vScale.y		= 1.0f;
	m_vScale.z		= 1.0f;

	m_bEmblemNeedRefresh = FALSE;
}

CNtlPLDojo::~CNtlPLDojo()
{
}

RwBool CNtlPLDojo::Create(const SPLEntityCreateParam * pParam /* = NULL */)
{
	NTL_FUNCTION("CNtlPLDojo::Create");

	SPLDojoCreateParam* pPLDojoCreateParam = (SPLDojoCreateParam*)pParam;
	
	m_iDojoID		= pPLDojoCreateParam->iDojoID;
	
	m_fFadeInTime	= pPLDojoCreateParam->fFadeInTime;
	m_fFadeOutTime	= pPLDojoCreateParam->fFadeOutTime;

	memcpy(m_acObjectName, pPLDojoCreateParam->acObjectName, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN * dPL_DOJO_MAX_LEVEL);
	memcpy(m_acEffectNameUp, pPLDojoCreateParam->acEffectNameUp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN * dPL_DOJO_MAX_LEVEL);
	memcpy(m_acEffectNameDown, pPLDojoCreateParam->acEffectNameDown, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN * dPL_DOJO_MAX_LEVEL);

	SNtlPLDojoProp sNtlPLDojoProp;

	if (GetSceneManager()->GetDojoContainer()->GetDojo(m_iDojoID, &sNtlPLDojoProp))
	{
		m_sEmblemNew	= sNtlPLDojoProp.sEmblem;
		m_iDojoLevel	= sNtlPLDojoProp.iLevel;
	}
	else
	{
		m_iDojoLevel	= 0;
	}

	SetPosition(pPLDojoCreateParam->pPos);	
	RefreshResource();

	NTL_RETURN(TRUE);
}

void CNtlPLDojo::Destroy()
{
	NTL_FUNCTION("CNtlPLDojo::Destroy");

	m_FadeControllerDown.Free();
	m_FadeControllerUp.Free();

	if (m_pDojoObject)
	{
		GetSceneManager()->DeleteEntity(m_pDojoObject);
		m_pDojoObject = NULL;
	}

	NTL_RETURNVOID();
}

RwBool CNtlPLDojo::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLDojo::SetProperty");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLDojo::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLDojo::Update");

	m_FadeControllerDown.Update(fElapsed);
	m_FadeControllerUp.Update(fElapsed);

	CNtlPLAttach::Update(fElapsed);

	if (m_bEmblemNeedRefresh)
	{
		m_bEmblemNeedRefresh = !RefreshEmblem();
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLDojo::Render()
{
	NTL_FUNCTION("CNtlPLDojo::Update");

	NTL_RETURN(TRUE);
}

void CNtlPLDojo::UpdateTransform()
{
	CNtlMath::MathGetMatrix(&m_matTrasform, &m_vScale, &m_vRot, &m_vPos);
}

void CNtlPLDojo::RefreshResource(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	if (strlen(m_acObjectName[m_iDojoLevel]))
	{
		if (m_pDojoObject)
		{
			if (strcmp(m_pDojoObject->GetName(), m_acObjectName[m_iDojoLevel]))
			{
				Detach(m_pDojoObject);
				m_FadeControllerDown.Init(m_pDojoObject);
				m_FadeControllerDown.SetParam(fFadeOutTime, 1.0f, 0.0f, TRUE);
				m_pDojoObject = NULL;
			}
		}

		if (m_pDojoObject == NULL)
		{
			SPLObjectCreateParam createParam;	
			createParam.pPos		= &m_vPos;
			createParam.bLoadMap	= TRUE;

			m_pDojoObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, m_acObjectName[m_iDojoLevel], &createParam));
			DBO_ASSERT(m_pDojoObject, "Entity create failed.");

			m_pDojoObject->SetDojo(this);

			m_FadeControllerUp.Init(m_pDojoObject);
			m_FadeControllerUp.SetParam(fFadeInTime, 0.0f, 1.0f, FALSE);

			RwV3d vOffset = { 0.0f, 0.0f, 0.0f };
			AttachWorldPosAndRotate(m_pDojoObject, vOffset);
		}
	}

	m_bEmblemNeedRefresh	= TRUE;
}

RwBool CNtlPLDojo::RefreshEmblem()
{
	if (m_pDojoObject->IsActiveUpdate())
	{
		if (m_sEmblemNew.byTypeA != dPLEMBLEM_INVALID_FACTOR &&
			m_sEmblemNew.byTypeB != dPLEMBLEM_INVALID_FACTOR &&
			m_sEmblemNew.byTypeC != dPLEMBLEM_INVALID_FACTOR)
		{
			m_sEmblem = m_sEmblemNew;

			RwTexture* pTexture = GetEmblemMaker()->CreateEmblem(&m_sEmblem, 128);

			m_pDojoObject->SetAtomicEmblemMap(pTexture);			
		}
		else
		{
			m_pDojoObject->DestroyEmblemMap();
		}
		return TRUE;
	}
	return FALSE;
}

void CNtlPLDojo::SetLevel(RwInt32 iLevel)
{
// 	if (!strlen(m_acObjectName[iLevel]) || !strcmp(m_pDojoObject->GetName(), m_acObjectName[iLevel]))
// 	{
// 		m_iDojoLevel = iLevel;
// 		return;
// 	}

	if (strlen(m_acObjectName[iLevel]) && strcmp(m_pDojoObject->GetName(), m_acObjectName[iLevel]))
	{
		if (iLevel > m_iDojoLevel)
		{
			if (strlen(m_acEffectNameUp[iLevel]))
			{
				CNtlPLEntity* pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_acEffectNameUp[iLevel]);
				pPLEntity->SetPosition(&m_vPos);
			}		
		}
		else if (iLevel < m_iDojoLevel)
		{
			if (strlen(m_acEffectNameUp[iLevel]))
			{
				CNtlPLEntity* pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_acEffectNameDown[iLevel]);
				pPLEntity->SetPosition(&m_vPos);
			}
		}
	}

	m_iDojoLevel = iLevel;
	RefreshResource(m_fFadeInTime, m_fFadeOutTime);
}

void CNtlPLDojo::RefreshDojo()
{
	SNtlPLDojoProp sNtlPLDojoProp;
	if (GetSceneManager()->GetDojoContainer()->GetDojo(m_iDojoID, &sNtlPLDojoProp))
	{
		m_sEmblemNew = sNtlPLDojoProp.sEmblem; // SetLevel를 호출 하기 전에 Setting 해야 한다.
		SetLevel(sNtlPLDojoProp.iLevel);
	}
}

CNtlPLObject* CNtlPLDojo::GetDojoObject()
{
	return m_pDojoObject;
}