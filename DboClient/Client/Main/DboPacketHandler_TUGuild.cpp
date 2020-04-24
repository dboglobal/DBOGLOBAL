/*****************************************************************************
* File			: DboPackethandler_TU.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2008. 2. 25
* Abstract		: 
*****************************************************************************
* Desc         : 게임 서버 & 커뮤니티 서버 길드 패킷 핸들
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// Shared
#include "NtlGuild.h"
#include "DojoTable.h"
#include "ObjectTable.h"

// presentation
#include "NtlPLVisualManager.h"
#include "NtlPLDojoContainer.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlCameraController.h"
#include "NtlSobPlayer.h"
#include "NtlSobGroup.h"

// dbo
#include "IconMoveManager.h"
#include "LobbyManager.h"


void PacketHandler_GSGuild_Create_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_GUILD_CREATE_RES );

	// 길드 생성 결과
	sGU_GUILD_CREATE_RES* pResult = (sGU_GUILD_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CREATE_RES"), TRUE );
		return;
	}

	// 길드를 만들었습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_CREATE");
}

void PacketHandler_GSGuild_Invite_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_GUILD_INVITE_RES );

	// 길드 초대 결과
	sGU_GUILD_INVITE_RES* pResult = (sGU_GUILD_INVITE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_INVITE_RES"), TRUE );
		return;
	}

	// %s님을 길드에 초대하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_INVITE", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_TUGuild_CreateNfy(void *pPacket)
{
	// 길드가 생성되었음을 기타 맴버에게 알린다
	sTU_GUILD_CREATED_NFY *pResult = (sTU_GUILD_CREATED_NFY*)pPacket;

	// %s 길드가 생성되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CREATE_NFY", FALSE, NULL, pResult->wszGuildName);
}

void PacketHandler_TUGuild_Info(void *pPacket)
{
	// 길드 정보를 받는다
	sTU_GUILD_INFO* pResult = (sTU_GUILD_INFO*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_NOTICE, wcslen(pResult->guildInfo.awchNotice),
													pResult->guildInfo.awchName,
													pResult->guildInfo.awchNotice);

	CNtlSLEventGenerator::SLGuildEvent(SLGE_GUILD_INFO, INVALID_SERIAL_ID, (void*)&pResult->guildInfo);
}

void PacketHandler_TUGuild_MemberInfo(void *pPacket)
{
	// 길드 맴버 정보를 받는다
	sTU_GUILD_MEMBER_INFO *pResult = (sTU_GUILD_MEMBER_INFO*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_ADD_MEMBER, INVALID_SERIAL_ID, (void*)&pResult->guildMemberInfo);

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar && pResult->guildMemberInfo.charId == pSobAvatar->GetCharID() )
	{
		SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
		CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
		if( !pLobby )
		{
			DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
			return;
		}

		RwUInt8 bySelectedCharIndex = pLobby->GetSelectedCharacterIndex();

		sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( bySelectedCharIndex );
		if( !pLOBBY_CHARACTER )
		{
			DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
			return;
		}

		RwUInt32 uiMapNameIndex = pLOBBY_CHARACTER->tSummary.dwMapInfoIndex;
		CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_POS, pResult->guildMemberInfo.charId, (void*)&uiMapNameIndex);
	}
}

void PacketHandler_TUGuild_DisbandRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_DISBAND_RES);

	// 길드가 해산 대기가 되었음을 길드장에게 알린다
	sTU_GUILD_DISBAND_RES *pResult = (sTU_GUILD_DISBAND_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_DISBAND_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_BEING_DISBAND, INVALID_SERIAL_ID, (void*)&pResult->timeToDisband);
}

void PacketHandler_TUGuild_BeingDisbanded_Nfy(void *pPacket)
{
	// 길드가 해산 대기중임을 길드 맴버에게 알린다
	sTU_GUILD_BEING_DISBANDED_NFY *pResult = (sTU_GUILD_BEING_DISBANDED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_BEING_DISBAND, INVALID_SERIAL_ID, (void*)&pResult->timeToDisband);
}

void PacketHandler_TUGuild_Disband_Cancel_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_DISBAND_CANCEL_RES);

	// 길드가 해산 대기중에서 취소되었음을 길드장에게 알린다
	sTU_GUILD_DISBAND_CANCEL_RES *pResult = (sTU_GUILD_DISBAND_CANCEL_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_DISBAND_CANCEL_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_CANCLE_DISBAND);
}

void PacketHandler_TUGuild_Disband_Canceled_Nfy(void *pPacket)
{
	// 길드가 해산 대기중에서 취소되었음을 부길드장과 길드원에게 알린다
	CNtlSLEventGenerator::SLGuildEvent(SLGE_CANCLE_DISBAND);
}

void PacketHandler_TUGuild_Disband_Nfy(void *pPacket)
{
	// 길드가 해산되었음을 부길드장과 길드원에게 알린다
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISBAND);

	// 길드가 해산되었습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_DISBAND");
}

void PacketHandler_TUGuild_Invited_Nfy(void *pPacket)
{
	// 길드 가입 권유를 받는다
	sTU_GUILD_INVITED_NFY *pResult = (sTU_GUILD_INVITED_NFY*)pPacket;

	// %s 길드에 가입하시겠습니까?
	GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_INVITE_REQ", FALSE, NULL, pResult->wszGuildName );
}

void PacketHandler_TUGuild_Response_Invitation_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_RESPONSE_INVITATION_RES);

	// 자신의 길드 가입 결과를 알려준다
	sTU_GUILD_RESPONSE_INVITATION_RES *pResult = (sTU_GUILD_RESPONSE_INVITATION_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_RESPONSE_INVITATION_RES"), TRUE);
		return;
	}
}

void PacketHandler_TUGuild_Member_Joined_Nfy(void *pPacket)
{
	// 기존 길드 맴버에게 새로운 길드원이 가입했음을 알린다
	sTU_GUILD_MEMBER_JOINED_NFY *pResult = (sTU_GUILD_MEMBER_JOINED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_ADD_MEMBER, INVALID_SERIAL_ID, (void*)&pResult->memberInfo);

	// %s님이 길드에 가입하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_NEW_MEMBER", FALSE, NULL, pResult->memberInfo.wszMemberName);
}

void PacketHandler_TUGuild_Invitation_Declined_Nfy(void *pPacket)
{
	// 길드 초대에 거절하였다
	sTU_GUILD_INVITATION_DECLINED_NFY *pResult = (sTU_GUILD_INVITATION_DECLINED_NFY*)pPacket;

	// %s님이 길드 가입을 거절하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DECLINE", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_TUGuild_Invitation_Expired_Nfy(void *pPacket)
{
	// 길드 초대에 응답하지 않았다
	sTU_GUILD_INVITATION_EXPIRED_NFY *pResult = (sTU_GUILD_INVITATION_EXPIRED_NFY*)pPacket;

	// %s님 길드 가입에 응답하지 않았습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_EXPIRED", FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_TUGuild_Leave_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_LEAVE_RES);

	// 자신이 길드를 탈퇴하였다
	sTU_GUILD_LEAVE_RES *pResult = (sTU_GUILD_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_LEAVE_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISBAND);
}

void PacketHandler_TUGuild_Member_Left_Nfy(void *pPacket)
{
	// 길드원이 탈퇴하였음을 부길드장과 길드맴버에게 알린다
	sTU_GUILD_MEMBER_LEFT_NFY *pResult = (sTU_GUILD_MEMBER_LEFT_NFY*)pPacket;	

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 길드를 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_LEAVE_MEMBER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DEL_MEMBER, pResult->memberCharId);
}

void PacketHandler_TUGuild_Kick_Out_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_KICK_OUT_RES);

	// 길드원을 추방시켰다
	sTU_GUILD_KICK_OUT_RES *pResult = (sTU_GUILD_KICK_OUT_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_KICK_OUT_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->kickedOutMemberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->kickedOutMemberCharId);
		return;
	}
}

void PacketHandler_TUGuild_Member_Kicked_Out_Nfy(void *pPacket)
{
	// 길드원이 추방되었음을 길드 맴버에게 알린다
	sTU_GUILD_MEMBER_KICKED_OUT_NFY* pResult = (sTU_GUILD_MEMBER_KICKED_OUT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 길드에서 추방되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_KICKOUT_MEMBER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DEL_MEMBER, pResult->memberCharId);
}


void PacketHandler_TUGuild_Appoint_Second_Master_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_APPOINT_SECOND_MASTER_RES);

	// 부길드장이 임명되었음을 길드장에게 알린다
	sTU_GUILD_APPOINT_SECOND_MASTER_RES* pResult = (sTU_GUILD_APPOINT_SECOND_MASTER_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_APPOINT_SECOND_MASTER_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->secondMasterCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->secondMasterCharId);
		return;
	}

	// %s님이 부길드장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_SECOND_MASTER_APPOINTED", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_SECOND_MASTER, pResult->secondMasterCharId);
}

void PacketHandler_TUGuild_Second_Master_Appointed_Nfy(void *pPacket)
{
	// 부길드장이 임명되었음을 부길드장과 길드원에게 알린다
	sTU_GUILD_SECOND_MASTER_APPOINTED_NFY* pResult = (sTU_GUILD_SECOND_MASTER_APPOINTED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 부길드장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_SECOND_MASTER_APPOINTED", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_SECOND_MASTER, pResult->memberCharId);
}

void PacketHandler_TUGuild_DismissSecondMasterRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_DISMISS_SECOND_MASTER_RES);

	// 부길드장의 직위 해제 결과
	sTU_GUILD_DISMISS_SECOND_MASTER_RES* pResult = (sTU_GUILD_DISMISS_SECOND_MASTER_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_DISMISS_SECOND_MASTER_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->secondMasterCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->secondMasterCharId);
		return;
	}

	// %s님이 부길드 마스터에서 일반 길드맴버가 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DISMISS_SECOND_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISMISS_SECOND_MASTER, pResult->secondMasterCharId);
}

void PacketHandler_TUGuild_SecondMasterDismissedNfy(void *pPacket)
{
	// 부길드장이 직위 해제 되었음을 길드 맴버에게 알린다
	sTU_GUILD_SECOND_MASTER_DISMISSED_NFY* pResult = (sTU_GUILD_SECOND_MASTER_DISMISSED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 부길드 마스터에서 일반 길드맴버가 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_DISMISS_SECOND_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DISMISS_SECOND_MASTER, pResult->memberCharId);
}

void PacketHandler_TUGuild_Change_Guild_Master_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_CHANGE_GUILD_MASTER_RES);

	// 길드장이 바뀌었음을 이전 길드장에게 알린다
	sTU_GUILD_CHANGE_GUILD_MASTER_RES* pResult = (sTU_GUILD_CHANGE_GUILD_MASTER_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_CHANGE_GUILD_MASTER_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->newMasterCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->newMasterCharId);
		return;
	}

	// %s님이 길드장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CHANGE_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_MASTER, pResult->newMasterCharId);
}

void PacketHandler_TUGuild_Guild_Master_Changed_Nfy(void *pPacket)
{
	// 길드장이 바뀌었음을 길드 맴버에게 알린다
	sTU_GUILD_GUILD_MASTER_CHANGED_NFY* pResult = (sTU_GUILD_GUILD_MASTER_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 길드장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CHANGE_MASTER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_APPOINT_MASTER, pResult->memberCharId);
}

void PacketHandler_TUGuild_MemberPositionChangedNfy(void* pPacket)
{
	// 길드 맴버의 지역 변경
	sTU_GUILD_MEMBER_POSITION_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_POSITION_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_POS, pResult->memberCharId, (void*)&pResult->newMapNameTblidx);
}

void PacketHandler_TUGuild_MemberClassChangedNfy(void* pPacket)
{
	// 길드 맴버의 클래스 변경
	sTU_GUILD_MEMBER_CLASS_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_CLASS_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 %s 클래스가 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CLASS_CHANGE_MEMBER", FALSE, NULL, pMember->wszMemberName, Logic_GetClassName(pResult->byNewClass));
	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_CLASS, pResult->memberCharId, (void*)&pResult->byNewClass);
}

void PacketHandler_TUGuild_MemberLevelChangedNfy(void* pPacket)
{
	// 길드 맴버의 레벨 변경
	sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 %d 레벨이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_LEVEL_CHANGE_MEMBER", FALSE, NULL, pMember->wszMemberName, pResult->byNewLevel);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_LEVEL, pResult->memberCharId, (void*)&pResult->byNewLevel);
}

void PacketHandler_TUGuild_ReputationChangedNfy(void* pPacket)
{
	// 길드 맴버의 평판 변경
	sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY* pResult = (sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_REPUTATION, pResult->memberCharId, (void*)&pResult->dwNewReputation);
}

void PacketHandler_TUGuild_MemberOnlineNfy(void* pPacket)
{
	// 길드 맴버가 게임에 접속하였다
	sTU_GUILD_MEMBER_ONLINE_NFY* pResult = (sTU_GUILD_MEMBER_ONLINE_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->memberCharId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->memberCharId);
		return;
	}

	// %s님이 접속했습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_ONLINE_MEMBER", FALSE, NULL, pMember->wszMemberName);
	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_ONLINE, pResult->memberCharId, (void*)&pResult->newMapNameTblidx);
}

void PacketHandler_TUGuild_MemberOfflineNfy(void* pPacket)
{
	// 길드 맴버가 게임을 나갔다
	sTU_GUILD_MEMBER_OFFLINE_NFY *pResult = (sTU_GUILD_MEMBER_OFFLINE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_MEM_OFFLINE, pResult->memberCharId);
}

void PacketHandler_GSGuild_NameChanged_Res(void* pPacket)
{
	// 길드의 이름이 변경되었음을 길드장에게 알린다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_CHANGE_NAME_RES);

	Logic_CancelNpcFacing();

	sGU_GUILD_CHANGE_NAME_RES* pResult = (sGU_GUILD_CHANGE_NAME_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CHANGE_NAME_RES"), TRUE );
		return;
	}

	// 길드 이름이 변경되었습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGED_GUILD_NAME");
}

void PacketHandler_GSGuild_NameChanged_Nfy(void* pPacket)
{
	sGU_GUILD_NAME_CHANGED_NFY* pResult = (sGU_GUILD_NAME_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::ChangeGuildName(pResult->hSubject, pResult->wszGuildName);

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	if( pGuild->IsHaveGroup() )
	{
		// 서버쪽 패킷 정리가 될 때까지 막는다
		/*
		// 길드 이름이 변경되었습니다
		GetAlarmManager()->AlarmMessage(DST_GUILD_CHANGED_GUILD_NAME);
		*/
	}
}

