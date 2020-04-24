#include "gui_precomp.h"
#include "gui_resourcemanager.h"
#include "gui_resourcemanager_generic.h"
#include "gui_componenttype.h"
#include "gui_texturemanager.h"

START_GUI

CHyperTextBuilder CResourceManager::m_HypertextBuilder;

CResourceManager::CResourceManager()
:m_pImpl(NULL)
{
	m_pImpl = NTL_NEW CResourceManager_Generic();
	m_pImpl->AddRef();
}


CResourceManager::~CResourceManager()
{
	if(m_pImpl)
		m_pImpl->ReleaseRef();
}


//: resource page add
bool CResourceManager::AddPage(const std::string& str)
{
	return m_pImpl->AddPage(str);
}


//: resource page remove
VOID CResourceManager::RemovePage(const std::string& str)
{
	m_pImpl->RemovePage(str); 
}

VOID CResourceManager::CreateTexture (const std::string& strName)
{
	CResourcePage *pPage = m_pImpl->GetPage(strName);
	if(pPage == NULL)
			return;
	pPage->CreateTexture();
}

VOID CResourceManager::DeleteTexture (const std::string& strName)
{
	CResourcePage *pPage = m_pImpl->GetPage(strName);
	if(pPage == NULL)
			return;
	pPage->DeleteTexture();
}

CTexture* CResourceManager::CreatureTexture(const std::string& strTex)
{
	return g_TextureMgr.CreateTexture((char*)strTex.c_str());
}

CTexture* CResourceManager::GetTextureHandle(const std::string& strPage,const std::string& strName)
{
	CResourcePage *pPage = m_pImpl->GetPage(strPage);
	if(pPage == NULL)
		return NULL;

	return pPage->GetTextureHandle(strName);
}

bool CResourceManager::CreateHyperText(const std::string& strName)
{
	return m_HypertextBuilder.Build(strName.c_str());
}	

CTextNode* CResourceManager::GetHyperText (const std::string& strKey)
{
	CHyperText*	pHyperText = m_HypertextBuilder.GetHyperText();
	return pHyperText->GetText(strKey);
}

CResourcePage*	CResourceManager::GetResourcePage( const std::string& strFilename )
{
	return m_pImpl->GetPage( strFilename );
}

END_GUI