#include "precomp_ntlpresentation.h"
#include "NtlPLSound.h"
#include "NtlDebug.h"
#include "NtlPLSceneManager.h"
#include "NtlMath.h"
#include "NtlPLEntityFreeList.h"
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

#ifdef dNTL_WORLD_TOOL_MODE

#include "NtlPLObject.h"
#include "NtlWorldPaletteDefine.h"

#endif


CNtlPLSound::CNtlPLSound(void)
{
	SetLayer(PLENTITY_LAYER_SOUND);
	SetClassType(PLENTITY_SOUND);
	SetFlags(NTL_PLEFLAG_NOT_PROPERTY_USED);

#ifdef dNTL_WORLD_TOOL_MODE

	m_pNtlPLObj = NULL;

#endif

	m_Volume	= 1.0f;
	m_RadiusMin	= 10.0f;	
	m_RadiusMax	= 50.0f;
	m_hSound	= INVALID_SOUND_HANDLE;
	ZeroMemory(m_SoundFileName, sizeof(m_SoundFileName));
}

CNtlPLSound::~CNtlPLSound(void)
{
#ifdef dNTL_WORLD_TOOL_MODE

	if(m_pNtlPLObj)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObj);
	}

#endif
}

void* CNtlPLSound::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLSound::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_SOUND));
}

void CNtlPLSound::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_SOUND, pObj);
}

VOID CNtlPLSound::SetVision(RwBool _Flag)
{
#ifdef dNTL_WORLD_TOOL_MODE

	if(_Flag)
	{
		if(!m_pNtlPLObj)
		{
			SPLEntityCreateParam Param;
			Param.pPos = &m_Pos;

			m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_SOUND", &Param));
			DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

			m_pNtlPLObj->SetFadeEnable(FALSE);

			RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);
		}
	}
	else
	{
		if(m_pNtlPLObj)
		{
			GetSceneManager()->DeleteEntity(m_pNtlPLObj);
			m_pNtlPLObj = NULL;
		}
	}

#endif
}

RwBool CNtlPLSound::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLSound::Create");

#ifdef dNTL_WORLD_TOOL_MODE

	if(dGET_CUR_PALETTE() == ePM_SE)
	{
		m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_SOUND", pParam));
		DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

		m_pNtlPLObj->SetFadeEnable(FALSE);

		RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);
	}

#endif

	CNtlMath::MathRwV3dAssign(&m_Pos, pParam->pPos->x, pParam->pPos->y, pParam->pPos->z);

	NTL_RETURN(TRUE);
}

void CNtlPLSound::Destroy()
{
	NTL_FUNCTION("CNtlPLSound::Destroy");

#ifdef dNTL_WORLD_TOOL_MODE

	if(m_pNtlPLObj)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObj);
		m_pNtlPLObj = NULL;
	}

#endif

	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		GetSoundManager()->Stop(m_hSound);
		m_hSound = INVALID_SOUND_HANDLE;
	}

	NTL_RETURNVOID();
}

RwBool CNtlPLSound::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLSound::Update");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLSound::Render(void)
{
	NTL_FUNCTION("CNtlPLSound::Render");

	NTL_RETURN(TRUE);
}

RwInt32 CNtlPLSound::PlaySound(RwChar* pFileName)
{
	strcpy_s(m_SoundFileName, sizeof(m_SoundFileName), pFileName);

	string BGMPath	= "Environment\\";
	string FileName	= pFileName;
	BGMPath += pFileName;

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= CHANNEL_GROUP_OBJECT_MUSIC;
	tSoundParam.pcFileName		= (char*)BGMPath.c_str();
	tSoundParam.bLoop			= true;
	tSoundParam.fVolume			= m_Volume;
	tSoundParam.fXPos			= m_Pos.x;
	tSoundParam.fYPos			= m_Pos.y;
	tSoundParam.fZPos			= m_Pos.z;
	tSoundParam.fMinDistance	= m_RadiusMin;
	tSoundParam.fMaxDistance	= m_RadiusMax;

	RwInt32 iResult = GetSoundManager()->Play(&tSoundParam);
	m_hSound = tSoundParam.hHandle;

	return iResult;
}

