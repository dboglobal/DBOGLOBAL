#include "StdAfx.h"
#include "TSMng.h"
#include "TSTrigger.h"
#include "Shape_GEntityCont.h"
#include "Shape_Link.h"
#include "AttrEntityMng.h"


CTSMng::CTSMng( void )
{
}

CTSMng::~CTSMng( void )
{
}

bool CTSMng::Create( void )
{
	return CDboTSMain::Create();
}

void CTSMng::Delete( void )
{
	CDboTSMain::Delete();
}

CNtlTSTrigger* CTSMng::CreateTrigger( CTSTrigger* pTSTrigger )
{
	POSITION Pos;
	CNtlTSScrProperty clProperty;

	//////////////////////////////////////////////////////////////////////////
	// 트리거 생성 및 정보 설정 //////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CNtlTSTrigger* pTrigger = (CNtlTSTrigger*)m_pEntityFactory->CreateObj( "CNtlTSTrigger" );

	CString strValue = pTSTrigger->GetAttr( _T("tid") );
	pTrigger->SetID( strValue.IsEmpty() ? NTL_TS_T_ID_INVALID : atoi( strValue.GetString() ) );
	pTrigger->SetRepeatQuest( atoi( pTSTrigger->GetAttr( _T("rq") ).GetString() ) == 1 ? true : false );
	pTrigger->SetShareQuest( atoi( pTSTrigger->GetAttr( _T("sq") ).GetString() ) == 1 ? true : false );
	pTrigger->SetOutStateMsg( atoi( pTSTrigger->GetAttr( _T("sm") ).GetString() ) == 1 ? true : false );
	pTrigger->SetTitle( atoi( pTSTrigger->GetAttr( _T("title") ).GetString() ) );
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 메인 그룹 생성 및 정보 설정 ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CNtlTSGroup* pMainGroup = (CNtlTSGroup*)m_pEntityFactory->CreateObj( "CNtlTSGroup" );

	// 자체 속성
	pMainGroup->SetID( pTSTrigger->GetMainGroup()->tgID );

	// 자식 속성
	for ( Pos = pTSTrigger->GetMainGroup()->ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape_GEntityCont* pShape = DYNAMIC_DOWNCAST( CShape_GEntityCont, pTSTrigger->GetMainGroup()->ContainerLayer.GetPrev( Pos ) );

		if ( NULL == pShape )
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		// 컨테이너
		//////////////////////////////////////////////////////////////////////////
		clProperty.Clear();
		pShape->GetProperty( clProperty );

		CNtlTSCont* pCont = (CNtlTSCont*)m_pEntityFactory->CreateObj( GetAttrEntityMng()->GetName( pShape->GetContType() ).GetString() );
		pCont->ApplyScriptDataForScript( clProperty );

		pMainGroup->AttachChildCont( pCont );
		pCont->SetParent( pMainGroup );

		//////////////////////////////////////////////////////////////////////////
		// 엔티티
		//////////////////////////////////////////////////////////////////////////
		const CArray<CShape_GEntityCont::sENTITY, const CShape_GEntityCont::sENTITY&>& List = pShape->GetEntityList();
		for ( int i = 0; i < (int)List.GetSize(); ++i )
		{
			clProperty.Clear();
			pShape->GetEntityProperty( i, clProperty );

			CNtlTSEntity* pEntity = (CNtlTSEntity*)m_pEntityFactory->CreateObj( GetAttrEntityMng()->GetName( List[i].strType ).GetString() );
			pEntity->ApplyScriptDataForScript( clProperty );

			pCont->AttachChildEntity( pEntity );
			pEntity->SetParent( pCont );
		}
	}

	pTrigger->AttachGroup( pMainGroup );
	pMainGroup->SetParent( pTrigger );

	for ( Pos = pTSTrigger->GetMainGroup()->LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape_Link* pShape = (CShape_Link*)pTSTrigger->GetMainGroup()->LinkLayer.GetPrev( Pos );

		if ( NULL == DYNAMIC_DOWNCAST( CShape_GEntityCont, pShape->GetSrcAnchor().pShape ) ||
			 NULL == DYNAMIC_DOWNCAST( CShape_GEntityCont, pShape->GetDestAnchor().pShape ) )
		{
			continue;
		}

		NTL_TS_TC_ID tcSrcID = ((CShape_GEntityCont*)pShape->GetSrcAnchor().pShape)->GetContID();
		NTL_TS_TC_ID tcDestID = ((CShape_GEntityCont*)pShape->GetDestAnchor().pShape)->GetContID();

		CShape_Link::eBRANCH_TYPE eBType = pShape->GetBranchType();

		CNtlTSCont* pSrcCont = pTrigger->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcSrcID );
		CNtlTSCont* pDestCont = pTrigger->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcDestID );

		pDestCont->AttachPrevLink( tcSrcID );

		switch ( pSrcCont->GetEntityType() )
		{
		case DBO_CONT_TYPE_ID_CONT_GCOND:
			if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
			{
				((CDboTSContGCond*)pSrcCont)->AttachYesLinkID( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContGCond*)pSrcCont)->AttachNoLinkID( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_START:
			if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
			{
				((CDboTSContStart*)pSrcCont)->AttachYesLinkID( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContStart*)pSrcCont)->AttachNoLinkID( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_GACT:
			if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
			{
				((CDboTSContGAct*)pSrcCont)->AttachNextLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_ERROR == eBType )
			{
				((CDboTSContGAct*)pSrcCont)->AttachErrorLink( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_REWARD:
			if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
			{
				((CDboTSContReward*)pSrcCont)->AttachNextLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContReward*)pSrcCont)->AttachCancelLink( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_END:
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_USERSEL:
			if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
			{
				((CDboTSContUsrSel*)pSrcCont)->AttachNextLink( tcDestID, atoi( pShape->GetBranchName().GetString() ) );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContUsrSel*)pSrcCont)->SetCancelLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_LOGIN == eBType )
			{
				((CDboTSContUsrSel*)pSrcCont)->SetLogInLink( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_NARRATION:
			if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
			{
				((CDboTSContNarration*)pSrcCont)->SetOkLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContNarration*)pSrcCont)->SetCancelLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_LOGIN == eBType )
			{
				((CDboTSContNarration*)pSrcCont)->SetLogInLink( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
			if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
			{
				((CDboTSContProposal*)pSrcCont)->SetOkLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContProposal*)pSrcCont)->SetCancelLink( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_SWITCH:
			if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
			{
				((CDboTSContSwitch*)pSrcCont)->AttachNextLink( tcDestID, atoi( pShape->GetBranchName().GetString() ) );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
			if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
			{
				((CDboTSContUnifiedNarration*)pSrcCont)->SetOkLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
			{
				((CDboTSContUnifiedNarration*)pSrcCont)->SetCancelLink( tcDestID );
			}
			else if ( CShape_Link::eBRANCH_TYPE_LOGIN == eBType )
			{
				((CDboTSContUnifiedNarration*)pSrcCont)->SetLogInLink( tcDestID );
			}
			else
			{
				CString strMsg;
				strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
				AfxMessageBox( strMsg.GetString() );
			}
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// 예외 그룹 생성 및 정보 설정 ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CNtlTSGroup* pExceptionGroup;
	int nSize = (int)pTSTrigger->GetExceptionGroupCnt();
	for ( int i = 0; i < nSize; ++i )
	{
		pExceptionGroup = (CNtlTSGroup*)m_pEntityFactory->CreateObj( "CNtlTSGroup" );

		// 자체 속성
		pExceptionGroup->SetID( pTSTrigger->GetExceptionGroup( i )->tgID );

		// 자식 속성
		for ( Pos = pTSTrigger->GetExceptionGroup( i )->ContainerLayer.GetTailPosition(); Pos != NULL; )
		{
			CShape_GEntityCont* pShape = DYNAMIC_DOWNCAST( CShape_GEntityCont, pTSTrigger->GetExceptionGroup( i )->ContainerLayer.GetPrev( Pos ) );

			if ( NULL == pShape )
			{
				continue;
			}

			//////////////////////////////////////////////////////////////////////////
			// 컨테이너
			//////////////////////////////////////////////////////////////////////////
			clProperty.Clear();
			pShape->GetProperty( clProperty );

			CNtlTSCont* pCont = (CNtlTSCont*)m_pEntityFactory->CreateObj( GetAttrEntityMng()->GetName( pShape->GetContType() ).GetString() );
			pCont->ApplyScriptDataForScript( clProperty );

			pExceptionGroup->AttachChildCont( pCont );
			pCont->SetParent( pExceptionGroup );

			//////////////////////////////////////////////////////////////////////////
			// 엔티티
			//////////////////////////////////////////////////////////////////////////
			const CArray<CShape_GEntityCont::sENTITY, const CShape_GEntityCont::sENTITY&>& List = pShape->GetEntityList();
			for ( int i = 0; i < (int)List.GetSize(); ++i )
			{
				clProperty.Clear();
				pShape->GetEntityProperty( i, clProperty );

				CNtlTSEntity* pEntity = (CNtlTSEntity*)m_pEntityFactory->CreateObj( GetAttrEntityMng()->GetName( List[i].strType ).GetString() );
				pEntity->ApplyScriptDataForScript( clProperty );

				pCont->AttachChildEntity( pEntity );
				pEntity->SetParent( pCont );
			}
		}

		pTrigger->AttachGroup( pExceptionGroup );
		pExceptionGroup->SetParent( pTrigger );

		for ( Pos = pTSTrigger->GetExceptionGroup( i )->LinkLayer.GetTailPosition(); Pos != NULL; )
		{
			CShape_Link* pShape = (CShape_Link*)pTSTrigger->GetExceptionGroup( i )->LinkLayer.GetPrev( Pos );

			if ( NULL == DYNAMIC_DOWNCAST( CShape_GEntityCont, pShape->GetSrcAnchor().pShape ) ||
				 NULL == DYNAMIC_DOWNCAST( CShape_GEntityCont, pShape->GetDestAnchor().pShape ) )
			{
				continue;
			}

			NTL_TS_TC_ID tcSrcID = ((CShape_GEntityCont*)pShape->GetSrcAnchor().pShape)->GetContID();
			NTL_TS_TC_ID tcDestID = ((CShape_GEntityCont*)pShape->GetDestAnchor().pShape)->GetContID();

			CShape_Link::eBRANCH_TYPE eBType = pShape->GetBranchType();

			CNtlTSCont* pSrcCont = pTrigger->GetGroup( pExceptionGroup->GetID() )->GetChildCont( tcSrcID );
			CNtlTSCont* pDestCont = pTrigger->GetGroup( pExceptionGroup->GetID() )->GetChildCont( tcDestID );

			pDestCont->AttachPrevLink( tcSrcID );

			switch ( pSrcCont->GetEntityType() )
			{
			case DBO_CONT_TYPE_ID_CONT_GCOND:
				if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
				{
					((CDboTSContGCond*)pSrcCont)->AttachYesLinkID( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContGCond*)pSrcCont)->AttachNoLinkID( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_START:
				if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
				{
					((CDboTSContStart*)pSrcCont)->AttachYesLinkID( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContStart*)pSrcCont)->AttachNoLinkID( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_GACT:
				if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
				{
					((CDboTSContGAct*)pSrcCont)->AttachNextLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_ERROR == eBType )
				{
					((CDboTSContGAct*)pSrcCont)->AttachErrorLink( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_REWARD:
				if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
				{
					((CDboTSContReward*)pSrcCont)->AttachNextLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContReward*)pSrcCont)->AttachCancelLink( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_END:
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_USERSEL:
				if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
				{
					((CDboTSContUsrSel*)pSrcCont)->AttachNextLink( tcDestID, atoi( pShape->GetBranchName().GetString() ) );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContUsrSel*)pSrcCont)->SetCancelLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_LOGIN == eBType )
				{
					((CDboTSContUsrSel*)pSrcCont)->SetLogInLink( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_NARRATION:
				if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
				{
					((CDboTSContNarration*)pSrcCont)->SetOkLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContNarration*)pSrcCont)->SetCancelLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_LOGIN == eBType )
				{
					((CDboTSContNarration*)pSrcCont)->SetLogInLink( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_PROPOSAL:
				if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
				{
					((CDboTSContProposal*)pSrcCont)->SetOkLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContProposal*)pSrcCont)->SetCancelLink( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_SWITCH:
				if ( CShape_Link::eBRANCH_TYPE_DEFAULT == eBType )
				{
					((CDboTSContSwitch*)pSrcCont)->AttachNextLink( tcDestID, atoi( pShape->GetBranchName().GetString() ) );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_UNIFIED_NARRATION:
				if ( CShape_Link::eBRANCH_TYPE_YES == eBType )
				{
					((CDboTSContUnifiedNarration*)pSrcCont)->SetOkLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_NO == eBType )
				{
					((CDboTSContUnifiedNarration*)pSrcCont)->SetCancelLink( tcDestID );
				}
				else if ( CShape_Link::eBRANCH_TYPE_LOGIN == eBType )
				{
					((CDboTSContUnifiedNarration*)pSrcCont)->SetLogInLink( tcDestID );
				}
				else
				{
					CString strMsg;
					strMsg.Format( _T("링크가 잘못되었습니다. [%d, %d]"), eBType, pTrigger->GetID() );
					AfxMessageBox( strMsg.GetString() );
				}
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return pTrigger;
}

void CTSMng::DeleteTrigger( CNtlTSTrigger* pNTLTrigger )
{
	m_pEntityFactory->DeleteObj( (CNtlTSScrObject*&)pNTLTrigger );
}

bool CTSMng::LoadLog( void )
{
	UnloadLog();

	m_pLog = new CNtlTSLog;
	CNtlTSLog::Register( m_pLog );

	return true;
}

void CTSMng::UnloadLog( void )
{
	if ( m_pLog )
	{
		CNtlTSLog::Unregister();
		delete m_pLog;
		m_pLog = 0;
	}
}

bool CTSMng::LoadFactories( void )
{
	UnloadFactories();

	// Entity factory 생성
	m_pEntityFactory = new CDboTSEntityFactory;
	m_pEntityFactory->RegisterEntityType();

	return true;
}

void CTSMng::UnloadFactories( void )
{
	if ( m_pUIFactory )
	{
		delete m_pUIFactory;
		m_pUIFactory = 0;
	}
}
