#include "stdafx.h"
#include "Shape_Start.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_Start.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_Start, CShape_GEntityCont, 1 )


CShape_Start::CShape_Start( const CPoint& point )
: CShape_GEntityCont( point, _T("CT_Start") )
{
	m_strContId = _T("0");

	AddAttr( _T("stdiag"), CString() );
}

CShape_Start::~CShape_Start( void )
{
}

void CShape_Start::ShowContAttrEditBox( void )
{
	CAttr_CONT_Start AttrPage;

	AttrPage.AddAttr( _T("cid"), m_strContId );
	AttrPage.AddAttr( _T("stdiag"), GetAttr( _T("stdiag") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("stdiag") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("stdiag"), strValue );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_Start::ShowEntityAttrAddBox( void )
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
			mapPageList[pAttMng->GetPage( _T("E_ColObject") )] = _T("E_ColObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickObject") )] = _T("E_ClickObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickNPC") )] = _T("E_ClickNPC");
			mapPageList[pAttMng->GetPage( _T("C_ClrQst") )] = _T("C_ClrQst");
			mapPageList[pAttMng->GetPage( _T("C_Item") )] = _T("C_Item");
			mapPageList[pAttMng->GetPage( _T("C_Lvl") )] = _T("C_Lvl");
			mapPageList[pAttMng->GetPage( _T("C_PCCls") )] = _T("C_PCCls");
			mapPageList[pAttMng->GetPage( _T("C_PCRace") )] = _T("C_PCRace");
			mapPageList[pAttMng->GetPage( _T("C_QItem") )] = _T("C_QItem");
