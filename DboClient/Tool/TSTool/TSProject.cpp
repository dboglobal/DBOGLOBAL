#include "stdafx.h"
#include "TSTool.h"
#include "TSProject.h"
#include "TSToolDefine.h"


CTSProject::CTSProject( void )
{
	m_bCreated = false;

	m_strHeader = _T("NTL_TRIGGER_SYSTEM_HEADER");
	m_dwVersion = eTS_VERSION;
	m_dwTSType = TS_TYPE_INVALID;
}

CTSProject::~CTSProject( void )
{
	Delete();
}

void CTSProject::CreateOnInit( const CString& strProjPath, const CString& strProjName, DWORD dwMode )
{
	m_dwVersion = eTS_VERSION;
	m_strProjName = strProjName;
	m_dwTSType = dwMode;
	m_strPath = strProjPath;

	CString strFullName = strProjPath;
	strFullName += _T('\\') != strFullName[strFullName.GetLength() - 1] ? _T("\\") : _T("");
	strFullName += strProjName;
	strFullName += _T(".");
	strFullName += PROJECT_FILE_EXTENTION;

	Save( strFullName );

	Create( strProjPath );
}

void CTSProject::Delete( void )
{
	m_bCreated = false;

	m_dwVersion = eTS_VERSION;
	m_dwTSType = TS_TYPE_INVALID;

	m_strProjName.Empty();

	m_aTriggerList.RemoveAll();

	m_TSTrigger.ClearAll();
}

void CTSProject::Load( const CString& strFullName )
{
	CWaitCursor clWaitCursor;

	CFile clFile( strFullName.GetString(), CFile::modeRead|CFile::shareDenyWrite );
	CArchive ar( &clFile, CArchive::load|CArchive::bNoFlushOnDelete );

	CString strHeader;

	ar >> m_dwVersion;
	ar >> strHeader;
	if ( strHeader != m_strHeader )
	{
		AfxMessageBox( _T("TS project file이 아닙니다") );
		return;
	}
	ar >> m_strProjName;
	ar >> m_dwTSType;

	CString strPath = ar.GetFile()->GetFilePath();
	int nFind = strPath.ReverseFind( _T('\\') );
	if ( -1 == nFind ) return;
	strPath = strPath.Left( nFind );

	m_strPath = strPath;

	Create( strPath );
}

void CTSProject::Save( const CString& strFullName )
{
	CWaitCursor clWaitCursor;

	CFile clFile( strFullName.GetString(), CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive );
	CArchive ar( &clFile, CArchive::store|CArchive::bNoFlushOnDelete );

	ar << eTS_VERSION;
	ar << m_strHeader;
	ar << m_strProjName;
	ar << m_dwTSType;

	ar.Flush();
}

void CTSProject::Reload( void )
{
	m_aTriggerList.RemoveAll();
	if ( !LoadTS( m_strPath ) )
	{
		CString strMessage;
		strMessage.Format( _T("%s에서 파일을 트리거 파일들을 읽어 들이는데 실패했습니다"), m_strPath.GetString() );
		AfxMessageBox( strMessage );

		Delete();
	}
}

void CTSProject::Create( const CString& strProjPath )
{
	ASSERT( !m_bCreated );

	m_bCreated = true;

	if ( !LoadTS( strProjPath ) )
	{
		CString strMessage;
		strMessage.Format( _T("%s에서 파일을 트리거 파일들을 읽어 들이는데 실패했습니다"), strProjPath.GetString() );
		AfxMessageBox( strMessage );

		Delete();
	}
}

bool CTSProject::LoadTS( const CString& strPath )
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::string strSearchSpec( strPath + "\\*.*" );

	hFind = FindFirstFile( strSearchSpec.c_str(), &FindFileData );
	if ( INVALID_HANDLE_VALUE == hFind )
	{
		AfxMessageBox( _T("Can not find the scripts to load.") );
		return false;
	}

	do 
	{
		if ( FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			'.' != FindFileData.cFileName[0] )
		{
			if ( !AddTriggerList( CString( FindFileData.cFileName ) ) )
			{
				FindClose( hFind );
				return false;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

bool CTSProject::AddTriggerList( CString& strName )
{
	int nFind = strName.ReverseFind( _T('.') );
	if ( -1 == nFind ) return true;
	CString strExtention = strName.Right( strName.GetLength() - nFind - 1 );

	if ( TS_FILE_EXTENTION == strExtention )
	{
		m_aTriggerList.Add( strName );
	}

	return true;
}
