#include "stdafx.h"
#include "TSTool.h"
#include "TSTrigger.h"
#include "TSProjectMainMenuForm.h"



/** 
	CTSGroup
*/

IMPLEMENT_SERIAL( CTSGroup, CObject, 1 )


CTSGroup::CTSGroup( void )
{
	tgID = NTL_TS_TG_ID_INVALID;
}

CTSGroup::~CTSGroup( void )
{
	RemoveAll();
}

void CTSGroup::RemoveAll( void )
{
	POSITION Pos;

	for ( Pos = LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = LinkLayer.GetPrev( Pos );
		delete pShape;
	}
	LinkLayer.RemoveAll();

	for ( Pos = ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = ContainerLayer.GetPrev( Pos );
		delete pShape;
	}
	ContainerLayer.RemoveAll();

	for ( Pos = NoteLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = NoteLayer.GetPrev( Pos );
		delete pShape;
	}
	NoteLayer.RemoveAll();

	tgID = NTL_TS_TG_ID_INVALID;
}

NTL_TS_TG_ID CTSGroup::GetGroupID( void )
{
	return tgID;
}

ShapeList* CTSGroup::GetContainerLayer( void )
{
	return &ContainerLayer;
}

void CTSGroup::Serialize( CArchive& ar )
{
	LinkLayer.Serialize( ar );
	ContainerLayer.Serialize( ar );
	NoteLayer.Serialize( ar );
}


/** 
	CTSTrigger
*/


IMPLEMENT_SERIAL( CTSTrigger, CObject, 1 )


CTSTrigger::CTSTrigger( void )
: m_dwVersion( eTS_VERSION )
{
	m_pMainGroup = NULL;
}

CTSTrigger::~CTSTrigger( void )
{
	ClearAll();
}

CString CTSTrigger::GetAttr( CString strName )
{
	CString strRet;

	int nStart = m_strAttr.Find( strName );
	if ( nStart < 0 || nStart >= m_strAttr.GetLength() ) return CString();

	nStart = m_strAttr.Find( '#', nStart );
	if ( nStart < 0 || nStart >= m_strAttr.GetLength() ) return CString();

	strRet = m_strAttr.Tokenize( "#", nStart );
	strRet.Trim();

	return strRet;
}

void CTSTrigger::SetAttr( CString& strData )
{
	m_strAttr = strData;
}

void CTSTrigger::AddAttr( CString strKey, CString strValue )
{
	m_strAttr += PakingPageData( strKey, strValue );
}

void CTSTrigger::AddAttr( CString strKey, int nValue )
{
	m_strAttr += PakingPageData( strKey, nValue );
}

void CTSTrigger::ClearAttr( void )
{
	m_strAttr.Empty();
}

int CTSTrigger::GetExceptionGroupCnt( void ) const
{
	return (int)m_ExceptGroup.GetSize();
}

CTSGroup* CTSTrigger::AddMainGroup( void )
{
	if ( NULL == m_pMainGroup )
	{
		CTSGroup* pGroup = (CTSGroup*)RUNTIME_CLASS(CTSGroup)->CreateObject();
		pGroup->tgID = NTL_TS_MAIN_GROUP_ID;
		m_pMainGroup = pGroup;

		g_pProjMainMenuForm->m_ctrMainMenu.AddMainGroup();
	}

	return m_pMainGroup;
}

void CTSTrigger::DelMainGroup( void )
{
	if ( m_pMainGroup )
	{
		delete m_pMainGroup;
		m_pMainGroup = NULL;
		if ( g_pProjMainMenuForm ) g_pProjMainMenuForm->m_ctrMainMenu.DelMainGroup();
	}
}

CTSGroup* CTSTrigger::GetExceptionGroup( NTL_TS_TG_ID tgID )
{
	int nSize = (int)m_ExceptGroup.GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		if ( m_ExceptGroup[i]->tgID == tgID )
		{
			return m_ExceptGroup[i];
		}
	}

	return NULL;
}

