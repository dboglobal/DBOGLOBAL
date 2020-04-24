/***************************************************************************
 *                                                                         *
 * Module  : badevice.h                                                    *
 *                                                                         *
 * Purpose : Device layer                                                  *
 *                                                                         *
 **************************************************************************/

#ifndef RWDEVICE_H
#define RWDEVICE_H

/****************************************************************************
 Includes
 */

#include "batkreg.h"
#include "baimmedi.h"

/****************************************************************************
 Defines
 */

/* RWPUBLIC */
/***************************************************************************/
/************************* System Requests *********************************/
/***************************************************************************/

/* Device controls:
 *
 * rwDEVICESYSTEMOPEN(NULL, RwEngineOpenParams *openParams, 0)
 * rwDEVICESYSTEMCLOSE(NULL, NULL, 0)
 * rwDEVICESYSTEMSTART(NULL, NULL, 0)
 * rwDEVICESYSTEMSTOP(NULL, NULL, 0)
 * rwDEVICESYSTEMREGISTER(RwDevice *coreDeviceBlock, RwMemoryFunctions *memFuncs, 0)
 * rwDEVICESYSTEMGETNUMMODES(RwInt32 *numModes, NULL, 0)
 * rwDEVICESYSTEMGETMODEINFO(RwVideoMode *modeinfo, NULL, RwInt32 modeNum)
 * rwDEVICESYSTEMUSEMODE(NULL, NULL, RwInt32 modeNum)
 * rwDEVICESYSTEMFOCUS(NULL, NULL, RwBool gainFocus)
 * rwDEVICESYSTEMINITPIPELINE(NULL, NULL, 0)
 * rwDEVICESYSTEMGETMODE(RwInt32 *curMode, NULL, 0)
 * rwDEVICESYSTEMSTANDARDS(RwStandardFunc *fnPtrArray, NULL, RwInt32 arraySize)
 * rwDEVICESYSTEMGETTEXMEMSIZE(RwInt32 *texMemSizeOut, NULL, 0)
 * rwDEVICESYSTEMGETNUMSUBSYSTEMS(RwInt32 *numSubSystemsOut, NULL, 0)
 * rwDEVICESYSTEMGETSUBSYSTEMINFO(RwSubSystemInfo *subSystemInfo, NULL, RwInt32 subSystemNum)
 * rwDEVICESYSTEMGETCURRENTSUBSYSTEM(RwInt32 *curSubSystem, NULL, 0)
 * rwDEVICESYSTEMSETSUBSYSTEM(NULL, NULL, RwInt32 subSystemNum)
 * rwDEVICESYSTEMFINALIZESTART(NULL, NULL, 0)
 * rwDEVICESYSTEMINITIATESTOP(NULL, NULL, 0)
 * rwDEVICESYSTEMRXPIPELINEREQUESTPIPE(RxPipeline **pipelineRef, NULL, RwInt32 pipeType)
 * rwDEVICESYSTEMGETID(RwUInt16 *id, NULL, 0)
 * rwDEVICESYSTEMDD         - start of device specific controls
 */

#define rwPIPETYPEMATERIAL            0
#define rwPIPETYPEWORLDSECTORINSTANCE 1
#define rwPIPETYPEATOMICINSTANCE      2

