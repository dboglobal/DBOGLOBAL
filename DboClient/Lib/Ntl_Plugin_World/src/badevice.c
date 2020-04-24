/*
 * Device handling.
 *
 * Devices are the libraries interface onto a piece of rendering technology
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwengine
 * \page rwengineoverview RwEngine Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection engineoverview Overview
 *
 * This object exposes RenderWare Graphics engine. The engine manages:
 *
 * \li memory handling
 * \li plugin registration;
 * \li enumerating and querying available output devices;
 * \li setting and querying video modes;
 * \li Texture memory;
 * \li obtaining focus in a windowed environment;
 * \li Metrics (in Metrics Builds only) and;
 * \li initializing and terminating RenderWare Graphics itself.
 *
 * All RenderWare Graphics applications must Initialize, Open, then Start the RenderWare Graphics
 * Engine before entering their main loop.
 *
 * All plugins must be registered - by calling their respective "PluginAttach()"
 * functions - prior to calling \ref RwEngineStart.
 *
*/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "balibtyp.h"

#include "bamemory.h"

/* String abstraction API for unicode support */
#include "rwstring.h"

/* The core modules */
#include "baerr.h"
#include "bavector.h"
#include "bamatrix.h"
#include "baframe.h"
#include "bastream.h"
#include "bacamera.h"
#include "baimage.h"
#include "baraster.h"
#include "batextur.h"
#include "bapipe.h"
#include "baim3d.h"
#include "baresour.h"
#include "rwgrp.h"

#include "badebug.h"
#include "drvmodel.h"
#include "drvfns.h"

#include "bapipe.h"
#include "batkreg.h"

/* devices !! */
#include "batkreg.h"
#include "badevice.h"

/* for the OutputDebugString function for RWMEMDEBUG builds */
#if defined(RWMEMDEBUG)
#if defined(_XBOX)
#include <xtl.h>
#elif defined(WIN32)
#include <windows.h>
#endif /* defined(WIN32) */
#endif /* defined(RWMEMDEBUG) */

#if (!defined(DOXYGEN))
/* We stick the build date and time in a local var here so that in a last
 * ditch effort, people who roll their own libs can tell when the build
 * happened using egrep.
 */
static const char  _rwBuildDateTime[] __RWUNUSED__ =
"\nCore built at " __DATE__ " " __TIME__ "\n";
#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local Static/Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals -- across program
 */

static RwUInt32     engineInstancesOpened = 0;

/* These are global to all instances */
static RwPluginRegistry engineTKList =
{ sizeof(RwGlobals),
  sizeof(RwGlobals),
#ifdef RWGLOBALSIZE
  RWGLOBALSIZE,
#else  /* RWGLOBALSIZE */
  0,
#endif  /* RWGLOBALSIZE */
  0,
  (RwPluginRegEntry *)NULL,
  (RwPluginRegEntry *)NULL };

/* The majority of globals across the library */
#ifndef RWGLOBALSIZE
void               *RwEngineInstance = NULL;
static RwGlobals    staticGlobals;
#else /* RWGLOBALSIZE */
RwUInt32            ourGlobals[RWGLOBALSIZE / sizeof(RwUInt32)];
#endif /* RWGLOBALSIZE */

RwInt8              _rwMsbBit[256] =
{ 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };

/****************************************************************************
 Local (static) Globals
 */


#if (defined(RWDEBUG) && defined(RWMEMDEBUG) && defined(_MSC_VER))
#if ((_MSC_VER>=1000) && defined(_DEBUG))
static _CrtMemState _InitialMemState;
#endif /* ((_MSC_VER>=1000) && defined(_DEBUG)) */
#endif /* (defined(RWDEBUG) && defined(RWMEMDEBUG) && defined(_MSC_VER)) */

#ifdef RWMETRICS

/****************************************************************************
 MetricsOpen

 On entry   : Instance, offset, size
 On exit    : Instance pointer on success
 */

static void        *
MetricsOpen(void *instance, RwInt32 offset,
               RwInt32 size __RWUNUSED__)
{
    RwMetrics          *metrics =
        (RwMetrics *) (((RwUInt8 *) instance) + offset);

    RWFUNCTION(RWSTRING("MetricsOpen"));
    RWASSERT(instance);

    /* Put the pointer to the metrics data block into the globals */
    RWSRCGLOBAL(metrics) = metrics;

    /* Zero the metrics values */
    metrics->numTriangles = 0;
    metrics->numProcTriangles = 0;
    metrics->numVertices = 0;
    metrics->numTextureUploads = 0;
    metrics->sizeTextureUploads = 0;
    metrics->numResourceAllocs = 0;
    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                                rwDEVICESYSTEMGETMETRICBLOCK,
                                &metrics->devSpecificMetrics, NULL, 0))
    {
        /* There are no device specific metrics */
        metrics->devSpecificMetrics = NULL;
    }

    RWRETURN(instance);
}

/****************************************************************************
 MetricsClose

 On entry   : Instance, offset, size
 On exit    : Instance pointer on success
 */

static void        *
MetricsClose(void *instance __RWUNUSED__,
                RwInt32 offset __RWUNUSED__, RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("MetricsClose"));

    /* May as well remove the metrics pointer from the globals */
    RWSRCGLOBAL(metrics) = (RwMetrics *) NULL;

    RWRETURN(instance);
}

#endif /* RWMETRICS */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Device layer

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 CorePluginAttach

 On entry   :
 On exit    : TRUE on success
 */

