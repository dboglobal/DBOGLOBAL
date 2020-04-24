/*****************************************************************************
 *                                                                           *
 * module : d3d9usage.c                                                      *
 *                                                                           *
 * purpose: Usage flags                                                      *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 includes
 */
#include <d3d9.h>

#include <string.h>

#include <rwcore.h>
#include "bageomet.h"
#include "baworld.h"

#include "d3d9usage.h"


/*****************************************************************************
 local defines
 */

//#define CHECK_RANGE

#define SMALL_FLOAT 1e-12f

#define RWD3D9GETUSAGEFLAG(_p, _o)    \
    ((RwUInt32 *)(((RwUInt8 *)(_p)) + (_o)))

#define RWD3D9GETUSAGEFLAGCONST(_p, _o)    \
    ((const RwUInt32 *)(((const RwUInt8 *)(_p)) + (_o)))

#define RWD3D9GEOMETRYGETUSAGEFLAG(_p)    \
    ((RwUInt32 *)(((RwUInt8 *)(_p)) + _rpD3D9GeometryUsageFlagOffset))

#define RWD3D9GEOMETRYGETCONSTUSAGEFLAG(_p)    \
    ((const RwUInt32 *)(((RwUInt8 *)(_p)) + _rpD3D9GeometryUsageFlagOffset))

#define RWD3D9WORLDSECTORGETUSAGEFLAG(_p)    \
    ((RwUInt32 *)(((RwUInt8 *)(_p)) + _rpD3D9WorldSectorUsageFlagOffset))

#define RWD3D9WORLDSECTORGETCONSTUSAGEFLAG(_p)    \
    ((const RwUInt32 *)(((RwUInt8 *)(_p)) + _rpD3D9WorldSectorUsageFlagOffset))

/*****************************************************************************
 local types
 */

/*****************************************************************************
 local global variables
 */
static RwInt32  _rpD3D9GeometryUsageFlagOffset = 0;
static RwInt32  _rpD3D9WorldSectorUsageFlagOffset = 0;

static RwModuleInfo     _RpUsageFlagsModule;

/*****************************************************************************
 _rpD3D9UsageFlagConst

 Purpose:

 On entry:
 On exit :
*/
static void *
_rpD3D9UsageFlagConst(void *object, RwInt32 offsetInObject,
                      RwInt32 sizeInObject __RWUNUSED__)
{
    RwUInt32 *usage;

    RWFUNCTION(RWSTRING("_rpD3D9UsageFlagConst"));

    usage = RWD3D9GETUSAGEFLAG(object, offsetInObject);

    *usage = 0;

    RWRETURN(object);
}


/*****************************************************************************
 _rpD3D9UsageFlagDest

 Purpose:

 On entry:
 On exit :
*/
static void *
_rpD3D9UsageFlagDest(void *object, RwInt32 offsetInObject,
                     RwInt32 sizeInObject __RWUNUSED__)
{
    RwUInt32 *usage;

    RWFUNCTION(RWSTRING("_rpD3D9UsageFlagDest"));

    usage = RWD3D9GETUSAGEFLAG(object, offsetInObject);

    *usage = 0;

    RWRETURN(object);
}

/*****************************************************************************
 _rpD3D9UsageFlagCopy

 Purpose:

 On entry:
 On exit :
*/
static void *
_rpD3D9UsageFlagCopy(void *dstObject,
                     const void *srcObject,
                     RwInt32 offsetInObject,
                     RwInt32 sizeInObject __RWUNUSED__ )
{
    RwUInt32 *dstfmt;
    const RwUInt32 *srcfmt;

    RWFUNCTION(RWSTRING("_rpD3D9UsageFlagCopy"));

    dstfmt = RWD3D9GETUSAGEFLAG(dstObject, offsetInObject);
    srcfmt = RWD3D9GETUSAGEFLAGCONST(srcObject, offsetInObject);

    *dstfmt = *srcfmt;

    RWRETURN(dstObject);
}

/*****************************************************************************
 _rpD3D9UsageFlagOpen

 Purpose:

 On entry:
 On exit :
*/
static void *
_rpD3D9UsageFlagOpen(void *object,
                RwInt32 offsetInObject,
                RwInt32 sizeInObject)
{
    RWFUNCTION(RWSTRING("_rpD3D9UsageFlagOpen"));

    /* one more module instance */
    _RpUsageFlagsModule.numInstances++;

    /* Init only in the first instance */
    if (1 == _RpUsageFlagsModule.numInstances)
    {

    }

    RWRETURN(object);
}


/*****************************************************************************
 _rpD3D9UsageFlagClose

 Purpose:

 On entry:
 On exit :
*/
static void *
_rpD3D9UsageFlagClose(void *object,
                 RwInt32 offsetInObject,
                 RwInt32 sizeInObject)
{
    RWFUNCTION(RWSTRING("_rpD3D9UsageFlagClose"));
    RWASSERT(_RpUsageFlagsModule.numInstances);

    /* one less module instance */
    _RpUsageFlagsModule.numInstances--;

    if (_RpUsageFlagsModule.numInstances == 0)
    {

    }

    RWRETURN(object);
}

