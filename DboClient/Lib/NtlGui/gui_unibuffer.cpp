#include "gui_precomp.h"
#include "gui_unibuffer.h"
#include "gui_font.h"


// PROFILE
#include "NtlProfiler.h"

START_GUI

#define MAX_INPUTBOX_LENGTH 0xFFFF

HINSTANCE CUniBuffer::m_hDll = NULL;  // Uniscribe DLL handle
HRESULT (WINAPI *CUniBuffer::_ScriptApplyDigitSubstitution)( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* ) = Dummy_ScriptApplyDigitSubstitution;
HRESULT (WINAPI *CUniBuffer::_ScriptStringAnalyse)( HDC, const VOID *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* ) = Dummy_ScriptStringAnalyse;
HRESULT (WINAPI *CUniBuffer::_ScriptStringCPtoX)( SCRIPT_STRING_ANALYSIS, int, BOOL, int* ) = Dummy_ScriptStringCPtoX;
HRESULT (WINAPI *CUniBuffer::_ScriptStringXtoCP)( SCRIPT_STRING_ANALYSIS, int, int*, int* ) = Dummy_ScriptStringXtoCP;
HRESULT (WINAPI *CUniBuffer::_ScriptStringFree)( SCRIPT_STRING_ANALYSIS* ) = Dummy_ScriptStringFree;
const SCRIPT_LOGATTR* (WINAPI *CUniBuffer::_ScriptString_pLogAttr)( SCRIPT_STRING_ANALYSIS ) = Dummy_ScriptString_pLogAttr;
const int* (WINAPI *CUniBuffer::_ScriptString_pcOutChars)( SCRIPT_STRING_ANALYSIS ) = Dummy_ScriptString_pcOutChars;

VOID CUniBuffer::Initialize()
{
    if( m_hDll ) // Only need to do once
        return;

    WCHAR wszPath[GUI_MAX_PATH];
    if( !::GetSystemDirectory( wszPath, GUI_MAX_PATH ) )
        return;

    // Verify whether it is safe to concatenate these strings
    int len1 = lstrlen(wszPath);
    int len2 = lstrlen(UNISCRIBE_DLLNAME);
    if (len1 + len2 > GUI_MAX_PATH)
        return;

    // We have verified that the concatenated string will fit into wszPath,
    // so it is safe to concatenate them.
    StringCchCat( wszPath, GUI_MAX_PATH, UNISCRIBE_DLLNAME );

    m_hDll = LoadLibrary( wszPath );
	if(!m_hDll)
		return;

	FARPROC Temp;
    GETPROCADDRESS( m_hDll, ScriptApplyDigitSubstitution, Temp );
    GETPROCADDRESS( m_hDll, ScriptStringAnalyse, Temp );
    GETPROCADDRESS( m_hDll, ScriptStringCPtoX, Temp );
    GETPROCADDRESS( m_hDll, ScriptStringXtoCP, Temp );
    GETPROCADDRESS( m_hDll, ScriptStringFree, Temp );
    GETPROCADDRESS( m_hDll, ScriptString_pLogAttr, Temp );
    GETPROCADDRESS( m_hDll, ScriptString_pcOutChars, Temp );
}



VOID CUniBuffer::Uninitialize()
{
    if( m_hDll )
    {
        PLACEHOLDERPROC( ScriptApplyDigitSubstitution );
        PLACEHOLDERPROC( ScriptStringAnalyse );
        PLACEHOLDERPROC( ScriptStringCPtoX );
        PLACEHOLDERPROC( ScriptStringXtoCP );
        PLACEHOLDERPROC( ScriptStringFree );
        PLACEHOLDERPROC( ScriptString_pLogAttr );
        PLACEHOLDERPROC( ScriptString_pcOutChars );

        FreeLibrary( m_hDll );
        m_hDll = NULL;
    }
}

