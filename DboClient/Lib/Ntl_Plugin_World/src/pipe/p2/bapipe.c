
/****************************************************************************
 *                                                                          *
 *  Module  :   bapipe.c                                                    *
 *                                                                          *
 *  Purpose :   Pipeline II plugin attach/globals stuff                     *
 *                                                                          *
 ****************************************************************************/

/**
 * \ingroup corepowerpipe
 * \page coreppoverview Core PowerPipe Overview
 *
 * Core PowerPipe structures, types and functions and platform specific
 * immediate mode pipelines.
 *
 */

/**
 * \ingroup altpipes
 * \page altpipesoverview Alternative Pipelines Overview
 *
 * These toolkits provide alternative pipelines which have been part of RwCore
 * or RpWorld in previous releases of RenderWare. They are supplied for
 * compatibility with existing code only and should not be used in new
 * projects.
 *
 */

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "bacamera.h"
#include "badebug.h"

#include "p2core.h"

#if (defined(WIN32) && !(defined(_XBOX) || defined(__GNUC__)))

/*
 * GetVolumeInformation() is defined in
 * /Program Files/Microsoft Visual Studio/VC98/Include/WINBASE.H
 * which is brought in from the context of
 * /Program Files/Microsoft Visual Studio/VC98/Include/WINDOWS.H
 */
#include <windows.h>
#define __RWWIN32GNUCUNUSED__  /* No op */
#else /* (defined(WIN32) && !defined(__GNUC__)) */
#define __RWWIN32GNUCUNUSED__ __RWUNUSED__
#endif /* (defined(WIN32) && !defined(__GNUC__)) */

#include "bapipe.h"

/****************************************************************************
 Globals (across program)
 */

RwInt32             _rxPipelineGlobalsOffset;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Functions

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWSEATIDENTIFIER))
RwBool
_rwSeatIdentifier(RwChar * RootPathName __RWWIN32GNUCUNUSED__,
                  RwUInt32 * VolumeSerialNumber __RWWIN32GNUCUNUSED__)
{
    RwBool              result = FALSE;

    RWFUNCTION(RWSTRING("_rwSeatIdentifier"));

#if (defined(WIN32) && !defined(__GNUC__))
    {

        RwChar              VolumeNameBuffer[16];
        RwChar              FileSystemNameBuffer[16];
        RwUInt32            MaximumComponentLength = 0;
        RwUInt32            FileSystemFlags = 0;

        VolumeNameBuffer[0] = '\0';
        FileSystemNameBuffer[0] = '\0';
        result =
            GetVolumeInformation(RootPathName,
                                 VolumeNameBuffer,
                                 sizeof(VolumeNameBuffer),
                                 VolumeSerialNumber,
                                 &MaximumComponentLength,
                                 &FileSystemFlags,
                                 FileSystemNameBuffer,
                                 sizeof(FileSystemNameBuffer));

#if (0)
        RWMESSAGE(("RootPathName %s", RootPathName ? RootPathName : NULL));
        RWMESSAGE(("VolumeNameBuffer %s", VolumeNameBuffer));
        RWMESSAGE(("FileSystemNameBuffer %s", FileSystemNameBuffer));
        RWMESSAGE(("VolumeSerialNumber %08x", *VolumeSerialNumber));
        RWMESSAGE(("MaximumComponentLength %ld", MaximumComponentLength));
        RWMESSAGE(("FileSystemFlags %ld", FileSystemFlags));
#endif /* (0) */
    }
#endif /* (defined(WIN32) && !defined(__GNUC__)) */

    RWRETURN(result);
}
#endif /* (defined(RWSEATIDENTIFIER)) */

/**********************************************************************/

void               *
_rwRenderPipelineOpen( void *instance,
                       RwInt32 offset,
                       RwInt32 size __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("_rwRenderPipelineOpen"));

    _rxPipelineGlobalsOffset = offset;

    if (!_rxPipelineOpen())
    {
        RWRETURN(NULL);
    }

/*
    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                                rwDEVICESYSTEMRXPIPELINEREQUESTPIPE,
                                &RXPIPELINEGLOBAL
                                (DefaultMaterialPipeline), NULL,
                                rwPIPETYPEMATERIAL))
    {
        RWRETURN(NULL);
    }

    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                                rwDEVICESYSTEMRXPIPELINEREQUESTPIPE,
                                &RXPIPELINEGLOBAL
                                (DefaultWorldSectorPipeline), NULL,
                                rwPIPETYPEWORLDSECTORINSTANCE))
    {
        RWRETURN(NULL);
    }

    if (!_rwDeviceSystemRequest(&RWSRCGLOBAL(dOpenDevice),
                                rwDEVICESYSTEMRXPIPELINEREQUESTPIPE,
                                &RXPIPELINEGLOBAL
                                (DefaultAtomicPipeline), NULL,
                                rwPIPETYPEATOMICINSTANCE))
    {
        RWRETURN(NULL);
    }
 */

    RWRETURN(instance);
}

/**********************************************************************/

void               *
_rwRenderPipelineClose( void *instance,
                        RwInt32 offset __RWUNUSED__,
                        RwInt32 size __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("_rwRenderPipelineClose"));

    _rxPipelineClose();

    RWRETURN(instance);
}

/**********************************************************************/

RwBool
_rwPipeAttach(void)
{
    RWFUNCTION(RWSTRING("_rwPipeAttach"));

    RWRETURN(TRUE);
}

/**********************************************************************/

void
_rwPipeInitForCamera(const RwCamera * camera __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rwPipeInitForCamera"));

    RWRETURNVOID();
}