void PacketHandler_GSGuild_FunctionAdd_Res(void* pPacket)
{
	// 길드의 기능이 변경되었음을 알린다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_FUNCTION_ADD_RES);

	Logic_CancelNpcFacing();

	sGU_GUILD_FUNCTION_ADD_RES* pResult = (sGU_GUILD_FUNCTION_ADD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_FUNCTION_ADD_RES"), TRUE );
		return;
	}	

	// 실제 기능 변경과 메세지를 보여주는 패킷 : PacketHandler_TUGuild_FunctionChange_Nfy
}

void PacketHandler_TUGuild_FunctionChange_Nfy(void* pPacket)
{
	// Guild or Dojo functions have changed
	sTU_GUILD_FUNCTION_CHANGE_NFY* pResult = (sTU_GUILD_FUNCTION_CHANGE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_CHANGE_FUNCTION, INVALID_DWORD, (VOID*)&pResult->qwGuildFunctionFlag, (void*)&pResult->byNewFunc);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return;

	// The function has changed in the guild
	if(  INVALID_BYTE == pResult->byNewFunc )
	{
		// Existing function removed
	}
	else if( IsGuildFunction((eDBO_GUILD_FUNCTION)pResult->byNewFunc) )
	{
		if (pAvatar->GetGuild()->IsGuildMaster(pAvatar->GetCharID()))
			GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_ADD_FUNCTION_NOTIFY_TO_MASTER", FALSE, NULL, Logic_GetGuildFunctionString(pResult->byNewFunc));
		else
			GetAlarmManager()->AlarmMessage("DST_GUILD_ADD_FUNCTION_NOTIFY_TO_MEMBER");
	}
	else if( IsDojoFunction((eDBO_GUILD_FUNCTION)pResult->byNewFunc) )
	{
		if (pAvatar->GetGuild()->IsGuildMaster(pAvatar->GetCharID()))
			GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_ADD_FUNCTION_NOTIFY_TO_MASTER", FALSE, NULL, Logic_GetGuildFunctionString(pResult->byNewFunc));
		else
			GetAlarmManager()->AlarmMessage("DST_DOJO_ADD_FUNCTION_NOTIFY_TO_MEMBER");
	}
	else
	{
		DBO_FAIL("Invalid guild function: " << pResult->byNewFunc);
	}
}

