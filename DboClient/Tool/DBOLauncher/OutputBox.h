/*****************************************************************************
*
* File			: OutputBox.h
* Author		:
* Copyright		: (주)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// COutputBox
//
//////////////////////////////////////////////////////////////////////////


class COutputBox : public CStatic
{
public:
	bool				m_bIsLog;

public:
	COutputBox( void );
    virtual ~COutputBox( void );

public:
    void				EraseBack( CDialog* pDlg );		// 글씨영역을 지운다.

protected:
	afx_msg HBRUSH		CtlColor( CDC* pDC, UINT nCtlColor );

	DECLARE_MESSAGE_MAP()
};
