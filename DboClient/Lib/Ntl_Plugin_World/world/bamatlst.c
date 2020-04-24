/***************************************************************************
 *                                                                         *
 * Module  : bamatlst.c                                                    *
 *                                                                         *
 * Purpose : Material list                                                 *
 *                                                                         *
 **************************************************************************/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include <rwcore.h>

#include "baworld.h"
#include "bamateri.h"
#include "bamatlst.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define rwMATERIALLISTGRANULARITY   20

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                          Material list

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/****************************************************************************
 _rpMaterialListDeinitialize

 On entry   : Material list
 On exit    : TRUE on success
 */

RpMaterialList     *
_rpMaterialListDeinitialize(RpMaterialList * matList)
{
    RpMaterial        **materialArray;

    RWFUNCTION(RWSTRING("_rpMaterialListDeinitialize"));
    RWASSERT(matList);

    materialArray = matList->materials;
    if (materialArray)
    {
        RwInt32             materialCount = matList->numMaterials;
        RwInt32             nI;

        for (nI = 0; nI < materialCount; nI++)
        {
            RpMaterialDestroy(materialArray[nI]);
            materialArray[nI] = (RpMaterial *)NULL;
        }

        RwFree(materialArray);
        materialArray = (RpMaterial **)NULL;
        matList->materials = materialArray;
    }

    /* Reset the structure */
    matList->numMaterials = 0;
    matList->space = 0;

    RWRETURN(matList);
}

/****************************************************************************
 _rpMaterialListInitialize

 On entry   : Material list
 On exit    : TRUE on success
 */

