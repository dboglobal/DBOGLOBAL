//////////////////////////////////////////////////////////////////////////////
//
// ScriptFileExtractor.h : Script File Extractor Class Header
//
//        Programed by June C. Kim (cynis@hotmail.com)
//
//        Last Update 2003.6.20
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDED_SCRIPT_EXTRACTOR_H_
#define _INCLUDED_SCRIPT_EXTRACTOR_H_

//////////////////
// Header Table //
//////////////////

#include "BaseType.h"


///////////////////////////////
// Type Pre-Definition Table //
///////////////////////////////

class CScriptFileExtractor;
typedef CScriptFileExtractor*	LPCSCRIPTFILEEXTRACTOR;


///////////////////////////////
// Constant Definition Table //
///////////////////////////////

#define SFE_INITIAL_SIZE					( 256 )

#define SFE_STATE_FILELODING				( 0x0001 )
#define SFE_STATE_PARSING					( 0x0002 )
#define SFE_STATE_UNMODIFIED				( 0x0010 )
#define SFE_STATE_MODIFIED					( 0x0020 )
#define SFE_STATE_DISABLED					( 0x8000 )

#define SFE_WORDTYPE_STRING					( 0x0001 )
#define SFE_WORDTYPE_INT					( 0x0002 )
#define SFE_WORDTYPE_FLOAT					( 0x0003 )

#define SFE_WORDSTATE_OUTOFWORD				( 0x0000 )
#define SFE_WORDSTATE_BEGINOFWORD			( 0x0001 )
#define SFE_WORDSTATE_INWORD				( 0x0002 )
#define SFE_WORDSTATE_ENDOFWORD				( 0x0004 )
#define SFE_WORDSTATE_BEGINOFSTRING			( 0x0010 )
#define SFE_WORDSTATE_INSTRING				( 0x0020 )
#define SFE_WORDSTATE_ENDOFSTRING			( 0x0040 )
#define SFE_WORDSTATE_BEGINOFVALUE			( 0x0100 )
#define SFE_WORDSTATE_INVALUE				( 0x0200 )
#define SFE_WORDSTATE_ENDOFVALUE			( 0x0400 )
#define SFE_WORDSTATE_BEGINOFCOMMENT		( 0x1000 )
#define SFE_WORDSTATE_INCOMMENT				( 0x2000 )
#define SFE_WORDSTATE_ENDOFCOMMENT			( 0x4000 )

#define SFE_INDEX_FIRSTLINE					( 1 )
#define SFE_INDEX_FIRSTWORD					( 1 )

#define SFE_INITIAL_STRING					( (LPSTR)NULL )
#define SFE_INITIAL_INT						( 0 )
#define SFE_INITIAL_FLOAT					( 0.0f )


///////////////////////////
// Type Definition Table //
///////////////////////////

typedef struct SScriptFileInfo
{
	TCHAR	atFileName[MAX_PATH_LENGTH];
	WORD	wWordCount;
} SScriptFileInfo, SCRIPTFILEINFO, *LPSCRIPTFILEINFO;


typedef struct SWordInfo
{
	WORD	wLineIndex;
	WORD	wWordIndex;
	WORD	wWordType;
	WORD	wWordLength;
	TCHAR *	lptWord;
} SWordInfo, WORDINFO, *LPWORDINFO;


////////////////////////////////
// class CScriptFileExtractor //
////////////////////////////////

class CScriptFileExtractor
{
	public:

		CScriptFileExtractor( void );
		~CScriptFileExtractor( void );

	public:

		bool						Create( TCHAR * lptFileName );
		bool						Destroy( void );

		DWORD						GetLastError( SWordInfo * lpsErrorWordInfo = (SWordInfo *)NULL );
		void						SetLastError( DWORD dwErrorCode, SWordInfo * lpsErrorWordInfo = (SWordInfo *)NULL );

		bool						GetScriptFileInfo( SScriptFileInfo * lpsScriptFileInfo );
		WORD						GetWordCount( void ) const { return m_wWordCount; }
		const TCHAR *				GetFileName( void ) const { return m_atFileName; }

		const SWordInfo *			GetWordInfo( WORD wWordIndex );

		const  SWordInfo *			operator[]( WORD wWordIndex );

		bool						Report( TCHAR * lptFileName );

		bool						SearchKeyword( WORD &rwIndex, TCHAR * lptKeyword, TCHAR * lptForbiddenChar = (TCHAR *)NULL );

		bool						GetString( WORD &rwIndex, TCHAR * &rlptResultword, TCHAR * lptForbiddenChar = (TCHAR *)NULL );
		bool						GetInt( WORD &rwIndex, long &rnInt );
		bool						GetFloat( WORD &rwIndex, float &rfFloat );

