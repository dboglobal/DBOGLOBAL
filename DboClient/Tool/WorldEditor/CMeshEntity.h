//*****************************************************************************
// File       : CMeshEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_MESH_ENTITY_H__
#define __C_MESH_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpMesh;

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntity.h"

#include "CMeshEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CMeshEntity
// Desc       : 
// Author     : 
//=============================================================================

class CMeshEntity : public CEntity
{
	protected:

	public:

										CMeshEntity( void );
										CMeshEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CMeshEntity( const CMeshEntity & rEntity );

		virtual							~CMeshEntity( void ) { Destroy(); }

		const CMeshEntity &				operator =( const CMeshEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		inline const CFixedString *		GetPropertyName( void ) const { return GetMeshProperty()->GetName(); }

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

	protected:

		virtual void					Initialize( void );

		inline CMeshEntityProperty *	GetMeshProperty( void ) const { return reinterpret_cast<CMeshEntityProperty *>(GetProperty()); }
		inline RpMesh *					GetRpMesh( void ) const { return reinterpret_cast<RpMesh *>(GetRwInstance()); }

		virtual RwBool					SetParameter( void );

		virtual RwBool					SetPlugInData( void );
		virtual RwBool					ResetPlugInData( void );

	protected:

		static RwUInt32					m_iRwInstanceExPlugInOffset;

	public:

		static RwBool					RwInstanceExPlugInAttach( void );

		static void *					RwInstanceExPlugInConstructor( void * pObject, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );
		static void *					RwInstanceExPlugInCopier( void * pDstRpMesh, const void * pSrcRpMesh, RwInt32 iOffsetInObject, RwInt32 iSizeInObject );

		inline static SRwInstanceEx *	GetPlugInDataFromRpMesh( RpMesh * pMesh )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pMesh);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static const SRwInstanceEx *	GetPlugInDataFromRpMesh( const RpMesh * pMesh )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pMesh);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			return reinterpret_cast<SRwInstanceEx *>(iPlugInAddress);
		}

		inline static RpMesh * SetPlugInDataToRpMesh( RpMesh * pMesh, const SRwInstanceEx * pPlugInData )
		{
			const RwUInt32	iBaseAddress	= reinterpret_cast<RwUInt32>(pMesh);
			const RwUInt32	iPlugInAddress	= iBaseAddress + m_iRwInstanceExPlugInOffset;

			reinterpret_cast<SRwInstanceEx *>(iPlugInAddress)->SetEntity( pPlugInData->GetEntity() );

			return pMesh;
		}

	public:

		static RwInt32					CreateInternalMeshFromMeshHeader( CEntity * pParent, RpMeshHeader * pRpMeshHeader );

};


//=============================================================================
// End of Class : CMeshEntity
//=============================================================================

#endif //__C_MESH_ENTITY_H__

//*****************************************************************************
//
// End of File : CMeshEntity.h
//
//*****************************************************************************