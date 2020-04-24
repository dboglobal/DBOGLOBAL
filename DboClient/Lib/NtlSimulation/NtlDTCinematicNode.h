/*****************************************************************************
 *
 * File			: NtlDTCinematicNode.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 06. 27	
 * Abstract		: direct cinematic node
 *****************************************************************************
 * Desc         : cinematic 연출을 위한 node
 *
 *****************************************************************************/

#ifndef __NTL_DTCINEMATICNODE_H__
#define __NTL_DTCINEMATICNODE_H__

#include "NtlSLDef.h"

class CNtlDTCinematicNode
{
private:
	
	RwBool			m_bFinish;

protected:

	void			Finish(void);

public:

	CNtlDTCinematicNode();
	virtual ~CNtlDTCinematicNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	virtual void	InteractiveDirectFinish(void);

	virtual void	Break(void);

	RwBool			IsFinish(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicViewNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicViewNode, NTL_SMALL_MEMORY_POOL)

private:

	RwBool	m_bViewClose;
	RwBool	m_bShow;
	RwReal	m_fDelayTime;
	RwReal	m_fCurrTime;

public:

	CNtlDTCinematicViewNode();
	virtual ~CNtlDTCinematicViewNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(RwBool bShow, RwReal fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicBalloon : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicBalloon, NTL_SMALL_MEMORY_POOL)

private:

	enum
	{
		CTCI_BALLON_PLAY_BEGIN,
		CTCI_BALLON_PLAY_PROGRESS,
		CTCI_BALLON_PLAY_END,
		CTCI_BALLON_PLAY_SINGLE,
	};

	enum
	{
		DTCI_BALLON_DEALY,
		DTCI_BALLON_IDLE,
		DTCI_BALLON_DESTROY
	};

	RwUInt8			m_byState;
	RwReal			m_fDelayTime;
	RwReal			m_fCurrTime;
	RwReal			m_fLifeTime;
	SERIAL_HANDLE	m_hSerialId;
	RwUInt32		m_uiQuestTextTblId;
	RwUInt8			m_byBalloonType;
	RwUInt8			m_byEmotionType;
	RwUInt8			m_byPlayType;

private:

	RwUInt8			GetBallonPlayType(void);

public:

	CNtlDTCinematicBalloon();
	~CNtlDTCinematicBalloon();

	virtual void	Enter(void);
	
	virtual void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	void			SetDataBegin(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime);
	void			SetDataProgress(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime);
	void			SetDataEnd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	void			SetDataSingle(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicFlash : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicFlash, NTL_SMALL_MEMORY_POOL)

private:

	enum
	{
		DTCI_FLASH_DEALY,
		DTCI_FLASH_IDLE,
		DTCI_FLASH_DESTROY
	};

	RwUInt8			m_byState;
	std::string		m_strFileName;
	RwReal			m_fDelayTime;
	RwReal			m_fCurrTime;
	RwReal			m_fLifeTime;
	RwBool			m_bLoop;

private:

	void			PlayFlash(void);

public:

	CNtlDTCinematicFlash();
	~CNtlDTCinematicFlash();

	virtual void	Enter(void);
	
	virtual void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(const RwChar *pFileName, RwBool bLoop, RwReal fLifeTime, RwReal fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicPauseNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicPauseNode, NTL_SMALL_MEMORY_POOL)

private:

	RwReal			m_fLifeTime;
	RwReal			m_fCurrLifeTime;

public:

	CNtlDTCinematicPauseNode();
	~CNtlDTCinematicPauseNode();

	virtual void	Update(RwReal fElapsed);

	void			SetData(RwReal fLifeTime);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicSobNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicSobNode, NTL_DEFAULT_MEMORY_POOL)

private:

	enum 
	{
		DTCI_SOB_CRTATE,
		DTCI_SOB_DELETE,
		DTCI_SOB_MOVE,
		DTCI_SOB_SPLINE_MOVE,
		DTCI_SOB_TRIGGER_OBJECT_STATE,
		DTCI_SOB_DIRECT_PLAY,
		DTCI_SOB_ANIM_PLAY,
		DTCI_SOB_ANIM_PLAY_POSITION_SETTING,
		DTCI_SOB_POSITION_SETTING,
		DTCI_SOB_DIRECT_SETTING,
		DTCI_SOB_SCALE
	};

	enum
	{
		DTCI_SOB_CREATE_MOB,
		DTCI_SOB_CREATE_NPC
	};

	struct SCinematicSobStuff
	{
		RwUInt8			bySobCmd;
		RwUInt8			bySubCmd;
		SERIAL_HANDLE	hSerialId;
		RwUInt32		uiTblId;
		RwUInt32		uiAnimId;
		RwUInt8			byTriggerObjMainState;
		RwUInt8			byTriggerObjSubState;
		RwReal			fAngleY;
		RwV3d			vLoc;
		RwV3d			vDir;
		RwV3d			vMoveDest;
		RwBool			bSplineGroundMove;
		RwBool			bRunMove;
		std::string		strSpline;
		RwBool			bTimeLoop;
		RwReal			fLoopTime;
		RwBool			bShow;

		float			fScaling;
		float			fScaleTime;
	};

	RwBool				m_bDelayCheckStep;
	RwReal				m_fDelayTime;
	RwReal				m_fCurrTime;
	SCinematicSobStuff	m_sStuff;


