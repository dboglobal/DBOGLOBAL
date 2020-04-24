#include "gui_precomp.h"
#include "gui_guimanager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic construction:

START_GUI

#define GUI_DEFAULT_SIZE_WIDTH	1024
#define GUI_DEFAULT_SIZE_HEIGHT	768

float CGUIManager_Generic::m_fZoomFactor_Width = 1.0f;
float CGUIManager_Generic::m_fZoomFactor_Height = 1.0f;


CGUIManager_Generic::CGUIManager_Generic(CGUIManager *pOwner)
:
	m_pOwner(pOwner),
	m_pParent(NULL),
	m_nCurrTabID(0),
	m_pCompFocus(pOwner),
	m_pCompCaptureMouse(NULL),
	m_pCompMouseOver(NULL),
	m_pCompDrag(NULL),
	m_nDataDrag(NULL),
	m_bDrag(false)
{
	m_pOwner->SetGuiManager(m_pOwner);
	m_SlotBeforeRemove = m_pOwner->SigBeforeRemove().Connect(this, &CGUIManager_Generic::OnBeforeRemove);
}

CGUIManager_Generic::~CGUIManager_Generic()
{

}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic attributes:

CComponent *CGUIManager_Generic::GetCapturedMouse(VOID)
{
	return m_pCompCaptureMouse;
}

CComponent *CGUIManager_Generic::GetFocus(VOID)
{
	return m_pCompFocus;
}

