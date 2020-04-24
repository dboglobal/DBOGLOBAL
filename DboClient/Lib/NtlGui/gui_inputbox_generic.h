#ifndef __GUI_INPUTBOX_GENERIC_H__
#define __GUI_INPUTBOX_GENERIC_H__

//--------------------------------------//
// WM_IME_STARTCOMPOSITION							// 
//  - 한글의 조합이 시작되는 것을 알림. //
//--------------------------------------//

//--------------------------------------//
// WM_IME_ENDCOMPOSITION								//
//  - 한글 조합이 끝나는 것을 알림.     //
//--------------------------------------//

//--------------------------------------//
// WM_IME_NOTIFY                        //
//  - 한/영 키가 바뀔때 발생된다.       //
//--------------------------------------// 

//---------------------------------------------------------// 
// WM_IME_COMPOSITION												  						 //
//  - 현재 한글이 조합중이라는 것을 알려준다.              //
//  - LPARAM : GCS_RESULTSTR => 한글 한글자 입력이 끝났당. //
//  - LPARAM : GCS_COMPSTR => 한글 한글자 입력중...        // 
//---------------------------------------------------------//

#define IME_IN_COMPOSITION      1
#define IME_IN_CHOSECAND        2

#define MAX_CANDLIST			 10
#define MAX_COMPSTRING_SIZE		256

#include "gui_inputbox.h"
#include "gui_unibuffer.h"

START_GUI


typedef struct _SCaretMoveInfo
{
	INT nCaretIdx;
	INT nCaretLine;
	INT nCaretPosX;
	INT nCaretPosY;
}SCaretMoveInfo;

class CInputBoxLineInfo
{
private:

	enum 
	{
		LINEITEM_GROW_SIZE	= 50
	};

	struct SLineItem
	{
		BOOL		bNewLine;
		BOOL		bCompString;
		INT			nStringIdx;
		INT			nStringLen;
		INT			nLineIdx;
		CRectangle	rtArea;
	};

	INT			m_nLineNum;
	INT			m_nActiveNum;
	SLineItem	*m_pLineItem;
	CRectangle	m_rtClientArea;
	CRectangle	m_rtValidArea;
	INT			m_nCurrHeight;

private:

	void	AddItem(INT nStringIdx, INT nStringLen, CRectangle rtArea, INT nLineIdx, BOOL bCompString, BOOL bNewLine);
	BOOL	CheckGrowItem(void);
	void	GrowItem(void);
	void	Clear(void);

	INT		FindLineIndex(INT nCaret);

public:

	CInputBoxLineInfo();
	~CInputBoxLineInfo();

	void			SetClientArea(INT nWidth, INT nHeight);
	SCaretMoveInfo	FindStringIndex(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo, BOOL bMouseClick = TRUE);
	SCaretMoveInfo	SetString(CUniBuffer *pString, CUniBuffer *pCompString, INT nCaret, INT nLineInterval, INT nTextHeight);
	void			SelectArea(CUniBuffer *pString, INT nStartCaret, INT nEndCaret, std::list<CRectangle>& listArea);
	CRectangle		GetValidArea(void) { return m_rtValidArea; }

	SCaretMoveInfo	Home(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo);
	SCaretMoveInfo	End(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo);
	SCaretMoveInfo	Up(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo);
	SCaretMoveInfo	Down(CUniBuffer *pString, SCaretMoveInfo sCurrCaretInfo);

	void			TextOut(CRectangle rtClipping, CUniBuffer *pString, CUniBuffer *pCompString, CGuiFont *pFont, CTexture *pTexture, COLORREF textcolor, COLORREF bkcolor);
};

class CGuiFont;

class CInputBox_Generic
{
public:

    typedef struct _SCandList
    {
        WCHAR awszCandidate[MAX_CANDLIST][256];
        CUniBuffer HoriCand;		// Candidate list string (for horizontal candidate window)
        int   nFirstSelected;		// First character position of the selected string in HoriCand
        int   nHoriSelectedLen;		// Length of the selected string in HoriCand
        DWORD dwCount;				// Number of valid entries in the candidate list
        DWORD dwSelection;			// Currently selected candidate entry relative to page top
        DWORD dwPageSize;
        int   nReadingError;		// Index of the error character
        bool  bShowWindow;			// Whether the candidate list window is visible
        RECT  rcCandidate;			// Candidate rectangle computed and filled each time before rendered
    }SCandList;

	typedef struct _SInputLocale
    {
        HKL   m_hKL;            // Keyboard layout
        WCHAR m_wszLangAbb[3];  // Language abbreviation
        WCHAR m_wszLang[64];    // Localized language name
    }SInputLocale;

	static HWND		 m_hWindow;

	static HINSTANCE m_hDllImm32;         // IMM32 DLL handle
    static HINSTANCE m_hDllVer;           // Version DLL handle
    static HIMC      m_hImcDef;           // Default input context

	static HKL		 m_hklCurrent;        // Current keyboard layout of the process
	static HINSTANCE m_hDllIme;           // Instance handle of the current IME module
	static IMESTATE  m_ImeState;          // IME global state
	static IMESTATE	 m_SavedImeState;	  
	static BOOL		 m_bSaveImeState;	  // Is Enable to Save ImeState. Generally, Focused state is true.

	static LPWSTR		m_wszCurrIndicator;    // Points to an indicator string that corresponds to current input locale
	static WCHAR		m_aszIndicator[5][3];  // String to draw to indicate current input locale
	static bool			m_bVerticalCand;       // Indicates that the candidates are listed vertically
	static CUniBuffer	m_CompString;
	static CPos			m_ptCompString;        // Composition string position. Updated every frame.
	static BYTE			m_abCompStringAttr[MAX_COMPSTRING_SIZE];
	static DWORD		m_adwCompStringClause[MAX_COMPSTRING_SIZE];
    static WCHAR		m_wszReadingString[32];			// Used only with horizontal reading window (why?)

	static bool			m_bEnableImeSystem;				// Whether the IME system is active

	static int			m_nCompCaret;
	static bool			m_bInsertOnType;			// Insert the character as soon as a key is pressed (Korean behavior)
	static SCandList	m_sCandList;				// Data relevant to the candidate list
	static bool			m_bShowReadingWindow;		// Indicates whether reading window is visible
	static bool			m_bHorizontalReading;		// Indicates whether the reading window is vertical or horizontal
	static bool			m_bChineseIME;
	static std::vector< SInputLocale > m_Locale;	// Array of loaded keyboard layout on system
	
public:

	CInputBox_Generic(CInputBox *pSelf, DWORD dwStyle, int nMaxLength);
	~CInputBox_Generic();

	CInputBox *m_pInputBox;

	//----------------------------------------
	// attribute

	DWORD			m_dwStyle;
	int				m_nMaxLength;
	COLORREF		m_TextColor;
	COLORREF		m_BkColor;
	int				m_nBkMode;
	int				m_nMultiLineInterval;

	//----------------------------------------
	// font
	CGuiFont		*m_pFont;
	std::string		m_strFont;
	int				m_nFontH;
	CPos			m_czFontText;

	//----------------------------------------
	// Caret

	static bool		m_bHideCaret;   // If true, we don't render the caret.
	bool			m_bShowCaret;
	bool			m_bCaretMode;
	int				m_nCaretCX;		// caret size x
	int				m_nCaretCY;		// caret size y
    int				m_nCaret;		// Caret position, in characters
	int				m_nCaretLine;	// 현재의 caret이 위치한 line
	int				m_nCaretPosX;	// 현재 caret 위치 X.
	int				m_nCaretPosY;	// 현재 caret 위치 Y.
	int				m_nCaretRenderPosX;
	int				m_nCaretRenderPosY;
	
	//----------------------------------------
	// string
	
