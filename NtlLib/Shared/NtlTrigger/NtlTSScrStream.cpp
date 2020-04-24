#include "precomp_trigger.h"
#include "NtlTSScrStream.h"
#include "NtlTSScrParser.h"
#include "NtlTSScrSaver.h"


/** 
	Script stream
*/



CNtlTSScrStream::CNtlTSScrStream( CNtlTSScrFactory* pFactory, const std::string& strFileName, const char* pBuff /*= 0*/, int nSize /*= 0*/ )
: m_pFactory( pFactory ),
  m_strFileName( strFileName ),
  m_pBuff( pBuff ),
  m_nSize( nSize )
{
}

CNtlTSScrStream::~CNtlTSScrStream( void )
{
}

std::vector<CNtlTSScrObject*>& CNtlTSScrStream::GetObjList( void )
{
	return m_defObjList;
}

bool CNtlTSScrStream::Load( void )
{
	CNtlTSScrParser clParser( m_pFactory );

	if ( m_pBuff && 0 != m_nSize )
	{
		return clParser.Parsing( m_strFileName, m_pBuff, m_nSize, m_defObjList );
	}
	else
	{
		return clParser.Parsing( m_strFileName, m_defObjList );
	}
}

bool CNtlTSScrStream::Save( std::vector<CNtlTSScrObject*>& defObjList )
{
	CNtlTSScrSaver clSaver( m_pFactory );
	return clSaver.Save( m_strFileName, defObjList );
}