CComponent *CGUIManager_Generic::GetMouseOver(VOID)
{
	return m_pCompMouseOver;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIManager_Generic operations:

VOID CGUIManager_Generic::AddPopup(CComponent *pComponent)
{
	std::list<CComponent*>::iterator begin,end,the;
	begin = m_listPopup.begin();
	end = m_listPopup.end();

	for(the=begin ; the!=end ; the++)
	{
		if((*the) == pComponent)
			return;
	}
	
	m_listPopup.push_back(pComponent);
}

VOID CGUIManager_Generic::RemovePopup(CComponent *pComponent)
{
	std::list<CComponent*>::iterator the;
	for(the=m_listPopup.begin() ; 
			the!=m_listPopup.end() ; 
			the++)
			{
				if(*the == pComponent)
				{
					m_listPopup.erase(the);
					return;
				}
			}
}


VOID CGUIManager_Generic::SetFocus(CComponent *pComponent)
{
	if (m_pCompFocus == pComponent) 
		return;

	m_pCompFocus->SigLostFocus()();
	m_pCompFocus = pComponent;
	m_pCompFocus->SigGotFocus()();
}

VOID CGUIManager_Generic::Quit(VOID)
{
	m_bQuitRun = true;
}

VOID CGUIManager_Generic::Run(VOID)
{
	m_bQuitRun = false;
	while (!m_bQuitRun)
	{
		Show();
	}
}

VOID CGUIManager_Generic::Show()
{
	if (m_pParent)
		m_pParent->Show();

	m_pOwner->Paint();
}

bool CGUIManager_Generic::IsEnableInput(VOID)
{
	return ( m_mapDisableInput.size() == 0 ) ? true : false;
}

VOID CGUIManager_Generic::EnableInput(void* key)
{
	DWORD64 key64 = (DWORD64)key;

	DISABLE_INPUT_IT it = m_mapDisableInput.find( key64 );

	if( it != m_mapDisableInput.end() )
	{
		(*it).second--;

		if( (*it).second <= 0 )
			m_mapDisableInput.erase( it );
	}	
}

VOID CGUIManager_Generic::DisableInput(void* key)
{
	DWORD64 key64 = (DWORD64)key;
	INT	nCount = 0;

	DISABLE_INPUT_IT it = m_mapDisableInput.find( key64 );
	
	if( it != m_mapDisableInput.end() )
	{
		(*it).second++;
	}
	else
	{
		m_mapDisableInput[key64] = 1;
	}	
}

VOID CGUIManager_Generic::GuiCaptureMouse(CComponent *pComponent)
{
	m_pCompCaptureMouse = pComponent;
}

VOID CGUIManager_Generic::GuiReleaseMouse(VOID)
{
	m_pCompCaptureMouse = NULL;
}

CComponent *CGUIManager_Generic::FindFocusComponent(CInputDevice *pDevice, const CKey &key)
{
	CComponent *focus_component = m_pCompFocus;

	if (m_pCompCaptureMouse)
		m_pCompFocus = m_pCompCaptureMouse;
	else if (pDevice->m_Type == CInputDevice::TYPE_MOUSE)
		m_pCompFocus = m_pOwner->GetComponentAt((int)key.m_fX - m_pOwner->GetPosition().left,(int)key.m_fY - m_pOwner->GetPosition().top);

	return m_pCompFocus;
}

VOID CGUIManager_Generic::OnKeyPress(CInputDevice *pDevice,const CKey &key)
{
	if (!IsEnableInput())
		return;

	CComponent *focus_component = m_pCompFocus;
	if(focus_component == NULL)
	{
		m_pOwner->SigCaptureKeyDown()(focus_component, pDevice, key);
		return;
	}
	/*
	if (key.m_nID == CL_KEY_TAB)
	{
		if (CL_Keyboard::get_keycode(CL_KEY_LSHIFT) || CL_Keyboard::get_keycode(CL_KEY_RSHIFT))
			focus_component = tab_previous();
		else
			focus_component = tab_next();
	}
  */

	// If component is disabled, do not send event:
	if (focus_component->IsEnabled() == false)
	{
		m_pOwner->SigCaptureKeyDown()(focus_component, pDevice, key);
		return;
	}

	focus_component->SigKeyDown()(focus_component, pDevice, key);
	m_pOwner->SigCaptureKeyDown()(focus_component, pDevice, key);
}

VOID CGUIManager_Generic::OnCharPress(CInputDevice *pDevice, const CKey& key)
{
	//CComponent *focus_component = m_pCompFocus;
//	m_pOwner->SigCaptureKeyDown()(focus_component, pDevice, key);
}	

VOID CGUIManager_Generic::OnKeyRelease(CInputDevice *pDevice,const CKey &key)
{
	if (!IsEnableInput()) 
		return;

	CComponent *focus_component = m_pCompFocus;
	if(focus_component == NULL)
	{
		m_pOwner->SigCaptureKeyUp()(focus_component, pDevice, key);
		return;
	}

	// Do not pass tab support down through the gui:
//	if (key.m_nID == CL_KEY_TAB)
//		return;

	// If component is disabled, do not send event:
	if (focus_component->IsEnabled() == false)
	{
		m_pOwner->SigCaptureKeyUp()(focus_component, pDevice, key);
		return;
	}

	// Send key up event:
	focus_component->SigKeyUp()(focus_component, pDevice, key);
	m_pOwner->SigCaptureKeyUp()(focus_component, pDevice, key);
}

VOID CGUIManager_Generic::OnMouseMove(int nKeys,int nX,int nY)
{
	if (!IsEnableInput()) 
		return;

	if( m_pCompDrag && !m_bDrag && (nKeys & UD_MK_LBUTTON) )
	{
		int nDragDelta = (m_ptDragStartPos.x - nX)*(m_ptDragStartPos.x - nX) + (m_ptDragStartPos.y - nY)*(m_ptDragStartPos.y - nY);

		if(nDragDelta >= 200)
		{
			CKey newkey;
			newkey.m_nID = UD_LEFT_BUTTON;
			newkey.m_nState = CKey::PRESSED;
			CPos ptClientPos = m_pCompDrag->ScreenToClient(m_ptDragStartPos);
			newkey.m_fX = (float)ptClientPos.x;
			newkey.m_fY = (float)ptClientPos.y;

			m_pCompDrag->SigDragDropQuery()(newkey, m_nDataDrag);  
			m_bDrag = true;
		}
	}

	CPos pos;
	pos.x = nX;
	pos.y = nY;

	CMouse::m_nX = nX;
	CMouse::m_nY = nY;

	m_pOwner->SigTransformCoords()(nX, nY);
	m_SigCaptureMouseMove( nKeys, nX, nY );

	CComponent *focus_component = m_pCompCaptureMouse;
	if (!m_pCompCaptureMouse)
	{
		if(m_listPopup.empty())
		{
			focus_component = m_pOwner->GetComponentReverseAt(
				nX - m_pOwner->GetPosition().left,
				nY - m_pOwner->GetPosition().top);
		}
		else
		{
			std::list<CComponent*>::reverse_iterator the = m_listPopup.rbegin();
			CComponent *pCompPopup = *the;
			focus_component = pCompPopup->GetComponentReverseAt(
				nX - pCompPopup->GetScreenRect().left,
				nY - pCompPopup->GetScreenRect().top);
		}
	}

	// Make coordinates relative to focus component.
	nX -= focus_component->GetScreenRect().left;
	nY -= focus_component->GetScreenRect().top;
	// Send mouse enter and mouse leave signals:
	DoMouseEnterLeave(nX, nY, focus_component);
	
	pos = focus_component->ScreenToClient(pos);

	focus_component->SigMouseMove()(nKeys, pos.x, pos.y);	
}

VOID CGUIManager_Generic::OnMouseWheel(int nFalgs,short shDelta,int nX,int nY)
{
	if (!IsEnableInput()) 
		return;

	CPos pos;
	pos.x = nX;
	pos.y = nY;

	m_pOwner->SigTransformCoords()(nX, nY);
	m_SigCaptureWheelMove( nFalgs, shDelta, pos );

	CComponent* focus_component = m_pCompCaptureMouse;
	if (!m_pCompCaptureMouse)
	{
		if(m_listPopup.empty())
		{
			focus_component = m_pOwner->GetComponentReverseAt(
				nX - m_pOwner->GetPosition().left,
				nY - m_pOwner->GetPosition().top);
		}
		else
		{
			std::list<CComponent*>::reverse_iterator the = m_listPopup.rbegin();
			CComponent *pCompPopup = *the;
			focus_component = pCompPopup->GetComponentReverseAt(
				nX - pCompPopup->GetScreenRect().left,
				nY - pCompPopup->GetScreenRect().top);
		}
	}
	
	// Make coordinates relative to focus component.
	SetFocus(focus_component);
	pos = m_pCompFocus->ScreenToClient(pos);

	m_pCompFocus->SigMouseWheel()(nFalgs,shDelta,pos);
}

VOID CGUIManager_Generic::OnMouseDown(const CKey& key,int nX,int nY)
{
	if (!IsEnableInput()) 
		return;

	m_pOwner->SigTransformCoords()(nX, nY);
	m_SigCaptureMouseDown( key );
	
	CComponent *focus_component = m_pCompCaptureMouse;
	if (!m_pCompCaptureMouse)
	{
		if(m_listPopup.empty())
		{
			focus_component = m_pOwner->GetComponentReverseAt(
				nX - m_pOwner->GetPosition().left,
				nY - m_pOwner->GetPosition().top);
		}
		else
		{
			std::list<CComponent*>::reverse_iterator the = m_listPopup.rbegin();
			CComponent *pCompPopup = *the;
			focus_component = pCompPopup->GetComponentReverseAt(
				nX - pCompPopup->GetScreenRect().left,
				nY - pCompPopup->GetScreenRect().top);
		}
	}

	// Make coordinates relative to focus component.

	SetFocus(focus_component);

	CKey newkey = key;
	CPos pos;
	pos.x = (int)newkey.m_fX;
	pos.y = (int)newkey.m_fY;
	pos = focus_component->ScreenToClient(pos);
	newkey.m_fX = (float)pos.x;
	newkey.m_fY = (float)pos.y;

	focus_component->SigMouseDown()(newkey);	

	if(key.m_nID == UD_LEFT_BUTTON)
	{
		m_ptDragStartPos.x = nX;
		m_ptDragStartPos.y = nY;
		m_pCompDrag = focus_component;
	}

	focus_component->TopTabOrder();
	
	m_SigCaptureEndMouseDown( key );
}

VOID CGUIManager_Generic::OnMouseUp(const CKey& key,int nX,int nY)
{
	if (!IsEnableInput()) 
		return;

	m_pOwner->SigTransformCoords()(nX, nY);
	m_SigCaptureMouseUp( key );

	CComponent *focus_component = m_pCompCaptureMouse;
	if (!m_pCompCaptureMouse)
	{
		if(m_listPopup.empty())
		{
			focus_component = m_pOwner->GetComponentReverseAt(
				nX - m_pOwner->GetPosition().left,
				nY - m_pOwner->GetPosition().top);
		}
		else
		{
			std::list<CComponent*>::reverse_iterator the = m_listPopup.rbegin();
			CComponent *pCompPopup = *the;
			focus_component = pCompPopup->GetComponentReverseAt(
				nX - pCompPopup->GetScreenRect().left,
				nY - pCompPopup->GetScreenRect().top);
		}
	}

	// Make coordinates relative to focus component.

	CKey newkey = key;
	CPos pos;
	pos.x = (int)newkey.m_fX;
	pos.y = (int)newkey.m_fY;
	pos = focus_component->ScreenToClient(pos);
	newkey.m_fX = (float)pos.x;
	newkey.m_fY = (float)pos.y;
	
	focus_component->SigMouseUp()(newkey);

	if(key.m_nID == UD_LEFT_BUTTON && m_pCompDrag)
	{
		bool bSuccess = FALSE;
		focus_component->SigDragDropRevote()(m_pCompDrag,newkey,m_nDataDrag,bSuccess);  
		if(!bSuccess)
			m_pCompDrag->SigDragDropNotProc()(m_nDataDrag);
	}

	m_bDrag = false;
	m_pCompDrag = NULL;
	m_nDataDrag = NULL;
}

VOID CGUIManager_Generic::SendMsg(int nMsg,int lParam,int wParam)
{
	m_pOwner->SigMessage()(nMsg,lParam,wParam);
}

LRESULT CGUIManager_Generic::OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(m_pCompFocus != NULL)
		return m_pCompFocus->OnWindowProc(hWnd,uMsg,wParam,lParam);
	return 0;
}


