			/******************************************************************************
* File			: CharCreateGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 12. 7
* Abstract		: 
*****************************************************************************
* Desc			: CharCreateGui
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// gui
#include "gui_button.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "Inputhandler.h"

// dbo
#include "Windowby3.h"
#include "SlotGui.h"
/*
class CCharCreateGuiGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	enum eAttiribute
	{
		ATTIRIBUTE_RACE,
		ATTIRIBUTE_GENDER,
		ATTIRIBUTE_CLASS,
		ATTIRIBUTE_HAIR,
		ATTIRIBUTE_HAIR_COLOR,
		ATTIRIBUTE_FACE,
		ATTIRIBUTE_SKIN,		

		ATTIRIBUTE_NUM
	};

	enum eRotation
	{
		ER_STOP,
		ER_TURN_LEFT,
		ER_TURN_RIGHT
	};

	struct AttiributeSlot
	{
		CSurfaceGui			Backboard;		///< 배경
		CSurfaceGui			Textboard;		///< text backboard
		gui::CButton*		pLeftButton;	///< 왼쪽 항목 선택
		gui::CButton*		pRightButton;	///< 오른쪽 항목 선택
		gui::CStaticBox*	pTitle;			///< 항목 설명
		gui::CStaticBox*	pAttribute;		///< 선택된 항목
		CSlotGui			Icon;			///< 속성 아이콘
		RwUInt8				byValue;		///< 현재 선택된 값
		std::list<std::wstring> listContent;	///< 속성 값

		~AttiributeSlot()
		{
			Icon.Destroy();
		}

		VOID SelectIndex(RwUInt8 byIndex)
		{
			if( byIndex >= listContent.size() )
			{
				return;
			}

			RwUInt8 byCount = 0;
			std::list<std::wstring>::iterator it = listContent.begin();
			for( ; it != listContent.end() ; ++it, ++byCount )
			{
				if( byIndex == byCount )
					break;
			}

			pAttribute->SetText(it->c_str());
			byValue = byIndex;
		}

		VOID ClearText()
		{
			listContent.clear();
		}

		VOID SetPosition(RwInt32 IPosX, RwInt32 iPosY)
		{
			Backboard.SetPosition(IPosX, iPosY);
			Textboard.SetPosition(IPosX + 49, iPosY + 34);
			Icon.SetPosition(IPosX + 12, iPosY + 18);
			pLeftButton->SetPosition(IPosX + 50, pLeftButton->GetPosition().top);
			pRightButton->SetPosition(IPosX + 175, pRightButton->GetPosition().top);
			pTitle->SetPosition(IPosX, pTitle->GetPosition().top);
			pAttribute->SetPosition(IPosX + 49, pAttribute->GetPosition().top);
		}
	};

	struct SavedAttribute
	{
		RwUInt8					byRaceValue;	///< 종족
		RwUInt8					byHair;			///< 머리 모양
		std::list<std::wstring> listContent;	///< 속성 값
	};

	CCharCreateGuiGui(const RwChar* pName);
	virtual ~CCharCreateGuiGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		Clear();
	VOID		SetAlpha(RwUInt8 byAlpha);

protected:
	CCharCreateGuiGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			EnableButtons(bool bEnable);

	VOID			UpdateSlot(RwBool bRight, RwUInt8 byIndex);///< 좌우 버튼에 따라 슬롯의 내용을 갱신한다.
	VOID			UpdateComboBox();		///< 콤보 박스의 내용들을 갱신한다	
	VOID			CheckEnableButton();	///< 항목에 따른 버튼 사용가능 여부를 알아본다

	RwUInt8			ConverIndexToClass(RwInt32 iIndex);	///< 콤보 리스트 인덱스에 따라 종족별 클래스 반환
	RwUInt8			ConverIndexToGenderbyRace(RwInt32 iRace);	///< 콤보 리스트 인덱스에 따라 종족의 성별 반환

	VOID			LocateComponent(RwInt32 IWidth, RwInt32 IHeight);
	
	VOID			ChangeModel(RwBool bSetDefaultCamera);	
	VOID			ClickLeftButton(gui::CComponent* pComponent);
	VOID			ClickRightButton(gui::CComponent* pComponent);
	VOID			ClickRotButton(gui::CComponent* pComponent);
	VOID			ClickedRandomButton(gui::CComponent* pComponent);
	VOID			ClickedMakeButton(gui::CComponent* pComponent);
	VOID			ClickedReturnButton(gui::CComponent* pComponent);
	VOID			OnAbsoluteWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	VOID			OnPaint();

protected:	
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotAbsoluteWheelMove;
	gui::CSlot			m_slotLeft[ATTIRIBUTE_NUM];
	gui::CSlot			m_slotRight[ATTIRIBUTE_NUM];
	gui::CSlot			m_slotLeftRotButton;	
	gui::CSlot			m_slotRightRotButton;	
	gui::CSlot			m_slotStopButton;
	gui::CSlot			m_slotRandomButton;
	gui::CSlot			m_slotMakeButton;
	gui::CSlot			m_slotReturnButton;	

	CWindowby3			m_RaceWin;			///< Race 배경
	CWindowby3			m_ClassWin;			///< Class 배경
	CWindowby3			m_MakeWin;			///< Make 배경
	CWindowby3			m_MakeWin2;			///< Make2 배경

	CSurfaceGui			m_RaceText;			///< Race text
	CSurfaceGui			m_RaceUnderLine;	///< Race under line
	CSurfaceGui			m_ClassText;		///< Class text	
	CSurfaceGui			m_ClassUnderLine;	///< Race under line
	CSurfaceGui			m_MakeText;			///< Make text
	CSurfaceGui			m_MakeUnderLine;	///< Make under line
	CSurfaceGui			m_NameInputBack;	///< 이름 입력란 배경

	gui::CStaticBox*	m_pRaceName;		///< 종족 이름
	gui::CStaticBox*	m_pClassName;		///< 클래스 이름
	gui::CStaticBox*	m_pRaceInfo;		///< 종족 설명
	gui::CStaticBox*	m_pClassInfo;		///< 클래스 설명
	gui::CStaticBox*	m_pCharMake;		///< '캐릭터 만들기' 스태틱
	gui::CStaticBox*	m_pNameInputStatic;	///< '이름' 스태틱

	gui::CInputBox*		m_pNameInput;		///< 이름 입력란

	gui::CButton*		m_pLeftRotButton;	///< 왼쪽 회전 버튼
	gui::CButton*		m_pRightRotButton;	///< 오른쪽 회전 버튼
	gui::CButton*		m_pStopButton;		///< 정지 버튼
	gui::CButton*		m_pRandomButton;	///< 랜덤 버튼
	gui::CButton*		m_pMakeButton;		///< 만들기 버튼
	gui::CButton*		m_pReturnButton;	///< 돌아가기 버튼

	AttiributeSlot		m_AttibuteSlot[ATTIRIBUTE_NUM];		///< 캐릭터 속성 슬롯
	SavedAttribute		m_TempAttribute;	///< 항목 선택 불가시 저장
	RwUInt8				m_byRotationState;	///< 캐릭터 회전상태
};
*/
/*
class CRaceExplainGui : public CNtlPLGui
{
public:
	CRaceExplainGui(const RwChar* pName);
	virtual ~CRaceExplainGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetRace(RwUInt8 byRace);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CRaceExplainGui() {}

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< 배경

	CSurfaceGui			m_srfRaceText;		///< Race text
	CSurfaceGui			m_srtRaceNameBack;	///< 종족 이름 배경
	
	gui::CStaticBox*	m_pRaceName;		///< 종족 이름
	//gui::CHtmlBox*		m_pExplainHtml;		///< 종족 설명
};

class CClassExplainGui : public CNtlPLGui
{
public:
	CClassExplainGui(const RwChar* pName);
	virtual ~CClassExplainGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetClass(RwUInt8 byRace);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CClassExplainGui() {}

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< 배경

	CSurfaceGui			m_srfClassText;		///< Class text
	CSurfaceGui			m_srtClassNameBack;	///< 클래스 이름 배경

	gui::CStaticBox*	m_pClassName;		///< 클래스 이름
	//gui::CHtmlBox*		m_pExplainHtml;		///< 클래스 설명
};

class CCharCreateGuiGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eRotation
	{
		ER_STOP,
		ER_TURN_LEFT,
		ER_TURN_RIGHT
	};

	CCharCreateGuiGui(const RwChar* pName);
	virtual ~CCharCreateGuiGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetAlpha(RwUInt8 byAlpha);

protected:
	CCharCreateGuiGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			LocateComponent(RwInt32 IWidth, RwInt32 IHeight);

	VOID			OnClickLeftTurnButton(gui::CComponent* pComponent);
	VOID			OnClickRightTurnButton(gui::CComponent* pComponent);
	VOID			OnClickStopButton(gui::CComponent* pComponent);

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotLeftRotButton;	
	gui::CSlot			m_slotRightRotButton;	
	gui::CSlot			m_slotStopButton;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pLeftRotButton;	///< 왼쪽 회전 버튼
	gui::CButton*		m_pRightRotButton;	///< 오른쪽 회전 버튼
	gui::CButton*		m_pStopButton;		///< 정지 버튼

	CSurfaceGui			m_NameInputBack;	///< 이름 입력란 배경

	CRaceExplainGui*	m_pRaceExplainGui;	///< 종족 설명 Gui
	CClassExplainGui*	m_pClassExplainGui;	///< 클래스 설명 Gui

	RwUInt8				m_byRotationState;	///< 캐릭터 회전상태
};*/