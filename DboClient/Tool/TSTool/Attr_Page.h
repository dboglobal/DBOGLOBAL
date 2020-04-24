#pragma once


#include "TSToolDefine.h"
#include "OptionPage.h"


// CAttr_Page 대화 상자입니다.

class CAttr_Page : public COptionPage
{
	DECLARE_DYNAMIC(CAttr_Page)

// Member variables
protected:
	CString			m_strData;

// Constructions and Destructions
public:
	CAttr_Page(unsigned int uiIDD);
	virtual ~CAttr_Page();

// Methods
public:
	const CString&	GetAttr( void ) const;
	CString			GetAttr( CString strName );
	void			SetAttr( CString& strData );
	void			AddAttr( CString strData );
	void			AddAttr( CString strKey, CString strValue );
	void			AddAttr( CString strKey, int nValue );
	void			ClearAttr( void );

	void			PakingPageData( CString& strResult, CString strKey, CString strValue );

// Implementations
protected:
	virtual CString	GetPageData( void );
	void			SetPageData( CString& strData );

	CString			PakingPageData( CString strKey, CString strValue );
	CString			PakingPageData( CString strKey, DWORD dwValue );
	CString			PakingPageData( CString strKey, int nValue );
	CString			PakingPageData( CString strKey, unsigned int uiValue );
	CString			PakingPageData( CString strKey, float fValue );

	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK();
};

inline const CString& CAttr_Page::GetAttr( void ) const
{
	return m_strData;
}

inline void CAttr_Page::SetAttr( CString& strData )
{
	m_strData = strData;
}
