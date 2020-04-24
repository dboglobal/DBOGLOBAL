#include "gui_precomp.h"
#include "gui_resourcepage.h"
#include "gui_resourcefilepaser.h"
#include "gui_resourcemanager.h"
#include "gui_surface.h"
#include "gui_texturemanager.h"

START_GUI


CResourcePage::CResourcePage()
: m_pPaser(NULL),m_strFileName(""),m_nRefCount(0),m_bCreate(FALSE)
{
}

CResourcePage::CResourcePage(const std::string &strFileName)
: m_pPaser(NULL),m_strFileName(strFileName),m_nRefCount(0)
{
	Parse();
}

CResourcePage::~CResourcePage()
{
	NTL_DELETE( m_pPaser );

	DeleteTexture();
}

VOID CResourcePage::Parse()
{
	m_pPaser = NTL_NEW CResourceFileParser(m_strFileName);
	
	std::string strError;
	m_pPaser->ReadError(strError);
	if(!strError.empty()) 
	{
		assert(0);
	}

	//: resource create
	CreateTexture();
}


VOID CResourcePage::CreateTexture(VOID)
{
	CTexture *pTexture;

	CResourceFileParser::ComponentInfo *pInfo;
	CResourceFileParser::stlCOMPONENT_SORT_MAP &stlSortMap = m_pPaser->GetSortedComponents( );
	CResourceFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;

	std::string strFilePath = ".\\texture\\gui\\";
	
	for (it=stlSortMap.begin();it!=stlSortMap.end();it++)
	{
		pInfo = (*it).second;
		if(pInfo->type != "resource")
			continue;

		std::string strFullPath;
		strFullPath = strFilePath + pInfo->Options.GetValue( "file" );
		
		pTexture = g_TextureMgr.CreateTexture((char*)pInfo->Options.GetValue("file").c_str());
		m_stlTextureMap.insert(stlTEXTURE_MAP::value_type(pInfo->name,pTexture));
	}

	m_bCreate = TRUE;
}

VOID CResourcePage::DeleteTexture(VOID)
{
	stlTEXTURE_MAP::iterator it;

	for(it=m_stlTextureMap.begin(); it!=m_stlTextureMap.end(); it++)
	{
		g_TextureMgr.DeleteTexture((*it).second);
	}

	m_bCreate = FALSE;
}

const CComponentOptions* CResourcePage::GetOptions(std::string strName) const
{
	CResourceFileParser::stlCOMPONENT_SORT_MAP &stlSortMap = m_pPaser->GetSortedComponents( );
	CResourceFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;
	
	for (it=stlSortMap.begin();it!=stlSortMap.end();it++)
	{
		CResourceFileParser::ComponentInfo *pInfo = (*it).second;
		if(pInfo->name == strName)
			return &pInfo->Options; 
	}

	return NULL;
}

const std::map<std::string,CTexture*>* CResourcePage::GetTextureMap(VOID) const
{
	return &m_stlTextureMap;
}

CTexture* CResourcePage::GetTextureHandle(std::string str) 
{
	stlTEXTURE_MAP::iterator the;
	the = m_stlTextureMap.find(str);
	if(the == m_stlTextureMap.end())
		return NULL;
	return (*the).second;
}

END_GUI

