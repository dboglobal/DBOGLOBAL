#include "precomp_dboclient.h"
#include "ChatReceiver.h"

// core

// simulation
#include "NtlSobManager.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSLEventFunc.h"

// dbo
#include "DboEventGenerator.h"
#include "ChatDisplayGui.h"
#include "AlarmManager.h"
#include "ChatBalloonManager.h"
#include "DboLogic.h"
#include "DiceManager.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"



CChatReceiver::CChatReceiver()
{

}

CChatReceiver::~CChatReceiver()
{

}

RwBool CChatReceiver::Create(CChatGui* pChatGui)
{
	NTL_FUNCTION( "CChatReceiver::Create" );

	m_pChatGui = pChatGui;

	LinkMsg(g_EventSayMessage);
	LinkMsg(g_EventDiceResultShow);
	LinkMsg(g_EventSkillInfoText);
	LinkMsg(g_EventSobBattleNotification);

	NTL_RETURN(TRUE);
}

VOID CChatReceiver::Destroy()
{
	NTL_FUNCTION("CChatReceiver::Destroy");


	ITER_CHATLOG it_chatLog = m_listChatLog.begin();
	for( ; it_chatLog != m_listChatLog.end() ; ++it_chatLog )
	{
		sChatLog* pChatLog = *it_chatLog;
		NTL_DELETE(pChatLog);
	}
	m_listChatLog.clear();

	m_listNotify.clear();


	UnLinkMsg(g_EventSayMessage);
	UnLinkMsg(g_EventDiceResultShow);
	UnLinkMsg(g_EventSkillInfoText);
	UnLinkMsg(g_EventSobBattleNotification);

	NTL_RETURNVOID();
}

ITER_CHATLOG CChatReceiver::Begin()
{
	return m_listChatLog.begin();
}

ITER_CHATLOG CChatReceiver::End()
{
	return m_listChatLog.end();
}

VOID CChatReceiver::RegisterNotify(CChatDisplayGui* pDisplayDialog)
{
	m_listNotify.push_back(pDisplayDialog);
}

VOID CChatReceiver::UnregisterNotify(CChatDisplayGui* pDisplayDialog)
{
	ITER_NOTIFY it_notify = m_listNotify.begin();
	for( ; it_notify != m_listNotify.end() ; ++it_notify )
	{
		CChatDisplayGui* pRegDisplayGui = *it_notify;

		if( pRegDisplayGui == pDisplayDialog )
		{
			m_listNotify.erase(it_notify);
			return;
		}
	}
}

VOID CChatReceiver::Chatlog_Save(RwUInt8 byType, const WCHAR* pText)
{
	if( m_listChatLog.size() > CHAT_DISP_MAX_LINE )
	{
		ITER_CHATLOG it_chatLog = m_listChatLog.begin();
		sChatLog* pDelChatLog = *it_chatLog;

		NTL_DELETE(pDelChatLog);
		m_listChatLog.erase(it_chatLog);
	}

	sChatLog* pNewChatLog = NTL_NEW sChatLog;
	pNewChatLog->wstrString	= pText;
	pNewChatLog->byChatType	= byType;

	m_listChatLog.push_back(pNewChatLog);	
}

VOID CChatReceiver::Chatlog_Notify(RwUInt8 byType, const WCHAR* pwcText)
{
	ITER_NOTIFY it_notify = m_listNotify.begin();
	for( ; it_notify != m_listNotify.end() ; ++it_notify )
	{
		CChatDisplayGui* pChatDisplayGui = *it_notify;
		pChatDisplayGui->AddText(byType, pwcText);
	}	
}

VOID CChatReceiver::Chatlog_Notify(RwUInt8 byType, const WCHAR * pwcText, unsigned int uiColor)
{
	ITER_NOTIFY it_notify = m_listNotify.begin();
	for (; it_notify != m_listNotify.end(); ++it_notify)
	{
		CChatDisplayGui* pChatDisplayGui = *it_notify;
		pChatDisplayGui->AddText(byType, pwcText, uiColor);
	}
}

