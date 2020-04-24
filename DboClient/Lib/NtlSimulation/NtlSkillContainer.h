/*****************************************************************************
 *
 * File			: NtlSkillContainer.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 24	
 * Abstract		: Simulation skill container
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SKILL_CONTAINER_H__
#define __NTL_SKILL_CONTAINER_H__

// shared
#include "NtlSkill.h"

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLServerDeclear.h"

#define NTL_MAX_PC_HAVE_ACTION_SKILL	50

class CNtlSobSkill;
class CNtlSobHTBSkill;
class CNtlSobActionSkill;

class CNtlSkillContainer : public RWS::CEventHandler
{
protected:

	SERIAL_HANDLE m_hSerial[NTL_MAX_PC_HAVE_SKILL];		/** skill 전체 slot */
	SERIAL_HANDLE m_hHTBSerial[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];	/** HTB Skill 분리*/
	SERIAL_HANDLE m_hActionSerial[NTL_MAX_PC_HAVE_ACTION_SKILL];

public:

	CNtlSkillContainer();
	~CNtlSkillContainer();

	virtual RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg) {}

	void SetSkillSerial(RwInt32 iSoltIdx, SERIAL_HANDLE hSerial);
	void SetHTBSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial);
	void SetActionSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial);

	RwInt32 GetSkillCount(void);
	CNtlSobSkill* GetSkill(RwInt32 iSlotIdx);
	CNtlSobSkill* GetSkillFromTableId(RwUInt32 uiTblId);
	CNtlSobSkill* GetSameKindSkillFromTableId(RwUInt32 uiTblId);
	sSKILL_TBLDAT* GetSkillFromTable(RwInt32 iSlotIdx);
	RwInt32 GetSkillSlotIdx( SERIAL_HANDLE hSerial );
	RwBool SkillExists( TBLIDX minIdx, TBLIDX maxIdx );	// Upgrade 체크용.

	RwInt32 GetHTBSkillCount(void);
	CNtlSobHTBSkill* GetHTBSkill(RwInt32 iSlotIdx);
	CNtlSobHTBSkill* GetHTBSkillFromTableId(RwUInt32 uiTblId);
	sHTB_SET_TBLDAT* GetHTBSkillFromTable(RwInt32 iSlotIdx);	
	RwInt32 GetHTBSkillSlotIDx( SERIAL_HANDLE hSerial );

	RwInt32 GetActionSkillCount(void);
	CNtlSobActionSkill* GetActionSkill(RwInt32 iSlotIdx);
	CNtlSobActionSkill* GetActionSkillFromTableId(RwUInt32 uiTblId);
	sACTION_TBLDAT* GetActionSkillFromTable(RwInt32 iSlotIdx);	
	RwInt32 GetActionSkillSlotIDx( SERIAL_HANDLE hSerial );
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlAvatarSkillContainer : public CNtlSkillContainer
{
private:

	RwBool		m_bDashPossible;
	DWORD		m_dwDashReqLP;
	RwUInt16	m_wDashReqEP;
	RwUInt16	m_wDashReqRP;

	RwBool		m_bChargingPossible;
	RwBool		m_bBlockingPossible;
	bool		m_bAirPossible;

private:

	void CreateEventHandler(RWS::CMsg &pMsg);
	void AddEventHandler(RWS::CMsg &pMsg);
	void UpgradeEventHandler(RWS::CMsg &pMsg);
	void SkillAction(RWS::CMsg &pMsg);

	void AddHTBEventHandler(RWS::CMsg& msg);
	void HTBSkillAction(RWS::CMsg& msg);

	void SocialActionCooling(RWS::CMsg& msg);

	void RPBonusSetupHandler(RWS::CMsg& msg);

	void SkillInitHandler( RWS::CMsg& msg );
	void PassiveSkillEffectHandler( RWS::CMsg& msg );
		
	// passive skill 판단.
	void PassiveSkillDetecting(RwUInt32 uiTblId);

public:

	CNtlAvatarSkillContainer();

	void	HandleEvents(RWS::CMsg &pMsg);

	RwBool	CanDash(RwUInt32& dwDashReqLP, RwUInt16& wDashReqEP, RwUInt16& wDashReqRP);

	bool	CanAir();

	RwBool	CanCharging();
	RwBool	CanBlocking();
};

inline RwBool CNtlAvatarSkillContainer::CanDash(RwUInt32& dwDashReqLP, RwUInt16& wDashReqEP, RwUInt16& wDashReqRP)
{
	dwDashReqLP = m_dwDashReqLP;
	wDashReqEP = m_wDashReqEP;
	wDashReqRP = m_wDashReqRP;

	return m_bDashPossible;
}

inline bool CNtlAvatarSkillContainer::CanAir()
{
	return m_bAirPossible;
}

inline RwBool CNtlAvatarSkillContainer::CanCharging()
{
	return m_bChargingPossible;
}

inline RwBool CNtlAvatarSkillContainer::CanBlocking()
{
	return m_bBlockingPossible;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlPetSkillContainer : public RWS::CEventHandler
{
protected:

	SERIAL_HANDLE	m_hSerial[NTL_MAX_NPC_HAVE_SKILL];		/** skill 전체 slot */
	RwUInt8			m_byUseBasis[NTL_MAX_NPC_HAVE_SKILL];	/** skill AI Basis */
	RwUInt16		m_wUseLp[NTL_MAX_NPC_HAVE_SKILL];		/** skill AI Lp 조건 */
	RwUInt16		m_wUseTime[NTL_MAX_NPC_HAVE_SKILL];		/** skill AI Time 조건 */

public:

	CNtlPetSkillContainer();
	~CNtlPetSkillContainer();

	virtual RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

	void			SetSkillSerial(RwInt32 iSlotIdx, SERIAL_HANDLE hSerial);
	void			SetSkillUseBasis(RwInt32 iSlotIdx, RwUInt8 byUseBasis);
	void			SetSkillUseLp(RwInt32 iSlotIdx, RwUInt16 wUseLp);
	void			SetSkillUseTime(RwInt32 iSlotIdx, RwUInt16 wUseTime);

	RwInt32			GetSkillCount(void);
	CNtlSobSkill*	GetSkill(RwInt32 iSlotIdx);
	CNtlSobSkill*	GetSkillFromTableId(RwUInt32 uiTblId);
	sSKILL_TBLDAT*	GetSkillFromTable(RwInt32 iSlotIdx);
	RwInt32			GetSkillSlotIdx( SERIAL_HANDLE hSerial );
	RwUInt8			GetSkillUseBasis(RwInt32 iSlotIdx);
	RwUInt16		GetSkillUseLp(RwInt32 iSlotIdx);
	RwUInt16		GetSkillUseTime(RwInt32 iSlotIdx);
};

#endif