#pragma once


// CPaletteTrigger 대화 상자입니다.

class CPaletteTrigger : public CDialog
{
	DECLARE_DYNAMIC(CPaletteTrigger)

public:
	CPaletteTrigger(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteTrigger();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TRIGGER };

public:
	void InitializeWorldData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
