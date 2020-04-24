//***********************************************************************************
//
//	File		:	NtlCharacterState.cpp
//
//	Begin		:	2007-10-23
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlCharacter.h"
#include "Utils.h"

const char* DBO_PURE_MAJIN_MODEL_RESOURCE_NAME = "A_PURE_MAJIN";

//-----------------------------------------------------------------------------------
// BotData 초기화
//-----------------------------------------------------------------------------------
void InitBotData(sBOT_DATA & rsBotData)
{
	rsBotData.tblidx = INVALID_TBLIDX;
	rsBotData.worldID = INVALID_WORLDID;
	rsBotData.worldtblidx = INVALID_TBLIDX;
	ZeroMemory( &rsBotData.vCurLoc, sizeof(sVECTOR3) );
	ZeroMemory( &rsBotData.vCurDir, sizeof(sVECTOR3) );

	ZeroMemory( &rsBotData.vSpawnLoc, sizeof(sVECTOR3) );
	ZeroMemory( &rsBotData.vSpawnDir, sizeof(sVECTOR3) );
	rsBotData.bySpawnRange = 0;
	rsBotData.wSpawnTime = 0;
	rsBotData.bySpawnFuncFlag = 0;

	rsBotData.byMoveType = SPAWN_MOVE_UNKNOWN;
	rsBotData.byMoveRange = 0;
	rsBotData.byWanderRange = 0;

	rsBotData.actionpatternTblIdx = INVALID_TBLIDX;

	rsBotData.pathTblidx = INVALID_TBLIDX;

	rsBotData.sScriptData.Init();

	rsBotData.partyID = INVALID_PARTYID;
	rsBotData.bPartyLeader = false;
	ZeroMemory( &rsBotData.vPartyLeaderDistance, sizeof(sVECTOR3) );

	rsBotData.qwCharConditionFlag = 0;

	rsBotData.hTargetFixedExecuter = INVALID_HOBJECT;

	rsBotData.byImmortalMode = 0;

	rsBotData.sBotSubData.Init();
}


//-----------------------------------------------------------------------------------
// NpcData 초기화
//-----------------------------------------------------------------------------------
void InitNpcData(sNPC_DATA & rsNpcData)
{
	InitBotData( rsNpcData );
}


//-----------------------------------------------------------------------------------
// MobData 초기화
//-----------------------------------------------------------------------------------
void InitMobData(sMOB_DATA & rsMobData)
{
	InitBotData( rsMobData );

	rsMobData.spawnGroupId = INVALID_SPAWNGROUPID;
}


