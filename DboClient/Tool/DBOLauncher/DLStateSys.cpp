/*****************************************************************************
*
* File			: DLStateSys.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DLStateSys.h"
#include "DLState.h"
#include "DLUtility.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLStateSys
//
//////////////////////////////////////////////////////////////////////////


CDLStateSys::CDLStateSys( void )
{
	m_pCurState = NULL;
}

CDLStateSys::~CDLStateSys( void )
{
	Delete();
}

bool CDLStateSys::Create( void )
{
	m_pCurState = CDLStateFactory::CreateState( eDL_STATE_START );

	if ( NULL == m_pCurState )
	{
		return false;
	}

	m_pCurState->Enter();

	return true;
}

void CDLStateSys::Delete( void )
{
	if ( m_pCurState )
	{
		m_pCurState->Exit();

		delete m_pCurState;
		m_pCurState = NULL;
	}
}

void CDLStateSys::Update( void )
{
	bool bFinish = false;

	if ( m_pCurState )
	{
		eDL_STATE_RESULT eResult = m_pCurState->GetStateResult();

		switch ( eResult )
		{
		case eDL_CMD_RESULT_CONTINUE:
			{
				m_pCurState->Update();
			}
			break;

		case eDL_CMD_RESULT_FINISH:
			{
				void* pStateTransData = m_pCurState->GetStateTransData();

				m_pCurState->Exit();

				CDLStateBase* pNextState = CDLStateFactory::CreateState( m_pCurState->GetNextState() );

				delete m_pCurState;

				m_pCurState = pNextState;

				if ( m_pCurState )
				{
					m_pCurState->SetStateTransData( pStateTransData );

					m_pCurState->Enter();
				}
				else
				{
					bFinish = true;
				}
			}
			break;

		default:
			{
				if ( m_pCurState )
				{
					m_pCurState->Exit();

					delete m_pCurState;

					m_pCurState = NULL;
				}

				bFinish = true;
			}
			break;
		}
	}
	else
	{
		bFinish = true;
	}

	if ( bFinish )
	{
		::PostMessage( NULL, WM_QUIT, NULL, NULL );
	}
}

void CDLStateSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( m_pCurState )
	{
		m_pCurState->DLMessageProc( eMsg, pData );
	}
}
