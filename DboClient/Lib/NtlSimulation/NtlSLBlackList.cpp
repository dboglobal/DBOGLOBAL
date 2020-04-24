#include "precomp_ntlsimulation.h"
#include "NtlSLBlackList.h"

// Core
#include "NtlDebug.h"

// SL
#include "NtlResultCode.h"
#include "NtlSlEvent.h"
#include "NtlSlEventFunc.h"
#include "NtlPacketTU.h"
#include "ResultCodeString.h"


CNtlSLBlackList::CNtlSLBlackList(void)
{
}

CNtlSLBlackList::~CNtlSLBlackList(void)
{
}

RwBool CNtlSLBlackList::Create( void ) 
{
    LinkMsg(g_EventBlackListAdd_Res);
    LinkMsg(g_EventBlackListDel_Res);
    LinkMsg(g_EventFriendMove_Res);
    LinkMsg(g_EventFriendList_Info);

    return TRUE;
}

VOID CNtlSLBlackList::Destroy( void ) 
{
    UnLinkMsg(g_EventBlackListAdd_Res);
    UnLinkMsg(g_EventBlackListDel_Res);
    UnLinkMsg(g_EventFriendMove_Res);
    UnLinkMsg(g_EventFriendList_Info);
}

VOID CNtlSLBlackList::Leave() 
{
    ClearMember();
}

void CNtlSLBlackList::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventBlackListAdd_Res)
    { 
        OnBlackListAddRes((SNtlEventBlackListAddRes*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventBlackListDel_Res)
    {
        OnBlackListDelRes((SNtlEventBlackListDelRes*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendMove_Res)
    {
        OnFriendMoveRes((SNtlEventFriendMoveRes*)pMsg.pData);
    }
    else if(pMsg.Id == g_EventFriendList_Info)
    {
        OnFriendListInfo((SNtlEventFriendListInfo*)pMsg.pData);
    }
}

VOID CNtlSLBlackList::OnBlackListAddRes( SNtlEventBlackListAddRes* pData ) 
{
    sBlackListMember* pMember = NTL_NEW sBlackListMember();
    ZeroMemory(pMember, sizeof(sBlackListMember));

    pMember->charID = pData->uiTargetID;
	wcscpy(pMember->wszMemberName, pData->wchName);

    if( AddMember(pMember->charID, pMember) )
	{
		CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_BLACK_ADD_RES, pData->uiTargetID);
	}
	else
	{
		NTL_DELETE(pMember);
	}
}

VOID CNtlSLBlackList::OnBlackListDelRes( SNtlEventBlackListDelRes* pData ) 
{
	DelMember(pData->uiTargetID);

	CNtlSLEventGenerator::FriendInfoNotify(TU_FRIEND_BLACK_DEL_RES, pData->uiTargetID);
}

VOID CNtlSLBlackList::OnFriendMoveRes( SNtlEventFriendMoveRes* pData ) 
{
    if(pData->wResultCode == CHAT_SUCCESS)
    {
        sBlackListMember* pMember = NTL_NEW sBlackListMember();
        ZeroMemory(pMember, sizeof(sBlackListMember));

        pMember->charID = pData->uiTargetID;        
        wcscpy(pMember->wszMemberName, pData->wchName);
        
        if( false == AddMember(pMember->charID, pMember) )
			NTL_DELETE(pMember);
    }    
}

VOID CNtlSLBlackList::OnFriendListInfo( SNtlEventFriendListInfo* pData ) 
{
    //ClearMember(); // by daneos

    for(int i = 0; i < pData->byCount; ++i)
    {
        if(pData->asInfo[i].bIsBlack)
        {
            sBlackListMember* pMember = NTL_NEW sBlackListMember();
            ZeroMemory(pMember, sizeof(sBlackListMember));

            pMember->charID = pData->asInfo[i].charID;
			wcscpy(pMember->wszMemberName, pData->asInfo[i].wchName);

            if( false == AddMember(pMember->charID, pMember) )
				NTL_DELETE(pMember);
        }
    }
}