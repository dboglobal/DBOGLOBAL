#include "precomp_ntlsound.h"
#include "NtlBGMPlayList.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"


#define dSHARE_THEME_PLAY_RATE				(0.3f)


CNtlBGMPlayList::CNtlBGMPlayList()
:m_fShareBGMSelectRate(dSHARE_THEME_PLAY_RATE)
{
	RwUInt8 byCount = 0;

	m_atBGM_INFO[BGM_TYPE_SHARE_THEME].hHandle			= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_SHARE_THEME].bLoop			= FALSE;
	m_atBGM_INFO[BGM_TYPE_SHARE_THEME].bClearLock		= FALSE;
	m_atBGM_INFO[BGM_TYPE_SHARE_THEME].fReplayRestTime	= 120.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_LOCAL_THEME].hHandle			= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_LOCAL_THEME].bLoop			= FALSE;
	m_atBGM_INFO[BGM_TYPE_LOCAL_THEME].bClearLock		= FALSE;
	m_atBGM_INFO[BGM_TYPE_LOCAL_THEME].fReplayRestTime	= 120.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_MAIN_THEME].hHandle			= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_MAIN_THEME].bLoop				= FALSE;
	m_atBGM_INFO[BGM_TYPE_MAIN_THEME].bClearLock		= FALSE;
	m_atBGM_INFO[BGM_TYPE_MAIN_THEME].fReplayRestTime	= 20.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_RANGE].hHandle				= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_RANGE].bLoop					= FALSE;
	m_atBGM_INFO[BGM_TYPE_RANGE].bClearLock				= FALSE;
	m_atBGM_INFO[BGM_TYPE_RANGE].fReplayRestTime		= 120.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_PVP].hHandle					= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_PVP].bLoop					= FALSE;
	m_atBGM_INFO[BGM_TYPE_PVP].bClearLock				= FALSE;
	m_atBGM_INFO[BGM_TYPE_PVP].fReplayRestTime			= 10.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_RANKBATTLE_BATTLE].hHandle	= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_RANKBATTLE_BATTLE].bLoop		= TRUE;
	m_atBGM_INFO[BGM_TYPE_RANKBATTLE_BATTLE].bClearLock	= FALSE;
	m_atBGM_INFO[BGM_TYPE_RANKBATTLE_BATTLE].fReplayRestTime	= 10.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_THEME_BGM].hHandle			= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_THEME_BGM].bLoop				= FALSE;
	m_atBGM_INFO[BGM_TYPE_THEME_BGM].bClearLock			= FALSE;
	m_atBGM_INFO[BGM_TYPE_THEME_BGM].fReplayRestTime	= 10.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_SERVER_PLAYED].hHandle		= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_SERVER_PLAYED].bLoop			= FALSE;
	m_atBGM_INFO[BGM_TYPE_SERVER_PLAYED].bClearLock		= FALSE;
	m_atBGM_INFO[BGM_TYPE_SERVER_PLAYED].fReplayRestTime= 5.f;
	++byCount;

	m_atBGM_INFO[BGM_TYPE_CINEMATIC].hHandle			= INVALID_SOUND_HANDLE;
	m_atBGM_INFO[BGM_TYPE_CINEMATIC].bLoop				= FALSE;
	m_atBGM_INFO[BGM_TYPE_CINEMATIC].bClearLock			= FALSE;
	m_atBGM_INFO[BGM_TYPE_CINEMATIC].fReplayRestTime	= 5.f;
	++byCount;

	DBO_ASSERT(NUM_BGM_TYPE == byCount, "Not defined loop of some BGM type");
}

CNtlBGMPlayList::~CNtlBGMPlayList()
{
	for( RwUInt8 i = 0 ; i < NUM_BGM_TYPE ; ++i )
		m_atBGM_INFO[i].listSoundFile.clear();
}

RwBool CNtlBGMPlayList::Create()
{
	NTL_FUNCTION("CNtlBGMPlayList::Create");

	NTL_RETURN(TRUE);
}

void CNtlBGMPlayList::Destroy()
{
	NTL_FUNCTION("CNtlBGMPlayList::Destroy");

	NTL_RETURNVOID();
}

VOID CNtlBGMPlayList::AddBGMList(eBGMType eType, const RwChar* pcFileName)
{
	if( !pcFileName )
		return;

	if( strlen(pcFileName) == 0 )
		return;

	if( NUM_BGM_TYPE <= eType )
		return;

	if( BGM_TYPE_SHARE_THEME == eType )
	{
		m_atBGM_INFO[BGM_TYPE_SHARE_THEME].listSoundFile.push_back(pcFileName);
	}
	else
	{
		m_atBGM_INFO[eType].listSoundFile.clear();
		m_atBGM_INFO[eType].listSoundFile.push_back(pcFileName);
	}
}

VOID CNtlBGMPlayList::ClearBGMList(eBGMType eType)
{
	if( NUM_BGM_TYPE <= eType )
		return;

	if( m_atBGM_INFO[eType].bClearLock )
		return;

	m_atBGM_INFO[eType].hHandle = INVALID_SOUND_HANDLE;
	m_atBGM_INFO[eType].listSoundFile.clear();
}

VOID CNtlBGMPlayList::ClearBGMAll()
{
	for( RwUInt8 i = 0 ; i < NUM_BGM_TYPE ; ++i )
	{
		if( m_atBGM_INFO[i].bClearLock )
			return;

		m_atBGM_INFO[i].hHandle = INVALID_SOUND_HANDLE;
		m_atBGM_INFO[i].listSoundFile.clear();
	}
}

