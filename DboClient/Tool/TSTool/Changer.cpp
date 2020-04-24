#include "stdafx.h"
#include "TSTool.h"
#include "Changer.h"
#include "TSTrigger.h"
#include "Shape_GEntityCont.h"


bool CAttrData::HasAttr( CString strKey )
{
	int nStart = m_strAttr.Find( strKey );
	if ( nStart < 0 || nStart >= m_strAttr.GetLength() ) return false;

	nStart = m_strAttr.Find( '#', nStart );
	if ( nStart < 0 || nStart >= m_strAttr.GetLength() ) return false;

	return true;
}

CString CAttrData::FindAttr( CString strKey )
{
	CString strRet;

	int nStart = m_strAttr.Find( strKey );
	if ( nStart < 0 || nStart >= m_strAttr.GetLength() ) return CString();

	nStart = m_strAttr.Find( '#', nStart );
	if ( nStart < 0 || nStart >= m_strAttr.GetLength() ) return CString();

	strRet = m_strAttr.Tokenize( "#", nStart );
	strRet.Trim();

	return strRet;
}

bool CAttrData::ChangeAttr( CString strKey, CString strVal )
{
	if ( !HasAttr( strKey ) ) return false;

	CString strFindVal = FindAttr( strKey );

	CString strSource = MakeAttr( strKey, strFindVal );
	CString strDest = MakeAttr( strKey, strVal );

	m_strAttr.Replace( strSource, strDest );

	return true;
}

CString CAttrData::MakeAttr( CString& strKey, CString& strVal )
{
	CString strRet;
	strRet.Format( _T("%s # %s # "), strKey, strVal );
	return strRet;
}


CChanger::CChanger( CTSTrigger* pTSTrigger )
: m_pTSTrigger( pTSTrigger )
{
}

CChanger::~CChanger( void )
{
}

void CChanger::Run( void )
{
	if ( NULL == m_pTSTrigger ) return;

	NTL_TS_T_ID tID;
	CString strAttr;
	CTSGroup* pGroup;
	CAttrData clAttr;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Trigger
	//
	//////////////////////////////////////////////////////////////////////////

	tID = atoi( m_pTSTrigger->GetAttr( _T("tid") ).GetBuffer() );

	clAttr.SetAttr( m_pTSTrigger->GetAttr() );

	strAttr = Trigger( tID, &clAttr );
	if ( !strAttr.IsEmpty() )
	{
		m_pTSTrigger->SetAttr( strAttr );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Main group
	//
	//////////////////////////////////////////////////////////////////////////

	pGroup = m_pTSTrigger->GetMainGroup();

	if ( pGroup )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	Container
		//
		//////////////////////////////////////////////////////////////////////////

		ShapeList* pShapeList = pGroup->GetContainerLayer();

		for ( POSITION Pos = pShapeList->GetTailPosition(); Pos != NULL; )
		{
			CShape_GEntityCont* pShape = DYNAMIC_DOWNCAST( CShape_GEntityCont, pShapeList->GetPrev( Pos ) );

			if ( pShape )
			{
				strAttr = pShape->GetAttrString();
				clAttr.SetAttr( strAttr );

				strAttr = Entity( tID, pGroup->GetGroupID(), pShape->GetContID(), pShape->GetContType(), &clAttr );

				if ( !strAttr.IsEmpty() )
				{
					pShape->SetAttrString( strAttr );
				}

				//////////////////////////////////////////////////////////////////////////
				//
				//	Entities
				//
				//////////////////////////////////////////////////////////////////////////

				CArray< CShape_GEntityCont::sENTITY, const CShape_GEntityCont::sENTITY& >* parEntityList = pShape->GetEntityListPtr();

				int nEntitiesCnt = parEntityList->GetCount();
				for ( int i = 0; i < nEntitiesCnt; ++i )
				{
					strAttr = (*parEntityList)[i].strData;
					clAttr.SetAttr( strAttr );

					strAttr = Entity( tID, pGroup->GetGroupID(), pShape->GetContID(), (*parEntityList)[i].strType, &clAttr );

					if ( !strAttr.IsEmpty() )
					{
						(*parEntityList)[i].strData = strAttr;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Except group
	//
	//////////////////////////////////////////////////////////////////////////

	int nExcGroupCnt = m_pTSTrigger->GetExceptionGroupCnt();

	for ( int i = 0; i < nExcGroupCnt; ++i )
	{
		pGroup = m_pTSTrigger->GetExceptionGroup( i );

		if ( pGroup )
		{
			//////////////////////////////////////////////////////////////////////////
			//
			//	Container
			//
			//////////////////////////////////////////////////////////////////////////

			ShapeList* pShapeList = pGroup->GetContainerLayer();

			for ( POSITION Pos = pShapeList->GetTailPosition(); Pos != NULL; )
			{
				CShape_GEntityCont* pShape = DYNAMIC_DOWNCAST( CShape_GEntityCont, pShapeList->GetPrev( Pos ) );

				if ( pShape )
				{
					strAttr = pShape->GetAttrString();
					clAttr.SetAttr( strAttr );

					strAttr = Entity( tID, pGroup->GetGroupID(), pShape->GetContID(), pShape->GetContType(), &clAttr );

					if ( !strAttr.IsEmpty() )
					{
						pShape->SetAttrString( strAttr );
					}

					//////////////////////////////////////////////////////////////////////////
					//
					//	Entities
					//
					//////////////////////////////////////////////////////////////////////////

					CArray< CShape_GEntityCont::sENTITY, const CShape_GEntityCont::sENTITY& >* parEntityList = pShape->GetEntityListPtr();

					int nEntitiesCnt = parEntityList->GetCount();
					for ( int i = 0; i < nEntitiesCnt; ++i )
					{
						strAttr = (*parEntityList)[i].strData;
						clAttr.SetAttr( strAttr );

						strAttr = Entity( tID, pGroup->GetGroupID(), pShape->GetContID(), (*parEntityList)[i].strType, &clAttr );

						if ( !strAttr.IsEmpty() )
						{
							(*parEntityList)[i].strData = strAttr;
						}
					}
				}
			}
		}
	}
}

CString CChanger::Trigger( NTL_TS_T_ID tID, CAttrData* pAttr )
{
	return CString();
}

CString CChanger::Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr )
{
	return CString();
}
