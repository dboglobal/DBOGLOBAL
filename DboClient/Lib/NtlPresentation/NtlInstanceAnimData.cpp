#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlInstanceAnimData.h"
#include "NtlPLResourceManager.h"

void CNtlInstanceAnimTable::Create(CNtlTypeAnimTable *pTypeAnimTable)
{
	m_pTypeAnimTable = pTypeAnimTable;
}

void CNtlInstanceAnimTable::Destroy()
{
	INSTANCE_ANIM_MAP_ITER stAnimMapIT  = m_mapInstanceAnim.begin();
	INSTANCE_ANIM_MAP_ITER endAnimMapIT = m_mapInstanceAnim.end();
	for(;stAnimMapIT != endAnimMapIT;)
	{
		SInstanceAnimData *pInstanceAnimData = stAnimMapIT->second;
		if(pInstanceAnimData != NULL)
		{
			if(pInstanceAnimData->pResource)
			{
				CNtlPLResourceManager::GetInstance()->UnLoad(pInstanceAnimData->pResource);
				pInstanceAnimData->pResource = NULL;
			}

			NTL_DELETE( pInstanceAnimData );
		}
		++stAnimMapIT;
	}
	
	m_mapInstanceAnim.clear();
}

SInstanceAnimData *CNtlInstanceAnimTable::Get(RwUInt32 uiKey)
{
	NTL_ASSERTE(m_pTypeAnimTable);
	if(m_pTypeAnimTable == NULL)
		return NULL;
	
	STypeAnimData *pTypeAnimData = m_pTypeAnimTable->Get(uiKey);
	if(pTypeAnimData == NULL)
		return NULL;
	
	SInstanceAnimData *pInstanceAnimData = NULL;
	if(m_mapInstanceAnim.find( uiKey ) == m_mapInstanceAnim.end())
	{
		SInstanceAnimData *pAnimData = Add( uiKey);
		pAnimData->pTypeAnimData = pTypeAnimData;
		pInstanceAnimData = pAnimData;
	}
	else
	{
		pInstanceAnimData = m_mapInstanceAnim[uiKey];
        pInstanceAnimData->pTypeAnimData = pTypeAnimData;
	}
	
	NTL_ASSERTE(pInstanceAnimData);
	if(pInstanceAnimData == NULL)
		return NULL;
	
	if( pInstanceAnimData->pResource &&
		(pInstanceAnimData->pTypeAnimData->m_bReLoad) )
	{
		return pInstanceAnimData;
	}
	else
	{
		if(pInstanceAnimData->pTypeAnimData == NULL)
			return NULL;
		
		std::string strAnimFileName;
		strAnimFileName += m_pTypeAnimTable->GetAnimPath();
		strAnimFileName += *m_pTypeAnimTable->GetFileName(uiKey);

		if(pInstanceAnimData->pResource)
			CNtlPLResourceManager::GetInstance()->UnLoad(pInstanceAnimData->pResource);

		pInstanceAnimData->pResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(strAnimFileName.c_str());
		if(pInstanceAnimData->pResource)
		{
			pInstanceAnimData->pTypeAnimData->m_bReLoad = TRUE;
			return pInstanceAnimData;
		}

		return NULL;
	}
}

SInstanceAnimData *CNtlInstanceAnimTable::Add(RwUInt32 uiKey)
{
	NTL_ASSERTE(m_mapInstanceAnim.find( uiKey ) == m_mapInstanceAnim.end());
	
	//Since Instance Data is generated dynamically, it is changed to FreeList.
	SInstanceAnimData *pInstanceAnimData = NTL_NEW SInstanceAnimData;
	m_mapInstanceAnim[uiKey] = pInstanceAnimData;
	
	return m_mapInstanceAnim[uiKey];
}




