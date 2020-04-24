#pragma once


// CPageGround dialog

class CPageGround : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageGround)

public:
	CPageGround();
	virtual ~CPageGround();

// Dialog Data
	enum { IDD = IDD_PAGEGROUND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
