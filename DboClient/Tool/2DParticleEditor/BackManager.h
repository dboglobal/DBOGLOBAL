#ifndef __BACK_MANAGER_H__
#define __BACK_MANAGER_H__

#pragma once

#include "gui_precomp.h"
#include "guicore_precomp.h"

// Filename Buffer
#define dBACKGROUND_TEX_NAME_BUFFER 1024

/**
* \brief Background manager
*/
class CBackManager
{
public:
	virtual ~CBackManager(void);

	static CBackManager* GetInstance()
	{
		static CBackManager instance;
		return &instance;
	}

	// Load
	void			LoadBack( const TCHAR* texname, int nOffsetX, int nOffsetY );
	
	// BackName
	const TCHAR*	GetBackName();

	// Render
	void			Render();

protected:
	CBackManager(void);

	// Texture Back
	gui::CTexture				m_TexBack;
	PlaneSnapShot				m_SnapBack;

	// Texture Back filename
	TCHAR						m_tcBackName[dBACKGROUND_TEX_NAME_BUFFER];
};

static CBackManager* GetBackManager()
{
	return CBackManager::GetInstance();
}

#endif