//-----------------------------------------------------------------------------------
// Copy character state data
//-----------------------------------------------------------------------------------
void CopyCharState( sCHARSTATE* pDest, const sCHARSTATE* pSrc )
{
	// State base

	memcpy( &pDest->sCharStateBase, &pSrc->sCharStateBase, sizeof( sCHARSTATE_BASE ) );


	// Detail state

	switch ( pSrc->sCharStateBase.byStateID )
	{
	case CHARSTATE_SPAWNING:
		{
			pDest->sCharStateDetail.sCharStateSpawning = pSrc->sCharStateDetail.sCharStateSpawning;
		}
		break;

	case CHARSTATE_DESPAWNING:
		{
			pDest->sCharStateDetail.sCharStateDespawning = pSrc->sCharStateDetail.sCharStateDespawning;
		}
		break;

	case CHARSTATE_STANDING:
		{
			pDest->sCharStateDetail.sCharStateStanding = pSrc->sCharStateDetail.sCharStateStanding;
		}
		break;

	case CHARSTATE_SITTING:
		{
			pDest->sCharStateDetail.sCharStateSitting = pSrc->sCharStateDetail.sCharStateSitting;
		}
		break;

	case CHARSTATE_FAINTING:
		{
			pDest->sCharStateDetail.sCharStateFainting = pSrc->sCharStateDetail.sCharStateFainting;
		}
		break;

	case CHARSTATE_CAMPING:
		{
			pDest->sCharStateDetail.sCharStateCamping = pSrc->sCharStateDetail.sCharStateCamping;
		}
		break;

	case CHARSTATE_LEAVING:
		{
			pDest->sCharStateDetail.sCharStateLeaving = pSrc->sCharStateDetail.sCharStateLeaving;
		}
		break;

	case CHARSTATE_MOVING:
		{
			pDest->sCharStateDetail.sCharStateMoving = pSrc->sCharStateDetail.sCharStateMoving;
		}
		break;

	case CHARSTATE_DESTMOVE:
		{
			pDest->sCharStateDetail.sCharStateDestMove.byMoveFlag = pSrc->sCharStateDetail.sCharStateDestMove.byMoveFlag;
			pDest->sCharStateDetail.sCharStateDestMove.bHaveSecondDestLoc = pSrc->sCharStateDetail.sCharStateDestMove.bHaveSecondDestLoc;
			pDest->sCharStateDetail.sCharStateDestMove.vSecondDestLoc = pSrc->sCharStateDetail.sCharStateDestMove.vSecondDestLoc;
			pDest->sCharStateDetail.sCharStateDestMove.byDestLocCount = pSrc->sCharStateDetail.sCharStateDestMove.byDestLocCount;
			::CopyMemory(pDest->sCharStateDetail.sCharStateDestMove.avDestLoc, pSrc->sCharStateDetail.sCharStateDestMove.avDestLoc, sizeof((pDest->sCharStateDetail.sCharStateDestMove.avDestLoc)[0]) * pDest->sCharStateDetail.sCharStateDestMove.byDestLocCount);
		}
		break;

	case CHARSTATE_FOLLOWING:
		{
			pDest->sCharStateDetail.sCharStateFollwing = pSrc->sCharStateDetail.sCharStateFollwing;
		}
		break;

	case CHARSTATE_FALLING:
		{
			pDest->sCharStateDetail.sCharStateFalling = pSrc->sCharStateDetail.sCharStateFalling;
		}
		break;

	case CHARSTATE_DASH_PASSIVE:
		{
			pDest->sCharStateDetail.sCharStateDashPassive = pSrc->sCharStateDetail.sCharStateDashPassive;
		}
		break;

	case CHARSTATE_TELEPORTING:
		{
			pDest->sCharStateDetail.sCharStateTeleporting = pSrc->sCharStateDetail.sCharStateTeleporting;
		}
		break;

	case CHARSTATE_SLIDING:
		{
			pDest->sCharStateDetail.sCharStateSliding = pSrc->sCharStateDetail.sCharStateSliding;
		}
		break;

	case CHARSTATE_KNOCKDOWN:
		{
			pDest->sCharStateDetail.sCharStateKnockdown = pSrc->sCharStateDetail.sCharStateKnockdown;
		}
		break;

	case CHARSTATE_FOCUSING:
		{
			pDest->sCharStateDetail.sCharStateFocusing = pSrc->sCharStateDetail.sCharStateFocusing;
		}
		break;

	case CHARSTATE_CASTING:
		{
			pDest->sCharStateDetail.sCharStateCasting = pSrc->sCharStateDetail.sCharStateCasting;
		}
		break;

	case CHARSTATE_SKILL_AFFECTING:
		{
			pDest->sCharStateDetail.sCharStateSkillAffecting = pSrc->sCharStateDetail.sCharStateSkillAffecting;
		}
		break;

	case CHARSTATE_KEEPING_EFFECT:
		{
			pDest->sCharStateDetail.sCharStateKeepingEffect = pSrc->sCharStateDetail.sCharStateKeepingEffect;
		}
		break;

	case CHARSTATE_CASTING_ITEM:
		{
			pDest->sCharStateDetail.sCharStateCastingItem = pSrc->sCharStateDetail.sCharStateCastingItem;
		}
		break;

	case CHARSTATE_STUNNED:
		{
			pDest->sCharStateDetail.sCharStateStunned = pSrc->sCharStateDetail.sCharStateStunned;
		}
		break;

	case CHARSTATE_SLEEPING:
		{
			pDest->sCharStateDetail.sCharStateSleeping = pSrc->sCharStateDetail.sCharStateSleeping;
		}
		break;

	case CHARSTATE_PARALYZED:
		{
			pDest->sCharStateDetail.sCharStateParalyzed = pSrc->sCharStateDetail.sCharStateParalyzed;
		}
		break;

	case CHARSTATE_HTB:
		{
			pDest->sCharStateDetail.sCharStateHTB.HTBId = pSrc->sCharStateDetail.sCharStateHTB.HTBId;
			pDest->sCharStateDetail.sCharStateHTB.hTarget = pSrc->sCharStateDetail.sCharStateHTB.hTarget;
			pDest->sCharStateDetail.sCharStateHTB.byStepCount = pSrc->sCharStateDetail.sCharStateHTB.byStepCount;
			pDest->sCharStateDetail.sCharStateHTB.byCurStep = pSrc->sCharStateDetail.sCharStateHTB.byCurStep;
			pDest->sCharStateDetail.sCharStateHTB.byResultCount = pSrc->sCharStateDetail.sCharStateHTB.byResultCount;
			for ( int i = 0; i < pSrc->sCharStateDetail.sCharStateHTB.byResultCount; ++i )
			{
				pDest->sCharStateDetail.sCharStateHTB.aHTBSkillResult[i] = pSrc->sCharStateDetail.sCharStateHTB.aHTBSkillResult[i];
			}
		}
		break;

	case CHARSTATE_SANDBAG:
		{
			pDest->sCharStateDetail.sCharStateSandBag = pSrc->sCharStateDetail.sCharStateSandBag;
		}
		break;

	case CHARSTATE_CHARGING:
		{
			pDest->sCharStateDetail.sCharStateCharging = pSrc->sCharStateDetail.sCharStateCharging;
		}
		break;

	case CHARSTATE_GUARD:
		{
		}
		break;

	case CHARSTATE_PRIVATESHOP:
		{
			pDest->sCharStateDetail.sCharStatePrivateShop = pSrc->sCharStateDetail.sCharStatePrivateShop;
		}
		break;

	case CHARSTATE_DIRECT_PLAY:
		{
			pDest->sCharStateDetail.sCharStateDirectPlay = pSrc->sCharStateDetail.sCharStateDirectPlay;
		}
		break;

	case CHARSTATE_OPERATING:
		{
			pDest->sCharStateDetail.sCharStateOperating = pSrc->sCharStateDetail.sCharStateOperating;
		}
		break;

	case CHARSTATE_RIDEON:
		{
			pDest->sCharStateDetail.sCharStateRideOn = pSrc->sCharStateDetail.sCharStateRideOn;
		}
		break;

		case CHARSTATE_TURNING:
		{
			pDest->sCharStateDetail.sCharStateTurning = pSrc->sCharStateDetail.sCharStateTurning;
		}
		break;

		case CHARSTATE_AIR_JUMP:
		{
			pDest->sCharStateDetail.sCharStateAirJumping = pSrc->sCharStateDetail.sCharStateAirJumping;
		}
		break;

		case CHARSTATE_AIR_DASH_ACCEL:
		{
			pDest->sCharStateDetail.sCharStateAirAccel = pSrc->sCharStateDetail.sCharStateAirAccel;
		}
		break;
	}
}


