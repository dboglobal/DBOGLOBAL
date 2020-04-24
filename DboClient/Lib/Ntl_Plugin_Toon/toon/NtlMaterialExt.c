/*****************************************************************************
 *
 * File			: toonmultitex.c
 * Author		: HongHoDong
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 12	
 * Abstract		: NTl Application
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#include <d3d9.h>
#include <string.h>

#include "rpplugin.h"
#include "rpdbgerr.h"
#include "rwcore.h"
#include "rpworld.h"
#include "NtlMaterialExt.h"


RwChar g_ToonErrStr[2][64];


/**
 * Version 0: RwTexture  *texture
 */
#define NTL_MATERIAL_EXT_VER	0


#define GetNtlMaterialExt(material, offset)   \
    ((RpNtlMaterialExt *)((RwUInt8 *)material + offset))

#define GetConstNtlMaterialExt(material, offset)   \
    ((const RpNtlMaterialExt *)((const RwUInt8 *)material + offset))

typedef struct rpNtlMaterialExt RpNtlMaterialExt;
struct rpNtlMaterialExt
{
	RwUInt16		uiVersion;			// Save Version
	RwTexture*		pTexture;			// multiTexture
	RwTexture*		pUVTexture;			// u,v Texture
	RwMatrix*		pUVMatrix;			// TextureMatrix
	RwTexture*		pEnvMapTexture;		// Sphere Map
	RwTexture*		pEmblemTex;				
	RwRGBA			addColor;			// AddColor
	RwUInt32		uiFlag;				// Material Flag
	RwRGBA			SkinColor;
	RpMeshRenderCB	fpCB;
	RwRGBA			DogiColor;			// dogi edited color
};

static RwInt32 LocalOffset = -1;


static void *NtlMaterialExtConstructor( void *object,
										RwInt32  offset,
										RwInt32  size   __RWUNUSED__)
{
	RpNtlMaterialExt *material = GetNtlMaterialExt(object, offset);

    RWFUNCTION(RWSTRING("NtlMaterialExtConstructor"));
    RWASSERT(object);

	material->pTexture			= NULL;
	material->pUVTexture		= NULL;
	material->pUVMatrix			= NULL;
	material->pEnvMapTexture	= NULL;
	material->pEmblemTex		= NULL;
	material->uiFlag			= rpNTL_MATERIAL_INVALID;
	material->uiVersion			= NTL_MATERIAL_EXT_VER;
	material->addColor.red		= 0;
	material->addColor.green	= 0;
	material->addColor.blue		= 0;
	material->addColor.alpha	= 0;
	material->fpCB				= 0;
	material->SkinColor.red		= 0;
	material->SkinColor.green	= 0;
	material->SkinColor.blue	= 0;
	material->SkinColor.alpha	= 0;
	material->DogiColor.red		= 0;
	material->DogiColor.green	= 0;
	material->DogiColor.blue	= 255;
	material->DogiColor.alpha	= 255;

	RWRETURN(object);
}

static void *NtlMaterialExtDestructor( void *object,
									   RwInt32  offset,
									   RwInt32  size __RWUNUSED__)
{
	RwUInt32 i = 0;
    RpNtlMaterialExt *materialExt = GetNtlMaterialExt(object, offset);

	RWFUNCTION(RWSTRING("NtlMaterialExtDestructor"));
	RWASSERT(object);

	if(materialExt->pTexture)
		RwTextureDestroy(materialExt->pTexture);

	if(materialExt->pUVTexture)
		RwTextureDestroy(materialExt->pUVTexture);
	
	if(materialExt->pEnvMapTexture)
		RwTextureDestroy(materialExt->pEnvMapTexture);

	RWRETURN(object);
}