static              RwBool
CorePluginAttach(void)
{
    RwInt32             state = 0;

    RWFUNCTION(RWSTRING("CorePluginAttach"));

    /* Register a bunch of default modules */
    state |=
        RwEngineRegisterPlugin(sizeof(rwErrorGlobals), rwID_ERRORMODULE,
                               _rwErrorOpen, _rwErrorClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwVectorGlobals), rwID_VECTORMODULE,
                               _rwVectorOpen, _rwVectorClose);
    state |=
        RwEngineRegisterPlugin(0, rwID_COLORMODULE,
                               _rwColorOpen, _rwColorClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwMatrixGlobals), rwID_MATRIXMODULE,
                               _rwMatrixOpen, _rwMatrixClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwFrameGlobals), rwID_FRAMEMODULE,
                               _rwFrameOpen, _rwFrameClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwStreamGlobals), rwID_STREAMMODULE,
                               _rwStreamModuleOpen, _rwStreamModuleClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwCameraGlobals), rwID_CAMERAMODULE,
                               _rwCameraOpen, _rwCameraClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwImageGlobals), rwID_IMAGEMODULE,
                               _rwImageOpen, _rwImageClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwRasterGlobals), rwID_RASTERMODULE,
                               _rwRasterOpen, _rwRasterClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwTextureGlobals), rwID_TEXTUREMODULE,
                               _rwTextureOpen, _rwTextureClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwPipeGlobals), rwID_PIPEMODULE,
                               _rwRenderPipelineOpen, _rwRenderPipelineClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwChunkGroupGlobals), rwID_CHUNKGROUPMODULE,
                               _rwChunkGroupOpen, _rwChunkGroupClose);
#if 0
    state |=
        rwGetCorePipeInterface()->pipeAttach();
#endif /* 0/1 */
    state |=
        _rwPipeAttach();
    state |=
        RwEngineRegisterPlugin(sizeof(rwImmediGlobals), rwID_IMMEDIATEMODULE,
                               _rwIm3DOpen, _rwIm3DClose);
    state |=
        RwEngineRegisterPlugin(sizeof(rwResourcesGlobals), rwID_RESOURCESMODULE,
                               _rwResourcesOpen, _rwResourcesClose);
#ifdef RWMETRICS
    state |=
        RwEngineRegisterPlugin(sizeof(RwMetrics), rwID_METRICSMODULE,
                               MetricsOpen, MetricsClose);
#endif /* RWMETRICS */

    /* If all are positive, we've succeeded */
    if (state >= 0)
    {
        RWMESSAGE((RWSTRING("CorePluginAttach OK.")));
        RWRETURN(TRUE);
    }

    RWMESSAGE((RWSTRING("CorePluginAttach Failed.")));

    RWRETURN(FALSE);
}

static void *
MallocWrapper(RwFreeList *fl, RwUInt32 hint)
{
    RWFUNCTION(RWSTRING("MallocWrapper"));

    RWRETURN(RwMalloc(fl->entrySize, hint));
}

static RwFreeList *
FreeWrapper(RwFreeList *fl, void *pData)
{
    RWFUNCTION(RWSTRING("FreeWrapper"));

    RwFree(pData);

    RWRETURN(fl);
}

static              RwBool
EngineOpen(RwDevice * device, RwEngineOpenParams * initParams)
{
    RWFUNCTION(RWSTRING("EngineOpen"));

    /* Allocate some memory
     * - we need to do this first because
     *   the device grabs references to the memory
     * - the device is not re-entrant/thread safe currently.
     */

#ifndef RWGLOBALSIZE
    RwEngineInstance = RwMalloc(engineTKList.sizeOfStruct,
        rwMEMHINTDUR_GLOBAL);

    if (RwEngineInstance)
#endif /* RWGLOBALSIZE */
    {
#ifndef RWGLOBALSIZE
        void               *instance = RwEngineInstance;

        /* Copy across the staticGlobals
         * so we don't screw up the debug stuff
         */
        memcpy(RwEngineInstance, &staticGlobals, sizeof(RwGlobals));
#endif /* RWGLOBALSIZE */

        /* Register it first - this pulls the device block back from the
         * device for fast access, and also pushes down a reference to the
         * memory interface.
         */
        _rwDeviceSystemRequest(device,
                               rwDEVICESYSTEMREGISTER,
                               &RWSRCGLOBAL(dOpenDevice),
                               &RWSRCGLOBAL(memoryFuncs), 0);

        /* Try and start it up - this gets enough info set up in the driver that
         * the user can make a decision regarding rendering sub system and modes
         */
        if (_rwDeviceSystemRequest(device,
                                   rwDEVICESYSTEMOPEN, NULL,
                                   initParams, 0))
        {
            /* Set up the standard functions */
            _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTANDARDS,
                                   RWSRCGLOBAL(stdFunc), NULL,
                                   sizeof
                                   (RWSRCGLOBAL(stdFunc))
                                   / sizeof(RwStandardFunc));

            /* One more instance opened */
            engineInstancesOpened++;

            /* All done */
            RWRETURN(TRUE);
        }

#ifndef RWGLOBALSIZE
        /* Point back at the static instance
         * so we can blow away the RwEngineInstance
         */
        RwEngineInstance = (void *) (&staticGlobals);

        /* Copy across the staticGlobals
         * so we don't screw up the debug stuff
         */
        memcpy(RwEngineInstance, instance, sizeof(RwGlobals));

        /* Blow away the global variables */
        RwFree(instance);
        instance = NULL;
#endif /* RWGLOBALSIZE */

        /* Failed to open the device */
        RWRETURN(FALSE);
    }
