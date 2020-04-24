#pragma once

#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlSharedType.h"
#include "SlotGui.h"

struct sITEM_RECIPE_TBLDAT;
class CRecipeDlgItem;

class CHoiPoiMixRecipeGui : public CNtlPLGui, public RWS::CEventHandler
{

public:

	CHoiPoiMixRecipeGui(const RwChar* pName);
	virtual ~CHoiPoiMixRecipeGui();

	enum eTAB
	{
		INVALID_TAB = -1,
		RECIPE_TAB,

		NUM_TAB
	};

	enum eFILTER { FILTER_NONE = -1, FILTER_NORMAL = 0, FILTER_WEAPON, FILTER_ARMOR, FILTER_ACCESSORY, NUM_FILTER };

	RwBool			Create();
	void			Destroy();

	void			HandleEvents(RWS::CMsg& msg);
	int				SwitchDialog(bool bOpen);

	void			SelectRecipe(CRecipeDlgItem* pSelectItem);

protected:

	void			SelectTab(eTAB eTabIndex);

	void			OnMove(int iOldX, int iOldY);

	void			OnClickedClose(gui::CComponent* pComponent);
	void			OnSelectTab(int nSelectIdx, int nPreviousIndex);

	VOID			OnAbsoluteMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnScrollChanged(RwInt32 nNewOffset);

	VOID			SelectFirstItem();

	RwBool			CreateAvatarRecipeList();
	VOID			DestroyAvatarRecipeList();

	VOID			OnToggledFilter(gui::CComponent* pComponent, bool bToggled);
	VOID			CreateRecipeFilterList();
	VOID			RecipeDispoisition();

	VOID			SelectRecipeItem(CRecipeDlgItem* pSelectItem);
	VOID			DeSelectRecipeItem();

protected:

	gui::CStaticBox*	m_pstbTitle;
	gui::CButton*		m_pbtnClose;
	gui::CTabButton*	m_ptabHoipoiMix;
	gui::CDialog*		m_pdlgRecipe;
	gui::CDialog*		m_pdlgRecipeClip;
	gui::CDialog*		m_pdlgRecipeList;
	gui::CScrollBar*	m_pScbBar;
	gui::CDialog*		m_pdlgEmptyRecipe;
	gui::CStaticBox*	m_pstbEmptyRecipe;

	gui::CSlot			m_slotSelectTab;
	gui::CSlot			m_slotClickedClose;

	gui::CButton*		m_pBtnFilter[NUM_FILTER];

	gui::CSlot			m_slotMove;

	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollSliderMoved;

	gui::CSlot			m_slotToggledFilter1;
	gui::CSlot			m_slotToggledFilter2;
	gui::CSlot			m_slotToggledFilter3;
	gui::CSlot			m_slotToggledFilter4;

	typedef std::vector< CRecipeDlgItem* > RECIPE_VEC;
	RECIPE_VEC			m_vecAvatarRecipe;					///< 아바타가 가지고 있는 레시피 리스트

	RECIPE_VEC			m_vecShowListRecipe;

	gui::CSlot			m_slotAbsoluteMouseWheel;

	RwUInt8				m_byDiscountRate;					///< 호이포이 믹스 머신의 가격 할인률 ( 0 ~ 100% )

	CRecipeDlgItem*		m_pSelectDlgItem;

private:

	HOBJECT				m_hObject;
	eTAB				m_eCurTab;
};


class CRecipeDlgItem
{

public:

	CRecipeDlgItem();
	virtual ~CRecipeDlgItem();

	RwBool		Create(RwInt32 nIndex, CHoiPoiMixRecipeGui* pParentGui, TBLIDX idxRecipeTbl, RwUInt8 byDiscountRate);
	VOID		Destroy();

	RwUInt8		GetRecipeType();
	TBLIDX		GetRecipeTableIndex();

	VOID		RecipeItemDisposition(RwUInt32 uiIndex);
	VOID		Show(RwBool bShow);
	VOID		SelectItem(RwBool bSelect);

	VOID		OnEnterItem(gui::CComponent* pComponent);
	VOID		OnLeaveItem(gui::CComponent* pComponent);

	VOID		OnMouseDown(const CKey& key);
	VOID		OnMouseUp(const CKey& key);
	VOID		OnMouseMove(RwInt32 nKeys, RwInt32 nX, RwInt32 nY);

protected:

	VOID		OnPaint();
	VOID		OnMove(int nOldX, int nOldY);

protected:

	gui::CDialog*		m_pThis;
	CRegularSlotGui		m_Slot;
	CSurfaceGui			m_surIcon;

	RwInt32				m_nIndex;
	TBLIDX				m_idxTbl;
	RwBool				m_bSelect;
	RwBool				m_bMouseDown;

	gui::CStaticBox*	m_pStbRecipeName;
	gui::CStaticBox*	m_pStbRequiredLevel;
	gui::CPanel*		m_pPnlSlot;
	gui::CStaticBox*	m_pStbZenny;

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	gui::CSlot			m_slotEnterItem;
	gui::CSlot			m_slotLeaveItem;

	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;

	sITEM_RECIPE_TBLDAT*	m_pItemRecipeTblDat;
	CHoiPoiMixRecipeGui*	m_pHoipoiMix;
};
