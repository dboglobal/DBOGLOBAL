#include "precomp_ntlpresentation.h"
#include "NtlPLPlanetHandler.h"
#include "NtlPLEvent.h"
#include "NtlPLSceneManager.h"
#include "ntlworldcommon.h"
#include "NtlPLWorldState.h"

CNtlPLPlanetHandler::CNtlPLPlanetHandler(void)
{
	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		m_pPlanetGroupReal[i] = NULL;
		m_pPlanetGroupDumm[i] = NULL;
	}

	m_bVisible = TRUE;
}

CNtlPLPlanetHandler::~CNtlPLPlanetHandler(void)
{
	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		if(m_pPlanetGroupReal[i])
		{
			GetSceneManager()->DeleteEntity(m_pPlanetGroupReal[i]);
			m_pPlanetGroupReal[i] = NULL;
		}

		if(m_pPlanetGroupDumm[i])
		{
			GetSceneManager()->DeleteEntity(m_pPlanetGroupDumm[i]);
			m_pPlanetGroupDumm[i] = NULL;
		}
	}
}

RwBool CNtlPLPlanetHandler::IsInterpolationNeeded(sNTL_FIELD_PROP* _pNewFieldProp)
{
	RwBool SameFlags[dNTLPL_PLANET_MAX_CNT];
	ZeroMemory(SameFlags, sizeof(RwBool) * dNTLPL_PLANET_MAX_CNT);

	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		if(m_pPlanetGroupReal[i])
		{
			if(m_pPlanetGroupReal[i]->Compare(_pNewFieldProp->_NtlPlanet[i]))
			{
				SameFlags[i] = TRUE;
			}
			else
			{
				SameFlags[i] = FALSE;
			}
		}
		else if(!m_pPlanetGroupReal[i])
		{
			if(_pNewFieldProp->_NtlPlanet[i].IsEmpty())
			{
				SameFlags[i] = TRUE;
			}
			else
			{
				SameFlags[i] = FALSE;
			}
		}
	}

	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		if(!SameFlags[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNtlPLPlanetHandler::HandleEvents(RWS::CMsg &pMsg)
{
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNewFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		if(!IsInterpolationNeeded(pNewFieldProp))
		{
			return;
		}

		for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
		{
			m_pPlanetGroupDumm[i] = m_pPlanetGroupReal[i];
			if(m_pPlanetGroupDumm[i])
			{
				m_pPlanetGroupDumm[i]->SetFadeEffect(FALSE, dNTLPL_PLANET_INT_SEC);
				m_pPlanetGroupDumm[i]->SetVisibility(m_bVisible);
			}

			m_pPlanetGroupReal[i] = NULL;

			if(!pNewFieldProp->_NtlPlanet[i].IsEmpty())
			{
				RwInt32				PlanetTypeIdx = eSFBT_PLANET;
				SPLSunCreateParam	Param;

				Param.pPos						= &pNewFieldProp->_NtlPlanet[i]._RelativePos;
				Param.bAttachLensFlare			= pNewFieldProp->_NtlPlanet[i]._LensFlareExistence;
				Param.strTexture				= pNewFieldProp->_NtlPlanet[i]._TexName;
				Param.Scale						= pNewFieldProp->_NtlPlanet[i]._Scale;
				Param._pUserData				= &PlanetTypeIdx;

				m_pPlanetGroupReal[i] = static_cast<CNtlPLPlanet*>(GetSceneManager()->CreateEntity(PLENTITY_SUN, "", &Param));
				DBO_ASSERT(m_pPlanetGroupReal[i], "Entity create failed.");

				m_pPlanetGroupReal[i]->SetPosition(Param.pPos);
				m_pPlanetGroupReal[i]->SetScale(Param.Scale);

				RwRGBA RGBA = {255, 255, 255, 0};
				m_pPlanetGroupReal[i]->SetColor(RGBA);
				m_pPlanetGroupReal[i]->SetFadeEffect(TRUE, dNTLPL_PLANET_INT_SEC);
				m_pPlanetGroupReal[i]->SetVisibility(m_bVisible);
			}
		}
	}
}

void CNtlPLPlanetHandler::Update(RwReal _ElapsedTime)
{
	for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
	{
		if(m_pPlanetGroupDumm[i])
		{
			if(m_pPlanetGroupDumm[i]->WasFadeOutDone())
			{
				GetSceneManager()->DeleteEntity(m_pPlanetGroupDumm[i]);
				m_pPlanetGroupDumm[i] = NULL;
			}
		}
	}
}

void CNtlPLPlanetHandler::SetVisible(RwBool bVisible)
{
	if (bVisible != m_bVisible)
	{
		m_bVisible = bVisible;
		for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
		{
			if (m_pPlanetGroupReal[i])
			{
				m_pPlanetGroupReal[i]->SetVisibility(m_bVisible);
			}
			if(m_pPlanetGroupDumm[i])
			{
				m_pPlanetGroupDumm[i]->SetVisibility(m_bVisible);
			}
		}
	}
}