#ifndef RWGLOBALSIZE
    else
    {
        /* Failed to allocate new globals memory so reset the
           RwEngineInstance pointer back to the staticGlobals
           so that the error handling works */
        RwEngineInstance = (void *) (&staticGlobals);
    }
#endif /* RWGLOBALSIZE */

    /* Failed to allocate some memory for the globals */
    RWERROR((E_RW_NOMEM, engineTKList.sizeOfStruct));
    RWRETURN(FALSE);
}

/****************************************************************************
 _rwDeviceSystemRequest

 On entry   : device pointer, request ID, rqeuest specific parameters
 On exit    : device pointer on success
 */

RwBool
_rwDeviceSystemRequest(RwDevice * device, RwInt32 requestID,
                       void *pOut, void *pInOut, RwInt32 numIn)
{
    RwBool result;

    RWFUNCTION(RWSTRING("_rwDeviceSystemRequest"));
    RWASSERT(device);

    result = (device->fpSystem) (requestID, pOut, pInOut, numIn);

    if (!result)
    {
        /* Catch the cases that we can provide default implementations for */
        switch (requestID)
        {
            case rwDEVICESYSTEMFINALIZESTART:
            case rwDEVICESYSTEMINITIATESTOP:
                /* It ain't the end of the world if these
                 * don't exist in the driver, so catch the error.
                 */
                result = TRUE;
                break;

            case rwDEVICESYSTEMGETNUMSUBSYSTEMS:
                /* Default to 1 sub system */
                *(RwInt32 *) pOut = 1;
                result = TRUE;
                break;

            case rwDEVICESYSTEMGETSUBSYSTEMINFO:
                /* Fill info in as only sub system */
                result = (numIn == 0);
                if (result)
                {
                    RwSubSystemInfo    *subSystemInfo =
                        (RwSubSystemInfo *) pOut;

                    rwstrcpy(subSystemInfo->name,
                             RWSTRING("Only rendering sub system"));
                }
                break;

            case rwDEVICESYSTEMGETCURRENTSUBSYSTEM:
                /* Fill in default as number 0 */
                *(RwInt32 *) pOut = 0;
                result = TRUE;
                break;

            case rwDEVICESYSTEMSETSUBSYSTEM:
                /* If it's sub system 0, everything's cool */
                result = (numIn == 0);
                break;

#if (0)
            default:
                break;
#endif /* (0) */
        }
    }

    if (!result)
    {
        /* Failure */
        RWERROR((E_RW_SYSTEMREQUEST, requestID));
    }

    RWRETURN(result);
}

/****************************************************************************
 _rwGetNumEngineInstances

 On entry   : None
 On exit    : Number of engine instances
 */
