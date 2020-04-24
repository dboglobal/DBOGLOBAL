#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_OperateObject 대화 상자입니다.

class CAttr_ACT_OperateObject : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_OperateObject)

public:
	// 현재 데이터 사용시 true가 시작, 
	// false가 종료로 사용되서 아래와 같이 맞추어 놓았음.
	enum
	{
		eOPERATEOBJECT_TYPE_END,
		eOPERATEOBJECT_TYPE_START
	};

public:
	CAttr_ACT_OperateObject();
	virtual ~CAttr_ACT_OperateObject();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_OPERATEOBJECT_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	CComboBox m_ctrTypeCtrl;
	DWORD m_dwOperateTime;
	DWORD m_dwDirectionTblIdx;
};
