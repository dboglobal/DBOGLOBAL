#include "stdafx.h"
#include "Shape_Narration.h"
#include "resource.h"
#include "TSDoc.h"
#include "Attr_CONT_Narration.h"
#include "EntityRegisterDiag.h"
#include "AttrEntityMng.h"
#include "OptionListBox.h"


IMPLEMENT_SERIAL( CShape_Narration, CShape_GEntityCont, 1 )


CShape_Narration::CShape_Narration( const CPoint &point )
: CShape_GEntityCont( point, _T("CT_Narration") )
{
	CString strTemp;

	strTemp.Format( _T("%d"), eNARRATION_PROGRESS_STATE_START );
	AddAttr( _T("pt"), strTemp );

	strTemp.Format( _T("%d"), eNARRATION_OWNER_NPC );
	AddAttr( _T("ot"), strTemp );

	strTemp.Format( _T("%d"), 0xffffffff );
	AddAttr( _T("oi"), strTemp );

	strTemp.Format( _T("%d"), eNARRATION_OWNER_STATE_1 );
	AddAttr( _T("os"), strTemp );

	strTemp.Format( _T("%d"), eNARRATION_DIALOG_DIR_TYPE_NORMAL );
	AddAttr( _T("dt"), strTemp );

	strTemp.Format( _T("%d"), 0xffffffff );
	AddAttr( _T("dg"), strTemp );

	strTemp.Format( _T("%d"), eNARRATION_GUI_TYPE_NORMAL );
	AddAttr( _T("gt"), strTemp );

	strTemp.Format( _T("%d"), 0xffffffff );
	AddAttr( _T("mlt"), strTemp );
}

CShape_Narration::~CShape_Narration( void )
{
}

void CShape_Narration::ShowContAttrEditBox( void )
{
	CAttr_CONT_Narration AttrPage;
	AttrPage.AddAttr( _T("cid"), m_strContId );

	AttrPage.AddAttr( _T("pt"), GetAttr( _T("pt") ) );
	AttrPage.AddAttr( _T("ot"), GetAttr( _T("ot") ) );
	AttrPage.AddAttr( _T("oi"), GetAttr( _T("oi") ) );
	AttrPage.AddAttr( _T("os"), GetAttr( _T("os") ) );
	AttrPage.AddAttr( _T("dt"), GetAttr( _T("dt") ) );
	AttrPage.AddAttr( _T("dg"), GetAttr( _T("dg") ) );
	AttrPage.AddAttr( _T("gt"), GetAttr( _T("gt") ) );
	AttrPage.AddAttr( _T("mlt"), GetAttr( _T("mlt") ) );

	COptionListBox clListBox;
	CEntityRegisterDiag clDiag;
	clDiag.SetListControl( &clListBox );

	clDiag.AddAttrPage( &AttrPage );

	if ( IDOK == clDiag.DoModal() && clDiag.GetEditedAttrPage() )
	{
		CString strValue;

		strValue = AttrPage.GetAttr( _T("cid") );
		if ( strValue.GetLength() != 0 ) m_strContId = strValue;

		strValue = AttrPage.GetAttr( _T("pt") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("pt"), strValue );

		strValue = AttrPage.GetAttr( _T("ot") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("ot"), strValue );

		strValue = AttrPage.GetAttr( _T("oi") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("oi"), strValue );

		strValue = AttrPage.GetAttr( _T("os") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("os"), strValue );

		strValue = AttrPage.GetAttr( _T("dt") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("dt"), strValue );

		strValue = AttrPage.GetAttr( _T("dg") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("dg"), strValue );

		strValue = AttrPage.GetAttr( _T("gt") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("gt"), strValue );

		strValue = AttrPage.GetAttr( _T("mlt") );
		if ( strValue.GetLength() != 0 ) SetAttr( _T("mlt"), strValue );

		UpdateHeightInfo();
		GetDoc()->OnShapeChange( this );
	}
}

void CShape_Narration::ShowEntityAttrAddBox( void )
{
	AfxMessageBox( _T("등록 시킬 수 있는 엔티티들이 존재하지 않습니다") );
}

void CShape_Narration::GetProperty( CNtlTSScrProperty& clProperty )
{
	clProperty.m_defProperty["cid"] = m_strContId;

	clProperty.m_defProperty["pt"] = GetAttr( _T("pt") );
	clProperty.m_defProperty["ot"] = GetAttr( _T("ot") );
	clProperty.m_defProperty["oi"] = GetAttr( _T("oi") );
	clProperty.m_defProperty["os"] = GetAttr( _T("os") );
	clProperty.m_defProperty["dt"] = GetAttr( _T("dt") );
	clProperty.m_defProperty["dg"] = GetAttr( _T("dg") );
	clProperty.m_defProperty["gt"] = GetAttr( _T("gt") );
	clProperty.m_defProperty["mlt"] = GetAttr( _T("mlt") );
}
