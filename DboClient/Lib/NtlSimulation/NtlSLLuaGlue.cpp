#include "precomp_ntlsimulation.h"
#include "NtlSLLuaGlue.h"

//lua
#include "NtlLuaState.h"

// core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEntity.h"
#include "NtlPLAttach.h"
#include "NtlPLSceneManager.h"
#include "NtlInstanceEffect.h"
#include "NtlPLHelpers.h"
#include "NtlDNController.h"
#include "NtlPLResourcePack.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLLuaGlueStuff.h"
#include "NtlSLDef.h"
#include "NtlSobManager.h"
#include "NtlSobActor.h"
#include "NtlSobProjectileAttr.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMStateBase.h"
#include "NtlBehaviorBase.h"
#include "NtlSobProjectile.h"
#include "NtlSobProxy.h"
#include "NtlCameraManager.h"
#include "NtlDTNode.h"
#include "NtlDTCinematicManager.h"
#include "NtlSLTBCrowdController.h"
#include "NtlSLTBCrowdManager.h"
#include "NtlSobNpc.h"


#define NTL_LUA_GLUE_RESULT_YES		1
#define NTL_LUA_GLUE_RESULT_NO		0

CNtlLuaState *g_pLuaState = NULL;

extern "C" {
typedef struct 
{
	const char *name;
	int (*func)(lua_State *);
}SSLLuaGlueDef;
}

extern SSLLuaGlueDef SLGlue[];

SSLLuaGlueDef SLGlue[] = 
{
	// Lua run script
	{"LuaGlue_RunScript",						LuaGlue_RunScript},
	
	// Lua register function
	{"LuaGlue_RegisterSLSetupData",				LuaGlue_RegisterSLSetupData},
	{"LuaGlue_RegisterSobCreate",				LuaGlue_RegisterSobCreate},
	{"LuaGlue_RegisterSobDestroy",				LuaGlue_RegisterSobDestroy},
	{"LuaGlue_RegisterCharStateEnter",			LuaGlue_RegisterCharStateEnter},
	{"LuaGlue_RegisterCharStateExit",			LuaGlue_RegisterCharStateExit},
	{"LuaGlue_RegisterCharBehaviorEnter",		LuaGlue_RegisterCharBehaviorEnter},
	{"LuaGlue_RegisterCharBehaviorExit",		LuaGlue_RegisterCharBehaviorExit},
	{"LuaGlue_RegisterAttack",					LuaGlue_RegisterAttack},
	{"LuaGlue_RegisterHurt",					LuaGlue_RegisterHurt},
	{"LuaGlue_RegisterSkillCastingEnter",		LuaGlue_RegisterSkillCastingEnter},
	{"LuaGlue_RegisterSkillCastingExit",		LuaGlue_RegisterSkillCastingExit},
	{"LuaGlue_RegisterSkillActionEnter",		LuaGlue_RegisterSkillActionEnter},
	{"LuaGlue_RegisterSkillActionExit",			LuaGlue_RegisterSkillActionExit},
	{"LuaGlue_RegisterHTBSkillStepEnter",		LuaGlue_RegisterHTBSkillStepEnter},
	{"LuaGlue_RegisterHTBSkillStepExit",		LuaGlue_RegisterHTBSkillStepExit},
	{"LuaGlue_RegisterHTBCameraStart",			LuaGlue_RegisterHTBCameraStart},
	{"LuaGlue_RegisterDragonDNEnter",			LuaGlue_RegisterDragonDNEnter},
	{"LuaGlue_RegisterExplosionCamera",			LuaGlue_RegisterExplosionCamera},
    {"LuaGlue_RegisterObserverCamera",          LuaGlue_RegisterObserverCamera},

	// 물리적/논리적 객체 생성 함수
	{"LuaGlue_CreateEffect",					LuaGlue_CreateEffect},
	{"LuaGlue_CreateSound",						LuaGlue_CreateSound},
	
	// data를 참조할 actor 객체 설정
	{"LuaGlue_ActiveActor",						LuaGlue_ActiveActor},
	{"LuaGlue_ActiveObject",					LuaGlue_ActiveObject},
	{"LuaGlue_ActiveActorEdit",					LuaGlue_ActiveActorEdit},
	{"LuaGlue_ActiveObjectEdit",				LuaGlue_ActiveObjectEdit},

	// transform position에 관련된 함수
	{"LuaGlue_BeginTransform",					LuaGlue_BeginTransform},
	{"LuaGlue_EndTransform",					LuaGlue_EndTransform},
	{"LuaGlue_SetPosition",						LuaGlue_SetPosition},
	{"LuaGlue_SetBonePosition",					LuaGlue_SetBonePosition},
	{"LuaGlue_AddPosition",						LuaGlue_AddPosition},

	// transform direction에 관련된 함수
	{"LuaGlue_SetDirection",					LuaGlue_SetDirection},
	
	// transform scale에 관련된 함수
	{"LuaGlue_SetScale",						LuaGlue_SetScale},

	// attach에 관련된 함수.
	{"LuaGlue_AttachWorld",						LuaGlue_AttachWorld},
	{"LuaGlue_AttachBone",						LuaGlue_AttachBone},

	// actor 정보와 관련된 함수.
	{"LuaGlue_GetActorPosition",				LuaGlue_GetActorPosition},
	{"LuaGlue_GetActorFrontPosition",			LuaGlue_GetActorFrontPosition},
	{"LuaGlue_GetActorBackPosition",			LuaGlue_GetActorBackPosition},
	{"LuaGlue_GetActorLeftPosition",			LuaGlue_GetActorLeftPosition},
	{"LuaGlue_GetActorRightPosition",			LuaGlue_GetActorRightPosition},

	// camera에 관련된 함수.
	{"LuaGlue_CameraShake",								LuaGlue_CameraShake},
	{"LuaGlue_SetCameraShakeFactor",					LuaGlue_SetCameraShakeFactor},
	{"LuaGlue_ResetCameraShakeFactor",					LuaGlue_ResetCameraShakeFactor},
	{"LuaGlue_SetCameraFov",							LuaGlue_SetCameraFov},
	{"LuaGlue_CameraDirectChildBegin",					LuaGlue_CameraDirectChildBegin},
	{"LuaGlue_CameraDirectChildEnd",					LuaGlue_CameraDirectChildEnd},
	{"LuaGlue_CameraDirectClear",						LuaGlue_CameraDirectClear},
	{"LuaGlue_AddCameraPauseNode",						LuaGlue_AddCameraPauseNode},
	{"LuaGlue_AddSkillSplineDynamicLookAtNode",			LuaGlue_AddSkillSplineDynamicLookAtNode},
	{"LuaGlue_AddSkillSplineDynamicLookAtCenterNode",	LuaGlue_AddSkillSplineDynamicLookAtCenterNode},
	{"LuaGlue_AddSkillSplineStaticPosDynamicLookAtNode",LuaGlue_AddSkillSplineStaticPosDynamicLookAtNode},
	{"LuaGlue_AddSkillAttackerSplineCameraSelfNode",	LuaGlue_AddSkillAttackerSplineCameraSelfNode},
	{"LuaGlue_AddSkillTargetSplineCameraSelfNode",		LuaGlue_AddSkillTargetSplineCameraSelfNode},
	{"LuaGlue_AddSkillSplineActorRefSelfNode",			LuaGlue_AddSkillSplineActorRefSelfNode},
    {"LuaGlue_AddSplineCameraSelfNode",                 LuaGlue_AddSplineCameraSelfNode},

	// actor에 관련된 함수.
	{"LuaGlue_GetOwnerSerialId",						LuaGlue_GetOwnerSerialId},
	{"LuaGlue_GetActorType",							LuaGlue_GetActorType},
	{"LuaGlue_IsAvatarActor",							LuaGlue_IsAvatarActor},

	// Logic => Skill camera control 속성에 관련된 함수.
	{"LuaGlue_GetSkillAttackSerialId",					LuaGlue_GetSkillAttackSerialId},
	{"LuaGlue_GetSkillTargetSerialId",					LuaGlue_GetSkillTargetSerialId},

	// presentation => day and night에 관련된 함수.
	{"LuaGlue_AddDNNode",								LuaGlue_AddDNNode},
	{"LuaGlue_AddDNNodeCurrentStart",					LuaGlue_AddDNNodeCurrentStart},

	// Logic => cinematic 연출에 관련된 함수.
	{"LuaGlue_BeginCinematic",							LuaGlue_BeginCinematic},
	{"LuaGlue_EndCinematic",							LuaGlue_EndCinematic},
	{"LuaGlue_BeignCinematicLayer",						LuaGlue_BeignCinematicLayer},
	{"LuaGlue_EndCinematicLayer",						LuaGlue_EndCinematicLayer},
	{"LuaGlue_BeignCinematicGroup",						LuaGlue_BeignCinematicGroup},
	{"LuaGlue_EndCinematicGroup",						LuaGlue_EndCinematicGroup},
	{"LuaGlue_BeginCinematicSibling",					LuaGlue_BeginCinematicSibling},
	{"LuaGlue_EndCinematicSibling",						LuaGlue_EndCinematicSibling},
	{"LuaGlue_BeginCinematicCameraAsync",				LuaGlue_BeginCinematicCameraAsync},
	{"LuaGlue_EndCinematicCameraAsync",					LuaGlue_EndCinematicCameraAsync},
	{"LuaGlue_BeginCinematicCameraLock",				LuaGlue_BeginCinematicCameraLock},
	{"LuaGlue_EndCinematicCameraLock",					LuaGlue_EndCinematicCameraLock},
	{"LuaGlue_CNViewShow",								LuaGlue_CNViewShow},
	{"LuaGlue_CNViewHide",								LuaGlue_CNViewHide},
	{"LuaGlue_CNBalloon",								LuaGlue_CNBalloon},
	{"LuaGlue_CNBalloonBegin",							LuaGlue_CNBalloonBegin},
	{"LuaGlue_CNBalloonProgress",						LuaGlue_CNBalloonProgress},
	{"LuaGlue_CNBalloonEnd",							LuaGlue_CNBalloonEnd},
	{"LuaGlue_CNBalloonSingle",							LuaGlue_CNBalloonSingle},
	{"LuaGlue_CNFlash",									LuaGlue_CNFlash},
	{"LuaGlue_AddCNMobCreate",							LuaGlue_AddCNMobCreate},
	{"LuaGlue_AddCNNpcCreate",							LuaGlue_AddCNNpcCreate},
	{"LuaGlue_AddCNSobDelete",							LuaGlue_AddCNSobDelete},
	{"LuaGlue_AddCNSobMove",							LuaGlue_AddCNSobMove},
	{"LuaGlue_AddCNSobSplineMove",						LuaGlue_AddCNSobSplineMove},
	{"LuaGlue_AddCNSobSplineWalkMove",					LuaGlue_AddCNSobSplineWalkMove},
	{"LuaGlue_AddCNSobSplineFlyMove",					LuaGlue_AddCNSobSplineFlyMove},
	{"LuaGlue_AddCNSobTriggerObjectState",				LuaGlue_AddCNSobTriggerObjectState},
	{"LuaGlue_AddCNSobDirectPlay",						LuaGlue_AddCNSobDirectPlay},
	{"LuaGlue_AddCNSobAnimPlay",						LuaGlue_AddCNSobAnimPlay},
	{"LuaGlue_AddCNSobAnimPlayPositionSetting",			LuaGlue_AddCNSobAnimPlayPositionSetting},
	{"LuaGlue_AddCNSobPositionSetting",					LuaGlue_AddCNSobPositionSetting},
	{"LuaGlue_AddCNSobDirectSetting",					LuaGlue_AddCNSobDirectSetting},
	{"LuaGlue_AddCNSobRotateY",							LuaGlue_AddCNSobRotateY},
	{"LuaGlue_AddCNPause",								LuaGlue_AddCNPause},
	{"LuaGlue_AddCNEffectActorBone",					LuaGlue_AddCNEffectActorBone},
	{"LuaGlue_AddCNEffectActorOffset",					LuaGlue_AddCNEffectActorOffset},
	{"LuaGlue_AddCNEffectWorld",						LuaGlue_AddCNEffectWorld},
	{"LuaGlue_AddCNEffectDelete",						LuaGlue_AddCNEffectDelete},
	{"LuaGlue_AddCNCameraSoftActorDist",				LuaGlue_AddCNCameraSoftActorDist},
	{"LuaGlue_AddCNCameraSoftActorDyncLookAtDist",		LuaGlue_AddCNCameraSoftActorDyncLookAtDist},
	{"LuaGlue_AddCNCameraSoftCurrRotX",					LuaGlue_AddCNCameraSoftCurrRotX},
	{"LuaGlue_AddCNCameraSoftCurrRotY",					LuaGlue_AddCNCameraSoftCurrRotY},
	{"LuaGlue_AddCNCameraSoftCurrRotZ",					LuaGlue_AddCNCameraSoftCurrRotZ},
	{"LuaGlue_AddCNCameraSoftCurrFov",					LuaGlue_AddCNCameraSoftCurrFov},
	{"LuaGlue_AddCNCameraSoftPause",					LuaGlue_AddCNCameraSoftPause},
	{"LuaGlue_AddCNCameraSplineSelf",					LuaGlue_AddCNCameraSplineSelf},
	{"LuaGlue_AddCNCameraSplineSelfActor",				LuaGlue_AddCNCameraSplineSelfActor},
	{"LuaGlue_AddCNCameraSplineCurrDyncLookAt",			LuaGlue_AddCNCameraSplineCurrDyncLookAt},
	{"LuaGlue_AddCNCameraShake",						LuaGlue_AddCNCameraShake},
	{"LuaGlue_AddCNCameraShakeFactor",					LuaGlue_AddCNCameraShakeFactor},
	{"LuaGlue_AddCNCameraShakeFactorReset",				LuaGlue_AddCNCameraShakeFactorReset},
	{"LuaGlue_AddCNFadeIn",								LuaGlue_AddCNFadeIn},
	{"LuaGlue_AddCNFadeOut",							LuaGlue_AddCNFadeOut},
	{"LuaGlue_AddCNFadeInTVH",							LuaGlue_AddCNFadeInTVH},
	{"LuaGlue_AddCNFadeOutTVH",							LuaGlue_AddCNFadeOutTVH},
	{"LuaGlue_AddCNSound2D",							LuaGlue_AddCNSound2D},	
	{"LuaGlue_AddCNSound3D",							LuaGlue_AddCNSound3D},
	{"LuaGlue_AddCNSoundBGM",							LuaGlue_AddCNSoundBGM},
	{"LuaGlue_AddCNSoundBGMStop",						LuaGlue_AddCNSoundBGMStop},
	{"LuaGlue_AddCNCharacterShadowOnOff",				LuaGlue_AddCNCharacterShadowOnOff},
	{"LuaGlue_AddCNCharacterVisibleOnOff",				LuaGlue_AddCNCharacterVisibleOnOff},
	{"LuaGlue_AddCNSceneMoonVisibleOnOff",				LuaGlue_AddCNSceneMoonVisibleOnOff},
	{"LuaGlue_AddCNSceneRainBloomLight",				LuaGlue_AddCNSceneRainBloomLight},
	{"LuaGlue_AddCNSceneRainSkyLight",					LuaGlue_AddCNSceneRainSkyLight},
	{"LuaGlue_AddCNGuiCommand",							LuaGlue_AddCNGuiCommand},
	{"LuaGlue_AddCNCinematicServerAck",					LuaGlue_AddCNCinematicServerAck},
	{"LuaGlue_AddSinExplosionAlgorithm_XAxis",			LuaGlue_AddSinExplosionAlgorithm_XAxis},
	{"LuaGlue_AddSinExplosionAlgorithm_YAxis",			LuaGlue_AddSinExplosionAlgorithm_YAxis},
	{"LuaGlue_AddSinExplosionAlgorithm_ZAxis",			LuaGlue_AddSinExplosionAlgorithm_ZAxis},
	{"LuaGlue_AddCNCinematicScale",						LuaGlue_AddCNCinematicScale},

	// 연출 관련
	{"LuaGlue_DTAddController",							LuaGlue_DTAddController},
	{"LuaGlue_DTRemoveController",						LuaGlue_DTRemoveController},    

	{NULL, NULL}
};


