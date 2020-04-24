#pragma once



#include "ExSliderCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

// CAnimPlayPane form view
enum EAnimPlayPaneState
{
	ANIMPLAYPANE_CHARACTER,
	ANIMPLAYPANE_OBJECT,
};

class CAnimPlayPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CAnimPlayPane)

protected:
	CAnimPlayPane();           // protected constructor used by dynamic creation
	virtual ~CAnimPlayPane();

public:
	enum { IDD = IDD_ANIMPLAYPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CAnimPlayPane* GetInstance() {return m_pIntance;};

    void    SetAnimData(CMTCharacter* pCharacter, STypeAnimData* pAnimData);	
	void	SetTriggerObjectAnimData(CMTObject* pObject, STypeAnimData* pAnimData);
    void    SetAnimTime(RwReal fCurrentTime, RwReal fTotalTime);       ///< 애니메이션의 시간을 설정한다.
    void    Update();                                                  ///< 플레이 타임을 업데이트 한다.
    void    SetMarkerPos(SEventAnim* pEventAnim);                      ///< 마커의 위치를 변경한다.    
    void    ChangeAnimPlay();                                          ///< 현재 애니메이션 플레이상태를 토글한다.

protected:
    void    SetEnable(BOOL bEnable);                                   ///< 활성화 유무를 설정한다.
    void    InitMarker();                                              ///< 슬라이드 뷰의 마커표시를 설정한다.
    RwReal  PosToTime(int nPos);					                   ///< 위치값을 시간값으로 변경한다.
    int     TimeToPos(RwReal fTime);                                   ///< Time값을 Pos위치로 변환한다.    
	void	UpdateEventComboBox();									   ///< Event Comobo Box Setup
    RwBool  CheckSkillCancel(RwReal fTime);                            ///< Skill Cancel 이벤트를 추가할수 있는지 확인한다.

protected:
    static CAnimPlayPane* m_pIntance;

    CMTCharacter*       m_pCharacter;                                   ///< 애니메이션 시간 업데이트시에 사용할 캐릭터 객체
	CMTObject*			m_pObject;										///< 오브젝트 애니메이션시 사용할 오브젝트 객체
    STypeAnimData*      m_pAnimData;                                    ///< Anim Data 포인터
    SEventAnim*         m_pEventHit;                                    ///< 현재 Hit 표시를 하고 있는 이벤트 객체
    SEventAnim*         m_pEventSound;                                  ///< 현재 Sound를 플레이하고 있는 이벤트 객체
    SEventAnim*         m_pEventClipBoard;                               ///< copy&paste용 event

	EAnimPlayPaneState	m_eAnimPlayPaneState;							///< 현재 상태

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
public:
    CExSliderCtrl m_slideAnimTime;
    CButton m_btAnimPlay;
    CButton m_btAnimStop;
    CEdit m_edAnimSpeed;
    CSpinButtonCtrl m_spinAnimSpeed;
    CEdit m_edAnimTime;
    CComboBox m_cbEvent;

public:
    afx_msg void OnBnClickedBtAnimPlay();
    afx_msg void OnBnClickedBtAnimStop();
    afx_msg void OnSlidePosChanged();    
    afx_msg void OnSlideMarkerAdd(SSliderMarker* pSliderMarker);
    afx_msg void OnSlideMarkerSelect();
    afx_msg void OnSlideMarkerMove();
    afx_msg void OnSlideMarkerChanged();
    afx_msg void OnSlideMarkerRemove(SSliderMarker* pRemovedMarker);
    afx_msg void OnSlideMarkerCopy();
    afx_msg void OnSlideMarkerPaste();

protected:
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    afx_msg void OnDeltaposSpinAnimSpeed(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEdSpeed();

public:
    afx_msg void OnCbnSelchangeCbEvent();
};


