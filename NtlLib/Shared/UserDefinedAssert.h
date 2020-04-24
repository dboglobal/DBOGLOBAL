//*****************************************************************************
// File       : UserDefindAssert.h
// Desc       : User Defined Assert Header File
// Begin      : 
// Copyright  : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 8. 7
//*****************************************************************************

#ifndef ___USER_DEFFINED_ASSERT_H___
#define ___USER_DEFFINED_ASSERT_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

// #include <intrin.h>
#include <stdio.h>
// #include <tchar.h>

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( uda )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#ifdef _DEBUG
	#define ASSERT_MODE							( ASSERT_MODE_USER_DEFINED_ASSERT )
#else //_DEBUG
	#define ASSERT_MODE							( ASSERT_MODE_USER_DEFINED_ASSERT )
#endif //_DEBUG

#ifndef MB_CANCELTRYCONTINUE
	#define MB_CANCELTRYCONTINUE				MB_ABORTRETRYIGNORE
	#define IDTRYAGAIN							IDRETRY
#endif //MB_CANCELTRYCONTINUE


#define ASSERT_MODE_NO_OPERATION				( 0 )
#define ASSERT_MODE_DEFAULT_ASSERT				( 1 )
#define ASSERT_MODE_USER_DEFINED_ASSERT			( 2 )

#define ASSERT_SELECT_MODE_BREAK				( IDCANCEL )
#define ASSERT_SELECT_MODE_RETRY				( IDTRYAGAIN )
#define ASSERT_SELECT_MODE_IGNORE				( IDIGNORE )

#define ASSERT_TEXT_BUFFER_SIZE					( 512 )

#if ( ASSERT_MODE == ASSERT_MODE_USER_DEFINED_ASSERT )
	#define Assert( Evaluation )				while( (bool)(Evaluation) == false && UserDefinedAssert( _T( #Evaluation ), _T( __FILE__ ), __LINE__ ) )
	#define AssertV( Evaluation, Description )	while( (bool)(Evaluation) == false && UserDefinedAssert( _T( Description ), _T( __FILE__ ), __LINE__ ) )
#elif ( ASSERT_MODE == ASSERT_MODE_DEFAULT_ASSERT )
	#define Assert( Evaluation )				( assert( Evaluation ) )
	#define AssertV( Evaluation, Description )	( assert( Evaluation ) )
#else
	#define Assert( Evaluation )				( (void)0 )
	#define AssertV( Evaluation, Description )	( (void)0 )
#endif //ASSERT_MODE

#define AssertE( Evaluation )					( Assert( !(Evaluation) ) )


//=============================================================================
// Name       : UserDefinedAssert
// Desc       : 
// Author     : 
//=============================================================================

inline bool UserDefinedAssert( const TCHAR * lptExpression, const TCHAR * lptFileName, int nLine )
{
	int			nResult;
	TCHAR		atAssertMessage[ASSERT_TEXT_BUFFER_SIZE];

	_stprintf_s( atAssertMessage, ASSERT_TEXT_BUFFER_SIZE,
				_T( "File\t%s\r\nLine\t%d\r\nExpr\t%s" ),
				lptFileName, nLine, lptExpression );

	nResult = MessageBox( static_cast<HWND>(NULL), atAssertMessage, _T( "Assert" ),
						MB_CANCELTRYCONTINUE | MB_ICONSTOP | MB_SYSTEMMODAL );

	if( nResult == IDCANCEL )
	{
		__debugbreak();			// old version : _asm{ int 3 }

		return false;
	}
	else if( nResult == IDTRYAGAIN )
	{
		return true;
	}
	else
	{
		return false;
	}
}

END_NAMESPACE( uda )

#endif //___USER_DEFFINED_ASSERT_H___

//*****************************************************************************
//
// End of File : UserDefindAssert.cpp
//
//*****************************************************************************