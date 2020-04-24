#include "precomp_dboclient.h"
#include "VehicleGui.h"

// core
#include "NtlDebug.h"

// shared
#include "VehicleTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobItemAttr.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"

// cleint
#include "DialogManager.h"
#include "InfoWndManager.h"
#include "DboGlobal.h"
#include "IconMoveManager.h"
#include "AlarmManager.h"


//#define dENGINE_WORKING_FLASH						"VehicleEffect.swf" // disabled by daneos


CVehicleGui::CVehicleGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bFocus(FALSE)
,m_bSelected(FALSE)
,m_bLeftMouseDown(FALSE)
,m_bRightMouseDown(FALSE)
{	
}

CVehicleGui::~CVehicleGui()
{	

}

RwBool CVehicleGui::Create()
{
	NTL_FUNCTION( "CVehicleGui::Create" );

	if(!CNtlPLGui::Create("gui\\Vehicle.rsr", "gui\\Vehicle.srf", "gui\\Vehicle.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDEICON);


	m_pExitButton		= (gui::CButton*)GetComponent("btnStart");
	m_pStopButton		= (gui::CButton*)GetComponent("btnStop");
	m_pStartButton		= (gui::CButton*)GetComponent("btnPower");

	// hide button because we (daneos) disabled the start/stop engine because its useless.
	m_pStopButton->Show(false);
	m_pStartButton->Show(false);

	m_pFuelGauge		= (gui::CProgressBar*)GetComponent("gaugeFuel");

	// Fuel slot
	m_tFuel.FuelSlot.Create(m_pThis, DIALOG_VEHICLE, REGULAR_SLOT_ITEM_SOB, SDS_NONE);
	m_tFuel.FuelSlot.SetPosition_fromParent(43, 6);

	m_pflashEngineWorking = (gui::CFlash*)GetComponent("flaEngineWorking");
	m_pflashEngineWorking->Show(false);
	//m_pflashEngineWorking->AutoRewind(TRUE);

	m_pExitButton	->SetToolTip( GetDisplayStringManager()->GetString("DST_VEHICLE_GET_OFF") );


	// Slot focus effect
	m_srfFocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_srfFocusEffect.SetPositionfromParent(41, 4);

	// Slot Select Effect
	m_srfSelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_srfSelectEffect.SetPositionfromParent(41, 4);	

	// Fuel Gauge Background
	m_srfFuelGaugeBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Vehicle.srf", "srfFuelGaugeBack" ) );
	m_srfFuelGaugeBack.SetPositionfromParent(23, 47);	
	
	GetNtlGuiManager()->AddUpdateFunc( this );	


	// default value
	/*m_pStopButton	->Show(false);*/
	m_pFuelGauge	->Show(false);

	m_tVehicle.iFuelGaugeTooltipUpdatedLastTime		= 0;
	m_tVehicle.fFuelGaugeElapsed					= 0.f;
	m_tVehicle.fFuelGaugeRemainTime					= 0.f;
	m_tVehicle.hVehicleItem							= INVALID_SERIAL_ID;
	m_tVehicle.pVEHICLE_TBLDAT						= NULL;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar )
	{
		CNtlOtherParam*	pOtherParam	= pAvatar->GetOtherParam();
		SERIAL_HANDLE	hFuelItem	= pOtherParam->GetRegisterFuelItem();
		CNtlSobItem*	pSobItem	= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hFuelItem));
		
		BYTE byPlace;
		BYTE byPos;


		if(pSobItem && Logic_GetItemContainerPosFromHandle(hFuelItem, byPlace, byPos))
		{
			API_GetSLPacketGenerator()->SendVehicle_FuelInsertReq(byPlace, byPos);
			//Fuel_Register(hFuelItem); // We use this, when we receive result from server
		}
		else
		{
			pOtherParam->SetRegisterFuelItem(INVALID_SERIAL_ID);
			pOtherParam->SetRegisterFuelItemTableIndex(INVALID_TBLIDX);
		}
	}
	else
	{
		DBO_FAIL("Not exist avatar instance");
	}


	// sig
	m_slotExitButton	= m_pExitButton->SigClicked().Connect(this, &CVehicleGui::OnClicked_ExitButton);	
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect(this, &CVehicleGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect(this, &CVehicleGui::OnMouseUp);
	m_slotMove			= m_pThis->SigMove().Connect(this, &CVehicleGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect(this, &CVehicleGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect(this, &CVehicleGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect(this, &CVehicleGui::OnPaint );


	LinkMsg(g_EventSobVehicleEngine);
	LinkMsg(g_EventSobInfoUpdate);	
	LinkMsg(g_EventIconMoveClick);


	Show(false);

	NTL_RETURN(TRUE);
}

VOID CVehicleGui::Destroy()
{
	NTL_FUNCTION("CVehicleGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	
	Fuel_Unegister(TRUE, FALSE);
	m_tFuel.FuelSlot.Destroy();
	
	Vehicle_UnlockVehichleItem();


	UnLinkMsg(g_EventSobVehicleEngine);
	UnLinkMsg(g_EventSobInfoUpdate);	
	UnLinkMsg(g_EventIconMoveClick);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CVehicleGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	//m_pflashEngineWorking->Update( fElapsed );

	if( m_tFuel.FuelSlot.GetSerial() == INVALID_SERIAL_ID )
		return;

	if( false == m_pFuelGauge->IsVisible() )
		return;


	m_tVehicle.fFuelGaugeElapsed -= fElapsed;

	if( m_tVehicle.fFuelGaugeElapsed <= 0.f )
		m_tVehicle.fFuelGaugeElapsed = 0.f;

	RwInt32 iMin, iMax, iGaugePos;
	m_pFuelGauge->GetRange(iMin, iMax);
	iGaugePos = (RwInt32)((RwReal)iMax * (m_tVehicle.fFuelGaugeElapsed / m_tVehicle.fFuelGaugeRemainTime) );
	m_pFuelGauge->SetPos(iGaugePos);	


	WCHAR* pwcText = Logic_GetCoolTimeString(m_tVehicle.fFuelGaugeElapsed);

	if( pwcText && m_tVehicle.wstrGaugeTooltop.compare(0, wcslen(pwcText), pwcText) != 0 )
	{
		m_tVehicle.wstrGaugeTooltop = pwcText;
		m_pFuelGauge->SetToolTip(m_tVehicle.wstrGaugeTooltop);
	}	
}

RwBool CVehicleGui::RegVehicleInfo(CNtlSobItem* pSobItem)
{
	CNtlSobItemAttr*	pSobItemAttr	= reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
	sITEM_TBLDAT*		pITEM_TBLDAT	= pSobItemAttr->GetItemTbl();
	TBLIDX				VehicleIndex	= (TBLIDX)Logic_GetItemSystemEffectValue(pITEM_TBLDAT, ACTIVE_VEHICLE);


	m_tVehicle.pVEHICLE_TBLDAT = (sVEHICLE_TBLDAT*)API_GetTableContainer()->GetVehicleTable()->FindData(VehicleIndex);
	if( !m_tVehicle.pVEHICLE_TBLDAT )
	{
		DBO_FAIL("Not exist VEHICLE_TABLE of index : " << VehicleIndex);
		return FALSE;
	}

	m_tVehicle.hVehicleItem = pSobItem->GetSerialID();

	return TRUE;
}

RwBool CVehicleGui::Fuel_CanRegister(SERIAL_HANDLE hHandle)
{
	if( hHandle == INVALID_SERIAL_ID )
		return FALSE;

	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
	if(!pSobItem)
	{
		DBO_FAIL("Not exist sob item of handle : " << hHandle);
		return FALSE;
	}
	
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	if (pSobItemAttr->IsNeedToIdentify())
	{
		return FALSE;
	}

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
	if (!pITEM_TBLDAT)
	{
		DBO_FAIL("Not exist item table of handle : " << hHandle);
		return FALSE;
	}

	if (pITEM_TBLDAT->byItem_Type != ITEM_TYPE_FUEL)
	{
		GetAlarmManager()->AlarmMessage("DST_VEHICLE_ONLY_REG_FUEL");
		return FALSE;
	}

	BYTE byPos = (BYTE)pSobItem->GetItemSlotIdx();
	BYTE byPlace = (BYTE)pSobItem->GetParentItemSlotIdx() + 1;

	API_GetSLPacketGenerator()->SendVehicle_FuelInsertReq(byPlace, byPos);

	return TRUE;
}

void CVehicleGui::Fuel_Register(SERIAL_HANDLE hHandle)
{
	// check if currently a fuel activated
	if (m_tFuel.FuelSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		// remove current fuel

		Fuel_LockUnlock_in_Bag();

		m_tFuel.FuelSlot.Clear();
		m_tVehicle.wstrGaugeTooltop.clear();
		m_pFuelGauge->Show(false);
		m_pFuelGauge->DeleteToolTip();
	}

	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
	if (!pSobItem)
	{
		DBO_FAIL("Not exist sob item of handle : " << hHandle);
		return;
	}

	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

	m_tFuel.FuelSlot.SetIcon(hHandle);
	Fuel_SetGauge();

	BYTE bySlotIndex = (BYTE)pSobItem->GetItemSlotIdx();
	BYTE byParentSlotIndex = (BYTE)pSobItem->GetParentItemSlotIdx();
	m_tFuel.setNewLockBagSlot.insert(PAIR_BAGSLOT(bySlotIndex, byParentSlotIndex));
	Fuel_LockUnlock_in_Bag();


	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pAvatar)
	{
		CNtlOtherParam*	pOtherParam = pAvatar->GetOtherParam();
		pOtherParam->SetRegisterFuelItem(hHandle);
		pOtherParam->SetRegisterFuelItemTableIndex(pITEM_TBLDAT->tblidx);
	}
	else
	{
		DBO_FAIL("Not exist avatar instance");
	}
}

bool CVehicleGui::Fuel_Unegister(RwBool bForce /* = FALSE */, RwBool bSaveUsedFuelItem /* = TRUE */)
{
	if( !bForce )
	{
		if( m_tFuel.FuelSlot.GetSerial() == INVALID_SERIAL_ID )
			return false;
	}

	if( bSaveUsedFuelItem )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( pAvatar )
		{
			CNtlOtherParam*	pOtherParam	= pAvatar->GetOtherParam();
			pOtherParam->SetRegisterFuelItem(INVALID_SERIAL_ID);
			pOtherParam->SetRegisterFuelItemTableIndex(INVALID_TBLIDX);
		}
	}	

	Fuel_LockUnlock_in_Bag();

	m_tFuel.FuelSlot.Clear();
	m_tVehicle.wstrGaugeTooltop.clear();
	m_pFuelGauge->Show(false);
	m_pFuelGauge->DeleteToolTip();

	return true;
}

VOID CVehicleGui::Fuel_SetGauge()
{
	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_tFuel.FuelSlot.GetSerial()));
	if(!pSobItem)
	{
		m_pFuelGauge->Show(false);
		return;
	}

	CNtlSobItemAttr*	pSobItemAttr	= reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	sITEM_TBLDAT*		pITEM_TBLDAT	= pSobItemAttr->GetItemTbl();
	if( !pITEM_TBLDAT )
	{
		DBO_FAIL("Not exist item table");
		m_pFuelGauge->Show(false);
		return;
	}

	RwUInt8 byDurationType = pSobItemAttr->GetDurationType();
	switch( byDurationType )
	{
		case eDURATIONTYPE_NORMAL:
		{
			m_tVehicle.fFuelGaugeElapsed	= 0.f;
			m_tVehicle.fFuelGaugeRemainTime	= 0.f;
			m_pFuelGauge->Show(false);
		}
		break;
		case eDURATIONTYPE_FLATSUM:
		case eDURATIONTYPE_METERRATE:
		{
			m_tVehicle.fFuelGaugeElapsed	= (RwReal)pSobItemAttr->GetRemainTime();
			m_tVehicle.fFuelGaugeRemainTime	= (RwReal)pITEM_TBLDAT->dwUseDurationMax;

			m_pFuelGauge->SetRange(0, (RwInt32)m_tVehicle.fFuelGaugeRemainTime);
			m_pFuelGauge->SetPos( (RwInt32)m_tVehicle.fFuelGaugeElapsed );
			m_pFuelGauge->Show(true);
		}
		break;
		default:
		{
			DBO_FAIL("Invalid item duration type : " << byDurationType);
			m_pFuelGauge->Show(false);
		}
		break;
	}
}

