#include "precomp_dboclient.h"
#include "CursorManager.h"

// share
#include "NtlCharacter.h"
#include "NtlRide.h"
#include "NPCTable.h"

// framework
#include "NtlApplication.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLSceneManager.h"

// simulation
#include "NtlWorldconcept.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobDynamicObject.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlFSMUtil.h"
#include "NtlSLEvent.h"


// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "HpGui.h"
#include "DialogManager.h"



#define CURSOR_PATH ".\\cursor\\"
#define PICK_FOCUS_TIME (0.2f)


CCursorManager::CCursorManager(VOID)
:m_eCurCursor(CS_INVALID_CURSOR)
,m_eDesignatedCursor(CS_INVALID_CURSOR)
,m_hFocusSerialId(INVALID_SERIAL_ID)
,m_iOldXPos(0)
,m_iOldYPos(0)
{
	m_eCurrentRace = RACE_HUMAN;
	memset( m_hMouseCursor, 0, sizeof( m_hMouseCursor ) );
	m_bShowCursor = TRUE;
	m_bSkillSelectMode = false;
}


VOID CCursorManager::CreateInstance(VOID)
{
	std::string strPath = CURSOR_PATH;
	std::string strFilename;
	
	SetHumanCursor();

	m_hMouseCursor[CS_MOVE_INDUNENTER] = LoadCursor(NULL, IDC_HELP);

	strFilename = strPath + "01.cur";
	m_hMouseCursor[CS_LOBBY_CURSOR] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "09.cur";
	m_hMouseCursor[CS_BATTLE_TARGET] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "10.cur";
	m_hMouseCursor[CS_BATTLE_ATTACK] = LoadCursorFromFile( strFilename.c_str() );
	
	strFilename = strPath + "13.cur";
	m_hMouseCursor[CS_BATTLE_BOUNDARY] = LoadCursorFromFile( strFilename.c_str() );
	
	strFilename = strPath + "15.cur";		
	m_hMouseCursor[CS_DEAL_VEHICLE] = LoadCursorFromFile( strFilename.c_str() );
	
	strFilename = strPath + "15.cur";
	m_hMouseCursor[CS_DEAL_SHOP] = LoadCursorFromFile( strFilename.c_str() );
	
	strFilename = strPath + "15.cur";
	m_hMouseCursor[CS_TRIGGER_OBJECT] = LoadCursorFromFile( strFilename.c_str() );
	
	m_hMouseCursor[CS_OBJECT_DRAGONBALL] = LoadCursor(NULL, IDC_HELP);
	
	m_hMouseCursor[CS_OBJECT_MAILCHECK] = LoadCursor(NULL, IDC_HELP);
	
	m_hMouseCursor[CS_OBJECT_RECIPE] = LoadCursor(NULL, IDC_HELP);
	
	m_hMouseCursor[CS_QUEST_DIALOG] = LoadCursor(NULL, IDC_HELP);
	
	m_hMouseCursor[CS_QUEST_ACCEPT] = LoadCursor(NULL, IDC_HELP);
	
	m_hMouseCursor[CS_QUEST_PROCEED] = LoadCursor(NULL, IDC_HELP);
	
	m_hMouseCursor[CS_QUEST_COMPLETE] = LoadCursor(NULL, IDC_HELP);
	
	strFilename = strPath + "12.cur";
	m_hMouseCursor[CS_SYSTEM_CAMROTATE] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "11.cur";
	m_hMouseCursor[CS_SYSTEM_LOADING] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "14.cur";
	m_hMouseCursor[CS_ITEM_REPAIR] = LoadCursorFromFile( strFilename.c_str() );
		
	strFilename = strPath + "17.cur";
	m_hMouseCursor[CS_ITEM_IDENTIFICATION] = LoadCursorFromFile( strFilename.c_str() );	
	
	m_hMouseCursor[CS_SYSTEM_HELP] = LoadCursor(NULL, IDC_HELP);

	strFilename = strPath + "18.cur";
	m_hMouseCursor[CS_ITEM_GET_ON_BUS] = LoadCursorFromFile( strFilename.c_str() );	

	strFilename = strPath + "19.cur";
	m_hMouseCursor[CS_ITEM_GET_OFF_BUS] = LoadCursorFromFile( strFilename.c_str() );	

	strFilename = strPath + "20.cur";
	m_hMouseCursor[CS_CHAT_RESIZE_HORI] = LoadCursorFromFile( strFilename.c_str() );	

	strFilename = strPath + "21.cur";
	m_hMouseCursor[CS_CHAT_RESIZE_VERT] = LoadCursorFromFile( strFilename.c_str() );	

	strFilename = strPath + "22.cur";
	m_hMouseCursor[CS_CHAT_RESIZE_RIGHTUP] = LoadCursorFromFile( strFilename.c_str() );	

	strFilename = strPath + "23.cur";
	m_hMouseCursor[CS_ITEM_CAN_NOT_GET_ON_BUS] = LoadCursorFromFile( strFilename.c_str() );	

	strFilename = strPath + "24.cur";
	m_hMouseCursor[CS_CHAT_RESIZE_RIGHTDOWN] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "25.cur";
	m_hMouseCursor[CS_ITEM_USE_SKILL_RESET] = LoadCursorFromFile(strFilename.c_str());

	strFilename = strPath + "26.ani";
	m_hMouseCursor[CS_SKILL_ATTACK] = LoadCursorFromFile(strFilename.c_str());

	strFilename = strPath + "28.cur";
	m_hMouseCursor[CS_ITEM_USE_DOGIBALL] = LoadCursorFromFile(strFilename.c_str());

	strFilename = strPath + "32.cur";
	m_hMouseCursor[CS_ITEM_DISASSEMBLE] = LoadCursorFromFile(strFilename.c_str());


	LinkMsg(g_EventSkillSelectMode);
}

