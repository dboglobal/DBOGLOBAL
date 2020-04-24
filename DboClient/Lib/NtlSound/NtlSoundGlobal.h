#pragma once

#include <set>


typedef std::set<std::string>					SET_ERRORMSG;
typedef std::set<SOUND_HANDLE>					SET_SOUNDHANDLE;


class CNtlSoundGlobal
{
public:
	static CNtlSoundGlobal*			m_pInstance;

	static FMOD::System*			m_pFMODSystem;				///< FMOD::System 인스턴스
		
	static std::string				m_strFilePath;
	static float					m_fMinMaxRate;
	static sNtlVolume				m_tMasterVolume;

	static SET_ERRORMSG				m_setMsgError;				// 같은 에러 메세지를 걸러내기 위한 전역 변수
	static SET_SOUNDHANDLE			m_setPlayingSoundHandle;
};