#include "gui_precomp.h"
#include "gui_globalcomponent.h"

START_GUI

CGlobalComponent::CGlobalComponent(VOID)
: m_pRsrMgr( NULL ), m_pSrfMgr( NULL ), m_pFrmMgr( NULL ), m_pGuiMgr( NULL ),
  m_pToolTip( NULL ), m_hComponentPage( 0 )
{
	
}

CGlobalComponent::~CGlobalComponent(VOID)
{
	
}

////////////////////////////////////////////////////////////////////////////////

CGlobalComponent* CGlobalComponent::GetInstance()
{
	static CGlobalComponent globalcomponent;
	return &globalcomponent;
}

////////////////////////////////////////////////////////////////////////////////

VOID CGlobalComponent::SetToolTip( CToolTip* pToolTip )
{
	NTL_DELETE( m_pToolTip );
			
	m_pToolTip = pToolTip;
}

VOID CGlobalComponent::SetToolTip( std::string strName )
{
	NTL_DELETE( m_pToolTip );

	m_pToolTip = (CToolTip*)m_pFrmMgr->GetComponent( m_hComponentPage, strName );
}

VOID CGlobalComponent::CreateComponent( std::string rsrFile, std::string srfFile, std::string frmFile, 
									    CResourceManager* pRsrMgr, CSurfaceManager* pSrfMgr, 
										CComponentManager* pFrmMgr, CComponent* pGuiMgr )
{
	m_pRsrMgr = pRsrMgr;
	m_pSrfMgr = pSrfMgr;
	m_pFrmMgr = pFrmMgr;	
	m_pGuiMgr = pGuiMgr;
	m_strRsr  = rsrFile;
	m_strSrf  = srfFile;
	
	m_pRsrMgr->AddPage( m_strRsr );
	m_pSrfMgr->AddPage( m_strSrf );
	m_hComponentPage = m_pFrmMgr->AddPage( frmFile );

	m_pFrmMgr->CreateComponents( m_hComponentPage, m_pGuiMgr, m_pSrfMgr );
}

VOID CGlobalComponent::DeleteComponent(VOID)
{
	NTL_DELETE( m_pToolTip );

	if( m_pRsrMgr )
	{
		m_pRsrMgr->RemovePage( m_strRsr );
		m_pRsrMgr = NULL;
	}
	if( m_pSrfMgr )
	{
		m_pSrfMgr->RemovePage( m_strSrf );
		m_pSrfMgr = NULL;
	}
	if( m_pFrmMgr )
	{
		m_pFrmMgr->RemovePage( m_hComponentPage );
		m_pFrmMgr = NULL;
	}
}

END_GUI