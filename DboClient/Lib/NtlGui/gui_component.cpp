#include "gui_precomp.h"
#include "gui_component.h"
#include "gui_component_generic.h"
#include "gui_guimanager.h"
#include "gui_componentmanager.h"
#include "gui_componentmanager_generic.h"
#include "mouse.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CComponent::CComponent(CComponent *pParent,CSurfaceManager *pSurfaceManager)
: m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CComponent_Generic( this, pParent, pSurfaceManager );

	if(pParent != NULL)
		pParent->AddChild(this);

	SetPosition( CRectangle( 0, 0, 0, 0 ) );
}

CComponent::CComponent(const CRectangle &rtPos, CComponent *pParent, CSurfaceManager *pSurfaceManager)
: m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CComponent_Generic(this, pParent,pSurfaceManager);

	if(pParent != NULL)
		pParent->AddChild(this);

	SetPosition( rtPos );
}

CComponent::~CComponent()
{
	// GUI Manager pointer를 구한다.
	CComponent *pGuiMgr = GetGuiManager();
	
	if(IsPopup())
		GetGuiManager()->RemovePopup(this);

	if(m_pImpl->m_pParent != NULL) 
	{
		m_pImpl->m_pParent->RemoveChild(this);
		m_pImpl->m_pParent = NULL;
	}
	
	std::list<CComponent*>::iterator the;
	for( the = m_pImpl->m_stlChildren.begin() ; the != m_pImpl->m_stlChildren.end() ; ++the )
	{
		if(pGuiMgr)
			pGuiMgr->m_pImpl->m_SigBeforeRemove(*the);

		m_pImpl->m_stlDeleteChildren.push_back(*the);
	}

	if(pGuiMgr)
		pGuiMgr->m_pImpl->m_SigBeforeRemove(this);
	
	m_pImpl->m_SigBeforeRemove(this);
	
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string& CComponent::GetPage(VOID) const
{
	return m_pImpl->m_strPage; 
}

const std::string& CComponent::GetID(VOID) const
{
	return m_pImpl->m_strID; 
}

const std::wstring& CComponent::GetToolTip(VOID) const
{
	return m_pImpl->m_wstrToolTip;	
}

const std::string& CComponent::GetDescription(VOID) const
{
	return m_pImpl->m_strDescription;
}

CComponent *CComponent::GetParent(VOID) const
{
	return m_pImpl->m_pParent;
}

CComponent *CComponent::GetRootParent(VOID)
{
	if(m_pImpl->m_pParent)
		return m_pImpl->m_pParent->GetRootParent();
	else
		return this;
}

CSurfaceManager* CComponent::GetSurfaceManager(VOID) const
{
	if (m_pImpl->m_pSurfaceMgr != NULL)
		return m_pImpl->m_pSurfaceMgr;
	if (m_pImpl->m_pParent != NULL) 
		return m_pImpl->m_pParent->GetSurfaceManager();

	return NULL;
}

CGUIManager *CComponent::GetGuiManager(VOID) const
{
	//if (m_pImpl->m_pGuiMgr != NULL)
	//	return m_pImpl->m_pGuiMgr;
	//if (m_pImpl->m_pParent != NULL) 
	//	return m_pImpl->m_pParent->GetGuiManager();
	//return NULL;
	return m_pImpl->m_pGuiMgr;
}


int CComponent::GetWidth(VOID) const
{
	return m_pImpl->m_rtPosition.right - m_pImpl->m_rtPosition.left;
}

int CComponent::GetHeight(VOID) const
{
	return m_pImpl->m_rtPosition.bottom - m_pImpl->m_rtPosition.top;
}

CRectangle CComponent::GetScreenRect(VOID) const
{
	if (m_pImpl->m_pParent == NULL)
	{
		return m_pImpl->m_rtPosition;
	}
	else
	{
		CRectangle r = m_pImpl->m_pParent->GetScreenRect();

		return CRectangle(
			r.left + m_pImpl->m_rtPosition.left, 
			r.top + m_pImpl->m_rtPosition.top,
			r.left + m_pImpl->m_rtPosition.right, 
			r.top + m_pImpl->m_rtPosition.bottom);
	}
}

CPos CComponent::ClientToScreen(CPos pos) const
{
	CRectangle rtScreen = GetScreenRect();
	CPos ptPos;
	ptPos.x = rtScreen.left+pos.x;
	ptPos.y = rtScreen.top+pos.y;
	return ptPos;
}

CPos CComponent::ScreenToClient(CPos pos) const
{
	CRectangle rtScreen = GetScreenRect();
	CPos ptPos;
	ptPos.x = pos.x-rtScreen.left;
	ptPos.y = pos.y-rtScreen.top;
	return ptPos;
}


CRectangle CComponent::GetClientRect(VOID) const
{
	CRectangle rect = GetPosition();
	CRectangle newrect = rect;
	newrect.left = 0;
	newrect.top = 0;
	newrect.right = rect.GetWidth();
	newrect.bottom = rect.GetHeight();

	return newrect;
}

const CRectangle &CComponent::GetPosition(VOID) const
{
	return m_pImpl->m_rtPosition;
}

bool CComponent::HasChild(CComponent *pComponent) const
{
	for( std::list<CComponent *>::iterator it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		if( *it == pComponent || (*it)->HasChild( pComponent ) )
			return true;
	}

	return false;
}

const std::list<CComponent*> &CComponent::GetChildren(VOID) const
{
	return m_pImpl->m_stlChildren;
}

CRectangle CComponent::GetChildrenRect(VOID)
{
	CRectangle rect(0, 0, 0, 0);
	
	for ( std::list<CComponent*>::iterator it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		CComponent* comp = (*it);
		CRectangle childrect = comp->GetPosition();

		if(childrect.left < rect.left)
			rect.left = childrect.left;
		if(childrect.top < rect.top)
			rect.top = childrect.top;
		if(childrect.right > rect.right)
			rect.right = childrect.right;
		if(childrect.bottom > rect.bottom)
			rect.bottom = childrect.bottom;
	}

	return rect;
}

// 아래에 찍히는 자식부터 찾음
CComponent *CComponent::GetComponentAt(int nPosX, int nPosY)
{
	std::list<CComponent *>::reverse_iterator it;
	for (it = m_pImpl->m_stlChildren.rbegin(); it != m_pImpl->m_stlChildren.rend(); it++)
	{
		CComponent *cur_component = (*it);

		if(cur_component->IsEnabled() && cur_component->IsVisible())
		{
			const CRectangle &r = cur_component->GetPosition();

			if (nPosX >= r.left && nPosX < r.right &&
				nPosY >= r.top && nPosY < r.bottom)
			{
				return cur_component->GetComponentAt(nPosX - r.left, nPosY - r.top);
			}
		}
	}

	return this;
}

// 위에 찍히는 자식부터 찾음
CComponent *CComponent::GetComponentReverseAt(int nPosX, int nPosY)
{
	std::list<CComponent *>::iterator it;
	for (it = m_pImpl->m_stlChildren.begin(); it != m_pImpl->m_stlChildren.end(); it++)
	{
		CComponent *cur_component = (*it);

		if(cur_component->IsEnabled() && cur_component->IsVisible())
		{
			const CRectangle &r = cur_component->GetPosition();

			if (nPosX >= r.left && nPosX < r.right &&
				nPosY >= r.top && nPosY < r.bottom)
			{
				return cur_component->GetComponentReverseAt(nPosX - r.left, nPosY - r.top);
			}
		}
	}

	return this;
}

CComponent* CComponent::GetChildComponentAt(int nPosX, int nPosY)
{
	std::list<CComponent *>::reverse_iterator it;
	for (it = m_pImpl->m_stlChildren.rbegin(); it != m_pImpl->m_stlChildren.rend(); it++)
	{
		CComponent* cur_component = (*it);

		if(cur_component->IsEnabled() && cur_component->IsVisible())
		{
			const CRectangle &r = cur_component->GetPosition();

			if (nPosX >= r.left && nPosX < r.right &&
				nPosY >= r.top && nPosY < r.bottom)
			{
				return cur_component;
			}
		}
	}

	return NULL;
}

CComponent* CComponent::GetChildComponentReverseAt(int nPosX, int nPosY)
{
	std::list<CComponent *>::iterator it;
	for (it = m_pImpl->m_stlChildren.begin(); it != m_pImpl->m_stlChildren.end(); it++)
	{
		CComponent* cur_component = (*it);

		if(cur_component->IsEnabled() && cur_component->IsVisible())
		{
			const CRectangle &r = cur_component->GetPosition();

			if (nPosX >= r.left && nPosX < r.right &&
				nPosY >= r.top && nPosY < r.bottom)
			{
				return cur_component;
			}
		}
	}

	return NULL;
}

CComponent *CComponent::GetFocus(VOID) const
{
	return GetGuiManager()->GetFocus();
}

bool CComponent::HasFocus(VOID) const
{
	return GetFocus() == this;
}

bool CComponent::HasMouseOver(VOID) const
{
	int x = CMouse::GetX();
	int y = CMouse::GetY();

	CRectangle rect = GetScreenRect();

	if(rect.PtInRect(x, y))
		return true;

	return false;
}

bool CComponent::IsVisible(VOID) const
{
	return m_pImpl->m_bVisible;
}

bool CComponent::IsVisibleTruly(VOID) const
{
	bool bVisible = IsVisible();

	if( bVisible )
	{
		if( m_pImpl->m_pParent )
			return m_pImpl->m_pParent->IsVisibleTruly();
		else
			return bVisible;
	}
	else
		return bVisible;
}

bool CComponent::IsEnabled(VOID) const
{
	return m_pImpl->m_bEnabled;
}


bool CComponent::IsPopup(VOID) const
{
	return m_pImpl->m_bPopup;
}

VOID CComponent::SetRenderTop(bool bTop)
{
	m_pImpl->m_bRenderTop = bTop;
}


bool CComponent::GetRenderTop(VOID)
{
	return m_pImpl->m_bRenderTop;
}


bool CComponent::IsMouseCaptured(VOID) const
{
	return m_pImpl->m_bCaptured;
}

bool CComponent::IsRemoveflagSet(VOID) const
{
	return m_pImpl->m_bRemoveflag;
}

CPos CComponent::GetMousePosition(VOID) const
{
	
	CPos pos(CMouse::GetX(),CMouse::GetY());
	CRectangle screen = GetScreenRect();
	pos.x -= screen.left;
	pos.y -= screen.top;
		
	return pos;
}

int CComponent::GetTabID(VOID) const
{
	return m_pImpl->m_nTabID;
}

WORD CComponent::GetPriority(VOID) const
{
	return m_pImpl->m_wPriority;
}

CComponent::POSITIONSTATE CComponent::PosInRect( INT nX, INT nY )
{
	CRectangle& rtScreen = GetScreenRect();
	DWORD retval = INRECT;

	if( rtScreen.left > nX )
	{
		retval |= OUT_LEFT;
	}
	else if( rtScreen.right <= nX )
	{
		retval |= OUT_RIGHT;
	}

	if( rtScreen.top > nY )
	{
		retval |= OUT_UP;
	}
	else if( rtScreen.bottom <= nY )
	{
		retval |= OUT_DOWN;
	}

	return (POSITIONSTATE)retval;
}

CRectangle* CComponent::GetClippingRect(VOID) const
{
	return &m_pImpl->m_rtClipping;
}

bool CComponent::IsClipped(VOID) 
{
	return m_pImpl->m_bClipping;
}

BYTE CComponent::GetAlpha(VOID) const
{
	return m_pImpl->m_ucAlpha;
}

DWORD CComponent::GetColor(VOID) const
{
	return m_pImpl->m_uiColor;
}
/// woosungs_test
CComponent_Generic*	CComponent::GetComponent_Generic(VOID) const
{
	return m_pImpl;
}
/////////////////////////////////////////////////////////////////////////////
// Operations:

VOID CComponent::SetPage(const std::string& strPage)
{
	m_pImpl->m_strPage = strPage;
}

VOID CComponent::SetID(const std::string& strID)
{
	m_pImpl->m_strID = strID;
}

VOID CComponent::SetToolTip( const std::string& strToolTip )
{
	INT nLen = (INT)strToolTip.size();
	WCHAR pBuffer[GUI_TEXT_BUFFER_SIZE];
	::MultiByteToWideChar( GetACP(), 0, strToolTip.c_str(), -1, pBuffer, nLen+1 );
	
	m_pImpl->SetToolTip( std::wstring( pBuffer ) );	
}

VOID CComponent::SetToolTip( const std::wstring& wstrToolTip )
{
	m_pImpl->SetToolTip( wstrToolTip );	
}

VOID CComponent::DeleteToolTip(VOID)
{
	m_pImpl->m_wstrToolTip.clear();
	m_pImpl->HideToolTip();
}

VOID CComponent::ShowToolTip( bool bShow )
{
	if( bShow )
		m_pImpl->ShowToolTip();
	else
		m_pImpl->HideToolTip();
}

VOID CComponent::SetTabID(int id)
{
	m_pImpl->m_nTabID = id;
}

// 1. Priority 변경시 변경된 Priority 제일 앞에 위치하게 된다. 
VOID CComponent::SetPriority( WORD wPriority )
{
	if( wPriority == m_pImpl->m_wPriority )
		return;

	m_pImpl->m_wPriority = wPriority;
	
	CComponent* pParent = m_pImpl->m_pParent;

	if( pParent )
	{
		std::list<CComponent*>& ParentChildList = pParent->m_pImpl->m_stlChildren;
		ParentChildList.remove( this );
		
		std::list<CComponent*>::iterator it;
		BOOL bNotInserted = TRUE;

		for( it = ParentChildList.begin() ; it != ParentChildList.end() ; ++it )
		{
			CComponent* pAddedChildComp = reinterpret_cast<CComponent*>( (*it) );

			if( GetPriority() >= pAddedChildComp->GetPriority() )
			{
				ParentChildList.insert( it, this );
				bNotInserted = FALSE;
				break;
			}
		}

		if( bNotInserted )
			ParentChildList.push_back( this );
	}	
}

VOID CComponent::ChildCascade( DWORD flagCascadeType )
{
	CRectangle rtOldPos;
	std::list<CComponent*>::iterator it = m_pImpl->m_stlChildren.begin();
	for (; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		CComponent* pComp = *it;

		rtOldPos = pComp->GetPosition();
		pComp->m_pImpl->CalcClippingRect();
		if( flagCascadeType & CHILD_DO_MOVE )
			pComp->m_pImpl->m_SigMove(rtOldPos.left,rtOldPos.top);
		// Even if the parent is resized, the child is not resized by policy.
		//if( flagCascadeType & CHILD_DO_RESIZE )
		//	(*it)->m_pImpl->m_SigResize(rtOldPos.GetWidth(),rtOldPos.GetHeight());		
		pComp->ChildCascade( flagCascadeType );
	}
}

VOID CComponent::SetPosition(const CRectangle &newpos)
{	
	int old_x = m_pImpl->m_rtPosition.left;
	int old_y = m_pImpl->m_rtPosition.top;

	int old_width = GetWidth();
	int old_height = GetHeight();

	DWORD flagChildCascade = 0;

	m_pImpl->m_rtPosition = newpos;
	m_pImpl->CalcClippingRect();

	if(old_x != m_pImpl->m_rtPosition.left || old_y != m_pImpl->m_rtPosition.top)
	{
		m_pImpl->m_SigMove(old_x, old_y);
		flagChildCascade |= CHILD_DO_MOVE;
	}
	if(old_width != GetWidth() || old_height != GetHeight())
	{
		m_pImpl->m_SigResize(old_width, old_height);
		flagChildCascade |= CHILD_DO_RESIZE;
	}

	if( flagChildCascade )
		ChildCascade( flagChildCascade );	
}



VOID CComponent::SetPosition(int nNewX, int nNewY)
{
	int old_x = m_pImpl->m_rtPosition.left;
	int old_y = m_pImpl->m_rtPosition.top;

	m_pImpl->m_rtPosition.right = GetWidth() + nNewX;
	m_pImpl->m_rtPosition.bottom = GetHeight() + nNewY;

	m_pImpl->m_rtPosition.left = nNewX;
	m_pImpl->m_rtPosition.top = nNewY;

	if(old_x != m_pImpl->m_rtPosition.left || old_y != m_pImpl->m_rtPosition.top)
	{
		m_pImpl->CalcClippingRect();
		m_pImpl->m_SigMove(old_x, old_y);		
		ChildCascade( CHILD_DO_MOVE );
	}	
}

VOID CComponent::SetSize(int nNewWidth, int nNewHeight)
{
	int old_width = GetWidth();
	int old_height = GetHeight();

	m_pImpl->m_rtPosition.right = m_pImpl->m_rtPosition.left + nNewWidth;
	m_pImpl->m_rtPosition.bottom = m_pImpl->m_rtPosition.top + nNewHeight;

	if(old_width != GetWidth() || old_height != GetHeight())
	{
		m_pImpl->CalcClippingRect();
		m_pImpl->m_SigResize(old_width, old_height);		
		ChildCascade( CHILD_DO_RESIZE );
	}
}

//VOID CComponent::Resize(CRectangle oldpos)
//{
//	
//}

VOID CComponent::SetWidth(int nWidth)
{
	int old_width = GetWidth();

	m_pImpl->m_rtPosition.right = m_pImpl->m_rtPosition.left + nWidth;

	if(old_width != GetWidth())
	{
		m_pImpl->m_SigResize(old_width, GetHeight());
		ChildCascade( CHILD_DO_RESIZE );
	}
}

VOID CComponent::SetHeight(int nHeight)
{
	int old_height = GetHeight();

	m_pImpl->m_rtPosition.bottom = m_pImpl->m_rtPosition.top + nHeight;

	if(old_height != GetHeight())
	{
		m_pImpl->m_SigResize(GetWidth(), old_height);
		ChildCascade( CHILD_DO_RESIZE );
	}
}

VOID CComponent::SetParent(CComponent *pParent, BOOL bParentAddRemoveProc /* = FALSE */)
{
	// 추가된 기능 : 외부에서 씀
	if( bParentAddRemoveProc )
	{
		if( m_pImpl->m_pParent )
			m_pImpl->m_pParent->RemoveChild( this );
	
		pParent->AddChild( this );

		// 위치 갱신
		CRectangle rect = GetPosition();
		m_pImpl->CalcClippingRect();		
		m_pImpl->m_SigMove(rect.left,rect.top);
		//m_pImpl->m_SigResize(rect.GetWidth(),rect.GetHeight());		
		ChildCascade( CHILD_DO_MOVE );
	}
	// 원래 기능 : 내부에서 씀.
	else
	{
		m_pImpl->m_pParent = pParent;
	}
}

VOID CComponent::SetGuiManager(CGUIManager *gui)
{
	m_pImpl->m_pGuiMgr = gui;
}

VOID CComponent::Show(bool bShow)
{
	m_pImpl->m_bVisible = bShow;

	// 부모창이 닫힐때 자식창의 툴팁도 같이 닫는다.
	if( !bShow )
		m_pImpl->CheckToolTipHide();
}

VOID CComponent::Enable(bool bEnable)
{
	m_pImpl->m_bEnabled = bEnable;
}

VOID CComponent::Popup(bool bPopup)
{
	m_pImpl->m_bPopup = bPopup;
	if(bPopup)
		GetGuiManager()->AddPopup(this);
	else
		GetGuiManager()->RemovePopup(this);
}

VOID CComponent::SetFocus(VOID)
{
	GetGuiManager()->SetFocus(this);
}

VOID CComponent::Raise( VOID )
{
	CComponent* pParent = m_pImpl->m_pParent;

	if( pParent )
	{
		std::list<CComponent*>& ParentChildList = pParent->m_pImpl->m_stlChildren;
		ParentChildList.remove( this );

		std::list<CComponent*>::iterator it;
		BOOL bNotInserted = TRUE;

		for( it = ParentChildList.begin() ; it != ParentChildList.end() ; ++it )
		{
			CComponent* pAddedChildComp = reinterpret_cast<CComponent*>( (*it) );

			if( GetPriority() >= pAddedChildComp->GetPriority() )
			{
				ParentChildList.insert( it, this );
				bNotInserted = FALSE;
				break;
			}
		}

		if( bNotInserted )
			ParentChildList.push_back( this );
	}		
}

VOID CComponent::Lower(VOID)
{
	CComponent* pParent = m_pImpl->m_pParent;

	if( pParent )
	{
		std::list<CComponent*>& ParentChildList = pParent->m_pImpl->m_stlChildren;
		ParentChildList.remove( this );

		std::list<CComponent*>::iterator it;
		BOOL bNotInserted = TRUE;

		for( it = ParentChildList.begin() ; it != ParentChildList.end() ; ++it )
		{
			CComponent* pAddedChildComp = reinterpret_cast<CComponent*>( (*it) );

			if( GetPriority() > pAddedChildComp->GetPriority() )
			{
				ParentChildList.insert( it, this );
				bNotInserted = FALSE;
				break;
			}
		}

		if( bNotInserted )
			ParentChildList.push_back( this );
	}		
}

VOID CComponent::CaptureMouse(VOID)
{
	CGUIManager *gui = GetGuiManager();
	if (gui != NULL)
	{
		gui->GuiCaptureMouse(this);
		m_pImpl->m_bCaptured = true;
	}
}



VOID CComponent::ReleaseMouse(VOID)
{
	CGUIManager *gui = GetGuiManager();
	if (gui != NULL)
	{
		gui->GuiReleaseMouse();
		m_pImpl->m_bCaptured = false;
	}
}


CComponent* CComponent::GetMouseOver(VOID)
{
	CGUIManager *gui = GetGuiManager();
	if (gui == NULL)
		return NULL;

	return gui->GetMouseOver();
}


VOID CComponent::Paint(VOID)
{
	m_pImpl->m_SigPaint();
	m_pImpl->m_SigChildPaint();
}

VOID CComponent::Run(CComponent *pParent)
{
//	CGUIManager modal_gui(pParent);
//	modal_gui.AddChild(this);
//	modal_gui.Run();
}

VOID CComponent::Quit(VOID)
{
	GetGuiManager()->Quit();
}

VOID CComponent::Close(VOID)
{
	if(GetGuiManager() != NULL)
		GetGuiManager()->Quit();

	if(GetParent() != NULL)
		GetParent()->RemoveChild(this);
}

VOID CComponent::ClearRemoveflag(VOID)
{
	m_pImpl->m_bRemoveflag = false;
}

// 1. Priority가 같은 녀석들중 제일앞(가장위에뜬다.)에 끼워넣는다.
// 2. Priority가 낮은 녀석들은 뒤로 보낸다. 
// 3. Priority가 높은 녀석들은 앞으로 보낸다. 

VOID CComponent::AddChild( CComponent* pChild )
{
	if( HasChild( pChild ) )
		return;

	m_pImpl->m_SigChildBeforeAdd( pChild );
	
	std::list<CComponent*>::iterator it;
	BOOL bNotInserted = TRUE;

	for( it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		CComponent* pAddedChildComp = reinterpret_cast<CComponent*>( (*it) );

		if( pChild->GetPriority() >= pAddedChildComp->GetPriority() )
		{
			m_pImpl->m_stlChildren.insert( it, pChild );
			bNotInserted = FALSE;
			break;
		}
	}
	
	if( bNotInserted )
		m_pImpl->m_stlChildren.push_back( pChild );

	pChild->SetParent( this );	

	m_pImpl->m_SigChildAdd( pChild );
}

VOID CComponent::RemoveChild( CComponent* pChild )
{
	m_pImpl->m_SigChildBeforeRemove( pChild );

	pChild->SetParent( NULL );
	m_pImpl->m_stlChildren.remove( pChild );

	m_pImpl->m_SigChildRemove( pChild );
}

VOID CComponent::AddComponents(CComponentManager *pComponentManager)
{
	/*
	std::vector<CComponent *> components;
	((CComponentManager_Generic *) pComponentManager)->GetRootComponentList(components);

	for (
		std::vector<CComponent *>::iterator it = components.begin();
		it != components.end();
		it++)
	{
		AddChild(*it);
	}
	*/
	
}


VOID CComponent::FindPreferredSize(VOID)
{
	m_pImpl->FindPreferredSize();
}

CComponent* CComponent::FindTabID(int nTabID)
{
	if ( m_pImpl->m_nTabID == nTabID )
		return this;

	std::list<CComponent*>::iterator it;
	for( it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		CComponent* comp = (*it)->FindTabID( nTabID );
		if( comp )  
			return comp;
	}

	return NULL;
}

// 현재 Raise와 별다른 차이 가 없는 기능을 하고 있음. 주석처리함.
VOID CComponent::TopTabOrder(CComponent *pComp)
{
	//std::list<CComponent*>::iterator it;

	//for(it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	//{
	//	CComponent* comp = (*it);
	//	if(comp == pComp) 
	//	{
	//		m_pImpl->m_stlChildren.erase(it);
	//		break;
	//	}
	//}

	////if(it != m_pImpl->m_stlChildren.end())
	//m_pImpl->m_stlChildren.push_front(pComp);
}

// 별 의미 없는 로직을 수행하고 있어 주석처리함. 
// pParent->TopTabOrder( this ); 로 해야 함.
VOID CComponent::TopTabOrder(VOID)
{
//	CComponent *pParent = GetParent();
//	if(pParent)
//		pParent->TopTabOrder();
}

VOID CComponent::SetAlpha( unsigned char ucAlpha, BOOL bMemo /* = TRUE  */ )
{
	// 설정값 저장
	if( bMemo )
		m_pImpl->m_ucAlpha = ucAlpha;
	// 자신 변경
	m_pImpl->m_SigSetAlpha( ucAlpha );
	// 자식 변경
	for ( std::list<CComponent *>::iterator it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		(*it)->SetAlpha( ucAlpha );
	}
}

VOID CComponent::SetColor(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, BOOL bMemo /* = TRUE  */)
{
	// 설정값 저장
	if( bMemo )
		m_pImpl->m_uiColor = RGB( ucRed, ucGreen, ucBlue );
	// 자신 변경
	m_pImpl->m_SigSetColor( ucRed, ucGreen, ucBlue );
	// 자식 변경
	for ( std::list<CComponent *>::iterator it = m_pImpl->m_stlChildren.begin() ; it != m_pImpl->m_stlChildren.end() ; ++it )
	{
		(*it)->SetColor( ucRed, ucGreen, ucBlue );
	}
}

VOID CComponent::SetClippingMode( bool bClipping )
{
	m_pImpl->m_bClipping = bClipping;

	SetPosition( GetPosition() );
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<const CComponentOptions &>& CComponent::SigSetOptions(VOID)
{
	return m_pImpl->m_SigSetOptions;
}

CSignal_v0 &CComponent::SigPaint()
{
	return m_pImpl->m_SigPaint;
}

CSignal_v1<const CKey &>& CComponent::SigMouseDown(VOID)
{
	return m_pImpl->m_SigMouseDown;
}

CSignal_v1<const CKey &>& CComponent::SigMouseUp(VOID)
{
	return m_pImpl->m_SigMouseUp;
}

CSignal_v2<int, int>& CComponent::SigTracking(VOID)
{
	return m_pImpl->m_SigTracking;
}

CSignal_v2<const CKey &,int& >& CComponent::SigDragDropQuery(VOID)
{
	return m_pImpl->m_SigDragDropQuery;
}

CSignal_v4<CComponent *,const CKey &,int,bool&>& CComponent::SigDragDropRevote(VOID)
{
	return m_pImpl->m_SigDragDropRevote;
}

CSignal_v1<int>& CComponent::SigDragDropNotProc(VOID)
{
	return m_pImpl->m_SigDragDropNotProc;
}


CSignal_v3<CComponent *, CInputDevice *, const CKey &>& CComponent::SigKeyDown(VOID)
{
	return m_pImpl->m_SigKeyDown;
}

CSignal_v3<CComponent *, CInputDevice *, const CKey &> &CComponent::SigKeyUp(VOID)
{
	return m_pImpl->m_SigKeyUp;
}

CSignal_v3<int, int, int> &CComponent::SigMouseMove(VOID)
{
	return m_pImpl->m_SigMouseMove;
}


CSignal_v3<int ,short ,CPos& >& CComponent::SigMouseWheel(VOID)
{
	return m_pImpl->m_SigMouseWheel;
}

CSignal_v0& CComponent::SigGotFocus(VOID)
{
	return m_pImpl->m_SigGotFocus;
}

CSignal_v0& CComponent::SigLostFocus(VOID)
{
	return m_pImpl->m_SigLostFocus;
}

CSignal_v2<int, int>& CComponent::SigResize(VOID)
{
	return m_pImpl->m_SigResize;
}

CSignal_v2<int, int>& CComponent::SigMove(VOID)
{
	return m_pImpl->m_SigMove;
}

CSignal_v1<CComponent *>& CComponent::SigMouseEnter(VOID)
{
	return m_pImpl->m_SigMouseEnter;
}

CSignal_v1<CComponent *>& CComponent::SigMouseLeave(VOID)
{
	return m_pImpl->m_SigMouseLeave;
}

CSignal_v2<int &, int &>& CComponent::SigTransformCoords(VOID)
{
	return m_pImpl->m_SigTransformCoords;
}

CSignal_v1<CComponent *>& CComponent::SigChildAdd(VOID)
{
	return m_pImpl->m_SigChildAdd;
}

CSignal_v1<CComponent *>& CComponent::SigChildRemove(VOID)
{
	return m_pImpl->m_SigChildRemove;
}

CSignal_v1<CComponent *>& CComponent::SigBeforeRemove(VOID)
{
	return m_pImpl->m_SigBeforeRemove;
}

CSignal_v1<CComponent *>& CComponent::SigChildBeforeAdd(VOID)
{
	return m_pImpl->m_SigChildBeforeAdd;
}

CSignal_v1<CComponent *>& CComponent::SigChildBeforeRemove(VOID)
{
	return m_pImpl->m_SigChildBeforeRemove;
}

CSignal_v1<CPos &>& CComponent::SigGetMinimumSize(VOID)
{
	return m_pImpl->m_SigGetMinimumSize;
}

CSignal_v1<CPos &>& CComponent::SigGetMaximumSize(VOID)
{
	return m_pImpl->m_SigGetMaxiumSize;
}

CSignal_v1<CPos &>& CComponent::SigGetPreferredSize(VOID)
{
	return m_pImpl->m_SigGetPreferredSize;
}

CSignal_v3<int , int, int>& CComponent::SigMessage(VOID) 
{
	return m_pImpl->m_SigSendMsg; 
}

VOID CComponent::SendMsg (CComponent *pComponent,int nMsg,int lParam,int wParam)
{
	pComponent->SigMessage()(nMsg,lParam,wParam);
}

CSignal_v1<unsigned char>& CComponent::SigSetAlpha(VOID)
{
	return m_pImpl->m_SigSetAlpha;
}

CSignal_v3<unsigned char, unsigned char, unsigned char>& CComponent::SigSetColor(VOID)
{
	return m_pImpl->m_SigSetColor;
}

END_GUI