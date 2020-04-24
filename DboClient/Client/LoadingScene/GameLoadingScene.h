/*****************************************************************************
 *
 * File			: GameLoadingScene.h
 * Author		: Jeong Ho, Rho
 * Copyright	: (주)NTL
 * Date			: 2006. 1. 3
 * Abstract		: Game loading scene
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __GAME_LOADING_SCENE_H__
#define __GAME_LOADING_SCENE_H__

// sound
#include "NtlSoundDefines.h"

// gui
#include "gui_precomp.h"
#include "guicore_precomp.h"

class CSurfaceGui;
class CNtlPLEntityAlphaWeightBlend;
class CLoadingGui;

#define LOADING_TEXT_NUM		10

enum ECharTeleportState
{
	CHAR_TELEPORT_NONE,
	CHAR_TELEPORT_READY,
	CHAR_TELEPORT,
	CHAR_TELEPORT_LOAD_END,
	CHAR_TELEPORT_SPAWN_READY,
	CHAR_TELEPORT_FINISH
};


class CGameTeleportSceneNode
{
protected:

	ECharTeleportState	m_eState;

public:

	CGameTeleportSceneNode() {}
	virtual ~CGameTeleportSceneNode() {}

	virtual void		Update(RwReal fElapsed) { }
	virtual void		Render(void) {}

	virtual void		Finish(void) {}

	virtual RwBool		IsAvatarReadySend(void)		{ return TRUE; }
	virtual RwBool		IsFinish(void)				{ return TRUE; }

	virtual void		SetState(ECharTeleportState eState) { m_eState = eState; }
	ECharTeleportState	GetState(void) { return m_eState; }

	virtual void		OnLoadingEvent(RwUInt8 byMessage) {}
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class CGameNormalTeleportScene : public CGameTeleportSceneNode
{
protected:
	
	RwReal							m_fTime;

	CLoadingGui						*m_pLoadingGui;

	CNtlPLEntityAlphaWeightBlend	*m_pAlphaBlend;
	RwBool							m_bFinish;

public:

	CGameNormalTeleportScene( RwBool bLoadResource = TRUE );
	~CGameNormalTeleportScene();

	void	Update(RwReal fElapsed);
	void	Render(void);

	RwBool	IsAvatarReadySend(void);
	RwBool	IsFinish(void);

	virtual void SetState(ECharTeleportState eState);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class CGameTLQTeleportScene : public CGameNormalTeleportScene
{
public:
	enum ETLQTeleportScene
	{
		ETLQTS_MAIN_RENDER,
		ETLQTS_CINEMATIC_RENDER,
		ETLQTS_CINEMATIC_FINISH
	};

protected:

	ETLQTeleportScene	m_eSubState;
	RwUInt32			m_byCounter;

public:
	CGameTLQTeleportScene( RwBool bLoadResource = TRUE );
	virtual ~CGameTLQTeleportScene();

	virtual void		Update(RwReal fElapsed);
	virtual void		Render(void);

	virtual RwBool		IsAvatarReadySend(void);
	virtual RwBool		IsFinish(void);

	virtual void		SetState(ECharTeleportState eState);

protected:
	void				SetSubState(ETLQTeleportScene eSubState);
	bool				LoadCinematic(void);
	void				UpdateSubState(RwReal fElapsed);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class CGameTMQTeleportScene : public CGameTeleportSceneNode
{
private:

	enum ETMQTeleportScene
	{
		ETMQTS_MAIN_RENDER,
		ETMQTS_AFTER_RENDER,
		ETMQTS_CINEMATIC_ENTER,
		ETMQTS_CINEMATIC_RUN,
		ETMQTS_CINEMATIC_FINISH
	};

	RwUInt8				m_byResState;
	RwUInt8				m_byResCount;
	RwBool				m_bSceneMovieEnd;
	RwUInt8				m_byCinematicNextState;
	RwBool				m_bFinish;

	CLoadingGui			*m_pLoadingGui;

	RwReal				m_fSavedChannelGroupVolume[NUM_CHANNEL_GROUP];

	gui::CSlot			m_slotAfterMovieEnd;

private:

	bool	LoadCinematic(void);

public:

	CGameTMQTeleportScene();
	~CGameTMQTeleportScene();

	void	Update(RwReal fElapsed);
	void	Render(void);

	RwBool	IsAvatarReadySend(void);
	RwBool	IsFinish(void);

	void	SetState(ECharTeleportState eState);

	void	OnLoadingEvent(RwUInt8 byMessage);
};



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class CGameCCBDTeleportScene : public CGameNormalTeleportScene
{
private:

	enum ECCBDTeleportScene
	{
		ECCBDTS_MAIN_RENDER,
		ECCBDTS_CINEMATIC_RENDER,
		ECCBDTS_CINEMATIC_FINISH
	};

protected:

	ECCBDTeleportScene	m_eSubState;
	RwUInt32			m_byCounter;

public:

	CGameCCBDTeleportScene(RwBool bLoadResource = TRUE);
	virtual ~CGameCCBDTeleportScene();

	virtual void		Update(RwReal fElapsed);
	virtual void		Render();

	virtual RwBool		IsAvatarReadySend();
	virtual RwBool		IsFinish();

	virtual void		SetState(ECharTeleportState eState);

protected:

	void				SetSubState(ECCBDTeleportScene eSubState);
	bool				LoadCinematic();
	void				UpdateSubState(RwReal fElapsed);
};



//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	게임중 텔레포트시 생성
//
//////////////////////////////////////////////////////////////////////////////////////////////////

class CGameTeleportScene
{
private:
	
	RwUInt8					m_byWorldRuleType;
	CGameTeleportSceneNode	*m_pSceneNode;	

protected:

	CGameTeleportScene();

public:

	CGameTeleportScene(RwBool bWorldChange);
	~CGameTeleportScene();

	void				Update(RwReal fElapsed);
	void				Render(void);

	RwBool				IsAvatarReadySend(void);
	RwBool				IsFinish(void);

	void				SetState(ECharTeleportState eState);
	ECharTeleportState	GetState(void);

	void				OnLoadingEvent(RwUInt8 byMessage);
};





/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	처음 게임 로딩시 생성
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class CGameLoadingScene
{
public:

	enum EGameLoadingState
	{
		EGAMELOADSTATE_NONE,

		EGAMELOADSTATE_FIRST_STEP_PREPARE,
		EGAMELOADSTATE_FIRST_STEP,

		EGAMELOADSTATE_SECOND_STEP_PREPARE,
		EGAMELOADSTATE_SECOND_STEP,

		EGAMELOADSTATE_THIRD_STEP_PREPARE,
		EGAMELOADSTATE_THIRD_STEP,

		EGAMELOADSTATE_FINISH,
	};


protected:
	EGameLoadingState	m_eInnerOldState;
	EGameLoadingState	m_eInnerState;

	RwBool				m_bFirstEnter;

	CLoadingGui			*m_pLoadingGui;

	gui::CSlot			m_slotSecondMovieEnd;
	gui::CSlot			m_slotThirdMovieEnd;

public:
	CGameLoadingScene( void );
	~CGameLoadingScene( void );

public:
	void				SetFirstEnterMode( RwBool bFirstEnter );

	RwBool				Update( RwReal fElapsed );
	void				Render( void );

	RwBool				IsFinish( void );

	void				GameLoadingFinished( void );

	void				OnLoadingEvent(RwUInt8 byMessage);

protected:
	void				SetInnerState( EGameLoadingState eInnerState );	
};


#endif