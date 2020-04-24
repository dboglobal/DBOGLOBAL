/*****************************************************************************
 *
 * File			: CharacterStage.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 11. 28	
 * Abstract		: Character stage class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __CHARACTER_STAGE_H__
#define __CHARACTER_STAGE_H__

#include "NtlStage.h"
#include "ceventhandler.h"
#include "eventtimer.h"

#include "NtlPLDummyWorld.h"
#include "NtlPLObject.h"
#include "NtlPLSky.h"
#include "NtlPLCharacter.h"
#include "CharGuiGroup.h"
#include "NtlSobAvatar.h"

#include "DboEvent.h"


class CCharStageState;
class CCharacterStage : public CNtlStage, public RWS::CEventHandler
{
private:

	CCharStageState			*m_pState;								// character stage state

	CNtlPLDummyWorld		*m_pWorldEntity;						// Dummy World
	CNtlPLObject			*m_pObjectEntity;						// Base Object
	CNtlPLSky				*m_pSkyEntity;							// Sky
	
	CNtlSobAvatar			*m_pAvatar;								// Avatar
	RwInt32					m_iAvatarRotState;						// Avatar Rotation State
	RwReal					m_fRotSpeedPerSec;						// Avatar Rotation Speed

	CCharGuiGroup			*m_pGuiGroup;							// Gui Group

	RpLight*				m_pLtAmbient;							// Ambient Light
	RpLight*				m_pLtDirectional;						// Directional Light 
	RwBool					m_bWorldAddCamera;						// World Add/Remove


private:
	// event handler
	void EventProcUpdateTick(RwReal fElapsed);

	// Change Create
	void CreateDummyAvatar( BYTE byRace, BYTE byClass, BYTE byGender, BYTE byFace, BYTE byHair, BYTE bySkinColor, BYTE byHairColor);
	void CreateDownLoadAvatar( sPC_SUMMARY *pCharInfo );

	void CreateBackGroundStage( RwUInt32 uiRace);
	void DestroyAvatar();
	
public:

	CCharacterStage(const char *pStageName);
	~CCharacterStage();

	void Init(void);
	virtual bool Create(void);
	virtual void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:
	void OnTimer(void);
};

#endif