#include "gui_precomp.h"
#include "gui_inputbox_generic.h"
#include "gui_componentoption.h"
#include "gui_texture.h"
#include "gui_texturemanager.h"
#include "gui_font.h"
#include "gui_fontmanager.h"
#include "gui_renderer.h"
#include "position.h"
#include <ctype.h>

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
#include "GUISoundDefine.h"
#endif

START_GUI

#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor )      ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )   // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )   // New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )   // New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )   // New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )   // New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )   // New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )   // New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41 ( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )   // MSPY1.5  // SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42 ( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )   // MSPY2    // Win2k/WinME
#define IMEID_CHS_VER53 ( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )   // MSPY3    // WinXP

// Function pointers
INPUTCONTEXT* (WINAPI * CInputBox_Generic::_ImmLockIMC)( HIMC ) = CInputBox_Generic::Dummy_ImmLockIMC;
BOOL (WINAPI * CInputBox_Generic::_ImmUnlockIMC)( HIMC ) = CInputBox_Generic::Dummy_ImmUnlockIMC;
LPVOID (WINAPI * CInputBox_Generic::_ImmLockIMCC)( HIMCC ) = CInputBox_Generic::Dummy_ImmLockIMCC;
BOOL (WINAPI * CInputBox_Generic::_ImmUnlockIMCC)( HIMCC ) = CInputBox_Generic::Dummy_ImmUnlockIMCC;
BOOL (WINAPI * CInputBox_Generic::_ImmDisableTextFrameService)( DWORD ) = CInputBox_Generic::Dummy_ImmDisableTextFrameService;
LONG (WINAPI * CInputBox_Generic::_ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD ) = CInputBox_Generic::Dummy_ImmGetCompositionStringW;
DWORD (WINAPI * CInputBox_Generic::_ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD ) = CInputBox_Generic::Dummy_ImmGetCandidateListW;
HIMC (WINAPI * CInputBox_Generic::_ImmGetContext)( HWND ) = CInputBox_Generic::Dummy_ImmGetContext;
BOOL (WINAPI * CInputBox_Generic::_ImmReleaseContext)( HWND, HIMC ) = CInputBox_Generic::Dummy_ImmReleaseContext;
HIMC (WINAPI * CInputBox_Generic::_ImmAssociateContext)( HWND, HIMC ) = CInputBox_Generic::Dummy_ImmAssociateContext;
BOOL (WINAPI * CInputBox_Generic::_ImmGetOpenStatus)( HIMC ) = CInputBox_Generic::Dummy_ImmGetOpenStatus;
BOOL (WINAPI * CInputBox_Generic::_ImmSetOpenStatus)( HIMC, BOOL ) = CInputBox_Generic::Dummy_ImmSetOpenStatus;
BOOL (WINAPI * CInputBox_Generic::_ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD ) = CInputBox_Generic::Dummy_ImmGetConversionStatus;
HWND (WINAPI * CInputBox_Generic::_ImmGetDefaultIMEWnd)( HWND ) = CInputBox_Generic::Dummy_ImmGetDefaultIMEWnd;
UINT (WINAPI * CInputBox_Generic::_ImmGetIMEFileNameA)( HKL, LPSTR, UINT ) = CInputBox_Generic::Dummy_ImmGetIMEFileNameA;
UINT (WINAPI * CInputBox_Generic::_ImmGetVirtualKey)( HWND ) = CInputBox_Generic::Dummy_ImmGetVirtualKey;
BOOL (WINAPI * CInputBox_Generic::_ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD ) = CInputBox_Generic::Dummy_ImmNotifyIME;
BOOL (WINAPI * CInputBox_Generic::_ImmSetConversionStatus)( HIMC, DWORD, DWORD ) = CInputBox_Generic::Dummy_ImmSetConversionStatus;
BOOL (WINAPI * CInputBox_Generic::_ImmSimulateHotKey)( HWND, DWORD ) = CInputBox_Generic::Dummy_ImmSimulateHotKey;
BOOL (WINAPI * CInputBox_Generic::_ImmIsIME)( HKL ) = CInputBox_Generic::Dummy_ImmIsIME;
UINT (WINAPI * CInputBox_Generic::_GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) = CInputBox_Generic::Dummy_GetReadingString; // Traditional Chinese IME
BOOL (WINAPI * CInputBox_Generic::_ShowReadingWindow)( HIMC, BOOL ) = CInputBox_Generic::Dummy_ShowReadingWindow; // Traditional Chinese IME
BOOL (APIENTRY * CInputBox_Generic::_VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT ) = CInputBox_Generic::Dummy_VerQueryValueA;
BOOL (APIENTRY * CInputBox_Generic::_GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID )= CInputBox_Generic::Dummy_GetFileVersionInfoA;
DWORD (APIENTRY * CInputBox_Generic::_GetFileVersionInfoSizeA)( LPSTR, LPDWORD ) = CInputBox_Generic::Dummy_GetFileVersionInfoSizeA;


HWND		CInputBox_Generic::m_hWindow;

HINSTANCE	CInputBox_Generic::m_hDllImm32;						// IMM32 DLL handle
HINSTANCE	CInputBox_Generic::m_hDllVer;						// Version DLL handle
HIMC		CInputBox_Generic::m_hImcDef;						// Default input context

HKL			CInputBox_Generic::m_hklCurrent;					// Current keyboard layout of the process  
HINSTANCE	CInputBox_Generic::m_hDllIme;						// Instance handle of the current IME module

IMESTATE	CInputBox_Generic::m_ImeState = IMEUI_STATE_OFF;	// IME global state
IMESTATE	CInputBox_Generic::m_SavedImeState = IMEUI_STATE_OFF;
BOOL		CInputBox_Generic::m_bSaveImeState = FALSE;
	
LPWSTR							CInputBox_Generic::m_wszCurrIndicator = CInputBox_Generic::m_aszIndicator[0];		// Points to an indicator string that corresponds to current input locale
WCHAR							CInputBox_Generic::m_aszIndicator[5][3];											// String to draw to indicate current input locale
bool							CInputBox_Generic::m_bVerticalCand;													// Indicates that the candidates are listed vertically
bool							CInputBox_Generic::m_bEnableImeSystem;
CUniBuffer						CInputBox_Generic::m_CompString = CUniBuffer( 0 );
CPos							CInputBox_Generic::m_ptCompString;													// Composition string position. Updated every frame.
BYTE							CInputBox_Generic::m_abCompStringAttr[MAX_COMPSTRING_SIZE];
DWORD							CInputBox_Generic::m_adwCompStringClause[MAX_COMPSTRING_SIZE];
WCHAR							CInputBox_Generic::m_wszReadingString[32];											// Used only with horizontal reading window (why?)

int								CInputBox_Generic::m_nCompCaret;
bool							CInputBox_Generic::m_bInsertOnType = false;				// Insert the character as soon as a key is pressed (Korean behavior)
CInputBox_Generic::SCandList	CInputBox_Generic::m_sCandList;					// Data relevant to the candidate list
bool							CInputBox_Generic::m_bShowReadingWindow;		// Indicates whether reading window is visible
bool							CInputBox_Generic::m_bHorizontalReading;		// Indicates whether the reading window is vertical or horizontal
bool							CInputBox_Generic::m_bChineseIME;
bool							CInputBox_Generic::m_bHideCaret;

std::vector< CInputBox_Generic::SInputLocale >		CInputBox_Generic::m_Locale;	// Array of loaded keyboard layout on system

CSignal_v1<INT>					CInputBox_Generic::m_SigIMEChanged;

/////////////////////////////////////////////////////////////////////////////

#define INPUTBOX_ITEM_GROW_SIZE		50

CInputBoxLineInfo::CInputBoxLineInfo()
{
	m_pLineItem = NULL;
	m_rtClientArea.SetRect(0, 0, 0, 0);
	m_rtValidArea.SetRect(0, 0, 0, 0);

	m_nLineNum	= INPUTBOX_ITEM_GROW_SIZE;
	m_pLineItem = NTL_NEW SLineItem[INPUTBOX_ITEM_GROW_SIZE];

	Clear();
}

CInputBoxLineInfo::~CInputBoxLineInfo()
{
	NTL_ARRAY_DELETE( m_pLineItem );
}

void CInputBoxLineInfo::AddItem(INT nStringIdx, INT nStringLen, CRectangle rtArea, INT nLineIdx, BOOL bCompString, BOOL bNewLine)
{
	if(CheckGrowItem())
		GrowItem();

	m_rtValidArea.bottom = rtArea.bottom;

	m_pLineItem[m_nActiveNum].bNewLine		= bNewLine;
	m_pLineItem[m_nActiveNum].bCompString	= bCompString;
	m_pLineItem[m_nActiveNum].nStringIdx	= nStringIdx;
	m_pLineItem[m_nActiveNum].nStringLen	= nStringLen;
	m_pLineItem[m_nActiveNum].nLineIdx		= nLineIdx;
	m_pLineItem[m_nActiveNum].rtArea		= rtArea;

	m_nActiveNum++;
}

BOOL CInputBoxLineInfo::CheckGrowItem(void)
{
	if(m_nActiveNum >= m_nLineNum)
		return TRUE;

	return FALSE;
}

void CInputBoxLineInfo::GrowItem(void)
{
	INT nNum = m_nLineNum + INPUTBOX_ITEM_GROW_SIZE;
	SLineItem *pNewLineItem = NTL_NEW SLineItem[nNum];

	for(INT i = 0; i < m_nLineNum; i++)
	{
		pNewLineItem[i].nStringIdx	= m_pLineItem[i].nStringIdx;
		pNewLineItem[i].rtArea		= m_pLineItem[i].rtArea;
	}

	m_nLineNum = nNum;

	NTL_ARRAY_DELETE( m_pLineItem );
	m_pLineItem = pNewLineItem;
}

void CInputBoxLineInfo::Clear(void)
{
	m_nActiveNum	= 0;
	m_nCurrHeight	= 0;
}

INT	CInputBoxLineInfo::FindLineIndex(INT nCaret)
{
	INT nLineIdx = 0;
	if(m_nActiveNum <= 0)
		return nLineIdx;

	for(INT i = 0; i < m_nActiveNum; i++)
	{
		if(!m_pLineItem[i].bCompString)
		{
			if(nCaret >= m_pLineItem[i].nStringIdx && nCaret < m_pLineItem[i].nStringIdx + m_pLineItem[i].nStringLen)
				return m_pLineItem[i].nLineIdx;

			nLineIdx = m_pLineItem[i].nLineIdx;
		}
	}

	return nLineIdx;
}

void CInputBoxLineInfo::SetClientArea(INT nWidth, INT nHeight)
{
	m_rtClientArea.SetRect(0, 0, nWidth, nHeight);
	m_rtValidArea.SetRect(0, 0, nWidth, 0);
}

SCaretMoveInfo CInputBoxLineInfo::FindStringIndex(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo, BOOL bMouseClick /*= TRUE*/)
{
	SCaretMoveInfo sCaretInfo = { 0, 0, 0 , 0};

	if(m_nActiveNum <= 0)
		return sCaretInfo;

	INT nLastIndex	= 0;
	INT nLastLen	= 0;
	INT nLastLine	= 0;
	SLineItem *pLastItem = NULL;
	
	INT nSumSize = 0;
	INT nSize;

	CRectangle rtNewLineArea;
	
	for(INT i = 0; i < m_nActiveNum; i++)
	{
		if(!m_pLineItem[i].bCompString)
		{
			if(m_pLineItem[i].rtArea.PtInRect(sCurrCaretInfo.nCaretPosX, sCurrCaretInfo.nCaretPosY))
			{
				for(int j = m_pLineItem[i].nStringIdx; j < m_pLineItem[i].nStringIdx + m_pLineItem[i].nStringLen; j++)
				{
					nSize = pString->GetItemSize(j);
					
					if(sCurrCaretInfo.nCaretPosX < m_pLineItem[i].rtArea.left + nSumSize + nSize)
					{
						if(i-1 >= 0 && m_pLineItem[i-1].bNewLine)
						{
							sCaretInfo.nCaretIdx = j;
						}
						else
						{
							if(sCurrCaretInfo.nCaretPosX < m_pLineItem[i].rtArea.left + nSumSize + nSize/2)
								sCaretInfo.nCaretIdx = j-1;
							else
								sCaretInfo.nCaretIdx = j;
						}

						if(sCaretInfo.nCaretIdx < 0)
							sCaretInfo.nCaretIdx = 0;

						sCaretInfo.nCaretLine = m_pLineItem[i].nLineIdx;
						
						return sCaretInfo;
					}

					nSumSize += nSize;
				}

				sCaretInfo.nCaretIdx	= m_pLineItem[i].nStringIdx + m_pLineItem[i].nStringLen;
				sCaretInfo.nCaretLine	= m_pLineItem[i].nLineIdx;
				return sCaretInfo;
			}
			else
			{
				nLastIndex	= m_pLineItem[i].nStringIdx;
				nLastLen	= m_pLineItem[i].nStringLen;
				nLastLine	= m_pLineItem[i].nLineIdx;
				pLastItem	= &m_pLineItem[i];
			}
		}
	}

	if(bMouseClick)
	{
		sCaretInfo.nCaretIdx	= nLastIndex + nLastLen;
		sCaretInfo.nCaretLine	= nLastLine;
	}
	else
	{
		if(sCurrCaretInfo.nCaretPosY <= 0 || sCurrCaretInfo.nCaretLine >= nLastLine)
		{
			sCaretInfo.nCaretIdx	= sCurrCaretInfo.nCaretIdx;
			sCaretInfo.nCaretLine	= sCurrCaretInfo.nCaretLine;
		}
		else
		{
			sCaretInfo.nCaretIdx	= nLastIndex + nLastLen;
			sCaretInfo.nCaretLine	= nLastLine;
		}
	}

	return sCaretInfo;
}

