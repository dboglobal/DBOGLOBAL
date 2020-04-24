#include "gui_precomp.h"
#include "gui_Flash.h"
#include "gui_Flash_generic.h"
#include "gui_componentoption.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

START_GUI

CFlash::CFlash( CComponent *pParent,CSurfaceManager *pSurfaceManager )
: CComponent( pParent,pSurfaceManager ),m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CFlash_Generic( this, NULL, FALSE, FALSE, FALSE );
}

CFlash::CFlash( const CRectangle& rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, CONST CHAR* pFlashFIleName,
				BOOL bUseFont /* = FALSE */, BOOL bOriginalSize /* = FALSE */, BOOL bScissor /* = FALSE  */)
: CComponent( rtPos, pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CFlash_Generic( this, pFlashFIleName, bUseFont, bOriginalSize, bScissor );
}

CFlash::~CFlash(VOID)
{
	NTL_DELETE( m_pImpl );
}

/////////////////////////////////////////////////////////////////////////////
// Operations:
BOOL CFlash::Load( CONST CHAR* pFlashFile, BOOL bOriginalSIze /* = FALSE  */ )
{
	return m_pImpl->Load( pFlashFile, bOriginalSIze );	
}

VOID CFlash::Unload(VOID)
{
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	GetSoundManager()->Stop( m_pImpl->m_hSound );	
#endif
	m_pImpl->Unload();
}

VOID CFlash::Update( FLOAT fElapsedTime )
{
	m_pImpl->Update( fElapsedTime );
}

VOID CFlash::PlayMovie( BOOL bPlay )
{
#ifdef SCALEFORM_GFX
	m_pImpl->m_pFlashObject->Play( bPlay );
#endif
}

VOID CFlash::RestartMovie(VOID)
{
#ifdef SCALEFORM_GFX
	m_pImpl->m_pFlashObject->RestartMovie();
#endif
}

VOID CFlash::Rotate( RwInt32 nDegree )
{
#ifdef SCALEFORM_GFX
	m_pImpl->m_pFlashObject->SetRotate( nDegree );
#endif
}

VOID CFlash::Show( bool bShow )
{
#ifdef FMOD_SOUND_ATTACHED_TO_GUI
	if( !bShow )
		GetSoundManager()->Stop( m_pImpl->m_hSound );	
#endif

	CComponent::Show( bShow );
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:
VOID CFlash::AutoRewind( BOOL bAutoRewind )
{
#ifdef SCALEFORM_GFX
	m_pImpl->m_pFlashObject->AutoRewind( bAutoRewind );
#endif
}

VOID CFlash::SetResizeType( eRESIZETYPE eResizeType )
{
	m_pImpl->SetResizeType( eResizeType );
}

VOID CFlash::SetUseFont( BOOL bUseFont )
{
	m_pImpl->m_pFlashObject->SetUseFont( bUseFont );
}

CRectangle CFlash::GetFrameResolution(VOID)
{
	return m_pImpl->GetFrameResolution();
}

INT CFlash::GetFrameCount(VOID)
{
	return m_pImpl->GetFrameCount();
}

FLOAT CFlash::GetFrameRate(VOID)
{
	return m_pImpl->GetFrameRate();
}

DWORD CFlash::GetCurrentFrame(VOID)
{
	return m_pImpl->GetCurrentFrame();
}

VOID CFlash::GotoFrame( DWORD dwFrame )
{
	m_pImpl->GotoFrame( dwFrame );
}

BOOL CFlash::IsPlayMovie(VOID)
{
	return m_pImpl->IsPlayMovie();
}

VOID CFlash::SetFrameCallback( BOOL bSet )
{
	m_pImpl->SetFrameCallback( bSet );
}	
		
BOOL CFlash::HasFrameCallback(VOID)		
{
	return m_pImpl->HasFrameCallback();
}

BOOL CFlash::SetVariable( const CHAR* path, const CHAR* pValue )
{
	return m_pImpl->SetVariable( path, pValue );
}

BOOL CFlash::SetVariable( const CHAR* path, const WCHAR* pValue )
{
	return m_pImpl->SetVariable( path, pValue );
}

BOOL CFlash::SetVariableFloat( const CHAR* path, FLOAT fValue )
{ 
	return m_pImpl->SetVariableFloat( path, fValue );
}

BOOL CFlash::SetVariableArray( eARRAYTYPE type, const CHAR* path, DWORD index, const VOID* pData, DWORD count )
{
	return m_pImpl->SetVariableArray( type, path, index, pData, count );
}

const CHAR*	CFlash::GetVariable( const CHAR* path )
{
	return m_pImpl->GetVariable( path );
}

const WCHAR* CFlash::GetVariableWideStr( const CHAR* path )
{
	return m_pImpl->GetVariableWideStr( path );
}

FLOAT CFlash::GetVariableFloat( const CHAR* path )
{
	return m_pImpl->GetVariableFloat( path );
}

BOOL CFlash::GetVariableArray( eARRAYTYPE type, const CHAR* path, DWORD index, VOID* pData, DWORD count )
{
	return m_pImpl->GetVariableArray( type, path, index, pData, count );
}

const CHAR* CFlash::Invoke( const CHAR* path, const CHAR* parameter, ... )
{
	va_list args;
	va_start( args, parameter );
	const CHAR* pRes = m_pImpl->Invoke( path, parameter, args );
	va_end( args );

	return pRes;
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CSignal_v1<gui::CComponent*>& CFlash::SigMovieEnd(VOID)
{
	return m_pImpl->m_sigMovieEnd;
}

CSignal_v2<const char*,const char*>& CFlash::SigFSCallBack(VOID)
{
	return m_pImpl->m_sigFSCallBack;
}

END_GUI