VOID CVehicleGui::Fuel_LockUnlock_in_Bag()
{
	// 기존에 가방의 연료에 걸려있던 아이템 락이 푼다
	SET_LOCKED_BAGSLOT::iterator it_ex = m_tFuel.setLockedBagSlot.begin();
	for( ; it_ex != m_tFuel.setLockedBagSlot.end() ; ++it_ex )
	{
		SET_LOCKED_BAGSLOT::iterator it_new = m_tFuel.setNewLockBagSlot.find( *it_ex );

		if( m_tFuel.setNewLockBagSlot.end() == it_new )
		{
			PAIR_BAGSLOT pairBagSlot = *it_ex;
			CDboEventGenerator::EnableItemIcon(TRUE, PLACE_BAG, pairBagSlot.first, pairBagSlot.second);
		}
	}

	// 새로이 가방의 연료에 아이템 락을 건다
	SET_LOCKED_BAGSLOT::iterator it_new = m_tFuel.setNewLockBagSlot.begin();	
	for( ; it_new != m_tFuel.setNewLockBagSlot.end() ; ++it_new )
	{
		it_ex = m_tFuel.setLockedBagSlot.find( *it_new );

		if( m_tFuel.setLockedBagSlot.end() == it_ex )
		{
			PAIR_BAGSLOT pairBagSlot = *it_new;
			CDboEventGenerator::EnableItemIcon(FALSE, PLACE_BAG, pairBagSlot.first, pairBagSlot.second);
		}
	}

	// 새로운 락 정보를 보관한다
	m_tFuel.setLockedBagSlot.clear();
	m_tFuel.setLockedBagSlot = m_tFuel.setNewLockBagSlot;
	m_tFuel.setNewLockBagSlot.clear();
}

