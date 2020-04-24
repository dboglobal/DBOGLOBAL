#include "StdAfx.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NormEnvMap.h"

/*
***************************************************************************
*/
static RpAtomic *
AtomicNormalMapTextureSpaceEnable(RpAtomic *atomic, void *data)
{
    RpNormMapAtomicPipeline pipe;
    BOOL bAnim = (BOOL)data;
    if(bAnim)
    {
        pipe = rpNORMMAPATOMICSKINNEDPIPELINE;
    }
    else
    {
        pipe = rpNORMMAPATOMICSTATICPIPELINE;
    }

    RpNormMapAtomicInitialize(atomic, pipe);

    return (atomic);
}

/*
***************************************************************************
*/

/*
***************************************************************************
*/
static RpAtomic *
AtomicSetPipeline(RpAtomic *atomic, void *data)
{
    RpAtomicSetPipeline(atomic, (RxPipeline *)data);

    return (atomic);
}

/*
***************************************************************************
*/

CNormEnvMap::CNormEnvMap(void)
{
}

CNormEnvMap::~CNormEnvMap(void)
{
}

RwTexture* CNormEnvMap::NormalMapTextureSpaceCreateFromTexture( RwTexture *texture, RwReal bumpness )
{

    RwRaster *raster;
    RwTexture *normalmap = NULL;

    raster = RwTextureGetRaster(texture);
    if (raster != NULL)
    {
        RwUInt32 width, height;
        RwImage *image;
        RwBool clamp;
        RwUInt32 rasterFlags;
        RwRaster *rasternormalmap;

        width = RwRasterGetWidth(raster);
        height = RwRasterGetHeight(raster);

        image = RwImageCreate(width, height, 32);
        RwImageAllocatePixels(image);
        RwImageSetFromRaster(image, raster);

        clamp = (RwTextureGetAddressingU(texture) == rwTEXTUREADDRESSCLAMP ||
            RwTextureGetAddressingV(texture) == rwTEXTUREADDRESSCLAMP);

        rasterFlags = (RwRasterGetType(raster) |
#ifdef FAST_MODE
            (RwRasterGetFormat(raster) & (rwRASTERFORMATMIPMAP | rwRASTERFORMATAUTOMIPMAP)) |
#else
            (RwRasterGetFormat(raster) & rwRASTERFORMATMIPMAP) |
#endif
            rwRASTERFORMAT888);

        rasternormalmap = NormalMapTextureSpaceCreateFromImage(image, rasterFlags, clamp, bumpness);

        /* Create texture */
        normalmap = RwTextureCreate(rasternormalmap);

        RwTextureSetFilterMode(normalmap, RwTextureGetFilterMode(texture));
        RwTextureSetAddressingU(normalmap, RwTextureGetAddressingU(texture));
        RwTextureSetAddressingV(normalmap, RwTextureGetAddressingV(texture));

        RwImageDestroy(image);
    }

    return normalmap;

}

RwRaster* CNormEnvMap::NormalMapTextureSpaceCreateFromImage( RwImage *image, RwUInt32 rasterFlags, RwBool clamp, RwReal bumpness ) 
{

    RwInt32 width, height;
    RwRaster *rasternormalmap;

    width = RwImageGetWidth(image);
    height = RwImageGetHeight(image);

    rasternormalmap = RwRasterCreate(width, height, 32, rasterFlags);

    if (rasternormalmap != NULL)
    {
        RwImage *imagenormalmap = NULL;

#ifdef FAST_MODE
        imagenormalmap = RtNormMapCreateFromImage(image, clamp, bumpness);

        if (imagenormalmap != NULL)
        {
            RwRasterSetFromImage(rasternormalmap, imagenormalmap);

            RwImageDestroy(imagenormalmap);
        }
#else
        RwUInt32 n, nummipmaps;

        width = RwRasterGetWidth(rasternormalmap);
        height = RwRasterGetHeight(rasternormalmap);
        nummipmaps = RwRasterGetNumLevels(rasternormalmap);

        for (n = 0; n < nummipmaps; n++)
        {
            RwRasterLock(rasternormalmap, n, rwRASTERLOCKWRITE | rwRASTERLOCKNOFETCH);

            if (RwImageGetWidth(image) != width ||
                RwImageGetHeight(image) != height)
            {
                RwImage *imagemipmap;

                imagemipmap = RwImageCreateResample(image, width, height);
                if (imagemipmap != NULL)
                {
                    imagenormalmap = RtNormMapCreateFromImage(imagemipmap, clamp, bumpness);

                    RwImageDestroy(imagemipmap);
                }
            }
            else
            {
                imagenormalmap = RtNormMapCreateFromImage(image, clamp, bumpness);
            }

            if (imagenormalmap != NULL)
            {
                RwRasterSetFromImage(rasternormalmap, imagenormalmap);

                RwImageDestroy(imagenormalmap);

                imagenormalmap = NULL;
            }

            RwRasterUnlock(rasternormalmap);

            width /= 2;
            if (width == 0)
            {
                width = 1;
            }

            height /= 2;
            if (height == 0)
            {
                height = 1;
            }

            bumpness *= 0.5f;
        }
#endif
    }

    return rasternormalmap;

}

