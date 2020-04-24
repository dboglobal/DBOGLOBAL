#include "precomp_dboclient.h"
#include "DumpTargetFile.h"
#include "NtlDebug.h"

CDumpTargetFile::CDumpTargetFile()
{
	strncpy_s(m_pcFileName, "dbodump.txt", 250);
	m_pcFileName[249] = NULL;
}

CDumpTargetFile::~CDumpTargetFile()
{
}

void CDumpTargetFile::Active(RwBool bActive)
{
	CNtlDumpTarget::Active(bActive); 

}

void CDumpTargetFile::Dump(void)
{
	if(!IsActive())
		return;

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, m_pcFileName, "a+") != 0)
		return;
#else
	fp = fopen(m_pcFileName, "a+");
	if(fp == NULL)
		return;
#endif
	
	const char *pData = GetStream();
	int iLen = (int)strlen(pData);
	fwrite(pData, sizeof(char) * iLen, 1, fp);
	fclose(fp);

	CNtlDumpTarget::Dump(); 
}

void CDumpTargetFile::SetDumpFileName(char* pcFileName)
{
	strncpy_s(m_pcFileName, pcFileName, 250);
	m_pcFileName[249] = NULL;
}