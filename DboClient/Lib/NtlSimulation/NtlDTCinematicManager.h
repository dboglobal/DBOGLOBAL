/*****************************************************************************
 *
 * File			: NtlDTCinematicManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 06. 27	
 * Abstract		: direct cinematic node manager
 *****************************************************************************
 * Desc         : cinematic 연출을 위한 node의 관리자.
 *
 *****************************************************************************/

#ifndef __NTL_DTCINEMATIC_MANAGER_H__
#define __NTL_DTCINEMATIC_MANAGER_H__

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"

class CNtlPLEntity;
class CNtlDTCinematicNodeChain;

class CNtlDTCinematicManager : public RWS::CEventHandler
{
private:

	struct SLivePLEffect
	{
		SERIAL_HANDLE	hInstance;
		RwBool			bSobAttach;
		SERIAL_HANDLE	hSerialId;
		CNtlPLEntity	*pPLEntity;
	};

	struct SLiveTriggerObject
	{
		RwUInt32	uiTblId;
		RwUInt8		byMainState;
		RwUInt8		bySubState;
	};

	typedef std::list<CNtlDTCinematicNodeChain*> ListNodeChain;
	typedef std::map<CNtlDTCinematicNodeChain*, CNtlDTCinematicNodeChain*> MapNodeChain;
	typedef std::list<SERIAL_HANDLE> ListLiveSob;
	typedef std::list<SLivePLEffect> ListLivePLEntity;
	typedef std::list<SLiveTriggerObject> ListLiveTriggerObject;
	typedef std::list<std::string>	ListLiveGui;

	static CNtlDTCinematicManager	*m_pInstance;

	CNtlDTCinematicNodeChain		*m_pLayerChain;
	CNtlDTCinematicNodeChain		*m_pGroupChain;

	MapNodeChain					m_mapInstanceChain;
	ListNodeChain					m_listUpdateChain;

	ListLiveSob						m_listLiveSob;
	ListLivePLEntity				m_listLivePLEntity;
	ListLiveTriggerObject			m_listLiveTriggerObject;
	ListLiveGui						m_listLiveGui;

	RwReal							m_fRestoreFov;
	RwBool							m_bCameraAsync;
	RwBool							m_bCameraColliCheck;
	RwBool							m_bRestoreSkipSimpleMaterial;
	
	RwBool							m_bRunning;
	RwBool							m_bCinematicPossible;
	RwBool							m_bIsLoadScheduling;
	RwBool							m_bServerAck;

	bool							m_bCanSkip;

private:

