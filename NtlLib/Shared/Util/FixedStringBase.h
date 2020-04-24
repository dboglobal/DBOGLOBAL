//*****************************************************************************
// File       : FixedStringBase.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___FIXED_STRING_BASE_H___
#define ___FIXED_STRING_BASE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//#define _DEBUG_STRING_
#define _FAST_COMPARE_
//#define _NULL_TEST_

//#define Assert( VAL )								(void)0


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include <list>
using std::list;

#include "BaseType.h"
#include "UserDefinedAssert.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define NULL_CHAR									_T( '\0' )

#ifdef _NULL_TEST_
	#define NULL_STRING								_T( "(Null)\0" )
#else
	#define NULL_STRING								_T( "\0" )
#endif //_NULL_TEST_

#define LINE_FEED_CHAR								_T( '\n' )
#define LINE_FEED_STRING							_T( "\n" )

#define ANSI_INDEX_EXTEND_SET						( 128 )
#define ANSI_INDEX_ALPHABET_UPPERCASE_A				( 65 )
#define ANSI_INDEX_ALPHABET_UPPERCASE_Z				( 90 )
#define ANSI_INDEX_ALPHABET_LOWERCASE_A				( 97 )
#define ANSI_INDEX_ALPHABET_LOWERCASE_Z				( 122 )
#define ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE	( 32 )

#define MAX_NUMBER_LENGTH							( 32 )

#define FIXED_STRING_SEARCH_FAIL					( 0xFFFFFFFF )

#define FIXED_STRING_SEPARATORS_DEFAULT_A			" ;\t\r\n\0"
#define FIXED_STRING_SEPARATORS_DEFAULT_W			L" ;\t\r\n\0"
#define FIXED_STRING_TEXT_INDICATOR_DEFAULT_A		"\"\0"
#define	FIXED_STRING_TEXT_INDICATOR_DEFAULT_W		L"\"\0"
#define FIXED_STRING_COMMENT_INDICATOR_DEFAULT_A	"//\0"
#define FIXED_STRING_COMMENT_INDICATOR_DEFAULT_W	L"//\0"

#define C_STRING_LIST_POSITION_FRONT				( 0 )
#define C_STRING_LIST_POSITION_BACK					( 0xFFFFFFFF )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum EFixedStringListParseState
{
	E_FIXED_STRING_LIST_PARSE_STATE_NONE,
	E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_COMMENT,
	E_FIXED_STRING_LIST_PARSE_STATE_IN_COMMENT,
	E_FIXED_STRING_LIST_PARSE_STATE_END_COMMENT,
	E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_STRING,
	E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_STRING_PRE,
	E_FIXED_STRING_LIST_PARSE_STATE_IN_STRING,
	E_FIXED_STRING_LIST_PARSE_STATE_END_STRING,
	E_FIXED_STRING_LIST_PARSE_STATE_END_STRING_POST,
	E_FIXED_STRING_LIST_PARSE_STATE_BEGIN_TEXT,
	E_FIXED_STRING_LIST_PARSE_STATE_IN_TEXT,
	E_FIXED_STRING_LIST_PARSE_STATE_END_TEXT,
	E_FIXED_STRING_LIST_PARSE_STATE_COUNT
} EFixedStringListParseState;


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

END_NAMESPACE( str )

#endif //___FIXED_STRING_BASE_H___

//*****************************************************************************
//
// End of File : FixedStringBase.h
//
//*****************************************************************************