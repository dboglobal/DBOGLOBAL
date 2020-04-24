#include "precomp_trigger.h"
#include "DboTSTAgency.h"
#include "DboTSTRecv.h"
#include "DboTSTCtrl.h"
#include "DboTSMain.h"
#include "DboTSCtrlFactory.h"


/** 
	Trigger agency
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSTAgency, CNtlTSAgency )


CDboTSTAgency::CDboTSTAgency( void )
: m_pParent( 0 ),
  m_pRecv( 0 )
{
}

CDboTSTAgency::~CDboTSTAgency( void )
{

}

void CDboTSTAgency::Update( void )
{
	mapdef_TRIGGER_LIST::iterator itCPQ = m_defCurTList.begin();
	for ( ; itCPQ != m_defCurTList.end(); )
	{
		CDboTSTCtrl* pQCtrl = itCPQ->second;
		if ( pQCtrl->IsExitState() )
		{
			GetParent()->GetControlFactory()->DeleteObj( (CNtlTSControlObject*&)pQCtrl );
			itCPQ = m_defCurTList.erase( itCPQ );
		}
		else
		{
			pQCtrl->Update();
			++itCPQ;
		}
	}
}

void CDboTSTAgency::SetParent( CDboTSMain* pParent )
{
	m_pParent = pParent;
}

void CDboTSTAgency::SetRecv( CDboTSTRecv* pRecv )
{
	m_pRecv = pRecv;
}

CDboTSTCtrl* CDboTSTAgency::FindProgressTrigger( NTL_TS_T_ID tId )
{
	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.find( tId );
	if ( it == m_defCurTList.end() ) return 0;
	else return it->second;
}

void CDboTSTAgency::AttachProgressTrigger( NTL_TS_T_ID tId, CDboTSTCtrl* pCtrl )
{
	m_defCurTList[tId] = pCtrl;
}

void CDboTSTAgency::DetachProgressTrigger( NTL_TS_T_ID tId )
{
	mapdef_TRIGGER_LIST::iterator it = m_defCurTList.find( tId );
	if ( it != m_defCurTList.end() ) m_defCurTList.erase( it );
}

CDboTSTCtrl* CDboTSTAgency::MakeTriggerController( CNtlTSTrigger* pTrig )
{
	// Trigger controller 를 생성한다
	CNtlTSControlObject* pCtrlObj = GetParent()->GetControlFactory()->CreateObj( "CDboTSTCtrl" );
	if ( !pCtrlObj->IsDerivedClass( "CDboTSTCtrl" ) )
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSTCtrl. Info[%s]. [%s]", pCtrlObj->GetClassName(), TS_CODE_TRACE() );
		m_pParent->GetControlFactory()->DeleteObj( pCtrlObj );
		return 0;
	}

	// TS의 trigger 와 Trigger 객체를 연결 및 저장한다
	((CDboTSTCtrl*)pCtrlObj)->SetTrigger( pTrig );
	((CDboTSTCtrl*)pCtrlObj)->SetParent( this );

	return (CDboTSTCtrl*)pCtrlObj;
}
