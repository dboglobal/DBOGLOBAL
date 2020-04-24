/*****************************************************************************
 *
 * File			: NtlSobAvatarAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object avatar attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBAVATAR_ATTR_H__
#define __NTL_SOBAVATAR_ATTR_H__

#include "NtlSobPlayerAttr.h"

class CNtlSobAvatarAttr : public CNtlSobPlayerAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobAvatarAttr, NTL_DEFAULT_MEMORY_POOL)
	
public:

	RwUInt32	m_uiExp;		// 현재 level에서의 current exp	
	RwUInt32	m_uiMaxExp;	// 현재 level에서의 max exp

	//////////////////////////////////////////////////////////////////////////

	RwUInt16	m_wApRegen;
	RwUInt16	m_wApSitdownRegen;
	RwUInt16	m_wApDegen;

	RwUInt16	m_wRpRegen;			// RP 증가 속도.(/second)
	RwUInt16	m_wRpDimimutionRate;	// RP 감소 속도.(/second)

	DWORD		m_dwWeightLimit;
	float		m_fItemUpgradeBonusRate;
	BYTE		m_byExpBooster;
	BYTE		m_byQuestDropRate;

	//////////////////////////////////////////////////////////////////////////

	RwUInt32 m_uiZenny;	

	RwUInt32 m_uiSp;

	RwUInt32 m_uiHonorPoint;			///< 명예 포인트
	RwUInt32 m_uiMudosaPoint;			///< 무도사 포인트

	RwBool	 m_bCanChangeClass;			///< 전직 가능 여부


    //////////////////////////////////////////////////////////////////////////
    // PC room related
    DWORD    m_dwNetPy;                 ///<
	DWORD    m_dwHlsCash;                 ///<

	//////////////////////////////////////////////////////////////////////////

	bool			m_bInvisibleTitleNameFlag;
	std::wstring	m_wszTitleName;

public:

	CNtlSobAvatarAttr(); 

	virtual ~CNtlSobAvatarAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_AVATAR); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	RwUInt32		GetAP(void);
};


#endif
