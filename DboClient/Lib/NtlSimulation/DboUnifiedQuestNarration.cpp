#include "precomp_ntlsimulation.h"
#include "DboUnifiedQuestNarration.h"
#include "NtlSLEventFunc.h"
#include "NtlDebug.h"


CDboUnifiedQuestNarration::CDboUnifiedQuestNarration( void )
{
	m_arStateFunc[eUQN_STATE_IDLE][eUQN_STATE_IDLE]			= NULL;
	m_arStateFunc[eUQN_STATE_IDLE][eUQN_STATE_BEGIN]		= &CDboUnifiedQuestNarration::OnIdle2Begin;
	m_arStateFunc[eUQN_STATE_IDLE][eUQN_STATE_PROGRESS]		= NULL;
	m_arStateFunc[eUQN_STATE_IDLE][eUQN_STATE_END]			= NULL;
	m_arStateFunc[eUQN_STATE_IDLE][eUQN_STATE_SINGLE]		= &CDboUnifiedQuestNarration::OnIdle2Single;

	m_arStateFunc[eUQN_STATE_BEGIN][eUQN_STATE_IDLE]		= NULL;
	m_arStateFunc[eUQN_STATE_BEGIN][eUQN_STATE_BEGIN]		= NULL;
	m_arStateFunc[eUQN_STATE_BEGIN][eUQN_STATE_PROGRESS]	= &CDboUnifiedQuestNarration::OnBegin2Progress;
	m_arStateFunc[eUQN_STATE_BEGIN][eUQN_STATE_END]			= &CDboUnifiedQuestNarration::OnBegin2End;
	m_arStateFunc[eUQN_STATE_BEGIN][eUQN_STATE_SINGLE]		= NULL;

	m_arStateFunc[eUQN_STATE_PROGRESS][eUQN_STATE_IDLE]		= NULL;
	m_arStateFunc[eUQN_STATE_PROGRESS][eUQN_STATE_BEGIN]	= NULL;
	m_arStateFunc[eUQN_STATE_PROGRESS][eUQN_STATE_PROGRESS]	= &CDboUnifiedQuestNarration::OnProgress2Process;
	m_arStateFunc[eUQN_STATE_PROGRESS][eUQN_STATE_END]		= &CDboUnifiedQuestNarration::OnProgress2End;
	m_arStateFunc[eUQN_STATE_PROGRESS][eUQN_STATE_SINGLE]	= NULL;

	m_arStateFunc[eUQN_STATE_END][eUQN_STATE_IDLE]			= &CDboUnifiedQuestNarration::OnEnd2Idle;
	m_arStateFunc[eUQN_STATE_END][eUQN_STATE_BEGIN]			= NULL;
	m_arStateFunc[eUQN_STATE_END][eUQN_STATE_PROGRESS]		= NULL;
	m_arStateFunc[eUQN_STATE_END][eUQN_STATE_END]			= NULL;
	m_arStateFunc[eUQN_STATE_END][eUQN_STATE_SINGLE]		= NULL;

	m_arStateFunc[eUQN_STATE_SINGLE][eUQN_STATE_IDLE]		= &CDboUnifiedQuestNarration::OnSingle2Idle;
	m_arStateFunc[eUQN_STATE_SINGLE][eUQN_STATE_BEGIN]		= NULL;
	m_arStateFunc[eUQN_STATE_SINGLE][eUQN_STATE_PROGRESS]	= NULL;
	m_arStateFunc[eUQN_STATE_SINGLE][eUQN_STATE_END]		= NULL;
	m_arStateFunc[eUQN_STATE_SINGLE][eUQN_STATE_SINGLE]		= NULL;

	m_eOldState = eUQN_STATE_IDLE;
	m_eCurState = eUQN_STATE_IDLE;

	m_uiTableIdx = 0xffffffff;
	m_uiRemainTime = 0xffffffff;
}


CDboUnifiedQuestNarration::~CDboUnifiedQuestNarration( void )
{
}


bool CDboUnifiedQuestNarration::IsUsing( void )
{
	return (m_eCurState == eUQN_STATE_IDLE ? false : true);
}


