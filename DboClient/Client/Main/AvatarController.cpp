#include "precomp_dboclient.h"
#include "AvatarController.h"

// shared
#include "NtlMovement.h"
#include "NtlResultCode.h"
#include "NtlDirection.h"

// core
#include "NtlSysEvent.h"
#include "NtlEventQueue.h"

// sound
#include "NtlSoundDefines.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLGuiManager.h"
#include "NtlDNController.h"
#include "NtlPostEffectCamera.h"

// simulation
#include "InputActionMap.h" 
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobIcon.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"
#include "NtlSobTriggerObject.h"
#include "NtlSkillContainer.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLVisualDeclear.h"
#include "NtlCameraManager.h"
#include "NtlSobHTBSkill.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"
#include "NtlDTCinematicManager.h"
#include "NtlFSMUtil.h"
#include "NtlFSMDef.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMLayer.h"
#include "NtlNetSender.h"
#include "NtlSobVehicle.h"

// framework
#include "NtlApplication.h"

// dbo 
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DumpCommand.h"
#include "DboLogic.h"
#include "IconMoveManager.h"
#include "CursorManager.h"
#include "AlarmManager.h"
#include "DialogManager.h"

#include "NtlPLObject.h"		//Test 추후 삭제 예정(by HoDong)
#include "NtlPLEventGenerator.h"//Test 추후 삭제 예정(by Peessi)
#include "NtlPLEvent.h"

#include "TBGui.h"
#include "TBTournamentGui.h"

#include "NtlPlWorldEntity.h"

const char* SOUND_SL_GROUND_PICKING[4] = {"Effect\\GENERAL_Picking_Ground.wav", 
"Effect\\GENERAL_Picking_Ground-01.wav",
"Effect\\GENERAL_Picking_Ground-02.wav",
"Effect\\GENERAL_Picking_Ground-03.wav"};


#define NTL_ZOOM_INOUT_SENSITIVITY		0.03f
#define NTL_MOUSE_SENSITIVITY_SCALE		0.5f
#define NTL_HEADING_UPDATE_TIME			5.0f
#define NTL_HEADING_DELTA_TIME			0.3f
#define NTL_HEADING_DELTA_ANGLE			10.0f
#define NTL_MAX_HEADING_ANGLE_X			60.0f

#define KEY_LOOTING_TIME				0.1f

#define PICK_FOCUS_TIME					0.1f

CAvatarController::CAvatarController()
{
	m_bHideMouse = false;
	m_bLButtonMoveFlag				= FALSE;
	m_bRButtonMoveFlag				= FALSE;
	m_bChangeHeadingFlag			= FALSE;
	m_bSkipMouseWheel				= FALSE;
	m_bAutoRunKeyboardChangeHeading	= FALSE;
	m_fChangeHeadingUpdateTime		= 0.0f;
	m_fChangeHeadingDeltaTime		= 0.0f;

	m_fPickTime						= 0.0f;

	m_bMouseAutoRun = false;
	m_fChangeHeadingDeltaAngleY = 0.0f;

	m_fChangeHeadingDeltaAngleX = 0.0f;

	m_hFocusSerialId				= INVALID_SERIAL_ID;

	m_iDeltaLButtonMouseX			= 0;
	m_iDeltaLButtonMouseY			= 0;
	m_iDeltaRButtonMouseX = 0;
	m_iDeltaRButtonMouseY = 0;

	m_fMoveMarkTime					= 0.0f;
	m_pMoveMark						= NULL;
	m_bAutoRun						= FALSE;
	
	m_eRotFlag						= CAMERA_ROTATE_FLAG_NONE;

	m_fKeyLootingTime				= 0.0f;

	m_bChangeAngleX = false;
	m_bChangeAngleXUp = false;

	m_bLButtonDownFlag = false;
	m_bRButtonDownFlag = false;

	sm_cameraMoveCount = 0;
}

CAvatarController::~CAvatarController()
{
	
}

RwBool CAvatarController::Create()
{
	m_handleMouseWheel = CInputHandler::GetInstance()->LinkMouseWheel(this, &CAvatarController::MouseWheelHandler);
	m_handleMouseDown  = CInputHandler::GetInstance()->LinkMouseDown(this, &CAvatarController::MouseDownHandler);
	m_handleMouseUp    = CInputHandler::GetInstance()->LinkMouseUp(this, &CAvatarController::MouseUpHandler);
	m_handleKeyDown    = CInputHandler::GetInstance()->LinkKeyDown(this, &CAvatarController::KeyboardDownHandler);
	m_handleKeyUp      = CInputHandler::GetInstance()->LinkKeyUp(this, &CAvatarController::KeyboardUpHandler);
	m_handleRestoreHandler = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore(this, &CAvatarController::RestoreHandler);

	GetInputActionMap()->LinkKeyboardMove(this, &CAvatarController::ActionMapKeyboardMove);
	GetInputActionMap()->LinkKeyboardDashMove(this, &CAvatarController::ActionMapKeyboardDashMove);
    GetInputActionMap()->LinkAutoRun(this, &CAvatarController::ActionAutoRun);
	GetInputActionMap()->LinkAvatarSelect(this, &CAvatarController::ActionAvatarSelect);
	GetInputActionMap()->LinkJump(this, &CAvatarController::ActionJump);
	GetInputActionMap()->LinkCharging(this, &CAvatarController::ActionCharging);
	GetInputActionMap()->LinkBlocking(this, &CAvatarController::ActionBlocking);
	GetInputActionMap()->LinkSitAndStand(this, &CAvatarController::ActionSitAndStand);
	GetInputActionMap()->LinkLooting(this, &CAvatarController::ActionLooting);
	GetInputActionMap()->LinkAutoTarget(this, &CAvatarController::ActionAutoTarget);
	GetInputActionMap()->LinkAutoAttack(this, &CAvatarController::ActionAutoAttack);
	GetInputActionMap()->LinkAutoFollow(this, &CAvatarController::ActionAutoFollow);
	GetInputActionMap()->LinkPartySelect(this, &CAvatarController::ActionPartySelect);
    GetInputActionMap()->LinkScouterUse(this, &CAvatarController::ActionScouterUse);
    GetInputActionMap()->LinkTargetMarking(this, &CAvatarController::ActionPartyShareTargetMarking);
    GetInputActionMap()->LinkTargetSelecting(this, &CAvatarController::ActionPartyShareTargetSelect);
    GetInputActionMap()->LinkPartyAutoSelect(this, &CAvatarController::ActionPartyAutoSelect);
	
	LinkMsg(g_EventQueueProcessFilter);
	LinkMsg(g_EventCameraShake);
	LinkMsg(g_EventSobDelete);
	LinkMsg(g_EventSobPickWorldItem);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventWorldChange);
	LinkMsg(g_EventHTBRPStock);
	LinkMsg(g_EventDialog);	
    LinkMsg(g_EventActionFuncBasicAttack);
    LinkMsg(g_EventActionFuncAssist);
    LinkMsg(g_EventActionFuncGetItem);
    LinkMsg(g_EventActionFuncSitAndStand);
	
	//test
	LinkMsg(g_EventSCS);
	
	m_uiCurMoveFlags	= 0;
	m_bAutoRun			= FALSE;

	// camera zoom min and max setting
	// disable by daneos
	/*if(Logic_IsDevUser())
	{
		SDevMasterInfo *pDevMasterInfo = API_GetSLDevMasterInformation();
		GetNtlGameCameraManager()->SetMinDist(pDevMasterInfo->fCameraZoomMin);
		GetNtlGameCameraManager()->SetMaxDist(pDevMasterInfo->fCameraZoomMax);
		GetNtlGameCameraManager()->SetFov(pDevMasterInfo->fCameraFov);
		GetNtlDTCinematicManager()->SetFov(pDevMasterInfo->fCameraFov);
	}*/

	return TRUE;
}

void CAvatarController::Destroy()
{
	CInputHandler::GetInstance()->UnLinkMouseWheel(m_handleMouseWheel);
	CInputHandler::GetInstance()->UnLinkMouseDown(m_handleMouseDown);
	CInputHandler::GetInstance()->UnLinkMouseUp(m_handleMouseUp);

	CInputHandler::GetInstance()->UnLinkKeyDown(m_handleKeyDown);
	CInputHandler::GetInstance()->UnLinkKeyUp(m_handleKeyUp);

	GetInputActionMap()->UnLinkKeyboardMove();
	GetInputActionMap()->UnLinkKeyboardDashMove();
	GetInputActionMap()->UnLinkAvatarSelect();
	GetInputActionMap()->UnLinkJump();
	GetInputActionMap()->UnLinkCharging();
	GetInputActionMap()->UnLinkBlocking();
	GetInputActionMap()->UnLinkSitAndStand();
	GetInputActionMap()->UnLinkLooting();
	GetInputActionMap()->UnLinkAutoTarget();
	GetInputActionMap()->UnLinkAutoAttack();
	GetInputActionMap()->UnLinkAutoFollow();
	GetInputActionMap()->UnLinkPartySelect();
    GetInputActionMap()->UnLinkScouterUse();
    GetInputActionMap()->UnLinkTargetMarking();
    GetInputActionMap()->UnLinkTargetSelecting();
    GetInputActionMap()->UnLinkPartyAutoSelect();
		
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore(m_handleRestoreHandler);

	UnLinkMsg(g_EventQueueProcessFilter);
	UnLinkMsg(g_EventCameraShake);
	UnLinkMsg(g_EventSobDelete);
	UnLinkMsg(g_EventSobPickWorldItem);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventWorldChange);
	UnLinkMsg(g_EventHTBRPStock);
	UnLinkMsg(g_EventDialog);
    UnLinkMsg(g_EventActionFuncBasicAttack);
    UnLinkMsg(g_EventActionFuncAssist);
    UnLinkMsg(g_EventActionFuncGetItem);
    UnLinkMsg(g_EventActionFuncSitAndStand);

	//test
	UnLinkMsg(g_EventSCS);
	
	// move mark를 삭제한다.
	DeleteMoveMark();
}

void CAvatarController::Update(RwReal fElapsed)
{
	// update move mark
	UpdateMoveMark(fElapsed);

	// update change heading
	UpdateChangeHeading(fElapsed);

	// update flight
	UpdateFlightAngle(fElapsed);

	CalcCameraRotateToCharBack(); // decide wether to move cam to char-back or not

	// Update Mouse
	UpdateMouse(fElapsed);

	UpdateAutoRun(fElapsed);

	UpdateCameraRotateToCharBack(fElapsed); // move cam to char back

	GetNtlGameCameraManager()->Update(fElapsed);

	m_fKeyLootingTime += fElapsed;

	UpdatePickFocus(fElapsed);	

	Logic_UpdateSkillResTimeOut(fElapsed);
}