static void *NtlMaterialExtCopy( void *destObject,
								 const void *srcObject,
								 RwInt32 offset,
								 RwInt32 size __RWUNUSED__)
{
    /*
	RwUInt32 i = 0;
	RpNtlMaterialExt *destMaterialExt;
	const RpNtlMaterialExt *srcMaterialExt;
	*/

	RWFUNCTION(RWSTRING("NtlMaterialExtCopy"));
	RWASSERT(destObject);
	RWASSERT(srcObject);

	/*
	destMaterialExt = GetNtlMaterialExt(destObject, LocalOffset);
	srcMaterialExt  = GetConstNtlMaterialExt(srcObject, LocalOffset);
	
	if(srcMaterialExt->pTexture && destMaterialExt->pTexture)
	{
        RwTextureDestroy(destMaterialExt->pTexture);
		destMaterialExt->pTexture = srcMaterialExt->pTexture;
        
		RwTextureAddRef( destMaterialExt->pTexture );
	}
	
	if(srcMaterialExt->pUVTexture && destMaterialExt->pUVTexture)
	{
        RwTextureDestroy(destMaterialExt->pUVTexture);
		destMaterialExt->pUVTexture = srcMaterialExt->pUVTexture;
        
		RwTextureAddRef( destMaterialExt->pUVTexture );
	}

	if(srcMaterialExt->pEnvMapTexture && destMaterialExt->pEnvMapTexture)
	{
        RwTextureDestroy(destMaterialExt->pEnvMapTexture);
		destMaterialExt->pEnvMapTexture = srcMaterialExt->pEnvMapTexture;
        
		RwTextureAddRef( destMaterialExt->pEnvMapTexture );
	}

	destMaterialExt->uiFlag = srcMaterialExt->uiFlag;
	destMaterialExt->addColor = srcMaterialExt->addColor;
	*/

	RWRETURN(destObject);
}

static RwStream *NtlMaterialExtStreamRead( RwStream *stream,
										   RwInt32 length __RWUNUSED__,
										   void *object,
										   RwInt32 offset,
										   RwInt32 sizeInObj __RWUNUSED__)
{
	RpNtlMaterialExt *materialExt = GetNtlMaterialExt(object, LocalOffset);

	if (RwStreamRead(stream, (void *)(&(materialExt->uiVersion)), sizeof(materialExt->uiVersion))
		!= sizeof(materialExt->uiVersion))
	{
		return NULL;
	}
	
	if(materialExt->uiVersion == NTL_MATERIAL_EXT_VER)
	{
		if( RwStreamFindChunk(stream, rwID_TEXTURE, NULL, NULL) )
        {
            materialExt->pTexture = RwTextureStreamRead(stream);
        } 
	}
	return stream;
}

static RwStream *NtlMaterialExtStreamWrite( RwStream *stream,
										     RwInt32 length __RWUNUSED__,
										     const void *object,
										     RwInt32 offset,
										     RwInt32 sizeInObj __RWUNUSED__)
{
	RpNtlMaterialExt *materialExt = GetNtlMaterialExt(object, LocalOffset);
	
	if(materialExt->pTexture)
	{
		if (!RwStreamWrite(stream, (void *)(&(materialExt->uiVersion)), sizeof(materialExt->uiVersion)))
			return NULL;
		
		if( !RwTextureStreamWrite(materialExt->pTexture, stream) )
			return NULL;
	}
	
	return stream;
}

static RwInt32 NtlMaterialExtStreamGetSize( const void *object,
										  RwInt32 offset,
										  RwInt32 sizeInObj __RWUNUSED__)
{
	RwInt32 nSize = 0;
	RpNtlMaterialExt *materialExt;
	materialExt = GetNtlMaterialExt(object, LocalOffset);

	if(materialExt->uiVersion == NTL_MATERIAL_EXT_VER)
	{
		if(materialExt->pTexture)
		{
			nSize += sizeof(materialExt->uiVersion);
			nSize += sizeof(RwTexture);
		}
	}

	return nSize;
}