	CUniBuffer		m_Buffer;				// Buffer to hold text
	CUniBuffer		m_PwBuffer;				// Buffer to password text
	bool			m_bInsertMode;			// If true, control is in insert mode. Else, overwrite mode.
	int				m_nSelStart;			// Starting position of the selection. The caret marks the end.
	int				m_nFirstVisible;		// First visible character in the edit control
	
	int				m_nCompLineStartX;
	int				m_nCompLineEndX;
	int				m_nCompCaretPosX;
	int				m_nCompAttrStartX;
	int				m_nCompAttrEndX;
	
    bool			m_bMouseDrag;   // True to indicate drag in progress

	//------------------------------------------
	// multi line
	
	bool					m_bMultiLine;
	CInputBoxLineInfo		*m_pMultiLine;	
	std::list<CRectangle>	m_listMultiSelArea;
	std::list<CSurface>		m_listMultiSelSurface;

	int						m_nScrollNum;
	int						m_nCurrScrollIdx;
	CPos					m_ptScrollPos;
	bool					m_bAutoScrollDec;
		
	//----------------------------------------
	// Surface

	CSurface				m_TextSurface;
	CSurface				m_CaretSurface;
	CSurface				m_SelSurface;
	CSurface				m_CompSurface;
	CSurface				m_CompAttrSurface;

	std::list<CSurface>		m_stlSurface;
	
	//----------------------------------------
	// Sound
	std::string				m_strKeyDownSound;

// signals
public:

	CSignal_v1<const std::string &> m_SigChanged;
	CSignal_v0 m_SigReturnPressed;
	CSignal_v0 m_SigSpaceUp;
	CSignal_v0 m_SigActivity;
	CSignal_v0 m_SigEscKeyUp;
	static CSignal_v1<INT> m_SigIMEChanged;

// callback function
private:

	VOID OnSetOptions(const CComponentOptions &options);
	VOID OnResize(int nX,int nY);
	VOID OnMove(int nOldX,int nOldY);
	VOID OnWheel(int nFlags ,short shDelta, CPos& ptPos);
	VOID OnSetAlpha( BYTE ucAlpha );
	VOID OnGotFocus(VOID);
	VOID OnLostFocus(VOID);
	
	CSlot m_SlotSetOptions;
	CSlot m_SlotResize;
	CSlot m_SlotMove;
	CSlot m_SlotWheel;
	CSlot m_SlotSetAlpha;
	CSlot m_SlotGotFocus;
	CSlot m_SlotLostFocus;
	CSlot m_SlotScrollChange;
	CSlot m_SlotSliderMove;
	
	CSlot m_SlotMouseDown;
	CSlot m_SlotMouseUp;
	CSlot m_SlotMouseMove;
	CSlot m_SlotKeyDown;
	
public:
	VOID SetKeyDownSound( const CHAR* szClickSoundFilename );
	VOID OnMouseDown(const CKey &key);
	VOID OnMouseUp(const CKey &key);
	VOID OnMouseMove(int,int,int);
	VOID OnKeyDown(CComponent *, CInputDevice *, const CKey &);
	VOID OnScrollMove( INT nPos );
	
private:

	//: IME procedure
	BOOL		SetOpenStatus(VOID); 
	
	int			GetCaretPosition(VOID);

	BOOL		IsPasswordMode(VOID);

	BOOL		IsReadOnly(VOID);

	CUniBuffer*	GetOutBuffer(VOID);

	//: edit procedure

	VOID		SingleLineTextOut(VOID);
	VOID		MultiLineTextCalc(VOID);
	VOID		MultiLineTextOut(VOID);
	VOID		MultiLineAutoScrollDecrement(INT nOldCaretLine);

	VOID		MakeCurrentCaretInfo(SCaretMoveInfo& sCaretInfo);
	VOID		MakeCurrentCaretInfo(SCaretMoveInfo& sCaretInfo, INT nCaretPosX, INT nCaretPosY);
	VOID		CalcScrollPosition(VOID);
	VOID		CalcCaretInfo(SCaretMoveInfo sCaretInfo);

	VOID		IncrementScroll(VOID);
	VOID		DecrementScroll(VOID);

