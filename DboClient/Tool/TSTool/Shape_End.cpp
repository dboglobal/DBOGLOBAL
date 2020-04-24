#include "stdafx.h"
#include "Shape_End.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_End.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_End, CShape_GEntityCont, 1 )


CShape_End::CShape_End( const CPoint &point )
: CShape_GEntityCont( point, _T("CT_End") )
{
	CString strTemp;

	strTemp.Format( _T("%d"), eEND_TYPE_NOT_PROGRESS );
	AddAttr( _T("type"), strTemp );
}

CShape_End::~CShape_End( void )
{
}

void CShape_End::ShowContAttrEditBox( void )
{
	CAttr_CONT_End AttrPage;
	AttrPage.AddAttr( _T("cid"), m_strContId );
	AttrPage.AddAttr( _T("type"), GetAttr( _T("type") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("type") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("type"), strValue );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_End::ShowEntityAttrAddBox( void )
{
	AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
}

void CShape_End::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
	clProperty.m_defProperty["type"] = GetAttr( _T("type") );
}