RwInt32 RpNtlMatFxPluginAttach(void)
{
	RwInt32 offset;
	LocalOffset = RpMaterialRegisterPlugin( sizeof(RpNtlMaterialExt),
                                            MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_NTL_MATERIAL_EXT),
                                            NtlMaterialExtConstructor,
                                            NtlMaterialExtDestructor,
                                            NtlMaterialExtCopy);

    if( LocalOffset < 0 )
    {
        return FALSE;
    }
	
	offset = -1;
    offset = RpMaterialRegisterPluginStream( MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_NTL_MATERIAL_EXT),
                                             NtlMaterialExtStreamRead,
                                             NtlMaterialExtStreamWrite,
                                             NtlMaterialExtStreamGetSize);

    if( offset != LocalOffset )
    {
        return FALSE;
    }
	return TRUE;
}

RwBool RpNtlMaterialExtSetEmblemTex(RpMaterial *pMaterial, RwTexture *pTexture)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetEmblemTex"));
	RWASSERT(pMaterial != NULL);

	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	RWASSERT(materialExt);

	if(pTexture)
	{
		if(materialExt->pEmblemTex != NULL)
		{
			RwTextureDestroy(materialExt->pEmblemTex);
			materialExt->pEmblemTex = RwTextureAddRef(pTexture);
		}
		else
		{
			materialExt->pEmblemTex = RwTextureAddRef(pTexture);
		}
		RWRETURN(TRUE);
	}
	else
	{
		if(materialExt->pEmblemTex)
		{
			RwTextureDestroy(materialExt->pEmblemTex);
			materialExt->pEmblemTex = NULL;
			RWRETURN(TRUE);
		}
		else
		{
			RWRETURN(FALSE);
		}
	}    

	RWRETURN(FALSE);
}

RwTexture* RpNtlMaterialExtGetEmblemTex(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetEmblemTex"));
	RWASSERT(pMaterial != NULL);

	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->pEmblemTex);
}

RwBool RpNtlMaterialExtSetTexture(RpMaterial *pMaterial, RwTexture *pTexture)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetTexture"));
    //RWASSERT( pTexture != NULL);
	RWASSERT( pMaterial != NULL);
	
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	RWASSERT( materialExt );

	
	if (pTexture)
    {
		if(materialExt->pTexture != NULL)
		{
			RwTextureDestroy(materialExt->pTexture);
			materialExt->pTexture = RwTextureAddRef(pTexture);
		}
		else
		{
			materialExt->pTexture = RwTextureAddRef(pTexture);
		}
        RWRETURN(TRUE);
    }
	else
	{
        if(materialExt->pTexture)
        {
		    RwTextureDestroy(materialExt->pTexture);
		    materialExt->pTexture = NULL;
            RWRETURN(TRUE);
        }
        else
        {
            RWRETURN(FALSE);
        }
    }    
		
	RWRETURN(FALSE);
}

RwTexture* RpNtlMaterialExtGetTexture(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetTexture"));
	RWASSERT( pMaterial != NULL);
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->pTexture);
}

void RpNtlMaterialSetRenderCB(RpMaterial *pMaterial, RpMeshRenderCB _fpCB)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExecuteRenderCB"));
	RWASSERT( pMaterial != NULL);
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	materialExt->fpCB = _fpCB;

	RWRETURNVOID();	
}

void RpNtlMaterialExecuteRenderCB(void* _pParam)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExecuteRenderCB"));
	RWASSERT(_pParam != NULL);
	materialExt = GetNtlMaterialExt(((sMatExtInfo*)_pParam)->pMaterial, LocalOffset);

	if(materialExt->fpCB)
	{
		(materialExt->fpCB)(_pParam);
	}

	RWRETURNVOID();	
}