void CAvatarController::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventQueueProcessFilter)
	{
		if(m_uiCurMoveFlags == NTL_MOVE_NONE)
		{
			EventQueueCurrentProcessFilterMsg(g_EventKeyboardMove);
		}
	}
	else if(pMsg.Id == g_EventSobDelete)
	{
		SNtlEventSobDelete* pData  = (SNtlEventSobDelete *)pMsg.pData;
		if(pData->hSerialId == Logic_GetAvatarTargetHandle())
		{
			Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
		}
	}
	else if(pMsg.Id == g_EventSobPickWorldItem)
	{
		SNtlEventSobPickWorldItem* pData = (SNtlEventSobPickWorldItem*)pMsg.pData;
		
		Logic_WorldItemLooting( GetNtlSobManager()->GetSobObject( pData->hSerialId ) );
	}
	else if(pMsg.Id == g_EventIconPopupResult)
	{
		SDboEventIconPopupResult* pData = (SDboEventIconPopupResult*)pMsg.pData;

		if( pData->nWorkId == PMW_FOLLOW )
			ActionAutoFollow();
	}
	else if(pMsg.Id == g_EventWorldChange)
	{
		TeleportReset();
	}
	else if(pMsg.Id == g_EventHTBRPStock)
	{
		m_bHTBRPStock = TRUE;
	}
	else if(pMsg.Id == g_EventDialog)
	{
		SDboEventDialog* pEvent = reinterpret_cast<SDboEventDialog*>( pMsg.pData );

		if( pEvent->iType == DIALOGEVENT_SKIP_MOUSE_WHEEL )
		{
			if( pEvent->iSrcDialog == DIALOG_CHAT )
			{
				m_bSkipMouseWheel = TRUE;
			}
		}
	}
    else if(pMsg.Id == g_EventActionFuncBasicAttack)
    {
		// by daneos..
        //CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        //if(!pTargetActor || pTargetActor->GetClassID() != SLCLASS_MONSTER)  // Only available for monster
        //    return;

        ActionAutoAttack();
    }
    else if(pMsg.Id == g_EventActionFuncAssist)
    {
		// by daneos.
        //CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        //if(!pTargetActor || pTargetActor->GetClassID() != SLCLASS_PLAYER)   // Available only for players
        //    return;
        //ActionAutoAttack();

		ActionAutoTarget();
    }
    else if(pMsg.Id == g_EventActionFuncGetItem)
    {
        ActionLooting();
    }
	else if(pMsg.Id == g_EventActionFuncSitAndStand)
    {
        ActionSitAndStand(1);
    }
	else if(pMsg.Id == g_EventSCS )
	{
		GetAlarmManager()->AlarmMessage( L"It is an auto care system !!" ,CAlarmManager::ALARM_ACTION_NOTIFY);	
	}
}

RwUInt8 CAvatarController::ConvertStraightMoveFlags(RwUInt8 uiMoveFlags)
{
	if (uiMoveFlags == NTL_MOVE_NONE)
		return NTL_MOVE_NONE;
	else if (uiMoveFlags == NTL_MOVE_B || uiMoveFlags == NTL_MOVE_B_TURN_L || uiMoveFlags == NTL_MOVE_B_TURN_R)
		return NTL_MOVE_B;
	else if (uiMoveFlags == NTL_MOVE_L || uiMoveFlags == NTL_MOVE_TURN_L || uiMoveFlags == NTL_MOVE_L_TURN_R || uiMoveFlags == NTL_MOVE_L_TURN_L)
		return NTL_MOVE_L;
	else if (uiMoveFlags == NTL_MOVE_R || uiMoveFlags == NTL_MOVE_TURN_R || uiMoveFlags == NTL_MOVE_R_TURN_R || uiMoveFlags == NTL_MOVE_R_TURN_L)
		return NTL_MOVE_R;

	return NTL_MOVE_F;
}

RwReal CAvatarController::CalcAvatarStraightRotate(RwUInt32 uiMoveFlags)
{
	RwReal fYaw = GetNtlGameCameraManager()->GetYaw();

	if(uiMoveFlags == NTL_MOVE_F_TURN_L)
	{
		fYaw += 45.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_F_TURN_R)
	{
		fYaw -= 45.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_B_TURN_L)
	{
		fYaw -= 45.0f;
	}
	else if(uiMoveFlags == NTL_MOVE_B_TURN_R)
	{
		fYaw += 45.0f;
	}

	return fYaw;
}


void CAvatarController::CalcCameraRotateSensitivity(int iPosX, int iPosY, int iMouseDeltaX, int iMouseDeltaY)
{
	RwReal fDeltaYaw = ((RwReal)iMouseDeltaX * NTL_MOUSE_SENSITIVITY_SCALE);
	RwReal fDeltaPitch = ((RwReal)iMouseDeltaY * NTL_MOUSE_SENSITIVITY_SCALE);
	
	GetNtlGameCameraManager()->AddYaw(fDeltaYaw);
	GetNtlGameCameraManager()->AddPitch(-fDeltaPitch);
}

void CAvatarController::TeleportReset(void)
{
	m_uiCurMoveFlags				= NTL_MOVE_NONE;
	m_bLButtonMoveFlag				= FALSE;
	m_bRButtonMoveFlag				= FALSE;
	m_bChangeHeadingFlag			= FALSE;
	m_bAutoRunKeyboardChangeHeading	= FALSE;
	m_fChangeHeadingUpdateTime		= 0.0f;
	m_fChangeHeadingDeltaTime		= 0.0f;
	m_fChangeHeadingDeltaAngleY = 0.0f;
	m_fChangeHeadingDeltaAngleX = 0.0f;

	m_bMouseAutoRun = false;

	m_iDeltaLButtonMouseX = 0;
	m_iDeltaLButtonMouseY = 0;
	m_iDeltaRButtonMouseX = 0;
	m_iDeltaRButtonMouseY = 0;

	m_fMoveMarkTime					= 0.0f;
	m_bAutoRun						= FALSE;
	
	m_eRotFlag						= CAMERA_ROTATE_FLAG_NONE;

	m_fKeyLootingTime				= 0.0f;

	m_bChangeAngleX = false;
	m_bChangeAngleXUp = false;
}

void CAvatarController::PickPLWorld(SWorldPickInfo& sPickInfo)
{
	//m_uiCurMoveFlags = NTL_MOVE_F;// not sure

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD))
		return;
	
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pSobAvatar == NULL)
		return;

//	if (Logic_GetActorStateId(pSobAvatar) != NTL_FSMSID_FIGHTING_POSE)
	{
		if (pSobAvatar->CanMove())
		{
			SetAutoRun(FALSE);

			CNtlSLEventGenerator::TerrainClick(sPickInfo.vPickPos);

			SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
			if (hTargetSerialId != INVALID_SERIAL_ID)
				CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);

			//Every time I shoot a Mark, I create it. Performance is expected to decline
			CreateMoveMark(&sPickInfo.vPickPos);
		}
	}
}

void CAvatarController::PickPLObject(SWorldPickInfo& sPickInfo, int someEnum, RwInt32 nXPos, RwInt32 nYPos)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
		return;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pSobAvatar)
		return;

    if(Logic_IsDirectMode(pSobAvatar))
        return;

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_TARGETING ) )
		return;

	RwUInt32 actorStateId = Logic_GetActorStateId(pSobAvatar);
	if (actorStateId == NTL_FSMSID_FAINTING || actorStateId == NTL_FSMSID_DIE)
		return;

	CNtlPLEntity* pEntity = sPickInfo.pPLEntity;
	if (pEntity == NULL)
		return;

	SERIAL_HANDLE hTarget = pEntity->GetSerialID();

	CNtlSobActor *pPickActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hTarget) );
	if(pPickActor == NULL)
		return;

	if (pPickActor->GetClassID() == SLCLASS_VEHICLE)
	{
		hTarget = pPickActor->GetOwnerID();
	}

	if(!pPickActor->CanClick())
		return;

	DeleteMoveMark();

	if (someEnum == 1) // left mouse button
	{
		RwBool bTSSuccess = Logic_SobTarget(hTarget, INVALID_BYTE);
		if (!bTSSuccess)
			return;

		//sub_457840(hTarget); // to do

		// if using scouter then measure power on selecting target
		if (pSobAvatar->GetOtherParam()->IsScouterOn())
		{
			CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_MEASURE_POWER_START, hTarget);
		}
	}
	else if (someEnum == 3) // right mouse button
	{
		if (Logic_IsEnemyTargetFromAvatarActor(hTarget))
		{
			if (GetNtlWorldConcept()->IsEnableAction(E_ACTION_ATTACK) && Logic_IsVehicleDriver(pSobAvatar) == FALSE && pPickActor->CanHurted())
			{
				if (Logic_isEnemyTargetOcclusion(hTarget))
				{
					GetAlarmManager()->AlarmMessage("DST_SKILL_OCCLUSION_CHECK");
				}
				else
				{
					if (m_uiCurMoveFlags)
					{
						CNtlSLEventGenerator::KeyboardMoveStop();
					}

					CNtlSLEventGenerator::SobAttackSelect(hTarget);
				}
			}
		}
		else
		{
			if (Logic_IsCommunityActor(pPickActor))
			{
				RwReal fCummuRange = Logic_GetCommunityRange(pPickActor);
				if (Logic_CanActorCommunity(pSobAvatar, pPickActor, fCummuRange))
				{
					Logic_SobTarget(hTarget, DBO_MOVE_FOLLOW_INVALID); // DONT FOLLOW

					Logic_BeginActorCommunity(pSobAvatar, pPickActor);
				}
				else
				{
					Logic_SobTarget(hTarget, DBO_MOVE_FOLLOW_AUTO_ATTACK); // DBO_MOVE_FOLLOW_AUTO_ATTACK = follow
				}
			}
			else
			{
				RwBool bTSSuccess = Logic_SobTarget(hTarget, INVALID_BYTE);
				if (!bTSSuccess)
					return;

				// if using scouter then measure power on selecting target
				if (pSobAvatar->GetOtherParam()->IsScouterOn())
				{
					CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_MEASURE_POWER_START, hTarget);
				}

				if (pPickActor->GetClassID() == SLCLASS_PLAYER)
				{
					CDboEventGenerator::IconPopupShow(TRUE, pPickActor->GetSerialID(), PLACE_SUB_TARGETUI, 0, nXPos, nYPos);
				}
				else if (pPickActor->GetClassID() == SLCLASS_VEHICLE)
				{
					CDboEventGenerator::IconPopupShow(TRUE, pPickActor->GetOwnerID(), PLACE_SUB_TARGETUI, 0, nXPos, nYPos);
				}
			}
		}
	}
}