RwInt32 CNtlPLSound::PlayEffectSound(RwChar* pFileName, RwBool bLoop, RwInt32 nVolume, RwReal fDistance, RwReal fDecayDist, RwReal fPitch)
{
    // NOTE : Sound files for effects are only present in the Sound \ Effect subfolder.   
    std::string strSoundPath = "Effect\\";
    strSoundPath += pFileName;
	//DBO_WARNING_MESSAGE("Play Sound: " << strSoundPath.c_str());

    strcpy_s(m_SoundFileName, sizeof(m_SoundFileName), strSoundPath.c_str());

    m_Volume = (RwReal)nVolume / 100.0f;

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
	tSoundParam.pcFileName		= (char*)strSoundPath.c_str();
	tSoundParam.bLoop			= bLoop;
	tSoundParam.fVolume			= m_Volume;
	tSoundParam.fPitch			= fPitch;	
	tSoundParam.fMinDistance	= fDistance;
	tSoundParam.fMaxDistance	= fDecayDist;

	RwInt32 iResult = GetSoundManager()->Play(&tSoundParam);
	m_hSound = tSoundParam.hHandle;

    return iResult;
}

VOID CNtlPLSound::ControlVolume(RwReal Value2Append)
{
	m_Volume += Value2Append;
	CLAMP(m_Volume, 0.0f, 1.0f);
	GetSoundManager()->SetChannelVolume(m_hSound, m_Volume);
}

VOID CNtlPLSound::ControlRadiusMin(RwReal Value2Append)
{
	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		GetSoundManager()->Stop(m_hSound);
		m_hSound = INVALID_SOUND_HANDLE;
	}

	string BGMPath	= "Environment\\";
	string FileName	= m_SoundFileName;
	BGMPath += FileName;

	m_RadiusMin += Value2Append;

	if(m_RadiusMin > m_RadiusMax)
		m_RadiusMin = (m_RadiusMax - 0.1f);

	CLAMP(m_RadiusMin, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= CHANNEL_GROUP_OBJECT_MUSIC;
	tSoundParam.pcFileName		= (char*)BGMPath.c_str();
	tSoundParam.bLoop			= TRUE;
	tSoundParam.fVolume			= m_Volume;
	tSoundParam.fXPos			= m_Pos.x;
	tSoundParam.fYPos			= m_Pos.y;
	tSoundParam.fZPos			= m_Pos.z;
	tSoundParam.fMinDistance	= m_RadiusMin;
	tSoundParam.fMaxDistance	= m_RadiusMax;

	GetSoundManager()->Play(&tSoundParam);
	m_hSound = tSoundParam.hHandle;
}

VOID CNtlPLSound::ControlRadiusMax(RwReal Value2Append)
{
	m_RadiusMax += Value2Append;

	if(m_RadiusMax < m_RadiusMin)
		m_RadiusMax = (m_RadiusMin + 0.1f);

	CLAMP(m_RadiusMax, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
	GetSoundManager()->SetMinMaxDistance(m_hSound, m_RadiusMin, m_RadiusMax);
}

RwBool CNtlPLSound::SetProperty(const CNtlPLProperty *pProperty)
{
	NTL_FUNCTION("CNtlPLSound::SetProperty");

	//strcpy_s(m_SoundFileName, 32, const_cast<CNtlPLProperty*>(pProperty)->GetName());

	NTL_RETURN(TRUE);
}

RwV3d CNtlPLSound::GetPosition(void)
{
	return m_Pos;
/*
	RwV3d vPos = { 0.f, 0.f, 0.f };

#ifdef dNTL_WORLD_TOOL_MODE

	vPos = m_pNtlPLObj->GetPosition();

#endif

	return vPos;
*/
}

void CNtlPLSound::SetPosition(const RwV3d *pPos)
{

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pNtlPLObj)
	{
		m_pNtlPLObj->SetPosition(pPos);
	}
#endif

	CNtlMath::MathRwV3dAssign(&m_Pos, pPos->x, pPos->y, pPos->z);

	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		GetSoundManager()->SetSoundPosition(m_hSound, m_Pos.x, m_Pos.y, m_Pos.z);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLSoundBGM::CNtlPLSoundBGM()
{
	SetLayer(PLENTITY_LAYER_SOUND);
	SetClassType(PLENTITY_SOUND_BGM);
	SetFlags(NTL_PLEFLAG_NOT_PROPERTY_USED);

	m_Radius = 50.0f;
}

CNtlPLSoundBGM::~CNtlPLSoundBGM()
{
}

void* CNtlPLSoundBGM::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLSound::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_SOUND_BGM));
}

void CNtlPLSoundBGM::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_SOUND_BGM, pObj);
}