RwUInt32
_rwGetNumEngineInstances(void)
{
    RWFUNCTION(RWSTRING("_rwGetNumEngineInstances"));
    RWRETURN(engineInstancesOpened);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Getting library version

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwengine
 * \ref RwEngineGetVersion returns the version of the RenderWare engine. 
 * This should be treated as a 5 digit hex number where the first three digits 
 * correspond to the RenderWare SDK release number, and the last two are a
 * binary stream format revision number. For example, the version
 * 0x31001 corresponds to 3.1.0.1 (SDK 3.1.0, binary revision 1).
 *
 * Note that the engine must be initialized before using this function.
 *
 * \return Returns a value containing the version number.
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineInit
 *
 */
RwUInt32
RwEngineGetVersion(void)
{
    RWAPIFUNCTION(RWSTRING("RwEngineGetVersion"));
    RWRETURN(rwLIBRARYCURRENTVERSION);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Adding modules

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwengine
 * The function\ref RwEngineRegisterPlugin registers a plugin and reserves
 * some space within the global engine space. This must happen after the
 * engine has been initialized but before the engine is opened and is the
 * mechanism for adding plugins to RenderWare Graphics. The global data is
 * initialized using the specified object constructor when \ref RwEngineStart
 * is called.
 * Note that a NULL return value from the constructor will cause
 * \ref RwEngineStart to fail.
 *
 * \param size  An \ref RwInt32 value equal to the size of the memory block
 * to reserve.
 * \param pluginID  An \ref RwInt32 value equal to the plugin ID (must be
 * unique - used to identify binary chunks).
 * \param initCB  A constructor for the plugin data block.
 * \param termCB  A destructor for the plugin data block.
 *
 * \return Returns an \ref RwInt32 value equal to the byte offset within the
 * engine global variables of memory reserved for this plugin or -1 if there
 * is an error.
 *
 * \see RwEngineGetPluginOffset
 * \see RwEngineInit
 *
 */
RwInt32
RwEngineRegisterPlugin(RwInt32 size,
                       RwUInt32 pluginID,
                       RwPluginObjectConstructor initCB,
                       RwPluginObjectDestructor termCB)
{
    RwInt32 plug;

    RWAPIFUNCTION(RWSTRING("RwEngineRegisterPlugin"));
    RWASSERT(engineInstancesOpened == 0);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on (no stream or copy constructor) */
    plug = _rwPluginRegistryAddPlugin(&engineTKList,
                                     size,
                                     pluginID,
                                     initCB,
                                     termCB,
                                     (RwPluginObjectCopy)NULL);


    RWRETURN(plug);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetPluginOffset gets the offset of a previously
 * registered RenderWare Graphics engine plugin.
 *
 * \param pluginID  An \ref RwUInt32 value equal to the plugin ID.
 *
 * \return Returns an \ref RwInt32 value equal to the data block offset if successful
 * or -1 if the plugin is not registered.
 *
 * \see RwEngineRegisterPlugin
 *
 */
RwInt32
RwEngineGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32 offset;

    RWAPIFUNCTION(RWSTRING("RwEngineGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&engineTKList, pluginID);

    RWRETURN(offset);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Starting and stoping

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwengine
 * \ref RwEngineGetNumSubSystems determines the number of
 * rendering subsystems (graphics devices) currently available on the
 * platform the RenderWare Graphics engine is running. This function should
 * only be called after the engine has been opened.
 *
 * \return Returns an \ref RwInt32 value equal to the number of rendering subsystems
 * if successful or -1 if there is an error.
 *
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineOpen
 *
 */
RwInt32
RwEngineGetNumSubSystems(void)
{
    RwBool              status;
    RwInt32             numSubSystems = 1;

    RWAPIFUNCTION(RWSTRING("RwEngineGetNumSubSystems"));
    RWASSERT((RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED) ||
             (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED));

    status = _rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                                    rwDEVICESYSTEMGETNUMSUBSYSTEMS,
                                    &numSubSystems, NULL, 0);
    if (!status)
    {
        /*
         * Device does not know about rendering sub systems,
         * not even the default implementation ?!
         */

        RWMESSAGE(("Device does not know about rendering sub systems"));
    }

    RWRETURN(numSubSystems);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetSubSystemInfo retrieves information about
 * the rendering subsystem (graphics device) corresponding to the specified
 * index. This function should only be called after the engine has been opened.
 *
 * \param subSystemInfo  Pointer to an \ref RwSubSystemInfo value that will receive the information.
 * \param subSystemIndex  An \ref RwInt32 value equal to the index of the subsystem.
 *
 * \return Returns pointer to the \ref RwSubSystemInfo value if successful or
 * NULL if there is an error.
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineOpen
 *
 * \verbatim
   The sequence to print out the identifying names of all graphics devices
   on a particular platform is as follows:

   RwInt32 numSubSystems, i;
   RwSubSystemInfo ssInfo;

   numSubSystems = RwEngineGetNumSubSystems();

   for(i=0; i<numSubSystems; i++)
   {
       RwSubSystemInfo ssInfo;

       if( RwEngineGetSubSystemInfo(&ssInfo, i) )
       {
           printf("SubSystem %d: %s\n", i, ssInfo.name );
       }
   }
   \endverbatim
 *
 */
RwSubSystemInfo *
RwEngineGetSubSystemInfo(RwSubSystemInfo *subSystemInfo,
                         RwInt32 subSystemIndex)
{
    RWAPIFUNCTION(RWSTRING("RwEngineGetSubSystemInfo"));
    RWASSERT(subSystemInfo);
    RWASSERT(subSystemIndex >= 0);
    RWASSERT((RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED) ||
             (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED));

    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETSUBSYSTEMINFO,
                               subSystemInfo,
                                NULL,
                               subSystemIndex))
    {
        /* Device doesn't know about rendering sub systems,
         * not even the default implementation ?!
         */

        subSystemInfo = (RwSubSystemInfo *)NULL;
    }

    RWRETURN(subSystemInfo);

}

/**
 * \ingroup rwengine
 * \ref RwEngineGetCurrentSubSystem retrieves the index
 * of the currently selected rendering subsystem (graphics device). This
 * function should only be called after the engine has been opened.
 *
 * \return Returns an \ref RwInt32 value equal to the current rendering subsystem if
 * successful or -1 if there is an error.
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineOpen
 *
 */
RwInt32
RwEngineGetCurrentSubSystem(void)
{
    RwInt32 subSystemNumber;

    RWAPIFUNCTION(RWSTRING("RwEngineGetCurrentSubSystem"));
    RWASSERT((RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED) ||
             (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED));

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETCURRENTSUBSYSTEM,
                               &subSystemNumber, NULL, 0))
    {
        RWRETURN(subSystemNumber);
    }

    /* Device doesn't know about rendering sub systems,
     * not even the default implementation ?! */
    RWRETURN(-1);
}

/**
 * \ingroup rwengine
 * \ref RwEngineSetSubSystem is used to set the current rendering
 * subsystem (graphic device) to the subsystem with the specified index.
 * The rendering subsystem can only be set after the RenderWare Graphics
 * engine has been opened.
 *
 * \param subSystemIndex  An \ref RwInt32 value equal to the subsystem's index
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineOpen
 *
 */
RwBool
RwEngineSetSubSystem(RwInt32 subSystemIndex)
{
    RWAPIFUNCTION(RWSTRING("RwEngineSetSubSystem"));
    RWASSERT(subSystemIndex >= 0);
    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED);

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMSETSUBSYSTEM, NULL,
                               NULL, subSystemIndex))
    {
        RWRETURN(TRUE);
    }

    /* Device doesn't know about rendering sub systems,
     * not even the default implementation ?!
     */
    RWRETURN(FALSE);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetNumVideoModes determines the number of
 * video modes available in the currently selected rendering subsystem
 * (graphics device). This function should only be called after the engine
 * has been opened.
 *
 * \return Returns an \ref RwInt32 value equal to the number of video modes
 * if successful or -1 if there is an error.
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineOpen
 *
 */
