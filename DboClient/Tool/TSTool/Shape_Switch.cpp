#include "stdafx.h"
#include "Shape_Switch.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_Switch.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_Switch, CShape_GEntityCont, 1 )


CShape_Switch::CShape_Switch( const CPoint &point )
: CShape_GEntityCont( point, _T("CT_Switch") )
{
}

CShape_Switch::~CShape_Switch( void )
{
}

void CShape_Switch::ShowContAttrEditBox( void )
{
	CAttr_CONT_Switch AttrPage;
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

void CShape_Switch::ShowEntityAttrAddBox( void )
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
			mapPageList[pAttMng->GetPage( _T("A_SWProbSF") )] = _T("A_SWProbSF");

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
			mapPageList[pAttMng->GetPage( _T("A_SWProbSF") )] = _T("A_SWProbSF");

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
			mapPageList[pAttMng->GetPage( _T("A_SWProbSF") )] = _T("A_SWProbSF");

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
}

void CShape_Switch::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
}
