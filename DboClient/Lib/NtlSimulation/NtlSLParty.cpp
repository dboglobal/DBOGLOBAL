#include "precomp_ntlsimulation.h"
#include "NtlParty.h"

// core
#include "NtlDebug.h"

// share
#include "Itemtable.h"
#include "CharmTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobCharProxy.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

CNtlParty::CNtlParty()
:m_hLeaderHandle(0xffffffff)
,m_byZennyLootingMethod(NTL_PARTY_ZENNY_LOOTING_UNKNOWN)
,m_byItemLootingMethod(NTL_PARTY_ITEM_LOOTING_UNKNOWN)
,m_eDungeonState(PARTY_DUNGEON_STATE_NORMAL)
{
	memset(m_pwPartyName, 0, sizeof(m_pwPartyName));

    for(int i = 0; i < NTL_MAX_SHARETARGET_COUNT; ++i)
    {
        m_aShareTargetInfo[i].hTarget = INVALID_SERIAL_ID;
    }
}

CNtlParty::~CNtlParty()
{
    Destroy();
}

RwBool CNtlParty::Create(void)
{
	NTL_FUNCTION("CNtlParty::Create");
    
	LinkMsg(g_EventPartyCreate);
	LinkMsg(g_EventPartyLeave);
	LinkMsg(g_EventPartyMemberAdd);
	LinkMsg(g_EventPartyMemberDel);
	LinkMsg(g_EventPartyLeaderChange);
	LinkMsg(g_EventPartyUpdate);
	LinkMsg(g_EventPartyMemberLocation);
	LinkMsg(g_EventPartyShareTargetRes);
	LinkMsg(g_EventPartyShareTargetNfy);
	LinkMsg(g_EventSobOnBus);
	LinkMsg(g_EventTObjectUpdateState);

	NTL_RETURN(TRUE);
}

void CNtlParty::Destroy(void)
{
	NTL_FUNCTION("CNtlParty::Destroy");

	UnLinkMsg(g_EventPartyCreate);
	UnLinkMsg(g_EventPartyLeave);
	UnLinkMsg(g_EventPartyMemberAdd);
	UnLinkMsg(g_EventPartyMemberDel);
	UnLinkMsg(g_EventPartyLeaderChange);
	UnLinkMsg(g_EventPartyUpdate);
	UnLinkMsg(g_EventPartyMemberLocation);
	UnLinkMsg(g_EventPartyShareTargetRes);
	UnLinkMsg(g_EventPartyShareTargetNfy);
	UnLinkMsg(g_EventSobOnBus);
	UnLinkMsg(g_EventTObjectUpdateState);

	ClearMember();

	NTL_RETURNVOID();
}

VOID CNtlParty::ClearMember()
{
	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list )
	{
		sPartyMember* pPartyMember = reinterpret_cast<sPartyMember*>( *it_list );
		if(pPartyMember)
		{
			if( pPartyMember->pFakeBuffContainer )
				NTL_DELETE(pPartyMember->pFakeBuffContainer);

			NTL_DELETE(pPartyMember);
		}
	}

	m_listMember.clear();
	m_mapMember.clear();
}

VOID CNtlParty::Leave()
{
	memset(m_pwPartyName, 0, sizeof(m_pwPartyName));
	m_hLeaderHandle				= INVALID_SERIAL_ID;
	m_byZennyLootingMethod		= NTL_PARTY_ZENNY_LOOTING_UNKNOWN;
	m_byItemLootingMethod		= NTL_PARTY_ITEM_LOOTING_UNKNOWN;
	m_eDungeonState				= PARTY_DUNGEON_STATE_NORMAL;

	std::list<CNtlSob*> listChangeHeadNameColor;

	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; it_list++ )
	{
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject( (*it_list)->uiKey );

		// Change the head name color if you have members nearby
		if( pSob )
			listChangeHeadNameColor.push_back(pSob);
	}

	ClearMember();

	std::list<CNtlSob*>::iterator it_HeadNameColor = listChangeHeadNameColor.begin();
	for (; it_HeadNameColor != listChangeHeadNameColor.end(); it_HeadNameColor++)
	{
		Logic_SetHeadNameColor(*it_HeadNameColor);
	}

    // If there is a shared target marking, release it.
    for(int i = 0; i < NTL_MAX_SHARETARGET_COUNT; ++i)
    {
        if(m_aShareTargetInfo[i].hTarget != INVALID_SERIAL_ID)
        {
            CNtlSLEventGenerator::ShareTargetRelease(m_aShareTargetInfo[i].hTarget);
        }
    }
}

