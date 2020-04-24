/**
 * \ingroup rpmorphtarget
 * \page rpmorphtargetoverview RpMorphTarget Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection morphtargetoverview Overview
 *
 * This object defines a keyframe for animation. When a model with multiple
 * keyframe animation is loaded or created, the keyframes are stored in Morph
 * Targets. During playback, RenderWare Graphics will morph the 3D model geometry over
 * time from one Morph Target to the next, until the last in the sequence has
 * been reached, at which point the cycle is reset. (An optional callback function
 * can be triggered at this point.)
 *
 * When animating between Morph Targets, RenderWare creates an interpolated model,
 * the data for which lives in an RpInterpolator object. If the animation cycle is
 * exactly on a Morph Target, the data from that object is simply copied to an
 * RpInterpolator object. All Geometry objects have at least one RpInterpolator object,
 * regardless of whether any animation data exists.
 *
*/


/**
 * \ingroup rpgeometry
 * \page rpgeometryoverview RpGeometry Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection geometryoverview Overview
 *
 * This object is the primary container object for dynamic model data.
 *
 * The data itself is stored as lists of Triangles (RpTriangle) stored in a
 * Morph Target (RpMorphTarget). Each such Triangle object also contains a
 * reference to a Material (RpMaterial) object, which defines that triangle's appearance.
 *
 * During scene generation process, RenderWare Graphics will use the Geometry data to
 * generate Mesh (RpMesh) data for rendering. A Geometry object cannot be directly
 * liked to a Frame as there is no storage for these. Instead, you should create an
 * Atomic with a reference the Geometry, then link that Atomic to a Frame.
 *
 * Further information is available in the \e Dynamic \e Models chapter of the User Guide.
 *
*/


/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include <rwcore.h>
#include "baclump.h"
#include "bamateri.h"
#include "bamatlst.h"
#include "bameshop.h"
#include "baworld.h"
#include "bageomet.h"

#include "bapipew.h"

/****************************************************************************
 Local Types
 */

/* Binary Representation */

typedef struct _rpTriangle _rpTriangle;
struct _rpTriangle
{
    RwUInt32            vertex01; /* V0 index in top 16 bits, V1 index in bottom 16 bits */
    RwUInt32            vertex2Mat; /* V2 index in top 16 bits, Material index in bottom 16 bit */
};
#define UINT32PACK(H,L)     (((H) & 0xffff)<<16)|((L) & 0xffff)

typedef struct _rpMorphTarget _rpMorphTarget;
struct _rpMorphTarget
{
    RwSphere            boundingSphere;
    RwBool              pointsPresent;
    RwBool              normalsPresent;
};

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWSTREAMTYPE(_type)                                              \
  ( ( rwNASTREAM == (_type) ) ? "rwNASTREAM" :                           \
    ( ( rwSTREAMFILE == (_type) ) ? "rwSTREAMFILE" :                     \
      ( ( rwSTREAMFILENAME == (_type) ) ? "rwSTREAMFILENAME" :           \
        ( ( rwSTREAMMEMORY == (_type) ) ? "rwSTREAMMEMORY" : "Unknown" ) \
        ) ) )

#define RPGEOMETRYGLOBAL(var) (RWPLUGINOFFSET(rpGeometryGlobals, RwEngineInstance, geometryModule.globalsOffset)->var)

/*
 * Geometry format specifier (for RpGeometryCreate):-
 *
 *  0x00nn00gg
 *  nn = num tex coords
 *  gg = flags
 *
 * A gap is left for possible extension of the 8-bit flags field to
 * 16-bit. See also bageomet.h: rpGEOMETRYTEXCOORDSETS(_num)
 *
 * To determine number of texture coordinate sets, the 'num tex coords'
 * field takes precedence, but if zero, we use the backward compatibility
 * flags.
 */
#define GeometryFormatGetFlagsMacro(_fmt)  ((_fmt) & rpGEOMETRYFLAGSMASK)

#define GeometryFormatGetNumTexCoordSetsMacro(_fmt)         \
    (((_fmt) & 0xff0000) ? (((_fmt) & 0xff0000)>>16) :      \
        (((_fmt) & rpGEOMETRYTEXTURED2) ? 2 :               \
            (((_fmt) & rpGEOMETRYTEXTURED) ? 1 : 0)))


/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwPluginRegistry geometryTKList =
    { sizeof(RpGeometry),
      sizeof(RpGeometry),
      0,
      0,
      (RwPluginRegEntry *) NULL,
      (RwPluginRegEntry *) NULL };
