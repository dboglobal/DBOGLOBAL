//////////////////////////////////////////////////////////////////////////////
//
// UserAssert.h : User-Defined Assert Header File
//
//        Programmed by June C. Kim (cynis@hotmail.com)
//
//        Last Update 2003.6.30
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _USER_DEFFINED_ASSERT_H_
#define _USER_DEFFINED_ASSERT_H_

//////////////////////////
// Compile Option Table //
//////////////////////////

//////////////////
// Header Table //
//////////////////

////////////////////
// Constant Table //
////////////////////

#ifndef MB_CANCELTRYCONTINUE
	#define MB_CANCELTRYCONTINUE MB_ABORTRETRYIGNORE
#endif //MB_CANCELTRYCONTINUE

#ifndef IDTRYAGAIN
	#define IDTRYAGAIN IDRETRY
#endif //IDTRYAGAIN

#define ASSERT_MODE_NO_OPERATION				( 0 )
#define ASSERT_MODE_DEFAULT_ASSERT				( 1 )
#define ASSERT_MODE_USER_DEFINED_ASSERT			( 2 )

#define ASSERT_OUTPUT_MODE_DIALOG				( 1 )
#define ASSERT_OUTPUT_MODE_LOG					( 2 )
#define ASSERT_OUTPUT_MODE_ALL					( ASSERT_OUTPUT_MODE_DIALOG | ASSERT_OUTPUT_MODE_LOG )

#define ASSERT_SELECT_MODE_BREAK				( IDCANCEL )
#define ASSERT_SELECT_MODE_RETRY				( IDTRYAGAIN )
#define ASSERT_SELECT_MODE_IGNORE				( IDIGNORE )

#ifdef _DEBUG
	#define ASSERT_MODE							( ASSERT_MODE_USER_DEFINED_ASSERT )

#if ( ASSERT_MODE == ASSERT_MODE_USER_DEFINED_ASSERT )
	#define ASSERT_OUTPUT_MODE					( ASSERT_OUTPUT_MODE_DIALOG )
#endif

#if ( ASSERT_OUTPUT_MODE == ASSERT_OUTPUT_MODE_LOG )
	#define ASSERT_SELECT_MODE					( ASSERT_SELECT_MODE_BREAK )
#endif

#else //_DEBUG
	#define ASSERT_MODE							( ASSERT_MODE_NO_OPERATION )

#if ( ASSERT_MODE == ASSERT_MODE_USER_DEFINED_ASSERT )
	#define ASSERT_OUTPUT_MODE					( ASSERT_OUTPUT_MODE_LOG )
#endif

#if ( ASSERT_OUTPUT_MODE == ASSERT_OUTPUT_MODE_LOG )
	#define ASSERT_SELECT_MODE					( ASSERT_SELECT_MODE_IGNORE )
#endif
#endif //_DEBUG

#ifdef Assert
	#undef Assert
#endif //Assert

#if ( ASSERT_MODE == ASSERT_MODE_DEFAULT_ASSERT )
	#define Assert( Evaluation )				( assert( Evaluation ) )
	#define AssertV( Evaluation, Description )	( assert( Evaluation ) )
#elif ( ASSERT_MODE == ASSERT_MODE_USER_DEFINED_ASSERT )
	#define Assert( Evaluation )				while( Evaluation == 0 && UserDefinedAssert( _T( #Evaluation ), _T( __FILE__ ), __LINE__ ) == true );
	#define AssertV( Evaluation, Description )	while( Evaluation == 0 && UserDefinedAssert( _T( Description ), _T( __FILE__ ), __LINE__ ) == true );
#else
	#define Assert( Evaluation )				( (void)0 )
	#define AssertV( Evaluation, Description )	( (void)0 )
#endif


inline bool UserDefinedAssert( const TCHAR * lptExpression, LPTSTR lptFileName, int nLine )
{
	int			nResult;
	TCHAR		atAssertMessage[512];

	_stprintf( atAssertMessage, _T( "Expression : %s\nFile : %s\nLine : %d" ), lptExpression, lptFileName, nLine );

#if ( ASSERT_OUTPUT_MODE | ASSERT_OUTPUT_MODE_DIALOG )
	nResult = MessageBox( (HWND)NULL, atAssertMessage, _T( "Assert" ),
						MB_CANCELTRYCONTINUE | MB_ICONSTOP | MB_SYSTEMMODAL );
#else
	nResult = ASSERT_SELECT_MODE;
#endif

	if( nResult == IDCANCEL || nResult == IDABORT )
	{
#if ( ASSERT_OUTPUT_MODE | ASSERT_OUTPUT_MODE_LOG )
		_stprintf( atAssertMessage, _T( "\r\nExpression : %s\r\nFile : %s\r\nLine : %d\r\n" ),
					lptExpression, lptFileName, nLine );
		g_Error( atAssertMessage );
#endif
		_asm{ int 3 }

		return false;
	}
	else if( nResult == IDTRYAGAIN || nResult == IDRETRY )
	{
#if ( ASSERT_OUTPUT_MODE | ASSERT_OUTPUT_MODE_LOG )
		_stprintf( atAssertMessage, _T( "\r\nExpression : %s\r\nFile : %s\r\nLine : %d\r\n" ),
					lptExpression, lptFileName, nLine );
		g_Error( atAssertMessage );
#endif
		return true;
	}
	else
	{
#if ( ASSERT_OUTPUT_MODE | ASSERT_OUTPUT_MODE_LOG )
		_stprintf( atAssertMessage, _T( "\r\nExpression : %s\r\nFile : %s\r\nLine : %d\r\n" ),
					lptExpression, lptFileName, nLine );
		g_Error( atAssertMessage );
#endif
		return false;
	}
}

#endif //_USER_DEFFINED_ASSERT_H_

//////////////////////////////////////////////////////////////////////////////
//
// End of File : UserAssert.h
//
//////////////////////////////////////////////////////////////////////////////