VOID CNtlParty::Update(RwReal fElapsed)
{
	COMMUNITY_ITER it_list = m_listMember.begin();
	for( ; it_list != m_listMember.end() ; ++it_list )
	{
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it_list );

		// Update buffs for players except avatars
		if( pMember->pFakeBuffContainer )
			pMember->pFakeBuffContainer->Update(fElapsed);
		
		CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject(pMember->hSerial));
		
		// 파티 맴버가 가시영역 밖으로 나가서 사라질 수도 있다.
		if(!pPlayer)
			continue;

		RwV3d vPos = pPlayer->GetPosition();
		
		// 아바타 주변에 있으므로 아바타와 같은 월드 아이디를 갖는다
		// 아바타 주변에 없을 때는 서버로 부터 별도의 패킷으로 데이터를 받는다
		pMember->uiWorldID	= Logic_GetActiveWorldId();
		pMember->uiWorldTbl	= Logic_GetActiveWorldTableId();

		pMember->vPos.x = vPos.x;
		pMember->vPos.y = vPos.y;
		pMember->vPos.z = vPos.z;		
	}
}

VOID CNtlParty::SetPartyName(WCHAR* pcName)
{
	if( !pcName )
		return;

	if( wcslen(m_pwPartyName) > 0 )
	{
		DBO_FAIL("Already avatar have a party");
		return;
	}	

	memset(m_pwPartyName, 0, sizeof(m_pwPartyName));

	if(pcName)
		memcpy(m_pwPartyName, pcName, sizeof(WCHAR)*NTL_MAX_SIZE_PARTY_NAME);
}

SERIAL_HANDLE CNtlParty::GetLeaderHandle()
{
	return m_hLeaderHandle;
}

const WCHAR* CNtlParty::GetPartyName()
{
	return m_pwPartyName;
}


RwInt32 CNtlParty::GetAvatarIndex()
{
	RwInt32 iAvatarIndex = 0;
	SERIAL_HANDLE hAvatarHandle = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();	
	COMMUNITY_ITER it = m_listMember.begin();

	for( ; it != m_listMember.end() ; ++it, ++iAvatarIndex )
	{
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it );
		NTL_ASSERT(pMember, "CNtlParty::GetAvatarIndex, Not exist party member");
		if( pMember->hSerial == hAvatarHandle )
			return iAvatarIndex;
	}

	return -1;
}

VOID CNtlParty::Sort(RwUInt8 byDataType, RwBool bAscent)
{
	CNtlSLCommunityGroup::Sort(byDataType, bAscent);
}

RwBool CNtlParty::IsHaveGroup()
{
	if( !m_pwPartyName )
		return FALSE;

	if( wcslen(m_pwPartyName) > 0 )
		return TRUE;

	return FALSE;
}

