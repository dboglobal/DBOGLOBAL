#include "precomp_ntlsimulation.h"
#include "NtlSLFriendList.h"

// Core
#include "NtlDebug.h"

// SL
#include "NtlResultCode.h"
#include "NtlSlEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlPacketTU.h"
#include "ResultCodeString.h"


CNtlSLFriendList::CNtlSLFriendList(void)
{
}

CNtlSLFriendList::~CNtlSLFriendList(void)
{
}

RwBool CNtlSLFriendList::Create( void ) 
{
	ClearMember();

    LinkMsg(g_EventFriendAdd_Res);
    LinkMsg(g_EventFriendDel_Res);
    LinkMsg(g_EventFriendMove_Res);    
    LinkMsg(g_EventFriendList_Info);
    LinkMsg(g_EventFriendInfo);
    LinkMsg(g_EventFriendInfoChange);

    return TRUE;
}

VOID CNtlSLFriendList::Destroy( void ) 
{
    NTL_FUNCTION("CNtlGuild::Destroy");

    UnLinkMsg(g_EventFriendAdd_Res);
    UnLinkMsg(g_EventFriendDel_Res);
    UnLinkMsg(g_EventFriendList_Info);
    UnLinkMsg(g_EventFriendMove_Res);
    UnLinkMsg(g_EventFriendInfo);
    UnLinkMsg(g_EventFriendInfoChange);

    Leave();

    NTL_RETURNVOID();
}

VOID CNtlSLFriendList::Leave() 
{
    ClearMember();
}

