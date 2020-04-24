#ifndef _DBO_TSCQRECV_H_
#define _DBO_TSCQRECV_H_


#include "DboTSCore.h"


class CDboTSCQCtrl;
class CDboTSCQAgency;


/** 
	Client quest receiver
*/


struct sCQRUN_PARAM : public sQRUN_PARAM
{
public:
	sCQRUN_PARAM( void ) : bQuestShare( false ), bQuestShareOutMsg( false ), bCheckForQuestMark( false ) { return; }

public:
	virtual void						Init( void ) { bQuestShare = false; bQuestShareOutMsg = false; bCheckForQuestMark = false; sQRUN_PARAM::Init(); }

	virtual bool						IsQuestShare( void ) { return bQuestShare; }
	virtual void						SetQuestShare( bool bQuestShare ) { this->bQuestShare = bQuestShare; }

	virtual bool						IsQuestShareOutMsg( void ) { return bQuestShareOutMsg; }
	virtual void						SetQuestShareOutMsg( bool bOutMsg ) { bQuestShareOutMsg = bOutMsg; }

	virtual bool						IsForCheckQuestMark( void ) { return bCheckForQuestMark; }
	virtual void						SetForCheckQuestMark( bool bCheckForQuestMark ) { this->bCheckForQuestMark = bCheckForQuestMark; }

	CDboTSCQCtrl*						GetCtrl( void) { return (CDboTSCQCtrl*)m_pCtrl; }
	CDboTSCQAgency*						GetAgency( void ) { return (CDboTSCQAgency*)m_pAgency; }

protected:
	bool								bQuestShare;
	bool								bQuestShareOutMsg;

	bool								bCheckForQuestMark;
};


class CDboTSCQRecv : public CDboTSQRecv
{
	NTL_TS_DECLARE_RTTI

// Methods
public:
	virtual	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	virtual	NTL_TSRESULT				Run( CNtlTSEntity* pEntity, void* pParam );
};


#endif