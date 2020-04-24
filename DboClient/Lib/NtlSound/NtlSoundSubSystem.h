/*****************************************************************************
* File			: NtlSoundManaager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 16	
* Abstract		: 
*****************************************************************************
* Desc			: 사운드 서브 시스템 컨테이너
*****************************************************************************/

#pragma once

class CNtlSoundEventHandler;


class CNtlSoundSubSystem
{
public:
	CNtlSoundSubSystem();
	virtual ~CNtlSoundSubSystem();

	RwBool	Create();
	void	Destroy();

protected:
	CNtlSoundEventHandler*	m_pSoundEventHandler;
};