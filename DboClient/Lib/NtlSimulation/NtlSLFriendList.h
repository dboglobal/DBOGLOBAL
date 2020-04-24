#pragma once

#include "NtlSLCommunityGroup.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"

struct sFriendMember : public sCommunityMember
{
	CHARACTERID		charID;			///< DB character id
	RwUInt32		mapTblIdx;		///< 맵 테이블 인덱스
	RwUInt8			byLevel;		///< 레벨
	RwUInt8			byClass;		///< 클래스	
	RwBool			bOnline;		///< 게임 접속 여부
	RwUInt32        uiSerialID;     ///< 시리얼ID (핸들)
	RwUInt8			byChannel;      ///< 채널
};

enum FriendDataType
{
	COMMUNITY_FRIEND_LEVEL	= COMMUNITY_DATA_NEXT_INDEX,
	COMMUNITY_FRIEND_ONLINE,
};

/// 친구리스트의 데이터를 관리하는 클래스
/// by agebreak 2007.12.14
class CNtlSLFriendList : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
    CNtlSLFriendList(void);
    virtual ~CNtlSLFriendList(void);

    virtual	RwBool	Create();
    virtual VOID	Destroy();
    virtual VOID	Leave();
    virtual	void	HandleEvents(RWS::CMsg &pMsg);

	// Sort를 실행하고 나서는 이전에 GetMemberbyIndex()로 받은 맴버 구조체와 다를 수 있다
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);
    virtual RwBool	IsHaveGroup() {return TRUE;}

protected:
	VOID    OnFriendAddRes(SNtlEventFriendAddRes* pData);
	VOID    OnFriendDelRes(SNtlEventFriendDelRes* pData);
	VOID    OnFriendMoveRes(SNtlEventFriendMoveRes* pData);
	VOID    OnFriendListInfo(SNtlEventFriendListInfo* pData);
	VOID    OnFriendInfo(SNtlEventFriendInfo* pData);
	VOID    OnFriendInfoChange(SNtlEventFriendInfoChange* pData);
	void    OnFriendNameChange(SNtlEventChangeCharName* pData);

    static RwBool CompareAscentLevel(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareDecentLevel(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareAscentLoc(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareDecentLoc(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareAscentChannel(sCommunityMember* pMember1, sCommunityMember* pMember2);
    static RwBool CompareDecentChannel(sCommunityMember* pMember1, sCommunityMember* pMember2);
protected:
};