		bool						GetStringAfterSearchKeyword( WORD &rwIndex, TCHAR * lptKeyword, TCHAR * &rlptResultword, TCHAR * lptForbiddenSearchChar = (TCHAR *)NULL, TCHAR * lptForbiddenGetChar = (TCHAR *)NULL );
		bool						GetIntAfterSearchKeyword( WORD &rwIndex, TCHAR * lptKeyword, long &rnInt, TCHAR * lptForbiddenSearchChar = (TCHAR *)NULL );
		bool						GetFloatAfterSearchKeyword( WORD &rwIndex, TCHAR * lptKeyword, float &rfFloat, TCHAR * lptForbiddenSearchChar = (TCHAR *)NULL );

	protected:

		void						Initialize( void );

		bool						ExpandWordList( WORD wNewWordListCount );
		void						DestroyWordList( void );

		long						ReadScript( HANDLE hFile, TCHAR ** lpptTextBuffer );
		bool						ParseScript( DWORD dwTextSize, TCHAR * lptTextBuffer );

		bool						InsertWord( SWordInfo * lpsWordInfo );

		HANDLE						CreateFileSafe( LPCTSTR lpctstrFileName, bool bForRead );

	protected:

		TCHAR						m_atFileName[MAX_PATH_LENGTH];

		WORD						m_wState;

		WORD						m_wMaxWordCount;
		WORD						m_wWordCount;

		DWORD						m_dwErrorCode;
		SWordInfo *					m_psErrorWordInfo;

		SWordInfo **				m_ppsWordList;

	public:

		static long					StringToLong( TCHAR * lptString );
		static float				StringToFloat( TCHAR * lptString );

	public:

		static const WORD			INITIAL_SIZE;

		static const WORD			STATE_FILELODING;
		static const WORD			STATE_PARSING;
		static const WORD			STATE_UNMODIFIED;
		static const WORD			STATE_MODIFIED;
		static const WORD			STATE_DISABLED;

		static const WORD			WORDTYPE_STRING;
		static const WORD			WORDTYPE_INT;
		static const WORD			WORDTYPE_FLOAT;

		static const WORD			WORDSTATE_OUTOFWORD;
		static const WORD			WORDSTATE_BEGINOFWORD;
		static const WORD			WORDSTATE_INWORD;
		static const WORD			WORDSTATE_ENDOFWORD;
		static const WORD			WORDSTATE_BEGINOFSTRING;
		static const WORD			WORDSTATE_INSTRING;
		static const WORD			WORDSTATE_ENDOFSTRING;
		static const WORD			WORDSTATE_BEGINOFVALUE;
		static const WORD			WORDSTATE_INVALUE;
		static const WORD			WORDSTATE_ENDOFVALUE;
		static const WORD			WORDSTATE_BEGINOFCOMMENT;
		static const WORD			WORDSTATE_INCOMMENT;
		static const WORD			WORDSTATE_ENDOFCOMMENT;

		static const WORD			INDEX_FIRSTLINE;
		static const WORD			INDEX_FIRSTWORD;

		static const LPSTR			INITIAL_STRING;
		static const long			INITIAL_INT;
		static const float			INITIAL_FLOAT;

	// Friend Functions

		friend bool IsString( SWordInfo * lpsWordInfo )
		{
			return ( ( lpsWordInfo != NULL ) ? ( (lpsWordInfo->wWordType == SFE_WORDTYPE_STRING ) ? true : false ) : false );
		}

		friend bool IsInt( SWordInfo * lpsWordInfo )
		{
			return ( ( lpsWordInfo != NULL ) ? ( (lpsWordInfo->wWordType == SFE_WORDTYPE_INT ) ? true : false ) : false );
		}

		friend bool IsFloat( SWordInfo * lpsWordInfo )
		{
			return ( ( lpsWordInfo != NULL ) ? ( (lpsWordInfo->wWordType == SFE_WORDTYPE_FLOAT ) ? true : false ) : false );
		}

		friend const TCHAR * MakeString( SWordInfo * lpsWordInfo )
		{
			return lpsWordInfo->lptWord;
		}

		friend long MakeInt( SWordInfo * lpsWordInfo )
		{
			return StringToLong( lpsWordInfo->lptWord );
		}
		
		friend float MakeFloat( SWordInfo * lpsWordInfo )
		{
			return StringToFloat( lpsWordInfo->lptWord );
		}
};

#endif _INCLUDED_SCRIPT_EXTRACTOR_H_

//////////////////////////////////////////////////////////////////////////////
//
// End of File : ScriptExtractor.h
//
//////////////////////////////////////////////////////////////////////////////