RwInt32
RwEngineGetNumVideoModes(void)
{
    RwInt32 numModes;

    RWAPIFUNCTION(RWSTRING("RwEngineGetNumVideoModes"));
    RWASSERT((RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED) ||
             (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED));

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETNUMMODES, &numModes,
                               NULL, 0))
    {
        RWRETURN(numModes);
    }

    /* Device doesn't know about modes */
    RWRETURN(-1);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetVideoModeInfo retrieves information about
 * the video mode in the current rendering subsystem (graphics device)
 * corresponding to the specified index. This function should
 * only be called after the engine has been opened.
 *
 * \param modeinfo  Pointer to an \ref RwVideoMode value that will receive the
 * information.
 * \param modeIndex  An \ref RwInt32 value equal to the index of the video mode.
 *
 * \return Returns a pointer to the \ref RwVideoMode value if successful or
 * NULL if there is an error.
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineOpen
 *
 * \verbatim
   The sequence to print out all the video modes for the current graphics
   device is as follows:

   RwInt32 numVideoModes, j;
   RwVideoMode vmodeInfo;

   numVideoModes = RwEngineGetNumVideoModes();

   for(j=0; j<numVideoModes; j++)
   {
       if( RwEngineGetVideoModeInfo(&vmodeInfo, j) )
       {
           printf("Video Mode %d: %dx%dx%d %s\n", j,
               vmodeInfo.width, vmodeInfo.height, vmodeInfo.depth,
              (vmodeInfo.flags & rwVIDEOMODEEXCLUSIVE ? "(EXCLUSIVE)" : "") );
       }
   }
   \endverbatim
 *
 */
RwVideoMode *
RwEngineGetVideoModeInfo(RwVideoMode *modeinfo, RwInt32 modeIndex)
{
    RWAPIFUNCTION(RWSTRING("RwEngineGetVideoModeInfo"));
    RWASSERT(modeinfo);
    RWASSERT((RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED) ||
             (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED));

    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETMODEINFO,
                               modeinfo, NULL, modeIndex))
    {
        /* Device doesn't know about modes */
        modeinfo = (RwVideoMode *)NULL;
    }

    RWRETURN(modeinfo);

}

/**
 * \ingroup rwengine
 * \ref RwEngineGetCurrentVideoMode retrieves the index of the
 * currently selected video mode. This function should only be called after
 * the engine has been opened.
 *
 * \return Returns an \ref RwInt32 value equal to the current video mode if
 * successful or -1 if there is an error.
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineOpen
 *
 */
RwInt32
RwEngineGetCurrentVideoMode(void)
{
    RwInt32 curMode;

    RWAPIFUNCTION(RWSTRING("RwEngineGetCurrentVideoMode"));
    RWASSERT((RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED) ||
             (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED));

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETMODE, &curMode,
                               NULL, 0))
    {
        RWRETURN(curMode);
    }

    /* Device doesn't know about modes */
    RWRETURN(-1);
}

/**
 * \ingroup rwengine
 * \ref RwEngineSetVideoMode sets the current video mode
 * to the video mode with the specified index. The video mode can only be set
 * after the RenderWare Graphics engine has been opened.
 *
 * \param modeIndex  An \ref RwInt32 value equal to the video mode's index
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineGetNumSubSystems
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetCurrentSubSystem
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetSubSystemInfo
 * \see RwEngineGetVideoModeInfo
 * \see RwEngineOpen
 *
 */
RwBool
RwEngineSetVideoMode(RwInt32 modeIndex)
{
    RWAPIFUNCTION(RWSTRING("RwEngineSetVideoMode"));
    RWASSERT(modeIndex >= 0);
    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED);

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMUSEMODE, NULL,
                               NULL, modeIndex))
    {
        RWRETURN(TRUE);
    }

    /* Device failed to set mode */
    RWRETURN(FALSE);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetTextureMemorySize determines the amount of
 * available texture memory.
 *
 * \return Returns an \ref RwInt32 value giving the number of bytes available for
 *        allocation to textures.  Returns -1 if the query failed.
 *
 * \see RwEngineGetMaxTextureSize
 *
 */
RwInt32
RwEngineGetTextureMemorySize(void)
{
    RwInt32 texMem;

    RWAPIFUNCTION(RWSTRING("RwEngineGetTextureMemorySize"));
    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED);

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETTEXMEMSIZE, &texMem,
                               NULL, 0))
    {
        RWRETURN(texMem);
    }

    /* We don't know how to do this */
    RWRETURN(-1);
}

/**
 * \ingroup rwengine
 * \ref RwEngineGetMaxTextureSize determines the maximum
 * dimensions of textures allocatable with this device.
 *
 * \return Returns an \ref RwInt32 value giving the maximum dimension (in
 * texels) of textures that may be allocated.  Returns -1 if the query failed.
 *
 * \see RwEngineGetTextureMemorySize
 *
 */
RwInt32
RwEngineGetMaxTextureSize(void)
{
    RwInt32 maxTexSize;

    RWAPIFUNCTION(RWSTRING("RwEngineGetMaxTextureSize"));
    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED);

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMGETMAXTEXTURESIZE,
                               &maxTexSize, NULL, 0))
    {
        RWRETURN(maxTexSize);
    }

    /* We don't know how to do this */
    RWRETURN(-1);
}