//-----------------------------------------------------------------------------------
// 전투력 계산
//-----------------------------------------------------------------------------------
DWORD Dbo_CalculatePowerLevel(
		WORD wLastPhysicalOffence, WORD wLastPhysicalDefence, WORD wLastEnergyOffence, WORD wLastEnergyDefence,
		WORD wLastAttackRate, WORD wLastDodgeRate, WORD wLastCurseSuccessRate, WORD wLastCurseTolerance,
		WORD wLastPhysicalCriticalRate, WORD wLastEnergyCriticalRate,
		WORD wAttackSpeedRate,
		WORD wLastMaxLp, WORD wLastMaxEp,
		BYTE byLevel, BYTE byMobGrade)
{
	float fB = (float)(wLastPhysicalOffence + wLastPhysicalDefence + wLastEnergyOffence + wLastEnergyDefence);
	float fC = (float)(wLastAttackRate + wLastDodgeRate + wLastCurseSuccessRate + wLastCurseTolerance);
	float fD = (float)(wLastPhysicalCriticalRate + wLastEnergyCriticalRate);
	float fAttackSpeedRate = (float)wAttackSpeedRate;
	float fMaxLp = (float)wLastMaxLp;
	float fMaxEp = (float)wLastMaxEp;
	float fLevel = (float)byLevel;

	float fPowerLevel = 0;

	fPowerLevel += fB * 1.5f;
	fPowerLevel += (fMaxLp + fMaxEp) * 0.2f;
	fPowerLevel += (fC + fD * 20.0f + (1300.0f - fAttackSpeedRate)) * 0.1f;
	fPowerLevel *= fLevel * 2.0f + powf(2.0f, byMobGrade);
	fPowerLevel *= 0.1f;

	if (1.0f > fPowerLevel)
	{
		fPowerLevel = 1.0f;
	}

	return (DWORD)fPowerLevel;
}

