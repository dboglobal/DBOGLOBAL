#include "stdafx.h"
#include "Shape_Reward.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_Reward.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_Reward, CShape_GEntityCont, 1 )


CShape_Reward::CShape_Reward( const CPoint& point )
: CShape_GEntityCont( point, _T("CT_Reward") )
{
	CString strRwdContType; strRwdContType.Format( _T("%d"), eREWARD_CONTAINER_TYPE_QUEST );
	AddAttr( _T("rwdtype"), strRwdContType );
	AddAttr( _T("ltime"), CString(_T("-1")) );
	AddAttr( _T("desc"), CString() );

	AddAttr( _T("usetbl"), CString(_T("0")) );
	AddAttr( _T("rwdtbl"), CString(_T("-1")) );
}

CShape_Reward::~CShape_Reward( void )
{
}

void CShape_Reward::ShowContAttrEditBox( void )
{
	CAttr_CONT_Reward AttrPage;
	AttrPage.AddAttr( _T("cid"), m_strContId );
	AttrPage.AddAttr( _T("rwdtype"), GetAttr( _T("rwdtype") ) );
	AttrPage.AddAttr( _T("ltime"), GetAttr( _T("ltime") ) );
	AttrPage.AddAttr( _T("desc"), GetAttr( _T("desc") ) );

	AttrPage.AddAttr( _T("usetbl"), GetAttr(_T("usetbl")) );
	AttrPage.AddAttr( _T("rwdtbl"), GetAttr(_T("rwdtbl")) );

	if ( GetAttr( _T("d0") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("d0") ) );
	if ( GetAttr( _T("d1") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("d1") ) );
	if ( GetAttr( _T("d2") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("d2") ) );
	if ( GetAttr( _T("d3") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("d3") ) );

	if ( GetAttr( _T("s0") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("s0") ) );
	if ( GetAttr( _T("s1") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("s1") ) );
	if ( GetAttr( _T("s2") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("s2") ) );
	if ( GetAttr( _T("s3") ).GetLength() > 0 ) AttrPage.AddAttr( GetAttr( _T("s3") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("rwdtype") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("rwdtype"), strValue );

		strValue = AttrPage.GetAttr( _T("ltime") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("ltime"), strValue );

		strValue = AttrPage.GetAttr( _T("desc") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("desc"), strValue );

		strValue = AttrPage.GetAttr( _T("usetbl") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("usetbl"), strValue );

		strValue = AttrPage.GetAttr( _T("rwdtbl") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("rwdtbl"), strValue );

		if ( AttrPage.GetAttr( _T("dtype0") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("dtype0"), AttrPage.GetAttr( _T("dtype0") ) );
			AttrPage.PakingPageData( strData, _T("didx0"), AttrPage.GetAttr( _T("didx0") ) );
			AttrPage.PakingPageData( strData, _T("dval0"), AttrPage.GetAttr( _T("dval0") ) );

			if ( HasAttr( _T("d0") ) )	SetAttr( _T("d0"), strData );
			else						AddAttr( _T("d0"), strData );
		}
		else RemoveAttr( _T("d0") );

		if ( AttrPage.GetAttr( _T("dtype1") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("dtype1"), AttrPage.GetAttr( _T("dtype1") ) );
			AttrPage.PakingPageData( strData, _T("didx1"), AttrPage.GetAttr( _T("didx1") ) );
			AttrPage.PakingPageData( strData, _T("dval1"), AttrPage.GetAttr( _T("dval1") ) );

			if ( HasAttr( _T("d1") ) )	SetAttr( _T("d1"), strData );
			else						AddAttr( _T("d1"), strData );
		}
		else RemoveAttr( _T("d1") );

		if ( AttrPage.GetAttr( _T("dtype2") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("dtype2"), AttrPage.GetAttr( _T("dtype2") ) );
			AttrPage.PakingPageData( strData, _T("didx2"), AttrPage.GetAttr( _T("didx2") ) );
			AttrPage.PakingPageData( strData, _T("dval2"), AttrPage.GetAttr( _T("dval2") ) );

			if ( HasAttr( _T("d2") ) )	SetAttr( _T("d2"), strData );
			else						AddAttr( _T("d2"), strData );
		}
		else RemoveAttr( _T("d2") );

		if ( AttrPage.GetAttr( _T("dtype3") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("dtype3"), AttrPage.GetAttr( _T("dtype3") ) );
			AttrPage.PakingPageData( strData, _T("didx3"), AttrPage.GetAttr( _T("didx3") ) );
			AttrPage.PakingPageData( strData, _T("dval3"), AttrPage.GetAttr( _T("dval3") ) );

			if ( HasAttr( _T("d3") ) )	SetAttr( _T("d3"), strData );
			else						AddAttr( _T("d3"), strData );
		}
		else RemoveAttr( _T("d3") );

		if ( AttrPage.GetAttr( _T("stype0") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("stype0"), AttrPage.GetAttr( _T("stype0") ) );
			AttrPage.PakingPageData( strData, _T("sidx0"), AttrPage.GetAttr( _T("sidx0") ) );
			AttrPage.PakingPageData( strData, _T("sval0"), AttrPage.GetAttr( _T("sval0") ) );

			if ( HasAttr( _T("s0") ) )	SetAttr( _T("s0"), strData );
			else						AddAttr( _T("s0"), strData );
		}
		else RemoveAttr( _T("s0") );

		if ( AttrPage.GetAttr( _T("stype1") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("stype1"), AttrPage.GetAttr( _T("stype1") ) );
			AttrPage.PakingPageData( strData, _T("sidx1"), AttrPage.GetAttr( _T("sidx1") ) );
			AttrPage.PakingPageData( strData, _T("sval1"), AttrPage.GetAttr( _T("sval1") ) );

			if ( HasAttr( _T("s1") ) )	SetAttr( _T("s1"), strData );
			else						AddAttr( _T("s1"), strData );
		}
		else RemoveAttr( _T("s1") );

		if ( AttrPage.GetAttr( _T("stype2") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("stype2"), AttrPage.GetAttr( _T("stype2") ) );
			AttrPage.PakingPageData( strData, _T("sidx2"), AttrPage.GetAttr( _T("sidx2") ) );
			AttrPage.PakingPageData( strData, _T("sval2"), AttrPage.GetAttr( _T("sval2") ) );

			if ( HasAttr( _T("s2") ) )	SetAttr( _T("s2"), strData );
			else						AddAttr( _T("s2"), strData );
		}
		else RemoveAttr( _T("s2") );

		if ( AttrPage.GetAttr( _T("stype3") ).GetLength() > 0 )
		{
			CString strData;
			AttrPage.PakingPageData( strData, _T("stype3"), AttrPage.GetAttr( _T("stype3") ) );
			AttrPage.PakingPageData( strData, _T("sidx3"), AttrPage.GetAttr( _T("sidx3") ) );
			AttrPage.PakingPageData( strData, _T("sval3"), AttrPage.GetAttr( _T("sval3") ) );

			if ( HasAttr( _T("s3") ) )	SetAttr( _T("s3"), strData );
			else						AddAttr( _T("s3"), strData );
		}
		else RemoveAttr( _T("s3") );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_Reward::ShowEntityAttrAddBox( void )
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
			AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
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
			AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
		}
	}
	else if ( TS_TYPE_OBJECT_TRIGGER_S == dwProjMode )
	{
		AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
	}
}