void CNormEnvMap::SetNormMap( RpMaterial* pMaterial, RwBool bEnable ) 
{
    if(!pMaterial)
        return;

    if(!bEnable)
    {
        RpNormMapMaterialSetNormMapTexture(pMaterial, NULL);
        return;
    }

    RwTexture* pTextureNorm = NormalMapTextureSpaceCreateFromTexture(pMaterial->texture, FALSE);
    if(pTextureNorm)
    {
        RwTextureSetFilterMode(pTextureNorm, rwFILTERLINEARMIPLINEAR);

        RpNormMapMaterialSetNormMapTexture(pMaterial, pTextureNorm);

        RwTextureDestroy(pTextureNorm);
    }    
}

void CNormEnvMap::SetEnvMap( RpMaterial* pMaterial, RwTexture* pTexture ) 
{
    if(!pMaterial)
        return;

    if(pTexture == NULL)
    {
        RpNormMapMaterialSetEnvMapTexture(pMaterial, NULL);        
        return;
    }

    RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
    RwTextureSetAddressing(pTexture, rwTEXTUREADDRESSCLAMP);
    RpNormMapMaterialSetEnvMapTexture(pMaterial, pTexture);
    RpNormMapMaterialSetEnvMapCoefficient(pMaterial, 1.0f);
    RpNormMapMaterialModulateEnvMap(pMaterial, FALSE);
    RpNormMapMaterialSetEnvMapFrame(pMaterial, NULL);

    //RpMatFXMaterialSetEffects(pMaterial, rpMATFXEFFECTENVMAP);
    //RpMatFXMaterialSetupEnvMap(pMaterial, pTexture, NULL, FALSE, 1.0f);


}

void CNormEnvMap::SetEnableNormMap( RpClump* pClump, RwBool bEnable ) 
{
    if(!pClump)
        return;

    BOOL bAnim = IsAnim(pClump);

    if(bEnable)
    {
        RpClumpForAllAtomics(pClump, AtomicNormalMapTextureSpaceEnable, (void*)bAnim);        
    }
    else
    {
        if(bAnim)
        {
            RpClumpForAllAtomics(pClump, AtomicSetPipeline, (void *)RpSkinGetD3D9Pipeline(rpSKIND3D9PIPELINEGENERIC));
        }
        else
        {
            RpClumpForAllAtomics(pClump, AtomicSetPipeline, (void *)RpAtomicGetDefaultPipeline());
        }
    }
}

RwBool CNormEnvMap::IsAnim( RpClump* pClump ) 
{
    if(!pClump)
        return FALSE;

    RpHAnimHierarchy* pBaseHierarchy = Helper_GetHierarchyClump(pClump);
    if(!pBaseHierarchy)
        return FALSE;

    if(RpHAnimHierarchyGetFlags(pBaseHierarchy) & rpHANIMHIERARCHYUPDATELTMS)
        return TRUE;

    return FALSE;
}

void CNormEnvMap::SetEnvMapShininess( RpMaterial* pMaterial, RwReal fShininess ) 
{
    RpNormMapMaterialSetEnvMapCoefficient(pMaterial, fShininess);
}