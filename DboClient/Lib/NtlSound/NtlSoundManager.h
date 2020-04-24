/*****************************************************************************
*
* File			: NtlSoundManaager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Ntl Sound Manager by Singleton
*****************************************************************************
* Desc: Background music is usually played only one song and is converted to another background music
* The previous background music is Fade Out and the new background music is Fade In
*				do.
*
* FMOD uses the left hand coordinate system.
*
* In fmod, the volume is a float value between 0.0 and 1.0. Entered volume value
* fVolume <0.0 fVolume = 0.0
* fVolume> 1.0 fVolume = 1.0
*
* FMOD :: sound has information about the sound source.
* FMOD :: Channel has current play information of sound source.
*
* The isPlaying function is called to see if the sound has finished playing.
* In FMOD, compare the length of the entire sound with the offset of the sound currently playing.
*, The current offset of the finished sound is 0
I can not figure it out.
*
* Background music is read in stream format and background loading
* When background music is in Loop mode When you play another background music
* The Loop mode argument to the Play (..) function does not apply.
* If you have PostMusic when background music changes, you can change PostMusic status
* To apply to new background music SetBackgroundMusicLoop ()
* Should be called.
*
* SoundManager interface for sounding situations
Control directly from the Sound Layer via event handlers rather than stretching
*				lets do it
*****************************************************************************/

#pragma once

#include "NtlSoundMessage.h"
#include "NtlSoundDefines.h"

#pragma comment(lib, "fmod_vc.lib")


class CNtlChannelGroup;
class CNtlSoundDSP;
class CNtlSound;
class CNtlSoundSubSystem;

class CNtlSoundManager
{
protected:
	CNtlSoundManager();
	CNtlSoundManager(const CNtlSoundManager& soundManager);

public:
	virtual ~CNtlSoundManager();

	static CNtlSoundManager* GetInstance();

	RwBool		IsUsableSound();


	// System

	///< FMOD System 초기화, Channel Group 초기화
	///< fDopplerScale : 반향( 음원의 이동 속도, 소리를 듣는 주체의 이동 속도에 따른 음파의 변화)
	///< fDistanceFactor : 소리가 들리는 길이의 비율(ex : 1미터, 1센티)
	///< fRollOffScale : 감쇠(거리에 따른 불륨의 크기 감소
	void		Init(const char* pcPath, float fMasterVolume = 1.0, float fDopplerScale = 1.0,
					 float fDistacneFactor = 1.0, float fRollOffScale = 1.0);				