VOID CUniBuffer::SetSizeCalcEnable(VOID)
{
	NTL_ARRAY_DELETE( m_pSize );
	NTL_ARRAY_DELETE( m_pCPtoXSize );

	m_pSize = NTL_NEW WORD [m_nBufferSize];
	m_pCPtoXSize = NTL_NEW INT [m_nBufferSize];

	m_bSizeCalc = true;
}

bool CUniBuffer::SetBufferSize( int nNewSize )
{
    // If the current size is already the maximum allowed,
    // we can't possibly allocate more.
    if( m_nBufferSize == MAX_INPUTBOX_LENGTH )
        return false;

    int nAllocateSize = ( nNewSize == -1 || nNewSize < m_nBufferSize * 2 ) ? ( m_nBufferSize ? m_nBufferSize * 2 : 256 ) : nNewSize * 2;

    // Cap the buffer size at the maximum allowed.
    if( nAllocateSize > MAX_INPUTBOX_LENGTH )
        nAllocateSize = MAX_INPUTBOX_LENGTH;

    WCHAR *pTempBuffer = NTL_NEW WCHAR[nAllocateSize];
    if( !pTempBuffer )
        return false;
	
    if( m_pwszBuffer )
    {
        CopyMemory( pTempBuffer, m_pwszBuffer, m_nBufferSize * sizeof(WCHAR) );

		NTL_ARRAY_DELETE( m_pwszBuffer );
	}
    else
    {
        ZeroMemory( pTempBuffer, sizeof(WCHAR) * nAllocateSize );
    }

	if(m_bSizeCalc)
	{
		WORD *pSize = NTL_NEW WORD [nAllocateSize];

		if(m_pSize)
		{
			CopyMemory( pSize, m_pSize, m_nBufferSize * sizeof(WORD) );
			NTL_ARRAY_DELETE( m_pSize );
		}

		INT *pIntSize = NTL_NEW INT [nAllocateSize];

		if(m_pCPtoXSize)
		{
			CopyMemory( pIntSize, m_pCPtoXSize, m_nBufferSize * sizeof(INT) );
			NTL_ARRAY_DELETE( m_pCPtoXSize );
		}

		m_pCPtoXSize = pIntSize;
		m_pSize = pSize;
	}

    m_pwszBuffer = pTempBuffer;
    m_nBufferSize = nAllocateSize;
    return true;
}


HRESULT CUniBuffer::Analyse()
{
    if( m_Analysis )
        _ScriptStringFree( &m_Analysis );

    SCRIPT_CONTROL ScriptControl; // For uniscribe
    SCRIPT_STATE   ScriptState;   // For uniscribe
    ZeroMemory( &ScriptControl, sizeof(ScriptControl) );
    ZeroMemory( &ScriptState, sizeof(ScriptState) );
    _ScriptApplyDigitSubstitution ( NULL, &ScriptControl, &ScriptState );

    if( !m_pFont )
       return E_FAIL;

	HFONT hFont = m_pFont->GetHFont();
	HDC hdc =  m_pFont->GetHdc(); 
	hFont = (HFONT)SelectObject(hdc, hFont);

    HRESULT hr = _ScriptStringAnalyse( hdc,
                                       m_pwszBuffer,
                                       lstrlenW( m_pwszBuffer ) + 1,  // NULL is also analyzed.
                                       lstrlenW( m_pwszBuffer ) * 3 / 2 + 16,
                                       -1,
                                       SSA_BREAK | SSA_GLYPHS | SSA_FALLBACK | SSA_LINK,
                                       0,
                                       &ScriptControl,
                                       &ScriptState,
                                       NULL,
                                       NULL,
                                       NULL,
                                       &m_Analysis );
    if( SUCCEEDED( hr ) )
        m_bAnalyseRequired = false;  // Analysis is up-to-date

	SelectObject(hdc, hFont);

    return hr;
}


CUniBuffer::CUniBuffer( int nInitialSize )
{
    CUniBuffer::Initialize();  // ensure static vars are properly init'ed first

    m_nBufferSize = 0;
    m_pwszBuffer = NULL;
    m_bAnalyseRequired = true;
    m_Analysis = NULL;
    m_pFont = NULL;
	m_bSizeCalc = false;
	m_pSize = NULL;
	m_pCPtoXSize = NULL;
    
    if( nInitialSize > 0 )
        SetBufferSize( nInitialSize );
}

