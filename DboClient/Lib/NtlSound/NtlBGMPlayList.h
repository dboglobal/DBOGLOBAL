/*****************************************************************************
* File			: NtlBGMPlayList.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 7. 22
* Abstract		: 
*****************************************************************************
* Desc			: 조건에 따라 Play 될 BGM의 List를 보관 및 관리한다
*****************************************************************************/

#pragma once

struct sPLAY_BGM_PARAMETA
{
	eBGMType			eType;
	RwBool				bEnableParameta;		/// TRUE 일 때만 값이 유효하다
	RwBool				bLoop;
	RwReal				fReplayRestTime;
	std::string			strFileName;
};


class CNtlBGMPlayList
{
protected:
	typedef std::list<std::string>			LIST_SOUND_FILE;

	struct sBGM_INFO
	{
		SOUND_HANDLE		hHandle;
		RwBool				bLoop;
		RwBool				bClearLock;
		RwReal				fReplayRestTime;
		LIST_SOUND_FILE		listSoundFile;
	};

public:
	CNtlBGMPlayList();
	virtual ~CNtlBGMPlayList();

	RwBool			Create();
	VOID			Destroy();

	VOID			AddBGMList(eBGMType eType, const RwChar* pcFileName);
	VOID			ClearBGMList(eBGMType eType);
	VOID			ClearBGMAll();
	
	VOID			SetLock(eBGMType eType, RwBool bLock);
	VOID			SetLoop(eBGMType eType, bool bLoop);
	VOID			SetShareBGMSelectRate(RwReal fRate);

	VOID			SetRestTime(eBGMType eType, RwReal fRestTime);
	RwReal			GetRestTime(eBGMType eType);

	sPLAY_BGM_PARAMETA*		GetBGM_TopPriority();

	RwBool			IsExistMorePriorityBGM(eBGMType eType);

protected:
	RwChar*			GetShareBGM_if_Play();

protected:
	RwReal			m_fShareBGMSelectRate;

	sBGM_INFO		m_atBGM_INFO[NUM_BGM_TYPE];
};