#include "stdafx.h"
#include "Shape_UsrSel.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_UserSel.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_UsrSel, CShape_GEntityCont, 1 )


CShape_UsrSel::CShape_UsrSel( const CPoint& point )
: CShape_GEntityCont( point, _T("CT_UsrSel") )
{
	CString strInvalid;
	strInvalid.Format( _T("%d"), 0xffffffff );

	CString strGeneral;
	strGeneral.Format( _T("%d"), eUSER_SEL_TYPE_GENERAL );

	AddAttr( _T("ust"), strGeneral );
	AddAttr( _T("uspt"), strInvalid );
	AddAttr( _T("type"), strInvalid );
	AddAttr( _T("idx"), strInvalid );
	AddAttr( _T("desc"), strInvalid );
}

CShape_UsrSel::~CShape_UsrSel( void )
{
}

void CShape_UsrSel::ShowContAttrEditBox( void )
{
	CAttr_CONT_UserSel AttrPage;
	AttrPage.AddAttr( _T("cid"), m_strContId );
	AttrPage.AddAttr( _T("ust"), GetAttr( _T("ust") ) );
	AttrPage.AddAttr( _T("uspt"), GetAttr( _T("uspt") ) );
	AttrPage.AddAttr( _T("type"), GetAttr( _T("type") ) );
	AttrPage.AddAttr( _T("idx"), GetAttr( _T("idx") ) );
	AttrPage.AddAttr( _T("desc"), GetAttr( _T("desc") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("ust") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("ust"), strValue );

		strValue = AttrPage.GetAttr( _T("uspt") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("uspt"), strValue );

		strValue = AttrPage.GetAttr( _T("type") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("type"), strValue );

		strValue = AttrPage.GetAttr( _T("idx") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("idx"), strValue );

		strValue = AttrPage.GetAttr( _T("desc") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("desc"), strValue );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_UsrSel::ShowEntityAttrAddBox( void )
{
	AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
}

void CShape_UsrSel::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
	clProperty.m_defProperty["ust"] = GetAttr( _T("ust") );
	clProperty.m_defProperty["uspt"] = GetAttr( _T("uspt") );
	clProperty.m_defProperty["type"] = GetAttr( _T("type") );
	clProperty.m_defProperty["idx"] = GetAttr( _T("idx") );
	clProperty.m_defProperty["desc"] = GetAttr( _T("desc") );
}