CUniBuffer::CUniBuffer( CUniBuffer &buffer )
{
	CUniBuffer::Initialize();  // ensure static vars are properly init'ed first

	m_nBufferSize = buffer.m_nBufferSize;
	m_bSizeCalc = buffer.m_bSizeCalc;
	m_pSize = NULL;
	m_pCPtoXSize = NULL;
	m_pwszBuffer = NULL;
	if( m_nBufferSize > 0 )
	{
		
		
		/*SetBufferSize( buffer.m_nBufferSize );*/
		m_pwszBuffer  = NTL_NEW WCHAR[buffer.m_nBufferSize];
		memcpy( m_pwszBuffer, buffer.m_pwszBuffer, sizeof(WCHAR) * buffer.m_nBufferSize );

		if( m_bSizeCalc )
		{
			m_pSize  = NTL_NEW WORD[buffer.m_nBufferSize];
			memcpy( m_pSize, buffer.m_pSize, sizeof(WORD) * buffer.m_nBufferSize );

			m_pCPtoXSize  = NTL_NEW INT[buffer.m_nBufferSize];
			memcpy( m_pCPtoXSize, buffer.m_pCPtoXSize, sizeof(INT) * buffer.m_nBufferSize );
		}
	}

	m_bAnalyseRequired = buffer.m_bAnalyseRequired;
	m_Analysis = buffer.m_Analysis;
	m_pFont = buffer.m_pFont;
		
}

CUniBuffer::~CUniBuffer()
{
	NTL_ARRAY_DELETE( m_pSize );
	NTL_ARRAY_DELETE( m_pCPtoXSize );
	NTL_ARRAY_DELETE( m_pwszBuffer );

	m_nBufferSize = 0;

    if( m_Analysis )
        _ScriptStringFree( &m_Analysis );
}

WCHAR& CUniBuffer::operator[]( int n )  // No param checking
{
    // This version of operator[] is called only
    // if we are asking for write access, so
    // re-analysis is required.

    m_bAnalyseRequired = true;
    return m_pwszBuffer[n];
}

VOID CUniBuffer::SetFont( const CGuiFont *pFont )
{
	// 만약 이전의 폰트가 있었다면 Uniscribe와 동기를 맞춰줘야 한다.
	if( m_pFont )
	{
		m_bAnalyseRequired = true;
	
		// 폰트를 적용하고,
		m_pFont = (CGuiFont*)pFont;

		// Uniscribe말고도 UI Library내부의 보관하고 있는 폰트 Size를
		// 여기서 계산한다. ( m_bSizeCalc 모드일 경우. )
		if( m_bSizeCalc )
		{
			SIZE size;
			int nBufferSize = GetTextSize(); 

			HFONT hFont = (HFONT)SelectObject(m_pFont->GetHdc(), m_pFont->GetHFont());

			int nWidth = 0;
			for(int i = 0; i < nBufferSize; i++)
			{
				::GetTextExtentPoint32( m_pFont->GetHdc(), &m_pwszBuffer[i], 1, &size );
				m_pSize[i] = (WORD)size.cx;
				m_pCPtoXSize[i] = nWidth;
				nWidth += (INT)size.cx;
			}

			SelectObject(m_pFont->GetHdc(), hFont);
		}
	}
	// 가장 처음 실행이라면 그냥 Font 셋팅.
	else
		m_pFont = (CGuiFont*)pFont;
}

VOID CUniBuffer::Clear()
{
	if(m_pwszBuffer == NULL)
		return;

    *m_pwszBuffer = L'\0';
	
	if(m_pSize) 
		memset( m_pSize, 0, sizeof(WORD) * m_nBufferSize );

	if(m_pCPtoXSize)
		memset( m_pCPtoXSize, 0, sizeof(INT) * m_nBufferSize );
		
    m_bAnalyseRequired = true;
}


