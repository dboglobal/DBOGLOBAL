#pragma once

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "NtlSharedType.h"
#include "SlotGui.h"

#define dHOIPOIMIX_ARROW_MAX_NUM			3
#define dHOIPOIMIX_PROGRESS_BAR_WIDTH		253
#define dHOIPOIMIX_PROGRESS_BAR_WIDTH_F		253.f


class CHoiPoiMixCraftGui : public CNtlPLGui, public RWS::CEventHandler
{

public:

	CHoiPoiMixCraftGui(const RwChar* pName);
	virtual ~CHoiPoiMixCraftGui();

	RwBool			Create();
	VOID			Destroy();

	int				SwitchDialog(bool bOpen);

	VOID			InitFlash();
	VOID			InitGui();

	VOID			Update(RwReal fElapsed);
	void			MaterialCheck();
	bool			UpdateMaterial(TBLIDX tblidx, HOBJECT hItem, BYTE byPlace, BYTE byPos, BYTE byStackCount);

	VOID			SetRecipe(TBLIDX idxRecipeTbl);

	VOID			HandleEvents(RWS::CMsg& msg);

protected:

	VOID			OnPaint();
	VOID			OnMove(int nOldX, int nOldY);

	VOID			StartCreate(RwInt32 nAmount);
	VOID			Step(RwBool bStep);
	VOID			StopCreate();

	VOID			PlayResultMovie(RwUInt8 byResultType);

	VOID			SetCreating(RwBool bCreate);

	VOID			SetOutputLog(const WCHAR* pLog, COLORREF color);

	VOID			OnClickedBtnMax(gui::CComponent* pComponent);
	VOID			OnClickedBtnCalc(gui::CComponent* pComponent);
	VOID			OnClickedBtnCreate(gui::CComponent* pComponent);
	VOID			OnClickedBtnStop(gui::CComponent* pComponent);

	VOID			OnMouseMove(RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos);
	void			OnMouseUp(const CKey& key);
	VOID			OnMovieEnd(gui::CComponent* pComponent);

	void			InitItems();

	int				Logic_AmountEnableCreateRecipe();

protected:

	gui::CTabButton*	m_ptabHoipoiMix;
	gui::CStaticBox*	m_pstbTitle;
	gui::CDialog*		m_pdlgCreate;
	gui::CStaticBox*	m_pstbTitleCreate;
	gui::CStaticBox*	m_pStbSettingCategory;
	gui::CStaticBox*	m_pStbZenny;
	gui::CButton*		m_pBtnMax;
	gui::CButton*		m_pBtnCalc;
	gui::CStaticBox*	m_pStbPreViewCategory;
	gui::CButton*		m_pBtnCreate;
	gui::CButton*		m_pBtnStop;
	gui::CStaticBox*	m_pStbControlCategory;
	gui::CProgressBar*	m_pPgbConcent;
	gui::CStaticBox*	m_pStbConcentText;
	gui::CPanel*		m_pPnlNotify;
	gui::COutputBox*	m_pOpbLog;
	gui::CParticleBox*	m_pPbBox;
	gui::CFlash*		m_pFlaResult[HOIPOI_MIX_RESULT_COUNT];

	gui::CSlot			m_slotClickedBtnMax;
	gui::CSlot			m_slotClickedBtnCalc;
	gui::CSlot			m_slotClickedBtnCreate;
	gui::CSlot			m_slotClickedBtnStop;
	gui::CSlot			m_slotMovieEnd[HOIPOI_MIX_RESULT_COUNT];

	TBLIDX				m_idxEnableRecipe;

	gui::CPanel*		m_pPnlMaterialSlot[DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM];
	gui::CPanel*		m_pPnlPreViewSlot[DBO_MAX_COUNT_RECIPE_CREATE_ITEM];

	CRegularSlotGui		m_SlotRecipe;

	CRegularSlotGui		m_SlotMaterial[DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM];
	CRegularSlotGui		m_SlotPreview[DBO_MAX_COUNT_RECIPE_CREATE_ITEM];
	gui::CStaticBox*	m_apStbStackMaterial[DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM];
	gui::CStaticBox*	m_apStbStackPreview[DBO_MAX_COUNT_RECIPE_CREATE_ITEM];

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseUp;

	RwBool				m_bCreating;
	int					m_nLeftCreate;
	RwBool				m_bStep;

	float				m_fElapsedTime;
	float				m_fNotifyTime;

	gui::CPanel*		m_pPnlEpArrow[dHOIPOIMIX_ARROW_MAX_NUM];
	gui::CPanel*		m_pPnlHitLine[dHOIPOIMIX_ARROW_MAX_NUM];

	UINT32				m_uiOpbLogCount;

	HOBJECT				m_hObject;

	struct sITEM
	{
		sITEM()
		{
			byPlace = INVALID_BYTE;
			byPos = INVALID_BYTE;
			byStack = 0;
			hItem = INVALID_HOBJECT;
		}

		BYTE	byPlace;
		BYTE	byPos;
		BYTE	byStack;
		HOBJECT	hItem;
	}
	m_recipeMat[DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM];

};
