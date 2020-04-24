#pragma once


// PointShapeDlg 대화 상자입니다.

class CPointShapeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPointShapeDlg)

public:
	CPointShapeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPointShapeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_POINTSHAPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
