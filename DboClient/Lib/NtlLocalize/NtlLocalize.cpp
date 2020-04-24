#include "stdafx.h"
#include "NtlLocalize.h"


unsigned char	g_byLocalType = NTL_LOCALIZE_DEV; // NTL_LOCALIZE_DEV, NTL_LOCALIZE_GLOBAL
SLocalConfig	*g_pLocalConfig = NULL;
char			*g_pDBOEncryptConfig = NULL;

#define	MAX_DBO_ENCRYPT_CONFIG		4096

void Local_Init(void)
{
	if(g_pLocalConfig == NULL)
	{
		g_pLocalConfig = new SLocalConfig;
		g_pLocalConfig->byLocalType = NTL_LOCALIZE_DEV;
		memset(g_pLocalConfig->chAddr, 0, MAX_LOCAL_ADDR);
		g_pLocalConfig->uiPort = 0;
		memset(&g_pLocalConfig->uCP, 0, sizeof(UCPData));

		g_pDBOEncryptConfig = new char [MAX_DBO_ENCRYPT_CONFIG];
		memset(g_pDBOEncryptConfig, 0, MAX_DBO_ENCRYPT_CONFIG);
	}
}

void Local_Terminate(void)
{
	if(g_pLocalConfig)
	{
		delete g_pLocalConfig;
		g_pLocalConfig = NULL;
	}

	if(g_pDBOEncryptConfig)
	{
		delete [] g_pDBOEncryptConfig;
		g_pDBOEncryptConfig = NULL;
	}
}


unsigned char Local_GetType(void)	
{
	return g_byLocalType;
}


SLocalConfig* Local_GetConfig(void)
{
	return g_pLocalConfig;
}

bool Local_DetectData(const char *pKey)
{
	bool bSuccess = true;// DetectLocalConfigCJ(g_pLocalConfig, pKey);
	if(bSuccess)
		g_byLocalType = NTL_LOCALIZE_GLOBAL;

	return bSuccess;
}

const char*	Local_DBOEncryptConfig(void)
{
	if(g_pLocalConfig == NULL)
		return NULL;

	if(g_pLocalConfig->byLocalType == NTL_LOCALIZE_DEV)
		return NULL;

	memset(g_pDBOEncryptConfig, 0, MAX_DBO_ENCRYPT_CONFIG);

	int iLen;	
	int iCurrSize = 0;
	char chBuffer[1024];
	
	// local type
	_itoa_s(g_pLocalConfig->byLocalType, chBuffer, 1024, 10);
	iLen = sizeof(chBuffer);
	memcpy(&g_pDBOEncryptConfig[iCurrSize], chBuffer, iLen);
	iCurrSize += iLen;
	g_pDBOEncryptConfig[iCurrSize] = ',';
	iCurrSize += sizeof(char);

	// server address
	memcpy(&g_pDBOEncryptConfig[iCurrSize], g_pLocalConfig->chAddr, MAX_LOCAL_ADDR);
	iCurrSize += MAX_LOCAL_ADDR;
	g_pDBOEncryptConfig[iCurrSize] = ',';
	iCurrSize += sizeof(char);

	// server port
	_itoa_s(g_pLocalConfig->uiPort, chBuffer, 1024, 10);
	memcpy(&g_pDBOEncryptConfig[iCurrSize], chBuffer, iLen);
	iCurrSize += iLen;
	g_pDBOEncryptConfig[iCurrSize] = ',';
	iCurrSize += sizeof(char);

	// local user data
	int nCPDataSize = strlen(g_pLocalConfig->uCP.chData);
	memcpy(&g_pDBOEncryptConfig[iCurrSize], &nCPDataSize, sizeof(int));
	iCurrSize += sizeof(int);
	
	g_pDBOEncryptConfig[iCurrSize] = ',';
	iCurrSize += sizeof(char);

	memcpy(&g_pDBOEncryptConfig[iCurrSize], g_pLocalConfig->uCP.chData, MAX_CP_DATA);
	iCurrSize += MAX_CP_DATA;

	return g_pDBOEncryptConfig;
}

bool Local_DBODecryptConfig(const char *pConfig)
{
	return true;
}