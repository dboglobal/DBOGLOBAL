#include "precomp_ntlsound.h"
#include "NtlSoundGlobal.h"


#include "NtlSoundDefines.h"
#include "GUISoundDefine.h"



CNtlSoundGlobal*		CNtlSoundGlobal::m_pInstance			= NULL;
FMOD::System*			CNtlSoundGlobal::m_pFMODSystem			= NULL;

std::string				CNtlSoundGlobal::m_strFilePath;
float					CNtlSoundGlobal::m_fMinMaxRate			= 1.f;
sNtlVolume				CNtlSoundGlobal::m_tMasterVolume;

SET_ERRORMSG			CNtlSoundGlobal::m_setMsgError;
SET_SOUNDHANDLE			CNtlSoundGlobal::m_setPlayingSoundHandle;