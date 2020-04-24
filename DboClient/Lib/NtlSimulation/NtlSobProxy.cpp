#include "precomp_ntlsimulation.h"
#include "NtlSobProxy.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLGlobal.h"
#include "NtlPLHelpers.h"
#include "NtlPLEntity.h"
#include "NtlPLSceneManager.h"
#include "NtlPLDamageBox.h"

// simulation 
#include "NtlSob.h"
#include "NtlSobElapsedController.h"
#include "NtlSobProxySystemEffect.h"
#include "NtlSlLogic.h"


CNtlSobProxy::CNtlSobProxy()
{
	m_uiSystemFlags	= 0;
	m_pSobObj		= NULL;
	m_uiCompType	= 0;

	m_byAlpha		= 255;

	m_pWorldLight	= NULL;
    m_bEnableCameraAlpha = FALSE;
    m_pCameraAlphaEffect = NULL;

	m_bDisableAniChange = false;
}

CNtlSobProxy::~CNtlSobProxy()
{
}

RwBool CNtlSobProxy::Create(RwUInt32 uiCompType)
{
	m_uiCompType = uiCompType;

	return TRUE;
}

void CNtlSobProxy::Destroy(void)
{
    if(m_pCameraAlphaEffect)
    {
        RemoveVisualSystemEffectAlpha(m_pCameraAlphaEffect);
    }

	// presentation entity clear
	CNtlPLEntity *pPLEntity;
	ListPLEntity::iterator it;

	for(it = m_listPLChild.begin(); it != m_listPLChild.end(); it++)
	{
		pPLEntity = (*it);
		GetSceneManager()->DeleteEntity(pPLEntity);
	}

	m_listPLChild.clear();


	//-----------------------------------------------------
	// Proxy system effect clear
    for each(CNtlSobProxySystemEffect* pSystemEffect in m_listProxySystemEffect)
    {
        pSystemEffect->Exit();
        NTL_DELETE(pSystemEffect);
    }
    m_listProxySystemEffect.clear();
}

void CNtlSobProxy::Update(RwReal fElapsed)
{
    if(!m_listProxySystemEffect.empty())
    {
        // ProxySystemEffect 업데이트
        RwBool bIsCalcWeight[CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COUNT];
        ZeroMemory(bIsCalcWeight, sizeof(RwBool) * CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COUNT);

        CNtlSobProxySystemEffect* pSystemEffect = NULL;
        for(ListProxySystemEffect::iterator it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end();)
        {
            pSystemEffect = (*it);
            if(pSystemEffect->IsFinish())       // 종료시에는 이펙트의 삭제후의 값을 계산해야 한다.
            {
                if(pSystemEffect->GetType() != CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_SCALE)
                {
                    bIsCalcWeight[pSystemEffect->GetType()] = TRUE;
                }
                
                NTL_DELETE(pSystemEffect);
                it = m_listProxySystemEffect.erase(it);
            }
            else
            {
                pSystemEffect->Update(fElapsed);
                if(pSystemEffect->IsCalcWeightValue())
                {
                    bIsCalcWeight[pSystemEffect->GetType()] = TRUE;
                }
                ++it;
            }
        }

        if(bIsCalcWeight[CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COLOR] || bIsCalcWeight[CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COLOR_FLICKER])
            UpdateSystemEffectColor(fElapsed);
        if(bIsCalcWeight[CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_ALPHA_BLEND])
            UpdateSystemEffectAlpha(fElapsed);
        if(bIsCalcWeight[CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_SCALE])
            UpdateSystemEffectScale(fElapsed);
    }
}

void CNtlSobProxy::EnableProxySystemVisible(RwBool bVisible)
{
	if(bVisible)
		m_uiSystemFlags = m_uiSystemFlags & (~NTL_PROXYSYSTEM_NOT_VISIBLE);
	else
		m_uiSystemFlags = m_uiSystemFlags | NTL_PROXYSYSTEM_NOT_VISIBLE;
}

