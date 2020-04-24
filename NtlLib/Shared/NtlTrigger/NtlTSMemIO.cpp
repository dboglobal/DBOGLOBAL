#include "precomp_trigger.h"
#include "NtlTSMemIO.h"


CNtlTSMemInput::CNtlTSMemInput( const char* pBuf, int nBufSize )
{
	m_pBuf = pBuf;
	m_nBufSize = nBufSize;

	m_nCurPos = 0;
}

CNtlTSMemInput::~CNtlTSMemInput( void )
{
}

bool CNtlTSMemInput::Read( void* pData, int nSize )
{
	if ( m_nCurPos + nSize > m_nBufSize )
	{
		return false;
	}

	memcpy( pData, &m_pBuf[m_nCurPos], nSize );

	m_nCurPos += nSize;

	return true;
}