VOID CCursorManager::DeleteInstance(VOID)
{
	UnLinkMsg(g_EventSkillSelectMode);
}

void CCursorManager::HandleEvents(RWS::CMsg & pMsg)
{
	if (pMsg.Id == g_EventSkillSelectMode)
	{
		SNtlEventSkillSelectMode* pMode = (SNtlEventSkillSelectMode*)pMsg.pData;

		if (pMode->bSelect)
		{
			if (m_bSkillSelectMode == true)
				DBO_WARNING_MESSAGE("Already skill already select mode on");

			m_bSkillSelectMode = true;
			GetDialogManager()->OffMode();
			GetDialogManager()->OnMode(DIALOGMODE_SKILL_SELECT);
		}
		else
		{
			if (m_bSkillSelectMode == false)
				DBO_WARNING_MESSAGE("Already already after select mode off");

			m_bSkillSelectMode = false;
			GetDialogManager()->OffMode();
			GetDialogManager()->OnMode(DIALOGMODE_UNKNOWN);
		}
	}
}

VOID CCursorManager::Update(RwReal fElapsed, sWorldPickInfo_for_Cursor* pWorldPickInfo_for_Cursor)
{
	if( CNtlPLGuiManager::GetInstance()->IsGuiFocus() )
	{
		GUICursorProc();
	}
	else
	{
		if( pWorldPickInfo_for_Cursor && pWorldPickInfo_for_Cursor->bWorldPick )
		{
			if( pWorldPickInfo_for_Cursor->pSobObj )
			{
				if( !(pWorldPickInfo_for_Cursor->pSobObj->GetFlags() & SLFLAG_NOT_HIGHLIGHTED) )
				{
					WorldCursorProc(pWorldPickInfo_for_Cursor->pSobObj);
					return;
				}
			}
		}

		if( GetDialogManager() )
		{
			eDialogMode eMode = GetDialogManager()->GetMode();
			
			if(eMode == DIALOGMODE_ITEM_REPAIR ||
				eMode == DIALOGMODE_ITEM_IDENTIFICATION ||
				eMode == DIALOGMODE_ITEM_DISASSEMBLE ||
				eMode == DIALOGMODE_ITEM_BEAD ||
				eMode == DIALOGMODE_SKILL_SELECT)
				return;
		}

		if (m_bSkillSelectMode == false)
		{
			SetMouseCursor(CS_MOVE_NORMAL);
		}
	}
}

VOID CCursorManager::GUICursorProc()
{
	if( m_eDesignatedCursor != CS_INVALID_CURSOR )
		return;

	if( CNtlPLGuiManager::GetInstance()->IsGuiFocus() == FALSE )
		return;

	SetMouseCursor( CCursorManager::CS_MOVE_NORMAL );
}

