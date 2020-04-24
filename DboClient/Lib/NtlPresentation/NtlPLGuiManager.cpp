#include "precomp_ntlpresentation.h"

// core
#include "NtlDebug.h"
#include "NtlProfiler.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiGroup.h"
#include "NtlPLGuiManager.h"




CNtlPLGuiManager::CNtlPLGuiManager()
:m_pGuiGroup(NULL)
{
	m_pResMgr = 0;
	m_pSurMgr = 0;
	m_pCompMgr = 0;
	m_pGuiMgr = 0;
	m_hListener.m_nHandle = -1;

	m_bRender = TRUE;
}

CNtlPLGuiManager::~CNtlPLGuiManager()
{
	
}

CNtlPLGuiManager* CNtlPLGuiManager::GetInstance(void)
{
	static CNtlPLGuiManager Instance;
	return &Instance;
}

RwBool CNtlPLGuiManager::Create(RwInt32 iWidth, RwInt32 iHeight)
{
	NTL_FUNCTION("CNtlPLGuiManager::Create");

	m_pResMgr = NTL_NEW gui::CResourceManager;
	m_pSurMgr = NTL_NEW gui::CSurfaceManager(m_pResMgr);
	m_pCompMgr = NTL_NEW gui::CComponentManager;
	m_pGuiMgr = NTL_NEW gui::CGUIManager(CRectangle(0, 0, iWidth, iHeight));

	m_hListener = CEventListener_Win32::AddListener (m_pGuiMgr ,&gui::CGUIManager::Win32EventProc);

	NTL_RETURN(TRUE);
}

void CNtlPLGuiManager::Destroy(void)
{
	MapGui::iterator it;
	for(it = m_mapGui.begin(); it != m_mapGui.end(); ++it)
	{
		CNtlPLGui *pGui = (*it).second;
		pGui->Destroy(); 
		NTL_DELETE(pGui);
	}

	RemoveAllUpdateBeforeFunc();
	RemoveAllUpdateFunc();

	if(m_pGuiMgr)
	{
		NTL_DELETE(m_pGuiMgr);
	}

	if(m_pCompMgr)
	{
		NTL_DELETE(m_pCompMgr);
	}

	if(m_pSurMgr)
	{
		NTL_DELETE(m_pSurMgr);
	}

	if(m_pResMgr)
	{
		NTL_DELETE(m_pResMgr);
	}

	// 여기는 더 점검을 해 봐야 한다...
	// 왜 이렇게 만들었쥐? 이해가 안되네... 내가 하고서리...;; 
	CEventListener_Win32::RemoveAll();

	/*
	if(m_hListener.m_nHandle >= 0)
	{
		CEventListener_Win32::RemoveListener (m_hListener);
		m_hListener.m_nHandle = -1;
	}
	*/
}

void CNtlPLGuiManager::Update(RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLGuiManager::Update")

	CEventTimer::KeepAlive();

	MapUpdateGui::iterator it;

	if( !m_mapRemoveUpdateGui.empty() )
	{
		MapUpdateGui::iterator deleteit;

		for( it = m_mapRemoveUpdateGui.begin() ; it != m_mapRemoveUpdateGui.end() ; ++it )
		{
			deleteit = m_mapUpdateGui.find( (*it).first );
			m_mapUpdateGui.erase( deleteit );
		}

		m_mapRemoveUpdateGui.clear();
	}

	for( it = m_mapUpdateGui.begin() ; it != m_mapUpdateGui.end() ; ++it )
	{
		(*it).first->Update( fElapsed );
	}

	NTL_EPROFILE()
}

void CNtlPLGuiManager::UpdateBeforeCamera(RwReal fElapsed)
{
	MapUpdateGui::iterator it;

	if( !m_mapRemoveUpdateBeforeGui.empty() )
	{
		MapUpdateGui::iterator deleteit;

		for( it = m_mapRemoveUpdateBeforeGui.begin() ; it != m_mapRemoveUpdateBeforeGui.end() ; ++it )
		{
			deleteit = m_mapUpdateBeforeGui.find( (*it).first );
			m_mapUpdateBeforeGui.erase( deleteit );
		}

		m_mapRemoveUpdateBeforeGui.clear();
	}
	
	for( it = m_mapUpdateBeforeGui.begin() ; it != m_mapUpdateBeforeGui.end() ; ++it )
	{
		(*it).first->UpdateBeforeCamera( fElapsed );
	}
}

