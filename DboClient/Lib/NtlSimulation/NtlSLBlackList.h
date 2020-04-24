#pragma once

#include "NtlSLCommunityGroup.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"

struct sBlackListMember : public sCommunityMember
{
	CHARACTERID		charID;			///< DB character id
};


// 블랙리스트 데이터를 관리하는 클래스
/// by agebreak 2007.12.14
class CNtlSLBlackList : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
    CNtlSLBlackList(void);
    virtual ~CNtlSLBlackList(void);
    
    virtual	RwBool	Create(void);
    virtual VOID	Destroy(void);
    virtual VOID	Leave();
    virtual	void	HandleEvents(RWS::CMsg &pMsg);
    virtual RwBool	IsHaveGroup() {return TRUE;}

protected:
    VOID    OnBlackListAddRes(SNtlEventBlackListAddRes* pData);
    VOID    OnBlackListDelRes(SNtlEventBlackListDelRes* pData);
    VOID    OnFriendMoveRes(SNtlEventFriendMoveRes* pData);
    VOID    OnFriendListInfo(SNtlEventFriendListInfo* pData);

protected:

};