const WCHAR * CChatReceiver::GetMsgTypeString(BYTE byMsgType, BYTE byChannelID)
{
	switch (byMsgType)
	{
		case CHAT_TYPE_TRADE:
		{
			static WCHAR wchBuffer[128];

			swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, GetDisplayStringManager()->GetString("DST_CHAT_MODE_TRADE_EXTEND"), GetChannelString(byChannelID));

			return wchBuffer;
		}
		break;
		case CHAT_TYPE_FIND_PARTY:
		{
			static WCHAR wchBuffer[128];

			swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, GetDisplayStringManager()->GetString("DST_CHAT_MODE_FIND_PARTY_EXTEND"), GetChannelString(byChannelID));

			return wchBuffer;
		}
		break;
		case CHAT_TYPE_GUILD:
		{
			return GetDisplayStringManager()->GetString("DST_CHAT_MODE_GUILD");
		}
		break;
		case CHAT_TYPE_PARTY:
		{
			return GetDisplayStringManager()->GetString("DST_CHAT_MODE_PARTY");
		}
		break;
		case CHAT_TYPE_WHISPER:
		{
			return GetDisplayStringManager()->GetString("DST_CHAT_MODE_WHISPER");
		}
		break;
		case CHAT_TYPE_SHOUT:
		{
			return GetDisplayStringManager()->GetString("DST_CHAT_MODE_SHOUT");
		}
		break;

		default: break;
	}

	return NULL;
}

const WCHAR * CChatReceiver::GetChannelString(BYTE byChannelID)
{
	static WCHAR wchBuffer[128];

	swprintf_s(wchBuffer, 64, L"%s %d", GetDisplayStringManager()->GetString("DST_LOBBY_CHANNEL"), byChannelID);

	return wchBuffer;
}

void CChatReceiver::BattleNotification(RWS::CMsg& msg)
{
	SNtlEventSob* pData = (SNtlEventSob*)msg.pData;

	CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pData->hSerialId)); // caster handle
	if (pActor == NULL)
		return;

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	CNtlAttackData* pAttData = pBeData->GetAttackData();
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();
	SAttackStuff* pAttStuff = pAttData->GetAttackStuff(0);

	bool bIsViewer = (pData->hSerialId != Logic_GetAvatarHandle()) && (pAttStuff->hDefenderSerialId != Logic_GetAvatarHandle()); // somone who is viewing the fight
	/*
		Only show message if 
		- We are attacker or defender
		- We have attacker or defender as target
	*/

	if (bIsViewer && (pData->hSerialId != Logic_GetAvatarTargetHandle() && pAttStuff->hDefenderSerialId != Logic_GetAvatarTargetHandle())) // check if viewer & check if viewer has someone as target
		return;

	bool bIsAvatarAttacker = pAttStuff->hAttackerSerialId == Logic_GetAvatarHandle();
	bool bViewAttacker = false;

	const WCHAR* pwchAttackerName = Logic_GetName(pActor);
	const WCHAR* pwchTargetName = NULL;

	CNtlSobActor* pActorTarget = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pAttStuff->hDefenderSerialId)); // target actor can be null
	if (pActorTarget)
	{
		pwchTargetName = Logic_GetName(pActorTarget);
	}

	WCHAR buf[256] = { 0, };
	unsigned int uiColor = INFOCOLOR_3;

	if (bIsViewer && pAttStuff->hAttackerSerialId == Logic_GetAvatarTargetHandle())
		bViewAttacker = true;

	switch (pAttStuff->uiType.bitResult)
	{
		case NTL_ATTACK_RESULT_SUCCESS:
		{
			if (bIsAvatarAttacker || bViewAttacker)
				uiColor = INFOCOLOR_13;
			else
				uiColor = INFOCOLOR_5;

			if (pAttStuff->uiType.bitCritical == NTL_ATTACK_ATTR_CRITICAL)
			{
				if (pAttStuff->uiType.bitLogic == NTL_ATTACK_LOGIC_SKILL)
				{
					
				}
				else
				{
					if (bIsAvatarAttacker || bViewAttacker)
						swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_CRITICAL_ATTACK_ATTACKER"), pwchTargetName, pAttStuff->sDamage.iDamage);
					else
						swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_CRITICAL_ATTACK_DEFENDER"), pwchAttackerName, pAttStuff->sDamage.iDamage);
				}
			}
			else
			{
				if (pAttStuff->uiType.bitLogic == NTL_ATTACK_LOGIC_SKILL)
				{
					
				}
				else
				{
					if (bIsAvatarAttacker || bViewAttacker)
						swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_NORMAL_ATTACK_ATTACKER"), pwchTargetName, pAttStuff->sDamage.iDamage);
					else
						swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_NORMAL_ATTACK_DEFENDER"), pwchAttackerName, pAttStuff->sDamage.iDamage);
				}
			}

			Chatlog_Save(CHAT_TYPE_SYSTEM, buf);
			Chatlog_Notify(CHAT_TYPE_SYSTEM, buf, uiColor);
		}
		break;
		case NTL_ATTACK_RESULT_BLOCK:
		{
			if (bIsAvatarAttacker || bViewAttacker)
			{
				uiColor = INFOCOLOR_5;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_BLOCK_ATTACK_ATTACKER"), pwchTargetName);
			}
			else
			{
				uiColor = INFOCOLOR_13;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_BLOCK_ATTACK_DEFENDER"), pwchAttackerName);
			}

			Chatlog_Save(CHAT_TYPE_SYSTEM, buf);
			Chatlog_Notify(CHAT_TYPE_SYSTEM, buf, uiColor);
		}
		break;
		case NTL_ATTACK_RESULT_DODGE:
		{
			if (bIsAvatarAttacker || bViewAttacker)
			{
				uiColor = INFOCOLOR_5;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_DODGE_ATTACK_ATTACKER"), pwchTargetName);
			}
			else
			{
				uiColor = INFOCOLOR_13;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_DODGE_ATTACK_DEFENDER"), pwchAttackerName);
			}

			Chatlog_Save(CHAT_TYPE_SYSTEM, buf);
			Chatlog_Notify(CHAT_TYPE_SYSTEM, buf, uiColor);
		}
		break;
		case NTL_ATTACK_RESULT_KNOCKDOWN:
		{
			if (bIsAvatarAttacker || bViewAttacker)
			{
				uiColor = INFOCOLOR_5;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_KNOCKDOWN_ATTACKER"), pwchTargetName);
			}
			else
			{
				uiColor = INFOCOLOR_13;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_KNOCKDOWN_DEFENDER"), pwchTargetName, pwchAttackerName);
			}

			Chatlog_Save(CHAT_TYPE_SYSTEM, buf);
			Chatlog_Notify(CHAT_TYPE_SYSTEM, buf, uiColor);
		}
		break;
		case NTL_ATTACK_RESULT_SLIDING:
		{

		}
		break;
		case NTL_ATTACK_RESULT_HEALING:
		{

		}
		break;
		case NTL_ATTACK_RESULT_BUFF:
		{

		}
		break;
		case NTL_ATTACK_RESULT_STEAL:
		{

		}
		break;
		case NTL_ATTACK_RESULT_IMMUNE:
		{

		}
		break;
		case NTL_ATTACK_RESULT_RESIST:
		{
			if (bIsAvatarAttacker || bViewAttacker)
			{
				uiColor = INFOCOLOR_5;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_RESISTED_ATTACK_ATTACKER"), pwchTargetName);
			}
			else
			{
				uiColor = INFOCOLOR_13;
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_RESISTED_ATTACK_DEFENDER"), pwchAttackerName);
			}

			Chatlog_Save(CHAT_TYPE_SYSTEM, buf);
			Chatlog_Notify(CHAT_TYPE_SYSTEM, buf, uiColor);
		}
		break;

		default: DBO_WARNING_MESSAGE("Unknown bitResult: " << pAttStuff->uiType.bitResult); break;
	}
}

