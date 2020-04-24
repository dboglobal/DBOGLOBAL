/*
 * Stub Driver
 *
 * Copyright (C) 2000 Criterion Software Limited
 *
 */

/*
 * Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* RenderWare types */
#include "batypes.h"
#include "batype.h"
#include "balibtyp.h"
#include "barwtyp.h"

/* Abstraction of string functionality  -- for unicode support */
#include "rwstring.h"

#include "bamemory.h"
#include "badevice.h"
#include "batextur.h"
#include "baimmedi.h"
#include "baraster.h"

/****************************************************************************
 Local 2defines
 */

#define PRIMARYSUBSYSTEM    0 /* Only one system and that's the primary */
#define NUMSUBSYSTEMS       1 /* Assume only 1 sub system and it is the primary */

/****************************************************************************
 Local types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Globals (across program)
 */

RwRwDeviceGlobals       dgGGlobals; /* Defined here, externed in barwtyp.h */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   'Standard' Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwStubStandard

 On entry   : pOut
            : pInOut
            : nIn
 On exit    : FALSE
 */
static RwBool
_rwStubStandard(void *pOut __RWUNUSED__,
                void *pInOut __RWUNUSED__,
                RwInt32 nIn __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwStubStandard"));

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwSbDeviceSystemStandards

 On entry   :
 On exit    :
 */
static RwBool
_rwSbDeviceSystemStandards(void *out, void *inOut __RWUNUSED__,
                           RwInt32 numStandardsFunctions)
{
    RwInt32         i;
    RwInt32         numDriverFunctions;
    RwStandardFunc  *standardFunctions;
    RwStandard      rwSbStandards[] =
    {
        /* Raster/Pixel operations */
        {rwSTANDARDRGBTOPIXEL,              _rwStubStandard},
        {rwSTANDARDPIXELTORGB,              _rwStubStandard},
        {rwSTANDARDRASTERSETIMAGE,          _rwStubStandard},
        {rwSTANDARDIMAGEGETRASTER,          _rwStubStandard},

        /* Raster creation and destruction */
        {rwSTANDARDRASTERDESTROY,           _rwStubStandard},
        {rwSTANDARDRASTERCREATE,            _rwStubStandard},

        /* Finding about a raster type */
        {rwSTANDARDIMAGEFINDRASTERFORMAT,   _rwStubStandard},

        /* Locking and releasing */
        {rwSTANDARDRASTERLOCK,              _rwStubStandard},
        {rwSTANDARDRASTERUNLOCK,            _rwStubStandard},
        {rwSTANDARDRASTERLOCKPALETTE,       _rwStubStandard},
        {rwSTANDARDRASTERUNLOCKPALETTE,     _rwStubStandard},

        /* Creating sub rasters */
        {rwSTANDARDRASTERSUBRASTER,         _rwStubStandard},

        /* Native texture serialization */
        {rwSTANDARDNATIVETEXTUREGETSIZE,    _rwStubStandard},
        {rwSTANDARDNATIVETEXTUREWRITE,      _rwStubStandard},
        {rwSTANDARDNATIVETEXTUREREAD,       _rwStubStandard},

        /* Raster Mip Levels */
        {rwSTANDARDRASTERGETMIPLEVELS,      _rwStubStandard}
    };

    RWFUNCTION(RWSTRING("_rwSbDeviceSystemStandards"));

    standardFunctions = (RwStandardFunc *)out;
    numDriverFunctions = sizeof(rwSbStandards) / sizeof(RwStandard);

    /* Clear out all of the standards initially */
    for (i = 0; i < numStandardsFunctions; i++)
    {
        standardFunctions[i] = _rwStubStandard;
    }

    /* Fill in all of the standards */
    while (numDriverFunctions--)
    {
        if ((rwSbStandards->nStandard < numStandardsFunctions) &&
            (rwSbStandards->nStandard >= 0))
        {
            standardFunctions[rwSbStandards[numDriverFunctions].nStandard] =
                rwSbStandards[numDriverFunctions].fpStandard;
        }
    }

    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                            System handler

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

 /****************************************************************************
 _rwSbSetRenderState

 On entry   : Render state to set
            : Parameter
 On exit    : TRUE on success else FALSE.
 */
static RwBool
_rwSbSetRenderState(RwRenderState state __RWUNUSED__,
                        void *param __RWUNUSED__)

{
    RWFUNCTION(RWSTRING("_rwSbSetRenderState"));

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwSbGetRenderState

 On entry   : Render state to get.
            : Parameter (OUT).
 On exit    : TRUE on success else FALSE.
 */
static RwBool
_rwSbGetRenderState(RwRenderState state __RWUNUSED__,
                    void *param __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwSbGetRenderState"));

    RWRETURN(TRUE);
}

 /****************************************************************************
 _rwSbIm2DRenderLine

 On entry   : vertices, num vertices in array
            : index of line ends
 On exit    :
 */
static RwBool
_rwSbIm2DRenderLine(RwIm2DVertex *verts __RWUNUSED__,
                    RwInt32 numVerts __RWUNUSED__,
                    RwInt32 vert1 __RWUNUSED__,
                    RwInt32 vert2 __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwSbIm2DRenderLine"));

    RWRETURN(TRUE);
}

 /****************************************************************************
 _rwSbIm2DRenderTriangle

 On entry   : vertices, num vertices in array
            : index of triangle vertices
 On exit    :
 */
static RwBool
_rwSbIm2DRenderTriangle(RwIm2DVertex *verts __RWUNUSED__,
                        RwInt32 numVerts __RWUNUSED__,
                        RwInt32 vert1 __RWUNUSED__,
                        RwInt32 vert2 __RWUNUSED__,
                        RwInt32 vert3 __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwSbIm2DRenderTriangle"));

    RWRETURN(TRUE);
}

 /****************************************************************************
 _rwSbIm2DRenderPrimitive

 On entry   : Primitive type
            : Vertices
            : Num Vertices
 On exit    :
 */
static RwBool
_rwSbIm2DRenderPrimitive(RwPrimitiveType primType __RWUNUSED__,
                         RwIm2DVertex *verts __RWUNUSED__,
                         RwInt32 numVerts __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwSbIm2DRenderPrimitive"));

    RWRETURN(TRUE);
}

 /****************************************************************************
 _rwSbIm2DRenderIndexedPrimitive

 On entry   : Primitive type
            : Vertices
            : Num Vertices
 On exit    :
 */
static RwBool
_rwSbIm2DRenderIndexedPrimitive(RwPrimitiveType primType __RWUNUSED__,
                                RwIm2DVertex *verts __RWUNUSED__,
                                RwInt32 numVerts __RWUNUSED__,
                                RwImVertexIndex *indices __RWUNUSED__,
                                RwInt32 numIndices __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwSbIm2DRenderIndexedPrimitive"));

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwSbSystem

 On entry   : Request
            : Data out
            : Data in/out
            : Number in
 On exit    : TRUE on success else FALSE.
 */
static RwBool
_rwSbSystem(RwInt32 request, void *out, void *inOut, RwInt32 in)
{
    RWFUNCTION(RWSTRING("_rwSbSystem"));

    switch(request)
    {
        case rwDEVICESYSTEMUSEMODE:
            RWRETURN(TRUE);

        case rwDEVICESYSTEMGETNUMMODES:
            *((RwInt32 *)out) = 1;
            RWRETURN(TRUE);

        case rwDEVICESYSTEMGETMODEINFO:
            RWRETURN(TRUE);

        case rwDEVICESYSTEMGETMODE:
            *((RwInt32 *)out) = 0;
            RWRETURN(TRUE);

        case rwDEVICESYSTEMFOCUS:
            RWRETURN(TRUE);

            /*** Register in the library block ***/

        case rwDEVICESYSTEMREGISTER:
            {
                RwDevice            *DlDriverDevice = _rwDeviceGetHandle();
                RwDevice            *deviceOut = (RwDevice*)out;
                RwMemoryFunctions   *memoryFunctions = (RwMemoryFunctions*)inOut;

                *deviceOut = *DlDriverDevice;
                dgGGlobals.memFuncs = memoryFunctions;

                RWRETURN(TRUE);
            }

            /******************* Opening/Closing **********************/

        case rwDEVICESYSTEMOPEN:
            RWRETURN(TRUE);

        case rwDEVICESYSTEMCLOSE:
            RWRETURN(TRUE);

            /********* Selecting a rendering sub system ********/

        case rwDEVICESYSTEMGETNUMSUBSYSTEMS:
            {
                RwInt32 *numSubSystems = (RwInt32 *)out;

                *numSubSystems = NUMSUBSYSTEMS;

                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMGETSUBSYSTEMINFO:
            {
#if (0)
                RwSubSystemInfo    *subSystemInfo = (RwSubSystemInfo *)out;
#endif /* (0) */
                RwInt32             subSystemIndex = (RwInt32)in;
                RwBool result =    (subSystemIndex == PRIMARYSUBSYSTEM);

                if (!result)
                {
                    RWERROR((E_RW_DEVICEERROR,
                             RWSTRING("Invalid subsystem number")));
                }

                RWRETURN(result);
            }

        case rwDEVICESYSTEMGETCURRENTSUBSYSTEM:
            {
                RwInt32 *currentSubSystem = (RwInt32 *)out;

                *currentSubSystem = PRIMARYSUBSYSTEM;

                RWRETURN(TRUE);
            }

        case rwDEVICESYSTEMSETSUBSYSTEM:
            {
                if (in != PRIMARYSUBSYSTEM)
                {
                    RWRETURN(FALSE);
                }
                else
                {
                    RWRETURN(TRUE);
                }
            }

            /************** Starting stopping ******************/

            /* Start means that this bit of hardware has been selected for
            * rendering - so get it into a state ready for rendering */

        case rwDEVICESYSTEMSTART:
            RWRETURN(TRUE);

        case rwDEVICESYSTEMSTOP:
            RWRETURN(TRUE);

            /************* standard device functions ************************/

        case rwDEVICESYSTEMSTANDARDS:
            RWRETURN(_rwSbDeviceSystemStandards(out, inOut, in));

        case rwDEVICESYSTEMINITPIPELINE:
            break;

        case rwDEVICESYSTEMGETTEXMEMSIZE:
            break;

        case rwDEVICESYSTEMFINALIZESTART:
            break;

        case rwDEVICESYSTEMINITIATESTOP:
            break;

        case rwDEVICESYSTEMGETMAXTEXTURESIZE:
            break;

        case rwDEVICESYSTEMRXPIPELINEREQUESTPIPE:
            break;

        case rwDEVICESYSTEMGETMETRICBLOCK:
            break;
    }

    RWRETURN(FALSE);
}

/****************************************************************************
 _rwDeviceGetHandle

 On entry   :
 On exit    : Device block handle
 */

RwDevice *
_rwDeviceGetHandle(void)
{
    static RwDevice rwSbDriverDevice =
    {
        1.0f,                               /* default gamma correction */
        _rwSbSystem,                        /* System handler */
        (RwReal) 0.0,                       /* near Z buffer limit */
        (RwReal) 1.0,                       /* nearfar Z buffer limit */
        _rwSbSetRenderState,                /* RwRenderStateSetFunction */
        _rwSbGetRenderState,                /* RwRenderStateGetFunction */
        _rwSbIm2DRenderLine,                /* RwIm2DRenderLineFunction */
        _rwSbIm2DRenderTriangle,            /* RwIm2DRenderTriangleFunction */
        _rwSbIm2DRenderPrimitive,           /* RwIm2DRenderPrimitiveFunction */
        _rwSbIm2DRenderIndexedPrimitive,    /* RwIm2DRenderIndexedPrimitiveFunction */
        /* These get set up when the 3D Immediate mode module is setup */
        (RwIm3DRenderLineFunction)NULL,
        (RwIm3DRenderTriangleFunction)NULL,
        (RwIm3DRenderPrimitiveFunction)NULL,
        (RwIm3DRenderIndexedPrimitiveFunction)NULL
    };

    RWFUNCTION(RWSTRING("_rwDeviceGetHandle"));

    RWRETURN(&rwSbDriverDevice);
}