VOID CNtlBGMPlayList::SetLock(eBGMType eType, RwBool bLock)
{
	if( NUM_BGM_TYPE <= eType )
		return;

	m_atBGM_INFO[eType].bClearLock = bLock;
}

VOID CNtlBGMPlayList::SetLoop(eBGMType eType, bool bLoop)
{
	if( NUM_BGM_TYPE <= eType )
		return;

	m_atBGM_INFO[eType].bLoop = bLoop;
}

VOID CNtlBGMPlayList::SetShareBGMSelectRate(RwReal fRate)
{
	if( fRate < 0.f )
		fRate = 0.f;

	if( fRate > 1.f )
		fRate = 1.f;

	m_fShareBGMSelectRate = fRate;
}

VOID CNtlBGMPlayList::SetRestTime(eBGMType eType, RwReal fRestTime)
{
	if( NUM_BGM_TYPE <= eType )
		return;

	m_atBGM_INFO[eType].fReplayRestTime = fRestTime;
}

RwReal CNtlBGMPlayList::GetRestTime(eBGMType eType)
{
	if( NUM_BGM_TYPE <= eType )
		return 0.f;

	return m_atBGM_INFO[eType].fReplayRestTime;
}

sPLAY_BGM_PARAMETA* CNtlBGMPlayList::GetBGM_TopPriority()
{
	static sPLAY_BGM_PARAMETA tPLAY_BGM_PARAMETA;
	tPLAY_BGM_PARAMETA.bEnableParameta = FALSE;


	// 우선순위가 높은 BGM이 있으면 그것을 반환한다
	// Share BGM은 Local BGM일 때 같이 검사한다
	for( RwUInt8 i = NUM_BGM_TYPE - 1 ; i > 0 ; --i )
	{
		if( BGM_TYPE_LOCAL_THEME == i )
		{
			RwChar* pcShareBGMFileName = GetShareBGM_if_Play();

			if( pcShareBGMFileName )
			{
				tPLAY_BGM_PARAMETA.eType				= BGM_TYPE_SHARE_THEME;
				tPLAY_BGM_PARAMETA.bEnableParameta		= TRUE;
				tPLAY_BGM_PARAMETA.bLoop				= m_atBGM_INFO[BGM_TYPE_SHARE_THEME].bLoop;
				tPLAY_BGM_PARAMETA.fReplayRestTime		= m_atBGM_INFO[BGM_TYPE_SHARE_THEME].fReplayRestTime;
				tPLAY_BGM_PARAMETA.strFileName			= pcShareBGMFileName;
			}
			else if( false == m_atBGM_INFO[BGM_TYPE_LOCAL_THEME].listSoundFile.empty() )
			{
				tPLAY_BGM_PARAMETA.eType				= (eBGMType)i;
				tPLAY_BGM_PARAMETA.bEnableParameta		= TRUE;
				tPLAY_BGM_PARAMETA.bLoop				= m_atBGM_INFO[i].bLoop;
				tPLAY_BGM_PARAMETA.fReplayRestTime		= m_atBGM_INFO[i].fReplayRestTime;
				tPLAY_BGM_PARAMETA.strFileName			= *(m_atBGM_INFO[i].listSoundFile.begin());
			}

			// Share BGM은 Local BGM일 때 같이 검사한다
			break;
		}
		else if( false == m_atBGM_INFO[i].listSoundFile.empty() )
		{
			tPLAY_BGM_PARAMETA.eType				= (eBGMType)i;
			tPLAY_BGM_PARAMETA.bEnableParameta		= TRUE;
			tPLAY_BGM_PARAMETA.bLoop				= m_atBGM_INFO[i].bLoop;
			tPLAY_BGM_PARAMETA.fReplayRestTime		= m_atBGM_INFO[i].fReplayRestTime;
			tPLAY_BGM_PARAMETA.strFileName			= *(m_atBGM_INFO[i].listSoundFile.begin());
			break;
		}
	}

	return &tPLAY_BGM_PARAMETA;
}

RwChar* CNtlBGMPlayList::GetShareBGM_if_Play()
{
	RwReal fRate = NtlRandomNumber(0.f, 1.0f);
	if( fRate >= m_fShareBGMSelectRate )
		return NULL;

	if( m_atBGM_INFO[BGM_TYPE_SHARE_THEME].listSoundFile.empty() )
		return NULL;


	RwInt32 iMaxIndex = (RwInt32)m_atBGM_INFO[BGM_TYPE_SHARE_THEME].listSoundFile.size() - 1;
	RwInt32 iIndex = NtlRandomNumber(0, iMaxIndex);

	LIST_SOUND_FILE::iterator it_SoundFile = m_atBGM_INFO[BGM_TYPE_SHARE_THEME].listSoundFile.begin();
	for( RwInt32 i = 0 ; i < iIndex  ; ++i )
	{
		++it_SoundFile;
	}

	if( it_SoundFile == m_atBGM_INFO[BGM_TYPE_SHARE_THEME].listSoundFile.end() )
		return NULL;

	return (RwChar*)(it_SoundFile->c_str());
}

RwBool CNtlBGMPlayList::IsExistMorePriorityBGM(eBGMType eType)
{
	for( RwUInt8 i = 0 ; i < NUM_BGM_TYPE ; ++i )
	{
		if( eType >= (eBGMType)i )
			continue;

		if( false == m_atBGM_INFO[i].listSoundFile.empty() )
			return TRUE;
	}

	return FALSE;
}