void CNtlSobProxy::EnableProxySystemGUILPVisible(RwBool bVisible)
{
	if(bVisible)
		m_uiSystemFlags = m_uiSystemFlags | NTL_PROXYSYSTEM_LP_VISIBLE;
	else
		m_uiSystemFlags = m_uiSystemFlags & (~NTL_PROXYSYSTEM_LP_VISIBLE);
		
}

void CNtlSobProxy::UpdateSystemEffectColor(RwReal fElapsed)
{
    RwReal fWeightRed	= 1.0f;
	RwReal fWeightGreen = 1.0f;
	RwReal fWeightBlue	= 1.0f;

	RwReal fCurrWeightRed	= 1.0f;
	RwReal fCurrWeightGreen = 1.0f;
	RwReal fCurrWeightBlue	= 1.0f;

    RwRGBAReal crAddColor;
    ZeroMemory(&crAddColor, sizeof(RwRGBAReal));

	for each(CNtlSobProxySystemEffectColor *pSystemEffect in m_listProxySystemEffect)
	{
        if(pSystemEffect->GetType() != CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COLOR &&
           pSystemEffect->GetType() != CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COLOR_FLICKER)
            continue;
        
		pSystemEffect->GetWeightValue(fWeightRed, fWeightGreen, fWeightBlue);

        if(pSystemEffect->IsAddColorFlag())
        {
            crAddColor.red      += fWeightRed;
            crAddColor.green    += fWeightGreen;
            crAddColor.blue     += fWeightBlue;
        }
        else
        {
            fCurrWeightRed		*= fWeightRed;
            fCurrWeightGreen	*= fWeightGreen;
            fCurrWeightBlue		*= fWeightBlue;
        }
	}

	SetColor( (RwUInt8)(255.0f * fCurrWeightRed), (RwUInt8)(255.0f * fCurrWeightGreen),(RwUInt8)(255.0f * fCurrWeightBlue) );

    // Add Color    
        GetPLMainEntity()->SetAddColor((RwUInt8)(crAddColor.red * 255.0f), 
                                       (RwUInt8)(crAddColor.green * 255.0f),
                                       (RwUInt8)(crAddColor.blue * 255.0f));
    
}

void CNtlSobProxy::UpdateSystemEffectAlpha(RwReal fElapsed)
{
	RwBool bAlphaEffectNum = 0;
	RwReal fWeightValue = 1.0f;
	RwBool bExceptDecorationProxy = FALSE;

    CNtlSobProxySystemEffectAlphaBlend *pSystemEffectAlpha;
    ListProxySystemEffect::iterator it;
	for(it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end(); it++)
	{
        if((*it)->GetType() != CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_ALPHA_BLEND)
            continue;

		bAlphaEffectNum++;
        pSystemEffectAlpha = (CNtlSobProxySystemEffectAlphaBlend*)(*it);

		fWeightValue *= pSystemEffectAlpha->GetWeightValue();

		if ( pSystemEffectAlpha->IsExceptDecorationProxy() )
		{
			bExceptDecorationProxy = TRUE;
		}
	}

	// 형석 작업(2008.03.10) - 민근 이 코딩 의미 있슴.
	if(bAlphaEffectNum == 0)
		return;

	if(fWeightValue < 1.0f)
	{
		SetInkThickness(0.0f);
	}
	else
	{
		SetInkThickness(NTL_INK_THICKNESS);
	}

	if(fWeightValue == 0.0f)
	{
		if( m_pSobObj->IsVisible() )
		{
			SetAlpha( 0 );
			m_pSobObj->EnableVisible(FALSE);
		}

		if(bExceptDecorationProxy && m_pSobObj->GetSobProxy())
		{
			m_pSobObj->GetSobProxy()->SetDecorationProxyAlpha( 255 ); // by daneos: 125 -> 255 to keep name fully visible when in vehicle.
			m_pSobObj->GetSobProxy()->EnableDecorationProxyVisible( TRUE );
		}
	}
	else
	{
		SetAlpha( (RwUInt8)(fWeightValue*255.0f) );

		if( !m_pSobObj->IsVisible() )
			m_pSobObj->EnableVisible(TRUE);

		if(bExceptDecorationProxy && m_pSobObj->GetSobProxy())
		{
			m_pSobObj->GetSobProxy()->SetDecorationProxyAlpha( 125 );
			m_pSobObj->GetSobProxy()->EnableDecorationProxyVisible( TRUE );
		}
	}
}