float Dbo_ConvertToAgentRadius(float fObjectRadius)
{
	if (fObjectRadius <= 0.3f)
	{
		return 0.3f;
	}
	else if (fObjectRadius <= 1.0f)
	{
		return 1.0f;
	}
	else if (fObjectRadius <= 2.0f)
	{
		return 2.0f;
	}
	
	return 3.0f;
}

float Dbo_GetTransformScale( eASPECTSTATE eAspect, BYTE byGrade ) 
{
    // The rate of change is hard-coded priority.
    if(eAspect == ASPECTSTATE_GREAT_NAMEK)
    {
        // NOTE : First, the current three times. Later it is changed according to the grade.
        return 3.0f * byGrade;
    }
    
    return 1.0f;
}

// 레벨에 따른 RP Ball 최대치를 반환한다.
// 현재 기획 ->  1~ 9 -> 1개
//			 -> 10~19 -> 2개
//			 ->     .
//			 ->     .
//			 ->     .  (레벨 10 마다 1개씩 늘어남. 최대치는 7)
const BYTE CalculateRPBallMaxByLevel( const BYTE byLevel )
{
	BYTE byNewRPBallMax( 1 );

	byNewRPBallMax += byLevel/NTL_CHAR_RP_BALL_INCREASE_LEVEL;
	if ( byNewRPBallMax>NTL_CHAR_RP_BALL_MAX ) byNewRPBallMax = NTL_CHAR_RP_BALL_MAX;
	
	return byNewRPBallMax;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD GetSafeRetAdd( DWORD dwMax, DWORD dwCur, DWORD dwAdd )
{
	DWORD dwMargin = dwMax - dwCur;	// 최대값에서 남은금액
	if ( dwMargin > dwAdd )	// 더할 돈을 더할 수 있다. 
	{
		return (dwCur + dwAdd);
	}
	else
	{
		return dwMax;
	}		
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int Dbo_IncreaseDboLp(int oldLp, int diff, int maxLP)
{
	int finallp = oldLp + diff;

	if (finallp > maxLP)
	{
		finallp = maxLP;
	}

	return finallp;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int Dbo_DecreaseDboLp(int oldLp, int diff, int maxLP)
{
	UNREFERENCED_PARAMETER(maxLP);

	int finalLp = oldLp - diff;

	if (finalLp < 0)
	{
		finalLp = 0;
	}

	return finalLp;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WORD Dbo_IncreaseDboEp(WORD oldEp, WORD diff, WORD maxEP)
{
	WORD finalEp = UnsignedSafeIncrease<WORD>(oldEp, diff);

	if (finalEp > maxEP)
	{
		finalEp = maxEP;
	}

	return finalEp;
}
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WORD Dbo_DecreaseDboEp(WORD oldEp, WORD diff, WORD maxEP)
{
	WORD finalEp = UnsignedSafeDecrease<WORD>(oldEp, diff);

	if (maxEP < finalEp)
	{
		finalEp = 0;
	}
	return finalEp;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int Dbo_IncreaseDboAp(int oldAp, int diff, int maxAP)
{
	int finalAp = oldAp + diff;

	if (diff <= 0)
	{
		if (oldAp < -diff)
			finalAp = 0;
	}
	else if (maxAP < finalAp)
	{
		finalAp = maxAP;
	}
	return finalAp;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int Dbo_DecreaseDboAp(int oldAp, int diff, int maxAP)
{
	int finalAp = oldAp - diff;

	if (diff <= 0)
	{
		if (maxAP < finalAp)
			finalAp = maxAP;
	}
	else if (oldAp < diff)
	{
		finalAp = 0;
	}
	return finalAp;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float Dbo_GetWeightRateInPercent(DWORD dwTotalWeight, DWORD dwWeightLimit)
{
	return (float)dwTotalWeight / (float)dwWeightLimit * 100.0f;
}

