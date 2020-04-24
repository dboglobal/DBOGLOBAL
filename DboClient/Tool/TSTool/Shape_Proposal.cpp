#include "stdafx.h"
#include "Shape_Proposal.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_Proposal.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_Proposal, CShape_GEntityCont, 1 )


CShape_Proposal::CShape_Proposal( const CPoint &point )
: CShape_GEntityCont( point, _T("CT_Proposal") )
{
	AddAttr( _T("ds"), CString(_T("1")) );
	AddAttr( _T("title"), CString() );
	AddAttr( _T("area"), CString() );
	AddAttr( _T("goal"), CString() );
	AddAttr( _T("grade"), CString() );
	AddAttr( _T("gtype"), CString() );
	AddAttr( _T("sort"), CString() );
	AddAttr( _T("cont"), CString() );

	CString strTemp;
	strTemp.Format( _T("%d"), NTL_TS_TC_ID_INVALID );
	AddAttr( _T("rwd"), strTemp );
}

CShape_Proposal::~CShape_Proposal( void )
{
}

void CShape_Proposal::ShowContAttrEditBox( void )
{
	CAttr_CONT_Proposal AttrPage;

	AttrPage.AddAttr( _T("cid"), m_strContId );
	AttrPage.AddAttr( _T("ds"), GetAttr( _T("ds") ) );
	AttrPage.AddAttr( _T("title"), GetAttr( _T("title") ) );
	AttrPage.AddAttr( _T("area"), GetAttr( _T("area") ) );
	AttrPage.AddAttr( _T("goal"), GetAttr( _T("goal") ) );
	AttrPage.AddAttr( _T("grade"), GetAttr( _T("grade") ) );
	AttrPage.AddAttr( _T("gtype"), GetAttr( _T("gtype") ) );
	AttrPage.AddAttr( _T("sort"), GetAttr( _T("sort") ) );
	AttrPage.AddAttr( _T("cont"), GetAttr( _T("cont") ) );
	AttrPage.AddAttr( _T("rwd"), GetAttr( _T("rwd") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("ds") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("ds"), strValue );

		strValue = AttrPage.GetAttr( _T("title") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("title"), strValue );

		strValue = AttrPage.GetAttr( _T("area") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("area"), strValue );

		strValue = AttrPage.GetAttr( _T("goal") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("goal"), strValue );

		strValue = AttrPage.GetAttr( _T("grade") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("grade"), strValue );

		strValue = AttrPage.GetAttr( _T("gtype") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("gtype"), strValue );

		strValue = AttrPage.GetAttr( _T("sort") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("sort"), strValue );

		strValue = AttrPage.GetAttr( _T("cont") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("cont"), strValue );

		strValue = AttrPage.GetAttr( _T("rwd") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("rwd"), strValue );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_Proposal::ShowEntityAttrAddBox( void )
{
	AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
}

void CShape_Proposal::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
	clProperty.m_defProperty["ds"] = GetAttr( _T("ds") );
	clProperty.m_defProperty["title"] = GetAttr( _T("title") );
	clProperty.m_defProperty["area"] = GetAttr( _T("area") );
	clProperty.m_defProperty["goal"] = GetAttr( _T("goal") );
	clProperty.m_defProperty["grade"] = GetAttr( _T("grade") );
	clProperty.m_defProperty["gtype"] = GetAttr( _T("gtype") );
	clProperty.m_defProperty["sort"] = GetAttr( _T("sort") );
	clProperty.m_defProperty["cont"] = GetAttr( _T("cont") );
	clProperty.m_defProperty["rwd"] = GetAttr( _T("rwd") );
}


void CShape_Proposal::Serialize( CArchive &ar )
{
	CShape_GEntityCont::Serialize( ar );

	if ( !ar.IsStoring() )
	{
		if ( !HasAttr( _T("gtype") ) )
		{
			AddAttr( _T("gtype"), CString(_T("-1")) );
		}

		if ( !HasAttr( _T("ds") ) )
		{
			AddAttr( _T("ds"), CString(_T("1")) );
		}
	}
}
