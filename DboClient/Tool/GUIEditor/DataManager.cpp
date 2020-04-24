#include "StdAfx.h"
#include ".\datamanager.h"
#include "GUIFileView.h"
#include "LayerMisc.h"
#include "SurfaceListDlg.h"
#include "ResourceListDlg.h"
#include "GUIFileInfoView.h"

#define CONFIGFILE	".\\GUIEditor.cfg"

CDataManager::CDataManager(void)
:m_mgrSurface( &m_mgrResource )
{
	m_bRsrFileLoad = FALSE;
	m_bSrfFileLoad = FALSE;
}

CDataManager::~CDataManager(void)
{
	SaveConfig();

	ClearResourceList();
	ClearSurfaceList();	
}

void CDataManager::ClearResourceList()
{
	listResourceData::iterator it;

	for( it = m_listResourceData.begin() ; it != m_listResourceData.end() ; ++it )
	{
		delete (*it);
	}

	m_listResourceData.clear();
}

void CDataManager::ClearSurfaceList()
{
	listSurfaceData::iterator it;

	for( it = m_listSurfaceData.begin() ; it != m_listSurfaceData.end() ; ++it )
	{
		delete (*it);
	}

	m_listSurfaceData.clear();
}

BOOL CDataManager::AddResourceData( RESOURCEDATA* pResourceData )
{
	m_listResourceData.push_back( pResourceData );

	return TRUE;
}

BOOL CDataManager::AddSurfaceData( SURFACEDATA* pSurfaceData )
{
	m_listSurfaceData.push_back( pSurfaceData );

	return TRUE;
}

BOOL CDataManager::RemoveResourceData( INT nIndex )
{
	listResourceData::iterator it;

	it = m_listResourceData.begin();
	
	for( INT i = 1 ; i < nIndex ; i++ )
	{
		++it;
	}

	delete (*it);
	m_listResourceData.erase( it );

	return TRUE;
}

BOOL CDataManager::RemoveSurfaceData( INT nIndex )
{
	listSurfaceData::iterator it;

	it = m_listSurfaceData.begin();
	
	for( INT i = 1 ; i < nIndex ; i++ )
	{
		++it;
	}

	delete (*it);
	m_listSurfaceData.erase( it );

	return TRUE;
}

RESOURCEDATA* CDataManager::GetResourceData( INT nIndex )
{
	listResourceData::iterator it;

	if( nIndex < 1 )
		return NULL;

	it = m_listResourceData.begin();

	for( INT i = 1 ; i < nIndex ; i++ )
	{
		++it;
	}

	return (*it);
}

RESOURCEDATA* CDataManager::GetResourceData( CString strName )
{
	listResourceData::iterator it;

	for( it = m_listResourceData.begin() ; it != m_listResourceData.end() ; ++it )
	{
		if( strName == (*it)->_strName )
			return (*it);
	}

	return NULL;
}

RESOURCEDATA* CDataManager::GetResourceDataFromFilename( CString strName )
{
	listResourceData::iterator it;

	for( it = m_listResourceData.begin() ; it != m_listResourceData.end() ; ++it )
	{
		if( strName == (*it)->_strFileName )
			return (*it);
	}

	return NULL;
}

SURFACEDATA* CDataManager::GetSurfaceData( INT nIndex )
{
	listSurfaceData::iterator it;

	if( nIndex < 1 )
		return NULL;

	it = m_listSurfaceData.begin();

	for( INT i = 1 ; i < nIndex ; i++ )
	{
		++it;
	}

	return (*it);
}

SURFACEDATA* CDataManager::GetSurfaceData( CString strName )
{
	listSurfaceData::iterator it;

	for( it = m_listSurfaceData.begin() ; it != m_listSurfaceData.end() ; ++it )
	{
		if( strName== (*it)->_strName )
			return (*it);
	}

	return NULL;
}

BOOL	CDataManager::LoadRsrFile( CString& strFullPath, CString& strSubPath, BOOL bTabChange /* = FALSE  */ )
{
	WIN32_FIND_DATA fileinfo;

	if( FindFirstFile( (LPCTSTR)strFullPath, &fileinfo ) == INVALID_HANDLE_VALUE )
		return FALSE;
	
	if( strSubPath == m_strRsrFileName )
		return TRUE;

	ClearRsrFile();

	std::string sFilename;
	sFilename = (LPCTSTR)strFullPath;

	if( !m_mgrResource.AddPage( sFilename ) )
		return FALSE;

	m_strRsrFileName = strSubPath;
	
	LoadResourceList();

	// 리스트 갱신
	CResourceLayer* pLayer = (CResourceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::RESOURCE_LAYER );
	CResourceListDlg* pDlg = (CResourceListDlg*)pLayer->GetRollupDlg( CResourceLayer::RESOURCELISTDLG );
	pDlg->DisplayList();

	// 파일 표시
	m_bRsrFileLoad = TRUE;
	g_pFileInfoView->SetRsrText( strSubPath );

	// 뷰전환
	if( bTabChange )
		CLayerMisc::GetInstance().SetLayer( CLayerMisc::RESOURCE_LAYER );

	return TRUE;
}