void CAvatarController::PickPLObject(SWorldPickInfo & sPickInfo)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return;

	if (Logic_IsDirectMode(pAvatar))
		return;

	if (!Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_TARGETING))
		return;

	RwUInt32 actorStateId = Logic_GetActorStateId(pAvatar);
	if (actorStateId == NTL_FSMSID_FAINTING || actorStateId == NTL_FSMSID_DIE)
		return;

	CNtlPLEntity * pEntity = sPickInfo.pPLEntity;
	if (pEntity == NULL)
		return;

	SERIAL_HANDLE hTarget = pEntity->GetSerialID();

	CNtlSobActor * pPickActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(hTarget));
	if (pPickActor == NULL)
		return;

	if (!pPickActor->CanClick())
		return;

	if (hTarget != Logic_GetAvatarHandle()) // dont select ourself when cast skill on ourself.
	{
		if (pPickActor->GetClassID() == SLCLASS_VEHICLE)
			hTarget = pPickActor->GetOwnerID();

		bool bSelectSuccess = Logic_SobTarget(hTarget, INVALID_BYTE);
		if (!bSelectSuccess)
			return;
	}

	DeleteMoveMark();

	Logic_UseProc(GetNtlSLGlobal()->GetSkillSelectModeHandle(), INVALID_BYTE, hTarget, true);
}

void CAvatarController::MouseUpLBtnHandler(const SMouseData *pData)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pSobAvatar == NULL)
		return;

	GetNtlGameCameraManager()->UserCameraControlEnable(FALSE); // disable rotate camera
	
	if (m_bMouseAutoRun)
		return;

	m_bLButtonMoveFlag = FALSE;

	if (m_bRButtonDownFlag)
		return;

	// mouse click 
	if(pData->uiDownDeltaTime <= 300 && m_iDeltaLButtonMouseX < 20 && m_iDeltaLButtonMouseY < 20)
	{
		if (Logic_IsDirectMode(pSobAvatar))
			return;

		if (GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
		{
			if (GetIconMoveManager()->GetSrcPlace() == PLACE_QUICKSLOT)
			{
				CDboEventGenerator::QuickSlotDelNfy(GetIconMoveManager()->GetSrcSlotIdx(), -1, TRUE);

				return;
			}
			if (GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
			{
				Logic_ItemDropConfirmProc();
				return;
			}
		}

		if (!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD) && !GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
			return;

		if (Logic_IsClickDisable())
			return;

		m_sPickInfo.bTerrainExclusion	= FALSE;
		m_sPickInfo.bCharacterExclusion = FALSE;
		

		RwBool bPickWorld = GetSceneManager()->PickWorld(pData->iPosX, pData->iPosY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);

		if (GetNtlSLGlobal()->IsSkillSelectMode() && pData->bDblClick == false)
		{
			PickPLObject(m_sPickInfo);

			if (GetNtlSLGlobal()->IsSkillSelectMode())
			{
				CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
			}

			return;
		}


		if (bPickWorld)
		{
			if (pData->bDblClick)
				return;

			ENtlPLEntityType ePLType = m_sPickInfo.pPLEntity->GetClassType();
			if (ePLType == PLENTITY_WORLD)
			{
				PickPLWorld(m_sPickInfo);
				return;
			}
			else if (ePLType == PLENTITY_OBJECT)
			{
				if (m_sPickInfo.pPLEntity->GetSerialID() == INVALID_SERIAL_ID) // check if its not an object like popo stone
				{
					PickPLWorld(m_sPickInfo);
					return;
				}
				else
				{
					// Required so we can walk on objects like "Statue of time and Dreams"
					CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_sPickInfo.pPLEntity->GetSerialID());
					if (pSobObj && (pSobObj->GetFlags() & SLFLAG_NOT_TARGETED))
					{
						PickPLWorld(m_sPickInfo);
						return;
					}
				}
			}

			PickPLObject(m_sPickInfo, 1, 0, 0);

			if (pData->uiFlags & MK_CONTROL)
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyShareTargetReq((RwUInt8)Logic_GetShareTargetBlankSlot(), m_sPickInfo.pPLEntity->GetSerialID());
			}
		}
		else if(pSobAvatar->IsAirMode())
		{
			DBO_WARNING_MESSAGE("CAvatarController::MouseUpLBtnHandler: bPickWorld == true && IsAirMode() == true");
		}
	}
}

void CAvatarController::MouseUpRBtnHandler(const SMouseData *pData)// release right button
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (m_bMouseAutoRun)
		return;

	m_bRButtonMoveFlag = FALSE;
	m_bChangeHeadingFlag = FALSE; // stop change heading
	m_fChangeHeadingDeltaTime = 0.0f;
	GetNtlGameCameraManager()->UserCameraControlEnable(FALSE); // disable rotate camera

	if (m_eRotFlag != CAMERA_ROTATE_FLAG_NONE)
		return;

	if (m_bLButtonDownFlag)
		return;

	if(pData->uiDownDeltaTime <= 300 && m_iDeltaRButtonMouseX < 10 && m_iDeltaRButtonMouseY < 10)
	{
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD) && !GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
			return;

		m_sPickInfo.bTerrainExclusion = FALSE;
		m_sPickInfo.bCharacterExclusion = FALSE;

		if(!pData->bDblClick)
		{
			RwBool bPick = GetSceneManager()->PickWorld(pData->iPosX, pData->iPosY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);
			if (bPick)
			{
				ENtlPLEntityType ePLType = m_sPickInfo.pPLEntity->GetClassType();
				if (ePLType != PLENTITY_WORLD)
				{
					if (ePLType == PLENTITY_OBJECT)
					{
						if (m_sPickInfo.pPLEntity->GetSerialID() != INVALID_SERIAL_ID)
							PickPLObject(m_sPickInfo, 3, 0, 0);
					}
					else
					{
						PickPLObject(m_sPickInfo, 3, pData->iPosX, pData->iPosY);
					}
				}
			}
		}
	}
}

void CAvatarController::MouseMoveLBtnHandler(int iPosX, int iPosY)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return;

	if (!m_bLButtonMoveFlag) // check if we start using left mouse movement
	{
		m_iOldLButtonMouseX = iPosX;
		m_iOldLButtonMouseY = iPosY;

		m_bLButtonMoveFlag = TRUE;
		GetNtlGameCameraManager()->UserCameraControlEnable(TRUE); // enable rotate camera
		return;
	}
	
	int iDeltaX = m_iOldLButtonMouseX - iPosX;
	int iDeltaY = m_iOldLButtonMouseY - iPosY;

	m_iDeltaLButtonMouseX += abs(iDeltaX);
	m_iDeltaLButtonMouseY += abs(iDeltaY);

	CalcCameraRotateSensitivity(iPosX, iPosY, iDeltaX, iDeltaY);

	m_iOldLButtonMouseX = iPosX;
	m_iOldLButtonMouseY = iPosY;

	if (iDeltaX == 0 && iDeltaY == 0)
	{
		ResetMousePosition();
	}
}

void CAvatarController::MouseMoveRBtnHandler(int iPosX, int iPosY)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pSobAvatar == NULL)
		return;

	if (!m_bRButtonMoveFlag)
	{
		m_iOldRButtonMouseX = iPosX;
		m_iOldRButtonMouseY = iPosY;

		m_bRButtonMoveFlag = TRUE;
		GetNtlGameCameraManager()->UserCameraControlEnable(TRUE); // enable rotate camera
		return;
	}

	RwBool bActiveWorldConcept = GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHANGE_HEADING);

	RwBool bChangeHeadingEnable = FALSE;

	if(m_bAutoRun)
	{
		if (!bActiveWorldConcept)
		{
			SetAutoRun(FALSE);
		}

		if (bActiveWorldConcept && !Logic_IsVehicleDriver(pSobAvatar))
		{
			bChangeHeadingEnable = TRUE;
		}
	}
	else
	{
		if (bActiveWorldConcept && !Logic_IsVehicleDriver(pSobAvatar))
		{
			bChangeHeadingEnable = TRUE;
		}
	}

	RwUInt32 actorStateId = Logic_GetActorStateId(pSobAvatar);
	if (actorStateId == NTL_FSMSID_DASH || actorStateId == NTL_FSMSID_SITDOWN || actorStateId == NTL_FSMSID_SIT || actorStateId == NTL_FSMSID_FIGHTING || actorStateId == NTL_FSMSID_SLEEP || actorStateId == NTL_FSMSID_PARALYZE
		|| actorStateId == NTL_FSMSID_SKILL_CASTING || actorStateId == NTL_FSMSID_SKILL_ACTION || actorStateId == NTL_FSMSID_SOCIAL_ACTION 
		|| actorStateId == NTL_FSMSID_FAINTING || actorStateId == NTL_FSMSID_DIE || actorStateId == NTL_FSMSID_AIR_JUMP)
		bChangeHeadingEnable = FALSE;

	else if (!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		bChangeHeadingEnable = FALSE;

	// disabled: to fix change heading while jumping
	//else if (!pSobAvatar->CanMove())
	//	bChangeHeadingEnable = FALSE;

	else if (Logic_IsDirectMode(pSobAvatar))
		bChangeHeadingEnable = FALSE;

	RwInt32 iDeltaX = m_iOldRButtonMouseX - iPosX;
	RwInt32 iDeltaY = m_iOldRButtonMouseY - iPosY;

	m_iDeltaRButtonMouseX += abs(iDeltaX);
	m_iDeltaRButtonMouseY += abs(iDeltaY);

	if(bChangeHeadingEnable && iDeltaX != 0 && iDeltaX != 0)
	{
		if(!m_bChangeHeadingFlag)
		{
			m_bChangeHeadingFlag		= TRUE;
			m_fChangeHeadingUpdateTime	= 0.0f;
			m_fChangeHeadingDeltaTime	= 0.0f;
			m_fChangeHeadingDeltaAngleY = 0.0f;
			m_fChangeHeadingDeltaAngleX = 0.0f;


			RwReal fYaw = GetNtlGameCameraManager()->GetYaw();
			pSobAvatar->SetAngleY(fYaw);

			if (pSobAvatar->IsAirMode())
			{
				RwReal fPitch = GetNtlGameCameraManager()->GetPitch();
				if (fabsf(fPitch) < NTL_MAX_HEADING_ANGLE_X)
					pSobAvatar->SetAngleX(fPitch);

				m_fChangeHeadingDeltaAngleX += (float)iDeltaY * NTL_MOUSE_SENSITIVITY_SCALE;
			}

			RwV3d vDir = pSobAvatar->GetDirection();
			API_GetSLPacketGenerator()->SendChangeHeading(vDir);
		}

		m_fChangeHeadingDeltaAngleY += (float)iDeltaX * NTL_MOUSE_SENSITIVITY_SCALE;

		RwBool bSuccess = true;
		CNtlSLEventGenerator::InputChagneHeading(false, (float)iDeltaX * NTL_MOUSE_SENSITIVITY_SCALE, pSobAvatar->IsAirMode() == TRUE, (float)iDeltaY * NTL_MOUSE_SENSITIVITY_SCALE, bSuccess);
	}

	CalcCameraRotateSensitivity(iPosX, iPosY, iDeltaX, iDeltaY);

	m_iOldRButtonMouseX = iPosX;
	m_iOldRButtonMouseY = iPosY;

	if (iDeltaX == 0 && iDeltaY == 0)
	{
		ResetMousePosition();
	}
}

