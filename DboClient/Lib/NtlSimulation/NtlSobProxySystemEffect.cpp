#include "precomp_ntlsimulation.h"
#include "NtlSobProxySystemEffect.h"

// core
#include "NtlDebug.h"

// PL
#include "NtlPLEntityBlend.h"
#include "NtlSoundManager.h"

// SL
#include "NtlSobProxy.h"


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

CNtlSobProxySystemEffect::CNtlSobProxySystemEffect() 
{
    m_pSobProxy = NULL;
}

CNtlSobProxySystemEffect::CNtlSobProxySystemEffect(CNtlSobProxy *pSobProxy)
{
    m_pSobProxy = pSobProxy;    
}

CNtlSobProxySystemEffect::~CNtlSobProxySystemEffect() 
{
    if(m_hSound != INVALID_SOUND_HANDLE)
    {
		GetSoundManager()->Stop(m_hSound);
        m_hSound = INVALID_SOUND_HANDLE;
    }
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

CNtlSobProxySystemEffectAlphaBlend::CNtlSobProxySystemEffectAlphaBlend()
{
    m_eType = PROXY_SYSTEM_EFFECT_ALPHA_BLEND;
    m_pWeightAlpha	= NULL;
    m_hSound = INVALID_SOUND_HANDLE;
	m_bExceptDecorationProxy = FALSE;
}

CNtlSobProxySystemEffectAlphaBlend::CNtlSobProxySystemEffectAlphaBlend(CNtlSobProxy *pSobProxy)
:CNtlSobProxySystemEffect(pSobProxy)
{
    m_eType = PROXY_SYSTEM_EFFECT_ALPHA_BLEND;
    m_pWeightAlpha	= NULL;
}

CNtlSobProxySystemEffectAlphaBlend::~CNtlSobProxySystemEffectAlphaBlend()
{
    if(m_pWeightAlpha)
    {
        NTL_DELETE(m_pWeightAlpha);
    }
}

void CNtlSobProxySystemEffectAlphaBlend::Enter(void)
{

}

void CNtlSobProxySystemEffectAlphaBlend::Exit(void)
{

}

void CNtlSobProxySystemEffectAlphaBlend::Update(RwReal fElapsed)
{
    if(m_pWeightAlpha)
        m_pWeightAlpha->Update(fElapsed);
}

RwBool CNtlSobProxySystemEffectAlphaBlend::IsFinish(void)
{
    if(m_pWeightAlpha)
        return m_pWeightAlpha->IsFinish();

    return FALSE;
}

RwBool CNtlSobProxySystemEffectAlphaBlend::IsCalcWeightValue(void)
{
    if(m_pWeightAlpha)
        return m_pWeightAlpha->IsCalcWeightValue();

    return FALSE;
}

RwReal CNtlSobProxySystemEffectAlphaBlend::GetWeightValue(void)
{
    if(m_pWeightAlpha)
        return m_pWeightAlpha->GetWeight();

    return 1.0f;
}

RwBool CNtlSobProxySystemEffectAlphaBlend::IsExceptDecorationProxy(void)
{
	return m_bExceptDecorationProxy;
}

void CNtlSobProxySystemEffectAlphaBlend::SetData(RwReal fWeightAlpha, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy /*= FALSE*/)
{
    m_pWeightAlpha = NTL_NEW CNtlPLEntityAlphaWeightBlend;
    m_pWeightAlpha->SetWeight(fWeightAlpha, fLifeTime, bLoop);

	m_bExceptDecorationProxy = bExceptDecorationProxy;
}

void CNtlSobProxySystemEffectAlphaBlend::SetData(RwReal fStartWeightAlpha, RwReal fEndWeightAlpha, RwReal fBlendTime, RwReal fLifeTime, RwBool bLoop, RwBool bExceptDecorationProxy /*= FALSE*/)
{
    m_pWeightAlpha = NTL_NEW CNtlPLEntityAlphaWeightBlend;
    m_pWeightAlpha->SetWeight(fStartWeightAlpha, fEndWeightAlpha, fBlendTime, fLifeTime, bLoop);
	m_bExceptDecorationProxy = bExceptDecorationProxy;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

CNtlSobProxySystemEffectColor::CNtlSobProxySystemEffectColor()
{
    m_eType = PROXY_SYSTEM_EFFECT_COLOR,
    m_fWeightRed	= 1.0f;
    m_fWeightGreen	= 1.0f;
    m_fWeightBlue	= 1.0f;

    m_byUpdateCount	= 0;

    m_bAddColorFlag = FALSE;
}

CNtlSobProxySystemEffectColor::CNtlSobProxySystemEffectColor(CNtlSobProxy *pSobProxy)
:CNtlSobProxySystemEffect(pSobProxy)
{
    m_eType = PROXY_SYSTEM_EFFECT_COLOR,
    m_fWeightRed	= 1.0f;
    m_fWeightGreen	= 1.0f;
    m_fWeightBlue	= 1.0f;

    m_byUpdateCount	= 0;

    m_bAddColorFlag = FALSE;    

}

CNtlSobProxySystemEffectColor::~CNtlSobProxySystemEffectColor()
{
}

void CNtlSobProxySystemEffectColor::Update(RwReal fElapsed)
{
    m_byUpdateCount++;
}

void CNtlSobProxySystemEffectColor::Enter(void)
{
    CNtlSobProxySystemEffect::Enter();
}

void CNtlSobProxySystemEffectColor::Exit(void)
{
    CNtlSobProxySystemEffect::Exit();
}

RwBool CNtlSobProxySystemEffectColor::IsFinish(void)
{
    return FALSE;
}

RwBool CNtlSobProxySystemEffectColor::IsCalcWeightValue(void)
{
    if(m_byUpdateCount > 2)
        return FALSE;

    return TRUE;
}

void CNtlSobProxySystemEffectColor::SetWeightValue(RwReal fWeightRed, RwReal fWeightGreen, RwReal fWeightBlue)
{
    m_fWeightRed	= fWeightRed;
    m_fWeightGreen	= fWeightGreen;
    m_fWeightBlue	= fWeightBlue;
}

void CNtlSobProxySystemEffectColor::GetWeightValue(RwReal& fWeightRed, RwReal& fWeightGreen, RwReal& fWeightBlue)
{
    fWeightRed		= m_fWeightRed;
    fWeightGreen	= m_fWeightGreen;
    fWeightBlue		= m_fWeightBlue;
}

void CNtlSobProxySystemEffectColor::SetData(RwInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwBool bAddColor /* = FALSE */)
{
    m_byRed		= byRed;
    m_byGreen	= byGreen;
    m_byBlue	= byBlue;
    m_bAddColorFlag = bAddColor;

    m_fWeightRed	= (RwReal)m_byRed / 255.0f;
    m_fWeightGreen	= (RwReal)m_byGreen / 255.0f;
    m_fWeightBlue	= (RwReal)m_byBlue / 255.0f;

    m_byUpdateCount	= 0;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


CNtlSobProxySystemEffectColorFlickering::CNtlSobProxySystemEffectColorFlickering()
{
    m_byRed			= 255;
    m_byGreen		= 255;
    m_byBlue		= 255;

    m_bOn			= TRUE;
    m_fTime			= 0.0f;
    m_fFlickerTime	= 1.0f;
    m_bAddColorFlag = FALSE;
}

CNtlSobProxySystemEffectColorFlickering::CNtlSobProxySystemEffectColorFlickering(CNtlSobProxy *pSobProxy)
:CNtlSobProxySystemEffectColor(pSobProxy)
{
    m_eType         = PROXY_SYSTEM_EFFECT_COLOR_FLICKER,
        m_bOn			= FALSE;
    m_fTime			= 0.0f;
    m_fFlickerTime	= 1.0f;
}

CNtlSobProxySystemEffectColorFlickering::~CNtlSobProxySystemEffectColorFlickering()
{
}

void CNtlSobProxySystemEffectColorFlickering::Enter(void)
{
    CNtlSobProxySystemEffectColor::Enter();
}

void CNtlSobProxySystemEffectColorFlickering::Exit(void)
{
    CNtlSobProxySystemEffectColor::SetWeightValue( (RwReal)255.0f, (RwReal)255.0f, (RwReal)255.0f );

    CNtlSobProxySystemEffectColor::Exit();
}

void CNtlSobProxySystemEffectColorFlickering::Update(RwReal fElapsed)
{
    m_fTime += fElapsed;

    if(m_bOn)
    {
        if(m_fTime >= m_fFlickerTime)
        {
            CNtlSobProxySystemEffectColor::SetWeightValue( (RwReal)m_byRed/255.0f, (RwReal)m_byGreen/255.0f, (RwReal)m_byBlue/255.0f );
            m_fTime = 0.0f;
            m_bOn = !m_bOn;
        }
        else
        {
            RwReal fWeightValue = m_fTime / m_fFlickerTime;

            RwInt32 iRed = (RwInt32)( 255 - ( (255 - m_byRed) * fWeightValue ) );
            RwInt32 iGreen = (RwInt32)( 255 - ( (255 - m_byGreen) * fWeightValue ) );
            RwInt32 iBlue = (RwInt32)( 255 - ( (255 - m_byBlue) * fWeightValue ) );

            if(iRed < 0)
                iRed = 0;

            if(iGreen < 0)
                iGreen = 0;

            if(iBlue < 0)
                iBlue = 0;

            CNtlSobProxySystemEffectColor::SetWeightValue( (RwReal)iRed/255.0f, (RwReal)iGreen/255.0f, (RwReal)iBlue/255.0f );

        }
    }
    else
    {
        if(m_fTime >= m_fFlickerTime)
        {
            CNtlSobProxySystemEffectColor::SetWeightValue( 1.0f, 1.0f, 1.0f );

            m_fTime = 0.0f;
            m_bOn = !m_bOn;
        }
        else
        {
            RwReal fWeightValue = m_fTime / m_fFlickerTime;

            RwInt32 iRed = (RwInt32)( m_byRed + ( (255 - m_byRed) * fWeightValue ) );
            RwInt32 iGreen = (RwInt32)( m_byGreen + ( (255 - m_byGreen) * fWeightValue ) );
            RwInt32 iBlue = (RwInt32)( m_byBlue + ( (255 - m_byBlue) * fWeightValue ) );

            if(iRed > 255)
                iRed = 255;

            if(iGreen > 255)
                iGreen = 255;

            if(iBlue > 255)
                iBlue = 255;

            CNtlSobProxySystemEffectColor::SetWeightValue( (RwReal)iRed/255.0f, (RwReal)iGreen/255.0f, (RwReal)iBlue/255.0f );
        }
    }
}

RwBool CNtlSobProxySystemEffectColorFlickering::IsCalcWeightValue(void)
{
    return TRUE;
}

void CNtlSobProxySystemEffectColorFlickering::SetData(RwInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fFlickerTime)
{
    m_byRed			= byRed;
    m_byGreen		= byGreen;
    m_byBlue		= byBlue;

    m_fFlickerTime	= fFlickerTime;
}

//////////////////////////////////////////////////////////////////////////
CNtlSobProxySystemEffectScale::CNtlSobProxySystemEffectScale() 
{
    m_eType = PROXY_SYSTEM_EFFECT_SCALE;
    m_fWeightValue = m_fStartValue = m_fEndValue = 0.0f;
    m_fElapsedTime = m_fTime = 0.0f;
}

CNtlSobProxySystemEffectScale::CNtlSobProxySystemEffectScale( CNtlSobProxy* pSobProxy ) 
: CNtlSobProxySystemEffect(pSobProxy)
{
    m_eType = PROXY_SYSTEM_EFFECT_SCALE;
    m_fWeightValue = m_fStartValue = m_fEndValue = 0.0f;
    m_fElapsedTime = m_fTime = 0.0f;
    m_bFinish = FALSE;
}

CNtlSobProxySystemEffectScale::~CNtlSobProxySystemEffectScale() 
{

}

void CNtlSobProxySystemEffectScale::Enter( void ) 
{

}

void CNtlSobProxySystemEffectScale::Exit( void ) 
{

}

void CNtlSobProxySystemEffectScale::Update( RwReal fElapsed ) 
{
    if(m_bFinish)
        return;

    m_fElapsedTime += fElapsed;
    m_fElapsedTime = min(m_fElapsedTime, m_fTime);

    if(m_fElapsedTime >= m_fTime)
    {
        m_fWeightValue = m_fEndValue;
        m_bFinish = TRUE;
    }
    else
    {
        RwReal fDeltaTime = m_fElapsedTime / m_fTime;
        m_fWeightValue = (1 - fDeltaTime) * m_fStartValue + fDeltaTime * m_fEndValue;
    }
}

RwBool CNtlSobProxySystemEffectScale::IsFinish( void ) 
{
    return m_bFinish;
}

RwBool CNtlSobProxySystemEffectScale::IsCalcWeightValue( void ) 
{
    return !m_bFinish;
}

RwReal CNtlSobProxySystemEffectScale::GetWeightValue() 
{
    return m_fWeightValue;
}

void CNtlSobProxySystemEffectScale::SetData( RwReal fEndScale, RwReal fTime ) 
{
    m_fStartValue = m_pSobProxy->GetScale();
    m_fEndValue = fEndScale;
    m_fTime = fTime;
    m_bFinish = FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