//			mapPageList[pAttMng->GetPage( _T("C_SSM") )] = _T("C_SSM");
			mapPageList[pAttMng->GetPage( _T("C_SToCEvt") )] = _T("C_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("C_WItem") )] = _T("C_WItem");
//			mapPageList[pAttMng->GetPage( _T("C_ObjItem") )] = _T("C_ObjItem");
			mapPageList[pAttMng->GetPage( _T("C_InNPC") )] = _T("C_InNPC");
			mapPageList[pAttMng->GetPage( _T("E_ItemUse") )] = _T("E_ItemUse");
			mapPageList[pAttMng->GetPage( _T("E_ItemGet") )] = _T("E_ItemGet");
			mapPageList[pAttMng->GetPage( _T("E_ItemEquip") )] = _T("E_ItemEquip");
			mapPageList[pAttMng->GetPage( _T("E_RcvSvrEvt") )] = _T("E_RcvSvrEvt");
			mapPageList[pAttMng->GetPage( _T("E_ScoutUse") )] = _T("E_ScoutUse");
//			mapPageList[pAttMng->GetPage( _T("C_CheckObjState") )] = _T("C_CheckObjState");
			mapPageList[pAttMng->GetPage( _T("C_CheckInWorld") )] = _T("C_CheckInWorld");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOPObject") )] = _T("C_CheckOPObject");
//			mapPageList[pAttMng->GetPage( _T("C_CheckHasCoupon") )] = _T("C_CheckHasCoupon");
//			mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
			mapPageList[pAttMng->GetPage( _T("E_SkillUse") )] = _T("E_SkillUse");
			mapPageList[pAttMng->GetPage( _T("E_ColRgn") )] = _T("E_ColRgn");
			mapPageList[pAttMng->GetPage( _T("C_CheckSToCDelivery") )] = _T("C_CheckSToCDelivery");
			mapPageList[pAttMng->GetPage( _T("E_RB") )] = _T("E_RB");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAttachObj") )] = _T("C_CheckAttachObj");
			mapPageList[pAttMng->GetPage( _T("C_CheckProgQuest") )] = _T("C_CheckProgQuest");
			mapPageList[pAttMng->GetPage( _T("C_CheckReputation") )] = _T("C_CheckReputation");
			mapPageList[pAttMng->GetPage( _T("C_CheckBudokaiState") )] = _T("C_CheckBudokaiState");
			mapPageList[pAttMng->GetPage( _T("E_ClickMob") )] = _T("E_ClickMob");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgCustomEvt") )] = _T("C_CheckProgCustomEvt");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
			mapPageList[pAttMng->GetPage( _T("E_BindStone") )] = _T("E_BindStone");
			mapPageList[pAttMng->GetPage( _T("E_SearchQuest") )] = _T("E_SearchQuest");
			mapPageList[pAttMng->GetPage( _T("E_ItemUpgrade") )] = _T("E_ItemUpgrade");
			mapPageList[pAttMng->GetPage( _T("E_Teleport") )] = _T("E_Teleport");
			mapPageList[pAttMng->GetPage( _T("E_Budokai") )] = _T("E_Budokai");
			mapPageList[pAttMng->GetPage( _T("E_SlotMachine") )] = _T("E_SlotMachine");
			mapPageList[pAttMng->GetPage( _T("E_HoipoiMix") )] = _T("E_HoipoiMix");
			mapPageList[pAttMng->GetPage( _T("E_PrivateShop") )] = _T("E_PrivateShop");
			mapPageList[pAttMng->GetPage( _T("E_FreeBattle") )] = _T("E_FreeBattle");
			mapPageList[pAttMng->GetPage( _T("E_ItemIdentity") )] = _T("E_ItemIdentity");
			mapPageList[pAttMng->GetPage( _T("E_UseMail") )] = _T("E_UseMail");
			mapPageList[pAttMng->GetPage( _T("E_Party") )] = _T("E_Party");

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
			mapPageList[pAttMng->GetPage( _T("E_ColObject") )] = _T("E_ColObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickObject") )] = _T("E_ClickObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickNPC") )] = _T("E_ClickNPC");
			mapPageList[pAttMng->GetPage( _T("C_ClrQst") )] = _T("C_ClrQst");
			mapPageList[pAttMng->GetPage( _T("C_Item") )] = _T("C_Item");
			mapPageList[pAttMng->GetPage( _T("C_Lvl") )] = _T("C_Lvl");
			mapPageList[pAttMng->GetPage( _T("C_PCCls") )] = _T("C_PCCls");
			mapPageList[pAttMng->GetPage( _T("C_PCRace") )] = _T("C_PCRace");
			mapPageList[pAttMng->GetPage( _T("C_QItem") )] = _T("C_QItem");