VOID CNtlParty::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlParty::HandleEvents");

	if(pMsg.Id == g_EventPartyCreate)
	{
		SNtlEventPartyCreate* pPartyCreate = reinterpret_cast<SNtlEventPartyCreate*>(pMsg.pData);
		SetPartyName(pPartyCreate->pcName);

		CNtlSLEventGenerator::PartyUpdateValue(PMT_PARTY_CREATE);
	}
	else if(pMsg.Id == g_EventPartyLeave)
	{
		Leave();

		CNtlSLEventGenerator::PartyUpdateValue(PMT_PARTY_LEAVE);
	}
	else if(pMsg.Id == g_EventPartyMemberAdd)
	{
		SNtlEventPartyAdd* pPacket = reinterpret_cast<SNtlEventPartyAdd*>(pMsg.pData);

		sPartyMember* pNewMember = NTL_NEW sPartyMember;
		ZeroMemory(pNewMember, sizeof(sPartyMember));

		pNewMember->hSerial		= pPacket->hSerial;		
		pNewMember->byRace		= pPacket->byRace;
		pNewMember->byClass		= pPacket->byClass;
		pNewMember->byLevel		= pPacket->byLevel;
		pNewMember->wCurLP		= pPacket->curLP;
		pNewMember->wMaxLP		= pPacket->maxLP;
		pNewMember->wCurEP		= pPacket->wCurEP;
		pNewMember->wMaxEP		= pPacket->wMaxEP;
		pNewMember->uiWorldID	= pPacket->uiWorldID;
		pNewMember->uiWorldTbl	= pPacket->uiWorldTableIndex;
		pNewMember->vPos		= pPacket->v3Pos;
		pNewMember->hBusSerial	= INVALID_SERIAL_ID;
		memcpy(pNewMember->wszMemberName, pPacket->pcName, sizeof(WCHAR)*NTL_MAX_SIZE_CHAR_NAME);

		if( AddMember(pNewMember->hSerial, pNewMember) )
		{
			if( pNewMember->hSerial != Logic_GetAvatarHandle() )
			{
				pNewMember->pFakeBuffContainer = NTL_NEW CNtlFakeBuffContainer;
				pNewMember->pFakeBuffContainer->Create(pNewMember->hSerial);
			}		

			CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pNewMember->hSerial);
			if( pSob )
				Logic_SetHeadNameColor( pSob );

			CNtlSLEventGenerator::PartyUpdateValue(PMT_PARTY_MEMBER_ADD, pPacket->hSerial);
		}
		else
		{
			NTL_DELETE(pNewMember);
		}
	}
	else if(pMsg.Id == g_EventPartyMemberDel)
	{
		SERIAL_HANDLE hAvatarHandle = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
		SERIAL_HANDLE hSerial = *(reinterpret_cast<SERIAL_HANDLE*>(pMsg.pData));

		if(hAvatarHandle == hSerial)
		{
			// 파티 강퇴를 통해서 Avatar가 강퇴되었다
			Leave();

			CNtlSLEventGenerator::PartyUpdateValue(PMT_PARTY_LEAVE);
		}
		else
		{
			sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(hSerial) );
			if( !pMember )
			{
				DBO_FAIL("g_EventPartyMemberDel, Not exist party member of handle : " << hSerial);
				NTL_RETURNVOID();
			}			

			if( pMember->pFakeBuffContainer )
				NTL_DELETE(pMember->pFakeBuffContainer);

			DelMember(hSerial);

			CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hSerial);
			if( pSob )
				Logic_SetHeadNameColor( pSob );

			CNtlSLEventGenerator::PartyUpdateValue(PMT_PARTY_MEMBER_DEL, hSerial);
		}		
	}
	else if(pMsg.Id == g_EventPartyLeaderChange)
	{
		SERIAL_HANDLE hSerial = *(reinterpret_cast<SERIAL_HANDLE*>(pMsg.pData));
		
		m_hLeaderHandle = hSerial;

		CNtlSLEventGenerator::PartyUpdateValue(PMT_PARTY_LEADER_CHANGE, hSerial);
	}
	else if(pMsg.Id == g_EventPartyUpdate)
	{
		SNtlEventPartyUpdateValue* pPacket = reinterpret_cast<SNtlEventPartyUpdateValue*>(pMsg.pData);

		switch( pPacket->iValueType )
		{
		case PMT_ZENNY_DIVISION:
			{
				m_byZennyLootingMethod = (RwUInt8)pPacket->uiValue;
				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		case PMT_ITEM_DIVISION:
			{
				m_byItemLootingMethod = (RwUInt8)pPacket->uiValue;
				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		case PMT_LP:
			{
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pPacket->hSerial) );
				if( !pMember )
				{
					DBO_FAIL("g_EventPartyUpdate, PMT_LP, Not exist party member of handle : " << pPacket->hSerial);
					NTL_RETURNVOID();
				}
				
				pMember->wCurLP = (RwUInt16)pPacket->uiValue;
				pMember->wMaxLP = (RwUInt16)pPacket->iValue2;
				
				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		case PMT_EP:
			{
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pPacket->hSerial) );
				if( !pMember )
				{
					DBO_FAIL("g_EventPartyUpdate, PMT_EP, Not exist party member of handle : " << pPacket->hSerial);
					NTL_RETURNVOID();
				}
				
				pMember->wCurEP = (RwUInt16)pPacket->uiValue;
				pMember->wMaxEP = (RwUInt16)pPacket->iValue2;

				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		case PMT_LEVEL:
			{
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pPacket->hSerial) );
				if( !pMember )
				{
					DBO_FAIL("g_EventPartyUpdate, PMT_LEVEL, Not exist party member of handle : " << pPacket->hSerial);
					NTL_RETURNVOID();
				}

				pMember->byLevel = (RwUInt8)pPacket->uiValue;

				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		case PMT_CLASS:
			{
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pPacket->hSerial) );
				if( !pMember )
				{
					DBO_FAIL("g_EventPartyUpdate, PMT_CLASS, Not exist party member of handle : " << pPacket->hSerial);
					NTL_RETURNVOID();
				}

				pMember->byClass = (RwUInt8)pPacket->uiValue;

				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		case PMT_PARTY_DUNGEON_STATE:
			{
				m_eDungeonState = (ePARTY_DUNGEON_STATE)pPacket->uiValue;
				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, INVALID_SERIAL_ID, pPacket->uiValue);
				break;
			}
		case PMT_PARTY_DUNGEON_INITIALIZE:
			{
				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType);
				break;
			}
		default:
			{
				CNtlSLEventGenerator::PartyUpdateValue(pPacket->iValueType, pPacket->hSerial, pPacket->uiValue, pPacket->iValue2);
				break;
			}
		}
	}
	else if(pMsg.Id == g_EventPartyMemberLocation)
	{
		SNtlEventPartyMemberLocation* pPacket = reinterpret_cast<SNtlEventPartyMemberLocation*>(pMsg.pData);

		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pPacket->uiSerial) );
		if(pMember)
		{
			CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject(pMember->hSerial));

			// 가시거리에 있는 맴버의 위치는 업데이트하지 않는다
			if(pPlayer)
				return;

			pMember->vPos			= pPacket->vCurLoc;
			pMember->uiWorldID		= pPacket->uiWorldID;
			pMember->uiWorldTbl		= pPacket->uiWorldTbl;

			CNtlSLEventGenerator::PartyUpdateValue(PMT_MEMBER_LOCATION);
		}
	}
	else if(pMsg.Id == g_EventSobFakeBuffAdd)
	{
		SNtlEventFakeBuffAdd *pFakeBuffAdd = reinterpret_cast<SNtlEventFakeBuffAdd*>(pMsg.pData);
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pFakeBuffAdd->hSerialId) );

		if( pMember )
		{
			pMember->pFakeBuffContainer->HandleEvents(pMsg);
		}
	}
	else if(pMsg.Id == g_EventSobFakeBuffDrop)
	{
		SNtlEventFakeBuffDrop *pFakeBuffDrop = reinterpret_cast<SNtlEventFakeBuffDrop*>(pMsg.pData);
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pFakeBuffDrop->hSerialId) );

		if( pMember )
		{
			pMember->pFakeBuffContainer->HandleEvents(pMsg);
		}
	}
	else if( pMsg.Id == g_EventSobFakeBuffRefreshAll )
	{
		SNtlEventSobBuffRefreshAll *pBuffRefreshAll = reinterpret_cast<SNtlEventSobBuffRefreshAll*>(pMsg.pData);
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( GetMemberbyKey(pBuffRefreshAll->hSerialId) );

		if( pMember )
		{
			pMember->pFakeBuffContainer->HandleEvents(pMsg);
		}
	}
	else if (pMsg.Id == g_EventSobFakeBuffActivate)
	{
		SNtlEventSobBuffActivate* pBuff = reinterpret_cast<SNtlEventSobBuffActivate*>(pMsg.pData);
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>(GetMemberbyKey(pBuff->hSerialId));

		if (pMember)
		{
			pMember->pFakeBuffContainer->HandleEvents(pMsg);
		}
	}
    else if(pMsg.Id == g_EventPartyShareTargetNfy)
    {
        PartyShareTargetNfyHandler(pMsg);
    }
    else if(pMsg.Id == g_EventPartyShareTargetRes)
    {    
        PartyShareTargetResHandler(pMsg);
    }
	else if(pMsg.Id == g_EventSobOnBus)
	{
		SNtlEventSobOnBus*	pEvent	= reinterpret_cast<SNtlEventSobOnBus*>( pMsg.pData );
		sPartyMember*		pMember	= reinterpret_cast<sPartyMember*>( GetMemberbyKey(pEvent->hSerialId) );

		if( pMember )
		{
			if( pEvent->bRideOn )
				pMember->hBusSerial = pEvent->hBusSerialId;
			else
				pMember->hBusSerial = INVALID_SERIAL_ID;
		}
	}
	else if(pMsg.Id == g_EventTObjectUpdateState)
	{
		SNtlEventTObjectUpdateState* pEvent = reinterpret_cast<SNtlEventTObjectUpdateState*>( pMsg.pData );

		for( RwUInt8 i = 0 ; i < NTL_MAX_SHARETARGET_COUNT ; ++i )
		{
			if( m_aShareTargetInfo[i].hTarget == pEvent->hSerialId )
			{
				if( false == BIT_FLAG_TEST(pEvent->bySubState, TOBJECT_SUBSTATE_FLAG_SHOW) )
					m_aShareTargetInfo[i].hTarget = INVALID_SERIAL_ID;
			}
		}		
	}
	else if (pMsg.Id == g_EventChangeCharName)
	{
		SNtlEventChangeCharName* pEvent = reinterpret_cast<SNtlEventChangeCharName*>(pMsg.pData);

		sPartyMember* pMember = reinterpret_cast<sPartyMember*>(GetMemberbyName(pEvent->wchOldCharName));
		if (!pMember)
		{
			NTL_RETURNVOID();
		}

		NTL_SAFE_WCSCPY(pMember->wszMemberName, pEvent->wchCharName);
	}

	NTL_RETURNVOID();
}