void CAvatarController::ResetMousePosition()
{
	int x = GetCursorManager()->GetOldPosX();
	int y = GetCursorManager()->GetOldPosY();

	m_bRButtonMoveFlag = FALSE;
	m_bLButtonMoveFlag = FALSE;

	m_iOldLButtonMouseX = x;
	m_iOldLButtonMouseY = y;
	m_iOldRButtonMouseX = x;
	m_iOldRButtonMouseY = y;

	GetCursorManager()->SetMouseCursorPosition(x, y);
}

void CAvatarController::CalcCameraRotateToCharBack(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pSobAvatar == NULL)
		return;

	if (!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return;

	if (m_bAutoRun)
	{
		if (m_uiCurMoveFlags != NTL_MOVE_NONE || m_bLButtonDownFlag || m_bRButtonMoveFlag)
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_NONE;
			return;
		}
		else
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_ADJUST;
			return;
		}
	}
	else
	{
		if (m_uiCurMoveFlags == NTL_MOVE_NONE || m_bLButtonDownFlag || m_bRButtonMoveFlag)
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_NONE;
			return;
		}
		else
		{
			m_eRotFlag = CAMERA_ROTATE_FLAG_ADJUST;
			return;
		}
	}

	if (m_eRotFlag == CAMERA_ROTATE_FLAG_ACCORD)
	{
		RwV3d vDir = pSobAvatar->GetDirection();
		RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);
	
		GetNtlGameCameraManager()->SetYaw(fAngleY);

		m_eRotFlag = CAMERA_ROTATE_FLAG_NONE;
	}
}


void CAvatarController::UpdateCameraRotateToCharBack(RwReal fElapsed)
{
	if(m_eRotFlag != CAMERA_ROTATE_FLAG_ADJUST)
		return;

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return;

    CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pSobAvatar == NULL)
		return;

	RwUInt32 stateID = Logic_GetActorStateId(pSobAvatar);
	if (stateID == NTL_FSMSID_DASH)
		return;
	
	RwReal fDeltaAngleY = 180.0f * fElapsed;

	RwV3d vDir = pSobAvatar->GetDirection();
	RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);
	RwReal fCovAngleY = GetNtlGameCameraManager()->GetYaw();


	if (fAngleY < 0.0f)
		fAngleY += 360.0f;
	if (fCovAngleY >= 360.0f)
		fCovAngleY -= 360.0f;

	// Rotation is counterclockwise rotation.
	// subtract the character angle from the camera angle.
	RwReal fSubAngleY = fCovAngleY - fAngleY;

	if (fSubAngleY > 0.0f)
	{
		if (fSubAngleY > 180.0f) // Increase the angle.
			fSubAngleY = 360.0f - fSubAngleY;
		else // Reduce the angle.
			fSubAngleY = -fSubAngleY; //(This is counterclockwise and must be removed.)
	}
	else
	{
		if (fSubAngleY > -180.0f) // Increase the angle.
			fSubAngleY = fabsf(fSubAngleY);
		else // Reduce the angle.
			fSubAngleY = -(360.0f - fabs(fSubAngleY)); // It is counterclockwise and must be removed.
	}

	if (fabsf(fSubAngleY) < fDeltaAngleY)
	{
		RwV3d vDir = pSobAvatar->GetDirection();
		RwReal fAngleY = CNtlMath::LineToAngleY(&vDir);

		GetNtlGameCameraManager()->SetYaw(fAngleY);
		
		m_eRotFlag = CAMERA_ROTATE_FLAG_ACCORD;
	}
	else
	{
		if (fSubAngleY > 0.0f)
			GetNtlGameCameraManager()->AddYaw(fDeltaAngleY);
		else
			GetNtlGameCameraManager()->AddYaw(-fDeltaAngleY);
	}
}

void CAvatarController::UpdateAutoRun(RwReal fElapsed)
{
	if(m_bAutoRun)
	{
		RwBool bMoveState = 0;

		RwV3d vPos;
		RwV3d vDir;
		RwReal fSpeed;

		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		NTL_ASSERT(pSobAvatar, "CAvatarController::UpdateAutoRun: not exist avatar instance");

		if ( !Logic_IsVehicleDriver( pSobAvatar ) )
		{
			bMoveState = FSMUtil_IsMove(pSobAvatar->GetCurrentState());

			vPos	= pSobAvatar->GetPosition();
			vDir	= pSobAvatar->GetDirection();
			fSpeed	= Logic_GetFrontRunSpeed(pSobAvatar);
		}
		else
		{
			CNtlSobVehicle* pVehicleObj = (CNtlSobVehicle*)GetNtlSobManager()->GetSobObject( pSobAvatar->GetVehicleID() );

			if ( pVehicleObj )
			{
				bMoveState = FSMUtil_IsMove(pVehicleObj->GetCurrentState());

				vPos	= pVehicleObj->GetPosition();
				vDir	= pVehicleObj->GetDirection();
				fSpeed	= Logic_GetFrontRunSpeed(pVehicleObj);
			}
			else
			{
				bMoveState = FSMUtil_IsMove(pSobAvatar->GetCurrentState());

				vPos	= pSobAvatar->GetPosition();
				vDir	= pSobAvatar->GetDirection();
				fSpeed	= Logic_GetFrontRunSpeed(pSobAvatar);
			}
		}

		if(!bMoveState)
		{
			SetAutoRun(FALSE);

			m_bMouseAutoRun = true;
			return;
		}

		if ( !Logic_IsVehicleDriver(pSobAvatar) )
		{
			CNtlVector vHeading, vDest;
			NtlGetDestination_Keyboard(vDir.x, vDir.y, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, m_uiCurMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

			RwV3d vNewDir;
			CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, vHeading.y, vHeading.z);

			pSobAvatar->SetDirection(&vNewDir);

			if(m_uiCurMoveFlags == NTL_MOVE_TURN_L || m_uiCurMoveFlags == NTL_MOVE_TURN_R)
			{
				RwReal fAngleY = CNtlMath::LineToAngleY(&vNewDir);
				GetNtlGameCameraManager()->SetYaw(fAngleY);
			}
		}
		else
		{
			CNtlSobVehicle* pSobVehicle = (CNtlSobVehicle*) GetNtlSobManager()->GetSobObject(pSobAvatar->GetVehicleID());

			if ( pSobVehicle )
			{
				RwUInt8 byMoveFlags = 0;

				if (m_uiCurMoveFlags == NTL_MOVE_TURN_L)
				{
					byMoveFlags = NTL_MOVE_F_TURN_L;
				}
				else if (m_uiCurMoveFlags == NTL_MOVE_TURN_R)
				{
					byMoveFlags = NTL_MOVE_F_TURN_R;
				}
				else
				{
					byMoveFlags = NTL_MOVE_F;
				}

				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pSobAvatar->GetBehaviorData());
				SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();
				if ( pMoveStuff->byMoveFlags != byMoveFlags )
				{
					CNtlSLEventGenerator::KeyboardMove(byMoveFlags, FALSE, 0.f);
				}

				CNtlVector vHeading, vDest;
				NtlGetDestination_Keyboard(vDir.x, vDir.z, vDir.y, fSpeed, vPos.x, vPos.y, vPos.z, byMoveFlags, (DWORD)(fElapsed*1000.f), 0.8f, &vHeading, &vDest);

				RwV3d vNewDir;
				CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, vHeading.y, vHeading.z);

				if(m_uiCurMoveFlags == NTL_MOVE_TURN_L || m_uiCurMoveFlags == NTL_MOVE_TURN_R)
				{
					RwReal fAngleY = CNtlMath::LineToAngleY(&vNewDir);
					GetNtlGameCameraManager()->SetYaw(fAngleY);
				}
			}
		}
	}
	//else if(m_bAutoRunKeyboardChangeHeading) // not sure
	//{
	//	ActionAutoRun();
	//}
}

int	CAvatarController::MouseWheelHandler(unsigned int uiMouseData)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAMERA_CONTROL))
		return TRUE;

	if( m_bSkipMouseWheel )
	{
		m_bSkipMouseWheel = FALSE;
		return TRUE;
	}

	SMouseData *pData = (SMouseData *)uiMouseData;

    RwReal fDelta = -pData->zDelta * NTL_ZOOM_INOUT_SENSITIVITY;

    if(GetNtlGameCameraManager()->IsActiveFreeCamera())
    {
        // In the free camera,
        RwReal fSpeed = GetNtlGameCameraManager()->FreeCameraGetMoveSpeed();
        fSpeed = pData->zDelta < 0? max(fSpeed - 0.02f, 0.01f): fSpeed + 0.02f;
        GetNtlGameCameraManager()->FreeCameraSetMoveSpeed(fSpeed);
    }
    else
    {
        GetNtlGameCameraManager()->ZoomInOut(fDelta);
    }

	return TRUE;
}

int CAvatarController::RestoreHandler(void)
{
	return TRUE;
}