BOOL CDataManager::LoadSrfFile(CString& strFullPath, CString& strSubPath )
{
	WIN32_FIND_DATA fileinfo;

	if( FindFirstFile( (LPCTSTR)strFullPath, &fileinfo ) == INVALID_HANDLE_VALUE )
		return FALSE;
	
	if( strSubPath == m_strSrfFileName )
		return TRUE;

	ClearSrfFile();

	std::string sFilename;
	sFilename = (LPCTSTR)strFullPath;

	if( !m_mgrSurface.AddPage( sFilename ) )
		return FALSE;

	// 파일 표시
	m_strSrfFileName = strSubPath;
	m_bSrfFileLoad = TRUE;
	g_pFileInfoView->SetSrfText( strSubPath );
	
	LoadSurfaceList();

	// 리스트 갱신
	CSurfaceLayer* pLayer = (CSurfaceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::SURFACE_LAYER );
	CSurfaceListDlg* pDlg = (CSurfaceListDlg*)pLayer->GetRollupDlg( CSurfaceLayer::SURFACELISTDLG );
	pDlg->DisplayList();

	// 뷰전환
	CLayerMisc::GetInstance().SetLayer( CLayerMisc::SURFACE_LAYER );

	return TRUE;
}

BOOL CDataManager::SaveRsrFile()
{
	CString strFilename;
	if( !FileDialogOpen( &strFilename, RSR, FALSE ) )
		return FALSE;

	// File 열기
	FILE* pFile = NULL;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	errno_t eValue = fopen_s( &pFile, (LPCTSTR)strFilename, "wt" );
	if( eValue )
		return FALSE;
#else
	pFile = fopen( (LPCTSTR)strFilename, "wt" );
	if( !pFile )
		return FALSE;
#endif

	fseek( pFile, 0, SEEK_SET );
	listResourceData::iterator it;
	
	// 저장
	for( it = m_listResourceData.begin() ; it != m_listResourceData.end() ; ++it )
	{
		fprintf( pFile, "resource \"%s\"\n{\n", (LPCTSTR)(*it)->_strName );
		fprintf( pFile, "\thandle\t= %d;\n", (*it)->_nHandle );
		fprintf( pFile, "\tfile\t= \"%s\";\n}\n\n", (LPCTSTR)(*it)->_strFileName );
	}

	fclose( pFile );

	// 파일 표시
	m_strRsrFileName = g_pFileView->GetTreeCtrl()->GetSubPath( strFilename );
	m_bRsrFileLoad = TRUE;
	g_pFileInfoView->SetRsrText( m_strRsrFileName );
	CResourceLayer* pLayer = (CResourceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::RESOURCE_LAYER );
	CResourceListDlg* pDlg = (CResourceListDlg*)pLayer->GetRollupDlg( CResourceLayer::RESOURCELISTDLG );
	pDlg->DisplayList();

	return TRUE;
}