	INT			GetScrollBarWidth(VOID);

protected:

	// Empty implementation of the IMM32 API
    static INPUTCONTEXT* WINAPI Dummy_ImmLockIMC( HIMC ) { return NULL; }
    static BOOL WINAPI Dummy_ImmUnlockIMC( HIMC ) { return FALSE; }
    static LPVOID WINAPI Dummy_ImmLockIMCC( HIMCC ) { return NULL; }
    static BOOL WINAPI Dummy_ImmUnlockIMCC( HIMCC ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmDisableTextFrameService( DWORD ) { return TRUE; }
    static LONG WINAPI Dummy_ImmGetCompositionStringW( HIMC, DWORD, LPVOID, DWORD ) { return IMM_ERROR_GENERAL; }
    static DWORD WINAPI Dummy_ImmGetCandidateListW( HIMC, DWORD, LPCANDIDATELIST, DWORD ) { return 0; }
    static HIMC WINAPI Dummy_ImmGetContext( HWND ) { return NULL; }
    static BOOL WINAPI Dummy_ImmReleaseContext( HWND, HIMC ) { return FALSE; }
    static HIMC WINAPI Dummy_ImmAssociateContext( HWND, HIMC ) { return NULL; }
    static BOOL WINAPI Dummy_ImmGetOpenStatus( HIMC ) { return 0; }
    static BOOL WINAPI Dummy_ImmSetOpenStatus( HIMC, BOOL ) { return 0; }
    static BOOL WINAPI Dummy_ImmGetConversionStatus( HIMC, LPDWORD, LPDWORD ) { return 0; }
    static HWND WINAPI Dummy_ImmGetDefaultIMEWnd( HWND ) { return NULL; }
    static UINT WINAPI Dummy_ImmGetIMEFileNameA( HKL, LPSTR, UINT ) { return 0; }
    static UINT WINAPI Dummy_ImmGetVirtualKey( HWND ) { return 0; }
    static BOOL WINAPI Dummy_ImmNotifyIME( HIMC, DWORD, DWORD, DWORD ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmSetConversionStatus( HIMC, DWORD, DWORD ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmSimulateHotKey( HWND, DWORD ) { return FALSE; }
    static BOOL WINAPI Dummy_ImmIsIME( HKL ) { return FALSE; }

    // Traditional Chinese IME
    static UINT WINAPI Dummy_GetReadingString( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) { return 0; }
    static BOOL WINAPI Dummy_ShowReadingWindow( HIMC, BOOL ) { return FALSE; }

    // Verion library imports
    static BOOL APIENTRY Dummy_VerQueryValueA( const LPVOID, LPSTR, LPVOID *, PUINT ) { return 0; }
    static BOOL APIENTRY Dummy_GetFileVersionInfoA( LPSTR, DWORD, DWORD, LPVOID ) { return 0; }
    static DWORD APIENTRY Dummy_GetFileVersionInfoSizeA( LPSTR, LPDWORD ) { return 0; }

    // Function pointers: IMM32
    static INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC );
    static BOOL (WINAPI * _ImmUnlockIMC)( HIMC );
    static LPVOID (WINAPI * _ImmLockIMCC)( HIMCC );
    static BOOL (WINAPI * _ImmUnlockIMCC)( HIMCC );
    static BOOL (WINAPI * _ImmDisableTextFrameService)( DWORD );
    static LONG (WINAPI * _ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD );
    static DWORD (WINAPI * _ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD );
    static HIMC (WINAPI * _ImmGetContext)( HWND );
    static BOOL (WINAPI * _ImmReleaseContext)( HWND, HIMC );
    static HIMC (WINAPI * _ImmAssociateContext)( HWND, HIMC );
    static BOOL (WINAPI * _ImmGetOpenStatus)( HIMC );
    static BOOL (WINAPI * _ImmSetOpenStatus)( HIMC, BOOL );
    static BOOL (WINAPI * _ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD );
    static HWND (WINAPI * _ImmGetDefaultIMEWnd)( HWND );
    static UINT (WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT );
    static UINT (WINAPI * _ImmGetVirtualKey)( HWND );
    static BOOL (WINAPI * _ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD );
    static BOOL (WINAPI * _ImmSetConversionStatus)( HIMC, DWORD, DWORD );
    static BOOL (WINAPI * _ImmSimulateHotKey)( HWND, DWORD );
    static BOOL (WINAPI * _ImmIsIME)( HKL );

    // Function pointers: Traditional Chinese IME
    static UINT (WINAPI * _GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT );
    static BOOL (WINAPI * _ShowReadingWindow)( HIMC, BOOL );

    // Function pointers: Verion library imports
    static BOOL (APIENTRY * _VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT );
    static BOOL (APIENTRY * _GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID );
    static DWORD (APIENTRY * _GetFileVersionInfoSizeA)( LPSTR, LPDWORD );

public:

	static VOID Initialize(VOID);
    static VOID Uninitialize(VOID);

	static VOID SendKey( BYTE nVirtKey );
	static WORD GetLanguage(VOID) { return LOWORD( m_hklCurrent ); }
	static WORD GetPrimaryLanguage(VOID) { return PRIMARYLANGID( LOWORD( m_hklCurrent ) ); }
	static WORD GetSubLanguage(VOID) { return SUBLANGID( LOWORD( m_hklCurrent ) ); }
	static DWORD GetImeId( UINT uIndex = 0 );
	static VOID CheckInputLocale(VOID);
	static VOID CheckToggleState(VOID);
	static VOID SetupImeApi(VOID);
	static VOID GetReadingWindowOrientation( DWORD dwId );
	static VOID ResetCompositionString(VOID);
	static VOID GetPrivateReadingString(VOID);
	
	static  HRESULT StaticOnCreateDevice(HWND hWnd);
	static LRESULT OnStaticMsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	static VOID SetEnglishMode( BOOL bSet = FALSE );
	static VOID SetEnglishModeDefault();
	static VOID SetNativeMode( BOOL bSet = FALSE );
	static VOID SetNativeModeDefault(VOID);

public:
	
	//----------------------------------------
	//: text clear
	VOID Clear(VOID);
	VOID SetPasswordMode(BOOL bPwMode);
	VOID SetNumberOnly(BOOL bNumberMode);
	VOID SetMultilineMode(BOOL bMultilineMode);
	VOID SetReadOnly(BOOL bReadOnly);
	VOID SetMaxLength(INT nLength);
	VOID SetLineInterval(INT nInterval);
	VOID SetText(const char *text);
	VOID SetText(const WCHAR *text);

	//-------------------------------------------------------------------------
	//: font Create
	VOID CreateFont(char *pFaceName, int nHeight, int nAttributes = 0);
	VOID CreateFont(VOID);
	
	//: Caret Create
	//: int nCX : caret width , int nCY : caret height 
	VOID CreateCaret(int nCX,int nCY);
	VOID CreateCaret(VOID);
	VOID SetCaretSize(int nCX,int nCY);
	VOID SetCaretColor(BYTE byRed,BYTE byGreen,BYTE byBlue,BYTE byAlpha);
	
	VOID CaretRenderPosition(VOID);
	VOID SingleLineCaretRenderPosition(VOID);
	VOID MultiLineCaretRenderPosition(VOID);

	VOID TextOut(VOID);
	VOID Render(VOID);
	VOID RenderCaret(VOID);
	
	VOID TruncateCompString( bool bUseBackSpace = true, int iNewStrLen = 0 );
	VOID SendCompString(HWND hWnd);
	VOID ResetCaretBlink(VOID);
	VOID PlaceCaret(int nCP);
	VOID DeleteSelectionText(VOID);
	VOID CopyToClipboard(VOID);
	VOID PasteFromClipboard(VOID);
	BOOL IsNumberOnly(VOID);

//! Signals:
public:

	CSignal_v1<int> m_SigCharChanged;
	
	LRESULT OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	bool OnMsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	LRESULT OnIMEMsgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
};

END_GUI

#endif