enum RwCoreDeviceSystemFn
{
    rwDEVICESYSTEMOPEN                  = 0x00,
    rwDEVICESYSTEMCLOSE,
    rwDEVICESYSTEMSTART,
    rwDEVICESYSTEMSTOP,
    rwDEVICESYSTEMREGISTER,
    rwDEVICESYSTEMGETNUMMODES,
    rwDEVICESYSTEMGETMODEINFO,
    rwDEVICESYSTEMUSEMODE,
    rwDEVICESYSTEMFOCUS,
    rwDEVICESYSTEMINITPIPELINE,
    rwDEVICESYSTEMGETMODE,
    rwDEVICESYSTEMSTANDARDS,
    rwDEVICESYSTEMGETTEXMEMSIZE,
    rwDEVICESYSTEMGETNUMSUBSYSTEMS,
    rwDEVICESYSTEMGETSUBSYSTEMINFO,
    rwDEVICESYSTEMGETCURRENTSUBSYSTEM,
    rwDEVICESYSTEMSETSUBSYSTEM,
    rwDEVICESYSTEMFINALIZESTART,
    rwDEVICESYSTEMINITIATESTOP,
    rwDEVICESYSTEMGETMAXTEXTURESIZE,
    rwDEVICESYSTEMRXPIPELINEREQUESTPIPE,
    rwDEVICESYSTEMGETMETRICBLOCK,
    rwDEVICESYSTEMGETID,
    rwDEVICESYSTEMDD                    = 0x1000,
    rwCOREDEVICESYSTEMFNFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCoreDeviceSystemFn RwCoreDeviceSystemFn;

/******************************************************************************/
/********************* Standard functions *************************************/
/******************************************************************************/

#define rwSTANDARDNASTANDARD            0
#define rwSTANDARDCAMERABEGINUPDATE     1   /* Start 3d camera update */
#define rwSTANDARDRGBTOPIXEL            2   /* For an RGB value return a pixel value */
#define rwSTANDARDPIXELTORGB            3   /* For a pixel value returns a RGB value */
#define rwSTANDARDRASTERCREATE          4   /* Create an raster */
#define rwSTANDARDRASTERDESTROY         5   /* Raster destroy */
#define rwSTANDARDIMAGEGETRASTER        6   /* Get image from a raster */
#define rwSTANDARDRASTERSETIMAGE        7   /* Set raster from an image */
#define rwSTANDARDTEXTURESETRASTER      8   /* Set texture's raster */
#define rwSTANDARDIMAGEFINDRASTERFORMAT 9   /* Find a suitable raster format for an image */
#define rwSTANDARDCAMERAENDUPDATE       10  /* End 3d camera update */
#define rwSTANDARDSETRASTERCONTEXT      11  /* Start destination of 2d operations */
#define rwSTANDARDRASTERSUBRASTER       12  /* Make a raster inside another raster */
#define rwSTANDARDRASTERCLEARRECT       13  /* Clear a rectangle of the current dest raster */
#define rwSTANDARDRASTERCLEAR           14  /* Clear the current dest raster */
#define rwSTANDARDRASTERLOCK            15  /* Lock a raster to get it's pixels */
#define rwSTANDARDRASTERUNLOCK          16  /* Unlock a raster to return it's pixels */
#define rwSTANDARDRASTERRENDER          17  /* Render a raster (not scaled, but masked) */
#define rwSTANDARDRASTERRENDERSCALED    18  /* Render a raster (scaled and masked) */
#define rwSTANDARDRASTERRENDERFAST      19  /* Render a raster (not scaled or masked) */
#define rwSTANDARDRASTERSHOWRASTER      20  /* Show a camera raster */
#define rwSTANDARDCAMERACLEAR           21  /* Clear a camera's raster and/or Z raster */
#define rwSTANDARDHINTRENDERF2B         22  /* Set hint for rendering direction in the world */
#define rwSTANDARDRASTERLOCKPALETTE     23  /* Lock a raster to get it's palette */
#define rwSTANDARDRASTERUNLOCKPALETTE   24  /* Unlock a raster to return it's palette */
#define rwSTANDARDNATIVETEXTUREGETSIZE  25  /* Get size of native texture when written to a stream */
#define rwSTANDARDNATIVETEXTUREREAD     26  /* Read native texture from the stream */
#define rwSTANDARDNATIVETEXTUREWRITE    27  /* Write native texture to the stream */
#define rwSTANDARDRASTERGETMIPLEVELS    28  /* Get the number of mip levels in a raster */
#define rwSTANDARDNUMOFSTANDARD         29

/* rwDEVICE_ is a 16 bit device id.
   When adding a new device, add it to the end of the apropriate list.
   "Real" devices should have an 8 bit id. NULLxxx devices should have the
   equivalent "Real" device in the bottom 8 bits, with a non-zero value in
   the top 8 bits */
#define rwDEVICE_UNKNOWN (0)
#define rwDEVICE_D3D8    (1)
#define rwDEVICE_D3D9    (2)
#define rwDEVICE_GCN     (3)
#define rwDEVICE_NULL    (4)
#define rwDEVICE_OPENGL  (5)
#define rwDEVICE_SKY2    (6)
#define rwDEVICE_SOFTRAS (7)
#define rwDEVICE_XBOX    (8)
#define rwDEVICE_PSP     (9)

#define rwDEVICE_NULLxxx  (0x100)
#define rwDEVICE_NULLGCN  (rwDEVICE_NULLxxx | rwDEVICE_GCN)
#define rwDEVICE_NULLSKY  (rwDEVICE_NULLxxx | rwDEVICE_SKY2)
#define rwDEVICE_NULLXBOX (rwDEVICE_NULLxxx | rwDEVICE_XBOX)
#define rwDEVICE_NULLPSP  (rwDEVICE_NULLxxx | rwDEVICE_PSP)

/****************************************************************************
 Global Types
 */

/* Standard functions */
typedef RwBool (*RwStandardFunc)(void *pOut,void *pInOut,RwInt32 nI);

typedef struct RwEngineOpenParams RwEngineOpenParams;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwengine
 * \struct RwEngineOpenParams
 * This type is used to specify device dependent parameters
 * for use by the API function \ref RwEngineOpen.
 * For a Windows application the displayID field
 * should be set to the window's handle (of type HWND).
 * For NULL and sky libraries displayID=0:
 *
 * \if xbox
 * \see RwXboxDeviceConfig
 * \endif
 *
 * \if gcn
 * \see RwGameCubeDeviceConfig
 * \endif
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwEngineOpenParams
{
    void    *displayID;     /**< Display Identifier */
};

/* nOption is one of a list of possible System defines (see above) */
typedef RwBool
    (*RwSystemFunc)(RwInt32 nOption,
                    void *pOut,
                    void *pInOut,
                    RwInt32 nIn);

/* Device block */
typedef RwBool
    (*RwRenderStateSetFunction)(RwRenderState nState,void *pParam);

typedef RwBool
    (*RwRenderStateGetFunction)(RwRenderState nState,void *pParam);

typedef RwBool
    (*RwIm2DRenderLineFunction)(RwIm2DVertex *vertices,
                                RwInt32 numVertices,
                                RwInt32 vert1,
                                RwInt32 vert2);

typedef RwBool
    (*RwIm2DRenderTriangleFunction)(RwIm2DVertex *vertices,
                                    RwInt32 numVertices,
                                    RwInt32 vert1,
                                    RwInt32 vert2,
                                    RwInt32 vert3);

typedef RwBool
    (*RwIm2DRenderPrimitiveFunction)(RwPrimitiveType primType,
                                     RwIm2DVertex *vertices,
                                     RwInt32 numVertices);

typedef RwBool
    (*RwIm2DRenderIndexedPrimitiveFunction)(RwPrimitiveType primType,
                                            RwIm2DVertex *vertices,
                                            RwInt32 numVertices,
                                            RwImVertexIndex *indices,
                                            RwInt32 numIndices);

typedef RwBool
    (*RwIm3DRenderLineFunction)(RwInt32 vert1,
                                RwInt32 vert2);

typedef RwBool
    (*RwIm3DRenderTriangleFunction)(RwInt32 vert1,
                                    RwInt32 vert2,
                                    RwInt32 vert3);

typedef RwBool
    (*RwIm3DRenderPrimitiveFunction)(RwPrimitiveType primType);

typedef RwBool
    (*RwIm3DRenderIndexedPrimitiveFunction)(RwPrimitiveType primtype,
                                            RwImVertexIndex *indices,
                                            RwInt32 numIndices);


typedef struct RwDevice RwDevice;

#if (!defined(DOXYGEN))
/*
 * struct RwDevice
 * Structure describing a display device
 */
struct RwDevice
{
    RwReal                                  gammaCorrection; /* Gamma correction  */
    RwSystemFunc                            fpSystem;  /* System handler */
    RwReal                                  zBufferNear; /* Near Z buffer value */
    RwReal                                  zBufferFar; /* Far Z buffer value */