int	CAvatarController::MouseDownHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pAvatar == NULL)
		return FALSE;

	if(!pData->bDblClick)
	{
		if (pData->chBtnType == MOUSE_RBTN) // right button
		{
			m_bRButtonDownFlag = true;
			m_iOldRButtonMouseX = pData->iPosX;
			m_iOldRButtonMouseY = pData->iPosY;
			m_iDeltaRButtonMouseX = 0;
			m_iDeltaRButtonMouseY = 0;
		}
		else if(pData->chBtnType == MOUSE_LBTN) // left button
		{
			m_bLButtonDownFlag = true;
			m_iOldLButtonMouseX = pData->iPosX;
			m_iOldLButtonMouseY = pData->iPosY;
			m_iDeltaLButtonMouseX = 0;
			m_iDeltaLButtonMouseY = 0;
		}

		if (pData->uiFlags & MK_LBUTTON && pData->uiFlags & MK_RBUTTON)
		{
			m_bLButtonDownFlag = true;
			m_bLButtonMoveFlag = TRUE;
			m_iOldLButtonMouseX = pData->iPosX;
			m_iOldLButtonMouseY = pData->iPosY;
			m_iDeltaLButtonMouseX = 0;
			m_iDeltaLButtonMouseY = 0;
			m_iDeltaRButtonMouseX = 0;
			m_iDeltaRButtonMouseY = 0;

			m_bMouseAutoRun = true;

			ActionAutoRun();
		}

		if (m_uiCurMoveFlags && pData->chBtnType == MOUSE_RBTN)
		{
			if(pAvatar->IsAirMode() == FALSE)
				ActionMapKeyboardMove(m_uiCurMoveFlags);
		}
		else
		{
			CalcCameraRotateToCharBack();
		}
	}
	else
	{
		m_bChangeHeadingFlag = FALSE;

		if (pData->chBtnType == MOUSE_LBTN)
		{
			m_bLButtonDownFlag = true;
			m_bLButtonMoveFlag = FALSE;
		}
		else if(pData->chBtnType == MOUSE_RBTN)
		{
			m_bRButtonDownFlag = true;
			m_bRButtonMoveFlag = FALSE;

			if (Logic_IsDirectMode(pAvatar) == FALSE && Logic_IsVehicleDriver(pAvatar) == FALSE)
			{
				RwUInt32 actorStateID = Logic_GetActorStateId(pAvatar);
				if (actorStateID != NTL_FSMSID_DASH && actorStateID != NTL_FSMSID_AIR_JUMP && actorStateID != NTL_FSMSID_AIR_ACCEL)
				{
					if (GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE) && GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_WORLD) && GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
					{
						m_sPickInfo.bTerrainExclusion = FALSE;
						m_sPickInfo.bCharacterExclusion = FALSE;
						RwBool bPick = GetSceneManager()->PickWorld(pData->iPosX, pData->iPosY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);
						if (bPick)
						{
							ENtlPLEntityType ePLType = m_sPickInfo.pPLEntity->GetClassType();

							if (ePLType == PLENTITY_WORLD || (ePLType == PLENTITY_OBJECT && m_sPickInfo.pPLEntity->GetSerialID() == INVALID_SERIAL_ID)) // the invalid checks if its something like a statue..
							{
								if (pAvatar->IsAirMode())
								{
									if (Logic_IsAirDashPossible())
									{
										SetAutoRun(FALSE);
										DeleteMoveMark();

										CNtlSLEventGenerator::AirDashMove(TRUE, &m_sPickInfo.vPickPos, NTL_MOVE_F);

										SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
										if (hTargetSerialId != INVALID_SERIAL_ID)
											CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);
									}
								}
								else if (Logic_IsPassiveDashUsePossible())
								{
									SetAutoRun(FALSE);
									DeleteMoveMark();

									/*CNtlSob::GetPosition((int)v16, (float *)&v36);
									if (!sub_518660((int)v16, &v36, (int)&v39))*/
									{
										CNtlSLEventGenerator::MouseDashMove(m_sPickInfo.vPickPos, MOUSE_DASH_LIMIT);
									}

									SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
									if (hTargetSerialId != INVALID_SERIAL_ID)
										CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);
								}
							}
						}
						else
						{
							if (Logic_IsAirMode())
							{
								if (Logic_IsAirDashPossible())
								{
									SetAutoRun(FALSE);
									DeleteMoveMark();

									CNtlSLEventGenerator::AirDashMove(TRUE, &m_sPickInfo.vPickPos, NTL_MOVE_F);

									SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
									if (hTargetSerialId != INVALID_SERIAL_ID)
										CNtlSLEventGenerator::SobAttackMarkRelease(hTargetSerialId);
								}
							}
						}
					}
				}
			}
		}

		if (m_bMouseAutoRun)
		{
			SetAutoRun(FALSE);
		//	CNtlSLEventGenerator::KeyboardMoveStop();
		}
	}

	if (m_bLButtonDownFlag == true || m_bRButtonDownFlag == true)
	{
		if (m_bHideMouse == false)
		{
			m_bHideMouse = true;
			GetCursorManager()->ShowMouseCursor(FALSE);
			GetCursorManager()->SaveCurrentPosition();
		}
	}
	
	return TRUE;
}


int	CAvatarController::MouseUpHandler(unsigned int uiMouseData)
{
	SMouseData *pData = (SMouseData *)uiMouseData;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if(pData->chBtnType == MOUSE_LBTN)
	{
		m_bLButtonDownFlag = false;
		MouseUpLBtnHandler(pData);
	}
	else if(pData->chBtnType == MOUSE_RBTN)
	{
		m_bRButtonDownFlag = false;
		MouseUpRBtnHandler(pData);
	}

	if (m_bMouseAutoRun)
	{
		SetAutoRun(FALSE);
		CNtlSLEventGenerator::KeyboardMoveStop();

		if (pAvatar)
		{
			// to do
			//v9 = *(_DWORD *)(*(_DWORD *)(v7 + 168) + 8);
			//if (v9)
			{
			//	if ((*(int(**)(void))(*(_DWORD *)v9 + 56))())
				{
					if (Logic_IsVehicleDriver(pAvatar) == FALSE)
					{
						SERIAL_HANDLE serial = pAvatar->GetTargetID();
						if (serial != INVALID_SERIAL_ID)
						{
							CNtlSob* pSobTar = GetNtlSobManager()->GetSobObject(serial);
							if (pSobTar)
							{
								// to do
							}
						}
					}
				}
			}
		}
	}

	// console debug module
    GetDumpCmdManager()->SetTargetSerial(Logic_GetAvatarTargetHandle());

	if (m_uiCurMoveFlags && pData->chBtnType == MOUSE_RBTN)
	{
		if (pAvatar->IsAirMode() == FALSE)
			ActionMapKeyboardMove(m_uiCurMoveFlags);
	}
	else
	{
		// by daneos: so when we move with both mouse buttons and release left button, there is camera rotate issue
	//	CalcCameraRotateToCharBack();
	}

	
	if (m_bLButtonDownFlag == false && m_bRButtonDownFlag == false)
	{
		if (m_bHideMouse == true)
		{			
			m_bHideMouse = false;
			GetCursorManager()->ShowMouseCursor(TRUE);
			GetCursorManager()->RestorePosition();
		}
	}

	return TRUE;
}

int	CAvatarController::KeyboardDownHandler(unsigned int uiKeyData)
{
	SKeyData* pData = (SKeyData*)uiKeyData;

	if(pData->uiChar & 0xFF)
	{
		if(m_bHTBRPStock)
		{
			RwUInt8 bySetRPStock = (RwUInt8)(pData->uiChar - 48);
			RwUInt32 uiRPStock = Logic_GetRpStock(GetNtlSLGlobal()->GetSobAvatar());
			if(bySetRPStock <= uiRPStock)
			{
				API_GetSLPacketGenerator()->SendHTBRPBallUseReq(bySetRPStock);
				m_bHTBRPStock = FALSE;
			}
		}
	}

	if(pData->uiChar == NTL_KEY_F3 )
	{
	}
	else if (pData->uiChar == NTL_KEY_F4)
	{
	}
	else if(pData->uiChar == NTL_KEY_F5 )
	{
		if( Logic_IsDevUser() )
			GetDboGlobal()->ToggleDebugDisplay();
	}
#ifndef NDEBUG
	else if(pData->uiChar == NTL_KEY_F8 )
	{
		if(Logic_IsDevUser())
		{
			static RwBool bShowProfilerGui = TRUE;
			CNtlPLEventGenerator::ShowProfilerGui(bShowProfilerGui);
			bShowProfilerGui = !bShowProfilerGui;
		}
	}
#endif
    else if(pData->uiChar == NTL_KEY_F11 ) 
    {
#ifdef _UICONFIG
        GetDboGlobal()->GetDBOUIConfig()->Reflash();
#endif        
		// peessi: 천하제일무도회 대진표 테스트용.
		//if( !CTBGui::m_pInstance->m_pTournamentGui )
		//{
		//	CTBGui::m_pInstance->m_pTournamentGui = NTL_NEW CTBTournamentGui( "BudokaiTournamentGui" );
		//	if( !CTBGui::m_pInstance->m_pTournamentGui->Create() )
		//	{
		//		CTBGui::m_pInstance->m_pTournamentGui->Destroy();
		//		NTL_DELETE( CTBGui::m_pInstance->m_pTournamentGui );
		//	}
		//	else
		//	{
		//		CNtlPLGuiManager::GetInstance()->AddGui( CTBGui::m_pInstance->m_pTournamentGui );
		//		GetDialogManager()->RegistDialog( DIALOG_BUDOKAI_TOURNAMENT, CTBGui::m_pInstance->m_pTournamentGui, &CTBTournamentGui::SwitchDialog );
		//		CTBGui::m_pInstance->m_pTournamentGui->Show( true );
		//	}
		//}
		//else
		//{
		//	if( CTBGui::m_pInstance->m_pTournamentGui )
		//	{	
		//		CTBGui::m_pInstance->m_pTournamentGui->Show( false );
		//		CNtlPLGuiManager::GetInstance()->RemoveGui( CTBGui::m_pInstance->m_pTournamentGui );
		//		GetDialogManager()->UnRegistDialog( DIALOG_BUDOKAI_TOURNAMENT );
		//		CTBGui::m_pInstance->m_pTournamentGui->Destroy();
		//		NTL_DELETE( CTBGui::m_pInstance->m_pTournamentGui );
		//	}
		//}
    } 
	else if( pData->uiChar == NTL_KEY_NUMLOCK )
	{		
		if(Logic_IsDevUser())
		{
			static RwBool bDrawGUI = FALSE;
			CNtlPLGuiManager::GetInstance()->RenderEnable(bDrawGUI);
			bDrawGUI = !bDrawGUI;
		}
	}
	else if( pData->uiChar == NTL_KEY_F9 )
	{
	}
	else if( pData->uiChar == NTL_KEY_F10 )
	{		

	}
	else if( pData->uiChar == NTL_KEY_F11 )
	{
	
	}
	else if( pData->uiChar == NTL_KEY_F12 )
	{

		
	}
	
	return TRUE;
}

int CAvatarController::KeyboardUpHandler(unsigned int uiKeyData)
{
	return TRUE;
}


