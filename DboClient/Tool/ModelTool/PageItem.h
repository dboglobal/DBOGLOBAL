#pragma once


// CPageItem dialog

class CPageItem : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageItem)

public:
	CPageItem();
	virtual ~CPageItem();

// Dialog Data
	enum { IDD = IDD_PAGEITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
