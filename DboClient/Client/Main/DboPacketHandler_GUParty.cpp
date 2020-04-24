/*****************************************************************************
* File			: DboPackethandler_GUParty.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 파티 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// shared
#include "ItemTable.h"

// sound
#include "GUISoundDefine.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"

// dbo
#include "IconMoveManager.h"
#include "DiceManager.h"

void PacketHandler_GSPartyCreateRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CREATE_RES );

	// 자신이 요청한 파티 생성 결과를 알려준다.
	sGU_PARTY_CREATE_RES* pResult = (sGU_PARTY_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_CREATE_RES"), TRUE );
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr());

	// Create Party
	CNtlSLEventGenerator::PartyCreate(pResult->wszPartyName);

	// Add yourself to the party list
	CNtlSLEventGenerator::PartyMemberAdd(pAvatar->GetSerialID(), (WCHAR*)pAvatarAttr->GetName());

	// You are the party leader.
	CNtlSLEventGenerator::PartyLeaderChange(pAvatar->GetSerialID());

	// Zeni distribution system
	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, NTL_PARTY_ZENNY_LOOTING_GREEDILY);

	// Item distribution method
	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, NTL_PARTY_ITEM_LOOTING_GREEDILY);

	// Party Quest
	CNtlSLEventGenerator::TSParty();
}

void PacketHandler_GSPartyDisbandRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DISBAND_RES );

	// 자신이 파티를 해산한 결과를 알려준다.
	sGU_PARTY_DISBAND_RES* pResult = (sGU_PARTY_DISBAND_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_DISBAND_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyDisband();
}

void PacketHandler_GSPartyDisbandNfy(void *pPacket)
{
	// 파티가 해산되었음을 파티장이 아닌 맴버들에게 알려준다.
	//sGU_PARTY_DISBANDED_NFY* pResult = (sGU_PARTY_DISBANDED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyDisband();
}

void PacketHandler_GSPartyInviteRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVITE_RES );

	// 파티 리더에게 파티 초대 결과를 알려준다.	
	sGU_PARTY_INVITE_RES* pResult = (sGU_PARTY_INVITE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_INVITE_RES"), TRUE );
		return;
	}	

	// %s님을 파티에 초대하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_INVITE_SUCCESS", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_GSPartyInviteNfy(void *pPacket)
{
	// 초대 대상에게 메세지를 보낸다.
	sGU_PARTY_INVITE_NFY* pResult = (sGU_PARTY_INVITE_NFY*)pPacket;

	if( wcslen(pResult->wszInvitorPcName) > 0)
	{
		GetAlarmManager()->FormattedAlarmMessage( "DST_PARTY_PARTY_INVITE", FALSE, NULL, pResult->wszInvitorPcName);
	}
}

void PacketHandler_GSPartyResponseInvitationRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_RESPONSE_INVITATION_RES );

	// 파티 초대의 응답에 대한 결과를 리더에게 보낸다.
	sGU_PARTY_RESPONSE_INVITATION_RES* pResult = (sGU_PARTY_RESPONSE_INVITATION_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_RESPONSE_INVITATION_RES"), TRUE );
		return;
	}	
}

void PacketHandler_GSPartyMemberJoinedNfy(void *pPacket)
{
	// 새로운 파티원이 가입했음을 알려준다.
	sGU_PARTY_MEMBER_JOINED_NFY* pResult = (sGU_PARTY_MEMBER_JOINED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyMemberAdd(pResult->memberInfo.hHandle, pResult->memberInfo.awchMemberName,
		pResult->memberInfo.byRace, pResult->memberInfo.byClass, pResult->memberInfo.byLevel,
		pResult->memberInfo.curLp, pResult->memberInfo.maxLp, pResult->memberInfo.wCurEP,
		pResult->memberInfo.wMaxEP, pResult->memberInfo.worldTblidx, pResult->memberInfo.worldId,
		pResult->memberInfo.vCurLoc.x, pResult->memberInfo.vCurLoc.y, pResult->memberInfo.vCurLoc.z);

	// %s님이 파티에 들어오셨습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_NEW_MEMBER", FALSE, NULL, pResult->memberInfo.awchMemberName);
}

void PacketHandler_GSPartyInfo(void *pPacket)
{
	// 새로 가입한 파티원이 파티정보를 받는다.
	sGU_PARTY_INFO* pResult = (sGU_PARTY_INFO*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	// 파티를 생성한다
	CNtlSLEventGenerator::PartyCreate(pResult->awchPartyName);

	for(RwInt32 i = 0 ; i < pResult->byMemberInfoCount ; ++i )
	{		
		CNtlSLEventGenerator::PartyMemberAdd(pResult->memberInfo[i].hHandle, pResult->memberInfo[i].awchMemberName,
			pResult->memberInfo[i].byRace, pResult->memberInfo[i].byClass,
			pResult->memberInfo[i].byLevel, pResult->memberInfo[i].curLp,
			pResult->memberInfo[i].maxLp, pResult->memberInfo[i].wCurEP,
			pResult->memberInfo[i].wMaxEP,
			pResult->memberInfo[i].worldTblidx, pResult->memberInfo[i].worldId,
			pResult->memberInfo[i].vCurLoc.x, pResult->memberInfo[i].vCurLoc.y, pResult->memberInfo[i].vCurLoc.z);
	}	

	// 파티 리스트에 자신을 추가시킨다	
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr());
	CNtlSLEventGenerator::PartyMemberAdd(pAvatar->GetSerialID(), (WCHAR*)pAvatarAttr->GetName());

	// 파티장을 설정한다
	CNtlSLEventGenerator::PartyLeaderChange(pResult->hLeader);

	// 아이템 분배 방식
	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, pResult->byItemLootingMethod);

	// 제니 분배 방식
	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, pResult->byZennyLootingMethod);

    // 공유 타겟 데이터    
    CNtlSLEventGenerator::PartyShareTargetNfy(pResult->sharetargetInfo);

	// 파티 던전 정보
	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_STATE, INVALID_SERIAL_ID, pResult->eDiff);	
}

void PacketHandler_GSPartyInviteDeclinedNfy(void *pPacket)
{
	// 초대한 대상이 거절했다.
	sGU_PARTY_INVITATION_DECLINED_NFY* pResult = (sGU_PARTY_INVITATION_DECLINED_NFY*)pPacket;	

	// %s님이 초대를 거절하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_INVITE_DECLINE", FALSE, NULL, pResult->wszPlayerName);
}

void PacketHandler_GSPartyInviteExpiredNfy(void *pPacket)
{
	// 파티 초대를 했지만 일정 시간동안 응답이 없었다.
	sGU_PARTY_INVITATION_EXPIRED_NFY* pResult = (sGU_PARTY_INVITATION_EXPIRED_NFY*)pPacket;

	// %s님이 초대를 거절하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_INVITE_DECLINE", FALSE, NULL, pResult->wszPlayerName);
}

void PacketHandler_GSPartyLeaveRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_LEAVE_RES );

	// 자신이 요청한 파티 탈퇴의 결과를 알려준다.
	sGU_PARTY_LEAVE_RES* pResult = (sGU_PARTY_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_LEAVE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyLeaderChange(INVALID_HOBJECT); // required to turn char name color back to normal

	CNtlSLEventGenerator::PartyDisband();
}

void PacketHandler_GSPartyMemberLeftNfy(void *pPacket)
{
	// 자신의 파티원이 떠났다.
	sGU_PARTY_MEMBER_LEFT_NFY* pResult = (sGU_PARTY_MEMBER_LEFT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText	= pMember->wszMemberName;

	// %s님이 파티에서 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_LEAVE", FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyMemberDel(pResult->hMember);
}

void PacketHandler_GSPartyKickOutRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_KICK_OUT_RES );

	// 파티 맴버 강퇴 결과
	sGU_PARTY_KICK_OUT_RES* pResult = (sGU_PARTY_KICK_OUT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_KICK_OUT_RES"), TRUE );
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hTargetMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hTargetMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티에서 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_LEAVE", FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyMemberDel(pResult->hTargetMember);
}

void PacketHandler_GSPartyMemberKickedOutNfy(void *pPacket)
{
	// 파티 맴버가 강퇴된 것을 남아있는 파티원에게 알려준다.
	sGU_PARTY_MEMBER_KICKED_OUT_NFY* pResult = (sGU_PARTY_MEMBER_KICKED_OUT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티에서 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_LEAVE", FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyMemberDel(pResult->hMember);
}

void PacketHandler_GSPartyChangeLeaderRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHANGE_LEADER_RES );

	// 파티 리더에게 리더 변경 요청을 결과를 알려준다.
	sGU_PARTY_CHANGE_LEADER_RES* pResult = (sGU_PARTY_CHANGE_LEADER_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_CHANGE_LEADER_RES"), TRUE );
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hNewLeader) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hNewLeader);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_CHANGE_LEADER", FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyLeaderChange(pResult->hNewLeader);
}

void PacketHandler_GSPartyChangedLeaderNfy(void *pPacket)
{
	// 파티 리더가 변경되었음을 파티 맴버에게 알려준다
	sGU_PARTY_LEADER_CHANGED_NFY* pResult = (sGU_PARTY_LEADER_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hNewLeader) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hNewLeader);
		return;
	}	

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_CHANGE_LEADER", FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyLeaderChange(pResult->hNewLeader);
}

void PacketHandler_GSPartyLevelUp(void* pPacket)
{
	// 파티원이 레벨업을 하였다
	sGU_PARTY_MEMBER_LEVELED_UP_NFY* pResult = (sGU_PARTY_MEMBER_LEVELED_UP_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_LEVEL, pResult->hMember, pResult->byLevel);
	CNtlSLEventGenerator::PartyUpdate(PMT_LP, pResult->hMember, pMember->wCurLP, pResult->maxLp);
	CNtlSLEventGenerator::PartyUpdate(PMT_EP, pResult->hMember, pMember->wCurEP, pResult->wMaxEP);	
}

void PacketHandler_GSPartyChangeClass(void* pPacket)
{
	// 파티원의 클래스가 바뀌었다
	sGU_PARTY_MEMBER_CLASS_CHANGED_NFY* pResult = (sGU_PARTY_MEMBER_CLASS_CHANGED_NFY*)pPacket;
	CNtlSLEventGenerator::PartyUpdate(PMT_CLASS, pResult->hMember, pResult->byNewClass);
}

void PacketHandler_GSPartyMember_LocationNfy(void* pPacket)
{
	// 파티 맴버의 위치를 업데이트 한다
	sGU_PARTY_MEMBER_LOCATION_NFY* pResult = (sGU_PARTY_MEMBER_LOCATION_NFY*)pPacket;

	RwV3d vPos;
	NtlLocationDecompress(&pResult->vCompressLoc, &vPos.x, &vPos.y, &vPos.z);

	CNtlSLEventGenerator::PartyMemberLocation(pResult->hMember, pResult->worldId, pResult->worldTblidx, vPos);
}

void PacketHandler_GSPartyMemberGainedItemNfy(void *pPacket)
{
	// 파티 맴버가 아이템을 주은 것을 알려준다
	sGU_PARTY_MEMBER_GAINED_ITEM_NFY* pResult = (sGU_PARTY_MEMBER_GAINED_ITEM_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	if(pResult->itemTblidx == INVALID_SERIAL_ID)
	{
		// %s님이 미확인 아이템을 획득하였습니다
		GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_INVEN_ADD_UNDEFINED_ITEM_OTHER", FALSE, NULL, pwcText);
	}
	else
	{
		// %s님의 획득 아이템 %s
		sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(pResult->itemTblidx);
		if( !pITEM_TBLDAT )
		{
			DBO_FAIL("Not exist item table of index : " << pResult->itemTblidx);
			return;
		}

		memset((char*)awcPacketMessageBuffer, 0, sizeof(WCHAR) * dPACKET_MEESAGE_LENGTH);

		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
		std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);		

		GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ITEM", FALSE, NULL, pwcText, strItemName.c_str());
	}
}

void PacketHandler_GSPartyMemberGainedZennyNfy(void *pPacket)
{
	// 제니를 분배받았다는 메세지
	sGU_PARTY_MEMBER_GAINED_ZENNY_NFY* pResult = (sGU_PARTY_MEMBER_GAINED_ZENNY_NFY*)pPacket;

	if( pResult->bIsShared )
	{
		// 다른 파티원이 제니를 주어서 분배받았다
		// %d중 %d 제니를 분배받았습니다
        if(pResult->dwBonusZenny == 0)
        {
		    GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ZENNY", FALSE, NULL, Logic_FormatZeni(pResult->dwOriginalZenny), Logic_FormatZeni(pResult->dwZenny));
        }
        else    // PC방 추가 획득
        {
            GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ZENNY_AND_BONUS", FALSE, NULL, Logic_FormatZeni(pResult->dwOriginalZenny), Logic_FormatZeni(pResult->dwAcquisitionZenny), Logic_FormatZeni(pResult->dwBonusZenny));
        }
	}
	else
	{
		// 다른 파티원이 제니를 주었다
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			return;
		}

		CNtlParty*		pParty	= pAvatar->GetParty();
		sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
		if( !pMember )
		{
			DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
			return;
		}

		// %s님이 %d 제니를 획득하였습니다
        if(pResult->dwBonusZenny == 0)
        {
		    GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ZENNY_OTHER", FALSE, NULL, pMember->wszMemberName, Logic_FormatZeni(pResult->dwZenny));
        }
        else    // PC방 추가 획득
        {
            GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ZENNY_OTHER_AND_BONUS", FALSE, NULL, pMember->wszMemberName, Logic_FormatZeni(pResult->dwAcquisitionZenny), Logic_FormatZeni(pResult->dwBonusZenny));
        }
	}
}

void PacketHandler_GSPartyChangeZennyLootinMethodRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES );

	// 제니 분배 방식 변경 결과를 파티장에게 알려준다
	sGU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES* pResult = (sGU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )	
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);	

	// 제니 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_CHANGE_ZENNY_DIVISION", FALSE, NULL, Logic_GetPartyZeniLootingMethod(pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyZennyLootingMethodChangedNfy(void *pPacket)
{
	// 파티장을 제외한 맴버들에게 제니 분배 방식 변경 알리기
	sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY* pResult = (sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);

	// 제니 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_CHANGE_ZENNY_DIVISION", FALSE, NULL, Logic_GetPartyZeniLootingMethod(pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyChangeItemLootinMethodRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES );

	// 아이템 분배 방식 변경 결과를 파티장에게 알려준다
	sGU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES* pResult = (sGU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);

	// 아이템 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_CHANGE_ITEM_DIVISION", FALSE, NULL, Logic_GetPartyItemLootingMethod(pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyItemLootingMethodChangedNfy(void *pPacket)
{
	// 파티장을 제외한 맴버들에게 제니 아이템 분배 방식 변경 알리기
	sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY* pResult = (sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);

	// 아이템 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_CHANGE_ITEM_DIVISION", FALSE, NULL, Logic_GetPartyItemLootingMethod(pResult->byNewLootingMethod));
}

// 파티 공유 타겟 요청에 대한 결과 패킷
void PacketHandler_GSPartyShareTargetRes( void* pPacket ) 
{
    sGU_PARTY_SHARETARGET_RES* pData = (sGU_PARTY_SHARETARGET_RES*)pPacket;
    if( pData->wResultCode != GAME_SUCCESS )
    {
        GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pData->wResultCode, "GU_PARTY_SHARETARGET_RES"), TRUE );
        return;
    }

    CNtlSLEventGenerator::PartyShareTargetRes(pData->wResultCode);
}

// 파티 공유 타겟 선택에 대한 알림 패킷
void PacketHandler_GSPartyShareTargetNfy( void* pPacket ) 
{
    sGU_PARTY_SHARETARGET_NFY* pData = (sGU_PARTY_SHARETARGET_NFY*)pPacket;

    CNtlSLEventGenerator::PartyShareTargetNfy(pData->sharetargetInfo);
}

void PacketHandler_GSPartyDungeonDiffRes(void *pPacket)
{
	// 파티장에게 파티 던전의 난이도 변경 결과를 알린다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DUNGEON_DIFF_RES );

	sGU_PARTY_DUNGEON_DIFF_RES* pResult = (sGU_PARTY_DUNGEON_DIFF_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_DUNGEON_DIFF_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_STATE, INVALID_SERIAL_ID, pResult->eDiff);	

	if (pResult->eDiff == PARTY_DUNGEON_STATE_NORMAL)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_NORMAL");
	else if (pResult->eDiff == PARTY_DUNGEON_STATE_HARD)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_HARD");
	else if (pResult->eDiff == PARTY_DUNGEON_STATE_LEGENDARY)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_LEGENDARY");
	else if (pResult->eDiff == PARTY_DUNGEON_STATE_HERO)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_HERO");
}

void PacketHandler_GSPartyDungeonDiffNfy(void *pPacket)
{
	// 파티원들에게 파티 던전 난이도 변경을 알린다
	sGU_PARTY_DUNGEON_DIFF_NFY* pResult = (sGU_PARTY_DUNGEON_DIFF_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_STATE, INVALID_SERIAL_ID, pResult->eDiff);	
	
	if(pResult->eDiff == PARTY_DUNGEON_STATE_NORMAL)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_NORMAL");
	else if (pResult->eDiff == PARTY_DUNGEON_STATE_HARD)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_HARD");
	else if (pResult->eDiff == PARTY_DUNGEON_STATE_LEGENDARY)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_LEGENDARY");
	else if (pResult->eDiff == PARTY_DUNGEON_STATE_HERO)
		GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_CHANGE_HERO");
}

void PacketHandler_GSPartyDungeonInitRes(void *pPacket)
{
	// 파티장에게 파티 던전 초기화 결과를 알린다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DUNGEON_INIT_RES );

	sGU_PARTY_DUNGEON_INIT_RES* pResult = (sGU_PARTY_DUNGEON_INIT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_DUNGEON_INIT_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_INITIALIZE);

	GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_INITIALIZE");
}

void PacketHandler_GSPartyDungeonInitNfy(void *pPacket)
{
	// 파티원에게 파티 던전 초기화를 알린다
	//sGU_PARTY_DUNGEON_INIT_NFY* pResult = (sGU_PARTY_DUNGEON_INIT_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage("DST_PARTY_DUNGEON_INITIALIZE");
}