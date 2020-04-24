#pragma once

#include "ceventhandler.h"
#include "SlotGui.h"

#include "NtlParty.h"


class CCCBDRewardGui : public CNtlPLGui, public RWS::CEventHandler
{

	struct sPARTY_MEMBER
	{
		BYTE				bySelectState;
		HOBJECT				hHandle; // party member handle

		gui::CStaticBox*	pstbMemberName;
		gui::CStaticBox*	pstbMemberState;

		CSurfaceGui			srfSelectState[NTL_PARTY_SELECT_TYPE_COUNT];
	};

protected:

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMouseMove;

	gui::CStaticBox*	m_psttTitle;
	gui::CStaticBox*	m_pstbRewardName;
	gui::CStaticBox*	m_pstbLeftTimeStatic;
	gui::CStaticBox*	m_pstbLeftTime;

	gui::CHtmlBox*		m_phbxExplain;

	gui::CStaticBox*	m_pstbCharName;
	gui::CStaticBox*	m_pstbState;
	gui::CStaticBox*	m_pstbRewardItem;

	CSurfaceGui			m_srfNameBar;

	CSurfaceGui			m_srfHideSelectButtons;

	gui::CStaticBox*	m_pstbChoiceReward;

	CSurfaceGui			m_srfChoiceRewardBar;

	gui::CButton*		m_pbtnChoiceReward;
	gui::CSlot			m_slotClickedBtnChoiceReward;

	gui::CButton*		m_pbtnChallengeNext;
	gui::CSlot			m_slotClickedBtnChallengeNext;

	gui::CButton*		m_pbtnExit;				// only visible in last stage
	gui::CSlot			m_slotClickedBtnExit;


	CRegularSlotGui		m_ItemSlot;

private:

	bool				m_bLastStage;
	float				m_fRewardCountdown;
	BYTE				m_bySelection;
	std::vector<sPARTY_MEMBER*>	m_members;
	std::map<HOBJECT, BYTE>		m_selectState;
	
	bool				m_bShowItemInfo;

public:

	CCCBDRewardGui(const RwChar* pName);
	~CCCBDRewardGui();

	//! Operation
	void						Init();
	int							Create();
	void						Destroy();

	int							SwitchDialog(bool bOpen);

	virtual void				HandleEvents(RWS::CMsg& msg);

	void						Update(float fElapsed);

private:

	void						LoadPartyMembers();
	void						ReloadPartyMember();

	void						SelectState(HOBJECT hHandle, BYTE byState);
	BYTE						GetSelectState(HOBJECT hHandle);

	void						SetStateText(gui::CStaticBox* pStateBox, BYTE byState);

	void						ShowItemInfoWindow(bool bIsShow);

protected:

	void						OnPaint();
	void						OnMove(int iOldX, int iOldY);
	void						OnMouseMove(int nFlags, int nX, int nY);

	void						OnClickedBtnChoiceReward(gui::CComponent* pComponent);
	void						OnClickedBtnChallengeNext(gui::CComponent* pComponent);
	void						OnClickedBtnExit(gui::CComponent* pComponent);

};