//			mapPageList[pAttMng->GetPage( _T("C_SSM") )] = _T("C_SSM");
			mapPageList[pAttMng->GetPage( _T("C_SToCEvt") )] = _T("C_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("C_WItem") )] = _T("C_WItem");
//			mapPageList[pAttMng->GetPage( _T("C_ObjItem") )] = _T("C_ObjItem");
			mapPageList[pAttMng->GetPage( _T("C_InNPC") )] = _T("C_InNPC");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUse") )] = _T("E_ItemUse");
//			mapPageList[pAttMng->GetPage( _T("E_ItemGet") )] = _T("E_ItemGet");
//			mapPageList[pAttMng->GetPage( _T("E_ItemEquip") )] = _T("E_ItemEquip");
//			mapPageList[pAttMng->GetPage( _T("E_RcvSvrEvt") )] = _T("E_RcvSvrEvt");
//			mapPageList[pAttMng->GetPage( _T("E_ScoutUse") )] = _T("E_ScoutUse");
//			mapPageList[pAttMng->GetPage( _T("C_CheckObjState") )] = _T("C_CheckObjState");
//			mapPageList[pAttMng->GetPage( _T("C_CheckInWorld") )] = _T("C_CheckInWorld");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOPObject") )] = _T("C_CheckOPObject");
//			mapPageList[pAttMng->GetPage( _T("C_CheckHasCoupon") )] = _T("C_CheckHasCoupon");
//			mapPageList[pAttMng->GetPage( _T("E_SkillUse") )] = _T("E_SkillUse");
//			mapPageList[pAttMng->GetPage( _T("E_ColRgn") )] = _T("E_ColRgn");
//			mapPageList[pAttMng->GetPage( _T("C_CheckSToCDelivery") )] = _T("C_CheckSToCDelivery");
//			mapPageList[pAttMng->GetPage( _T("E_RB") )] = _T("E_RB");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAttachObj") )] = _T("C_CheckAttachObj");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgQuest") )] = _T("C_CheckProgQuest");
//			mapPageList[pAttMng->GetPage( _T("C_CheckReputation") )] = _T("C_CheckReputation");
			mapPageList[pAttMng->GetPage( _T("C_CheckBudokaiState") )] = _T("C_CheckBudokaiState");
//			mapPageList[pAttMng->GetPage( _T("E_ClickMob") )] = _T("E_ClickMob");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgCustomEvt") )] = _T("C_CheckProgCustomEvt");
//			mapPageList[pAttMng->GetPage( _T("E_BindStone") )] = _T("E_BindStone");
//			mapPageList[pAttMng->GetPage( _T("E_SearchQuest") )] = _T("E_SearchQuest");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUpgrade") )] = _T("E_ItemUpgrade");
//			mapPageList[pAttMng->GetPage( _T("E_Teleport") )] = _T("E_Teleport");
//			mapPageList[pAttMng->GetPage( _T("E_Budokai") )] = _T("E_Budokai");
//			mapPageList[pAttMng->GetPage( _T("E_SlotMachine") )] = _T("E_SlotMachine");
//			mapPageList[pAttMng->GetPage( _T("E_HoipoiMix") )] = _T("E_HoipoiMix");
//			mapPageList[pAttMng->GetPage( _T("E_PrivateShop") )] = _T("E_PrivateShop");
//			mapPageList[pAttMng->GetPage( _T("E_FreeBattle") )] = _T("E_FreeBattle");
//			mapPageList[pAttMng->GetPage( _T("E_ItemIdentity") )] = _T("E_ItemIdentity");
//			mapPageList[pAttMng->GetPage( _T("E_UseMail") )] = _T("E_UseMail");
//			mapPageList[pAttMng->GetPage( _T("E_Party") )] = _T("E_Party");

			CTSGroup* pGroup = GetDoc()->GetSelGroup();
			if ( NTL_TS_EXCEPT_GIVEUP_ID == pGroup->tgID )
			{
				mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
				mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
				mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
				mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
			}
			else if ( NTL_TS_EXCEPT_TLIMT_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_TLIMT_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
				mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
				mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
				mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
			}
			else if ( NTL_TS_EXCEPT_SERVER_GROUP_ID_BEGIN <= pGroup->tgID && pGroup->tgID < NTL_TS_EXCEPT_SERVER_GROUP_ID_END )
			{
				mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
				mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
				mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
				mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
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
	else if ( TS_TYPE_PC_TRIGGER_CS == dwProjMode )
	{
		if ( eSEL_GROUP_TYPE_MAIN == eGroupType )
		{
			mapPageList[pAttMng->GetPage( _T("E_ColObject") )] = _T("E_ColObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickObject") )] = _T("E_ClickObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickNPC") )] = _T("E_ClickNPC");
			mapPageList[pAttMng->GetPage( _T("C_ClrQst") )] = _T("C_ClrQst");
			mapPageList[pAttMng->GetPage( _T("C_Item") )] = _T("C_Item");
			mapPageList[pAttMng->GetPage( _T("C_Lvl") )] = _T("C_Lvl");
			mapPageList[pAttMng->GetPage( _T("C_PCCls") )] = _T("C_PCCls");
			mapPageList[pAttMng->GetPage( _T("C_PCRace") )] = _T("C_PCRace");
			mapPageList[pAttMng->GetPage( _T("C_QItem") )] = _T("C_QItem");
