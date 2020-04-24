// 2DParticleEditorDoc.h : interface of the CMy2DParticleEditorDoc class
//

// Gui Library

#pragma once


class CMy2DParticleEditorDoc : public CDocument
{
protected: // create from serialization only
	CMy2DParticleEditorDoc();
	DECLARE_DYNCREATE(CMy2DParticleEditorDoc)

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
	virtual ~CMy2DParticleEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void LoadAttr( LPCTSTR lpszPathName );

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

extern CMy2DParticleEditorDoc* g_Doc;
