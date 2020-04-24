#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// client
#include "Windowby3.h"
#include "ButtonList.h"

#define TITLE_LIST_MAX_VISIBLE 13

class CPlayerTitleGui : public CNtlPLGui, public RWS::CEventHandler
{
	struct sTITLES
	{
		sTITLES::sTITLES()
		{
			titleIdx = -1;
			bRegistered = false;
			nEffectCount = 0;
		}

		RwUInt32		titleIdx;		//< Title table index
		bool			bRegistered;	//< Is title available flag
		std::wstring	wstrEffect[3];	// NTL_MAX_CHAR_TITLE_EFFECT effect text
		int				nEffectCount;
	};

	int				    m_nButtonIndex;
	unsigned int		m_uiActiveCharTitleIndex;
	int					m_nSelectedButton;
	unsigned int		m_uiSelectedTitleIndex;

	std::vector<sTITLES*>	m_vecTitles;	//< hold all titles

public:

	CPlayerTitleGui(const RwChar* pName);
	virtual ~CPlayerTitleGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:

	CPlayerTitleGui() {}

	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			CloseButtonClicked(gui::CComponent* pComponent);

	VOID			OnPaint();

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnClickedBtnHelp(gui::CComponent* pComponent);
	VOID			OnToggled(gui::CComponent* pComponent, bool bToggled);
	VOID			OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnTitleScrollChanged(RwInt32 nNewOffset);

	gui::CButton*	OnCreateItem();
	void			OnDestroyItem(gui::CButton* pButton);
	void			OnClickedItem(RwInt32 iIndex);

	void			OnApplyToggled(gui::CComponent* pComponent, bool bToggle);

	void			CreateInfoStaticBox();
	void			RefreshTitleList();
	void			UpdateTitleInfo(unsigned int uiTitleIdx = 0);

private:

	void			LoadCharTitleInfo();
	void			SelectCharTitle(unsigned int uiIndex);
	void			UpdateCharTitle(unsigned int uiIndex, bool bDelete);

protected:

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureWheelMove;

	gui::CStaticBox*	m_pDialogName;

	CButtonList<CPlayerTitleGui, sTITLES*> m_InfoList;
	gui::CPanel*        m_pPnlBackSpace[TITLE_LIST_MAX_VISIBLE];
	gui::CSlot          m_slotListWheelMove[TITLE_LIST_MAX_VISIBLE];
	gui::CSlot          m_slotToggled[TITLE_LIST_MAX_VISIBLE];

	CSurfaceGui			m_srfEffectBar;
	CWindowby3			m_EffectBackSurface;
	gui::COutputBox*	m_pEffectBox;
	gui::CStaticBox*	m_pEffectStatic;

	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;

	gui::CScrollBar*	m_pSbTitleScroll;
	gui::CSlot			m_slotScroll;
	gui::CSlot			m_slotScrollMove;

	gui::CScrollBar*	m_pSbEffectScroll;

	gui::CButton*		m_pApplyButton;
	gui::CSlot			m_slotApplyButton;
	gui::CStaticBox*	m_psttApply;

	gui::CButton*		m_pExitButton;
	gui::CSlot			m_slotCloseButton;


};