//			mapPageList[pAttMng->GetPage( _T("C_SSM") )] = _T("C_SSM");
//			mapPageList[pAttMng->GetPage( _T("C_SToCEvt") )] = _T("C_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("C_WItem") )] = _T("C_WItem");
//			mapPageList[pAttMng->GetPage( _T("C_ObjItem") )] = _T("C_ObjItem");
//			mapPageList[pAttMng->GetPage( _T("C_InNPC") )] = _T("C_InNPC");
			mapPageList[pAttMng->GetPage( _T("E_ItemUse") )] = _T("E_ItemUse");
			mapPageList[pAttMng->GetPage( _T("E_ItemGet") )] = _T("E_ItemGet");
			mapPageList[pAttMng->GetPage( _T("E_ItemEquip") )] = _T("E_ItemEquip");
			mapPageList[pAttMng->GetPage( _T("E_RcvSvrEvt") )] = _T("E_RcvSvrEvt");
			mapPageList[pAttMng->GetPage( _T("E_ScoutUse") )] = _T("E_ScoutUse");
//			mapPageList[pAttMng->GetPage( _T("C_CheckObjState") )] = _T("C_CheckObjState");
			mapPageList[pAttMng->GetPage( _T("C_CheckInWorld") )] = _T("C_CheckInWorld");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOPObject") )] = _T("C_CheckOPObject");
//			mapPageList[pAttMng->GetPage( _T("C_CheckHasCoupon") )] = _T("C_CheckHasCoupon");
//			mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
			mapPageList[pAttMng->GetPage( _T("E_SkillUse") )] = _T("E_SkillUse");
			mapPageList[pAttMng->GetPage( _T("E_ColRgn") )] = _T("E_ColRgn");
