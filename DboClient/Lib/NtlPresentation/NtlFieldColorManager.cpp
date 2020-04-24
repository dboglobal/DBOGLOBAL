#include "precomp_ntlpresentation.h"

#include "NtlPLApi.h"

#include "NtlPLWorldState.h"

#include "NtlPLVisualManager.h"
#include "NtlFieldColorManager.h"

CNtlFieldColorManager* CNtlFieldColorManager::g_pFieldColorManager = NULL;

CNtlFieldColorManager::CNtlFieldColorManager()
{
	m_clrf.alpha	= 1.0f;
	m_clrf.red		= 1.0f;
	m_clrf.green	= 1.0f;
	m_clrf.blue		= 1.0f;

	m_clrfNew = m_clrfOld = m_clrf;

	m_bFading		= FALSE;
}

CNtlFieldColorManager::~CNtlFieldColorManager()
{
}

void CNtlFieldColorManager::CreateInstance()
{
	if (!g_pFieldColorManager)
	{
		g_pFieldColorManager = NTL_NEW CNtlFieldColorManager;
	}
}

void CNtlFieldColorManager::DestroyInstance()
{
	NTL_DELETE(g_pFieldColorManager);
}

RwBool CNtlFieldColorManager::Create()
{
	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	return FALSE;
}

void CNtlFieldColorManager::Destroy()
{
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);
}

void CNtlFieldColorManager::HandleEvents(RWS::CMsg &pMsg)
{
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		SetFieldColor(&pNtlFieldProp->_FieldColor);
	}
}

void CNtlFieldColorManager::Update(RwReal fElapsed)
{
	if (m_bFading)
	{
		UpdateFade(fElapsed);
	}
}

void CNtlFieldColorManager::SetFieldColor(RwRGBA* pColor)
{
	RwRGBAReal clrf;
	RwRGBARealFromRwRGBA(&clrf, pColor);

	if (m_clrfNew.red == clrf.red && m_clrfNew.green == clrf.green && m_clrfNew.blue == clrf.blue)
	{
		return;
	}
	RunableFade(&clrf, 3.0f);
}

void CNtlFieldColorManager::RunableFade(RwRGBAReal* pColorf, RwReal fTime)
{
	m_clrfOld		= m_clrf;
	m_clrfNew		= *pColorf;
	m_fFadeTime		= fTime;
	m_fFadeElapsed	= 0.0f;
	m_bFading		= TRUE;
}

void CNtlFieldColorManager::UpdateFade(RwReal fElapsed)
{
	m_fFadeElapsed += fElapsed;

	RwReal fDelta = m_fFadeElapsed / m_fFadeTime;
	if(fDelta > 1.0f)
	{
		m_bFading	= FALSE;
		fDelta		= 1.0f;
	}

	m_clrf.red		= m_clrfOld.red + (fDelta * (m_clrfNew.red - m_clrfOld.red));
	m_clrf.green	= m_clrfOld.green + (fDelta * (m_clrfNew.green - m_clrfOld.green));
	m_clrf.blue		= m_clrfOld.blue + (fDelta * (m_clrfNew.blue - m_clrfOld.blue));
}