SCaretMoveInfo CInputBoxLineInfo::SetString(CUniBuffer *pString, CUniBuffer *pCompString, INT nCaret, INT nLineInterval, INT nTextHeight)
{
	Clear();

	SCaretMoveInfo sNewCaret = { 0, 0, 0 , 0};

	m_rtValidArea.SetRect(0, 0, m_rtClientArea.right, 0);

	if(pString == NULL)
		return sNewCaret;

	int nLen		= pString->GetTextSize(); 
	int nCompLen	= pCompString->GetTextSize();
	if(nLen <= 0 && nCompLen <= 0)
		return sNewCaret;

	int nLeft = 0, nTop = 0;
	int nPrevPosX = 0, nPosX = 0;
	int nLineOffsetX = 0;
	int nStringIdx = 0;
	int nLineIdx = 0;
	int nSize;

	CRectangle rtArea;

	// caret before
	if(nLen > 0)
	{
		for(int i = 0; i <= nCaret; i++)
		{
			nSize = pString->GetItemSize(i);

			if(i > 0)
			{
				if((*pString)[i-1] == L'\\')
				{
					if(i < nLen && (*pString)[i] == L'n')
					{
						rtArea.SetRect(nLeft, nTop, m_rtClientArea.GetWidth(), nTop + nTextHeight + nLineInterval);
					
						AddItem(nStringIdx, i - nStringIdx - 1, rtArea, nLineIdx, FALSE, FALSE);
						
						nLineOffsetX	= 0;
						nTop			+= nLineInterval + nTextHeight;

						nStringIdx		= i-1;
						nLineIdx++;		

						// new line
						nLeft			= 0;

						rtArea.SetRect(nLeft, nTop, 0, nTop + nTextHeight + nLineInterval);
						AddItem(nStringIdx, 2, rtArea, nLineIdx, FALSE, TRUE);
						
						nStringIdx		+= 2;
						nLineIdx++;		

						i++;
						nLineOffsetX	= 0;

						continue;
					}
				}
			}
		
			if(nLineOffsetX + nSize > m_rtClientArea.GetWidth())
			{
				rtArea.SetRect(nLeft, nTop, m_rtClientArea.GetWidth(), nTop + nTextHeight + nLineInterval);
				
				AddItem(nStringIdx, i - nStringIdx - 1, rtArea, nLineIdx, FALSE, FALSE);
				
				nLineOffsetX	= 0;
				nTop			+= nLineInterval + nTextHeight;

				nStringIdx		= i-1;
				nLineIdx++;		

			}

			nLineOffsetX	+= nSize;
		}
	}

	// caret 위치...
	
	sNewCaret.nCaretPosX = nLineOffsetX;
	sNewCaret.nCaretPosY = nTop;

	if(nCaret != nLen)
	{
		if( !( nCaret + 2 <= nLen && (*pString)[nCaret] == L'\\' && (*pString)[nCaret+1] == L'n') )
		{
			nSize = pString->GetItemSize(nCaret + 1);

			if(nLineOffsetX + nSize > m_rtClientArea.GetWidth())
			{
				sNewCaret.nCaretPosX = 0;
				sNewCaret.nCaretPosY = nTop + nLineInterval + nTextHeight;
			}
		}
	}

	if(nLineOffsetX > 0)
	{
		rtArea.SetRect(nLeft, nTop, nLineOffsetX, nTop + nTextHeight + nLineInterval);
		AddItem(nStringIdx, nCaret - nStringIdx, rtArea, nLineIdx, FALSE, FALSE);

		nLeft = nLineOffsetX;
	}

	sNewCaret.nCaretLine = nLineIdx;

	// composition 

	if(nCompLen > 0)
	{
		nLineOffsetX	= nLeft;
		nPrevPosX		= 0;
		nStringIdx		= 0;

		for(int i = 0; i <= nCompLen; i++)
		{
			pCompString->CPtoX(i, FALSE, &nPosX);
			if(nLineOffsetX + (nPosX - nPrevPosX) > m_rtClientArea.GetWidth())
			{
				rtArea.SetRect(nLeft, nTop, m_rtClientArea.GetWidth() - nLeft, nTop + nTextHeight + nLineInterval);

				AddItem(nStringIdx, i - nStringIdx - 1, rtArea, nLineIdx, TRUE, FALSE);

				nLeft			= 0;
				nTop			+= nLineInterval + nTextHeight;
				nLineOffsetX	= 0;
				nStringIdx		= nPrevPosX;

				nLineIdx++;
			}

			nLineOffsetX		+= nPosX - nPrevPosX;
			nPrevPosX			= nPosX;
		}

		if(nLineOffsetX > 0)
		{
			rtArea.SetRect(nLeft, nTop, nLineOffsetX, nTop + nTextHeight + nLineInterval);
			AddItem(nStringIdx, nCompLen - nStringIdx, rtArea, nLineIdx, TRUE, FALSE);

			nLeft = nLineOffsetX;
		}
	}
	
	// caret after
	
	if(nLen > 0 && nCaret < nLen)
	{
		nLineOffsetX = nLeft;
		nStringIdx   = nCaret;

		for(int i = nCaret + 1; i <= nLen; i++)
		{
			nSize = pString->GetItemSize(i);
			pString->CPtoX(i, FALSE, &nPosX);

			if(i > 0 && i < nLen )
			{
				int nIdx = i-1;

				if( (*pString)[nIdx] == L'\\' && (*pString)[nIdx+1] == L'n')
				{
					rtArea.SetRect(nLeft, nTop, m_rtClientArea.GetWidth(), nTop + nTextHeight + nLineInterval);
				
					AddItem(nStringIdx, i - nStringIdx - 1, rtArea, nLineIdx, FALSE, FALSE);
					
					nLineOffsetX	= 0;
					nTop			+= nLineInterval + nTextHeight;

					nStringIdx		= i-1;
					nLineIdx++;		

					// new line
					nLeft			= 0;

					rtArea.SetRect(nLeft, nTop, 0, nTop + nTextHeight + nLineInterval);
					AddItem(nStringIdx, 2, rtArea, nLineIdx, FALSE, TRUE);
					
					nStringIdx		+= 2;
					nLineIdx++;		

					i++;
					nLineOffsetX	= 0;

					continue;
				}
			}
			
			if(nLineOffsetX + nSize > m_rtClientArea.GetWidth())
			{
				rtArea.SetRect(nLeft, nTop, m_rtClientArea.GetWidth() - nLeft, nTop + nTextHeight + nLineInterval);
				
				AddItem(nStringIdx, i - nStringIdx - 1, rtArea, nLineIdx, FALSE, FALSE);
				
				nLeft			= 0;
				nLineOffsetX	= 0;
				nTop			+= nLineInterval + nTextHeight;

				nStringIdx		= i-1;
				nLineIdx++;
			}

			nLineOffsetX	+= nSize;
		}

		if(nLineOffsetX > 0)
		{
			rtArea.SetRect(nLeft, nTop, nLineOffsetX, nTop + nTextHeight + nLineInterval);
			AddItem(nStringIdx, nLen - nStringIdx, rtArea, nLineIdx, FALSE, FALSE);

			nLeft = nLineOffsetX;
		}
	}

	return sNewCaret;
}

void CInputBoxLineInfo::SelectArea(CUniBuffer *pString, INT nStartCaret, INT nEndCaret, std::list<CRectangle>& listArea)
{
	if(m_nActiveNum <= 0)
		return;

	if(nStartCaret == nEndCaret)
		return;

	INT nSCaret = nStartCaret;
	INT nECaret = nEndCaret;

	if(nStartCaret > nEndCaret)
	{
		nSCaret = nEndCaret;
		nECaret = nStartCaret;
	}

	BOOL bStart = FALSE;
	BOOL bEnd = FALSE;

	INT nPosX, nNextPosX;

	CRectangle rtArea;
	
	for(INT i = 0; i < m_nActiveNum; i++)
	{
		if(m_pLineItem[i].bCompString)
			continue;

		if(nSCaret > m_pLineItem[i].nStringIdx + m_pLineItem[i].nStringLen)
			continue;

		if(bStart && bEnd)
			return;

		if(!bStart)
		{
			pString->CPtoX(m_pLineItem[i].nStringIdx, FALSE, &nPosX);

			for(INT j = m_pLineItem[i].nStringIdx; j < m_pLineItem[i].nStringIdx + m_pLineItem[i].nStringLen; j++)
			{
				pString->CPtoX(j, FALSE, &nNextPosX);

				if(j >= nSCaret && !bStart)
				{
					rtArea.left		= m_pLineItem[i].rtArea.left + nNextPosX - nPosX;
					rtArea.top		= m_pLineItem[i].rtArea.top;
						
					bStart = TRUE;
				}

				if(bStart)
				{
					if(nECaret == j)
					{
						rtArea.right	= m_pLineItem[i].rtArea.left + nNextPosX - nPosX;
						rtArea.bottom	= m_pLineItem[i].rtArea.bottom;

						listArea.push_back(rtArea);

						bEnd = TRUE;

						break;
					}
				}
			}

			if(bStart && !bEnd)
			{
				rtArea.right	= m_pLineItem[i].rtArea.right;
				rtArea.bottom	= m_pLineItem[i].rtArea.bottom;

				listArea.push_back(rtArea);
			}
		}
		else
		{
			pString->CPtoX(m_pLineItem[i].nStringIdx, FALSE, &nPosX);
			rtArea.left		= m_pLineItem[i].rtArea.left;
			rtArea.top		= m_pLineItem[i].rtArea.top;

			for(INT j = m_pLineItem[i].nStringIdx; j < m_pLineItem[i].nStringIdx + m_pLineItem[i].nStringLen; j++)
			{
				pString->CPtoX(j, FALSE, &nNextPosX);

				if(nECaret == j)
				{
					rtArea.right	= m_pLineItem[i].rtArea.left + nNextPosX - nPosX;
					rtArea.bottom	= m_pLineItem[i].rtArea.bottom;

					listArea.push_back(rtArea);

					bEnd = TRUE;

					break;
				}
			}

			if(!bEnd)
			{
				rtArea.right	= m_pLineItem[i].rtArea.right;
				rtArea.bottom	= m_pLineItem[i].rtArea.bottom;

				listArea.push_back(rtArea);
			}
		}
	}
}

void CInputBoxLineInfo::TextOut(CRectangle rtClipping, CUniBuffer *pString, CUniBuffer *pCompString, CGuiFont *pFont, CTexture *pTexture, COLORREF textcolor, COLORREF bkcolor)
{
	CRectangle rtBound, rtDraw;

	pTexture->FillClear();
	
	CUniBuffer *pOut;

	CRectangle rtArea;

	for(INT i = 0; i < m_nActiveNum; i++)
	{
		if(m_pLineItem[i].bNewLine)
			continue;

		if(m_pLineItem[i].rtArea.top < rtClipping.top)
			continue;

		if(m_pLineItem[i].rtArea.bottom > rtClipping.bottom)
			continue;

		rtBound.SetRect(0, 0, 0, 0);

		if(m_pLineItem[i].bCompString)
			pOut = pCompString;
		else
			pOut = pString;

		rtBound = pFont->TextOutW(m_pLineItem[i].nStringLen, pOut->GetBuffer() + m_pLineItem[i].nStringIdx);

		rtArea		= m_pLineItem[i].rtArea;
		rtArea.top -= rtClipping.top;
		
		rtDraw.SetRect(rtArea.left, rtArea.top, rtArea.left + rtBound.GetWidth(), rtArea.top + rtBound.GetHeight());

		g_FontMgr.BitmapToTexture(pTexture, rtDraw, textcolor);
	}

	// caret 전까지 문자를 찍는다.
}

SCaretMoveInfo CInputBoxLineInfo::Home(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo)
{
	SCaretMoveInfo sCaretInfo = { 0, 0, 0, 0};
	
	if(m_nActiveNum <= 0)
		return sCaretInfo;

	INT nLineIdx = FindLineIndex(sCurrCaretInfo.nCaretIdx);
	if(nLineIdx <= 0)
		return sCaretInfo;

	sCaretInfo = sCurrCaretInfo;

	for(INT i = 0; i < m_nActiveNum; i++)
	{
		if(m_pLineItem[i].nLineIdx == nLineIdx)
		{
			sCaretInfo.nCaretIdx = m_pLineItem[i].nStringIdx;
			return sCaretInfo;
		}
	}

	return sCaretInfo;
}

SCaretMoveInfo CInputBoxLineInfo::End(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo)
{
	SCaretMoveInfo sCaretInfo = { 0, 0, 0, 0};

	if(m_nActiveNum <= 0)
		return sCaretInfo;

	INT nLineIdx = FindLineIndex(sCurrCaretInfo.nCaretIdx);

	sCaretInfo = sCurrCaretInfo;

	for(INT i = 0; i < m_nActiveNum; i++)
	{
		if(m_pLineItem[i].nLineIdx == nLineIdx+1)
		{
			sCaretInfo.nCaretIdx = m_pLineItem[i].nStringIdx;
			return sCaretInfo;
		}
	}

	sCaretInfo.nCaretIdx = pString->GetTextSize();

	return sCaretInfo;
}

SCaretMoveInfo CInputBoxLineInfo::Up(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo)
{
	return FindStringIndex(pString, sCurrCaretInfo, FALSE);
}

SCaretMoveInfo CInputBoxLineInfo::Down(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo)
{
	return FindStringIndex(pString, sCurrCaretInfo, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// Construction:

CInputBox_Generic::CInputBox_Generic(
	CInputBox *pSelf, 
	DWORD dwStyle,
	int nMaxLength)
: m_pInputBox(pSelf),m_nMaxLength(nMaxLength)
{
	m_pFont = NULL;
	m_dwStyle = dwStyle;
	
	m_bShowCaret = true;
	m_nCaretCX = 2;
	m_nCaretCY = 13; // by daneos: 13 seems to be better than 15
	m_TextColor = DEFAULT_TEXTCOLOR;
	m_BkColor = DEFAULT_BKCOLOR;
	m_nBkMode = BK_TRANSPARENT;
	m_nFontH = DEFAULT_FONT_SIZE;
	m_strFont = DEFAULT_FONT;

	this->CreateFont();
	CreateCaret(m_nCaretCX,m_nCaretCY);
	
	m_SlotSetOptions = m_pInputBox->SigSetOptions().Connect(this, &CInputBox_Generic::OnSetOptions);
	m_SlotResize = m_pInputBox->SigResize().Connect(this, &CInputBox_Generic::OnResize);
	m_SlotMove = m_pInputBox->SigMove().Connect(this, &CInputBox_Generic::OnMove);
	m_SlotWheel = m_pInputBox->SigMouseWheel().Connect(this, &CInputBox_Generic::OnWheel);
	m_SlotSetAlpha = m_pInputBox->SigSetAlpha().Connect( this, &CInputBox_Generic::OnSetAlpha );
	m_SlotGotFocus = m_pInputBox->SigGotFocus().Connect( this, &CInputBox_Generic::OnGotFocus );
	m_SlotLostFocus = m_pInputBox->SigLostFocus().Connect( this, &CInputBox_Generic::OnLostFocus );

	m_SlotMouseDown = m_pInputBox->SigMouseDown().Connect(this, &CInputBox_Generic::OnMouseDown);
	m_SlotMouseUp = m_pInputBox->SigMouseUp().Connect(this, &CInputBox_Generic::OnMouseUp);
	m_SlotMouseMove = m_pInputBox->SigMouseMove().Connect(this, &CInputBox_Generic::OnMouseMove);
	m_SlotKeyDown = m_pInputBox->SigKeyDown().Connect(this,&CInputBox_Generic::OnKeyDown); 

	m_SlotScrollChange = m_pInputBox->m_pScrollBar->SigValueChanged().Connect( this, &CInputBox_Generic::OnScrollMove );
	m_SlotSliderMove = m_pInputBox->m_pScrollBar->SigSliderMoved().Connect( this, &CInputBox_Generic::OnScrollMove );

	CInputBox_Generic::Initialize();			// ensure static vars are properly init'ed first
    _ImmDisableTextFrameService( (DWORD)-1 );	// Disable TSF for the current process

	m_bEnableImeSystem	= true;

    m_bHideCaret		= false;
	m_bCaretMode		= false;
	m_nFirstVisible		= 0;
    m_nCaret			= 0;
	m_nCaretLine		= 0;
	m_nCaretPosX		= 0;
	m_nCaretPosY		= 0;
	m_nCaretRenderPosX	= 0;
	m_nCaretRenderPosY	= 0;
	m_nSelStart			= 0;
    m_bInsertMode		= true;
    m_bMouseDrag		= false;
	
	m_bMultiLine		= false;
	m_pMultiLine		= NULL;
	m_nMultiLineInterval = 3;
	m_nScrollNum		= 0;
	m_nCurrScrollIdx	= 0;
	m_bAutoScrollDec	= false;
	m_ptScrollPos.SetPos(0, 0);

	m_strKeyDownSound = GSD_SYSTEM_KEY_DOWN;
	//CInputBox_Generic::SetEnglishMode();
	SetMultilineMode( FALSE );
}


CInputBox_Generic::~CInputBox_Generic()
{
	NTL_DELETE( m_pMultiLine );

	if(m_pFont) 
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}

	if(m_TextSurface.m_pTexture) 
	{
		g_TextureMgr.DeleteTexture(m_TextSurface.m_pTexture);
		m_TextSurface.m_pTexture = NULL;
	}
}

VOID CInputBox_Generic::OnSetOptions(const CComponentOptions &options)
{
	if(options.Exists("maxlength"))
		m_pInputBox->SetMaxLength(options.GetValueAsInt("maxlength"));
	if(options.Exists("readonly"))
	{
		if(options.GetValueAsBool("readonly"))
			m_dwStyle |= IS_READONLY;
	}
	if(options.Exists("password_mode"))
	{
		if(options.GetValueAsBool("password_mode"))
			m_dwStyle |= IS_PASSWORD;
	}
	if(options.Exists("numberonly"))
	{
		if(options.GetValueAsBool("numberonly"))
			m_dwStyle |= IS_NUMBERONLY;
	}

	if(options.Exists("font_name"))
		m_strFont = options.GetValue("font_name"); 

	if(options.Exists("font_height"))
		m_nFontH = options.GetValueAsInt("font_height" );

	CreateFont();
	CreateCaret();

	if(options.Exists("text"))
		m_pInputBox->SetText(options.GetValue("text"));
}


VOID CInputBox_Generic::OnResize(int nX,int nY)
{
	CRectangle rect = m_pInputBox->GetScreenRect();

	if(m_TextSurface.m_pTexture)
	{
		int nReWidth	= g_TextureMgr.GetSortSize(rect.GetWidth());
		int nTexWidth	= m_TextSurface.m_pTexture->GetWidth();

		if(nReWidth != nTexWidth)
		{
			if(m_TextSurface.m_pTexture) 
			{
				g_TextureMgr.DeleteTexture(m_TextSurface.m_pTexture);
				m_TextSurface.m_pTexture = NULL;

				m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture(rect.GetWidth(), rect.GetHeight());

				m_TextSurface.m_SnapShot.rtRect.left = rect.left;
				m_TextSurface.m_SnapShot.rtRect.right = rect.left+m_TextSurface.m_pTexture->GetWidth();
				m_TextSurface.m_SnapShot.rtRect.top = rect.top;
				m_TextSurface.m_SnapShot.rtRect.bottom = rect.top+m_TextSurface.m_pTexture->GetHeight();
			}
		}

		if(m_pInputBox->IsVisible())
		{
			CUniBuffer *pOutBuffer = GetOutBuffer();

			if(m_nCaret >= 0 && pOutBuffer->GetTextSize())
			{
				PlaceCaret( m_nCaret );
			}

			TextOut();
		}
	}
}

VOID CInputBox_Generic::OnMove(int nOldX,int nOldY)
{
	if(m_pFont == NULL) 
		return;
	if(m_TextSurface.m_pTexture == NULL) 
		return;
	
	int nTextHeight = m_pFont->GetHeight();

	CRectangle rect = m_pInputBox->GetScreenRect();

	m_TextSurface.m_SnapShot.rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left+m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top+m_TextSurface.m_pTexture->GetHeight();

	m_CaretSurface.m_SnapShot.rtRect.left = rect.left+m_nCaretPosX;
	m_CaretSurface.m_SnapShot.rtRect.right = rect.left+m_nCaretPosX+m_nCaretCX;
	m_CaretSurface.m_SnapShot.rtRect.top = rect.top;
	m_CaretSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;
}

VOID CInputBox_Generic::OnWheel(int nFlags ,short shDelta, CPos& ptPos)
{
	if(m_bMultiLine)
	{
		if(shDelta < 0)
			IncrementScroll();
		else
			DecrementScroll();

		MultiLineTextOut();
	}
}

LRESULT CInputBox_Generic::OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return OnIMEMsgProc(hWnd, uMsg, wParam, lParam);
}