void PacketHandler_GSGuild_GiveZenny_Res(void* pPacket)
{
	// 제니를 길드에 투자하였다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_GIVE_ZENNY_RES);

	Logic_CancelNpcFacing();

	sGU_GUILD_GIVE_ZENNY_RES* pResult = (sGU_GUILD_GIVE_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_GIVE_ZENNY_RES"), TRUE );
		return;
	}

	// 아직 미구현
	CNtlSLEventGenerator::SLGuildEvent(SLGE_CHANGE_INVEST_ZENNY);

	// 제니를 기부하였습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_CONTRIBUTION_ZENNY_NOTIFY");
}

void PacketHandler_TUGuild_GiveZenny_Nfy(void* pPacket)
{
	// 길드원이 제니를 투자했음을 알린다
	sTU_GUILD_GIVE_ZENNY_NFY* pResult = (sTU_GUILD_GIVE_ZENNY_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild*		pGuild	= pAvatar->GetGuild();
	sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey(pResult->charId) );

	if( !pMember )
	{
		DBO_FAIL("Not exist guild member of charID : " << pResult->charId);
		return;
	}

	// %s님이 %d 제니를 기부하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CONTRIBUTION_ZENNY_OTHERS", FALSE, NULL, pMember->wszMemberName, Logic_FormatZeni(pResult->dwZenny));
}

