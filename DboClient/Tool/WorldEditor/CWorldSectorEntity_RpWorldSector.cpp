//*****************************************************************************
// File       : CWorldSectorEntity_RpWorldSector.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CAtomicEntity.h"
#include "CSceneManager.h"

#include "CWorldSectorEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: RpWorldSectorExPlugInAttach
// Desc			: 
// Privilege	: 
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

// ¿ìµð
RwBool CWorldSectorEntity::RenderWorldSectorAtomic( CEntity * pRenderer )
{
	RpWorldSector *		pRpWorldSector = GetRpWorldSector();
	RwCamera *			pRwCamera = reinterpret_cast<RwCamera *>(pRenderer->GetRwInstance());
	RwLLLink *			pNodeCurrent;
	RwLLLink *			pNodeEnd;
	RpTie *				pRpAtomicTie;
	RpAtomic *			pRpAtomic;
	const RwSphere *	pBoundingSphere;
	SRwInstanceEx *		pRwInstanceEx;
	CEntity *			pAtomicEntity;

	pNodeCurrent = rwLinkListGetFirstLLLink( &pRpWorldSector->collAtomicsInWorldSector );
	pNodeEnd = rwLinkListGetTerminator( &pRpWorldSector->collAtomicsInWorldSector );

	while( pNodeCurrent != pNodeEnd )
    {
		pRpAtomicTie = rwLLLinkGetData( pNodeCurrent, RpTie, lAtomicInWorldSector );
		pRpAtomic = pRpAtomicTie->apAtom;

		if( rwObjectTestFlags( pRpAtomic, rpATOMICRENDER ) )
		{
			if (pRpAtomic->renderFrame != RWSRCGLOBAL(renderFrame))
            {
				pBoundingSphere = RpAtomicGetWorldBoundingSphere( pRpAtomic );

				if( RwCameraFrustumTestSphere( pRwCamera, pBoundingSphere ) != rwSPHEREOUTSIDE )
				{
					pRwInstanceEx = CAtomicEntity::GetPlugInDataFromRpAtomic( pRpAtomic );

					pAtomicEntity = pRwInstanceEx->GetEntity();

					if( pAtomicEntity != NULL )
					{
						if( pAtomicEntity->RenderFrame( pRenderer ) == FALSE )
						{
							return FALSE;
						}
					}
				}

				pRpAtomic->renderFrame = RWSRCGLOBAL( renderFrame );
            }
		}

		pNodeCurrent = rwLLLinkGetNext( pNodeCurrent );
	}

	return TRUE;
}


//*****************************************************************************
//
// End of File : CWorldSectorEntity_RpWorldSector.cpp
//
//*****************************************************************************