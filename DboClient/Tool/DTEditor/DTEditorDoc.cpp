// DTEditorDoc.cpp : implementation of the CDTEditorDoc class
//

#include "stdafx.h"
#include "DTEditor.h"
#include "MainFrm.h"

#include "DTEditorDoc.h"
#include "ObjectPane.h"
#include "DTEditorGlobal.h"
#include "Msjexhnd.h"


// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlSLApi.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDTEditorDoc *g_pDoc = NULL;

VOID DTEditorExceptionHandler( PEXCEPTION_POINTERS pExceptionInfo )
{
	if(g_pDoc)
	{
		AfxMessageBox("DTEditor is system halt - save!!!"); 
		g_pDoc->OnFileSaveAs();
	}
}


// CDTEditorDoc

IMPLEMENT_DYNCREATE(CDTEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDTEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, &CDTEditorDoc::OnFileNew)
	ON_COMMAND(ID_FILE_SAVE, &CDTEditorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CDTEditorDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, &CDTEditorDoc::OnFileOpen)
END_MESSAGE_MAP()


// CDTEditorDoc construction/destruction

CDTEditorDoc::CDTEditorDoc()
{
	// TODO: add one-time construction code here

	g_pDoc = this;
	g_MSJExceptionHandler.SetApplicationFilter(DTEditorExceptionHandler);

}

CDTEditorDoc::~CDTEditorDoc()
{
}

void CDTEditorDoc::LoadPrj(CString& strFileName)
{
	NtlFileAttrReadOnlyRelease( (RwChar*)strFileName.GetBuffer());

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, strFileName.GetBuffer(), "rb") != 0)
		return;
#else
	fp = fopen(strFileName.GetBuffer(), "rb");
	if(fp == NULL)
		return;
#endif

	fseek(fp, 0, SEEK_END);
	RwInt32 iSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(iSize == 0)
	{
		fclose(fp);
		return;
	}

	SetActiveDTProp(NULL);

	CNtlSerializer s(iSize, iSize/10);
	fread((void*)s.GetData(), iSize, 1, fp);
	s.IncrementEndPointer(iSize);

	fclose(fp);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CObjectPane *pObjectPane = pMainFrame->GetObjectPane();
	pObjectPane->Load(s);
}

void CDTEditorDoc::ImportPrj(CString& strFileName, CNtlDTContainer *pDTContainer)
{
	NtlFileAttrReadOnlyRelease( (RwChar*)strFileName.GetBuffer());

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, strFileName.GetBuffer(), "rb") != 0)
		return;
#else
	fp = fopen(strFileName.GetBuffer(), "rb");
	if(fp == NULL)
		return;
#endif

	fseek(fp, 0, SEEK_END);
	RwInt32 iSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(iSize == 0)
	{
		fclose(fp);
		return;
	}

	CNtlSerializer s(iSize, iSize/10);
	fread((void*)s.GetData(), iSize, 1, fp);
	s.IncrementEndPointer(iSize);

	fclose(fp);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CObjectPane *pObjectPane = pMainFrame->GetObjectPane();
	pObjectPane->Import(s, pDTContainer);
}

void CDTEditorDoc::SavePrj(CString& strFileName)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CObjectPane *pObjectPane = pMainFrame->GetObjectPane();

	RwInt32 iBufferSize = 1024*1024*10;
	CNtlSerializer s(1024*100, 1024*10); 

	pObjectPane->Save(s);
	
	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, strFileName.GetBuffer(), "wb") != 0)
		return;
#else
	fp = fopen(strFileName.GetBuffer(), "wb");
	if(fp == NULL)
		NTL_RETURN(FALSE);
#endif

	fwrite(s.GetData(), s.GetDataSize(), 1, fp);

	fclose(fp);
}

BOOL CDTEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	GetCurrentDirectory(MAX_DIR_PATH, m_szWorkDir);

	return TRUE;
}




// CDTEditorDoc serialization

void CDTEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDTEditorDoc diagnostics

#ifdef _DEBUG
void CDTEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDTEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDTEditorDoc commands

void CDTEditorDoc::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	
}

