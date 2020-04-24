// MrPoPoDoc.h : interface of the CMrPoPoDoc class
//


#pragma once


class CMrPoPoDoc : public CDocument
{
protected: // create from serialization only
	CMrPoPoDoc();
	DECLARE_DYNCREATE(CMrPoPoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CMrPoPoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void SetTitle(LPCTSTR lpszTitle);
};
