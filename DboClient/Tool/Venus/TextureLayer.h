#pragma once


// CTextureLayer 뷰입니다.
#include "rwcore.h"

#include "RollupCtrl.h"

class CTextureLayer : public CView
{
	DECLARE_DYNCREATE(CTextureLayer)

protected:
	CTextureLayer();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTextureLayer();

public:
	void	CreateBitmap(const RwChar* strPathName);

	void	BuildTextureList();

public:
	enum EDIT_DIALOG
	{
		EDIT_DIALOG_TEXTURE_LIST = 0x00,
		EDIT_DIALOG_TEXTURE_INFO,
	};

	BOOL					m_bInitialized;
	CRollupCtrl				m_RollupControl;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