    /* Immediate mode functions */
    RwRenderStateSetFunction                fpRenderStateSet; /* Internal Use */
    RwRenderStateGetFunction                fpRenderStateGet; /* Internal Use */

    /* Render functions */
    RwIm2DRenderLineFunction                fpIm2DRenderLine; /* Internal Use */
    RwIm2DRenderTriangleFunction            fpIm2DRenderTriangle; /* Internal Use */
    RwIm2DRenderPrimitiveFunction           fpIm2DRenderPrimitive; /* Internal Use */
    RwIm2DRenderIndexedPrimitiveFunction    fpIm2DRenderIndexedPrimitive; /* Internal Use */

    RwIm3DRenderLineFunction                fpIm3DRenderLine; /* Internal Use */
    RwIm3DRenderTriangleFunction            fpIm3DRenderTriangle; /* Internal Use */
    RwIm3DRenderPrimitiveFunction           fpIm3DRenderPrimitive; /* Internal Use */
    RwIm3DRenderIndexedPrimitiveFunction    fpIm3DRenderIndexedPrimitive; /* Internal Use */
};

#endif /* (!defined(DOXYGEN)) */

typedef struct RwMetrics RwMetrics;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwengine
 * \struct RwMetrics
 * This structure provides information about the performance
 * of the application.  The metrics are recorded only in the metrics
 * libraries.  To use metrics, you should compile with the RWMETRICS
 * preprocessor symbol defines, and link with the metrics libraries
 * that ship with the SDK.  The metrics are recorded on a per-frame
 * basis.  Each platform may provide additional information that
 * is specific to that platform.  Note that either the
 * \ref numTextureUploads or \ref numResourceAllocs being non-zero can
 * be considered as bad news and will indicate a significantly
 * reduced rendering performance.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwMetrics
{
    RwUInt32    numTriangles;           /**< The number of triangles processed. */
    RwUInt32    numProcTriangles;       /**< The number of mesh triangles processed. */
    RwUInt32    numVertices;            /**< The number of vertices processed. */
    RwUInt32    numTextureUploads;
    /**<
     * \if sky2 The number of textures used. \endif
     * \if d3d8 The number of times RwD3D8SetTexture is called. \endif
     * \if d3d9 The number of times RwD3D9SetTexture is called.  \endif
     * \if opengl The number of times RwOpenGLSetTexture is called. \endif
     * \if xbox The number of textures used. \endif
     * \if gcn The number of textures used. \endif
     * \if softras The number of times the \ref rwRENDERSTATETEXTURERASTER renderstate
     * \if psp The number of times SCE_GE_CMD_TFLUSH is sent to the GPU.  \endif
     * is set with a valid (non-NULL) texture. \endif
     */
    RwUInt32    sizeTextureUploads;
    /**<
     * \if sky2 The number of bytes uploaded to the GS. \endif
     * \if d3d8 The size of the textures, in bytes, that have been passed to RwD3D8SetTexture. \endif
     * \if d3d9 The size of the textures, in bytes, that have been passed to RwD3D9SetTexture. \endif
     * \if opengl The size of the textures, in bytes, that have been passed to RwOpenGLSetTexture. \endif
     * \if xbox Size of textures swapped. \endif
     * \if gcn Size of textures swapped. \endif
     * \if softras The size of the textures, in bytes, that have been set with
     * \if psp The size of the textures, in bytes, that have been passed to RwPspDLTexImage. \endif
     * the \ref rwRENDERSTATETEXTURERASTER renderstate. \endif
     */
    RwUInt32    numResourceAllocs;      /**< The number of resource blocks swapped. */
    void        *devSpecificMetrics;    /**< Device specific metrics. */
};

#define SUBSYSTEMNAME_MAXLEN 80

typedef struct RwSubSystemInfo RwSubSystemInfo;

#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwengine
 * \struct RwSubSystemInfo
 * This type is used to represent information about a device.
 * The only available field specifies a character string
 * which identifies the subsystem
 * (see API function \ref RwEngineGetSubSystemInfo). */
#endif /* RWADOXYGENEXTERNAL */
struct RwSubSystemInfo
{
    RwChar  name[SUBSYSTEMNAME_MAXLEN]; /**< Sub system string */
};


/* Video modes */
/* These are flag bits which may be ORd */
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwengine
 * RwVideoModeFlag
 * These flags specify the type of display that RenderWare
 * will use.  The flags may be OR'd together to build composite modes.
 * Note that not all modes are supported on all platforms.
 */
#endif /* RWADOXYGENEXTERNAL */
enum RwVideoModeFlag
{
    rwVIDEOMODEEXCLUSIVE    = 0x0001, /**<Exclusive (i.e. full-screen) */
    rwVIDEOMODEINTERLACE    = 0x0002, /**<Interlaced                   */
    rwVIDEOMODEFFINTERLACE  = 0x0004, /**<Flicker Free Interlaced      */