void CAvatarController::CreateMoveMark(const RwV3d *pPos)
{
	if(m_pMoveMark == NULL)
	{
        m_pMoveMark = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_MARK_COMMON_MOVE);
	}

	m_pMoveMark->SetPosition(pPos);

	// 사운드 
	RwInt32 nSoundIndex = NtlRandomNumber(0, 3);
	Logic_PlayGUISound((char*)(SOUND_SL_GROUND_PICKING[nSoundIndex]));
}

void CAvatarController::DeleteMoveMark(void)
{
	if(m_pMoveMark)
	{
		GetSceneManager()->DeleteEntity(m_pMoveMark);
		m_pMoveMark = NULL;
		m_fMoveMarkTime = 0.0f;
	}
}

void CAvatarController::UpdateMoveMark(RwReal fElapsed)
{
	// move mark를 잠깐 띄웠다가 없앤다.
	if(m_pMoveMark)
	{
		m_fMoveMarkTime += fElapsed;
		if(m_fMoveMarkTime > 3.0f)
			DeleteMoveMark();
	}
}

void CAvatarController::UpdateMouse(RwReal fElapsed)
{
	POINT pos;
	GetCursorPos(&pos);

	if (m_bRButtonDownFlag)
	{
		MouseMoveRBtnHandler(pos.x, pos.y);
	}
	else if (m_bLButtonDownFlag)
	{
		MouseMoveLBtnHandler(pos.x, pos.y);

		sm_cameraMoveCount += 1;
	}
}

void CAvatarController::UpdateChangeHeading(RwReal fElapsed)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	DBO_ASSERT(pSobAvatar, "CAvatarController::UpdateChangeHeading: Not exist avatar instance");

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHANGE_HEADING))
		return;

	// change heading
	if(m_bChangeHeadingFlag)
	{
		m_fChangeHeadingDeltaTime += fElapsed;
		m_fChangeHeadingUpdateTime += fElapsed;

		if(m_fChangeHeadingDeltaTime >= NTL_HEADING_DELTA_TIME)
		{
			if(fabsf(m_fChangeHeadingDeltaAngleY) > NTL_HEADING_DELTA_ANGLE || fabsf(m_fChangeHeadingDeltaAngleX) > NTL_HEADING_DELTA_ANGLE)
			{
				RwV3d vDir = pSobAvatar->GetDirection();
				API_GetSLPacketGenerator()->SendChangeHeading(vDir);

				m_fChangeHeadingDeltaAngleY = 0.0f;
				m_fChangeHeadingDeltaAngleX = 0.0f;
				m_fChangeHeadingUpdateTime = 0.0f;
			}

			m_fChangeHeadingDeltaTime = 0.0f;
		}

		if(m_fChangeHeadingUpdateTime >= NTL_HEADING_UPDATE_TIME)
		{
			RwV3d vDir = pSobAvatar->GetDirection();
			API_GetSLPacketGenerator()->SendChangeHeading(vDir);

			m_fChangeHeadingDeltaAngleY = 0.0f;
			m_fChangeHeadingDeltaAngleX = 0.0f;
			m_fChangeHeadingUpdateTime = 0.0f;
		}
	}

	if(m_bAutoRun && m_bAutoRunKeyboardChangeHeading )
	{
		m_fChangeHeadingUpdateTime += fElapsed;

		if(m_fChangeHeadingUpdateTime >= NTL_HEADING_DELTA_TIME)
		{
			if ( !Logic_IsVehicleDriver(pSobAvatar) )
			{
				RwV3d vDir = pSobAvatar->GetDirection();
				API_GetSLPacketGenerator()->SendChangeHeading(vDir);
			}

			m_fChangeHeadingUpdateTime = 0.0f;
		}
	}
}

void CAvatarController::UpdateFlightAngle(float fElapsed)
{
	if (m_bChangeAngleX)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (pSobAvatar && pSobAvatar->IsAirMode())
		{
			if (m_bChangeAngleXUp) // fly up
			{
				pSobAvatar->SetDeltaAngleX(-0.8);
			}
			else // fly down
			{
				pSobAvatar->SetDeltaAngleX(0.8);
			}
		}
	}
}

void CAvatarController::UpdatePickFocus(RwReal fElapsed)
{
	m_fPickTime += fElapsed;	

	if(GetNtlGuiManager()->IsGuiFocus() )
	{
		m_sPickInfo_for_Cursor.bWorldPick	= false;
		m_sPickInfo_for_Cursor.pSobObj		= NULL;

		GetCursorManager()->Update(fElapsed, &m_sPickInfo_for_Cursor);
	}
	else
	{
		if(m_fPickTime < PICK_FOCUS_TIME)
			return;

		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_PICK_OBJECT))
			return;

		m_sPickInfo.bTerrainExclusion	= false;
		m_sPickInfo.bCharacterExclusion = false;

		CNtlSob* pSobObj = NULL;
		RwBool bWorldPick = GetSceneManager()->PickWorld(CMouse::m_nX, CMouse::m_nY, m_sPickInfo, PICK_TERRAIN_LIMIT, PICK_OBJECT_LIMIT);

		if( bWorldPick )
		{
			RwUInt32 uiSerialId = m_sPickInfo.pPLEntity->GetSerialID();

			pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
			if( pSobObj )
			{
				if (pSobObj->GetClassID() == SLCLASS_MONSTER)
				{
					RwUInt32 actorStateId = Logic_GetActorStateId(reinterpret_cast<CNtlSobActor*>(pSobObj));
					if (actorStateId == NTL_FSMSID_FAINTING || actorStateId == NTL_FSMSID_DIE)
					{
						m_fPickTime = 0.f;
						m_sPickInfo_for_Cursor.bWorldPick = false;
						m_sPickInfo_for_Cursor.pSobObj = NULL;

						GetCursorManager()->Update(fElapsed, &m_sPickInfo_for_Cursor);
						return;
					}
				}

				if (m_hFocusSerialId != INVALID_SERIAL_ID && m_hFocusSerialId != uiSerialId)
					CNtlSLEventGenerator::SobLostFocus(m_hFocusSerialId);

				if (!(pSobObj->GetFlags() & SLFLAG_NOT_HIGHLIGHTED))
				{
					if (m_hFocusSerialId != uiSerialId)
					{
						m_hFocusSerialId = uiSerialId;
						CNtlSLEventGenerator::SobGotFocus(m_hFocusSerialId);
					}
				}
			}
			else
			{
				if(m_hFocusSerialId != INVALID_SERIAL_ID)
				{
					CNtlSLEventGenerator::SobLostFocus(m_hFocusSerialId);
                    m_hFocusSerialId = INVALID_SERIAL_ID;
				}
			}
		}
		else
		{
			if(m_hFocusSerialId != INVALID_SERIAL_ID)
			{
				CNtlSLEventGenerator::SobLostFocus(m_hFocusSerialId);
                m_hFocusSerialId = INVALID_SERIAL_ID;
			}
		}

		m_sPickInfo_for_Cursor.bWorldPick	= bWorldPick;
		m_sPickInfo_for_Cursor.pSobObj		= pSobObj;

		GetCursorManager()->Update(fElapsed, &m_sPickInfo_for_Cursor);
		m_fPickTime = 0.f;
	}		
}

int CAvatarController::ActionMapKeyboardMove(unsigned int uiMoveFlags)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	DBO_ASSERT(pSobAvatar, "CAvatarController::ActionMapKeyboardMove: Not exist avatar instance");

	if (Logic_IsDirectMode(pSobAvatar))
		return TRUE;

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MOVE ) )
		return FALSE;

	RwUInt32 actorStateId = Logic_GetActorStateId(pSobAvatar);
	if (actorStateId == NTL_FSMSID_FAINTING || actorStateId == NTL_FSMSID_DIE || actorStateId == NTL_FSMSID_TELEPORT 
		|| actorStateId == NTL_FSMSID_SLIDING || actorStateId == NTL_FSMSID_SKILL_ACTION || actorStateId == NTL_FSMSID_DIRECTION)
		return TRUE;

    // Make sure the free camera is enabled.
    if(GetNtlGameCameraManager()->IsActiveFreeCamera())
    {
        GetNtlGameCameraManager()->FreeCameraMove(uiMoveFlags);
        return TRUE;
    }

	if(m_bAutoRun)
	{
		if (m_bMouseAutoRun)
			return FALSE;

		if (!(uiMoveFlags == NTL_MOVE_NONE || uiMoveFlags == NTL_MOVE_TURN_L || uiMoveFlags == NTL_MOVE_TURN_R))
		{
			SetAutoRun(FALSE);
			m_bAutoRunKeyboardChangeHeading = FALSE;
		}
		else
		{
			if (uiMoveFlags == NTL_MOVE_TURN_L || uiMoveFlags == NTL_MOVE_TURN_R)
			{
				if (m_uiCurMoveFlags != uiMoveFlags) //we dont want to keep sending "change heading" every time but only when it actually changes
				{
					if (!Logic_IsVehicleDriver(pSobAvatar))
					{
						RwV3d vDir = pSobAvatar->GetDirection();
						API_GetSLPacketGenerator()->SendChangeHeading(vDir);

						m_bAutoRunKeyboardChangeHeading = TRUE;
						m_fChangeHeadingUpdateTime = 0.0f;
					}
				}
			}
			else
			{
				m_bAutoRunKeyboardChangeHeading = FALSE;
			}

			m_uiCurMoveFlags = (RwUInt8)uiMoveFlags;
			return TRUE;
		}
	}

	RwUInt32 uiOldMoveFlags = m_uiCurMoveFlags;


	/*
		info:	if we do return TRUE/FALSE here while actorStateId == NTL_FSMSID_JUMP
				then player will keep moving after jump finished.
				So dont do this here.
	*/

	if (pSobAvatar->IsAirMode() && actorStateId == NTL_FSMSID_AIR_ACCEL) // change air dash/accel direction
	{
		if (uiMoveFlags == NTL_MOVE_NONE || uiMoveFlags == NTL_MOVE_TURN_L || uiMoveFlags == NTL_MOVE_TURN_R)
		{
			if (uiMoveFlags == NTL_MOVE_TURN_L)
				uiMoveFlags = NTL_MOVE_F_TURN_L;
			else if (uiMoveFlags == NTL_MOVE_TURN_R)
				uiMoveFlags = NTL_MOVE_F_TURN_R;
			else 
				uiMoveFlags = NTL_MOVE_F;

			m_uiCurMoveFlags = (RwUInt8)uiMoveFlags;

			CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(pSobAvatar->GetBehaviorData());
			SMoveStuff* pMoveStuff = pBeData->GetMoveStuff();

			if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
			{
				CNtlSLEventGenerator::AirDashMove(FALSE, NULL, uiMoveFlags, FALSE);
			}
			else
			{
				CNtlSLEventGenerator::AirAccelMove(uiMoveFlags, FALSE);
			}

			return TRUE;
		}
	}

	m_uiCurMoveFlags = (RwUInt8)uiMoveFlags;
	bool bRotateCamera = m_bRButtonMoveFlag == FALSE ? true:false;

	if (uiMoveFlags == NTL_MOVE_NONE)
	{
		CNtlSLEventGenerator::KeyboardMoveStop();
	}
	else
	{
		RwBool bAvatarAngleChange = FALSE;
		RwReal fAngleY = 0.0f;

		if (Logic_IsVehicleDriver(pSobAvatar))
		{
			if (uiMoveFlags == NTL_MOVE_L)
			{
				uiMoveFlags = NTL_MOVE_TURN_L;
				bRotateCamera = false;
			}
			else if (uiMoveFlags == NTL_MOVE_R)
			{
				uiMoveFlags = NTL_MOVE_TURN_R;
				bRotateCamera = false;
			}

			if (m_bRButtonMoveFlag)
				bRotateCamera = false;
		}
		else if (m_bRButtonMoveFlag)
		{
			bAvatarAngleChange = TRUE;
			fAngleY = CalcAvatarStraightRotate(uiMoveFlags);
			uiMoveFlags = ConvertStraightMoveFlags((RwUInt8)uiMoveFlags);

			m_uiCurMoveFlags = (RwUInt8)uiMoveFlags;
		}

		CNtlSLEventGenerator::KeyboardMove((RwUInt8)uiMoveFlags, bAvatarAngleChange, fAngleY); // if this is not here, then char wont move if he didnt move before

		// this is important. In case we move forward/back + left/right angle
		if (m_bRButtonMoveFlag)
		{
			if (uiOldMoveFlags == uiMoveFlags)
			{
				if (!Logic_IsVehicleDriver(pSobAvatar))
				{
					RwV3d vDir = pSobAvatar->GetDirection();
					API_GetSLPacketGenerator()->SendChangeHeading(vDir);
				}
			}
		}

		DeleteMoveMark();
	}
	
	// console debug module
	GetDumpCmdManager()->SetTargetSerial(Logic_GetAvatarTargetHandle());

	// camera auto rotate.
	if(bRotateCamera)
	{
		if(uiOldMoveFlags != uiMoveFlags)
			CalcCameraRotateToCharBack();
	}
	
	return 1;
}