VOID CInputBox_Generic::Clear(VOID)
{
	m_Buffer.Clear();
	m_PwBuffer.Clear();
    m_nFirstVisible = 0;
    PlaceCaret( 0 );
    m_nSelStart = 0;

	TextOut();
}

BOOL CInputBox_Generic::IsPasswordMode(VOID)
{
	if(m_dwStyle & IS_PASSWORD)
		return TRUE;

	return FALSE;
}

BOOL CInputBox_Generic::IsNumberOnly( VOID )
{
	if(m_dwStyle & IS_NUMBERONLY)
		return TRUE;

	return FALSE;
}

BOOL CInputBox_Generic::IsReadOnly(VOID)
{
	if(m_dwStyle & IS_READONLY)
		return TRUE;

	return FALSE;
}


CUniBuffer*	CInputBox_Generic::GetOutBuffer(VOID)
{
	if(IsPasswordMode())
		return &m_PwBuffer;
	else
		return &m_Buffer;
}

VOID CInputBox_Generic::SetPasswordMode(BOOL bPwMode)
{
	if(bPwMode)
		m_dwStyle = m_dwStyle | IS_PASSWORD;
	else
		m_dwStyle = m_dwStyle & ~IS_PASSWORD;
}

VOID CInputBox_Generic::SetNumberOnly( BOOL bNumberMode )
{
	if( bNumberMode )
		m_dwStyle = m_dwStyle | IS_NUMBERONLY;
	else
		m_dwStyle = m_dwStyle & ~IS_NUMBERONLY;
}

VOID CInputBox_Generic::SetMultilineMode(BOOL bMultilineMode)
{
	if(bMultilineMode)
	{
		if(m_pMultiLine == NULL)
		{
			m_pMultiLine = NTL_NEW CInputBoxLineInfo;
		}

		m_bMultiLine = true;

		m_Buffer.SetSizeCalcEnable();

		m_pInputBox->m_pScrollBar->Show(true);
	}
	else
	{
		NTL_DELETE( m_pMultiLine );

		m_bMultiLine = false;

		m_pInputBox->m_pScrollBar->Show(false);
	}
}


VOID CInputBox_Generic::SetReadOnly(BOOL bReadOnly)
{
	if(bReadOnly)
		m_dwStyle |= IS_READONLY;
	else
		m_dwStyle = m_dwStyle & ~(IS_READONLY);
}

VOID CInputBox_Generic::SetMaxLength(INT nLength)
{
	m_nMaxLength = nLength;
}

VOID CInputBox_Generic::SetLineInterval(INT nInterval)
{
	m_nMultiLineInterval = nInterval;
}

VOID CInputBox_Generic::SetText(const char *text)
{
	m_nCurrScrollIdx	= 0;
	m_nScrollNum		= 0;	

	int nLen = (int)strlen(text);
	WCHAR *pBuffer = NTL_NEW WCHAR[nLen+1];

	//memset(pBuffer, 0, sizeof(WCHAR) * (nLen+1));
	::MultiByteToWideChar(GetACP(), 0, text, -1, pBuffer, nLen+1);

	SetText(pBuffer);

	NTL_ARRAY_DELETE( pBuffer );
}

VOID CInputBox_Generic::SetText(const WCHAR *text)
{
	m_Buffer.Clear(); 
	m_PwBuffer.Clear();
    m_Buffer.InsertString(0, text);
	if(IsPasswordMode())
	{
		int nSize = m_Buffer.GetTextSize();
		for(int i = 0; i < nSize; i++)
			m_PwBuffer.InsertChar(i, L'*');
	}

    m_nFirstVisible = 0;
    // Move the caret to the end of the text
	if(IsPasswordMode())
		PlaceCaret( m_PwBuffer.GetTextSize() );
	else
		PlaceCaret( m_Buffer.GetTextSize() );

    m_nSelStart = m_nCaret;

	TextOut();
}

VOID CInputBox_Generic::CreateFont(char *pFaceName, int nHeight, int nAttributes /*= 0*/)
{
	CRectangle rect = m_pInputBox->GetScreenRect(); 
	if(rect.GetWidth() == 0 || rect.GetHeight() == 0)
		return;

	if(m_pFont) 
	{
		g_FontMgr.DeleteGuiFont(m_pFont);
		m_pFont = NULL;
	}

	if(m_TextSurface.m_pTexture) 
	{
		g_TextureMgr.DeleteTexture(m_TextSurface.m_pTexture);
		m_TextSurface.m_pTexture = NULL;
	}

	m_pFont = g_FontMgr.CreateGuiFont(pFaceName, nHeight, 0);
	if(m_pFont == NULL)
		return;

	m_czFontText = m_pFont->GetBlankTextSize();

	m_Buffer.SetFont(m_pFont); 
	m_PwBuffer.SetFont(m_pFont);
	
	m_TextSurface.m_pTexture = g_TextureMgr.CreateTexture(rect.GetWidth(), rect.GetHeight());

	m_TextSurface.m_SnapShot.uRed = 255;
	m_TextSurface.m_SnapShot.uGreen = 255;
	m_TextSurface.m_SnapShot.uBlue = 255;
	m_TextSurface.m_SnapShot.uAlpha = 255;
	m_TextSurface.m_SnapShot.UVs[0] = 0;
	m_TextSurface.m_SnapShot.UVs[1] = 0;
	m_TextSurface.m_SnapShot.UVs[2] = 1;
	m_TextSurface.m_SnapShot.UVs[3] = 1;

	m_TextSurface.m_SnapShot.rtRect.left = rect.left;
	m_TextSurface.m_SnapShot.rtRect.right = rect.left+m_TextSurface.m_pTexture->GetWidth();
	m_TextSurface.m_SnapShot.rtRect.top = rect.top;
	m_TextSurface.m_SnapShot.rtRect.bottom = rect.top+m_TextSurface.m_pTexture->GetHeight();
}

VOID CInputBox_Generic::CreateFont(VOID)
{
	CreateFont((char*)m_strFont.data(), m_nFontH);
}


VOID CInputBox_Generic::CreateCaret(int nCX,int nCY)
{
	CRectangle rect = m_pInputBox->GetScreenRect();
	
	m_CaretSurface.m_SnapShot.rtRect.left = rect.left;
	m_CaretSurface.m_SnapShot.rtRect.right = rect.left+nCX;
	m_CaretSurface.m_SnapShot.rtRect.top = rect.top;
	m_CaretSurface.m_SnapShot.rtRect.bottom = rect.top+nCY;
	m_CaretSurface.m_SnapShot.uRed = 255;
	m_CaretSurface.m_SnapShot.uGreen = 255;
	m_CaretSurface.m_SnapShot.uBlue = 38;
	m_CaretSurface.m_SnapShot.uAlpha = 255;
	m_CaretSurface.m_SnapShot.uBlend = 0;
	m_CaretSurface.m_SnapShot.UVs[0] = 0; 
	m_CaretSurface.m_SnapShot.UVs[1] = 0; 
	m_CaretSurface.m_SnapShot.UVs[2] = 1; 
	m_CaretSurface.m_SnapShot.UVs[3] = 1; 

	m_SelSurface.m_SnapShot.uAlpha = 255;
	m_SelSurface.m_SnapShot.uRed = 99;
	m_SelSurface.m_SnapShot.uGreen = 90;
	m_SelSurface.m_SnapShot.uBlue = 255;
	m_SelSurface.m_SnapShot.UVs[0] = 0; 
	m_SelSurface.m_SnapShot.UVs[1] = 0; 
	m_SelSurface.m_SnapShot.UVs[2] = 1; 
	m_SelSurface.m_SnapShot.UVs[3] = 1;

	m_CompSurface.m_SnapShot.uRed = 255;
	m_CompSurface.m_SnapShot.uGreen = 255;
	m_CompSurface.m_SnapShot.uBlue = 38;
	m_CompSurface.m_SnapShot.uAlpha = 255;
	m_CompSurface.m_SnapShot.UVs[0] = 0; 
	m_CompSurface.m_SnapShot.UVs[1] = 0; 
	m_CompSurface.m_SnapShot.UVs[2] = 1; 
	m_CompSurface.m_SnapShot.UVs[3] = 1;

	m_CompAttrSurface.m_SnapShot.uRed = 99;
	m_CompAttrSurface.m_SnapShot.uGreen = 90;
	m_CompAttrSurface.m_SnapShot.uBlue = 255;
	m_CompAttrSurface.m_SnapShot.uAlpha = 255;
	m_CompAttrSurface.m_SnapShot.UVs[0] = 0; 
	m_CompAttrSurface.m_SnapShot.UVs[1] = 0; 
	m_CompAttrSurface.m_SnapShot.UVs[2] = 1; 
	m_CompAttrSurface.m_SnapShot.UVs[3] = 1;
}

VOID CInputBox_Generic::CreateCaret(VOID)
{
	CreateCaret(m_nCaretCX,m_nCaretCY);
}

VOID CInputBox_Generic::SetCaretSize(int nCX,int nCY)
{
	m_nCaretCX = nCX;
	m_nCaretCY = nCY;
	
	m_CaretSurface.m_SnapShot.rtRect.right = m_CaretSurface.m_SnapShot.rtRect.left+nCX;
	m_CaretSurface.m_SnapShot.rtRect.bottom = m_CaretSurface.m_SnapShot.rtRect.top+nCY;
}

VOID CInputBox_Generic::SetCaretColor(BYTE byRed,BYTE byGreen,BYTE byBlue,BYTE byAlpha)
{
	m_CaretSurface.m_SnapShot.uRed = byRed;
	m_CaretSurface.m_SnapShot.uGreen = byGreen;
	m_CaretSurface.m_SnapShot.uBlue = byBlue;
	m_CaretSurface.m_SnapShot.uAlpha = byAlpha;
}

VOID CInputBox_Generic::SingleLineTextOut(VOID)
{
	CRectangle rect = m_pInputBox->GetClientRect(); 
	m_CompString.SetFont(m_pFont); 

	CRectangle rtBound, rtDraw;

	int nCaretX, nFirstX;

	CUniBuffer *pOutBuffer = GetOutBuffer();

	pOutBuffer->CPtoX(m_nCaret, FALSE, &nCaretX);
	pOutBuffer->CPtoX(m_nFirstVisible, FALSE, &nFirstX);

	int nVisible = m_nFirstVisible;
	int nDeltaCaretX = nCaretX - nFirstX;
	int nSubCaret = m_nCaret - m_nFirstVisible;
	
	int nCompX, nCompFirstX = 0, nCompVisible = 0;
	int nCompSize = m_CompString.GetTextSize(); 
	m_CompString.CPtoX(nCompSize, FALSE, &nCompX);
	int nDeltaCompX = nCompX - nCompFirstX;

	while(1)
	{
		if(nDeltaCaretX + nDeltaCompX < rect.GetWidth()) 
			break;

		nVisible++;
		if(nVisible < m_nCaret)
		{
			pOutBuffer->CPtoX(nVisible, FALSE, &nFirstX);
			nDeltaCaretX = nCaretX - nFirstX;
		}
		else
		{
			if(m_nCompCaret <= nCompVisible)
				break;

			nDeltaCaretX = 0;
			nCompVisible++;
			m_CompString.CPtoX(nCompVisible, FALSE, &nCompFirstX);
			nDeltaCompX = nCompX - nCompFirstX;
		}
	}

	// compsition string attr 계산.

	int nCompStartAttr = -1;
	int nCompEndAttr = -1;

	for(int i = 0; i < nCompSize; i++)
	{
		if(m_abCompStringAttr[i] == ATTR_TARGET_CONVERTED)
		{
			if(nCompStartAttr < 0)
			{
				nCompStartAttr = i;
			}

			nCompEndAttr = i+1;
		}
		else if(m_abCompStringAttr[i] == ATTR_TARGET_NOTCONVERTED)
		{
			if(nCompStartAttr < 0)
			{
				nCompStartAttr = i;
			}

			nCompEndAttr = i+1;
		}
	}
  
	m_TextSurface.m_pTexture->FillClear();

	// caret 전까지 문자를 찍는다.
	rtBound.SetRect(0, 0, 0, 0);

	COLORREF color;
	if(m_nBkMode == BK_OPAQUE)
		color = m_BkColor;
	else
		color = 0;
	
	if(nVisible < m_nCaret)
	{
		rtBound = m_pFont->TextOutW( m_nCaret - nVisible,pOutBuffer->GetBuffer() + nVisible);

		rtDraw = rtBound;
		if(rtDraw.right >= rect.right)
			rtDraw.right = rect.right; 

		g_FontMgr.BitmapToTexture(m_TextSurface.m_pTexture, rtDraw, m_TextColor);
	}

	// composition 문자를 출력한다.
	int nStartX = rtBound.right;
	rtBound = m_pFont->TextOutW(nCompSize - nCompVisible, m_CompString.GetBuffer()+nCompVisible);
	rtDraw = rtBound;
	rtDraw.Offset(nStartX, 0); 
	if(rtDraw.left >= rect.right)
		rtDraw.left = rect.right; 
	if(rtDraw.right >= rect.right)
		rtDraw.right = rect.right; 

	m_nCompLineStartX = rtDraw.left;
	m_nCompLineEndX = rtDraw.right;
		
	m_CompString.CPtoX(m_nCompCaret, FALSE, &m_nCompCaretPosX);
	m_CompString.CPtoX(nCompVisible, FALSE, &nCompFirstX);
	m_nCompCaretPosX = m_nCompCaretPosX - nCompFirstX + nStartX;

	m_nCompAttrStartX = 0;
	m_nCompAttrEndX = 0;

	if(nCompStartAttr >= 0 && nCompEndAttr >= 0)
	{
		m_CompString.CPtoX(nCompStartAttr, FALSE, &m_nCompAttrStartX);
		m_CompString.CPtoX(nCompEndAttr, FALSE, &m_nCompAttrEndX);

		m_nCompAttrStartX = m_nCompAttrStartX - nCompFirstX + nStartX;
		m_nCompAttrEndX = m_nCompAttrEndX - nCompFirstX + nStartX;
	}
			
	g_FontMgr.BitmapToTexture(m_TextSurface.m_pTexture, rtDraw, m_TextColor);

	// caret 다음에 문자를 찍는다.
	nStartX += rtBound.right;
	rtBound = m_pFont->TextOutW(pOutBuffer->GetBuffer() + m_nCaret);

	rtDraw = rtBound;
	rtDraw.Offset(nStartX, 0); 
	if(rtDraw.right >= rect.right)
		rtDraw.right = rect.right; 
	if(rtDraw.left >= rect.right)
		rtDraw.left = rect.right; 

	g_FontMgr.BitmapToTexture(m_TextSurface.m_pTexture, rtDraw, m_TextColor);

    // caret랜더링.    
	SingleLineCaretRenderPosition();

}

VOID CInputBox_Generic::MultiLineTextCalc(VOID)
{
	
	m_CompString.SetFont(m_pFont); 

	// compsition string attr 계산.

	int nCompSize = m_CompString.GetTextSize(); 

	int nCompStartAttr = -1;
	int nCompEndAttr = -1;

	for(int i = 0; i < nCompSize; i++)
	{
		if(m_abCompStringAttr[i] == ATTR_TARGET_CONVERTED)
		{
			if(nCompStartAttr < 0)
			{
				nCompStartAttr = i;
			}

			nCompEndAttr = i+1;
		}
		else if(m_abCompStringAttr[i] == ATTR_TARGET_NOTCONVERTED)
		{
			if(nCompStartAttr < 0)
			{
				nCompStartAttr = i;
			}

			nCompEndAttr = i+1;
		}
	}

	CRectangle rect = m_pInputBox->GetClientRect(); 
	rect.right -= GetScrollBarWidth();

	m_pMultiLine->SetClientArea(rect.GetWidth(), rect.GetHeight());
	SCaretMoveInfo sNewCaret = m_pMultiLine->SetString(&m_Buffer, &m_CompString, m_nCaret, m_nMultiLineInterval, m_czFontText.y);

	INT nCurrCaretLine = m_nCaretLine;
	
	CalcCaretInfo(sNewCaret);

	MultiLineAutoScrollDecrement(nCurrCaretLine);

	m_SigCharChanged(m_Buffer.GetTextSize());
}


VOID CInputBox_Generic::MultiLineTextOut(VOID)
{
	CRectangle rtClipping = m_pInputBox->GetClientRect();
	rtClipping.Offset(m_ptScrollPos.x, m_ptScrollPos.y);

	CRectangle rtScroll = m_pInputBox->m_pScrollBar->GetClientRect();
	rtClipping.right -= GetScrollBarWidth();

	COLORREF bkcolor;
	if(m_nBkMode == BK_OPAQUE)
		bkcolor = m_BkColor;
	else
		bkcolor = 0;

	m_pMultiLine->TextOut(rtClipping, &m_Buffer, &m_CompString, m_pFont, m_TextSurface.m_pTexture, m_TextColor, bkcolor);
	
	MultiLineCaretRenderPosition();
}

