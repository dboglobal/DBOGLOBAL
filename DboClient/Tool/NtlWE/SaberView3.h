#pragma once


// CSaberView3 대화 상자입니다.

class CSaberView3 : public CDialog
{
	DECLARE_DYNAMIC(CSaberView3)

public:
	CSaberView3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSaberView3();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SABER_VIEW3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
