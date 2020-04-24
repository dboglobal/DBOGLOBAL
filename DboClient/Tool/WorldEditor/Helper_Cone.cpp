//*****************************************************************************
// File       : Helper_Cone.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#include "StdAfx.h"

#include "Helper.h"


//-----------------------------------------------------------------------------
// Name			: CreateCone
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CreateCone( SImediateRenderInfo * pRenderInfo, RwV2d * pSize, RwUInt32 iDirection )
{
	// Local Apex

	switch( iDirection )
	{
		case 0:
		{
			RwIm3DVertexSetPos( &pRenderInfo->pVertices[0], pSize->y, 0.0f, 0.0f );
			RwIm3DVertexSetRGBA( &pRenderInfo->pVertices[0], 255, 0, 0, 255 );

			RwReal		fBaseRadius;
			RwReal		fBaseXPos;
			RwReal		fBaseZPos;

			for( RwUInt32	iBaseIndex = 1; iBaseIndex < pRenderInfo->iVertexCount; ++iBaseIndex )
			{
				fBaseRadius = 2.0f * rwPI * static_cast<RwReal>(iBaseIndex) / static_cast<RwReal>(pRenderInfo->iVertexCount - 2);
				fBaseXPos = cosf( fBaseRadius ) * pSize->x;
				fBaseZPos = sinf( fBaseRadius ) * pSize->x;

				RwIm3DVertexSetPos( &pRenderInfo->pVertices[iBaseIndex], 0.0f, fBaseXPos, fBaseZPos );
				RwIm3DVertexSetRGBA( &pRenderInfo->pVertices[iBaseIndex], 128, 0, 0, 128 );
			}

			break;
		}

		case 1:
		{
			RwIm3DVertexSetPos( &pRenderInfo->pVertices[0], 0.0f, pSize->y, 0.0f );
			RwIm3DVertexSetRGBA( &pRenderInfo->pVertices[0], 0, 255, 0, 255 );

			RwReal		fBaseRadius;
			RwReal		fBaseXPos;
			RwReal		fBaseZPos;

			for( RwUInt32	iBaseIndex = 1; iBaseIndex < pRenderInfo->iVertexCount; ++iBaseIndex )
			{
				fBaseRadius = 2.0f * rwPI * static_cast<RwReal>(iBaseIndex) / static_cast<RwReal>(pRenderInfo->iVertexCount - 2);
				fBaseXPos = cosf( fBaseRadius ) * pSize->x;
				fBaseZPos = -sinf( fBaseRadius ) * pSize->x;

				RwIm3DVertexSetPos( &pRenderInfo->pVertices[iBaseIndex], fBaseXPos, 0.0f, fBaseZPos );
				RwIm3DVertexSetRGBA( &pRenderInfo->pVertices[iBaseIndex], 0, 128, 0, 128 );
			}

			break;
		}

		case 2:
		{
			RwIm3DVertexSetPos( &pRenderInfo->pVertices[0], 0.0f, 0.0f, pSize->y );
			RwIm3DVertexSetRGBA( &pRenderInfo->pVertices[0], 0, 0, 255, 255 );

			RwReal		fBaseRadius;
			RwReal		fBaseXPos;
			RwReal		fBaseZPos;

			for( RwUInt32	iBaseIndex = 1; iBaseIndex < pRenderInfo->iVertexCount; ++iBaseIndex )
			{
				fBaseRadius = 2.0f * rwPI * static_cast<RwReal>(iBaseIndex) / static_cast<RwReal>(pRenderInfo->iVertexCount - 2);
				fBaseXPos = cosf( fBaseRadius ) * pSize->x;
				fBaseZPos = sinf( fBaseRadius ) * pSize->x;

				RwIm3DVertexSetPos( &pRenderInfo->pVertices[iBaseIndex], fBaseXPos, fBaseZPos, 0.0f );
				RwIm3DVertexSetRGBA( &pRenderInfo->pVertices[iBaseIndex], 0, 0, 128, 128 );
			}

			break;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RenderCone
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool RenderCone( SImediateRenderInfo * pRenderInfo, RwMatrix * pMatrix )
{
//	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
//	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
//	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	if( RwIm3DTransform( pRenderInfo->pVertices, pRenderInfo->iVertexCount, pMatrix, 0 ) )
	{
        RwIm3DRenderPrimitive( rwPRIMTYPETRIFAN );

        RwIm3DEnd();
    }

//	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

	return TRUE;
}


//*****************************************************************************
//
// End of File : Helper_Cone.cpp
//
//*****************************************************************************