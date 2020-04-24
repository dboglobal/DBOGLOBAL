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
#include <string.h>

#include "rpplugin.h"
#include "rpdbgerr.h"
#include "rwcore.h"
#include "rpworld.h"

#include "toonmultiTex.h"

#define rwID_TOON_TEST_EXAMPLE (0xfc)

#define GetToonMultiTexture(material, offset)   \
    ((RpToonMultiTexture *)((RwUInt8 *)material + offset))

#define GetConstToonMultiTexture(material, offset)   \
    ((const RpToonMultiTexture *)((const RwUInt8 *)material + offset))


typedef struct rpToonMultiTexture RpToonMultiTexture;
struct rpToonMultiTexture
{
	RwRGBAReal				matColor;	/**<Material Color */
	RwTexture              *texture;
};



static RwInt32 LocalOffset = -1;


static void *
ToonMultiTexConstructor(void    *object,
                        RwInt32  offset,
                        RwInt32  size   __RWUNUSED__)
{
	RpToonMultiTexture *multitexture = GetToonMultiTexture(object, offset);

    RWFUNCTION(RWSTRING("ToonMultiTexConstructor"));
    RWASSERT(object);
	multitexture->texture = NULL;
	
	multitexture->matColor.red   = 1.f;
	multitexture->matColor.green = 1.f;
	multitexture->matColor.blue  = 1.f;
	multitexture->matColor.alpha = 1.f;

	RWRETURN(object);
}

static void *
ToonMultiTexDestructor(void    *object,
                       RwInt32  offset,
                       RwInt32  size __RWUNUSED__)
{
	RwUInt32 i = 0;
    RpToonMultiTexture *multitexture = GetToonMultiTexture(object, offset);

	RWFUNCTION(RWSTRING("ToonMultiTexDestructor"));
	RWASSERT(object);

	if(multitexture->texture)
		RwTextureDestroy(multitexture->texture);
	
	RWRETURN(object);
}

static void *
ToonMultiTexCopier(void       *dstObject,
                   const void *srcObject,
                   RwInt32     offset,
                   RwInt32     size   __RWUNUSED__)
{
    
	RwUInt32 i = 0;
	RpToonMultiTexture *destMultiTexture;
	const RpToonMultiTexture *srcMultiTexture;
	
	RWFUNCTION(RWSTRING("ToonMultiTexCopier"));
	RWASSERT(dstObject);
	RWASSERT(srcObject);

	destMultiTexture = GetToonMultiTexture(dstObject, LocalOffset);
	srcMultiTexture  = GetConstToonMultiTexture(srcObject, LocalOffset);
	
    
	if(srcMultiTexture->texture)
	{
        RwTextureDestroy(destMultiTexture->texture);
		destMultiTexture->texture = srcMultiTexture->texture;
        
		RwTextureAddRef( destMultiTexture->texture );
	}
	
	memcpy(&destMultiTexture->matColor, &srcMultiTexture->matColor, sizeof(RwRGBAReal));
	

	RWRETURN(destMultiTexture);
}

static RwStream *ToonMultiTexStreamRead( RwStream *stream,
										 RwInt32   length    __RWUNUSED__,
										 void     *object,
										 RwInt32   offset,
										 RwInt32   sizeInObj __RWUNUSED__)
{
	RpToonMultiTexture *multiTexture;
	RwBool bSave = FALSE;
	
    multiTexture = GetToonMultiTexture(object, LocalOffset);

    if( !RwStreamReadReal(stream, &(multiTexture->matColor.red), sizeof(RwReal)) )
        return NULL;
    if( !RwStreamReadReal(stream, &(multiTexture->matColor.green), sizeof(RwReal)) )
        return NULL;
    if( !RwStreamReadReal(stream, &(multiTexture->matColor.blue), sizeof(RwReal)) )
        return NULL;
    if( !RwStreamReadReal(stream, &(multiTexture->matColor.alpha), sizeof(RwReal)) )
        return NULL;

	if (!RwStreamReadInt32(stream, (RwInt32 *)&bSave, sizeof(RwBool)))
		return NULL;

	if(bSave)
	{
        if( RwStreamFindChunk(stream, rwID_TEXTURE, NULL, NULL) )
        {
            multiTexture->texture = RwTextureStreamRead(stream);
        }
	}
	
	return stream;
}