RwBool RpNtlMaterialExtSetUVTexture(RpMaterial *pMaterial, RwTexture *pTexture)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetUVTexture"));
    //RWASSERT( pTexture != NULL);
	RWASSERT( pMaterial != NULL);
	
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	RWASSERT( materialExt );
	
	if (pTexture)
    {
		if(materialExt->pUVTexture != NULL)
		{
			RwTextureDestroy(materialExt->pUVTexture);
			materialExt->pUVTexture = RwTextureAddRef(pTexture);
		}
		else
		{
			materialExt->pUVTexture = RwTextureAddRef(pTexture);
		}

		if(materialExt->pUVMatrix != NULL)
			materialExt->uiFlag = materialExt->uiFlag | rpNTL_MATERIAL_UV_MATRIX;

        RWRETURN(TRUE);
    }
	else
	{
        if(materialExt->pUVTexture)
        {
		    RwTextureDestroy(materialExt->pUVTexture);
		    materialExt->pUVTexture = NULL;
			materialExt->uiFlag = materialExt->uiFlag & !rpNTL_MATERIAL_UV_MATRIX;

			RWRETURN(TRUE);
        }
        else
        {
            RWRETURN(FALSE);
        }
    }    
		
	RWRETURN(FALSE);
}

RwTexture *RpNtlMaterialExtGetUVTexture(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetUVTexture"));
	RWASSERT( pMaterial != NULL);
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->pUVTexture);	
}

void RpNtlMaterialExtSetUVTransform(RpMaterial *pMaterial, RwMatrix *pMatrix)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtUVTransform"));
	RWASSERT( pMaterial != NULL);
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	materialExt->pUVMatrix = pMatrix;

	if(materialExt->pUVTexture)
		materialExt->uiFlag = materialExt->uiFlag | rpNTL_MATERIAL_UV_MATRIX;
	
	RWRETURNVOID();	
}

RwMatrix *RpNtlMaterialExtGetUVTransform(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetUVTransform"));
	RWASSERT( pMaterial != NULL);
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	
	RWRETURN(materialExt->pUVMatrix);	
}

void RpNtlMaterialExtApplyTextureMatrix(RwUInt32 stage, RwMatrix *matrix)
{
    static RwMatrix texMat =
    {
        {0.0f, 0.0f, 0.0f}, 0,
        {0.0f, 0.0f, 0.0f}, 0,
        {0.0f, 0.0f, 1.0f}, 0,
        {0.0f, 0.0f, 0.0f}, 0
    };

    RWFUNCTION(RWSTRING("RpNtlMaterialExtApplyTextureMatrix"));

    if ( matrix != NULL &&
         FALSE == rwMatrixTestFlags(matrix, rwMATRIXINTERNALIDENTITY) )
    {
        RwD3D9SetTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

        texMat.right.x = matrix->right.x;
        texMat.right.y = matrix->right.y;

        texMat.up.x = matrix->up.x;
        texMat.up.y = matrix->up.y;

        texMat.at.x = matrix->pos.x;
        texMat.at.y = matrix->pos.y;

        RwD3D9SetTransform(D3DTS_TEXTURE0 + stage, &texMat);
    }

    RWRETURNVOID();
}

RwBool RpNtlMaterialExtSetEnvMapTexture(RpMaterial *pMaterial, RwTexture *pTexture)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetEnvMapTexture"));
	RWASSERT( pMaterial != NULL);
	
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	RWASSERT( materialExt );
	
	if(pTexture)
    {
		if(materialExt->pEnvMapTexture != NULL)
		{
			RwTextureDestroy(materialExt->pEnvMapTexture);
			materialExt->pEnvMapTexture = RwTextureAddRef(pTexture);
		}
		else
		{
			materialExt->pEnvMapTexture = RwTextureAddRef(pTexture);
		}

		if(materialExt->pEnvMapTexture != NULL)
			materialExt->uiFlag = materialExt->uiFlag | rpNTL_MATERIAL_ENV_MAP;

        RWRETURN(TRUE);
    }
	else
	{
        if(materialExt->pEnvMapTexture)
        {
		    RwTextureDestroy(materialExt->pEnvMapTexture);
		    materialExt->pEnvMapTexture = NULL;
			materialExt->uiFlag = materialExt->uiFlag & (!rpNTL_MATERIAL_ENV_MAP);

			RWRETURN(TRUE);
        }
        else
        {
            RWRETURN(FALSE);
        }
    }    
		
}

