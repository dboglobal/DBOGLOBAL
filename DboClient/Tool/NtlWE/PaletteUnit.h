#pragma once


// CPaletteUnit 대화 상자입니다.

class CPaletteUnit : public CDialog
{
	DECLARE_DYNAMIC(CPaletteUnit)

public:
	CPaletteUnit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteUnit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNIT };

public:
	void InitializeWorldData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
