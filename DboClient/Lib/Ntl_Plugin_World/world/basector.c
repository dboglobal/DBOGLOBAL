/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include <rwcore.h>

#include "baworld.h"
#include "balight.h"
#include "bamesh.h"
#include "basector.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    sectorTKList =
    { sizeof(RpWorldSector),
      sizeof(RpWorldSector),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo sectorModule;

/****************************************************************************
 _rpSectorOpen

 On entry   :
 On exit    : NULL on failure
 */

void               *
_rpSectorOpen(void *instance,
              RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpSectorOpen"));

    /* One more module instance */
    sectorModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rpSectorClose

 On entry   :
 On exit    : NULL on failure
 */

void               *
_rpSectorClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpSectorClose"));

    /* One less module instance */
    sectorModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                          Sector handling functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorRender is used to render all static objects in
 * the specified world sector to the current camera's image raster.
 *
 * Note this function should only be called between
 * \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate to ensure that
 * any rendering that takes place is directed towards an image raster
 * connected to a camera.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns pointer to the world sector if successful or
 * NULL if there is an error.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RpWorldSetSectorRenderCallBack
 * \see RpWorldGetSectorRenderCallBack
 * \see RpWorldRender
 * \see RpClumpRender
 * \see RpAtomicRender
 * \see RpWorldPluginAttach
 *
 */
RpWorldSector      *
RpWorldSectorRender(RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorRender"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    /* App has allowed the rendering to continue */
    RWRETURN(((RpWorld *)RWSRCGLOBAL(curWorld))->renderCallBack(sector));
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorForAllMeshes is used to apply the given callback
 * function to all meshes in the given world sector.  The format of the
 * callback function is:
 *
 * RpMesh *(*RpMeshCallBack)(RpMesh *mesh, RpMeshHeader *meshHeader, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated.  However, \ref RpWorldSectorForAllMeshes will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector containing the meshes.
 * \param fpCallBack  Pointer to the callback function to apply to each mesh.
 * \param pData  Pointer to the user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the world sector if successful or NULL if there
 * is an error or if the world sector does not have any mesh information.
 *
 * \see RpWorldSectorGetBBox
 * \see RpWorldSectorGetNumVertices
 * \see RpGeometryForAllMeshes
 * \see RpWorldSectorGetNumTriangles
 * \see RpWorldSectorGetWorld
 * \see RpWorldPluginAttach
 *
 */
const RpWorldSector *
RpWorldSectorForAllMeshes(const RpWorldSector * sector,
                          RpMeshCallBack fpCallBack, void *pData)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorForAllMeshes"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);
    RWASSERT(fpCallBack);

    if (sector->mesh)
    {
        if (_rpMeshHeaderForAllMeshes(sector->mesh, fpCallBack, pData))
        {
            RWRETURN(sector);
        }
    }

    /* No mesh data, of callback function failed */
    RWRETURN((const RpWorldSector *)NULL);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetBBox is used to retrieve the specified world
 * sector's bounding box. This corresponds to the maximum extent of
 * the world sector based on the planes of the BSP tree. The function
 * \ref RpWorldSectorGetTightBBox may be used instead to obtain a bounding
 * box that tightly encloses the geometry in the sector.
 *
 * Note that this function is used for debug purposes only and, for efficiency,
 * is available as a macro for final release versions of an application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns pointer to a RwBBox value if successful or NULL if
 * there is an error.
 *
 * \see RpWorldSectorGetTightBBox
 * \see RpWorldSectorForAllMeshes
 * \see RpWorldSectorGetNumVertices
 * \see RpWorldSectorGetNumTriangles
 * \see RpWorldSectorGetWorld
 * \see RpWorldGetBBox
 * \see RpWorldCreate
 * \see RpWorldStreamRead
 * \see RpWorldPluginAttach
 *
 */
const RwBBox       *
RpWorldSectorGetBBox(const RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetBBox"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    RWRETURN(RpWorldSectorGetBBoxMacro(sector));
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetTightBBox is used to retrieve the tight bounding box
 * of the specified world sector. This tightly encloses the geometry of the
 * sector. Use \ref RpWorldSectorGetBBox to retrieve the outer bounding box
 * that extends to the BSP planes. For space filling (empty) sectors, both
 * functions return the full extent.
 *
 * Atomics and other objects in the world sector may or may not lie within the
 * tight bounding box.
 *
 * Note that this function is used for debug purposes only and, for efficiency,
 * is available as a macro for final release versions of an application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns pointer to a RwBBox value if successful or NULL if
 * there is an error.
 *
 * \see RpWorldSectorGetBBox
 * \see RpWorldGetBBox
 * \see RpWorldCreate
 * \see RpWorldStreamRead
 * \see RpWorldPluginAttach
 */
const RwBBox       *
RpWorldSectorGetTightBBox(const RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetTightBBox"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    RWRETURN(RpWorldSectorGetTightBBoxMacro(sector));
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetNumTriangles is used to retrieve the number of
 * triangles that reside within the specified world sector. Only triangles
 * associated with static geometry are counted.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns a RwInt32 value equal to the number of triangles if
 * successful or zero if there is an error or if there are no triangles.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \see RpWorldSectorGetTriangles
 * \see RpWorldSectorGetBBox
 * \see RpWorldSectorForAllMeshes
 * \see RpWorldSectorGetNumVertices
 * \see RpWorldSectorGetWorld
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorGetNumTriangles(const RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetNumTriangles"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    RWRETURN(RpWorldSectorGetNumTrianglesMacro(sector));
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetNumVertices is used to retrieve the number of
 * vertices that reside within the specified atomic sector. Only vertices
 * associated with static geometry are counted.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns a RwInt32 value equal to the number of vertices if successful
 * or zero if there is an error or if there are no vertices.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \see RpWorldSectorGetVertices
 * \see RpWorldSectorGetBBox
 * \see RpWorldSectorForAllMeshes
 * \see RpWorldSectorGetNumTriangles
 * \see RpWorldSectorGetWorld
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorGetNumVertices(const RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetNumVertices"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    RWRETURN(RpWorldSectorGetNumVerticesMacro(sector));
}


/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetVertices is used to retrieve the array of vertex
 * positions for the static geometry of the specified world sector.
 * These values should be considered read-only.
 *
 * The vertices returned are the platform independent (PI) data, and not the
 * vertex buffer used for rendering. If the world has been pre-instanced
 * (using \ref RpWorldInstance) then the PI data will no longer exist and
 * this function will return NULL.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns a pointer to the array of vertex positions. This
 * can be NULL if the sector contains no vertices, or if the world
 * has been pre-instanced.
 *
 * \see RpWorldSectorGetNumVertices
 * \see RpWorldSectorGetNumTriangles
 * \see RpWorldSectorGetTriangles
 */
const RwV3d *
RpWorldSectorGetVertices(const RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetVertices"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    RWRETURN(RpWorldSectorGetVerticesMacro(sector));
}


/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetTriangles is used to retrieve the array of
 * triangles for the static geometry of a world sector. These should
 * be considered read-only.
 *
 * The triangles are platform independent (PI) data, and define the
 * vertex connectivity and material properties of the world sector geometry.
 * They are used in the generation of meshes during a call to
 * \ref RpWorldUnlock, and may also be used for collision detection. If the
 * world has been pre-instanced (using \ref RpWorldInstance)
 * then the PI data will no longer exist and this function will return NULL.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param sector  Pointer to the world sector.
 *
 * \return Returns a pointer to the array of triangles. This
 * can be NULL if the sector contains no triangles, or if the world
 * has been pre-instanced.
 *
 * \see RpWorldSectorGetVertices
 * \see RpWorldSectorGetNumVertices
 * \see RpWorldSectorGetNumTriangles
 */
const RpTriangle *
RpWorldSectorGetTriangles(const RpWorldSector * sector)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetTriangles"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

    RWRETURN(RpWorldSectorGetTrianglesMacro(sector));
}


#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorRegisterPlugin is used to register a plugin and
 * reserve some space within a world sector. This must happen after the engine
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
 * \return Returns RwInt32 value equal to the byte offset within the world
 * sector of memory reserved for this plugin if successful or -1 if there
 * is an error.
 *
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldValidatePlugins
 * \see RpWorldGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                            RwPluginObjectConstructor constructCB,
                            RwPluginObjectDestructor destructCB,
                            RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorRegisterPlugin"));
    RWASSERT(!sectorModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&sectorTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);
    RWRETURN(plug);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered world sector
 * plugin.
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
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * sector of memory reserved for this plugin if successful or -1 if there is
 * an error.
 *
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldGetPluginOffset
 * \see RpWorldValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorRegisterPluginStream(RwUInt32 pluginID,
                                  RwPluginDataChunkReadCallBack readCB,
                                  RwPluginDataChunkWriteCallBack
                                  writeCB,
                                  RwPluginDataChunkGetSizeCallBack
                                  getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStream(&sectorTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorSetStreamAlwaysCallBack is used to associate
 * a binary stream callback with a previously registered world sector plugin.
 * This callback is called for all plugins after stream data reading has
 * completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param alwaysCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * sector of memory reserved for this plugin if successful or -1 if there is
 * an error.
 *
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldGetPluginOffset
 * \see RpWorldValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorSetStreamAlwaysCallBack(
    RwUInt32 pluginID, RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStreamAlwaysCB(
               &sectorTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorSetStreamRightsCallBack is used to associate
 * a binary stream callback with a previously registered world sector plugin.
 * This callback is called for the plugin with rights after stream data
 * reading has * completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param rightsCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the world
 * sector of memory reserved for this plugin if successful or -1 if there is
 * an error.
 *
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldGetPluginOffset
 * \see RpWorldValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorSetStreamRightsCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkRightsCallBack rightsCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorSetStreamRightsCallBack"));
    RWASSERT(rightsCB);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlgnStrmRightsCB(&sectorTKList, pluginID,
                                                rightsCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorGetPluginOffset is used to get the offset of a
 * previously registered world sector plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns the data block offset if successful or -1 if the plugin
 * is not registered.
 *
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorValidatePlugins
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldGetPluginOffset
 * \see RpWorldValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpWorldSectorGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpWorldSectorGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&sectorTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rpworldsector
 * \ref RpWorldSectorValidatePlugins is used to validate the plugin memory
 * allocated within the specified world sector. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  sector   Pointer to the world sector.
 *
 * \return Returns TRUE if the world sector data is valid or FALSE if there is an
 * error or if the world sector data has become corrupt.
 *
 * \see RpWorldSectorRegisterPlugin
 * \see RpWorldSectorRegisterPluginStream
 * \see RpWorldSectorSetStreamAlwaysCallBack
 * \see RpWorldSectorGetPluginOffset
 * \see RpWorldRegisterPlugin
 * \see RpWorldRegisterPluginStream
 * \see RpWorldSetStreamAlwaysCallBack
 * \see RpWorldValidatePlugins
 * \see RpWorldGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpWorldSectorValidatePlugins(const RpWorldSector * sector __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RpWorldSectorValidatePlugins"));
    RWASSERT(sectorModule.numInstances);
    RWASSERT(sector);

#ifdef RWDEBUG
    {
        RwBool              valid;

        valid = rwPluginRegistryValidateObject(&sectorTKList, sector);

        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}
