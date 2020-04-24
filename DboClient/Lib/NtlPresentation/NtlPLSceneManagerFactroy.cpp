#include "precomp_ntlpresentation.h"
#include "NtlPLSceneManagerFactory.h"
#include "NtlPLSceneManager.h"
#include "NtlDebug.h"


CNtlPLSceneManager* CNtlPLSceneManagerFactory::m_pSceneManager = NULL;

CNtlPLSceneManager* CNtlPLSceneManagerFactory::GetInstance()
{
	return m_pSceneManager;
}

void CNtlPLSceneManagerFactory::ActiveSceneManager(CNtlPLSceneManager *pSceneManager)
{
	NTL_PRE(pSceneManager);
	
	m_pSceneManager = pSceneManager;
}