void CShape_Reward::SetAttrString( const CString& strString )
{
	m_AttrList.RemoveAll();

	enum { eTOKEN_TYPE_KEY, eTOKEN_TYPE_VALUE, eTOKEN_TYPE_SUB_KEY, eTOKEN_TYPE_SUB_VALUE };

	CString strKey, strValue;
	CString strSubKey, strSubValue;

	CString strToken;
	int nStart = 0;
	bool bLoop = true;

	int nTokType = eTOKEN_TYPE_KEY;

	while ( bLoop )
	{
		strToken = strString.Tokenize( _T("#"), nStart );
		if ( nStart >= strString.GetLength() ) bLoop = false;

		switch ( nTokType )
		{
		case eTOKEN_TYPE_KEY:
			{
				strKey = strToken.Trim();
				strValue.Empty();

				if ( strKey == _T("d0") || strKey == _T("d1") || strKey == _T("d2") || strKey == _T("d3") ||
					 strKey == _T("s0") || strKey == _T("s1") || strKey == _T("s2") || strKey == _T("s3") )
				{
					nTokType = eTOKEN_TYPE_SUB_KEY;
				}
				else
				{
					nTokType = eTOKEN_TYPE_VALUE;
				}
			}
			break;

		case eTOKEN_TYPE_VALUE:
			{
				strValue = strToken.Trim();

				{
					int nIndex = (int)m_AttrList.GetSize();

					CShape_GEntityCont::sATTR newAttr;
					newAttr.strName = strKey;
					newAttr.strValue = strValue;

					m_AttrList.InsertAt( nIndex, newAttr );
				}

				strKey.Empty();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_KEY;
			}
			break;

		case eTOKEN_TYPE_SUB_KEY:
			{
				strSubKey = strToken.Trim();

				if ( strSubKey.IsEmpty() )
				{
					int nIndex = (int)m_AttrList.GetSize();

					CShape_GEntityCont::sATTR newAttr;
					newAttr.strName = strKey;
					newAttr.strValue = strValue;

					m_AttrList.InsertAt( nIndex, newAttr );

					strKey.Empty();
					strValue.Empty();

					nTokType = eTOKEN_TYPE_KEY;
				}
				else
				{
					nTokType = eTOKEN_TYPE_SUB_VALUE;
				}
			}
			break;

		case eTOKEN_TYPE_SUB_VALUE:
			{
				strSubValue = strToken.Trim();

				CString strSum;
				strSum.Format( _T("%s # %s # "), strSubKey, strSubValue );

				strValue += strSum;

				strSubKey.Empty();
				strSubValue.Empty();

				nTokType = eTOKEN_TYPE_SUB_KEY;
			}
			break;
		}
	}

	UpdateHeightInfo();
	GetDoc()->OnShapeChange( this );
}