/*****************************************************************************
 _rpD3D9UsageFlagPluginAttach

 Purpose:

 On entry:
 On exit :
*/
RwBool
_rpD3D9UsageFlagPluginAttach(void)
{
    RwInt32 rpD3D9EngineUsageOffset;

    RWFUNCTION(RWSTRING("_rpD3D9UsageFlagPluginAttach"));

    rpD3D9EngineUsageOffset =
        RwEngineRegisterPlugin(0,
                               rwID_RXWORLDDEVICEMODULE,
                               _rpD3D9UsageFlagOpen,
                               _rpD3D9UsageFlagClose);
    if (rpD3D9EngineUsageOffset < 0)
    {
        RWRETURN(FALSE);
    }

    /* GEOMETRY */
    _rpD3D9GeometryUsageFlagOffset =
        RpGeometryRegisterPlugin(sizeof(RwUInt32),
                                 rwID_RXWORLDDEVICEMODULE,
                                 _rpD3D9UsageFlagConst,
                                 _rpD3D9UsageFlagDest,
                                 _rpD3D9UsageFlagCopy);

    if (_rpD3D9GeometryUsageFlagOffset < 0)
    {
        RWRETURN(FALSE);
    }

    /* WORLD SECTOR */
    _rpD3D9WorldSectorUsageFlagOffset =
        RpWorldSectorRegisterPlugin(sizeof(RwUInt32),
                                    rwID_RXWORLDDEVICEMODULE,
                                    _rpD3D9UsageFlagConst,
                                    _rpD3D9UsageFlagDest,
                                    _rpD3D9UsageFlagCopy);
    if (_rpD3D9WorldSectorUsageFlagOffset < 0)
    {
        RWRETURN(FALSE);
    }

    RWRETURN(TRUE);
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*
 *                                                                           *
 *                             - API FUNCTIONS -                             *
 *                                                                           *
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

/**
 * \ingroup worldextensionsd3d9
 * \ref RpD3D9GeometrySetUsageFlags sets the usage flags for the \ref RpGeometry
 * passed as parameter.
 *
 * \param geometry Pointer to the \ref RpGeometry whose usage flags are
 * going to be set.
 * \param flags The \ref RpD3D9GeometryUsageFlag to be set.
 *
 * \see RpD3D9GeometryGetUsageFlags
 * \see RpD3D9WorldSectorSetUsageFlags
 * \see RpD3D9WorldSectorGetUsageFlags
 */
void
RpD3D9GeometrySetUsageFlags(RpGeometry *geometry,
                            RpD3D9GeometryUsageFlag flags)
{
    RwUInt32 *usage;

    RWAPIFUNCTION(RWSTRING("RpD3D9GeometrySetUsageFlags"));
    RWASSERT(_RpUsageFlagsModule.numInstances > 0);
    RWASSERT(geometry != NULL);

    usage = RWD3D9GEOMETRYGETUSAGEFLAG(geometry);

    *usage = flags;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RpD3D9GeometryGetUsageFlags gets the usage flags from the \ref RpGeometry
 * passed as parameter.
 *
 * \param geometry Const pointer to the \ref RpGeometry to get the usage flags from.
 *
 * \return The usage flags of the current geometry.
 *
 * \see RpD3D9GeometrySetUsageFlags
 * \see RpD3D9WorldSectorSetUsageFlags
 * \see RpD3D9WorldSectorGetUsageFlags
 */
RpD3D9GeometryUsageFlag
RpD3D9GeometryGetUsageFlags(const RpGeometry *geometry)
{
    const RwUInt32 *usage;

    RWAPIFUNCTION(RWSTRING("RpD3D9GeometryGetUsageFlags"));
    RWASSERT(_RpUsageFlagsModule.numInstances > 0);
    RWASSERT(geometry != NULL);

    usage = RWD3D9GEOMETRYGETCONSTUSAGEFLAG(geometry);

    RWRETURN(*usage);
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RpD3D9WorldSectorSetUsageFlags sets the usage flags for the \ref RpWorldSector
 * passed as parameter.
 *
 * \param sector Pointer to the \ref RpWorldSector whose usage flags are
 * going to be set.
 * \param flags The \ref RpD3D9WorldSectorUsageFlag to be set.
 *
 * \see RpD3D9GeometryGetUsageFlags
 * \see RpD3D9WorldSectorSetUsageFlags
 * \see RpD3D9WorldSectorGetUsageFlags
 */
void
RpD3D9WorldSectorSetUsageFlags(RpWorldSector *sector,
                               RpD3D9WorldSectorUsageFlag flags)
{
    RwUInt32 *usage;

    RWAPIFUNCTION(RWSTRING("RpD3D9WorldSectorSetUsageFlags"));
    RWASSERT(_RpUsageFlagsModule.numInstances > 0);
    RWASSERT(sector != NULL);

    usage = RWD3D9WORLDSECTORGETUSAGEFLAG(sector);

    *usage = flags;

    RWRETURNVOID();
}

/**
 * \ingroup worldextensionsd3d9
 * \ref RpD3D9WorldSectorGetUsageFlags gets the usage flags from the \ref RpWorldSector
 * passed as parameter.
 *
 * \param sector Const pointer to the \ref RpWorldSector to get the usage flags from.
 *
 * \return The usage flags of the current world sector.
 *
 * \see RpD3D9GeometrySetUsageFlags
 * \see RpD3D9WorldSectorSetUsageFlags
 * \see RpD3D9WorldSectorGetUsageFlags
 */
RpD3D9WorldSectorUsageFlag
RpD3D9WorldSectorGetUsageFlags(const RpWorldSector *sector)
{
    const RwUInt32 *usage;

    RWAPIFUNCTION(RWSTRING("RpD3D9WorldSectorGetUsageFlags"));
    RWASSERT(_RpUsageFlagsModule.numInstances > 0);
    RWASSERT(sector != NULL);

    usage = RWD3D9WORLDSECTORGETCONSTUSAGEFLAG(sector);

    RWRETURN(*usage);
}
