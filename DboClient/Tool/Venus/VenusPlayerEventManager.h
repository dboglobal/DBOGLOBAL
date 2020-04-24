//***********************************************************************************
//	File		:	VenusPlayerEventManager.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <vector>
#include ".\venusplayerevent.h"

#include "VenusEventBall.h"

#include "NtlResourceFileManager.h"

#include "VenusEventEnemy.h"


#define EVENT_FILE_EXE	"ent"

class CVenusPlayerEventManager
{
public:
#define FILE_HEADER		"event"
#define FILE_VERSION	0x0000000


	typedef std::vector<CVenusPlayerEvent*>		svdef_VenusPlayerEvent;
	svdef_VenusPlayerEvent						m_svVenusPlayerEvent;

	CVenusPlayerEvent*							m_pSelectPlayerEvent;

	std::string									m_strClumpTexturePath;
	CNtlResourceFileManager						m_ClumpFileManager;
	CNtlResourceFileManager						m_AnimationFileManager;

	typedef std::list<CVenusEventBall*>	sldef_EventBall;
	sldef_EventBall								m_slEventBall;

	RwBool										m_bCopyPlayerEvent;
	CVenusPlayerEvent*							m_pCopyPlayerEvent;

	CVenusEventEnemy							m_EventEmemy;

	typedef std::list<CNtlPLEntity*> sldef_PLEntity;
	sldef_PLEntity								m_slDamageEntity;

public:
	CVenusPlayerEventManager(void);
	~CVenusPlayerEventManager(void);

	static	CVenusPlayerEventManager&	GetInstance(void);

	void	Delete();
	void	Create();
	RwBool	Update(RwReal fElapsedTime);
	RwBool	Render();

	CVenusPlayerEvent*	GetPlayerEvent(const RwChar* strName);
	CVenusPlayerEvent*	CreatePlayerEvent(const RwChar* strName, CVenusPlayerEvent* pSrcPlayerEvent = NULL);
	RwBool				DeletePlayerEvent(const RwChar* strName);

	void				SetSelectPlayerEvent(CVenusPlayerEvent* pPlayerEvent);
	void				DeleteEffectEntity();

	RwBool	Save(const RwChar* strPathName);
	RwBool	Load(const RwChar* strPathName);

public:

	void	SetCopyPlayerEvent(RwBool bCopy, CVenusPlayerEvent* pCopyPlayerEvent = NULL);
};

inline void CVenusPlayerEventManager::SetCopyPlayerEvent(RwBool bCopy, CVenusPlayerEvent* pCopyPlayerEvent)
{
	m_bCopyPlayerEvent	= bCopy;
	m_pCopyPlayerEvent	= pCopyPlayerEvent;
}