/**
 * \ingroup rwengine
 * \ref RwEngineSetFocus changes the focus properties of the
 * currently selected rendering subsystem (graphics device). Use this
 * function to grab or release the current device focus. This function is
 * especially useful for full screen applications.
 *
 * \param enable  An \ref RwBool value equal to the focus action:
 *        \li TRUE  - Grab focus.
 *        \li FALSE - Release focus.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineSetVideoMode
 * \see RwEngineGetCurrentVideoMode
 * \see RwEngineGetNumVideoModes
 * \see RwEngineGetVideoModeInfo
 *
 */
RwBool
RwEngineSetFocus(RwBool enable)
{
    RWAPIFUNCTION(RWSTRING("RwEngineSetFocus"));
    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED);

    if (_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                               rwDEVICESYSTEMFOCUS, NULL, NULL,
                               enable))
    {
        RWRETURN(TRUE);
    }

    /* Device failed */
    RWRETURN(FALSE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Open/Close devices

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwengine
 * \ref RwEngineGetMetrics gets metrics from RenderWare Graphics
 * to evaluate the performance of the system.  Information such as
 * current polygon and vertex throughput, and cache swapping occurring in the
 * resource arena and texture cache (if applicable) is available.  Metrics are
 * only available on a metrics build.
 *
 * Note: The metrics structure is an accumulated value since the last
 * \ref RwCameraShowRaster or \ref RwRasterShowRaster, and is most useful
 * when examined immediately before calls to these functions.
 *
 * \return Returns a pointer to the metrics data block, or NULL on failure.
 *
 */
RwMetrics *
RwEngineGetMetrics(void)
{
    RWAPIFUNCTION(RWSTRING("RwEngineGetMetrics"));
    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED);

    RWRETURN(RWSRCGLOBAL(metrics));
}

/**
 * \ingroup rwengine
 * \ref RwEngineStop stops the RenderWare Graphics engine. This
 * function must be called before any attempt is made to close (and then
 * terminate, if required) the rendering engine.
 *
 * Note the sequence of function calls used to run-up and run-down the
 * RenderWare Graphics engine is as follows: \ref RwEngineInit,
 * \ref RwEngineOpen, \ref RwEngineStart to run-up and \ref RwEngineStop,
 * \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineClose
 * \see RwEngineTerm
 *
 */
RwBool
RwEngineStop(void)
{
    RwBool result;
    RwDevice *const device = &RWSRCGLOBAL(dOpenDevice);

    RWAPIFUNCTION(RWSTRING("RwEngineStop"));
    RWNOTIFY("RwEngineStop"," .. called");

    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSSTARTED);
    RWCRTCHECKMEMORY();

    /* Get the device to remove it's overloads */
    _rwDeviceSystemRequest(device,
                           rwDEVICESYSTEMINITIATESTOP, NULL, NULL,
                           0);
    RWNOTIFY("RwEngineStop", " .. got here");

    /* De-initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&engineTKList, RwEngineInstance);
    RWNOTIFY("RwEngineStop", " .. got here");

    result =
        _rwDeviceSystemRequest(device, rwDEVICESYSTEMSTOP,
                               NULL, NULL, 0);

    if (result)
    {
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSOPENED;
    }

    /* Device failed to stop */
    RWRETURN(result);
}

/**
 * \ingroup rwengine
 * \ref RwEngineStart starts the RenderWare Graphics engine. This
 * function must only be called after the rendering engine has been
 * initialized and opened (in that order). This function also initializes
 * the memory requested by all registered plugins.
 *
 * Note the sequence of function calls used to run-up and run-down the RenderWare
 * Graphics engine is as follows: \ref RwEngineInit, \ref RwEngineOpen,
 * \ref RwEngineStart to run-up and \ref RwEngineStop, \ref RwEngineClose,
 * \ref RwEngineTerm to run-down.
 *
 * A NULL return value from any of the registered plugin object constructors
 * will cause this function to fail.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStop
 * \see RwEngineClose
 * \see RwEngineTerm
 *
 */
RwBool
RwEngineStart(void)
{
    RwDevice *const device = &RWSRCGLOBAL(dOpenDevice);
    RWAPIFUNCTION(RWSTRING("RwEngineStart"));

    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED);

    RWNOTIFY("RwEngineStart"," .. called");

    if (_rwDeviceSystemRequest(device,
                               rwDEVICESYSTEMSTART, NULL, NULL, 0))
    {
        /* Initialize the plugin memory */
        if (_rwPluginRegistryInitObject(&engineTKList, RwEngineInstance))
        {
            /* Now we can set up the gamma correction - we have an open image module!!! */
            RwImageSetGamma(RWSRCGLOBAL(dOpenDevice).gammaCorrection);

            /* Get the device to finalise it's objects (device specific overload time) */
            _rwDeviceSystemRequest(device,
                                   rwDEVICESYSTEMFINALIZESTART,
                                   NULL, NULL, 0);

            /* All done */
            RWSRCGLOBAL(engineStatus) = rwENGINESTATUSSTARTED;

            RWCRTCHECKMEMORY();

            RWRETURN(TRUE);
        }

        _rwDeviceSystemRequest(device,
                               rwDEVICESYSTEMSTOP, NULL, NULL, 0);
    }

    RWRETURN(FALSE);
}