VOID CGUIManager_Generic::DoMouseEnterLeave(int nX, int nY, CComponent *pNewFocus)
{
	CComponent *pOldFocus = m_pCompMouseOver;
	if (nX < 0 || nY < 0 || nX >= pNewFocus->GetWidth() || nY >= pNewFocus->GetHeight()) pNewFocus = 0;
	
	// No signals needed if same component kept focus.
	if (pOldFocus == pNewFocus) return;

	if (pOldFocus) pOldFocus->SigMouseLeave()(pOldFocus);
	if (pNewFocus) pNewFocus->SigMouseEnter()(pNewFocus);

	m_pCompMouseOver = pNewFocus;
}

CComponent *CGUIManager_Generic::TabNext(VOID)
{
	// First try to find component by tab id:
	CComponent *tab_component = m_pOwner->FindTabID(++m_nCurrTabID);
	if (tab_component == 0) // reached end, start from beginning
	{
		m_nCurrTabID = 0;
		tab_component = m_pOwner->FindTabID(m_nCurrTabID);
	}
	if (tab_component)
	{
		SetFocus(tab_component);
		return m_pCompFocus;
	}

	// No tab ids - fall back to tree walking:
	if (m_pCompFocus->GetChildren().empty()) // tab to sibling or uncle
	{
		CComponent *cur = m_pCompFocus;

		while (true)
		{
			CComponent *parent = cur->GetParent();
			if (parent == 0) break;

			// Search siblings:
			std::list<CComponent *>::const_iterator it;
			for (it = parent->GetChildren().begin(); it != parent->GetChildren().end(); it++)
			{
				if (*it == cur)
				{
					it++;

					// no more siblings
					if (it == parent->GetChildren().end()) break;

					// give sibling focus
					SetFocus(*it);
					return m_pCompFocus;
				}
			}

			// Was last sibling, continue search in parent (uncles)
			cur = parent;
		}

		// if we are top node (no siblings), and got no children:
		if (cur == m_pCompFocus)
			return m_pCompFocus;

		// walked through all components. Restart at first component:
		SetFocus(m_pOwner);
		return m_pCompFocus;
	}
	else // tab to children
	{
		// give first child focus
		SetFocus(m_pCompFocus->GetChildren().front());
		return m_pCompFocus;
	}
}