CTSGroup* CTSTrigger::AddExceptionGroup( NTL_TS_TG_ID tgID )
{
	CTSGroup* pGroup = (CTSGroup*)RUNTIME_CLASS(CTSGroup)->CreateObject();
	pGroup->tgID = tgID;
	m_ExceptGroup.Add( pGroup );

	g_pProjMainMenuForm->m_ctrMainMenu.AddExceptionGroup( tgID );

	return pGroup;
}

void CTSTrigger::DelExceptionGroup( NTL_TS_TG_ID tgID )
{
	int i;
	int nSize = (int)m_ExceptGroup.GetSize();
	for ( i = 0; i < nSize; ++i )
	{
		if ( m_ExceptGroup[i]->tgID == tgID )
		{
			delete m_ExceptGroup[i];
			break;
		}
	}

	if ( i < nSize ) m_ExceptGroup.RemoveAt( i );

	if ( g_pProjMainMenuForm ) g_pProjMainMenuForm->m_ctrMainMenu.DelExceptionGroup( tgID );
}

void CTSTrigger::DelExceptionGroupAll( void )
{
	int i;
	int nSize = (int)m_ExceptGroup.GetSize();
	for ( i = 0; i < nSize; ++i )
	{
		if ( g_pProjMainMenuForm ) g_pProjMainMenuForm->m_ctrMainMenu.DelExceptionGroup( m_ExceptGroup[i]->tgID );
		delete m_ExceptGroup[i];
	}
	m_ExceptGroup.RemoveAll();
}

void CTSTrigger::ClearAll( void )
{
	DelMainGroup();
	DelExceptionGroupAll();
}

void CTSTrigger::Load( CArchive& ar )
{
	// 버전 로딩
	ar >> m_dwVersion;
	if ( m_dwVersion < eTS_VERSION )
	{
		AfxMessageBox( "Not correct version" );
		return;
	}

	// 트리거의 속성 로딩
	ar >> m_strAttr;

	// 메인 그룹 로딩
	bool bMain;
	ar >> bMain;

	if ( bMain )
	{
		AddMainGroup();

		m_pMainGroup->LinkLayer.Serialize( ar );
		m_pMainGroup->ContainerLayer.Serialize( ar );
		m_pMainGroup->NoteLayer.Serialize( ar );
	}

	// 예외 그룹 로딩
	int nSize;
	ar >> nSize;

	NTL_TS_TG_ID tgID;
	for ( int i = 0; i < nSize; ++i )
	{
		ar >> tgID;

		CTSGroup* pGroup = AddExceptionGroup( tgID );

		pGroup->Serialize( ar );
	}
}

void CTSTrigger::Save( CArchive& ar )
{
	// 버전 저장
	ar << eTS_VERSION;

	// 트리거의 속성 저장
	ar << m_strAttr;

	// 메인 그룹 저장
	ar << (m_pMainGroup ? true : false);

	if ( m_pMainGroup )
	{
		m_pMainGroup->LinkLayer.Serialize( ar );
		m_pMainGroup->ContainerLayer.Serialize( ar );
		m_pMainGroup->NoteLayer.Serialize( ar );
	}

	// 예외 그룹 저장
	int nSize = (int)m_ExceptGroup.GetSize();
	ar << nSize;

	for ( int i = 0; i < nSize; ++i )
	{
		ar << m_ExceptGroup[i]->tgID;
		m_ExceptGroup[i]->Serialize( ar );
	}
}


CString CTSTrigger::PakingPageData( CString strKey, CString strValue )
{
	CString strData;
	strData.Format( _T("%s # %s # "), strKey, strValue );
	return strData;
}

CString CTSTrigger::PakingPageData( CString strKey, int nValue )
{
	CString strData;
	strData.Format( _T("%s # %d # "), strKey, nValue );
	return strData;
}
