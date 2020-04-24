//***********************************************************************************
//	File		:	VenusPlayerEventManager.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venusplayereventmanager.h"

#include "VenusFrame.h"
#include "VenusConfig.h"
#include "NtlDebug.h"

#include "NtlEffectDefine.h"

#include "VenusVisualManager.h"

#include "EventLayer.h"

CVenusPlayerEventManager::CVenusPlayerEventManager(void)
{
	m_pSelectPlayerEvent	= NULL;

	m_bCopyPlayerEvent		= TRUE;
	m_pCopyPlayerEvent		= NULL;
}

CVenusPlayerEventManager::~CVenusPlayerEventManager(void)
{
	m_ClumpFileManager.Destroy();
	m_AnimationFileManager.Destroy();
}


//------------------------------------------------------------------
//	FuncName	: GetInstance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CVenusPlayerEventManager& CVenusPlayerEventManager::GetInstance(void)
{
	static CVenusPlayerEventManager manager;
	return manager;
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayerEventManager::Delete()
{
	m_EventEmemy.Delete();

	if (!m_slDamageEntity.empty())
	{
		sldef_PLEntity::iterator it;
		for (it = m_slDamageEntity.begin(); it != m_slDamageEntity.end(); ++it)
		{
			CVenusVisualManager::GetInstance().DeleteEventEffect(*it);
		}
		m_slDamageEntity.clear();
	}
	if (!m_slEventBall.empty())
	{
		sldef_EventBall::iterator it;
		for (it = m_slEventBall.begin(); it != m_slEventBall.end(); ++ it)
		{
			NTL_DELETE(*it);
		}
		m_slEventBall.clear();
	}

	if (m_pSelectPlayerEvent != NULL)
	{
		m_pSelectPlayerEvent->Delete();
		m_pSelectPlayerEvent = NULL;
	}

	if (!m_svVenusPlayerEvent.empty())
	{
		svdef_VenusPlayerEvent::iterator it;
		for (it = m_svVenusPlayerEvent.begin(); it != m_svVenusPlayerEvent.end(); ++ it)
		{
			NTL_DELETE(*it);
		}
		m_svVenusPlayerEvent.clear();
	}
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayerEventManager::Create()
{
	m_strClumpTexturePath	= "TOOL\\VenusData\\Player\\Textures\\";
	m_ClumpFileManager.AddExt("dff");
	m_ClumpFileManager.LoadPath("TOOL\\VenusData\\Player\\Mesh\\");
	m_AnimationFileManager.AddExt("anm");
	m_AnimationFileManager.LoadPath("TOOL\\VenusData\\Player\\Animation\\");
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEventManager::Update(RwReal fElapsedTime)
{

	if (m_pSelectPlayerEvent != NULL)
	{
		m_pSelectPlayerEvent->Update(fElapsedTime);
		if (m_pSelectPlayerEvent->m_Avatar.GetAnimationResource())
		{
			CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
			pView->ResetAnimationTime(m_pSelectPlayerEvent->m_Avatar.GetAnimCurrentTime(), m_pSelectPlayerEvent->m_Avatar.GetMaxTime());
		}
	}

	sldef_EventBall::iterator it = m_slEventBall.begin();
	while (it != m_slEventBall.end())
	{
		CVenusEventBall* pEventBall = (*it);
		if (!pEventBall->Update(fElapsedTime))
		{
			if (!m_EventEmemy.m_strEffectName.empty())
			{
				CNtlPLEntity* pEntity = CVenusVisualManager::GetInstance().CreateEventEffect(m_EventEmemy.m_strEffectName.c_str());
				if (pEntity != NULL)
				{
					RwMatrix matWorld = (*it)->GetMatrix();
					pEntity->SetMatrix(matWorld);

					m_slDamageEntity.push_back(pEntity);
				}
			}
			m_EventEmemy.SetDamage();


			NTL_DELETE(*it);
			it = m_slEventBall.erase(it);
		}
		else
		{
			++ it;
		}
	}

	if (!m_slDamageEntity.empty())
	{
		sldef_PLEntity::iterator it = m_slDamageEntity.begin();
		while (it != m_slDamageEntity.end())
		{
			if (!(*it)->Update(fElapsedTime))
			{
				CVenusVisualManager::GetInstance().DeleteEventEffect(*it);
				it = m_slDamageEntity.erase(it);
			}
			else
			{
				++ it;
			}
		}
	}

	RwV3d vPosition = { 0, 0, CVenusConfig::GetInstance().m_fEventBallSpeed * CVenusConfig::GetInstance().m_fEventBallLifeTime };
	m_EventEmemy.SetPosition(vPosition);
	m_EventEmemy.Update(fElapsedTime);

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEventManager::Render()
{
	if (m_pSelectPlayerEvent != NULL)
	{
		m_pSelectPlayerEvent->Render();
	}


	sldef_EventBall::iterator it;
	for (it = m_slEventBall.begin(); it != m_slEventBall.end(); ++ it)
	{
		CVenusEventBall* pEventBall = (*it);
		pEventBall->Render();
	}

	m_EventEmemy.Render();

	if (!m_slDamageEntity.empty())
	{
		sldef_PLEntity::iterator it;
		for (it = m_slDamageEntity.begin() ; it != m_slDamageEntity.end(); ++ it)
		{
			(*it)->Render();
		}
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: GetPlayerEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CVenusPlayerEvent* CVenusPlayerEventManager::GetPlayerEvent(const RwChar* strName)
{
	svdef_VenusPlayerEvent::iterator it;
	for (it = m_svVenusPlayerEvent.begin(); it != m_svVenusPlayerEvent.end(); ++ it)
	{
		if (_stricmp(strName, (*it)->GetName()) == 0)
		{
			return (*it);
		}
	}
	return NULL;
}

//------------------------------------------------------------------
//	FuncName	: CreatePlayerEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CVenusPlayerEvent* CVenusPlayerEventManager::CreatePlayerEvent(const RwChar* strName, CVenusPlayerEvent* pSrcPlayerEvent)
{
	CVenusPlayerEvent* pVenusPlayerEvent = NTL_NEW CVenusPlayerEvent(strName);
	NTL_ASSERTE(pVenusPlayerEvent);

	if (pSrcPlayerEvent != NULL)
	{
		pVenusPlayerEvent->CopyData(pSrcPlayerEvent);
	}


	m_svVenusPlayerEvent.push_back(pVenusPlayerEvent);

	return pVenusPlayerEvent;
}

//------------------------------------------------------------------
//	FuncName	: DeletePlayerEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEventManager::DeletePlayerEvent(const RwChar* strName)
{
	if (m_pSelectPlayerEvent != NULL)
	{
		if (_stricmp(strName, m_pSelectPlayerEvent->GetName()) == 0)
		{
			m_pSelectPlayerEvent = NULL;
		}
	}
	if (m_pCopyPlayerEvent != NULL)
	{
		if (_stricmp(m_pCopyPlayerEvent->GetName(), strName) == 0)
		{
			SetCopyPlayerEvent(TRUE);
		}
	}

	svdef_VenusPlayerEvent::iterator it;
	for (it = m_svVenusPlayerEvent.begin(); it != m_svVenusPlayerEvent.end(); ++ it)
	{
		if (_stricmp(strName, (*it)->GetName()) == 0)
		{
			NTL_DELETE(*it);
			m_svVenusPlayerEvent.erase(it);
			return TRUE;
		}
	}
	return FALSE;
}

//------------------------------------------------------------------
//	FuncName	: SetSelectPlayerEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayerEventManager::SetSelectPlayerEvent(CVenusPlayerEvent* pPlayerEvent)
{
	if (m_pSelectPlayerEvent != NULL)
	{
		m_pSelectPlayerEvent->Delete();
	}
	m_pSelectPlayerEvent = pPlayerEvent;
	if (m_pSelectPlayerEvent != NULL)
	{
		m_pSelectPlayerEvent->Create();
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEffectEntity
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayerEventManager::DeleteEffectEntity()
{
	if (m_pSelectPlayerEvent != NULL)
	{
		m_pSelectPlayerEvent->DeleteEffectEntity();
	}

	if (!m_slEventBall.empty())
	{
		sldef_EventBall::iterator it;
		for (it = m_slEventBall.begin(); it != m_slEventBall.end(); ++ it)
		{
			NTL_DELETE(*it);
		}
		m_slEventBall.clear();
	}

	if (!m_slDamageEntity.empty())
	{
		sldef_PLEntity::iterator it;
		for (it = m_slDamageEntity.begin(); it != m_slDamageEntity.end(); ++it)
		{
			CVenusVisualManager::GetInstance().DeleteEventEffect(*it);
		}
		m_slDamageEntity.clear();
	}
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEventManager::Save(const RwChar* strPathName)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	FILE* pFile = NULL;
	fopen_s(&pFile, strPathName, "wb");
#else
	FILE* pFile = fopen(strPathName, "wb");
#endif
	if (pFile == NULL)
	{
		return FALSE;
	}
	RwChar* strFileHeader = FILE_HEADER;
	fwrite(strFileHeader,				strlen(FILE_HEADER),					1,			pFile);

	RwUInt32 nVersion = FILE_VERSION;
	fwrite(&nVersion,					sizeof(RwUInt32),							1,			pFile);

	// ============================================================================
	// Count
	// ============================================================================
	RwInt32 nEventCount = (RwInt32)m_svVenusPlayerEvent.size();
	fwrite(&nEventCount,				sizeof(RwInt32),							1,			pFile);
	if (nEventCount > 0)
	{
		svdef_VenusPlayerEvent::iterator it;
		for (it = m_svVenusPlayerEvent.begin(); it != m_svVenusPlayerEvent.end(); ++ it)
		{
			CVenusPlayerEvent* pPlayerEvent = (*it);

			RwInt32 length = (RwInt32)strlen(pPlayerEvent->GetName()) + 1;
			fwrite(&length,					sizeof(RwInt32),							1,			pFile);

			fwrite(pPlayerEvent->GetName(),	sizeof(RwChar),							length,		pFile);

			pPlayerEvent->Save(pFile);
		}
	}

	m_EventEmemy.Save(pFile);

	fclose(pFile);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEventManager::Load(const RwChar* strPathName)
{
	FILE* pFile = fopen(strPathName, "rb");
	if (pFile == NULL)
	{
		return FALSE;
	}

	RwChar strFileHeader[MAX_PATH];
	memset(strFileHeader, 0, MAX_PATH);
	fread(strFileHeader,				strlen(FILE_HEADER),					1,			pFile);
	if (_stricmp(strFileHeader, FILE_HEADER) != 0)
	{
		fclose(pFile);
		return FALSE;
	}

	RwInt32 nVersion;
	fread(&nVersion,					sizeof(RwUInt32),							1,			pFile);
	if (nVersion < FILE_VERSION)
	{
		fclose(pFile);
		return FALSE;
	}

	// ============================================================================
	// Player Event 
	// ============================================================================
	RwInt32 nEventCount;
	fread(&nEventCount,					sizeof(RwInt32),							1,			pFile);
	if (nEventCount > 0)
	{
		RwChar strName[MAX_PATH];
		for (RwInt32 i = 0; i < nEventCount; ++ i)
		{
			RwInt32 length;
			fread(&length,					sizeof(RwInt32),							1,			pFile);

			memset(strName, 0, MAX_PATH);
			fread(strName,					sizeof(RwChar),								length,		pFile);
			CVenusPlayerEvent* pPlayerEvent = CreatePlayerEvent(strName);

			pPlayerEvent->Load(pFile);
		}
	}

	m_EventEmemy.Load(pFile);

	m_EventEmemy.CreateResource();

	fclose(pFile);
	return TRUE;
}
