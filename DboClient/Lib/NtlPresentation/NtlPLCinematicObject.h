////////////////////////////////////////////////////////////////////////////////
// File: CinematicObject.h
// Desc: CinematicObject
//
// 2007.08.2 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTL_PL_CINEMATIC_OBJECT_H__
#define __NTL_PL_CINEMATIC_OBJECT_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlCallbackWidget.h"
#include "NtlDebug.h"

#define CINEMATIC_MOVETIME				1.0f
#define CINEMATIC_TEXT_NORMALAPPEARTIME	1.0f
#define CINEMATIC_TEXT_APPEARTIME		0.2f
#define CINEMATIC_DEFAULT_WIDTH			1024
#define CINEMATIC_DEFAULT_HEIGHT		768

#define CINEMATIC_MAX_PC_SELECT			4

class CNtlPLCinematicFlash
{
public:
	CNtlPLCinematicFlash( CRectangle rtPos, gui::CComponent* pParent, RwChar* pFilename );
	virtual ~CNtlPLCinematicFlash(VOID);

	virtual RwBool Update( RwReal fElapsedTime );

	gui::CFlash*	GetFlashObject(VOID) { return m_pFlash; }

protected:
	gui::CFlash*	m_pFlash;
};

class CNtlPLCinematicObject : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum BALLOONTYPE	 { BALLOON_TYPE_NORMAL, BALLOON_TYPE_BOMB };
	enum TEXTDISPLAYTYPE { TEXT_NORMAL, TEXT_HTML };

	//! Destructor
	~CNtlPLCinematicObject(VOID);

	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	
	VOID	Update( RwReal fElapsedTime );

	RwInt32	SwitchDialog( bool bOpen );

	VOID	HandleEvents( RWS::CMsg& msg );

	//! Access
	static CNtlPLCinematicObject* GetInstance(VOID) { return m_pInstance; }

	gui::CFrame*		GetImageFrame(VOID) { return m_pImageFrame; }
	gui::CDialog*		GetBalloon(VOID) { return m_pBalloon; }
	gui::CHtmlBox*		GetHtmlText(VOID) { return m_pHtmlText; }
	gui::CStaticBox*	GetText(VOID) { return m_pText; }

	gui::CDialog*		GetPCFrame(VOID) { return m_pfrmPCFrame; }
	gui::CHtmlBox*		GetPCMessage(VOID) { return m_phtmlPCMessage; }

	gui::CStaticBox*	GetPCPCName(VOID) { return m_pstbPCName; }
	gui::CButton*		GetPCSelect( RwUInt8 byIndex );
	gui::CPanel*		GetPCSelectFocus(VOID) { return m_ppnlPCSelectFocus; }
	RwUInt8				GetPCSelectValue(VOID) { return m_byPCSelect; }

	gui::CStaticBox*	GetSkipHintText() { return m_pstbCanSkipHint; }
	gui::COutputBox*	GetSkippedText() { return m_pstbNameSkipped; }

	VOID				CinematicFadeIn( RwBool bAlpha, RwBool bDirect );
	VOID				CinematicFadeOut( RwBool bAlpha, RwBool bDirect );
	VOID				CinematicBackFadeIn( RwBool bDirect, RwReal fCurrentTime = 0.0f );
	VOID				CinematicBackFadeOut( RwBool bDirect, RwReal fCurrentTime = 0.0f );
	RwBool				IsCinematicShow(VOID);
				
	VOID				SetBalloonType( RwInt32 eBalloonType );
	RwBool				SetImageTexture( RwChar* pFilename );
	RwBool				SetImageTexture( RwChar* szPrefix, RwInt32 eState );
	VOID				SetImageAlpha( RwUInt8 bAlpha );
	VOID				ShowImage( RwBool bShow );	
	VOID				SetStaticText( const WCHAR* pText );
	VOID				SetHtmlText( const WCHAR* pText, RwInt32 nSize );
	VOID				SetName( const WCHAR* pName );

	VOID				SetFocusOnNPCBar( RwBool bFocusOnNPC );	

	VOID				SetPCMessage( const WCHAR* pText, RwInt32 nSize );
	VOID				SetPCName( const WCHAR* pName );
	VOID				SetPCSelect( RwUInt8 byIndex, std::wstring strText );
	VOID				SetPCSelectValue( RwUInt8 byIndex );
	VOID				SetPCSelectRegion( RwUInt8 byTotalCount, RwUInt8 byPCSelectRegionHeight, RwBool bShowProc );
	VOID				DisableAllPCSelect(VOID);

	VOID				AddFlashObject( CNtlPLCinematicFlash* pFlash );	// 반드시 New로 생성해서 넘겨야함.
	VOID				DeleteAllFlashObject(VOID);
	RwBool				IsFlashObjectUpdated(VOID);

	RwReal				GetCinematicCurrentTime(VOID) { return m_fCurrentTime; }
	VOID				SetCinematicCurrentTime( RwReal fCurrentTime ) { m_fCurrentTime = fCurrentTime; }
	VOID				ResetCinematicCurrentTime(VOID) { m_fCurrentTime = 0.0f; }

	//! Operation
	template<class CallbackClass>
	VOID	LinkUpdateCallBack( CallbackClass* cbclass, RwInt32 (CallbackClass::*callback)(RwUInt32) );
	VOID	UnLinkUpdateCallBack(VOID) { NTL_DELETE( m_pUpdateCallback ); }	

	VOID	InitShowSetting(VOID);			

