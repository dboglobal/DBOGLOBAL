////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CGlobalComponent
// Desc: GUI Library내에서사용되는 Component 관리. 
//		 Phase에 관계없이 전역으로 생성되야할 Component의 관리.	
//			 
// 2006.03.6 Peessi@hitel.net   
//
// To Be Desired: 
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_GLOBALCOMPONENT_H__
#define __GUI_GLOBALCOMPONENT_H__

START_GUI

class CGlobalComponent
{
public:
//! Destructor:
	~CGlobalComponent(VOID);

//! Attributes:
	static CGlobalComponent* GetInstance();

//! Operations:
	VOID SetToolTip( CToolTip* pToolTip );	// 주의!! : new로 생성한 포인터만 넘겨야 한다.
	VOID SetToolTip( std::string strName );	// 주의!! : CreateComponent가 선행되어야 한다.
	CToolTip* GetToolTip() { return m_pToolTip; }
	
	VOID CreateComponent( std::string rsrFile, std::string srfFile, std::string frmFile, 
						  CResourceManager* pRsrMgr, CSurfaceManager* pSrfMgr, CComponentManager* pFrmMgr,
						  CComponent* pGuiMgr );
	VOID DeleteComponent(VOID);

protected:
//! Variables:
	CToolTip*		m_pToolTip;
	COMPPAGE_HANDLE m_hComponentPage;

	CResourceManager*	m_pRsrMgr;
	CSurfaceManager*	m_pSrfMgr;
	CComponentManager*	m_pFrmMgr;
	CComponent*			m_pGuiMgr;

	std::string			m_strRsr;
	std::string			m_strSrf;
	
//! Etc:
	CGlobalComponent(VOID);				
	CGlobalComponent( CGlobalComponent& copy );
};

static CGlobalComponent* GetGlobalComponent(VOID)
{
	return CGlobalComponent::GetInstance();
}

END_GUI

#endif//__GUI_GLOBALCOMPONENT_H__