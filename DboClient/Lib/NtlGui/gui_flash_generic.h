#ifndef __GUI_FLASH_GENERIC_H__
#define __GUI_FLASH_GENERIC_H__

#include "gui_Flash.h"
#include "gui_surface.h"
#include "NtlFlasher.h"

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
#include "NtlSoundManager.h"
#endif

START_GUI

//////////////////////////////////////////////////////////////////////////////////////
// Flash Library
// desc : Flash Library
//////////////////////////////////////////////////////////////////////////////////////

#define SCALEFORM_GFX

#ifndef SCALEFORM_GFX
typedef VOID		CGuiFlashObject;
#else
typedef CNtlFlasher	CGuiFlashObject;
#endif

class CFlash_Generic
{
public:
	typedef struct _stFSCallBackData
	{
		std::string		m_strCommand;
		std::string		m_strArgs;

		_stFSCallBackData( const CHAR* szCommond, const CHAR* szArgs ) { m_strCommand = szCommond; m_strArgs = szArgs; }
	}stFSCallBackData;	

	typedef struct _stMovieEndData
	{
		BOOL	m_bMovieEndCallBack;

		VOID Clear(VOID) { m_bMovieEndCallBack = FALSE; }
		BOOL DoSendMovieEndCallBack(VOID) { return m_bMovieEndCallBack; }
		VOID SendMovieEndCallBack(VOID) { m_bMovieEndCallBack = TRUE; }
	}stMovieEndData;

	// Construction & Destruction
	CFlash_Generic( CFlash* pSelf, CONST CHAR* pFileName, BOOL bUseFont, BOOL bOriginalSize, BOOL bScissor );
	~CFlash_Generic(VOID);

	// Operation:
	BOOL Load( CONST CHAR* pFileName, BOOL bOriginalSize );
	VOID Unload(VOID);

	VOID Update( FLOAT fElapsedTime );
	
	BOOL	SetVariable( const CHAR* path, const CHAR* pValue );
	BOOL	SetVariable( const CHAR* path, const WCHAR* pValue );
	BOOL	SetVariableFloat( const CHAR* path, FLOAT fValue );
	BOOL	SetVariableArray( INT type, const CHAR* path, DWORD index, const VOID* pData, DWORD count );

	const CHAR*		GetVariable( const CHAR* path );
	const WCHAR*	GetVariableWideStr( const CHAR* path );
	FLOAT			GetVariableFloat( const CHAR* path );
	BOOL			GetVariableArray( INT type, const CHAR* path, DWORD index, VOID* pData, DWORD count );

	const CHAR*		Invoke( const CHAR* path, const CHAR* parameter, va_list args);

	// Attributes:
	CRectangle	GetFrameResolution(VOID);
	INT			GetFrameCount(VOID);
	FLOAT		GetFrameRate(VOID);
	BOOL		IsPlayMovie(VOID);
	VOID		SetResizeType( INT eResizeType );
	VOID		SetFrameCallback( BOOL bSet );
	BOOL		HasFrameCallback(VOID);
	
	DWORD		GetCurrentFrame(VOID);
	VOID		GotoFrame( DWORD dwFrame );

	// Implementation:
	VOID		SetFlashPosition(VOID);
	
	//! CallBack
	VOID OnMove( INT nX,INT nY );
	VOID OnResize( INT nOldCX, INT nOldCY );
	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);
	VOID OnMouseMove(INT nFlags,INT nX,INT nY);
	VOID OnMouseWheel( INT nFlags, short shDelta, CPos& ptPos );
	VOID OnKeyDown( CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key );
	VOID OnKeyUp( CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key );
	VOID OnMouseLeave( gui::CComponent* pComponent );
		
	VOID OnSetAlpha( BYTE ucAlpha );
	VOID OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

	INT	 OnFSCallBack( VOID* szCommand, VOID* szArgs);

	CSlot m_SlotMove;
	CSlot m_slotResize;
	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	
	CSlot m_SlotMouseDown;
	CSlot m_SlotMouseUp;
	CSlot m_SlotMouseMove;
	CSlot m_SlotMouseWheel;
	CSlot m_SlotKeyDown;
	CSlot m_SlotKeyUp;
	CSlot m_SlotMouseLeave;

	CSlot m_SlotSetAlpha;
	CSlot m_SlotSetColor;

public:
	//! Signal
	CSignal_v1<gui::CComponent*>		m_sigMovieEnd;
	CSignal_v2<const char*,const char*> m_sigFSCallBack;

	//! Variable
	CGuiFlashObject*	m_pFlashObject;
	
	CFlash*				m_pFlash;

	stMovieEndData		m_stMovieEndData;
	std::vector<stFSCallBackData>	 m_vecFSCallBackData;

#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	SOUND_HANDLE		m_hSound;
	SOUND_HANDLE		m_hBGM;
#endif

public:
	// Static Function
	static BOOL CreateFlashManager( HWND hWnd );
	static VOID	DeleteFlashManager(VOID);	
	static VOID	ResetFlashRenderer(VOID);
	static BOOL ParseFlashFont( const CHAR* pFilename );
	static VOID SetScreenSize( INT nWidth, INT nHeight );
};  

END_GUI

#endif