static RwModuleInfo geometryModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                        Slave functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static RwBool
GeometryAnnihilate(RpGeometry *geometry)
{
    RWFUNCTION(RWSTRING("GeometryAnnihilate"));

    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(geometry->refCount <= 0);

    /* Temporarily bump up reference count to avoid assertion failures */
    geometry->refCount++;

    RWASSERT(geometry->refCount > 0);

    /* Lock it so it can be destroyed */
    RpGeometryLock(geometry, (rpGEOMETRYLOCKALL));

    /* DeInitialize the plugin memory */
    rwPluginRegistryDeInitObject(&geometryTKList, geometry);

    /* destroy morphTargets */
    if (geometry->morphTarget)
    {
        RwFree(geometry->morphTarget);
        geometry->morphTarget = (RpMorphTarget *)NULL;
    }

    _rpMaterialListDeinitialize(&geometry->matList);

    /* Reinstate reference count */
    --geometry->refCount;
    RWASSERT(geometry->refCount <= 0);

    RwFree(geometry);

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Opening and closing

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rpGeometryOpen

 On entry   :
 On exit    : FALSE on failure
 */

void               *
_rpGeometryOpen(void *instance, RwInt32 offset,
                RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpGeometryOpen"));

    /* Grab the global data offset (same for all instances) */
    geometryModule.globalsOffset = offset;

    /* One more module instance */
    geometryModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rpGeometryClose

 On entry   :
 On exit    : FALSE on failure
 */

void               *
_rpGeometryClose(void *instance,
                 RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rpGeometryClose"));

    /* One less module instance */
    geometryModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

RwPluginRegEntry   *
_rpGeometryGetTKListFirstRegEntry(void)
{
    RWFUNCTION("_rpGeometryGetTKListFirstRegEntry");

    RWRETURN(geometryTKList.firstRegEntry);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                        Geometry functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryTransform is used to apply the specified transformation
 * matrix to the given geometry. The transformation is applied equally to all
 * morph targets defined in the geometry and for each morph target transforms both
 * the vertex positions and vertex normals. All morph target bounding spheres are
 * also recalculated.
 *
 * Note that the transformation modifies the morph target data
 * and is permanent. Note also that geometry locking and unlocking is performed,
 * as appropriate, before and after applying the transformation.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 * \param matrix  Pointer to the matrix describing the transformation.
 *
 * \return Returns pointer to the geometry if successsful or NULL if there
 * is an error.
 *
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpGeometryCreate
 * \see RpMorphTarget
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryGetTriangles
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometryTransform(RpGeometry * geometry, const RwMatrix * matrix)
{
    RwInt32             i, j;

    RWAPIFUNCTION(RWSTRING("RpGeometryTransform"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWASSERT(matrix);

    /* Lock it so that it can be updated -
     * this will do the state counter thing too */
    if (RpGeometryLock(geometry, (rpGEOMETRYLOCKVERTICES |
                                  rpGEOMETRYLOCKNORMALS)))
    {
        for (i = 0; i < geometry->numMorphTargets; i++)
        {
            RpMorphTarget      *morphTarget = &geometry->morphTarget[i];

            RwV3d              *normals = morphTarget->normals;
            RwV3d              *vpVert = morphTarget->verts;

            /* Transform it verts to possibly normals too */
            RwV3dTransformPoints(vpVert, vpVert,
                                 geometry->numVertices, matrix);
            if (normals)
            {
                RwV3dTransformVectors(normals, normals,
                                      geometry->numVertices, matrix);

                /* And normalise the normals */
                j = geometry->numVertices;
                while (j--)
                {
                    _rwV3dNormalize(normals, normals);
                    normals++;
                }
            }

            /* Set the radius */
            RpMorphTargetCalcBoundingSphere(morphTarget, &morphTarget->boundingSphere);
        }

        /* Can put it back into a device dependent form */
        if (!RpGeometryUnlock(geometry))
        {
            RpGeometryDestroy(geometry);
            RWRETURN((RpGeometry *)NULL);
        }

        RWASSERT(geometry);
        RWASSERTISTYPE(geometry, rpGEOMETRY);
        RWASSERT(0 < geometry->refCount);

        RWRETURN(geometry);
    }

    /* Lock failure */
    RWRETURN((RpGeometry *)NULL);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryCreateSpace is used to create an empty geometry
 * containing no vertices or polygons and one morph target with a bounding
 * sphere of the specified radius, centered at the origin.
 *
 * Note that the returned geometry is unlocked.
 *
 * This function is typically used to create 'invisible' surfaces for use
 * in collision simulations.
 *
 * The world plugin must be attached before using this function.
 *
 * \param radius  A RwReal value equal to the radius of the geometry's bounding sphere.
 *
 * \return Returns pointer to the geometry if successful or NULL if there
 * is an error.
 *
 * \see RpGeometryCreate
 * \see RpGeometryDestroy
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometryCreateSpace(RwReal radius)
{
    RpGeometry         *geometry;

    RWAPIFUNCTION(RWSTRING("RpGeometryCreateSpace"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(radius >= (RwReal) (0.0));

    geometry = RpGeometryCreate(0, 0, 0);
    if (geometry)
    {
        RpMorphTarget      *morphTarget = &geometry->morphTarget[0];

        morphTarget->boundingSphere.center.x = (RwReal) (0.0);
        morphTarget->boundingSphere.center.y = (RwReal) (0.0);
        morphTarget->boundingSphere.center.z = (RwReal) (0.0);
        morphTarget->boundingSphere.radius = radius;

		if (!RpGeometryUnlock(geometry))
		{
			RpGeometryDestroy(geometry);
			RWRETURN((RpGeometry *)NULL);
		}

		RWASSERT(geometry);
		RWASSERTISTYPE(geometry, rpGEOMETRY);
		RWASSERT(geometry->refCount);

	}

    RWRETURN(geometry);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                        Geometry Building functions

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpmorphtarget
 * \ref RpMorphTargetSetBoundingSphere is used to modify the specified
 * morph target's bounding sphere. This function is typically used in conjunction
 * with \ref RpMorphTargetCalcBoundingSphere to calculate and store the center
 * and radius of a sphere enclosing all the vertices in the given morph target.
 * This bounding sphere is used in collision and pick testing; therefore, if
 * the given sphere has not been calculated using \ref RpMorphTargetCalcBoundingSphere
 * incorrect results will occur.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param morphTarget  Pointer to the morph target.
 * \param boundingSphere  Pointer to a RwSphere value describing the new bounding sphere.
 *
 * \return Returns pointer to the morph target if successful or NULL if there is
 * an error.
 *
 * \see RpMorphTargetCalcBoundingSphere
 * \see RpmorphTargetGetBoundingSphere
 * \see RpWorldPluginAttach
 *
 */
RpMorphTarget *
RpMorphTargetSetBoundingSphere(RpMorphTarget *morphTarget,
                               const RwSphere *boundingSphere)
{
    RWAPIFUNCTION(RWSTRING("RpMorphTargetSetBoundingSphere"));

    RWASSERT(geometryModule.numInstances);
    RWASSERT(morphTarget);
    RWASSERT(boundingSphere);
    RWASSERT(boundingSphere->radius >= (RwReal)(0.0));

    RWRETURN(RpMorphTargetSetBoundingSphereMacro(morphTarget, boundingSphere));
}

/**
 * \ingroup rpmorphtarget
 * \ref RpMorphTargetGetBoundingSphere is used to get the bounding sphere
 * from a morph target.  The bounding sphere is a sphere that encloses all the
 * vertices in the given morph target.  This bounding sphere is used in collision
 * and pick testing.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param morphTarget  Pointer to the morph target.
 *
 * \return Returns pointer to a RwSphere value if successful or NULL if
 * there is an error.
 *
 * \see RpMorphTargetCalcBoundingSphere
 * \see RpMorphTargetSetBoundingSphere
 * \see RpWorldPluginAttach
 *
 */
RwSphere *
RpMorphTargetGetBoundingSphere(RpMorphTarget *morphTarget)
{
    RWAPIFUNCTION(RWSTRING("RpMorphTargetGetBoundingSphere"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(morphTarget);

    RWRETURN(RpMorphTargetGetBoundingSphereMacro(morphTarget));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpmorphtarget
 * \ref RpMorphTargetCalcBoundingSphere is used to calculate the specified
 * morph target's optimal bounding sphere. The bounding sphere is used in collision
 * and picking tests at the atomic and clump level. Note that this function
 * does not modify the center and radius of the bounding sphere. Use the
 * API function \ref RpMorphTargetSetBoundingSphere to change the morph target's
 * actual bounding sphere.
 *
 * The world plugin must be attached before using this function.
 *
 * \param morphTarget  Pointer to the morph target.
 * \param boundingSphere  Pointer to a RwSphere value that will receive the bounding sphere.
 *
 * \return Returns pointer to the morph target if successful or NULL if there is
 * an error.
 *
 * \see RpMorphTargetSetBoundingSphere
 * \see RpMorphTargetGetBoundingSphere
 * \see RpWorldPluginAttach
 *
 */
const RpMorphTarget *
RpMorphTargetCalcBoundingSphere(const RpMorphTarget * morphTarget,
                                RwSphere * boundingSphere)
{
    RpGeometry         *geometry;
    RwV3d              *vert;
    RwInt32             numVerts;
    RwSphere            sphere;
    RwReal              sphere_radius = (RwReal) (0);
    RwBBox              boundBox;

    RWAPIFUNCTION(RWSTRING("RpMorphTargetCalcBoundingSphere"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(morphTarget);
    RWASSERT(boundingSphere);

    geometry = morphTarget->parentGeom;
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    /* Find the median (sort of) by doing a bounding box,
     * then using the center of the box.
     */
    numVerts = geometry->numVertices;
    RwBBoxCalculate(&boundBox, morphTarget->verts, numVerts);
    RwV3dAdd(&sphere.center, &boundBox.inf, &boundBox.sup);
    RwV3dScale(&sphere.center, &sphere.center, (RwReal) (0.5));

    vert = morphTarget->verts;

    /* Find the radius (we do this in square space */
    while (numVerts--)
    {
        RwReal              nDist;
        RwV3d               vTmp;

        RwV3dSubMacro(&vTmp, vert, &sphere.center);
        nDist = RwV3dDotProduct(&vTmp, &vTmp);

        if (nDist > sphere_radius)
        {
            sphere_radius = nDist;
        }

        vert++;
    }

    /* Now do the root */
    if (sphere_radius > (RwReal) (0))
    {
        rwSqrt(&sphere_radius, sphere_radius);
    }

    /* Add 0.1% for numerical inaccuracy */
    sphere.radius = sphere_radius * ((RwReal) (1.001));

    /* Save off result */
    (*boundingSphere) = sphere;

    RWRETURN(morphTarget);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryAddMorphTargets is used to add N new morph target to the
 * specified geometry. Space is allocated for the morph targets vertices (and
 * vertex normals if the geometry requires them) but their values are
 * completely undefined.
 *
 * The new morph targets bounding spheres are also undefined. The spheres
 * must be determined and set using \ref RpMorphTargetCalcBoundingSphere and
 * \ref RpMorphTargetSetBoundingSphere before they are used.
 *
 * All morph targets in the geometry have the same number of vertices and
 * triangle topology (vertex connectivity).
 *
 * The world plugin must be attached before using this function.
 *
 * \note All calls to \ref RpGeometryAddMorphTarget or
 * \ref RpGeometryAddMorphTargets invalidate any pointers to morph
 * targets returned by earlier calls to \ref RpGeometryGetMorphTarget.
 *
 * \param geometry  Pointer to the geometry.
 * \param mtcount   RwInt32 count of how many new morph targets.
 *
 * \return Returns a RwInt32 value equal to the morph target's index (first morph
 * target has index 0) or -1 if there is an error.
 *
 * \see RpGeometryCreate
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryGetNumVertices
 * \see RpMorphTargetGetVertices
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryRemoveMorphTarget
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryAddMorphTargets(RpGeometry *geometry, RwInt32 mtcount)
{
    RwInt32 i;
    RwUInt32            mtsize,bytes;
    RpMorphTarget      *morphTarget;
    RwV3d               *vertexData;
    RWAPIFUNCTION(RWSTRING("RpGeometryAddMorphTargets"));

    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(mtcount > 0);

    if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
    {
        mtsize = sizeof(RpMorphTarget);
    }
    else
    {
        mtsize = sizeof(RpMorphTarget) + sizeof(RwV3d) * geometry->numVertices;

        if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
        {
            mtsize += sizeof(RwV3d) * geometry->numVertices;
        }
    }

    bytes = mtsize * (geometry->numMorphTargets + mtcount);

    /* Is it a realloc or a first time alloc? */
    if (geometry->morphTarget)
    {
        RwUInt8 *src,*dst;
        RwInt32 len;

        morphTarget = (RpMorphTarget *)RwRealloc(geometry->morphTarget, bytes,
                                                 rwID_GEOMETRY | rwMEMHINTDUR_EVENT);
        if (!morphTarget)
        {
            /* Failed to allocate memory for the new morph target array */
            RWERROR((E_RW_NOMEM, (bytes)));
            RWRETURN(-1);
        }

        /* we want the MorphTarget structures at the beginning so we need to
           open a gap for mtcount MorphTargets */
        src = (RwUInt8 *)morphTarget + (mtsize * geometry->numMorphTargets) - 1;
        dst = src + (sizeof(RpMorphTarget) * mtcount);
        len = (mtsize * geometry->numMorphTargets) - (sizeof(RpMorphTarget) * geometry->numMorphTargets);
        while (len--)
        {
            *dst-- = *src--;
        }
    }
    else
    {
        morphTarget = (RpMorphTarget *)RwMalloc(bytes, rwID_GEOMETRY | rwMEMHINTDUR_EVENT);
        if (!morphTarget)
        {
            /* Failed to allocate memory for the new morph target array */
            RWERROR((E_RW_NOMEM, (bytes)));
            RWRETURN(-1);
        }
    }

    /* Add the extra frames */
    geometry->numMorphTargets += mtcount;

    /* Memory allocation is successful, so install */
    geometry->morphTarget = morphTarget;

    /* setup ALL pointers */
    vertexData = (RwV3d *)((RwUInt8 *)morphTarget +
                           (sizeof(RpMorphTarget) *
                            geometry->numMorphTargets));

    for (i = 0; i < geometry->numMorphTargets; i++)
    {
        RpMorphTarget *aMorph = &geometry->morphTarget[i];

        aMorph->verts = (RwV3d *)NULL;
        aMorph->normals = (RwV3d *)NULL;

        if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
        {
            if (geometry->numVertices)
            {
                aMorph->verts = vertexData;
                vertexData += geometry->numVertices;

                if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
                {
                    aMorph->normals = vertexData;
                    vertexData += geometry->numVertices;
                }
            }
        }
    }

    /* just initialize new ones */
    for (i=geometry->numMorphTargets-mtcount; i<geometry->numMorphTargets; i++)
    {
        RpMorphTarget *aMorph = &geometry->morphTarget[i];

        /* Initialize */
        aMorph->boundingSphere.center.x = (RwReal) (0);
        aMorph->boundingSphere.center.y = (RwReal) (0);
        aMorph->boundingSphere.center.z = (RwReal) (0);
        aMorph->boundingSphere.radius = (RwReal) (0);
        aMorph->parentGeom = geometry;
    }

    /* Done */
    RWRETURN(geometry->numMorphTargets - mtcount);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryAddMorphTarget is used to add a new morph target to the
 * specified geometry. Space is allocated for the morph target's vertices
 * (and vertex normals if the geometry requires them) but their values are
 * completely undefined.
 *
 * The new morph target's bounding sphere is also undefined. The sphere
 * must be determined and set using \ref RpMorphTargetCalcBoundingSphere
 * and \ref RpMorphTargetSetBoundingSphere before it is used.
 *
 * All morph targets in the geometry have the same number of vertices and
 * triangle topology (vertex connectivity).
 *
 * The world plugin must be attached before using this function.
 *
 * \note All calls to \ref RpGeometryAddMorphTarget or
 * \ref RpGeometryAddMorphTargets invalidate any pointers to morph
 * targets returned by earlier calls to \ref RpGeometryGetMorphTarget.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns a RwInt32 value equal to the morph target's index (first morph
 * target has index 0) or -1 if there is an error.
 *
 * \see RpGeometryCreate
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryGetNumVertices
 * \see RpMorphTargetGetVertices
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryRemoveMorphTarget
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryAddMorphTarget(RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryAddMorphTarget"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryAddMorphTargets(geometry, 1));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryRemoveMorphTarget is used to destroy the morph target
 * with the specified index in the given geometry.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the morph targets.
 * \param morphTargetIndex  A RwInt32 value specifying the index of the morph target
 * (first morph target has index 0).
 *
 * \return Returns pointer to the geometry if successful, or FALSE if
 * there is an error.
 *
 * \see RpGeometryAddMorphTarget
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometryRemoveMorphTarget(RpGeometry * geometry,
                            RwInt32 morphTargetIndex)
{
    RwInt32 i,numMorphs;
    RwUInt32            mtsize,bytes;
    RpMorphTarget      *morphTarget;
    RwV3d               *vertexData;
    RwUInt8             *srcData,*dstData;

    RWAPIFUNCTION(RWSTRING("RpGeometryRemoveMorphTarget"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(morphTargetIndex >= 0);
    RWASSERT(morphTargetIndex < geometry->numMorphTargets);

    mtsize = sizeof(RpMorphTarget) + sizeof(RwV3d) * geometry->numVertices;
    if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
    {
        mtsize += sizeof(RwV3d) * geometry->numVertices;
    }
    bytes = mtsize * (geometry->numMorphTargets - 1);

    morphTarget = (RpMorphTarget *)RwMalloc(bytes, rwID_GEOMETRY | rwMEMHINTDUR_EVENT);
    if (!morphTarget)
    {
        /* Failed to allocate memory for the new morph target array */
        RWERROR((E_RW_NOMEM, (bytes)));
        RWRETURN((RpGeometry *)NULL);
    }

    /* copy across headers */
    numMorphs = 0;
    for (i=0; i<geometry->numMorphTargets; i++)
    {
        if (i != morphTargetIndex)
        {
            morphTarget[numMorphs] = geometry->morphTarget[i];
            numMorphs++;
        }
    }

    /* copy across vertexData */
    srcData = (RwUInt8 *)geometry->morphTarget + (sizeof(RpMorphTarget) * geometry->numMorphTargets);
    dstData = (RwUInt8 *)morphTarget + (sizeof(RpMorphTarget) * numMorphs);
    for (i=0; i<geometry->numMorphTargets; i++)
    {
        if (i != morphTargetIndex)
        {
            memcpy(dstData, srcData, mtsize - sizeof(RpMorphTarget));
            dstData += mtsize - sizeof(RpMorphTarget);
        }

        srcData += mtsize - sizeof(RpMorphTarget);
    }

    RwFree(geometry->morphTarget);
    geometry->morphTarget = morphTarget;
    geometry->numMorphTargets--;

    /* setup ALL pointers */
    vertexData = (RwV3d *)((RwUInt8 *)morphTarget + (sizeof(RpMorphTarget) * geometry->numMorphTargets));
    for (i=0; i<geometry->numMorphTargets; i++)
    {
        RpMorphTarget *aMorph = &geometry->morphTarget[i];

        aMorph->verts = (RwV3d *)NULL;
        aMorph->normals = (RwV3d *)NULL;
        if (geometry->numVertices)
        {
            aMorph->verts = vertexData;
            vertexData += geometry->numVertices;
            if (rpGEOMETRYNORMALS & RpGeometryGetFlags(geometry))
            {
                aMorph->normals = vertexData;
                vertexData += geometry->numVertices;
            }
        }
    }

    RWRETURN(geometry);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetNumMorphTargets is used to determine the number of
 * morph targets defined in the specified geometry. All geometries have at least
 * one morph target allocated to them at creation time (though it may not
 * necessarily be initialized).
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns a RwInt32 value equal to the number of morph targets if
 * successful or -1 if there is an error.
 *
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryAddMorphTarget
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryRemoveMorphTarget
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryGetNumMorphTargets(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetNumMorphTargets"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetNumMorphTargetsMacro(geometry));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetMorphTarget is used to retrieve the morph target
 * with the specified index from the given geometry.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \note All calls to \ref RpGeometryAddMorphTarget or
 * \ref RpGeometryAddMorphTargets invalidate any pointers to morph
 * targets returned by earlier calls to \ref RpGeometryGetMorphTarget.
 *
 * \param geometry  Pointer to the geometry containing the morph targets.
 * \param morphTargetIndex  A RwInt32 value equal to the index of the morph
 * target (first morph target has index 0).
 *
 * \return Returns pointer to the morph target if successful or NULL
 * if there is an error or if the morph target does not exist.
 *
 * \see RpGeometryAddMorphTarget
 * \see RpMorphTargetGetVertices
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetNumMorphTargets
 * \see RpGeometryRemoveMorphTarget
 * \see RpWorldPluginAttach
 *
 */
RpMorphTarget *
RpGeometryGetMorphTarget(const RpGeometry *geometry,
                         RwInt32 morphTargetIndex)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetMorphTarget"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(morphTargetIndex >= 0);
    RWASSERT(morphTargetIndex < geometry->numMorphTargets);

    /* Success */
    RWRETURN(RpGeometryGetMorphTargetMacro(geometry, morphTargetIndex));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetPreLightColors is used to retrieve the array of
 * pre-lighting colors from the specified geometry. The array only exists if
 * the geometry has been created using the rpGEOMETRYPRELIT flag. The
 * pre-lighting colors reside within the geometry's topology, at one color per
 * vertex, and are therefore shared between all morph targets. Use this function
 * to initialize or redefine the pre-lighting colors.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the pre-lighting colors.
 *
 * \return Returns pointer to an array of RwRGBA values if successful or NULL
 * if there is an error.
 *
 * \see RpGeometryCreate
 * \see RpGeometryGetFlags
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetVertexTexCoords
 * \see RpMorphTargetGetVertices
 * \see RpMorphTargetGetVertexNormals
 * \see RpWorldPluginAttach
 *
 */
RwRGBA *
RpGeometryGetPreLightColors(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetPreLightColors"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetPreLightColorsMacro(geometry));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetVertexTexCoords is used to retrieve a particular set
 * of vertex texture coordinates, if any exist in the specified geometry.
 * The number of sets available may be determined using
 * \ref RpGeometryGetNumTexCoordSets, and is fixed when the geometry
 * is created (see \ref RpGeometryCreate).
 *
 * Vertex texture coordinates reside within the geometry's topology, with
 * each set having a (u,v) coordinate pair per vertex, and are therefore shared
 * between all morph targets. Use this function to initialize or redefine the
 * vertex texture coordinates.
 *
 * There is a one-to-one correspondence between the vertex texture coordinates
 * array and the vertex list array defined in the morph targets.
 *
 * The geometry must be locked before modifying the vertex texture coordinates.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the vertex texture coordinates.
 * \param uvIndex   The index of the desired texture coordinate set.
 *
 * \return Returns a pointer to an array of RwTexCoords if successful or NULL
 * if the specified texture coordinate set does not exist.
 *
 * \see RpGeometryGetFlags
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetPreLightColors
 * \see RpMorphTargetGetVertices
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryGetTriangles
 * \see RpGeometryCreate
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpWorldPluginAttach
 *
 * To set the vertex texture coordinates of a geometry consisting of two
 * triangles sharing an edge (4 vertices, 4 (u,v) pairs): |
 * \verbatim
   RpGeometry *geometry;
   RwTexCoords *texCoord;

   texCoord = RpGeometryGetVertexTexCoords(geometry, rwTEXTURECOORDINATEINDEX0);

   texCoord[0].u = (RwReal)(0.0); texCoord[0].v = (RwReal)(0.0);
   texCoord[1].u = (RwReal)(1.0); texCoord[1].v = (RwReal)(0.0);
   texCoord[2].u = (RwReal)(0.0); texCoord[2].v = (RwReal)(1.0);
   texCoord[3].u = (RwReal)(1.0); texCoord[3].v = (RwReal)(1.0);
  \endverbatim
 */
RwTexCoords *
RpGeometryGetVertexTexCoords(const RpGeometry *geometry,
                             RwTextureCoordinateIndex uvIndex)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetVertexTexCoords"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(0 < uvIndex);
    RWASSERT(rwMAXTEXTURECOORDS > (uvIndex - 1));

    /* Note that index starts at 1 for the first set */
    RWRETURN(RpGeometryGetVertexTexCoordsMacro(geometry, uvIndex));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetNumTexCoordSets is used to determine the number of
 * sets of texture coordinates in the specified geometry. Each set has
 * one texture coordinate per vertex.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns a RwInt32 value equal to the number of texture coordinate
 * sets.
 *
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryGetNumMorphTargets
 * \see RpGeometryGetVertexTexCoords
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryGetNumTexCoordSets(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetNumTexCoordSets"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetNumTexCoordSetsMacro(geometry));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetNumVertices is used to determine the number of
 * vertices defining the specified geometry. All morph targets have the same
 * number of vertices.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns a RwInt32 value equal to the number of vertices if successful
 * or -1 if there is an error.
 *
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryGetNumMorphTargets
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryGetNumVertices(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetNumVertices"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetNumVerticesMacro(geometry));
}

/**
 * \ingroup rpmorphtarget
 * \ref RpMorphTargetGetVertices is used to retrieve the array of vertex
 * positions from the specified morph target. Use this function to initialize or
 * redefine the morph target's vertex list.
 *
 * The geometry must be locked before the vertex data can be modified.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param morphTarget  Pointer to the morph target containing the vertex array.
 *
 * \return Returns a pointer to an array of RwV3d values if successful or
 * NULL if there is an error.
 *
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryCreate
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpWorldPluginAttach
 *
 */
RwV3d *
RpMorphTargetGetVertices(const RpMorphTarget *morphTarget)
{
    RWAPIFUNCTION(RWSTRING("RpMorphTargetGetVertices"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(morphTarget);

    RWRETURN(RpMorphTargetGetVerticesMacro(morphTarget));
}

/**
 * \ingroup rpmorphtarget
 * \ref RpMorphTargetGetVertexNormals is used to retrieve the array of
 * vertex normals from the specified morph target. The array only exists if
 * the morph target's geometry has been created using the rpGEOMETRYNORMALS
 * flag. Use this function to initialize or redefine the morph target's
 * vertex normal list. There is a one-to-one correspondence between the
 * vertex list array and the vertex normal array.
 *
 * The geometry must be locked before the vertex normal data can be modified.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param morphTarget  Pointer to the morph target containing the vertex normal array.
 *
 * \return Returns a pointer to an array of RwV3d values if successful or
 * NULL if there is an error.
 *
 * \see RpMorphTargetGetVertices
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetMorphTarget
 * \see RpGeometryCreate
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpWorldPluginAttach
 *
 */
RwV3d *
RpMorphTargetGetVertexNormals(const RpMorphTarget *morphTarget)
{
    RWAPIFUNCTION(RWSTRING("RpMorphTargetGetVertexNormals"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(morphTarget);

    RWRETURN(RpMorphTargetGetVertexNormalsMacro(morphTarget));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetTriangles is used to retrieve the array of triangles
 * from the specified geometry. The triangles define the geometry's topology
 * (vertex connectivity) and material properties, which are shared between each
 * morph target. Hence, all morph targets have the same number of triangles.
 * The material associated with each triangle can be used to define the
 * geometry's color, opacity and texture. Use this function prior to
 * initializing or redefining the geometry's topology and material properties.
 *
 * The geometry must be locked before modifying the geometry's topology and
 * material data.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry that contains the triangles.
 *
 * \return Returns pointer to an array of RpTriangle values if successful
 * or NULL if there is an error.
 *
 * \see RpGeometryTriangleSetVertexIndices
 * \see RpGeometryTriangleSetMaterial
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryGetPreLightColors
 * \see RpMorphTargetGetVertices
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryCreate
 * \see RpGeometryLock
 * \see RpGeometryUnlock
 * \see RpMaterialCreate
 * \see RpWorldPluginAttach
 *
 * To set the topology and material properties of a geometry consisting
 * of two triangles sharing an edge (4 vertices):
 * \verbatim
   RpGeometry *geometry;
   RpTriangle *tlist;
   RpMaterial *red, *white;

   tlist = RpGeometryGetTriangles(geometry);

   // First triangle...
   RpGeometryTriangleSetVertexIndices(geometry, &tlist[0], 0, 1, 2 );
   RpGeometryTriangleSetMaterial(geometry, &tlist[0], red );

   // Second triangle...
   RpGeometryTriangleSetVertexIndices(geometry, &tlist[1], 0, 2, 3 );
   RpGeometryTriangleSetMaterial(geometry, &tlist[1], white );
   \endverbatim
 *
 */
RpTriangle *
RpGeometryGetTriangles(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetTriangles"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetTrianglesMacro(geometry));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetNumTriangles is used to retrieve the number of
 * triangles defining the specified geometry. The triangles define the
 * geometry's topology (vertex connectivity) and material properties which
 * are shared between each morph target. Hence, all morph targets have the same
 * number of triangles.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns a RwInt32 value equal to the number of triangles if
 * successful or -1 if there is an error.
 *
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetNumMorphTargets
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryGetNumTriangles(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetNumTriangles"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetNumTrianglesMacro(geometry));
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            Accessing Geometry

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetMaterial is used to retrieve the material with the
 * given index from the specified geometry's material list.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 * \param matNum  A RwInt32 value equal to the index of the material.
 *
 * \return Returns pointer to the material if successful or NULL if there
 * is an error.
 *
 * \see RpGeometryTriangleGetMaterial
 * \see RpGeometryGetNumMaterials
 * \see RpGeometryForAllMaterials
 * \see RpMaterialCreate
 * \see RpGeometryGetTriangles
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RpMaterial *
RpGeometryGetMaterial(const RpGeometry *geometry, RwInt32 matNum)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetMaterial"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    
	if ( matNum >= geometry->matList.numMaterials || matNum < 0 )
	{
		RWRETURN(0);
	}

    RWRETURN(RpGeometryGetMaterialMacro(geometry, matNum));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryTriangleSetVertexIndices is used to define the specified
 * triangle in the given geometry according to the given vertex indices.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the triangles.
 * \param triangle  Pointer to the triangle.
 * \param vert1  A RwUInt32 value specifying the index of the first vertex.
 * \param vert2  A RwUInt32 value specifying the index of the second vertex.
 * \param vert3  A RwUInt32 value specifying the index of the third vertex.
 *
 * \return Returns pointer to the geometry if successful or NULL if there
 * is an error.
 *
 * \see RpGeometryTriangleGetVertexIndices
 * \see RpGeometryGetTriangles
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetMaterial
 * \see RpGeometryTriangleGetMaterial
 * \see RpGeometryTriangleSetMaterial
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
const RpGeometry   *
RpGeometryTriangleSetVertexIndices(const RpGeometry * geometry,
                                   RpTriangle * triangle,
                                   RwUInt16 vert1, RwUInt16 vert2,
                                   RwUInt16 vert3)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryTriangleSetVertexIndices"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(triangle);
    /* ASSERT(triangle is part of this geometry) */
    RWASSERT( /* (vert1 >= 0) && */ (vert1 < geometry->numVertices));
    RWASSERT( /* (vert2 >= 0) && */ (vert2 < geometry->numVertices));
    RWASSERT( /* (vert3 >= 0) && */ (vert3 < geometry->numVertices));

    triangle->vertIndex[0] = vert1;
    triangle->vertIndex[1] = vert2;
    triangle->vertIndex[2] = vert3;

    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryTriangleSetMaterial is used to associate the specified
 * material with the given triangle in the given geometry. The geometry's material
 * list is modified accordingly and the material's reference count incremented.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the triangles.
 * \param triangle  Pointer to the triangle.
 * \param material  Pointer to the material.
 *
 * \return Returns pointer to the geometry if successful or NULL if
 * there is an error.
 *
 * \see RpGeometryTriangleGetMaterial
 * \see RpGeometryGetNumMaterials
 * \see RpGeometryForAllMaterials
 * \see RpGeometryGetMaterial
 * \see RpMaterialCreate
 * \see RpGeometryTriangleGetVertexIndices
 * \see RpGeometryTriangleSetVertexIndices
 * \see RpGeometryGetTriangles
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RpGeometry *
RpGeometryTriangleSetMaterial(RpGeometry *geometry,
                              RpTriangle *triangle,
                              RpMaterial *material)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryTriangleSetMaterial"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(triangle);
    /* ASSERT(triangle is part of this geometry) */

    if (material)
    {
        RwInt32 i;

        i = _rpMaterialListFindMaterialIndex(&geometry->matList, material);
        if (i < 0)
        {
            i = _rpMaterialListAppendMaterial(&geometry->matList, material);
            if (i < 0)
            {
                /* failed to AppendMaterial */
                RWRETURN((RpGeometry *)NULL);
            }
        }

        /* Now shove it in the poly */
        triangle->matIndex = (RwUInt16)i;
    }
    else
    {
        /* Removing the material */
        triangle->matIndex = 0xFFFF;
    }

    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryTriangleGetVertexIndices is used to retrieve the vertex
 * indices that define the given triangle in the specified geometry. Specify
 * NULL for those vertex indices not required.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the triangles.
 * \param triangle  Pointer to the triangle.
 * \param vert1  Pointer to a RwUInt16 value which will receive the first vertex index.
 * \param vert2  Pointer to a RwUInt16 value which will receive the second vertex index.
 * \param vert3  Pointer to a RwUInt16 value which will receive the third vertex index.
 *
 * \return Returns pointer to the geometry if successful or NULL if there
 * is an error.
 *
 * \see RpGeometryTriangleSetVertexIndices
 * \see RpGeometryGetTriangles
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetMaterial
 * \see RpGeometryTriangleSetMaterial
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
const RpGeometry   *
RpGeometryTriangleGetVertexIndices(const RpGeometry * geometry, const
                                   RpTriangle * triangle,
                                   RwUInt16 * vert1, RwUInt16 * vert2,
                                   RwUInt16 * vert3)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryTriangleGetVertexIndices"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(triangle);

    if (vert1)
    {
        *vert1 = triangle->vertIndex[0];
    }
    if (vert2)
    {
        *vert2 = triangle->vertIndex[1];
    }
    if (vert3)
    {
        *vert3 = triangle->vertIndex[2];
    }

    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryTriangleGetMaterial is used to retrieve the material
 * associated with the specified triangle in the given geometry.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the triangles.
 * \param triangle  Pointer to the triangle.
 *
 * \return Returns pointer to the material if successsful or NULL if
 * there is an error.
 *
 * \see RpGeometryTriangleSetMaterial
 * \see RpGeometryGetMaterial
 * \see RpGeometryGetNumMaterials
 * \see RpGeometryForAllMaterials
 * \see RpMaterialCreate
 * \see RpGeometryTriangleGetVertexIndices
 * \see RpGeometryTriangleSetVertexIndices
 * \see RpGeometryGetTriangles
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */
RpMaterial         *
RpGeometryTriangleGetMaterial(const RpGeometry * geometry,
                              const RpTriangle * triangle)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryTriangleGetMaterial"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(triangle);
    /* ASSERT(triangle is part of this geometry) */

    if (triangle->matIndex == 0xFFFF)
    {
        RWRETURN((RpMaterial *)NULL);
    }
    else
    {
        RpMaterial         *materialRet;

        materialRet =
            RpGeometryGetMaterial(geometry, triangle->matIndex);
        RWRETURN(materialRet);
    }
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetNumMaterials is used to retrieve the number of different
 * materials in use by all polygons in the specified geometry.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry
 *
 * \return Returns a RwInt32 value equal to the number of materials if
 * successful or -1 if there is an error.
 *
 * \see RpGeometryTriangleGetMaterial
 * \see RpGeometryTriangleSetMaterial
 * \see RpGeometryGetMaterial
 * \see RpGeometryForAllMaterials
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryGetNumMaterials(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetNumMaterials"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetNumMaterialsMacro(geometry));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryForAllMaterials is used to apply the given callback
 * function to all materials referenced by polygons in the specified geometry.
 * The format of the callback function is:
 *
 * RpMaterial * (*RpMaterialCallBack)(RpMaterial *material, void *data)
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure status
 * the iteration is terminated. However, \ref RpGeometryForAllMaterials will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the materials.
 * \param fpCallBack  Pointer to the callback function to apply to each material.
 * \param pData  Pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the geometry if successful or NULL if there
 * is an error.
 *
 * \see RpGeometryGetNumMaterials
 * \see RpGeometryTriangleGetMaterial
 * \see RpGeometryTriangleSetMaterial
 * \see RpGeometryGetMaterial
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometryForAllMaterials(RpGeometry * geometry,
                          RpMaterialCallBack fpCallBack, void *pData)
{
    RwInt32             numMaterials;
    RwInt32             i;

    RWAPIFUNCTION(RWSTRING("RpGeometryForAllMaterials"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(fpCallBack);

    numMaterials = RpGeometryGetNumMaterials(geometry);
    for (i = 0; i < numMaterials; i++)
    {
        RpMaterial         *material;

        material = RpGeometryGetMaterial(geometry, i);
        if (!fpCallBack(material, pData))
        {
            /* Early out */
            RWRETURN(geometry);
        }
    }

    /* All ok */
    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryLock is used to lock the specified geometry so an
 * application can access it's internal data structures. This function
 * prepares the geometry for unlocking after its data has been modified so that
 * a geometry's meshes can be rebuilt. (Meshes are used to group polygons with
 * the same material, enabling efficient rendering.) A newly created geometry
 * is already locked.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 * \param lockMode  A bit-field specifying which of the geometry's data to lock;
 *     use bitwise OR to specify more than one (bit-field type \ref RpGeometryLockMode):
 *
 * \return Returns pointer to the geometry if successful or NULL if there
 * is an error.
 *
 * \see RpGeometryUnlock
 * \see RpGeometryCreate
 * \see RpGeometryForAllMeshes
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometryLock(RpGeometry * geometry, RwInt32 lockMode)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryLock"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(!(lockMode & ~(rpGEOMETRYLOCKALL)));

    /* Serial number (for re-instancing) */
    geometry->lockedSinceLastInst |= lockMode;

    if (lockMode & (RwInt32) rpGEOMETRYLOCKPOLYGONS)
    {
        /* If we're changing the polygons, then we need to recreate the mesh */
        if (geometry->mesh)
        {
            _rpMeshDestroy(geometry->mesh);
            geometry->mesh = (RpMeshHeader *)NULL;
        }
    }

    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryUnlock is used to unlock the specified geometry. This
 * function is typically used to release the geometry's data structures
 * after modification so that a geometry's meshes can be rebuilt from the new
 * data. (Meshes are used to group polygons with the same material, enabling
 * efficient rendering.)
 *
 * Newly created geometry is always locked and should be unlocked before it
 * can be used in any rendering.
 *
 * Note all triangles in a geometry must have a material assigned to them
 * before it can be unlocked.
 *
 * Note that the bounding sphere on each morph target will need recalculating
 * if the geometry's vertex data has changed; \ref RpGeometryUnlock does not do
 * this for you.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns pointer to the geometry if successful or NULL if there
 * an error,
 *
 * \see RpGeometryLock
 * \see RpGeometryCreate
 * \see RpGeometryForAllMeshes
 * \see RpMorphTargetCalcBoundingSphere
 * \see RpMorphTargetSetBoundingSphere
 * \see RpWorldPluginAttach
 *
 */
RpGeometry         *
RpGeometryUnlock(RpGeometry * geometry)
{
    RwTexture **textureArray;
    RwRaster **rasterArray;
    RxPipeline **pipelineArray;
    RwUInt16 numTex = 0;
    RwUInt16 numRas = 0;
    RwUInt16 numPip = 0;

    RWAPIFUNCTION(RWSTRING("RpGeometryUnlock"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    if (!geometry->mesh)
    {
        RpBuildMesh        *buildMesh;

        /* Provide a useful hint as to size */
        buildMesh = _rpBuildMeshCreate(geometry->numTriangles);
        if (buildMesh)
        {
            RpMeshHeader       *newMesh;
            RwInt32             i;
            RwInt32             numMaterials =
                                    RpGeometryGetNumMaterials(geometry);

            if (0 < numMaterials)
            {
                textureArray = RwMalloc(sizeof(RwTexture *) * numMaterials,
                                rwID_TEXTURE | rwMEMHINTDUR_FUNCTION);
                rasterArray = RwMalloc(sizeof(RwRaster *) * numMaterials,
                                rwID_GEOMETRYMODULE | rwMEMHINTDUR_FUNCTION);
                pipelineArray = RwMalloc(sizeof(RxPipeline *) * numMaterials,
                                rwID_GEOMETRYMODULE | rwMEMHINTDUR_FUNCTION);

                for (i = 0; i < geometry->numTriangles; i++)
                {
                    RpTriangle         *triangle = &geometry->triangles[i];
                    RpMaterial         *material;
                    RwUInt16            texIndex, rasIndex, pipIndex;
                    RxPipeline         *pipeline = NULL;
                    RwTexture          *texture = NULL;
                    RwRaster           *raster = NULL;

                    RWASSERTM(triangle->matIndex != 0xFFFF,
                              (RWSTRING("All triangles in a geometry must have "
                               "materials assigned to unlock the geometry")));

                    material =
                            _rpMaterialListGetMaterial(&geometry->matList,
                                                       triangle->matIndex);

                    texture = RpMaterialGetTexture(material);
                    for (texIndex = 0; texIndex < numTex; texIndex++)
                    {
                        if (textureArray[texIndex] == texture)
                        {
                            /* Found texture break out the loop */
                            break;
                        }
                    }
                    /* Was the texture found, if not add it */
                    if (texIndex == numTex)
                    {
                        textureArray[texIndex] = texture;
                        numTex++;
                    }
                    if (texture)
                    {
                        raster = RwTextureGetRaster(texture);
                    }

                    for (rasIndex = 0; rasIndex < numRas; rasIndex++)
                    {
                        if (rasterArray[rasIndex] == raster)
                        {
                            /* Found raster break out the loop */
                            break;
                        }
                    }
                    /* Was the raster found, if not add it */
                    if (rasIndex == numRas)
                    {
                        rasterArray[rasIndex] = raster;
                        numRas++;
                    }

                    RpMaterialGetPipeline(material, &pipeline);
                    for (pipIndex = 0; pipIndex < numPip; pipIndex++)
                    {
                        if (pipelineArray[pipIndex] == pipeline)
                        {
                            /* Found pipeline break out the loop */
                            break;
                        }
                    }
                    /* Was the pipeline found, if not add it */
                    if (pipIndex == numPip)
                    {
                        pipelineArray[pipIndex] = pipeline;
                        numPip++;
                    }

                    _rpBuildMeshAddTriangle(buildMesh, material,
                                            triangle->vertIndex[0],
                                            triangle->vertIndex[1],
                                            triangle->vertIndex[2],
                                            triangle->matIndex, texIndex,
                                            rasIndex, pipIndex);
                }

                RwFree(textureArray);
                RwFree(rasterArray);
                RwFree(pipelineArray);
            }
            else
            {
                /* If there are no materials there must also be no triangles */
                RWASSERTM(0 == geometry->numTriangles,
                          (RWSTRING("All triangles in a geometry must have "
                           "materials assigned to unlock the geometry")));
            }

            /* TODO: If we are to support other primitive types
             * for export and unlocking/mesh-rebuilding then we
             * need to add more geometry flags and upgrade the
             * _meshOptimise func and add appropriate stuff to
             * the exporters. */
            if (rpGEOMETRYTRISTRIP & RpGeometryGetFlags(geometry))
            {
                newMesh =
                    _rpMeshOptimise(buildMesh, rpMESHHEADERTRISTRIP);
            }
            else
            {
                newMesh = _rpMeshOptimise(buildMesh, 0);
            }

            if (newMesh)
            {
                geometry->mesh = newMesh;
                RWRETURN(geometry);
            }

            /* Failed to optimise mesh */
            _rpBuildMeshDestroy(buildMesh);
            RWRETURN((RpGeometry *)NULL);
        }

        /* Couldn't create mesh */
        RWRETURN((RpGeometry *)NULL);
    }

    /* Polygons are not locked, but that's OK */
    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryForAllMeshes is used to apply the given callback
 * function to all meshes in the given geometry.  The format of the callback
 * function is:
 *
 * RpMesh *(*RpMeshCallBack)(RpMesh *mesh, RpMeshHeader *meshHeader, void *data);
 *
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * If any invocation of the callback function returns a failure status the
 * iteration is terminated.  However \ref RpGeometryForAllMeshes will still
 * return successfully.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry containing the mesh.
 * \param fpCallBack  Pointer to the callback function to apply to each mesh.
 * \param pData  Pointer to the user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the geometry if successful or NULL if there is an
 * error or if the geometry does not have any mesh information.
 *
 * \see RpGeometryCreate
 * \see RpGeometryUnlock
 * \see RpGeometryLock
 * \see RpGeometryForAllMeshes
 * \see RpWorldPluginAttach
 *
 */
const RpGeometry   *
RpGeometryForAllMeshes(const RpGeometry * geometry,
                       RpMeshCallBack fpCallBack, void *pData)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryForAllMeshes"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(fpCallBack);

    if (geometry->mesh)
    {
        if (_rpMeshHeaderForAllMeshes
            (geometry->mesh, fpCallBack, pData))
        {
            RWRETURN(geometry);
        }
    }

    /* No mesh info, or callback function failed */
    RWRETURN((const RpGeometry *)NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                          Creation/Destruction

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/**
 * \ingroup rpgeometry
 * \ref RpGeometryCreate is used to create a new geometry according to
 * the specified number of vertices and triangles and the specified format.
 * The geometry is created with one morph target. Arrays are allocated
 * for the vertex and triangle lists as well as for the vertex normals,
 * pre-light colors and texture coordinates if any of these are required
 * according to the format. All arrays are undefined and must be initialized
 * before the geometry is unlocked, including the material properties defined
 * in the triangle list.
 *
 * The format is specified by combining \ref RpGeometryFlag flags using
 * an OR operation. The rpGEOMETRYTEXTURED and rpGEOMETRYTEXTURED2
 * flags specify that either a single or double set of texture coordinates
 * are required. Alternatively, the rpGEOMETRYTEXCOORDSETS(n) macro may be
 * used to explicitly set the number of sets required up to
 * rwMAXTEXTURECOORDS (restricted to 8). In this case, the
 * backward compatibility flags (rpGEOMETRYTEXTURED and rpGEOMETRYTEXTURED2)
 * are set internally to reflect the status, rpGEOMETRYTEXTURED2 indicating
 * that 2 or more sets are present. For example:
 *
 * \verbatim
   geom1 = RpGeometryCreate(numVerts, numTris,
                            rpGEOMETRYTRISTRIP | rpGEOMETRYTEXTURED |
                            rpGEOMETRYNORMALS | rpGEOMETRYLIGHT);

   geom2 = RpGeometryCreate(numVerts, numTris,
                            rpGEOMETRYTRISTRIP | rpGEOMETRYTEXCOORDSETS(2) |
                            rpGEOMETRYNORMALS | rpGEOMETRYLIGHT);

   \endverbatim
 *
 * Note that the geometry is created with a reference count of one. Hence, it
 * is the responsiblity of the application to destroy the geometry if it is
 * no longer required.
 *
 * The newly created geometry is in a locked state and must be unlocked if it
 * is to be used in any rendering.
 *
 * The world plugin must be attached before using this function.
 *
 * \param numVerts      A RwInt32 value equal to the number of vertices
 *                      (maximum allowable is 65535).
 * \param numTriangles  A RwInt32 value equal to the number of triangles.
 * \param format        A 32-bit unsigned integer bit-field specifying the
 *                      geometry's format. This should consist of an OR
 *                      combined list of flags of type \ref RpGeometryFlag.
 *
 * \return Returns pointer to the geometry if successful or NULL if there is
 * an error.
 *
 * \see RpGeometryDestroy
 * \see RpGeometryAddMorphTarget
 * \see RpMorphTargetGetVertices
 * \see RpGeometryGetNumVertices
 * \see RpGeometryGetTriangles
 * \see RpGeometryGetNumTriangles
 * \see RpGeometryGetNumTexCoordSets
 * \see RpMorphTargetGetVertexNormals
 * \see RpGeometryGetPreLightColors
 * \see RpGeometryGetVertexTexCoords
 * \see RpAtomicSetGeometry
 * \see RpWorldPluginAttach
 *
 */
RpGeometry *
RpGeometryCreate(RwInt32 numVerts, RwInt32 numTriangles, RwUInt32 format)
{
    RpGeometry     *geometry;
    RwUInt8        *goffset;
    RwUInt32        gsize;
    RwUInt32        numTexCoordSets;
    RwInt32         flags;

    RWAPIFUNCTION(RWSTRING("RpGeometryCreate"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(numVerts >= 0);
    RWASSERT(numVerts < 65536);
    RWASSERT(numTriangles >= 0);

    if ((numVerts < 0) || (numVerts >= 65536) || (numTriangles < 0))
    {
        if (numVerts < 0)
        {
            RwDebugSendMessage(rwDEBUGERROR, "RpGeometryCreate",
                "You cannot construct an RpGeometry with a negative number of vertices.");
        }
        else if (numVerts >= 65536)
        {
            RWERROR((E_RP_WORLD_TOOMANYVERTICES));
        }
        if (numTriangles < 0)
        {
            RwDebugSendMessage(rwDEBUGERROR, "RpGeometryCreate",
                "You cannot construct an RpGeometry with a negative number of triangles.");
        }
        RWRETURN((RpGeometry *)NULL);
    }

    flags = GeometryFormatGetFlagsMacro(format);
    gsize = geometryTKList.sizeOfStruct;

    /* How many texture coordinate sets? */
    numTexCoordSets = GeometryFormatGetNumTexCoordSetsMacro(format);
    RWASSERT(numTexCoordSets <= rwMAXTEXTURECOORDS);

    /* Make sure backward compatibility flags are set correctly */
    flags = (flags & ~(rpGEOMETRYTEXTURED | rpGEOMETRYTEXTURED2))
             | ((numTexCoordSets == 1) ? rpGEOMETRYTEXTURED :
               ((numTexCoordSets > 1) ? rpGEOMETRYTEXTURED2 : 0));

    if (!(rpGEOMETRYNATIVE & format))
    {
        if (flags & (RwInt32) rpGEOMETRYPRELIT)
        {
            gsize += sizeof(RwRGBA) * numVerts;
        }

        if (numTexCoordSets > 0)
        {
            /* Include space for tex coords */
            gsize += sizeof(RwTexCoords) * numVerts * numTexCoordSets;
        }

        gsize += sizeof(RpTriangle) * numTriangles;
    }

    geometry = (RpGeometry *)RwMalloc(gsize, rwID_GEOMETRY | rwMEMHINTDUR_EVENT);
    if (!geometry)
    {
        RWRETURN((RpGeometry *)NULL);
    }

    /* Set up the material list */
    if (!_rpMaterialListInitialize(&geometry->matList))
    {
        /* Failed to initialise the geometry list */
        RWRETURN((RpGeometry *)NULL);
    }

    /* Allocate initial array of morph targets (0) */
    geometry->morphTarget = (RpMorphTarget *)NULL;

    /* Set up key frames */
    geometry->numMorphTargets = 0;

    /* Set up type */
    rwObjectInitialize(geometry, rpGEOMETRY, 0);

    /* Set the instancing information */
    geometry->repEntry = (RwResEntry *)NULL;

    /* Nothing locked since last time */
    geometry->lockedSinceLastInst = 0;

    /* Initialise ref count */
    geometry->refCount = 1;

    /* Set up the device information -> its locked! */
    geometry->mesh = (RpMeshHeader *)NULL;

    /* Set up texture coords */
    geometry->numTexCoordSets = numTexCoordSets;
    memset(geometry->texCoords, 0, rwMAXTEXTURECOORDS * sizeof(RwTexCoords *));

    geometry->preLitLum = (RwRGBA *)NULL;

    /* Set up the triangles */
    geometry->triangles = (RpTriangle *)NULL;
    geometry->numTriangles = numTriangles;

    /* Set the geometries flags */
    RpGeometrySetFlags(geometry, flags | (rpGEOMETRYNATIVEFLAGSMASK & format));
    geometry->numVertices = numVerts;

    if (!(rpGEOMETRYNATIVE & format))
    {
        /* step past structure to allocate arrays */
        goffset = (RwUInt8 *)geometry + geometryTKList.sizeOfStruct;

        /* Create prelight values if necessary */
        if ((flags & (RwInt32)rpGEOMETRYPRELIT) && numVerts)
        {
            geometry->preLitLum = (RwRGBA *)goffset;
            goffset += sizeof(RwRGBA *) * numVerts;
        }

        /* Create texture coordinates if necessary - in the right place */
        if (numTexCoordSets > 0 && numVerts)
        {
            RwUInt32    i;

            for (i=0; i<numTexCoordSets; i++)
            {
                geometry->texCoords[i] = (RwTexCoords *)goffset;
                goffset += sizeof(RwTexCoords) * numVerts;
            }
        }

        /* Set up the triangles */
        if (numTriangles)
        {
            RwInt32 i;

            geometry->triangles = (RpTriangle *)goffset;
            goffset += sizeof(RpTriangle) * numTriangles;

            /* Setup all of the materials */
            for (i = 0; i < numTriangles; i++)
            {
                geometry->triangles[i].matIndex = 0xFFFF;
            }
        }
    }

    /* Allocate one key frame, because geometry is useless without it */
    if (RpGeometryAddMorphTarget(geometry) < 0)
    {
        _rpMaterialListDeinitialize(&geometry->matList);
        RwFree(geometry);
        RWRETURN((RpGeometry *)NULL);
    }

    /* Initialize the plugin memory */
    rwPluginRegistryInitObject(&geometryTKList, geometry);

    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    /* All done */
    RWRETURN(geometry);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                    Geometry operations based on clumps

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryAddRef is used to increment the reference count of the
 * specified geometry. Use this function to indicate that a new object depends
 * on the geometry's existence. This ensures that the geometry is not
 * lost when other objects that use this geometry are destroyed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns pointer to the geometry if successful or NULL if there is
 * an error.
 *
 * \see RpGeometryDestroy
 * \see RpWorldPluginAttach
 */
RpGeometry         *
RpGeometryAddRef(RpGeometry * geometry)
{
    RWFUNCTION(RWSTRING("RpGeometryAddRef"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    geometry->refCount++;

    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryDestroy is used to destroy the specified geometry and
 * any resources associated with it. Only
 * geometries whose reference count is zero are actually removed from the
 * system. Reference counts greater than one indicate that this particular
 * geometry is still being used by other atomics; in this situation, this
 * function will only decrement the geometry's reference count. It is the
 * application's responsibility to destroy the geometry if it is no longer
 * required and it is not referenced by any atomics. This does not apply if
 * the geometry was acquired by reading an atomic from a binary stream:
 * destroying the atomic will also destroy the geometry it references.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpGeometryCreate
 * \see RpGeometryRemoveMorphTarget
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpGeometryDestroy(RpGeometry * geometry)
{
    RwBool              result = TRUE;

    RWAPIFUNCTION(RWSTRING("RpGeometryDestroy"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    /* Do we  actually need to blow it away  ? */
    if ((geometry->refCount - 1) <= 0)
    {
        /* Blow away it's instance copy if it has one */
        if (geometry->repEntry)
        {
            RwResourcesFreeResEntry(geometry->repEntry);
        }

        /* RWCRTCHECKMEMORY(); */
        /*
         * Don't decrement the refCount until after the resource
         * entry has been destroyed as we many need to call API
         * geometry functions.
         */
        --geometry->refCount;
        /* RWCRTCHECKMEMORY(); */

        result = GeometryAnnihilate(geometry);
    }
    else
    {
        /* RWCRTCHECKMEMORY(); */
        --geometry->refCount;
        /* RWCRTCHECKMEMORY(); */
    }

    RWRETURN(result);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryRegisterPlugin is used to register a plugin and
 * reserve some space within a geometry. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * The world plugin must be attached before using this function.
 *
 * \param size  A RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 * to identify binary chunks).
 * \param constructCB  Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB  Copy constructor for the plugin data block.
 *
 * \return Returns the byte offset within the geometry of memory reserved
 * for this plugin or a negative value if there is an error.
 *
 * \see RpGeometryRegisterPluginStream
 * \see RpGeometrySetStreamAlwaysCallBack
 * \see RpGeometryGetPluginOffset
 * \see RpGeometryValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                         RwPluginObjectConstructor constructCB,
                         RwPluginObjectDestructor destructCB,
                         RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpGeometryRegisterPlugin"));
    RWASSERT(!geometryModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&geometryTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 * to identify binary chunks).
 * \param readCB  Callback used when a chunk is read that is identified as being for
 *       this plugin.  If the plugin does not wish to read chunks it should
 *       skip bytes in the stream in this callback.
 * \param writeCB  Callback used when a chunk should be written out for this plugin.
 * \param getSizeCB  Callback used to determine the binary stream size required for this
 *       plugin (return negative to suppress chunk writing).
 *
 * \return Returns the byte offset within the geometry of memory reserved
 * for this plugin or a negative value if there is an error.
 *
 * \see RpGeometrySetStreamAlwaysCallBack
 * \see RpGeometryRegisterPlugin
 * \see RpGeometryGetPluginOffset
 * \see RpGeometryValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryRegisterPluginStream(RwUInt32 pluginID,
                               RwPluginDataChunkReadCallBack readCB,
                               RwPluginDataChunkWriteCallBack writeCB,
                               RwPluginDataChunkGetSizeCallBack
                               getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpGeometryRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */

    plug = rwPluginRegistryAddPluginStream(&geometryTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometrySetStreamAlwaysCallBack is used to associate a
 * binary stream callback with a previously registered plugin. This callback
 * is called for all plugins after stream data reading has completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 * to identify binary chunks).
 * \param alwaysCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns the byte offset within the geometry of memory reserved
 * for this plugin or a negative value if there is an error.
 *
 * \see RpGeometryRegisterPluginStream
 * \see RpGeometryRegisterPlugin
 * \see RpGeometryGetPluginOffset
 * \see RpGeometryValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometrySetStreamAlwaysCallBack(RwUInt32 pluginID,
                                  RwPluginDataChunkAlwaysCallBack
                                  alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpGeometrySetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */

    plug = rwPluginRegistryAddPluginStreamAlwaysCB(&geometryTKList,
                                                pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetPluginOffset is used to get the offset of a previously
 * registered plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns the data block offset if successful or -1 if the plugin
 * is not registered.
 *
 * \see RpGeometryRegisterPlugin
 * \see RpGeometryRegisterPluginStream
 * \see RpGeometrySetStreamAlwaysCallBack
 * \see RpGeometryValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpGeometryGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpGeometryGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&geometryTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryValidatePlugins is used to validate the plugin memory
 * allocated within the specified geometry. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry to validate.
 *
 * \return Returns TRUE is the geometry data is valid or FALSE if there is an
 * error or if the geometry data has become corrupt.
 *
 * \see RpGeometryRegisterPlugin
 * \see RpGeometryRegisterPluginStream
 * \see RpGeometrySetStreamAlwaysCallBack
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpGeometryValidatePlugins(const RpGeometry * geometry __RWUNUSEDRELEASE__)
{
    RwBool              valid = TRUE;

    RWAPIFUNCTION(RWSTRING("RpGeometryValidatePlugins"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

#ifdef RWDEBUG
    valid = rwPluginRegistryValidateObject(&geometryTKList, geometry);
#endif /* RWDEBUG */

    RWRETURN(valid);
}

static RwInt32
GeometryStreamGetSizeActual(const RpGeometry *geometry)
{
    RwInt32 size;
    RwInt32 i;

    RWFUNCTION(RWSTRING("GeometryStreamGetSizeActual"));
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    size = sizeof(_rpGeometry);

    if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
    {
        if (geometry->numVertices)
        {
            /* Information in geometry */
            if (rpGEOMETRYPRELIT & RpGeometryGetFlags(geometry))
            {
                /* Pre light colors */
                size += sizeof(RwRGBA) * geometry->numVertices;
            }

            /* Vertex texture coordinates */
            size += sizeof(RwTexCoords) * geometry->numVertices
                    * geometry->numTexCoordSets;

            /* Triangles */
            size += sizeof(_rpTriangle) * geometry->numTriangles;
        }

        /* Key frames */
        for (i = 0; i < geometry->numMorphTargets; i++)
        {
            size += sizeof(_rpMorphTarget);

            if (geometry->morphTarget[i].verts)
            {
                size += sizeof(RwV3d) * geometry->numVertices;
            }

            if (geometry->morphTarget[i].normals)
            {
                size += sizeof(RwV3d) * geometry->numVertices;
            }
        }
    }
    else
    {
        /* Can only have 1 morph target with NATIVE data, at the moment */
        size += sizeof(_rpMorphTarget);
    }

    RWRETURN(size);
}

/****************************************************************************
 *
 * Binary Format Functions
 */

/**
 * \ingroup rpgeometry
 * \ref RpGeometryStreamGetSize is used to determine the size in bytes of
 * the binary representation of the specified geometry. This is used in the
 * binary chunk header to indicate the size of the geometry chunk. The size
 * does not include the size of the chunk header.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry whose binary size is required.
 *
 * \return Returns the chunk size of the geometry if successful or zero if
 * there is an error.
 *
 * \see RpGeometryStreamRead
 * \see RpGeometryStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpGeometryStreamGetSize(const RpGeometry * geometry)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RpGeometryStreamGetSize"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    /* Get size of geometry without the extension data */
    size = GeometryStreamGetSizeActual(geometry) +
           rwCHUNKHEADERSIZE;

    /* Material size */
    size += _rpMaterialListStreamGetSize(&geometry->matList) +
            rwCHUNKHEADERSIZE;

    /* Add on size of the geometry extension chunks */
    size += rwPluginRegistryGetSize(&geometryTKList, geometry) +
            rwCHUNKHEADERSIZE;

    RWRETURN(size);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryStreamWrite is used to write the specified geometry to
 * the given binary stream. The stream must have been opened prior to this
 * function call.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 * \param stream  Pointer to the binary stream.
 *
 * \return Returns pointer to the geometry if successful or NULL if there is
 * an error.
 *
 * \see RpGeometryStreamGetSize
 * \see RpGeometryStreamRead
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RpWorldPluginAttach
 *
 */
const RpGeometry   *
RpGeometryStreamWrite(const RpGeometry * geometry, RwStream * stream)
{
    _rpGeometry geom;
    RwInt32     i;
    RwInt32     flags;

    RWAPIFUNCTION(RWSTRING("RpGeometryStreamWrite"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);
    RWASSERT(stream);

#if (defined(RWDEBUG))
    /* For PowerPipe we have to do more work. We need to get vertices
     * sorted by material and duplicated at material boundaries (see
     * _rpGeometrySortByMaterial() above). Inform the app if this has
     * not been done */

    if (geometry->matList.numMaterials > 1)
    {
        RwBool              result;

        if (RpGeometryIsCorrectlySorted(geometry, &result) == NULL)
        {
            /* Out of memory most probably */
            RWRETURN((const RpGeometry *)NULL);
        }

        if (result == FALSE)
        {
            RWERROR((E_RX_MESHES_RANGES_OVERLAP, stream,
                     RWSTREAMTYPE(stream->type)));
            /* the write MUST fail here
             * to maintain orthogonality of function */
            RWRETURN((const RpGeometry *)NULL);
        }
    }
#endif /* (defined(RWDEBUG)) */

    /* This writes a chunk header for the geometry
     * without the extension chunks */
    if (!RwStreamWriteChunkHeader(stream, rwID_GEOMETRY,
                                  RpGeometryStreamGetSize(geometry)))
    {
        RWRETURN((const RpGeometry *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT,
                                  GeometryStreamGetSizeActual(geometry)))
    {
        RWRETURN((const RpGeometry *)NULL);
    }

    flags = RpGeometryGetFlags(geometry);

    /* Verify texture flags */
    RWASSERT((geometry->numTexCoordSets == 1) == ((flags & rpWORLDTEXTURED) > 0));
    RWASSERT((geometry->numTexCoordSets > 1) == ((flags & rpWORLDTEXTURED2) > 0));

    geom.format = flags | rpGEOMETRYTEXCOORDSETS(geometry->numTexCoordSets);
    geom.numTriangles = geometry->numTriangles;
    geom.numVertices = geometry->numVertices;
    geom.numMorphTargets = geometry->numMorphTargets;

    /* Verify texture flags */
    RWASSERT((geometry->numTexCoordSets == 1) == ((geom.format & rpWORLDTEXTURED) > 0));
    RWASSERT((geometry->numTexCoordSets > 1) == ((geom.format & rpWORLDTEXTURED2) > 0));

    (void)RwMemLittleEndian32(&geom, sizeof(geom));

    if (!RwStreamWrite(stream, &geom, sizeof(geom)))
    {
        RWRETURN((const RpGeometry *)NULL);
    }

    if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
    {
        if (geometry->numVertices)
        {
            /* Save off pre lighting information */
            if (rpGEOMETRYPRELIT & RpGeometryGetFlags(geometry))
            {
                /* Save off pre light colours - these are chars, so no conversion needed */
                if (!RwStreamWrite(stream, geometry->preLitLum,
                     sizeof(RwRGBA) * geometry->numVertices))
                {
                    RWRETURN((const RpGeometry *)NULL);
                }
            }

            /* Save off texture coordinate info */
            if (geometry->numTexCoordSets > 0)
            {
                const RwUInt32      sizeTC =
                    sizeof(RwTexCoords) * geometry->numVertices;

                /* Save off vertex texture coordinates - reals */
                for (i=0; i<geometry->numTexCoordSets; i++)
                {
                    RWASSERT(geometry->texCoords[i]);

                    if (!RwStreamWriteReal(
                            stream, (RwReal *) geometry->texCoords[i], sizeTC) )
                    {
                        RWRETURN((const RpGeometry *)NULL);
                    }
                }
            }

            /* Save off triangles */
            if (geometry->numTriangles)
            {
                _rpTriangle         tri;
                RwInt32             numTris;
                RpTriangle         *srceTri;

                /* Save off the triangle information */
                srceTri = geometry->triangles;
                numTris = geometry->numTriangles;
                while (numTris--)
                {
                    /* Fill it */
                    tri.vertex01 = UINT32PACK(srceTri->vertIndex[0],srceTri->vertIndex[1]);
                    tri.vertex2Mat = UINT32PACK(srceTri->vertIndex[2],srceTri->matIndex);
                    srceTri++;

                    /* Convert it */
                    (void)RwMemLittleEndian32(&tri, sizeof(tri));

                    /* Write it */
                    if (!RwStreamWrite(stream, &tri, sizeof(tri)))
                    {
                        RWRETURN((const RpGeometry *)NULL);
                    }
                }
            }
        }
    }

    /* Key frames */
    for (i = 0; i < geometry->numMorphTargets; i++)
    {
        _rpMorphTarget  kf;

        /* Fill it */
        kf.boundingSphere = ((geometry->morphTarget)[i]).boundingSphere;

        if (rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry))
        {
            kf.pointsPresent = FALSE;
            kf.normalsPresent = FALSE;
        }
        else
        {
            kf.pointsPresent = (geometry->morphTarget[i].verts != NULL);
            kf.normalsPresent = (geometry->morphTarget[i].normals != NULL);
        }

        /* Convert it */
        (void)RwMemRealToFloat32(&kf.boundingSphere, sizeof(kf.boundingSphere));
        (void)RwMemLittleEndian32(&kf, sizeof(kf));

        /* Write it */
        if (!RwStreamWrite(stream, &kf, sizeof(kf)))
        {
            RWRETURN((const RpGeometry *)NULL);
        }

        if (kf.pointsPresent)
        {
            if (!RwStreamWriteReal(stream, (RwReal *) (geometry->morphTarget)[i].verts,
                 sizeof(RwV3d) * geometry->numVertices))
            {
                RWRETURN((const RpGeometry *)NULL);
            }
        }

        if (kf.normalsPresent)
        {
            if (!RwStreamWriteReal(stream, (RwReal *) (geometry->morphTarget)[i].normals,
                 sizeof(RwV3d) * geometry->numVertices))
            {
                RWRETURN((const RpGeometry *)NULL);
            }
        }
    }

    /* Save off materials */
    if (!_rpMaterialListStreamWrite(&geometry->matList, stream))
    {
        RWRETURN((const RpGeometry *)NULL);
    }

    /* GEOMETRY EXTENSION DATA */
    if (!rwPluginRegistryWriteDataChunks(&geometryTKList, stream, geometry))
    {
        /* Failed to write extension data */
        RWRETURN((const RpGeometry *)NULL);
    }

    RWRETURN(geometry);
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometryStreamRead is used to read a geometry from the specified
 * binary stream. Prior to this function call a binary geometry chunk must
 * have been found in the stream.
 *
 * The world plugin must be attached before using this function.
 *
 * The sequence to locate and read a geometry from a binary stream connected
 * to a disk file is as follows:
 * \code
   RwStream *stream;
   RpGeometry *NewGeometry;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if( stream )
   {
       if( RwStreamFindChunk(stream, rwID_GEOMETRY, NULL, NULL) )
       {
           NewGeometry = RpGeometryStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
  \endcode
 *
 * \param stream  Pointer to the binary stream.
 *
 * \return Returns pointer to the geometry if successful or NULL if there is
 * an error.
 *
 * \see RpGeometryStreamGetSize
 * \see RpGeometryStreamWrite
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 * \see RpWorldPluginAttach
 */
RpGeometry         *
RpGeometryStreamRead(RwStream * stream)
{
    RpGeometry         *geometry;
    _rpGeometry         geom;
    RwUInt32            version;

    RWAPIFUNCTION(RWSTRING("RpGeometryStreamRead"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           (RwUInt32 *)NULL, &version))
    {
        RWRETURN((RpGeometry *)NULL);
    }

    if ((version < rwLIBRARYBASEVERSION)
        || (version > rwLIBRARYCURRENTVERSION))
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpGeometry *)NULL);
    }

    if (RwStreamRead(stream, &geom, sizeof(geom)) != sizeof(geom))
    {
        RWRETURN((RpGeometry *)NULL);
    }

    (void)RwMemNative32(&geom, sizeof(geom));

    /* RpGeometryCreate will take care of the format */
    geometry =
        RpGeometryCreate(geom.numVertices, geom.numTriangles, geom.format);

    if (!geometry)
    {
        RWRETURN((RpGeometry *)NULL);
    }

    /* Add sufficient morph targets for the geometry to fill in */
    if (geom.numMorphTargets > 1)
    {
        if (RpGeometryAddMorphTargets(geometry, geom.numMorphTargets-1) < 0)
        {
            RpGeometryDestroy(geometry);
            RWRETURN((RpGeometry *)NULL);
        }
    }

    if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
    {
        if (geometry->numVertices)
        {
            /* Read prelighting information */
            if (geom.format & rpGEOMETRYPRELIT)
            {
                RwUInt32    sizeLum = geometry->numVertices * sizeof(RwRGBA);

                /* No conversion needed - it's made of chars */
                if (RwStreamRead(stream, geometry->preLitLum, sizeLum)
                    != sizeLum)
                {
                    /* Failed, so tidy up */
                    RpGeometryDestroy(geometry);
                    RWRETURN((RpGeometry *)NULL);
                }
            }

            /* Read texture coordinate information */
            if (geometry->numTexCoordSets > 0)
            {
                RwInt32         i;
                const RwUInt32  sizeTC = geometry->numVertices *
                                         sizeof(RwTexCoords);

                /* Read vertex texture coordinates - reals, remember */
                for (i = 0; i < geometry->numTexCoordSets; i++)
                {
                    RWASSERT(geometry->texCoords[i]);

                    if (!RwStreamReadReal(stream,
                                          (RwReal *)geometry->texCoords[i],
                                          sizeTC))
                    {
                        /* Failed, so tidy up */
                        RpGeometryDestroy(geometry);
                        RWRETURN((RpGeometry *)NULL);
                    }
                }
            }

            /* Read in the triangles */
            if (geometry->numTriangles)
            {
                RpTriangle  *destTri;
                RwInt32     numTris;
                RwUInt32    size;

                /* Load the triangle information */
                numTris = geometry->numTriangles;
                destTri = geometry->triangles;

                size = numTris * sizeof(_rpTriangle);
                if (RwStreamRead(stream, (void *) destTri, size) != size)
                {
                    RpGeometryDestroy(geometry);

                    RWRETURN((RpGeometry *)NULL);
                }
                (void)RwMemNative32((void *) destTri, size);

                /* These are unpacked "in-place" into RwUInt16 fields */
                while (numTris--)
                {
                    RwUInt16    hi,lo;
                    _rpTriangle *srceTri;

                    srceTri = (_rpTriangle *) destTri;

                    hi = (RwUInt16) (srceTri->vertex01 >> 16) & 0xFFFF;
                    lo = (RwUInt16) (srceTri->vertex01) & 0xFFFF;
                    destTri->vertIndex[0] = hi;
                    destTri->vertIndex[1] = lo;

                    hi = (RwUInt16) (srceTri->vertex2Mat >> 16) & 0xFFFF;
                    lo = (RwUInt16) (srceTri->vertex2Mat) & 0xFFFF;
                    destTri->vertIndex[2] = hi;
                    destTri->matIndex = lo;

                    destTri++;
                }
            }
        }
    }

    /* Morph targets */
    {
        RwInt32             i;

        for (i = 0; i < geometry->numMorphTargets; i++)
        {
            _rpMorphTarget      kf;
            RpMorphTarget      *morphTarget = RpGeometryGetMorphTarget(geometry, i);

            if (RwStreamRead(stream, &kf, sizeof(kf)) != sizeof(kf))
            {
                RpGeometryDestroy(geometry);
                RWRETURN((RpGeometry *)NULL);
            }

            (void)RwMemNative32(&kf, sizeof(kf));
            (void)RwMemFloat32ToReal(&kf.boundingSphere.center, sizeof(kf.boundingSphere.center));
            (void)RwMemFloat32ToReal(&kf.boundingSphere.radius, sizeof(kf.boundingSphere.radius));

            RpMorphTargetSetBoundingSphere(morphTarget, &kf.boundingSphere);

            /* Optimised for reading morph target vertices and normals
             * at the same time */
            if (kf.pointsPresent && kf.normalsPresent)
            {
                RWASSERT((morphTarget->normals - morphTarget->verts) == geometry->numVertices);

                if (!RwStreamReadReal(stream, (RwReal *) morphTarget->verts,
                     sizeof(RwV3d) * 2 * geometry->numVertices))
                {
                    RpGeometryDestroy(geometry);
                    RWRETURN((RpGeometry *)NULL);
                }
            }
            else
            {
                if (kf.pointsPresent)
                {
                    if (!RwStreamReadReal(stream, (RwReal *) morphTarget->verts,
                         sizeof(RwV3d) * geometry->numVertices))
                    {
                        RpGeometryDestroy(geometry);
                        RWRETURN((RpGeometry *)NULL);
                    }
                }
                if (kf.normalsPresent)
                {
                    if (!RwStreamReadReal(stream, (RwReal *) morphTarget->normals,
                         sizeof(RwV3d) * geometry->numVertices))
                    {
                        RpGeometryDestroy(geometry);
                        RWRETURN((RpGeometry *)NULL);
                    }
                }
            }
        }
    }

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_MATLIST,
                           (RwUInt32 *)NULL, &version))
    {
        RWRETURN((RpGeometry *)NULL);
    }

    if ((version < rwLIBRARYBASEVERSION)
        || (version > rwLIBRARYCURRENTVERSION))
    {
        RpGeometryDestroy(geometry);
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpGeometry *)NULL);
    }

    if (!_rpMaterialListStreamRead(stream, &geometry->matList))
    {
        RpGeometryDestroy(geometry);
        RWRETURN((RpGeometry *)NULL);
    }

#if defined(RWDEBUG)
    if (!(rpGEOMETRYNATIVE & RpGeometryGetFlags(geometry)))
    {
        RwInt32 i;
        RwInt16 matIndex;

        for (i = 0; i < geometry->numTriangles; i++)
        {
            matIndex = (geometry->triangles)[i].matIndex;
            RWASSERT(matIndex < geometry->matList.numMaterials);
        }
    }
#endif /* RWDEBUG */

    /* Read the plugin data whilst the geometry is locked */
    if (!rwPluginRegistryReadDataChunks(&geometryTKList, stream, geometry))
    {
        RpGeometryDestroy(geometry);
        RWRETURN((RpGeometry *)NULL);
    }

    /* Release it so that it can be tailored */
    if (!RpGeometryUnlock(geometry))
    {
        RpGeometryDestroy(geometry);
        RWRETURN((RpGeometry *)NULL);
    }

#if ( defined(RWDEBUG) )
    /* Is this geometry arranged ok? */
    {
        RwBool              result;

        RpGeometryIsCorrectlySorted(geometry, &result);

        if (result == FALSE)
        {
            /* Some meshes' ranges overlap - PL2 doesn't like this */
            RWERROR((E_RX_MESHES_RANGES_OVERLAP, stream, RWSTREAMTYPE(stream->type)));
        }
    }
#endif /* ( defined(RWDEBUG) ) */

    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(geometry);
}

/*
 * $ref _rpGeometryChunkInfoRead _rpGeometryChunkInfoRead extracts
 * Chunk Info data from a RenderWare stream. The data is converted
 * from its original format and inserted into an RpGeometryChunkInfo
 * structure. A pointer to this structure is returned on success.
 *
 * $param stream  Pointer to the stream from which to read the chunk info.
 * $param geometryChunkInfo  Pointer to an RpGeometryChunkInfo object.
 * $param bytesRead  Pointer to a RwInt32 which will be used to return the
 * number of bytes read.
 *
 * $return Returns pointer to RpGeometryChunkInfo if successful or NULL
 * if there is an error
 *
 * $see RpGeometryStreamRead
 *
 */
RpGeometryChunkInfo *
_rpGeometryChunkInfoRead(RwStream * stream,
                         RpGeometryChunkInfo * geometryChunkInfo,
                         RwInt32 * bytesRead)
{
    RwUInt32            size, readSize, version;

    RWFUNCTION(RWSTRING("_rpGeometryChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(geometryChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT, &size, &version))
    {
        RWRETURN((RpGeometryChunkInfo *)NULL);
    }

    RWASSERT(size >= sizeof(RpGeometryChunkInfo));
    memset(geometryChunkInfo, 0, sizeof(RpGeometryChunkInfo));
    readSize = sizeof(RpGeometryChunkInfo);

    if (RwStreamRead(stream, geometryChunkInfo, readSize) != readSize)
    {
        RWRETURN((RpGeometryChunkInfo *)NULL);
    }

    /* Move on to known place */
    RwStreamSkip(stream, size - readSize);

    /* Include chunk header in bytes read */
    *bytesRead = size + (sizeof(RwInt32) * 3);

    (void)RwMemNative32(geometryChunkInfo, readSize);

    RWRETURN(geometryChunkInfo);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rpgeometry
 * \ref RpGeometryGetFlags is used to retrieve the property flags
 * associated with the specified geometry. For a list for property flags, 
 * see \ref RpGeometryFlag.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 *
 * \return Returns a 32-bit integer bit-field whose value is the bit-wise OR of
 * \ref RpGeometryFlag values if successful or zero if there is
 * an error or if the geometry's flags are zero.
 *
 * \see RpGeometrySetFlags
 * \see RpGeometryCreate
 * \see RpMeshSetTriStripMethod
 * \see RpWorldPluginAttach
 *
 */

RwUInt32
RpGeometryGetFlags(const RpGeometry *geometry)
{
    RWAPIFUNCTION(RWSTRING("RpGeometryGetFlags"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometryGetFlagsMacro(geometry));
}

/**
 * \ingroup rpgeometry
 * \ref RpGeometrySetFlags is used to modify the property flags for the
 * given geometry. For a list for property flags, see \ref RpGeometryFlag.
 *
 * Note that the new flag settings will completely replace the existing ones.
 * Developers can logically OR (or add) flags together to combine them.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param geometry  Pointer to the geometry.
 * \param flags  A 32-bit integer bit-field specifying the geometry's properties.
 *
 * \return Returns pointer to the geometry if successful or NULL if there is
 * an error.
 *
 * \see RpGeometryGetFlags
 * \see RpGeometryCreate
 * \see RpWorldPluginAttach
 *
 */

RpGeometry *
RpGeometrySetFlags(RpGeometry *geometry, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RpGeometrySetFlags"));
    RWASSERT(geometryModule.numInstances);
    RWASSERT(geometry);
    RWASSERTISTYPE(geometry, rpGEOMETRY);
    RWASSERT(0 < geometry->refCount);

    RWRETURN(RpGeometrySetFlagsMacro(geometry, flags));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */
