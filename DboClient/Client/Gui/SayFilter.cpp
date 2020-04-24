#include "precomp_dboclient.h"
#include "SayFilter.h"

// core
#include "NtlDebug.h"

// shared
#include "TableContainer.h"
#include "ChattingFilterTable.h"
#include "TextAllTable.h"

// simulation
#include "NtlSLApi.h"
#include "NtlService.h"


CFilterManager::CFilterManager() 
: m_bIsEmpty( TRUE )
{
	InitOnce();
}

CFilterManager::~CFilterManager()
{
	Destroy();
}

void CFilterManager::InitOnce() 
{
	m_defaultReplace = L"--";

	m_pFilterWord = NTL_NEW CWFILTERWORD;
}

void CFilterManager::Destroy() 
{
	if( m_pFilterWord )
		NTL_DELETE(m_pFilterWord);
}

void CFilterManager::Create(void) 
{
	// TODO: Read data from the table and add it.
	// Add filtering word table
	CChattingFilterTable* pFilterTable = API_GetTableContainer()->GetChattingFilterTable();
	
	// The game should go on without the table.
	DBO_ASSERT( pFilterTable, "CFilterManager: filter table pointer is NULL");
	if( NULL == pFilterTable )
		return;

	sCHAT_FILTER_TBLDAT* pFilter = NULL;

	for(CChattingFilterTable::TABLEIT it = pFilterTable->Begin(); it != pFilterTable->End(); it++)
	{
		pFilter = (sCHAT_FILTER_TBLDAT*)it->second;
		AddFilter( pFilter->tblidx, pFilter->wszSlangText, pFilter->filteringTextIndex );
	}

	// If more than one word is registered, it is determined that the table is not empty.
	if( m_pFilterWord->GetNumFilter() > 0 )
		m_bIsEmpty = FALSE;
}

void CFilterManager::AddFilter(int nIndex, wchar_t* pText, int nLink)
{
	m_pFilterWord->AddText( nIndex, pText, lstrlenW( pText ), nLink );
}

/**
* \ brief UNICODE string filtering function
*
* UNICODE Replace abuse of string.
*
* \ param pText (std :: wstring &) The wstring to filter
* \ return (std :: wstring) Filtered wstring
*/
std::wstring CFilterManager::Filtering(std::wstring& pText)
{
	// If the table is not loaded, it just returns.
	if( m_bIsEmpty )
	{
		DBO_FAIL( "CFilterManager:: Filter word is empty.");
		return pText;
	}

	// Copy the source string and filter it, then return the filtered string.
	std::wstring		strText = pText;

	// It fills in the vecResult structure with the location and information values of the words to be filtered.
	std::vector<CWFILTERWORD::SResult> vecResult;
	m_pFilterWord->FindText(strText.c_str(), (int)strText.length(), vecResult);

	// Have a table in advance.
	CTextTable* pReplaceTable = API_GetTableContainer()->GetTextAllTable()->GetFilteringTable();
	
	// If the table fails to load, it returns without changing the word.
	DBO_ASSERT( pReplaceTable , "CFilterManager: Replace table pointer is NULL");
	if( NULL == pReplaceTable )
		return strText;
		
	// Execute the substitution in reverse order to the result entered in vecResult.
	// This is in case the character string length is changed when the character string is substituted before the string.
	for (int i = (int)vecResult.size() - 1; i >= 0; --i)
	{
		CWFILTERWORD::SResult& resultRef = vecResult.at(i);

		// Replace word to be filtered. strText.erase( StartPos, Offset );
		strText.erase(resultRef.uPosStart, resultRef.uPosEnd-resultRef.uPosStart+1);
		

		// If the index of the word to be replaced does not exist, change it to the default word.
		sTEXT_TBLDAT* pReplace = (sTEXT_TBLDAT*)pReplaceTable->FindData(resultRef.nLink);

		if( NULL == pReplace )
			strText.insert(resultRef.uPosStart, m_defaultReplace.c_str() );
		else
			strText.insert(resultRef.uPosStart, pReplace->wstrText.c_str() );
		
	}

	return strText;
}

/**
* \ brief MULTIBYTE string filtering function
*
* After converting MULTIBYTE string to UNICODE, std :: wstring CWSayFilterManager :: Filtering (std :: wstring & pText);
* To filter it, convert it back to MULTIBYTE, and return it. (Using ATL macros)
*
* \ param pText (std :: string &) The string to filter
* \ return (std :: string) Filtered string
*/
std::string CFilterManager::Filtering(std::string& pText)
{
	// Convert MULTIBYTE to UNICODE and perform filtering
	// Convert back to MULTIBYTE and return.
	// replace with WinAPI.
	
	WCHAR awcBuffer[64] = L" ";
	::MultiByteToWideChar( GetACP(), 0, pText.c_str(), -1, awcBuffer, 64 );
	
	std::wstring wstrResult = Filtering( awcBuffer );
	
	CHAR acBuffer[64]= " ";
	::WideCharToMultiByte( GetACP(), 0, wstrResult.c_str(), -1, acBuffer, 64, NULL, NULL );
	std::string strResult = acBuffer;

	return strResult;
}

/**
* \ brief UNICODE string filtering function
*
* \ param pText (wchar_t *) UNICODE string pointer to filter
* \ return (std :: wstring) Filtered wstring
*/
std::wstring CFilterManager::Filtering(const wchar_t* pText)
{
	std::wstring strText = pText;

	return Filtering( strText );
}

/**
* \ brief UNICODE string Hyphenation check function
*
* \ param pText (std :: wstring) String to check
* \ return (BOOL) return TRUE if abusive, FALSE otherwise
*/
RwBool CFilterManager::IsSlang(std::wstring& pText )
{
	// If the table is not loaded, it just returns.
	if( m_bIsEmpty )
	{
		DBO_FAIL("CFilterManager: Filterword is empty.");
		return FALSE;
	}

	std::wstring strText = pText;

	std::vector<CWFILTERWORD::SResult> vecResult;
	m_pFilterWord->FindText(strText.c_str(), (int)strText.length(), vecResult);

	// If one of the filter words is searched, this string is abusive.
	if( vecResult.size() > 0 )
		return TRUE;

	return FALSE;
}

RwBool CFilterManager::IsSlang(std::string& pText)
{
	WCHAR awcBuffer[64] = L" ";
	::MultiByteToWideChar( GetACP(), 0, pText.c_str(), -1, awcBuffer, 64 );


	std::wstring buf = awcBuffer;

	return IsSlang( buf );
}

RwBool CFilterManager::IsSlang( const wchar_t* pText )
{
	std::wstring buf = pText;

	return IsSlang( buf );
}

RwBool CFilterManager::IsAnyInvalidText( const WCHAR* pText )
{
	std::wstring buf = pText;

	if( IsSlang( buf ) )
		return TRUE;

	if(Dbo_IsValidText((WCHAR*)pText) == FALSE)
		return TRUE;

	return FALSE;
}