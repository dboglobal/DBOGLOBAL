#pragma once


#include "TSToolDefine.h"


// CTSQuestTextListCtrl 뷰입니다.

class CTSQuestTextListCtrl : public CListCtrl
{
	DECLARE_DYNCREATE(CTSQuestTextListCtrl)

public:
	struct sENTITY
	{
		CString							strTblIdx;
		CString							strQText;
	};

public:
	CTSQuestTextListCtrl();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTSQuestTextListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CArray< sENTITY, const sENTITY& >	m_aStringList;

public:
	void								Init( void );
	void								ClearAll( void );

	void								AddEntity( unsigned int uiTableIndex, const CString& strQText );

public:
};