VOID CVehicleGui::Vehicle_Start()
{
	/*if( false == m_pflashEngineWorking->Load(dENGINE_WORKING_FLASH) )
	{
		DBO_FAIL("Not eixst file : " << dENGINE_WORKING_FLASH << ". You can continue the game.");
	}*/

	// 플래쉬의 위치를 맞추기 위해
	OnMove(0, 0);

	//m_pflashEngineWorking->PlayMovie( TRUE );
}

VOID CVehicleGui::Vehicle_Stop()
{
//	m_pflashEngineWorking->Unload();

	if( m_tVehicle.fFuelGaugeElapsed == 0.f )
	{		
		sITEM_TBLDAT* pITEM_TBLDAT = m_tFuel.FuelSlot.GetItemTable();
		if( !pITEM_TBLDAT )
			return;

		switch( pITEM_TBLDAT->byDurationType )
		{
			case eDURATIONTYPE_NORMAL:
			{
			}
			break;
			case eDURATIONTYPE_FLATSUM:
			case eDURATIONTYPE_METERRATE:
			{
				m_tFuel.FuelSlot.Clear();
				m_tVehicle.wstrGaugeTooltop.clear();
				m_pFuelGauge->Show(false);
				m_pFuelGauge->DeleteToolTip();
				Fuel_LockUnlock_in_Bag();

				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( pAvatar )
				{
					CNtlOtherParam*	pOtherParam	= pAvatar->GetOtherParam();
					pOtherParam->SetRegisterFuelItem(INVALID_SERIAL_ID);
					pOtherParam->SetRegisterFuelItemTableIndex(INVALID_TBLIDX);
				}
			}		
			break;
			default:
			{
				DBO_FAIL("Invalid item duration type : " << pITEM_TBLDAT->byDurationType);
				break;
			}
		}		
	}
}