void CDTEditorDoc::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	TCHAR chCurDir[MAX_DIR_PATH];
	GetCurrentDirectory(MAX_DIR_PATH, chCurDir);
	
	CFileDialog	LoadDlg( TRUE, _T( "dpf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T( "Direct Project Files (*.dpf)|*.dpf|All Files (*.*)|*.*||" ), NULL );

	if(LoadDlg.DoModal() == IDOK)
	{
		m_strWorkFull			= LoadDlg.GetPathName();
		m_strWorkFileName		= LoadDlg.GetFileName();
		m_strWorkFileTitle		= LoadDlg.GetFileTitle();
		m_strWorkPath			= m_strWorkFull;
		m_strWorkPath.Delete(m_strWorkPath.GetLength() - m_strWorkFileName.GetLength(), m_strWorkFileName.GetLength());

		CString strPropFullPath = m_strWorkPath + m_strWorkFileTitle + "." + DT_FILE_EXT;

		// direct property load
		API_GetDTContainter()->Destroy();
		API_GetDTContainter()->Load(strPropFullPath.GetBuffer());

		// project file load
		LoadPrj(m_strWorkFull);
	}

	SetCurrentDirectory(chCurDir);
}


void CDTEditorDoc::OnImportFile(void)
{
	TCHAR chCurDir[MAX_DIR_PATH];
	GetCurrentDirectory(MAX_DIR_PATH, chCurDir);
	
	CFileDialog	LoadDlg( TRUE, _T( "dpf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T( "Direct Project Files (*.dpf)|*.dpf|All Files (*.*)|*.*||" ), NULL );

	if(LoadDlg.DoModal() == IDOK)
	{
		CString strFull			= LoadDlg.GetPathName();
		CString strFileName		= LoadDlg.GetFileName();
		CString strFileTitle	= LoadDlg.GetFileTitle();
		CString strPath			= strFull;

		strPath.Delete(strPath.GetLength() - strFileName.GetLength(), strFileName.GetLength());

		CString strPropFullPath = strPath + strFileTitle + "." + DT_FILE_EXT;

		// direct property load
		CNtlDTContainer DTContainer;
		DTContainer.Load(strPropFullPath.GetBuffer());

		// project file load
		ImportPrj(strFull, &DTContainer);

		API_GetDTContainter()->Append(DTContainer);
	}

	SetCurrentDirectory(chCurDir);
}

void CDTEditorDoc::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if(m_strWorkFull.GetLength() == 0)
	{
		TCHAR chCurDir[MAX_DIR_PATH];
		GetCurrentDirectory(MAX_DIR_PATH, chCurDir);
		
		CFileDialog	SaveDlg( FALSE, _T( "dpf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							_T( "Direct Project Files (*.dpf)|*.dpf|All Files (*.*)|*.*||" ), NULL );

		if(SaveDlg.DoModal() == IDOK)
		{
			m_strWorkFull			= SaveDlg.GetPathName();
			m_strWorkFileName		= SaveDlg.GetFileName();
			m_strWorkFileTitle		= SaveDlg.GetFileTitle();
			m_strWorkPath			= m_strWorkFull;
			m_strWorkPath.Delete(m_strWorkPath.GetLength() - m_strWorkFileName.GetLength(), m_strWorkFileName.GetLength());

			CString strPropFullPath = m_strWorkPath + m_strWorkFileTitle + "." + DT_FILE_EXT;

			// direct property save
			API_GetDTContainter()->Save(strPropFullPath.GetBuffer());

			// project file save
			SavePrj(m_strWorkFull);
		}

		SetCurrentDirectory(chCurDir);
	}
	else
	{
		CString strPropFullPath = m_strWorkPath + m_strWorkFileTitle + "." + DT_FILE_EXT;

		// direct property save
		API_GetDTContainter()->Save(strPropFullPath.GetBuffer());

		// project file save
		SavePrj(m_strWorkFull);
	}
}

void CDTEditorDoc::OnFileSaveAs()
{
	TCHAR chCurDir[MAX_DIR_PATH];
	GetCurrentDirectory(MAX_DIR_PATH, chCurDir);
	
	CFileDialog	SaveDlg( FALSE, _T( "dpf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T( "Direct Project Files (*.dpf)|*.dpf|All Files (*.*)|*.*||" ), NULL );

	if(SaveDlg.DoModal() == IDOK)
	{
		m_strWorkFull			= SaveDlg.GetPathName();
		m_strWorkFileName		= SaveDlg.GetFileName();
		m_strWorkFileTitle		= SaveDlg.GetFileTitle();
		m_strWorkPath			= m_strWorkFull;
		m_strWorkPath.Delete(m_strWorkPath.GetLength() - m_strWorkFileName.GetLength(), m_strWorkFileName.GetLength());

		CString strPropFullPath = m_strWorkPath + m_strWorkFileTitle + "." + DT_FILE_EXT;

		// direct property save
		API_GetDTContainter()->Save(strPropFullPath.GetBuffer());

		// project file save
		SavePrj(m_strWorkFull);
	}

	SetCurrentDirectory(chCurDir);
}

