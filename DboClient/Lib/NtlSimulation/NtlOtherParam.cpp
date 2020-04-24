#include "precomp_ntlsimulation.h"
#include "NtlOtherParam.h"


// core
#include "NtlDebug.h"

// share
#include "NPCTable.h"
#include "WorldTable.h"
#include "TextAllTable.h"
#include "ObjectTable.h"
#include "QuestTextDataTable.h"
#include "TableContainer.h"

// gui
#include "gui_precomp.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "DboTSCQAgency.h"
#include "DboTSCMain.h"
#include "NtlSLLogic.h"


CNtlOtherParam::CNtlOtherParam()
:m_uiHelpHint(0x00000000)
,m_hRegisteredFuelItem(INVALID_SERIAL_ID)
,m_idxRegisteredFuelItem(INVALID_TBLIDX)
{
}

CNtlOtherParam::~CNtlOtherParam()
{
}

RwBool CNtlOtherParam::Create()
{
	NTL_FUNCTION("CNtlOtherParam::Create");

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	m_uiHelpHint = pAvatarInfo->sCharPf.dwTutorialHint;
	CNtlSLEventGenerator::HelpHintEvent(m_uiHelpHint);

	m_BindInfo.byBindType				= pAvatarInfo->sCharPf.byBindType;
	m_BindInfo.BindedWorldID			= pAvatarInfo->sCharPf.bindWorldId;
	m_BindInfo.uiBindedObejcTblIdx		= pAvatarInfo->sCharPf.bindObjectTblidx;

	m_bScouterOn = false;

	NTL_RETURN(TRUE);
}

VOID CNtlOtherParam::Destroy()
{
	NTL_FUNCTION("CNtlOtherParam::Destroy");

	m_multimapNextQuest.clear();
	m_mapBusRoute.clear();

	NTL_RETURNVOID();
}

MAP_NEXTQUEST_ITER CNtlOtherParam::GetNextQuestBegin()
{
	return m_multimapNextQuest.begin();
}

MAP_NEXTQUEST_ITER CNtlOtherParam::GetNextQuestEnd()
{
	return m_multimapNextQuest.end();
}

const sBusRoute* CNtlOtherParam::GetBusRoute(SERIAL_HANDLE hHandle)
{
	MAP_BUS_ROUTE_ITER it_BusRoute = m_mapBusRoute.find(hHandle);
	if( it_BusRoute == m_mapBusRoute.end() )
		return NULL;

	return &(it_BusRoute->second);
}

MAP_BUS_ROUTE_ITER CNtlOtherParam::GetBusRouteBegin()
{
	return m_mapBusRoute.begin();
}

MAP_BUS_ROUTE_ITER CNtlOtherParam::GetBusRouteEnd()
{
	return m_mapBusRoute.end();
}

void CNtlOtherParam::SetWarFolgValue(RwInt32 iFlagIndex)
{
	Logic_SetFlags( GetWarFomFlag(), iFlagIndex, NTL_MAX_COUNT_WAR_FOG);
}

char* CNtlOtherParam::GetWarFomFlag()
{
	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return pAvatarInfo->acWarFogFlag;
}

void CNtlOtherParam::SetCharTitleValue(RwInt32 iFlagIndex)
{
	Logic_SetFlags(GetCharTitleFlag(), iFlagIndex, -1);
}

void CNtlOtherParam::UnSetCharTitleValue(RwInt32 iFlagIndex)
{
	Logic_UnsetFlags(GetCharTitleFlag(), iFlagIndex, -1);
}

char* CNtlOtherParam::GetCharTitleFlag()
{
	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return pAvatarInfo->abyTitleIndexFlag;
}

void CNtlOtherParam::SetRegisterFuelItem(SERIAL_HANDLE hItem)
{
	m_hRegisteredFuelItem = hItem;
}

void CNtlOtherParam::SetRegisterFuelItemTableIndex(TBLIDX idx)
{
	m_idxRegisteredFuelItem = idx;
}

SERIAL_HANDLE CNtlOtherParam::GetRegisterFuelItem()
{
	return m_hRegisteredFuelItem;
}

TBLIDX CNtlOtherParam::GetRegisterFuelItemTableIndex()
{
	return m_idxRegisteredFuelItem;
}

void CNtlOtherParam::SetScouterOn(bool bFlag)
{
	m_bScouterOn = bFlag;
}

bool CNtlOtherParam::IsScouterOn()
{
	return m_bScouterOn;
}

RwBool CNtlOtherParam::IsExistNextQuest_of_NPC(TBLIDX idNPCTableIndex)
{
	std::multimap<QM_KEY, sNextQuest>::iterator it = m_multimapNextQuest.begin();
	for( ; it != m_multimapNextQuest.end() ; ++it )
	{
		sNextQuest& nextQuest = it->second;

		if( nextQuest.Tblidx == idNPCTableIndex )
			return true;
	}

	return false;
}