int CAvatarController::ActionAutoRun()
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pSobAvatar == NULL)
		return FALSE;

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

	CNtlWorldConceptController* pController = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_RANK_BATTLE);
	if (pController)
	{
		/*if (*(_DWORD *)(v7 + 32))
			return 1;*/
	}

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_AUTORUN ) )
		return FALSE;

	//if(!pSobAvatar->CanMove())
	//	return TRUE;

	if(m_uiCurMoveFlags != NTL_MOVE_NONE)
		return TRUE;

    if(Logic_IsDirectMode(pSobAvatar))
        return TRUE;

	RwUInt32 actorStateId = Logic_GetActorStateId(pSobAvatar);
	if (actorStateId == NTL_FSMSID_FAINTING || actorStateId == NTL_FSMSID_DIE || actorStateId == NTL_FSMSID_TELEPORT || actorStateId == NTL_FSMSID_SLIDING || actorStateId == NTL_FSMSID_SKILL_ACTION || actorStateId == NTL_FSMSID_DIRECTION)
		return TRUE;

	RwReal fYaw = GetNtlGameCameraManager()->GetYaw();
	
	pSobAvatar->SetAngleY(fYaw);

	CNtlSLEventGenerator::KeyboardMove(NTL_MOVE_F, FALSE, 0.0f);

	SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		Logic_SobTarget(hTargetSerialId, INVALID_BYTE);
		
	DeleteMoveMark();
	
	// console debug module
	GetDumpCmdManager()->SetTargetSerial(hTargetSerialId);

	SetAutoRun(TRUE);

    return 1;
}

int CAvatarController::ActionMapKeyboardDashMove(unsigned int uiMoveFlags)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
        return TRUE;

	CNtlSobAvatar *pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	DBO_ASSERT(pAvatar, "CAvatarController::ActionMapKeyboardDashMove: Not exist avatar instance");

	RwUInt32 actorStateID = Logic_GetActorStateId(pAvatar);
	if (actorStateID == NTL_FSMSID_DASH || actorStateID == NTL_FSMSID_AIR_JUMP || actorStateID == NTL_FSMSID_AIR_ACCEL)
		return TRUE;

	if (pAvatar->IsAirMode())
	{
		if (uiMoveFlags == NTL_MOVE_F)
		{
			if (Logic_IsAirDashPossible())
			{
				CNtlSLEventGenerator::AirDashMove(FALSE, NULL, uiMoveFlags);
				SetAutoRun(FALSE);
			}
		}
	}
	else
	{
		if (!Logic_IsPassiveDashUsePossible())
			return TRUE;

		if(uiMoveFlags == NTL_MOVE_F || uiMoveFlags == NTL_MOVE_B || uiMoveFlags == NTL_MOVE_L || uiMoveFlags == NTL_MOVE_R)
		{
			CNtlSLEventGenerator::KeyboardDashMove(uiMoveFlags);
		}

		SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
		if (hTargetSerialId != INVALID_SERIAL_ID)
			Logic_SobTarget(hTargetSerialId, INVALID_BYTE);

		SetAutoRun(FALSE);
	}

	return TRUE;
}

int CAvatarController::ActionAvatarSelect(void)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return TRUE;

    if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
        return TRUE;

    if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
        return FALSE;

    if(Logic_GetActorStateFlags(pAvatar) & NTL_FSMSF_NOT_INPUT)
        return FALSE;

	Logic_SobTarget(pAvatar->GetSerialID(), INVALID_BYTE);

	return 1;
}

int CAvatarController::ActionJump(unsigned int uiMoveFlags)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	DBO_ASSERT(pAvatar, "CAvatarController::ActionJump: Not exist avatar instance");

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

	//if (GetDialogManager()->IsOpenDialog(103)) // to do
	//{
	//	return FALSE;
//	}
	
    if(Logic_IsTransformGreatNamek(pAvatar) || Logic_IsTransformSpinAttack(pAvatar) || Logic_IsTransformRollingAttack(pAvatar))
        return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MOVE ) )
		return FALSE;

	BYTE byStateID = Logic_GetActorStateId(pAvatar);

	//if (byStateID == NTL_FSMSID_FIGHTING_POSE)
	//{
	//	return TRUE;
	//}

	if (Logic_IsVehicleDriver(pAvatar))
	{
		if (uiMoveFlags != NTL_MOVE_UNKNOWN) // if we release space
		{
			API_GetSLPacketGenerator()->SendVehicleStuntNfy();

			CNtlSLEventGenerator::SobVehicleStunt(pAvatar->GetSerialID());
		}

		return TRUE;
	}

	m_bAutoRunKeyboardChangeHeading = FALSE;

	if (uiMoveFlags == NTL_MOVE_UNKNOWN) // check if we release the space
	{
		m_bChangeAngleX = false;

		if (pAvatar->IsAirMode())
		{
			if (m_uiCurMoveFlags == NTL_MOVE_HEIGHT_UP)
			{
				m_uiCurMoveFlags = NTL_MOVE_NONE;

				CNtlSLEventGenerator::KeyboardMoveStop(); // stop air_jump / moving up
			}
		}
	}
	else
	{
		if (byStateID == NTL_FSMSID_JUMP) // check if we press space while jump
		{
			if (!Logic_IsPassiveAirPossible())
			{
				return TRUE;
			}

			switch (m_uiCurMoveFlags)
			{
				case NTL_MOVE_F:
				case NTL_MOVE_F_TURN_L_JUMP:
				case NTL_MOVE_F_TURN_R_JUMP:
					m_uiCurMoveFlags = NTL_MOVE_HEIGHT_UP_F; break;

				case NTL_MOVE_B:
				case NTL_MOVE_B_TURN_L_JUMP:
				case NTL_MOVE_B_TURN_R_JUMP:
					m_uiCurMoveFlags = NTL_MOVE_HEIGHT_UP_B; break;

				case NTL_MOVE_L:
				case NTL_MOVE_L_TURN_L_JUMP:
				case NTL_MOVE_L_TURN_R_JUMP:
					m_uiCurMoveFlags = NTL_MOVE_HEIGHT_UP_L; break;

				case NTL_MOVE_R:
				case NTL_MOVE_R_TURN_L_JUMP:
				case NTL_MOVE_R_TURN_R_JUMP:
					m_uiCurMoveFlags = NTL_MOVE_HEIGHT_UP_R; break;

				default: m_uiCurMoveFlags = NTL_MOVE_HEIGHT_UP; break;
			}

			CNtlSLEventGenerator::ActionMapAirJump(m_uiCurMoveFlags);
		}
		else if (pAvatar->IsAirMode()) // if we press space while flying.. then we fly up
		{
			m_bChangeAngleXUp = true;

			if (byStateID == NTL_FSMSID_DASH || byStateID == NTL_FSMSID_AIR_ACCEL) // if we press space during air-dash or air-accel, then we change direction
			{
				m_bChangeAngleX = true;
			}
			else
			{
				if (m_uiCurMoveFlags == NTL_MOVE_NONE)
				{
					uiMoveFlags = NTL_MOVE_HEIGHT_UP;

					m_uiCurMoveFlags = uiMoveFlags;

					CNtlSLEventGenerator::KeyboardMove(uiMoveFlags, FALSE, 0.f);
				}
				else
				{
					m_bChangeAngleX = true;
				}
			}
		}
		else
		{
			if (m_bRButtonMoveFlag)
			{
				uiMoveFlags = ConvertStraightMoveFlags((RwUInt8)uiMoveFlags);
			}

			CNtlSLEventGenerator::ActionMapJump(uiMoveFlags);
		}
	}

	return TRUE;
}

int CAvatarController::ActionCharging(unsigned int uiCharging)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return TRUE;

	if(uiCharging)
	{
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHARGING))
			return TRUE;
	}

	if(!Logic_IsPassiveChargingUsePossible())
	{
		return TRUE;
	}

	if (uiCharging)
	{
		CNtlSLEventGenerator::ActionMapCharging(TRUE);
	}
	else
	{
		CNtlSLEventGenerator::ActionMapCharging(FALSE);
	}

	SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
	if (hTargetSerialId != INVALID_SERIAL_ID)
		Logic_SobTarget(hTargetSerialId, INVALID_BYTE);

	return TRUE;
}