void CNtlSobProxy::UpdateSystemEffectScale( RwReal fElapsed ) 
{
    RwReal fScale = 1.0f;
    
    CNtlSobProxySystemEffectScale* pSystemEffectScale = NULL;
    for each(CNtlSobProxySystemEffect* pSystemEffect in m_listProxySystemEffect)
    {
        if(pSystemEffect->GetType() == CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_SCALE)
        {
            pSystemEffectScale = (CNtlSobProxySystemEffectScale*)pSystemEffect;
            fScale *= pSystemEffectScale->GetWeightValue();
        }
    }

    SetScale(fScale);
}

CNtlPLEntity* CNtlSobProxy::CreatePLEffect(const RwChar *pKey, const RwV3d *pPos /* = NULL */, RwBool bIgnoreVisible /* = FALSE */ )
{
	if(!bIgnoreVisible && !m_pSobObj->IsVisible())
		return NULL;

	if(pKey == NULL)
		return NULL;

	RwInt32 iKeyLen = (RwInt32)strlen(pKey);
	if(iKeyLen == NULL)
		return NULL;

	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);
	if(pPLEntity == NULL)
		return NULL;

    if(pPos)
    {
	    pPLEntity->SetPosition(pPos);    
    }
    else 
    {
        pPLEntity->SetPosition(&m_pSobObj->GetPosition());
    }

	// elapsed controller setting
	pPLEntity->SetSerialID(m_pSobObj->GetSerialID());
	RwReal fLifeTime, fWeightValue;
	if(GetNtlSobElapsedControlManager()->GetControlFactor(m_pSobObj, fLifeTime, fWeightValue))
		pPLEntity->CreateWeightElapsedController(fLifeTime, fWeightValue);

    pPLEntity->SetScale(GetScale());

	return pPLEntity;
}

CNtlPLEntity* CNtlSobProxy::CreatePLChildEffect(const RwChar *pKey, RwBool bIgnoreVisible /*= false*/)
{
	if(!bIgnoreVisible)
	{
		if(!m_pSobObj->IsVisible())
			return NULL;
	}

	if(pKey == NULL)
		return NULL;

	RwInt32 iKeyLen = (RwInt32)strlen(pKey);
	if(iKeyLen == NULL)
		return NULL;

	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);
	if(pPLEntity == NULL)
		return NULL;

	AddPLChildEntity(pPLEntity);

	// elapsed controller setting
	pPLEntity->SetSerialID(m_pSobObj->GetSerialID());
	RwReal fLifeTime, fWeightValue;
	if(GetNtlSobElapsedControlManager()->GetControlFactor(m_pSobObj, fLifeTime, fWeightValue))
		pPLEntity->CreateWeightElapsedController(fLifeTime, fWeightValue);

	return pPLEntity;
}