RwInt32 CNtlPLSoundBGM::PlaySound(RwChar* pFileName)
{
	strcpy_s(m_SoundFileName, sizeof(m_SoundFileName), pFileName);

	string BGMPath	= "BGM\\";
	BGMPath			+= m_SoundFileName;

	CNtlSoundEventGenerator::SendRangeBGM_AddList(&m_hSound, BGMPath.c_str(), m_Radius, &m_Pos);	

	if (m_hSound == INVALID_SOUND_HANDLE)
	{
		return SOUNDRESULT_NOT_CREATED_SOUND;
	}		
	return SOUNDRESULT_OK;
}

VOID CNtlPLSoundBGM::ControlRadiusMin(RwReal Value2Append)
{
	m_Radius += Value2Append;

	CLAMP(m_Radius, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);

	m_RadiusMin = m_RadiusMax = m_Radius;

	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		string BGMPath	= "Environment\\";
		BGMPath			+= m_SoundFileName;
		CNtlSoundEventGenerator::SendRangeBGM_ChangeParam(m_hSound, BGMPath.c_str(), m_Radius, &m_Pos);
	}
}

VOID CNtlPLSoundBGM::ControlRadiusMax(RwReal Value2Append)
{
	m_Radius += Value2Append;

	CLAMP(m_Radius, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);

	m_RadiusMin = m_RadiusMax = m_Radius;

	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		string BGMPath	= "Environment\\";
		BGMPath			+= m_SoundFileName;
		CNtlSoundEventGenerator::SendRangeBGM_ChangeParam(m_hSound, BGMPath.c_str(), m_Radius, &m_Pos);
	}
}

VOID CNtlPLSoundBGM::ControlVolume(RwReal Value2Append)
{
	// 	m_Volume += Value2Append;
	// 	CLAMP(m_Volume, 0.0f, 1.0f);
	// 	GetSoundManager()->SetChannelVolume(m_hSound, m_Volume);
}

VOID CNtlPLSoundBGM::SetVision(RwBool _Flag)
{
#ifdef dNTL_WORLD_TOOL_MODE

	if(_Flag)
	{
		if(!m_pNtlPLObj)
		{
			SPLEntityCreateParam Param;
			Param.pPos = &m_Pos;

			m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_SOUND", &Param));
			DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

			m_pNtlPLObj->SetFadeEnable(FALSE);

			RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);
		}
	}
	else
	{
		if(m_pNtlPLObj)
		{
			GetSceneManager()->DeleteEntity(m_pNtlPLObj);
			m_pNtlPLObj = NULL;
		}
	}

#endif
}

void CNtlPLSoundBGM::SetPosition(const RwV3d *pPos)
{

#ifdef dNTL_WORLD_TOOL_MODE
	if (m_pNtlPLObj)
	{
		m_pNtlPLObj->SetPosition(pPos);
	}
#endif

	CNtlMath::MathRwV3dAssign(&m_Pos, pPos->x, pPos->y, pPos->z);

	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		string BGMPath	= "BGM\\";
		BGMPath			+= m_SoundFileName;
		CNtlSoundEventGenerator::SendRangeBGM_ChangeParam(m_hSound, BGMPath.c_str(), m_Radius, &m_Pos);
	}
}

RwBool CNtlPLSoundBGM::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLSound::Create");

#ifdef dNTL_WORLD_TOOL_MODE

	if(dGET_CUR_PALETTE() == ePM_SE)
	{
		m_pNtlPLObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_SOUND", pParam));
		DBO_ASSERT(m_pNtlPLObj, "Entity create failed.");

		m_pNtlPLObj->SetFadeEnable(FALSE);

		RpNtlClumpSetData(m_pNtlPLObj->GetClump(), this);
	}

#endif

	CNtlMath::MathRwV3dAssign(&m_Pos, pParam->pPos->x, pParam->pPos->y, pParam->pPos->z);

	NTL_RETURN(TRUE);
}

void CNtlPLSoundBGM::Destroy()
{
	NTL_FUNCTION("CNtlPLSoundBGM::Destroy");

#ifdef dNTL_WORLD_TOOL_MODE

	if(m_pNtlPLObj)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLObj);
		m_pNtlPLObj = NULL;
	}

#endif

	if(m_hSound != INVALID_SOUND_HANDLE)
	{
		CNtlSoundEventGenerator::SendRangeBGM_DelList(m_hSound);
		m_hSound = INVALID_SOUND_HANDLE;
	}

	NTL_RETURNVOID();
}