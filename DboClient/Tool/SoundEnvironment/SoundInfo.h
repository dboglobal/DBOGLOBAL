/******************************************************************************
* File			: CSoundInfo.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 12. 18
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: 플랫폼의 사운드 환경 정보를 표시한다
*****************************************************************************/

#pragma once

class CSoundInfo
{
public:
	CSoundInfo();
	virtual ~CSoundInfo();

	bool			Create();
	void			Destory();

	void			ReportSoundEnvironment();	

protected:
	void			DisplaySoundEnvironment(FILE* fp, FMOD::System* pFMODSystem);

	bool			GetSpeakerModeName(char* pcResultText, int ibufferSize, FMOD_SPEAKERMODE mode);
	bool			IsExistError(FILE* fp, FMOD_RESULT result);
};