CNtlPLEntity* CNtlSobProxy::CreatePLDamageBox(RwV3d vPos, RwUInt32 uiDamage, RwBool bHealing /*= FALSE*/)
{
	if(!m_pSobObj->IsVisible())
		return NULL;

	RwV3d vDamagePos;
	SPLDamageBoxCreateParam sParam;
	RwV3dAssignMacro(&vDamagePos, &vPos);
	vDamagePos.y += GetPLEntityHeight() + NTL_DAMAGEBOX_OFFSET;
	sParam.pPos = &vDamagePos;
	sParam.uiDamage = uiDamage;

	RwUInt32 uiClassId = m_pSobObj->GetClassID(); 
	if(bHealing)
	{
		if(uiClassId == SLCLASS_AVATAR || uiClassId == SLCLASS_PLAYER)
			sParam.iClolorType = DAMAGEBOX_COLOR_PC_HEAL;
		else
			sParam.iClolorType = DAMAGEBOX_COLOR_MOB_HEAL;
	}
	else
	{
		if(uiClassId == SLCLASS_AVATAR || uiClassId == SLCLASS_PLAYER)
			sParam.iClolorType = DAMAGEBOX_COLOR_PC;
		else
			sParam.iClolorType = DAMAGEBOX_COLOR_MOB;
	}

	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_DAMAGE_BOX, "NULL", &sParam);

	// elapsed controller setting
	pPLEntity->SetSerialID(m_pSobObj->GetSerialID());
	RwReal fLifeTime, fWeightValue;
	if(GetNtlSobElapsedControlManager()->GetControlFactor(m_pSobObj, fLifeTime, fWeightValue))
		pPLEntity->CreateWeightElapsedController(fLifeTime, fWeightValue);

	return pPLEntity;
}

void CNtlSobProxy::DeletePLChildEffect(CNtlPLEntity *pPLEntity)
{
	ListPLEntity::iterator it;
	for(it = m_listPLChild.begin(); it != m_listPLChild.end(); it++)
	{
		if((*it) == pPLEntity)
		{
			GetSceneManager()->DeleteEntity((*it));
			m_listPLChild.erase(it);
			return;
		}
	}
}

RwV3d CNtlSobProxy::GetPosition(void)
{
	return CNtlPLGlobal::m_vZeroV3;
}


RwV3d CNtlSobProxy::GetDirection(void) 
{
	return CNtlPLGlobal::m_vZeroV3;
}

void CNtlSobProxy::AddPLChildEntity(CNtlPLEntity *pPLEntity)
{
	if(pPLEntity == NULL)
		return;

	RwV3d vPos = m_pSobObj->GetPosition();
	RwV3d vDir = m_pSobObj->GetDirection();

	RwMatrix mat;
	CNtlMath::MathRwMatrixAssign(&mat, &vPos, &vDir);
	pPLEntity->SetMatrix(mat); 
	
	if(pPLEntity->IsAutoDelete())
		return;

	m_listPLChild.push_back(pPLEntity);
}


void CNtlSobProxy::TransformPLChildEntity(RwMatrix& mat)
{
	ListPLEntity::iterator it;
	for(it = m_listPLChild.begin(); it != m_listPLChild.end(); it++)
	{
		(*it)->SetMatrix(mat);
	}
}

RwV3d CNtlSobProxy::GetBonePosition(const RwChar *pBoneName)
{
	return m_pSobObj->GetPosition();
}

RwV3d CNtlSobProxy::GetWeaponBonePosition(const RwChar *pBoneName)
{
	return m_pSobObj->GetPosition();
}

RwV3d CNtlSobProxy::GetSubWeaponBonePosition(const RwChar *pBoneName)
{
	return m_pSobObj->GetPosition();
}

void CNtlSobProxy::EnableVisible(RwBool bEnable)
{
	ListPLEntity::iterator it;
	for(it = m_listPLChild.begin(); it != m_listPLChild.end(); it++)
	{
		(*it)->SetVisible(bEnable);
	}
}

void CNtlSobProxy::CreateElapsedController(RwReal fCtrlTime, RwReal fWeightValue)
{
	ListPLEntity::iterator it;
	for(it = m_listPLChild.begin(); it != m_listPLChild.end(); it++)
	{
		(*it)->CreateWeightElapsedController(fCtrlTime, fWeightValue);
	}
}

void CNtlSobProxy::DeleteElapsedController(void)
{
	ListPLEntity::iterator it;
	for(it = m_listPLChild.begin(); it != m_listPLChild.end(); it++)
	{
		(*it)->DeleteWeightElapsedController();
	}
}