void CNtlParty::PartyShareTargetNfyHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventPartyShareTargetNfy* pData = (SNtlEventPartyShareTargetNfy*)pMsg.pData;

    // 1. Removes marks that are not included in an existing target. (NOTE: check to prevent the effect from being continuously generated / released)   
    for(int i = 0; i < NTL_MAX_SHARETARGET_COUNT; ++i)
    {
        if(m_aShareTargetInfo[i].hTarget == INVALID_SERIAL_ID)
            continue;

        RwBool bIsExist = FALSE;
        for(int j = 0; j < NTL_MAX_SHARETARGET_COUNT; ++j)
        {
            if(m_aShareTargetInfo[i].hTarget == pData->sharetargetInfo[j].hTarget)
            {
                bIsExist = TRUE;
                break;
            }
        }
        
        if(!bIsExist)
        {
            // Remove effects
            CNtlSLEventGenerator::ShareTargetRelease(m_aShareTargetInfo[i].hTarget);
        }
    }
    
    for(RwUInt8 i = 0; i < NTL_MAX_SHARETARGET_COUNT; ++i)
    {
        // 2. Generate a complete mark on the received target. (Ignore if the same mark already exists)
        if(pData->sharetargetInfo[i].hTarget != INVALID_SERIAL_ID)
        {
            CNtlSLEventGenerator::ShareTargetMarking(pData->sharetargetInfo[i].hTarget, i);
        }

        // 3. Store it in AvatarInfo.
        m_aShareTargetInfo[i] = pData->sharetargetInfo[i];
    }
}

void CNtlParty::PartyShareTargetResHandler( RWS::CMsg& pMsg ) 
{
    //SNtlEventPartyShareTargetRes* pData = (SNtlEventPartyShareTargetRes*)pMsg.pData;
}