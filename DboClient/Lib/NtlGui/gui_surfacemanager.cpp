#include "gui_precomp.h"
#include "gui_surfacemanager.h"
#include "gui_surfacemanager_generic.h"
#include "gui_componenttype.h"
#include "gui_resourcemanager.h"
#include "gui_texturemanager.h"
#include "gui_texture.h"

START_GUI

CSurfaceManager::CSurfaceManager(CResourceManager *pResourceManager)
:m_pImpl(NULL),m_pResourceManager(pResourceManager)
{
	m_pImpl = NTL_NEW CSurfaceManager_Generic();
	m_pImpl->AddRef();
}


CSurfaceManager::~CSurfaceManager()
{
	if(m_pImpl)
		m_pImpl->ReleaseRef();
}

bool CSurfaceManager::AddPage(const std::string& str)
{
	return m_pImpl->AddPage(str);
}

VOID CSurfaceManager::RemovePage(const std::string& str)
{
	m_pImpl->RemovePage(str); 
}

CSurface CSurfaceManager::CreateSurface(const std::string& strTex)
{
	CSurface surface;
	if(m_pResourceManager == NULL)
		return surface;

	surface.m_pTexture = m_pResourceManager->CreatureTexture(strTex);

	int nWidth = 0, nHeight = 0;
	if(surface.m_pTexture)
	{
		nWidth = surface.m_pTexture->GetWidth();
		nHeight = surface.m_pTexture->GetHeight();
	}
		
	surface.m_SnapShot.rtRect.left = 0;   
	surface.m_SnapShot.rtRect.top = 0;   
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + nWidth; 
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + nHeight; 

	surface.m_SnapShot.uRed = 255;  
	surface.m_SnapShot.uGreen = 255;  
	surface.m_SnapShot.uBlue = 255;  
	surface.m_SnapShot.uAlpha = 255;

	surface.m_SnapShot.UVs[0] = 0;
	surface.m_SnapShot.UVs[1] = 0;
	surface.m_SnapShot.UVs[2] = 1;
	surface.m_SnapShot.UVs[3] = 1;

	surface.m_SnapShot.uBlend = 0;

	surface.m_Original = surface.m_SnapShot;

	return surface;
}

VOID CSurfaceManager::RemoveSurface(CSurface& surface)
{
	g_TextureMgr.DeleteTexture(surface.m_pTexture);

	surface.m_pTexture = NULL;
}

CSurface CSurfaceManager::GetSurface(const std::string& strPage,const std::string& strName)
{
	CSurface surface;
	CSurfacePage *pPage = m_pImpl->GetPage(strPage);
	if(pPage == NULL)
		return surface;

	const CComponentOptions *pOptions = pPage->GetOptions(strName);
	if(pOptions == NULL)
		return surface;
	
	if(pOptions->Exists("resource_file") &&  pOptions->Exists("resource_name"))
	{
		std::string strFile = pOptions->GetValue("resource_file");
		std::string strResource = pOptions->GetValue("resource_name");
		if(m_pResourceManager != NULL)
			surface.m_pTexture = m_pResourceManager->GetTextureHandle(strFile,strResource);
	}
	
	surface.m_SnapShot.rtRect.left = pOptions->GetValueAsInt("x");   
	surface.m_SnapShot.rtRect.top = pOptions->GetValueAsInt("y");   
	surface.m_SnapShot.rtRect.right = surface.m_SnapShot.rtRect.left + pOptions->GetValueAsInt("width"); 
	surface.m_SnapShot.rtRect.bottom = surface.m_SnapShot.rtRect.top + pOptions->GetValueAsInt("height"); 
	surface.m_SnapShot.uRed = pOptions->GetValueAsInt("color_red");  
	surface.m_SnapShot.uGreen = pOptions->GetValueAsInt("color_green");  
	surface.m_SnapShot.uBlue = pOptions->GetValueAsInt("color_blue");  
	surface.m_SnapShot.uAlpha = pOptions->GetValueAsInt("color_alpha");
		
	int nUV0 = pOptions->GetValueAsInt("uv_left");
	int nUV1 = pOptions->GetValueAsInt("uv_top");
	int nUV2 = pOptions->GetValueAsInt("uv_right");
	int nUV3 = pOptions->GetValueAsInt("uv_bottom");
	
	int nWidth = 0, nHeight = 0;
	if(surface.m_pTexture)
	{
		nWidth = surface.m_pTexture->GetWidth(); 
		nHeight = surface.m_pTexture->GetHeight();
	}
		
	surface.m_SnapShot.UVs[0] = 0;
	surface.m_SnapShot.UVs[1] = 0;
	surface.m_SnapShot.UVs[2] = 1;
	surface.m_SnapShot.UVs[3] = 1;

	if(nWidth > 0 && nHeight > 0)
	{
		surface.m_SnapShot.UVs[0] = (float)nUV0/(float)nWidth;
		surface.m_SnapShot.UVs[1] = (float)nUV1/(float)nHeight;
		surface.m_SnapShot.UVs[2] = (float)nUV2/(float)nWidth;
		surface.m_SnapShot.UVs[3] = (float)nUV3/(float)nHeight;
	}

	surface.m_SnapShot.uBlend = pOptions->GetValueAsInt("blend");  

	surface.m_Original = surface.m_SnapShot;

	return surface;
}

CSurfacePage* CSurfaceManager::GetSurfacePage( const std::string& strFilename )
{
	return m_pImpl->GetPage( strFilename );
}

END_GUI