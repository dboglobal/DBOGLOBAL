#pragma once

#include "gui_precomp.h"
#include "gui_resourcefilepaser.h"
#include "gui_surfacefilepaser.h"
#include "gui_resourcepage.h"
#include "gui_surfacepage.h"
#include <list>

typedef struct _CONFIGDATA
{
	char	_WorkData[256];

}CONFIGDATA;

typedef struct _RESOURCEDATA
{
	CString	_strName;
	INT		_nHandle;
	CString _strFileName;

}RESOURCEDATA;

typedef struct _SURFACEDATA
{
	CString	_strName;
	CString _strResourceFileName;
	CString _strResourceName;

	CRect	_rcCoord;			// Width, Height는 Right, Bottom에 저장. 

	INT		_cRed;
	INT		_cGreen;
	INT		_cBlue;
	INT		_cAlpha;

	CRect	_rcUV;

	INT		_nHandle;
	CString	_strBlendFunction;

}SURFACEDATA;

class CDataManager
{
public:

	enum TYPE { RSR = 0, SRF, FRM };

	typedef std::list< RESOURCEDATA* >	listResourceData;
	typedef std::list< SURFACEDATA* >	listSurfaceData;

	CDataManager(void);
	~CDataManager(void);

	// Config File Load
	BOOL	LoadConfig();
	BOOL	SaveConfig();

	listResourceData&	GetResourceList() { return m_listResourceData; }
	listSurfaceData&	GetSurfaceList() { return m_listSurfaceData; }

	INT		GetResourceDataCount() { return (INT)m_listResourceData.size(); }
	INT		GetSurfaceDataCount() { return (INT)m_listSurfaceData.size(); }

	BOOL	IsEmptyResourceList() { return m_listResourceData.empty(); }
	BOOL	IsEmptySurfaceList() { return m_listSurfaceData.empty(); }

	BOOL	AddResourceData( RESOURCEDATA* pResourceData );
	BOOL	AddSurfaceData( SURFACEDATA* pSurfaceData );

	BOOL	RemoveResourceData( INT nIndex );	// 1 base Index
	BOOL	RemoveSurfaceData( INT nIndex );

	RESOURCEDATA*	GetResourceData( INT nIndex );
	RESOURCEDATA*	GetResourceData( CString strName );
	SURFACEDATA*	GetSurfaceData( INT nIndex );
	SURFACEDATA*	GetSurfaceData( CString strName );

	void	ClearResourceList();
	void	ClearSurfaceList();

	BOOL	IsRsrFileLoad() { return m_bRsrFileLoad; }
	BOOL	IsSrfFileLoad() { return m_bSrfFileLoad; }

	CString GetRsrFileName() { return m_strRsrFileName; }
	CString GetStfFileName() { return m_strSrfFileName; }

	void	SetWorkDir( CString& strDir );
	CString	GetWorkDir() { return m_strWorkDir; }  

	RESOURCEDATA*	GetResourceDataFromFilename( CString strFilename );
	
	// I/O
	BOOL	LoadRsrFile( CString& strFullPath, CString& strSubPath, BOOL bTabChange = FALSE );
	BOOL	LoadSrfFile( CString& strFullPath, CString& strSubPath );
	BOOL	SaveRsrFile();
	BOOL	SaveSrfFile();
	void	ClearRsrFile();
	void	ClearSrfFile();

private:

	BOOL	FileDialogOpen( CString* pFilename, TYPE eFileType, BOOL bLoad ); 
	void	LoadResourceList();
	void	LoadSurfaceList();
	void	LoadResourceData( gui::CResourceFileParser::ComponentInfo* pInfo );
	void	LoadSurfaceData( gui::CSurfaceFileParser::ComponentInfo* pInfo );

private:

	listResourceData	m_listResourceData;
	listSurfaceData		m_listSurfaceData;

	gui::CResourceManager	m_mgrResource;
	gui::CSurfaceManager	m_mgrSurface;

	BOOL	m_bRsrFileLoad;						// 파일로드여부.
	BOOL	m_bSrfFileLoad;							

	CString	m_strRsrFileName;					// 로드된 파일 이름. 이름만.
	CString m_strSrfFileName;

	CString m_strWorkDir;						// Work Directory;
};