void PacketHandler_GSGuild_WarehouseStart_Res(void* pPacket)
{
	// 길드 창고를 연다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_START_RES);

	sGU_GUILD_BANK_START_RES* pResult = (sGU_GUILD_BANK_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		Logic_CancelNpcFacing();

		// 원래는 아래의 이벤트를 받아서 처리하는 것이 맞으나 이 시점에는 길드창고GUI클래스 객체가
		// 생성되어 있지 않아서 이벤트를 받았을 때의 처리를 여기에서 해준다.
		// 이벤트를 발생시키는 것은 DIALOGEVENT_OPEN_FAIL_NPC_DIALOG를 발생시키는 다른 로직들과의 통일성 때문이다
		CDboEventGenerator::DialogEvent(DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, DIALOG_GUILD_WAREHOUSEBAR);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_START_RES"), TRUE );
		return;
	}

	CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
	pWorldConceptController->ChangeState(WORLD_NPC_GUILD_WAREHOUSE);

	CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_START, pResult->handle);
}

void PacketHandler_GSGuild_WarehouseMove_Res(void* pPacket)
{
	// 길드 창고의 아이템을 옮긴다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_MOVE_RES);

	sGU_GUILD_BANK_MOVE_RES* pResult = (sGU_GUILD_BANK_MOVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_MOVE_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSLEventGenerator::GuildWarehouseItemMove(pResult->hSrcItem, pResult->hDstItem,
		pResult->bySrcPlace, pResult->bySrcPos, pResult->byDestPlace, pResult->byDestPos);
}

void PacketHandler_GSGuild_WarehouseMoveStack_Res(void* pPacket)
{
	// 길드 창고의 스텍 아이템을 옮긴다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_MOVE_STACK_RES);

	sGU_GUILD_BANK_MOVE_STACK_RES* pResult = (sGU_GUILD_BANK_MOVE_STACK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_MOVE_STACK_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSLEventGenerator::GuildWarehouseItemStackMove(pResult->hSrcItem, pResult->hDestItem,
		pResult->bySrcPlace, pResult->bySrcPos, pResult->byDestPlace, pResult->byDestPos,
		pResult->byStackCount1, pResult->byStackCount2);
}

void PacketHandler_GSGuild_WarehouseZenny_Res(void* pPacket)
{
	// 길드 창고의 제니를 변경한다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_ZENNY_RES);

	sGU_GUILD_BANK_ZENNY_RES* pResult = (sGU_GUILD_BANK_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_ZENNY_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	if( pResult->bIsSave )
	{
		// %d 제니를 저금하였습니다
		GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_WAREHOUSE_SAVE_ZENNY", FALSE, NULL, Logic_FormatZeni(pResult->dwZenny));

		CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_ADD_ZENNY, pResult->dwZenny);
	}
	else
	{
		// %d 제니를 출금하였습니다
		GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_WAREHOUSE_ZENNY_DRAW_OUT", FALSE, NULL, Logic_FormatZeni(pResult->dwZenny));

		CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_SUB_ZENNY, pResult->dwZenny);
	}
}

void PacketHandler_GSGuild_WarehouseZennyInfo(void* pPacket)
{
	// 길드 창고의 제니 정보를 받았다
	sGU_GUILD_BANK_ZENNY_INFO* pResult = (sGU_GUILD_BANK_ZENNY_INFO*)pPacket;

	CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_SET_ZENNY, pResult->dwZenny);
}

void PacketHandler_GSGuild_WarehouseItemInfo(void* pPacket)
{
	// 길드 창고의 아이템 정보를 받았다
	sGU_GUILD_BANK_ITEM_INFO* pResult = (sGU_GUILD_BANK_ITEM_INFO*)pPacket;

	CNtlSLEventGenerator::GuildWarehouseItemInfo(pResult->byPlace, pResult->byItemCount, (VOID*)&pResult->aItemProfile);
}

void PacketHandler_GSGuild_WarehouseEnd_Res(void* pPacket)
{
	// 길드 창고를 닫았다
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_BANK_END_RES);

	sGU_GUILD_BANK_END_RES* pResult = (sGU_GUILD_BANK_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_BANK_END_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::GuildWarehouseEvent(GUILD_WAREHOUSE_EVENT_END);
}

void PacketHandler_TUGuild_Reputation_Change_Nfy(void* pPacket)
{
	// 길드의 명성이 변경되었다
	sTU_GUILD_REPUTATION_CHANGE_NFY* pResult = (sTU_GUILD_REPUTATION_CHANGE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_CHANGE_REPUTATION, INVALID_DWORD, (VOID*)&pResult->dwReputation, (VOID*)&pResult->dwMaxGuildPointEver);
}