VOID CInputBox_Generic::MultiLineAutoScrollDecrement(INT nOldCaretLine)
{
	CRectangle rtClient = m_pInputBox->GetClientRect();
	while(m_nCurrScrollIdx > 0 && m_nCaretRenderPosY < 0)
	{
		DecrementScroll();
	}
	
	while(m_nCurrScrollIdx < m_nScrollNum && 
		m_nCaretRenderPosY + m_czFontText.y + m_nMultiLineInterval > rtClient.bottom)
	{
		IncrementScroll();
	}

	if(m_bAutoScrollDec)
	{
		if(nOldCaretLine > m_nCaretLine)
		{
			int nAutoNum = nOldCaretLine - m_nCaretLine;
			for(int i = 0; i < nAutoNum; i++)
			{
				DecrementScroll();
			}
		}

		m_bAutoScrollDec = false;
	}
}

VOID CInputBox_Generic::MakeCurrentCaretInfo(SCaretMoveInfo& sCaretInfo)
{
	sCaretInfo.nCaretIdx	=	m_nCaret;
	sCaretInfo.nCaretLine	=	m_nCaretLine;
	sCaretInfo.nCaretPosX	=	m_nCaretPosX;
	sCaretInfo.nCaretPosY	=	m_nCaretPosY;
}

VOID CInputBox_Generic::MakeCurrentCaretInfo(SCaretMoveInfo& sCaretInfo, INT nCaretPosX, INT nCaretPosY)
{
	sCaretInfo.nCaretIdx	=	m_nCaret;
	sCaretInfo.nCaretLine	=	m_nCaretLine;
	sCaretInfo.nCaretPosX	=	nCaretPosX;
	sCaretInfo.nCaretPosY	=	nCaretPosY;
}

VOID CInputBox_Generic::CalcCaretInfo(SCaretMoveInfo sCaretInfo)
{
	CRectangle rect		= m_pInputBox->GetClientRect(); 
	CRectangle rtValid	= m_pMultiLine->GetValidArea();

	INT nDeltaBottom = rtValid.bottom - rect.bottom;
	INT nDiv		 = m_czFontText.y+m_nMultiLineInterval;

	if(nDeltaBottom < 0)
		m_nScrollNum = 0;
	else
	{
		m_nScrollNum	= nDeltaBottom/nDiv;
		if(nDeltaBottom%nDiv > 0)
			m_nScrollNum++;
	}

	m_nCaretPosX		= sCaretInfo.nCaretPosX;
	m_nCaretPosY		= sCaretInfo.nCaretPosY;

	m_nCaretRenderPosX	= sCaretInfo.nCaretPosX;

	m_nCaretLine		= sCaretInfo.nCaretLine;

	m_pInputBox->m_pScrollBar->SetRange(0, m_nScrollNum);
}

VOID CInputBox_Generic::CalcScrollPosition(VOID)
{
	m_ptScrollPos.x = 0;
	m_ptScrollPos.y = m_nCurrScrollIdx * (m_czFontText.y+m_nMultiLineInterval);

	m_nCaretRenderPosY = m_nCaretPosY - m_ptScrollPos.y;
}

VOID CInputBox_Generic::IncrementScroll(VOID)
{
	if(m_nCurrScrollIdx >= m_nScrollNum)
		return;

	m_nCurrScrollIdx++;
	if(m_nCurrScrollIdx >= m_nScrollNum)
		m_nCurrScrollIdx = m_nScrollNum;

	CalcScrollPosition();

	m_pInputBox->m_pScrollBar->SetValue(m_nCurrScrollIdx);
}

VOID CInputBox_Generic::DecrementScroll(VOID)
{
	if(m_nCurrScrollIdx <= 0)
		return;

	m_nCurrScrollIdx--;
	if(m_nCurrScrollIdx < 0)
		m_nCurrScrollIdx = 0;

	CalcScrollPosition();

	m_pInputBox->m_pScrollBar->SetValue(m_nCurrScrollIdx);
}

INT	CInputBox_Generic::GetScrollBarWidth(VOID)
{
	return m_pInputBox->m_pScrollBar->GetWidth() + 2;
}

VOID CInputBox_Generic::TextOut(void)
{
	//
    // Compute the X coordinates of the first visible character.
    //
    if(m_pFont == NULL || m_TextSurface.m_pTexture == NULL)
		return;

	if(m_bMultiLine)
	{
		MultiLineTextCalc();
		CalcScrollPosition();
		MultiLineTextOut();
	}
	else
	{
		SingleLineTextOut();
	}
}

VOID CInputBox_Generic::CaretRenderPosition(VOID)
{
	SingleLineCaretRenderPosition();
}

VOID CInputBox_Generic::SingleLineCaretRenderPosition(VOID)
{
	int nXFirst, nCaretX, nSelStartX;

	CUniBuffer *pOutBuffer = GetOutBuffer();
    
	pOutBuffer->CPtoX( m_nFirstVisible, FALSE, &nXFirst );
	pOutBuffer->CPtoX( m_nCaret, FALSE, &nCaretX );
	pOutBuffer->CPtoX( m_nSelStart, FALSE, &nSelStartX );
	
	int nDeltaCaretX = nCaretX - nXFirst;
	int nDeltaSelX = abs(nCaretX - nSelStartX);

	m_nCaretPosX = nDeltaCaretX;

	CRectangle rect = m_pInputBox->GetScreenRect();

	if(!m_bHideCaret)
	{
		m_CaretSurface.m_SnapShot.rtRect.left = rect.left+nDeltaCaretX;
		m_CaretSurface.m_SnapShot.rtRect.right = rect.left+nDeltaCaretX+m_nCaretCX;
		m_CaretSurface.m_SnapShot.rtRect.top = rect.top;
		m_CaretSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;
	}
	else
	{
		// compsition string caret으로 바꾼다.
		m_CaretSurface.m_SnapShot.rtRect.left = rect.left + m_nCompCaretPosX;
		m_CaretSurface.m_SnapShot.rtRect.right = rect.left + m_nCompCaretPosX+m_nCaretCX;
		m_CaretSurface.m_SnapShot.rtRect.top = rect.top;
		m_CaretSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;
	}
	
	m_SelSurface.m_SnapShot.rtRect.left = rect.left+__min( nCaretX, nSelStartX ) - nXFirst;
	m_SelSurface.m_SnapShot.rtRect.right = rect.left+__min( nCaretX, nSelStartX ) - nXFirst + nDeltaSelX;
	m_SelSurface.m_SnapShot.rtRect.top = rect.top;
	m_SelSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;

	if(m_SelSurface.m_SnapShot.rtRect.right > rect.right)
		m_SelSurface.m_SnapShot.rtRect.right = rect.right;

//	m_nCompCaret

	if(m_CompString.GetTextSize() > 0)
	{
		m_CompSurface.m_SnapShot.rtRect.left = rect.left + m_nCompLineStartX;
		m_CompSurface.m_SnapShot.rtRect.right = rect.left + m_nCompLineEndX;
		m_CompSurface.m_SnapShot.rtRect.top = rect.top+m_nCaretCY;
		m_CompSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY+1;

		if(m_CompSurface.m_SnapShot.rtRect.right > rect.right)
			m_CompSurface.m_SnapShot.rtRect.right = rect.right;

		m_CompAttrSurface.m_SnapShot.rtRect.left = rect.left + m_nCompAttrStartX;
		m_CompAttrSurface.m_SnapShot.rtRect.right = rect.left + m_nCompAttrEndX;
		m_CompAttrSurface.m_SnapShot.rtRect.top = rect.top;
		m_CompAttrSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;

		if(m_CompAttrSurface.m_SnapShot.rtRect.right > rect.right)
			m_CompAttrSurface.m_SnapShot.rtRect.right = rect.right;
	}
	else
	{
		m_CompSurface.m_SnapShot.rtRect.left = 0;  
		m_CompSurface.m_SnapShot.rtRect.right = 0;
		m_CompSurface.m_SnapShot.rtRect.top = 0;
		m_CompSurface.m_SnapShot.rtRect.bottom = 0;

		m_CompAttrSurface.m_SnapShot.rtRect.left = 0;  
		m_CompAttrSurface.m_SnapShot.rtRect.right = 0;
		m_CompAttrSurface.m_SnapShot.rtRect.top = 0;
		m_CompAttrSurface.m_SnapShot.rtRect.bottom = 0;
	}

	/*
	if( m_bHasFocus && m_bCaretOn && !s_bHideCaret )
    {
        // Start the rectangle with insert mode caret
        RECT rcCaret = { m_rcText.left - nXFirst + nCaretX - 1, m_rcText.top,
                         m_rcText.left - nXFirst + nCaretX + 1, m_rcText.bottom };

        // If we are in overwrite mode, adjust the caret rectangle
        // to fill the entire character.
        if( !m_bInsertMode )
        {
            // Obtain the right edge X coord of the current character
            int nRightEdgeX;
            m_Buffer.CPtoX( m_nCaret, TRUE, &nRightEdgeX );
            rcCaret.right = m_rcText.left - nXFirst + nRightEdgeX;
        }

       // m_pDialog->DrawRect( &rcCaret, m_CaretColor );
    }
	*/
}

VOID CInputBox_Generic::MultiLineCaretRenderPosition(VOID)
{
	CRectangle rect = m_pInputBox->GetScreenRect();

	if(!m_bHideCaret)
	{
		m_CaretSurface.m_SnapShot.rtRect.left	= rect.left + m_nCaretRenderPosX;
		m_CaretSurface.m_SnapShot.rtRect.right	= rect.left + m_nCaretRenderPosX + m_nCaretCX;
		m_CaretSurface.m_SnapShot.rtRect.top	= rect.top + m_nCaretRenderPosY;
		m_CaretSurface.m_SnapShot.rtRect.bottom = rect.top + m_nCaretRenderPosY + m_nCaretCY;
	}
	else
	{
		// compsition string caret으로 바꾼다.
		m_CaretSurface.m_SnapShot.rtRect.left = rect.left + m_nCompCaretPosX;
		m_CaretSurface.m_SnapShot.rtRect.right = rect.left + m_nCompCaretPosX+m_nCaretCX;
		m_CaretSurface.m_SnapShot.rtRect.top = rect.top;
		m_CaretSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;
	}

	// 
	m_listMultiSelArea.clear();
	m_listMultiSelSurface.clear();

	m_pMultiLine->SelectArea(&m_Buffer, m_nSelStart, m_nCaret, m_listMultiSelArea);

	if(m_listMultiSelArea.size() > 0)
	{
		CSurface sur;

		sur.m_SnapShot.uAlpha = 255;
		sur.m_SnapShot.uRed = 99;
		sur.m_SnapShot.uGreen = 90;
		sur.m_SnapShot.uBlue = 255;
		sur.m_SnapShot.UVs[0] = 0; 
		sur.m_SnapShot.UVs[1] = 0; 
		sur.m_SnapShot.UVs[2] = 1; 
		sur.m_SnapShot.UVs[3] = 1;

		CRectangle rtClient = m_pInputBox->GetClientRect();
				
		std::list<CRectangle>::iterator it;
		for(it = m_listMultiSelArea.begin(); it != m_listMultiSelArea.end(); it++)
		{
			if((*it).top - m_ptScrollPos.y < rtClient.top || (*it).bottom - m_ptScrollPos.y > rtClient.bottom)
				continue;

			sur.m_SnapShot.rtRect.left		= rect.left + (*it).left;
			sur.m_SnapShot.rtRect.top		= rect.top + (*it).top - m_ptScrollPos.y;
			sur.m_SnapShot.rtRect.right		= rect.left + (*it).right;
			sur.m_SnapShot.rtRect.bottom	= rect.top + (*it).bottom - m_ptScrollPos.y;

			m_listMultiSelSurface.push_back(sur);
		}
	}

	/*

//	m_nCompCaret

	if(m_CompString.GetTextSize() > 0)
	{
		m_CompSurface.m_SnapShot.rtRect.left = rect.left + m_nCompLineStartX;
		m_CompSurface.m_SnapShot.rtRect.right = rect.left + m_nCompLineEndX;
		m_CompSurface.m_SnapShot.rtRect.top = rect.top+m_nCaretCY;
		m_CompSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY+1;

		if(m_CompSurface.m_SnapShot.rtRect.right > rect.right)
			m_CompSurface.m_SnapShot.rtRect.right = rect.right;

		m_CompAttrSurface.m_SnapShot.rtRect.left = rect.left + m_nCompAttrStartX;
		m_CompAttrSurface.m_SnapShot.rtRect.right = rect.left + m_nCompAttrEndX;
		m_CompAttrSurface.m_SnapShot.rtRect.top = rect.top;
		m_CompAttrSurface.m_SnapShot.rtRect.bottom = rect.top+m_nCaretCY;

		if(m_CompAttrSurface.m_SnapShot.rtRect.right > rect.right)
			m_CompAttrSurface.m_SnapShot.rtRect.right = rect.right;
	}
	else
	{
		m_CompSurface.m_SnapShot.rtRect.left = 0;  
		m_CompSurface.m_SnapShot.rtRect.right = 0;
		m_CompSurface.m_SnapShot.rtRect.top = 0;
		m_CompSurface.m_SnapShot.rtRect.bottom = 0;

		m_CompAttrSurface.m_SnapShot.rtRect.left = 0;  
		m_CompAttrSurface.m_SnapShot.rtRect.right = 0;
		m_CompAttrSurface.m_SnapShot.rtRect.top = 0;
		m_CompAttrSurface.m_SnapShot.rtRect.bottom = 0;
	}
	*/

	/*
	if( m_bHasFocus && m_bCaretOn && !s_bHideCaret )
    {
        // Start the rectangle with insert mode caret
        RECT rcCaret = { m_rcText.left - nXFirst + nCaretX - 1, m_rcText.top,
                         m_rcText.left - nXFirst + nCaretX + 1, m_rcText.bottom };

        // If we are in overwrite mode, adjust the caret rectangle
        // to fill the entire character.
        if( !m_bInsertMode )
        {
            // Obtain the right edge X coord of the current character
            int nRightEdgeX;
            m_Buffer.CPtoX( m_nCaret, TRUE, &nRightEdgeX );
            rcCaret.right = m_rcText.left - nXFirst + nRightEdgeX;
        }

       // m_pDialog->DrawRect( &rcCaret, m_CaretColor );
    }
	*/
}


VOID CInputBox_Generic::Render(VOID)
{
	g_GuiRenderer.RenderQueue(&m_CompAttrSurface.m_SnapShot, m_CompAttrSurface.m_pTexture);

	if(m_bMultiLine)
	{
		std::list<CSurface>::iterator it;
		for(it = m_listMultiSelSurface.begin(); it != m_listMultiSelSurface.end(); it++)
		{
			g_GuiRenderer.RenderQueue(&(*it).m_SnapShot, NULL);
		}
	}
	else
	{
		g_GuiRenderer.RenderQueue(&m_SelSurface.m_SnapShot, m_SelSurface.m_pTexture);
	}

	g_GuiRenderer.RenderQueue(&m_TextSurface.m_SnapShot, m_TextSurface.m_pTexture);
	
	RenderCaret();
}


VOID CInputBox_Generic::RenderCaret(VOID)
{
	g_GuiRenderer.RenderQueue(&m_CompSurface.m_SnapShot, m_SelSurface.m_pTexture);

	if(m_bShowCaret && m_bCaretMode)
	{
		CRectangle rect = m_pInputBox->GetScreenRect();
		if(m_CaretSurface.m_SnapShot.rtRect.top >= rect.top && 
			m_CaretSurface.m_SnapShot.rtRect.bottom <= rect.bottom)
		{
			g_GuiRenderer.RenderQueue(&m_CaretSurface.m_SnapShot, m_CaretSurface.m_pTexture);
		}
	}
}

VOID CInputBox_Generic::SetEnglishMode( BOOL bSet /* = FALSE  */ )
{
	HIMC hIMC;
    DWORD dwConv, dwSent, dwTemp;

    hIMC = ImmGetContext(m_hWindow);
    ImmGetConversionStatus(hIMC, &dwConv, &dwSent);

    if (!(dwConv & IME_CMODE_NATIVE))
        return;

    dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
    dwConv = dwTemp;

    ImmSetConversionStatus(hIMC, dwConv, dwSent);
	ImmReleaseContext(m_hWindow, hIMC);	
}

VOID CInputBox_Generic::SetEnglishModeDefault()
{
	if (!m_bSaveImeState)
		m_SavedImeState = IMEUI_STATE_ON;

	SetEnglishMode();
}

VOID CInputBox_Generic::SetNativeMode( BOOL bSet /* = FALSE  */ )
{
	HIMC hIMC;
    DWORD dwConv, dwSent, dwTemp;

	hIMC = ImmGetContext(m_hWindow);
    ImmGetConversionStatus(hIMC, &dwConv, &dwSent);

	
	if ((dwConv & IME_CMODE_NATIVE))
	{
		return;
	}

    dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
    dwTemp |= IME_CMODE_NATIVE;

	if (!(dwConv & IME_CMODE_FULLSHAPE))
	{
		dwConv |= IME_CMODE_FULLSHAPE;	//full width
	}

	if (!(dwConv & IME_CMODE_SYMBOL))
	{
		dwConv |= IME_CMODE_SYMBOL;		//Chinese punctuation
	}

    dwConv = dwTemp;

    ImmSetConversionStatus(hIMC, dwConv, dwSent);
	ImmReleaseContext(m_hWindow, hIMC);	
}