//			mapPageList[pAttMng->GetPage( _T("C_CheckSToCDelivery") )] = _T("C_CheckSToCDelivery");
			mapPageList[pAttMng->GetPage( _T("E_RB") )] = _T("E_RB");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAttachObj") )] = _T("C_CheckAttachObj");
			mapPageList[pAttMng->GetPage( _T("C_CheckProgQuest") )] = _T("C_CheckProgQuest");
			mapPageList[pAttMng->GetPage( _T("C_CheckReputation") )] = _T("C_CheckReputation");
			mapPageList[pAttMng->GetPage( _T("C_CheckBudokaiState") )] = _T("C_CheckBudokaiState");
			mapPageList[pAttMng->GetPage( _T("E_ClickMob") )] = _T("E_ClickMob");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgCustomEvt") )] = _T("C_CheckProgCustomEvt");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
			mapPageList[pAttMng->GetPage( _T("E_BindStone") )] = _T("E_BindStone");
			mapPageList[pAttMng->GetPage( _T("E_SearchQuest") )] = _T("E_SearchQuest");
			mapPageList[pAttMng->GetPage( _T("E_ItemUpgrade") )] = _T("E_ItemUpgrade");
			mapPageList[pAttMng->GetPage( _T("E_Teleport") )] = _T("E_Teleport");
			mapPageList[pAttMng->GetPage( _T("E_Budokai") )] = _T("E_Budokai");
			mapPageList[pAttMng->GetPage( _T("E_SlotMachine") )] = _T("E_SlotMachine");
			mapPageList[pAttMng->GetPage( _T("E_HoipoiMix") )] = _T("E_HoipoiMix");
			mapPageList[pAttMng->GetPage( _T("E_PrivateShop") )] = _T("E_PrivateShop");
			mapPageList[pAttMng->GetPage( _T("E_FreeBattle") )] = _T("E_FreeBattle");
			mapPageList[pAttMng->GetPage( _T("E_ItemIdentity") )] = _T("E_ItemIdentity");
			mapPageList[pAttMng->GetPage( _T("E_UseMail") )] = _T("E_UseMail");
			mapPageList[pAttMng->GetPage( _T("E_Party") )] = _T("E_Party");

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
//			mapPageList[pAttMng->GetPage( _T("E_ColObject") )] = _T("E_ColObject");
//			mapPageList[pAttMng->GetPage( _T("E_ClickObject") )] = _T("E_ClickObject");
//			mapPageList[pAttMng->GetPage( _T("E_ClickNPC") )] = _T("E_ClickNPC");
//			mapPageList[pAttMng->GetPage( _T("C_ClrQst") )] = _T("C_ClrQst");
//			mapPageList[pAttMng->GetPage( _T("C_Item") )] = _T("C_Item");
//			mapPageList[pAttMng->GetPage( _T("C_Lvl") )] = _T("C_Lvl");
//			mapPageList[pAttMng->GetPage( _T("C_PCCls") )] = _T("C_PCCls");
//			mapPageList[pAttMng->GetPage( _T("C_PCRace") )] = _T("C_PCRace");
//			mapPageList[pAttMng->GetPage( _T("C_QItem") )] = _T("C_QItem");
//			mapPageList[pAttMng->GetPage( _T("C_SSM") )] = _T("C_SSM");
//			mapPageList[pAttMng->GetPage( _T("C_SToCEvt") )] = _T("C_SToCEvt");
//			mapPageList[pAttMng->GetPage( _T("C_WItem") )] = _T("C_WItem");
//			mapPageList[pAttMng->GetPage( _T("C_ObjItem") )] = _T("C_ObjItem");
//			mapPageList[pAttMng->GetPage( _T("C_InNPC") )] = _T("C_InNPC");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUse") )] = _T("E_ItemUse");
//			mapPageList[pAttMng->GetPage( _T("E_ItemGet") )] = _T("E_ItemGet");
//			mapPageList[pAttMng->GetPage( _T("E_ItemEquip") )] = _T("E_ItemEquip");
//			mapPageList[pAttMng->GetPage( _T("E_RcvSvrEvt") )] = _T("E_RcvSvrEvt");
//			mapPageList[pAttMng->GetPage( _T("E_ScoutUse") )] = _T("E_ScoutUse");
//			mapPageList[pAttMng->GetPage( _T("C_CheckObjState") )] = _T("C_CheckObjState");
//			mapPageList[pAttMng->GetPage( _T("C_CheckInWorld") )] = _T("C_CheckInWorld");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOPObject") )] = _T("C_CheckOPObject");
//			mapPageList[pAttMng->GetPage( _T("C_CheckHasCoupon") )] = _T("C_CheckHasCoupon");
//			mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
//			mapPageList[pAttMng->GetPage( _T("E_SkillUse") )] = _T("E_SkillUse");
//			mapPageList[pAttMng->GetPage( _T("E_ColRgn") )] = _T("E_ColRgn");
//			mapPageList[pAttMng->GetPage( _T("C_CheckSToCDelivery") )] = _T("C_CheckSToCDelivery");
//			mapPageList[pAttMng->GetPage( _T("E_RB") )] = _T("E_RB");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAttachObj") )] = _T("C_CheckAttachObj");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgQuest") )] = _T("C_CheckProgQuest");
//			mapPageList[pAttMng->GetPage( _T("C_CheckReputation") )] = _T("C_CheckReputation");
			mapPageList[pAttMng->GetPage( _T("C_CheckBudokaiState") )] = _T("C_CheckBudokaiState");
//			mapPageList[pAttMng->GetPage( _T("E_ClickMob") )] = _T("E_ClickMob");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgCustomEvt") )] = _T("C_CheckProgCustomEvt");
			mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
