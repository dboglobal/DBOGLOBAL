#include "precomp_dboclient.h"
#include "LoginGuiGroup.h"
#include "NtlDebug.h"
#include "NtlPLGuiManager.h"

#include "LogoGui.h"
#include "ContractGui.h"
#include "LoginGui.h"
#include "MsgBoxManager.h"


CLogInGuiGroup::CLogInGuiGroup()
: m_pLogoGui(NULL)
, m_pContractGui(NULL)
, m_pLogin(NULL)
{	
}

CLogInGuiGroup::~CLogInGuiGroup()
{
}


RwBool CLogInGuiGroup::Create(void)
{
	NTL_FUNCTION("CLogInGuiGroup::Create");

	// logo gui
	m_pLogoGui = NTL_NEW CLogoGui("LogoGui");
	if(!m_pLogoGui->Create())
	{
		m_pLogoGui->Destroy(); 
		NTL_DELETE(m_pLogoGui);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pLogoGui); 

	// contract gui
	m_pContractGui = NTL_NEW CContractGui("ContractGui");
	if (!m_pContractGui->Create())
	{
		m_pContractGui->Destroy();
		NTL_DELETE(m_pContractGui);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pContractGui);

	// login gui
	m_pLogin = NTL_NEW CLogInGui("LogInGui");
	if(!m_pLogin->Create())
	{
		m_pLogin->Destroy(); 
		NTL_DELETE(m_pLogin);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pLogin); 

	NTL_RETURN(TRUE);
}

void CLogInGuiGroup::Destroy(void)
{
	NTL_FUNCTION("CLogInGuiGroup::Destroy");

	if(m_pLogoGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pLogoGui);
		m_pLogoGui->Destroy();
		NTL_DELETE(m_pLogoGui);
	}

	if (m_pContractGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pContractGui);
		m_pContractGui->Destroy();
		NTL_DELETE(m_pContractGui);
	}

	if(m_pLogin)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pLogin);
		m_pLogin->Destroy();
		NTL_DELETE(m_pLogin);
	}



	NTL_RETURNVOID();
}