VOID CInputBox_Generic::SetNativeModeDefault(VOID)
{
	if( !m_bSaveImeState )
		m_SavedImeState = IMEUI_STATE_ON;			
	
	SetNativeMode();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


VOID CInputBox_Generic::Initialize(VOID)
{
	if(m_hDllImm32) // Only need to do once
        return;

	FARPROC Temp;
	m_CompString.SetBufferSize( MAX_COMPSTRING_SIZE );

    WCHAR wszPath[GUI_MAX_PATH];
    if( !::GetSystemDirectory( wszPath, GUI_MAX_PATH ) )
        return;

    StringCchCat( wszPath, GUI_MAX_PATH, IMM32_DLLNAME );
    m_hDllImm32 = LoadLibrary( wszPath );

    if( m_hDllImm32 )
    {
        GETPROCADDRESS( m_hDllImm32, ImmLockIMC, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmUnlockIMC, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmLockIMCC, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmUnlockIMCC, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmDisableTextFrameService, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetCompositionStringW, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetCandidateListW, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetContext, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmReleaseContext, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmAssociateContext, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetOpenStatus, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmSetOpenStatus, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetConversionStatus, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetDefaultIMEWnd, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetIMEFileNameA, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmGetVirtualKey, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmNotifyIME, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmSetConversionStatus, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmSimulateHotKey, Temp );
        GETPROCADDRESS( m_hDllImm32, ImmIsIME, Temp );
    }

    if( !::GetSystemDirectory( wszPath, GUI_MAX_PATH ) )
        return;
    StringCchCat( wszPath, GUI_MAX_PATH, VER_DLLNAME );
    m_hDllVer = LoadLibrary( wszPath );
    if( m_hDllVer )
    {
        GETPROCADDRESS( m_hDllVer, VerQueryValueA, Temp );
        GETPROCADDRESS( m_hDllVer, GetFileVersionInfoA, Temp );
        GETPROCADDRESS( m_hDllVer, GetFileVersionInfoSizeA, Temp );
    }
}

VOID CInputBox_Generic::Uninitialize(VOID)
{
	if( m_hDllImm32 )
    {
        PLACEHOLDERPROC( ImmLockIMC );
        PLACEHOLDERPROC( ImmUnlockIMC );
        PLACEHOLDERPROC( ImmLockIMCC );
        PLACEHOLDERPROC( ImmUnlockIMCC );
        PLACEHOLDERPROC( ImmDisableTextFrameService );
        PLACEHOLDERPROC( ImmGetCompositionStringW );
        PLACEHOLDERPROC( ImmGetCandidateListW );
        PLACEHOLDERPROC( ImmGetContext );
        PLACEHOLDERPROC( ImmReleaseContext );
        PLACEHOLDERPROC( ImmAssociateContext );
        PLACEHOLDERPROC( ImmGetOpenStatus );
        PLACEHOLDERPROC( ImmSetOpenStatus );
        PLACEHOLDERPROC( ImmGetConversionStatus );
        PLACEHOLDERPROC( ImmGetDefaultIMEWnd );
        PLACEHOLDERPROC( ImmGetIMEFileNameA );
        PLACEHOLDERPROC( ImmGetVirtualKey );
        PLACEHOLDERPROC( ImmNotifyIME );
        PLACEHOLDERPROC( ImmSetConversionStatus );
        PLACEHOLDERPROC( ImmSimulateHotKey );
        PLACEHOLDERPROC( ImmIsIME );

        FreeLibrary( m_hDllImm32 );
        m_hDllImm32 = NULL;
    }
    if( m_hDllIme )
    {
        PLACEHOLDERPROC( GetReadingString );
        PLACEHOLDERPROC( ShowReadingWindow );

        FreeLibrary( m_hDllIme );
        m_hDllIme = NULL;
    }
    if( m_hDllVer )
    {
        PLACEHOLDERPROC( VerQueryValueA );
        PLACEHOLDERPROC( GetFileVersionInfoA );
        PLACEHOLDERPROC( GetFileVersionInfoSizeA );

        FreeLibrary( m_hDllVer );
        m_hDllVer = NULL;
    }
}


VOID CInputBox_Generic::SendKey( BYTE nVirtKey )
{
	keybd_event( nVirtKey, 0, 0,               0 );
    keybd_event( nVirtKey, 0, KEYEVENTF_KEYUP, 0 );
}

HRESULT CInputBox_Generic::StaticOnCreateDevice(HWND hWnd)
{
    // Save the default input context
	m_hWindow = hWnd;
	m_hImcDef = _ImmGetContext( hWnd );
   _ImmReleaseContext( hWnd, m_hImcDef );

    return S_OK;
}

/*
VOID CInputBox_Generic::UpdateRects(VOID)
{
	
    // Temporary adjust m_width so that CDXUTEditBox can compute
    // the correct rects for its rendering since we need to make space
    // for the indicator button

	CRectangle rect = m_pInputBox->
	
    int nWidth = m_width;
    m_width -= m_nIndicatorWidth + m_nBorder * 2; // Make room for the indicator button
    CDXUTEditBox::UpdateRects();
    m_width = nWidth;  // Restore

    // Compute the indicator button rectangle
    SetRect( &m_rcIndicator, m_rcBoundingBox.right, m_rcBoundingBox.top, m_x + m_width, m_rcBoundingBox.bottom );
//    InflateRect( &m_rcIndicator, -m_nBorder, -m_nBorder );
    m_rcBoundingBox.right = m_rcBoundingBox.left + m_width;

}

*/

#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)


DWORD CInputBox_Generic::GetImeId( UINT uIndex )
{
    static HKL hklPrev = 0;
    static DWORD dwID[2] = { 0, 0 };  // Cache the result
    
    DWORD   dwVerSize;
    DWORD   dwVerHandle;
    LPVOID  lpVerBuffer;
    LPVOID  lpVerData;
    UINT    cbVerData;
    char    szTmp[1024];

    if( uIndex >= sizeof( dwID ) / sizeof( dwID[0] ) )
        return 0;

    if( hklPrev == m_hklCurrent )
        return dwID[uIndex];

    hklPrev = m_hklCurrent;  // Save for the next invocation

    // Check if we are using an older Chinese IME
    if( !( ( m_hklCurrent == _CHT_HKL ) || ( m_hklCurrent == _CHT_HKL2 ) || ( m_hklCurrent == _CHS_HKL ) ) )
    {
        dwID[0] = dwID[1] = 0;
        return dwID[uIndex];
    }

    // Obtain the IME file name
    if ( !_ImmGetIMEFileNameA( m_hklCurrent, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
    {
        dwID[0] = dwID[1] = 0;
        return dwID[uIndex];
    }

    // Check for IME that doesn't implement reading string API
    if ( !_GetReadingString )
    {
        if( ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
            ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) )
        {
            dwID[0] = dwID[1] = 0;
            return dwID[uIndex];
        }
    }

    dwVerSize = _GetFileVersionInfoSizeA( szTmp, &dwVerHandle );
    if( dwVerSize )
    {
        lpVerBuffer = HeapAlloc( GetProcessHeap(), 0, dwVerSize );
        if( lpVerBuffer )
        {
            if( _GetFileVersionInfoA( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) )
            {
                if( _VerQueryValueA( lpVerBuffer, "\\", &lpVerData, &cbVerData ) )
                {
                    DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
                    dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
                    if( _GetReadingString
                        ||
                        ( GetLanguage() == LANG_CHT &&
                          ( dwVer == MAKEIMEVERSION(4, 2) || 
                            dwVer == MAKEIMEVERSION(4, 3) || 
                            dwVer == MAKEIMEVERSION(4, 4) || 
                            dwVer == MAKEIMEVERSION(5, 0) ||
                            dwVer == MAKEIMEVERSION(5, 1) ||
                            dwVer == MAKEIMEVERSION(5, 2) ||
                            dwVer == MAKEIMEVERSION(6, 0) ) )
                        ||
                        ( GetLanguage() == LANG_CHS &&
                          ( dwVer == MAKEIMEVERSION(4, 1) ||
                            dwVer == MAKEIMEVERSION(4, 2) ||
                            dwVer == MAKEIMEVERSION(5, 3) ) )
                      )
                    {
                        dwID[0] = dwVer | GetLanguage();
                        dwID[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
                    }
                }
            }
            HeapFree( GetProcessHeap(), 0, lpVerBuffer );
        }
    }

    return dwID[uIndex];

}


VOID CInputBox_Generic::CheckInputLocale(VOID)
{
	static HKL hklPrev = 0;
    m_hklCurrent = GetKeyboardLayout( 0 );
    if ( hklPrev == m_hklCurrent )
        return;

    hklPrev = m_hklCurrent;
    switch ( GetPrimaryLanguage() )
    {
        // Simplified Chinese
        case LANG_CHINESE:
            m_bVerticalCand = true;
            switch ( GetSubLanguage() )
            {
                case SUBLANG_CHINESE_SIMPLIFIED:
                    m_wszCurrIndicator = m_aszIndicator[INDICATOR_CHS];
                    m_bVerticalCand = GetImeId() == 0;
                    break;
                case SUBLANG_CHINESE_TRADITIONAL:
                    m_wszCurrIndicator = m_aszIndicator[INDICATOR_CHT];
                    break;
                default:    // unsupported sub-language
                    m_wszCurrIndicator = m_aszIndicator[INDICATOR_NON_IME];
                    break;
            }
            break;
        // Korean
        case LANG_KOREAN:
            m_wszCurrIndicator = m_aszIndicator[INDICATOR_KOREAN];
            m_bVerticalCand = false;
            break;
        // Japanese
        case LANG_JAPANESE:
            m_wszCurrIndicator = m_aszIndicator[INDICATOR_JAPANESE];
            m_bVerticalCand = true;
            break;
        default:
            // A non-IME language.  Obtain the language abbreviation
            // and store it for rendering the indicator later.
            m_wszCurrIndicator = m_aszIndicator[INDICATOR_NON_IME];
    }

    // If non-IME, use the language abbreviation.
    if( m_wszCurrIndicator == m_aszIndicator[INDICATOR_NON_IME] )
    {
        WCHAR wszLang[5];
        GetLocaleInfoW( MAKELCID( LOWORD( m_hklCurrent ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszLang, 5 );
        m_wszCurrIndicator[0] = wszLang[0];
        m_wszCurrIndicator[1] = towlower( wszLang[1] );
    }
}

VOID CInputBox_Generic::CheckToggleState(VOID)
{
	CheckInputLocale();
    bool bIme = _ImmIsIME( m_hklCurrent ) != 0;
    m_bChineseIME = ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

    HIMC hImc;
   if( NULL != ( hImc = _ImmGetContext( m_hWindow ) ) )
    {
        if( m_bChineseIME )
        {
            DWORD dwConvMode, dwSentMode;
            _ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
            m_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;
        }
        else
        {
			DWORD dwConvMode, dwSentMode;
			_ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
			m_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;

			if( m_bSaveImeState )
			{
				if( m_SavedImeState != m_ImeState )
				{
					m_SavedImeState = m_ImeState;
					m_SigIMEChanged( (INT)m_SavedImeState );
				}
			}

			//m_ImeState = ( bIme && _ImmGetOpenStatus( hImc ) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
        }
        _ImmReleaseContext( m_hWindow, hImc );
    }
    else
        m_ImeState = IMEUI_STATE_OFF;
}

VOID CInputBox_Generic::SetupImeApi(VOID)
{
	 char szImeFile[GUI_MAX_PATH];

    _GetReadingString = NULL;
    _ShowReadingWindow = NULL;
    if( _ImmGetIMEFileNameA( m_hklCurrent, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) == 0 )
        return;

    if( m_hDllIme ) FreeLibrary( m_hDllIme );
    m_hDllIme = LoadLibraryA( szImeFile );
    if ( !m_hDllIme )
        return;
    _GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
        ( GetProcAddress( m_hDllIme, "GetReadingString" ) );
    _ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
        ( GetProcAddress( m_hDllIme, "ShowReadingWindow" ) );
}

VOID CInputBox_Generic::GetReadingWindowOrientation( DWORD dwId )
{
	m_bHorizontalReading = ( m_hklCurrent == _CHS_HKL ) || ( m_hklCurrent == _CHT_HKL2 ) || ( dwId == 0 );
    if( !m_bHorizontalReading && ( dwId & 0x0000FFFF ) == LANG_CHT )
    {
        WCHAR wszRegPath[GUI_MAX_PATH];
        HKEY hKey;
        DWORD dwVer = dwId & 0xFFFF0000;
        StringCchCopy( wszRegPath, GUI_MAX_PATH, L"software\\microsoft\\windows\\currentversion\\" );
        StringCchCat( wszRegPath, GUI_MAX_PATH, ( dwVer >= MAKEIMEVERSION( 5, 1 ) ) ? L"MSTCIPH" : L"TINTLGNT" );
        LONG lRc = RegOpenKeyExW( HKEY_CURRENT_USER, wszRegPath, 0, KEY_READ, &hKey );
        if (lRc == ERROR_SUCCESS)
        {
            DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
            lRc = RegQueryValueExW( hKey, L"Keyboard Mapping", NULL, &dwType, (PBYTE)&dwMapping, &dwSize );
            if (lRc == ERROR_SUCCESS)
            {
                if ( ( dwVer <= MAKEIMEVERSION( 5, 0 ) && 
                       ( (BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23 ) )
                     ||
                     ( ( dwVer == MAKEIMEVERSION( 5, 1 ) || dwVer == MAKEIMEVERSION( 5, 2 ) ) &&
                       (BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24 )
                   )
                {
                    m_bHorizontalReading = true;
                }
            }
            RegCloseKey( hKey );
        }
    }
}

VOID CInputBox_Generic::TruncateCompString( bool bUseBackSpace /*= true*/, int iNewStrLen /*= 0*/ )
{
	if( !m_bInsertOnType )
        return;

    int cc = (int) wcslen( m_CompString.GetBuffer() );
//    assert( iNewStrLen == 0 || iNewStrLen >= cc );

    // Send right arrow keystrokes to move the caret
    //   to the end of the composition string.
    for (int i = 0; i < cc - m_nCompCaret; ++i )
        SendMessage( m_hWindow, WM_KEYDOWN, VK_RIGHT, 0 );
    SendMessage( m_hWindow, WM_KEYUP, VK_RIGHT, 0 );

    if( bUseBackSpace || m_bInsertMode )
        iNewStrLen = 0;

    // The caller sets bUseBackSpace to false if there's possibility of sending
    // new composition string to the app right after this function call.
    // 
    // If the app is in overwriting mode and new comp string is 
    // shorter than current one, delete previous comp string 
    // till it's same long as the new one. Then move caret to the beginning of comp string.
    // New comp string will overwrite old one.
    if( iNewStrLen < cc )
    {
        for( int i = 0; i < cc - iNewStrLen; ++i )
        {
            SendMessage( m_hWindow, WM_KEYDOWN, VK_BACK, 0 );  // Backspace character
            SendMessageW( m_hWindow, WM_CHAR, VK_BACK, 0 );
        }
        SendMessage( m_hWindow, WM_KEYUP, VK_BACK, 0 );
    }
    else
        iNewStrLen = cc;

    // Move the caret to the beginning by sending left keystrokes
    for (int i = 0; i < iNewStrLen; ++i )
        SendMessage( m_hWindow, WM_KEYDOWN, VK_LEFT, 0 );
    SendMessage( m_hWindow, WM_KEYUP, VK_LEFT, 0 );
}


VOID CInputBox_Generic::SendCompString(HWND hWnd)
{
	for( int i = 0; i < lstrlen( m_CompString.GetBuffer() ); ++i )
        OnMsgProc( hWnd, WM_CHAR, (WPARAM)m_CompString[i], 0 );
}

VOID CInputBox_Generic::ResetCompositionString(VOID)
{
    m_nCompCaret = 0;
    m_CompString.SetText( L"" );
    ZeroMemory( m_abCompStringAttr, sizeof(m_abCompStringAttr) );
}

VOID CInputBox_Generic::GetPrivateReadingString(VOID)
{
	DWORD dwId = GetImeId();
    if( !dwId )
    {
        m_bShowReadingWindow = false;
        return;
    }

    HIMC hImc;
    hImc = _ImmGetContext( m_hWindow );
    if( !hImc )
    {
        m_bShowReadingWindow = false;
        return;
    }

    DWORD dwReadingStrLen = 0;
    DWORD dwErr = 0;
    WCHAR *pwszReadingStringBuffer = NULL;  // Buffer for when the IME supports GetReadingString()
    WCHAR *wstr = 0;
    bool bUnicodeIme = false;  // Whether the IME context component is Unicode.
    INPUTCONTEXT *lpIC = NULL;

    if( _GetReadingString )
    {
        UINT uMaxUiLen;
        BOOL bVertical;
        // Obtain the reading string size
        dwReadingStrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );
        if( dwReadingStrLen )
        {
            wstr = pwszReadingStringBuffer = (LPWSTR)HeapAlloc( GetProcessHeap(), 0, sizeof(WCHAR) * dwReadingStrLen );
            if( !pwszReadingStringBuffer )
            {
                // Out of memory. Exit.
                _ImmReleaseContext( m_hWindow, hImc );
                return;
            }

            // Obtain the reading string
            dwReadingStrLen = _GetReadingString( hImc, dwReadingStrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
        }

        m_bHorizontalReading = !bVertical;
        bUnicodeIme = true;
    }
    else
    {
        // IMEs that doesn't implement Reading String API

        lpIC = _ImmLockIMC( hImc );
        
        LPBYTE p = 0;
        switch( dwId )
        {
            case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
            case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
            case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 24 );
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
                dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
                wstr = (WCHAR *)( p + 56 );
                bUnicodeIme = true;
                break;

            case IMEID_CHT_VER50: // 5.0.x.x // WinME
                p = *(LPBYTE *)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 3 * 4 );
                if( !p ) break;
                p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
                dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
                wstr = (WCHAR *)(p + 1*4 + (16*2+2*4) + 5*4);
                bUnicodeIme = false;
                break;

            case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
            case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
            case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 4);
                if( !p ) break;
                p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
                dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
                wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
                bUnicodeIme = true;
                break;

            // the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
            case IMEID_CHS_VER41:
            {
                int nOffset;
                nOffset = ( GetImeId( 1 ) >= 0x00000002 ) ? 8 : 7;

                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + nOffset * 4);
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 7*4 + 16*2*4);
                dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
                dwErr = __min( dwErr, dwReadingStrLen );
                wstr = (WCHAR *)(p + 6*4 + 16*2*1);
                bUnicodeIme = true;
                break;
            }

            case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
            {
                OSVERSIONINFOW osi;
                osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
                GetVersionExW( &osi );

                int nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(WCHAR) : sizeof(char);
                p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
                if( !p ) break;
                dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
                dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
                wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
                bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
            }
        }   // switch
    }

    // Copy the reading string to the candidate list first
    m_sCandList.awszCandidate[0][0] = 0;
    m_sCandList.awszCandidate[1][0] = 0;
    m_sCandList.awszCandidate[2][0] = 0;
    m_sCandList.awszCandidate[3][0] = 0;
    m_sCandList.dwCount = dwReadingStrLen;
    m_sCandList.dwSelection = (DWORD)-1; // do not select any char
    if( bUnicodeIme )
    {
        UINT i;
        for( i = 0; i < dwReadingStrLen; ++i ) // dwlen > 0, if known IME
        {
            if( dwErr <= i && m_sCandList.dwSelection == (DWORD)-1 )
            {
                // select error char
                m_sCandList.dwSelection = i;
            }

            m_sCandList.awszCandidate[i][0] = wstr[i];
            m_sCandList.awszCandidate[i][1] = 0;
        }
        m_sCandList.awszCandidate[i][0] = 0;
    }
    else
    {
        char *p = (char *)wstr;
        DWORD i, j;
        for( i = 0, j = 0; i < dwReadingStrLen; ++i, ++j ) // dwlen > 0, if known IME
        {
            if( dwErr <= i && m_sCandList.dwSelection == (DWORD)-1 )
            {
                m_sCandList.dwSelection = j;
            }
            // Obtain the current code page
            WCHAR wszCodePage[8];
            UINT uCodePage = CP_ACP;  // Default code page
            if( GetLocaleInfoW( MAKELCID( GetLanguage(), SORT_DEFAULT ),
                                LOCALE_IDEFAULTANSICODEPAGE,
                                wszCodePage,
                                sizeof(wszCodePage)/sizeof(wszCodePage[0]) ) )
            {
                uCodePage = wcstoul( wszCodePage, NULL, 0 );
            }
            MultiByteToWideChar( uCodePage, 0, p + i, IsDBCSLeadByteEx( uCodePage, p[i] ) ? 2 : 1,
                                 m_sCandList.awszCandidate[j], 1 );
            if( IsDBCSLeadByteEx( uCodePage, p[i] ) )
                ++i;
        }
        m_sCandList.awszCandidate[j][0] = 0;
        m_sCandList.dwCount = j;
    }
    if( !_GetReadingString )
    {
        _ImmUnlockIMCC( lpIC->hPrivate );
        _ImmUnlockIMC( hImc );
        GetReadingWindowOrientation( dwId );
    }
    _ImmReleaseContext( m_hWindow, hImc );

    if( pwszReadingStringBuffer )
        HeapFree( GetProcessHeap(), 0, pwszReadingStringBuffer );

    // Copy the string to the reading string buffer
    if( m_sCandList.dwCount > 0 )
        m_bShowReadingWindow = true;
    else
        m_bShowReadingWindow = false;
    if( m_bHorizontalReading )
    {
        m_sCandList.nReadingError = -1;
        m_wszReadingString[0] = 0;
        for( UINT i = 0; i < m_sCandList.dwCount; ++i )
        {
            if( m_sCandList.dwSelection == i )
                m_sCandList.nReadingError = lstrlen( m_wszReadingString );
            StringCchCat( m_wszReadingString, 32, m_sCandList.awszCandidate[i] );
        }
    }

    m_sCandList.dwPageSize = MAX_CANDLIST;
}


VOID CInputBox_Generic::ResetCaretBlink(VOID)
{
	
}

VOID CInputBox_Generic::PlaceCaret(int nCP)
{
	CUniBuffer *pOutBuffer = GetOutBuffer();

	assert( nCP >= 0 && nCP <= pOutBuffer->GetTextSize() );
    m_nCaret = nCP;

    // Obtain the X offset of the character.
    int nX1st, nX, nX2;
    pOutBuffer->CPtoX( m_nFirstVisible, FALSE, &nX1st );  // 1st visible char
    pOutBuffer->CPtoX( nCP, FALSE, &nX );  // LEAD
    // If nCP is the NULL terminator, get the leading edge instead of trailing.
    if( nCP == pOutBuffer->GetTextSize() )
        nX2 = nX;
    else
        pOutBuffer->CPtoX( nCP, TRUE, &nX2 );  // TRAIL

    // If the left edge of the char is smaller than the left edge of the 1st visible char,
    // we need to scroll left until this char is visible.
    if( nX < nX1st )
    {
        // Simply make the first visible character the char at the new caret position.
        m_nFirstVisible = nCP;
    }
    else
	{
		CRectangle rect = m_pInputBox->GetClientRect(); 
		// If the right of the character is bigger than the offset of the control's
		// right edge, we need to scroll right to this character.
		if( nX2 > nX1st + rect.GetWidth() )
		{
			// Compute the X of the new left-most pixel
			int nXNewLeft = nX2 - rect.GetWidth();

			// Compute the char position of this character
			int nCPNew1st, nNewTrail;
			pOutBuffer->XtoCP( nXNewLeft, &nCPNew1st, &nNewTrail );

			// If this coordinate is not on a character border,
			// start from the next character so that the caret
			// position does not fall outside the text rectangle.
			int nXNew1st;
			pOutBuffer->CPtoX( nCPNew1st, FALSE, &nXNew1st );
			if( nXNew1st < nXNewLeft )
				++nCPNew1st;

			m_nFirstVisible = nCPNew1st;
		}
	}
}

VOID CInputBox_Generic::DeleteSelectionText(VOID)
{
	int nFirst = __min( m_nCaret, m_nSelStart );
    int nLast = __max( m_nCaret, m_nSelStart );
    // Update caret and selection
    PlaceCaret( nFirst );
    m_nSelStart = m_nCaret;
    // Remove the characters
    for( int i = nFirst; i < nLast; ++i )
	{
        m_Buffer.RemoveChar( nFirst );
		m_PwBuffer.RemoveChar( nFirst );
	}
}

VOID CInputBox_Generic::CopyToClipboard(VOID)
{
	// Copy the selection text to the clipboard
    if( m_nCaret != m_nSelStart && OpenClipboard( NULL ) )
    {
        EmptyClipboard();

        HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE, sizeof(WCHAR) * ( m_Buffer.GetTextSize() + 1 ) );
        if( hBlock )
        {
            WCHAR *pwszText = (WCHAR*)GlobalLock( hBlock );
            if( pwszText )
            {
                int nFirst = __min( m_nCaret, m_nSelStart );
                int nLast = __max( m_nCaret, m_nSelStart );
                if( nLast - nFirst > 0 )
                    CopyMemory( pwszText, m_Buffer.GetBuffer() + nFirst, (nLast - nFirst) * sizeof(WCHAR) );
                pwszText[nLast - nFirst] = L'\0';  // Terminate it
                GlobalUnlock( hBlock );
            }
            SetClipboardData( CF_UNICODETEXT, hBlock );
        }
        CloseClipboard();
        // We must not free the object until CloseClipboard is called.
        if( hBlock )
            GlobalFree( hBlock );
    }
}


VOID CInputBox_Generic::PasteFromClipboard(VOID)
{
    DeleteSelectionText();

    if( OpenClipboard( NULL ) )
    {
        HANDLE handle = GetClipboardData( CF_UNICODETEXT );
        if( handle && !IsReadOnly() )
        {
            // Convert the ANSI string to Unicode, then
            // insert to our buffer.
            WCHAR *pwszText = (WCHAR*)GlobalLock( handle );
            if( pwszText )
            {
                // Copy all characters up to null.

				int nLen = lstrlenW(pwszText); 
				if(m_Buffer.GetTextSize() + nLen > m_nMaxLength)
				{
					nLen = m_nMaxLength - m_Buffer.GetTextSize();
					if( m_Buffer.InsertString(m_nCaret, pwszText, nLen) )
					{
						if(IsPasswordMode())
						{
							for(int i = 0; i < nLen; i++)
								m_PwBuffer.InsertChar(m_nCaret+i, L'*');
						}

				        PlaceCaret( m_nCaret + nLen );
					}
				}
				else
				{
			        if( m_Buffer.InsertString( m_nCaret, pwszText ) )
					{
						int nLen = lstrlenW( pwszText );
						if(IsPasswordMode())
						{
							for(int i = 0; i < nLen; i++)
								m_PwBuffer.InsertChar(m_nCaret+i, L'*');
						}

				        PlaceCaret( m_nCaret + nLen );
					}
				}
                m_nSelStart = m_nCaret;
                GlobalUnlock( handle );
            }
        }
        CloseClipboard();
    }
}


LRESULT CInputBox_Generic::OnStaticMsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HIMC hImc;

    if( !m_bEnableImeSystem )
        return false;

    switch( uMsg )
    {
        case WM_ACTIVATEAPP:
            if( wParam )
            {
                // Populate m_Locale with the list of keyboard layouts.
                UINT cKL = GetKeyboardLayoutList( 0, NULL );
                m_Locale.clear();
                HKL *phKL = NTL_NEW HKL[cKL];
                if( phKL )
                {
                    GetKeyboardLayoutList( cKL, phKL );
                    for( UINT i = 0; i < cKL; ++i )
                    {
                        SInputLocale Locale;

                        // Filter out East Asian languages that are not IME.
                        if( ( PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_CHINESE ||
                              PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_JAPANESE ||
                              PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_KOREAN ) &&
                              !_ImmIsIME( phKL[i] ) )
                              continue;

                        // If this language is already in the list, don't add it again.
                        bool bBreak = false;
                        for( int e = 0; e < (int)m_Locale.size(); ++e )
						{
                            if( LOWORD( m_Locale[e].m_hKL ) == LOWORD( phKL[i] ) )
                            {
                                bBreak = true;
                                break;
                            }
						}
                        if( bBreak )
                            break;

                        Locale.m_hKL = phKL[i];
                        WCHAR wszDesc[128] = L"";
                        switch( PRIMARYLANGID( LOWORD( phKL[i] ) ) )
                        {
                            // Simplified Chinese
                            case LANG_CHINESE:
                                switch( SUBLANGID( LOWORD( phKL[i] ) ) )
                                {
                                    case SUBLANG_CHINESE_SIMPLIFIED:
                                        StringCchCopy( Locale.m_wszLangAbb, 3, m_aszIndicator[INDICATOR_CHS] );
                                        break;
                                    case SUBLANG_CHINESE_TRADITIONAL:
                                        StringCchCopy( Locale.m_wszLangAbb, 3, m_aszIndicator[INDICATOR_CHT] );
                                        break;
                                    default:    // unsupported sub-language
                                        GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszDesc, 128 );
                                        Locale.m_wszLangAbb[0] = wszDesc[0];
                                        Locale.m_wszLangAbb[1] = towlower( wszDesc[1] );
                                        Locale.m_wszLangAbb[2] = L'\0';
                                        break;
                                }
                                break;
                            // Korean
                            case LANG_KOREAN:
                                StringCchCopy( Locale.m_wszLangAbb, 3, m_aszIndicator[INDICATOR_KOREAN] );
                                break;
                            // Japanese
                            case LANG_JAPANESE:
                                StringCchCopy( Locale.m_wszLangAbb, 3, m_aszIndicator[INDICATOR_JAPANESE] );
                                break;         
                            default:
                                // A non-IME language.  Obtain the language abbreviation
                                // and store it for rendering the indicator later.
                                GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszDesc, 128 );
                                Locale.m_wszLangAbb[0] = wszDesc[0];
                                Locale.m_wszLangAbb[1] = towlower( wszDesc[1] );
                                Locale.m_wszLangAbb[2] = L'\0';
                                break;
                        }

                        GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SLANGUAGE, wszDesc, 128 );
                        StringCchCopy( Locale.m_wszLang, 64, wszDesc );

                        m_Locale.push_back( Locale );
                    }

					NTL_ARRAY_DELETE( phKL );
                }
            }
            break;

        case WM_INPUTLANGCHANGE:
            {
                UINT uLang = GetPrimaryLanguage();
                CheckToggleState();
                if ( uLang != GetPrimaryLanguage() )
                {
                    // Korean IME always inserts on keystroke.  Other IMEs do not.
                    m_bInsertOnType = ( GetPrimaryLanguage() == LANG_KOREAN );
                }

                // IME changed.  Setup the new IME.
                SetupImeApi();
               if( _ShowReadingWindow )
                {
                    if ( NULL != ( hImc = _ImmGetContext( hWnd ) ) )
                    {
                        _ShowReadingWindow( hImc, false );
                        _ImmReleaseContext( hWnd, hImc );
                    }
                }
            }
            return true;

        case WM_IME_SETCONTEXT:
            //
            // We don't want anything to display, so we have to clear this
            //
            lParam = 0;
            return false;

        // Handle WM_IME_STARTCOMPOSITION here since
        // we do not want the default IME handler to see
        // this when our fullscreen app is running.
        case WM_IME_STARTCOMPOSITION:
            ResetCompositionString();
            // Since the composition string has its own caret, we don't render
            // the edit control's own caret to aVOID double carets on screen.
            m_bHideCaret = true;
			return true;

        case WM_IME_COMPOSITION:
			return false;
    }

    return false;
}