	CNtlDTCinematicNodeChain*		CreateChainNode(void);
	CNtlDTCinematicNodeChain*		MakeChainViewShowHide(RwBool bShow, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainBalloon(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainBalloonBegin(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainBalloonProgress(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainBalloonEnd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainBalloonSingle(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainFlash(const RwChar *pFileName, RwBool bLoop, RwReal fLifeTime, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainMobCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainNpcCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobDelete(SERIAL_HANDLE hSerialId, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobMove(SERIAL_HANDLE hSerialId, RwV3d vDest, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobSplineMove(SERIAL_HANDLE hSerialId, RwV3d vPos, RwBool bGoundMove, RwBool bRunMove, const RwChar *pKey, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobTriggerObjectState(RwUInt32 uiTblId, RwUInt8 byMainState, RwUInt8 bySubState, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobAnimPlayPositionSetting(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwV3d vPos, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobPositionSetting(SERIAL_HANDLE hSerialId, RwV3d vPos, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSobDirectSetting(SERIAL_HANDLE hSerialId, RwV3d vDir, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainPause(RwReal fLifeTime);
	CNtlDTCinematicNodeChain*		MakeChainSobScale(SERIAL_HANDLE hSerialId, float fScale, float fScaleTime, float fDelayTime);
	CNtlDTCinematicNodeChain*		MakeVisualEffectCreate(SERIAL_HANDLE hInstance, const RwChar *pKey, SERIAL_HANDLE hSerialId, 
															const RwChar *pBoneName, RwV3d vPos, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainCameraShake(RwReal fWeightValue, RwReal fHeightValue, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainCameraShakeFactor(RwReal fWeightValue, RwReal fHeightValue);
	CNtlDTCinematicNodeChain*		MakeChainCameraShakeFactorReset(void);
	CNtlDTCinematicNodeChain*		MakeChainFadeInOut(RwBool bFadeIn, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainFadeInOutTVH(RwBool bFadeIn, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSound2D(const RwChar *pFileName, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSound3D(const RwChar *pFileName, RwV3d vPos, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainSoundBGM(const RwChar *pFileName, RwBool bLoop);
	CNtlDTCinematicNodeChain*		MakeChainSoundBGMStop();
	CNtlDTCinematicNodeChain*		MakeChainCharacterShadowOnOff(RwBool bOnOff);
	CNtlDTCinematicNodeChain*		MakeChainCharacterShowOnOff(SERIAL_HANDLE hSerialId, RwBool bOnOff);
	CNtlDTCinematicNodeChain*		MakeChainMoonShowOnOff(RwBool bOnOff);
	CNtlDTCinematicNodeChain*		MakeChainRainBloomLight(RwReal fFadeInTime, RwReal fFadeOutTime);
	CNtlDTCinematicNodeChain*		MakeChainRainSkyLight(RwReal fFadeInTime, RwReal fFadeOutTime);
	CNtlDTCinematicNodeChain*		MakeChainGuiCommand(const RwChar *pGuiCmd, RwBool bShow, RwReal fDelayTime);
	CNtlDTCinematicNodeChain*		MakeChainServerAck(void);



	void							InsertChain(CNtlDTCinematicNodeChain *pChain);

	//------------------------------------------------------------------------
	// event handler
	//------------------------------------------------------------------------
	void							CinematicEchoEventHandler(RWS::CMsg &pMsg);

	void							CinematicPlayCheck(RwReal fStandardPosX, RwReal fStandardPosZ);
	
public:
	
	CNtlDTCinematicManager();
	~CNtlDTCinematicManager();

	static CNtlDTCinematicManager* GetInstance(void);

	RwBool					Create(void);

	void					Destroy(void);

	void					Update(RwReal fElapsed);

	void					HandleEvents(RWS::CMsg &pMsg);

	void					Clear(void);

	RwBool					IsRunning(void);

	void					SetFov(RwReal fFov);

	void					RegisterSob(SERIAL_HANDLE hSerialId);
	void					UnRegisterSob(SERIAL_HANDLE hSerialId);

	void					RegisterPLEntity(SERIAL_HANDLE hInstance, RwBool bSobAttach, SERIAL_HANDLE hSerialId, CNtlPLEntity *pPLEntity);

	void					RegisterGuiCmd(const RwChar *pGuiKey);
	void					UnRegisterGuiCmd(const RwChar *pGuiKey);

	void					CommandBegin(RwReal fStandardPosX, RwReal fStandardPosZ);
	void					CommandEnd(void);

	void					CommandBeginLayer(void);
	void					CommandEndLayer(void);

	void					CommnadBeginGroup(void);
	void					CommnadEndGroup(void);

	void					CommandSiblingBegin(void);
	void					CommandSiblingEnd(void);

	void					CommandCameraAsyncBegin(void);
	void					CommandCameraAsyncEnd(void);

	void					CommandCameraLockBegin(void);
	void					CommandCameraLockEnd(void);

	void					CommandViewShowHide(RwBool bShow, RwReal fDelayTime);
	void					CommandBalloon(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandBalloonBegin(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime);
	void					CommandBalloonProgress(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime);
	void					CommandBalloonEnd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandBalloonSingle(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandFlash(const RwChar *pFileName, RwBool bLoop, RwReal fLifeTime, RwReal fDelayTime);

	SERIAL_HANDLE			CommandMobCreate(RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime); 
	SERIAL_HANDLE			CommandNpcCreate(RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime); 
	void					CommandSobDelete(SERIAL_HANDLE hSerialId, RwReal fDelayTime);
	void					CommandSobMove(SERIAL_HANDLE hSerialId, RwV3d vDest, RwReal fDelayTime);
	void					CommandSobSplineMove(SERIAL_HANDLE hSerialId, RwV3d vPos, const RwChar *pKey, RwReal fDelayTime);
	void					CommandSobSplineWalkMove(SERIAL_HANDLE hSerialId, RwV3d vPos, const RwChar *pKey, RwReal fDelayTime);
	void					CommandSobSplineFlyMove(SERIAL_HANDLE hSerialId, RwV3d vPos, const RwChar *pKey, RwReal fDelayTime);
	void					CommandSobTriggerObjectState(RwUInt32 uiTblId, RwUInt8 byMainState, RwUInt8	bySubState, RwReal fDelayTime);
	void					CommandSobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime);
	void					CommandSobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime);
	void					CommandSobAnimPlayPositionSetting(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwV3d vPos, RwReal fDelayTime);
	void					CommandSobPositionSetting(SERIAL_HANDLE hSerialId, RwV3d vPos, RwReal fDelayTime);
	void					CommandSobDirectSetting(SERIAL_HANDLE hSerialId, RwV3d vDir, RwReal fDelayTime);
	void					CommandSobRotateY(SERIAL_HANDLE hSerialId, RwReal fAngleY, RwReal fDelayTime);
	void					CommandSobScale(SERIAL_HANDLE hSerialId, float fScale, float fScaleTime, float fDelayTime);

	void					CommandPause(RwReal fLifeTime);

	SERIAL_HANDLE			CommandVisualEffectCreate(const RwChar *pKey, SERIAL_HANDLE hSerialId, const RwChar *pBoneName, RwReal fDelayTime);
	SERIAL_HANDLE			CommandVisualEffectCreate(const RwChar *pKey, SERIAL_HANDLE hSerialId, RwV3d vOffset, RwReal fDelayTime);
	SERIAL_HANDLE			CommandVisualEffectCreate(const RwChar *pKey, RwV3d vWorld, RwReal fDelayTime);
	void					CommandVisualEffectDelete(SERIAL_HANDLE hInstance);

	void					CommandCameraSoftActorDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
														RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, RwReal fDelayTime);
	void					CommandCameraSoftActorDynamicLookAtDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
																	RwReal fDistance, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandCameraSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandCameraSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandCameraSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandCameraSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, RwReal fDelayTime);
	void					CommandCameraSoftPause(RwReal fLifeTime, RwReal fDelayTime);
	void					CommandCameraSplineSelf(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime);
	void					CommandCameraSplineSelfActor(SERIAL_HANDLE hSerialId, RwReal fHeightRate, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime);
	void					CommandCameraSplineCurrentDynamicLookAt(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwReal fDelayTime);
	void					CommandCameraShake(RwReal fWeightValue, RwReal fHeightValue, RwReal fDelayTime);
	void					CommandCameraShakeFactor(RwReal fWeightValue, RwReal fHeightValue);
	void					CommandCameraShakeFactorReset(void);

	void					CommandFadeIn(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);
	void					CommandFadeOut(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);

	void					CommandFadeInTVH(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);
	void					CommandFadeOutTVH(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime);

	void					CommnadSound2D(const RwChar *pFileName, RwReal fDelayTime);
	void					CommandSound3D(const RwChar *pFileName, RwV3d vPos, RwReal fDelayTime);
	void					CommnadSoundBGM(const RwChar *pFileName, RwBool bLoop);
	void					CommnadSoundBGMStop();

	void					CommandCharacterShadowOnOff(RwBool bOnOff);
	void					CommandCharacterShowOnOff(SERIAL_HANDLE hSerialId, RwBool bOnOff);

	void					CommandMoonOnOff(RwBool bOnOff);
	void					CommandRainBloomLight(RwReal fFadeInTime, RwReal fFadeOutTime);
	void					CommandRainSkyLight(RwReal fFadeInTime, RwReal fFadeOutTime);

	void					CommandGuiCmd(const RwChar *pGuiCmd, RwBool bShow, RwReal fDelayTime);

	void					CommandServerAck(void);

	inline void				SetCanSkip(bool bFlag) { m_bCanSkip = bFlag; }
	inline bool				CanSkip() { return m_bCanSkip; }


	RwBool					IsCinematicPlayPossible(void);

};

static CNtlDTCinematicManager* GetNtlDTCinematicManager(void)
{
	return CNtlDTCinematicManager::GetInstance();
}

inline RwBool CNtlDTCinematicManager::IsRunning(void)
{
	return m_bRunning;
}

inline RwBool CNtlDTCinematicManager::IsCinematicPlayPossible(void)
{
	return m_bCinematicPossible;
}



#endif