
/****************************************************************************
 *                                                                          *
 * module : native.c                                                        *
 *                                                                          *
 * purpose: Write/Read native data                                          *
 *                                                                          *
 ****************************************************************************/

/****************************************************************************
 includes
 */

#include "rwcore.h"
#include "baworld.h"
#include "native.h"

/****************************************************************************
 Local defines
 */

/****************************************************************************
 Local types
 */

/****************************************************************************
 Externed globals
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                   GEOMETRY

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

RwStream *
_rpGeometryNativeWrite(RwStream *stream, const RpGeometry *geometry)
{
    RWFUNCTION(RWSTRING("_rpGeometryNativeWrite"));
    RWASSERT(stream);
    RWASSERT(geometry);

    if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
    {
        /* Write some native data */
    }

    RWRETURN(stream);
}

RpGeometry *
_rpGeometryNativeRead(RwStream *__RWUNUSEDRELEASE__ stream,
                      RpGeometry *geometry)
{
    RWFUNCTION(RWSTRING("_rpGeometryNativeRead"));
    RWASSERT(stream);
    RWASSERT(geometry);

    RWRETURN(geometry);
}

RwInt32
_rpGeometryNativeSize(const RpGeometry *geometry)
{
    RwUInt32    size = 0;

    RWFUNCTION(RWSTRING("_rpGeometryNativeSize"));
    RWASSERT(geometry);

    if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
    {
        /* Calculat the native data size */
    }

    RWRETURN(size);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                 WORLD SECTOR

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

RwStream *
_rpWorldSectorNativeWrite(RwStream *stream, const RpWorldSector *sector)
{
    RpWorld     *world;

    RWFUNCTION(RWSTRING("_rpWorldSectorNativeWrite"));

    world = RpWorldSectorGetWorld(sector);
    if (rpWORLDNATIVE & RpWorldGetFlags(world))
    {
        /* Write some native data */
    }

    RWRETURN(stream);
}

RpWorldSector *
_rpWorldSectorNativeRead(RwStream *stream __RWUNUSED__,
                         RpWorldSector *sector)
{
    RWFUNCTION(RWSTRING("_rpWorldSectorNativeRead"));

    RWRETURN(sector);
}

RwInt32
_rpWorldSectorNativeSize(const RpWorldSector *sector)
{
    RwUInt32    size = 0;
    RpWorld     *world;

    RWFUNCTION(RWSTRING("_rpWorldSectorNativeSize"));

    /*
     * Calculate the native data size
     */
    world = RpWorldSectorGetWorld(sector);
    if (rpWORLDNATIVE & RpWorldGetFlags(world))
    {
    }

    RWRETURN(size);
}