#define MAX_LUAFUNC_LEN		128
#define MAX_LUACMD_LEN		256

typedef struct 
{
	char fpSLSetupData[MAX_LUAFUNC_LEN];
	char fpSobCreate[MAX_LUAFUNC_LEN];
	char fpSobDestroy[MAX_LUAFUNC_LEN];
	char fpCharStateEnter[MAX_LUAFUNC_LEN];
	char fpCharStateExit[MAX_LUAFUNC_LEN];
	char fpCharBehaviorEnter[MAX_LUAFUNC_LEN];
	char fpCharBehaviorExit[MAX_LUAFUNC_LEN];
	char fpAttack[MAX_LUAFUNC_LEN];
	char fpHurt[MAX_LUAFUNC_LEN];
	char fpSkillCastingEnter[MAX_LUAFUNC_LEN];
	char fpSkillCastingExit[MAX_LUAFUNC_LEN];
	char fpSkillActionEnter[MAX_LUAFUNC_LEN];
	char fpSkillActionExit[MAX_LUAFUNC_LEN];
	char fpHTBSkillStepEnter[MAX_LUAFUNC_LEN];
	char fpHTBSkillStepExit[MAX_LUAFUNC_LEN];
	char fpHTBCameraStart[MAX_LUAFUNC_LEN];
	char fpDragonDNEnter[MAX_LUAFUNC_LEN];	
	char fpExplosionCamera[MAX_LUAFUNC_LEN];	
    char fpObserverCamera[MAX_LUAFUNC_LEN];
}SLuaExecFunc;

SLuaExecFunc g_LuaExecFunc;


void LuaState_ErrorHandler(const char *pError)
{
	NtlLogFilePrint(const_cast<RwChar*>(pError));
}


CNtlLuaState* GetLuaState(void)
{
	return g_pLuaState;
}

void SLLua_CreateState(void)
{
	if(g_pLuaState)
	{
		NTL_DELETE(g_pLuaState);
		g_pLuaState = NULL;
	}

	g_pLuaState = NTL_NEW CNtlLuaState;
	g_pLuaState->LinkErrorHandler(LuaState_ErrorHandler);


	// init the glue functions required for the GUI
	for(int i=0; SLGlue[i].name; i++)
	{
		g_pLuaState->AddFunction(SLGlue[i].name, SLGlue[i].func);
	}
}

void SLLua_Compile(void)
{
	if ( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT )
	{
		char* pBuffer = NULL;
		int nBufferSize;

		GetNtlResourcePackManager()->LoadScript("script\\sl.lua", (void**)&pBuffer, &nBufferSize );

		DBO_ASSERTE( pBuffer );

		if ( pBuffer )
		{
			char *pString = NTL_NEW char[nBufferSize+1];
			memcpy(pString, pBuffer, nBufferSize);
			pString[nBufferSize] = 0;

			g_pLuaState->RunString(pString);

			NTL_ARRAY_DELETE( pString );
			NTL_ARRAY_DELETE( pBuffer );
		}
	}
	else
	{
		g_pLuaState->RunScript("script\\sl.lua");
	}

	// sl setup data setting lua 실행.
	LuaExec_SLSetupData();
}

void SLLua_Setup(void)
{
	SLLua_CreateState();

	SLLua_Compile();
}

void SLLua_Release(void)
{
	if(g_pLuaState)
	{
		NTL_DELETE(g_pLuaState);
		g_pLuaState = NULL;
	}
}

void LuaExec_SLSetupData(void)
{
	if(g_pLuaState == NULL)
		return;

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u)", g_LuaExecFunc.fpSLSetupData, 1);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_SobCreate(CNtlSob *pSobObj)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetActiveObj(LUA_STACK_ACTIVEOBJ_SOB, reinterpret_cast<void*>(pSobObj)); 

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u,%u)", g_LuaExecFunc.fpSobCreate, pSobObj->GetSerialID(), pSobObj->GetClassID(), pSobObj->GetVisualLuaID());
	g_pLuaState->RunString(chCmd);
}

void LuaExec_SobDestroy(CNtlSob *pSobObj)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetActiveObj(LUA_STACK_ACTIVEOBJ_SOB, reinterpret_cast<void*>(pSobObj)); 

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpSobDestroy, pSobObj->GetSerialID(), pSobObj->GetClassID());
	g_pLuaState->RunString(chCmd);
}

void LuaExec_CharStateEnter(unsigned int uiSerialId, CNtlFSMStateBase *pState)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetActiveObj(LUA_STACK_ACTIVEOBJ_STATE, reinterpret_cast<void*>(pState)); 

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpCharStateEnter, uiSerialId, pState->GetStateId());
	g_pLuaState->RunString(chCmd);
}

void LuaExec_CharStateExit(unsigned int uiSerialId, CNtlFSMStateBase *pState)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetActiveObj(LUA_STACK_ACTIVEOBJ_STATE, reinterpret_cast<void*>(pState)); 

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpCharStateExit, uiSerialId, pState->GetStateId());
	g_pLuaState->RunString(chCmd);
}

void LuaExec_CharBehaviorEnter(unsigned int uiSerialId, CNtlBehaviorBase *pBehavior)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetActiveObj(LUA_STACK_ACTIVEOBJ_BEHAVIOR, reinterpret_cast<void*>(pBehavior)); 

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpCharBehaviorEnter, uiSerialId, pBehavior->GetBehaviorId());
	g_pLuaState->RunString(chCmd);
}

void LuaExec_CharBehaviorExit(unsigned int uiSerialId, CNtlBehaviorBase *pBehavior)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetActiveObj(LUA_STACK_ACTIVEOBJ_BEHAVIOR, reinterpret_cast<void*>(pBehavior)); 

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpCharBehaviorExit, uiSerialId, pBehavior->GetBehaviorId());
	g_pLuaState->RunString(chCmd);
}

void LuaExec_Attack(unsigned int uiAttackerSerialId, unsigned int uiDefenderSerialId, const SHitStuff *pHit)
{
	// (Fluorite - not yet used)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetHit(pHit);
	
	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpAttack, uiAttackerSerialId, uiDefenderSerialId);
	g_pLuaState->RunString(chCmd);
}