CComponent *CGUIManager_Generic::TabPrevious(VOID)
{
	// First try to find component by tab id:
	CComponent *tab_component = m_pOwner->FindTabID(--m_nCurrTabID);
	if (m_nCurrTabID == -1) // reached beginning, start from end
	{
		m_nCurrTabID = FindHighestTabID(m_pOwner);
		tab_component = m_pOwner->FindTabID(m_nCurrTabID);
	}
	if (tab_component)
	{
		SetFocus(tab_component);
		return m_pCompFocus;
	}

	// No tab ids - fall back to tree walking:
	CComponent *cur = m_pCompFocus;
	while (true)
	{
		CComponent *parent = cur->GetParent();
		if (parent == 0) break;

		// Search siblings:
		std::list<CComponent *>::const_iterator it;
		for (it = parent->GetChildren().begin(); it != parent->GetChildren().end(); it++)
		{
			if (*it == cur)
			{
				// no more siblings
				if (it == parent->GetChildren().begin()) break;

				it--;

				// give sibling focus
				SetFocus(*it);
				return m_pCompFocus;
			}
		}

		// Was last sibling, continue search in parent (uncles)
		cur = parent;
	}

	// walked through all components. Restart at end component:
	cur = m_pOwner;
	while (!cur->GetChildren().empty()) cur = cur->GetChildren().back();
	SetFocus(cur);
	return m_pCompFocus;
}

#define cl_max(a,b) ((a > b) ? a : b)

int CGUIManager_Generic::FindHighestTabID(const CComponent *pComponent)
{
	int id = cl_max(0, pComponent->GetTabID());
	const std::list<CComponent *> &children = pComponent->GetChildren();
	std::list<CComponent *>::const_iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		id = cl_max(id, FindHighestTabID(*it));
	}
	return id;
}


VOID CGUIManager_Generic::OnBeforeRemove(CComponent *pComp)
{
	if(m_pCompFocus == pComp)
	{
		m_pCompFocus->SigLostFocus()();
		m_pCompFocus = m_pOwner;
	}
	if(m_pCompCaptureMouse == pComp)
		m_pCompCaptureMouse = NULL;
	if(m_pCompMouseOver == pComp)
		m_pCompMouseOver = NULL;	
	if(m_pCompDrag == pComp)
		m_pCompDrag = NULL;
}

END_GUI