void CNtlPLGuiManager::Render(void)
{
	NTL_SPROFILE("CNtlPLGuiManager::Render")

	if(!m_bRender)
	{
		NTL_RPROFILE_VOID()
	}

	g_GuiRenderer.ResetRender();

	if(m_pGuiMgr)
		m_pGuiMgr->Show();

	g_GuiRenderer.Render();

	NTL_EPROFILE()
}

void CNtlPLGuiManager::PostRender(void)
{
}

CNtlPLGui* CNtlPLGuiManager::FindPLGui(const RwChar *pName)
{
	MapGui::iterator it = m_mapGui.find(pName);
	if(it == m_mapGui.end())
		return NULL;

	return (*it).second;
}

RwBool CNtlPLGuiManager::LoadUserOption(CNtlSerializer& s)
{
	// gui total size
	RwInt32 iGuiSize;
	s >> iGuiSize;

	// gui 
	CNtlSerializer gui_s(iGuiSize, iGuiSize/10);
	s.Out(gui_s, iGuiSize);

	// gui unit
	RwInt32		iPLGuiNameLen, iUnitSize;
	RwChar		chName[64];
	CNtlPLGui	*pPLGui;

	CNtlSerializer unit_s(256, 128);

	while(1)
	{
		if(gui_s.IsEmpty())
			break;

		// pl gui name
		memset(chName, 0, 64);
		gui_s >> iPLGuiNameLen;
		gui_s.Out(chName, iPLGuiNameLen);

		gui_s >> iUnitSize;
		unit_s.Refresh();
		gui_s.Out(unit_s, iUnitSize);

		// PL gui를 찾는다.
		pPLGui = FindPLGui(chName);
		if(pPLGui)
		{
			if(!pPLGui->LoadUserOption(unit_s))
				return FALSE;
		}
	}

	return TRUE;
}

RwBool CNtlPLGuiManager::SaveUserOption(CNtlSerializer& s)
{
	CNtlSerializer gui_s(1024*10, 1024*5);
	CNtlSerializer unit_s(256, 128);

	CNtlPLGui *pPLGui;
	MapGui::iterator it;

	for(it = m_mapGui.begin(); it != m_mapGui.end(); it++)
	{
		pPLGui = (*it).second;
		if(pPLGui->IsEnableSerialize())
		{
			unit_s.Refresh();

			RwInt32 iUnitNameSize = strlen(pPLGui->GetName());
			NTL_ASSERT(iUnitNameSize != 0, "CNtlPLGuiManager::SaveUserOption => PLGui name is null !!!");

			// unit 이름.
			gui_s << iUnitNameSize;
			gui_s << pPLGui->GetName();

			pPLGui->SaveUserOption(unit_s);
		
			// unit data
			gui_s << unit_s.GetDataSize();
			gui_s.In(unit_s.GetData(), unit_s.GetDataSize());
		}
	}

	s << gui_s.GetDataSize();
	s.In(gui_s.GetData(), gui_s.GetDataSize());

	return TRUE;
}

void CNtlPLGuiManager::AddGui(CNtlPLGui *pGui)
{
	RwInt32 i = 0;
	RwChar buf[256] = { 0, };
	MapGui::iterator it;
	strcpy_s( buf, 256, pGui->GetName() );
	
	while( 1 )
	{
		if( !FindPLGui( buf ) )
		{
			pGui->SetName( buf );
			break;
		}
		
		sprintf( buf, "%s_%d", pGui->GetName(), i );
		++i;
	}

	m_mapGui[buf] = pGui;
}

void CNtlPLGuiManager::RemoveGui(CNtlPLGui *pGui)
{
	MapGui::iterator it;
	const RwChar *pName = pGui->GetName(); 
	it = m_mapGui.find(pName);
	if(it == m_mapGui.end())
		return;

	m_mapGui.erase(it); 
}