RpMaterialList     *
_rpMaterialListInitialize(RpMaterialList * matList)
{
    RWFUNCTION(RWSTRING("_rpMaterialListInitialize"));
    RWASSERT(matList);

    matList->space = 0;
    matList->materials = (RpMaterial **)NULL;
    matList->numMaterials = 0;

    RWRETURN(matList);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             On the list

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rpMaterialListCopy

 On entry   : MatList (OUT)
            : MatList
 On exit    : TRUE on success
 */

RpMaterial **
_rpMaterialListAlloc(RwUInt32 count)
{
    RpMaterial **result = (RpMaterial **)NULL;
    RwUInt32     memSize;

    RWFUNCTION(RWSTRING("_rpMaterialListAlloc"));

    memSize = count*sizeof(RpMaterial *);

    result = (RpMaterial **)RwMalloc(memSize,
        rwMEMHINTDUR_EVENT | rwID_MATLIST | rwMEMHINTFLAG_RESIZABLE);

    RWRETURN(result);
}

/****************************************************************************
 _rpMaterialListCopy

 On entry   : MatList (OUT)
            : MatList
 On exit    : TRUE on success
 */

RpMaterialList     *
_rpMaterialListCopy(RpMaterialList * matListOut,
                   const RpMaterialList * matListIn)
{
    RwInt32             i;

    RWFUNCTION(RWSTRING("_rpMaterialListCopy"));
    RWASSERT(matListIn);
    RWASSERT(matListOut);

    matListOut->numMaterials = matListIn->numMaterials;
    matListOut->space = matListIn->space;
    matListOut->materials = (RpMaterial **)NULL;

    if (matListIn->materials)
    {
        matListOut->materials = _rpMaterialListAlloc(matListOut->space);
        if (!matListOut->materials)
        {
            size_t              memSize =
                sizeof(RpMaterial *) * matListOut->space;
            RWERROR((E_RW_NOMEM, memSize));
            RWRETURN(FALSE);
        }

        /* Copy and add ref */
        for (i = 0; i < matListOut->numMaterials; i++)
        {
            matListOut->materials[i] = matListIn->materials[i];
            (void)RpMaterialAddRef(matListOut->materials[i]);
        }

        /* Zero all excess entries */
        while(i < matListOut->space)
        {
            matListOut->materials[i++] = (RpMaterial *)NULL;
        }
    }

    /* All done */
    RWRETURN(matListOut);
}

/****************************************************************************
 _Rpmateriallistgetmaterial

 On entry   : Material list
            : Material index
 On exit    : Material
 */

RpMaterial         *
_rpMaterialListGetMaterial(const RpMaterialList * matList,
                          RwInt32 matIndex)
{
    RWFUNCTION(RWSTRING("_rpMaterialListGetMaterial"));
    RWASSERT(matList);
    RWASSERT((matIndex >= 0) && (matIndex < matList->numMaterials));

    RWRETURN(matList->materials[matIndex]);
}

/******************************************************************************
 _rpMaterialListSetSize

 On entry   : MatList
            : required size
 On exit    : MatList
 */

RpMaterialList *
_rpMaterialListSetSize(RpMaterialList * matList, RwInt32 size)
{
    RWFUNCTION(RWSTRING("_rpMaterialListSetSize"));
    RWASSERT(matList);
    RWASSERT(size > 0);

    if (matList->space < size)
    {
        RpMaterial        **materials;
        size_t memSize = sizeof(RpMaterial *) * size;

        if (matList->materials)
        {
            materials = (RpMaterial **) RwRealloc(matList->materials, memSize,
                rwMEMHINTDUR_EVENT | rwID_MATLIST | rwMEMHINTFLAG_RESIZABLE);
        }
        else
        {
            materials = (RpMaterial **)RwMalloc(memSize,
                rwMEMHINTDUR_EVENT | rwID_MATLIST | rwMEMHINTFLAG_RESIZABLE);
        }

        if (!materials)
        {
            RWERROR((E_RW_NOMEM, memSize));
            RWRETURN((RpMaterialList *)NULL);
        }

        /* Shove in the new */
        matList->materials = materials;
        matList->space = size;
    }

    RWRETURN(matList);
}

/******************************************************************************
 _rpMaterialListAppendMaterial

 On entry   : MatList
            : Material to add
 On exit    : Index in the list of the added material - negative on failure
 */

RwInt32
_rpMaterialListAppendMaterial(RpMaterialList * matList,
                             RpMaterial * material)
{
    RpMaterial        **materials;
    RwUInt32            count;
    size_t              memSize;
#if (0)
    RwInt32             i;
#endif /* (0) */

    RWFUNCTION(RWSTRING("_rpMaterialListAppendMaterial"));
    RWASSERT(matList);
    RWASSERT(material);

    /* See if there is a blank entry we can use */
    /* Add it to the list */
    if (matList->space > matList->numMaterials)
    {
        materials = &(matList->materials)[matList->numMaterials];

        (*materials) = material;
        RpMaterialAddRef(material);

        matList->numMaterials++;

        RWRETURN(matList->numMaterials - 1);
    }

    /* Need to allocates some more space */
    count = (matList->space +
             rwMATERIALLISTGRANULARITY);

    memSize = sizeof(RpMaterial *) * count;

    if (matList->materials)
    {
        materials =
            (RpMaterial **) RwRealloc(matList->materials, memSize,
                  rwMEMHINTDUR_EVENT | rwID_MATLIST |
                  rwMEMHINTFLAG_RESIZABLE);
    }
    else
    {
        materials = _rpMaterialListAlloc(count);
    }

    if (!materials)
    {
        RWERROR((E_RW_NOMEM, memSize));
        RWRETURN(-1);
    }

    /* Shove in the new */
    matList->materials = materials;
    matList->space += rwMATERIALLISTGRANULARITY;

    materials[matList->numMaterials] = material;
    RpMaterialAddRef(material);
    matList->numMaterials++;

    RWRETURN(matList->numMaterials - 1);
}

/******************************************************************************
 _rpMaterialListFindMaterialIndex

 On entry   : MatList
            : Material to find index of
 On exit    : Index in the list of the material, or -1 if not found/error
 */

RwInt32
_rpMaterialListFindMaterialIndex(const RpMaterialList * matList,
                                const RpMaterial * material)
{
    RwInt32             numMats;

    RWFUNCTION(RWSTRING("_rpMaterialListFindMaterialIndex"));
    RWASSERT(matList);
    RWASSERT(material);

    numMats = matList->numMaterials;
    while (numMats-- > 0)
    {
        if (matList->materials[numMats] == material)
        {
            /* Found it */
            break;
        }
    }

    /* Didn't find it */
    RWRETURN(numMats);
}

/****************************************************************************
 *
 * Binary Format Functions
 */

static              RwUInt32
MaterialListStreamGetSizeActual(const RpMaterialList * matList)
{
    RwUInt32            size;

    RWFUNCTION(RWSTRING("MaterialListStreamGetSizeActual"));
    RWASSERT(matList);

    size = sizeof(matList->numMaterials);
    size += matList->numMaterials * sizeof(RwInt32);
    RWRETURN(size);
}

/****************************************************************************
 _rpMaterialListStreamGetSize

 On entry   :
 On exit    : Size of Binary MatList
 */

RwUInt32
_rpMaterialListStreamGetSize(const RpMaterialList * matList)
{
    RwUInt32            size = 0;
    RwInt32             i;

    RWFUNCTION(RWSTRING("_rpMaterialListStreamGetSize"));
    RWASSERT(matList);

    /* Size of material list */
    size = ( MaterialListStreamGetSizeActual(matList) +
             rwCHUNKHEADERSIZE );

    for (i = 0; i < matList->numMaterials; i++)
    {
        RwInt32             j;

        j = i;
        while (j-- )
        {
            if ( matList->materials[j] == matList->materials[i])
                break;
        }


        /* is an as-yet unreferenced material */
        if (j < 0)
        {
            size += ( RpMaterialStreamGetSize(matList->materials[i]) +
                      rwCHUNKHEADERSIZE );
        }
    }

    /* All done */
    RWRETURN(size);
}

/****************************************************************************
 _rpMaterialListStreamWrite

 On entry   : Stream to write to
 On exit    :
 */
const RpMaterialList *
_rpMaterialListStreamWrite(const RpMaterialList * matList,
                          RwStream * stream)
{
    RwInt32             i, j;

    RWFUNCTION(RWSTRING("_rpMaterialListStreamWrite"));
    RWASSERT(matList);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_MATLIST,
                                  _rpMaterialListStreamGetSize(matList)))
    {
        RWRETURN((const RpMaterialList *)NULL);
    }

    /* wrap structured data */
    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT,
                                  MaterialListStreamGetSizeActual
                                  (matList)))
    {
        RWRETURN((const RpMaterialList *)NULL);
    }

    if (!RwStreamWriteInt(stream, &matList->numMaterials,
                          sizeof(matList->numMaterials)))
    {
        RWRETURN(FALSE);
    }

    for (i = 0; i < matList->numMaterials; i++)
    {
        j = i;
        while (j--)
        {
            if ( matList->materials[j] == matList->materials[i] )
                break;
        }

        if (!RwStreamWriteInt(stream, &j, sizeof(j)))
        {
            RWRETURN(FALSE);
        }
    }

    for (i = 0; i < matList->numMaterials; i++)
    {
        j = i;
        while (j--)
        {
            if ( matList->materials[j] == matList->materials[i] )
                break;
        }

        /* is an as yet unreferenced material */
        if (j < 0)
        {
            if (!RpMaterialStreamWrite(matList->materials[i], stream))
            {
                RWRETURN(FALSE);
            }
        }
    }

    RWRETURN(matList);
}

