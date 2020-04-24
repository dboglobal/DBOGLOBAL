//*****************************************************************************
// File       : BaseType_RenderWare.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __BASE_TYPE_RENDER_WARE_H__
#define __BASE_TYPE_RENDER_WARE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward Refenence Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard Template Library Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Const Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

#define WORLDCAMERAEXTFROMCAMERA(camera) \
   ((RpWorldCameraExt *)(((RwUInt8 *)(camera)) + sizeof( RwCamera ) + 4 ))
#define WORLDCAMERAEXTFROMCONSTCAMERA(camera) \
   ((const RpWorldCameraExt *)(((const RwUInt8 *)(camera)) + sizeof( RwCamera ) + 4))

#define ATOMICEXTFROMATOMIC(atomic) \
   ((RpWorldAtomicExt *)(((RwUInt8 *)(atomic)) + atomicExtOffset))
#define ATOMICEXTFROMCONSTATOMIC(atomic) \
   ((const RpWorldAtomicExt *)(((const RwUInt8 *)(atomic)) + atomicExtOffset))

#define CLUMPEXTFROMCLUMP(clump) \
   ((RpWorldClumpExt *)(((RwUInt8 *)(clump)) + clumpExtOffset))
#define CLUMPEXTFROMCONSTCLUMP(clump) \
   ((const RpWorldClumpExt *)(((const RwUInt8 *)(clump)) + clumpExtOffset))

#define LIGHTEXTFROMLIGHT(light) \
   ((RpWorldLightExt *)(((RwUInt8 *)(light)) + lightExtOffset))
#define LIGHTEXTFROMCONSTLIGHT(light) \
   ((const RpWorldLightExt *)(((const RwUInt8 *)(light)) + lightExtOffset))

#define RPWORLDOBJGLOBAL(var) (RWPLUGINOFFSET(rpWorldObjGlobals, RwEngineInstance, worldObjModule.globalsOffset)->var)


typedef struct RpWorldCameraExt RpWorldCameraExt;
struct RpWorldCameraExt
{
    /* List of atomic sectors within the frustum */
    RpWorldSector     **frustumSectors;
    RwInt32             space;
    RwInt32             position;

    /* The world the camera lives in */
    RpWorld            *world;

    /* Old start update/end update functions */
    RwCameraBeginUpdateFunc oldBeginUpdate;
    RwCameraEndUpdateFunc oldEndUpdate;

    /* Old sync function */
    RwObjectHasFrameSyncFunction oldSync;
};


//-----------------------------------------------------------------------------
// Macro & Template Function Table
//-----------------------------------------------------------------------------

#endif //__BASE_TYPE_RENDER_WARE_H__

//*****************************************************************************
//
// End of File : BaseType_RenderWare.h
//
//*****************************************************************************