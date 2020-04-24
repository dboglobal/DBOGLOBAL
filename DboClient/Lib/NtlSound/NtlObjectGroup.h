/*****************************************************************************
*
* File			: NtlObjectGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 13
* Abstract		: Object music channel group
*****************************************************************************
* Desc			: 오브젝트 채널 그룹이 다른 채널 그룹의 우선 순위에 밀렸을 때
*				  임의의 오브젝트 채널 하나를 강제종료 시키고 그 이름을 저장한다
*				  차후 빈 채널이 생기면 강제종료된 순서대로 사운드를 플레이한다
*
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlChannelGroup.h"

class CNtlObjectGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
public:
	CNtlObjectGroup(eChannelGroupType eGroup);
	virtual ~CNtlObjectGroup();

	virtual void			Destory();

	virtual bool			Stop(SOUND_HANDLE hHandle);
	virtual CNtlSound*		GetSound(SOUND_HANDLE hHandle);

	virtual bool			ReleaseLowRankChannel();
	virtual bool			IsExistSound(SOUND_HANDLE handle);

public:
	virtual void			PostUpdate(float fXPos, float fYPos, float fZPos);

	virtual void			StoreReleasedSound(CNtlSound* pSound);
	virtual void			StoreSleepingSound(CNtlSound* pSound);

	virtual void			DelSleepingSound(SOUND_HANDLE hHandle);
	virtual void			DelReleasedSound(SOUND_HANDLE hHandle);	

	virtual CNtlSound*		GetReleasedSoundbyPriority();	///< 가장 오래전에 강제 종료된 사운드의 이름을 반환한다.
	virtual void			SuccessRelay();					///< 종료됬던 사운드를 다시 플레이하는데 성공했을 때 호출해야 한다.

protected:
	virtual VOID			HandleEvents( RWS::CMsg &msg );


public:
	SOUND_MAP			m_mapReleasedSound;			///< 상위 그룹에 의한 강제종료됬던 사운드 이름
	///< 다른 채널이 release되면 차례대로 플레이한다.

	SOUND_MAP			m_mapSleepingSound;			///< 일정 반경밖의 사운드는 플레이되지 않고 리스트로 관리한다
};