/****************************************************************************
 _rpMaterialListStreamRead

 On entry   : Stream to read from
 On exit    : MatList created
 */
RpMaterialList     *
_rpMaterialListStreamRead(RwStream * stream, RpMaterialList * matList)
{
    RwInt32             i, len, *matindex;
    RwUInt32            size, version;

    RWFUNCTION(RWSTRING("_rpMaterialListStreamRead"));
    RWASSERT(stream);
    RWASSERT(matList);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RpMaterialList *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION)
        && (version <= rwLIBRARYCURRENTVERSION))
    {
        RwBool status;

        status = (NULL != RwStreamReadInt(stream, &len, sizeof(len)));

        if (!status)
        {
            RWRETURN((RpMaterialList *)NULL);
        }
        _rpMaterialListInitialize(matList);

        if (len == 0)
        {
            /* zero entry material list simply return the initialized
               matList we've created */
            RWRETURN(matList);
        }

        /* make the list as large as needed */
        if (!_rpMaterialListSetSize(matList, len))
        {
            _rpMaterialListDeinitialize(matList);
            RWRETURN((RpMaterialList *)NULL);
        }


        RWASSERT(size == (sizeof(len) + len * sizeof(RwInt32)));
        matindex = (RwInt32 *) RwMalloc(sizeof(RwInt32) * len,
            rwMEMHINTDUR_FUNCTION | rwID_MATERIALMODULE);
        RWASSERT(matindex != (RwInt32 *)NULL);

        status = (NULL !=
                  RwStreamReadInt(stream, matindex, sizeof(RwInt32) * len));

        if (!status)
        {
            RwFree(matindex);
            _rpMaterialListDeinitialize(matList);
            RWRETURN((RpMaterialList *)NULL);
        }

        for (i = 0; i < len; i++)
        {
            RpMaterial         *material;

            /* new material */
            if (matindex[i] < 0)
            {
                if (!RwStreamFindChunk(stream, (RwUInt32)rwID_MATERIAL,
                                       (RwUInt32 *)NULL, &version))
                {
                    RwFree(matindex);
                    _rpMaterialListDeinitialize(matList);
                    RWRETURN((RpMaterialList *)NULL);
                }
                if ((version >= rwLIBRARYBASEVERSION)
                    && (version <= rwLIBRARYCURRENTVERSION))
                {
                    if (!(material = RpMaterialStreamRead(stream)))
                    {
                        RwFree(matindex);
                        _rpMaterialListDeinitialize(matList);
                        RWRETURN((RpMaterialList *)NULL);
                    }
                }
                else
                {
                    RWERROR((E_RW_BADVERSION));
                    RwFree(matindex);
                    _rpMaterialListDeinitialize(matList);
                    RWRETURN((RpMaterialList *)NULL);
                }
            }
            else
            {
                material =
                    _rpMaterialListGetMaterial(matList, matindex[i]);
                RpMaterialAddRef(material);
            }

            /* Add the material to the end of the list - this bumps up the
             * reference count
             */
            _rpMaterialListAppendMaterial(matList, material);

            /* We want to drop the reference count back down, so that only
             * the material list owns it (so it goes away when the material
             * list does.
             */
            RpMaterialDestroy(material);
        }

        RwFree(matindex);

        /* All done */
        RWRETURN(matList);
    }
    else
    {
        RWERROR((E_RW_BADVERSION));
        RWRETURN((RpMaterialList *)NULL);
    }
}

