/*****************************************************************************
 *
 * File			: NtlFrameSkipShceduler.h
 * Author		: HyungSuk,Jang
 * Copyright	: (аж)NTL
 * Date			: 2009. 6. 09	
 * Abstract		: 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_FRAMESKIP_SCHEDULER_H_
#define __NTL_FRAMESKIP_SCHEDULER_H_

#define MAX_FRAME_SKIP_GROUP_NUM	30
#define INVAID_HFRAMESKIPID			-1

typedef RwInt32	HFRAMESKIPID;
typedef std::list<HFRAMESKIPID> ListFrameSkipHandle;

class CNtlFrameSkipGroup
{
private:

	RwInt32			m_iNum;
	RwInt32			m_iGroupId;
	HFRAMESKIPID	m_hStartId;
	RwChar			m_arrMark[MAX_FRAME_SKIP_GROUP_NUM];


public:

	CNtlFrameSkipGroup();
	~CNtlFrameSkipGroup();

	void	SetGroupId(RwInt32 iGroupId, HFRAMESKIPID hStartId);
	RwInt32	GetGroupId(void);

	void	AddHandle(HFRAMESKIPID h);
	void	RemoevHandle(HFRAMESKIPID h);
	RwInt32 GetCount(void);
};


class CNtlFrameSkipScheduler
{
private:

typedef std::map<RwInt32, CNtlFrameSkipGroup*, std::less<RwUInt32>>	MapFrameSkipGroup;
typedef std::map<RwInt32, RwInt32>	MapFrameUpdateGroup;

	RwInt32							m_iMaxId;
	RwInt32							m_iScheduleCount;
	ListFrameSkipHandle				m_hQueue;
	MapFrameSkipGroup				m_mapSkipGroup;
	MapFrameSkipGroup::iterator		m_itCurr;
	MapFrameUpdateGroup				m_mapUpdateGroup;				

private:

	RwInt32				MakeGroupId(HFRAMESKIPID h);
	RwInt32				GetStartFrameSkipId(RwInt32 iGroupId);
	CNtlFrameSkipGroup*	FindSkipGroup(HFRAMESKIPID h);
	void				RemoveGroup(CNtlFrameSkipGroup *pGroup);
	RwInt32				GetFpsScheduleCount(RwReal fFps);
	
public:

	CNtlFrameSkipScheduler();
	~CNtlFrameSkipScheduler();

	void			Update(RwReal fFps);
	HFRAMESKIPID	AddScheduler(void);
	void			RemoveFrameSkip(HFRAMESKIPID h);
	
	void			SetFpsScheduleCount(RwInt32 iCount);
	RwBool			IsFrameSkip(HFRAMESKIPID h);
};

#endif