void CCursorManager::WorldCursorProc(CNtlSob* pSobObj)
{
	if( m_eDesignatedCursor != CS_INVALID_CURSOR )
		return;

	RwUInt32		uiClassId	= pSobObj->GetClassID();
	SERIAL_HANDLE	hHandle		= pSobObj->GetSerialID();

	if(uiClassId == SLCLASS_MONSTER) 
	{
		SetMouseCursor(CS_BATTLE_ATTACK);
	}
	else if(uiClassId == SLCLASS_WORLD_ITEM) 
	{
		SetMouseCursor( CS_BATTLE_LOOTING );
	}
	else if(uiClassId == SLCLASS_NPC )
	{
		CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>(pSobObj);

		if( Logic_IsBus(pActor) )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

			if( pCtrlStuff->sRide.hTargetSerialId == INVALID_SERIAL_ID )
			{
				CNtlSobNpc* pSobNPC = reinterpret_cast<CNtlSobNpc*>( pActor );
				CNtlSobNpcAttr* pSobNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSobNPC->GetSobAttr() );
				sNPC_TBLDAT* pNPC_TBLDAT = pSobNPCAttr->GetNpcTbl();

				// 아바타가 버스 요금을 가지고 있고 탈 수 있는 거리인지 조사
				if( Logic_GetZenny() >= pNPC_TBLDAT->amerchant_Tblidx[0] )
				{
					if( Logic_InFollowRange(reinterpret_cast<CNtlSobActor*>(pAvatar), pActor, (RwReal)NTL_MAX_BUS_DISTANCE) )
					{
						SetMouseCursor( CS_ITEM_GET_ON_BUS );
						return;
					}
				}
			
				SetMouseCursor( CS_ITEM_CAN_NOT_GET_ON_BUS );
			}
			else if( pSobObj->GetSerialID() == pCtrlStuff->sRide.hTargetSerialId )
			{
				SetMouseCursor( CS_ITEM_GET_OFF_BUS );
			}
			else
			{
				// 아바타가 타고 있는 버스가 아닌 다른 버스에 마우스 커서를 옮겼다
				SetMouseCursor( CS_DEAL_SHOP );	
			}
		}		
		else
		{
			SetMouseCursor( CS_DEAL_SHOP );
		}		
	}
	else if(uiClassId == SLCLASS_TRIGGER_OBJECT)
	{
		CNtlSobTriggerObject* pTriggerObj = reinterpret_cast<CNtlSobTriggerObject*>( GetNtlSobManager()->GetSobObject( pSobObj->GetSerialID() ) ); 

		if( pTriggerObj->CanClick() )
			SetMouseCursor( CS_TRIGGER_OBJECT );
	}
	else if(uiClassId == SLCLASS_DYNAMIC_OBJECT )
	{
		CNtlSobDynamicObject* pDynamciObject = reinterpret_cast<CNtlSobDynamicObject*>( GetNtlSobManager()->GetSobObject( pSobObj->GetSerialID() ) );

		if( pDynamciObject->CanClick() )
			SetMouseCursor( CS_TRIGGER_OBJECT );
	}
	else if(uiClassId == SLCLASS_PLAYER || uiClassId == SLCLASS_PET	)
	{
		if( Logic_IsEnemyTargetFromAvatarActor(hHandle) )
		{
			if (Logic_CanAttacked(reinterpret_cast<CNtlSobActor*>(pSobObj)))
				SetMouseCursor(CS_BATTLE_ATTACK);

			return;
		}
		
		SetMouseCursor( CS_MOVE_NORMAL );
	}
	else
	{
		SetMouseCursor( CS_MOVE_NORMAL );
	}
}

VOID CCursorManager::InitLobbyCursor()
{
	SetMouseCursor(CS_LOBBY_CURSOR);
	SetClassLong( CNtlApplication::GetInstance()->GetHWnd(), GCL_HCURSOR, (LONG)m_hMouseCursor[m_eCurCursor] );
}

VOID CCursorManager::InitGameCursor()
{
	if (m_eCurCursor != CS_MOVE_NORMAL)
	{
		m_eCurCursor = CS_MOVE_NORMAL;
		SetCursor(m_hMouseCursor[m_eCurCursor]);
	}

	SetClassLong(CNtlApplication::GetInstance()->GetHWnd(), GCL_HCURSOR, (LONG)m_hMouseCursor[m_eCurCursor]);
}

VOID CCursorManager::ShowMouseCursor( RwBool bShow /* = TRUE  */)
{
	if(m_bShowCursor != bShow)
	{
		m_bShowCursor = bShow;
		ShowCursor(m_bShowCursor);
	}
}