BOOL	CDataManager::SaveSrfFile()
{
	CString strFilename;
	if( !FileDialogOpen( &strFilename, SRF, FALSE ) )
		return FALSE;

	// File 열기
	FILE* pFile = NULL;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	errno_t eValue = fopen_s( &pFile, (LPCTSTR)strFilename, "wt" );
	if( eValue )
		return FALSE;
#else
	pFile = fopen( (LPCTSTR)strFilename, "wt" );
	if( !pFile )
		return FALSE;
#endif

	fseek( pFile, 0, SEEK_SET );
	listSurfaceData::iterator it;
	
	// 저장
	for( it = m_listSurfaceData.begin() ; it != m_listSurfaceData.end() ; ++it )
	{
		fprintf( pFile, "surface \"%s\"\n{\n", (LPCTSTR)(*it)->_strName );
		fprintf( pFile, "\tresource_file\t= \"%s\";\n", (LPCTSTR)(*it)->_strResourceFileName );
		fprintf( pFile, "\tresource_name\t= \"%s\";\n\n", (LPCTSTR)(*it)->_strResourceName );
		fprintf( pFile, "\tx\t\t= %d;\n", (*it)->_rcCoord.left );
		fprintf( pFile, "\ty\t\t= %d;\n", (*it)->_rcCoord.top );
		fprintf( pFile, "\twidth\t\t= %d;\n", (*it)->_rcCoord.right );
		fprintf( pFile, "\theight\t\t= %d;\n\n", (*it)->_rcCoord.bottom );
		fprintf( pFile, "\tcolor_red\t= %d;\n", (*it)->_cRed );
		fprintf( pFile, "\tcolor_green\t= %d;\n", (*it)->_cGreen );
		fprintf( pFile, "\tcolor_blue\t= %d;\n", (*it)->_cBlue );
		fprintf( pFile, "\tcolor_alpha\t= %d;\n\n", (*it)->_cAlpha );
		fprintf( pFile, "\tuv_left\t\t= %d;\n", (*it)->_rcUV.left );
		fprintf( pFile, "\tuv_top\t\t= %d;\n", (*it)->_rcUV.top );
		fprintf( pFile, "\tuv_right\t= %d;\n", (*it)->_rcUV.right );
		fprintf( pFile, "\tuv_bottom\t= %d;\n\n", (*it)->_rcUV.bottom );
		fprintf( pFile, "\thandle\t\t= %d;\n", (*it)->_nHandle );
		fprintf( pFile, "\tblend\t\t= %s;\n}\n\n", (LPCTSTR)(*it)->_strBlendFunction );
	}

	fclose( pFile );

	// 파일 표시
	m_strSrfFileName = g_pFileView->GetTreeCtrl()->GetSubPath( strFilename );
	m_bSrfFileLoad = TRUE;
	g_pFileInfoView->SetSrfText( m_strSrfFileName );
	CSurfaceLayer* pLayer = (CSurfaceLayer*)CLayerMisc::GetInstance().GetLayer( CLayerMisc::SURFACE_LAYER );
	CSurfaceListDlg* pDlg = (CSurfaceListDlg*)pLayer->GetRollupDlg( CSurfaceLayer::SURFACELISTDLG );
	pDlg->DisplayList();

	return TRUE;
}

void	CDataManager::ClearRsrFile()
{
	std::string sFilename;
	sFilename = (LPCTSTR)m_strRsrFileName;

	ClearResourceList();
	m_bRsrFileLoad = FALSE;
	m_mgrResource.RemovePage( sFilename );
	m_strRsrFileName = CString( "" );
	g_pFileInfoView->SetRsrText( CString( "" ) );
	
}

void	CDataManager::ClearSrfFile()
{
	std::string sFilename;
	sFilename = (LPCTSTR)m_strSrfFileName;

	ClearSurfaceList();
	m_bSrfFileLoad = FALSE;
	m_mgrSurface.RemovePage( sFilename );
	m_strSrfFileName = CString( "" );
	g_pFileInfoView->SetSrfText( CString( "" ) );
}

BOOL CDataManager::FileDialogOpen( CString* pFilename, TYPE eFileType, BOOL bLoad )
{
	BOOL	bOpenDlg	 = bLoad;
	DWORD	dwFlags		 = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	LPCTSTR lpszDefExt;
	LPCTSTR lpszFilter;
	CString	strFileName;

	switch( eFileType )
	{
	case RSR:
		lpszDefExt	 = "rsr";
		lpszFilter	 = "Resource Script File(*.rsr)|*.rsr|";
		break;
	case SRF:
		lpszDefExt	 = "srf";
		lpszFilter	 = "Surface Script File(*.srf)|*.srf|";
		break;
	case FRM:
		lpszDefExt	 = "frm";
		lpszFilter	 = "Component Script File(*.frm)|*.frm|";
		break;
	}
	
	CFileDialog openDlg( bOpenDlg, lpszDefExt, "", dwFlags, lpszFilter );
	openDlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_strWorkDir;

	if( openDlg.DoModal() != IDOK )
		return FALSE;

	(*pFilename) = openDlg.GetPathName();
	
	return TRUE;
}

void CDataManager::LoadResourceList()
{
	gui::CResourcePage* pPage = NULL;
	std::string sFilename = (LPCTSTR)m_strRsrFileName;
	pPage = m_mgrResource.GetResourcePage( sFilename );

	gui::CResourceFileParser::stlCOMPONENT_SORT_MAP &stlSortMap = pPage->GetParser()->GetSortedComponents();
	gui::CResourceFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;

	for ( it=stlSortMap.begin() ; it != stlSortMap.end() ; it++ )
	{
		gui::CResourceFileParser::ComponentInfo* pInfo = (*it).second;
		LoadResourceData( pInfo ); 
	}
}