bool CInputBox_Generic::OnMsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(!m_pInputBox->HasFocus() || !m_pInputBox->IsEnabled() || !m_pInputBox->IsVisible())
		return false;

	switch( uMsg )
	{
		case WM_CHAR:
        {
            switch( (WCHAR)wParam )
            {
                // Backspace
                case VK_BACK:
                {
					if(!IsReadOnly())
					{
						// If there's a selection, treat this
						// like a delete key.
						if( m_nCaret != m_nSelStart )
						{
							DeleteSelectionText();
						}
						else
						{
							if( m_nCaret > 0 )
							{
								// Move the caret, then delete the char.
								
								if(m_bMultiLine && m_nCaret >= 2 && 
									m_Buffer[m_nCaret-2] == L'\\' && m_Buffer[m_nCaret-1] == L'n')
								{
									PlaceCaret( m_nCaret - 1 );
									m_Buffer.RemoveChar( m_nCaret );
									if(IsPasswordMode())
										m_PwBuffer.RemoveChar( m_nCaret );

									PlaceCaret( m_nCaret - 1 );
									m_Buffer.RemoveChar( m_nCaret );
									if(IsPasswordMode())
										m_PwBuffer.RemoveChar( m_nCaret );

									m_nSelStart = m_nCaret;
								}
								else
								{
									PlaceCaret( m_nCaret - 1 );
									m_nSelStart = m_nCaret;
									m_Buffer.RemoveChar( m_nCaret );
									if(IsPasswordMode())
										m_PwBuffer.RemoveChar( m_nCaret );
								}
							}
							ResetCaretBlink();
						}

						m_bAutoScrollDec = true;

						TextOut();
					}
			
                    break;
                }

                case 24:        // Ctrl-X Cut
                case VK_CANCEL: // Ctrl-C Copy
                {
                    CopyToClipboard();

                    // If the key is Ctrl-X, delete the selection too.
                    if( (WCHAR)wParam == 24 )
                    {
                        DeleteSelectionText();
                      
                    }

					TextOut();

                    break;
                }

                // Ctrl-V Paste
                case 22:
                {
					if(!IsReadOnly())
					{
						PasteFromClipboard();
						TextOut();
					}
                    break;
                }

                // Ctrl-A Select All
                case 1:
                    if( m_nSelStart == m_nCaret )
                    {
                        m_nSelStart = 0;
						CUniBuffer *pOutBuffer = GetOutBuffer();
                        PlaceCaret( pOutBuffer->GetTextSize() );
                    }

					TextOut();

                    break;

                case VK_RETURN:
                    // Invoke the callback when the user presses Enter.
					if(!IsReadOnly() && m_bMultiLine)
					{
						if( m_Buffer.GetTextSize() + 2 < m_nMaxLength)
						{
							m_Buffer.InsertString(m_nCaret, L"\\n");
							PlaceCaret( m_nCaret + 2 );

	//						m_PwBuffer.InsertChar(m_nCaret, L'n');
	//						PlaceCaret( m_nCaret + 1 );
							
							m_nSelStart = m_nCaret;
					
							ResetCaretBlink();

							TextOut();
						}
					}
                   break;

                // Junk characters we don't want in the string
                case 26:  // Ctrl Z
                case 2:   // Ctrl B
                case 14:  // Ctrl N
                case 19:  // Ctrl S
                case 4:   // Ctrl D
                case 6:   // Ctrl F
                case 7:   // Ctrl G
                case 10:  // Ctrl J
                case 11:  // Ctrl K
                case 12:  // Ctrl L
                case 17:  // Ctrl Q
                case 23:  // Ctrl W
                case 5:   // Ctrl E
                case 18:  // Ctrl R
                case 20:  // Ctrl T
                case 25:  // Ctrl Y
                case 21:  // Ctrl U
                case 9:   // Ctrl I
                case 15:  // Ctrl O
                case 16:  // Ctrl P
                case 27:  // Ctrl [
                case 29:  // Ctrl ]
                case 28:  // Ctrl \ 
                    break;

                default:
                {
					if(!IsReadOnly())
					{
						// If there's a selection and the user
						// starts to type, the selection should
						// be deleted.
						if( m_nCaret != m_nSelStart )
							DeleteSelectionText();

						// If we are in overwrite mode and there is already
						// a char at the caret's position, simply replace it.
						// Otherwise, we insert the char as normal.
						if( !m_bInsertMode && m_nCaret < m_Buffer.GetTextSize() )
						{
							if( IsNumberOnly() )
							{
								if( !iswdigit( (WCHAR)wParam ) )
									break;
							}

							m_Buffer[m_nCaret] = (WCHAR)wParam;
							if(IsPasswordMode())
								m_PwBuffer[m_nCaret] = L'*';

							PlaceCaret( m_nCaret + 1 );
							m_nSelStart = m_nCaret;
						}
						else
						{
							// Insert the char
							if( m_Buffer.GetTextSize() < m_nMaxLength)
							{
								if( IsNumberOnly() )
								{
									if( !iswdigit( (WCHAR)wParam ) )
										break;
								}

								if(m_Buffer.InsertChar( m_nCaret, (WCHAR)wParam ))
								{
									if(IsPasswordMode())
										m_PwBuffer.InsertChar(m_nCaret, L'*');

									PlaceCaret( m_nCaret + 1 );
									m_nSelStart = m_nCaret;


									if( !m_strKeyDownSound.empty() )
									{
										sNtlSoundPlayParameta tSoundParam;
										tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
										tSoundParam.pcFileName		= (char*)m_strKeyDownSound.c_str();

										GetSoundManager()->Play( &tSoundParam );
									}
								}
							}
						}
						ResetCaretBlink();

						TextOut();
					}
                }
            }
            return true;
        }

	}

	return false;
}


