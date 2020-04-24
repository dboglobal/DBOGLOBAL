#pragma once

#include "NtlDebug.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "ButtonList.h"
#include "DragonBallRewardTable.h"
#include "ItemTable.h"
#include "SkillTable.h"
#include "SurfaceGui.h"
#include "DragonBallTable.h"
#include "DragonBallRewardTable.h"

class CTextTable;
class CNtlWorldConceptDBC;

#define REWARD_LIST_MAX_VISIBLE		4

/// 보상 GUI 상태
enum ERewardUIStatus
{
	E_REWARD_STATUS_SELECT,			///< 보상 선택 단계
	E_REWARD_STATUS_CONFIRM,		///< 보상 확인 단계
};


/// 보상 항목들의 트리 구조를 구성하는 노드
struct SRewardItem
{
	sDRAGONBALL_REWARD_TBLDAT* pData;			///< Reward Table Data
	SRewardItem* pParent;
	std::vector<SRewardItem*> itemChild;	///< 자식 아이템들

	SRewardItem()
	{
		pData = NULL;
		pParent = NULL;
	}
	~SRewardItem()
	{
		for each(SRewardItem* pItem in itemChild)
		{
			NTL_DELETE(pItem);
		}
	}		
};


/**
 * \ingroup Client
 * \brief 용신 소환후 소원보상 GUI
 * \date 2007-06-24
 * \author agebreak
 */
class CDBCRewardGUI : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CDBCRewardGUI(void);
	CDBCRewardGUI(const RwChar* pName);
	virtual ~CDBCRewardGUI(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);
	RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

	// 싱글톤 생성/제거 
	static	void	CreateInstance();
	static	void	DeleteInstance();

	VOID			AdjustDialog();							///< 다이얼로그 위치를 정렬

protected:
	// CallBack
	VOID	OnClickBackBtn(gui::CComponent* pComponent);
	VOID	OnClickOkBtn(gui::CComponent* pComponent);
	VOID	OnClickCancelBtn(gui::CComponent* pComponent);
	VOID	OnPaint();
	VOID	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID	OnMouseLeave(gui::CComponent* pComponent);
	VOID	OnScrollBarChanged(RwInt32 iValue);
    VOID	OnCaptureMouseDown(const CKey& key);
    VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );

	
	// 항목 선택 CallBack
	gui::CButton*	OnCreateItem();
	void			OnDestroyItem(gui::CButton* pButton);
	void			OnClickedItem(RwInt32 iIndex);


	VOID			SetStatus(ERewardUIStatus eStatus);		///< 보상창 UI의 현재 상태를 설정한다.
	
	void			ShowList(SRewardItem* pItem, RwInt32 nPos);
	void			CreateRewardTree(SRewardItem* pParent, RwInt32 nDepth);						///< 보상 데이터들을 트리로 구축한다.
	void			ShowItemInfo(SRewardItem* pItem);		///< 보상 아이템 정보를 표시한다.
	void			UnLockDragonBall();						///< Lock을건 드래곤볼들을 모두 UnLock한다.	
	RwBool			IsExistSkill(RwUInt32 nTblIdx);			///< 이미 가지고 있는 스킬인지 확인한다.	

protected:
	static	CDBCRewardGUI*	m_pInstance;			///< 싱글톤 인스턴스 객체

	WCHAR					m_pBuf[20];
	RwBool					m_bDisplayHurryUp;		///< 시간이 없다라는 메시지를 표시했는지 유무 플래그
	RwBool					m_bDelete;				///< 삭제할지 여부 플래그
	DWORD					m_nDepthIdx;			///< 선택 항목 깊이의 인덱스 값	
	DWORD					m_nPrevDepthIdx;		///< 이전 항목 깊이의 인덱스 값
    RwInt32                 m_nBtnCount;            ///< 버튼 생성 갯수
	
	CTextTable*				m_pDBRewardTextTable;	///< 보상 관련 텍스트 테이블	
	CDragonBallTable*		m_pDragonBallTable;		///< 드래곤볼 테이블
	CDragonBallRewardTable* m_pDragonBallRewardTable; ///< 보상 테이블

	SRewardItem				m_RewardItemRoot;		///< 보상 아이템 트리의 루트 노드
	SRewardItem*			m_pPrevItem;			///< 한단계전의 트리 노드
	SRewardItem*			m_pRewardItem;			///< 최종 보상 아이템
	sTBLDAT*				m_pRewardItemTblData;	///< 보상 아이템의 테이블 데이터		

	CNtlWorldConceptDBC*	m_pWorldConceptDBC;		///< DBC Concept 컨트롤러


	// gui
    gui::CPanel*            m_pPnlSelectMenu;
    gui::CPanel*            m_pPnlSelectItem;
	gui::CButton*			m_pBtnBack;
	gui::CButton*			m_pBtnOK;
	gui::CButton*			m_pBtnCancel;	
	gui::CStaticBox*		m_pSttTime;
	gui::CStaticBox*		m_pSttItemName;
	gui::CStaticBox*		m_pSttItemText;	
	gui::CScrollBar*		m_pScrollBar;
	CSurfaceGui				m_sufItem;				///< 아이템의 Icon을 표시할 Surface

	CButtonList<CDBCRewardGUI, SRewardItem*>	m_ButtonList;	///< 항목 버튼 리슽

	// slot
	gui::CSlot				m_slotClickExitBtn;
	gui::CSlot				m_slotClickBackBtn;
	gui::CSlot				m_slotClickOKBtn;
	gui::CSlot				m_slotClickCancelBtn;
	gui::CSlot				m_slotPaint;
	gui::CSlot				m_slotMove;
	gui::CSlot				m_slotMouseMove;
	gui::CSlot				m_slotMouseLeave;
	gui::CSlot				m_slotSliderMove;
	gui::CSlot				m_slotSliderChanged;
    gui::CSlot  			m_slotCaptureMouseDown;
    gui::CSlot              m_slotListWheelMove[REWARD_LIST_MAX_VISIBLE];
};	
