//-----------------------------------------------------------------------------
// Name: class CDevIL
// Desc: DevIL SDK를 이용하여 이미지를 로드하는 클래스.
//
//			 
// 2006.01.16 Peessi@hitel.net  
//
// TODO: 하나의 이미지만 로드하게 되어 있다. 
//-----------------------------------------------------------------------------

#ifndef	__CDEVIL_H__
#define __CDEVIL_H__

#ifdef _DEBUG
#define IL_DEBUG
#endif

#include <windows.h>

// TODO: 파일위치를 맞춰준다.
#include "DevIL/include/il/il.h"
#include "DevIL/include/il/ilu.h"
#include "DevIL/include/il/ilut.h"

class CDevIL
{
protected:

	ILuint		m_imageIdx;			// 이미지 인덱스. 0이 아니면 로드되어 있다.
	
	// Infomation Variable
	INT			m_nWidth;  
	INT			m_nHeight; 
	INT			m_nBytes;
	INT			m_nBits;  
	INT			m_nFormat; 
	INT			m_nType;   
	INT			m_nFSet;
	INT			m_nFMode;
	INT			m_nTSet;
	INT			m_nTMode;
	INT			m_nOrgMode;

	ILubyte*	m_pPic;				// Image Data.

public:

	CDevIL();
	~CDevIL();

	void Init();
	void Shutdown();

	void ClearMember();
	
	virtual BOOL Load( LPCTSTR szFilename );
	virtual void Unload();

	// Operation
	BOOL	GetPixel( INT x, INT y, BYTE* pRed, BYTE* pGreen, BYTE* pBlue, BYTE* pAlpha );
	
	// query
	BOOL	IsLoaded()				{ return ilIsImage( m_imageIdx ); }
		
	// access
	INT		GetWidth()				{ return m_nWidth; }
	INT		GetHeight()				{ return m_nHeight; }
	INT		GetBytesPerPixel()		{ return m_nBytes; }
	INT		GetBItsPerPixel()		{ return m_nBits; }
	INT		GetFormat()				{ return m_nFormat; }
	INT		GetType()				{ return m_nType; }
	INT		GetFormatSet()			{ return m_nFSet; }
	INT		GetFormatMode()			{ return m_nFMode; }
	INT		GetTypeSet()			{ return m_nTSet; }
	INT		GetTypeMode()			{ return m_nTMode; }
	INT		GetOriginMode()			{ return m_nOrgMode; }

};

#endif // __C_DEVIL_H__