void CNtlSLFriendList::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventFriendAdd_Res)         // 친구 추가
    {
        OnFriendAddRes((SNtlEventFriendAddRes*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendDel_Res)    // 친구 삭제
    {
        OnFriendDelRes((SNtlEventFriendDelRes*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendMove_Res)   // 친구 블록
    {
        OnFriendMoveRes((SNtlEventFriendMoveRes*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendList_Info)  // 친구 리스트 정보 수신
    {
        OnFriendListInfo((SNtlEventFriendListInfo*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendInfo)       // 친구 정보 
    {
        OnFriendInfo((SNtlEventFriendInfo*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendInfoChange) // 친구 정보 변경
    {
        OnFriendInfoChange((SNtlEventFriendInfoChange*)pMsg.pData);
    }
	else if (pMsg.Id == g_EventChangeCharName) // 친구 정보 변경
	{
		OnFriendNameChange((SNtlEventChangeCharName*)pMsg.pData);
	}
}

VOID CNtlSLFriendList::OnFriendAddRes( SNtlEventFriendAddRes* pData ) 
{
    sFriendMember* pMember = NTL_NEW sFriendMember();
    ZeroMemory(pMember, sizeof(sFriendMember));

    pMember->charID = pData->uiTargetID;
	wcscpy(pMember->wszMemberName, pData->wchName);

    if( AddMember(pMember->charID, pMember) )
	{
		CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_ADD_RES, pMember->charID);
	}
	else
	{
		NTL_DELETE(pMember);
	}
}

VOID CNtlSLFriendList::OnFriendDelRes( SNtlEventFriendDelRes* pData ) 
{
	DelMember(pData->uiTargetID);

	CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_DEL_RES, pData->uiTargetID);
}

VOID CNtlSLFriendList::OnFriendMoveRes( SNtlEventFriendMoveRes* pData ) 
{
	DelMember(pData->uiTargetID);

	CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_MOVE_RES, pData->uiTargetID);
}

VOID CNtlSLFriendList::OnFriendListInfo( SNtlEventFriendListInfo* pData ) 
{
    //ClearMember(); // by daneos

    for(int i = 0; i < pData->byCount; ++i)
    {
        if(!pData->asInfo[i].bIsBlack)
        {
            sFriendMember* pMember = NTL_NEW sFriendMember();
            ZeroMemory(pMember, sizeof(sFriendMember));
            
            pMember->charID = pData->asInfo[i].charID;
			wcscpy(pMember->wszMemberName, pData->asInfo[i].wchName);

            if( false == AddMember(pMember->charID, pMember) )
				NTL_DELETE(pMember);
        }
    }

    CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_LIST_INFO, NULL);
}

VOID CNtlSLFriendList::OnFriendInfo( SNtlEventFriendInfo* pData ) 
{
    sFriendMember* pMember = reinterpret_cast<sFriendMember*>( GetMemberbyKey(pData->sInfo.charID) );
    if(!pMember)
        return;

    pMember->byClass = pData->sInfo.byClass;
    pMember->byLevel = pData->sInfo.byLevel;
    pMember->mapTblIdx = pData->sInfo.mapNameTblidx;
    pMember->byChannel = pData->sInfo.byChannel;    
    pMember->uiSerialID = pData->sInfo.hHandle;
    pMember->bOnline = TRUE;

    CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_INFO, pMember->charID);

    if(!pData->bIsFirst) // Meaning that a friend is connected
    {
        CNtlSLEventGenerator::FriendInfoNotify(SLFE_FRIEND_LOGIN, pMember->charID);
    }
}

VOID CNtlSLFriendList::OnFriendInfoChange( SNtlEventFriendInfoChange* pData ) 
{
    sFriendMember* pMember = reinterpret_cast<sFriendMember*>( GetMemberbyKey(pData->targetID) );
    if(!pMember)
        return;

    switch(pData->byChangeType)
    {
    case eFRIEND_CHANGE_TYPE_OFFLINE:
        pMember->bOnline = FALSE;        
        break;
    case eFRIEND_CHANGE_TYPE_CLASS:        
        pMember->byClass = (BYTE)pData->dwChangeValue;
        break;
    case eFRIEND_CHANGE_TYPE_LEVEL:
        pMember->byLevel = (BYTE)pData->dwChangeValue;
        break;
    case eFRIEND_CHANGE_TYPE_POSITION:
        pMember->mapTblIdx = pData->dwChangeValue;
        break;        
    }

    CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_INFO_CHANGE, pMember->charID);
}

void CNtlSLFriendList::OnFriendNameChange(SNtlEventChangeCharName * pData)
{
	sFriendMember* pMember = reinterpret_cast<sFriendMember*>(GetMemberbyKey(pData->charid));
	if (!pMember)
		return;

	wcscpy_s(pMember->wszMemberName, NTL_MAX_SIZE_CHAR_NAME + 1, pData->wchCharName);

	CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_INFO_CHANGE, pMember->charID);
}

VOID CNtlSLFriendList::Sort(RwUInt8 byDataType, RwBool bAscent)
{
    switch(byDataType)
    {
    case COMMUNITY_DATA_NAME:
        CNtlSLCommunityGroup::Sort(byDataType, bAscent);
        break;
    case COMMUNITY_DATA_LEVEL:
        dCOMMUNITY_GROUP_SORT(bAscent, CompareAscentLevel, CompareDecentLevel);
        break;
    case COMMUNITY_DATA_LOC:
        dCOMMUNITY_GROUP_SORT(bAscent, CompareAscentLoc, CompareDecentLoc);
        break;
    case COMMUNITY_DATA_CHANNEL:
        dCOMMUNITY_GROUP_SORT(bAscent, CompareAscentChannel, CompareDecentChannel);
        break;
    }
}

RwBool CNtlSLFriendList::CompareAscentLevel( sCommunityMember* pMember1, sCommunityMember* pMember2 ) 
{
    sFriendMember* pFriend1 = (sFriendMember*)pMember1;
    sFriendMember* pFriend2 = (sFriendMember*)pMember2;

    return (pFriend1->byLevel < pFriend2->byLevel);    
}

RwBool CNtlSLFriendList::CompareDecentLevel( sCommunityMember* pMember1, sCommunityMember* pMember2 ) 
{
    sFriendMember* pFriend1 = (sFriendMember*)pMember1;
    sFriendMember* pFriend2 = (sFriendMember*)pMember2;
    return (pFriend1->byLevel > pFriend2->byLevel);
}

RwBool CNtlSLFriendList::CompareAscentLoc( sCommunityMember* pMember1, sCommunityMember* pMember2 ) 
{
    sFriendMember* pFriend1 = (sFriendMember*)pMember1;
    sFriendMember* pFriend2 = (sFriendMember*)pMember2;
    return (pFriend1->mapTblIdx < pFriend2->mapTblIdx);
}

RwBool CNtlSLFriendList::CompareDecentLoc( sCommunityMember* pMember1, sCommunityMember* pMember2 ) 
{
    sFriendMember* pFriend1 = (sFriendMember*)pMember1;
    sFriendMember* pFriend2 = (sFriendMember*)pMember2;
    return (pFriend1->mapTblIdx > pFriend2->mapTblIdx);
}

RwBool CNtlSLFriendList::CompareAscentChannel( sCommunityMember* pMember1, sCommunityMember* pMember2 ) 
{
    sFriendMember* pFriend1 = (sFriendMember*)pMember1;
    sFriendMember* pFriend2 = (sFriendMember*)pMember2;
    return (pFriend1->byChannel < pFriend2->byChannel);
}

RwBool CNtlSLFriendList::CompareDecentChannel( sCommunityMember* pMember1, sCommunityMember* pMember2 ) 
{
    sFriendMember* pFriend1 = (sFriendMember*)pMember1;
    sFriendMember* pFriend2 = (sFriendMember*)pMember2;
    return (pFriend1->byChannel > pFriend2->byChannel);
}