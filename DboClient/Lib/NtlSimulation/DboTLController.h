/*****************************************************************************
*
* File			: DboTLController.h
* Author		: JeongHo, Rho
* Copyright		: (аж)NTL
* Date			: 2008. 4. 22	
* Abstract		: 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_DBO_TL_CONTROLLER_H__
#define __NTL_DBO_TL_CONTROLLER_H__


#include "ceventhandler.h"


class CDboTLSyncGroup;


class CDboTLController
{
// Declarations
public:
	typedef std::list< CDboTLSyncGroup* > listdef_GroupList;
	typedef listdef_GroupList::iterator listdef_GroupListIT;

// Member variables
protected:
	listdef_GroupList	m_defGroupList;
	listdef_GroupListIT	m_itCurGroup;
	RwBool				m_bEnableSkip;
	RwBool				m_bSkipping;

// Constructions and Destructions
public:
	CDboTLController( void );
	~CDboTLController( void );

// Operations
public:
	void				PushTLSyncGroup( CDboTLSyncGroup* pTLSyncGroup );

	RwBool				IsFinish( void );

	void				Update( RwReal fElapsedTime );

	void				Skip();

	RwBool				IsSkipping();

	void				HandleEvents( RWS::CMsg &pMsg );

// Implementations
protected:
	void				ChangeCurGroup( void );
};


#endif
