// ExportEventDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "ExportEventDlg.h"
#include "DirDialog.h"
#include "TSUtil.h"
#include "TableContainer.h"
#include "ExportEvtTSMng.h"
#include "NtlZip.h"
#include "NtlCipher.h"


// CExportEventDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExportEventDlg, CDialog)

CExportEventDlg::CExportEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportEventDlg::IDD, pParent)
	, m_strTblPath(_T(""))
	, m_strTSPath(_T(""))
{

}

CExportEventDlg::~CExportEventDlg()
{
}

void CExportEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TABLE_PATH_EDIT, m_strTblPath);
	DDX_Text(pDX, IDC_TS_PATH_EDIT, m_strTSPath);
}


BEGIN_MESSAGE_MAP(CExportEventDlg, CDialog)
	ON_BN_CLICKED(IDC_TABLE_PATH, &CExportEventDlg::OnBnClickedTablePath)
	ON_BN_CLICKED(IDC_TS_PATH, &CExportEventDlg::OnBnClickedTsPath)
END_MESSAGE_MAP()


// CExportEventDlg 메시지 처리기입니다.

void CExportEventDlg::OnBnClickedTablePath()
{
	CDirDialog clDir( m_strCurPath, NULL, this );
	clDir.m_ofn.lpstrTitle = _T("Table path");

	if ( IDOK == clDir.DoModal() )
	{
		m_strTblPath = clDir.GetPath();
		m_strCurPath = m_strTblPath;

		UpdateData( FALSE );
	}
}

void CExportEventDlg::OnBnClickedTsPath()
{
	CDirDialog clDir( m_strCurPath, NULL, this );
	clDir.m_ofn.lpstrTitle = _T("TS path");

	if ( IDOK == clDir.DoModal() )
	{
		m_strTSPath = clDir.GetPath();
		m_strCurPath = m_strTSPath;

		UpdateData( FALSE );
	}
}

void CExportEventDlg::OnOK()
{
	CDialog::OnOK();

	if ( m_strTblPath.IsEmpty() )
	{
		AfxMessageBox( _T("Table path name is invalid") );
		return;
	}

	if ( m_strTSPath.IsEmpty() )
	{
		AfxMessageBox( _T("TS path name is invalid") );
		return;
	}

	std::string strExportFolder;
	CDirDialog clDir( m_strCurPath, NULL, this );
	clDir.m_ofn.lpstrTitle = _T("Export table folder");

	if ( IDOK != clDir.DoModal() )
	{
		return;
	}

	strExportFolder = clDir.GetPath();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Table
	//
	//////////////////////////////////////////////////////////////////////////

	CTableContainer* pTblContainer = CreateTableContainer( m_strTblPath.GetBuffer(), CTable::LOADING_METHOD_BINARY );
	if ( NULL == pTblContainer )
	{
		AfxMessageBox( _T("Building tables failed") );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	TS eventor <-> Table connection
	//
	//////////////////////////////////////////////////////////////////////////

	CExportEvtTSMng clTSMng;
	if ( !clTSMng.Create( m_strTSPath.GetBuffer(), pTblContainer ) )
	{
		AfxMessageBox( _T("Building TS Eventor failed") );
		DeleteTableContainer( pTblContainer );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Save eventor
	//
	//////////////////////////////////////////////////////////////////////////

	CDboTSMain::hashdef_EVT_MAPPER_LIST& defList = clTSMng.GetEventMapper();
	CDboTSMain::hashdef_EVT_MAPPER_LIST::iterator it = defList.begin();
	for ( ; it != defList.end(); ++it )
	{
		std::string strName = it->first;
		CNtlTSEvtMapper* pEvtMapper = it->second;

		std::string strPath = strExportFolder;
		strPath += strName;
		strPath += ".evt";

		if ( !pEvtMapper->Save( strPath ) )
		{
			CString strMsg;
			strMsg.Format( _T("[%s] build failed"), strPath.c_str() );

			AfxMessageBox( strMsg );

			continue;
		}
	}

	DeleteTableContainer( pTblContainer );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Zip
	//
	//////////////////////////////////////////////////////////////////////////

	std::string strZipName = strExportFolder + "temp.zip";

	{
		CNtlZip clZip;
		if ( !clZip.OpenZip( strZipName.c_str() ) )
		{
			AfxMessageBox( _T("Opening zip failed") );
			return;
		}

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		std::string strSearchSpec( strExportFolder + "*" );

		hFind = FindFirstFile( strSearchSpec.c_str(), &FindFileData );
		if ( INVALID_HANDLE_VALUE == hFind )
		{
			AfxMessageBox( _T("Zipping failed") );
			return;
		}

		do 
		{
			if ( FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				 '.' != FindFileData.cFileName[0] )
			{
				std::string strFindFileName = FindFileData.cFileName;

				if ( std::string::npos != strFindFileName.find( ".evt" ) )
				{
					clZip.AddFileToZip( FindFileData.cFileName, true );
				}
			}
		}
		while ( FindNextFile( hFind, &FindFileData ) != 0 );

		FindClose( hFind );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Encryption
	//
	//////////////////////////////////////////////////////////////////////////

	std::string strOutputName = strExportFolder + "evt.e";

	{
		// 파일 로딩
		FILE* pFile;
		fopen_s( &pFile, strZipName.c_str(), "rb" );
		fseek( pFile, 0, SEEK_END );
		int nReadSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );
		char* pReadBuf = new char[nReadSize];
		fread( pReadBuf, 1, nReadSize, pFile );
		fclose( pFile );

		// 암호화
		std::string strKey = "dnfldbofmftkfkdgowntpdy";
		CNtlCipher Cipher;
		Cipher.SetKey( DES_CIPHER, strKey.c_str(), strKey.size() );
		char* pEncryptBuf = new char[nReadSize+256];
		ZeroMemory( pEncryptBuf, nReadSize+256 );
		int nEncSize = Cipher.Encrypt( pReadBuf, nReadSize, pEncryptBuf, nReadSize+256 );

		// 파일에 기록
		fopen_s( &pFile, strOutputName.c_str(), "wb" );
		fwrite( &nReadSize, sizeof(int), 1, pFile );
		fwrite( pEncryptBuf, 1, nEncSize, pFile );
		fclose( pFile );

		delete [] pReadBuf;
		delete [] pEncryptBuf;
	}
}

