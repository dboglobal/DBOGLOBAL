#include "stdafx.h"
#include "Finder.h"
#include "Attr_Page.h"


CFinder::CFinder( CTSTrigger* pTSTrigger )
: CChanger( pTSTrigger )
{
}

CFinder::~CFinder( void )
{
}

CString CFinder::Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr )
{
	mapdef_FindList::iterator it = m_defFindList.find( strType );

	if ( it != m_defFindList.end() )
	{
		sFIND_ENTITY sFindEntity;

		sFindEntity.tId = tID;
		sFindEntity.tgId = tgID;
		sFindEntity.tcId = tcID;

		CString strTAId = pAttr->FindAttr( _T("taid") );
		sFindEntity.taId = strTAId.IsEmpty() ? NTL_TS_TA_ID_INVALID : atoi( strTAId.GetBuffer() );
		sFindEntity.strType = strType;
		sFindEntity.strAttr = pAttr->GetAttr();

		m_defFindResultList.push_back( sFindEntity );
	}

	return pAttr->GetAttr();
}

void CFinder::AddFindEntity( CString& strType )
{
	m_defFindList[strType] = strType;
}
