
/**
 * \ingroup rpmaterial
 * \page rpmaterialoverview RpMaterial Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection materialoverview Overview
 *
 * This object defines how the surface of a particular triangle in a
 * model will look and react to lighting. Materials define color, texture,
 * specularity, ambient reflectivity and diffusion. Materials also provide
 * a hook into the PowerPipe mechanism, so you can attach your own rendering
 * nodes on a per-material basis.
 *
 * Further information is available in the Dynamic Models chapter of the User Guide.
 *
*/


/****************************************************************************
 Includes
 */


#include <stdlib.h>
#include <string.h>

#include <rwcore.h>

#include "baworld.h"
#include "bamateri.h"
#include "bapipew.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWMATERIALGLOBAL(var)                           \
    (RWPLUGINOFFSET(rpMaterialGlobals,                  \
                    RwEngineInstance,                   \
                    materialModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwPluginRegistry materialTKList =
    { sizeof(RpMaterial),
      sizeof(RpMaterial),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

static RwModuleInfo materialModule;

static RwSurfaceProperties defaultSurfaceProperties =
    { (RwReal) (1.0), (RwReal) (1.0), (RwReal) (1.0) };

static RwUInt32 lastSeenRightsPluginId;
static RwUInt32 lastSeenExtraData;

RwStream*
_rpReadMaterialRights(RwStream *s,
                    RwInt32 len,
                    void *obj __RWUNUSED__,
                    RwInt32 off __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpReadMaterialRights"));

    RWASSERT((len == 4) || (len == 8));

    if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenRightsPluginId),
                           sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (len == 8)
    {
        if (!RwStreamReadInt32(s, (RwInt32*)&(lastSeenExtraData),
                               sizeof(RwInt32)))
        {
            RWRETURN((RwStream *) NULL);
        }
    }
    RWRETURN(s);
}

RwStream*
_rpWriteMaterialRights(RwStream *s,
                     RwInt32 len __RWUNUSED__,
                     const void *obj,
                     RwInt32 off __RWUNUSED__,
                     RwInt32 size __RWUNUSED__)
{
    const RpMaterial *mat;

    RWFUNCTION(RWSTRING("_rpWriteMaterialRights"));
    RWASSERT(obj);

    mat = (const RpMaterial*)obj;
    RWASSERT(mat->pipeline);
    RWASSERT(mat->pipeline->pluginId);

    if (!RwStreamWriteInt32(s, (RwInt32*)&(mat->pipeline->pluginId),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }
    if (!RwStreamWriteInt32(s, (RwInt32*)&(mat->pipeline->pluginData),
                            sizeof(RwInt32)))
    {
        RWRETURN((RwStream *) NULL);
    }

    RWRETURN(s);
}

RwInt32
_rpSizeMaterialRights(const void *obj,
                    RwInt32 off __RWUNUSED__,
                    RwInt32 size __RWUNUSED__)
{
    const RpMaterial *mat;

    RWFUNCTION(RWSTRING("_rpSizeMaterialRights"));
    RWASSERT(obj);

    mat = (const RpMaterial*)obj;
    if ((mat->pipeline) && (mat->pipeline->pluginId))
    {
        RWRETURN(sizeof(RwUInt32) * 2);
    }
    else
    {
        RWRETURN(0);
    }
}


/* Legacy support routine - when reading an geometry, copy it's old
 * surfaceProperties into this local global, and use these values if the
 * material doesn't have any surfaceProperties of it's own.
 */
void
_rpMaterialSetDefaultSurfaceProperties(const RwSurfaceProperties *
                                      surfaceProps)
{
    RWFUNCTION(RWSTRING("_rpMaterialSetDefaultSurfaceProperties"));

    if (surfaceProps == NULL)
    {
        defaultSurfaceProperties.ambient = (RwReal) (1.0);
        defaultSurfaceProperties.diffuse = (RwReal) (1.0);
        defaultSurfaceProperties.specular = (RwReal) (1.0);
    }
    else
    {
        RwSurfacePropertiesAssign(&defaultSurfaceProperties,
                                  surfaceProps);
    }

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                       Opening/Closing materials

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwInt32 _rpMaterialFreeListBlockSize = 256,
               _rpMaterialFreeListPreallocBlocks = 1;
static RwFreeList _rpMaterialFreeList;

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetFreeListCreateParams allows the developer to specify
 * how many \ref RpMaterial s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RpMaterialSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RpMaterialSetFreeListCreateParams"));
#endif
    _rpMaterialFreeListBlockSize = blockSize;
    _rpMaterialFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/****************************************************************************
 _rpMaterialOpen

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rpMaterialOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpMaterialOpen"));

    /* Grab the material globals offset (same for all instances) */
    materialModule.globalsOffset = offset;

    /* Get on with the set up */
    RWMATERIALGLOBAL(matFreeList) = RwFreeListCreateAndPreallocateSpace(
        materialTKList.sizeOfStruct,
        _rpMaterialFreeListBlockSize, sizeof(RwUInt32),
        _rpMaterialFreeListPreallocBlocks, &_rpMaterialFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_MATERIAL);

    if (!RWMATERIALGLOBAL(matFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* One more module instance */
    materialModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rpMaterialClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rpMaterialClose(void *instance,
                 RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpMaterialClose"));

    if (RWMATERIALGLOBAL(matFreeList))
    {
        RwFreeListDestroy(RWMATERIALGLOBAL(matFreeList));
        RWMATERIALGLOBAL(matFreeList) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    materialModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                              Materials

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                    Creation and destruction of materials

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpmaterial
 * \ref RpMaterialCreate is used to create a new material. New materials
 * have a default color of opaque white (red = green = blue = alpha = 255)
 * and are untextured.
 *
 * Note that the material is created with a reference count of one. Hence, it
 * is the responsiblity of the application to destroy the material if it is
 * no longer required.
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns pointer to the new material if successful or
 * NULL if there is an error.
 *
 * \see RpMaterialDestroy
 * \see RpMaterialAddRef
 * \see RpMaterialSetColor
 * \see RpMaterialGetColor
 * \see RpMaterialSetTexture
 * \see RpMaterialGetTexture
 * \see RpWorldPluginAttach
 */
RpMaterial         *
RpMaterialCreate(void)
{
    RpMaterial         *material;
    RwRGBA              color;

    RWAPIFUNCTION(RWSTRING("RpMaterialCreate"));
    RWASSERT(materialModule.numInstances);

    material = (RpMaterial *) RwFreeListAlloc(RWMATERIALGLOBAL(matFreeList),
               rwMEMHINTDUR_EVENT | rwID_MATERIAL);
    if (!material)
    {
        RWRETURN((RpMaterial *)NULL);
    }

    material->refCount = 1;

    /* White is an appropriate material color (especially for textured materials) */
    color.red = 0xff;
    color.green = 0xff;
    color.blue = 0xff;
    color.alpha = 0xff;

    RpMaterialSetColor(material, &color);
    material->texture = (RwTexture *)NULL;  /* Non textured */

    /* use the default material pipeline */
    material->pipeline = (RxPipeline *)NULL;

    RpMaterialSetSurfaceProperties(material, &defaultSurfaceProperties);

    /* Initialize memory allocated to toolkits */
    rwPluginRegistryInitObject(&materialTKList, material);

    RWASSERT(0 < material->refCount);

    RWRETURN(material);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialDestroy is used to destroy the specified material.
 * Only materials with a reference count of zero are actually removed from the
 * system. Reference counts greater than one indicate that this particular
 * material is still being used by other geometries; in this situation, this
 * function will only decrement the material's reference count. It is the
 * application's responsibility to destroy the material if it is no longer
 * required and it is not referenced by any geometries. This does not apply if
 * the material was acquired by reading a geometry from a binary stream:
 * destroying the geometry will also destroy the material it references.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpMaterialCreate
 * \see RpMaterialAddRef
 * \see RpWorldPluginAttach
 */
RwBool
RpMaterialDestroy(RpMaterial *material)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialDestroy"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    if (1 == material->refCount)
    {
        /* De-initialize memory allocated to toolkits */
        rwPluginRegistryDeInitObject(&materialTKList, material);

        /* Decreases the reference count on the texture too */
        RpMaterialSetTexture(material, (RwTexture *)NULL);

        RwFreeListFree(RWMATERIALGLOBAL(matFreeList), material);
    }
    else
    {
        /* RWCRTCHECKMEMORY(); */
        --material->refCount;
        /* RWCRTCHECKMEMORY(); */
    }

    RWRETURN(TRUE);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialClone is used to duplicate the specified material. This
 * function creates and initializes a new material and copies the old
 * materials's texture, color, pipeline, and surface properties
 * to the new one. If there are any plug-ins registered with the material
 * then the associated plugin data is also copied to the new material using
 * the registered copy constructor function.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 *
 * \return Returns pointer to the new material if successful or NULL if
 * there is an error.
 *
 * \see RpMaterialCreate
 * \see RpMaterialRegisterPlugin
 * \see RpWorldPluginAttach
 *
 */
RpMaterial *
RpMaterialClone(RpMaterial *material)
{
    RpMaterial                *newMaterial;
    RwTexture                 *texture;
    RxPipeline                *pipeline;
    const RwSurfaceProperties *surfaceProperties;

    RWAPIFUNCTION(RWSTRING("RpMaterialClone"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    /* Lets create a new empty material */
    newMaterial = RpMaterialCreate();
    if(!newMaterial)
    {
        /* Unable to create empty material */
        RWRETURN((RpMaterial *)NULL);
    }

    /* Lets get the texture ... */
    texture = RpMaterialGetTexture(material);
    if(texture)
    {
        /* ... and set it */
        if(!RpMaterialSetTexture(newMaterial, texture))
        {
            /* Lets free the material and return */
            RpMaterialDestroy(newMaterial);

            RWRETURN((RpMaterial *)NULL);
        }
    }

    /* Lets set the colour */
    RpMaterialSetColor(newMaterial, RpMaterialGetColor(material));

    /* Lets set the pipeline information */

    /* Need to set the pipeline and surface properties */
    if(!RpMaterialGetPipeline(material, &pipeline) ||
       !RpMaterialSetPipeline(newMaterial, pipeline))
    {
        /* Lets free the material and return */
        RpMaterialDestroy(newMaterial);

        RWRETURN((RpMaterial *)NULL);
    }

    surfaceProperties = RpMaterialGetSurfaceProperties(material);
    if(!surfaceProperties ||
       !RpMaterialSetSurfaceProperties(newMaterial, surfaceProperties))
    {
        /* Lets free the material and return */
        RpMaterialDestroy(newMaterial);

        RWRETURN((RpMaterial *)NULL);
    }

    /* Copy over plugin info */
    rwPluginRegistryCopyObject(&materialTKList, newMaterial, material);

    /* Lets return the new cloned material */
    RWRETURN(newMaterial);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpmaterial
 * \ref RpMaterialAddRef is used to increment the reference count of the
 * specified material. Use this function to indicate that a new object depends
 * on the material's existence. This ensures that the material is not
 * lost when other objects that use this material are destroyed.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 *
 * \return Returns pointer to the material if successful or NULL if there is
 * an error.
 *
 * \see RpMaterialDestroy
 * \see RwTextureAddRef
 * \see RpWorldPluginAttach
 */
RpMaterial *
RpMaterialAddRef(RpMaterial *material)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialAddRef"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    RWRETURN(RpMaterialAddRefMacro(material));
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetColor is used to specify the color of the given
 * material. Material colors are specified as RwUInt8 values in the range
 * 0 to 255 in each color channel.
 *
 * The default material color is opaque white (red = green = blue = alpha = 255).
 *
 * To use material colors the rpGEOMETRYMODULATEMATERIALCOLOR
 * flag needs to be set (see \ref RpGeometryFlag). If a model is exported with
 * material color 255, 255, 255, 255 the rpGEOMETRYMODULATEMATERIALCOLOR flag
 * is not set and the material color will \e not be used.
 *
 * The rpGEOMETRYMODULATEMATERIALCOLOR flag is set when creating a world.
 * For more information about material colors, refer to the Dynamic Models
 * User Guide chapter.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to material.
 * \param color  Pointer to RwRGBA value equal to the material color.
 *
 * \return Returns pointer to the material if successful or NULL if there
 * is an error.
 *
 * \see RpMaterialGetColor
 * \see RpMaterialSetTexture
 * \see RpMaterialGetTexture
 * \see RpWorldPluginAttach
 */
RpMaterial *
RpMaterialSetColor(RpMaterial *material, const RwRGBA *color)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialSetColor"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);
    RWASSERT(color);

    RWRETURN(RpMaterialSetColorMacro(material, color));
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetColor is used to retrieve the specified material's
 * color. Material colors are specified as RwUInt8 values in the range 0 to
 * 255 in each color channel.
 *
 * The default material color is opaque white (red = green = blue = alpha = 255).
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 *
 * \return Returns pointer to a RwRGBA value equal to the material's color
 * if successful or NULL if there is an error.
 *
 * \see RpMaterialSetColor
 * \see RpMaterialSetTexture
 * \see RpMaterialGetTexture
 * \see RpWorldPluginAttach
 */
const RwRGBA *
RpMaterialGetColor(const RpMaterial *material)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialGetColor"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    RWRETURN(RpMaterialGetColorMacro(material));
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetSurfaceProperties is used to modify the surface lighting
 * characteristics of the specified material. These surface properties define the
 * ambient, diffuse and specular reflection coefficients for all surfaces within
 * the geometry. The reflection coefficients are shared between all morph targets.
 * Default values for the coefficients are defined as follows: 0.2 for the ambient,
 * 0.6 for the diffuse and 0.6 for the specular.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  * Pointer to the material.
 *
 * \param surfaceProperties  * Pointer to a RwSurfaceProperties value specifying the new surface reflection
 * coefficients.
 *
 * \return
 * Returns a pointer to the geometry if successful or NULL if there is an error.
 *
 * \see RpMaterialGetSurfaceProperties
 * \see RpMaterialCreate
 * \see RpWorldPluginAttach
 */
RpMaterial *
RpMaterialSetSurfaceProperties(RpMaterial *material,
                               const RwSurfaceProperties *surfaceProperties)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialSetSurfaceProperties"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);
    RWASSERT(surfaceProperties);

    RWRETURN(RpMaterialSetSurfacePropertiesMacro(material,
                                                 surfaceProperties));
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetSurfaceProperties is used to retrieve the surface
 * lighting characteristics from the specified material. These surface
 * properties define the ambient, diffuse and specular reflection coefficients
 * for all surfaces within the geometry. The reflection coefficients are
 * shared between all morph targets. Default values for the coefficients
 * are defined as follows: 0.2 for the ambient, 0.6 for the diffuse and 0.6
 * for the specular.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the RpMaterial structure.
 *
 * \return Returns a pointer to an RwSurfaceProperties structure filled
 * with the surface properties information for the RpMaterial, or NULL
 * if an error occurred.
 *
 * \see RpMaterialSetSurfaceProperties
 * \see RpMaterialCreate
 * \see RpWorldPluginAttach
 */
const RwSurfaceProperties *
RpMaterialGetSurfaceProperties(const RpMaterial *material)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialGetSurfaceProperties"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    RWRETURN(RpMaterialGetSurfacePropertiesMacro(material));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetTexture is used to specify a texture for the given
 * material. If the material already posesses a texture the old texture is
 * destroyed before the new one is attached to the material.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to material.
 * \param texture  Pointer to texture.
 *
 * \return Returns pointer to the material if successful or NULL if there
 * is an error.
 *
 * \see RpMaterialGetTexture
 * \see RpMaterialSetColor
 * \see RpMaterialGetColor
 * \see RwTextureRead
 * \see RwTextureCreate
 * \see RwTextureDestroy
 * \see RpWorldPluginAttach
 */
RpMaterial *
RpMaterialSetTexture(RpMaterial *material, RwTexture *texture)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialSetTexture"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    /* Add ref;
     * the new one first just in case it's the same as the old one
     * - saves destroying it */
    if (texture)
    {
        RwTextureAddRef(texture);
    }

    if (material->texture)
    {
        /* Decrease the reference count on the old texture */
        RwTextureDestroy(material->texture);
    }

    material->texture = texture;

    RWRETURN(material);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetTexture is used to retrieve the texture attached
 * to the specified material.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to material.
 *
 * \return Returns pointer to the material's texture if successful or NULL if
 * there is an error or if the material has no texture.
 *
 * \see RpMaterialSetTexture
 * \see RpMaterialSetColor
 * \see RpMaterialGetColor
 * \see RpWorldPluginAttach
 */
RwTexture *
RpMaterialGetTexture(const RpMaterial *material)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialGetTexture"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    RWRETURN(RpMaterialGetTextureMacro(material));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/****************************************************************************
 *
 * Plugin registration functions
 */

/**
 * \ingroup rpmaterial
 * \ref RpMaterialRegisterPlugin is used to register a plugin and
 * reserve some space within a material. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * The world plugin must be attached before using this function.
 *
 * \param size  A RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param constructCB  Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB  Copy constructor for the plugin data block.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the material
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpMaterialValidatePlugins
 * \see RpMaterialRegisterPluginStream
 * \see RpMaterialSetStreamAlwaysCallBack
 * \see RpMaterialGetPluginOffset
 * \see RpWorldPluginAttach
 */
RwInt32
RpMaterialRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                         RwPluginObjectConstructor constructCB,
                         RwPluginObjectDestructor destructCB,
                         RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpMaterialRegisterPlugin"));
    RWASSERT(!materialModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&materialTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered material plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param readCB  Callback used when a chunk is read that is identified as being for
 *       this plugin.
 * \param writeCB  Callback used when a chunk should be written out for this plugin.
 * \param getSizeCB  Callback used to determine the binary stream size required for this
 *       plugin (return negative to suppress chunk writing).
 *
 * \return Returns a RwInt32 value equal to the byte offset within the material
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpMaterialSetStreamAlwaysCallBack
 * \see RpMaterialValidatePlugins
 * \see RpMaterialRegisterPlugin
 * \see RpMaterialGetPluginOffset
 * \see RpWorldPluginAttach
 */
RwInt32
RpMaterialRegisterPluginStream(RwUInt32 pluginID,
                               RwPluginDataChunkReadCallBack readCB,
                               RwPluginDataChunkWriteCallBack writeCB,
                               RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpMaterialRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    plug =
        rwPluginRegistryAddPluginStream(&materialTKList, pluginID,
                                        readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetRegistry is used to get the material plugin registry
 *
 * The world plugin must be attached before using this function.
 *
 * \return Returns the plugin registry.
 *
 * \see RwPluginRegistry
 */
RwPluginRegistry *
RpMaterialGetRegistry(void)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialGetRegistry"));

    RWRETURN(&materialTKList);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetStreamAlwaysCallBack is used to associate a
 * binary stream callback with a previously registered material plugin. This
 * callback is called for all plugins after stream data reading has completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param alwaysCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the material
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpMaterialRegisterPluginStream
 * \see RpMaterialValidatePlugins
 * \see RpMaterialRegisterPlugin
 * \see RpMaterialGetPluginOffset
 * \see RpWorldPluginAttach
 */
RwInt32
RpMaterialSetStreamAlwaysCallBack(
    RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpMaterialSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    plug = rwPluginRegistryAddPluginStreamAlwaysCB(
               &materialTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialSetStreamRightsCallBack is used to associate a
 * binary stream callback with a previously registered plugin. This callback
 * is called for the plugin whose rights are asserted on the read material.
 * Rights are asserted if the material had an attached pipeline with a plugin
 * id when it was streamed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID
 *                  (must be unique; used to identify binary chunks).
 * \param rightsCB  Callback used when object base and plugin data reading
 *                  is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the material
 *         of memory reserved for this plugin or
 *         a negative value if there is an error.
 *
 * \see RpMaterialRegisterPluginStream
 * \see RpMaterialValidatePlugins
 * \see RpMaterialRegisterPlugin
 * \see RpMaterialGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpMaterialSetStreamRightsCallBack(RwUInt32 pluginID,
                                  RwPluginDataChunkRightsCallBack rightsCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpMaterialSetStreamRightsCallBack"));
    RWASSERT(NULL != rightsCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlgnStrmRightsCB(
        &materialTKList, pluginID, rightsCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialGetPluginOffset is used to get the offset of a previously
 * registered material plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns the data block offset if successful or -1 if the plugin
 * is not registered.
 *
 * \see RpMaterialRegisterPlugin
 * \see RpMaterialRegisterPluginStream
 * \see RpMaterialSetStreamAlwaysCallBack
 * \see RpMaterialValidatePlugins
 * \see RpWorldPluginAttach
 */
RwInt32
RpMaterialGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpMaterialGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&materialTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialValidatePlugins validates the plugin memory
 * allocated within the specified material. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  material   Pointer to the material to validate.
 *
 * \return Returns TRUE is the material data is valid or FALSE if there is an
 * error or if the material data has become corrupt.
 *
 * \see RpMaterialRegisterPlugin
 * \see RpMaterialRegisterPluginStream
 * \see RpWorldPluginAttach
 */
RwBool
RpMaterialValidatePlugins(const RpMaterial * material __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RpMaterialValidatePlugins"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);

#ifdef RWDEBUG
    {
        RwBool              valid;

        RWASSERT(0 < material->refCount);

        valid = rwPluginRegistryValidateObject(&materialTKList, material);

        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

/*
 * $ref _rpMaterialChunkInfoRead extracts
 * Chunk Info data from a RenderWare stream. The data is converted
 * from its original format and inserted into an RpMaterialChunkInfo
 * structure. A pointer to this structure is returned on success.
 *
 * $param stream  Pointer to the stream from which to read the chunk info.
 * $param materialChunkInfo  Pointer to an RpMaterialChunkInfo object.
 * $param bytesRead  Pointer to a RwInt32 which will be used to return the
 * number of bytes read.
 *
 * $return Returns pointer to RpMaterialChunkInfo if successful or NULL
 * if there is an error
 *
 * $see RpMaterialStreamRead
 *
 */
RpMaterialChunkInfo *
_rpMaterialChunkInfoRead(RwStream * stream,
                         RpMaterialChunkInfo * materialChunkInfo,
                         RwInt32 * bytesRead)
{
    RwUInt32            size;   /*, readSize; */
    RwRGBA              tmpCol;

    RWFUNCTION(RWSTRING("_rpMaterialChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(materialChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           &size, (RwUInt32 *)NULL))
    {
        RWRETURN((RpMaterialChunkInfo *)NULL);
    }

    RWASSERT(size <= sizeof(RpMaterialChunkInfo));
    memset(materialChunkInfo, 0, sizeof(RpMaterialChunkInfo));

    /*
     * readSize = sizeof(RpMaterialChunkInfo);
     * if (RwStreamRead(stream, materialChunkInfo, readSize) != readSize)
     */
    if (RwStreamRead(stream, materialChunkInfo, size) != size)
    {
        RWRETURN(FALSE);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);

    tmpCol = materialChunkInfo->color;
    /*(void)RwMemNative32(materialChunkInfo, readSize); */
    (void)RwMemNative32(materialChunkInfo, size);
    materialChunkInfo->color = tmpCol;

    if (size <=
        (((RwUInt32) & materialChunkInfo->surfaceProps) -
         ((RwUInt32) materialChunkInfo)))
    {
        materialChunkInfo->surfaceProps.ambient = (RwReal)(1.0);
        materialChunkInfo->surfaceProps.diffuse = (RwReal)(1.0);
        materialChunkInfo->surfaceProps.specular = (RwReal)(1.0);
    }
    else
    {
        (void)RwMemFloat32ToReal(&materialChunkInfo->surfaceProps,
                           sizeof(materialChunkInfo->surfaceProps));
    }

    RWRETURN(materialChunkInfo);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialStreamRead is used to read a material from the specified
 * binary stream. Prior to this function call a binary material chunk must
 * have been found in the stream.
 *
 * The world plugin must be attached before using this function.
 *
 * The sequence to locate and read a material from a binary stream connected
 * to a disk file is as follows:
 * \code
   RwStream *stream;
   RpMaterial *NewMaterial;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_MATERIAL, NULL, NULL) )
       {
           NewMaterial = RpMaterialStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
   \endcode
 *
 * \param stream  Pointer to the binary stream.
 *
 * \return Returns pointer to the material if successful or NULL if there is
 * an error.
 *
 * \see RpMaterialStreamGetSize
 * \see RpMaterialStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 * \see RpWorldPluginAttach
 */
RpMaterial         *
RpMaterialStreamRead(RwStream * stream)
{
    RwUInt32            size;
    RwUInt32            version;

    RWAPIFUNCTION(RWSTRING("RpMaterialStreamRead"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RpMaterial *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        RpMaterial         *material;
        _rpMaterial         mat;

        RWASSERT(size <= sizeof(mat));
        memset(&mat, 0, sizeof(mat));
        if (RwStreamRead(stream, &mat, size) != size)
        {
            RWRETURN(FALSE);
        }

        {
            RwRGBA              tmp;

            tmp = mat.color;
            (void)RwMemNative32(&mat, sizeof(mat));
            mat.color = tmp;
        }

        /* Create the material */
        material = RpMaterialCreate();
        if (!material)
        {
            RWRETURN(FALSE);
        }

        /* We want the color */
        RpMaterialSetColor(material, &mat.color);


        /* set the surfaceProps */
        (void)RwMemFloat32ToReal(&mat.surfaceProps, sizeof(mat.surfaceProps));
        /*material->surfaceProps = mat.surfaceProps; */
        RpMaterialSetSurfaceProperties(material, &mat.surfaceProps);

        /* Check if it has a texture */
        material->texture = (RwTexture *)NULL;
        if (mat.textured)
        {
            /* Read in the texture */
            if (!RwStreamFindChunk(stream, (RwUInt32)rwID_TEXTURE,
                                   (RwUInt32 *)NULL, &version))
            {
                RpMaterialDestroy(material);
                RWRETURN((RpMaterial *)NULL);
            }
            if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
            {
                /* If we don't get the material, the polygons will just be the
                 * color of the underlying material - usually white
                 */
                material->texture = RwTextureStreamRead(stream);
            }
            else
            {
                RpMaterialDestroy(material);
                RWERROR((E_RW_BADVERSION));
                RWRETURN((RpMaterial *)NULL);
            }
        }

        /* A bit ugly. If a rights owner is spotted, this will be set */
        lastSeenRightsPluginId = 0;
        lastSeenExtraData = 0;

        /* MATERIAL EXTENSION DATA */
        if (!rwPluginRegistryReadDataChunks(&materialTKList, stream, material))
        {
            RpMaterialDestroy(material);
            RWRETURN((RpMaterial *)NULL);
        }

        if (lastSeenRightsPluginId)
        {
            /* Silently ignore errors if any */
            _rwPluginRegistryInvokeRights(&materialTKList, lastSeenRightsPluginId,
                                          material, lastSeenExtraData);
        }

        RWASSERT(0 < material->refCount);

        RWRETURN(material);
    }

    RWERROR((E_RW_BADVERSION));
    RWRETURN((RpMaterial *)NULL);
}

/****************************************************************************
 *
 * Binary Format Functions
 */

/**
 * \ingroup rpmaterial
 * \ref RpMaterialStreamGetSize is used to determine the size in bytes of
 * the binary representation of the specified material. This is used in the
 * binary chunk header to indicate the size of the material chunk. The size
 * does not include the size of the chunk header.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 *
 * \return Returns a RwUInt32 value equal to the chunk size of the material
 * in bytes if successful or zero if there is an error.
 *
 * \see RpMaterialStreamRead
 * \see RpMaterialStreamWrite
 * \see RpWorldPluginAttach
 */
RwUInt32
RpMaterialStreamGetSize(const RpMaterial * material)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RpMaterialStreamGetSize"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);

    size = sizeof(_rpMaterial) + rwCHUNKHEADERSIZE;
    if (material->texture)
    {
        size +=  ( RwTextureStreamGetSize(material->texture) +
                   rwCHUNKHEADERSIZE );
    }
    size += ( rwPluginRegistryGetSize(&materialTKList, material) +
              rwCHUNKHEADERSIZE ) ;

    RWRETURN(size);
}

/**
 * \ingroup rpmaterial
 * \ref RpMaterialStreamWrite is used to write the
 * specified material to the given binary stream.
 * The stream must have been opened prior to this
 * function call.
 *
 * The world plugin must be attached before using this function.
 *
 * \param material  Pointer to the material.
 * \param stream  Pointer to the binary stream.
 *
 * \return Returns pointer to the material if successful or NULL if there is
 * an error.
 *
 * \see RpMaterialStreamGetSize
 * \see RpMaterialStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RpWorldPluginAttach
 *
 */
const RpMaterial   *
RpMaterialStreamWrite(const RpMaterial * material, RwStream * stream)
{
    _rpMaterial         mat;
    const RwSurfaceProperties * source;
    const RwRGBA       *color;
    RWAPIFUNCTION(RWSTRING("RpMaterialStreamWrite"));
    RWASSERT(materialModule.numInstances);
    RWASSERT(material);
    RWASSERT(0 < material->refCount);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_MATERIAL,
                                  RpMaterialStreamGetSize(material)))
    {
        RWRETURN((const RpMaterial *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(_rpMaterial)))
    {
        RWRETURN((const RpMaterial *)NULL);
    }

    /* Set up the structure */
    mat.flags = 0;

    if (material->texture)
    {
        mat.textured = TRUE;
    }
    else
    {
        mat.textured = FALSE;
    }

    source = RpMaterialGetSurfaceProperties(material);

    RwSurfacePropertiesAssign(&mat.surfaceProps, source);
    (void)RwMemRealToFloat32(&mat.surfaceProps,
                       sizeof(mat.surfaceProps));

    (void)RwMemLittleEndian32(&mat, sizeof(mat));
    /* Has to happen after we swap bytes */

    color = RpMaterialGetColor(material);
    RwRGBAAssign(&mat.color, color);

    if (!RwStreamWrite(stream, &mat, sizeof(mat)))
    {
        RWRETURN((const RpMaterial *)NULL);
    }

    /* Write off the texture name information */
    if (material->texture)
    {
        if (!RwTextureStreamWrite(material->texture, stream))
        {
            RWRETURN((const RpMaterial *)NULL);
        }
    }

    /* MATERIAL EXTENSION DATA */
    if (!rwPluginRegistryWriteDataChunks(&materialTKList, stream, material))
    {
        /* Failed to write extension data */
        RWRETURN((const RpMaterial *)NULL);
    }

    RWRETURN(material);
}