	void		Reset();
	void		Release();					///< FMOD 시스템 Shut down

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos);	///< Lister 3D 위치 설정

	void		SetListenerPosition(float fXPos, float fYPos, float fZPos, 
									float fXFoward, float fYFoward, float fZFoward, 
									float fXUp, float fYUp, float fZUp); 

	int			Play(sNtlSoundPlayParameta* pParameta);

	int			ReplayEnvironmentSound(CNtlSound* pSound);///< 오브젝트 뮤직 다시 플레이 한다.(핸들이 바뀌지 않는다)
	int			Replay(CNtlSound* pSound);		///< 기존에 플레이되고 있는 사운드를 다시 플레이한다.

	void		Update(float fElapsed);			///< 각 ChannelGroup Update와 FMOD::System->update()



	//////////////////////////////////////////////////////////////////////////
	//				 Master Group
	//////////////////////////////////////////////////////////////////////////

	unsigned int	GetPlayingChannels();			///< 현재 연주되고 sound channel의 갯수

	///< 볼륨
	void		SetMasterVolume(float fVolume);
	float		GetMasterVolume();

	// 효과 적용
	void		SetMasterEffect(FMOD_DSP_TYPE eType);
	void		ReleaseMasterEffect(FMOD_DSP_TYPE eType);

	void		SetMute(bool bMute);					///< 전체 사운드의 Mute 설정


	//////////////////////////////////////////////////////////////////////////
	//				 Channel Group
	//////////////////////////////////////////////////////////////////////////

	void		StopGroup(int iChannelGroup);	///< 채널 그룹 전체를 즉각 Release 시킨다.

	CNtlChannelGroup*	GetChannelGroup(int iChannelGroup);
	char*		GetGroupName(int iChannelGroup);

	void		SetGroupVolume(int iChannelGroup, float fVolume);///< ChannelGroup의 볼륨을 설정한다
	float		GetGroupVolume(int iChannelGroup);				///< ChannelGroup의 볼륨을 반환한다

	// 효과 적용
	void		SetGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);
	void		ReleaseGroupEffect(int iChannelGroup, FMOD_DSP_TYPE eType);

	void		SetMute(int iChannelGroup, bool bMute);	///< ChannelGroup의 활성화를 설정 t : 끄기, f : 켜기

	void		SetProhibition(int iChannelGroup, RwBool bProhibition);
	RwBool		IsProhibition(int iChannelGroup);

	void		SetValidGroupRange(int iChannelGroup, float fRange);	///< 채널 그룹별 연주 가능한 거리를 설정한다.
	float		GetValidGroupRange(int iChannelGroup);				///< 채널 그룹별 연주 가능한 거리를 알아본다.

	void		FadeIn(int iGroup, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 크거나 ulTime이 0 이하면 실행되지 않는다

	void		FadeOut(int iGroup, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 작거나 ulTime이 0 이하면 실행되지 않는다

	bool		IsMute(int iChannelGroup);


	//////////////////////////////////////////////////////////////////////////
	//				 Channel
	//////////////////////////////////////////////////////////////////////////

	void		Stop(SOUND_HANDLE& rHandle);	///< 사운드를 Release 시킨다.
	int			SetSoundPosition(SOUND_HANDLE hHandle, float fPosX, float fPosY, float fPosZ);	///< 채널의 3D 공간상의 위치를 이동시킨다.

	const char*	GetSoundName(SOUND_HANDLE hHandle);	///< 사운드의 이름을 반환한다

	void		SetChannelVolume(SOUND_HANDLE hHandle, float fVolume);	///< Channel의 볼륨을 설정한다
	float		GetChannelVolume(SOUND_HANDLE hHandle);					///< Channel의 볼륨을 반환한다

	// 효과 적용
	void		SetChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);
	void		ReleaseChannelEffect(SOUND_HANDLE hHandle, FMOD_DSP_TYPE eType);

	void		SetMinMaxDistance(SOUND_HANDLE hHandle, float fMinDistance, float fMaxDistance);
	void		GetMinMaxDistance(SOUND_HANDLE hHandle, float &fMinDistance, float &fMaxDistance);

	void		FadeIn(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 크거나 ulTime이 0 이하면 실행되지 않는다

	void		FadeOut(SOUND_HANDLE hHandle, float fDestVolume, unsigned int ulTime);
	///< fVolume이 현재 볼륨보다 작거나 ulTime이 0 이하면 실행되지 않는다

	CNtlSound*	GetSound(SOUND_HANDLE hHandle);			///< 사운드 정보를 반환한다.

protected:
	void		CreateChannelGroups();		///< Channel Group 을 초기화한다	

	int			CanPlay(int iChannelGroups, const char* pcName,
						float fPosX, float fPosY, float fPosZ);	///< 새로운 사운드를 연주할 수 있는지 여부

	int			CanPlay(sNtlSoundPlayParameta* pParameta);

	bool		IsValidGroupRange(int iChannelGroup, float fPosX, float fPosY, float fPosZ);
	///< 채널 그룹별 연주할 수 있는 범위내에 사운드인지 검사

	float		LengthFromListenerToSound(float fPosX, float fPosY, float fPosZ);
	bool		IsExistGroup(int iGroup);	///< true : 알맞는 Group index, false : 존재하지 않는 그룹 index


protected:
	FMOD::ChannelGroup*		m_pMasterChannelGroup;		///< Top Channel Group of Ntl Sound
	CNtlChannelGroup*		m_apChannelGroup[NUM_CHANNEL_GROUP];	///< Ntl Channel Group		
	CNtlSoundDSP*			m_pMasterDSP;
	
	CNtlSoundSubSystem*		m_pSubSystem;

#ifdef _DEBUG
	int						m_iDebugFlag;				///< crtdbg용
#endif
};

static CNtlSoundManager* GetSoundManager()
{
	return CNtlSoundManager::GetInstance();
}