LRESULT CInputBox_Generic::OnIMEMsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(!m_pInputBox->HasFocus() || !m_pInputBox->IsEnabled() || !m_pInputBox->IsVisible())
		return FALSE;

	bool trappedData;
    bool *trapped = &trappedData;

	HIMC hImc;
    static LPARAM lAlt = 0x80000000, lCtrl = 0x80000000, lShift = 0x80000000;

	*trapped = false;

	switch( uMsg )
	{
		case WM_IME_COMPOSITION:
			{
				LONG lRet;  // Returned count in CHARACTERS
                WCHAR wszCompStr[MAX_COMPSTRING_SIZE];

				*trapped = true;
                if( NULL == ( hImc = _ImmGetContext( m_hWindow ) ) )
                {
                   break;
                }
	
				// composition string의 caret position을 얻는다.
				if ( lParam & GCS_CURSORPOS )
                {
                    m_nCompCaret = _ImmGetCompositionStringW( hImc, GCS_CURSORPOS, NULL, 0 );
                    if( m_nCompCaret < 0 )
                        m_nCompCaret = 0; // On error, set caret to pos 0.
                }

				if ( lParam & GCS_RESULTSTR )
                {
                    lRet = _ImmGetCompositionStringW( hImc, GCS_RESULTSTR, wszCompStr, sizeof( wszCompStr ) );
                    if( lRet > 0 )
                    {
                        lRet /= sizeof(WCHAR);
                        wszCompStr[lRet] = 0;  // Force terminate
                        TruncateCompString( false, (int)wcslen( wszCompStr ) );
                        m_CompString.SetText( wszCompStr );
                        SendCompString(hWnd);
                        ResetCompositionString();
                    }
                }

				//
                // Reads in the composition string.
                //
                if ( lParam & GCS_COMPSTR )
                {
                    //////////////////////////////////////////////////////
                    // Retrieve the latest user-selected IME candidates
                    lRet = _ImmGetCompositionStringW( hImc, GCS_COMPSTR, wszCompStr, sizeof( wszCompStr ) );
                    if( lRet > 0 )
                    {
						lRet /= sizeof(WCHAR);  // Convert size in byte to size in char
                        wszCompStr[lRet] = 0;  // Force terminate
                        //
                        // Remove the whole of the string
                        //
                        TruncateCompString( false, (int)wcslen( wszCompStr ) );

						m_CompString.SetText( wszCompStr );

						 // Older CHT IME uses composition string for reading string
                        if ( GetLanguage() == LANG_CHT && !GetImeId() )
                        {
                            if( lstrlen( m_CompString.GetBuffer() ) )
                            {
                                m_sCandList.dwCount = 4;             // Maximum possible length for reading string is 4
                                m_sCandList.dwSelection = (DWORD)-1; // don't select any candidate

								// Copy the reading string to the candidate list

                                for( int i = 3; i >= 0; --i )
                                {
                                    if( i > lstrlen( m_CompString.GetBuffer() ) - 1 )
                                        m_sCandList.awszCandidate[i][0] = 0;  // Doesn't exist
                                    else
                                    {
                                        m_sCandList.awszCandidate[i][0] = m_CompString[i];
                                        m_sCandList.awszCandidate[i][1] = 0;
                                    }
                                }

								m_sCandList.dwPageSize = MAX_CANDLIST;
                                // Clear comp string after we are done copying
                                ZeroMemory( (LPVOID)m_CompString.GetBuffer(), 4 * sizeof(WCHAR) );
                                m_bShowReadingWindow = true;
                                GetReadingWindowOrientation( 0 );
                                if( m_bHorizontalReading )
                                {
                                    m_sCandList.nReadingError = -1;  // Clear error

                                    // Create a string that consists of the current
                                    // reading string.  Since horizontal reading window
                                    // is used, we take advantage of this by rendering
                                    // one string instead of several.
                                    //
                                    // Copy the reading string from the candidate list
                                    // to the reading string buffer.
                                    m_wszReadingString[0] = 0;
                                    for( UINT i = 0; i < m_sCandList.dwCount; ++i )
                                    {
                                        if( m_sCandList.dwSelection == i )
                                            m_sCandList.nReadingError = lstrlen( m_wszReadingString );
                                        StringCchCat( m_wszReadingString, 32, m_sCandList.awszCandidate[i] );
                                    }
                                }
							}
							else
							{
								m_sCandList.dwCount = 0;
                                m_bShowReadingWindow = false;
							}
						}

						if( m_bInsertOnType )
                        {
                            // Send composition string to the edit control
                            SendCompString(hWnd);
                            // Restore the caret to the correct location.
                            // It's at the end right now, so compute the number
                            // of times left arrow should be pressed to
                            // send it to the original position.
                            int nCount = lstrlen( m_CompString.GetBuffer() + m_nCompCaret );
                            // Send left keystrokes
                            for( int i = 0; i < nCount; ++i )
                                SendMessage( hWnd, WM_KEYDOWN, VK_LEFT, 0 );
                            SendMessage( hWnd, WM_KEYUP, VK_LEFT, 0 );
                        }
					}
					else
					{
						m_CompString.Clear(); 
					}

                    ResetCaretBlink();
				}

				 // Retrieve comp string attributes
                if( lParam & GCS_COMPATTR )
                {
                    lRet = _ImmGetCompositionStringW( hImc, GCS_COMPATTR, m_abCompStringAttr, sizeof( m_abCompStringAttr ) );
                    if( lRet > 0 )
                        m_abCompStringAttr[lRet] = 0;  // ??? Is this needed for attributes?
                }

                // Retrieve clause information
                if( lParam & GCS_COMPCLAUSE )
                {
                    lRet = _ImmGetCompositionStringW(hImc, GCS_COMPCLAUSE, m_adwCompStringClause, sizeof( m_adwCompStringClause ) );
                    m_adwCompStringClause[lRet / sizeof(DWORD)] = 0;  // Terminate
                }

                _ImmReleaseContext( hWnd, hImc );

			}
			break;
		case WM_IME_ENDCOMPOSITION:
			{
				TruncateCompString();
				ResetCompositionString();
				// We can show the edit control's caret again.
				m_bHideCaret = false;
				// Hide reading window
				m_bShowReadingWindow = false;
			}
            break;
        case WM_IME_NOTIFY:
            switch( wParam )
            {		
				//case IMN_SETCONVERSIONMODE:
                case IMN_SETOPENSTATUS:
                    CheckToggleState();
                    break;
				/*
                case IMN_OPENCANDIDATE:
                case IMN_CHANGECANDIDATE:
                {
                    m_sCandList.bShowWindow = true;
                    *trapped = true;
                    if( NULL == ( hImc = _ImmGetContext( hWnd ) ) )
                        break;

                    LPCANDIDATELIST lpCandList = NULL;
                    DWORD dwLenRequired;

                    m_bShowReadingWindow = false;
                    // Retrieve the candidate list
                    dwLenRequired = _ImmGetCandidateListW( hImc, 0, NULL, 0 );
                    if( dwLenRequired )
                    {
                        lpCandList = (LPCANDIDATELIST)HeapAlloc( GetProcessHeap(), 0, dwLenRequired );
                        dwLenRequired = _ImmGetCandidateListW( hImc, 0, lpCandList, dwLenRequired );
                    }

                    if( lpCandList )
                    {
                        // Update candidate list data
                        m_sCandList.dwSelection = lpCandList->dwSelection;
                        m_sCandList.dwCount = lpCandList->dwCount;

                        int nPageTopIndex = 0;
                        m_sCandList.dwPageSize = __min( lpCandList->dwPageSize, MAX_CANDLIST );
                        if( GetPrimaryLanguage() == LANG_JAPANESE )
                        {
                            // Japanese IME organizes its candidate list a little
                            // differently from the other IMEs.
                            nPageTopIndex = ( m_sCandList.dwSelection / m_sCandList.dwPageSize ) * m_sCandList.dwPageSize;
                        }
                        else
                            nPageTopIndex = lpCandList->dwPageStart;

                        // Make selection index relative to first entry of page
                        m_sCandList.dwSelection = ( GetLanguage() == LANG_CHS && !GetImeId() ) ? (DWORD)-1
                                                 : m_sCandList.dwSelection - nPageTopIndex;

                        ZeroMemory( m_sCandList.awszCandidate, sizeof(m_sCandList.awszCandidate) );
                        for( UINT i = nPageTopIndex, j = 0;
                            (DWORD)i < lpCandList->dwCount && j < m_sCandList.dwPageSize;
                            i++, j++ )
                        {
                            // Initialize the candidate list strings
                            LPWSTR pwsz = m_sCandList.awszCandidate[j];
                            // For every candidate string entry,
                            // write [index] + Space + [String] if vertical,
                            // write [index] + [String] + Space if horizontal.
                            *pwsz++ = (WCHAR)( L'0' + ( (j + 1) % 10 ) );  // Index displayed is 1 based
                            if( m_bVerticalCand )
                                *pwsz++ = L' ';
                            WCHAR *pwszNewCand = (LPWSTR)( (LPBYTE)lpCandList + lpCandList->dwOffset[i] );
                            while ( *pwszNewCand )
                                *pwsz++ = *pwszNewCand++;
                            if( !m_bVerticalCand )
                                *pwsz++ = L' ';
                            *pwsz = 0;  // Terminate
                        }

                        // Make dwCount in m_sCandList be number of valid entries in the page.
                        m_sCandList.dwCount = lpCandList->dwCount - lpCandList->dwPageStart;
                        if( m_sCandList.dwCount > lpCandList->dwPageSize )
                            m_sCandList.dwCount = lpCandList->dwPageSize;

                        HeapFree( GetProcessHeap(), 0, lpCandList );
                        _ImmReleaseContext( hWnd, hImc );

                        // Korean and old Chinese IME can't have selection.
                        // User must use the number hotkey or Enter to select
                        // a candidate.
                        if( GetPrimaryLanguage() == LANG_KOREAN ||
                            GetLanguage() == LANG_CHT && !GetImeId() )
                        {
                            m_sCandList.dwSelection = (DWORD)-1;
                        }

                        // Initialize m_sCandList.HoriCand if we have a
                        // horizontal candidate window.
                        if( !m_bVerticalCand )
                        {
                            WCHAR wszCand[256] = L"";

                            m_sCandList.nFirstSelected = 0;
                            m_sCandList.nHoriSelectedLen = 0;
                            for( UINT i = 0; i < MAX_CANDLIST; ++i )
                            {
                                if( m_sCandList.awszCandidate[i][0] == L'\0' )
                                    break;

                                WCHAR wszEntry[32];
                                StringCchPrintf( wszEntry, 32, L"%s ", m_sCandList.awszCandidate[i] );
                                // If this is the selected entry, mark its char position.
                                if( m_sCandList.dwSelection == i )
                                {
                                    m_sCandList.nFirstSelected = lstrlen( wszCand );
                                    m_sCandList.nHoriSelectedLen = lstrlen( wszEntry ) - 1;  // Minus space
                                }
                                StringCchCat( wszCand, 256, wszEntry );
                            }
                            wszCand[lstrlen(wszCand) - 1] = L'\0';  // Remove the last space
                            m_sCandList.HoriCand.SetText( wszCand );
                        }
                    }
                    break;
                }

                case IMN_CLOSECANDIDATE:
                {
                    m_sCandList.bShowWindow = false;
                    if( !m_bShowReadingWindow )
                    {
                        m_sCandList.dwCount = 0;
                        ZeroMemory( m_sCandList.awszCandidate, sizeof(m_sCandList.awszCandidate) );
                    }
                    *trapped = true;
                    break;
                }
				
                case IMN_PRIVATE:
                    {
                        if( !m_sCandList.bShowWindow )
                            GetPrivateReadingString();

                        // Trap some messages to hide reading window
                        DWORD dwId = GetImeId();
                        switch( dwId )
                        {
                            case IMEID_CHT_VER42:
                            case IMEID_CHT_VER43:
                            case IMEID_CHT_VER44:
                            case IMEID_CHS_VER41:
                            case IMEID_CHS_VER42:
                                if( ( lParam == 1 ) || ( lParam == 2 ) )
                                {
                                    *trapped = true;
                                }
                                break;

                            case IMEID_CHT_VER50:
                            case IMEID_CHT_VER51:
                            case IMEID_CHT_VER52:
                            case IMEID_CHT_VER60:
                            case IMEID_CHS_VER53:
                                if( (lParam == 16) || (lParam == 17) || (lParam == 26) || (lParam == 27) || (lParam == 28) )
                                {
                                    *trapped = true;
                                }
                                break;
                        }
                    }
                    break;

					*/

                default:
                    *trapped = true;
                    break;
            }
            break;

		// When Text Service Framework is installed in Win2K, Alt+Shift and Ctrl+Shift combination (to switch input
        // locale / keyboard layout) doesn't send WM_KEYUP message for the key that is released first. We need to check
        // if these keys are actually up whenever we receive key up message for other keys.
        case WM_KEYUP:
        case WM_SYSKEYUP:
            if ( !( lAlt & 0x80000000 ) && wParam != VK_MENU && ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) == 0 )
            {
                PostMessageW( GetFocus(), WM_KEYUP, (WPARAM)VK_MENU, ( lAlt & 0x01ff0000 ) | 0xC0000001 );
            }   
            else if ( !( lCtrl & 0x80000000 ) && wParam != VK_CONTROL && ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) == 0 )
            {
                PostMessageW( GetFocus(), WM_KEYUP, (WPARAM)VK_CONTROL, ( lCtrl & 0x01ff0000 ) | 0xC0000001 );
            }
            else if ( !( lShift & 0x80000000 ) && wParam != VK_SHIFT && ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) == 0 )
            {
                PostMessageW( GetFocus(), WM_KEYUP, (WPARAM)VK_SHIFT, ( lShift & 0x01ff0000 ) | 0xC0000001 );
            }
            // fall through WM_KEYDOWN / WM_SYSKEYDOWN
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            switch ( wParam )
            {
                case VK_MENU:
                    lAlt = lParam;
                    break;
                case VK_SHIFT:
                    lShift = lParam;
                    break;
                case VK_CONTROL:
                    lCtrl = lParam;
                    break;				
            }
		default:
			OnMsgProc(hWnd, uMsg, wParam, lParam);
			break;
	}

	if(*trapped)
		TextOut();

	return *trapped;
}

