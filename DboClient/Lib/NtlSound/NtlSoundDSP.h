/*****************************************************************************
*
* File			: NtlSoundDSP.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 4. 3
* Abstract		: FMOD의 DSP(Disital Signal Processor)를 관리한다
*****************************************************************************
* Desc: A class for applying effects to FMOD systems, channel groups, and channels.
* You can apply the same effect to duplicate objects
* Sound is not good and you can control each effect by giving it an argument.
* The same effect can not be applied in duplicate.
* However, the same effect on the system (master group), channel group, and channel
* It can be duplicated, but it is not recommended.
*
* Note) The range of parameter of each effect is different. Is currently
* Use fixed parameter values.
*****************************************************************************/

#pragma once

#include <map>

class CNtlSoundDSP
{
public:
	typedef std::map<RwUInt8, FMOD::DSP*>				MAP_DSP;
	typedef std::map<RwUInt8, FMOD::DSP*>::iterator		MAP_DSP_ITER;

	CNtlSoundDSP() {}
	~CNtlSoundDSP();

	VOID		Destroy();

	FMOD::DSP*	CreateDSP(FMOD_DSP_TYPE eType);
	VOID		ReleaseDSP(FMOD_DSP_TYPE eType);

	VOID		SetParameta(FMOD_DSP_TYPE eType, RwUInt8 byParametaType, RwReal fValue);
	RwReal		GetParameta(FMOD_DSP_TYPE eType, RwUInt8 byParametaType);

	RwBool		IsExistDSP(FMOD_DSP_TYPE eType);

protected:
	VOID		SetDetailParameta(FMOD_DSP_TYPE eType, FMOD::DSP* pDSP);

protected:
	MAP_DSP		m_mapDSP;
};