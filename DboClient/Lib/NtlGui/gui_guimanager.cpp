#include "gui_precomp.h"
#include "gui_guimanager.h"
#include "gui_guimanager_generic.h"
#include "gui_inputbox_generic.h"

/////////////////////////////////////////////////////////////////////////////
// GUIManager Construction:

START_GUI

CGUIManager::CGUIManager(CRectangle rect)
:CComponent(rect,NULL,NULL)
{
	m_pImpl = NTL_NEW CGUIManager_Generic(this);	
}

CGUIManager::~CGUIManager()
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Attributes:

CComponent *CGUIManager::GetCapturedMouse(VOID)
{
	return m_pImpl->GetCapturedMouse();
}

CComponent *CGUIManager::GetFocus(VOID)
{
	return m_pImpl->GetFocus();
}


CComponent *CGUIManager::GetMouseOver(VOID)
{
	return m_pImpl->GetMouseOver();
}

/////////////////////////////////////////////////////////////////////////////
// 

CSignal_v3<CComponent*, CInputDevice *, const CKey &>& CGUIManager::SigCaptureKeyDown(VOID)
{
	return m_pImpl->m_SigCaptureKeyDown;
}

CSignal_v3<CComponent*, CInputDevice *, const CKey &>& CGUIManager::SigCaptureKeyUp(VOID)
{
	return m_pImpl->m_SigCaptureKeyUp;
}

CSignal_v3<int,int,int>& CGUIManager::SigCaptureMouseMove(VOID)
{
	return m_pImpl->m_SigCaptureMouseMove;
}

CSignal_v1<const CKey &>& CGUIManager::SigCaptureMouseUp(VOID)
{
	return m_pImpl->m_SigCaptureMouseUp;
}

CSignal_v1<const CKey&>& CGUIManager::SigCaptureMouseDown(VOID)
{
	return m_pImpl->m_SigCaptureMouseDown;
}

CSignal_v1<const CKey&>& CGUIManager::SigCaptureEndMouseDown(VOID)
{
	return m_pImpl->m_SigCaptureEndMouseDown;
}

CSignal_v3<int,short,CPos&>& CGUIManager::SigCaptureWheelMove(VOID)
{
	return m_pImpl->m_SigCaptureWheelMove;
}

/////////////////////////////////////////////////////////////////////////////
// GUIManager Operations:


VOID CGUIManager::AddPopup(CComponent *pComponent)
{
	m_pImpl->AddPopup(pComponent);
}

VOID CGUIManager::RemovePopup(CComponent *pComponent)
{
	m_pImpl->RemovePopup(pComponent);
}

VOID CGUIManager::SetFocus(CComponent *pComponent)
{
	m_pImpl->SetFocus(pComponent);
}

VOID CGUIManager::Run(VOID)
{
	m_pImpl->Run();
}

VOID CGUIManager::Show(VOID)
{
	m_pImpl->Show();
}

VOID CGUIManager::Quit(VOID)
{
	m_pImpl->Quit();
}

VOID CGUIManager::EnableInput(void* key)
{
	m_pImpl->EnableInput(key);
}

VOID CGUIManager::DisableInput(void* key)
{
	m_pImpl->DisableInput(key);
}

VOID CGUIManager::GuiCaptureMouse(CComponent *pComponent)
{
	m_pImpl->GuiCaptureMouse(pComponent);
}

VOID CGUIManager::GuiReleaseMouse(VOID)
{
	m_pImpl->GuiReleaseMouse();
}

VOID CGUIManager::Input(VOID)
{
	std::list<WIN32EVENT>::iterator it;
	for(it=m_stlInput.begin(); it!=m_stlInput.end(); it++)
	{
		InputProcess((*it).hWnd,(*it).uMsg,(*it).wParam,(*it).lParam);
	}

	m_stlInput.clear();
}