void LuaExec_Hurt(unsigned int uiAttackerSerialId, unsigned int uiDefenderSerialId, const SHitStuff *pHit)
{
	// (형석 - 아직 사용 안함)
	return;

	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();
	GetLuaGlueStack()->SetHit(pHit);

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpHurt, uiAttackerSerialId, uiDefenderSerialId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_SkillCastingEnter(unsigned int uiSerialId, unsigned int uiSkillId)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpSkillCastingEnter, uiSerialId, uiSkillId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_SkillCastingExit(unsigned int uiSerialId, unsigned int uiSkillId, unsigned int uiSkillSuccess)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u,%u)", g_LuaExecFunc.fpSkillCastingExit, uiSerialId, uiSkillId, uiSkillSuccess);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_SkillActionEnter(unsigned int uiSerialId, unsigned int uiSkillId)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpSkillActionEnter, uiSerialId, uiSkillId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_SkillActionExit(unsigned int uiSerialId, unsigned int uiSkillId)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u)", g_LuaExecFunc.fpSkillActionExit, uiSerialId, uiSkillId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_HTBSkillStepEnter(unsigned int uiSerialId, unsigned int uiHtbTblId, unsigned int uiSkillTblId, unsigned int uiStepId)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u,%u,%u)", g_LuaExecFunc.fpHTBSkillStepEnter, uiSerialId, uiHtbTblId, uiSkillTblId, uiStepId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_HTBSkillStepExit(unsigned int uiSerialId, unsigned int uiHtbTblId, unsigned int uiSkillTblId, unsigned int uiStepId)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->Reset();

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u,%u,%u,%u)", g_LuaExecFunc.fpHTBSkillStepExit, uiSerialId, uiHtbTblId, uiSkillTblId, uiStepId);
	g_pLuaState->RunString(chCmd);
}


void LuaExec_HTBCameraStart(unsigned int  uiAttackSerialId, unsigned int uiTargetSerialId, unsigned int uiHtbTblId)
{
	if(g_pLuaState == NULL)
		return;

	GetLuaGlueStack()->SetHTBAttackSerialId(uiAttackSerialId);
	GetLuaGlueStack()->SetHTBTargetSerialId(uiTargetSerialId);

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u)", g_LuaExecFunc.fpHTBCameraStart, uiHtbTblId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_DragonDNEnter() 
{
	if(g_pLuaState == NULL)
		return;

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, "%s()", g_LuaExecFunc.fpDragonDNEnter);
	g_pLuaState->RunString(chCmd);
}


void LuaExec_ExplosionCamera(unsigned int uiExplosionId)
{
	if(g_pLuaState == NULL)
		return;

	char chCmd[MAX_LUACMD_LEN];
	sprintf_s(chCmd, MAX_LUACMD_LEN, "%s(%u)", g_LuaExecFunc.fpExplosionCamera, uiExplosionId);
	g_pLuaState->RunString(chCmd);
}

void LuaExec_ObserverCamera( unsigned int uiIndex ) 
{
    if(g_pLuaState == NULL)
        return;

    char chCmd[MAX_LUACMD_LEN];
    sprintf_s(chCmd, "%s(%u)", g_LuaExecFunc.fpObserverCamera, uiIndex);
    g_pLuaState->RunString(chCmd);
}

void LuaExec_DirectionStep(const char* pcStepName)
{
    if(g_pLuaState == NULL)
        return;
    
	if(!pcStepName)
		return;

	g_pLuaState->RunString(pcStepName);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Lua Glue Function
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// lua run script
////////////////////////////////////////////////////////////////////////////

int LuaGlue_RunScript(lua_State *L)
{
	const char *pLuaFile = g_pLuaState->GetStringArgument(1);
	if(g_pLuaState)
	{
		char chCmd[MAX_LUACMD_LEN];
		sprintf_s(chCmd, MAX_LUACMD_LEN, "script\\%s", pLuaFile);

		if ( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT )
		{
			char* pBuffer = NULL;
			int nBufferSize;

			GetNtlResourcePackManager()->LoadScript( chCmd, (void**)&pBuffer, &nBufferSize );

			DBO_ASSERTE( pBuffer );

			if(pBuffer)
			{
				char *pString = NTL_NEW char[nBufferSize+1];
				memcpy(pString, pBuffer, nBufferSize);
				pString[nBufferSize] = 0;

				g_pLuaState->RunString(pString);

				NTL_ARRAY_DELETE( pString );
				NTL_ARRAY_DELETE( pBuffer );
			}
		}
		else
		{
			g_pLuaState->RunScript(chCmd);
		}
	}

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// lua register function
////////////////////////////////////////////////////////////////////////////
#define LUA_REGISTER(funcName) \
    const char *pKey = g_pLuaState->GetStringArgument(1);\
    strcpy_s(funcName, MAX_LUAFUNC_LEN, pKey);\
    return NTL_LUA_GLUE_RESULT_NO;

int LuaGlue_RegisterSLSetupData(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSLSetupData);
}

int LuaGlue_RegisterSobCreate(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSobCreate);
}

int LuaGlue_RegisterSobDestroy(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSobDestroy);
}

int LuaGlue_RegisterCharStateEnter(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpCharStateEnter);
}

int LuaGlue_RegisterCharStateExit(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpCharStateExit);
}

int LuaGlue_RegisterCharBehaviorEnter(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpCharBehaviorEnter);
}

int LuaGlue_RegisterCharBehaviorExit(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpCharBehaviorExit);
}

int LuaGlue_RegisterAttack(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpAttack);
}

int LuaGlue_RegisterHurt(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpHurt);
}

int LuaGlue_RegisterSkillCastingEnter(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSkillCastingEnter);
}

int LuaGlue_RegisterSkillCastingExit(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSkillCastingExit);
}

int LuaGlue_RegisterSkillActionEnter(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSkillActionEnter);
}

int LuaGlue_RegisterSkillActionExit(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpSkillActionExit);
}

int LuaGlue_RegisterHTBSkillStepEnter(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpHTBSkillStepEnter);
}

int LuaGlue_RegisterHTBSkillStepExit(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpHTBSkillStepExit);
}

int LuaGlue_RegisterHTBCameraStart(lua_State *L)
{
    LUA_REGISTER(g_LuaExecFunc.fpHTBCameraStart);
}

int LuaGlue_RegisterDragonDNEnter( lua_State* L ) 
{
    LUA_REGISTER(g_LuaExecFunc.fpDragonDNEnter);
}

int LuaGlue_RegisterExplosionCamera(lua_State* L)
{
    LUA_REGISTER(g_LuaExecFunc.fpExplosionCamera);
}

int LuaGlue_RegisterObserverCamera( lua_State* L ) 
{
    LUA_REGISTER(g_LuaExecFunc.fpObserverCamera);
}


////////////////////////////////////////////////////////////////////////////
// lua 물리적/논리적 객체 생성 함수
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//  des : effect를 생성하는 lua glue 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : effect property key name.
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CreateEffect(lua_State *L)
{
	return 0;

	/*
	const char *pKey = g_pLuaState->GetStringArgument(1);
	
	if(pKey == NULL)
	{
		NtlLogFilePrint("LuaGlue_CreateEffect Function Error!!! - effect key is NULL");
		return INVALID_SERIAL_ID;
	}

	CNtlSobActor *pReferActor = GetLuaGlueStack()->GetReferActor();
	if(pReferActor == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_CreateEffect(%s) Function Error!!! - active parent actor is NUll", pKey);
		return INVALID_SERIAL_ID;
	}

	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pKey);
	if(pPLEntity == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_CreateEffect Function Error!!! - not exist effect key(%s)", pKey);
		return INVALID_SERIAL_ID;
	}

	GetLuaGlueStack()->SetPLEntity(pPLEntity); 

	if(!pPLEntity->IsAutoDelete())
	{
		RwBool bRet = GetLuaGlueStack()->AddActivePLEntity(pPLEntity);
		if(!bRet)
		{
			NtlLogFilePrintArgs("LuaGlue_CreateEffect Function Warning!!! - can't add effect key(%s)", pKey);
		}
	}

	RwV3d vPos = pReferActor->GetPosition();
	pPLEntity->SetPosition(&vPos); 

	return 0;
	*/
}


