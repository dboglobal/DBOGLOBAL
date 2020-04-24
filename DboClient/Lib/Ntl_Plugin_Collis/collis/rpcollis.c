/*
 * RenderWare Graphics Collision Plugin
 *
 * File:    rpcollis.c
 * Purpose: Top level docs
 */

/**
 * \defgroup rpcollis RpCollision
 * \ingroup collisiondetection
 *
 * Collision Plugin for RenderWare Graphics.
 */

/**
 * \ingroup rpcollis
 * \page rpcollisoverview RpCollision Plugin Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h, rtintsec.h, rpcollis.h
 * \li \b Libraries: rwcore, rpworld, rtintsec, rpcollis
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach,
 *      \ref RpCollisionPluginAttach
 *
 * <b> Collision data </b>
 *
 * The collision plugin allows an \ref RpWorldSector or \ref RpGeometry
 * to be extended with a 'collision tree'. This works much like the 'BSP'
 * tree that defines the large-scale sectorization of an \ref RpWorld, but
 * continues the sectorization down to the level of a few triangles per
 * leaf node. The collision tree does not store any triangle data
 * directly, but references the original triangle array in the world
 * sector or geometry.
 *
 * The additional collision data greatly improves the efficiency of finding
 * triangle intersections, and various functions are provided to do this.
 * Other functions provide coarse-grain intersection tests with
 * whole world sectors or atomics via their bounding spheres.
 *
 * For best triangle intersection performance, it is advisable to use
 * low-resolution collision models separate from those to be rendered.
 * However, if collision with the graphics data is desired then the object
 * should not be preinstanced as this process removes the platform
 * independent vertex/triangle data (see \ref RpWorldInstance).
 *
 * <b> Managing collision data </b>
 *
 * Collision data for a world sector or geometry can be built using
 * \ref RpCollisionWorldBuildData, and \ref RpCollisionGeometryBuildData
 * (this might commonly be done when exporting a model from an art package).
 * The existence of the data can be tested using
 * \ref RpCollisionWorldQueryData etc, and it may be destroyed using
 * \ref RpCollisionWorldDestroyData etc. Collision data, if it exists, is
 * streamed automatically with a world sector or geometry as plugin extension
 * data.
 *
 * <b> Low-level Sector Iterator API </b>
 *
 * \li \ref RpSectorLineIt
 * \li \ref RpSectorBBoxIt
 * \li \ref RpCollSectorLineIt
 * \li \ref RpCollSectorBBoxIt
 *
 * This API provides methods for iterating over a world sector tree at the 
 * large scale or collision tree at the small scale to find the leaf sectors 
 * that intersect a line or box. For a collision tree, the leaf nodes simply 
 * reference triangles in an external array, and these may be processed in 
 * a variety of ways by the application. For example, this might involve doing 
 * precise triangle intersection tests using the functions in the intersection 
 * toolkit (\ref rtintersectionoverview).
 *
 * The collision tree of a world sector or geometry may be obtained using
 * \ref RpCollisionWorldSectorGetCollTree or
 * \ref RpCollisionGeometryGetCollTree.
 *
 * <b> Intersection Test API </b>
 *
 * This API is built on the low-level sector iterators and provides all-in-one
 * functions that return, via a callback, each intersection
 * found between a given primitive (\ref RpIntersection) and target:
 *
 * \li \ref RpWorldForAllWorldSectorIntersections finds intersections
 *     with world sectors in a world, based on their bounding boxes.
 * \li \ref RpWorldForAllAtomicIntersections finds intersections with
 *     atomics in a world, based on their bounding spheres.
 * \li \ref RpCollisionWorldForAllIntersections finds intersections
 *     with the triangles of the static geometry of a world.
 * \li \ref RpCollisionGeometryForAllIntersections finds intersections
 *     with triangles of a geometry in local space.
 * \li \ref RpAtomicForAllIntersections finds intersections with the
 *     triangles of an atomic in world space. Morphed atomics are handled
 *     but this can be slow as it involves interpolating and testing every
 *     triangle of the geometry (a collision tree can only be used for
 *     rigid geometries).
 *
 * The RpCollision prefix is used for the functions that operate on
 * an object extended with collision plugin data.
 */

#if 0

#include "rwcore.h"

#endif
