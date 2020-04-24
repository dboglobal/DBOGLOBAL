//*****************************************************************************
// File       : CHeightMap.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_HEIGHT_MAP_H__
#define __C_HEIGHT_MAP_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define CREATE_HEIGHT_SEARCH_OFFSET_MAX		(16.0f)

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CHeightMap
// Desc       : 
// Author     : 
//=============================================================================

class CHeightMap
{
	protected:

		SBoundingBox					m_sBounding;
		RwReal							m_fPrecision;
		SLocation3DInt					m_sNodeCount;
		RwUInt32						m_iNodeCount;

		RwReal *						m_pHeightMap;

	public:

										CHeightMap( void ) { Initialize(); }
										CHeightMap( const CHeightMap & rcHeightMap );

		virtual							~CHeightMap( void ) { Destroy(); }

	public:

		const CHeightMap &				operator =( const CHeightMap & rcHeightMap );

	public:

		virtual RwBool					Create( const SBoundingBox * pBounding, RwReal fPrecision );
		virtual void					Destroy( void );

		RwReal							GetNode( SLocation3DInt * pNodeIndex ) const;
		void							SetNode( SLocation3DInt * pNodeIndex, RwReal fHeight );

		const SLocation3DInt *			GetNodeCount( void ) const { return &m_sNodeCount; }

		RwBool							GetNodeIndex( SLocation3D * pPosition, SLocation3DInt * pNodeIndex ) const;
		RwReal							GetHeight( SLocation3D * pPosition ) const;

		RwBool							Import( const CFixedString * pName );
		RwBool							Export( const CFixedString * pName );

	protected:

		virtual void					Initialize( void );

		RwUInt32						CalculateNodeOffset( SLocation3DInt * pNodeIndex ) const;

		RwReal							GetInterpolatedHeightRightSide( SLocation3D * pOffset, RwReal fTop, RwReal fLeft, RwReal fRight ) const;
		RwReal							GetInterpolatedHeightBottomSide( SLocation3D * pOffset, RwReal fTop, RwReal fLeft, RwReal fRight ) const;

	protected:

		static CFixedString 			m_FilePath;
		static CFixedString 			m_FileExt;

	public:

		static void						SetFilePath( CFixedString & rFilePath ) { m_FilePath.Assign( rFilePath ); }
		static const CFixedString *		GetFilePath( void ) { return &m_FilePath; }

		static void						SetFileExt( CFixedString & rFileExt ) { m_FileExt.Assign( rFileExt ); }
		static const CFixedString *		GetFileExt( void ) {  return &m_FileExt; }
};


//=============================================================================
// End of Class : CHeightMap
//=============================================================================

#endif //__C_HEIGHT_MAP_H__

//*****************************************************************************
//
// End of File : CHeightMap.h
//
//*****************************************************************************