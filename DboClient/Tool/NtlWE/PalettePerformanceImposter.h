#pragma once


// CPalettePerformanceImposter 대화 상자입니다.

class CPalettePerformanceImposter : public CDialog
{
	DECLARE_DYNAMIC(CPalettePerformanceImposter)

public:
	CPalettePerformanceImposter(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPalettePerformanceImposter();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PERFORMANCE_IMPOSTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeWorldData();
};
