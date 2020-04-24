#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlDragonBall.h"

class CNtlSobItem;
class CNtlWorldConceptDBC;

/// 드래곤볼 타입
enum EDragonBallType
{
	E_DRAGONBALL_NONE,
	E_DRAGONBALL_1,
	E_DRAGONBALL_2,
	E_DRAGONBALL_3,
	E_DRAGONBALL_4,
	E_DRAGONBALL_5,
	E_DRAGONBALL_6,
	E_DRAGONBALL_7,
};

/// 드래곤볼 슬롯 구조체
struct DragonBallSlot
{
	CRectangle		 m_rt;		///< 드래곤볼 슬롯의 영역
	CSurfaceGui		 m_surface;	///< 드래곤볼 슬롯의 서페이스 (텍스쳐 설정에 필요)
	EDragonBallType	 m_eDBType;	///< 슬롯에 있는 드래곤볼 타입
	CNtlSobItem*	 m_pSobItem; ///< 슬롯에 있는 아이템의 포인터

	DragonBallSlot()
	{
		m_eDBType = E_DRAGONBALL_NONE;
		m_pSobItem = NULL;
	}
};


/**
 * \ingroup Client
 * \brief 드래곤볼 용신 소환 제단 클릭시 나오는 GUI
 * 드래곤볼 7개를 세팅하여 용신을 소환하기 위해서 사용한다.
 * \date 2007-06-20
 * \author agebreak
 */
class CAltarGui	: public CNtlPLGui, public RWS::CEventHandler
{
public:
	CAltarGui(void);
	CAltarGui(const RwChar* pName);
	virtual ~CAltarGui(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

	// Singleton create / remove function
// Almost no GUI is generated, so you only need to create it.
	static void		  CreateInstance();
	static void		  DeleteInstance();

protected:
	// CallBack
	VOID	OnClickExitBtn(gui::CComponent* pComponent);
	VOID	OnClickOKBtn(gui::CComponent* pComponent);
	VOID	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID	OnMouseDown(const CKey& key);
	VOID	OnMouseUp(const CKey& key);
	VOID	OnPaint();
	VOID	OnInputBoxGotFocus();
	VOID	OnInputBoxLostFocus();
    VOID	OnCaptureMouseDown(const CKey& key);


	RwInt32	GetChildSlotIdx(RwInt32 nX, RwInt32 nY);	///< 마우스를 클릭할때 어떤 Slot에 해당하는지 계산한다.

	VOID	SetDragonBallSlot(RwInt32 nSlotIdx, EDragonBallType eDragonBallType);	///< 드래곤볼을 slot에 세팅한다.
	VOID	RemoveDragonBallSlot(RwInt32 nSlotIdx);									///< 세팅한 드래곤볼을 slot으로부터 해제한다.
    RwBool  IsExistSameType(EDragonBallType eType);                                 ///< 같은 타입(번호)의 드래곤볼이 이미 놓여있는지 체크한다.

	VOID	SpawnDragon();								///< 용신 소환 연출을 실행한다.

	

protected:
	static CAltarGui*	m_pInstance;			///< 싱글톤 인스턴스 객체	

	gui::CButton*		m_pBtnExit;				///< Exit 버튼
	gui::CButton*		m_pBtnOK;				///< OK 버튼
	gui::CInputBox*		m_pInKeyword;			///< 주문 인풋 박스
	gui::CPanel*		m_pPnlInputBack;		///< 인풋 박스 배경
	gui::CPanel*		m_apPnlLight[7];		///< 드래곤볼이 삽입되면 변경될 배경
	gui::CStaticBox*	m_pStaticKeyword;		///< 주문 인풋 박스의 앞의 텍스트

	gui::CSlot			m_slotClickExitBtn;	
	gui::CSlot			m_slotClickOKBtn;	
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseDown;	
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;
    gui::CSlot			m_slotCaptureMouseDown;

    DragonBallSlot      m_DBSlot[DRAGON_BALL_TYPE_COUNT][NTL_ITEM_MAX_DRAGONBALL];          ///< 드래곤볼 아이콘을 표시할 슬롯
	RwInt32				m_nDBCount;				///< 현재 장착되어 있는 드래곤볼 Count
	RwBool				m_bSpawnDragon;			///< 용신을 소환했는지 여부
	RwBool				m_bDelete;

	CNtlWorldConceptDBC*	m_pWorldConceptDBC;	///< 드래곤볼 컬렉션 컨트롤러의 포인터

};