void CNtlPLGuiManager::AddUpdateFunc( CNtlPLGui* pUpdateGui )
{
	RwInt32 nCount = 1;

	MapUpdateGui::iterator it = m_mapRemoveUpdateGui.find( pUpdateGui );
	if( it != m_mapRemoveUpdateGui.end() )
	{
		m_mapRemoveUpdateGui.erase( it );
		m_mapUpdateGui[pUpdateGui] = 1;
	}
	else
	{
		it = m_mapUpdateGui.find( pUpdateGui );
		if( it == m_mapUpdateGui.end() )
			m_mapUpdateGui[pUpdateGui] = 1;
		else
			nCount = ++(*it).second;
	}	
}

void CNtlPLGuiManager::RemoveUpdateFunc( CNtlPLGui* pUpdateGui )
{
	MapUpdateGui::iterator it = m_mapUpdateGui.find( pUpdateGui );
	if( it == m_mapUpdateGui.end() )
		return;

	if( --(*it).second <= 0 )
		m_mapRemoveUpdateGui[pUpdateGui] = 0;
}

void CNtlPLGuiManager::RemoveUpdateFuncWithoutRefCheck( CNtlPLGui* pUpdateGui )
{
	MapUpdateGui::iterator it = m_mapUpdateGui.find( pUpdateGui );
	if( it == m_mapUpdateGui.end() )
		return;

	(*it).second = 0;
	m_mapRemoveUpdateGui[pUpdateGui] = 0;
}

void CNtlPLGuiManager::RemoveAllUpdateFunc(VOID)
{
	m_mapUpdateGui.clear();
	m_mapRemoveUpdateGui.clear();
}

void CNtlPLGuiManager::AddUpdateBeforeFunc( CNtlPLGui* pUpdateGui )
{
	RwInt32 nCount = 1;

	MapUpdateGui::iterator it = m_mapRemoveUpdateGui.find( pUpdateGui );
	if( it != m_mapRemoveUpdateGui.end() )
	{
		m_mapRemoveUpdateGui.erase( it );
		m_mapUpdateGui[pUpdateGui] = 1;
	}
	else
	{
		it = m_mapUpdateBeforeGui.find( pUpdateGui );
		if( it == m_mapUpdateBeforeGui.end() )
			m_mapUpdateBeforeGui[pUpdateGui] = 1;
		else
			nCount = ++(*it).second;
	}
}

void CNtlPLGuiManager::RemoveUpdateBeforeFunc( CNtlPLGui* pUpdateGui )
{
	MapUpdateGui::iterator it = m_mapUpdateBeforeGui.find( pUpdateGui );
	if( it == m_mapUpdateBeforeGui.end() )
		return;

	if( --(*it).second <= 0 )
		m_mapRemoveUpdateBeforeGui[pUpdateGui] = 0;
}

void CNtlPLGuiManager::RemoveUpdateBeforeFuncWithoutRefCheck( CNtlPLGui* pUpdateGui )
{
	MapUpdateGui::iterator it = m_mapUpdateBeforeGui.find( pUpdateGui );
	if( it == m_mapUpdateBeforeGui.end() )
		return;

	(*it).second = 0;
	m_mapRemoveUpdateBeforeGui[pUpdateGui] = 0;
}

void CNtlPLGuiManager::RemoveAllUpdateBeforeFunc(VOID)
{
	m_mapUpdateBeforeGui.clear();
	m_mapRemoveUpdateBeforeGui.clear();
}


RwBool CNtlPLGuiManager::IsGuiFocus(void)
{
	if( m_pGuiMgr == NULL ||
		  ( m_pGuiMgr->GetFocus() == NULL || m_pGuiMgr->GetFocus() == m_pGuiMgr ) &&
		  ( m_pGuiMgr->GetCapturedMouse() == NULL || m_pGuiMgr->GetCapturedMouse() == m_pGuiMgr ) &&
		  ( m_pGuiMgr->GetMouseOver() == NULL || m_pGuiMgr->GetMouseOver() == m_pGuiMgr ) )
		return FALSE;

	return TRUE;
}

void CNtlPLGuiManager::RenderEnable(RwBool bRender /*= TRUE*/)
{
	m_bRender = bRender;
}

void CNtlPLGuiManager::CreatePLGuiGroup()
{
	m_pGuiGroup = NTL_NEW CNtlPLGuiGroup;
	if(!m_pGuiGroup->Create())
	{
		NTL_DELETE(m_pGuiGroup);
	}
}

void CNtlPLGuiManager::DestroyPLGuiGroup()
{
	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy();
		NTL_DELETE(m_pGuiGroup);
	}
}