//--------------------------------------------------------------------------------------
// Inserts the char at specified index.
// If nIndex == -1, insert to the end.
//--------------------------------------------------------------------------------------
bool CUniBuffer::InsertChar( int nIndex, WCHAR wChar )
{
    assert( nIndex >= 0 );

    if( nIndex < 0 || nIndex > lstrlenW( m_pwszBuffer ) )
        return false;  // invalid index

    // Check for maximum length allowed
    if( GetTextSize() + 1 >= MAX_INPUTBOX_LENGTH )
        return false;

    if( lstrlenW( m_pwszBuffer ) + 1 >= m_nBufferSize )
    {
        if( !SetBufferSize( -1 ) )
            return false;  // out of memory
    }

    assert( m_nBufferSize >= 2 );

    // Shift the characters after the index, start by copying the null terminator
	int nLen = lstrlenW(m_pwszBuffer);
    WCHAR* dest = m_pwszBuffer + nLen+1;
    WCHAR* stop = m_pwszBuffer + nIndex;
    WCHAR* src = dest - 1;
	
    while( dest > stop )
    {
        *dest-- = *src--;
    }

	if(m_pSize)
	{
		WORD* sizedest = m_pSize + nLen+1;
		WORD* sizestop = m_pSize + nIndex;
		WORD* sizesrc = sizedest - 1;

		while( sizedest > sizestop )
		{
			*sizedest-- = *sizesrc--;
		}
		
		if(m_pFont)
		{
			HFONT hFont = (HFONT)SelectObject(m_pFont->GetHdc(), m_pFont->GetHFont());

			SIZE size;
			::GetTextExtentPoint32( m_pFont->GetHdc(), &wChar, 1, &size );
			m_pSize[nIndex] = (WORD)size.cx;

			SelectObject(m_pFont->GetHdc(), hFont);
		}
	}

	 // Set new character
    m_pwszBuffer[ nIndex ] = wChar;
    m_bAnalyseRequired = true;
  	
    return true;
}


//--------------------------------------------------------------------------------------
// Removes the char at specified index.
// If nIndex == -1, remove the last char.
//--------------------------------------------------------------------------------------
bool CUniBuffer::RemoveChar( int nIndex )
{
	int nLen = lstrlenW( m_pwszBuffer );
    if( !nLen || nIndex < 0 || nIndex >= nLen )
        return false;  // Invalid index

    MoveMemory( m_pwszBuffer + nIndex, m_pwszBuffer + nIndex + 1, sizeof(WCHAR) * ( lstrlenW( m_pwszBuffer ) - nIndex ) );
    m_bAnalyseRequired = true;

	if(m_pFont && m_pSize)
	{
		MoveMemory( m_pSize + nIndex, m_pSize + nIndex + 1, sizeof(WORD) * ( nLen - nIndex ) );
	}

    return true;
}


//--------------------------------------------------------------------------------------
// Inserts the first nCount characters of the string pStr at specified index.
// If nCount == -1, the entire string is inserted.
// If nIndex == -1, insert to the end.
//--------------------------------------------------------------------------------------
bool CUniBuffer::InsertString( int nIndex, const WCHAR *pStr, int nCount )
{
    assert( nIndex >= 0 );

    if( nIndex > lstrlenW( m_pwszBuffer ) )
        return false;  // invalid index

    if( -1 == nCount )
        nCount = lstrlenW( pStr );

    // Check for maximum length allowed
    if( GetTextSize() + nCount >= MAX_INPUTBOX_LENGTH )
        return false;

    if( lstrlenW( m_pwszBuffer ) + nCount >= m_nBufferSize )
    {
        if( !SetBufferSize( lstrlenW( m_pwszBuffer ) + nCount + 1 ) )
            return false;  // out of memory
    }

	int nLen = lstrlenW( m_pwszBuffer );

    MoveMemory( m_pwszBuffer + nIndex + nCount, m_pwszBuffer + nIndex, sizeof(WCHAR) * ( nLen - nIndex + 1 ) );
    CopyMemory( m_pwszBuffer + nIndex, pStr, nCount * sizeof(WCHAR) );
    m_bAnalyseRequired = true;

	if(m_pFont && m_pSize)
	{
		HFONT hFont = (HFONT)SelectObject(m_pFont->GetHdc(), m_pFont->GetHFont());

		SIZE size;
		MoveMemory( m_pSize + nIndex + nCount, m_pSize + nIndex, sizeof(WORD) * ( nLen - nIndex + 1 ) );
		for(int i = 0; i < nCount; i++)
		{
			::GetTextExtentPoint32( m_pFont->GetHdc(), &pStr[i], 1, &size );
			m_pSize[nIndex+i] = (WORD)size.cx;
		}

		SelectObject(m_pFont->GetHdc(), hFont);
	}

    return true;
}


