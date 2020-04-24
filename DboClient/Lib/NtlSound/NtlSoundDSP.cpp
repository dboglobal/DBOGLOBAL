#include "precomp_ntlsound.h"
#include "NtlSoundDSP.h"

// core
#include "NtlDebug.h"

// sound
#include "NtlSoundGlobal.h"

CNtlSoundDSP::~CNtlSoundDSP()
{
	NTL_ASSERT(m_mapDSP.size() == 0, "CNtlSoundDSP::~CNtlSoundDSP, There is used dsp");
}

VOID CNtlSoundDSP::Destroy()
{
	MAP_DSP_ITER it = m_mapDSP.begin();
	for( ; it != m_mapDSP.end() ; ++it )
	{
		FMOD::DSP* pDSP = it->second;
		pDSP->release();
	}
	m_mapDSP.clear();
}

FMOD::DSP* CNtlSoundDSP::CreateDSP(FMOD_DSP_TYPE eType)
{
	MAP_DSP_ITER it = m_mapDSP.find(eType);
	if( it != m_mapDSP.end() )
	{
		NTL_ASSERT(false, "CNtlSoundDSP::SetDSP, Already exist dps type : " << eType);
		return NULL;
	}	

	FMOD::DSP* pDSP = NULL;
	
	FMOD_RESULT result = CNtlSoundGlobal::m_pFMODSystem->createDSPByType(eType, &pDSP);
	if( result == FMOD_OK )
	{
		SetDetailParameta(eType, pDSP);
		m_mapDSP[eType] = pDSP;
	}
	else
		Logic_NtlSoundLog("CNtlSoundDSP::CreateDSP, fail to create dsp", result);

	return pDSP;
}

VOID CNtlSoundDSP::ReleaseDSP(FMOD_DSP_TYPE eType)
{
	MAP_DSP_ITER it = m_mapDSP.find(eType);
	if( it == m_mapDSP.end() )
	{
		NTL_ASSERT(false, "CNtlSoundDSP::ReleaseDSP, Not exist dps type : " << eType);
		return;
	}	

	FMOD::DSP* pDSP = it->second;
	pDSP->release();
	m_mapDSP.erase(it);
}

VOID CNtlSoundDSP::SetParameta(FMOD_DSP_TYPE eType, RwUInt8 byParametaType, RwReal fValue)
{
	MAP_DSP_ITER it = m_mapDSP.find(eType);
	if( it == m_mapDSP.end() )
	{
		NTL_ASSERT(false, "CNtlSoundDSP::SetParameta, Not exist dps type : " << eType);
		return;
	}	

	FMOD::DSP* pDSP = it->second;
	pDSP->setParameterFloat(byParametaType, fValue);
	//pDSP->setParameter(byParametaType, fValue);
}

RwReal CNtlSoundDSP::GetParameta(FMOD_DSP_TYPE eType, RwUInt8 byParametaType)
{
	MAP_DSP_ITER it = m_mapDSP.find(eType);
	if( it == m_mapDSP.end() )
	{
		NTL_ASSERT(false, "CNtlSoundDSP::GetParameta, Not exist dps type : " << eType);
		return 0.f;
	}	

	// FMOD 문서 참고 : getParameter의 3번째 인자 즉 버퍼로 들어오는 문자는 16자를 넘지 않는다
	RwReal fValue;
	char acBuffer[17] = "";
	FMOD::DSP* pDSP = it->second;
	pDSP->getParameterFloat(byParametaType, &fValue, acBuffer, 17);

	return fValue;
}

RwBool CNtlSoundDSP::IsExistDSP(FMOD_DSP_TYPE eType)
{
	if( m_mapDSP.find(eType) == m_mapDSP.end() )
		return FALSE;

	return TRUE;
}
VOID CNtlSoundDSP::SetDetailParameta(FMOD_DSP_TYPE eType, FMOD::DSP* pDSP)
{
	/*if( eType == FMOD_DSP_TYPE_REVERB )
	{
		pDSP->setParameter(FMOD_DSP_REVERB_ROOMSIZE,	0.01f);
		pDSP->setParameter(FMOD_DSP_REVERB_DAMP,		0.f);
		pDSP->setParameter(FMOD_DSP_REVERB_WETMIX,		0.23f);
		pDSP->setParameter(FMOD_DSP_REVERB_DRYMIX,		0.55f);
		pDSP->setParameter(FMOD_DSP_REVERB_WIDTH,		0.f);
		pDSP->setParameter(FMOD_DSP_REVERB_MODE,		0.f);
	}*/
}