VOID CChatReceiver::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CChatReceiver::HandleEvents");

	static WCHAR wchBuffer[CHAT_MSG_BUFFER_LEN];

	if( msg.Id == g_EventSayMessage )
	{
		SDboEventSayMessage* pSayMessage = reinterpret_cast<SDboEventSayMessage*>( msg.pData ); 		

		memset( wchBuffer, 0, sizeof( wchBuffer ) );

		bool bBalloon = false;
		const WCHAR* wchMsgType = GetMsgTypeString(pSayMessage->byMsgType, pSayMessage->byChannel);

		if( pSayMessage->byMsgType == CHAT_TYPE_SYSTEM )
		{
			swprintf_s( wchBuffer, CHAT_MSG_BUFFER_LEN, L"%s", pSayMessage->wchMessage );
		}
		else if (!wchMsgType) // "all" message
		{
			swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, L"%s: %s", pSayMessage->wchUserName, pSayMessage->wchMessage);
			bBalloon = true;
		}
		else if (pSayMessage->byMsgType == CHAT_TYPE_WHISPER)
		{
			if (pSayMessage->uiSerial == 1) // me -> other
			{
				swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, GetDisplayStringManager()->GetString("DST_CHAT_TYPE_WHISPER_TARGET"), wchMsgType, pSayMessage->wchUserName, pSayMessage->wchMessage);
			}
			else // other -> me
			{
				swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, GetDisplayStringManager()->GetString("DST_CHAT_TYPE_WHISPER_AVATER"), wchMsgType, pSayMessage->wchUserName, pSayMessage->wchMessage);

				// remember who last msg me
				m_pChatGui->SetLastRecieveWhisperName(pSayMessage->wchUserName);
			}
		}
		else if (pSayMessage->byMsgType == CHAT_TYPE_TRADE || pSayMessage->byMsgType == CHAT_TYPE_FIND_PARTY)
		{
			bBalloon = false;
			swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, GetDisplayStringManager()->GetString("DST_CHAT_TYPE_GENERAL"), wchMsgType, pSayMessage->wchUserName, pSayMessage->wchMessage);
		}
		else 
		{
			bBalloon = true;
			swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, GetDisplayStringManager()->GetString("DST_CHAT_TYPE_GENERAL"), wchMsgType, pSayMessage->wchUserName, pSayMessage->wchMessage);
		}

		if (bBalloon)
		{
			// Boost speech bubbles
			if (pSayMessage->uiSerial != INVALID_SERIAL_ID)
			{
				if (Logic_GetAvatarHandle() == pSayMessage->uiSerial)
				{
					GetBalloonManager()->AddAvatarBalloon(pSayMessage->wchMessage, pSayMessage->byMsgType == CHAT_TYPE_PARTY ? TRUE:FALSE);
				}
				else
				{
					CNtlSob* pObject = GetNtlSobManager()->GetSobObject(pSayMessage->uiSerial);
					if (pObject)
					{
						GetBalloonManager()->AddPlayerBalloon(pObject, pSayMessage->wchMessage, pSayMessage->byMsgType == CHAT_TYPE_PARTY ? TRUE : FALSE);
					}
				}
			}
		}

		Chatlog_Save(pSayMessage->byMsgType, wchBuffer);
		Chatlog_Notify(pSayMessage->byMsgType, wchBuffer);

		if( pSayMessage->uiSerial != Logic_GetAvatarHandle() &&
			_wcsicmp(pSayMessage->wchUserName, Logic_GetAvatarName()) != 0 )
			Logic_NameNotify(wchBuffer);
	}
	else if( msg.Id == g_EventDiceResultShow )
	{
		SDboEventDiceResultShow* pEvent = reinterpret_cast<SDboEventDiceResultShow*>( msg.pData );

		switch(pEvent->byWorkID)
		{
			case DICE_WORKD_ID_ACTION:		
			{
				CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pEvent->hHandle) );
				if( !pSobPlayer )
					NTL_RETURNVOID();

				CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );

				memset( wchBuffer, 0, sizeof( wchBuffer ) );
				swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, L"%s : %d", pSobPlayerAttr->GetName(), pEvent->byResultNum);

				Chatlog_Save(CHAT_TYPE_GENERAL, wchBuffer);
				Chatlog_Notify(CHAT_TYPE_GENERAL, wchBuffer);
				break;
			}
			case DICE_WORKD_ID_PARTY_INVEN_DICE:
			{
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->hHandle) );
				if( !pMember )
				{
					DBO_FAIL("Not exist party member of handle : " << pEvent->hHandle);
					NTL_RETURNVOID();
				}

				memset( wchBuffer, 0, sizeof( wchBuffer ) );
				swprintf_s(wchBuffer, CHAT_MSG_BUFFER_LEN, L"%s : %d", pMember->wszMemberName, pEvent->byResultNum);

				Chatlog_Save(CHAT_TYPE_PARTY, wchBuffer);
				Chatlog_Notify(CHAT_TYPE_PARTY, wchBuffer);

				break;
			}
			case DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER:
			// peessi : The results of the first ball are shown separately on a team basis.
			break;
			default:
			{
				DBO_FAIL("Invalid dice work ID : " << pEvent->byWorkID);
				NTL_RETURNVOID();
			}
		}		
	}
	else if (msg.Id == g_EventSkillInfoText)
	{
		SNtlEventSobSkillInfoText* pEvent = reinterpret_cast<SNtlEventSobSkillInfoText*>(msg.pData);

		Chatlog_Save(pEvent->byMsgType, pEvent->wchMsg);
		Chatlog_Notify(pEvent->byMsgType, pEvent->wchMsg, pEvent->uiColor);
	}
	else if (msg.Id == g_EventSobBattleNotification)
	{
		BattleNotification(msg);
	}

	NTL_RETURNVOID();
}