#pragma once


// CPaletteRegion 대화 상자입니다.

class CPaletteRegion : public CDialog
{
	DECLARE_DYNAMIC(CPaletteRegion)

public:
	CPaletteRegion(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteRegion();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_REGION };

public:
	void InitializeWorldData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
