#include "precomp_ntlcore.h"
#include "NtlDumpTarget.h"
#include "NtlDebug.h"

#define NTL_DUMP_GROW_SIZE		1024

CNtlDumpTarget::CNtlDumpTarget()
{
	m_pBuffer = NTL_NEW RwChar [NTL_DUMP_GROW_SIZE];
	memset(m_pBuffer, 0, NTL_DUMP_GROW_SIZE);
	m_iCurrSize = NTL_DUMP_GROW_SIZE;
	m_iDataSize = 0;
	m_bActive = FALSE;
}

CNtlDumpTarget::~CNtlDumpTarget()
{
	if(m_pBuffer)
	{
		NTL_ARRAY_DELETE(m_pBuffer);
		m_pBuffer = NULL;
	}

	m_iCurrSize = 0;
	m_iDataSize = 0;
	m_bActive = FALSE;
}

RwBool CNtlDumpTarget::CheckBuffer(RwInt32 iLen)
{
	if(m_pBuffer == NULL)
		return FALSE;

	if(m_iCurrSize <= m_iDataSize + iLen)
	{
		RwChar *pTempBuffer = NTL_NEW RwChar [m_iCurrSize + NTL_DUMP_GROW_SIZE];
		memset(pTempBuffer, 0, m_iCurrSize + NTL_DUMP_GROW_SIZE);
		memcpy(pTempBuffer, m_pBuffer, m_iCurrSize);

		NTL_ARRAY_DELETE(m_pBuffer);
		m_pBuffer = pTempBuffer;
		m_iCurrSize += NTL_DUMP_GROW_SIZE;
	}

	return TRUE;
}


CNtlDumpTarget& CNtlDumpTarget::operator<<(char chData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", chData);
#else
	sprintf(chBuffer, "%d", chData);
#endif
	int nLen = (int)strlen(chBuffer);
	
	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(unsigned char byData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", byData);
#else
	sprintf(chBuffer, "%d", byData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(short shData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", shData);
#else
	sprintf(chBuffer, "%d", shData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(unsigned short wData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", wData);
#else
	sprintf(chBuffer, "%d", wData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(int iData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", iData);
#else
	sprintf(chBuffer, "%d", iData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(unsigned int uiData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", uiData);
#else
	sprintf(chBuffer, "%d", uiData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(long lData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%d", lData);
#else
	sprintf(chBuffer, "%d", lData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(__int64 i64Data)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	_i64toa_s(i64Data, chBuffer, 128, 10);
#else
	_i64toa(i64Data, chBuffer, 128);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(unsigned __int64 ui64Data)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	_ui64toa_s(ui64Data, chBuffer, 128, 10);
#else
	_ui64toa(ui64Data, chBuffer, 128);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(float fData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%f", fData);
#else
	sprintf(chBuffer, "%f", fData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(double dData)
{
	char chBuffer[128];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuffer, 128, "%f", dData);
#else
	sprintf(chBuffer, "%f", dData);
#endif
	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(char *pData)
{
	int nLen = (int)strlen(pData);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], pData, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator << (const char *pData)
{
	int nLen = (int)strlen(pData);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], pData, nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::operator<<(std::string	&str)
{
	int nLen = (int)str.c_str(); 

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], str.c_str(), nLen);
	m_iDataSize += nLen;
	
	return *this;
}

CNtlDumpTarget& CNtlDumpTarget::Format(const char* pData /*= NULL*/, ...)
{
	if(pData == NULL) 
		return *this;
	
	char chBuffer[1024];
	
	va_list args;
	va_start(args, pData);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s(chBuffer, 1024, pData, args);
#else
	vsprintf(chBuffer, pData, args);
#endif
	va_end(args);

	int nLen = (int)strlen(chBuffer);

	if(!CheckBuffer(nLen))
		return *this;

	memcpy(&m_pBuffer[m_iDataSize], chBuffer, nLen);
	m_iDataSize += nLen;

	return *this;
}

void CNtlDumpTarget::Dump(void)
{
	if(m_pBuffer)
		memset(m_pBuffer, 0, m_iCurrSize);

	m_iDataSize = 0;
}

void CNtlDumpTarget::Active(RwBool bActive)
{
	m_bActive = bActive;
}