void CShape_Reward::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
	clProperty.m_defProperty["rwdtype"] = GetAttr( _T("rwdtype") );
	clProperty.m_defProperty["ltime"] = GetAttr( _T("ltime") );
	clProperty.m_defProperty["desc"] = GetAttr( _T("desc") );

	clProperty.m_defProperty["usetbl"] = GetAttr( _T("usetbl") );
	clProperty.m_defProperty["rwdtbl"] = GetAttr( _T("rwdtbl") );

	if ( HasAttr( _T("d0") ) )
	{
		CString strData = GetAttr( _T("d0" ) );
		clProperty.m_defProperty["dtype0"] = GetPropertyInfo( strData, _T("dtype0") );
		clProperty.m_defProperty["didx0"] = GetPropertyInfo( strData, _T("didx0") );
		clProperty.m_defProperty["dval0"] = GetPropertyInfo( strData, _T("dval0") );
	}
	if ( HasAttr( _T("d1") ) )
	{
		CString strData = GetAttr( _T("d1" ) );
		clProperty.m_defProperty["dtype1"] = GetPropertyInfo( strData, _T("dtype1") );
		clProperty.m_defProperty["didx1"] = GetPropertyInfo( strData, _T("didx1") );
		clProperty.m_defProperty["dval1"] = GetPropertyInfo( strData, _T("dval1") );
	}
	if ( HasAttr( _T("d2") ) )
	{
		CString strData = GetAttr( _T("d2" ) );
		clProperty.m_defProperty["dtype2"] = GetPropertyInfo( strData, _T("dtype2") );
		clProperty.m_defProperty["didx2"] = GetPropertyInfo( strData, _T("didx2") );
		clProperty.m_defProperty["dval2"] = GetPropertyInfo( strData, _T("dval2") );
	}
	if ( HasAttr( _T("d3") ) )
	{
		CString strData = GetAttr( _T("d3" ) );
		clProperty.m_defProperty["dtype3"] = GetPropertyInfo( strData, _T("dtype3") );
		clProperty.m_defProperty["didx3"] = GetPropertyInfo( strData, _T("didx3") );
		clProperty.m_defProperty["dval3"] = GetPropertyInfo( strData, _T("dval3") );
	}

	if ( HasAttr( _T("s0") ) )
	{
		CString strData = GetAttr( _T("s0" ) );
		clProperty.m_defProperty["stype0"] = GetPropertyInfo( strData, _T("stype0") );
		clProperty.m_defProperty["sidx0"] = GetPropertyInfo( strData, _T("sidx0") );
		clProperty.m_defProperty["sval0"] = GetPropertyInfo( strData, _T("sval0") );
	}
	if ( HasAttr( _T("s1") ) )
	{
		CString strData = GetAttr( _T("s1" ) );
		clProperty.m_defProperty["stype1"] = GetPropertyInfo( strData, _T("stype1") );
		clProperty.m_defProperty["sidx1"] = GetPropertyInfo( strData, _T("sidx1") );
		clProperty.m_defProperty["sval1"] = GetPropertyInfo( strData, _T("sval1") );
	}
	if ( HasAttr( _T("s2") ) )
	{
		CString strData = GetAttr( _T("s2" ) );
		clProperty.m_defProperty["stype2"] = GetPropertyInfo( strData, _T("stype2") );
		clProperty.m_defProperty["sidx2"] = GetPropertyInfo( strData, _T("sidx2") );
		clProperty.m_defProperty["sval2"] = GetPropertyInfo( strData, _T("sval2") );
	}
	if ( HasAttr( _T("s3") ) )
	{
		CString strData = GetAttr( _T("s3" ) );
		clProperty.m_defProperty["stype3"] = GetPropertyInfo( strData, _T("stype3") );
		clProperty.m_defProperty["sidx3"] = GetPropertyInfo( strData, _T("sidx3") );
		clProperty.m_defProperty["sval3"] = GetPropertyInfo( strData, _T("sval3") );
	}
}

CString CShape_Reward::GetPropertyInfo( CString& strData, CString strName )
{
	CString strRet;

	int nStart = strData.Find( strName );
	if ( nStart < 0 || nStart >= strData.GetLength() ) return CString();

	nStart = strData.Find( '#', nStart );
	if ( nStart < 0 || nStart >= strData.GetLength() ) return CString();

	strRet = strData.Tokenize( "#", nStart );
	strRet.Trim();

	return strRet;
}
