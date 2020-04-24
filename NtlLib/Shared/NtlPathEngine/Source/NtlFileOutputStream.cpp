#include "Precomp_NtlPathEngine.h"
#include "NtlFileOutputStream.h"

CNtlFileOutputStream::CNtlFileOutputStream( const char* name )
: m_fp(NULL)
{
	if( fopen_s( &m_fp, name, "wb" ) != 0 )
	{
		return;
	}
}

CNtlFileOutputStream::~CNtlFileOutputStream()
{
	int errorOccurred = fclose(m_fp);
}

void CNtlFileOutputStream::put( const char* data, unsigned long dataSize )
{
	/*size_t written = fwrite(data, 1, dataSize, m_fp);
	int flushResult = fflush(m_fp);*/

	size_t written = fwrite(data, dataSize, 1, m_fp);
	int flushResult = fflush(m_fp);
}