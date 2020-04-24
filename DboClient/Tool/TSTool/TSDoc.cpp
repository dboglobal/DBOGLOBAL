#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "TSView.h"
#include "TSProjectProperty.h"
#include "TSProjectMainMenuForm.h"
#include "Shape_Link.h"
#include "Shape_GEntityCont.h"
#include "Shape_Note.h"
#include "DirDialog.h"
#include "NtlCipher.h"
#include "ExportEventDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
//	CTSDoc
//
//////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE( CTSDoc, CDocument )

BEGIN_MESSAGE_MAP( CTSDoc, CDocument )
END_MESSAGE_MAP()


CTSDoc* CTSDoc::s_pInstance = 0;


CTSDoc::CTSDoc( void )
: c_nExtentX( 4000 ),
  c_nExtentY( 3000 ),
  m_eSelGroupType( eSEL_GROUP_TYPE_INVALID ),
  m_pTSSelGroup( NULL )
{
	ASSERT( NULL == s_pInstance );
	s_pInstance = this;
}

CTSDoc::~CTSDoc( void )
{
	m_TSProject.Delete();
}

BOOL CTSDoc::OnNewDocument( void )
{
	if ( !CDocument::OnNewDocument() ) return FALSE;

	return TRUE;
}

BOOL CTSDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
	if ( !CDocument::OnOpenDocument( lpszPathName ) ) return FALSE;

	return TRUE;
}

void CTSDoc::Serialize( CArchive& ar )
{
	if ( ar.IsStoring() )
	{
		m_TSProject.GetTrigger().Save( ar );
	}
	else
	{
		m_TSProject.GetTrigger().Load( ar );
	}
}

void CTSDoc::DeleteContents( void )
{
	CDocument::DeleteContents();

	m_TSProject.GetTrigger().ClearAll();

	UnSelGroup();
	Unselect();
	OnShapeChange();
}

bool CTSDoc::IsProjectLoaded( void ) const
{
	return m_TSProject.IsCreated();
}

DWORD CTSDoc::GetProjectMode( void ) const
{
	return m_TSProject.GetProjectMode();
}

eSEL_GROUP_TYPE CTSDoc::GetSelGroupType( void ) const
{
	return m_eSelGroupType;
}

void CTSDoc::SelMainGroup( void )
{
	m_pTSSelGroup = m_TSProject.GetTrigger().GetMainGroup();
	m_eSelGroupType = eSEL_GROUP_TYPE_MAIN;

	Unselect();
	UpdateAllViews( NULL );
}

void CTSDoc::SelExceptGroup( NTL_TS_TG_ID tgID )
{
	int nCnt = m_TSProject.GetTrigger().GetExceptionGroupCnt();
	for ( int i = 0; i < nCnt; ++i )
	{
		CTSGroup* pGroup = m_TSProject.GetTrigger().GetExceptionGroup( i );
		if ( pGroup->tgID == tgID )
		{
			m_pTSSelGroup = pGroup;
			m_eSelGroupType = eSEL_GROUP_TYPE_EXCEPTION;
		}
	}

	Unselect();
	UpdateAllViews( NULL );
}

void CTSDoc::UnSelGroup( void )
{
	m_pTSSelGroup = NULL;
	m_eSelGroupType = eSEL_GROUP_TYPE_INVALID;

	Unselect();
	UpdateAllViews( NULL );
}

CShape* CTSDoc::GetShapeAt( const CPoint &point ) const
{
	if ( NULL == m_pTSSelGroup ) return NULL;

	POSITION Pos;

	for ( Pos = m_pTSSelGroup->NoteLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->NoteLayer.GetPrev( Pos );

		if ( pShape->Intersects( point ) )
		{
			return pShape;
		}
	}

	for ( Pos = m_pTSSelGroup->ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->ContainerLayer.GetPrev( Pos );

		if ( pShape->Intersects( point ) )
		{
			return pShape;
		}
	}

	for ( Pos = m_pTSSelGroup->LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape_Link* pShape = DYNAMIC_DOWNCAST( CShape_Link, m_pTSSelGroup->LinkLayer.GetPrev( Pos ) );

		if ( pShape->IsCreated() && ((CShape*)pShape)->Intersects( point ) )
		{
			return pShape;
		}
	}

	return NULL;
}