void PacketHandler_TUGuild_ChangeNotice_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_GUILD_CHANGE_NOTICE_RES);

	// 길드 공지가 변경되었음을 공지를 변경한 사람에게 알린다
	sTU_GUILD_CHANGE_NOTICE_RES* pResult = (sTU_GUILD_CHANGE_NOTICE_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_GUILD_CHANGE_NOTICE_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_GUILD_NOTICE);

	// 길드 공지를 발송하였습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGED_GUILD_NOTICE_SUCCESS");
}

void PacketHandler_TUGuild_changeNotice_Nfy(void* pPacket)
{
	// 길드 공지가 변경되었음을 길드원에게 알린다
	sTU_GUILD_CHANGE_NOTICE_NFY* pResult = (sTU_GUILD_CHANGE_NOTICE_NFY*)pPacket;

	CNtlSLEventGenerator::SLGuildEvent(SLGE_NOTICE, pResult->wNoticeLengthInUnicode, 
										pResult->awchCharName,
										pResult->awchNotice);

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	if( pGuild->IsHaveGroup() )
	{
		// 새로운 길드 공지가 있습니다
		GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGED_GUILD_NOTICE");
	}	
}

void PacketHandler_GSGuild_Create_Emblem_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_CREATE_MARK_RES);

	// 길드 문장을 생성하였다
	sGU_GUILD_CREATE_MARK_RES* pResult = (sGU_GUILD_CREATE_MARK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CREATE_MARK_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_EMBLEM_MAKER);

	// 길드 문장이 생성되었습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_CREATE_GUILD_EMBLEM");	
}

void PacketHandler_GSGuild_Change_Emblem_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_CHANGE_MARK_RES);

	// 길드 문장을 변경하였다
	sGU_GUILD_CHANGE_MARK_RES* pResult = (sGU_GUILD_CHANGE_MARK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_CHANGE_MARK_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_EMBLEM_MAKER);

	// 길드 문장이 변경되었습니다
	GetAlarmManager()->AlarmMessage("DST_GUILD_CHANGE_GUILD_EMBLEM");	
}

void PacketHandler_GSGuild_Change_Emblem_Nfy(void* pPacket)
{
	sGU_GUILD_MARK_CHANGED_NFY* pResult = (sGU_GUILD_MARK_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::ChangeGuildEmblem(pResult->hSubject,
											pResult->sMark.byMarkMain, pResult->sMark.byMarkMainColor,
											pResult->sMark.byMarkInLine, pResult->sMark.byMarkInColor,
											pResult->sMark.byMarkOutLine, pResult->sMark.byMarkOutColor);

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	if( pGuild->IsHaveGroup() )
	{
		// 서버쪽 패킷 정리가 될 때까지 막는다
		/*
		if( pGuild->IsHaveEmblem() )
			// 길드 문장이 변경되었습니다
			GetAlarmManager()->AlarmMessage(DST_GUILD_CHANGE_GUILD_EMBLEM);	
		else
			// 길드 문장이 생성되었습니다
			GetAlarmManager()->AlarmMessage(DST_GUILD_CREATE_GUILD_EMBLEM);	
			*/

		CNtlSLEventGenerator::NotifyGuildEvent(SLGE_CHANGE_GUILD_EMBLEM);
	}
}


//////////////////////////////////////////////////////////////////////////
//								도 장									//
//////////////////////////////////////////////////////////////////////////

void PacketHandler_GSDojo_CreateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_CREATE_RES);

	Logic_CancelNpcFacing();

	// 도장을 설립하였음을 유파장에게 알린다
	sGU_DOJO_CREATE_RES* pResult = (sGU_DOJO_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_CREATE_RES"), TRUE );
		return;
	}
}

void PacketHandler_TUDojo_CreateNfy(void* pPacket)
{
	// 도장을 설립했음을 모든 유파원에게 알린다
	//sTU_DOJO_CREATED_NFY* pResult = (sTU_DOJO_CREATED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild* pGuild = pAvatar->GetGuild();
	GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SUCCESS_ESTABLISHMENT", FALSE, NULL, pGuild->GetGuildName() );


	// 기존에 설정된 도장 도복 색상이 있다면 적용한다
	RwUInt8 byColorIndex = pGuild->GetDojoDogiColorIndex();
	if( INVALID_BYTE == byColorIndex )
		return;

	Logic_SetDogiItemColor(pAvatar, byColorIndex);

	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
	if( pSobGroup )
	{					
		COMMUNITY_ITER	it_Begin	= pGuild->Begin();
		COMMUNITY_ITER	it_End		= pGuild->End();

		for( ; it_Begin != it_End ; ++it_Begin )
		{
			sGuildMember* pGuildMember = reinterpret_cast<sGuildMember*>( *it_Begin );
			CNtlSobGroup::MapObject::iterator it;

			for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
			{
				CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );

				if( pSobPlayer->GetCharID() == pGuildMember->charID )
				{
					// 도복 색상
					Logic_SetDogiItemColor(pSobPlayer, byColorIndex);
					break;
				}
			}
		}
	}
}

void PacketHandler_TUDojo_DestoryNfy(void* pPacket)
{
	// 유파를 해체하여 도장의 소유권을 잃어버렸음을 모든 유파원에게 알린다
	sTU_DOJO_DESTROYED_NFY* pResult = (sTU_DOJO_DESTROYED_NFY*)pPacket;

	sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(pResult->dojoTblidx) );
	if(!pDOJO_TBLDAT)
	{
		DBO_FAIL("Not exist dojo table of index : " << pResult->dojoTblidx);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_LOST_DOJO");

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_LOST_DOJO, pResult->dojoTblidx);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild* pGuild = pAvatar->GetGuild();

	// 기존에 설정된 유파 도복 색상이 있다면 적용한다
	RwUInt8 byColorIndex = pGuild->GetGuildDogiColorIndex();
	if( INVALID_BYTE == byColorIndex )
		return;

	Logic_SetDogiItemColor(pAvatar, byColorIndex);

	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
	if( pSobGroup )
	{					
		COMMUNITY_ITER	it_Begin	= pGuild->Begin();
		COMMUNITY_ITER	it_End		= pGuild->End();

		for( ; it_Begin != it_End ; ++it_Begin )
		{
			sGuildMember* pGuildMember = reinterpret_cast<sGuildMember*>( *it_Begin );
			CNtlSobGroup::MapObject::iterator it;

			for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
			{
				CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );

				if( pSobPlayer->GetCharID() == pGuildMember->charID )
				{
					// 도복 색상
					Logic_SetDogiItemColor(pSobPlayer, byColorIndex);
					break;
				}
			}
		}
	}
}

