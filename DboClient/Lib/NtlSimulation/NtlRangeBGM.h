/*****************************************************************************
* File			: NtlSoundRangeBGM.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 
* Update		: 2009. 7. 10
* Abstract		: Simulation sound range BGM
*****************************************************************************
* Desc         : 일정 반경 안에서 플레이 되는 BGM
*****************************************************************************/

#ifndef __NTL_RANGE_BGM_H__
#define __NTL_RANGE_BGM_H__

// sound
#include "NtlSoundDefines.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSoundNode.h"

class CNtlRangeBGM : public CNtlSoundNode
{
protected:
	struct sRANGE_BGM
	{
		SOUND_HANDLE		hHandle;
		std::string			strFileName;
		RwReal				fRadius;
		RwV3d				v3Position;
	};

	typedef SOUND_HANDLE	RANGE_BGM_KEY;
	typedef std::map<RANGE_BGM_KEY, sRANGE_BGM*>					MAP_RANGE_BGM;

public:
	CNtlRangeBGM();
	virtual ~CNtlRangeBGM();

	virtual RwBool			Create();
	virtual VOID			Update(RwReal fElapsed);
	virtual VOID			Destory();
	virtual VOID			HandleEvents(RWS::CMsg &pMsg);

protected:
	VOID					Update_PlayableRangeBGM(RwReal fElapsed);
	VOID					Update_PlayingRangeBGM(RwReal fElapsed);

	VOID					RangeBGM_Register(RANGE_BGM_KEY key, const RwChar* pcFileName, RwReal fRadius, RwV3d* pPosition);
	VOID					RangeBGM_Unregister(RANGE_BGM_KEY key);

	VOID					Released_RangeSound();
	sRANGE_BGM*				GetRangeBGM(RANGE_BGM_KEY key);

protected:
	MAP_RANGE_BGM			m_mapRangeBGM;
	SOUND_HANDLE			m_hPlayingSound;
	
	RwReal					m_fUpdateSkipTime;
};

#endif