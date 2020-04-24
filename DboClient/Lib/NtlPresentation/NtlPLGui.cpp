#include "precomp_ntlpresentation.h"
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

CNtlPLGui::CNtlPLGui()
{
	m_strName		= "BLANK";
	m_pThis			= 0;
	m_hCompPage		= 0;
	m_bSerialize	= FALSE;
}

CNtlPLGui::CNtlPLGui(const RwChar *pName)
{
	m_strName		= pName;
	m_pThis			= 0;
	m_hCompPage		= 0;
	m_bSerialize	= FALSE;
}

CNtlPLGui::~CNtlPLGui()
{
	UnLinkAll();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
}

void CNtlPLGui::AddResPage (const std::string& str)
{
	gui::CResourceManager *pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	pResMgr->AddPage(str);
	m_strResPage = str;
}

void CNtlPLGui::AddSurPage	(const std::string& str)
{
	gui::CSurfaceManager *pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();
	pSurMgr->AddPage(str);
	m_strSurPage = str;
}

void CNtlPLGui::AddCompPage (const std::string& str)
{
	gui::CComponentManager *pCompMgr = CNtlPLGuiManager::GetInstance()->GetComponentManager();
	m_hCompPage = pCompMgr->AddPage(str);
	m_strCompPage = str;
}

void CNtlPLGui::RemoveResPage (const std::string& str)
{
	gui::CResourceManager *pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	pResMgr->RemovePage(str);
	m_strResPage = "";
}

void CNtlPLGui::RemoveSurPage (const std::string& str)
{
	gui::CSurfaceManager *pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();
	pSurMgr->RemovePage(str);
	m_strSurPage = "";
}

void CNtlPLGui::RemoveCompPage	(const std::string& str)
{
	gui::CComponentManager *pCompMgr = CNtlPLGuiManager::GetInstance()->GetComponentManager();
	pCompMgr->RemovePage(m_hCompPage);
}

RwBool CNtlPLGui::Create(const std::string& strResPage, const std::string& strSurPage, const std::string& strCompPage)
{
	if( !strResPage.empty() )
		AddResPage( strResPage );
	if( !strSurPage.empty() )
		AddSurPage( strSurPage );
	if( !strCompPage.empty() )
		AddCompPage( strCompPage );

	return TRUE;
}

void CNtlPLGui::Destroy(void)
{
	if( !m_strSurPage.empty() )
		RemoveSurPage(m_strSurPage);
	if( !m_strResPage.empty() )
		RemoveResPage(m_strResPage);
	if( !m_strCompPage.empty() )
		RemoveCompPage(m_strCompPage);
}

void CNtlPLGui::Update(RwReal fElapsed)
{
}

void CNtlPLGui::UpdateBeforeCamera(RwReal fElapsed)
{
}

void CNtlPLGui::Render(void)
{
}

RwBool CNtlPLGui::LoadUserOption(CNtlSerializer& s)
{
	RwChar		chSerializeType;
	CRectangle	rtPos;		
	
	while(1)
	{
		s >> chSerializeType;
		switch(chSerializeType)
		{
		case PLGUI_SERIALIZE_POS_X:
			s >> rtPos.left;
			break;
		case PLGUI_SERIALIZE_POS_Y:
			s >> rtPos.top;
			if(m_pThis)
				m_pThis->SetPosition(rtPos.left, rtPos.top);
			break;
		case PLGUI_SERIALIZE_END:
			return TRUE;
			break;
		default:
			return FALSE;
			break;
		}
	}

	return TRUE;
}

RwBool CNtlPLGui::SaveUserOption(CNtlSerializer& s)
{
	if(m_pThis == NULL)
	{
		s << (char)PLGUI_SERIALIZE_END;
		return TRUE;
	}
	
	const CRectangle& rt = m_pThis->GetPosition();

	s << (char)PLGUI_SERIALIZE_POS_X;
	s << rt.left;
	s << (char)PLGUI_SERIALIZE_POS_Y;
	s << rt.top;

	s << (char)PLGUI_SERIALIZE_END;
	
	return TRUE;
}

void CNtlPLGui::SetName( const RwChar* pName )
{
	m_strName = pName;
}

void CNtlPLGui::CreateComponents(gui::CComponent* pParent)
{
	gui::CSurfaceManager *pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();
	gui::CComponentManager *pCompMgr = CNtlPLGuiManager::GetInstance()->GetComponentManager();
	pCompMgr->CreateComponents (m_hCompPage, pParent, pSurMgr);
}

void CNtlPLGui::DestroyComponents(void)
{
	gui::CComponentManager *pCompMgr = CNtlPLGuiManager::GetInstance()->GetComponentManager();
	pCompMgr->DestroyComponents (m_hCompPage);

	m_pThis	= NULL;
}

gui::CComponent* CNtlPLGui::GetComponent(const std::string& str)
{
	gui::CComponentManager *pCompMgr = CNtlPLGuiManager::GetInstance()->GetComponentManager();
	return pCompMgr->GetComponent(m_hCompPage,str);
}

void CNtlPLGui::Resize(int iWidth, int iHeight)
{
	CRectangle rect = m_pThis->GetClientRect();
	rect.right = rect.left + rect.GetWidth();
	rect.bottom = rect.top + rect.GetHeight();

	//m_pThis->Resize(rect);
	m_pThis->SetPosition( rect );
}

