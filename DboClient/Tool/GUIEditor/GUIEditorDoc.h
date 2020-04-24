// GUIEditorDoc.h : CGUIEditorDoc 클래스의 인터페이스
//
#pragma once

#include "DataManager.h"

class CGUIEditorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CGUIEditorDoc();
	DECLARE_DYNCREATE(CGUIEditorDoc)

// 특성
public:
	CDataManager&	GetDataManager() { return m_DataManager; }
	
// 작업
public:
	
// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CGUIEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CDataManager	m_DataManager;
	
// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
};

extern CGUIEditorDoc* g_pDocument;

