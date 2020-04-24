#pragma once

#include "NtlDebug.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlSLEvent.h"
#include "ButtonList.h"
#include "NtlSLFriendList.h"
#include "NtlSLBlackList.h"
#include "SurfaceGui.h"

#define FRIEND_LIST_MAX_VISIBLE 13

/**
 * \ingroup Client
 * \brief 친구 리스트/블랙 리스트 UI를 구현하는 클래스 
 * \date 2007-12-07
 * \author agebreak
 */
class CFriendListGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
    enum EFriendListMode                ///< UI 리스트 모드
    {
        FRIEND_LIST,
        BLACK_LIST,
    };

public:
	CFriendListGui(void);
	CFriendListGui(const RwChar* pName);
	virtual ~CFriendListGui(void);

	virtual VOID		Init();
	virtual RwBool		Create();
	virtual VOID		Destroy();	
	virtual RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		        HandleEvents(RWS::CMsg &pMsg);

protected:
	// CallBack
    VOID	OnCloseBtnClicked(gui::CComponent* pComponent);
    VOID    OnTitleNameClicked(gui::CComponent* pComponent);
    VOID    OnTitleLevelClicked(gui::CComponent* pComponent);
    VOID    OnTitleLocClicked(gui::CComponent* pComponent);
    VOID    OnTitleChannelClicked(gui::CComponent* pComponent);
    VOID    OnAddBtnClicked(gui::CComponent* pComponent);
    VOID    OnPartyBtnClicked(gui::CComponent* pComponent);
    VOID    OnMailBtnClicked(gui::CComponent* pComponent);
    VOID    OnWhisperClicked(gui::CComponent* pComponent);
    VOID    OnBlockClicked(gui::CComponent* pComponent);
    VOID    OnDeleteClicked(gui::CComponent* pComponent);    
    VOID	OnScrollChanged( RwInt32 nNewOffset );
    VOID	OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex );
    VOID	OnPaint();
    VOID	OnMove(RwInt32 iOldX, RwInt32 iOldY);
    VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
    VOID	OnMouseLeave(gui::CComponent* pComponent);    
    VOID    OnToggled(gui::CComponent* pComponent, bool bToggled);
    VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnClickedBtnHelp(gui::CComponent* pComponent);
	void	OnCaptureMouseDown(const CKey& key);
    
    gui::CButton*	OnCreateItem();
    void			OnDestroyItem(gui::CButton* pButton);
    void			OnClickedItem(RwInt32 iIndex);

    // 수신 패킷 처리 함수
    VOID    OnFriendAddRes(RwUInt32 uiTargetID);
    VOID    OnFriendDelRes(RwUInt32 uiTargetID);
    VOID    OnFriendMoveRes(RwUInt32 uiTargetID);
    VOID    OnFriendListInfo();
    VOID    OnBlackListAddRes(RwUInt32 uiTargetID);
    VOID    OnBlackListDelRes(RwUInt32 uiTargetID);
    VOID    OnFriendInfo(RwUInt32 uiTargetID);
    VOID    OnFriendInfoChange(RwUInt32 uiTargetID);
    VOID    OnFriendLogin(RwUInt32 uiTargetID);

    VOID    CreateInfoStaticBox();                                  ///< 정보를 표시하기위한 스태틱박스들을 생성한다.
    VOID    RefreshList();                                          ///< 현재 보이는 정보들을 갱신한다.    
    VOID    UpdateInfo(RwUInt32 uiCharID = NULL);                   ///< 정보를 업데이트한다.
    RwBool  IsEnableAdd(const WCHAR* pwszName, EFriendListMode eMode);    ///< 친구 or 블랙리스트에 추가가 가능한지 확인한다.
    VOID    OnTargeting(sFriendMember* pMember);                       ///< 커뮤니티 타겟팅을 설정한다.

protected:
    // 컨트롤
    gui::CStaticBox*    m_pSttDialogTitle;            
    gui::CTabButton*    m_pTabMode;
    gui::CButton*       m_pBtnClose;
    gui::CButton*       m_pBtnTitleName;
    gui::CButton*       m_pBtnTitleLevel;
    gui::CButton*       m_pBtnTitleLoc;
    gui::CButton*       m_pBtnTitleChannel;
    gui::CButton*       m_pBtnAdd;
    gui::CButton*       m_pBtnParty;
    gui::CButton*       m_pBtnMail;
    gui::CButton*       m_pBtnWhisper;
    gui::CButton*       m_pBtnBlock;
    gui::CButton*       m_pBtnDelete;
    gui::CScrollBar*    m_pSbScroll;
    CButtonList<CFriendListGui, sCommunityMember*> m_InfoList;
    gui::CStaticBox*    m_pSttLevel[FRIEND_LIST_MAX_VISIBLE];
    gui::CStaticBox*    m_pSttLoc[FRIEND_LIST_MAX_VISIBLE];
    gui::CStaticBox*    m_pSttChannel[FRIEND_LIST_MAX_VISIBLE];    
    gui::CPanel*        m_pPnlBackSpace[FRIEND_LIST_MAX_VISIBLE];
    gui::CPanel*        m_pPnlIcon[FRIEND_LIST_MAX_VISIBLE];

    // 슬롯
    gui::CSlot          m_slotClose;
    gui::CSlot          m_slotTab;
    gui::CSlot          m_slotBtnAdd;
    gui::CSlot          m_slotBtnParty;
    gui::CSlot          m_slotBtnMail;
    gui::CSlot          m_slotBtnWhisper;
    gui::CSlot          m_slotBtnBlock;
    gui::CSlot          m_slotBtnDelete;
    gui::CSlot          m_slotScroll;
    gui::CSlot          m_slotScrollMove;
    gui::CSlot          m_slotTabChange;
    gui::CSlot          m_slotMouseLeave;
    gui::CSlot          m_slotCaptureWheelMove;
    gui::CSlot          m_slotMouseMove[FRIEND_LIST_MAX_VISIBLE];
    gui::CSlot          m_slotBtnMouseLeave[FRIEND_LIST_MAX_VISIBLE];    
    gui::CSlot          m_slotListWheelMove[FRIEND_LIST_MAX_VISIBLE];
    gui::CSlot          m_slotToggled[FRIEND_LIST_MAX_VISIBLE];
    gui::CSlot          m_slotBtnTitleName;
    gui::CSlot          m_slotBtnTitleLevel;
    gui::CSlot          m_slotBtnTitleLoc;
    gui::CSlot          m_slotBtnTitleChannel;

    EFriendListMode     m_eListMode;
    RwUInt32            m_uiCurrentCharID;
    RwInt32             m_nButtonIndex;                     ///< 버튼에 이벤트 슬롯을 달기위한 변수

	// Help Button
	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;

	gui::CSlot			m_slotCaptureMouseDown;
};