static RwStream *ToonMultiTexStreamWrite( RwStream   *stream,
										  RwInt32     length    __RWUNUSED__,
										  const void *object,
										  RwInt32     offset,
										  RwInt32     sizeInObj __RWUNUSED__)
{
	RpToonMultiTexture *multiTexture;
	RwBool bSave = TRUE;
	multiTexture = GetToonMultiTexture(object, LocalOffset);

    if( !RwStreamWriteReal(stream, &(multiTexture->matColor.red), sizeof(RwReal)) )
        return NULL;
    if( !RwStreamWriteReal(stream, &(multiTexture->matColor.green), sizeof(RwReal)) )
        return NULL;
    if( !RwStreamWriteReal(stream, &(multiTexture->matColor.blue), sizeof(RwReal)) )
        return NULL;
    if( !RwStreamWriteReal(stream, &(multiTexture->matColor.alpha), sizeof(RwReal)) )
        return NULL;	
    
    if(multiTexture->texture == NULL)
	{
		bSave = FALSE;
	}

	if (!RwStreamWriteInt32(stream, (RwInt32 *)&bSave, sizeof(RwBool)))
		return NULL;

	if(bSave)
	{
		if( !RwTextureStreamWrite(multiTexture->texture, stream) )
			return NULL;
	}
	
	return stream;
}

static RwInt32 ToonMultiTexStreamGetSize( const void *object,
										  RwInt32     offset,
										  RwInt32     sizeInObj __RWUNUSED__)
{
	RwInt32 size = 0;
	RpToonMultiTexture *multiTexture;
	multiTexture = GetToonMultiTexture(object, LocalOffset);

    size += sizeof(RwReal) * 4;
	size += sizeof(RwBool);
	if(multiTexture->texture)
	{
        size += sizeof(RwTexture);
	}

	return size;
}


RwInt32 RpToonMultiTexPluginAttach(void)
{
	RwInt32 offset;
	LocalOffset = RpMaterialRegisterPlugin( sizeof(RpToonMultiTexture),
                                            MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_TOON_TEST_EXAMPLE),
                                            ToonMultiTexConstructor,
                                            ToonMultiTexDestructor,
                                            ToonMultiTexCopier);

    if( LocalOffset < 0 )
    {
        return FALSE;
    }
	
	offset = -1;
    offset = RpMaterialRegisterPluginStream( MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_TOON_TEST_EXAMPLE),
                                             ToonMultiTexStreamRead,
                                             ToonMultiTexStreamWrite,
                                             ToonMultiTexStreamGetSize);

    if( offset != LocalOffset )
    {
        return FALSE;
    }
	return TRUE;
}

RwBool RpToonMultiTexSetTexture(RpMaterial *pMaterial, RwTexture *pTexture)
{

	RpToonMultiTexture *multitexture = NULL;
	RWAPIFUNCTION(RWSTRING("RpToonMultiTextureSetTexture"));
    //RWASSERT( pTexture != NULL);
	RWASSERT( pMaterial != NULL);
	
	multitexture = GetToonMultiTexture(pMaterial, LocalOffset);
	RWASSERT( multitexture );

	
	if (pTexture)
    {
		if(multitexture->texture != NULL)
		{
			RwTextureDestroy(multitexture->texture);
			multitexture->texture = RwTextureAddRef(pTexture);
		}
		else
		{
			multitexture->texture = RwTextureAddRef(pTexture);
		}
        RWRETURN(TRUE);
    }
	else
	{
        if(multitexture->texture)
        {
		    RwTextureDestroy(multitexture->texture);
		    multitexture->texture = NULL;
            RWRETURN(TRUE);
        }
        else
        {
            RWRETURN(FALSE);
        }
    }    
		
	RWRETURN(FALSE);
}


RwTexture* RpToonMultiTexGetTexture(RpMaterial *pMaterial)
{
	RpToonMultiTexture *multitexture = NULL;
	RWAPIFUNCTION(RWSTRING("RpToonMultiTextureGetTexture"));
	RWASSERT( pMaterial != NULL);
	multitexture = GetToonMultiTexture(pMaterial, LocalOffset);

	RWRETURN(multitexture->texture);
}

void RpToonMaterialSetColor(RpMaterial *pMaterial, RwRGBAReal *pRGBA)
{
	RpToonMultiTexture *multiTexture = NULL;

	RWAPIFUNCTION(RWSTRING("RpToonMultiTextureGetTexture"));
	RWASSERT( pRGBA != NULL);
	multiTexture = GetToonMultiTexture(pMaterial, LocalOffset);

	multiTexture->matColor.red   = pRGBA->red;
	multiTexture->matColor.green = pRGBA->green;
	multiTexture->matColor.blue  = pRGBA->blue;
	multiTexture->matColor.alpha = pRGBA->alpha;
}

RwRGBAReal *RpToonMaterialGetColor(RpMaterial *pMaterial)
{
	RpToonMultiTexture *multiTexture = NULL;

	RWAPIFUNCTION(RWSTRING("RpToonMaterialGetColor"));
	
	multiTexture = GetToonMultiTexture(pMaterial, LocalOffset);

	return &(multiTexture->matColor);
}