RwTexture *RpNtlMaterialExtGetEnvMapTexture(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetEnvMapTexture"));
	RWASSERT( pMaterial != NULL);
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->pEnvMapTexture);	
}

RwRGBA *RpNtlMaterialExtGetSkinColor(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetSkinColor"));

	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(&materialExt->SkinColor);
}

void RpNtlMaterialExtSetSkinColor(RpMaterial *pMaterial, RwRGBA *pColor)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetSkinColor"));

	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	materialExt->SkinColor.red		= pColor->red;
	materialExt->SkinColor.green	= pColor->green;
	materialExt->SkinColor.blue		= pColor->blue;
	materialExt->SkinColor.alpha	= pColor->alpha;

	RWRETURNVOID();
}

RwRGBA *RpNtlMaterialExtGetDogiColor(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;

	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	return (&materialExt->DogiColor);
}

void RpNtlMaterialExtSetDogiColor(RpMaterial *pMaterial, RwRGBA *pColor)
{
	RpNtlMaterialExt *materialExt = NULL;

	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	materialExt->DogiColor.red		= pColor->red;
	materialExt->DogiColor.green	= pColor->green;
	materialExt->DogiColor.blue		= pColor->blue;
	materialExt->DogiColor.alpha	= pColor->alpha;

	return;
}

void RpNtlMaterialExtSetAddColor(RpMaterial *pMaterial, const RwRGBA *pColor)
{
	RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetAddColor"));
	
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	materialExt->addColor.red	= pColor->red;
	materialExt->addColor.green = pColor->green;
	materialExt->addColor.blue	= pColor->blue;
	materialExt->addColor.alpha = 255;

	if( materialExt->addColor.red == 0 &&
		materialExt->addColor.green == 0 &&
		materialExt->addColor.blue == 0)
		materialExt->uiFlag = (materialExt->uiFlag) &(~rpNTL_MATERIAL_ADD_COLOR);
	else
		materialExt->uiFlag = materialExt->uiFlag | rpNTL_MATERIAL_ADD_COLOR;
	RWRETURNVOID();
}

const RwRGBA *RpNtlMaterialExtGetAddColor(const RpMaterial *pMaterial)
{
	const RpNtlMaterialExt *materialExt = NULL;
	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetAddColor"));
	
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	
	RWRETURN(&materialExt->addColor);
}

RwUInt32 RpNtlMaterialExtGetFlag(const RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;

	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtGetRenderFlag"));
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->uiFlag);
}

void RpNtlMaterialExtSetFlag(RpMaterial *pMaterial, RwUInt32 uiFlag)
{
	RpNtlMaterialExt *materialExt = NULL;

	RWAPIFUNCTION(RWSTRING("RpToonMaterialExtSetRenderFlag"));
	
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	materialExt->uiFlag = uiFlag;
}

/*
void RpNtlMaterialExtSetCurUpgrade(RpMaterial *pMaterial, BYTE* _CurGrade)
{
	RpNtlMaterialExt *materialExt = NULL;

	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetUpgradeEffect"));
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);
	
	materialExt->_CurItemGrade	= *_CurGrade;
}

void RpNtlMaterialExtSetCurUVAniTime(RpMaterial *pMaterial, RwReal* _CurUVAniTime)
{
	RpNtlMaterialExt *materialExt = NULL;

	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetUpgradeEffect"));
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	materialExt->_CurUVAniTime	= *_CurUVAniTime;
}

BYTE RpNtlMaterialExtGetCurUpgrade(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;

	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetUpgradeEffect"));
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->_CurItemGrade);
}

RwReal RpNtlMaterialExtGetCurUVAniTime(RpMaterial *pMaterial)
{
	RpNtlMaterialExt *materialExt = NULL;

	RWAPIFUNCTION(RWSTRING("RpNtlMaterialExtSetUpgradeEffect"));
	materialExt = GetNtlMaterialExt(pMaterial, LocalOffset);

	RWRETURN(materialExt->_CurUVAniTime);
}*/