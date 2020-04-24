#include "StdAfx.h"
#include "BackManager.h"

// Core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"

// Gui
#include "gui_renderer.h"

// Editor
#include "2DParticleGlobal.h"
#include "2DParticleApp.h"

// Panes
#include "SystemPane.h"


CBackManager::CBackManager(void)
{	
	// Texture Back filename
	::ZeroMemory( m_tcBackName, sizeof( TCHAR ) * dBACKGROUND_TEX_NAME_BUFFER );
}

CBackManager::~CBackManager(void)
{
}

void CBackManager::LoadBack( const TCHAR* texname, int nOffsetX, int nOffsetY )
{
	// 이름이 다르다면
	if( _tcscmp( texname, m_tcBackName ) != 0 )
	{
		m_TexBack.Unload();
		if( !m_TexBack.Load( texname ) )
		{
			TCHAR tErrorBuffer[dBACKGROUND_TEX_NAME_BUFFER];
			_stprintf_s( tErrorBuffer, dBACKGROUND_TEX_NAME_BUFFER, _T("Loading to failed : \n%s"), texname );
			::AfxMessageBox( tErrorBuffer );
			INIT_PLANESNAPSHOT( m_SnapBack );

			return;
		}
	}

	_stprintf_s( m_tcBackName, dBACKGROUND_TEX_NAME_BUFFER, _T("%s"), texname );
	
	INIT_PLANESNAPSHOT( m_SnapBack );
	m_SnapBack.rtRect.SetRectWH( nOffsetX, nOffsetY, nOffsetX + m_TexBack.GetWidth(), nOffsetY + m_TexBack.GetHeight() );
}

const TCHAR* CBackManager::GetBackName()
{
	return m_tcBackName;
}

void CBackManager::Render()
{
	g_GuiRenderer.RenderQueue( &m_SnapBack, &m_TexBack );
}