#pragma once


// CPropTreeInfo

class CPropTree;

class CPropTreeInfo : public CStatic
{
	DECLARE_DYNAMIC(CPropTreeInfo)

protected:
	CPropTree*		m_pProp;

public:
	CPropTreeInfo();
	virtual ~CPropTreeInfo();

// Attributes
public:
	// CPropTree class that this class belongs
	void SetPropOwner(CPropTree* pProp);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