VOID CVehicleGui::Vehicle_UnlockVehichleItem()
{
	if( INVALID_SERIAL_ID == m_tVehicle.hVehicleItem )
		return;

	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_tVehicle.hVehicleItem));
	if(!pSobItem)
		return;
	
	RwUInt8	bySlotIndex			= (RwUInt8)pSobItem->GetItemSlotIdx();
	RwUInt8	byParentSlotIndex	= (RwUInt8)pSobItem->GetParentItemSlotIdx();
	RwInt32	iPlaceIdx			= (RwInt32)Logic_ConvertBagIdxToContainerType(byParentSlotIndex);

	CDboEventGenerator::CapsuleLockItem(FALSE, PLACE_BAG, bySlotIndex, iPlaceIdx);
}

VOID CVehicleGui::ShowIconDestination()
{
	if( GetIconMoveManager()->IsActive() )
	{
		SERIAL_HANDLE	hHandle			= GetIconMoveManager()->GetSrcSerial();
		sITEM_TBLDAT*	pITEM_TBLDAT	= Logic_GetItemDataFromSob(hHandle);

		if( !pITEM_TBLDAT )
			return;

		if( pITEM_TBLDAT->byItem_Type != ITEM_TYPE_FUEL )
			return;


		m_bSelected = TRUE;
	}
	else
	{
		m_bSelected = FALSE;
	}
}

