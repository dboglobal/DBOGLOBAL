#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ExSliderCtrl.h"


// CAnimToolView form view

class CAnimToolView : public CFormView
{
	DECLARE_DYNCREATE(CAnimToolView)

protected:
	CAnimToolView();           // protected constructor used by dynamic creation
	virtual ~CAnimToolView();

public:
	enum { IDD = IDD_ANIMTOOLVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CAnimToolView* GetInstance();                                ///< 싱글톤 반환함수

    void    SetInit(CMTCharacter* pCharacter,STypeAnimData* pAnimData);
    void    SetAnimTime(RwReal fCurrentTime, RwReal fTotalTime);       ///< 애니메이션의 시간을 설정한다.
    void    SetEnable(BOOL bEnable);                                   ///< 활성화 유무를 설정한다.
    void    Update();                                                  ///< 플레이 타임을 업데이트 한다.
    void    SetMarkerPos(SEventAnim* pEventAnim);                      ///< 마커의 위치를 변경한다.
    void    ChangeAnimPlay();                                          ///< 애니메이션 플레이 상태를 변경한다.
   
protected:
    void    InitMarker();                                              ///< 슬라이드 뷰의 마커표시를 설정한다.
    RwReal  PosToTime(int nPos);                                       ///< 위치값을 시간값으로 변경한다.
    int     TimeToPos(RwReal fTime);                                   ///< Time값을 Pos위치로 변환한다.

protected:
    static CAnimToolView* m_pInstance;
    
    CMTCharacter*       m_pCharacter;                                   ///< 애니메이션 시간 업데이트시에 사용할 캐릭터 객체
    STypeAnimData*      m_pAnimData;                                    ///< Anim Data 포인터
    BOOL                m_bUpdate;                                      ///< Update 유무

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
    CString m_sAnimTime;
    CExSliderCtrl m_slideAnimTime;
    CButton m_btAnimPlay;
    CButton m_btAnimStop;
    CEdit m_edAnimSpeed;
public:
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton1();    
    afx_msg void OnSlidePosChanged();    
    afx_msg void OnSlideMarkerSelect();
    afx_msg void OnSlideMarkerMove();
    afx_msg void OnSlideMarkerChanged();
    afx_msg void OnSlideMarkerRemove(SSliderMarker* pRemovedMarker);

protected:
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

public:
    afx_msg void OnDeltaposSpinAnimSpeed(NMHDR *pNMHDR, LRESULT *pResult);
protected:
    CSpinButtonCtrl m_spinAnimSpeed;
public:
    afx_msg void OnEnChangeEdit2();
};