void PacketHandler_GSDojo_FunctionAddRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_FUNCTION_ADD_RES);

	// 도장 기능을 추가하였음을 유파장에게 알린다
	sGU_DOJO_FUNCTION_ADD_RES* pResult = (sGU_DOJO_FUNCTION_ADD_RES*)pPacket;

	switch( pResult->byFunction )
	{
//	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_1:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_2:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_3:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_4:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_5:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_6:
	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_7:
		{
			break;
		}
	default:
		{
		Logic_CancelNpcFacing();
			break;
		}
	}

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_FUNCTION_ADD_RES"), TRUE );
		return;
	}

	// 실제 기능 변경과 메세지를 보여주는 패킷 : PacketHandler_TUGuild_FunctionChange_Nfy
}

void PacketHandler_GSDojo_LevelChangedNfy(void* pPacket)
{
	// 유저에게 도장 레벨이 변경되었음을 알린다
	sGU_DOJO_LEVEL_CHANGED_NFY* pResult = (sGU_DOJO_LEVEL_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_CHANGE_LEVEL, pResult->dojoTblidx, (VOID*)&pResult->byLevel);

	{
		CNtlPLVisualManager* pVisualMng = (CNtlPLVisualManager*)GetSceneManager();

		if ( GAMERULE_DOJO == Logic_GetActiveWorldRuleType() )
		{
			pVisualMng->GetDojoContainer()->SetDojoWorld_Level( pResult->byLevel );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
		else
		{
			pVisualMng->GetDojoContainer()->SetDojo_Level( pResult->dojoTblidx, pResult->byLevel );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
	}
}

void PacketHandler_GSDojo_MarkChangedNfy(void* pPacket)
{
	// 유저에게 도장에 붙여있는 엠블렘 정보가 변경되었음을 알린다
	sGU_DOJO_MARK_CHANGED_NFY* pResult = (sGU_DOJO_MARK_CHANGED_NFY*)pPacket;

	{
		CNtlPLVisualManager* pVisualMng = (CNtlPLVisualManager*)GetSceneManager();

		sEmblemFactor sEmFactor;

		sEmFactor.byTypeA		= pResult->sMark.byMarkMain;
		sEmFactor.byTypeB		= pResult->sMark.byMarkInLine;
		sEmFactor.byTypeC		= pResult->sMark.byMarkOutLine;

		sEmFactor.byTypeAColor	= pResult->sMark.byMarkMainColor;
		sEmFactor.byTypeBColor	= pResult->sMark.byMarkInColor;
		sEmFactor.byTypeCColor	= pResult->sMark.byMarkOutColor;

		if ( GAMERULE_DOJO == Logic_GetActiveWorldRuleType() )
		{
			pVisualMng->GetDojoContainer()->SetDojoWorld_Emblem( &sEmFactor );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
		else
		{
			pVisualMng->GetDojoContainer()->SetDojo_Emblem( pResult->dojoTblidx, &sEmFactor );
			pVisualMng->GetDojoContainer()->RefreshDojo();
		}
	}
}

void PacketHandler_GSDojo_ScrambleRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_SCRAMBLE_RES);

	// 쟁탈전을 신청한 길드 마스터에게 응답
	sGU_DOJO_SCRAMBLE_RES* pResult = (sGU_DOJO_SCRAMBLE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_SCRAMBLE_RES"), TRUE );
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_ASK_SCRAMBLE");
}

void PacketHandler_TUDojo_ScrambleNfy(void* pPacket)
{
	// 쟁탈전을 신청받은 길드 마스터에게 쟁탈전 수락 여부를 물어본다
	sTU_DOJO_SCRAMBLE_NFY* pResult = (sTU_DOJO_SCRAMBLE_NFY*)pPacket;

	GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE", FALSE, NULL, pResult->wszName, DBO_DOJO_SCRAMBLE_REJECT_FARE);
}

void PacketHandler_TUDojo_ScrambleReceiveNfy(void* pPacket)
{
	// 쟁탈전을 신청한 길드 마스터에게 쟁탈전 신청을 받은 길드 마스터의 응답 결과를 알린다
	sTU_DOJO_SCRAMBLE_RECEIVE_NFY* pResult = (sTU_DOJO_SCRAMBLE_RECEIVE_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlDojo* pDojo = pAvatar->GetDojo();
	sDOJO_INFO* pDOJO_INFO = pDojo->GetDojoInfo( pResult->dojoTblidx );

	if( !pDOJO_INFO )
	{
		DBO_FAIL("Invalid dojo info of dojo table index : " << pResult->dojoTblidx);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_ACCEPTED_SCRAMBLE");

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_ACCEPT, pResult->dojoTblidx, (VOID*)&pResult->guildId);
}

void PacketHandler_TUDojo_ScrambleReceiveInfoNfy(void* pPacket)
{
	// 쟁탈전에 참가하는 유파의 모든 유파원들이 쟁탈전 기본 정보를 받았다
	sTU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY* pResult = (sTU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY*)pPacket;

	CNtlSLEventGenerator::ScrambleDefaultInfo(pResult->dojoTblidx, pResult->defGuildId, pResult->attGuildId,
											  pResult->wszDefGuildName, pResult->wszAttGuildName,
											  (VOID*)&pResult->sDefMark, (VOID*)&pResult->sAttMark);
}

void PacketHandler_GSDojo_ScrambleResponseRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_SCRAMBLE_RESPONSE_RES);

	// 쟁탈전을 신청받은 길드 마스터가 대답한 결과
	sGU_DOJO_SCRAMBLE_RESPONSE_RES* pResult = (sGU_DOJO_SCRAMBLE_RESPONSE_RES*)pPacket;

	if( pResult->bIsRetry )
	{
		GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE", FALSE, NULL, pResult->wszGuildName, DBO_DOJO_SCRAMBLE_REJECT_FARE);
	}

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_SCRAMBLE_RESPONSE_RES"), TRUE );
		return;
	}
}

