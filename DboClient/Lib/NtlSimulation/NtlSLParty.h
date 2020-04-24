/*****************************************************************************
*
* File			: NtlParty.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 10. 20	
* Abstract		: Simulation party info class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PARTY_INFO_H__
#define __NTL_PARTY_INFO_H__

// core
#include "ceventhandler.h"

// share
#include "NtlParty.h"
#include "NtlPartyDungeon.h"

// simulation
#include "NtlSharedDef.h"
#include "NtlSLDef.h"
#include "NtlSLCommunityGroup.h"

#define dINVALID_CHARM_BUFF_INDEX	NTL_PARTY_CHARM_INVENTORY_COUNT

class CNtlFakeBuffContainer;
struct sITEM_TBLDAT;
struct sCHARM_TBLDAT;

struct sPartyMember : public sCommunityMember
{
	SERIAL_HANDLE	hSerial;		///< 맴버 핸들
	RwUInt8			byRace;			///< 종족
	RwUInt8			byClass;		///< 직업
	RwUInt8			byLevel;		///< 레벨
	RwUInt16		wCurLP;			///< 현재 LP
	RwUInt16		wMaxLP;			///< 최대 LP
	RwUInt16		wCurEP;			///< 현재 EP
	RwUInt16		wMaxEP;			///< 최대 EP
	RwUInt32		uiWorldID;		///< 월드 ID
	RwUInt32		uiWorldTbl;		///< 월드 테이블 Index
	RwV3d			vPos;			///< 위치(가시거리 밖에 있을 때는 패킷을 받는다)
	SERIAL_HANDLE	hBusSerial;		///< 타고 있는 버스의 핸들

	CNtlFakeBuffContainer*	pFakeBuffContainer;		///< Save buff information of members except yourself
};

enum ePartyMemberDataType
{
	PARTY_MEMBER_DATA_RACE,

	NUM_PARTY_MEMBER_DATA
};

class CNtlParty : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
	CNtlParty();
	virtual ~CNtlParty();

	virtual	RwBool	Create(void);
	VOID			Update(RwReal fElapsed);
	virtual VOID	Destroy(void);

	virtual VOID	ClearMember();
	virtual VOID	Leave();

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

	VOID			SetPartyName(WCHAR* pcName);			///< 파티 이름 설정

	const WCHAR*	GetPartyName();							///< 파티 이름을 반환한다
	SERIAL_HANDLE	GetLeaderHandle();						///< 리더 핸들을 반환한다
	RwInt32			GetAvatarIndex();

	// 분배 방식 반환
	RwUInt8				 GetZennyLootingMethod();
	RwUInt8				 GetItemLootingMethod();

	ePARTY_DUNGEON_STATE GetPartyDungeonState();

	// After executing Sort, it may be different from the member structure previously received with GetMemberbyIndex ()
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	virtual RwBool	IsHaveGroup();

    sSHARETARGET_INFO*  GetShareTargetInfos() {return m_aShareTargetInfo;}      ///< 파티 공유타겟 정보를 반환한다

protected:
    void            PartyShareTargetResHandler(RWS::CMsg& pMsg);         ///< 파티 공유 타겟 응답 처리
    void            PartyShareTargetNfyHandler(RWS::CMsg& pMsg);         ///< 파티 공유 타겟 알림 처리 

protected:
	WCHAR					m_pwPartyName[NTL_MAX_SIZE_PARTY_NAME+1];	///< 파티 이름

	SERIAL_HANDLE			m_hLeaderHandle;			///< 파티 리더 핸들

	sSHARETARGET_INFO       m_aShareTargetInfo[NTL_MAX_SHARETARGET_COUNT];        // 파티 공유 타겟 정보, 배열의 인덱스가 공유 타겟 번호이다

	RwUInt8					m_byZennyLootingMethod;		///< 제니 분배 방식
	RwUInt8					m_byItemLootingMethod;		///< 아이템 분배 방식

	ePARTY_DUNGEON_STATE	m_eDungeonState;			///< 파티 인스턴스 던전의 상태
};

inline RwUInt8 CNtlParty::GetZennyLootingMethod()
{
	return m_byZennyLootingMethod;
}

inline RwUInt8 CNtlParty::GetItemLootingMethod()
{
	return m_byItemLootingMethod;
}

inline ePARTY_DUNGEON_STATE CNtlParty::GetPartyDungeonState()
{
	return m_eDungeonState;
}

#endif