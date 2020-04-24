#ifndef _DBO_TSCTRECV_H_
#define _DBO_TSCTRECV_H_


#include "DboTSCore.h"


class CDboTSCTCtrl;
class CDboTSCTAgency;


/** 
	Client trigger receiver
*/


struct sCTRUN_PARAM : public sTRUN_PARAM
{
public:
	sCTRUN_PARAM( void ) { return; }

public:
	virtual void						Init( void ) { sTRUN_PARAM::Init(); }

	CDboTSCTCtrl*						GetCtrl( void ) { return (CDboTSCTCtrl*)m_pCtrl; }
	CDboTSCTAgency*						GetAgency( void ) { return (CDboTSCTAgency*)m_pAgency; }
};


class CDboTSCTRecv : public CDboTSTRecv
{
	NTL_TS_DECLARE_RTTI

// Methods
public:
	virtual	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	virtual	NTL_TSRESULT				Run( CNtlTSEntity* pEntity, void* pParam );
};


#endif