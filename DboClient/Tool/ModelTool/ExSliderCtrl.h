#pragma once

#include <vector>
#include "NtlAnimEventData.h"


/// 슬라이더에 사용되는 마커 구조체
struct SSliderMarker
{
    int          m_nPoint;           ///< 마커가 가르키는 위치
    CRect        m_rt;               ///< 마커의 영역
    SEventAnim*  m_pAnimEvent;       ///< 마커와 매칭될 데이터 
};

typedef std::vector<SSliderMarker> VMarker;

// 메시지 정의
#define EXSM_MARKER_ADD     (WM_USER + 100)
#define EXSM_MARKER_REMOVE  (WM_USER + 101)
#define EXSM_MARKER_SELECT  (WM_USER + 102)
#define EXSM_POS_CHANGED    (WM_USER + 103)
#define EXSM_MARKER_POS_CHANGED (WM_USER + 104)
#define EXSM_MARKER_POS_CHANGE_END (WM_USER + 105)

class CExSliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(CExSliderCtrl)
private:
	/// Bar 높이
	int m_nBarHeight;

	/// Tick 너비
	int m_nTickWidth;

	/// Point 너비
	int m_nPntWidth;

	/// 포인트 높이
	int m_nPntHeight;

	/// 전체 영역
	CRect m_rtWnd;

    CRect m_rtMarkerSpace;          ///< 마커 영역
	/// Bar 영역
	CRect m_rtBar;

	/// 시작 점 영역
	CRect m_rtStart;

	/// 종료 점 영역
	CRect m_rtEnd;

	/// Tick 영역
	CRect m_rtTick;

	/// 시작점 값
	int m_nStartPnt;

	/// 종료점 값
	int m_nEndPnt;

    BOOL m_bClickMarker;            ///< 마커가 클릭되었는지 여부
    int  m_nClickMarkerID;          ///< 클릭된 마커의 ID

	/// 시작 점이 클릭되었는지 여부
	BOOL m_bClickStart;

	/// 종료 점이 클릭되었는지 여부
	BOOL m_bClickEnd;

	/// Tick이 클릭되었는지 여부
	BOOL m_bClickTick;

	/// 설정 점이 클릭되었을 당시 마우스 좌표
	CPoint m_ptClick;	

	/// 툴팁 컨트롤
	CToolTipCtrl m_ctrTip;

	/// 범위 설정을 수행했는지 여부
	BOOL m_bSetRange;

    VMarker m_vMarker;          ///< 마커들의 벡터

public:
	CExSliderCtrl();
	virtual ~CExSliderCtrl();

	/**
	범위를 설정한다. 범위를 설정하게 되면 Start Point와 End Point도
	Min값과 Max값에 따라 재 설정된다.

	* @ param nMin		최소 값
	* @ param nMax		최대 값
	* @ param bRedraw	다시 그리기 여부
	*/
	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);	
	void SetRangeMin(int nMin, BOOL bRedraw = FALSE);	
	void SetRangeMax(int nMax, BOOL bRedraw = FALSE);

	/**
	시작점의 값을 얻어온다. 

	* @ return 시작점 값
	*/
	int GetStartPnt(){return m_nStartPnt;};

	/**
	시작점 값을 설정한다. 값은 최소 값보다 작을 수 없고, 종료 점 값보다 클 수 없다.
	최소 값보다 작은 경우 = 최소 값으로 설정
	종료 값보다 큰   경우 = 종료 값으로 설정

	* @ param nVal 설정 값
	*/
	void SetStartPnt(int nVal);

	/**
	종료점의 값을 얻어온다.

	* @ return 설정 값
	*/
	int GetEndPnt(){return m_nEndPnt;};

    

	/**
	종료 값을 설정한다. 값은 최대 값보다 클 수 없고, 시작점 값보다 작을 수 없다.
	최대 값보다 큰 경우 = 최대 값으로 설정
	시작점 값보다 작은 경우 = 시작점 값으로 설정

	* @ param nVal 설정 값
	*/
	void SetEndPnt(int nVal);

    SSliderMarker* GetSelectMarker();           ///< 현재 선택된 마커를 반환한다.
    void SelectMarker(int nPos);                ///< 마커를 선택한다.
    void AddMarker(int nPos, SEventAnim* pEventAnim = NULL);                   ///< 마커를 추가한다.
    void RemoveMarker(int nPos);                ///< 기존에 있던 마커를 제거한다.
    void ClearMarker();                         ///< 모든 마커를 삭제한다.
    void SetSelectMarkerPos(int nPos);          ///< 선택된 마커의 위치를 변경한다.

protected:
	/**
	슬라이더 컨트롤의 각 구성요소(틱, 바, 설정 포인트 등)의 영역을 계산한다.
	프로그램 중간중간에 이 함수가 자주 불리우는데, 아무래도 개선의 여지가
	필요할듯 싶다. 컨트롤의 사이즈가 변경되지 않는 이상 몇몇 값들은 변경되지 
	않기 때문이다.
	*/
	void CalcLayout();

	/**
	Pos의 값이 Bar 영역의 어떤곳에 위치하는지 알기 위해 사용하는 함수이다.

	* @ param rtBar Bar 영역
	* @ param nPos	Pos 값
	* @ return		Bar 영역의 X좌표(좌표 기준은 전체 클라이언트 영역이다.)
	*/
	int PosToPixel(CRect rtBar, int nPos);

	/**
	PosToPixel의 반대 개념이다. Bar 영역의 X좌표 값을 주면 이를 Pos 값으로 환산한다.

	* @ param rtBar		Bar 영역
	* @ param nPixel	X좌표
	* @ return			Pos 값
	*/
	int PixelToPos(CRect rtBar, int nPixel);

	/**
	오른족 버튼을 눌렀을 경우 팝업 메뉴를 띄우기 위해 사용되는 함수이다.
	Start, End는 마우스로 조작이 가능하지만 좀더 정교한 설정을 위해,
	사용자가 키보드로 입력할 수 있도록 다이얼로그 창을 띄운다.	
	메뉴는 총 3가지다. 전체 범위 설정, 시작 설정, 종료 설정.

	* @ param point 메뉴를 띄울 현재 마우스의 좌표 값
	*/
	void PopupMenu(CPoint point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);    
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
	void OnNeedText(NMHDR* pnmh, LRESULT* pResult);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};