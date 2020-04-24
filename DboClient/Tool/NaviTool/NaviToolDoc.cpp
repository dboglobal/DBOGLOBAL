// NaviToolDoc.cpp : CNaviToolDoc 클래스의 구현
//

#include "stdafx.h"
#include "NaviTool.h"

#include "NaviToolDoc.h"

#include "DirDialog.h"
#include "DlgPathExport.h"
#include "PathEngine.h"

#include "NaviTestBed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNaviToolDoc

IMPLEMENT_DYNCREATE(CNaviToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CNaviToolDoc, CDocument)
	ON_COMMAND(ID_LOAD_WORLD_DATA, &CNaviToolDoc::OnLoadWorldData)
	ON_COMMAND(ID_SAVE_PATH_DATA, &CNaviToolDoc::OnSavePathData)
	ON_COMMAND(ID_LOAD_PATH_DATA, &CNaviToolDoc::OnLoadPathData)
	ON_COMMAND(ID_PROJECT_VIEWTOTALMEMORY, &CNaviToolDoc::OnProjectViewTotalMemory)
END_MESSAGE_MAP()


// CNaviToolDoc 생성/소멸

CNaviToolDoc::CNaviToolDoc()
{
	char szBuff[4096];
	GetCurrentDirectory( 4096, szBuff );

	m_strPath = szBuff;

	if ( !CPathEngine::GetInstance()->Create( "PathEngine" ) )
	{
		AfxMessageBox( "The path engine creation failed.\nCan not find the pathengin.dll" );
	}
}

CNaviToolDoc::~CNaviToolDoc()
{
	CPathEngine::GetInstance()->Delete();
}

BOOL CNaviToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CNaviToolDoc serialization

void CNaviToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CNaviToolDoc 진단

#ifdef _DEBUG
void CNaviToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNaviToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNaviToolDoc 명령

void CNaviToolDoc::OnLoadWorldData()
{
	char szBuff[4096];
	GetCurrentDirectory( 4096, szBuff );

	CDirDialog clDirDlg( m_strPath );

	clDirDlg.m_ofn.lpstrTitle = _T("Import world data");

	if ( IDOK != clDirDlg.DoModal() )
	{
		return;
	}

	m_strPath = clDirDlg.GetPath();

	SetCurrentDirectory( szBuff );

	if ( !CPathEngine::GetInstance()->ImportWorldData( m_strPath ) )
	{
		AfxMessageBox( "The loading world data failed." );
	}

	AfxMessageBox("ImportWorldData Success");
}

void CNaviToolDoc::OnSavePathData()
{
	CDlgPathExport dlgExport;

	int nResult = (INT)dlgExport.DoModal();

	if( IDCANCEL == nResult )
		return;
	else if( ID_BTN_PATH_EXPORT_ALL == nResult )
	{
		char szBuff[4096];
		GetCurrentDirectory( 4096, szBuff );

		CDirDialog clDirDlg( m_strPath );

		clDirDlg.m_ofn.lpstrTitle = _T("Export path data");

		if ( IDOK != clDirDlg.DoModal() )
		{
		return;
		}

		m_strPath = clDirDlg.GetPath();

		SetCurrentDirectory( szBuff );

		if ( !CPathEngine::GetInstance()->ExportPathData( m_strPath ) )
		{
			AfxMessageBox( "The exporting path data failed." );
		}
	}
	else
	{
		mapdef_ExportList list;
		dlgExport.GetExportList( list );

		char szBuff[4096];
		GetCurrentDirectory( 4096, szBuff );

		CDirDialog clDirDlg( m_strPath );

		clDirDlg.m_ofn.lpstrTitle = _T("Export path data");

		if ( IDOK != clDirDlg.DoModal() )
		{
			return;
		}

		m_strPath = clDirDlg.GetPath();

		SetCurrentDirectory( szBuff );

		if ( !CPathEngine::GetInstance()->ExportPathDataSelect( m_strPath, list ) )
		{
			AfxMessageBox( "The exporting path data failed." );
		}
	}
}

void CNaviToolDoc::OnLoadPathData()
{
	char szBuff[4096];
	GetCurrentDirectory( 4096, szBuff );

	CDirDialog clDirDlg( m_strPath );

	clDirDlg.m_ofn.lpstrTitle = _T("Load path data");

	if ( IDOK != clDirDlg.DoModal() )
	{
		return;
	}

	m_strPath = clDirDlg.GetPath();

	SetCurrentDirectory( szBuff );

	if ( !CPathEngine::GetInstance()->LoadPathData( m_strPath ) )
	{
		AfxMessageBox( "The loading path data failed." );
	}

	//NAVI_INST_HANDLE hHandle = CPathEngine::GetInstance()->CreateInstanceHandler( 12 );
	//sNAVI_POS vPos( 300.f, 0.f, 300.f );
	//iMesh* pMesh = CPathEngine::GetInstance()->GetNearstMesh( hHandle, vPos );
	//GetNaviTestBed()->SetMesh( pMesh );
	//CPathEngine::GetInstance()->GetInstance()->DeleteInstanceHandler( hHandle );

	//// Count vertex count
	//int nVertexCount = CPathEngine::GetInstance()->GetWorldVertexCount( hHandle );
	//	
	//// Create vertex buffer
	//sNAVI_VERTEX* pBuffer = new sNAVI_VERTEX[nVertexCount];
	//CPathEngine::GetInstance()->GetWorldVertexToBuffer( hHandle, pBuffer );

	//CRenderingFramework::GetInstance()->SetWorldVertex( pBuffer, nVertexCount );

	//CPathEngine::GetInstance()->GetInstance()->DeleteInstanceHandler( hHandle );

	//delete pBuffer;
}


void CNaviToolDoc::OnProjectViewTotalMemory()
{
	unsigned long ulTotalMemory = CPathEngine::GetInstance()->GetTotalMemory();

	CString strString;
	strString.Format( _T("Total memory is %u"), ulTotalMemory );

	AfxMessageBox( strString );
}