RwBool CNtlOtherParam::IsExistWarFog_of_Index(RwInt32 iFlagIndex)
{
	return !Logic_IsExistFlag( GetWarFomFlag(), iFlagIndex, NTL_MAX_COUNT_WAR_FOG);
}

VOID CNtlOtherParam::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlOtherParam::HandleEvents");

	if (pMsg.Id == g_EventHelpHint)
	{
		SNtlEventHelpHint* pPacket = reinterpret_cast<SNtlEventHelpHint*>(pMsg.pData);
		m_uiHelpHint				 = pPacket->uiHelpHint;
	}
	else if( pMsg.Id == g_EventRegisterQuestPosMark )
	{
		SNtlEventRegisterQuestPosMark* pEvent = reinterpret_cast<SNtlEventRegisterQuestPosMark*>( pMsg.pData );

		sNextQuest nextQuest;

		nextQuest.v3RealPosition.x		= pEvent->fTargetPosX;
		nextQuest.v3RealPosition.y		= pEvent->fTargetPosY;
		nextQuest.v3RealPosition.z		= pEvent->fTargetPosZ;
		nextQuest.eTargetType			= pEvent->eTargetType;
		nextQuest.QMKey					= pEvent->QMKey;
		nextQuest.WorldID				= pEvent->uiTargetWorldTblIdx;
		nextQuest.Tblidx				= pEvent->uiTargetTableIdx;
		nextQuest.bShow					= TRUE;

		switch(nextQuest.eTargetType)
		{
		case eQMI_TARGET_TYPE_NPC:
			{
				nextQuest.pNPC_TBLDAT = reinterpret_cast<sNPC_TBLDAT*>( API_GetTableContainer()->GetNpcTable()->FindData(nextQuest.Tblidx) );

				if(!nextQuest.pNPC_TBLDAT)
				{
					// Release 버전에서 뻑나는 것을 방지하기 위해
					char acBuffer[256] = "";
					WCHAR awcBuffer[256] = L"";
					sprintf_s(acBuffer, 256, "preparation NPC table of index : %d", nextQuest.Tblidx);
					NtlLogFilePrint(acBuffer);			
					MultiByteToWideChar(GetACP(), 0, acBuffer, -1, awcBuffer, 256);
					CNtlSLEventGenerator::SysMsg(awcBuffer);
					nextQuest.wstrName = awcBuffer;
				}
				else
				{
					CTextTable* pNPCTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
					NTL_ASSERT(pNPCTextTable, "CNtlOtherParam::HandleEvents, Not exist npc text table");
					nextQuest.wstrName = pNPCTextTable->GetText(nextQuest.pNPC_TBLDAT->Name);
				}
				
				break;
			}
		case eQMI_TARGET_TYPE_OBJECT:
			{
				sOBJECT_TBLDAT* pOBJECT_TBLDAT = reinterpret_cast<sOBJECT_TBLDAT*>( API_GetTableContainer()->GetObjectTable(nextQuest.WorldID)->FindData(nextQuest.Tblidx) );

				if(!pOBJECT_TBLDAT)
				{
					// Release 버전에서 뻑나는 것을 방지하기 위해
					char acBuffer[128] = "";
					WCHAR awcBuffer[256] = L"";
					sprintf_s(acBuffer, 128, "preparation Object table of index : %d", nextQuest.Tblidx);
					NtlLogFilePrint(acBuffer);
					MultiByteToWideChar(GetACP(), 0, acBuffer, -1, awcBuffer, 256);
					CNtlSLEventGenerator::SysMsg(awcBuffer);
					nextQuest.wstrName = awcBuffer;
				}
				else
				{
					CTextTable* pObjectTextTable = API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
					NTL_ASSERT(pObjectTextTable, "CNtlOtherParam::HandleEvents, Not exist object text table");
					nextQuest.wstrName = pObjectTextTable->GetText(pOBJECT_TBLDAT->dwName);
				}

				break;
			}
		case eQMI_TARGET_TYPE_POSITION:
			{
				sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData(pEvent->uiTooltipTblIdx) );

				if(!pQUEST_TEXT_DATA_TBLDAT)
				{
					// Release 버전에서 뻑나는 것을 방지하기 위해
					char acBuffer[128] = "";
					WCHAR awcBuffer[256] = L"";
					sprintf_s(acBuffer, 128, "preparation quest text table of index : %d", nextQuest.Tblidx);
					NtlLogFilePrint(acBuffer);
					MultiByteToWideChar(GetACP(), 0, acBuffer, -1, awcBuffer, 256);
					CNtlSLEventGenerator::SysMsg(awcBuffer);
					nextQuest.wstrName = awcBuffer;
				}
				else
				{
					nextQuest.wstrName = pQUEST_TEXT_DATA_TBLDAT->wstrText;
					nextQuest.wstrName = gui::GetHtmlFromMemoryString(nextQuest.wstrName.c_str(), nextQuest.wstrName.size());
				}
				break;
			}
		default:
			{
				DBO_FAIL("Invalid next quest type" << nextQuest.eTargetType);
				break;
			}
		}		

		m_multimapNextQuest.insert( std::make_pair(pEvent->QMKey, nextQuest) );
	}
	else if( pMsg.Id == g_EventUnregisterQuestPosMark )
	{
		SNtlEventUnregisterQuestPosMark* pEvent = reinterpret_cast<SNtlEventUnregisterQuestPosMark*>( pMsg.pData );

		m_multimapNextQuest.erase(pEvent->QMKey);
	}
	else if( pMsg.Id == g_EventShowQuestPosMark )
	{
		SNtlEventShowQuestPosMark* pEvent = reinterpret_cast<SNtlEventShowQuestPosMark*>( pMsg.pData );

		CDboTSCQAgency* pQuestAgency = API_GetQuestAgency();
		if ( NULL == pQuestAgency ) NTL_RETURNVOID();

		std::multimap<QM_KEY, sNextQuest>::iterator it = m_multimapNextQuest.begin();
		for( ; it != m_multimapNextQuest.end() ; ++ it)
		{
			sNextQuest& nextQuest = it->second;
			if( pQuestAgency->IsSameKeyTSKeyWithQMKey(pEvent->sTSKey, nextQuest.QMKey) )
			{
				nextQuest.bShow = pEvent->bShow;
			}
		}
	}
	else if( pMsg.Id == g_EventBind )
	{
		SNtlEventBind* pEvent = reinterpret_cast<SNtlEventBind*>( pMsg.pData );

		m_BindInfo.byBindType			= pEvent->byBindType;
		m_BindInfo.BindedWorldID		= pEvent->BindedWorldID;
		m_BindInfo.uiBindedObejcTblIdx	= pEvent->uiBindObjectTblIdx;

		CNtlSLEventGenerator::BindNotify();
	}
	else if( pMsg.Id == g_EventBusMove )
	{
		SNtlEventBusMove* pEvent = reinterpret_cast<SNtlEventBusMove*>( pMsg.pData );

		if( BUS_MOVE_POS_DIR == pEvent->byMessage )
		{
			MAP_BUS_ROUTE_ITER it_BusRoute = m_mapBusRoute.find(pEvent->hBus);
			if( it_BusRoute != m_mapBusRoute.end() )
			{
				sBusRoute& rBusRoute = it_BusRoute->second;
				rBusRoute.v3Pos			= *pEvent->pV3Pos;
				rBusRoute.v3Dir			= *pEvent->pV3Dir;
				NTL_RETURNVOID();
			}

			sBusRoute tBusRoute;

			tBusRoute.hBus			= pEvent->hBus;
			tBusRoute.v3Pos			= *pEvent->pV3Pos;
			tBusRoute.v3Dir			= *pEvent->pV3Dir;

			tBusRoute.pNPC_TBLDAT = reinterpret_cast<sNPC_TBLDAT*>( API_GetTableContainer()->GetNpcTable()->FindData(pEvent->idxBusTable) );
			if( !tBusRoute.pNPC_TBLDAT )
			{
				DBO_FAIL("Not exist NPC table of index : " << pEvent->idxBusTable);
				NTL_RETURNVOID();
			}

			if( strcmp(tBusRoute.pNPC_TBLDAT->szModel, "N_BUS_A1") == 0 )
			{
				tBusRoute.eBusShapeType = BUS_SHAPE_HUMAN;
			}
			else if( strcmp(tBusRoute.pNPC_TBLDAT->szModel, "N_BUS_C1") == 0 )
			{
				tBusRoute.eBusShapeType = BUS_SHAPE_NAMEK;
			}
			else if( strcmp(tBusRoute.pNPC_TBLDAT->szModel, "N_BUS_B1") == 0 )
			{
				tBusRoute.eBusShapeType = BUS_SHAPE_MAJIN;
			}
			else
			{
				DBO_FAIL("Invalid bus model name : " << tBusRoute.pNPC_TBLDAT->szModel);
				return;
			}

			m_mapBusRoute[tBusRoute.hBus] = tBusRoute;
		}
		else if( BUS_MOVE_DELETE_BUS == pEvent->byMessage )
		{
			MAP_BUS_ROUTE_ITER it_BusRoute = m_mapBusRoute.find(pEvent->hBus);
			if( it_BusRoute == m_mapBusRoute.end() )
				NTL_RETURNVOID();

			m_mapBusRoute.erase(it_BusRoute);
		}
		else if( BUS_MOVE_RESET == pEvent->byMessage )
		{
			m_mapBusRoute.clear();
		}
	}
	
	NTL_RETURNVOID();
}