bool CUniBuffer::SetText( LPCWSTR wszText )
{
    assert( wszText != NULL );

    int nRequired = int(wcslen( wszText ) + 1);

    // Check for maximum length allowed
    if( nRequired >= MAX_INPUTBOX_LENGTH )
        return false;

	while (GetBufferSize() < nRequired)
	{
		if (!SetBufferSize(-1))
			break;
	}

    // Check again in case out of memory occurred inside while loop.
    if( GetBufferSize() >= nRequired )
    {
        wcscpy_s( m_pwszBuffer, GetBufferSize(), wszText );
        m_bAnalyseRequired = true;

		// If m_pSize is assigned, the width of the character is calculated and stored in m_pSize.
		if( m_bSizeCalc )
		{
			SIZE size;
			int nBufferSize = GetTextSize(); 

			HFONT hFont = (HFONT)SelectObject(m_pFont->GetHdc(), m_pFont->GetHFont());

			int nWidth = 0;
			for(int i = 0; i < nBufferSize; i++)
			{
				::GetTextExtentPoint32( m_pFont->GetHdc(), &wszText[i], 1, &size );
				m_pSize[i] = (WORD)size.cx;
				m_pCPtoXSize[i] = nWidth;
				nWidth += (INT)size.cx;
			}

			SelectObject(m_pFont->GetHdc(), hFont);
		}

        return true;
    }
	else
	{
		return false;
	}
}


HRESULT CUniBuffer::CPtoX( int nCP, BOOL bTrail, int *pX )
{
	assert( pX );
	*pX = 0;  // Default
	HRESULT hr = S_OK;

	if( m_bSizeCalc )
	{
		*pX = GetItemSizeByFirst( nCP );
	}
	else
	{	
		if( m_bAnalyseRequired )
			hr = Analyse();

		if( SUCCEEDED( hr ) )
			hr = _ScriptStringCPtoX( m_Analysis, nCP, bTrail, pX );
	}   

	return hr;
}


HRESULT CUniBuffer::XtoCP( int nX, int *pCP, int *pnTrail )
{
    assert( pCP && pnTrail );
    *pCP = 0; *pnTrail = FALSE;  // Default

    HRESULT hr = S_OK;
    if( m_bAnalyseRequired )
        hr = Analyse();

    if( SUCCEEDED( hr ) )
        hr = _ScriptStringXtoCP( m_Analysis, nX, pCP, pnTrail );

    // If the coordinate falls outside the text region, we
    // can get character positions that don't exist.  We must
    // filter them here and convert them to those that do exist.
    if( *pCP == -1 && *pnTrail)
    {
        *pCP = 0; *pnTrail = FALSE;
    } 
	else
	{
		if( *pCP > lstrlenW( m_pwszBuffer ) && *pnTrail == FALSE )
		{
			*pCP = lstrlenW( m_pwszBuffer ); *pnTrail = TRUE;
		}
	}

    return hr;
}


