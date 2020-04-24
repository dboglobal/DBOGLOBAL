/*****************************************************************************
*
* File			: NtlFadeInOut.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 7. 12	
* Abstract		: Fade In/Out Effect
*****************************************************************************
* Desc			: Channel, ChannelGroup에 Effect를 적용할 수 있다
*
*				  FMOD System에서 Fade In/Out Effect를 제공하지 않아서
*				  따로 구현하였다.
*****************************************************************************/

#pragma once

#include <list>
#include "ceventhandler.h"
#include "fmod/fmod.hpp"

class CNtlSound;
class CNtlChannelGroup;

#define dFADE_STOP_FLAG_NOT_NOTIFY						0x01

struct sFadeInOut
{
	eFadeInOutType		eFadeType;					///< Fade In/Out Type
	eSoundResourceType	eResourceType;				///< 사운드 종류
	bool				bFinish;	
	float				fDestVolume;				///< 최종 변환된 볼륨
	float				fAdjustVolume;				///< 매번 변화되는 볼륨양
	unsigned long		ulApplyTime;				///< 이전에 Fade In/Out 효과가 적용된 시간

	union
	{
		struct
		{
			SOUND_HANDLE	u1_hHandle;
			CNtlSound*		u1_pSound;
		};

		struct 
		{
			CNtlChannelGroup*	u2_pChannelGroup;
		};		
	};
};


class CNtlFadeInOut : public RWS::CEventHandler
{
public:
	typedef std::list<sFadeInOut>::iterator	FADE_ITER;

public:
	static bool		CreateInstance();
	static void		DestroyInstance();
	static CNtlFadeInOut* GetInstance() { return m_pInstance; }


	CNtlFadeInOut();
	virtual ~CNtlFadeInOut();

	void		Update();

	void		FadeIn(CNtlSound* pSound, float fDestVolume, unsigned long ulTime);
	void		FadeIn(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime);

	void		FadeOut(CNtlSound* pSound, float fDestVolume, unsigned long ulTime);
	void		FadeOut(CNtlChannelGroup* pChannelGroup, float fDestVolume, unsigned long ulTime);

	void		StopImmdiately(FMOD::Channel* m_pFMODChannel, RwUInt8 byFlag = 0);
	void		StopImmdiately(CNtlChannelGroup* pChannelGroup, RwUInt8 byFlag = 0);

	bool		IsFadeIn(FMOD::Channel* pChannel);
	bool		IsFadeOut(FMOD::Channel* pChannel);

	bool		IsFadeIn(CNtlChannelGroup* pChannelGroup);
	bool		IsFadeOut(CNtlChannelGroup* pChannelGroup);

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

public:
	static CNtlFadeInOut* m_pInstance;

	std::list<sFadeInOut>		m_listFade;
};

static CNtlFadeInOut* GetFadeInOut()
{
	return CNtlFadeInOut::GetInstance();
}