/**
 * \ingroup rwengine
 * \ref RwEngineClose closes the RenderWare Graphics engine by
 * releasing the currently selected rendering subsystem (graphics device).
 * This function must only be called after the rendering engine has been
 * stopped.
 *
 * Note the sequence of function calls used to run-up and run-down the
 * RenderWare Graphics engine is as follows: \ref RwEngineInit,
 * \ref RwEngineOpen, \ref RwEngineStart to run-up and \ref RwEngineStop,
 * \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineTerm
 *
 */
RwBool
RwEngineClose(void)
{
    RwBool              result = FALSE;
    RwDevice           *target = (RwDevice *)NULL;

    RWAPIFUNCTION(RWSTRING("RwEngineClose"));
    RWNOTIFY("RwEngineClose", " .. called");

    RWASSERT(RWSRCGLOBAL(engineStatus) == rwENGINESTATUSOPENED);
    RWCRTCHECKMEMORY();
    target = &RWSRCGLOBAL(dOpenDevice);

    result = _rwDeviceSystemRequest(target, rwDEVICESYSTEMCLOSE,
                                    NULL, NULL, 0);

    if (result)
    {
#ifndef RWGLOBALSIZE
        void               *instance = NULL;
        /*
         * Point back at the static instance
         * so we can blow away the RwEngineInstance
         */
        instance = RwEngineInstance;
        RwEngineInstance = (void *) (&staticGlobals);

        /* Copy across the staticGlobals
         * so we don't screw up the debug stuff
         */
        memcpy(RwEngineInstance, instance, sizeof(RwGlobals));

        /* Blow away the global variables */
        RwFree(instance);
        instance = NULL;
#endif /* RWGLOBALSIZE */

        /* One less instance to worry about */
        engineInstancesOpened--;
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSINITED;

    }
    RWRETURN(result);
}

/**
 * \ingroup rwengine
 * \ref RwEngineOpen initializes the default rendering subsystem
 * (graphics device) and video mode. The device is opened on the specified
 * display. Once the device has been opened use the API functions
 * \ref RwEngineSetSubSystem and \ref RwEngineSetVideoMode to set the display
 * device, as appropriate.
 *
 * This function must only be called after the rendering engine has been
 * initialized.
 *
 * Note the sequence of function calls used to run-up and run-down the
 * RenderWare Graphics engine is as follows: \ref RwEngineInit,
 * \ref RwEngineOpen, \ref RwEngineStart to run-up and \ref RwEngineStop,
 * \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \param initParams  A pointer to the \ref RwEngineOpenParams value describing a
 *       device-dependent parameter (see example below)
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineClose
 * \see RwEngineTerm
 * \see RwEngineSetSubSystem
 * \see RwEngineSetVideoMode
 *
 * \verbatim
   To open the RenderWare Graphics engine from a Windows application. The variable
   hwnd is a Windows device handle returned from CreateWindow (type HWND).
   Assume the engine has been initialized:

   RwEngineOpenParams openParams;

   openParams.displayID = hwnd;
   if( !RwEngineOpen(&openParams) )
   {
       RwEngineTerm();
       fprintf(stderr, "RwEngineOpen failed\n");
       ...
  }
  \endverbatim
 *
 */
RwBool
RwEngineOpen(RwEngineOpenParams *initParams)
{
    RwBool result ;
    RWAPIFUNCTION(RWSTRING("RwEngineOpen"));

    RWNOTIFY("RwEngineOpen", " .. called");

#if (!defined(RWGLOBALSIZE))
    if (!RwEngineInstance)
    {
        /* Bootstrap the library by giving it some static globals
         * - this happens on subsequent instance generation,
         * not the first
         */
        RwEngineInstance = &staticGlobals;
    }
#endif /* (!defined(RWGLOBALSIZE)) */

    result = ( (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSINITED) );

    if (result)
    {
        result = (NULL != initParams);

        if (result)
        {
            /* Attempt to get the device block */
            RwDevice           *const device = _rwDeviceGetHandle();

            result = (NULL != device);

            if (result)
            {
                result = EngineOpen(device, initParams);
                if (result)
                {
                    RWSRCGLOBAL(engineStatus) = rwENGINESTATUSOPENED;
                }

            }
            /* else: Failed to get device handle */
        }
        else
        {
            RWERROR((E_RW_NULLP));
        }
    }
    else
    {
        RWERROR((E_RW_BADENGINESTATE));
    }

    RWCRTCHECKMEMORY();

    RWRETURN(result);
}

/**
 * \ingroup rwengine
 * \ref RwEngineTerm terminates the RenderWare Graphics engine.
 * Before termination, the engine must first be stopped and then
 * closed (in that order).
 * This function should be called before the application closes down.
 *
 * Note the sequence of function calls used to run-up and run-down
 * the RenderWare Graphics engine is as follows:
 * \ref RwEngineInit, \ref RwEngineOpen, \ref RwEngineStart to run-up and
 * \ref RwEngineStop, \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineInit
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineClose
 *
 */
RwBool
RwEngineTerm(void)
{
    RwBool result;

#if (0)
    RWAPIFUNCTION(RWSTRING("RwEngineTerm"));
#endif /* (0) */

    /* Can't shut down whilst there are instances opened */

    result = (0 == engineInstancesOpened);

    if (result)
    {
        _rwPluginRegistryClose();
        _rwMemoryClose();
#ifdef RWDEBUG
        _rwDebugClose();
#endif /* RWDEBUG */

        /* The processor fast divide mode stuff is done in
         * RwCameraBeginUpdate and RwCameraEndUpdate.
         */

        /* Mark the engine as terminated */
        RWSRCGLOBAL(engineStatus) = rwENGINESTATUSIDLE;

        RWCRTCHECKMEMORY();

        RWCRTHEAPDIFFERENCESINCE(&_InitialMemState);

    }

    return (result);
}

