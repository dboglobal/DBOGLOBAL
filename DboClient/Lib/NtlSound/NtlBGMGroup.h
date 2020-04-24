/*****************************************************************************
*
* File			: NtlBGMGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 28
* Abstract		: Background Music Channel Group
*****************************************************************************
* Desc			: 다른 배경음악으로 전환될시 이전 배경음악(PostMusic)은
*				  Fade Out 이 되며 사라진다
*
*				  배경음악은 우선순위가 있어서 우선순위 배경음악의 연주가
*				  끝나야 대기중인 배경음악이 Play된다.
*****************************************************************************/

#pragma once

#include <string>
#include "ceventhandler.h"
#include "NtlChannelGroup.h"

class CNtlBGMPlayList;


class CNtlBGMGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
protected:
	struct sPLAY_BGM_INFO
	{
		SOUND_HANDLE	hHandle;
		eBGMType		eType;
	};

public:
	CNtlBGMGroup(eChannelGroupType eGroup);
	virtual ~CNtlBGMGroup();

	virtual bool			Create(FMOD::ChannelGroup* pChannelGroup, unsigned int uiSoundDuplicatoin = 0);
	virtual void			Update(float fElapsed);
	virtual void			Destory();

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);
	void					StopImmediately(SOUND_HANDLE hHandle);

public:
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

protected:
	void			Update_Replay(float fElapsed);
	void			ClearBGMList(SOUND_HANDLE hHandle);

	virtual void	ReleaseFinishedSound(float fElapsed);

protected:
	float			m_fReplayElapsed;						///< 현재 음악의 다음 재생까지 남은 시간

	sPLAY_BGM_INFO	m_tPlaySound;							///< 현재 Play 되고 있는 사운드
	sPLAY_BGM_INFO	m_tPostSound;							///< Fade out 후 삭제될 사운드

	CNtlBGMPlayList*	m_pBGMPlayList;
};