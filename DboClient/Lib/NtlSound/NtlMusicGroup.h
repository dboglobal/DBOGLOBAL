/*****************************************************************************
*
* File			: NtlMusicGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 11	
* Abstract		: Background Music Channel Group
*****************************************************************************
* Desc			: 다른 음악으로 전환될시 이전 음악(PostMusic)은
*				  Fade Out 이 되며 map의 처음에 위치하고 새로운 음악은
*				  map의 두번째에 위치한다
*
*				  일단 Fade Out이 시작된 PostMusic은 사라져야만 한다
*****************************************************************************/

#pragma once

#include <string>
#include "ceventhandler.h"
#include "NtlChannelGroup.h"


class CNtlMusicGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
public:
	CNtlMusicGroup(eChannelGroupType eGroup);
	virtual ~CNtlMusicGroup();

	virtual void			Update(float fElapsed);

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);

public:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
	virtual void	ReleaseFinishedSound(float fElapsed);

protected:
	float			m_fDelayReplayTime;						///< 반복되는 음악의 다음 재생까지 휴식 시간(단위 : 초)
	float			m_fReplayElapsed;						///< 현재 음악의 다음 재생까지 남은 시간

	SOUND_HANDLE	m_hPostSound;							///< Fade out 후 삭제될 사운드 핸들
};