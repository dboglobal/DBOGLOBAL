#ifndef _PATHMANAGER_H_
#define _PATHMANAGER_H_

#pragma once

//class CPathPoint;
#include "gui_precomp.h"
#include "guicore_precomp.h"

#include "gui_particlepath.h"
#include "gui_particle_doc.h"

#include "gui_line_renderer.h"

class CPathPoint;

class CPathManager
{
public:
	~CPathManager();

	void Destroy();

	void SetControlPoint( gui::SCtrlPoint* pCtrlPoint, int nCount );

	// SetActive/DeActive
	void SetActivePathItem( gui::CValueItem* pValueItem );
	void DeActivePathItem();

	// Build Active
	void BuildActivePathItem();
	
	// Update/Save
	void UpdateCurrentPathData();
	void SaveCurrentPathData();

	// Tooltip
	void ShowToolTip( int nPosX, int nPosY, TCHAR* tcToolTip, BOOL bShow = TRUE );
	
	// Offset이 변경되었을 때 호출
	void RefreshOffsetData();
	
	// Render
	void Render();

	// Getter/Setter
public:
	int GetCount();
	// Path의 총 시간을 세팅
	void SetTotalTime( float fTime );

	static CPathManager* GetInstance()
	{
		static CPathManager instance;
		return &instance;
	}

	///< Line
	void	CreateActivePathLine();
	void	DestroyActivePathLine();

protected:
	// Create
	CPathManager();

	void Create( int nCount );

protected:
	CPathPoint*			m_pPath;					///< 점의 렌더 아이템
	
	gui::SCtrlPoint*	m_pCtrlPoint;				///< 현재 Ctrl Point의 Data
	gui::SPathInfo		m_sPathInfo;				///< 패스의 정보
	gui::CParticlePath	m_GuiPath;					///< 라이브러리 패스
	
	gui::CValueItem*	m_pActiveValueItem;			///< 현재 적용중인 Path

	gui::CStaticBox*	m_pStbToolTip;				///< ToolTip의 박스

	gui::CGuiLine*		m_pLinePath;				///< 패스의 라인

	int					m_nCount;						///< 패스 갯수
};

static CPathManager* GetPathManager()
{
	return CPathManager::GetInstance();
}

#endif//_PATHMANAGER_H_