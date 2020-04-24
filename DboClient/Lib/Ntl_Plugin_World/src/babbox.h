/**************************************************************************
 * Module  : babbox.h                                                      *
 *                                                                         *
 * Purpose : World handling functions.                                     *
 *                                                                         *
 **************************************************************************/

#ifndef RWBBOX_H
#define RWBBOX_H

/****************************************************************************
 Includes
 */

#include "batype.h"
#include "balibtyp.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global types
 */

typedef struct RwBBox RwBBox;
/**
 * \ingroup rwbbox
 * \struct RwBBox
 * This type represents a 3D axis-aligned bounding-box
 * specified by the positions of two corners which lie on a diagonal.
 * Typically used to specify a world bounding-box when the world is created
 * 
 * \param sup Supremum vertex (contains largest values)
 * \param inf Infimum vertex (contains smallest values)
 * 
 * \see RpWorldCreate
 */
struct RwBBox
{
    /* Must be in this order */
    RwV3d sup;   /**< Supremum vertex. */
    RwV3d inf;   /**< Infimum vertex. */
};

#if (!defined(RwBBoxAssign))
#define RwBBoxAssign(_target, _source)            \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwBBoxAssign)) */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */


extern RwBBox *RwBBoxCalculate(RwBBox *boundBox,
                               const RwV3d *verts,
                               RwInt32 numVerts);
extern RwBBox *RwBBoxInitialize(RwBBox *boundBox,
                                const RwV3d *vertex);
extern RwBBox *RwBBoxAddPoint(RwBBox *boundBox,
                              const RwV3d *vertex);
extern RwBool RwBBoxContainsPoint(const RwBBox *boundBox,
                                  const RwV3d *vertex);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWBBOX_H */