RwBool CVehicleGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	return m_tFuel.FuelSlot.PtInRect(iX, iY);
}

VOID CVehicleGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_VEHICLE )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CVehicleGui::FocusEffect(RwBool bFocus)
{
	m_bFocus = bFocus;
}

VOID CVehicleGui::SelectEffect(RwBool bSelected)
{
	m_bSelected = bSelected;
}

VOID CVehicleGui::OnClicked_ExitButton(gui::CComponent* pComponent)
{
	API_GetSLPacketGenerator()->SendVehicle_EndReq();
}

VOID CVehicleGui::OnMouseDown(const CKey& key)
{
	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	if( FALSE == PtinSlot( (RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		return;


	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( GetIconMoveManager()->IsActive() )
			m_bLeftMouseDown = TRUE;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( FALSE == GetIconMoveManager()->IsActive() )
			m_bRightMouseDown = TRUE;
	}
}

VOID CVehicleGui::OnMouseUp(const CKey& key)
{
	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
	{
		m_bLeftMouseDown		= FALSE;
		m_bRightMouseDown		= FALSE;
		return;
	}

	if( FALSE == PtinSlot( (RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		m_bLeftMouseDown		= FALSE;
		m_bRightMouseDown		= FALSE;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_bLeftMouseDown && GetIconMoveManager()->IsActive() )
		{
			// Register new fuel
			if( Fuel_CanRegister( GetIconMoveManager()->GetSrcSerial() ) )
				GetIconMoveManager()->IconMoveEnd();
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( m_bRightMouseDown && FALSE == GetIconMoveManager()->IsActive() )
		{
			// Unregister fuel
			if (Fuel_Unegister())
			{
				API_GetSLPacketGenerator()->SendVehicle_EngineFuelRemoveNfy();
			}
		}
	}

	m_bLeftMouseDown		= FALSE;
	m_bRightMouseDown		= FALSE;
}

VOID CVehicleGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iFuelSlotCenterX = m_tFuel.FuelSlot.GetX_fromParent() + m_tFuel.FuelSlot.GetWidth()/2;
	RwInt32 iFuelSlotCenterY = m_tFuel.FuelSlot.GetY_fromParent() + m_tFuel.FuelSlot.GetHeight()/2;

	m_srfFocusEffect	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSelectEffect	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfFuelGaugeBack	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_tFuel.FuelSlot	.SetParentPosition(rtScreen.left, rtScreen.top);

	//CRectangle rtFlash = m_pflashEngineWorking->GetFrameResolution();
	//m_pflashEngineWorking->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());
	//m_pflashEngineWorking->SetPosition(iFuelSlotCenterX - rtFlash.GetWidth()/2,
	//								   iFuelSlotCenterY - rtFlash.GetHeight()/2);
}

VOID CVehicleGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( PtinSlot(nX, nY) )
	{
		FocusEffect(TRUE);

		if( GetInfoWndManager()->GetRequestGui() == DIALOG_VEHICLE )
			return;

		if( m_tFuel.FuelSlot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_ITEM &&
				GetInfoWndManager()->GetRequestGui() != DIALOG_VEHICLE )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM,
													rtScreen.left + m_tFuel.FuelSlot.GetX_fromParent(),
													rtScreen.top + m_tFuel.FuelSlot.GetY_fromParent(),
													m_tFuel.FuelSlot.GetSobItem(), DIALOG_VEHICLE );
			}
		}
	}
	else
	{
		FocusEffect(FALSE);
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CVehicleGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CVehicleGui::OnPaint()
{
	m_tFuel.FuelSlot.Paint();

	if( m_bFocus )
		m_srfFocusEffect.Render();

	if( m_bSelected )
		m_srfSelectEffect.Render();

	if( m_pFuelGauge->IsVisible() )
		m_srfFuelGaugeBack.Render();
}

RwInt32 CVehicleGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{		
	}
	else
	{
		m_bFocus				= FALSE;
		m_bLeftMouseDown		= FALSE;
		m_bRightMouseDown		= FALSE;
	}
	
	Show(bOpen);
	return 1;
}

