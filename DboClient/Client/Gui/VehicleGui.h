/******************************************************************************
* File			: Vehicle.h
* Author		: Hong SungBock
* Copyright		: (¡÷)NTL
* Date			: 2009. 1. 5
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc: Top's GUI
* In the initial plan, the fuel item is a stack item.
* All of the same fuels were registered at once and all were locked.
* Planning changes, fuel items are not stacked items, only one
* Only the fuel will be registered, so write info about the item lock on stl
* If you do not need it but you want to change it again later, stl :: pair
* To manage the item lock information.
*****************************************************************************/
#pragma once

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"

// cleint
#include "SlotGui.h"

struct sVEHICLE_TBLDAT;
struct sITEM_TBLDAT;
class CNtlSobItem;


class CVehicleGui : public CNtlPLGui, public RWS::CEventHandler
{
	typedef std::pair<RwUInt8, RwUInt8>			PAIR_BAGSLOT;
	typedef std::set<PAIR_BAGSLOT>				SET_LOCKED_BAGSLOT;

	struct sFUEL
	{
		CRegularSlotGui				FuelSlot;				///< ø¨∑· ΩΩ∑‘
		SET_LOCKED_BAGSLOT		setLockedBagSlot;
		SET_LOCKED_BAGSLOT		setNewLockBagSlot;
	};

	struct sVEHICLE
	{
		RwBool					bMoving;
		RwInt32					iFuelGaugeTooltipUpdatedLastTime;
		RwReal					fFuelGaugeElapsed;
		RwReal					fFuelGaugeRemainTime;
		SERIAL_HANDLE			hVehicleItem;

		std::wstring			wstrGaugeTooltop;

		sVEHICLE_TBLDAT*		pVEHICLE_TBLDAT;
	};

public:

	CVehicleGui(const RwChar* pName);
	virtual ~CVehicleGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	RwBool		RegVehicleInfo(CNtlSobItem* pSobItem);

protected:

	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			CheckInfoWindow();

	RwBool			Fuel_CanRegister(SERIAL_HANDLE hHandle);
	void			Fuel_Register(SERIAL_HANDLE hHandle);
	bool			Fuel_Unegister(RwBool bForce = FALSE, RwBool bSaveUsedFuelItem = TRUE);
	VOID			Fuel_SetGauge();
	VOID			Fuel_LockUnlock_in_Bag();

	VOID			Vehicle_Start();
	VOID			Vehicle_Stop();
	VOID			Vehicle_UnlockVehichleItem();

	VOID			ShowIconDestination();
	RwBool			PtinSlot(RwInt32 iX, RwInt32 iY);
	VOID			FocusEffect(RwBool bFocus);
	VOID			SelectEffect(RwBool bSelected);

	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:

	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pStopButton;
	gui::CButton*		m_pStartButton;

	gui::CProgressBar*	m_pFuelGauge;
	gui::CFlash*		m_pflashEngineWorking;

	CSurfaceGui			m_srfFocusEffect;		///< ΩΩ∑‘ ∆˜ƒøΩ∫ ¿Ã∆Â∆Æ
	CSurfaceGui			m_srfSelectEffect;		///< ΩΩ∑‘ ºø∑∫∆Æ ¿Ã∆Â∆Æ	
	CSurfaceGui			m_srfFuelGaugeBack;		///< ø¨∑· ∞‘¿Ã¡ˆ πË∞Ê

	RwBool				m_bFocus;
	RwBool				m_bSelected;
	RwBool				m_bLeftMouseDown;
	RwBool				m_bRightMouseDown;

	sFUEL				m_tFuel;
	sVEHICLE			m_tVehicle;
};