void CDataManager::LoadSurfaceList()
{
	gui::CSurfacePage* pPage = NULL;
	std::string sFilename = (LPCTSTR)m_strSrfFileName;
	pPage = m_mgrSurface.GetSurfacePage( sFilename );

	gui::CSurfaceFileParser::stlCOMPONENT_SORT_MAP &stlSortMap = pPage->GetParser()->GetSortedComponents();
	gui::CSurfaceFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;

	for ( it=stlSortMap.begin() ; it != stlSortMap.end() ; it++ )
	{
		gui::CSurfaceFileParser::ComponentInfo* pInfo = (*it).second;
		LoadSurfaceData( pInfo ); 
	}
}

void CDataManager::LoadResourceData( gui::CResourceFileParser::ComponentInfo* pInfo )
{
	RESOURCEDATA* pData = new RESOURCEDATA;
	gui::CComponentOptions option = pInfo->Options;

	pData->_strName = pInfo->name.c_str();

	if( option.Exists( "handle" ) )
		pData->_nHandle = option.GetValueAsInt( "handle" );
	if( option.Exists( "file" ) ) 
		pData->_strFileName = option.GetValue( "file" ).c_str();

	m_listResourceData.push_back( pData );
}

void CDataManager::LoadSurfaceData( gui::CSurfaceFileParser::ComponentInfo* pInfo )
{
	SURFACEDATA* pData = new SURFACEDATA;
	gui::CComponentOptions option = pInfo->Options;

	pData->_strName = pInfo->name.c_str();

	if( option.Exists( "resource_file" ) )
		pData->_strResourceFileName = option.GetValue( "resource_file" ).c_str();
	if( option.Exists( "resource_name" ) )
		pData->_strResourceName = option.GetValue( "resource_name" ).c_str();
	
	if( option.Exists( "x" ) )
		pData->_rcCoord.left = option.GetValueAsInt( "x" );
	if( option.Exists( "y" ) )
		pData->_rcCoord.top = option.GetValueAsInt( "y" );
	if( option.Exists( "width" ) )
		pData->_rcCoord.right = option.GetValueAsInt( "width" );
	if( option.Exists( "height" ) )
		pData->_rcCoord.bottom = option.GetValueAsInt( "height" );
	
	if( option.Exists( "color_red" ) )
		pData->_cRed = option.GetValueAsInt( "color_red" );
	if( option.Exists( "color_green" ) )
		pData->_cGreen = option.GetValueAsInt( "color_green" );
	if( option.Exists( "color_blue" ) )
		pData->_cBlue = option.GetValueAsInt( "color_blue" );
	if( option.Exists( "color_alpha" ) )
		pData->_cAlpha = option.GetValueAsInt( "color_alpha" );

	if( option.Exists( "uv_left" ) )
		pData->_rcUV.left = option.GetValueAsInt( "uv_left" );
	if( option.Exists( "uv_top" ) )
		pData->_rcUV.top = option.GetValueAsInt( "uv_top" );
	if( option.Exists( "uv_right" ) )
		pData->_rcUV.right = option.GetValueAsInt( "uv_right" );
	if( option.Exists( "uv_bottom" ) )
		pData->_rcUV.bottom = option.GetValueAsInt( "uv_bottom" );
	
	if( option.Exists( "handle" ) )
		pData->_nHandle = option.GetValueAsInt( "handle" );
	if( option.Exists( "blend" ) )
		pData->_strBlendFunction = option.GetValue( "blend" ).c_str();

	m_listSurfaceData.push_back( pData );
}

void CDataManager::SetWorkDir( CString& strDir )
{
	m_strWorkDir = strDir;
	g_pFileInfoView->SetWorkDirText( strDir );
}

BOOL CDataManager::LoadConfig()
{
	FILE* pFile = NULL;
	char buf[256];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	errno_t eValue = fopen_s( &pFile, CONFIGFILE, "rb" );
	if( eValue )
		return FALSE;
#else
	pFile = fopen( (LPCTSTR)strFilename, "rb" );
	if( !pFile )
		return FALSE;

#endif

	fread( buf, 256, 1, pFile );
	SetWorkDir( CString( buf ) );

	g_pFileView->GetTreeCtrl()->SetSelPath( buf );

	fclose( pFile );

	return TRUE;
}

BOOL CDataManager::SaveConfig()
{
	FILE* pFile = NULL;
	char buf[256];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	errno_t eValue = fopen_s( &pFile, CONFIGFILE, "wb" );
	if( eValue )
		return FALSE;
	
	strcpy_s( buf, 256, (LPCTSTR)m_strWorkDir );
#else
	pFile = fopen( (LPCTSTR)strFilename, "wb" );
	if( !pFile )
		return FALSE;

	strcpy( buf, (LPCTSTR)m_strWorkDir );
#endif

	fwrite( buf, 256, 1, pFile );

	fclose( pFile );	

	return TRUE;
}