void CTSDoc::GetShapesInRect( const CRect &rect, ShapeList& ShapeList ) const
{
	if ( NULL == m_pTSSelGroup ) return;

	POSITION Pos;

	for ( Pos = m_pTSSelGroup->NoteLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->NoteLayer.GetPrev( Pos );

		if ( pShape->Intersects( rect ) && NULL == ShapeList.Find( pShape ) )
		{
			ShapeList.AddTail( pShape );
		}
	}

	for ( Pos = m_pTSSelGroup->ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->ContainerLayer.GetPrev( Pos );

		if ( pShape->Intersects( rect ) && NULL == ShapeList.Find( pShape ) )
		{
			ShapeList.AddTail( pShape );
		}
	}

	for ( Pos = m_pTSSelGroup->LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->LinkLayer.GetPrev( Pos );

		if ( pShape->Intersects( rect ) && NULL == ShapeList.Find( pShape ) )
		{
			ShapeList.AddTail( pShape );
		}
	}
}

void CTSDoc::MakeShapesBounds( CRect &rect ) const
{
	rect.SetRectEmpty();

	if ( NULL == m_pTSSelGroup ) return;

	POSITION Pos;

	for ( Pos = m_pTSSelGroup->NoteLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->NoteLayer.GetPrev( Pos );
		rect.UnionRect( rect, pShape->GetBoundBox() );
	}

	for ( Pos = m_pTSSelGroup->ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->ContainerLayer.GetPrev( Pos );
		rect.UnionRect( rect, pShape->GetBoundBox() );
	}

	for ( Pos = m_pTSSelGroup->LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->LinkLayer.GetPrev( Pos );
		rect.UnionRect( rect, pShape->GetBoundBox() );
	}
}

void CTSDoc::AddShape( CShape* pShape )
{
	if ( pShape->IsKindOf( RUNTIME_CLASS(CShape_Link) ) )
	{
		AddLinkShape( pShape );
	}
	else if ( pShape->IsKindOf( RUNTIME_CLASS(CShape_GEntityCont) ) )
	{
		AddContainerShape( pShape );
	}
	else if ( pShape->IsKindOf( RUNTIME_CLASS(CShape_Note) ) )
	{
		AddNoteShape( pShape );
	}
}

void CTSDoc::AddLinkShape( CShape* pShape )
{
	if ( NULL == m_pTSSelGroup ) return;

	m_pTSSelGroup->LinkLayer.AddTail( pShape );
	OnShapeChange( pShape );
}

void CTSDoc::AddContainerShape( CShape* pShape )
{
	if ( NULL == m_pTSSelGroup ) return;

	m_pTSSelGroup->ContainerLayer.AddTail( pShape );
	OnShapeChange( pShape );
}

void CTSDoc::AddNoteShape( CShape* pShape )
{
	if ( NULL == m_pTSSelGroup ) return;

	m_pTSSelGroup->NoteLayer.AddTail( pShape );
	OnShapeChange( pShape );
}

void CTSDoc::RemoveShape( CShape* pShape )
{
	if ( NULL == m_pTSSelGroup ) return;

	pShape->MakeZombie();
	UpdateConstraints( pShape );

	POSITION Pos, OldPos;

	for ( Pos = m_pTSSelGroup->NoteLayer.GetTailPosition(); Pos != NULL; )
	{
		OldPos = Pos;
		CShape* pShape = m_pTSSelGroup->NoteLayer.GetPrev( Pos );

		if ( pShape->IsZombie() )
		{
			Unselect( pShape );
			m_pTSSelGroup->NoteLayer.RemoveAt( OldPos );
			delete pShape;
		}
	}

	for ( Pos = m_pTSSelGroup->ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		OldPos = Pos;
		CShape* pShape = m_pTSSelGroup->ContainerLayer.GetPrev( Pos );

		if ( pShape->IsZombie() )
		{
			Unselect( pShape );
			m_pTSSelGroup->ContainerLayer.RemoveAt( OldPos );
			delete pShape;
		}
	}

	for ( Pos = m_pTSSelGroup->LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		OldPos = Pos;
		CShape* pShape = m_pTSSelGroup->LinkLayer.GetPrev( Pos );

		if ( pShape->IsZombie() )
		{
			Unselect( pShape );
			m_pTSSelGroup->LinkLayer.RemoveAt( OldPos );
			delete pShape;
		}
	}

	OnShapeChange();
}

void CTSDoc::Render( CDC* pDC )
{
	if ( NULL == m_pTSSelGroup ) return;

	POSITION Pos;

	for ( Pos = m_pTSSelGroup->LinkLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->LinkLayer.GetPrev( Pos );
		pShape->Render( pDC );
	}

	for ( Pos = m_pTSSelGroup->ContainerLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->ContainerLayer.GetPrev( Pos );
		pShape->Render( pDC );
	}

	for ( Pos = m_pTSSelGroup->NoteLayer.GetTailPosition(); Pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->NoteLayer.GetPrev( Pos );
		pShape->Render( pDC );
	}
}

