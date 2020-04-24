//*****************************************************************************
// File       : Collision.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __COLLISION_H__
#define __COLLISION_H__

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

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum ECollisionDetectionLevel
{
	E_COLLISION_DETECTION_LEVEL_NONE,
	E_COLLISION_DETECTION_LEVEL_BOUNDING,
	E_COLLISION_DETECTION_LEVEL_MESH,
	E_COLLISION_DETECTION_LEVEL_COUNT
} ECollisionDetectionLevel;


//=============================================================================
// Name       : CCollisionDetectionConditon
// Desc       : 
// Author     : 
//=============================================================================

class CCollisionDetectionConditon
{
	protected:

		RwBool							m_bUseScreenPos;

		RwV2d							m_sScreenPos;
		RpIntersection					m_sIntersection;

		ECollisionDetectionLevel		m_eTerrainDetectionLevel;
		ECollisionDetectionLevel		m_eObjectDetectionLevel;
		ECollisionDetectionLevel		m_eHelperDetectionLevel;

	public:

										CCollisionDetectionConditon( void ) { ZeroMemory( this, sizeof( CCollisionDetectionConditon ) ); }
										CCollisionDetectionConditon( const CCollisionDetectionConditon & rCollisionDetectionConditon );

										~CCollisionDetectionConditon( void ) {}

		const CCollisionDetectionConditon &	operator =( const CCollisionDetectionConditon & rCollisionDetectionConditon );

	public:

		inline RwBool					GetUseScreenPos( void ) const { return m_bUseScreenPos; }

		ECollisionDetectionLevel		GetTerrainDetectionLevel( void ) const { return m_eTerrainDetectionLevel; }
		ECollisionDetectionLevel		GetObjectDetectionLevel( void ) const { return m_eObjectDetectionLevel; }
		ECollisionDetectionLevel		GetHelperDetectionLevel( void ) const { return m_eHelperDetectionLevel; }

		inline const RpIntersection *	GetIntersection( void ) const { return &m_sIntersection; }
		inline const RwV2d *			GetScreenPos( void ) const { return const_cast<RwV2d *>(&m_sScreenPos); }

		void							SetTerrainDetectionLevel( ECollisionDetectionLevel eNewDetectionLevel ) { m_eTerrainDetectionLevel = eNewDetectionLevel; }
		void							SetObjectDetectionLevel( ECollisionDetectionLevel eNewDetectionLevel ) { m_eObjectDetectionLevel = eNewDetectionLevel; }
		void							SetHelperDetectionLevel( ECollisionDetectionLevel eNewDetectionLevel ) { m_eHelperDetectionLevel = eNewDetectionLevel; }

		inline void						SetUseScreenPos( RwBool bNewUseScreenPos ) { m_bUseScreenPos = bNewUseScreenPos; }
		inline void						SetScreenPos( const RwV2d * pNewScreenPos ) { memcpy( &m_sScreenPos, pNewScreenPos, sizeof( RwV2d ) ); }
		inline void						SetIntersection( const RpIntersection * pNewIntersection ) { memcpy( &m_sIntersection, pNewIntersection, sizeof( RpIntersection ) ); }
		inline void						SetIntersection( const RpIntersectType eIntersectionType, void * pNewIntersectionData )
		{
			m_sIntersection.type		= eIntersectionType;

			switch( eIntersectionType ) 
			{
				case rpINTERSECTLINE:
				{
					memcpy( &m_sIntersection.t.line, pNewIntersectionData, sizeof( RwLine ) );

					break;
				}

				case rpINTERSECTPOINT:
				{
					memcpy( &m_sIntersection.t.point, pNewIntersectionData, sizeof( RwV3d ) );

					break;
				}

				case rpINTERSECTSPHERE:
				{
					memcpy( &m_sIntersection.t.sphere, pNewIntersectionData, sizeof( RwSphere ) );

					break;
				}

				case rpINTERSECTBOX:
				{
					memcpy( &m_sIntersection.t.box, pNewIntersectionData, sizeof( RwBBox ) );

					break;
				}

				case rpINTERSECTATOMIC:
				{
					m_sIntersection.t.object = pNewIntersectionData;

					break;
				}

				default:
				{
					memcpy( &m_sIntersection.t, pNewIntersectionData, sizeof( RpIntersectData ) );
				}
			}
		}

};

//=============================================================================
// End of Class : CCollisionDetectionConditon
//=============================================================================


//=============================================================================
// Name       : CCollisionDetectionInfo
// Desc       : 
// Author     : 
//=============================================================================

class CCollisionDetectionInfo
{
	protected:

		CCollisionDetectionConditon	*	m_pCondition;
		CEntityCollisionList *			m_pResultList;
		RpAtomic *						m_pRpAtomic;

	public:

										CCollisionDetectionInfo( void ) { Initialize(); }
										~CCollisionDetectionInfo( void ) { Destroy(); }

	public:

		RwBool							CreateByPickTerrain( const RwV2d * pScreenPos );
		RwBool							CreateByPickObject( const RwV2d * pScreenPos );
		RwBool							CreateByDirectionalLight( const RwLine * pLightLine, RwBool bForward );
		RwBool							CreateByGetHeight( const RwV3d * pPosition );

		inline CCollisionDetectionConditon *	GetCondition( void ) const { return m_pCondition; }
		inline CEntityCollisionList *	GetResultList( void ) const { return m_pResultList; }

		inline void						SetCondition( CCollisionDetectionConditon * pNewCondition ) { m_pCondition = pNewCondition; }
		inline void						SetResultList( CEntityCollisionList * pNewResultList ) { m_pResultList = pNewResultList; }

		inline RpAtomic *				GetRpAtomic( void ) const { return m_pRpAtomic; }
		inline void						SetRpAtomic( RpAtomic * pAtomic ) { m_pRpAtomic = pAtomic; }

	protected:

		void							Initialize( void );
		void							Destroy( void );

};


//=============================================================================
// End of Class : CCollisionDetectionInfo
//=============================================================================

RpAtomic * DetectAtomicInWorldCB( RpIntersection * pRpIntersection, RpWorldSector * pRpSector, RpAtomic * pRpAtomic, RwReal fDistance, void * pData );
RpCollisionTriangle * DetectPositionInAtomicCB( RpIntersection * pIntersection, RpCollisionTriangle * pCollisionTriangle, RwReal fDistance, void * pData );
void GetLocationFromIntersection( RwV3d * pLocation, RpIntersection * pIntersection );


#endif //__COLLISION_H__

//*****************************************************************************
//
// End of File : Collision.h
//
//*****************************************************************************