    /* Platform specific video mode flags. */

    rwVIDEOMODE_PS2_FSAASHRINKBLIT    = 0x0100,
    /**< \if sky2
     *   Full-screen antialiasing mode 0
     *   \endif
     */
    rwVIDEOMODE_PS2_FSAAREADCIRCUIT   = 0x0200,
    /**< \if sky2
     *   Full-screen antialiasing mode 1
     *   \endif
     */

    rwVIDEOMODE_XBOX_WIDESCREEN       = 0x0100,
    /**< \if xbox
     *   Wide screen.
     *   \endif
     */
    rwVIDEOMODE_XBOX_PROGRESSIVE      = 0x0200,
    /**< \if xbox
     *   Progressive.
     *   \endif
     */
    rwVIDEOMODE_XBOX_FIELD            = 0x0400,
    /**< \if xbox
     *   Field rendering.
     *   \endif
     */
    rwVIDEOMODE_XBOX_10X11PIXELASPECT = 0x0800,
    /**< \if xbox
     *   The frame buffer is centered on the display.
     *   On a TV that is 704 pixels across, this would leave 32 pixels of black
     *   border on the left and 32 pixels of black border on the right.
     *   \endif
     */

    rwVIDEOMODEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwVideoModeFlag RwVideoModeFlag;

#define rwVIDEOMODEFSAA0 rwVIDEOMODE_PS2_FSAASHRINKBLIT
#define rwVIDEOMODEFSAA1 rwVIDEOMODE_PS2_FSAAREADCIRCUIT

typedef struct RwVideoMode RwVideoMode;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwengine
 * \struct RwVideoMode
 * This type represents a video mode available on a device specified
 * by the frame buffer resolution (width and height) and depth,
 * and a flag indicating  whether the device has exclusive use of
 * the mode (see API function \ref RwEngineGetVideoModeInfo): */
#endif /* RWADOXYGENEXTERNAL */
struct RwVideoMode
{
        RwInt32         width;   /**< Width  */
        RwInt32         height;  /**< Height */
        RwInt32         depth;   /**< Depth  */
        RwVideoModeFlag flags;   /**< Flags  */
        RwInt32         refRate; /**< Approximate refresh rate */
        RwInt32         format;  /**< Raster format
                                  * \see RwRasterFormat 
                                  */
};
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwengine
 * RwEngineInitFlag
 * Engine initialization flags.  An application can use
 * these to control the memory manager that RenderWare uses for dynamic
 * memory management.  By default RenderWare uses FreeLists.  This is the
 * preferred way of using RenderWare.  If the application does not want
 * RenderWare to use the memory manager, then the application can pass
 * rwENGINEINITNOFREELISTS as the argument to \ref RwEngineInit and
 * RenderWare will replace freelist calls to corresponding calls to
 * RwMalloc and RwFree.  This will result in more memory management
 * related calls.
 */
#endif /* RWADOXYGENEXTERNAL */
enum RwEngineInitFlag
{
    rwENGINEINITFREELISTS = 0,      /**<Use Freelists */
    rwENGINEINITNOFREELISTS = 0x1,  /**<Don't use Freelists */
    rwENGINEINITFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwEngineInitFlag RwEngineInitFlag;

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Get the library binary version */
extern RwUInt32 RwEngineGetVersion(void);

/* Sequence of events to get RenderWare up and running */
extern RwBool RwEngineInit(const RwMemoryFunctions *memFuncs,
                           RwUInt32 initFlags,
                           RwUInt32 resArenaSize);
extern RwInt32 RwEngineRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                                  RwPluginObjectConstructor initCB,
                                  RwPluginObjectDestructor termCB);
extern RwInt32 RwEngineGetPluginOffset(RwUInt32 pluginID);
extern RwBool RwEngineOpen(RwEngineOpenParams *initParams);
extern RwBool RwEngineStart(void);
extern RwBool RwEngineStop(void);
extern RwBool RwEngineClose(void);
extern RwBool RwEngineTerm(void);

/* Finding out about the rendering sub systems available */
extern RwInt32 RwEngineGetNumSubSystems(void);
extern RwSubSystemInfo *RwEngineGetSubSystemInfo(RwSubSystemInfo *subSystemInfo, RwInt32 subSystemIndex);
extern RwInt32 RwEngineGetCurrentSubSystem(void);
extern RwBool RwEngineSetSubSystem(RwInt32 subSystemIndex);

/* Finding out about the modes available */
extern RwInt32 RwEngineGetNumVideoModes(void);
extern RwVideoMode *RwEngineGetVideoModeInfo(RwVideoMode *modeinfo, RwInt32 modeIndex);
extern RwInt32 RwEngineGetCurrentVideoMode(void);
extern RwBool RwEngineSetVideoMode(RwInt32 modeIndex);

/* Finding out how much texture memory is available */
extern RwInt32 RwEngineGetTextureMemorySize(void);
extern RwInt32 RwEngineGetMaxTextureSize(void);

/* Getting/Releasing the focus */
extern RwBool RwEngineSetFocus(RwBool enable);

/* Getting metrics */
extern RwMetrics *RwEngineGetMetrics(void);

/* RWPUBLICEND */

extern RwBool _rwDeviceSystemRequest(RwDevice *d, RwInt32 id, void *out,
                                 void *inOut, RwInt32 numIn);
extern RwUInt32 _rwGetNumEngineInstances(void);


/*
 * _rwDeviceGetHandle() is not defined in badevice.c;
 * rather, it is the link
 * + from the library side
 * + to the device side
 * of the system
 */
extern RwDevice *_rwDeviceGetHandle(void);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif /* RWDEVICE_H */