VOID CInputBox_Generic::OnMouseDown(const CKey &key)
{
	if(key.m_nID == UD_LEFT_BUTTON)
	{
		/*
		int nCompStrWidth;
        m_CompString.CPtoX( m_CompString.GetTextSize(), FALSE, &nCompStrWidth );
		
        if( s_ptCompString.x <= pt.x &&
            s_ptCompString.y <= pt.y &&
            s_ptCompString.x + nCompStrWidth > pt.x &&
            s_ptCompString.y + pFont->nHeight > pt.y )
        {
            int nCharBodyHit, nCharHit;
            int nTrail;

            // Determine the character clicked on.
            s_CompString.XtoCP( pt.x - s_ptCompString.x, &nCharBodyHit, &nTrail );
            if( nTrail && nCharBodyHit < s_CompString.GetTextSize() )
                nCharHit = nCharBodyHit + 1;
            else
                nCharHit = nCharBodyHit;

            // Now generate keypress events to move the comp string cursor
            // to the click point.  First, if the candidate window is displayed,
            // send Esc to close it.
            HIMC hImc = _ImmGetContext( m_hWindow );
            if( !hImc )
                return true;

            _ImmNotifyIME( hImc, NI_CLOSECANDIDATE, 0, 0 );
            _ImmReleaseContext( m_hWindow, hImc );

            switch( GetPrimaryLanguage() )
            {
                case LANG_JAPANESE:
                    // For Japanese, there are two cases.  If s_nFirstTargetConv is
                    // -1, the comp string hasn't been converted yet, and we use
                    // s_nCompCaret.  For any other value of s_nFirstTargetConv,
                    // the string has been converted, so we use clause information.

                    if( s_nFirstTargetConv != -1 )
                    {
                        int nClauseClicked = 0;
                        while( (int)s_adwCompStringClause[nClauseClicked + 1] <= nCharBodyHit )
                            ++nClauseClicked;

                        int nClauseSelected = 0;
                        while( (int)s_adwCompStringClause[nClauseSelected + 1] <= s_nFirstTargetConv )
                            ++nClauseSelected;

                        BYTE nVirtKey = nClauseClicked > nClauseSelected ? VK_RIGHT : VK_LEFT;
                        int nSendCount = abs( nClauseClicked - nClauseSelected );
                        while( nSendCount-- > 0 )
                            SendKey( nVirtKey );

                        return true;
                    }

                    // Not converted case. Fall thru to Chinese case.

                case LANG_CHINESE:
                {
                    // For Chinese, use s_nCompCaret.
                    BYTE nVirtKey = nCharHit > s_nCompCaret ? VK_RIGHT : VK_LEFT;
                    int nSendCount = abs( nCharHit - s_nCompCaret );
                    while( nSendCount-- > 0 )
                        SendKey( nVirtKey );
                    break;
                }
            }
			return;
		}

        

		// Check if the click is on top of the candidate window
        if( m_sCandList.bShowWindow && PtInRect( &m_sCandList.rcCandidate, pt ) )
        {
            if( m_bVerticalCand )
            {
                // Vertical candidate window

                // Compute the row the click is on
                int nRow = ( pt.y - m_sCandList.rcCandidate.top ) / pFont->nHeight;

                if( nRow < (int)m_sCandList.dwCount )
                {
                    // nRow is a valid entry.
                    // Now emulate keystrokes to select the candidate at this row.
                    switch( GetPrimaryLanguage() )
                    {
                        case LANG_CHINESE:
                        case LANG_KOREAN:
                            // For Chinese and Korean, simply send the number keystroke.
                            SendKey( (BYTE) ('0' + nRow + 1) );
                            break;

                        case LANG_JAPANESE:
                            // For Japanese, move the selection to the target row,
                            // then send Right, then send Left.

                            BYTE nVirtKey;
                            if( nRow > (int)m_sCandList.dwSelection )
                                nVirtKey = VK_DOWN;
                            else
                                nVirtKey = VK_UP;
                            int nNumToHit = abs( int( nRow - m_sCandList.dwSelection ) );
                            for( int nStrike = 0; nStrike < nNumToHit; ++nStrike )
                                SendKey( nVirtKey );

                            // Do this to close the candidate window without ending composition.
                            SendKey( VK_RIGHT );
                            SendKey( VK_LEFT );

                            break;
                    }
                }
            } else
            {
                // Horizontal candidate window

                // Determine which the character the click has hit.
                int nCharHit;
                int nTrail;
                m_sCandList.HoriCand.XtoCP( pt.x - m_sCandList.rcCandidate.left, &nCharHit, &nTrail );

                // Determine which candidate string the character belongs to.
                int nCandidate = m_sCandList.dwCount - 1;

                int nEntryStart = 0;
                for( UINT i = 0; i < m_sCandList.dwCount; ++i )
                {
                    if( nCharHit >= nEntryStart )
                    {
                        // Haven't found it.
                        nEntryStart += lstrlenW( m_sCandList.awszCandidate[i] ) + 1;  // plus space separator
                    } else
                    {
                        // Found it.  This entry starts at the right side of the click point,
                        // so the char belongs to the previous entry.
                        nCandidate = i - 1;
                        break;
                    }
                }

                // Now emulate keystrokes to select the candidate entry.
                switch( GetPrimaryLanguage() )
                {
                    case LANG_CHINESE:
                    case LANG_KOREAN:
                        // For Chinese and Korean, simply send the number keystroke.
                        SendKey( (BYTE) ('0' + nCandidate + 1) );
                        break;
                }
            }

            return true;
        }
		*/

	    m_bMouseDrag = true;
		m_pInputBox->CaptureMouse();

		if(m_bMultiLine)
		{
			SCaretMoveInfo sCaretInfo;
			MakeCurrentCaretInfo(sCaretInfo, (int)key.m_fX + m_ptScrollPos.x, (int)key.m_fY + m_ptScrollPos.y);
			SCaretMoveInfo sNewCaret = m_pMultiLine->FindStringIndex(&m_Buffer, sCaretInfo);

			PlaceCaret( sNewCaret.nCaretIdx );
			m_nSelStart = m_nCaret;
			ResetCaretBlink();
		}
		else
		{
			// Determine the character corresponding to the coordinates.
			int nCP, nTrail, nX1st;

			CUniBuffer *pOutBuffer = GetOutBuffer();
			pOutBuffer->CPtoX( m_nFirstVisible, FALSE, &nX1st );  // X offset of the 1st visible char

			if( SUCCEEDED( pOutBuffer->XtoCP( (int)key.m_fX + nX1st, &nCP, &nTrail ) ) )
			{
				// Cap at the NULL character.
				if( nTrail && nCP < pOutBuffer->GetTextSize() )
					PlaceCaret( nCP + 1 );
				else
					PlaceCaret( nCP );
				m_nSelStart = m_nCaret;
				ResetCaretBlink();
			}
		}

		TextOut();
	}

}

VOID CInputBox_Generic::OnMouseUp(const CKey &key)
{
	m_pInputBox->ReleaseMouse(); 
    m_bMouseDrag = false;
}

VOID CInputBox_Generic::OnMouseMove(int nFlag,int nPosX, int nPosY)
{
	if( m_bMouseDrag )
    {
        // Determine the character corresponding to the coordinates.

		if(m_bMultiLine)
		{
			SCaretMoveInfo sCaretInfo;
			MakeCurrentCaretInfo(sCaretInfo, nPosX + m_ptScrollPos.x, nPosY + m_ptScrollPos.y);

			if(sCaretInfo.nCaretPosX < 0)
				sCaretInfo.nCaretPosX = 0;

			if(sCaretInfo.nCaretPosY < 0)
				sCaretInfo.nCaretPosY = 0;

			SCaretMoveInfo sNewCaret = m_pMultiLine->FindStringIndex(&m_Buffer, sCaretInfo);

			if(m_nCaret != sNewCaret.nCaretIdx)
			{
				PlaceCaret( sNewCaret.nCaretIdx );
				TextOut();
			}
		}
		else
		{
			int nCP, nTrail, nX1st;

			CUniBuffer *pOutBuffer = GetOutBuffer();

			pOutBuffer->CPtoX( m_nFirstVisible, FALSE, &nX1st );  // X offset of the 1st visible char
			if( SUCCEEDED( pOutBuffer->XtoCP( nPosX + nX1st, &nCP, &nTrail ) ) )
			{
				// Cap at the NULL character.
				if( nTrail && nCP < pOutBuffer->GetTextSize() )
					PlaceCaret( nCP + 1 );
				else
					PlaceCaret( nCP );
			}

			TextOut();
		}
    }
}


VOID CInputBox_Generic::OnKeyDown(CComponent *pComp, CInputDevice *pDevice, const CKey& key)
{
	switch(key.m_dwVKey)
    {
        case VK_TAB:
            // We don't process Tab in case keyboard input is enabled and the user
            // wishes to Tab to other controls.
            break;

        case VK_HOME:
			if(m_bMultiLine)
			{
				SCaretMoveInfo sCaretInfo;
				MakeCurrentCaretInfo(sCaretInfo);
				SCaretMoveInfo sNewCaret = m_pMultiLine->Home(&m_Buffer, sCaretInfo);
				
				PlaceCaret( sNewCaret.nCaretIdx );
			}
			else
			{
			    PlaceCaret( 0 );
			}

            if( GetKeyState( VK_SHIFT ) >= 0 )
			{
                // Shift is not down. Update selection
                // start along with the caret.
                 m_nSelStart = m_nCaret;
			}
            ResetCaretBlink();
			TextOut();
            break;

        case VK_END:
			if(m_bMultiLine)
			{
				SCaretMoveInfo sCaretInfo;
				MakeCurrentCaretInfo(sCaretInfo);
				SCaretMoveInfo sNewCaret = m_pMultiLine->End(&m_Buffer, sCaretInfo);

				PlaceCaret( sNewCaret.nCaretIdx );
			}
			else
			{
				PlaceCaret( GetOutBuffer()->GetTextSize() );
			}

            if( GetKeyState( VK_SHIFT ) >= 0 )
			{
                // Shift is not down. Update selection
                // start along with the caret.
                m_nSelStart = m_nCaret;
			}
            ResetCaretBlink();
			TextOut();
            break;

        case VK_INSERT:
			if(!IsReadOnly())
			{
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control Insert. Copy to clipboard
					CopyToClipboard();
				}
				else
				{
					if( GetKeyState( VK_SHIFT ) < 0 )
					{
						// Shift Insert. Paste from clipboard
						PasteFromClipboard();
					} 
					else
					{
						// Toggle caret insert mode
						m_bInsertMode = !m_bInsertMode;
					}
				}

				TextOut();
			}

            break;

        case VK_DELETE:
			if(!IsReadOnly())
			{
				// Check if there is a text selection.
				if( m_nCaret != m_nSelStart )
				{
					DeleteSelectionText();
	        
				}
				else
				{
					// Deleting one character
					if(m_bMultiLine && m_nCaret <= m_Buffer.GetTextSize() - 2 && 
						m_Buffer[m_nCaret] == L'\\' && m_Buffer[m_nCaret+1] == L'n')
					{
						m_Buffer.RemoveChar( m_nCaret );
						m_Buffer.RemoveChar( m_nCaret );
						if(IsPasswordMode())
						{
							m_PwBuffer.RemoveChar( m_nCaret );
							m_PwBuffer.RemoveChar( m_nCaret );
						}
					}
					else
					{
						m_Buffer.RemoveChar( m_nCaret );
						if(IsPasswordMode())
							m_PwBuffer.RemoveChar( m_nCaret );
					}
				}
				ResetCaretBlink();

				m_bAutoScrollDec = true;
				TextOut();
			}

            break;

        case VK_LEFT:
            if( GetKeyState( VK_CONTROL ) < 0 )
            {
                // Control is down. Move the caret to a new item
                // instead of a character.
                GetOutBuffer()->GetPriorItemPos( m_nCaret, &m_nCaret );
                PlaceCaret( m_nCaret );
            }
            else
			{
				if( m_nCaret > 0 )
				{
					if(m_bMultiLine && m_nCaret >= 2 && 
						m_Buffer[m_nCaret-2] == L'\\' && m_Buffer[m_nCaret-1] == L'n')
						PlaceCaret( m_nCaret - 2 );
					else
						PlaceCaret( m_nCaret - 1 );
				}
			}

            if( GetKeyState( VK_SHIFT ) >= 0 )
			{
                // Shift is not down. Update selection
                // start along with the caret.
                m_nSelStart = m_nCaret;
			}

            ResetCaretBlink();
			
			TextOut();

            break;

        case VK_RIGHT:
            if( GetKeyState( VK_CONTROL ) < 0 )
            {
                // Control is down. Move the caret to a new item
                // instead of a character.
                GetOutBuffer()->GetNextItemPos( m_nCaret, &m_nCaret );
                PlaceCaret( m_nCaret );
            }
            else
			{
				if( m_bMultiLine && m_nCaret + 2 <= m_Buffer.GetTextSize() && 
					m_Buffer[m_nCaret] == L'\\' && m_Buffer[m_nCaret+1] == L'n')
				{
					PlaceCaret( m_nCaret + 2 );
				}
				else
				{
					if( m_nCaret < GetOutBuffer()->GetTextSize() )
						PlaceCaret( m_nCaret + 1 );
				}
			}

            if( GetKeyState( VK_SHIFT ) >= 0 )
			{
                // Shift is not down. Update selection
                // start along with the caret.
                m_nSelStart = m_nCaret;
			}

            ResetCaretBlink();
			
			TextOut();

            break;

        case VK_UP:
			if(m_bMultiLine)
			{
				SCaretMoveInfo sCaretInfo;
				MakeCurrentCaretInfo(sCaretInfo);

				sCaretInfo.nCaretPosY -= (m_czFontText.y  - m_nMultiLineInterval);
				
				if(sCaretInfo.nCaretPosY < m_ptScrollPos.y)
				{
					DecrementScroll();
					if(sCaretInfo.nCaretPosY < 0)
					{
						sCaretInfo.nCaretPosY = 0;
					}
				}

				SCaretMoveInfo sNewCaret = m_pMultiLine->Up(&m_Buffer, sCaretInfo);
				PlaceCaret( sNewCaret.nCaretIdx );

				if( GetKeyState( VK_SHIFT ) >= 0 )
				{
					// Shift is not down. Update selection
					// start along with the caret.
					 m_nSelStart = m_nCaret;
				}
				ResetCaretBlink();
				TextOut();
			}
			break;
        case VK_DOWN:
			{
				if(m_bMultiLine)
				{
					SCaretMoveInfo sCaretInfo;
					MakeCurrentCaretInfo(sCaretInfo);
					sCaretInfo.nCaretPosY += (m_czFontText.y + m_nMultiLineInterval);

					CRectangle rtClient		= m_pInputBox->GetClientRect();
					CRectangle rtValidArea	= m_pMultiLine->GetValidArea();

					if(m_nCaretRenderPosY + m_czFontText.y*2 + m_nMultiLineInterval > rtClient.bottom)
					{
						if(sCaretInfo.nCaretPosY < rtValidArea.bottom)
						{
							IncrementScroll();
						}
					}
					
					SCaretMoveInfo sNewCaret = m_pMultiLine->Down(&m_Buffer, sCaretInfo);
					PlaceCaret( sNewCaret.nCaretIdx );

					if( GetKeyState( VK_SHIFT ) >= 0 )
					{
						// Shift is not down. Update selection
						// start along with the caret.
						 m_nSelStart = m_nCaret;
					}
					ResetCaretBlink();
					TextOut();
				}
			}
			break;
        default:
            break;
	}
}

VOID CInputBox_Generic::SetKeyDownSound( const CHAR* szClickSoundFilename )
{
	if( szClickSoundFilename )
		m_strKeyDownSound = szClickSoundFilename;
	else
		m_strKeyDownSound.clear();
}

VOID CInputBox_Generic::OnScrollMove( INT nPos )
{
	m_nCurrScrollIdx = nPos;
	if(m_nCurrScrollIdx >= m_nScrollNum)
		m_nCurrScrollIdx = m_nScrollNum;

	CalcScrollPosition();

	MultiLineTextOut();
}

VOID CInputBox_Generic::OnSetAlpha( BYTE ucAlpha )
{
	m_TextSurface.m_SnapShot.uAlpha = ucAlpha;
	m_CaretSurface.m_SnapShot.uAlpha = ucAlpha;
	m_SelSurface.m_SnapShot.uAlpha = ucAlpha;
	m_CompSurface.m_SnapShot.uAlpha = ucAlpha;
	m_CompAttrSurface.m_SnapShot.uAlpha = ucAlpha;

	std::list<CSurface>::iterator it; 
	for( it = m_stlSurface.begin() ; it != m_stlSurface.end() ; ++it )
	{
		(*it).m_SnapShot.uAlpha = ucAlpha;
	}
	
	for(it = m_listMultiSelSurface.begin(); it != m_listMultiSelSurface.end(); it++)
	{
		(*it).m_SnapShot.uAlpha = ucAlpha;
	}
}

VOID CInputBox_Generic::OnGotFocus(VOID)
{
	m_bSaveImeState = TRUE;

	if( m_SavedImeState == IMEUI_STATE_ON && m_ImeState == IMEUI_STATE_OFF )
		SetNativeMode();
}

VOID CInputBox_Generic::OnLostFocus(VOID)
{
	m_bSaveImeState = FALSE;

	if( m_ImeState == IMEUI_STATE_ON )
		SetEnglishMode();
}

END_GUI