private:
	//! Implementation
	VOID	FitSizeToScreen( RwInt32 nWidth, RwInt32 nHeight );
	
	RwBool	LoadImageTexture( RwChar* pFilename );
	VOID	UnloadImageTexture(VOID);

	//! CallBack
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);

	//! Variable
	RwReal				m_fCurrentTime;

	RwInt32				m_nCinematicUpperInPosY;
	RwInt32				m_nCinematicUpperOutPosY;
	RwInt32				m_nCinematicLowerInPosY;
	RwInt32				m_nCinematicLowerOutPosY;

	RwInt32				m_eBalloonType;

	gui::CPanel*		m_pCinematicBack;
	gui::CPanel*		m_pCinematicUpper;
	gui::CPanel*		m_pCinematicLower;
	gui::CDialog*		m_pBalloon;
	gui::CHtmlBox*		m_pHtmlText;
	gui::CStaticBox*	m_pText;
	gui::CDialog*		m_pNameBox;
	gui::CStaticBox*	m_pName;

	gui::CFrame*		m_pImageFrame;

	gui::CSurface		m_surImage;	
	RwBool				m_bShowImage;

	gui::CDialog*		m_pfrmPCFrame;
	gui::CHtmlBox*		m_phtmlPCMessage;

	gui::CDialog*		m_pdlgPCNameBar;
	gui::CStaticBox*	m_pstbPCName;
	gui::CButton*		m_pbtnPCSelect[CINEMATIC_MAX_PC_SELECT];
	gui::CPanel*		m_ppnlPCSelectFocus;
	RwUInt8				m_byPCSelect;
	RwUInt8				m_byPCSelectCount;

	gui::CStaticBox*	m_pstbCanSkipHint;
	gui::COutputBox*	m_pstbNameSkipped;
	
	std::vector<CNtlPLCinematicFlash*>	m_vecFlash;

	CNtlCallbackParam1*	m_pUpdateCallback;

	static CNtlPLCinematicObject* m_pInstance;

	//! Constructor
	CNtlPLCinematicObject(VOID);
	CNtlPLCinematicObject( const RwChar* pName );
};

template<class CallbackClass>
VOID CNtlPLCinematicObject::LinkUpdateCallBack(CallbackClass* cbclass, RwInt32 (CallbackClass::*callback)( RwUInt32 uiElapsedTime ) )
{
	UnLinkUpdateCallBack();
	m_pUpdateCallback = NTL_NEW CNtlCallbackWidget1<CallbackClass>( cbclass, callback );
}

static CNtlPLCinematicObject* GetNtlPLCinematicObject(VOID)
{
	return CNtlPLCinematicObject::GetInstance();
}

#endif