/**
 * \ingroup rwengine
 * \ref RwEngineInit initializes the core of the RenderWare
 * Graphics rendering engine.
 *
 * This function must be called before any other RenderWare function except
 * functions listed in \ref rwfreelistoverview.
 *
 * This function may also be used to specify an alternative interface that
 * RenderWare Graphics will use for memory management. If specified, RenderWare
 * Graphics will use the supplied memory allocation routines rather than the
 * default standard library ones. Specifying NULL for this parameter forces the
 * default ones to be used.
 *
 * \if gcn
 * On GameCube specifying NULL will cause RwEngineInit to fail as
 * there are no default memory allocation routines.
 * \endif
 *
 * Similarly, a default file system interface is initialized by
 * \ref RwEngineInit.
 *
 * Note the sequence of function calls used to run-up and run-down the
 * RenderWare Graphics engine is as follows: \ref RwEngineInit,
 * \ref RwEngineOpen, \ref RwEngineStart to run-up and \ref RwEngineStop,
 * \ref RwEngineClose, \ref RwEngineTerm to run-down.
 *
 * Note that the purpose of FreeLists is to improve memory management
 * performance. It is recommended that developers benchmark their applications
 * with and without the use of FreeLists. FreeLists should be used if
 * possible.
 *
 * Note too that ultimately RenderWare Graphics FreeLists make use of the
 * memory functions supplied as the first parameter. Most customers will
 * want to use FreeLists as they increase performance.
 *
 * Running RenderWare Graphics without Freelists is allowed purely to help
 * developers observe memory usage since calls to RwMalloc are easily hooked.
 *
 * \param memFuncs  Pointer to an \ref RwMemoryFunctions value specifying the
 * memory management interface (specify NULL to use the default interface).
 *
 * \param openFlags  Additional parameters for the Initialization of
 * RenderWare Graphics. Currently the only flag is whether to use Freelists or
 * not. A value of rwENGINEINITNOFREELISTS can be used to tell RenderWare
 * Graphics to use \ref RwMalloc and \ref RwFree in place of
 * \ref RwFreeListAlloc and
 * \ref RwFreeListFree.
 *
 * \param resArenaSize    An \ref RwUInt32 specifying the size of the memory
 * block (in bytes) reserved for the resources instancing arena. Can be set to
 * zero if no instance data will be used
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RwEngineOpen
 * \see RwEngineStart
 * \see RwEngineStop
 * \see RwEngineClose
 * \see RwEngineTerm
 * \see RwOsGetMemoryInterface
 *
 */
RwBool
RwEngineInit(const RwMemoryFunctions *memFuncs,
             RwUInt32 openFlags,
             RwUInt32 resArenaSize)
{
    RwBool  result = FALSE;

#ifndef RWGLOBALSIZE
    /* Bootstrap the library by giving it some static globals */
    RwEngineInstance = &staticGlobals;
#endif /* RWGLOBALSIZE */

    if (openFlags & rwENGINEINITNOFREELISTS)
    {
        RWSRCGLOBAL(memoryAlloc) = MallocWrapper;
        RWSRCGLOBAL(memoryFree)  = FreeWrapper;
        _rwFreeListEnable(FALSE);
    }
    else
    {
        RWSRCGLOBAL(memoryAlloc) = _rwFreeListAllocReal;
        RWSRCGLOBAL(memoryFree)  = _rwFreeListFreeReal;
        _rwFreeListEnable(TRUE);
    }

    RWSRCGLOBAL(resArenaInitSize)  = resArenaSize;

    /* We mustn't have initted the engine previously */
    /* We can't use RWASSERTs yet - the library isn't initted */
    if (RWSRCGLOBAL(engineStatus) == rwENGINESTATUSIDLE)
    {
        RWCRTCHECKMEMORY();
        RWCRTMEMCHECKPOINT(&_InitialMemState);

        /*
         * The first thing we need is the string operations,
         * so that the debug stuff stands a chance
         */
        result = _rwStringOpen();

        if (result)
        {
            /* The first thing that must be done is to initialise the
             * debug reporting mechanism
             */
#ifdef RWDEBUG
            result = _rwDebugOpen();
            if (result)
#endif /* RWDEBUG */
            {
                result = _rwMemoryOpen(memFuncs);
                if (result)
                {
                    {
                        result = _rwPluginRegistryOpen();
                        if (result)
                        {
                            result = CorePluginAttach();
                            if (result)
                            {
                                /* Give the drivers a chance to register plugins */
                                result = _rwDeviceRegisterPlugin();
                                if (result)
                                {
                                    /* RWREGSETDEBUGTRACE(_T("RWDEBUGTRACE")); */
                                    {
                                        RWAPIFUNCTION(RWSTRING("RwEngineInit"));

                                        /* We've succeeded */
                                        RWSRCGLOBAL(engineStatus) =
                                            rwENGINESTATUSINITED;


                                        RWCRTCHECKMEMORY();

                                        RWRETURN(result);
                                    }
                                }
                            }
                            _rwPluginRegistryClose();
                        }
                    }
                    _rwMemoryClose();
                }
#ifdef RWDEBUG
                _rwDebugClose();
#endif /* RWDEBUG */
            }
            _rwStringClose();
        }
    }

    /* We don't have debug macro capability yet */
    return result;
}