VOID CCursorManager::SetDesignatedCursor( STATE eCursorState )
{
	if( m_eDesignatedCursor == eCursorState )
		return;

	m_eDesignatedCursor = (RwUInt8)eCursorState;

	if( eCursorState == CS_INVALID_CURSOR )
		SetMouseCursor( CCursorManager::CS_MOVE_NORMAL );
	else
		SetMouseCursor( eCursorState );
}

VOID CCursorManager::SetMouseCursor( STATE eState )
{
	if( m_eCurCursor == eState || eState >= CS_NUM_CURSOR)
		return;

	m_eCurCursor = (RwUInt8)eState;

	// 바로 커서를 변경하기 위해
	SetCursor(m_hMouseCursor[m_eCurCursor]);

	// 커서 등록
	SetClassLong( CNtlApplication::GetInstance()->GetHWnd(), GCL_HCURSOR, (LONG)m_hMouseCursor[m_eCurCursor] );

	if( eState == CS_BATTLE_TARGET )
	{	// 타겟 선택시
		Logic_PlayGUISound( GSD_SYSTEM_ENEMY_FOUSE );
	}
}

RwUInt8 CCursorManager::GetCursorState()
{
	return m_eCurCursor;
}

VOID CCursorManager::SetMouseCursorPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	SetCursorPos(iXPos, iYPos);
}

VOID CCursorManager::SaveCurrentPosition()
{
	POINT pos;
	GetCursorPos(&pos);

	m_iOldXPos = (RwInt32)pos.x;
	m_iOldYPos = (RwInt32)pos.y;
}

VOID CCursorManager::RestorePosition()
{
	SetCursorPos(m_iOldXPos, m_iOldYPos);
}

RwBool CCursorManager::IsShowCursor()
{
	return m_bShowCursor;
}

CCursorManager* CCursorManager::GetInstance(VOID)
{
	static CCursorManager intance;
	return &intance;
}

VOID CCursorManager::SetRace( RwUInt8 eRace )
{
	if( eRace == m_eCurrentRace )
		return;

	switch( eRace )
	{
	case RACE_HUMAN: SetHumanCursor(); break;
	case RACE_NAMEK: SetNamekCursor(); break;
	case RACE_MAJIN: SetMajinCUrsor(); break;
	default: DBO_FAIL("Invalid race : " << eRace);
	}

	m_eCurrentRace = eRace;	
}

VOID CCursorManager::SetHumanCursor(VOID)
{
	std::string strPath = CURSOR_PATH;
	std::string strFilename;

	strFilename = strPath + "01.cur";
	m_hMouseCursor[CS_MOVE_NORMAL] = LoadCursorFromFile( strFilename.c_str() );
	strFilename = strPath + "04.cur";
	m_hMouseCursor[CS_MOVE_DISABLE] = LoadCursorFromFile( strFilename.c_str() );
	strFilename = strPath + "07.cur";
	m_hMouseCursor[CS_BATTLE_LOOTING] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "29.cur";
	m_hMouseCursor[CS_GATHER_DISABLED] = LoadCursorFromFile(strFilename.c_str());
}

VOID CCursorManager::SetNamekCursor(VOID)
{
	std::string strPath = CURSOR_PATH;
	std::string strFilename;

	strFilename = strPath + "00.cur";
	m_hMouseCursor[CS_MOVE_NORMAL] = LoadCursorFromFile( strFilename.c_str() );
	strFilename = strPath + "03.cur";
	m_hMouseCursor[CS_MOVE_DISABLE] = LoadCursorFromFile( strFilename.c_str() );
	strFilename = strPath + "06.cur";
	m_hMouseCursor[CS_BATTLE_LOOTING] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "30.cur";
	m_hMouseCursor[CS_GATHER_DISABLED] = LoadCursorFromFile(strFilename.c_str());
}

VOID CCursorManager::SetMajinCUrsor(VOID)
{
	std::string strPath = CURSOR_PATH;
	std::string strFilename;

	strFilename = strPath + "02.cur";
	m_hMouseCursor[CS_MOVE_NORMAL] = LoadCursorFromFile( strFilename.c_str() );
	strFilename = strPath + "05.cur";
	m_hMouseCursor[CS_MOVE_DISABLE] = LoadCursorFromFile( strFilename.c_str() );
	strFilename = strPath + "08.cur";
	m_hMouseCursor[CS_BATTLE_LOOTING] = LoadCursorFromFile( strFilename.c_str() );

	strFilename = strPath + "31.cur";
	m_hMouseCursor[CS_GATHER_DISABLED] = LoadCursorFromFile(strFilename.c_str());
}