VOID CVehicleGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CVehicleGui::HandleEvents");

	if( msg.Id == g_EventSobVehicleEngine )
	{
		SNtlEventSobVehicleEngine* pEvent = reinterpret_cast<SNtlEventSobVehicleEngine*>( msg.pData );

		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		switch( pEvent->byMessage )
		{
			case VEHICLE_READY:
			{
				Vehicle_Start();
			}
			break;
			case VEHICLE_ENGINE_ASK_REG_FROM_BAG:
			{
				Fuel_CanRegister(pEvent->uiParam);
			}
			break;
			case VEHICLE_ENGINE_REG_FUEL:
			{
				Fuel_Register(pEvent->uiParam);
			}
			break;

			case VEHICLE_ENGINE_END:
			{
				Vehicle_Stop();
			}
			break;
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		// disabled by daneos.. No idea why this exist.
		//if( Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) == FALSE )
		//	NTL_RETURNVOID();

		//SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		//if( Logic_GetAvatarHandle() != pEvent->hSerialId )
		//	NTL_RETURNVOID();

		//if( m_tFuel.FuelSlot.GetSerial() == INVALID_SERIAL_ID )
		//	NTL_RETURNVOID();

		//CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_tFuel.FuelSlot.GetSerial()));
		//if( !pSobItem )
		//{
		//	// Deletion of fixed-term items? ChetKey? For some reason, the registered fuel item disappeared
		//	Vehicle_Stop();
		//	Fuel_Unegister(TRUE);
		//}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
			ShowIconDestination();
	}

	NTL_RETURNVOID();
}