VOID CUniBuffer::GetPriorItemPos( int nCP, int *pPrior )
{
    *pPrior = nCP;  // Default is the char itself

    if( m_bAnalyseRequired )
        if( FAILED( Analyse() ) )
            return;

    const SCRIPT_LOGATTR *pLogAttr = _ScriptString_pLogAttr( m_Analysis );
    if( !pLogAttr )
        return;

    if( !_ScriptString_pcOutChars( m_Analysis ) )
        return;
    int nInitial = *_ScriptString_pcOutChars( m_Analysis );
    if( nCP - 1 < nInitial )
        nInitial = nCP - 1;

    for( int i = nInitial; i > 0; --i )
	{
        if( pLogAttr[i].fWordStop ||       // Either the fWordStop flag is set
            ( !pLogAttr[i].fWhiteSpace &&  // Or the previous char is whitespace but this isn't.
                pLogAttr[i-1].fWhiteSpace ) )
        {
            *pPrior = i;
            return;
        }
	}

    // We have reached index 0.  0 is always a break point, so simply return it.
    *pPrior = 0;
}
    

VOID CUniBuffer::GetNextItemPos( int nCP, int *pPrior )
{
    *pPrior = nCP;  // Default is the char itself

    HRESULT hr = S_OK;
    if( m_bAnalyseRequired )
        hr = Analyse();
    if( FAILED( hr ) )
        return;

    const SCRIPT_LOGATTR *pLogAttr = _ScriptString_pLogAttr( m_Analysis );
    if( !pLogAttr )
        return;

    if( !_ScriptString_pcOutChars( m_Analysis ) )
        return;
    int nInitial = *_ScriptString_pcOutChars( m_Analysis );
    if( nCP + 1 < nInitial )
        nInitial = nCP + 1;
    for( int i = nInitial; i < *_ScriptString_pcOutChars( m_Analysis ) - 1; ++i )
    {
        if( pLogAttr[i].fWordStop )      // Either the fWordStop flag is set
        {
            *pPrior = i;
            return;
        }
        else
        if( pLogAttr[i].fWhiteSpace &&  // Or this whitespace but the next char isn't.
            !pLogAttr[i+1].fWhiteSpace )
        {
            *pPrior = i+1;  // The next char is a word stop
            return;
        }
    }
    // We have reached the end. It's always a word stop, so simply return it.
    *pPrior = *_ScriptString_pcOutChars( m_Analysis ) - 1;
}

INT CUniBuffer::GetItemSize( int nCP )
{
	if(m_pSize == NULL)
		return 0;
		
	if(nCP <= 0)
		return 0;
		
	nCP = nCP - 1;

	if(nCP >= m_nBufferSize)
		return 0;
		
	return (INT)m_pSize[nCP];
}

INT CUniBuffer::GetItemSizeByFirst( int nCP )
{
	if(m_pCPtoXSize == NULL )
		return 0;

	if(nCP >= m_nBufferSize )
		return 0;

	if( nCP < 0 )
		return 0;
	
	INT nTextSize = GetTextSize();
	if( nCP >= nTextSize )
		return (INT)m_pCPtoXSize[nTextSize-1] + (INT)m_pSize[nTextSize-1];
	
	return (INT)m_pCPtoXSize[nCP];	
}

VOID CUniBuffer::SetSizeCopy( WORD* pSize, int* pCPtoXSize, int nBufferSize )
{
	NTL_ARRAY_DELETE( m_pSize );
	NTL_ARRAY_DELETE( m_pCPtoXSize );

	assert( m_nBufferSize || "CUniBuffer::SetSizeCopy - BufferSize is 0" );

	m_pSize = NTL_NEW WORD[m_nBufferSize];
	m_pCPtoXSize = NTL_NEW int[m_nBufferSize];

	::CopyMemory( m_pSize, pSize, sizeof(WORD) * nBufferSize );
	::CopyMemory( m_pCPtoXSize, pCPtoXSize, sizeof(INT) * nBufferSize );
}

WORD* CUniBuffer::GetSizeBuffer()
{
	if( m_pSize )
		return m_pSize;

	return NULL;
}

INT* CUniBuffer::GetCPtoXSizeBuffer()
{
	if( m_pCPtoXSize )
		return m_pCPtoXSize;

	return NULL;
}

END_GUI