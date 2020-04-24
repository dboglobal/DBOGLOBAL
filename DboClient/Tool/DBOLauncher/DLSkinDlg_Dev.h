/*****************************************************************************
*
* File			: DLSkinDlg_Dev.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once

#include "resource.h"
#include "DLSkinBaseDlg.h"
#include "Explorer_information.h"
#include "OutputBox.h"
#include "ImageTextButton.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinDlg_Dev
//
//////////////////////////////////////////////////////////////////////////


class CDLSkinDlg_Dev : public CDLSkinBaseDlg
{
public:
	#define MAX_PROGRESS_RANGE (1000)

protected:
	CExplorer_information		m_ctrExplorerInfo;

	COutputBox					m_ctrDownload;
	COutputBox					m_ctrPatch;
	COutputBox					m_ctrDownloadInfo;    
	COutputBox					m_ctrLog;

	CImageTextButton			m_ctrCancel;
	CImageTextButton			m_ctrGameStart;
	CImageTextButton			m_ctrGameIntegrity;

	int							m_nIDDownloadProgress;
	int							m_nIDPatchProgress;

	HICON						m_hIcon;

	unsigned int				m_uiTotalFilesInUpdateFile;
	unsigned int				m_uiCurFilesInUpdateFile;

public:
	CDLSkinDlg_Dev( void );
	virtual ~CDLSkinDlg_Dev( void );

public:
	virtual bool				CreateDlg( void );

	virtual void				DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	virtual void				DoDataExchange( CDataExchange* pDX );
	virtual BOOL				OnInitDialog( void );
	virtual BOOL				DestroyWindow( void );

	afx_msg void				OnPaint( void );
	afx_msg HCURSOR				OnQueryDragIcon( void );
	afx_msg void				OnClose( void );
	afx_msg void				OnBnClickedDevGameStart( void );
	afx_msg void				OnBnClickedDevCancel( void );
	afx_msg void				OnBnClickedDevIntegrityCheck( void );

	DECLARE_MESSAGE_MAP()
};