void PacketHandler_GSDojo_NPCInfoRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOJO_NPC_INFO_RES);

	// 도장 정보창의 정보를 받았다
	sGU_DOJO_NPC_INFO_RES* pResult = (sGU_DOJO_NPC_INFO_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOJO_NPC_INFO_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_NPC_GUI_INFO, pResult->sData.dojoTblidx, (VOID*)&pResult->sData);
}

void PacketHandler_GSDojo_SealCurrentStateNfy(void* pPacket)
{
	// 도장 쟁탈전 진입시 초기 인장 정보를 받는다
	sGU_DOJO_SEAL_CURRENT_STATE_NFY* pResult = (sGU_DOJO_SEAL_CURRENT_STATE_NFY*)pPacket;

	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SEAL_ATTACK_STATE, pResult->sealObjectTblidx,
									(VOID*)&pResult->byState, (VOID*)&pResult->vLoc);
}
}

void PacketHandler_GSDojo_SealAttackBeginNfy(void* pPacket)
{
	// 도장 쟁탈전중 인장을 뺏기 위한 캐스팅을 시작한다
	sGU_DOJO_SEAL_ATTACK_BEGIN_NFY* pResult = (sGU_DOJO_SEAL_ATTACK_BEGIN_NFY*)pPacket;

	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SEAL_ATTACK_BEGIN, pResult->sealObjectTblidx,
										  (VOID*)&pResult->bIsBeingStolen, (VOID*)&pResult->vLoc);
}
}

void PacketHandler_GSDojo_SealAttackEndNfy(void* pPacket)
{
	// 도장 쟁탈전중 인장을 뺏기 위한 캐스팅을 종료한다.
	sGU_DOJO_SEAL_ATTACK_END_NFY* pResult = (sGU_DOJO_SEAL_ATTACK_END_NFY*)pPacket;

	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
	{
		CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SEAL_ATTACK_END, pResult->sealObjectTblidx);
	}
}

void PacketHandler_GSDojo_ScrambleShiftStateNfy(void* pPacket)
{
	// 도장 쟁탈전중 인장의 상태가 변경되었음을 공/방 유파원에게 알린다
	sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY* pResult = (sGU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY*)pPacket;

	if( FALSE == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
		return;


	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER, pResult->dojoTblidx, (VOID*)&pResult->objectTblidx, (VOID*)&pResult->guildId);


	CObjectTable* pObjectTable = API_GetTableContainer()->GetObjectTable( Logic_GetActiveWorldId() );
	if( pObjectTable == NULL )
	{
		DBO_FAIL("Not exist obejct table of world ID : " << Logic_GetActiveWorldId());
		return;
	}

	sOBJECT_TBLDAT* pOBJECT_TBLDAT		= reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTable->FindData(pResult->objectTblidx) );
	if( pOBJECT_TBLDAT )
	{
		CTextTable* pObjectTextTable	= API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
		std::wstring wstrText			= pObjectTextTable->GetText(pOBJECT_TBLDAT->dwName);
		const WCHAR* pwcText			= GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_CHANGE_SEAL_OWNER");
		if( pwcText )
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_CHANGE_SEAL_OWNER", FALSE, NULL, pResult->wszName, wstrText.c_str());
		}		
	}	
}

void PacketHandler_TUDojo_BriefNfy(void* pPacket)
{
	// (서버의 모든 유저에게)도장 정보를 받았다
	sTU_DOJO_BRIEF_NFY* pResult = (sTU_DOJO_BRIEF_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_DOJO_INFO, 0xffffffff, (VOID*)&pResult->sDojoData);
}

void PacketHandler_TUDojo_ScrambleRejectNfy(void* pPacket)
{
	// 쟁탈전을 신청한 길드 마스터에게 거절당했음을 알린다
	//sTU_DOJO_SCRAMBLE_REJECT_NFY* pResult = (sTU_DOJO_SCRAMBLE_REJECT_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage("DST_DOJO_REJECT_SCRAMBLE");
}

void PacketHandler_TUDojo_ScrambleInformNfy(void* pPacket)
{
	// 쟁탈전을 신청한 길드 마스터와 쟁탈전을 신청받은 길드 마스터에게
	// 도장의 상태와 정보를 보낸다
	sTU_DOJO_SCRAMBLE_INFORM_NFY* pResult = (sTU_DOJO_SCRAMBLE_INFORM_NFY*)pPacket;
	CNtlSLEventGenerator::DojoState(pResult->dojoTblidx, pResult->byState, pResult->tmNextStepTime);
}

void PacketHandler_TUDojo_AddBudokaiSeedAddRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_DOJO_BUDOKAI_SEED_ADD_RES);

	// 부도카이 시드 참가자를 추가한 결과를 유파장에게 알린다
	sTU_DOJO_BUDOKAI_SEED_ADD_RES* pResult = (sTU_DOJO_BUDOKAI_SEED_ADD_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_DOJO_BUDOKAI_SEED_ADD_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_CHANGE_TENKAI_SEED_PLAYER, pResult->dojoTblidx, (VOID*)pResult->wszCharName);
}

