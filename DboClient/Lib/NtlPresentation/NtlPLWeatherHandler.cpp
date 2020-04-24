#include "precomp_ntlpresentation.h"

#include "NtlWorldDI.h"

#include "NtlDebug.h"
#include "NtlWorldCommon.h"

#include "NtlPLEvent.h"
#include "NtlPLWorldState.h"

#include "NtlPLSceneManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLOptionManager.h"

#include "NtlPLWorldEntity.h"

#include "NtlPLWeatherHandler.h"

CNtlPLWeatherHandler::CNtlPLWeatherHandler()
:m_bVisible(TRUE)
//,m_uiWeatherLevel(0)
{
	CNtlPLWeatherParticleFreeList::AllocFreeList();

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);
}

CNtlPLWeatherHandler::~CNtlPLWeatherHandler()
{
	UnLoadWeatherDumm();
	UnLoadWeatherReal();

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	CNtlPLWeatherParticleFreeList::DeAllocFreeList();
}

void CNtlPLWeatherHandler::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlPLWeatherHandler::HandleEvents");

	if (CheckEventsWorldEffectChanged(pMsg))
	{
#ifdef dNTL_WORLD_TOOL_MODE
		if (!m_bVisible)
		{
			NTL_RETURNVOID();
		}
#endif
		sNTL_FIELD_PROP*		pNtlFieldProp	= reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		CNtlPLWeather*			pPLWeather		= NULL;
		eNTL_WORLD_DIR			NWD				= GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetWorldFieldMoveDir();

		RwReal fFadeTime = dNTL_PL_WEATHER_FADE_TIME;
		if (NWD == ePORTAL || NWD == eC || NWD == eIDLE)
		{
			fFadeTime = 0.0f;
		}

		// FADE OUT : Real -> Dumm
		LoadWeatherDumm(fFadeTime);
		// FADE IN : New Real Push
		LoadWeatherReal(pNtlFieldProp, fFadeTime);
	}

	NTL_RETURNVOID();
}

void CNtlPLWeatherHandler::Update(RwReal fElapsedTime)
{
	NTL_FUNCTION("CNtlPLWeatherHandler::Update");

	VEC_WEATHER_IT it = m_vecWeatherDumm.begin();
	while (it != m_vecWeatherDumm.end())
	{
		CNtlPLWeather* pPLWeather = *it;
		if (pPLWeather->GetFadeState() == EPLWEATHER_FADE_OUT_DONE && pPLWeather->GetCountParticle() == 0)
		{
			it = m_vecWeatherDumm.erase(it);

			GetSceneManager()->DeleteEntity(pPLWeather);
		}
		else
		{
			++it;
		}
	}
	NTL_RETURNVOID();
}

void CNtlPLWeatherHandler::SetVisible(RwBool bVisible/*, RwReal fFade*/)
{	
	if (m_bVisible == bVisible)
	{
		return;
	}

	if ((m_bVisible = bVisible))
	{
		RefreshWeather();		
	}
	else
	{
		InitWeather();
	}
}

RwInt32 CNtlPLWeatherHandler::GetNumList()
{
	RwInt32 iNum = 0;

	for (int i = 0; i < (int)m_vecWeatherDumm.size(); ++i)
	{
		iNum += m_vecWeatherDumm.at(i)->GetCountParticle();
	}

	for (int i = 0; i < (int)m_vecWeatherReal.size(); ++i)
	{
		iNum += m_vecWeatherReal.at(i)->GetCountParticle();
	}

	return iNum;
}

RwInt32 CNtlPLWeatherHandler::GetNumRender()
{
	RwInt32 iNum = 0;

	for (int i = 0; i < (int)m_vecWeatherDumm.size(); ++i)
	{
		iNum += m_vecWeatherDumm.at(i)->GetCountRender();
	}

	for (int i = 0; i < (int)m_vecWeatherReal.size(); ++i)
	{
		iNum += m_vecWeatherReal.at(i)->GetCountRender();
	}

	return iNum;
}

void CNtlPLWeatherHandler::InitWeather()
{
	UnLoadWeatherDumm();
	UnLoadWeatherReal();
}

void CNtlPLWeatherHandler::RefreshWeather()
{
	if (!m_bVisible)
	{
		return;
	}

	UnLoadWeatherReal();
	UnLoadWeatherDumm();

	CNtlWorldFieldManager* pNWFManager = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	sNTL_FIELD_PROP sNtlFieldProp;
	pNWFManager->GetAFieldProp(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))), sNtlFieldProp);

	LoadWeatherReal(&sNtlFieldProp, 0.0f);
}

void CNtlPLWeatherHandler::LoadWeatherReal(sNTL_FIELD_PROP* pNtlFieldProp, RwReal fFadeTime)
{
	char acKeyName[256];
	sprintf_s(acKeyName, 256, "%s_%u", NTL_PLEN_WEATHER, CNtlPLOptionManager::GetInstance()->GetWeatherLevel());
	for (int i = 0; i < dPL_WEATHER_MAX_COUNT; ++i)
	{
		if (pNtlFieldProp->_IdxWeather[i] != -1)
		{
			SPLEntityCreateParam sPLEntityCreateParam;
			sPLEntityCreateParam.pPos		= RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
			sPLEntityCreateParam._pUserData	= (void*)pNtlFieldProp->_IdxWeather[i];

			CNtlPLWeather* pPLWeather = reinterpret_cast<CNtlPLWeather*>(GetSceneManager()->CreateEntity(PLENTITY_WEATHER, acKeyName, &sPLEntityCreateParam));
			DBO_ASSERT(pPLWeather, "Entity create failed.");
			
			m_vecWeatherReal.push_back(pPLWeather);
			pPLWeather->SetFadeState(EPLWEATHER_FADE_IN, fFadeTime);
		}
	}
}

void CNtlPLWeatherHandler::LoadWeatherDumm(RwReal fFadeTime)
{
	CNtlPLWeather* pPLWeather	= NULL;
	VEC_WEATHER_IT it			= m_vecWeatherReal.begin();
	while (it != m_vecWeatherReal.end())
	{
		pPLWeather = *it;

		it = m_vecWeatherReal.erase(it);

		m_vecWeatherDumm.push_back(pPLWeather);

		pPLWeather->SetFadeState(EPLWEATHER_FADE_OUT, fFadeTime);							
	}
}

void CNtlPLWeatherHandler::UnLoadWeatherReal()
{
	VEC_WEATHER_IT it;

	it = m_vecWeatherReal.begin();
	while (it != m_vecWeatherReal.end())
	{
		CNtlPLWeather* pPLWeather = *it;
		if (pPLWeather)
		{
			GetSceneManager()->DeleteEntity(pPLWeather);
			it = m_vecWeatherReal.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void CNtlPLWeatherHandler::UnLoadWeatherDumm()
{
	VEC_WEATHER_IT it;

	it = m_vecWeatherDumm.begin();
	while (it != m_vecWeatherDumm.end())
	{
		CNtlPLWeather* pPLWeather = *it;
		if (pPLWeather)
		{
			GetSceneManager()->DeleteEntity(pPLWeather);
			it = m_vecWeatherDumm.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// void CNtlPLWeatherHandler::SetWeatherLevel(RwUInt32 uiWeatherLevel)
// {
// 	m_uiWeatherLevel = uiWeatherLevel;
// 	CLAMP(m_uiWeatherLevel, 0, dNTL_PL_WEATHER_PROP_NUM);
// 	
// 	RefreshWeather();
// }
// 
// RwUInt32 CNtlPLWeatherHandler::GetWeatherLevel()
// {
// 	return m_uiWeatherLevel;
// }