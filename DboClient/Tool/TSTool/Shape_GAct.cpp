#include "stdafx.h"
#include "Shape_GAct.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_GAct.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_GAct, CShape_GEntityCont, 1 )


CShape_GAct::CShape_GAct( const CPoint &point )
: CShape_GEntityCont( point, _T("CT_GAct") )
{
}

CShape_GAct::~CShape_GAct( void )
{
}

void CShape_GAct::ShowContAttrEditBox( void )
{
	CAttr_CONT_GAct AttrPage;
	AttrPage.AddAttr( _T("cid"), m_strContId );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_GAct::ShowEntityAttrAddBox( void )
{
	CAttrEntityMng* pAttMng = GetAttrEntityMng();

	DWORD dwProjMode = GetDoc()->GetProjectMode();
	eSEL_GROUP_TYPE eGroupType = GetDoc()->GetSelGroupType();

	std::map<CAttr_Page*, CString> mapPageList;
	std::map<CAttr_Page*, CString>::iterator itPageList;

	std::map<CString, CAttr_Page*> mapSortedPageList;
	std::map<CString, CAttr_Page*>::iterator itSortedPageList;

	if ( TS_TYPE_QUEST_CS == dwProjMode )
	{
		if ( eSEL_GROUP_TYPE_MAIN == eGroupType )
		{
//			mapPageList[pAttMng->GetPage( _T("A_Cine") )] = _T("A_Cine");
			mapPageList[pAttMng->GetPage( _T("A_Dir") )] = _T("A_Dir");
			mapPageList[pAttMng->GetPage( _T("A_ETimerS") )] = _T("A_ETimerS");
			mapPageList[pAttMng->GetPage( _T("A_ETimerE") )] = _T("A_ETimerE");
//			mapPageList[pAttMng->GetPage( _T("A_InSSM") )] = _T("A_InSSM");
			mapPageList[pAttMng->GetPage( _T("A_Item") )] =	 _T("A_Item");
			mapPageList[pAttMng->GetPage( _T("A_NPCConv") )] = _T("A_NPCConv");
//			mapPageList[pAttMng->GetPage( _T("A_OPCam") )] = _T("A_OPCam");
			mapPageList[pAttMng->GetPage( _T("A_QItem") )] = _T("A_QItem");
			mapPageList[pAttMng->GetPage( _T("A_RegQInfo") )] = _T("A_RegQInfo");
			mapPageList[pAttMng->GetPage( _T("A_SToCEvt") )] = _T("A_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("A_TWaitTS") )] = _T("A_TWaitTS");
			mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
			mapPageList[pAttMng->GetPage( _T("A_Potal") )] = _T("A_Potal");
			mapPageList[pAttMng->GetPage( _T("A_ObjState") )] = _T("A_ObjState");
//			mapPageList[pAttMng->GetPage( _T("A_ConcCheck") )] = _T("A_ConcCheck");
			mapPageList[pAttMng->GetPage( _T("A_ObjConv") )] = _T("A_ObjConv");
			mapPageList[pAttMng->GetPage( _T("A_Hint") )] = _T("A_Hint");
			mapPageList[pAttMng->GetPage( _T("A_SendSvrEvt") )] = _T("A_SendSvrEvt");
			mapPageList[pAttMng->GetPage( _T("A_TMQStage") )] = _T("A_TMQStage");
			mapPageList[pAttMng->GetPage( _T("A_OpenWindow") )] = _T("A_OpenWindow");
			mapPageList[pAttMng->GetPage( _T("A_TMQTimeBonus") )] = _T("A_TMQTimeBonus");
			mapPageList[pAttMng->GetPage( _T("A_BroadCast") )] = _T("A_BroadCast");
			mapPageList[pAttMng->GetPage( _T("A_DirIndicator") )] = _T("A_DirIndicator");
//			mapPageList[pAttMng->GetPage( _T("A_OperateObject") )] = _T("A_OperateObject");
			mapPageList[pAttMng->GetPage( _T("A_Drop") )] = _T("A_Drop");
//			mapPageList[pAttMng->GetPage( _T("A_RmvCoupon") )] = _T("A_RmvCoupon");
			mapPageList[pAttMng->GetPage( _T("A_Escort") )] = _T("A_Escort");
			mapPageList[pAttMng->GetPage( _T("A_TMQInfoShow") )] = _T("A_TMQInfoShow");
			mapPageList[pAttMng->GetPage( _T("A_WorldPlayScript") )] = _T("A_WorldPlayScript");
//			mapPageList[pAttMng->GetPage( _T("A_SWProbSF") )] = _T("A_SWProbSF");
			mapPageList[pAttMng->GetPage( _T("A_PCConv") )] = _T("A_PCConv");
			mapPageList[pAttMng->GetPage( _T("A_CustomEvt") )] = _T("A_CustomEvt");
			mapPageList[pAttMng->GetPage( _T("A_Minormatch") )] = _T("A_Minormatch");
			mapPageList[pAttMng->GetPage( _T("A_Mudosa") )] = _T("A_Mudosa");
			mapPageList[pAttMng->GetPage( _T("A_PIDgn") )] = _T("A_PIDgn");
			mapPageList[pAttMng->GetPage( _T("A_TLQ") )] = _T("A_TLQ");
//			mapPageList[pAttMng->GetPage( _T("A_DoSkill") )] = _T("A_DoSkill");
//			mapPageList[pAttMng->GetPage( _T("A_TObjFriendly") )] = _T("A_TObjFriendly");
			mapPageList[pAttMng->GetPage( _T("A_BroadMsg") )] = _T("A_BroadMsg");
			mapPageList[pAttMng->GetPage( _T("A_MiniNarration") )] = _T("A_MiniNarration");
			mapPageList[pAttMng->GetPage( _T("A_RegCTiming") )] = _T("A_RegCTiming");
			mapPageList[pAttMng->GetPage( _T("A_ExcCGroup") )] = _T("A_ExcCGroup");
			mapPageList[pAttMng->GetPage( _T("A_SkipCont") )] = _T("A_SkipCont");
//			mapPageList[pAttMng->GetPage( _T("A_ObjWPS") )] = _T("A_ObjWPS");
			mapPageList[pAttMng->GetPage( _T("A_Dojo") )] = _T("A_Dojo");

			mapSortedPageList.clear();
			for ( itPageList = mapPageList.begin(); itPageList != mapPageList.end(); ++itPageList )
			{
				mapSortedPageList[ itPageList->second ] = itPageList->first;
			}

			COptionListBox clListBox;
			CEntityRegisterDiag clDiag;
			clDiag.SetListControl( &clListBox );

			for ( itSortedPageList = mapSortedPageList.begin(); itSortedPageList != mapSortedPageList.end(); ++itSortedPageList ) clDiag.AddAttrPage( itSortedPageList->second );

			if ( IDOK == clDiag.DoModal() )
			{
				CAttr_Page* pEditedPage = clDiag.GetEditedAttrPage();

				itPageList = mapPageList.find( pEditedPage );
				if ( itPageList != mapPageList.end() )
				{
					AddEntity( itPageList->second, pEditedPage->GetAttr() );
				}

				UpdateHeightInfo();
				GetDoc()->OnShapeChange( this );
			}
		}
		else if ( eSEL_GROUP_TYPE_EXCEPTION == eGroupType )
		{
			CTSGroup* pGroup = GetDoc()->GetSelGroup();
			if ( NTL_TS_EXCEPT_GIVEUP_ID == pGroup->tgID )
			{
				mapPageList[pAttMng->GetPage( _T("A_QItem") )] = _T("A_QItem");
				mapPageList[pAttMng->GetPage( _T("A_Item") )] =	 _T("A_Item");
				mapPageList[pAttMng->GetPage( _T("A_ObjState") )] = _T("A_ObjState");
			}
			else if ( NTL_TS_EXCEPT_TLIMT_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_TLIMT_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
			}
			else if ( NTL_TS_EXCEPT_SERVER_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_SERVER_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
			}
			else if ( NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_CLIENT_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("A_NPCConv") )] = _T("A_NPCConv");
				mapPageList[pAttMng->GetPage( _T("A_ObjConv") )] = _T("A_ObjConv");
				mapPageList[pAttMng->GetPage( _T("A_RegCTiming") )] = _T("A_RegCTiming");
			}

			mapSortedPageList.clear();
			for ( itPageList = mapPageList.begin(); itPageList != mapPageList.end(); ++itPageList )
			{
				mapSortedPageList[ itPageList->second ] = itPageList->first;
			}

			COptionListBox clListBox;
			CEntityRegisterDiag clDiag;
			clDiag.SetListControl( &clListBox );

			for ( itSortedPageList = mapSortedPageList.begin(); itSortedPageList != mapSortedPageList.end(); ++itSortedPageList ) clDiag.AddAttrPage( itSortedPageList->second );

			if ( IDOK == clDiag.DoModal() )
			{
				CAttr_Page* pEditedPage = clDiag.GetEditedAttrPage();

				itPageList = mapPageList.find( pEditedPage );
				if ( itPageList != mapPageList.end() )
				{
					AddEntity( itPageList->second, pEditedPage->GetAttr() );
				}

				UpdateHeightInfo();
				GetDoc()->OnShapeChange( this );
			}
		}
	}
	else if ( TS_TYPE_PC_TRIGGER_CS == dwProjMode )
	{
		if ( eSEL_GROUP_TYPE_MAIN == eGroupType )
		{
//			mapPageList[pAttMng->GetPage( _T("A_Cine") )] = _T("A_Cine");
			mapPageList[pAttMng->GetPage( _T("A_Dir") )] = _T("A_Dir");
//			mapPageList[pAttMng->GetPage( _T("A_ETimerS") )] = _T("A_ETimerS");
//			mapPageList[pAttMng->GetPage( _T("A_ETimerE") )] = _T("A_ETimerE");
//			mapPageList[pAttMng->GetPage( _T("A_InSSM") )] = _T("A_InSSM");
//			mapPageList[pAttMng->GetPage( _T("A_Item") )] =	 _T("A_Item");
			mapPageList[pAttMng->GetPage( _T("A_NPCConv") )] = _T("A_NPCConv");
//			mapPageList[pAttMng->GetPage( _T("A_OPCam") )] = _T("A_OPCam");
//			mapPageList[pAttMng->GetPage( _T("A_QItem") )] = _T("A_QItem");
//			mapPageList[pAttMng->GetPage( _T("A_RegQInfo") )] = _T("A_RegQInfo");
//			mapPageList[pAttMng->GetPage( _T("A_SToCEvt") )] = _T("A_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("A_TWaitTS") )] = _T("A_TWaitTS");
//			mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
			mapPageList[pAttMng->GetPage( _T("A_Potal") )] = _T("A_Potal");
//			mapPageList[pAttMng->GetPage( _T("A_ObjState") )] = _T("A_ObjState");
//			mapPageList[pAttMng->GetPage( _T("A_ConcCheck") )] = _T("A_ConcCheck");
			mapPageList[pAttMng->GetPage( _T("A_ObjConv") )] = _T("A_ObjConv");
			mapPageList[pAttMng->GetPage( _T("A_Hint") )] = _T("A_Hint");
			mapPageList[pAttMng->GetPage( _T("A_SendSvrEvt") )] = _T("A_SendSvrEvt");
			mapPageList[pAttMng->GetPage( _T("A_TMQStage") )] = _T("A_TMQStage");
			mapPageList[pAttMng->GetPage( _T("A_OpenWindow") )] = _T("A_OpenWindow");
			mapPageList[pAttMng->GetPage( _T("A_TMQTimeBonus") )] = _T("A_TMQTimeBonus");
			mapPageList[pAttMng->GetPage( _T("A_BroadCast") )] = _T("A_BroadCast");
			mapPageList[pAttMng->GetPage( _T("A_DirIndicator") )] = _T("A_DirIndicator");
//			mapPageList[pAttMng->GetPage( _T("A_OperateObject") )] = _T("A_OperateObject");
			mapPageList[pAttMng->GetPage( _T("A_Drop") )] = _T("A_Drop");
//			mapPageList[pAttMng->GetPage( _T("A_RmvCoupon") )] = _T("A_RmvCoupon");
//			mapPageList[pAttMng->GetPage( _T("A_Escort") )] = _T("A_Escort");
			mapPageList[pAttMng->GetPage( _T("A_TMQInfoShow") )] = _T("A_TMQInfoShow");
//			mapPageList[pAttMng->GetPage( _T("A_WorldPlayScript") )] = _T("A_WorldPlayScript");
//			mapPageList[pAttMng->GetPage( _T("A_SWProbSF") )] = _T("A_SWProbSF");
			mapPageList[pAttMng->GetPage( _T("A_PCConv") )] = _T("A_PCConv");
			mapPageList[pAttMng->GetPage( _T("A_CustomEvt") )] = _T("A_CustomEvt");
			mapPageList[pAttMng->GetPage( _T("A_Minormatch") )] = _T("A_Minormatch");
			mapPageList[pAttMng->GetPage( _T("A_Mudosa") )] = _T("A_Mudosa");
			mapPageList[pAttMng->GetPage( _T("A_PIDgn") )] = _T("A_PIDgn");
			mapPageList[pAttMng->GetPage( _T("A_TLQ") )] = _T("A_TLQ");
//			mapPageList[pAttMng->GetPage( _T("A_DoSkill") )] = _T("A_DoSkill");
//			mapPageList[pAttMng->GetPage( _T("A_TObjFriendly") )] = _T("A_TObjFriendly");
			mapPageList[pAttMng->GetPage( _T("A_BroadMsg") )] = _T("A_BroadMsg");
			mapPageList[pAttMng->GetPage( _T("A_MiniNarration") )] = _T("A_MiniNarration");
//			mapPageList[pAttMng->GetPage( _T("A_RegCTiming") )] = _T("A_RegCTiming");
//			mapPageList[pAttMng->GetPage( _T("A_ExcCGroup") )] = _T("A_ExcCGroup");
//			mapPageList[pAttMng->GetPage( _T("A_SkipCont") )] = _T("A_SkipCont");
//			mapPageList[pAttMng->GetPage( _T("A_ObjWPS") )] = _T("A_ObjWPS");
			mapPageList[pAttMng->GetPage( _T("A_Dojo") )] = _T("A_Dojo");

			mapSortedPageList.clear();
			for ( itPageList = mapPageList.begin(); itPageList != mapPageList.end(); ++itPageList )
			{
				mapSortedPageList[ itPageList->second ] = itPageList->first;
			}

			COptionListBox clListBox;
			CEntityRegisterDiag clDiag;
			clDiag.SetListControl( &clListBox );

			for ( itSortedPageList = mapSortedPageList.begin(); itSortedPageList != mapSortedPageList.end(); ++itSortedPageList ) clDiag.AddAttrPage( itSortedPageList->second );

			if ( IDOK == clDiag.DoModal() )
			{
				CAttr_Page* pEditedPage = clDiag.GetEditedAttrPage();

				itPageList = mapPageList.find( pEditedPage );
				if ( itPageList != mapPageList.end() )
				{
					AddEntity( itPageList->second, pEditedPage->GetAttr() );
				}

				UpdateHeightInfo();
				GetDoc()->OnShapeChange( this );
			}
		}
		else if ( eSEL_GROUP_TYPE_EXCEPTION == eGroupType )
		{
			AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
		}
	}
	else if ( TS_TYPE_OBJECT_TRIGGER_S == dwProjMode )
	{
		if ( eSEL_GROUP_TYPE_MAIN == eGroupType )
		{
//			mapPageList[pAttMng->GetPage( _T("A_Cine") )] = _T("A_Cine");
			mapPageList[pAttMng->GetPage( _T("A_Dir") )] = _T("A_Dir");
			mapPageList[pAttMng->GetPage( _T("A_ETimerS") )] = _T("A_ETimerS");
			mapPageList[pAttMng->GetPage( _T("A_ETimerE") )] = _T("A_ETimerE");
//			mapPageList[pAttMng->GetPage( _T("A_InSSM") )] = _T("A_InSSM");
			mapPageList[pAttMng->GetPage( _T("A_Item") )] =	 _T("A_Item");
//			mapPageList[pAttMng->GetPage( _T("A_NPCConv") )] = _T("A_NPCConv");
//			mapPageList[pAttMng->GetPage( _T("A_OPCam") )] = _T("A_OPCam");
			mapPageList[pAttMng->GetPage( _T("A_QItem") )] = _T("A_QItem");
//			mapPageList[pAttMng->GetPage( _T("A_RegQInfo") )] = _T("A_RegQInfo");
//			mapPageList[pAttMng->GetPage( _T("A_SToCEvt") )] = _T("A_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("A_TWaitTS") )] = _T("A_TWaitTS");
//			mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
			mapPageList[pAttMng->GetPage( _T("A_Potal") )] = _T("A_Potal");
			mapPageList[pAttMng->GetPage( _T("A_ObjState") )] = _T("A_ObjState");
			mapPageList[pAttMng->GetPage( _T("A_ConcCheck") )] = _T("A_ConcCheck");
			mapPageList[pAttMng->GetPage( _T("A_ObjConv") )] = _T("A_ObjConv");
//			mapPageList[pAttMng->GetPage( _T("A_Hint") )] = _T("A_Hint");
			mapPageList[pAttMng->GetPage( _T("A_SendSvrEvt") )] = _T("A_SendSvrEvt");
			mapPageList[pAttMng->GetPage( _T("A_TMQStage") )] = _T("A_TMQStage");
//			mapPageList[pAttMng->GetPage( _T("A_OpenWindow") )] = _T("A_OpenWindow");
			mapPageList[pAttMng->GetPage( _T("A_TMQTimeBonus") )] = _T("A_TMQTimeBonus");
			mapPageList[pAttMng->GetPage( _T("A_BroadCast") )] = _T("A_BroadCast");
			mapPageList[pAttMng->GetPage( _T("A_DirIndicator") )] = _T("A_DirIndicator");
			mapPageList[pAttMng->GetPage( _T("A_OperateObject") )] = _T("A_OperateObject");
			mapPageList[pAttMng->GetPage( _T("A_Drop") )] = _T("A_Drop");
			mapPageList[pAttMng->GetPage( _T("A_RmvCoupon") )] = _T("A_RmvCoupon");
//			mapPageList[pAttMng->GetPage( _T("A_Escort") )] = _T("A_Escort");
			mapPageList[pAttMng->GetPage( _T("A_TMQInfoShow") )] = _T("A_TMQInfoShow");
//			mapPageList[pAttMng->GetPage( _T("A_WorldPlayScript") )] = _T("A_WorldPlayScript");
//			mapPageList[pAttMng->GetPage( _T("A_SWProbSF") )] = _T("A_SWProbSF");
			mapPageList[pAttMng->GetPage( _T("A_PCConv") )] = _T("A_PCConv");
			mapPageList[pAttMng->GetPage( _T("A_CustomEvt") )] = _T("A_CustomEvt");
//			mapPageList[pAttMng->GetPage( _T("A_Minormatch") )] = _T("A_Minormatch");
//			mapPageList[pAttMng->GetPage( _T("A_Mudosa") )] = _T("A_Mudosa");
			mapPageList[pAttMng->GetPage( _T("A_PIDgn") )] = _T("A_PIDgn");
			mapPageList[pAttMng->GetPage( _T("A_TLQ") )] = _T("A_TLQ");
//			mapPageList[pAttMng->GetPage( _T("A_DoSkill") )] = _T("A_DoSkill");
			mapPageList[pAttMng->GetPage( _T("A_TObjFriendly") )] = _T("A_TObjFriendly");
			mapPageList[pAttMng->GetPage( _T("A_BroadMsg") )] = _T("A_BroadMsg");
			mapPageList[pAttMng->GetPage( _T("A_MiniNarration") )] = _T("A_MiniNarration");
//			mapPageList[pAttMng->GetPage( _T("A_RegCTiming") )] = _T("A_RegCTiming");
//			mapPageList[pAttMng->GetPage( _T("A_ExcCGroup") )] = _T("A_ExcCGroup");
//			mapPageList[pAttMng->GetPage( _T("A_SkipCont") )] = _T("A_SkipCont");
			mapPageList[pAttMng->GetPage( _T("A_ObjWPS") )] = _T("A_ObjWPS");
//			mapPageList[pAttMng->GetPage( _T("A_Dojo") )] = _T("A_Dojo");

			mapSortedPageList.clear();
			for ( itPageList = mapPageList.begin(); itPageList != mapPageList.end(); ++itPageList )
			{
				mapSortedPageList[ itPageList->second ] = itPageList->first;
			}

			COptionListBox clListBox;
			CEntityRegisterDiag clDiag;
			clDiag.SetListControl( &clListBox );

			for ( itSortedPageList = mapSortedPageList.begin(); itSortedPageList != mapSortedPageList.end(); ++itSortedPageList ) clDiag.AddAttrPage( itSortedPageList->second );

			if ( IDOK == clDiag.DoModal() )
			{
				CAttr_Page* pEditedPage = clDiag.GetEditedAttrPage();

				itPageList = mapPageList.find( pEditedPage );
				if ( itPageList != mapPageList.end() )
				{
					AddEntity( itPageList->second, pEditedPage->GetAttr() );
				}

				UpdateHeightInfo();
				GetDoc()->OnShapeChange( this );
			}
		}
		else if ( eSEL_GROUP_TYPE_EXCEPTION == eGroupType )
		{
			CTSGroup* pGroup = GetDoc()->GetSelGroup();
			if ( NTL_TS_EXCEPT_GIVEUP_ID == pGroup->tgID )
			{
				mapPageList[pAttMng->GetPage( _T("A_QItem") )] = _T("A_QItem");
				mapPageList[pAttMng->GetPage( _T("A_Item") )] =	 _T("A_Item");
			}
			else if ( NTL_TS_EXCEPT_TLIMT_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_TLIMT_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
			}
			else if ( NTL_TS_EXCEPT_SERVER_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_SERVER_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("A_TSState") )] = _T("A_TSState");
				mapPageList[pAttMng->GetPage( _T("A_DoSkill") )] = _T("A_DoSkill");
			}
			else if ( NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_CLIENT_GROUP_ID_END )
			{
			}

			mapSortedPageList.clear();
			for ( itPageList = mapPageList.begin(); itPageList != mapPageList.end(); ++itPageList )
			{
				mapSortedPageList[ itPageList->second ] = itPageList->first;
			}

			COptionListBox clListBox;
			CEntityRegisterDiag clDiag;
			clDiag.SetListControl( &clListBox );

			for ( itSortedPageList = mapSortedPageList.begin(); itSortedPageList != mapSortedPageList.end(); ++itSortedPageList ) clDiag.AddAttrPage( itSortedPageList->second );

			if ( IDOK == clDiag.DoModal() )
			{
				CAttr_Page* pEditedPage = clDiag.GetEditedAttrPage();

				itPageList = mapPageList.find( pEditedPage );
				if ( itPageList != mapPageList.end() )
				{
					AddEntity( itPageList->second, pEditedPage->GetAttr() );
				}

				UpdateHeightInfo();
				GetDoc()->OnShapeChange( this );
			}
		}
	}
}

void CShape_GAct::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
}