CNtlSobProxySystemEffect* CNtlSobProxy::AddVisualSystemEffectAlphaBlend(RwReal fWeightAlpha, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy /*= FALSE*/)	
{
	CNtlSobProxySystemEffectAlphaBlend *pAlphaBlend = NTL_NEW CNtlSobProxySystemEffectAlphaBlend(this);
	pAlphaBlend->SetData(fWeightAlpha, fLifeTime, bLoop, bExceptDecorationProxy);
	pAlphaBlend->Enter();

	m_listProxySystemEffect.push_back(pAlphaBlend);

	return pAlphaBlend;
}

CNtlSobProxySystemEffect* CNtlSobProxy::AddVisualSystemEffectAlphaBlend(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy /*= FALSE*/)
{
	CNtlSobProxySystemEffectAlphaBlend *pAlphaBlend = NTL_NEW CNtlSobProxySystemEffectAlphaBlend(this);
	pAlphaBlend->SetData(fStartWeightAlpha, fEndWeightAlpha, fBlendTime, fLifeTime, bLoop, bExceptDecorationProxy);
	pAlphaBlend->Enter();

	m_listProxySystemEffect.push_back(pAlphaBlend);

	return pAlphaBlend;
}

void CNtlSobProxy::ResetRemoveSystemEffectAlpha()
{
	SetAlpha( 255 );
	SetInkThickness(NTL_INK_THICKNESS);

	if( !m_pSobObj->IsVisible() )
		m_pSobObj->EnableVisible(TRUE);
}

void CNtlSobProxy::RemoveVisualSystemEffectAlpha(CNtlSobProxySystemEffect *pSystemEffect)
{
	RwBool bRemoveSystemEffectAlpha = FALSE;
	if(pSystemEffect->GetType() == CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_ALPHA_BLEND)
		bRemoveSystemEffectAlpha = TRUE;

	ListProxySystemEffect::iterator it;
	for(it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end(); it++)
	{
		if((*it) == pSystemEffect)
		{
			pSystemEffect->Exit();
			NTL_DELETE(pSystemEffect);
			m_listProxySystemEffect.erase(it);
			break;
		}
	}

	// (형석 - 2008.05.20)
	// system effect alpha가 존재하는지를 판단. 없으면 reset 해준다.
	if(!bRemoveSystemEffectAlpha)
		return;

	for(it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end(); it++)
	{
		// update에서 alpha system effect가 존재한다면?
		if((*it)->GetType() == CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_ALPHA_BLEND)
		{
			return;
		}
	}

	ResetRemoveSystemEffectAlpha();
}

CNtlSobProxySystemEffect* CNtlSobProxy::AddVisualSystemEffectColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwBool bAddColor)
{
    CNtlSobProxySystemEffectColor* pSystemColor = NTL_NEW CNtlSobProxySystemEffectColor(this);
    pSystemColor->SetData(byRed, byGreen, byBlue, bAddColor);
    pSystemColor->Enter();

    m_listProxySystemEffect.push_back(pSystemColor);

    return pSystemColor;
}

void CNtlSobProxy::RemoveVisualSystemEffectColor( CNtlSobProxySystemEffect* pSystemEffect ) 
{
    ListProxySystemEffect::iterator it;
    for(it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end(); it++)
    {
        if((*it) == pSystemEffect)
        {
            pSystemEffect->Exit();
            NTL_DELETE(pSystemEffect);
            m_listProxySystemEffect.erase(it);
            UpdateSystemEffectColor(0.0f);
            return;
        }
    }
}

CNtlSobProxySystemEffect* CNtlSobProxy::AddVisualSystemEffectColorFlickering(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fFlickerTime)
{
	CNtlSobProxySystemEffectColorFlickering *pColorFlickering = NTL_NEW CNtlSobProxySystemEffectColorFlickering(this);
	pColorFlickering->SetData(byRed, byGreen, byBlue, fFlickerTime);
	pColorFlickering->Enter();

	m_listProxySystemEffect.push_back(pColorFlickering);

	return pColorFlickering;
}

