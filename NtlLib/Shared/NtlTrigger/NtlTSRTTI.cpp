#include "precomp_trigger.h"
#include "NtlTSRTTI.h"


/** 
	Runtime type infomation
*/


CNtlTSRTTI::CNtlTSRTTI( const char* pClassName, const CNtlTSRTTI* pBaseRTTI )
: m_pClassName( pClassName ), m_pBaseRTTI( pBaseRTTI )
{
}

bool CNtlTSRTTI::IsSameClass( const CNtlTSRTTI& clVal ) const
{
	return this == &clVal;
}

bool CNtlTSRTTI::IsDerivedClass( const CNtlTSRTTI& clVal ) const
{
	const CNtlTSRTTI* pFindRTTI = &clVal;

	while ( pFindRTTI )
	{
		if ( pFindRTTI == this )
		{
			return true;
		}

		pFindRTTI = pFindRTTI->m_pBaseRTTI;
	}

	return false;
}

bool CNtlTSRTTI::IsDerivedClass( const std::string& strClassName ) const
{
	if ( strClassName.empty() ) return false;

	const CNtlTSRTTI* pFindRTTI = this;

	while ( pFindRTTI )
	{
		if ( _stricmp( pFindRTTI->GetClassName(), strClassName.c_str() ) == 0 )
		{
			return true;
		}

		pFindRTTI = pFindRTTI->m_pBaseRTTI;
	}

	return false;
}
