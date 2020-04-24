/******************************************************************************
* File			: PetitionSatisFactionGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 10. 23
* Abstract		: 
*****************************************************************************
* Desc			: GM과의 상담 내용 만족도 설문조사 GUI
*****************************************************************************/

#pragma once

// presetation
#include "NtlPLGui.h"

// share
#include "NtlSharedType.h"
#include "NtlPetition.h"

#define dTEMP_GG	5

class CPetitionSatisfaction : public CNtlPLGui
{
public:
	CPetitionSatisfaction(const RwChar* pName);
	virtual ~CPetitionSatisfaction();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CPetitionSatisfaction() {}

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnClicked_RadioToggleButton(gui::CComponent* pComponent, bool bToggled);
	VOID			OnClicked_SendButton(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotRadioButton[dTEMP_GG];
	gui::CSlot			m_slotSendButton;

	gui::CStaticBox*	m_pDialogName;					///< 다이얼로그 제목
	gui::CStaticBox*	m_pMessage;						///< 안내 메세지
	gui::CStaticBox*	m_pToggleText[dTEMP_GG];		///< 만족도 텍스트

	gui::CButton*		m_pCloseButton;					///< 닫기 버튼
	gui::CButton*		m_pRadioButton[dTEMP_GG];		///< 만족도 선택 라디오 버튼
	gui::CButton*		m_pSendButton;					///< 만족도 보내기 버튼

	RwInt32				m_iSelectedIndex;				///< 선택한 만족도 인덱스

	PETITIONID			m_petitionID;
	ACCOUNTID			m_GMID;
};