//			mapPageList[pAttMng->GetPage( _T("E_BindStone") )] = _T("E_BindStone");
//			mapPageList[pAttMng->GetPage( _T("E_SearchQuest") )] = _T("E_SearchQuest");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUpgrade") )] = _T("E_ItemUpgrade");
//			mapPageList[pAttMng->GetPage( _T("E_Teleport") )] = _T("E_Teleport");
//			mapPageList[pAttMng->GetPage( _T("E_Budokai") )] = _T("E_Budokai");
//			mapPageList[pAttMng->GetPage( _T("E_SlotMachine") )] = _T("E_SlotMachine");
//			mapPageList[pAttMng->GetPage( _T("E_HoipoiMix") )] = _T("E_HoipoiMix");
//			mapPageList[pAttMng->GetPage( _T("E_PrivateShop") )] = _T("E_PrivateShop");
//			mapPageList[pAttMng->GetPage( _T("E_FreeBattle") )] = _T("E_FreeBattle");
//			mapPageList[pAttMng->GetPage( _T("E_ItemIdentity") )] = _T("E_ItemIdentity");
//			mapPageList[pAttMng->GetPage( _T("E_UseMail") )] = _T("E_UseMail");
//			mapPageList[pAttMng->GetPage( _T("E_Party") )] = _T("E_Party");

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
	else if ( TS_TYPE_OBJECT_TRIGGER_S == dwProjMode )
	{
		if ( eSEL_GROUP_TYPE_MAIN == eGroupType )
		{
			mapPageList[pAttMng->GetPage( _T("E_ColObject") )] = _T("E_ColObject");
			mapPageList[pAttMng->GetPage( _T("E_ClickObject") )] = _T("E_ClickObject");
//			mapPageList[pAttMng->GetPage( _T("E_ClickNPC") )] = _T("E_ClickNPC");
			mapPageList[pAttMng->GetPage( _T("C_ClrQst") )] = _T("C_ClrQst");
			mapPageList[pAttMng->GetPage( _T("C_Item") )] = _T("C_Item");
			mapPageList[pAttMng->GetPage( _T("C_Lvl") )] = _T("C_Lvl");
			mapPageList[pAttMng->GetPage( _T("C_PCCls") )] = _T("C_PCCls");
			mapPageList[pAttMng->GetPage( _T("C_PCRace") )] = _T("C_PCRace");
			mapPageList[pAttMng->GetPage( _T("C_QItem") )] = _T("C_QItem");
//			mapPageList[pAttMng->GetPage( _T("C_SSM") )] = _T("C_SSM");
//			mapPageList[pAttMng->GetPage( _T("C_SToCEvt") )] = _T("C_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("C_WItem") )] = _T("C_WItem");
			mapPageList[pAttMng->GetPage( _T("C_ObjItem") )] = _T("C_ObjItem");
//			mapPageList[pAttMng->GetPage( _T("C_InNPC") )] = _T("C_InNPC");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUse") )] = _T("E_ItemUse");
//			mapPageList[pAttMng->GetPage( _T("E_ItemGet") )] = _T("E_ItemGet");
//			mapPageList[pAttMng->GetPage( _T("E_ItemEquip") )] = _T("E_ItemEquip");
//			mapPageList[pAttMng->GetPage( _T("E_RcvSvrEvt") )] = _T("E_RcvSvrEvt");
//			mapPageList[pAttMng->GetPage( _T("E_ScoutUse") )] = _T("E_ScoutUse");
			mapPageList[pAttMng->GetPage( _T("C_CheckObjState") )] = _T("C_CheckObjState");
			mapPageList[pAttMng->GetPage( _T("C_CheckInWorld") )] = _T("C_CheckInWorld");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOPObject") )] = _T("C_CheckOPObject");
			mapPageList[pAttMng->GetPage( _T("C_CheckHasCoupon") )] = _T("C_CheckHasCoupon");
//			mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
//			mapPageList[pAttMng->GetPage( _T("E_SkillUse") )] = _T("E_SkillUse");
//			mapPageList[pAttMng->GetPage( _T("E_ColRgn") )] = _T("E_ColRgn");
//			mapPageList[pAttMng->GetPage( _T("C_CheckSToCDelivery") )] = _T("C_CheckSToCDelivery");
//			mapPageList[pAttMng->GetPage( _T("E_RB") )] = _T("E_RB");
			mapPageList[pAttMng->GetPage( _T("C_CheckAttachObj") )] = _T("C_CheckAttachObj");
			mapPageList[pAttMng->GetPage( _T("C_CheckProgQuest") )] = _T("C_CheckProgQuest");
//			mapPageList[pAttMng->GetPage( _T("C_CheckReputation") )] = _T("C_CheckReputation");
//			mapPageList[pAttMng->GetPage( _T("C_CheckBudokaiState") )] = _T("C_CheckBudokaiState");
//			mapPageList[pAttMng->GetPage( _T("E_ClickMob") )] = _T("E_ClickMob");
			mapPageList[pAttMng->GetPage( _T("C_CheckProgCustomEvt") )] = _T("C_CheckProgCustomEvt");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
//			mapPageList[pAttMng->GetPage( _T("E_BindStone") )] = _T("E_BindStone");
//			mapPageList[pAttMng->GetPage( _T("E_SearchQuest") )] = _T("E_SearchQuest");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUpgrade") )] = _T("E_ItemUpgrade");
//			mapPageList[pAttMng->GetPage( _T("E_Teleport") )] = _T("E_Teleport");
//			mapPageList[pAttMng->GetPage( _T("E_Budokai") )] = _T("E_Budokai");
//			mapPageList[pAttMng->GetPage( _T("E_SlotMachine") )] = _T("E_SlotMachine");
//			mapPageList[pAttMng->GetPage( _T("E_HoipoiMix") )] = _T("E_HoipoiMix");
//			mapPageList[pAttMng->GetPage( _T("E_PrivateShop") )] = _T("E_PrivateShop");
//			mapPageList[pAttMng->GetPage( _T("E_FreeBattle") )] = _T("E_FreeBattle");
//			mapPageList[pAttMng->GetPage( _T("E_ItemIdentity") )] = _T("E_ItemIdentity");
//			mapPageList[pAttMng->GetPage( _T("E_UseMail") )] = _T("E_UseMail");
//			mapPageList[pAttMng->GetPage( _T("E_Party") )] = _T("E_Party");

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
//			mapPageList[pAttMng->GetPage( _T("E_ColObject") )] = _T("E_ColObject");
//			mapPageList[pAttMng->GetPage( _T("E_ClickObject") )] = _T("E_ClickObject");
//			mapPageList[pAttMng->GetPage( _T("E_ClickNPC") )] = _T("E_ClickNPC");
			mapPageList[pAttMng->GetPage( _T("C_ClrQst") )] = _T("C_ClrQst");
			mapPageList[pAttMng->GetPage( _T("C_Item") )] = _T("C_Item");
			mapPageList[pAttMng->GetPage( _T("C_Lvl") )] = _T("C_Lvl");
			mapPageList[pAttMng->GetPage( _T("C_PCCls") )] = _T("C_PCCls");
			mapPageList[pAttMng->GetPage( _T("C_PCRace") )] = _T("C_PCRace");
			mapPageList[pAttMng->GetPage( _T("C_QItem") )] = _T("C_QItem");
//			mapPageList[pAttMng->GetPage( _T("C_SSM") )] = _T("C_SSM");
//			mapPageList[pAttMng->GetPage( _T("C_SToCEvt") )] = _T("C_SToCEvt");
			mapPageList[pAttMng->GetPage( _T("C_WItem") )] = _T("C_WItem");
//			mapPageList[pAttMng->GetPage( _T("C_ObjItem") )] = _T("C_ObjItem");
//			mapPageList[pAttMng->GetPage( _T("C_InNPC") )] = _T("C_InNPC");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUse") )] = _T("E_ItemUse");
//			mapPageList[pAttMng->GetPage( _T("E_ItemGet") )] = _T("E_ItemGet");
//			mapPageList[pAttMng->GetPage( _T("E_ItemEquip") )] = _T("E_ItemEquip");
//			mapPageList[pAttMng->GetPage( _T("E_RcvSvrEvt") )] = _T("E_RcvSvrEvt");
//			mapPageList[pAttMng->GetPage( _T("E_ScoutUse") )] = _T("E_ScoutUse");
			mapPageList[pAttMng->GetPage( _T("C_CheckObjState") )] = _T("C_CheckObjState");
//			mapPageList[pAttMng->GetPage( _T("C_CheckInWorld") )] = _T("C_CheckInWorld");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOPObject") )] = _T("C_CheckOPObject");
//			mapPageList[pAttMng->GetPage( _T("C_CheckHasCoupon") )] = _T("C_CheckHasCoupon");
//			mapPageList[pAttMng->GetPage( _T("C_CheckNPCDead") )] = _T("C_CheckNPCDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckDistNPC") )] = _T("C_CheckDistNPC");
//			mapPageList[pAttMng->GetPage( _T("E_SkillUse") )] = _T("E_SkillUse");
//			mapPageList[pAttMng->GetPage( _T("E_ColRgn") )] = _T("E_ColRgn");
//			mapPageList[pAttMng->GetPage( _T("C_CheckSToCDelivery") )] = _T("C_CheckSToCDelivery");
//			mapPageList[pAttMng->GetPage( _T("E_RB") )] = _T("E_RB");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAttachObj") )] = _T("C_CheckAttachObj");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgQuest") )] = _T("C_CheckProgQuest");
//			mapPageList[pAttMng->GetPage( _T("C_CheckReputation") )] = _T("C_CheckReputation");
//			mapPageList[pAttMng->GetPage( _T("C_CheckBudokaiState") )] = _T("C_CheckBudokaiState");
//			mapPageList[pAttMng->GetPage( _T("E_ClickMob") )] = _T("E_ClickMob");
//			mapPageList[pAttMng->GetPage( _T("C_CheckProgCustomEvt") )] = _T("C_CheckProgCustomEvt");
//			mapPageList[pAttMng->GetPage( _T("C_CheckAvatarDead") )] = _T("C_CheckAvatarDead");
//			mapPageList[pAttMng->GetPage( _T("C_CheckOutRgn") )] = _T("C_CheckOutRgn");
//			mapPageList[pAttMng->GetPage( _T("E_BindStone") )] = _T("E_BindStone");
//			mapPageList[pAttMng->GetPage( _T("E_SearchQuest") )] = _T("E_SearchQuest");
//			mapPageList[pAttMng->GetPage( _T("E_ItemUpgrade") )] = _T("E_ItemUpgrade");
//			mapPageList[pAttMng->GetPage( _T("E_Teleport") )] = _T("E_Teleport");
//			mapPageList[pAttMng->GetPage( _T("E_Budokai") )] = _T("E_Budokai");
//			mapPageList[pAttMng->GetPage( _T("E_SlotMachine") )] = _T("E_SlotMachine");
//			mapPageList[pAttMng->GetPage( _T("E_HoipoiMix") )] = _T("E_HoipoiMix");
//			mapPageList[pAttMng->GetPage( _T("E_PrivateShop") )] = _T("E_PrivateShop");
//			mapPageList[pAttMng->GetPage( _T("E_FreeBattle") )] = _T("E_FreeBattle");
//			mapPageList[pAttMng->GetPage( _T("E_ItemIdentity") )] = _T("E_ItemIdentity");
//			mapPageList[pAttMng->GetPage( _T("E_UseMail") )] = _T("E_UseMail");
//			mapPageList[pAttMng->GetPage( _T("E_Party") )] = _T("E_Party");

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

void CShape_Start::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
	clProperty.m_defProperty["stdiag"] = GetAttr( _T("stdiag") );
}