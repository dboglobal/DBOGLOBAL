//*****************************************************************************
// File       : CFixedStringList.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef _C_FIXED_STRING_LIST_H_
#define _C_FIXED_STRING_LIST_H_

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"
#include "CFixedString.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define C_STRING_LIST_POSITION_FRONT	( 0 )
#define C_STRING_LIST_POSITION_BACK		( (UINT)-1 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<CFixedString *>	 		TFixedStringList;
typedef TFixedStringList::iterator		TFixedStringListIterator;


//=============================================================================
// Name       : CStringList
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringList
{
	protected:

		TFixedStringList *				m_ptList;
		TFixedStringListIterator		m_tCurrentPosition;

		UINT							m_nCount;

	public:

										CFixedStringList( void );
										~CFixedStringList( void );

	private:

										CFixedStringList( const CFixedStringList & rcFixedStringList );

		const CFixedStringList &		operator=( const CFixedStringList & rcFixedStringList );

	public:

		UINT							GetCount( void ) const { return m_nCount; }
		bool							IsEmpty( void ) const { return !m_nCount; }

		void							Push( CFixedString * lpcString );
		void							PushAt( CFixedString * lpcString, UINT nPosition );

		void							PushFast( CFixedString * lpcString );
		void							PushAtFast( CFixedString * lpcString, UINT nPosition );

		CFixedString *					Pop( bool bDestroy = false );
		CFixedString *					PopAt( UINT nPosition, bool bDestroy = false );
		void							PopAll( bool bDestroy = false );

		const CFixedString *			Get( void ) const;
		const CFixedString *			GetAt( UINT nPosition ) const;

		bool							MoveFront( void );
		bool							MoveBack( void );
		bool							MovePrev( void );
		bool							MoveNext( void );
		bool							MoveAt( UINT nPosition );

		UINT							GetBufferSize( void ) const;

		UINT							Serialize( TCHAR * lptBuffer, UINT nBufferSize ) const;
		UINT							Serialize( HANDLE hFile ) const;

	protected:

		TFixedStringListIterator		GetPrevIterator( TFixedStringListIterator tSearchPosition ) const;

	public:

		static CFixedString *			CreateString( const TCHAR * lptRawString = NULL );
		static void						DestroyString( CFixedString ** pptString );

	public:

		static const UINT				POSITION_FRONT;
		static const UINT				POSITION_BACK;
};


//=============================================================================
// End of Class : CFixedStringList
//=============================================================================

#endif //_C_FIXED_STRING_LIST_H_

//*****************************************************************************
//
// End of File : CFixedStringList.h
//
//*****************************************************************************