void CTSDoc::OnShapeChange( CShape* pShape /*= NULL*/ )
{
	UpdateConstraints( pShape );

	SetModifiedFlag();
	UpdateAllViews( NULL );
}

void CTSDoc::OnCreateProject( void )
{
	// 이전에 열려 있던 프로젝트를 저장한다
	if ( IsModified() )
	{
		int nRet = AfxMessageBox( _T("이전에 열린 프로젝트가 수정되었습니다.\n저장하시겠습니까?"), MB_OKCANCEL );
		if ( IDCANCEL == nRet ) return;

		DoFileSave();
	}

	// Document를 초기화한다
	OnNewDocument();

	// Project를 생성한다
	CTSProjectProperty clProjProperty;
	if ( IDOK == clProjProperty.DoModal() )
	{
		OnCloseProject();

		m_TSProject.CreateOnInit( clProjProperty.m_strProjPath, clProjProperty.m_strProjName, clProjProperty.m_dwMode );

		UnSelGroup();
		Unselect();
		UpdateAllViews( NULL );

		// Main menu 초기화
		g_pProjMainMenuForm->m_ctrTriggerList.ClearAll();
		int nSize = (int)m_TSProject.GetTriggerList().GetSize();
		for ( int i = 0; i < nSize; ++i )
		{
			g_pProjMainMenuForm->m_ctrTriggerList.AddList( m_TSProject.GetTriggerList()[i] );
		}
	}
}

void CTSDoc::OnOpenProject( void )
{
	// 이전에 열려 있던 프로젝트를 저장한다
	if ( IsModified() )
	{
		int nRet = AfxMessageBox( _T("이전에 열린 프로젝트가 수정되었습니다.\n저장하시겠습니까?"), MB_OKCANCEL );
		if ( IDCANCEL == nRet ) return;

		DoFileSave();
	}

	// Document를 초기화한다
	OnNewDocument();

	// 프로젝트를 연다
	CFileDialog clFile( TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "TS Project files (*.pro)|*.pro||", NULL, 0 );
	if ( IDOK == clFile.DoModal() )
	{
		OnCloseProject();

		CString strPath = clFile.GetPathName();

		m_TSProject.Load( strPath );

		UnSelGroup();
		Unselect();
		UpdateAllViews( NULL );

		// Main menu 초기화
		g_pProjMainMenuForm->m_ctrTriggerList.ClearAll();
		int nSize = (int)m_TSProject.GetTriggerList().GetSize();
		for ( int i = 0; i < nSize; ++i )
		{
			g_pProjMainMenuForm->m_ctrTriggerList.AddList( m_TSProject.GetTriggerList()[i] );
		}
	}
}

void CTSDoc::OnSaveProject( void )
{
	DoFileSave();
	m_TSProject.Save( m_TSProject.GetProjectFullName() );
}

void CTSDoc::OnCloseProject( void )
{
	m_TSProject.Delete();

	UnSelGroup();
	Unselect();
	UpdateAllViews( NULL );

	// Main menu 초기화
	g_pProjMainMenuForm->m_ctrMainMenu.ClearAll();
	g_pProjMainMenuForm->m_ctrTriggerList.ClearAll();
}

void CTSDoc::OnExportTrigger( void )
{
	m_clTSMng.Create();

	CNtlTSTrigger* pTrigger = m_clTSMng.CreateTrigger( &m_TSProject.GetTrigger() );

	CString strPath;
	strPath.Format( _T("%s\\%d.t"), m_TSProject.GetProjectPath(), pTrigger->GetID() );

	std::vector<CNtlTSScrObject*> vecList;
	vecList.push_back( pTrigger );

	CNtlTSScrSaver clSaver( m_clTSMng.GetEntityFactory() );
	clSaver.Save( strPath.GetString(), vecList );

	m_clTSMng.DeleteTrigger( pTrigger );

	m_clTSMng.Delete();
}

