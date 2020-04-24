/*****************************************************************************
*
* File			: DLMessageImp.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


#include "DLMessageDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder
//
//////////////////////////////////////////////////////////////////////////


class CDLMsgFinder
{
public:
	typedef std::map< eDL_MSG, CString > mapdef_MsgList;

protected:
	CString							m_strDefMsg;
	mapdef_MsgList					m_defMsgList;

public:
	CDLMsgFinder( void );
	virtual ~CDLMsgFinder( void );

public:
	bool							Create( void );
	void							Delete( void );

	CString							FindString( eDL_MSG eMsg );

protected:
	virtual void					RegString( void ) { return; }
};


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_KOR
//
//////////////////////////////////////////////////////////////////////////


class CDLMsgFinder_KOR : public CDLMsgFinder
{
protected:
	virtual void					RegString( void );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_JPN
//
//////////////////////////////////////////////////////////////////////////


class CDLMsgFinder_JPN : public CDLMsgFinder
{
protected:
	virtual void					RegString( void );
};


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_ENG
//
//////////////////////////////////////////////////////////////////////////


class CDLMsgFinder_ENG : public CDLMsgFinder
{
protected:
	virtual void					RegString( void );
};
