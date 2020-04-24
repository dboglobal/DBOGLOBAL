#include "StdAfx.h"
#include ".\extfilter.h"
#include <algorithm>

CExtFilter::CExtFilter(void)
{
	Create();
}

CExtFilter::~CExtFilter(void)
{
	Destroy();
}

void CExtFilter::Create()
{
	ExtDefinition();
}

void CExtFilter::Destroy()
{
	m_stlExtMap.clear();
}

BOOL CExtFilter::IsValid( LPCTSTR szPath )
{
	return GetType( szPath ) ? TRUE : FALSE;
}

BOOL CExtFilter::GetExt( LPCTSTR szPath, stlString* pExt )
{
	stlString stlExt;
	stlString::size_type st;

	if( !strcmp( szPath , "" ) || szPath == NULL )
		return FALSE;

	stlExt = szPath;

	st = stlExt.find( '.' );

	if( st == stlString::npos )
		return FALSE;
	else
	{
		*pExt = stlExt.substr( st, stlString::npos );
		return TRUE;
	}
    
}

CExtFilter::EXTTYPE CExtFilter::GetType( LPCTSTR szPath )
{
	stlString stlExt;
	stlExtMap::iterator it;

	if( !GetExt( szPath, &stlExt ) )
		return NONE;

	transform ( stlExt.begin(), stlExt.end(),   // source
			    stlExt.begin(),					// destination
				tolower );						// operation

    it = m_stlExtMap.find( stlExt );

	if( it == m_stlExtMap.end() )
		return NONE;
	else
    	return it->second;
}

void CExtFilter::ExtDefinition()
{
	char* ImageExts[] = { ".jpe",".jpg",".jpeg",".lif",".bmp",".ico",".pbm",".pgm",".pnm",".ppm",".png",".bw"
		".rgb",".rgba",".sgi",".tga",".tif",".tiff",".pcx",".xpm",".psp",".psd",".pix",".pxr",".cut",".dcx",".dds" };
	char* ResourceExt = ".rsr";
	char* SurfaceExt  = ".srf";
	char* ComponentExt= ".frm";

    INT	   ExtCount = sizeof( ImageExts ) / 4;

	for( INT i = 0 ; i < ExtCount ; i++ )
	{
		m_stlExtMap[ImageExts[i]] = IMAGE;
    }

	m_stlExtMap[ResourceExt] = RESOURCE;
	m_stlExtMap[SurfaceExt]	 = SURFACE;
	m_stlExtMap[ComponentExt]= COMPONENT;
}