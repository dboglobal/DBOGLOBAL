/*****************************************************************************
*
* File			: NtlSound.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 4. 3
* Abstract		: 
*****************************************************************************
* Desc			: Dbo에서 쓰이는 개별 사운드를 다룬다
*****************************************************************************/

#pragma once

#include "NtlSoundDefines.h"

class CNtlSoundDSP;

class CNtlSound
{
public:
	static void*	operator new(size_t size);
	static void		operator delete(void *pObj);
	static void		Shutdown();

	CNtlSound(SOUND_HANDLE hHandle, char* pcFileName);
	virtual ~CNtlSound();

	void			Release();
	void			FreeMemoryData();

	void			SetVolume_FadeInit();

	void			SetMinMax(float fMin, float fMax);

public:
	static RwFreeList*	m_pFreeList;

	SOUND_HANDLE		m_hHandle;
	int					m_iChannelGroup;
	std::string			m_strName;
	eSoundPlayState		m_eState;
	bool				m_bLoop;
	bool				m_bCreatedfromSoundPool;	///< Sound pool에서 FMOD::Sound를 가지고 왔다
	sNtlVolume			m_tVolume;
	sNtlMinMax			m_tMinMax;
	float				m_fXPos;
	float				m_fYPos;
	float				m_fZPos;
	float				m_fMinDistance;
	float				m_fMaxDistance;
	FMOD::Sound*		m_pFMODSound;				///< Information about sound sources
	FMOD::Channel*		m_pFMODChannel;				///< It has the current Play information of the sound source.
	CNtlSoundDSP*		m_pDSP;						///< FMOD effect container
	char*				pMemoryData;				///< Pointer to sound file loaded into memory
};