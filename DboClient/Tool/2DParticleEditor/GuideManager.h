#ifndef __GUIDE_MANAGER_H__
#define __GUIDE_MANAGER_H__

// Pre-Compile
#include "gui_precomp.h"
#include "guicore_precomp.h"

#include "gui_line_renderer.h"

#pragma once

/**
* \brief TOOL상의 Guide Line을 그리기 위한 매니저 클래스
*/
class CGuideManager
{
public:
	enum eCENTER_LINE
	{
		eCENTER_LINE_HORIZONTAL,	///< 가로
		eCENTER_LINE_VERTICAL,		///< 세로

		eCENTER_LINE_NUMS			///< 가이드라인 갯수
	};

	CGuideManager(void);
	virtual ~CGuideManager(void);

	static CGuideManager* GetInstance(VOID)
	{
		static CGuideManager instance;
		return &instance;
	}

	// Guide Line ( 중앙선 )
	VOID			SetCenter( float fX, float fY );

	VOID			SetBox( float fX, float fY, float fWidth, float fHeight );
	VOID			ShowBox( BOOL bShow );
	
	VOID			Render();

protected:
	gui::CGuiLine	m_lineCenter[eCENTER_LINE_NUMS];			///< 화면의 중앙선 ( + 표시 )

	float			m_fCenterX;
	float			m_fCenterY;

	gui::CGuiLine	m_lineBox;									///< Emitter가 Box Type일 경우 렌더링 해주는 인터페이스
	BOOL			m_bLineBox;

	float			m_fBoxPosX;
	float			m_fBoxPosY;
	float			m_fBoxWidth;
	float			m_fBoxHeight;
};

static CGuideManager* GetGuideManager(void)
{
	return CGuideManager::GetInstance();
}

#endif