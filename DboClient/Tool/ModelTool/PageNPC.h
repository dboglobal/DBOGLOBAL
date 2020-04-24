#pragma once


// CPageNPC dialog

class CPageNPC : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageNPC)

public:
	CPageNPC();
	virtual ~CPageNPC();

// Dialog Data
	enum { IDD = IDD_PAGENPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