private:

	void			CreateMob(void);
	void			CreateNpc(void);

	void			PositionSetting(void);
	void			DirectSetting(void);

	void			UpdateSobCreate(RwReal fElapsed);
	void			UpdateSobDelete(RwReal fElapsed);
	void			UpdateSobMove(RwReal fElapsed);
	void			UpdateSobSplineMove(RwReal fElapsed);
	void			UpdateSobTriggetObjectState(RwReal fElapsed);
	void			UpdateSobDirectPlay(RwReal fElapsed);
	void			UpdateSobAnimPlay(RwReal fElapsed);
	void			UpdateSobPositionSetting(RwReal fElapsed);
	void			UpdateSobDirectSetting(RwReal fElapsed);

public:

	CNtlDTCinematicSobNode();
	~CNtlDTCinematicSobNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetDataMobCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime); 
	void			SetDataNpcCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime); 
	void			SetDataSobDelete(SERIAL_HANDLE hSerialId, RwReal fDelayTime);
	void			SetDataSobMove(SERIAL_HANDLE hSerialId, RwV3d vDest, RwReal fDelayTime);
	void			SetDataSobSplineMove(SERIAL_HANDLE hSerialId, RwV3d vPos, RwBool bGroundMove, RwBool bRunMove, const RwChar *pKey, RwReal fDelayTime);
	void			SetDataSobTriggerObjectState(RwUInt32 uiTblId, RwUInt8 byMainState, RwUInt8 bySubState, RwReal fDelayTime);
	void			SetDataSobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime);
	void			SetDataSobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime);
	void			SetDataSobAnimPlayPositionSetting(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwV3d vPos, RwReal fDelayTime);
	void			SetDataSobPositionSetting(SERIAL_HANDLE hSerialId, RwV3d vPos, RwReal fDelayTime);
	void			SetDataSobDirectSetting(SERIAL_HANDLE hSerialId, RwV3d vDir, RwReal fDelayTime);
	void			SetDataSobScale(SERIAL_HANDLE hSerialId, float fScale, float fScaleTime, float fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicPLEffectNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicPLEffectNode, NTL_SMALL_MEMORY_POOL)

public:

	enum
	{
		CN_PLEFFECT_TYPE_ACTOR_BONE,
		CN_PLEFFECT_TYPE_ACTOR_OFFSET,
		CN_PLEFFECT_TYPE_WORLD
	};

private:

	RwReal			m_fDelayTime;
	RwReal			m_fCurrTime;

	SERIAL_HANDLE	m_hInstance;
	RwUInt8			m_byEffectType;
	SERIAL_HANDLE	m_hSerialId;
	RwV3d			m_vWorld;
	RwV3d			m_vOffset;
	std::string		m_strBond;
	std::string		m_strKey;

private:

	void			CreatePLEffect(void);
	
