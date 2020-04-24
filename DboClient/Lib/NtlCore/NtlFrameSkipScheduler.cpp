#include "precomp_ntlcore.h"
#include "NtlFrameSkipScheduler.h"
#include "NtlDebug.h"


CNtlFrameSkipGroup::CNtlFrameSkipGroup()
{
	m_iNum		= 0;
	m_iGroupId	= 0;
	m_hStartId	= 0;
	memset(m_arrMark, 0, sizeof(RwChar) * MAX_FRAME_SKIP_GROUP_NUM);
}

CNtlFrameSkipGroup::~CNtlFrameSkipGroup()
{
}

void CNtlFrameSkipGroup::SetGroupId(RwInt32 iGroupId, HFRAMESKIPID hStartId)
{
	m_iGroupId = iGroupId;
	m_hStartId = hStartId;
}

RwInt32	CNtlFrameSkipGroup::GetGroupId(void)
{
	return m_iGroupId;
}


void CNtlFrameSkipGroup::AddHandle(HFRAMESKIPID h)
{
	HFRAMESKIPID hConvert = h - m_hStartId;

	NTL_ASSERT(hConvert >= 0 && hConvert < MAX_FRAME_SKIP_GROUP_NUM, "ListFrameSkipHandle::AddHandle");

	m_arrMark[hConvert] = 1;
	m_iNum++;
}

void CNtlFrameSkipGroup::RemoevHandle(HFRAMESKIPID h)
{
	HFRAMESKIPID hConvert = h - m_hStartId;

	NTL_ASSERT(hConvert >= 0 && hConvert < MAX_FRAME_SKIP_GROUP_NUM, "ListFrameSkipHandle::RemoevHandle");

	m_arrMark[hConvert] = 0;
	m_iNum--;
}

RwInt32 CNtlFrameSkipGroup::GetCount(void)
{
	return m_iNum;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



CNtlFrameSkipScheduler::CNtlFrameSkipScheduler()
{
	m_iMaxId			= 2000;
	m_iScheduleCount	= 200;

	for(RwInt32 i = 0; i < m_iMaxId; i++)
	{
		m_hQueue.push_back(i);
	}

	m_itCurr = m_mapSkipGroup.begin();
}

CNtlFrameSkipScheduler::~CNtlFrameSkipScheduler()
{
	MapFrameSkipGroup::iterator it;

	for(it = m_mapSkipGroup.begin(); it != m_mapSkipGroup.end(); it++)
	{
		NTL_DELETE((*it).second);
	}
}

RwInt32 CNtlFrameSkipScheduler::MakeGroupId(HFRAMESKIPID h)
{
	return h/MAX_FRAME_SKIP_GROUP_NUM;
}

RwInt32	CNtlFrameSkipScheduler::GetStartFrameSkipId(RwInt32 iGroupId)
{
	return iGroupId * MAX_FRAME_SKIP_GROUP_NUM;
}

CNtlFrameSkipGroup* CNtlFrameSkipScheduler::FindSkipGroup(HFRAMESKIPID h)
{
	RwInt32 iGroupId = MakeGroupId(h);

	MapFrameSkipGroup::iterator it = m_mapSkipGroup.find(iGroupId);
	if(it != m_mapSkipGroup.end())
		return (*it).second;

	return NULL;
}

void CNtlFrameSkipScheduler::RemoveGroup(CNtlFrameSkipGroup *pGroup)
{
	RwInt32 iGroupId = pGroup->GetGroupId();
	MapFrameSkipGroup::iterator it = m_mapSkipGroup.find(iGroupId);
	
	if(it == m_mapSkipGroup.end())
		return;

	
	NTL_DELETE(pGroup);
	if(it == m_itCurr)
	{
		m_itCurr = m_mapSkipGroup.erase(it);
	}
	else
	{
		m_mapSkipGroup.erase(it);
	}
}

RwInt32 CNtlFrameSkipScheduler::GetFpsScheduleCount(RwReal fFps)
{
	RwReal fRatio = 1.0f;
	
	if(fFps < 5.0f)
		fRatio = 0.3f;
	else if(fFps < 10.0f)
		fRatio = 0.6f;
	else if(fFps < 20.0f)
		fRatio = 0.9f;

	return (RwInt32) ( (RwReal)m_iScheduleCount * fRatio );
}

void CNtlFrameSkipScheduler::Update(RwReal fFps)
{
	m_mapUpdateGroup.clear();

	if(m_itCurr == m_mapSkipGroup.end())
		m_itCurr = m_mapSkipGroup.begin();

	if(m_itCurr == m_mapSkipGroup.end())
		return;

	RwInt32 iNum = 0;
	RwInt32 iGroupId;
	MapFrameSkipGroup::iterator it;

	RwInt32 iScheduleCount = GetFpsScheduleCount(fFps);

	for(it = m_itCurr; it != m_mapSkipGroup.end(); it++)
	{
		iNum += (*it).second->GetCount();
		
		iGroupId = (*it).second->GetGroupId();
		m_mapUpdateGroup[iGroupId] = iGroupId;
	
		if(iNum > iScheduleCount)
		{
			m_itCurr = it++;
			if(m_itCurr == m_mapSkipGroup.end())
				m_itCurr = m_mapSkipGroup.begin();

			return;
		}
	}

	for(it = m_mapSkipGroup.begin(); it != m_itCurr; it++)
	{
		iNum += (*it).second->GetCount();

		iGroupId = (*it).second->GetGroupId();
		m_mapUpdateGroup[iGroupId] = iGroupId;

		if(iNum > iScheduleCount)
		{
			m_itCurr = it++;
			if(m_itCurr == m_mapSkipGroup.end())
				m_itCurr = m_mapSkipGroup.begin();

			return;
		}
	}
}

HFRAMESKIPID CNtlFrameSkipScheduler::AddScheduler(void)
{
	ListFrameSkipHandle::iterator it = m_hQueue.begin();
	HFRAMESKIPID h = (*it);

	m_hQueue.pop_front();

	CNtlFrameSkipGroup *pGroup = FindSkipGroup(h);
	if(pGroup == NULL)
	{
		pGroup = NTL_NEW CNtlFrameSkipGroup;
		RwInt32 iGroupId = MakeGroupId(h);
		pGroup->SetGroupId( iGroupId, GetStartFrameSkipId(iGroupId) );

		m_mapSkipGroup[iGroupId] = pGroup;
	}

	pGroup->AddHandle(h);

	return h;
}

void CNtlFrameSkipScheduler::RemoveFrameSkip(HFRAMESKIPID h)
{
	m_hQueue.push_front(h);

	CNtlFrameSkipGroup *pGroup = FindSkipGroup(h);

	NTL_ASSERT(pGroup, "CNtlFrameSkipScheduler::RemoveFrameSkip");

	if(pGroup)
	{
		pGroup->RemoevHandle(h);

		if(pGroup->GetCount() == 0)
		{
			RemoveGroup(pGroup);
		}
	}
}

void CNtlFrameSkipScheduler::SetFpsScheduleCount(RwInt32 iCount)
{
	m_iScheduleCount = iCount;
}

RwBool CNtlFrameSkipScheduler::IsFrameSkip(HFRAMESKIPID h)
{
	RwInt32 iGroupId = MakeGroupId(h);
	MapFrameUpdateGroup::iterator it = m_mapUpdateGroup.find(iGroupId);

	if(it == m_mapUpdateGroup.end())
		return TRUE;

	return FALSE;
}