//***********************************************************************************
//
//	File		:	ModelToolCharDataTable.h
//
//	Begin		:	2007-03-5
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hong Ho Dong   ( battery@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************
#pragma once

#include "GraphicDataTable.h"
#include <map>
#include <string>
#include "NtlSharedType.h"
#include "NtlVector.h"
class CNtlFileSerializer;

#define	NTL_MAX_SIZE_HIT		16
#define NTL_ST_CHAIN_SEQUENCE	1

const int NTL_MAX_SIZE_ANIM_COMMON			= (COMMON_ANIMATION_END - COMMON_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_BATTLE			= (BATTLE_ANIMATION_END - BATTLE_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_ATTACK			= (ATTACK_ANIMATION_END - ATTACK_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_SOCIAL			= (SOCIAL_ANIMATION_END - SOCIAL_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_SKILL			= (SKILL_ANIMATION_END - SKILL_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_HTB				= (HTB_ANIMATION_END - HTB_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_TRIGGER			= (PC_TRIGGER_ANIMATION_END - PC_TRIGGER_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_TRANSFORM		= (TRANSFORM_ANIMATION_END - TRANSFORM_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_VEHICLE_SRP1    = (VEHICLE_SRP1_ANIMATION_END - VEHICLE_SRP1_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_VEHICLE_SRP2    = (VEHICLE_SRP2_ANIMATION_END - VEHICLE_SRP2_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_ITEM_USE        = (ITEM_USE_ANIMATION_END - ITEM_USE_ANIMATION_START);
const int NTL_MAX_SIZE_ANIM_ALL = 1029;

const int NTL_ST_INDEX_ANIM_COMMON			= 0;
const int NTL_ST_INDEX_ANIM_BATTLE			= (NTL_ST_INDEX_ANIM_COMMON + NTL_MAX_SIZE_ANIM_COMMON);
const int NTL_ST_INDEX_ANIM_ATTACK			= (NTL_ST_INDEX_ANIM_BATTLE + NTL_MAX_SIZE_ANIM_BATTLE);

const int NTL_ST_INDEX_ANIM_ATTACK_DEF		= (NTL_ST_INDEX_ANIM_ATTACK);
const int NTL_ST_INDEX_ANIM_ATTACK_GLOVE	= (NTL_ST_INDEX_ANIM_ATTACK + ATTACK_ANIMATION_SET_NUM);
const int NTL_ST_INDEX_ANIM_ATTACK_GUN		= (NTL_ST_INDEX_ANIM_ATTACK + ATTACK_ANIMATION_SET_NUM*2);
const int NTL_ST_INDEX_ANIM_ATTACK_STAFF	= (NTL_ST_INDEX_ANIM_ATTACK + ATTACK_ANIMATION_SET_NUM*3);
const int NTL_ST_INDEX_ANIM_ATTACK_DUALGUN	= (NTL_ST_INDEX_ANIM_ATTACK + ATTACK_ANIMATION_SET_NUM*4);

const int NTL_ST_INDEX_ANIM_SOCIAL			= (NTL_ST_INDEX_ANIM_ATTACK + NTL_MAX_SIZE_ANIM_ATTACK);
const int NTL_ST_INDEX_ANIM_SKILL			= (NTL_ST_INDEX_ANIM_SOCIAL + NTL_MAX_SIZE_ANIM_SOCIAL);
const int NTL_ST_INDEX_ANIM_HTB				= (NTL_ST_INDEX_ANIM_SKILL  + NTL_MAX_SIZE_ANIM_SKILL);
const int NTL_ST_INDEX_ANIM_TRIGGER			= (NTL_ST_INDEX_ANIM_HTB + NTL_MAX_SIZE_ANIM_HTB);
const int NTL_ST_INDEX_ANIM_TRANSFORM		= (NTL_ST_INDEX_ANIM_TRIGGER + NTL_MAX_SIZE_ANIM_TRIGGER);
const int NTL_ST_INDEX_ANIM_VEHICLE_SRP1    = (NTL_ST_INDEX_ANIM_TRANSFORM + NTL_MAX_SIZE_ANIM_TRANSFORM);
const int NTL_ST_INDEX_ANIM_VEHICLE_SRP2    = (NTL_ST_INDEX_ANIM_VEHICLE_SRP1 + NTL_MAX_SIZE_ANIM_VEHICLE_SRP1);
const int NTL_ST_INDEX_ANIM_ITEM_USE        = (NTL_ST_INDEX_ANIM_VEHICLE_SRP2 + NTL_MAX_SIZE_ANIM_ITEM_USE);

const int NTL_ST_INDEX_ANIM_MODULE_SKILL = 1022;


struct sCHAR_DATA_INFO
{
	float	fDurationTime;			// Animation 전체 시간
	BYTE	byPushCount;			// Push 갯수
	BYTE	byKnockDownCount;		// Knock Down 갯수(현재 1개)
    bool    bKB2Push;               ///< 넉다운을 Push로 변경하는지 유무 
	bool	bDataLoaded;			// Data가 Load가 되는지

	BYTE	byHitCount;						// Hit의 갯수
	float	aHitTime[NTL_MAX_SIZE_HIT];		// Hit Time
    float   fSkillCancelTime;               ///< 스킬 캔슬 이벤트의 타이밍

	sCHAR_DATA_INFO() : fDurationTime(0), 
		                byPushCount(0), 
						byKnockDownCount(0),
						byHitCount(0),
                        bKB2Push(false),
                        fSkillCancelTime(0.0f),
						bDataLoaded(false) { memset(aHitTime, 0, sizeof(aHitTime)); }
};


struct sMODELTOOL_CHAR_TBLDAT
{
private:
	sCHAR_DATA_INFO	m_aCharTbldat[ NTL_MAX_SIZE_ANIM_ALL ];
public:
	CNtlVector		m_vMin;
	CNtlVector		m_vMax;
	float			m_fScale;

protected:
	int GetRemapAnimKeyID( int nAnimKeyID);
	
public:

	bool Add(int nAnimKeyID, float fDurationTime, BYTE byPushCount, BYTE byKnockDownCount, BYTE byHitCount, float *pHitTime, bool bKB2Push = false);
    bool AddSkillCancelTime(int nAnimKeyID, float fTime);
	
	sCHAR_DATA_INFO *GetChainAttack(BYTE byClass, BYTE byWeapon, BYTE byChainSequence);			//ChainAttack의 경우 얻는다.

	sCHAR_DATA_INFO *GetAnim(BYTE byWeapon, ANIMATIONID AnimKeyID); //AnimationKey를 넣으면 Data를 얻는다.(현재 PC의 스킬과 MOB에 관한 ANI만 가능, PC 무기에 따른 다른 애니는 추후 인터페이스 재작성)


public:

// Wrapping Interface

	float				GetAnimationLength(ANIMATIONID AnimKeyID);

	DWORD				GetChainAttackDurationTime(BYTE byClass, BYTE byWeapon, BYTE byChainSequence);
	BYTE				GetChainAttackPushCount(BYTE byClass, BYTE byWeapon, BYTE byChainSequence);
	bool				GetChainAttackKnockDown(BYTE byClass, BYTE byWeapon, BYTE byChainSequence);


	bool				GetSkillKnockDown(ANIMATIONID skillAnimationId);
    bool                GetSkillKB2Push(ANIMATIONID skillAnimationId);                              ///< 넉다운을 Push로 적용할지 유무를 반환
	float				GetSkillAnimationLength(ANIMATIONID skillAnimationId);
	BYTE				GetSkillAnimationPushCount(ANIMATIONID skillAnimationId);
	DWORD				GetSkillAnimationFirstHitTime(ANIMATIONID skillAnimationId);
	DWORD				GetSkillAnimationSkillCancelTime(ANIMATIONID skillAnimationId);

	DWORD				GetNpcAttackDurationTime(BYTE byAttackType);

};


class CModelToolCharDataTable
{
public:
	typedef	std::map<std::string, sMODELTOOL_CHAR_TBLDAT *> TABLE;
	typedef TABLE::iterator TABLEIT;
	typedef TABLE::value_type TABLEVAL;
	
	CModelToolCharDataTable();
	~CModelToolCharDataTable();

private:
	TABLE	m_mapTableList;

protected:
	bool	LoadPropertyList(const char *lpszPath, const char * lpszFileName);
	bool	LoadProperty(const char * lpszFullPathFileName);
	bool	Add(std::string &strModelName, sMODELTOOL_CHAR_TBLDAT *pModelToolCharData);

	bool	LoadPropertySerialize(const char * lpszFullPathFileName, bool bEncrypt, char *szCryptPassword);
	bool	LoadCharacterProperty(CNtlFileSerializer *nsl);
	bool	LoadAnimTable(CNtlFileSerializer *nsl, sMODELTOOL_CHAR_TBLDAT *pModelToolCharData);

#ifdef _IS_SERVER_
	void	LoadAnimHitEvent(SEventAnimHit *pEventAnimHit, CNtlFileSerializer *sIn);
#endif

public:
	bool	Create(const char *lpszPath, bool bIsSharedAccess, char *szCryptPassword);
	void	Destroy();

	sMODELTOOL_CHAR_TBLDAT *FindData(const char* strModelName);
};