void CNtlPLGui::RaiseTop(void)
{
	if(m_pThis)
		m_pThis->Raise();
}

void CNtlPLGui::Lower(void)
{
	if(m_pThis)
		m_pThis->Lower();
}

void CNtlPLGui::Popup(bool bPopup)
{
	if(m_pThis)
		m_pThis->Popup(bPopup);
}

void CNtlPLGui::Show(bool bShow)
{
	if(m_pThis)
		m_pThis->Show (bShow);
}

RwBool CNtlPLGui::IsShow(void)
{
	if(!m_pThis)
		return FALSE;

	if(m_pThis->IsVisible())
		return TRUE;

	return FALSE;
}

void CNtlPLGui::Enable(bool bEnable)
{
	if(m_pThis)
		m_pThis->Enable (bEnable);
}

RwBool CNtlPLGui::IsEnable(void)
{
	if(!m_pThis)
		return FALSE;

	return m_pThis->IsEnabled ();
}

void CNtlPLGui::SetMovable(RwBool bMovable)
{
	m_pThis->SetMovable(bMovable);
}

void CNtlPLGui::SetPosition(int nX,int nY)
{
	if(m_pThis)
	{
		//CRectangle OldPosition = m_pThis->GetPosition();

		m_pThis->SetPosition(nX,nY);		
	}
}

void CNtlPLGui::SetPosition(const CRectangle &rect)
{
	//CRectangle OldPosition = m_pThis->GetPosition();

	m_pThis->SetPosition(rect);	
}

CRectangle CNtlPLGui::GetPosition (void)
{
	CRectangle rect(0, 0, 0, 0);
	if(m_pThis)
		rect = m_pThis->GetPosition();

	return rect;
}

RwInt32 CNtlPLGui::GetWidth()
{
	return m_pThis->GetWidth();
}

RwInt32 CNtlPLGui::GetHeight()
{
	return m_pThis->GetHeight();
}

void CNtlPLGui::SetAlpha(RwUInt8 byAlpha)
{
	if( m_pThis )
		m_pThis->SetAlpha( byAlpha );
}

void CNtlPLGui::CenterAlign( RwUInt32 uiWidth, RwUInt32 uiHeight )
{
	CRectangle rect = GetPosition();
	RwInt32 iPosX = ( (RwInt32)uiWidth - rect.GetWidth() ) / 2;
	RwInt32 iPosY = ( (RwInt32)uiHeight - rect.GetHeight() ) / 2;

	SetPosition( iPosX, iPosY );
}

void CNtlPLGui::Link(gui::CDialog* pDialog)
{
	ListDialog::iterator it = m_listLinked.begin();
	for( ; it != m_listLinked.end() ; ++it )
	{
		if( *it == pDialog )
		{
			return;
		}
	}

	m_listLinked.push_back(pDialog);
}

void CNtlPLGui::UnLink(gui::CDialog* pDialog)
{
	ListDialog::iterator it = m_listLinked.begin();
	for( ; it != m_listLinked.end() ; ++it )
	{
		if( *it == pDialog )
		{
			m_listLinked.erase(it);
			return;
		}
	}
}

void CNtlPLGui::UnLinkAll()
{
	m_listLinked.clear();
}

void CNtlPLGui::RaiseLinked()
{
	m_pThis->Raise();

	ListDialog::iterator it = m_listLinked.begin();
	for( ; it != m_listLinked.end() ; ++it )
	{
		(*it)->Raise();
	}
}

void CNtlPLGui::ShowLinked(bool bShow)
{
	ListDialog::iterator it = m_listLinked.begin();
	for( ; it != m_listLinked.end() ; ++it )
	{
		(*it)->Show(bShow);
	}
}

void CNtlPLGui::MoveLinkedPLGui(int nAdjustX, int nAdjustY)
{
	ListDialog::iterator it = m_listLinked.begin();
	for( ; it != m_listLinked.end() ; ++it )
	{
		CRectangle rect = (*it)->GetPosition();

		rect.left += nAdjustX;
		rect.top += nAdjustY;
		rect.right += nAdjustX;
		rect.bottom += nAdjustY;

		(*it)->SetPosition(rect);
		(*it)->Raise();
	}
}

RwBool CNtlPLGui::IsHaveLinkedGui()
{
	if( m_listLinked.size() > 0 )
		return true;

	return false;
}

void CNtlPLGui::EnableSerialize(RwBool bSerialize)
{
	m_bSerialize = bSerialize;
}

RwBool CNtlPLGui::IsEnableSerialize(void) const
{
	return m_bSerialize;
}

CRectangle CNtlPLGui::GetLinkedArea()
{
	CRectangle rtArea = m_pThis->GetScreenRect();
	ListDialog::iterator it = m_listLinked.begin();

	for( ; it != m_listLinked.end() ; ++it )
	{
		CRectangle rect = (*it)->GetPosition();

		if( rect.left < rtArea.left )
			rtArea.left = rect.left;

		if( rect.top < rtArea.top )
			rtArea.top = rect.top;

		if( rect.right > rtArea.right )
			rtArea.right = rect.right;

		if( rect.bottom > rtArea.bottom )
			rtArea.bottom = rect.bottom;
	}

	return rtArea;
}