#include "StdAfx.h"
#include "DirTreeCtrlMSC.h"
#include "GUIEditorDoc.h"
#include "GUIResourceView.h"
#include "GUIFileInfoView.h"

CDirTreeCtrlMSC::CDirTreeCtrlMSC(void)
{
}

CDirTreeCtrlMSC::~CDirTreeCtrlMSC(void)
{
}

BEGIN_MESSAGE_MAP(CDirTreeCtrlMSC, CDirTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDirTreeCtrlMSC::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CDirTreeCtrlMSC::OnNMRclick)
END_MESSAGE_MAP()

void CDirTreeCtrlMSC::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString strFullPath = (LPCTSTR)GetFullPath( pNMTreeView->itemNew.hItem );
	CString strSubPath = (LPCTSTR)GetSubPath( (LPCTSTR)strFullPath );
	
	//strFullPath.MakeLower();
	//strSubPath.MakeLower();

	LoadDevILImage( strFullPath, strSubPath );
	LoadRsr( strFullPath, strSubPath );
	LoadSrf( strFullPath, strSubPath );
	

	m_strCurrentPath = strFullPath;

	*pResult = 0;
}

void CDirTreeCtrlMSC::LoadDevILImage( CString& strFullPath, CString& strSubPath )
{
	if( m_extFilter.GetType( (LPCTSTR)strFullPath ) == CExtFilter::IMAGE )
	{
		g_pResourceView->LoadImage( strFullPath, strSubPath );
	}
}

void CDirTreeCtrlMSC::LoadRsr( CString& strFullPath, CString& strSubPath )
{
	if( m_extFilter.GetType( (LPCTSTR)strFullPath ) == CExtFilter::RESOURCE )
	{
		g_pDocument->GetDataManager().LoadRsrFile( strFullPath, strSubPath, TRUE );
	}
}

void CDirTreeCtrlMSC::LoadSrf( CString& strFullPath, CString& strSubPath )
{
	if( m_extFilter.GetType( (LPCTSTR)strFullPath ) == CExtFilter::SURFACE )
	{
		g_pDocument->GetDataManager().LoadSrfFile( strFullPath, strSubPath );
	}
}

CString CDirTreeCtrlMSC::GetFullDir( CString& strFullPath )
{
	CString strSubPath = GetSubPath( (LPCTSTR)strFullPath );
		
	if( strSubPath.Find( '.' ) == -1 )
		return strFullPath;

	CString strPath;
	INT		nPos;

	nPos = strFullPath.ReverseFind( '\\' );
	
	if ( nPos != -1 )
			strPath = strFullPath.Left( nPos );
	
	return strPath;
}

void CDirTreeCtrlMSC::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString strFullDir = GetFullDir( m_strCurrentPath );

	g_pDocument->GetDataManager().SetWorkDir( strFullDir );
	
	*pResult = 0;
}