int CAvatarController::ActionBlocking(unsigned int uiBlocking)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return TRUE;

	if(uiBlocking)
	{
		if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_CHARGING))
			return TRUE;

		if (!Logic_IsPassiveBlockingUsePossible())
		{
			return TRUE;
		}

		if(!Logic_IsPassiveBlockingUsePossibleCoolTime()) // Check out cool-time.
		{
			GetAlarmManager()->AlarmMessage( "DST_BLOCK_MODE_COOLTIME" );
			return 1;
		}
	
		CNtlSLEventGenerator::ActionMapBlocking(TRUE);
	}
	else
	{
		CNtlSLEventGenerator::ActionMapBlocking(FALSE);
	}

	SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
	if(hTargetSerialId != INVALID_SERIAL_ID)
		Logic_SobTarget(hTargetSerialId, INVALID_BYTE);

	return 1;
}

int CAvatarController::ActionSitAndStand(unsigned int bKeyDown)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	NTL_ASSERT(pSobAvatar, "CAvatarController::ActionSitAndStand: not exist avatar instance");

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
		return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MOVE ) )
		return FALSE;

	if (Logic_IsVehicleDriver(pSobAvatar))
		return TRUE;

	BYTE byStateID = Logic_GetActorStateId(pSobAvatar);

	if (pSobAvatar->IsAirMode())
	{
		if (bKeyDown)
		{
			if (byStateID == NTL_FSMSID_DASH || byStateID == NTL_FSMSID_AIR_ACCEL) // if we press sit-down during air-dash or air-accel, then we change direction
			{
				m_bChangeAngleX = true;
			}
			else
			{
				if (m_uiCurMoveFlags == NTL_MOVE_NONE) // only fly down if we are not moving
				{
					m_uiCurMoveFlags = NTL_MOVE_HEIGHT_DOWN;

					CNtlSLEventGenerator::KeyboardMove(NTL_MOVE_HEIGHT_DOWN, FALSE, 0.f);
				}
				else
				{
					m_bChangeAngleX = true;
				}
			}
		}
		else
		{
			if (m_uiCurMoveFlags == NTL_MOVE_HEIGHT_DOWN)
			{
				m_uiCurMoveFlags = NTL_MOVE_NONE;

				if (byStateID != NTL_FSMSID_FALLING)
				{
					CNtlSLEventGenerator::KeyboardMoveStop();
				}
			}
		}
	}
	else
	{
		if (bKeyDown)
		{
			CNtlSLEventGenerator::ActionMapSitAndStand();
		}
		else
		{
			m_uiCurMoveFlags = NTL_MOVE_NONE;
		}
	}

	if (bKeyDown)
	{
		m_bChangeAngleXUp = false;

		SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
		if (hTargetSerialId != INVALID_SERIAL_ID)
			Logic_SobTarget(hTargetSerialId, INVALID_BYTE);
	}
	else
	{
		m_bChangeAngleX = false;
	}

	return TRUE;
}

int CAvatarController::ActionLooting(void)
{
	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_LOOTING))
		return TRUE;

	if( m_fKeyLootingTime >= KEY_LOOTING_TIME )
	{
        RwV3d vPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
		CNtlSob* pSobObject = GetNtlSobManager()->GetSobNearWorldItem(vPos);

		Logic_WorldItemLooting( pSobObject );
		
		m_fKeyLootingTime = 0.0f;
	}

	return 1;
}

int CAvatarController::ActionAutoTarget(void)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_TARGETTING))
		return TRUE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
		return FALSE;

    if(Logic_GetActorStateFlags(pAvatar) & NTL_FSMSF_NOT_INPUT)
        return FALSE;

	CNtlSobActor *pSobActor = Logic_FindNearEnemyFromAvatarActor();
	if(pSobActor)
	{
		Logic_SobTarget(pSobActor->GetSerialID(), INVALID_BYTE);
	}

	return 1;
}

int CAvatarController::ActionAutoAttack(void)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return TRUE;
	
	if (!GetNtlWorldConcept()->IsEnableAction(E_ACTION_ATTACK))
		return TRUE;

	if (Logic_IsDirectMode(pAvatar))
		return TRUE;

	SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
	if (hTargetSerialId != INVALID_SERIAL_ID)
	{
		CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(hTargetSerialId);
		if (pTarSobObj)
		{
			RwBool bEnemy = Logic_IsEnemyTargetFromAvatarActor(hTargetSerialId);

			if (bEnemy)
			{
				if (pTarSobObj->CanHurted() && Logic_IsVehicleDriver(pAvatar) == FALSE)
				{
					if (Logic_isEnemyTargetOcclusion(hTargetSerialId))
					{
						GetAlarmManager()->AlarmMessage("DST_SKILL_OCCLUSION_CHECK");
						return TRUE;
					}
					
					CNtlSLEventGenerator::SobAttackSelect(hTargetSerialId);
				}
			}
			else
			{
				API_GetSLPacketGenerator()->SendCrossFireReq(hTargetSerialId);
			}
		}
	}

	return TRUE;
}

int CAvatarController::ActionAutoFollow(void)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return TRUE;

	// auto follow
	SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
	if(hTargetSerialId == INVALID_SERIAL_ID)
	{
		return TRUE;
	}

	if(hTargetSerialId == pAvatar->GetSerialID())
	{
		return TRUE;
	}

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hTargetSerialId);

	if(pSobObj == NULL)
	{
		return TRUE;
	}

	if( !(pSobObj->GetFlags() & SLFLAG_CAN_AUTO_FOLLOW_TARGET) )
	{
		return TRUE;
	}

	if (Logic_IsAirMode())
	{
		GetAlarmManager()->AlarmMessage("GAME_COMMON_CANT_DO_THAT_IN_AIR_MODE_STATE");
		return TRUE;
	}

	if (GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_RANK_BATTLE))
		return TRUE;

	if (GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_T_BUDOKAI))
		return TRUE;

	if (GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DOJO_SCRAMBLE))
		return TRUE;

	if (GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DOJO_DUEL))
		return TRUE;

	CNtlSLEventGenerator::ActionMapAutoFollow(hTargetSerialId);

	return TRUE;
}

int CAvatarController::ActionPartySelect(unsigned int uiIdx)
{
	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
		return FALSE;

	if(GetNtlDTCinematicManager()->IsRunning())
		return TRUE;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar)
	{
		CNtlParty *pParty = pSobAvatar->GetParty();

		// 아바타 또한 파티 구조체에 파티 가입 순서에 따라 포함된다
		if( pParty->GetAvatarIndex() <= (RwInt32)uiIdx )
			++uiIdx;

		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyIndex(uiIdx) );
		if(pMember)
		{
			SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
			if(pMember->hSerial != hTargetSerialId)
			{
				CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pMember->hSerial);
				if(pSobObj)
				{
					Logic_SobTarget(pMember->hSerial, INVALID_BYTE);
				}
			}
		}
	}

	return 1;
}

int CAvatarController::ActionScouterUse( void ) 
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pSobAvatar)
		return FALSE;

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_SCOUTER_MODE ) )
		return FALSE;

	bool bStatusOn = false;

	if (pSobAvatar->GetOtherParam()->IsScouterOn() == false)
	{
		bStatusOn = true;
	}
	else
	{
		bStatusOn = false;
	}

	// send event to avatar status
	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_STATUS, true, bStatusOn);


    return TRUE;
}


void CAvatarController::SetAutoRun(RwBool bAutoRun)
{
	if(m_bAutoRun != bAutoRun)
	{
		Logic_SetAutoRun(bAutoRun);
	}

	if (bAutoRun == false && m_bMouseAutoRun == true)
	{
		m_bMouseAutoRun = false;
	}

	m_bAutoRun = bAutoRun;
}

int CAvatarController::ActionPartyShareTargetMarking( unsigned int uiSlot ) 
{
    if(uiSlot < 0 || uiSlot >= NTL_MAX_SHARETARGET_COUNT)
        return FALSE;

	SERIAL_HANDLE hTargetId = Logic_GetAvatarTargetHandle();

    if(hTargetId == INVALID_SERIAL_ID)
        return FALSE;

	GetDboGlobal()->GetGamePacketGenerator()->SendPartyShareTargetReq( (RwUInt8)uiSlot, hTargetId);

    return TRUE;
}

int CAvatarController::ActionPartyShareTargetSelect( unsigned int uiSlot ) 
{
    if(uiSlot < 0 || uiSlot >= NTL_MAX_SHARETARGET_COUNT)
        return FALSE;

    RwUInt32 uiSerialID = Logic_GetShareTargetSerial(uiSlot);
    if(uiSerialID == INVALID_SERIAL_ID)
        return FALSE;
    
    // The PickPLObject () method is used to select the target.
    SWorldPickInfo info;
    PickPLObject(info, 1, 0, 0);

    return TRUE;
}

/**
 * 순차적으로 파티멤버를 타겟팅한다. 
 */
int CAvatarController::ActionPartyAutoSelect() 
{
    if(!GetNtlWorldConcept()->IsEnableAction(E_ACTION_MOVE))
        return TRUE;

    if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_TARGETING ) )
        return FALSE;

    if(Logic_GetActorStateFlags(GetNtlSLGlobal()->GetSobAvatar()) & NTL_FSMSF_NOT_INPUT)
        return FALSE;

    static RwInt32 nSelectIndex = 0;
    CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
    if(!pParty || pParty->GetMemberCount() == 0)
        return FALSE;

    nSelectIndex = nSelectIndex >= pParty->GetMemberCount() ? 0 : nSelectIndex;    
    SERIAL_HANDLE hNewTargetID = pParty->GetMemberbyIndex(nSelectIndex)->uiKey;         
    SERIAL_HANDLE hTargetSerialId = Logic_GetAvatarTargetHandle();
    if(hNewTargetID != hTargetSerialId)
    {
        if(GetNtlSobManager()->GetSobObject(hNewTargetID))
        {
			Logic_SobTarget(hNewTargetID, INVALID_BYTE);
        }
        else
        {
            if(GetDboGlobal()->GetCommunityTarget() != INVALID_SERIAL_ID)
                CNtlSLEventGenerator::CommuTargetRelease(GetDboGlobal()->GetCommunityTarget());

            sPartyMember* pMember = (sPartyMember*)pParty->GetMemberbyIndex(nSelectIndex);
            CNtlSLEventGenerator::CommuTargetSelect(NULL, hNewTargetID, pMember->wszMemberName, COMMU_TARGET_PARTY);
        }

        ++nSelectIndex;
    }

    return TRUE;
}
