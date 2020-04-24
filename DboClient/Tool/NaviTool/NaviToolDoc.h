// NaviToolDoc.h : CNaviToolDoc 클래스의 인터페이스
//


#pragma once


class CNaviToolDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CNaviToolDoc();
	DECLARE_DYNCREATE(CNaviToolDoc)

// 특성입니다.
public:
	CString m_strPath;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CNaviToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLoadWorldData();
	afx_msg void OnSavePathData();
	afx_msg void OnLoadPathData();
	afx_msg void OnProjectViewTotalMemory();
};


