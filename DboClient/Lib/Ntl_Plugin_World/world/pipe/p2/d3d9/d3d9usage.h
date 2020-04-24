/***************************************************************************
 *                                                                         *
 * Module  : d3d9usage.h                                                   *
 *                                                                         *
 * Purpose : See d3d9usage.c                                               *
 *                                                                         *
 **************************************************************************/

#ifndef D3D9USAGE_H
#define D3D9USAGE_H

/****************************************************************************
 Includes
 */

#include <d3d9.h>

#include "rwcore.h"
#include "baworld.h"

/****************************************************************************
 Global Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global Types
 */

/**
 * \ingroup worldextensionsd3d9
 * RpD3D9GeometryUsageFlag represents the options available to inform
 * RWD3D9 about the future usage of a specific geometry.
 * The usage flags may be set via \ref RpD3D9GeometrySetUsageFlags.
 */
enum RpD3D9GeometryUsageFlag
{
    rpD3D9GEOMETRYUSAGE_DYNAMICPOSITIONS  =     0x02,   /**<Positions will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICNORMALS    =     0x04,   /**<Normals will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICPRELIT     =     0x08,   /**<Prelight information will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS  =     0x10,   /**<The 1st set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS1 =     0x10,   /**<The 1st set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS2 =     0x20,   /**<The 2nd set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS3 =     0x40,   /**<The 3rd set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS4 =     0x80,   /**<The 4th set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS5 =   0x0100,   /**<The 5th set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS6 =   0x0200,   /**<The 6th set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS7 =   0x0400,   /**<The 7th set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDS8 =   0x0800,   /**<The 8th set of texture coordinates will be locked and changed every frame. */
    rpD3D9GEOMETRYUSAGE_DYNAMICTEXCOORDSALL = 0x0ff0,   /**<All sets of texture coordinates will be locked and changed every frame. */

    rpD3D9GEOMETRYUSAGE_DYNAMICMASK       =   0x0fff,   /**<Mask for finding dynamic information. */

    rpD3D9GEOMETRYUSAGE_CREATETANGENTS    =  0x10000,   /**<Per-vertex tangents are going to be used for this geometry. */

    rpD3D9GEOMETRYUSAGEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT

};
typedef enum RpD3D9GeometryUsageFlag RpD3D9GeometryUsageFlag;

/**
 * \ingroup worldextensionsd3d9
 * RpD3D9WorldSectorUsageFlag represents the options available to inform
 * RWD3D9 about the future usage of a specific world sector.
 * The usage flags may be set via \ref RpD3D9WorldSectorSetUsageFlags.
 */
enum RpD3D9WorldSectorUsageFlag
{
    rpD3D9WORLDSECTORUSAGE_CREATETANGENTS    =  0x10000,   /**<Per-vertex tangents are going to be used for this world sector. */

    rpD3D9WORLDSECTORUSAGEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT

};
typedef enum RpD3D9WorldSectorUsageFlag RpD3D9WorldSectorUsageFlag;

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern void
RpD3D9GeometrySetUsageFlags(RpGeometry *geometry,
                            RpD3D9GeometryUsageFlag flags);

extern RpD3D9GeometryUsageFlag
RpD3D9GeometryGetUsageFlags(const RpGeometry *geometry);

extern void
RpD3D9WorldSectorSetUsageFlags(RpWorldSector *sector,
                               RpD3D9WorldSectorUsageFlag flags);

extern RpD3D9WorldSectorUsageFlag
RpD3D9WorldSectorGetUsageFlags(const RpWorldSector *sector);
/* RWPUBLICEND */

extern RwBool
_rpD3D9UsageFlagPluginAttach(void);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif /* __cplusplus */
/* RWPUBLICEND */

#endif /* D3D9USAGE_H */
