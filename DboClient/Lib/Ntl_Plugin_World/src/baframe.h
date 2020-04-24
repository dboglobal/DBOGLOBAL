/***************************************************************************
 * Module  : baframe.h                                                     *
 *                                                                         *
 * Purpose : frame handling for engine                                     *
 *                                                                         *
 **************************************************************************/

#ifndef RWFRAME_H
#define RWFRAME_H

/****************************************************************************
 Includes
 */

#include "batype.h"
#include "bamemory.h"
#include "bamatrix.h"
#include "batkreg.h"

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

#if (!defined(RWFRAMESTATICPLUGINSSIZE))
#define RWFRAMESTATICPLUGINSSIZE 0
#endif

#define RWFRAMEALIGNMENT(_frame) \
   (! (((rwFRAMEALIGNMENT)-1) & ((RwUInt32)(_frame))))

/* Type ID */
#define rwFRAME 0

/* dirty flag bits */
#define rwFRAMEPRIVATEHIERARCHYSYNCLTM  0x01
#define rwFRAMEPRIVATEHIERARCHYSYNCOBJ  0x02
#define rwFRAMEPRIVATESUBTREESYNCLTM    0x04
#define rwFRAMEPRIVATESUBTREESYNCOBJ    0x08
#define rwFRAMEPRIVATESTATIC            0x10

/****************************************************************************
 Global Types
 */

#if (!defined(DOXYGEN))
struct RwFrame
{
    RwObject            object;

    RwLLLink            inDirtyListLink;

    /* Put embedded matrices here to ensure they remain 16-byte aligned */
    RwMatrix            modelling;
    RwMatrix            ltm;

    RwLinkList          objectList; /* List of objects connected to a frame */

    struct RwFrame      *child;
    struct RwFrame      *next;
    struct RwFrame      *root;   /* Root of the tree */

#if (RWFRAMESTATICPLUGINSSIZE)
    RWALIGN(RwUInt8             pluginData[RWFRAMESTATICPLUGINSSIZE], rwFRAMEALIGNMENT);
#endif /* defined(RWFRAMESTATICPLUGINSIZE)) */
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rwframe
 * \struct RwFrame 
 * Frame for defining object position and orientation. 
 * This should be considered an opaque type.
 * Use the RwFrame API functions to access.
 */
typedef struct RwFrame RWALIGN(RwFrame, rwFRAMEALIGNMENT);

/**
 * \ingroup rwframe
 * \ref RwFrameCallBack type represents the function 
 * called from \ref RwFrameForAllChildren for all child frames linked to a given frame.  
 * This function should return a pointer to the current frame to indicate success.  
 * The callback may return NULL to terminate further callbacks on the child frames.
 * 
 * \param  frame   Pointer to the current frame, supplied by
 * iterator.
 * \param  data  Pointer to developer-defined data structure.
 *
 * \return Pointer to the current frame, or NULL If not found.
 *
 * \see RwFrameForAllChildren
 *
 */
typedef RwFrame *(*RwFrameCallBack)(RwFrame *frame, void *data);


#define RwFrameGetParentMacro(_f)   ((RwFrame *)rwObjectGetParent(_f))
#if (! ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ))
#define RwFrameGetParent(_f)    RwFrameGetParentMacro(_f)
#endif

#define RwFrameGetMatrixMacro(_f)   (&(_f)->modelling)
#if (! ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ))
#define RwFrameGetMatrix(_f)    RwFrameGetMatrixMacro(_f)
#endif


/* RWPUBLICEND */


typedef struct rwFrameGlobals rwFrameGlobals;
struct rwFrameGlobals
{
    RwFreeList         *frames;
};

/****************************************************************************
 Globals (across program)
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwPluginRegistry frameTKList;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLIC */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */


/* Finding what is attached to a frame */
extern RwFrame *
RwFrameForAllObjects(RwFrame * frame,
                     RwObjectCallBack callBack,
                     void *data);

/* Matrix operations */
extern RwFrame *
RwFrameTranslate(RwFrame * frame,
                 const RwV3d * v,
                 RwOpCombineType combine);

extern RwFrame *
RwFrameRotate(RwFrame * frame,
              const RwV3d * axis,
              RwReal angle,
              RwOpCombineType combine);

extern RwFrame *
RwFrameScale(RwFrame * frame,
             const RwV3d * v,
             RwOpCombineType combine);

extern RwFrame *
RwFrameTransform(RwFrame * frame,
                 const RwMatrix * m,
                 RwOpCombineType combine);

extern RwFrame *
RwFrameOrthoNormalize(RwFrame * frame);

extern RwFrame *
RwFrameSetIdentity(RwFrame * frame);

/* Cloning */
extern RwFrame *
RwFrameCloneHierarchy(RwFrame * root);

/* Destruction */
extern RwBool
RwFrameDestroyHierarchy(RwFrame * frame);

/* Building a frame */
extern RwFrame *
RwFrameForAllChildren(RwFrame * frame,
                      RwFrameCallBack callBack,
                      void *data);

extern RwFrame *
RwFrameRemoveChild(RwFrame * child);

extern RwFrame *
RwFrameAddChild(RwFrame * parent, RwFrame * child);

extern RwFrame *
RwFrameAddChildNoUpdate(RwFrame *parent, RwFrame *child);

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )
extern RwFrame *
RwFrameGetParent(const RwFrame * frame);
#endif

/* Getting the root */
extern RwFrame *
RwFrameGetRoot(const RwFrame * frame);

/* Getting Matrices */
extern RwMatrix *
RwFrameGetLTM(RwFrame * frame);

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )
extern RwMatrix *
RwFrameGetMatrix(RwFrame * frame);
#endif

/* Elements */
extern RwFrame *
RwFrameUpdateObjects(RwFrame * frame);

/* Creating destroying frames */
extern void
RwFrameSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

extern RwFrame *
RwFrameCreate(void);

extern RwBool
RwFrameDestroy(RwFrame * frame);

/* Finding a frames state */
extern RwBool
RwFrameDirty(const RwFrame * frame);

/* Find the amount of frames in a hierarchy */
extern RwInt32
RwFrameCount(RwFrame * frame);

/* static frame functions */
extern RwBool
_rwFrameSetStaticPluginsSize(RwInt32 size);

extern RwBool
_rwFrameInit(RwFrame *frame);

extern RwBool
_rwFrameDeInit(RwFrame *frame);

/* Plugins */
extern RwInt32
RwFrameRegisterPlugin(RwInt32 size,
                      RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB);

extern RwInt32
RwFrameGetPluginOffset(RwUInt32 pluginID);

extern RwBool
RwFrameValidatePlugins(const RwFrame * frame);

/* Cloning */
extern RwFrame *
_rwFrameCloneAndLinkClones(RwFrame * root);

extern
RwFrame *
_rwFramePurgeClone(RwFrame *root);

/* RWPUBLICEND */

/* Opening and closing */
extern void *
_rwFrameClose(void *instance,
              RwInt32 offset,
              RwInt32 size);

extern void *
_rwFrameOpen(void *instance,
             RwInt32 offset,
                RwInt32 size);

    /* Getting Matrices */
#define _rwFrameGetLTM(f) (&(f)->ltm)

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLICEND */

#endif                          /* RWFRAME_H */

