/*
 * Bounding box handling
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/**
 * \ingroup rwbbox
 * \page rwbboxoverview RwBBox Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection bboxoverview Overview
 *
 * RenderWare Graphics Bounding Box datatype.
 * This object defines a bounding-box. It is used in a number
 * of RenderWare objects to define sizes, boundaries or collision-detection zones.
 * Aside from basic initialization, functionality is also provided for resizing the
 * bounding-box to enclose an arbitrary point.
 *
 * More information can be found in the \e Fundamentals chapter of the User Guide.
 *
*/




/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"
#include "bavector.h"
#include "badebug.h"
#include "bamemory.h"

/* Assumes we have a statically bound library */

#include "babbox.h"

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

/****************************************************************************
 Functions
 */

/**
 * \ingroup rwbbox
 * \ref RwBBoxCalculate is used to calculate the axis-aligned
 * bounding-box of the specified array of 3D points.
 *
 * \param boundBox  Pointer to the bounding-box.
 * \param verts  An array of vertex positions.
 * \param numVerts  A RwInt32 value equal to the number of vertices in the
 * array.
 *
 * \return Returns a pointer to the bounding-box if successful, or NULL if
 * there is an error.
 *
 * \see RwBBoxAddPoint
 * \see RwBBoxContainsPoint
 * \see RwBBoxInitialize
 *
 */

RwBBox *
RwBBoxCalculate(RwBBox *boundBox, const RwV3d *verts, RwInt32 numVerts)
{
    RWAPIFUNCTION(RWSTRING("RwBBoxCalculate"));
    RWASSERT(boundBox);
    RWASSERT(verts);
    RWASSERT(numVerts > 0);

    RwV3dAssign(&boundBox->inf, verts);
    RwV3dAssign(&boundBox->sup, verts);

    verts++;
    numVerts--;

    while (numVerts--)
    {
        if (boundBox->inf.x > verts->x)
        {
            boundBox->inf.x = verts->x;
        }
        if (boundBox->inf.y > verts->y)
        {
            boundBox->inf.y = verts->y;
        }
        if (boundBox->inf.z > verts->z)
        {
            boundBox->inf.z = verts->z;
        }

        if (boundBox->sup.x < verts->x)
        {
            boundBox->sup.x = verts->x;
        }
        if (boundBox->sup.y < verts->y)
        {
            boundBox->sup.y = verts->y;
        }
        if (boundBox->sup.z < verts->z)
        {
            boundBox->sup.z = verts->z;
        }

        verts++;
    }

    RWRETURN(boundBox);
}

/**
 * \ingroup rwbbox
 * \ref RwBBoxInitialize is used to initialize a bounding-box. The
 * bounding-box is initialized with both the infimum and supremum vertices
 * set to the supplied vertex. A bounding-box normally has the infimum and
 * supremum vertices set to different values, otherwise it is just a point.
 * Therefore, initialization is a two-stage process actually completed by a
 * making a subsequent call to the \ref RwBBoxAddPoint function.
 *
 * (This two-stage system proves its usefulness when calculating bounding-boxes
 * that contain an arbitrary number of vectors.)
 *
 * \param boundBox  Pointer to the bounding-box structure to fill in.
 * \param vertex  Pointer to the first vertex to initialize with.
 *
 * \return Returns a pointer to the bounding-box if successful, or NULL if
 * there is an error.
 *
 * \see RwBBoxAddPoint
 * \see RwBBoxCalculate
 * \see RwBBoxContainsPoint
 *
 */

RwBBox *
RwBBoxInitialize(RwBBox *boundBox, const RwV3d *vertex)
{
    RWAPIFUNCTION(RWSTRING("RwBBoxInitialize"));
    RWASSERT(boundBox);
    RWASSERT(vertex);

    RwV3dAssign(&boundBox->inf, vertex);
    RwV3dAssign(&boundBox->sup, vertex);

    RWRETURN(boundBox);
}

/**
 * \ingroup rwbbox
 * \ref RwBBoxAddPoint is used to adjust the specified bounding-box so
 * that it contains the specified vertex. The bounding-box must have been
 * previously initialized with a single vertex or calculated from an array of
 * vertices.
 *
 * \param boundBox  Pointer to the bounding-box.
 * \param vertex  Pointer to a RwV3d value containing the position of the vertex.
 *
 * \return Returns a pointer to the bounding-box if successful, or NULL if
 * there is an error.
 *
 * \see RwBBoxCalculate
 * \see RwBBoxContainsPoint
 * \see RwBBoxInitialize
 *
 */

RwBBox *
RwBBoxAddPoint(RwBBox *boundBox, const RwV3d *vertex)
{
    RWAPIFUNCTION(RWSTRING("RwBBoxAddPoint"));
    RWASSERT(boundBox);
    RWASSERT(vertex);

    if (boundBox->inf.x > vertex->x)
    {
        boundBox->inf.x = vertex->x;
    }
    if (boundBox->inf.y > vertex->y)
    {
        boundBox->inf.y = vertex->y;
    }
    if (boundBox->inf.z > vertex->z)
    {
        boundBox->inf.z = vertex->z;
    }

    if (boundBox->sup.x < vertex->x)
    {
        boundBox->sup.x = vertex->x;
    }
    if (boundBox->sup.y < vertex->y)
    {
        boundBox->sup.y = vertex->y;
    }
    if (boundBox->sup.z < vertex->z)
    {
        boundBox->sup.z = vertex->z;
    }

    RWRETURN(boundBox);
}

/**
 * \ingroup rwbbox
 * \ref RwBBoxContainsPoint is used to test whether the specified vertex
 * falls within the given bounding-box. Points lying exactly on the box’s
 * boundary are considered to be inside the box.
 *
 * \param boundBox  Pointer to the bounding-box.
 * \param vertex  Pointer to a RwV3d value containing the position of the vertex.
 *
 * \return Returns TRUE if vertex lies inside box or FALSE otherwise, or if
 * there is an error.
 *
 * \see RwBBoxAddPoint
 * \see RwBBoxCalculate
 * \see RwBBoxInitialize
 *
 */

RwBool
RwBBoxContainsPoint(const RwBBox *boundBox, const RwV3d *vertex)
{
    RWAPIFUNCTION(RWSTRING("RwBBoxContainsPoint"));
    RWASSERT(boundBox);
    RWASSERT(vertex);

    if (boundBox->inf.x > vertex->x)
    {
        RWRETURN(FALSE);
    }
    if (boundBox->inf.y > vertex->y)
    {
        RWRETURN(FALSE);
    }
    if (boundBox->inf.z > vertex->z)
    {
        RWRETURN(FALSE);
    }

    if (boundBox->sup.x < vertex->x)
    {
        RWRETURN(FALSE);
    }
    if (boundBox->sup.y < vertex->y)
    {
        RWRETURN(FALSE);
    }
    if (boundBox->sup.z < vertex->z)
    {
        RWRETURN(FALSE);
    }

    /* must be within */
    RWRETURN(TRUE);
}

