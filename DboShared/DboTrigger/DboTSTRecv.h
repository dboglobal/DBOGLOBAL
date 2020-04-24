#ifndef _DBO_TSTRECV_H_
#define _DBO_TSTRECV_H_


class CDboTSTCtrl;
class CDboTSTAgency;


/** 
	Trigger receiver
*/


struct sTRUN_PARAM
{
public:
	sTRUN_PARAM( void ) : m_pCtrl( 0 ), m_pAgency( 0 ) { return; }

public:
	virtual void						Init( void ) { m_pCtrl = 0; m_pAgency = 0; }

	void								SetControl( CDboTSTCtrl* pCtrl ) { m_pCtrl = pCtrl; }
	void								SetAgency( CDboTSTAgency* pAgency ) { this->m_pAgency = pAgency; }

	virtual bool						IsQuestShare( void ) { return false; }
	virtual void						SetQuestShare( bool bQuestShare ) { UNREFERENCED_PARAMETER( bQuestShare ); }

	virtual bool						IsQuestShareOutMsg( void ) { return false; }
	virtual void						SetQuestShareOutMsg( bool bOutMsg ) { UNREFERENCED_PARAMETER( bOutMsg ); }

	virtual bool						IsForCheckQuestMark( void ) { return false; }
	virtual void						SetForCheckQuestMark( bool bCheckForQuestMark ) { UNREFERENCED_PARAMETER( bCheckForQuestMark ); }

	virtual bool						IsSkipContainer( void ) { return false; }
	virtual void						SetSkipContainer( bool bSkipCont ) { UNREFERENCED_PARAMETER( bSkipCont ); }

protected:

	CDboTSTCtrl*						m_pCtrl;
	CDboTSTAgency*						m_pAgency;
};


class CDboTSTRecv : public CNtlTSRecv
{
	NTL_TS_DECLARE_RTTI

// Methods
public:
	virtual	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	virtual	NTL_TSRESULT				Run( CNtlTSEntity* pEntity, void* pParam );
};


#endif
