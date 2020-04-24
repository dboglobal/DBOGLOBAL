#pragma once


// CSaberView2 대화 상자입니다.

class CSaberView2 : public CDialog
{
	DECLARE_DYNAMIC(CSaberView2)

public:
	CSaberView2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSaberView2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SABER_VIEW2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
