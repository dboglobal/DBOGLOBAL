/***************************************************************************
 *                                                                         *
 * Module  : bamatlst.h                                                    *
 *                                                                         *
 * Purpose : Material list handling                                        *
 *                                                                         *
 **************************************************************************/

#ifndef RWMATLST_H
#define RWMATLST_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "bamateri.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/****************************************************************************
 Global Types
 */

typedef struct RpMaterialList RpMaterialList;

#if (!defined(DOXYGEN))
struct RpMaterialList
{
    RpMaterial     **materials;
    RwInt32        numMaterials;
    RwInt32        space;
};
#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#define rpMaterialListGetNumMaterials(mlist) ((mlist)->numMaterials)

/* Setting up and destroying material lists */
extern RpMaterialList *_rpMaterialListInitialize(RpMaterialList *matList);
extern RpMaterialList *_rpMaterialListDeinitialize(RpMaterialList *matList);

/* Accessing material lists */
extern RpMaterial     ** _rpMaterialListAlloc(RwUInt32 count);
extern RpMaterial *_rpMaterialListGetMaterial(const RpMaterialList *matList,
                                             RwInt32 matIndex);
extern RpMaterialList * _rpMaterialListSetSize(RpMaterialList * matList, 
                                               RwInt32 size);
extern RpMaterialList *_rpMaterialListCopy(RpMaterialList *matListOut,
                                          const RpMaterialList *matListIn);
extern RwInt32 _rpMaterialListAppendMaterial(RpMaterialList *matList,
                                            RpMaterial *material);
extern RwInt32 _rpMaterialListFindMaterialIndex(const RpMaterialList *matList,
                                               const RpMaterial *material);

/* Binary format */
extern RwUInt32 _rpMaterialListStreamGetSize(const RpMaterialList *matList);
extern RpMaterialList *_rpMaterialListStreamRead(RwStream *stream,
                                                RpMaterialList *matList);
extern const RpMaterialList *_rpMaterialListStreamWrite(const RpMaterialList *matList,
                                                       RwStream *stream);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define rpMaterialListInitialize(_matList) \
    _rpMaterialListInitialize(_matList)

#define rpMaterialListDeinitialize(_matList) \
    _rpMaterialListDeinitialize(_matList)

#define rpMaterialListGetMaterial(_matList, _matIndex) \
    _rpMaterialListGetMaterial(_matList, _matIndex)

#define rpMaterialListCopy(_matListOut, _matListIn) \
    _rpMaterialListCopy(_matListOut, _matListIn)

#define rpMaterialListAppendMaterial(_matList, _material) \
    _rpMaterialListAppendMaterial(_matList, _material)

#define rpMaterialListStreamRead(_stream, _matList) \
    _rpMaterialListStreamRead(_stream, _matList)

#define rpMaterialListStreamWrite(_matList, _stream) \
    _rpMaterialListStreamWrite(_matList, _stream)

/* RWPUBLICEND */

#endif /* RWMATLST_H */