void CNtlSobProxy::RemoveVisualSystemEffectColorFlickering(CNtlSobProxySystemEffect *pSystemEffect)
{
	ListProxySystemEffect::iterator it;
	for(it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end(); it++)
	{
		if((*it) == pSystemEffect)
		{
			pSystemEffect->Exit();
			NTL_DELETE(pSystemEffect);
			m_listProxySystemEffect.erase(it);
			break;
		}
	}

    RwBool bEmpty = TRUE;
    for each(CNtlSobProxySystemEffect* pProxySystemEffect in m_listProxySystemEffect)
    {
        if(pProxySystemEffect->GetType() == CNtlSobProxySystemEffect::PROXY_SYSTEM_EFFECT_COLOR_FLICKER)
        {
            bEmpty = FALSE;
            break;
        }
    }

	if(bEmpty)
	{
		SetColor(255, 255, 255);
	}
}

CNtlSobProxySystemEffect* CNtlSobProxy::AddVisualSystemEffectScale( RwReal fEndScale, RwReal fTime ) 
{
    CNtlSobProxySystemEffectScale* pScaleEffect = NTL_NEW CNtlSobProxySystemEffectScale(this);
    pScaleEffect->SetData(fEndScale, fTime);
    pScaleEffect->Enter();

    m_listProxySystemEffect.push_back(pScaleEffect);

    return pScaleEffect;
}

RwBool CNtlSobProxy::IsExitSystemEffect( CNtlSobProxySystemEffect* pSystemEffect ) 
{
    ListProxySystemEffect::iterator it;
    for(it = m_listProxySystemEffect.begin(); it != m_listProxySystemEffect.end(); it++)
    {
        if((*it) == pSystemEffect)
            return TRUE;
    }

    return FALSE;
}

void CNtlSobProxy::SetAlpha(RwUInt8 byAlpha)
{
	m_byAlpha = byAlpha;
}

RwUInt8	CNtlSobProxy::GetAlpha(void)
{
	return m_byAlpha;
}

void CNtlSobProxy::AddAlpha(RwUInt8 byAddValue)
{
	RwInt32 iAlpha = m_byAlpha + byAddValue;
	if(iAlpha < 0)
		iAlpha = 0;

	if(iAlpha > 255)
		iAlpha = 255;

	SetAlpha((RwUInt8)iAlpha);
}

void CNtlSobProxy::EnableWorldLight(RwBool bEnable, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if (bEnable)
	{
		if (!m_pWorldLight)
		{
			m_pWorldLight = NTL_NEW CNtlSobProxySystemEffectColor(this);
			m_pWorldLight->Enter();

			m_listProxySystemEffect.push_back(m_pWorldLight);
		}

		m_pWorldLight->SetData(byRed, byGreen, byBlue);
	}
	else
	{
		if(m_pWorldLight)
		{
			RemoveVisualSystemEffectColorFlickering(m_pWorldLight);
			m_pWorldLight = NULL;
		}
	}
}

/**
 * 카메라와의 거리에 따른 알파 이펙트 적용시에 사용하는 함수
 * \param bEnable 적용 유무
 */
void CNtlSobProxy::EnableCameraAlpha( RwBool bEnable ) 
{
    if(m_bEnableCameraAlpha == bEnable)
        return;

	RwUInt32 uiFlags = m_pSobObj->GetFlags();

	// 형석.
	// 현재 camera와 충돌한 object가 Community NPC 이면 aplha를 적용하지 않느다.
	if(bEnable && uiFlags & SLFLAG_NOT_ALPHA_CAMERA_COLLI)
		return;

    m_bEnableCameraAlpha = bEnable;
    
    static RwReal fAlpha = 100.0f / 255.0f;

    if(bEnable)
    {
		m_pCameraAlphaEffect = AddVisualSystemEffectAlphaBlend(fAlpha, 0.0f, TRUE);
    }
    else if(m_pCameraAlphaEffect)
    {
        RemoveVisualSystemEffectAlpha(m_pCameraAlphaEffect);        
        m_pCameraAlphaEffect = NULL;
    }
}