public:

	CNtlDTCinematicPLEffectNode();
	virtual ~CNtlDTCinematicPLEffectNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(SERIAL_HANDLE hInstance, const RwChar *pKey, SERIAL_HANDLE hSerialId, const RwChar *pBoneName, RwReal fDelayTime);
	void			SetData(SERIAL_HANDLE hInstance, const RwChar *pKey, SERIAL_HANDLE hSerialId, RwV3d vOffset, RwReal fDelayTime);
	void			SetData(SERIAL_HANDLE hInstance, const RwChar *pKey, RwV3d vWorld, RwReal fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicCameraNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicCameraNode, NTL_SMALL_MEMORY_POOL)

	enum 
	{
		CN_CAMERA_SOFT_ACTOR_DIST,
		CN_CAMERA_SOFT_ACTOR_DYNAMIC_LOOKAT_DIST,
		CN_CAMERA_SOFT_CURRENT_ROTATE_X,
		CN_CAMERA_SOFT_CURRENT_ROTATE_Y,
		CN_CAMERA_SOFT_CURRENT_ROTATE_Z,
		CN_CAMERA_SOFT_CURRENT_FOV,
		CN_CAMERA_SOFT_PAUSE,
		CN_CAMERA_SPLINE_CURRENT_DYNAMIC_LOOKAT,
		CN_CAMERA_SPLINE_SELF,
		CN_CAMERA_SPLINE_SELF_ACTOR,
		CN_CAMERA_LOCK_UNLOCK
	};

	struct SCNCameraSoftActorDist
	{
		SERIAL_HANDLE	hSerialId;
		RwReal			fHeightOffset;
		RwReal			fAngleX;
		RwReal			fAngleY;
		RwReal			fDistance;
		RwReal			fLifeTime;
		RwV3d			vLookAt;
	};

	struct SCNCameraSoftActorDynamicLookAtDist
	{
		SERIAL_HANDLE	hSerialId;
		RwReal			fHeightOffset;
		RwReal			fAngleX;
		RwReal			fAngleY;
		RwReal			fDistance;
		RwReal			fLifeTime;
	};

	struct SCNCameraSoftCurrentRotate
	{
		RwReal			fLastAngle;
		RwReal			fInterTime;
		RwReal			fLifeTime;
	};

	struct SCNCameraSoftCurrentFov
	{
		RwReal			fLastFov;
		RwReal			fInterTime;
	};

	struct SCNCameraSoftPause
	{
		RwReal			fLifeTime;
		RwBool			bLoop;
	};

	struct SCNCameraSplineCurrentDynamicLookAt
	{
		SERIAL_HANDLE	hSerialId;
		RwReal			fHeightOffset;
		RwChar			chPropKey[32];
		RwReal			fLifeTime;
	};

	struct SCNCameraSplineSelf
	{
		RwV3d			vPos;
		RwReal			fAngleX;
		RwReal			fAngleY;
		RwReal			fAngleZ;
		RwChar			chPropKey[32];
	};

	struct SCNCameraSplineSelfActor
	{
		SERIAL_HANDLE	hSerialId;
		RwReal			fHeightRate;
		RwReal			fAngleX;
		RwReal			fAngleY;
		RwReal			fAngleZ;
		RwChar			chPropKey[32];
	};

	struct SCNCameraLockUnLock
	{
		RwBool			bLock;
	};

	union UCNCameraStuff
	{
		SCNCameraSoftActorDist					sSoftActorDist;
		SCNCameraSoftActorDynamicLookAtDist		sSoftActorDynaLookAtDist;
		SCNCameraSoftCurrentRotate				sSoftCurrRot;
		SCNCameraSoftCurrentFov					sSoftCurrFov;
		SCNCameraSoftPause						sSoftPause;
		SCNCameraSplineCurrentDynamicLookAt		sSplineCurrDynaLookAt;
		SCNCameraSplineSelf						sSplineSelf;
		SCNCameraSplineSelfActor				sSplineSelfActor;
		SCNCameraLockUnLock						sLockUnLock;
	};

private:

	RwReal			m_fDelayTime;
	RwReal			m_fCurrTime;
	RwBool			m_bCreateCamera;

	RwUInt8			m_byDTType;
	UCNCameraStuff	m_uCameraStuff;

	RwBool			m_bAsync;

	void			CreateCameraDTNode(void);

