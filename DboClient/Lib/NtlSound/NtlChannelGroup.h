/*****************************************************************************
*
* File			: NtlChannelGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Sound channel group
*****************************************************************************
* Desc			: 게임 상에서의 사운드 Channel 성격에 따라 Group 지어진 channel
*				을 일괄 관리할 수 있다.
*****************************************************************************/

#pragma once

#include <map>
#include <string>

// core
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"

// sound
#include "NtlSoundDefines.h"

class CNtlSound;
class CNtlSoundDSP;

class CNtlChannelGroup
{
public:
	CNtlChannelGroup(eChannelGroupType eGroup);
	virtual ~CNtlChannelGroup();

	typedef std::map<SOUND_HANDLE, CNtlSound*>				SOUND_MAP;
	typedef std::map<SOUND_HANDLE, CNtlSound*>::value_type	SOUND_VALUE;
	typedef std::map<SOUND_HANDLE, CNtlSound*>::iterator	SOUND_ITER;

	typedef std::map<FMOD_DSP_TYPE, FMOD::DSP*>				MAP_DSP;
	typedef std::map<FMOD_DSP_TYPE, FMOD::DSP*>::iterator	MAP_DSP_ITER;


	virtual bool			Create(FMOD::ChannelGroup* pChannelGroup, unsigned int uiSoundDuplicatoin = 0);
	///< FMOD::System에 의해서 만들어진 채널 그룹을 등록한다
	///< uiSoundDuplicatoin : 같은 사운드를 동시에 중복해서 연주할 수 있는 횟수

	virtual void			Reset();
	virtual void			Update(float fElapsed);
	virtual void			Destory();	

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);
	virtual void			StopGroup();

	virtual CNtlSound*		GetSound(SOUND_HANDLE hHandle);

	virtual void			AddEffect(FMOD_DSP_TYPE eType);
	virtual void			RemoveEffect(FMOD_DSP_TYPE eType);
	virtual void			RemoveAllEffect();

	virtual bool			ReleaseLowRankChannel();
	virtual int				CanPlay(const char* pcName);

	virtual void			SetMute(bool bMute);

	virtual void			SetProhibition(RwBool bProhibition);
	virtual RwBool			IsProhibition();

	virtual void			SetValidGroupRange(float fRange);
	virtual float			GetValidGroupRange();

	virtual void			SetGroupVolume(float fVolume);
	virtual bool			SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);
	virtual void			SetVolume_FadeInit();
	virtual float			GetGroupVolume();
	virtual sNtlVolume*		GetNtlVolume();

	virtual void			SetGroupPitch(float fPitch);
	virtual void			SetPitch_SlowMotionRate(float fPitchRate);
	virtual void			SetPitch_InitSlowMotionRate();
	virtual float			GetGroupPitch();

	CNtlSoundDSP*			GetDSP();

	virtual eChannelGroupType GetGroupType();
	virtual unsigned int	GetPlayingChannels();
	virtual unsigned int	GetSoundDuplication();
	virtual FMOD::ChannelGroup*	GetFMODChannelGroup();

	virtual bool			IsExistSound(SOUND_HANDLE hHandle);	
	virtual bool			IsMute();

protected:	
	virtual void			ReleaseFinishedSound(float fElapsed);

public:	
	eChannelGroupType			m_eGroup;					///< 채널 그룹
	unsigned int				m_uiSoundDuplication;		///< 같은 사운드를 동시에 중복해서 연주할 수 있는 횟수

	sNtlVolume					m_tVolume;
	sNtlPitch					m_tPitch;

	float						m_fValidRange;				///< 연주 가능한 리스너로부터의 거리
	RwBool						m_bProhibition;				///< Mute 기능이 아닌 아예 플레이를 하지 않는 상태	

	CNtlSoundDSP*				m_pDSP;
	FMOD::ChannelGroup*			m_pMasterLayer;				///< FMOD channel group master layer
	SOUND_MAP					m_mapGroup;
	MAP_DSP						m_mapDSP;
};