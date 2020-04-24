/*****************************************************************************
*
* File			: NtlWeatherGroup.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 5. 30	
* Abstract		: Sound weather channel group
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

#include <map>
#include <string>

// core
#include "ceventhandler.h"

// sound
#include "NtlChannelGroup.h"
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"

class CNtlWeatherGroup : public CNtlChannelGroup, public RWS::CEventHandler
{
public:
	CNtlWeatherGroup(eChannelGroupType eGroup);
	virtual ~CNtlWeatherGroup();

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);
	virtual void			StopGroup();

	virtual int				CanPlay(const char* pcName);

	SOUND_HANDLE			GetRecoverySoundHandle();

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:
	SOUND_HANDLE			m_hRecoverySoundHandle;
};