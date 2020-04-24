#pragma once


#include "Attr_Page.h"


// CAttr_COND_OperateObject 대화 상자입니다.

class CAttr_COND_OperateObject : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_COND_OperateObject)

public:
	CAttr_COND_OperateObject();
	virtual ~CAttr_COND_OperateObject();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_COND_OPERATEOBJECT_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
};
