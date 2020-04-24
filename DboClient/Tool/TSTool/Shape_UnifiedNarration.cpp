#include "stdafx.h"
#include "Shape_UnifiedNarration.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_UnifiedNarration.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_UnifiedNarration, CShape_GEntityCont, 1 )


CShape_UnifiedNarration::CShape_UnifiedNarration( const CPoint &point )
: CShape_GEntityCont( point, _T("CT_UnifiedNarration") )
{
	CString strData;

	strData.Format( _T("-1") );
	AddAttr( _T("idx"), strData );
}

CShape_UnifiedNarration::~CShape_UnifiedNarration( void )
{
}

void CShape_UnifiedNarration::ShowContAttrEditBox( void )
{
	CAttr_CONT_UnifiedNarration AttrPage;
	AttrPage.AddAttr( _T("cid"), m_strContId );
	AttrPage.AddAttr( _T("idx"), GetAttr( _T("idx") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("idx") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("idx"), strValue );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_UnifiedNarration::ShowEntityAttrAddBox( void )
{
	AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
}

void CShape_UnifiedNarration::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;
	clProperty.m_defProperty["idx"] = GetAttr( _T("idx") );
}
