/*****************************************************************************
*
* File			: DLState.h
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


#include "DLEventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// eDL_STATE_RESULT
//
//////////////////////////////////////////////////////////////////////////


enum eDL_STATE_RESULT
{
	eDL_CMD_RESULT_CONTINUE,		// 현재 상태 유지
	eDL_CMD_RESULT_FINISH,			// 현재 상태 종료
};


//////////////////////////////////////////////////////////////////////////
//
// eDL_STATE
//
//////////////////////////////////////////////////////////////////////////


enum eDL_STATE
{
	eDL_STATE_START,					// 상태가 처음 시작되면 무조건 이 상태부터 시작함.

	eDL_STATE_DISPATCH_DECISION,		// 분기 결정 상태

	eDL_STATE_LAUNCHER_PATCH,			// 런처 패치 상태

	eDL_STATE_CLIENT_PATCH,				// 클라이언트 패치 상태

	eDL_STATE_IDLE,						// 사용자의 입력을 대기하는 상태

	eDL_STATE_INTEGRITY_CHECK,			// 무결성 검사 상태

	eDL_STATE_END,						// 종료 상태

	eDL_STATE_INVALID
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateBase
//
//////////////////////////////////////////////////////////////////////////


class CDLStateBase
{
protected:
	bool							m_bUserExit;

	eDL_STATE						m_eCurState;
	eDL_STATE						m_eNextState;

	void*							m_pStateTransData;

	eDL_STATE_RESULT				m_eStateResult;

public:
	CDLStateBase( eDL_STATE eCurState ) : m_eCurState( eCurState ) { m_bUserExit = false; m_eNextState = eDL_STATE_INVALID; m_pStateTransData = NULL; m_eStateResult = eDL_CMD_RESULT_CONTINUE; }
	virtual ~CDLStateBase( void )	{ return; }

public:
	eDL_STATE_RESULT				GetStateResult( void )							{ return m_eStateResult; }

	eDL_STATE						GetCurState( void )								{ return m_eCurState; }

	void							SetNextState( eDL_STATE eState )				{ m_eNextState = eState; }
	eDL_STATE						GetNextState( void )							{ return m_eNextState; }

	void*							GetStateTransData( void )						{ return m_pStateTransData; }
	void							SetStateTransData( void* pStateTransData )		{ m_pStateTransData = pStateTransData; }

	void							ChangeState( eDL_STATE eState, void* pStateTransData = NULL )	{ m_eNextState = eState; m_pStateTransData = pStateTransData; m_eStateResult = eDL_CMD_RESULT_FINISH; }

	virtual void					Enter( void )									{ return; }
	virtual void					Update( void )									{ return; }
	virtual void					Exit( void )									{ return; }

	void							DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateStart
//
//////////////////////////////////////////////////////////////////////////


class CDLStateStart : public CDLStateBase
{
protected:
	unsigned int					m_uiGoalTime;

public:
	CDLStateStart( void ) : CDLStateBase( eDL_STATE_START ) { return; }

public:
	virtual void					Enter( void );
	virtual void					Update( void );

protected:
	bool							IsMultiOpen( CString strExeName, bool bCreateMutex );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateDispatchDecision
//
//////////////////////////////////////////////////////////////////////////


class CDLStateDispatchDecision : public CDLStateBase
{
public:
	CDLStateDispatchDecision( void ) : CDLStateBase( eDL_STATE_DISPATCH_DECISION ) { return; }

public:
	virtual void					Enter( void );

protected:
	bool							LoadLocalConfig( void );
	bool							LoadServerConfig( void );
	bool							LoadVersionList( void );
	bool							LoadCmdLine( void );
	bool							CreateSkin( void );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateLauncherPatch
//
//////////////////////////////////////////////////////////////////////////


class CDLStateLauncherPatch : public CDLStateBase
{
public:
	CDLStateLauncherPatch( void ) : CDLStateBase( eDL_STATE_LAUNCHER_PATCH ) { return; }

public:
	virtual void					Enter( void );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateClientPatch
//
//////////////////////////////////////////////////////////////////////////


class CDLStateClientPatch : public CDLStateBase
{
public:
	CDLStateClientPatch( void ) : CDLStateBase( eDL_STATE_CLIENT_PATCH ) { return; }

public:
	virtual void					Enter( void );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateIdle
//
//////////////////////////////////////////////////////////////////////////


class CDLStateIdle : public CDLStateBase
{
public:
	CDLStateIdle( void ) : CDLStateBase( eDL_STATE_IDLE ) { return; }

public:
	virtual void					Enter( void );
	virtual void					Exit( void );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateIntegrityCheck
//
//////////////////////////////////////////////////////////////////////////


class CDLStateIntegrityCheck : public CDLStateBase
{
public:
	CDLStateIntegrityCheck( void ) : CDLStateBase( eDL_STATE_INTEGRITY_CHECK ) { return; }

public:
	virtual void					Enter( void );

protected:
	bool							UnZip( CString strSrcPath, CString strDestPath, CString strFileName );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateEnd
//
//////////////////////////////////////////////////////////////////////////


class CDLStateEnd : public CDLStateBase
{
public:
	CDLStateEnd( void ) : CDLStateBase( eDL_STATE_END ) { return; }

public:
	virtual void					Enter( void );

protected:
	void							DeleteSkin( void );
	void							DeleteFolder( CString strFolderName );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLStateFactory
//
//////////////////////////////////////////////////////////////////////////


class CDLStateFactory
{
public:
	static CDLStateBase*			CreateState( eDL_STATE eState );
};
