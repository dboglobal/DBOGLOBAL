/*
 *  RenderWare Graphics Collision Plugin
 *
 *  File:    collbuild.h
 *  Purpose: Collision data generation
 */

#ifndef COLLBUILD_H
#define COLLBUILD_H

/* RWPUBLIC */

/******************************************************************************
 *  Types
 */

/**
 * \ingroup rpcollis
 * These flags are used in an \ref RpCollisionBuildParam structure to
 * control the generation of collision data for an \ref RpGeometry or
 * \ref RpWorldSector.
 *
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldBuildData
 */
enum RpCollisionBuildFlags
{
    rpCOLLISIONBUILD_SORTTRIANGLES = 0x01, 
    /**<
     * <b>Use with caution</b>. This specifies that the triangles of an 
     * \ref RpWorldSector or \ref RpGeometry should be sorted according to 
     * their location in the collision tree, thus eliminating the need
     * for a remapping array in the datastructure (see 
     * \ref RpCollTreeGetEntryMap). 
     *
     * If this option is used, it is the responsibility of the application to
     * update any additional data in the world sector or geometry that is 
     * dependent on the triangle order. 
     *
     * If the sort order is required by the application, an 
     * \ref RpCollisionBuildSortCallBack should be specified in the 
     * \ref RpCollisionBuildParam. 
     */

    rpCOLLISIONBUILDFLAGS_FORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpCollisionBuildFlags RpCollisionBuildFlags;


/**
 * \ingroup rpcollis
 * RpCollisionBuildSortCallBack represents a function that may be called 
 * during the generation of collision data for an \ref RpWorldSector or 
 * \ref RpGeometry when the \ref rpCOLLISIONBUILD_SORTTRIANGLES option is
 * used. This option means that the triangles of the world sector
 * or geometry are automatically sorted, and this call back is only
 * required in cases where additional triangle data must be processed
 * such as an \ref RpUserDataArray.
 *
 * The call back may be setup in the \ref RpCollisionBuildParam.
 *
 * \param  object  \ref RpWorldSector or \ref RpGeometry. Use 
 *                 \ref RwObjectGetType to distinguish between the two.
 * \param  tree    The \ref RpCollTree for the sector or geometry.
 * \param  table   An array holding the sort table. This represents the
 *                 new triangle array and each entry contains the 
 *                 original index of the triangle.
 * \param  data    Application specific data.
 *
 * \return Pointer to the world sector or geometry.
 *
 * \see RpCollisionGeometryBuildData
 * \see RpCollisionWorldSectorBuildData
 * \see RpCollisionWorldBuildData
 */
typedef void * (*RpCollisionBuildSortCallBack)(void        *object, 
                                               RpCollTree  *tree, 
                                               RwUInt16    *sortTable,
                                               void        *data);

/**
 * \ingroup rpcollis
 * \struct RpCollisionBuildParam
 * This parameter block may be passed to the functions 
 * \ref RpCollisionWorldSectorBuildData, \ref RpCollisionWorldBuildData 
 * and \ref RpCollisionGeometryBuildData to control the generation of
 * collision data (otherwise a NULL argument indicates that default
 * settings should be used). The function 
 * \ref RpCollisionBuildParamInitDefaults can be used to initialize the
 * parameters to their default settings.
 */
typedef struct RpCollisionBuildParam  RpCollisionBuildParam;
struct RpCollisionBuildParam
{
    RwUInt32    flags;              
        /**< \ref RpCollisionBuildFlags */

    RpCollisionBuildSortCallBack    sortCallBack;
        /**< Function to recieve information about sorting order when the
        \ref rpCOLLISIONBUILD_SORTTRIANGLES option is set. */

    void                           *sortCallBackData;
        /**< Data to be passed to the sort call back */
};


/******************************************************************************
 *  Functions
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* General collision data */
extern RpCollisionBuildParam *
RpCollisionBuildParamInitDefaults(RpCollisionBuildParam *param);

/* WorldSector collision data */
extern RpWorldSector *
RpCollisionWorldSectorBuildData(
    RpWorldSector           *worldSector,
    RpCollisionBuildParam   *param);

/* World collision data (convenience functions which call WorldSector functions) */
extern RpWorld *
RpCollisionWorldBuildData(
    RpWorld                 *world,
    RpCollisionBuildParam   *param);

/* Geometry collision data */
extern RpGeometry *
RpCollisionGeometryBuildData(
    RpGeometry              *geometry,
    RpCollisionBuildParam   *param);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

/* RWPUBLICEND */

#endif /* COLLBUILD_H */