LRESULT CGUIManager::Win32EventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	InputPush(hWnd,uMsg,wParam,lParam);

	if ( !m_pImpl->IsEnableInput() )
		return 0;

	if(CInputBox_Generic::OnStaticMsgProc(hWnd,uMsg,wParam,lParam))
		return 1;

	LRESULT hResult = m_pImpl->OnWindowProc(hWnd,uMsg,wParam,lParam);

	InputProcess(hWnd,uMsg,wParam,lParam);

	return hResult;
}

VOID CGUIManager::InputPush(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE: case WM_LBUTTONDOWN: case WM_RBUTTONDOWN:
	case WM_LBUTTONDBLCLK: case WM_RBUTTONDBLCLK: case WM_LBUTTONUP: 
	case WM_RBUTTONUP: case WM_KEYDOWN: 
		m_stlInput.push_back(WIN32EVENT(hWnd,uMsg,wParam,lParam));
		break;
	case WM_KEYUP:
		{
			if( wParam == 229)
				break;
			m_stlInput.push_back(WIN32EVENT(hWnd,uMsg,wParam,lParam));
		}
		break;
	}
}

bool CGUIManager::InputProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);
			m_pImpl->OnMouseMove((int)wParam,nX,nY);		
		}
		return true;
	case WM_MOUSEWHEEL:
		{
			POINT point;
			point.x = GET_X_LPARAM( lParam );
			point.y = GET_Y_LPARAM( lParam );
				
			::ScreenToClient( hWnd, &point );

			m_pImpl->OnMouseWheel( LOWORD(wParam), HIWORD(wParam), point.x, point.y );   
		}
		break;
	case WM_LBUTTONDOWN: 
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_MOUSE;
			CKey key(UD_LEFT_BUTTON,CKey::PRESSED, (INT)wParam,(float)nX, (float)nY);
			m_pImpl->OnMouseDown(key,nX,nY);
		}
		return true;
	case WM_RBUTTONDOWN:
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_MOUSE;
			CKey key(UD_RIGHT_BUTTON,CKey::PRESSED, (INT)wParam,(float)nX, (float)nY);
			m_pImpl->OnMouseDown(key,nX,nY);
		}
		return true;
	case WM_LBUTTONDBLCLK:
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_MOUSE;
			CKey key(UD_LEFT_BUTTON,CKey::DBLPRESSED, (INT)wParam,(float)nX, (float)nY);
			m_pImpl->OnMouseDown(key,nX,nY);
		}
		return true;
	case WM_RBUTTONDBLCLK:
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_MOUSE;
			CKey key(UD_RIGHT_BUTTON,CKey::DBLPRESSED, (INT)wParam, (float)nX, (float)nY);
			m_pImpl->OnMouseDown(key,nX,nY);
		}
		return true;
	case WM_LBUTTONUP: 
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);			
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_MOUSE;
			CKey key(UD_LEFT_BUTTON,CKey::RELEASE, (INT)wParam, (float)nX, (float)nY);
			m_pImpl->OnMouseUp(key,nX,nY);
		}
		return true;
	case WM_RBUTTONUP:
		{
			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);			
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_MOUSE;
			CKey key(UD_RIGHT_BUTTON,CKey::RELEASE, (INT)wParam, (float)nX, (float)nY);
			m_pImpl->OnMouseUp(key,nX,nY);
		}
		return true;
	case WM_KEYDOWN:
		{
			CKey key((int)lParam,CKey::PRESSED,(int)wParam,0,0);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_KEYBOARD;
			m_pImpl->OnKeyPress(&InputDevice,key);
		}
		return true;
	case WM_KEYUP:
		{
//			if( wParam == 229)
//				break;

			CKey key((int)lParam,CKey::RELEASE,(int)wParam,0,0);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_KEYBOARD;
			m_pImpl->OnKeyRelease(&InputDevice,key);
		}
		return true;
	case WM_CHAR:
		{
			CKey key(0,CKey::PRESSED,(int)wParam,0,0);
			CInputDevice InputDevice;
			InputDevice.m_Type = CInputDevice::TYPE_KEYBOARD;
			m_pImpl->OnCharPress(&InputDevice,key);
		}
		return true;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////////
// GUIManager Operations:


END_GUI