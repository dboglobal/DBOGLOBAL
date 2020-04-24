/*****************************************************************************
*
* File			: Dib.h
* Author		:
* Copyright		: (аж)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDib
//
//////////////////////////////////////////////////////////////////////////


class CDib
{
public:
	BYTE			m_byKeyRed;
	BYTE			m_byKeyGreen;
	BYTE			m_byKeyBlue;

	int				m_nWidth;
	int				m_nHeight;

	int				m_nSize;
	int*			m_pData;

public:
	CDib( void );
	~CDib( void );

public:
	bool			Load( const CString& strFileName );
	bool			LoadLLE( const CString& strFileName );
	bool			SaveLLE( const CString& strFileName );
	
	void			UnLoad( void );

	bool			IsLoad( void );
};