bool CDboUnifiedQuestNarration::Build( sTS_KEY& sKey, sQUEST_NARRATION_TBLDAT* pData )
{
	if ( NULL == pData )
	{
		return false;
	}

	if ( 0 == pData->byNumber )
	{
		return false;
	}

	m_defQuestNarrationList.clear();

	m_sTSKey = sKey;

	m_uiTableIdx = pData->tblidx;

	m_uiRemainTime = (pData->byTime == 255 ? 0xffffffff : pData->byTime);

	sQUEST_NARRATION_DATASET sQNData;
	for ( int i = 0; i < pData->byNumber; ++i )
	{
		memcpy( &sQNData, &pData->asData[i], sizeof( sQUEST_NARRATION_DATASET ) );

		m_defQuestNarrationList.push_back( sQNData );
	}

	if ( pData->byNumber == 1 )
	{
		ChangeState( eUQN_STATE_SINGLE );
	}
	else
	{
		ChangeState( eUQN_STATE_BEGIN );
	}

	return true;
}


CDboUnifiedQuestNarration::eUQN_STATE CDboUnifiedQuestNarration::NextState( sTS_KEY& sKey )
{
	if ( eUQN_STATE_SINGLE == m_eCurState )
	{
		ChangeState( eUQN_STATE_IDLE );
	}
	else if ( eUQN_STATE_BEGIN == m_eCurState )
	{
		if ( m_defQuestNarrationList.size() == 1 )
		{
			ChangeState( eUQN_STATE_END );
		}
		else
		{
			ChangeState( eUQN_STATE_PROGRESS );
		}
	}
	else if ( eUQN_STATE_PROGRESS == m_eCurState )
	{
		if ( m_defQuestNarrationList.size() == 1 )
		{
			ChangeState( eUQN_STATE_END );
		}
		else
		{
			ChangeState( eUQN_STATE_PROGRESS );
		}
	}
	else if ( eUQN_STATE_END == m_eCurState )
	{
		ChangeState( eUQN_STATE_IDLE );
	}
	else
	{
		NTL_ASSERTFAIL( "Not defined state of unified quest narration" );
	}

	return m_eCurState;
}


void CDboUnifiedQuestNarration::Update( void )
{
	if ( m_defNarrationEntityQueue.empty() ) return;

	vecdef_NarrationEntityList::iterator it = m_defNarrationEntityQueue.begin();

	sNARRATION_ENTITY& sEntity = *it;

	CNtlSLEventGenerator::QuestNarrationDialog_Req( m_sTSKey,
													sEntity.eNarrationProgType,
													(eNARRATION_OWNER_TYPE)sEntity.sData.byOwnerType,
													sEntity.sData.dwOwner,
													(eNARRATION_OWNER_STATE)sEntity.sData.byCondition,
													(eNARRATION_DIALOG_DIR_TYPE)sEntity.sData.byDirection,
													sEntity.sData.dwDialog,
													(eNARRATION_GUI_TYPE)sEntity.sData.byUiType,
													m_uiRemainTime * 1000 );

	m_defNarrationEntityQueue.erase( it );

}


void CDboUnifiedQuestNarration::ChangeState( eUQN_STATE eState )
{
	m_eOldState = m_eCurState;
	m_eCurState = eState;

	if ( m_arStateFunc[m_eOldState][m_eCurState] )
	{
		(this->*m_arStateFunc[m_eOldState][m_eCurState])();
	}
	else
	{
		NTL_ASSERTFAIL( "Not defined state of unified quest narration" );
	}
}


void CDboUnifiedQuestNarration::OnIdle2Single( void )
{
	NTL_ASSERT( (1 == m_defQuestNarrationList.size()), "Quest narration counter must be one" );

	vecdef_QuestNarrationList::iterator it = m_defQuestNarrationList.begin();

	sQUEST_NARRATION_DATASET* pData = &(*it);

	sNARRATION_ENTITY sData;

	sData.eNarrationProgType = eNARRATION_PROGRESS_STATE_SINGLE;
	memcpy( &sData.sData, pData, sizeof(sQUEST_NARRATION_DATASET) );

	m_defNarrationEntityQueue.push_back( sData );

	m_defQuestNarrationList.erase( it );

	ChangeState( eUQN_STATE_IDLE );
}