///////////////////////////////////////////////////////////////////////////
//  des : sound play lua glue 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : sound file name.
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CreateSound(lua_State *L)
{
	const char *pFileName = g_pLuaState->GetStringArgument(1);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
//  des : active actor를 설정하는 lua glue 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : actor serial id
////////////////////////////////////////////////////////////////////////////

int LuaGlue_ActiveActor(lua_State *L)
{
	unsigned int uiSerialId = (unsigned int)g_pLuaState->GetNumberArgument(1);
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_ActiveActor Function Error!!! - not exist serial id %u", uiSerialId);
		return NTL_LUA_GLUE_RESULT_NO;
	}

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>(pSobObj);
	GetLuaGlueStack()->SetActor(pActor);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
//  des : active presentation entity를 설정하는 lua glue 함수.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : presentation entity serial id
////////////////////////////////////////////////////////////////////////////

int LuaGlue_ActiveObject(lua_State *L)
{
	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
//  des : edit할 객체가 simulation actor로 설정한다
//  ------------------------------------------------------------------------
//  paramater : 없슴
////////////////////////////////////////////////////////////////////////////

int LuaGlue_ActiveActorEdit(lua_State *L)
{
	GetLuaGlueStack()->SetEditType(LUA_STACK_EDIT_ACTOR);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
//  des : edit할 객체가 presentation entity로 설정한다.
//  ------------------------------------------------------------------------
//  paramater : 없슴
////////////////////////////////////////////////////////////////////////////

int LuaGlue_ActiveObjectEdit(lua_State *L)
{
	GetLuaGlueStack()->SetEditType(LUA_STACK_EDIT_OBJECT);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 좌표를 설정한다.
////////////////////////////////////////////////////////////////////////////

bool LuaGlue_PreTransformResearch(char *pString)
{
	if(GetLuaGlueStack()->GetActor() == NULL)
	{
		NtlLogFilePrintArgs("%s - edit actor not setting!!! used LuaGlue_ActiveActor function", pString);
		return false;
	}

	if(GetLuaGlueStack()->GetEditType() == LUA_STACK_EDIT_OBJECT)
	{
		if(GetLuaGlueStack()->GetPLEntity() == NULL)
		{
			NtlLogFilePrintArgs("%s - edit presentation object not setting!!! used LuaGlue_ActiveObject function", pString);
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 transform 시작을 설정한다.
//  ------------------------------------------------------------------------
//  paramater : 없슴
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeginTransform(lua_State *L)
{
	GetLuaGlueStack()->BeginTransform();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 transform을 끝났음을 알리고, 실제 transform을 
//       적용한다. 
//  ------------------------------------------------------------------------
//  paramater : 없슴
////////////////////////////////////////////////////////////////////////////

int LuaGlue_EndTransform(lua_State *L)
{
	GetLuaGlueStack()->EndTransform();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 좌표를 설정한다.
//  ------------------------------------------------------------------------
//  paramater1 : offfset x(number)
//  paramater2 : offfset y(number)
//  paramater3 : offfset z(number)
////////////////////////////////////////////////////////////////////////////

int LuaGlue_SetPosition(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_SetPosition"))
		return NTL_LUA_GLUE_RESULT_NO;
	
	CNtlSobActor *pActor = GetLuaGlueStack()->GetActor(); 

	RwV3d vPos = pActor->GetPosition();

	RwReal fOffsetX = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fOffsetY = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fOffsetZ = (RwReal)g_pLuaState->GetNumberArgument(3);

	GetLuaGlueStack()->SetPosition(vPos.x+fOffsetX, vPos.y+fOffsetY, vPos.z+fOffsetZ);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 좌표를 설정한다.
//  ------------------------------------------------------------------------
//  paramater1 : bone name(string)
//  paramater2 : offset x(number)
//  paramater3 : offset y(number)
//  paramater4 : offset z(number)
////////////////////////////////////////////////////////////////////////////

int LuaGlue_SetBonePosition(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_SetBonePosition"))
		return NTL_LUA_GLUE_RESULT_NO;

	const char *pBoneKey = g_pLuaState->GetStringArgument(1);
	RwReal fOffsetX = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fOffsetY = (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fOffsetZ = (RwReal)g_pLuaState->GetNumberArgument(4);

	RwBool bBoneExist = TRUE;
	CNtlSobActor *pActor = GetLuaGlueStack()->GetActor();
	CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
	if(pSobProxy == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_SetBonePosition - refer actor proxy instance null !!!");
		return NTL_LUA_GLUE_RESULT_NO;
	}

	CNtlPLEntity *pPLEntity = pSobProxy->GetPLMainEntity();
	if(pPLEntity == NULL || !(pPLEntity->GetFlags() & NTL_PLEFLAG_ATTACH) || pBoneKey)
	{
		NtlLogFilePrintArgs("LuaGlue_SetBonePosition - bone name(%s) is not exist", pBoneKey);
		bBoneExist = FALSE;
	}

	RwV3d vPos = pActor->GetPosition();

	if(bBoneExist)
	{
		CNtlPLAttach *pPLAttach = reinterpret_cast<CNtlPLAttach*>(pPLEntity);
		RwMatrix *pMat = pPLAttach->GetBoneMatrix(pBoneKey);
		if(pMat == NULL)
		{
			NtlLogFilePrintArgs("LuaGlue_SetBonePosition - bone name(%s) is not exist", pBoneKey);
		}
		else
		{
			vPos = *RwMatrixGetPos(pMat); 
		}
	}
	else
	{
		GetLuaGlueStack()->SetPosition(vPos.x+fOffsetX, vPos.y+fOffsetY, vPos.z+fOffsetZ);
	}

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 설정된 좌표에 offset만큼 더한다.
//  ------------------------------------------------------------------------
//  paramater1 : offset x(number)
//  paramater2 : offset y(number)
//  paramater3 : offset z(number)
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddPosition(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_AddPosition"))
		return NTL_LUA_GLUE_RESULT_NO;

	RwReal fOffsetX = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fOffsetY = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fOffsetZ = (RwReal)g_pLuaState->GetNumberArgument(3);

	GetLuaGlueStack()->AddPosition(fOffsetX, fOffsetY, fOffsetZ);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// lua : transform direction에 관련된 함수.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 설정된 방향을 설정한다.(reference actor 방향 참조)
//  ------------------------------------------------------------------------
//  paramater1 : 없슴.
////////////////////////////////////////////////////////////////////////////
int LuaGlue_SetDirection(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_SetDirection"))
		return NTL_LUA_GLUE_RESULT_NO;

	CNtlSobActor *pActor = GetLuaGlueStack()->GetActor(); 
	RwV3d vDir = pActor->GetDirection();
	
	GetLuaGlueStack()->SetDirection(vDir.x, vDir.y, vDir.z);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// lua : transform scale에 관련된 함수.
////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////
// des : edit할 active객체의 scale을 설정한다.
//  ------------------------------------------------------------------------
//  paramater1 : scale x(number)
//  paramater2 : scale y(number)
//  paramater3 : scale z(number)
////////////////////////////////////////////////////////////////////////////

int LuaGlue_SetScale(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_SetScale"))
		return NTL_LUA_GLUE_RESULT_NO;

	RwReal fScaleX = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fScaleY = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fScaleZ = (RwReal)g_pLuaState->GetNumberArgument(3);

	GetLuaGlueStack()->SetScale(fScaleX, fScaleY, fScaleZ);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// lua : attach에 관련된 함수.
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AttachWorld(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_SetScale"))
		return NTL_LUA_GLUE_RESULT_NO;

	CNtlPLEntity *pPLEntity = GetLuaGlueStack()->GetPLEntity();
	if(pPLEntity == NULL)
	{
		NtlLogFilePrintArgs("%s - edit presentation object not setting!!! used LuaGlue_ActiveObject function", "LuaGlue_AttachWorld");
		return NTL_LUA_GLUE_RESULT_NO;
	}

	RwReal fOffsetX = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fOffsetY = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fOffsetZ = (RwReal)g_pLuaState->GetNumberArgument(3);

	CNtlSobActor *pSobActor = GetLuaGlueStack()->GetActor();
	CNtlSobProxy *pSobProxy = pSobActor->GetSobProxy();
	if(pSobProxy == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_AttachWorld actor proxy instance null !!!");
		return NTL_LUA_GLUE_RESULT_NO;
	}

	CNtlPLEntity *pPLTargetEntity = pSobProxy->GetPLMainEntity();

	RwV3d vOffset;
	CNtlMath::MathRwV3dAssign(&vOffset, fOffsetX, fOffsetY, fOffsetZ);

	Helper_AttachWorldPos(pPLTargetEntity, pPLEntity, vOffset);

	return NTL_LUA_GLUE_RESULT_NO;
}

int LuaGlue_AttachBone(lua_State *L)
{
	if(!LuaGlue_PreTransformResearch("LuaGlue_SetScale"))
		return NTL_LUA_GLUE_RESULT_NO;

	CNtlPLEntity *pPLEntity = GetLuaGlueStack()->GetPLEntity();
	if(pPLEntity == NULL)
	{
		NtlLogFilePrintArgs("%s - edit presentation object not setting!!! used LuaGlue_ActiveObject function", "LuaGlue_AttachWorld");
		return NTL_LUA_GLUE_RESULT_NO;
	}

	const char *pBoneKey = g_pLuaState->GetStringArgument(1);

	CNtlSobActor *pSobActor = GetLuaGlueStack()->GetActor();
	CNtlSobProxy *pSobProxy = pSobActor->GetSobProxy();
	if(pSobProxy == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_AttachBone actor proxy instance null !!!");
		return NTL_LUA_GLUE_RESULT_NO;
	}

	CNtlPLEntity *pPLTargetEntity = pSobProxy->GetPLMainEntity(); 

	Helper_AttachBone(pPLTargetEntity, pPLEntity, pBoneKey);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// lua : actor 정보와 관련된 함수.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 actor의 position을 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset rate
////////////////////////////////////////////////////////////////////////////
int LuaGlue_GetActorPosition(lua_State *L)
{
	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightRate	= (RwReal)g_pLuaState->GetNumberArgument(2);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("%s - actor is not exist!!!", "LuaGlue_GetActorPosition");

		g_pLuaState->PushNumber(vPos.x);
		g_pLuaState->PushNumber(vPos.y);
		g_pLuaState->PushNumber(vPos.z);

		return 3;
	}

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	RwReal fHeight = pSobProxy->GetPLEntityHeight();
	vPos = pSobObj->GetPosition();
	vPos.y += fHeight*fHeightRate;

	g_pLuaState->PushNumber(vPos.x);
	g_pLuaState->PushNumber(vPos.y);
	g_pLuaState->PushNumber(vPos.z);

	return 3;
}


////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 actor의 front 방향의 position을 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset rate
//  paramater3 : front distance
////////////////////////////////////////////////////////////////////////////
int LuaGlue_GetActorFrontPosition(lua_State *L)
{
	RwV3d vPos, vFront;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightRate	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDistance	= (RwReal)g_pLuaState->GetNumberArgument(3);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("%s - actor is not exist!!!", "LuaGlue_GetActorFrontPosition");

		g_pLuaState->PushNumber(vPos.x);
		g_pLuaState->PushNumber(vPos.y);
		g_pLuaState->PushNumber(vPos.z);

		return 3;
	}

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	RwReal fHeight = pSobProxy->GetPLEntityHeight();

	vPos	= pSobObj->GetPosition();
	vFront	= pSobObj->GetDirection();
	CNtlMath::MathRwV3dAdd(&vPos, vFront.x*fDistance, vFront.y*fDistance, vFront.z*fDistance);
	vPos.y += fHeight*fHeightRate;

	g_pLuaState->PushNumber(vPos.x);
	g_pLuaState->PushNumber(vPos.y);
	g_pLuaState->PushNumber(vPos.z);

	return 3;
}


////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 actor의 front 방향의 position을 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset rate
//  paramater3 : back distance
////////////////////////////////////////////////////////////////////////////
int LuaGlue_GetActorBackPosition(lua_State *L)
{
	RwV3d vPos, vFront;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightRate	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDistance	= (RwReal)g_pLuaState->GetNumberArgument(3);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("%s - actor is not exist!!!", "LuaGlue_GetActorBackPosition");

		g_pLuaState->PushNumber(vPos.x);
		g_pLuaState->PushNumber(vPos.y);
		g_pLuaState->PushNumber(vPos.z);

		return 3;
	}

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	RwReal fHeight = pSobProxy->GetPLEntityHeight();

	vPos	= pSobObj->GetPosition();
	vFront	= pSobObj->GetDirection();
	CNtlMath::MathRwV3dAdd(&vPos, -vFront.x*fDistance, -vFront.y*fDistance, -vFront.z*fDistance);
	vPos.y += fHeight*fHeightRate;

	g_pLuaState->PushNumber(vPos.x);
	g_pLuaState->PushNumber(vPos.y);
	g_pLuaState->PushNumber(vPos.z);

	return 3;
}

////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 actor의 left 방향의 position을 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset rate
//  paramater3 : left distance
////////////////////////////////////////////////////////////////////////////
int LuaGlue_GetActorLeftPosition(lua_State *L)
{
	RwV3d vPos, vFront;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightRate	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDistance	= (RwReal)g_pLuaState->GetNumberArgument(3);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("%s - actor is not exist!!!", "LuaGlue_GetActorLeftPosition");

		g_pLuaState->PushNumber(vPos.x);
		g_pLuaState->PushNumber(vPos.y);
		g_pLuaState->PushNumber(vPos.z);

		return 3;
	}

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	RwReal fHeight = pSobProxy->GetPLEntityHeight();

	vPos	= pSobObj->GetPosition();
	vFront	= pSobObj->GetDirection();

	RwMatrix mat;
	CNtlMath::MathRwMatrixAssign(&mat, &CNtlPLGlobal::m_vZeroV3, &vFront);
	CNtlMath::MathRwV3dAdd(&vPos, -mat.right.x*fDistance, -mat.right.y*fDistance, -mat.right.z*fDistance);
	vPos.y += fHeight*fHeightRate;

	g_pLuaState->PushNumber(vPos.x);
	g_pLuaState->PushNumber(vPos.y);
	g_pLuaState->PushNumber(vPos.z);

	return 3;
}


////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 actor의 right 방향의 position을 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset rate
//  paramater3 : left distance
////////////////////////////////////////////////////////////////////////////
int LuaGlue_GetActorRightPosition(lua_State *L)
{
	RwV3d vPos, vFront;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightRate	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDistance	= (RwReal)g_pLuaState->GetNumberArgument(3);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("%s - actor is not exist!!!", "LuaGlue_GetActorLeftPosition");

		g_pLuaState->PushNumber(vPos.x);
		g_pLuaState->PushNumber(vPos.y);
		g_pLuaState->PushNumber(vPos.z);

		return 3;
	}

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	RwReal fHeight = pSobProxy->GetPLEntityHeight();

	vPos	= pSobObj->GetPosition();
	vFront	= pSobObj->GetDirection();

	RwMatrix mat;
	CNtlMath::MathRwMatrixAssign(&mat, &CNtlPLGlobal::m_vZeroV3, &vFront);
	CNtlMath::MathRwV3dAdd(&vPos, mat.right.x*fDistance, mat.right.y*fDistance, mat.right.z*fDistance);
	vPos.y += fHeight*fHeightRate;

	g_pLuaState->PushNumber(vPos.x);
	g_pLuaState->PushNumber(vPos.y);
	g_pLuaState->PushNumber(vPos.z);

	return 3;
}

////////////////////////////////////////////////////////////////////////////
// lua : Logic : camera에 관련된 함수.
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CameraShake(lua_State *L)
{
//	CNtlSLEventGenerator::CameraShake();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : camera shake factor 값을 변경한다.
//  ------------------------------------------------------------------------
//  paramater1 : shake weight value
//  paramater1 : shake height value
////////////////////////////////////////////////////////////////////////////

int LuaGlue_SetCameraShakeFactor(lua_State *L)
{
	RwReal fWeightValue = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightValue = (RwReal)g_pLuaState->GetNumberArgument(2);

	CNtlCameraShakeController::SetShakeFactor(fWeightValue, fHeightValue);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : camera shake factor 값을 Reset 한다.
//  ------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////

int LuaGlue_ResetCameraShakeFactor(lua_State *L)
{
	CNtlCameraShakeController::ResetShakeFactor();

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : camera fov를 설정한다.
//  ------------------------------------------------------------------------
//  paramater1 : fov value
////////////////////////////////////////////////////////////////////////////

int LuaGlue_SetCameraFov(lua_State *L)
{
	RwReal fFov = (RwReal)g_pLuaState->GetNumberArgument(1);

	GetNtlGameCameraManager()->SetFov(fFov);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : camera direction node previous node에 attach 시키는 flag를 enable 한다.
//  ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CameraDirectChildBegin(lua_State *L)
{
	GetNtlGameCameraManager()->SetDTNodeAttach(TRUE);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : camera direction node previous node에 attach 시키는 flag를 disable 한다.
//  ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CameraDirectChildEnd(lua_State *L)
{
	GetNtlGameCameraManager()->SetDTNodeAttach(FALSE);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : camera direct node clear
//  ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CameraDirectClear(lua_State *L)
{
	
	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       지정된 actor의 위치를 구한 다음, actor의 위에 회전한 다음 거리를 계산한여, 
//		 최종 camera 위치를 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_SetSoftCameraActorLookAt(lua_State *L)
{
	SERIAL_HANDLE hSerialId = (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset	= (RwReal)g_pLuaState->GetNumberArgument(2);

	CNtlSobActor *pActor	= reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hSerialId) );
	if(pActor)
	{
		RwV3d vLookAt	= pActor->GetPosition();
		RwReal fHeight	= pActor->GetSobProxy()->GetPLEntityHeight();

		vLookAt.y += fHeight*fHeightOffset;

		GetLuaGlueStack()->SetCameraLookAt(vLookAt);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       지정된 actor의 위치를 구한 다음, actor의 위에 회전한 다음 거리를 계산한여, 
//		 최종 camera 위치를 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset
//  paramater3 : actor 가 바라보는 방향에서 x 축으로 회전시킨다.
//  paramater4 : actor 가 바라보는 방향에서 y 축으로 회전시킨다.
//  paramater5 : actor 에서의 떨어진 distance
//
////////////////////////////////////////////////////////////////////////////


int LuaGlue_AddSoftCameraActorDistNode(lua_State *L)
{
	SERIAL_HANDLE hSerialId = (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fAngleX			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fAngleY			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDistance		= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(6);

	RwV3d vLookAt = GetLuaGlueStack()->GetCameraLookAt();

	GetNtlGameCameraManager()->AddDTSoftActorDistNode(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, vLookAt);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       지정된 actor의 위치를 구한 다음, actor의 위에 회전한 다음 거리를 계산한여, 
//		 최종 camera 위치를 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset
//  paramater3 : actor 가 바라보는 방향에서 x 축으로 회전시킨다.
//  paramater4 : actor 가 바라보는 방향에서 y 축으로 회전시킨다.
//  paramater5 : actor 에서의 떨어진 distance
//
////////////////////////////////////////////////////////////////////////////


int LuaGlue_AddSoftCameraActorDynamicLookAtDistNode(lua_State *L)
{
	SERIAL_HANDLE hSerialId = (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fAngleX			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fAngleY			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDistance		= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlGameCameraManager()->AddDTSoftActorDynamicLookAtDistNode(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       카메라 x 축 회전
//  ------------------------------------------------------------------------
//  paramater1 : last angle
//  paramater2 : life time
//
////////////////////////////////////////////////////////////////////////////


int LuaGlue_AddSoftCameraCurrentRotateX(lua_State *L)
{
	RwReal fLastAngle	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlGameCameraManager()->AddDTSoftCurrentRotateX(fLastAngle, fInterTime, fLifeTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       카메라 y 축 회전
//  ------------------------------------------------------------------------
//  paramater1 : last angle
//  paramater2 : life time
//
////////////////////////////////////////////////////////////////////////////


int LuaGlue_AddSoftCameraCurrentRotateY(lua_State *L)
{
	RwReal fLastAngle	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlGameCameraManager()->AddDTSoftCurrentRotateY(fLastAngle, fInterTime, fLifeTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       카메라 z 축 회전
//  ------------------------------------------------------------------------
//  paramater1 : last angle
//  paramater2 : life time
//
////////////////////////////////////////////////////////////////////////////


int LuaGlue_AddSoftCameraCurrentRotateZ(lua_State *L)
{
	RwReal fLastAngle	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlGameCameraManager()->AddDTSoftCurrentRotateZ(fLastAngle, fInterTime, fLifeTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : soft direct node를 추가한다.
//       fov 변화.
//  ------------------------------------------------------------------------
//  paramater1 : last fov
//  paramater2 : 보간 time
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSoftCameraCurrentFov(lua_State *L)
{
	RwReal fLastFov		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlGameCameraManager()->AddDTSoftCurrentFov(fLastFov, fInterTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : soft direct pause node를 추가한다.
//  ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCameraPauseNode(lua_State *L)
{
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(1);

	GetNtlGameCameraManager()->AddDTSoftPauseNode(fLifeTime, FALSE);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : camera 현재 위치에서 camera actor를 바라보면서 spline data를 적용한다.
//		 spline data의 방향은 사용하지 않는다.
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : actor height offset
//  paramater3 : spline property key
//  paramater4 : life time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSplineCurrentDynamicLookAtNode(lua_State *L)
{
	SERIAL_HANDLE hLookAtSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset			= (RwReal)g_pLuaState->GetNumberArgument(2);
	const char *pKey				= g_pLuaState->GetStringArgument(3);
	RwReal fLifeTime				= (RwReal)g_pLuaState->GetNumberArgument(4);
	
	GetNtlGameCameraManager()->AddDTSplineCurrentDynamicLookAtNode(hLookAtSerialId, fHeightOffset, pKey, fLifeTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : Skill position actor의 현재 위치에서 look at actor를 바라보면서 spline data를 적용한다.
//		 spline data의 방향은 사용하지 않는다.
//  ------------------------------------------------------------------------
//  paramater1 : position actor serial id
//  paramater1 : look at actor serial id
//  paramater1 : actor height offset
//  paramater2 : spline property key
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSkillSplineDynamicLookAtNode(lua_State *L)
{
	SERIAL_HANDLE hPosSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	SERIAL_HANDLE hLookAtSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(2);
	RwReal fHeightOffset			= (RwReal)g_pLuaState->GetNumberArgument(3);
	const char *pKey				= g_pLuaState->GetStringArgument(4);
		
	GetNtlGameCameraManager()->AddDTSplineActorPositionDynamicLookAtNode(hPosSerialId, hLookAtSerialId, fHeightOffset, pKey, 0.0f);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : Skill position actor의 현재 위치에서 pos actor 와 look at actor의 center 위치를
//       바라보면서 spline data를 적용한다.
//		 spline data의 방향은 사용하지 않는다.
//  ------------------------------------------------------------------------
//  paramater1 : position actor serial id
//  paramater1 : look at actor serial id
//  paramater1 : actor height offset
//  paramater2 : spline property key
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSkillSplineDynamicLookAtCenterNode(lua_State *L)
{
	SERIAL_HANDLE hPosSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	SERIAL_HANDLE hLookAtSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(2);
	RwReal fHeightOffset			= (RwReal)g_pLuaState->GetNumberArgument(3);
	const char *pKey				= g_pLuaState->GetStringArgument(4);
		
	GetNtlGameCameraManager()->AddDTSplineActorPositionDyncmicLookAtCenterNode(hPosSerialId, hLookAtSerialId, fHeightOffset, pKey, 0.0f);

	return 0;
}

//  ------------------------------------------------------------------------
//  paramater1 : position actor serial id
//  paramater1 : look at actor serial id
//  paramater1 : actor height offset
//  paramater2 : spline property key
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSkillSplineStaticPosDynamicLookAtNode(lua_State *L)
{
	SERIAL_HANDLE hPosSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	SERIAL_HANDLE hLookAtSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(2);
	RwReal fHeightOffset			= (RwReal)g_pLuaState->GetNumberArgument(3);
	const char *pKey				= g_pLuaState->GetStringArgument(4);

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hPosSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrint("LuaGlue_AddSkillSplineStaticPosDynamicLookAtNode is handle invalid !!!");
		return NTL_LUA_GLUE_RESULT_NO;
	}

	RwV3d vStartPos = pSobObj->GetPosition();
	RwV3d vDir		= pSobObj->GetDirection();

	RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);
		
	GetNtlGameCameraManager()->AddDTSplinePositionDynamicLookAtNode(vStartPos, fAngleY, hLookAtSerialId, fHeightOffset, pKey, 0.0f);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : camera의 look at은 spline data에 의해 결정되고, 전혀 transform 시키지 않는다. 
//		 
//  ------------------------------------------------------------------------
//  paramater1 : start position x
//  paramater2 : start position y
//  paramater3 : start position z
//  paramater4 : direct property key name
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSplineCameraSelfNode(lua_State *L)
{
	RwV3d vStartPos, vEndPos, vLookAtPos ;

	CNtlMath::MathRwV3dAssign(&vEndPos, 0.0f, 0.0f, 0.0f);
	CNtlMath::MathRwV3dAssign(&vLookAtPos, 0.0f, 0.0f, 0.0f);

	vStartPos.x			= (RwReal)g_pLuaState->GetNumberArgument(1);
	vStartPos.y			= (RwReal)g_pLuaState->GetNumberArgument(2);
	vStartPos.z			= (RwReal)g_pLuaState->GetNumberArgument(3);
	const char *pKey	= g_pLuaState->GetStringArgument(4);

	GetNtlGameCameraManager()->AddDTSplineSelfNode(vStartPos, 0.0f, 0.0f, 0.0f, pKey);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : camera의 look at은 spline data에 의해 결정되고, skill 공격 actor의 현재 좌표만 참조한다.
//		 
//  ------------------------------------------------------------------------
//  paramater1 : spline name 
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSkillAttackerSplineCameraSelfNode(lua_State *L)
{
	const char *pKey	= g_pLuaState->GetStringArgument(1);
	
	SERIAL_HANDLE hSerialId = GetLuaGlueStack()->GetHTBAttackSerialId();

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrint("LuaGlue_AddSkillAttackerSplineCameraSelfNode is handle invalid !!!");
		return NTL_LUA_GLUE_RESULT_NO;
	}
	
	GetNtlGameCameraManager()->AddDTSplineSelfNodeBeginActor(hSerialId, pKey);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : camera의 look at은 spline data에 의해 결정되고, skill target actor의 현재 좌표만 참조한다.
//		 
//  ------------------------------------------------------------------------
//  paramater1 : spline name 
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSkillTargetSplineCameraSelfNode(lua_State *L)
{
	const char *pKey	= g_pLuaState->GetStringArgument(1);
	
	SERIAL_HANDLE hSerialId = GetLuaGlueStack()->GetHTBTargetSerialId();

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrint("LuaGlue_AddSkillTargetSplineCameraSelfNode is handle invalid !!!");
		return NTL_LUA_GLUE_RESULT_NO;
	}


	GetNtlGameCameraManager()->AddDTSplineSelfNodeBeginActor(hSerialId, pKey);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : camera의 look at은 spline data에 의해 결정되고, 
//       node가 실행되는 시점에 actor의 좌표 및 방향을 참조한다.
//		 
//  ------------------------------------------------------------------------
//  paramater1 : actor serial id
//  paramater2 : spline name 
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddSkillSplineActorRefSelfNode(lua_State *L)
{
	SERIAL_HANDLE hSerialId			= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	const char *pKey				= g_pLuaState->GetStringArgument(2);

	GetNtlGameCameraManager()->AddDTSplineSelfNodeActor(hSerialId, pKey);

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// lua : Logic => actor
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 owner serial id를 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : serial id(number)
////////////////////////////////////////////////////////////////////////////

int LuaGlue_GetOwnerSerialId(lua_State *L)
{
	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(!pSobObj)
	{
		NtlLogFilePrintArgs("LuaGlue_GetOwnerSerialId - serial id is invalid(%d)", uiSerialId);
		g_pLuaState->PushNumber(INVALID_SERIAL_ID);
		return NTL_LUA_GLUE_RESULT_YES;
	}

	g_pLuaState->PushNumber(pSobObj->GetOwnerID());

	return NTL_LUA_GLUE_RESULT_YES; 
}

////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 target serial id를 구한다.
//  ------------------------------------------------------------------------
//  paramater1 : serial id(number)
////////////////////////////////////////////////////////////////////////////
int LuaGlue_GetActorType(lua_State *L)
{
	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(!pSobObj)
	{
		NtlLogFilePrintArgs("LuaGlue_GetActorType - serial id is invalid(%d)", uiSerialId);
		g_pLuaState->PushNumber(INVALID_SERIAL_ID);
		return NTL_LUA_GLUE_RESULT_YES;
	}

	g_pLuaState->PushNumber(pSobObj->GetClassID());

	return NTL_LUA_GLUE_RESULT_YES; 
}

////////////////////////////////////////////////////////////////////////////
// des : serial에 해당하는 actor가 avatar인가?
//  ------------------------------------------------------------------------
//  paramater1 : serial id(number)
////////////////////////////////////////////////////////////////////////////
int LuaGlue_IsAvatarActor(lua_State *L)
{
	RwUInt32 uiSerialId = (RwUInt32)g_pLuaState->GetNumberArgument(1);
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		NtlLogFilePrintArgs("LuaGlue_IsAvatarActor - serial id is invalid(%d)", uiSerialId);
		g_pLuaState->PushNumber(0);
		return NTL_LUA_GLUE_RESULT_YES; 
	}

	if(pSobObj->GetClassID() == SLCLASS_AVATAR)
		g_pLuaState->PushNumber(1);
	else
		g_pLuaState->PushNumber(0);

	return NTL_LUA_GLUE_RESULT_YES; 
}

////////////////////////////////////////////////////////////////////////////
// lua : Logic => Skill camera control 속성에 관련된 함수.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// des : skill 공격자 serial id를 구한다.
// ------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////

int LuaGlue_GetSkillAttackSerialId(lua_State *L)
{
	SERIAL_HANDLE hAttackSerialId = GetLuaGlueStack()->GetHTBAttackSerialId();
	g_pLuaState->PushNumber(hAttackSerialId);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// des : skill 공격자 target id를 구한다.
// ------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////

int LuaGlue_GetSkillTargetSerialId(lua_State *L)
{
	SERIAL_HANDLE hTargetSerialId = GetLuaGlueStack()->GetHTBTargetSerialId();
	g_pLuaState->PushNumber(hTargetSerialId);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// lua : Logic => day and night
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// des : day and night control node를 추가한다.
//       start value와 finish value를 같이 넣어준다.
// ------------------------------------------------------------------------
// paramater1 : 처음 delay time.
// paramater2 : duration time
// paramater3 : start control value
// paramater4 : finish control value
// paramater5 : blend factor
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddDNNode(lua_State *L)
{
	RwReal fBeginTime = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fDurationTime = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwUInt32 uiStartValue = (RwUInt32)g_pLuaState->GetNumberArgument(3);
	RwUInt32 uiFinishValue = (RwUInt32)g_pLuaState->GetNumberArgument(4);
	RwReal fBlendFactor = (RwReal)g_pLuaState->GetNumberArgument(5);

	GetDnController()->AddDNNode(fBeginTime, fDurationTime, uiStartValue, uiFinishValue, fBlendFactor);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : day and night control node를 추가한다.
//       start value는 현재의 값으로 하고 finish value만 넣어준다.
// ------------------------------------------------------------------------
// paramater1 : 처음 delay time.
// paramater2 : duration time
// paramater4 : finish control value
// paramater5 : blend factor
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddDNNodeCurrentStart(lua_State *L)
{
	RwReal fBeginTime = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fDurationTime = (RwReal)g_pLuaState->GetNumberArgument(2);
	RwUInt32 uiFinishValue = (RwUInt32)g_pLuaState->GetNumberArgument(3);
	RwReal fBlendFactor = (RwReal)g_pLuaState->GetNumberArgument(4);

	GetDnController()->AddDNNodeCurrentStart(fBeginTime, fDurationTime, uiFinishValue, fBlendFactor);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic node 추가하기(begin)
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeginCinematic(lua_State *L)
{
	RwReal fStandardPosX = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fStandardPosZ = (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandBegin(fStandardPosX, fStandardPosZ);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic node 추가하기(end)
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_EndCinematic(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandEnd();

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : cinematic begin layer 생성하기
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeignCinematicLayer(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandBeginLayer();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic end layer 생성하기
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_EndCinematicLayer(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandEndLayer();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic begin group 생성하기
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeignCinematicGroup(lua_State *L)
{
	GetNtlDTCinematicManager()->CommnadBeginGroup();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic begin group 생성하기
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_EndCinematicGroup(lua_State *L)
{
	GetNtlDTCinematicManager()->CommnadEndGroup();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic node 병렬로 추가하기(begin)
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeginCinematicSibling(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandSiblingBegin();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic node 병렬로 추가하기(end)
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_EndCinematicSibling(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandSiblingEnd();

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : cinematic camera node sync를 시키지 않는다.
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeginCinematicCameraAsync(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandCameraAsyncBegin();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic camera node sync 시킨다.
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_EndCinematicCameraAsync(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandCameraAsyncEnd();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic camera node lock 시킨다.
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_BeginCinematicCameraLock(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandCameraLockBegin();

	return NTL_LUA_GLUE_RESULT_NO;
}


////////////////////////////////////////////////////////////////////////////
// des : cinematic camera node unlock 시킨다.
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////
int LuaGlue_EndCinematicCameraLock(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandCameraLockEnd();

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic view show
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNViewShow(lua_State *L)
{
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(1);

	GetNtlDTCinematicManager()->CommandViewShowHide(TRUE, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic view hide
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNViewHide(lua_State *L)
{
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(1);

	GetNtlDTCinematicManager()->CommandViewShowHide(FALSE, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic balloon
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNBalloon(lua_State *L)
{
	SERIAL_HANDLE hSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiQuestTextTblId	= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byBalloonType		= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byEmotionType		= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal	fLifeTime			= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal	fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlDTCinematicManager()->CommandBalloon(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic balloon begin
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNBalloonBegin(lua_State *L)
{
	SERIAL_HANDLE hSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiQuestTextTblId	= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byBalloonType		= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byEmotionType		= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal	fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandBalloonBegin(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic balloon progress
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNBalloonProgress(lua_State *L)
{
	SERIAL_HANDLE hSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiQuestTextTblId	= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byBalloonType		= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byEmotionType		= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal	fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandBalloonProgress(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic balloon end
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNBalloonEnd(lua_State *L)
{
	SERIAL_HANDLE hSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiQuestTextTblId	= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byBalloonType		= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byEmotionType		= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal	fLifeTime			= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal	fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlDTCinematicManager()->CommandBalloonEnd(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic balloon single
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNBalloonSingle(lua_State *L)
{
	SERIAL_HANDLE hSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiQuestTextTblId	= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byBalloonType		= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byEmotionType		= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal	fLifeTime			= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal	fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlDTCinematicManager()->CommandBalloonSingle(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : cinematic balloon
// ------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

int LuaGlue_CNFlash(lua_State *L)
{
	const char *pFileName		= g_pLuaState->GetStringArgument(1);
	RwBool bLoop				= (RwBool)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(4);

	GetNtlDTCinematicManager()->CommandFlash(pFileName, bLoop, fLifeTime, fDelayTime);

	return NTL_LUA_GLUE_RESULT_NO;
}

////////////////////////////////////////////////////////////////////////////
// des : monster을 생성하는 cinematic node를 추가한다.
//       리턴값으로 simulation object의 serial을 넘겨준다.
// ------------------------------------------------------------------------
// paramater1 : monster table id.
// paramater2 : create position x
// paramater3 : create position y
// paramater4 : create position z
// paramater5 : create angle y
// paramater6 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNMobCreate(lua_State *L)
{
	RwUInt32 uiTblId;
	RwV3d vLoc;
	RwReal fAngleY, fDelayTime;		

	uiTblId		= (RwUInt32)g_pLuaState->GetNumberArgument(1);
	vLoc.x		= (RwReal)g_pLuaState->GetNumberArgument(2);
	vLoc.y		= (RwReal)g_pLuaState->GetNumberArgument(3);
	vLoc.z		= (RwReal)g_pLuaState->GetNumberArgument(4);
	fAngleY		= (RwReal)g_pLuaState->GetNumberArgument(5);
	fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(6);

	SERIAL_HANDLE hSerialId = GetNtlDTCinematicManager()->CommandMobCreate(uiTblId, vLoc, fAngleY, TRUE, fDelayTime);

	g_pLuaState->PushNumber(hSerialId);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// des : npc을 생성하는 cinematic node를 추가한다.
//       리턴값으로 simulation object의 serial을 넘겨준다.
// ------------------------------------------------------------------------
// paramater1 : monster table id.
// paramater2 : create position x
// paramater3 : create position y
// paramater4 : create position z
// paramater5 : create angle y
// paramater6 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNNpcCreate(lua_State *L)
{
	RwUInt32 uiTblId;
	RwV3d vLoc;
	RwReal fAngleY, fDelayTime;		

	uiTblId		= (RwUInt32)g_pLuaState->GetNumberArgument(1);
	vLoc.x		= (RwReal)g_pLuaState->GetNumberArgument(2);
	vLoc.y		= (RwReal)g_pLuaState->GetNumberArgument(3);
	vLoc.z		= (RwReal)g_pLuaState->GetNumberArgument(4);
	fAngleY		= (RwReal)g_pLuaState->GetNumberArgument(5);
	fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(6);

	SERIAL_HANDLE hSerialId = GetNtlDTCinematicManager()->CommandNpcCreate(uiTblId, vLoc, fAngleY, TRUE, fDelayTime);

	g_pLuaState->PushNumber(hSerialId);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 삭제한다.
// ------------------------------------------------------------------------
// paramater1 : 삭제할 simulation object serial id.
// paramater2 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobDelete(lua_State *L)
{
	SERIAL_HANDLE hSerialId		= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fDelayTime			= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandSobDelete(hSerialId, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 이동시킨다.
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : 이동할 위치 x
// paramater3 : 이동할 위치 y
// paramater4 : 이동할 위치 z
// paramater5 : dealy time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobMove(lua_State *L)
{
	SERIAL_HANDLE hSerialId;
	RwV3d vLoc;
	RwReal fDelayTime;

	hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	vLoc.x		= (RwReal)g_pLuaState->GetNumberArgument(2);
	vLoc.y		= (RwReal)g_pLuaState->GetNumberArgument(3);
	vLoc.z		= (RwReal)g_pLuaState->GetNumberArgument(4);
	fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandSobMove(hSerialId, vLoc, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 spline line data에 따라 run 이동시킨다.
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : spline data start position - x
// paramater3 : spline data start position - y
// paramater4 : spline data start position - z
// paramater5 : spline data key
// paramater6 : dealy time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobSplineMove(lua_State *L)
{
	SERIAL_HANDLE hSerialId;
	RwV3d vLoc;
	RwReal fDelayTime;

	hSerialId				= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	vLoc.x					= (RwReal)g_pLuaState->GetNumberArgument(2);
	vLoc.y					= (RwReal)g_pLuaState->GetNumberArgument(3);
	vLoc.z					= (RwReal)g_pLuaState->GetNumberArgument(4);
	const char *pSplineName	= g_pLuaState->GetStringArgument(5);
	fDelayTime				= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlDTCinematicManager()->CommandSobSplineMove(hSerialId, vLoc, pSplineName, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 spline line data에 따라 walk 이동시킨다. 
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : spline data start position - x
// paramater3 : spline data start position - y
// paramater4 : spline data start position - z
// paramater5 : spline data key
// paramater6 : dealy time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobSplineWalkMove(lua_State *L)
{
	SERIAL_HANDLE hSerialId;
	RwV3d vLoc;
	RwReal fDelayTime;

	hSerialId				= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	vLoc.x					= (RwReal)g_pLuaState->GetNumberArgument(2);
	vLoc.y					= (RwReal)g_pLuaState->GetNumberArgument(3);
	vLoc.z					= (RwReal)g_pLuaState->GetNumberArgument(4);
	const char *pSplineName	= g_pLuaState->GetStringArgument(5);
	fDelayTime				= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlDTCinematicManager()->CommandSobSplineWalkMove(hSerialId, vLoc, pSplineName, fDelayTime);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : simulation object를 spline line data에 따라 이동시킨다. (world height를 check 하지 않는다.)
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : spline data start position - x
// paramater3 : spline data start position - y
// paramater4 : spline data start position - z
// paramater5 : spline data key
// paramater6 : dealy time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobSplineFlyMove(lua_State *L)
{
	SERIAL_HANDLE hSerialId;
	RwV3d vLoc;
	RwReal fDelayTime;

	hSerialId				= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	vLoc.x					= (RwReal)g_pLuaState->GetNumberArgument(2);
	vLoc.y					= (RwReal)g_pLuaState->GetNumberArgument(3);
	vLoc.z					= (RwReal)g_pLuaState->GetNumberArgument(4);
	const char *pSplineName	= g_pLuaState->GetStringArgument(5);
	fDelayTime				= (RwReal)g_pLuaState->GetNumberArgument(6);

	GetNtlDTCinematicManager()->CommandSobSplineFlyMove(hSerialId, vLoc, pSplineName, fDelayTime);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : simulation object를 spline line data에 따라 이동시킨다.
// ------------------------------------------------------------------------
// paramater1 : simulation trigger object table id.
// paramater2 : main state
// paramater3 : sub state
// paramater4 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobTriggerObjectState(lua_State *L)
{
	RwUInt32 uiTblId	= (RwUInt32)g_pLuaState->GetNumberArgument(1);
	RwUInt8	byMainState = (RwUInt8)g_pLuaState->GetNumberArgument(2);
	RwUInt8	bySubState	= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(4);

	GetNtlDTCinematicManager()->CommandSobTriggerObjectState(uiTblId, byMainState, bySubState, fDelayTime);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : simulation object를 direct play
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : table id
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobDirectPlay(lua_State *L)
{
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiTblId		= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwBool bTimeLoop		= (RwBool)g_pLuaState->GetNumberArgument(3);
	RwReal fLoopTime		= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandSobDirectPlay(hSerialId, uiTblId, bTimeLoop, fLoopTime, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 anim play
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : animation id
// paramater3 : animation loop flag
// paramater4 : animation loop time
// paramater5 : dealy time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobAnimPlay(lua_State *L)
{
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiAnimd		= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwBool bTimeLoop		= (RwBool)g_pLuaState->GetNumberArgument(3);
	RwReal fLoopTime		= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandSobAnimPlay(hSerialId, uiAnimd, bTimeLoop, fLoopTime, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 anim play
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : animation id
// paramater3 : animation loop flag
// paramater4 : animation loop time
// paramater5 : dealy time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobAnimPlayPositionSetting(lua_State *L)
{
	RwV3d vLoc;

	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwUInt32 uiAnimd		= (RwUInt32)g_pLuaState->GetNumberArgument(2);
	RwBool bTimeLoop		= (RwBool)g_pLuaState->GetNumberArgument(3);
	RwReal fLoopTime		= (RwReal)g_pLuaState->GetNumberArgument(4);
	vLoc.x					= (RwReal)g_pLuaState->GetNumberArgument(5);
	vLoc.y					= (RwReal)g_pLuaState->GetNumberArgument(6);
	vLoc.z					= (RwReal)g_pLuaState->GetNumberArgument(7);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(8);

	GetNtlDTCinematicManager()->CommandSobAnimPlayPositionSetting(hSerialId, uiAnimd, bTimeLoop, fLoopTime, vLoc, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 position setting
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : position x
// paramater3 : position y
// paramater4 : position z
// paramater5 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobPositionSetting(lua_State *L)
{
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fPosX			= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fPosY			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fPosZ			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(5);

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, fPosX, fPosY, fPosZ);

	GetNtlDTCinematicManager()->CommandSobPositionSetting(hSerialId, vPos, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 direct setting
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : direct x
// paramater3 : direct y
// paramater4 : direct z
// paramater5 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobDirectSetting(lua_State *L)
{
	RwV3d vDir;
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	vDir.x					= (RwReal)g_pLuaState->GetNumberArgument(2);
	vDir.y					= (RwReal)g_pLuaState->GetNumberArgument(3);
	vDir.z					= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(5);

	RwV3dNormalize(&vDir, &vDir);

	GetNtlDTCinematicManager()->CommandSobDirectSetting(hSerialId, vDir, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : simulation object를 rotate y setting
// ------------------------------------------------------------------------
// paramater1 : simulation object serial id.
// paramater2 : rotate y angle
// paramater5 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNSobRotateY(lua_State *L)
{
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fAngleY			= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlDTCinematicManager()->CommandSobRotateY(hSerialId, fAngleY, fDelayTime);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : time delay.
// ------------------------------------------------------------------------
// paramater1 : life time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNPause(lua_State *L)
{
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(1);

	GetNtlDTCinematicManager()->CommandPause(fLifeTime);

	return 0;
}


////////////////////////////////////////////////////////////////////////////
// des : visual effect actor bone attach.
// ------------------------------------------------------------------------
// paramater1 : effect key
// paramater2 : actor serial id
// paramater3 : bone name
// paramater4 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNEffectActorBone(lua_State *L)
{
	const char *pKey		= g_pLuaState->GetStringArgument(1);
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(2);
	const char *pBoneName	= g_pLuaState->GetStringArgument(3);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(4);

	SERIAL_HANDLE hInstance = GetNtlDTCinematicManager()->CommandVisualEffectCreate(pKey, hSerialId, pBoneName, fDelayTime);
	
	g_pLuaState->PushNumber(hInstance);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// des : visual effect actor offset attach.
// ------------------------------------------------------------------------
// paramater1 : effect key
// paramater2 : actor serial id
// paramater3 : actor offset y
// paramater4 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNEffectActorOffset(lua_State *L)
{
	RwV3d vOffset;
	CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, 0.0f);

	const char *pKey		= g_pLuaState->GetStringArgument(1);
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(2);
	vOffset.y				= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(4);

	SERIAL_HANDLE hInstance = GetNtlDTCinematicManager()->CommandVisualEffectCreate(pKey, hSerialId, vOffset, fDelayTime);
	
	g_pLuaState->PushNumber(hInstance);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// des : visual effect world position.
// ------------------------------------------------------------------------
// paramater1 : effect key
// paramater2 : world position x
// paramater3 : world position y
// paramater4 : world position z
// paramater5 : delay time
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNEffectWorld(lua_State *L)
{
	RwV3d vWorld;

	const char *pKey		= g_pLuaState->GetStringArgument(1);
	vWorld.x				= (RwReal)g_pLuaState->GetNumberArgument(2);
	vWorld.y				= (RwReal)g_pLuaState->GetNumberArgument(3);
	vWorld.z				= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(5);

	SERIAL_HANDLE hInstance = GetNtlDTCinematicManager()->CommandVisualEffectCreate(pKey, vWorld, fDelayTime);

	g_pLuaState->PushNumber(hInstance);

	return 1;
}

////////////////////////////////////////////////////////////////////////////
// des : visual effect delete.
// ------------------------------------------------------------------------
// paramater1 : visual effect instance
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNEffectDelete(lua_State *L)
{
	SERIAL_HANDLE hInstance	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);

	GetNtlDTCinematicManager()->CommandVisualEffectDelete(hInstance);

	return 0;
}

////////////////////////////////////////////////////////////////////////////
// des : visual effect delete.
// ------------------------------------------------------------------------
// paramater1 : visual effect instance
////////////////////////////////////////////////////////////////////////////

int LuaGlue_AddCNCameraSoftActorDist(lua_State *L)
{
	SERIAL_HANDLE hSerialId = (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fAngleX			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fAngleY			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDistance		= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(6);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(7);

	RwV3d vLookAt			= GetLuaGlueStack()->GetCameraLookAt();

	GetNtlDTCinematicManager()->CommandCameraSoftActorDist(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, vLookAt, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSoftActorDyncLookAtDist(lua_State *L)
{
	SERIAL_HANDLE hSerialId = (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fAngleX			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fAngleY			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDistance		= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(6);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(7);

	GetNtlDTCinematicManager()->CommandCameraSoftActorDynamicLookAtDist(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSoftCurrRotX(lua_State *L)
{
	RwReal fLastAngle	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(4);

	GetNtlDTCinematicManager()->CommandCameraSoftCurrentRotateX(fLastAngle, fInterTime, fLifeTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSoftCurrRotY(lua_State *L)
{
	RwReal fLastAngle	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(4);

	GetNtlDTCinematicManager()->CommandCameraSoftCurrentRotateY(fLastAngle, fInterTime, fLifeTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSoftCurrRotZ(lua_State *L)
{
	RwReal fLastAngle	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(4);

	GetNtlDTCinematicManager()->CommandCameraSoftCurrentRotateZ(fLastAngle, fInterTime, fLifeTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSoftCurrFov(lua_State *L)
{
	RwReal fLastFov		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fInterTime	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlDTCinematicManager()->CommandCameraSoftCurrentFov(fLastFov, fInterTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSoftPause(lua_State *L)
{
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandCameraSoftPause(fLifeTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSplineSelf(lua_State *L)
{
	RwV3d	vPos;
	
	vPos.x				= (RwReal)g_pLuaState->GetNumberArgument(1);
	vPos.y				= (RwReal)g_pLuaState->GetNumberArgument(2);
	vPos.z				= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fAngleX		= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fAngleY		= (RwReal)g_pLuaState->GetNumberArgument(5);
	RwReal fAngleZ		= (RwReal)g_pLuaState->GetNumberArgument(6);
	const char *pKey	= g_pLuaState->GetStringArgument(7);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(8);
	
	GetNtlDTCinematicManager()->CommandCameraSplineSelf(vPos, fAngleX, fAngleY, fAngleZ, pKey, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraSplineSelfActor(lua_State *L)
{
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightRate	 	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fAngleX			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fAngleY			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fAngleZ			= (RwReal)g_pLuaState->GetNumberArgument(5);
	const char *pKey		= g_pLuaState->GetStringArgument(6);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(7);
	
	GetNtlDTCinematicManager()->CommandCameraSplineSelfActor(hSerialId, fHeightRate, fAngleX, fAngleY, fAngleZ, pKey, fDelayTime);

	return 0;
}


int LuaGlue_AddCNCameraSplineCurrDyncLookAt(lua_State *L)
{
	SERIAL_HANDLE hLookAtSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightOffset			= (RwReal)g_pLuaState->GetNumberArgument(2);
	const char *pKey				= g_pLuaState->GetStringArgument(3);
	RwReal fLifeTime				= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime				= (RwReal)g_pLuaState->GetNumberArgument(5);
	
	GetNtlDTCinematicManager()->CommandCameraSplineCurrentDynamicLookAt(hLookAtSerialId, fHeightOffset, pKey, fLifeTime, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraShake(lua_State *L)
{
	RwReal fWeightValue = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightValue	= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fDelayTime	= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlDTCinematicManager()->CommandCameraShake(fWeightValue, fHeightValue, fDelayTime);

	return 0;
}

int LuaGlue_AddCNCameraShakeFactor(lua_State *L)
{
	RwReal fWeightValue = (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fHeightValue	= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandCameraShakeFactor(fWeightValue, fHeightValue);

	return 0;
}

int LuaGlue_AddCNCameraShakeFactorReset(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandCameraShakeFactorReset();

	return 0;
}


int LuaGlue_AddCNFadeIn(lua_State *L)
{
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwUInt8 byRed			= (RwUInt8)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byGreen			= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byBlue			= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwUInt8)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandFadeIn(fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	return 0;
}

int LuaGlue_AddCNFadeOut(lua_State *L)
{
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwUInt8 byRed			= (RwUInt8)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byGreen			= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byBlue			= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwUInt8)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandFadeOut(fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	return 0;
}

int LuaGlue_AddCNFadeInTVH(lua_State *L)
{
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwUInt8 byRed			= (RwUInt8)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byGreen			= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byBlue			= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwUInt8)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandFadeInTVH(fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	return 0;
}

int LuaGlue_AddCNFadeOutTVH(lua_State *L)
{
	RwReal fLifeTime		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwUInt8 byRed			= (RwUInt8)g_pLuaState->GetNumberArgument(2);
	RwUInt8 byGreen			= (RwUInt8)g_pLuaState->GetNumberArgument(3);
	RwUInt8 byBlue			= (RwUInt8)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwUInt8)g_pLuaState->GetNumberArgument(5);

	GetNtlDTCinematicManager()->CommandFadeOutTVH(fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	return 0;
}

int LuaGlue_AddCNSound2D(lua_State *L)
{
	const char *pFileName	= g_pLuaState->GetStringArgument(1);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommnadSound2D(pFileName, fDelayTime);

	return 0;
}

int LuaGlue_AddCNSound3D(lua_State *L)
{
	const char *pFileName	= g_pLuaState->GetStringArgument(1);
	RwReal fPosX			= (RwReal)g_pLuaState->GetNumberArgument(2);
	RwReal fPosY			= (RwReal)g_pLuaState->GetNumberArgument(3);
	RwReal fPosZ			= (RwReal)g_pLuaState->GetNumberArgument(4);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(5);

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, fPosX, fPosY, fPosZ);

	GetNtlDTCinematicManager()->CommandSound3D(pFileName, vPos, fDelayTime);

	return 0;
}

int LuaGlue_AddCNSoundBGM(lua_State *L)
{
	const char *pFileName	= g_pLuaState->GetStringArgument(1);
	RwBool bLoop			= (RwBool)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommnadSoundBGM(pFileName, bLoop);

	return 0;
}

int LuaGlue_AddCNSoundBGMStop(lua_State *L)
{
	GetNtlDTCinematicManager()->CommnadSoundBGMStop();

	return 0;
}

int LuaGlue_AddCNCharacterShadowOnOff(lua_State *L)
{
	RwBool bOnOff			= (RwBool)g_pLuaState->GetNumberArgument(1);

	GetNtlDTCinematicManager()->CommandCharacterShadowOnOff(bOnOff);

	return 0;
}

int LuaGlue_AddCNCharacterVisibleOnOff(lua_State *L)
{
	SERIAL_HANDLE hSerialId	= (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	RwBool bOnOff			= (RwBool)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandCharacterShowOnOff(hSerialId, bOnOff);

	return 0;
}

int LuaGlue_AddCNSceneMoonVisibleOnOff(lua_State *L)
{
	RwBool bOnOff			= (RwBool)g_pLuaState->GetNumberArgument(1);

	GetNtlDTCinematicManager()->CommandMoonOnOff(bOnOff);

	return 0;
}

int LuaGlue_AddCNSceneRainBloomLight(lua_State *L)
{
	RwReal fFadeInTime		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fFadeOutTime		= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandRainBloomLight(fFadeInTime, fFadeOutTime);

	return 0;
}

int LuaGlue_AddCNSceneRainSkyLight(lua_State *L)
{
	RwReal fFadeInTime		= (RwReal)g_pLuaState->GetNumberArgument(1);
	RwReal fFadeOutTime		= (RwReal)g_pLuaState->GetNumberArgument(2);

	GetNtlDTCinematicManager()->CommandRainSkyLight(fFadeInTime, fFadeOutTime);

	return 0;
}

int LuaGlue_AddCNGuiCommand(lua_State *L)
{
	const char *pKey		= (const char*)g_pLuaState->GetStringArgument(1);
	RwBool bShow			= (RwBool)g_pLuaState->GetNumberArgument(2);
	RwReal fDelayTime		= (RwReal)g_pLuaState->GetNumberArgument(3);

	GetNtlDTCinematicManager()->CommandGuiCmd(pKey, bShow, fDelayTime);

	return 0;
}


int LuaGlue_AddCNCinematicServerAck(lua_State *L)
{
	GetNtlDTCinematicManager()->CommandServerAck();

	return 0;
}


int LuaGlue_AddSinExplosionAlgorithm_XAxis(lua_State *L)
{
	RwUInt32 uiExcuteID	= (RwUInt32)g_pLuaState->GetNumberArgument( 1 );
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument( 2 );
	RwReal fFrequancy	= (RwReal)g_pLuaState->GetNumberArgument( 3 );
	RwReal fPhase		= (RwReal)g_pLuaState->GetNumberArgument( 4 );
	RwReal fAmp			= (RwReal)g_pLuaState->GetNumberArgument( 5 );
	RwReal fDescFactor	= (RwReal)g_pLuaState->GetNumberArgument( 6 );

	GetNtlGameCameraManager()->AddSinExplosionAlgorithm_XAxis( uiExcuteID, fLifeTime, fFrequancy, fPhase, fAmp, fDescFactor );

	return 0;
}


int LuaGlue_AddSinExplosionAlgorithm_YAxis(lua_State *L)
{
	RwUInt32 uiExcuteID	= (RwUInt32)g_pLuaState->GetNumberArgument( 1 );
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument( 2 );
	RwReal fFrequancy	= (RwReal)g_pLuaState->GetNumberArgument( 3 );
	RwReal fPhase		= (RwReal)g_pLuaState->GetNumberArgument( 4 );
	RwReal fAmp			= (RwReal)g_pLuaState->GetNumberArgument( 5 );
	RwReal fDescFactor	= (RwReal)g_pLuaState->GetNumberArgument( 6 );

	GetNtlGameCameraManager()->AddSinExplosionAlgorithm_YAxis( uiExcuteID, fLifeTime, fFrequancy, fPhase, fAmp, fDescFactor );

	return 0;
}


int LuaGlue_AddSinExplosionAlgorithm_ZAxis(lua_State *L)
{
	RwUInt32 uiExcuteID	= (RwUInt32)g_pLuaState->GetNumberArgument( 1 );
	RwReal fLifeTime	= (RwReal)g_pLuaState->GetNumberArgument( 2 );
	RwReal fFrequancy	= (RwReal)g_pLuaState->GetNumberArgument( 3 );
	RwReal fPhase		= (RwReal)g_pLuaState->GetNumberArgument( 4 );
	RwReal fAmp			= (RwReal)g_pLuaState->GetNumberArgument( 5 );
	RwReal fDescFactor	= (RwReal)g_pLuaState->GetNumberArgument( 6 );

	GetNtlGameCameraManager()->AddSinExplosionAlgorithm_ZAxis( uiExcuteID, fLifeTime, fFrequancy, fPhase, fAmp, fDescFactor );

	return 0;
}

int LuaGlue_AddCNCinematicScale(lua_State * L)
{
	SERIAL_HANDLE hSerialId = (SERIAL_HANDLE)g_pLuaState->GetNumberArgument(1);
	float fScale = (float)g_pLuaState->GetNumberArgument(2); // in percent
	float fScaleTime = (float)g_pLuaState->GetNumberArgument(3); // time
	float fDelayTime = (float)g_pLuaState->GetNumberArgument(4);

	GetNtlDTCinematicManager()->CommandSobScale(hSerialId, fScale, fScaleTime, fDelayTime);

	return 0;
}

int LuaGlue_DTAddController(lua_State *L)
{
	const char* pcControllerName = g_pLuaState->GetStringArgument( 1 );
	if(!pcControllerName)
		return 0;

	GetNtlSLCrowdManager()->AddPlayController(pcControllerName);

	return 0;
}

int LuaGlue_DTRemoveController(lua_State *L)
{
	const char* pcControllerName	= g_pLuaState->GetStringArgument( 1 );
	if(!pcControllerName)
		return 0;

	GetNtlSLCrowdManager()->RemovePlayController(pcControllerName);

	return 0;
}

int LuaGlue_GetRefreePos( lua_State* L ) 
{ 
    RwUInt32 hSerialID = GetNtlSLCrowdManager()->GetRefreeActor()->GetSerialID();
    g_pLuaState->PushNumber(hSerialID);

    return 1;
}