/*****************************************************************************
*
* File			: DLProcessSys.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


#include "DLEventSys.h"


class CDLMessageSys;
class CDLDownLoadSys;
class CDLPatcherSys;
class CDLIntegritySys;
class CDLSkinSys;
class CDLStateSys;


//////////////////////////////////////////////////////////////////////////
//
// CDLProcessSys
//
//////////////////////////////////////////////////////////////////////////


class CDLProcessSys : public CDLEventBase
{
protected:
	CDLEventSys*			m_pEvtSys;
	CDLMessageSys*			m_pMsgSys;
	CDLDownLoadSys*			m_pDownloadSys;
	CDLPatcherSys*			m_pPatcherSys;
	CDLIntegritySys*		m_pIntegritySys;
	CDLSkinSys*				m_pSkinSys;
	CDLStateSys*			m_pStateSys;

public:
	CDLProcessSys( void );
	virtual ~CDLProcessSys( void );

public:
	bool					Create( void );
	void					Delete( void );
	bool					Update( void );

	CDLEventSys*			GetEventSys( void )		{ return m_pEvtSys; }
	CDLMessageSys*			GetMessageSys( void )	{ return m_pMsgSys; }
	CDLIntegritySys*		GetIntegritySys( void )	{ return m_pIntegritySys; }
	CDLSkinSys*				GetSkinSys( void )		{ return m_pSkinSys; }
	CDLStateSys*			GetStateSys( void )		{ return m_pStateSys; }

protected:
	virtual void			DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );
};


extern CDLProcessSys g_clDLProcessSys;