void CDboUnifiedQuestNarration::OnIdle2Begin( void )
{
	NTL_ASSERT( (1 < m_defQuestNarrationList.size()), "Quest narration counter must be more than one" );

	vecdef_QuestNarrationList::iterator it = m_defQuestNarrationList.begin();

	sQUEST_NARRATION_DATASET* pData = &(*it);

	sNARRATION_ENTITY sData;

	sData.eNarrationProgType = eNARRATION_PROGRESS_STATE_START;
	memcpy( &sData.sData, pData, sizeof(sQUEST_NARRATION_DATASET) );

	m_defNarrationEntityQueue.push_back( sData );

	m_defQuestNarrationList.erase( it );
}


void CDboUnifiedQuestNarration::OnBegin2Progress( void )
{
	NTL_ASSERT( (1 < m_defQuestNarrationList.size()), "Quest narration counter must be more than one" );

	vecdef_QuestNarrationList::iterator it = m_defQuestNarrationList.begin();

	sQUEST_NARRATION_DATASET* pData = &(*it);

	sNARRATION_ENTITY sData;

	sData.eNarrationProgType = eNARRATION_PROGRESS_STATE_PROGRESS;
	memcpy( &sData.sData, pData, sizeof(sQUEST_NARRATION_DATASET) );

	m_defNarrationEntityQueue.push_back( sData );

	m_defQuestNarrationList.erase( it );
}


void CDboUnifiedQuestNarration::OnBegin2End( void )
{
	NTL_ASSERT( (1 == m_defQuestNarrationList.size()), "Quest narration counter must be one" );

	vecdef_QuestNarrationList::iterator it = m_defQuestNarrationList.begin();

	sQUEST_NARRATION_DATASET* pData = &(*it);

	sNARRATION_ENTITY sData;

	sData.eNarrationProgType = eNARRATION_PROGRESS_STATE_END;
	memcpy( &sData.sData, pData, sizeof(sQUEST_NARRATION_DATASET) );

	m_defNarrationEntityQueue.push_back( sData );

	m_defQuestNarrationList.erase( it );

	ChangeState( eUQN_STATE_IDLE );
}


void CDboUnifiedQuestNarration::OnProgress2Process( void )
{
	NTL_ASSERT( (1 < m_defQuestNarrationList.size()), "Quest narration counter must be more than one" );

	vecdef_QuestNarrationList::iterator it = m_defQuestNarrationList.begin();

	sQUEST_NARRATION_DATASET* pData = &(*it);

	sNARRATION_ENTITY sData;

	sData.eNarrationProgType = eNARRATION_PROGRESS_STATE_PROGRESS;
	memcpy( &sData.sData, pData, sizeof(sQUEST_NARRATION_DATASET) );

	m_defNarrationEntityQueue.push_back( sData );

	m_defQuestNarrationList.erase( it );
}


void CDboUnifiedQuestNarration::OnProgress2End( void )
{
	NTL_ASSERT( (1 == m_defQuestNarrationList.size()), "Quest narration counter must be one" );

	vecdef_QuestNarrationList::iterator it = m_defQuestNarrationList.begin();

	sQUEST_NARRATION_DATASET* pData = &(*it);

	sNARRATION_ENTITY sData;

	sData.eNarrationProgType = eNARRATION_PROGRESS_STATE_END;
	memcpy( &sData.sData, pData, sizeof(sQUEST_NARRATION_DATASET) );

	m_defNarrationEntityQueue.push_back( sData );

	m_defQuestNarrationList.erase( it );

	ChangeState( eUQN_STATE_IDLE );
}


void CDboUnifiedQuestNarration::OnEnd2Idle( void )
{
}


void CDboUnifiedQuestNarration::OnSingle2Idle( void )
{
}
