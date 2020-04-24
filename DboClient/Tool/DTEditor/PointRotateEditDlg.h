#pragma once
#include "afxwin.h"

#include "NtlMath.h"

class CNtlCamera;

// CPointRotateEditDlg 대화 상자입니다.

class CPointRotateEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointRotateEditDlg)

private:
	
	RpWorld		*m_pWorld;
	RpLight		*m_pAmbient;
	CNtlCamera	*m_pCamera;

	RpClump		*m_pMClump;
	RpClump		*m_pAxisXClump;
	RpClump		*m_pAxisYClump;
	RpClump		*m_pAxisZClump;

	RwRGBA		m_BackgroundColor;
	RwReal		m_fGridScale;
	RwReal		m_fGridIntervalScale;
	RwReal		m_fAxisOffset;
	CNtlPlane	m_planeBox[6];

	RwReal		m_fMinDist;			// minimun distance.
	RwReal		m_fMaxDist;			// maximun distance.	
	RwReal		m_fDist;				// camera 현재 거리.

	RwReal		m_fMinPitch;		// camera min pitch.
	RwReal		m_fMaxPitch;		// camera max pitch.	
	RwReal		m_fPitch;			// camera current pitch.	
	RwReal		m_fYaw;				// camera current yaw.	
	RwReal		m_fRoll;				// camera current roll;

	RwV3d		m_vUp;
	RwMatrix	m_matTrans;

	RwBool		m_bLBtnClick;
	CPoint		m_ptOldLBtnMouse;

	void		CreateWorld(void);
	void		CreateCamera(INT nWidth, INT nHeight);
	RpClump*	CreateClump(const RwChar *filename);
	void		CreateBoxPlane(void);

	void		DestroyWorld(void);
	void		DestroyCamera(void);

	void		ResetCamera(void);

	void		UpdateCamera(void);

	void		AddPitch(RwReal fDeltaPitch);
	void		AddYaw(RwReal fDeltaYaw);
	void		ZoomInOut(RwReal fDelta);

	void		Picking(RwReal fScreenPosX, RwReal fScreenPosY);
	RwInt32		FindRotatePlaneSplitPoint(RwV3d vAt, RwV3d& vSplit);

	// rendering 

	void		AxisRender(void);
	void		GridRender(void);
	void		PlaneRender(void);
	void		PointBoxRender(const RwV3d *pPos, const RwRGBA *pColor, RwMatrix *pWorldMatrix, RwReal fOffset);
	void		SelectPointRender(void);

public:

	void CreateResouce(void);
	void Render(float fElapsed);

public:
	CPointRotateEditDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPointRotateEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_POINTROTATEEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnBnClickedPointrotateeditResetCamera();
public:
	int m_nGridNum;
public:
	afx_msg void OnEnChangePointrotateeditGridNum();
public:
	afx_msg void OnEnKillfocusPointrotateeditGridNum();
public:
	CButton m_btnSnap;
public:
	afx_msg void OnBnClickedPointrotateeditBack();
public:
	afx_msg void OnBnClickedPointrotateeditPointDirection();
public:
	afx_msg void OnBnClickedPointrotateeditInterpolation();
};
