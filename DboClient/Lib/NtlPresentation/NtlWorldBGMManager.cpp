#include "precomp_ntlpresentation.h"

#include "NtlDebug.h"
#include "NtlCoreUtil.h"
#include "NtlStringUtil.h"

#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

#include "NtlWorldCommon.h"

#include "NtlPLPropertyContainer.h"
#include "NtlPLSoundProp.h"

#include "NtlPLEvent.h"
#include "NtlPLWorldState.h"
#include "NtlPLResourcePack.h"
#include "NtlPLResourceManager.h"

using std::ifstream;
#include <fstream>

#include "NtlWorldBGMManager.h"

CNtlWorldBGMManager* g_pNtlWorldBGMManager = NULL;


CNtlWorldBGMManager::CNtlWorldBGMManager(void)
{
	Create();
}

CNtlWorldBGMManager::~CNtlWorldBGMManager(void)
{
	Delete();
}

void CNtlWorldBGMManager::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlWorldBGMManager::HandleEvents");

	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);

		// Same properties, just ignore messages
		if(m_IdxBGMTbl != pNtlFieldProp->_IdxBGMTbl)
		{
			const RwChar* pName = GetBGMSoundName(pNtlFieldProp->_IdxBGMTbl);
			if (pName)
			{					
				string BGMPath = "BGM\\"; BGMPath += pName;

				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.eBGM_Type		= BGM_TYPE_LOCAL_THEME;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_BGM;
				tSoundParam.pcFileName		= (char*)BGMPath.c_str();

				if (SOUNDRESULT_OK == GetSoundManager()->Play( &tSoundParam ) )
				{
					m_IdxBGMTbl = pNtlFieldProp->_IdxBGMTbl;
					m_hBGM = tSoundParam.hHandle;
				}
				else
				{
					m_IdxBGMTbl	= -1;
					m_hBGM		= INVALID_SOUND_HANDLE;
				}
			}
			else
			{
				NTL_TRACE((pNtlFieldProp->_IdxBGMTbl == -1), "FIELD BGM ERROR.(" << pNtlFieldProp->_IdxBGMTbl << ")"); 

				GetSoundManager()->Stop(m_hBGM);

				m_IdxBGMTbl = -1;
				m_hBGM		= INVALID_SOUND_HANDLE;
			}	
		}

		if(m_IdxEnvTbl != pNtlFieldProp->_IdxEnvTbl)
		{
			const RwChar* pName = GetEnvSoundName(pNtlFieldProp->_IdxEnvTbl);
			if (pName)
			{
				string EnvPath = "BGM\\"; EnvPath += pName;

				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_AMBIENT_MUSIC;
				tSoundParam.pcFileName		= (char*)EnvPath.c_str();
				tSoundParam.bLoop			= true;

				if (SOUNDRESULT_OK == GetSoundManager()->Play( &tSoundParam ) )
				{
					m_IdxEnvTbl = pNtlFieldProp->_IdxEnvTbl;
					m_hEnv		= tSoundParam.hHandle;
				}
				else
				{
					m_IdxEnvTbl	= -1;
					m_hEnv		= INVALID_SOUND_HANDLE;
				}
			}
			else
			{
				NTL_TRACE((pNtlFieldProp->_IdxBGMTbl == -1), "FIELD ENV ERROR.(" << pNtlFieldProp->_IdxEnvTbl << ")"); 

				GetSoundManager()->Stop(m_hEnv);

				m_IdxEnvTbl = -1;
				m_hEnv		= INVALID_SOUND_HANDLE;
			}
		}

		CNtlSoundEventGenerator::SendShareBGM_DeleteAllList();
		for (int i = 0; i < 5; ++i)
		{
			const RwChar* pName = GetShareSoundName(pNtlFieldProp->_IdxShareTbl[i]);
			if (pName)
			{
				string SharePath = "BGM\\";
				SharePath += pName;
				CNtlSoundEventGenerator::SendShareBGM_AddList( SharePath.c_str() );
			}
			else
			{
				NTL_TRACE((pNtlFieldProp->_IdxShareTbl[i] == -1), "FIELD SHARE ERROR.(" << pNtlFieldProp->_IdxShareTbl[i] << ")"); 
			}
		}
	}

	NTL_RETURNVOID();
}

void CNtlWorldBGMManager::Create()
{
	NTL_FUNCTION("CNtlWorldBGMManager::Create");

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	m_IdxBGMTbl = -1;
	m_hBGM		= INVALID_SOUND_HANDLE;

	m_IdxEnvTbl = -1;
	m_hEnv		= INVALID_SOUND_HANDLE;

	NTL_RETURNVOID();
}

void CNtlWorldBGMManager::Delete()
{
	NTL_FUNCTION("CNtlWorldBGMManager::Delete");

	if(m_hBGM != INVALID_SOUND_HANDLE)
	{
		GetSoundManager()->Stop(m_hBGM);
	}

	if(m_hEnv != INVALID_SOUND_HANDLE)
	{
		GetSoundManager()->Stop(m_hEnv);
	}

	CNtlSoundEventGenerator::SendShareBGM_DeleteAllList();

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	NTL_RETURNVOID();
}

const RwChar* CNtlWorldBGMManager::GetBGMSoundName(RwInt32 _IdxBGMTbl)
{
	CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_BGM"));
	
	if (_IdxBGMTbl != -1)
	{
		CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.find(_IdxBGMTbl);
		if (it != pSoundFieldProp->m_mapSound.end())
		{
			return it->second.c_str();
		}
	}
	return NULL;
}

const RwChar* CNtlWorldBGMManager::GetEnvSoundName(RwInt32 _IdxEnvTbl)
{
	CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_ENV"));

	if (_IdxEnvTbl != -1)
	{
		CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.find(_IdxEnvTbl);
		if (it != pSoundFieldProp->m_mapSound.end())
		{
			return it->second.c_str();
		}
	}
	return NULL;
}

const RwChar* CNtlWorldBGMManager::GetShareSoundName(RwInt32 _IdxShareTbl)
{
	CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_SHARE"));

	if (_IdxShareTbl != -1)
	{
		CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.find(_IdxShareTbl);
		if (it != pSoundFieldProp->m_mapSound.end())
		{
			return it->second.c_str();
		}
	}
	return NULL;
}


RwInt32 CNtlWorldBGMManager::GetBGMSoundIdx(RwChar* pName)
{
	if (strcmp(pName, "NULL"))
	{
		CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_BGM"));

		for (CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.begin(); it != pSoundFieldProp->m_mapSound.end(); ++it)
		{
			if (it->second.compare(pName))
			{
				return it->first;
			}
		}
	}

	return -1;
}

RwInt32 CNtlWorldBGMManager::GetEnvSoundIdx(RwChar* pName)
{
	if (strcmp(pName, "NULL"))
	{
		CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_ENV"));

		for (CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.begin(); it != pSoundFieldProp->m_mapSound.end(); ++it)
		{
			if (it->second.compare(pName))
			{
				return it->first;
			}
		}
	}

	return -1;
}

RwInt32 CNtlWorldBGMManager::GetShareSoundIdx(RwChar* pName)
{
	if (strcmp(pName, "NULL"))
	{
		CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_SHARE"));

		for (CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.begin(); it != pSoundFieldProp->m_mapSound.end(); ++it)
		{
			if (it->second.compare(pName))
			{
				return it->first;
			}
		}
	}

	return -1;
}