#include "stdafx.h"
#include "RwdTblIdxChanger.h"
#include "Attr_Page.h"


CRwdTblIdxChanger::CRwdTblIdxChanger( CTSTrigger* pTSTrigger )
: CChanger( pTSTrigger )
{
}

CRwdTblIdxChanger::~CRwdTblIdxChanger( void )
{
}

CString CRwdTblIdxChanger::Trigger( NTL_TS_T_ID tID, CAttrData* pAttr )
{
	return pAttr->GetAttr();
}

CString CRwdTblIdxChanger::Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr )
{
	//////////////////////////////////////////////////////////////////////////
	// Container
	//////////////////////////////////////////////////////////////////////////
	if ( strType == _T("CT_Reward") )
	{
		CString strAttr = pAttr->GetAttr();

		CString strVal;

		if ( pAttr->FindAttr( _T("rwdtbl") ).IsEmpty() )
		{
			strVal.Format( _T("%d"), (unsigned int) tID * 100 + 1 );
			strVal = pAttr->MakeAttr( CString( _T("rwdtbl") ), strVal );

			strAttr += strVal;

			pAttr->SetAttr( strAttr );
		}
		else
		{
			strVal.Format( _T("%d"), (unsigned int) tID * 100 + 1 );

			pAttr->ChangeAttr( _T("rwdtbl"), strVal );
		}

		if ( pAttr->FindAttr( _T("usetbl") ).IsEmpty() )
		{
			strVal.Format( _T("%d"), 1 );
			strVal = pAttr->MakeAttr( CString( _T("usetbl") ), strVal );

			strAttr += strVal;

			pAttr->SetAttr( strAttr );
		}
		else
		{
			strVal.Format( _T("%d"), 1 );

			pAttr->ChangeAttr( _T("usetbl"), strVal );
		}
	}

	return pAttr->GetAttr();
}