void CTSDoc::OnExportTriggerAll( void )
{
	// 이전에 수정된 트리거 정보는 저장한다
	if ( IsModified() )
	{
		int nRet = AfxMessageBox( _T("이전에 수정중인 트리거가 존재합니다.\n저장하시겠습니까?"), MB_OKCANCEL );
		if ( IDCANCEL == nRet ) return;

		DoFileSave();
	}

	CDirDialog clDir( m_TSProject.GetProjectPath(), NULL, NULL );
	clDir.m_ofn.lpstrTitle = _T("Trigger system export path");
	if ( IDOK != clDir.DoModal() ) return;

	CString strExportPath = clDir.GetPath();

	CString strPath;
	CArray<CString, const CString&>& clArray = m_TSProject.GetTriggerList();
	int nSize = (int)clArray.GetCount();
	for ( int i = 0; i < nSize; ++i )
	{
		// 툴의 TS 데이터를 읽어 들인다
		strPath.Format( _T("%s\\%s"), m_TSProject.GetProjectPath(), clArray[i] );

		CFile clFile( strPath.GetString(), CFile::modeRead|CFile::shareDenyWrite );
		CArchive ar( &clFile, CArchive::load|CArchive::bNoFlushOnDelete );

		CTSTrigger clTrigger;
		clTrigger.Load( ar );

		// 데이터를 익스포트한다
		m_clTSMng.Create();

		CNtlTSTrigger* pTrigger = m_clTSMng.CreateTrigger( &clTrigger );

		CString strPath;
		strPath.Format( _T("%s\\%d.t"), strExportPath, pTrigger->GetID() );

		std::vector<CNtlTSScrObject*> vecList;
		vecList.push_back( pTrigger );

		CNtlTSScrSaver clSaver( m_clTSMng.GetEntityFactory() );
		clSaver.Save( strPath.GetString(), vecList );

		m_clTSMng.DeleteTrigger( pTrigger );

		m_clTSMng.Delete();
	}
}

void CTSDoc::OnEncryptionTrigger( void )
{
	// 이전에 수정된 트리거 정보는 저장한다
	if ( IsModified() )
	{
		int nRet = AfxMessageBox( _T("이전에 수정중인 트리거가 존재합니다.\n저장하시겠습니까?"), MB_OKCANCEL );
		if ( IDCANCEL == nRet ) return;

		DoFileSave();
	}

	CFileDialog clFile( TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "TS Zip 파일 files (*.t)|*.t||", NULL, 0 );
	if ( IDOK == clFile.DoModal() )
	{
		CString strPath = clFile.GetPathName();

		// 파일 로딩
		FILE* pFile;
		fopen_s( &pFile, strPath, "rb" );
		fseek( pFile, 0, SEEK_END );
		int nReadSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );
		char* pReadBuf = new char[nReadSize];
		fread( pReadBuf, 1, nReadSize, pFile );
		fclose( pFile );

		// 암호화
		CString strKey = "dnfldbofmftkfkdgowntpdy";
		CNtlCipher Cipher;
		Cipher.SetKey( DES_CIPHER, strKey, strKey.GetLength() );
		char* pEncryptBuf = new char[nReadSize+256];
		ZeroMemory( pEncryptBuf, nReadSize+256 );
		int nEncSize = Cipher.Encrypt( pReadBuf, nReadSize, pEncryptBuf, nReadSize+256 );

		// 파일에 기록
		static char szDir[1024];
		static char szPath[1024];
		static char szName[1024];
		_splitpath( strPath, szDir, szPath, szName, 0 );
		CString strOutputName = szDir;
		strOutputName += szPath;
		strOutputName += szName;
		strOutputName += ".e";
		fopen_s( &pFile, strOutputName, "wb" );
		fwrite( &nReadSize, sizeof(int), 1, pFile );
		fwrite( pEncryptBuf, 1, nEncSize, pFile );
		fclose( pFile );

		delete [] pReadBuf;
		delete [] pEncryptBuf;
	}
}

void CTSDoc::OnExportEventor( void )
{
	CExportEventDlg clDlg;

	clDlg.DoModal();
}

void CTSDoc::OnListReload( void )
{
	OnNewDocument();

	m_TSProject.Reload();

	g_pProjMainMenuForm->m_ctrTriggerList.ClearAll();
	int nSize = (int)m_TSProject.GetTriggerList().GetSize();
	for ( int i = 0; i < nSize; ++i )
	{
		g_pProjMainMenuForm->m_ctrTriggerList.AddList( m_TSProject.GetTriggerList()[i] );
	}
}

void CTSDoc::UpdateConstraints( CShape* pShape )
{
	if ( NULL == m_pTSSelGroup ) return;

	for ( POSITION pos = m_pTSSelGroup->LinkLayer.GetTailPosition(); pos != NULL; )
	{
		CShape* pShape = m_pTSSelGroup->LinkLayer.GetPrev( pos );
		pShape->Update();
	}
}

void CTSDoc::Unselect( CShape* pShape /*= NULL*/ )
{
	if ( g_pTSView && !g_pTSView->GetSelectionList().IsEmpty() )
	{
		if ( pShape )
		{
			g_pTSView->UnselectShape( pShape );
		}
		else
		{
			g_pTSView->ClearSelections();
		}
	}
}

#ifdef _DEBUG
void CTSDoc::AssertValid( void ) const
{
	CDocument::AssertValid();
}

void CTSDoc::Dump( CDumpContext& dc ) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