public:

	CNtlDTCinematicCameraNode();
	virtual ~CNtlDTCinematicCameraNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetDataSoftActorDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
										 RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSoftActorDynamicLookAtDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
										    			RwReal fDistance, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSoftPause(RwReal fLifeTime, RwBool bLoop, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSplineCurrentDynamicLookAt(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSPlineSelf(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime, RwBool bAsync);
	void			SetDataSPlineSelfActor(SERIAL_HANDLE hSerialId, RwReal fHeightRate, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime, RwBool bAsync);
	void			SetDataLockUnLock(RwBool bLock, RwReal fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicCameraShakeNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicCameraShakeNode, NTL_SMALL_MEMORY_POOL)

private:

	enum 
	{
		CN_CAMERA_SHAKE,
		CN_CAMERA_SHAKE_FACTOR,
		CN_CAMERA_SHAKE_FACTOR_RESET
	};

	RwUInt8			m_byShakeType;
	RwReal			m_fCurrTime;
	RwReal			m_fDelayTime;
	RwReal			m_fWeightValue;
	RwReal			m_fHeightValue;

public:

	CNtlDTCinematicCameraShakeNode();
	virtual ~CNtlDTCinematicCameraShakeNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(RwReal fWeightValue, RwReal fHeightValue, RwReal fDelayTime);
	void			SetDataShakeFactor(RwReal fWeightValue, RwReal fHeightValue);
	void			SetDataShakeFactorReset(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicFadeInOutNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicFadeInOutNode, NTL_SMALL_MEMORY_POOL)

private:

	RwUInt8			m_byFadeType;
	RwReal			m_fDelayTime;
	RwReal			m_fCurrTime;
	RwReal			m_fLifeTime;

	RwBool			m_bFadeIn;
	RwUInt8			m_byRed;
	RwUInt8			m_byGreen;
	RwUInt8			m_byBlue;

	RwBool			m_bSend;

public:

	CNtlDTCinematicFadeInOutNode();
	virtual ~CNtlDTCinematicFadeInOutNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(RwUInt8 byFadeType, RwBool bFadeIn, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicSoundNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicSoundNode, NTL_SMALL_MEMORY_POOL)

private:

	RwReal			m_fCurrTime;
	RwReal			m_fDelayTime;
	RwUInt8			m_byChannelGroup;
	std::string		m_strFileName;
	RwV3d			m_vPos;
	RwBool			m_bLoop;

private:

	void			PlaySound(void);

public:

	CNtlDTCinematicSoundNode();
	virtual ~CNtlDTCinematicSoundNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(RwUInt8 byChannelGroup, const RwChar *pFileName, RwV3d *pV3dPos, RwReal fDelayTime, RwBool bLoop = false);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicBGMStop : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicBGMStop, NTL_SMALL_MEMORY_POOL)

public:

	CNtlDTCinematicBGMStop();
	virtual ~CNtlDTCinematicBGMStop();

	virtual	void	Enter(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicCharacterOptNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicCharacterOptNode, NTL_SMALL_MEMORY_POOL)

public:

	enum
	{
		CINEMATIC_CHAROPT_SHADOW,
		CINEMATIC_CHAROPT_SHOW
	};

private:

	RwUInt8			m_byOpt;
	RwBool			m_bOnOff;
	SERIAL_HANDLE	m_hSerialId;

public:

	CNtlDTCinematicCharacterOptNode();
	virtual ~CNtlDTCinematicCharacterOptNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetDataShoadow(RwBool bOnOff);
	void			SetDataShow(SERIAL_HANDLE hSerialId, RwBool bOnOff);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicSceneNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicSceneNode, NTL_SMALL_MEMORY_POOL)

public:

	enum
	{
		CINEMATIC_MOONOPT_SHOW,
		CINEMATIC_RAIN_BLOOM_LIGHT,
		CINEMATIC_RAIN_SKY_LIGHT
	};

private:

	RwUInt8			m_byOpt;
	RwBool			m_bOnOff;
	RwReal			m_fFadeInTime;
	RwReal			m_fFadeOutTime;

public:

	CNtlDTCinematicSceneNode();
	virtual ~CNtlDTCinematicSceneNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetDataMoon(RwBool bOnOff);
	void			SetRainBloomLight(RwReal fFadeInTime, RwReal fFadeOutTime);
	void			SetRainSkyLight(RwReal fFadeInTime, RwReal fFadeOutTime);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicGuiCmdNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicGuiCmdNode, NTL_SMALL_MEMORY_POOL)

private:

	std::string m_strCmdKey;
	RwBool		m_bShow;
	RwReal		m_fCurrTime;
	RwReal		m_fDelayTime;

public:

	CNtlDTCinematicGuiCmdNode();
	virtual ~CNtlDTCinematicGuiCmdNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(const RwChar *pCmdKey, RwBool bShow, RwReal fDelayTime);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicServerAckNode : public CNtlDTCinematicNode
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicServerAckNode, NTL_SMALL_MEMORY_POOL)

public:

	CNtlDTCinematicServerAckNode();
	virtual ~CNtlDTCinematicServerAckNode();

	virtual	void	Enter(void);

	virtual	void	Exit(void);

	virtual void	Update(RwReal fElapsed);

	void			SetData(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlDTCinematicNodeChain
{
	DECLEAR_MEMORY_POOL(CNtlDTCinematicNodeChain, NTL_LARGE_MEMORY_POOL)

private:

	typedef std::list<CNtlDTCinematicNodeChain*> ListChild;

	CNtlDTCinematicNode			*m_pNode;
	CNtlDTCinematicNodeChain	*m_pParent;
	ListChild					m_listSibling;
	ListChild					m_listChild;

	RwBool						m_bFinish;

public:

	CNtlDTCinematicNodeChain();
	~CNtlDTCinematicNodeChain();

	void						Enter(void);

	void						Exit(void);

	void						Update(RwReal fElapsed);

	void						SetNode(const CNtlDTCinematicNode *pNode);

	void						SetParentChain(const CNtlDTCinematicNodeChain *pChain);

	void						AddSiblingChain(const CNtlDTCinematicNodeChain *pChain);

	void						AddChildChain(const CNtlDTCinematicNodeChain *pChain);

	RwBool						IsFinish(void);

	CNtlDTCinematicNode*		GetNode(void) const;

	CNtlDTCinematicNodeChain*	GetParentChain(void) const;
};

#endif
