/*****************************************************************************
*
* File			: NtlJingleGroup.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 5. 30	
* Abstract		: Jingel(event) channel group
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

#include <map>
#include <string>

// sound
#include "fmod/fmod.h"
#include "fmod/fmod.hpp"
#include "NtlChannelGroup.h"


class CNtlJingleGroup : public CNtlChannelGroup
{
public:
	CNtlJingleGroup(eChannelGroupType eGroup);
	virtual ~CNtlJingleGroup();

	virtual eStoreResult	StoreSound(CNtlSound* pSound, sNtlSoundPlayParameta* pParameta);
	virtual bool			Stop(SOUND_HANDLE hHandle);
	virtual void			StopGroup();

protected:	
	virtual void			ReleaseFinishedSound(float fElapsed);

public:	

};