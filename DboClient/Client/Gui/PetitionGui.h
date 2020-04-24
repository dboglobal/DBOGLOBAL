/******************************************************************************
* File			: PetitionGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 10. 23
* Abstract		: 
*****************************************************************************
* Desc			: GM 상담 요청창
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// cleint
#include "Windowby3.h"
#include "SurfaceGui.h"

class CPetitionGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPetitionGui(const RwChar* pName);
	virtual ~CPetitionGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	// avooo's comment : GM 상담 요청창은 게임의 내용이 아니며 이미 다른 다이얼로그가
	//					 꽉 들어찬 상태에서 고정적으로 자리를 정할 수도 없다.
	//					 언제나 유저가 위치를 변경할 수 있도록 한다.
	virtual void	SetMovable(RwBool bMovable) {}

protected:
	CPetitionGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			EnableControll(bool bEnable);

	VOID			SetTextCategory(RwInt32 iSelect = INVALID_INDEX);
	VOID			SetTextCategory2(RwInt32 iSelect = INVALID_INDEX);

	const WCHAR*	GetCategory1Text(RwInt32 iSelect);
	const WCHAR*	GetCategory2Text(RwInt32 iSelect);

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnClicked_SendButton(gui::CComponent* pComponent);

	VOID			OnItemSelect( INT nIndex );
	VOID			OnItemSelect2( INT nIndex );
	
	VOID			OnGotFocus();
	VOID			OnLostFocus();
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotItemSelect;
	gui::CSlot			m_slotItemSelect2;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotSendButton;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pCloseButton;			///< 닫기 버튼

	gui::CComboBox*		m_pCategory;			///< 대분류 카테고리
	gui::CComboBox*		m_pCategory2;			///< 소분류 카테고리
	
	gui::CInputBox*		m_pInput;				///< 진정내용
	
	gui::CStaticBox*	m_pDialogName;			///< 다이얼로그 제목
	gui::CStaticBox*	m_pCategoryStatic;		///< 대분류 카테고리
	gui::CStaticBox*	m_pCategoryStatic2;		///< 소분류 카테고리
	gui::CStaticBox*	m_pCategoryMessage;		///< 대분류 카테고리 선택 메세지
	gui::CStaticBox*	m_pCategoryMessage2;	///< 소분류 카테고리 선택 메세지
	
	gui::CButton*		m_pSendButton;			///< 전송버튼

	CWindowby3			m_BackPanel;			///< 배경
	CSurfaceGui			m_srfCaregoryBack;		///< 카테고리 1 배경
	CSurfaceGui			m_srfCaregoryBack2;		///< 카테고리 2 배경
};