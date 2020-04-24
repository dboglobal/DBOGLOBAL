#include "gui_precomp.h"
#include "gui_surfacepage.h"
#include "gui_surfacefilepaser.h"
#include "gui_surface.h"


START_GUI


CSurfacePage::CSurfacePage()
: m_pPaser(NULL),m_strFileName(""),m_nRefCount(0)
{
}

CSurfacePage::CSurfacePage(const std::string &strFileName)
: m_pPaser(NULL),m_strFileName(strFileName),m_nRefCount(0)
{
	Parse();
}

CSurfacePage::~CSurfacePage()
{
	NTL_DELETE( m_pPaser );
}

VOID CSurfacePage::Parse()
{
	m_pPaser = NTL_NEW CSurfaceFileParser(m_strFileName);

	std::string strError;
	m_pPaser->ReadError(strError);
	if(!strError.empty())	
	{
		assert(0);
	}
}


const CComponentOptions* CSurfacePage::GetOptions(const std::string& strName) const
{
	CSurfaceFileParser::stlCOMPONENT_SORT_MAP &stlSortMap = m_pPaser->GetSortedComponents( );
	CSurfaceFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;
	
	for (it=stlSortMap.begin();it!=stlSortMap.end();it++)
	{
		CSurfaceFileParser::ComponentInfo *pInfo = (*it).second;
		if(pInfo->name == strName)
			return &pInfo->Options; 
	}

	return NULL;
}


END_GUI

