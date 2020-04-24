#pragma once
#include "afxcmn.h"


// COptionDialog 대화 상자입니다.

#include "Resource.h"

#include "MacSliderCtrl.h"
#include "ColorButton.h"
#include "afxwin.h"

class COptionDialog : public CDialog
{
	DECLARE_DYNAMIC(COptionDialog)

public:
	COptionDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COptionDialog();

	static COptionDialog& GetInstance();


	void	BuildGridWidth();
	void	BuildGridCount();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CMacSliderCtrl m_GridWidthSlider;
	CMacSliderCtrl m_GridCountSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	float m_fObjectMoveSpeed;
	float m_fObjectRotateSpeed;
	float m_fObjectScaleSpeed;
	CMacSliderCtrl m_ObjectMoveSpeedSlider;
	CMacSliderCtrl m_ObjectRotateSpeedSlider;
	CMacSliderCtrl m_ObjectScaleSpeedSlider;
	CMacSliderCtrl m_CameraMoveSpeedSlider;
	CMacSliderCtrl m_CameraRotateSpeedSlider;
	CMacSliderCtrl m_CameraWheelSpeedSlider;
	float m_fCameraMoveSpeed;
	float m_fCameraRotateSpeed;
	float m_fCameraWheelSpeed;
	CColorButton m_BackGroundButton;
	afx_msg void OnBnClickedBackground();
	CString m_strGridWidth;
	CString m_strGridCount;
};
