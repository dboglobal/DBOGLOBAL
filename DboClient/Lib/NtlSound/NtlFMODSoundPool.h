/*****************************************************************************
* File			: NtlFMODSoundPool.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 16
* Abstract		: 
*****************************************************************************
* Desc			: FMOD::Sound 
*****************************************************************************/

#pragma once


class CNtlFMODSoundPool
{
public:
	struct sPOOL
	{
		FMOD::Sound*	pFMODSound;
		RwInt32			iUseCount;
	};

	typedef std::map<std::string, sPOOL*>			MAP_FMODSOUND;
	typedef std::map<std::string, FMOD::Sound*>		MAP_RESIDENT;

public:
	static RwBool	CreateInstance();
	static VOID		DestroyInstance();
	static CNtlFMODSoundPool* GetInstance() { return m_pInstance; }

	CNtlFMODSoundPool();
	virtual ~CNtlFMODSoundPool();

	FMOD::Sound*		GetFMODSound(const char* pcFileName);

protected:
	VOID				LoadResidentPool();

protected:
	static CNtlFMODSoundPool* m_pInstance;

	MAP_FMODSOUND		m_mapFMODSoundPool;
	MAP_RESIDENT		m_mapResidentPool;		///< 자주 사용하는 FMOD::Sound는 메모리에 상주시킨다

	RwUInt32			m_uiFileSize_FMODSound;
	RwUInt32			m_uiFileSize_ResidentPool;
};

static CNtlFMODSoundPool* GetSoundPool()
{
	return CNtlFMODSoundPool::GetInstance();
}