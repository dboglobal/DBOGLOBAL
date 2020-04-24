/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 9. 13
* Last Update	: 2007. 8. 28
* Abstract		:
*****************************************************************************
* Desc			:
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

#include "NtlSLDef.h"
#include "NtlSob.h"

// dbo
#include "SurfaceGui.h"
#include "NumberGui.h"

class CNtlSobItem;
class CNtlSobActor;

class CScouterMeasureDlgGui : public CNtlPLGui, public RWS::CEventHandler
{

public:

	enum eNumberColor
	{
		NUMBERCOLOR_RED,
		NUMBERCOLOR_YELLOW,
		NUMBERCOLOR_WHITE,

		NUM_COLOR
	};

	struct sTarget
	{
		SERIAL_HANDLE		hTarget;
		CNtlSob*			pSobTarget;
		DWORD				dwDisplayFlag;
		RwReal				fTimer;

		sTarget()
			:hTarget(INVALID_SERIAL_ID)
			, pSobTarget(NULL)
			, dwDisplayFlag(0)
			, fTimer(0.f)
		{}
	};

	struct sPowerData : public sTarget
	{

		RwBool				bShowScouterBack;

		RwUInt32			uiOffencePower;		///< 목표 전투력
		RwUInt32			uiMoreMeasureCount;	///< 전투력을 갱신한 횟수

		RwReal				fTimeRecievePower;	///< 전투력을 서버로부터 받은 시간
		RwReal				fTimeAdjustPower;	///< 전투력 보간 시간
		RwReal				fTimeDisplayPower;	///< 전투력 수치를 보여주는 시간

		RwInt32				iAdjustCount;		///< 전투력 보간 횟수

		bool				bAskPower;			///< Requested attack power level	

		sPowerData()
			:bShowScouterBack(true)
			, uiOffencePower(0)
			, uiMoreMeasureCount(0)
			, fTimeRecievePower(0.f)
			, fTimeAdjustPower(0.f)
			, fTimeDisplayPower(0.f)
			, iAdjustCount(0)
			, bAskPower(false)
		{}
	};


	CScouterMeasureDlgGui(const RwChar* pName);
	virtual ~CScouterMeasureDlgGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();
	VOID		Clear();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CScouterMeasureDlgGui() {}
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID		StartMeasure();		///< 스카우터 측정을 시작한다

	// 전투력 측정 관련
	RwBool		MeasurePower_Init();
	VOID		MeasurePower_Destroy();
	VOID		MeasurePower_Display(RwUInt32 uiPower, int nPhysOffence, int nPhysDefence, int nEnergyOffence, int nEnergyDefence, int nHitRate, int nDodge);
	VOID		MeasurePower_AskPowertoServer();

	// 히든 오브젝트 찾기 관련

	VOID		EndDisplayImmediately();

	VOID		TargetEdgeEffect(bool bActive);

	VOID		OnPaint();

protected:


	gui::CFlash*		m_pFlaMeasureBg;
	gui::CFlash*		m_pFlashDirect;
	gui::CPanel*		m_pPn_AttackRate;
	gui::CPanel*		m_pPn_DodgeRate;

	gui::CDialog*		m_pdlgPhysicalEnergy;

	gui::CPanel*		m_pPn_OffenceBg;
	gui::CPanel*		m_pPn_DefenceBg;
	gui::CPanel*		m_pPn_AttackEnergyName;

	gui::CPanel*		m_pPn_EnergyOffece;
	gui::CProgressBar*	m_pPgb_PhysicalOffence;

	gui::CPanel*		m_pPn_EnergyDefence;
	gui::CProgressBar*	m_pPgb_PhysicalDefence;

	gui::CStaticBox*	m_pStb_PhysicalOffence;
	gui::CStaticBox*	m_pStb_EnergyOffece;
	gui::CStaticBox*	m_pStb_PhysicalDefence;
	gui::CStaticBox*	m_pStb_EnergyDefence;

	CNumberGui			m_Number[NUM_COLOR];
	CNumberGui			m_NumberDistance;

	CNumberGui			m_numberHitRate;
	CNumberGui			m_numberDodge;

	CSurfaceGui			m_srfDistanceMark;
	CSurfaceGui			m_srfDistanceUnit;

	//

	gui::CSlot			m_slotPaint;

	//

	bool				m_bShowCombatInfo;

	sPowerData			m_scouterData;

	bool				m_bOutofOrder;
};