void PacketHandler_TUDojo_ScramblePointNfy(void* pPacket)
{
	// 도장 쟁탈전 점수를 유파원에게 알린다
	sTU_DOJO_SCRAMBLE_POINT_NFY* pResult = (sTU_DOJO_SCRAMBLE_POINT_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_SCORE, pResult->sData.dojoTblidx, (VOID*)&pResult->sData);
}

void PacketHandler_TUDojo_NoticeChangeRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(TU_DOJO_NOTICE_CHANGE_RES);

	// 도장 공지를 변경한 결과를 유파장에게 알려준다
	sTU_DOJO_NOTICE_CHANGE_RES* pResult = (sTU_DOJO_NOTICE_CHANGE_RES*)pPacket;

	if( pResult->wResultCode != CHAT_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "TU_DOJO_NOTICE_CHANGE_RES"), TRUE );
		CDboEventGenerator::DialogPostEvent(DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE);
		return;
	}

	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_GUILD_NOTICE);
}

void PacketHandler_TUDojo_NoticeChangeNfy(void* pPacket)
{
	// 도장 공지가 변경되었음을 유파장에게 알린다
	// 기획이 변경되어 다른 사람도 도장 공지가 변경되었음을 알게 될 수도 있다. 이 때 별도 작업 불필요
	sTU_DOJO_NOTICE_CHANGE_NFY* pResult = (sTU_DOJO_NOTICE_CHANGE_NFY*)pPacket;

	if( NTL_MAX_LENGTH_OF_GUILD_NOTICE < pResult->wNoticeLengthInUnicode )
		pResult->wNoticeLengthInUnicode = NTL_MAX_LENGTH_OF_GUILD_NOTICE;

	WCHAR acNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
	ZeroMemory(acNotice, sizeof(acNotice));
	memcpy(acNotice, pResult->awchNotice, sizeof(WCHAR)*pResult->wNoticeLengthInUnicode);
	
	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_CHANGE_DOJO_NOTICE, pResult->dojoTblidx,
									(VOID*)pResult->awchCharName, (VOID*)acNotice);
}

void PacketHandler_TUDojo_ScrambleResultNfy(void* pPacket)
{
	// 도장 쟁탈전 결과를 유파원들에게 알린다
	sTU_DOJO_SCRAMBLE_RESULT_NFY* pResult = (sTU_DOJO_SCRAMBLE_RESULT_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_RESULT, pResult->winGuildId);
}

void PacketHandler_TUDojo_ScrambleRewardNfy(void* pPacket)
{
	// 도장 쟁탈전에 이긴 유파원들에게 보상을 알린다
	sTU_DOJO_SCRAMBLE_REWARD_NFY* pResult = (sTU_DOJO_SCRAMBLE_REWARD_NFY*)pPacket;

	CNtlSLEventGenerator::DojoEvent(DOJO_EVENT_SCRAMBLE_REWARD, pResult->dwAddGuildReputation,
									(VOID*)&pResult->itemTblidx, (VOID*)&pResult->byCount);	
}

//////////////////////////////////////////////////////////////////////////
//								도 복									//
//////////////////////////////////////////////////////////////////////////

void PacketHandler_GSDogi_CreateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOGI_CREATE_RES);

	// 유파 도복을 만들었음을 유파장에게 알린다
	sGU_DOGI_CREATE_RES* pResult = (sGU_DOGI_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOGI_CREATE_RES"), TRUE );
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOGI_CHANGED");
	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}

void PacketHandler_GSDogi_ChangeRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DOGI_CHANGE_RES);

	// 유파 도복을 변경했음을 유파장에게 알린다
	sGU_DOGI_CHANGE_RES* pResult = (sGU_DOGI_CHANGE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DOGI_CHANGE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}

void PacketHandler_GSDogi_UpdateNfy(void* pPacket)
{
	// 어떤 유파의 도복에 변화가 있음을 주변 유저에게 알린다
	sGU_DOGI_UPDATE_NFY* pResult = (sGU_DOGI_UPDATE_NFY*)pPacket;

	if( Logic_GetAvatarHandle() == pResult->handle )
	{
		CNtlSobAvatar*	pAvatar			= GetNtlSLGlobal()->GetSobAvatar();
		CNtlGuild*		pGuild			= pAvatar->GetGuild();
		sDBO_DOGI_DATA&	rDBO_DOGI_DATA	= pResult->sData;

		if( pGuild->GetGuildID() == rDBO_DOGI_DATA.guildId )
		{
			CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI_NFY, INVALID_SERIAL_ID, (void*)&rDBO_DOGI_DATA);
		}
	}

	CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pResult->handle) );
	if( !pSobPlayer )
	{
		DBO_FAIL("Not exist sob player of handle : " << pResult->handle);
		return;
	}

	pSobPlayer->SetGuildDogiColorIndex(pResult->sData.byGuildColor);
	pSobPlayer->SetDojoDogiColorIndex(pResult->sData.byDojoColor);
	Logic_SetDogiItemColor(pSobPlayer, pResult->sData.byGuildColor);		
}

void PacketHandler_GSGuildDogi_CreateRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_DOGI_CREATE_RES);

	// 유파 도복을 만들었음을 유파장에게 알린다
	sGU_GUILD_DOGI_CREATE_RES* pResult = (sGU_GUILD_DOGI_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_DOGI_CREATE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}

void PacketHandler_GSGuildDogi_ChangeRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_GUILD_DOGI_CHANGE_RES);

	// 유파 도복을 변경했음을 유파장에게 알린다
	sGU_GUILD_DOGI_CHANGE_RES* pResult = (sGU_GUILD_DOGI_CHANGE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GUILD_DOGI_CHANGE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::SLGuildEvent(SLGE_DOGI);
}
