/*
 * Frame handling
 * Frames define relationships between objects and the world
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwframe
 * \page rwframeoverview RwFrame Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection frameoverview Overview
 *
 * This object provides a frame of reference for other objects, allowing them to
 * be positioned relative to each other in world space. In addition, a frame object
 * can also be linked to parent and/or child frames in a hierarchical manner. Combined,
 * these features make the frame among the most important RenderWare Graphics objects. It is
 * used primarily in conjunction with RenderWare Graphics' Retained Mode API, exposed by the
 * RpWorld Plugin.
 *
 * More information can be found in the \e Fundamentals chapter of the User Guide.
 *
*/

/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <string.h>

#include "batypes.h"
#include "batypehf.h"
#include "balibtyp.h"
#include "bamemory.h"
#include "basync.h"
#include "badebug.h"
#include "batkreg.h"
#include "bamatrix.h"
#include "bavector.h"

/* Abstraction of string functionality -- for unicode support */
#include "rwstring.h"

#include "baframe.h"

#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline off
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWFRAMEGLOBAL(var) (RWPLUGINOFFSET(rwFrameGlobals, RwEngineInstance, frameModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    frameTKList =
    { sizeof(RwFrame),
      sizeof(RwFrame),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo frameModule;

static RwInt32 _rwFrameFreeListBlockSize = 50,
               _rwFrameFreeListPreallocBlocks = 1;

/**
 * \ingroup rwframe
 * \ref RwFrameSetFreeListCreateParams allows the developer to specify
 * how many \ref RwFrame s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwFrameSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwFrameSetFreeListCreateParams"));
#endif
    _rwFrameFreeListBlockSize = blockSize;
    _rwFrameFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening and closing library

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwFrameOpen

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwFrameOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    static RwFreeList frameFreeList;

    RWFUNCTION(RWSTRING("_rwFrameOpen"));

    /* Store offset of global data (sam for all instances) */
    frameModule.globalsOffset = offset;

    RWFRAMEGLOBAL(frames) = RwFreeListCreateAndPreallocateSpace(frameTKList.sizeOfStruct,
        _rwFrameFreeListBlockSize, rwFRAMEALIGNMENT, _rwFrameFreeListPreallocBlocks,
        &frameFreeList, rwMEMHINTDUR_GLOBAL | rwID_FRAMELIST);

    if (!RWFRAMEGLOBAL(frames))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* Empty the dirty matrices */
    rwLinkListInitialize(&RWSRCGLOBAL(dirtyFrameList));

    /* One more module instance */
    frameModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwFrameClose

 On entry   :
 On exit    :
 */

void *
_rwFrameClose(void *instance,
              RwInt32 __RWUNUSED__ offset,
              RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwFrameClose"));

    if (RWFRAMEGLOBAL(frames))
    {
        RwFreeListDestroy(RWFRAMEGLOBAL(frames));
        RWFRAMEGLOBAL(frames) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    frameModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   cloning of frames

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwSetHierarchyRoot

 Goes through the hierarchy setting the root

 On entry   : Frame
            : Root
 On exit    :
 */

static void
rwSetHierarchyRoot(RwFrame *frame, RwFrame *root)
{
    RWFUNCTION(RWSTRING("rwSetHierarchyRoot"));
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(root);
    RWASSERT(RWFRAMEALIGNMENT(root));
    RWASSERTISTYPE(root, rwFRAME);

    frame->root = root;

    frame = frame->child;
    while (frame)
    {
        rwSetHierarchyRoot(frame, root);
        frame = frame->next;
    }

    RWRETURNVOID();
}

/***************************************************************************
 rwFrameCloneRecurse

 On entry   : Frame to recurse on
 On exit    : New frame
 */

static RwFrame *
rwFrameCloneRecurse(RwFrame *oldFrame, RwFrame *newRoot)
{
    RwFrame *newFrame;

    RWFUNCTION(RWSTRING("rwFrameCloneRecurse"));
    RWASSERT(oldFrame);
    RWASSERT(RWFRAMEALIGNMENT(oldFrame));
    RWASSERTISTYPE(oldFrame, rwFRAME);

    newFrame = (RwFrame *)RwFreeListAlloc(RWFRAMEGLOBAL(frames),
                           rwMEMHINTDUR_EVENT | rwID_FRAMELIST);
    RWASSERT(RWFREELISTALIGNED(newFrame, RWFRAMEGLOBAL(frames)));

    if (!newFrame)
    {
        /* Failed to create it */
        RWRETURN((RwFrame *)NULL);
    }

    if (!newRoot)
    {
        /* Not got a root yet, we have now */
        newRoot = newFrame;
    }

    RWASSERT(RWFRAMEALIGNMENT(newRoot));

    /* This initialises the type also */
    rwObjectCopy(newFrame, oldFrame);

    /* Nothing's attached */
    rwLinkListInitialize(&newFrame->objectList);
    rwObjectSetParent(newFrame, NULL);

    /* Copy and mark as dirty */
    RwMatrixCopy(&newFrame->modelling, &oldFrame->modelling);

    newFrame->child = (RwFrame *)NULL;
    newFrame->next = (RwFrame *)NULL;

    /* Point the root to the newly created frame */
    newFrame->root = newRoot;

    /* Point the source frame to the new cloned frame
     * (necessary for clump hierarchy cloning)
     */
    oldFrame->root = newFrame;

    /* Recurse over the children */
    {
        RwFrame            *child;

        child = oldFrame->child;

        while (child)
        {
            RwFrame *newChild = rwFrameCloneRecurse(child, newRoot);

            if (!newChild)
            {
                /* Destroy all children and root created here */
                RwFrameDestroyHierarchy(newFrame);

                RWRETURN((RwFrame *)NULL);
            }

            /* Insert on front of frame sibling list */
            newChild->next = newFrame->child;
            newFrame->child = newChild;

            rwObjectSetParent(newChild, newFrame);

            child = child->next;
        }
    }

    /* Call constructors */
    _rwPluginRegistryInitObject(&frameTKList, newFrame);

    /* Copy object data */
    _rwPluginRegistryCopyObject(&frameTKList, newFrame, oldFrame);

    /* Done */
    RWRETURN(newFrame);
}

/****************************************************************************
 _rwFrameCloneAndLinkClones

 NOTE: This will set the root to point to the corresponding frame

 On entry   : Frame hierarchy
 On exit    :
 */

RwFrame *
_rwFrameCloneAndLinkClones(RwFrame *root)
{
    RwFrame *newFrame;

    RWFUNCTION(RWSTRING("_rwFrameCloneAndLinkClones"));
    RWASSERT(root);
    RWASSERT(RWFRAMEALIGNMENT(root));
    RWASSERTISTYPE(root, rwFRAME);

    /* Duplicate and set the clone's root to be the new root */
    newFrame = rwFrameCloneRecurse(root, (RwFrame *)NULL);

    if (newFrame)
    {
        /* clear flag */
        rwObjectSetPrivateFlags(newFrame, rwObjectGetPrivateFlags(newFrame) &
                                        ~(rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                                          rwFRAMEPRIVATEHIERARCHYSYNCOBJ));
        RwFrameUpdateObjects(newFrame);
    }

    RWRETURN(newFrame);
}

RwFrame *
_rwFramePurgeClone(RwFrame *root)
{
    RwFrame *parent;

    RWFUNCTION(RWSTRING("_rwFramePurgeClone"));
    RWASSERT(root);
    RWASSERT(RWFRAMEALIGNMENT(root));
    RWASSERTISTYPE(root, rwFRAME);

    parent = RwFrameGetParent(root);
    if (parent)
    {
        rwSetHierarchyRoot(root, parent->root);
    }
    else
    {
        rwSetHierarchyRoot(root, root);
    }
    RWRETURN(root);
}

/**
 * \ingroup rwframe
 * \ref RwFrameCloneHierarchy makes a copy of a frame hierarchy
 * whose root is specified by the given frame. All frames below and including
 * the given frame are copied together with their modeling matrices.
 * This function also marks the frame, hence the whole hierarchy, as dirty so
 * that all LT matrices will be recalculated at the next synchronization
 * stage.
 *
 * \param root  A pointer to the root of frame hierarchy to clone.
 *
 * \return Returns a pointer to the new frame hierarchy if successful or NULL
 * if there is error.
 *
 * \see RwFrameGetRoot
 * \see RwFrameDestroyHierarchy
 * \see RwFrameCount
 *
 */
RwFrame *
RwFrameCloneHierarchy(RwFrame *root)
{
    RwFrame *newFrame;

    RWAPIFUNCTION(RWSTRING("RwFrameCloneHierarchy"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(root);
    RWASSERT(RWFRAMEALIGNMENT(root));
    RWASSERTISTYPE(root, rwFRAME);

    /* Clone and link across */
    newFrame = _rwFrameCloneAndLinkClones(root);

    /* Now unlink the source of the clone (from the clone) */
    _rwFramePurgeClone(root);

    /* All done, will return NULL if failed here */
    RWRETURN(newFrame);
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )
/**
 * \ingroup rwframe
 * \ref RwFrameGetParent determines the immediate parent
 * of the specified frame. If the frame is not part of a hierarchy or is the
 * root frame of a hierarchy then its parent is undefined.
 *
 * \param frame  A pointer to the frame.
 *
 * \return Returns a pointer to parent frame if successful or NULL there is an
 * error or if the frame has no parent.
 *
 * \see RwFrameCreate
 * \see RwFrameForAllObjects
 * \see RwFrameAddChild
 * \see RwFrameForAllChildren
 *
 */
RwFrame *
RwFrameGetParent(const RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameGetParent"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    RWRETURN(RwFrameGetParentMacro(frame));
}

#endif  /* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

/**
 * \ingroup rwframe
 * \ref RwFrameDirty checks the specified frame
 * and returns TRUE if it is dirty.
 *
 * \param frame  A pointer to the frame to check.
 *
 * \return Returns TRUE if frame is dirty, or FALSE if it's clean.
 *
 * \see RwFrameRegisterPlugin
 */
RwBool
RwFrameDirty(const RwFrame *frame)
{
    RwBool  ret;

    RWAPIFUNCTION(RWSTRING("RwFrameDirty"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    frame = frame->root;

    ret = rwObjectTestPrivateFlags(frame,
                                   (rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                                    rwFRAMEPRIVATEHIERARCHYSYNCOBJ));
    RWRETURN(ret);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Frame handling

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 rwFrameInternalInit

 On entry   : Frame to be initialised.
 On exit    :
 */

static void
rwFrameInternalInit(RwFrame *frame)
{
    RWFUNCTION(RWSTRING("rwFrameInternalInit"));
    RWASSERT(frame);

    rwObjectInitialize(frame, rwFRAME, 0);
    rwLinkListInitialize(&frame->objectList); /* No objects attached here */

    /* Set up the structure */
    rwMatrixInitializeIdentity(&frame->modelling, rwMATRIXTYPEORTHONORMAL);
    rwMatrixInitializeIdentity(&frame->ltm, rwMATRIXTYPEORTHONORMAL);

    /* Set up the position in the hierarchy */
    frame->child = (RwFrame *)NULL;
    frame->next = (RwFrame *)NULL;

    /* Point to root */
    frame->root = frame;

    /* Call constructors */
    _rwPluginRegistryInitObject(&frameTKList, frame);

    RWRETURNVOID();
}


/**
 * \ingroup rwframe
 * \ref RwFrameCreate creates a new frame. Frames define the
 * spatial relationships between the objects to which they are attached. The
 * newly created frame has no parent and is its own root. To be of any use
 * the frame should be attached to an object; frames can be added to cameras
 * and lights. In addition, frames may be used in a plugin attached to
 * geometry.
 *
 * Frames contain position and orientation information. They define object
 * hierarchies. The frame contains two matrices: the local transformation
 * matrix (LTM) and the modeling matrix. The modeling matrix describes how the
 * frame (and hence the object attached to it) is transformed relative to
 * its local origin while the LTM describes the absolute transformation from
 * local to world coordinates. If the frame is not part of a hierarchy, or
 * forms the hierarchy's root, the modeling and LT matrices are identical.
 * Otherwise, the modeling matrix is taken to be relative to the frame's
 * parent and the LTM becomes the concatenation of all the modeling matrices
 * up to the root frame. This ensures that any change in the parent's position
 * or orientation will transfer through to the child frames.
 *
 * \return Returns a pointer to the new frame if successful or NULL if there
 * is an error.
 *
 * \see RwFrameDestroy
 * \see RwFrameRotate
 * \see RwFrameTranslate
 * \see RwFrameSetIdentity
 * \see RwFrameAddChild
 * \see RwFrameRemoveChild
 * \see RwCameraSetFrame
 * \see RpLightSetFrame
 * \see RpAtomicSetFrame
 * \see RpClumpSetFrame
 *
 */
RwFrame *
RwFrameCreate(void)
{
    RwFrame *frame;

    RWAPIFUNCTION(RWSTRING("RwFrameCreate"));
    RWASSERT(frameModule.numInstances);

    RWASSERT(RWFRAMEGLOBAL(frames));
    frame = (RwFrame *) RwFreeListAlloc(RWFRAMEGLOBAL(frames),
                         rwMEMHINTDUR_EVENT | rwID_FRAMELIST);
    RWASSERT(RWFREELISTALIGNED(frame, RWFRAMEGLOBAL(frames)));
    if (!frame)
    {
        RWRETURN((RwFrame *)NULL);
    }

    rwFrameInternalInit(frame);

    /* All done */
    RWRETURN(frame);
}


/*
 * RwFrameInit initializes a static frame. Frames define the
 * spatial relationships between the objects to which they are attached. The
 * newly initialized frame has no parent and is its own root. To be of any use
 * the frame should be attached to an object; frames can be added to cameras
 * and lights. In addition, frames may be used in a plugin attached to
 * geometry.
 *
 * Frames contain position and orientation information. They define object
 * hierarchies. The frame contains two matrices: the local transformation
 * matrix (LTM) and the modeling matrix. The modeling matrix describes how the
 * frame (and hence the object attached to it) is transformed relative to
 * its local origin while the LTM describes the absolute transformation from
 * local to world coordinates. If the frame is not part of a hierarchy, or
 * forms the hierarchy's root, the modeling and LT matrices are identical.
 * Otherwise, the modeling matrix is taken to be relative to the frame's
 * parent and the LTM becomes the concatenation of all the modeling matrices
 * up to the root frame. This ensures that any change in the parent's position
 * or orientation will transfer through to the child frames.
 *
 * Do not call this if \ref RwFrameCreate has been called to create
 * the frame as the frame has already been initialized and this may result
 * in the frame not being freed properly.
 *
 * frame A pointer to the RwFrame to initialize.
 *
 * Returns TRUE if successful or FALSE if the static plugin data size
 * has not been set by _rwFrameSetStaticPluginsSize.
 *
 */
RwBool
_rwFrameInit(RwFrame *frame)
{
    RWFUNCTION(RWSTRING("_rwFrameInit"));
    RWASSERT(frame);
    RWASSERT(frameTKList.staticAlloc);

    rwFrameInternalInit(frame);

    rwObjectSetPrivateFlags(frame, rwObjectGetPrivateFlags(frame) | rwFRAMEPRIVATESTATIC);

    RWRETURN(TRUE);
}

/****************************************************************************
 _rwFrameDeInit

 On entry   : Frame to be deinitialised.
 On exit    :
 */
static void
_rwFrameInternalDeInit(RwFrame *frame)
{
    RwFrame *parent, *child;

    RWFUNCTION(RWSTRING("_rwFrameInternalDeInit"));
    RWASSERT(frame);

    /* Call destructors */
    _rwPluginRegistryDeInitObject(&frameTKList, frame);

    parent = RwFrameGetParent(frame);
    if (parent)
    {
        /* Its a child, detach from the parent (which adds it to the dirty list) */
        RwFrameRemoveChild(frame);
    }

    if (rwObjectTestPrivateFlags(frame,
                                 (rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                                  rwFRAMEPRIVATEHIERARCHYSYNCOBJ)))
    {
        rwLinkListRemoveLLLink(&frame->inDirtyListLink);
    }

    /* Remove children's references to this frame and make them all roots */
    child = frame->child;
    while (child)
    {
        RWASSERTISTYPE(child, rwFRAME);

        rwObjectSetParent(child, NULL);

        /* should really point root to child too but we're about to destroy it */

        /* Skip to the next sub tree */
        child = child->next;
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwframe
 * \ref RwFrameDestroy destroys the specified frame. If the
 * frame is a member of a hierarchy it will be removed from the hierarchy
 * and all frames below it will be lost. It is recommended that all objects
 * should be detached from the frame prior to destruction.
 *
 * \param frame  A pointer to the frame to destroy
 *
 * \return Returns TRUE or false if there is an error
 *
 * \see RwFrameCreate
 * \see RwFrameDestroyHierarchy
 * \see RwFrameRemoveChild
 *
 */
RwBool
RwFrameDestroy(RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameDestroy"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(!(rwObjectGetPrivateFlags(frame) & rwFRAMEPRIVATESTATIC));

    _rwFrameInternalDeInit(frame);

    RwFreeListFree(RWFRAMEGLOBAL(frames), frame);
    RWRETURN(TRUE);
}

/*
 * _rwFrameDeInit de-initializes the specified frame. If the
 * frame is a member of a hierarchy it will be removed from the hierarchy
 * and all frames below it will be lost. It is recommended that all objects
 * should be detached from the frame prior to destruction.
 *
 * frame  A pointer to the frame to destroy.
 *
 * Returns TRUE if successful, FALSE if there is an error.
 *
 */
RwBool
_rwFrameDeInit(RwFrame *frame)
{
    RWFUNCTION(RWSTRING("_rwFrameDeInit"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(frameTKList.staticAlloc);

    _rwFrameInternalDeInit(frame);

    RWRETURN(TRUE);
}

/****************************************************************************
 FrameDestroyRecurseDeInitLeaf

 On entry   : Deinit the leaf node of a destroyed hierarchy
 On exit    :
 */
static void
FrameDestroyRecurseDeInitLeaf(RwFrame *frame)
{
    RWFUNCTION(RWSTRING("FrameDestroyRecurseDeInitLeaf"));
    RWASSERT(frame);

    /* Call destructors */
    _rwPluginRegistryDeInitObject(&frameTKList, frame);

    /* Its the root -> handle removing from the list */
    if (rwObjectTestPrivateFlags(frame,
                                 (rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                                  rwFRAMEPRIVATEHIERARCHYSYNCOBJ)))
    {
        rwLinkListRemoveLLLink(&frame->inDirtyListLink);
    }

    /* We know there are no children at the Leaf */
    RWRETURNVOID();
}

/****************************************************************************
 rwFrameDestroyRecurseDestroyLeaf

 On entry   : Destroy the leaf node of a hierarchy
 On exit    :
 */
static void
rwFrameDestroyRecurseDestroyLeaf(RwFrame *frame)
{
    RWFUNCTION(RWSTRING("rwFrameDestroyRecurseDestroyLeaf"));

    FrameDestroyRecurseDeInitLeaf(frame);

    /* only frames we allocated */
    RWASSERT(!(rwObjectGetPrivateFlags(frame) & rwFRAMEPRIVATESTATIC));
    RwFreeListFree(RWFRAMEGLOBAL(frames), frame);

    RWRETURNVOID();
}


/****************************************************************************
 rwFrameDestroyRecurse

 Destroy hierarchy rooted at supplied frame

 On entry   : Frame
 On exit    :
 */

static void
rwFrameDestroyRecurse(RwFrame * frame)
{
    RWFUNCTION(RWSTRING("rwFrameDestroyRecurse"));

    if (frame)
    {
        RwFrame            *child;

        RWASSERT(RWFRAMEALIGNMENT(frame));
        RWASSERTISTYPE(frame, rwFRAME);

        child = frame->child;
        while (child)
        {
            RwFrame            *nextChild = child->next;

            /* Destroy the first child's hierarchy */
            rwFrameDestroyRecurse(child);

            /* Skip to the next sub tree */
            child = nextChild;
        }

        /* We can now destroy the leaf of the hierarchy */
        rwFrameDestroyRecurseDestroyLeaf(frame);
    }

    RWRETURNVOID();
}

/**
 * \ingroup rwframe
 * \ref RwFrameDestroyHierarchy destroys or de-inits the hierarchy of
 * frames below and including the specified frame. All previously attached
 * types (e.g. lights, atomics) should either be detached from the frames that
 * are about to be destroyed or attached to new frames. All frames above the
 * specified one will survive.
 *
 * \param frame  A pointer to the first frame in the hierarchy
 *
 * \return Returns TRUE on success or FALSE if there is an error
 *
 * \see RwFrameCreate
 * \see RwFrameDestroy
 * \see RwFrameRemoveChild
 * \see RwFrameAddChild
 *
 */
RwBool
RwFrameDestroyHierarchy(RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameDestroyHierarchy"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    rwFrameDestroyRecurse(frame);
    RWRETURN(TRUE);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Tweaking elements

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwframe
 * \ref RwFrameUpdateObjects marks the specified frame as dirty
 * following a modeling matrix transformation. This will cause the frame's
 * LTM to be recalculated, and all objects attached to the frame to be
 * updated, at the next synchronization stage (for example, during a call to
 * RwCameraBeginUpdate). If the frame resides within a hierarchy the entire
 * hierarchy is marked as dirty, hence all LTMs and objects will be updated.
 *
 * \param frame  A pointer to the frame to mark as dirty.
 *
 * \return Returns a pointer to the frame.
 *
 * \warning This function can only be called outside of the 
 * \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate block.
 *
 * \see RwFrameCreate
 * \see RwFrameGetMatrix
 * \see RwFrameGetLTM
 *
 */
RwFrame *
RwFrameUpdateObjects(RwFrame *frame)
{
    RwUInt32    oldFlags;

    RWAPIFUNCTION(RWSTRING("RwFrameUpdateObjects"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    oldFlags = rwObjectGetPrivateFlags(frame->root);
    if (!(oldFlags & (rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                      rwFRAMEPRIVATEHIERARCHYSYNCOBJ)))
    {
        /* Not in the dirty list - add it */
        rwLinkListAddLLLink(&RWSRCGLOBAL(dirtyFrameList),
                                &frame->root->inDirtyListLink);
    }

    /* whole hierarchy needs resync */
    rwObjectSetPrivateFlags(frame->root, oldFlags |
                            (rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                             rwFRAMEPRIVATEHIERARCHYSYNCOBJ));


    /* this frame in particular */
    rwObjectSetPrivateFlags(frame,  rwObjectGetPrivateFlags(frame) |
                            (rwFRAMEPRIVATESUBTREESYNCLTM |
                            rwFRAMEPRIVATESUBTREESYNCOBJ));

    RWRETURN(frame);
}

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )
/**
 * \ingroup rwframe
 * \ref RwFrameGetMatrix retrieves the specified frame's
 * modeling matrix.
 *
 * \param frame  A pointer to the frame to query.
 *
 * \return Returns a pointer to the frame's modeling matrix.
 *
 * \see RwFrameGetLTM
 * \see RwFrameGetParent
 * \see RwFrameTranslate
 * \see RwFrameRotate
 * \see RwFrameOrthoNormalize
 * \see RwFrameSetIdentity
 * \see RwFrameUpdateObjects
 * \see RwFrameCreate
 * \see RwFrameScale
 * \see RwFrameTransform
 *
 */
RwMatrix *
RwFrameGetMatrix(RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameGetMatrix"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    RWRETURN(RwFrameGetMatrixMacro(frame));
}
#endif

/**
 * \ingroup rwframe
 * \ref RwFrameGetLTM retrieves the specified frame's
 * local transformation matrix (LTM).
 *
 * \param frame  A pointer to the frame to query.
 *
 * \return Returns a pointer to the frame's LTM.
 *
 * \see RwFrameGetMatrix
 * \see RwFrameGetParent
 * \see RwFrameTranslate
 * \see RwFrameRotate
 * \see RwFrameOrthoNormalize
 * \see RwFrameSetIdentity
 * \see RwFrameUpdateObjects
 * \see RwFrameCreate
 * \see RwFrameScale
 * \see RwFrameTransform
 *
 */
RwMatrix *
RwFrameGetLTM(RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameGetLTM"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    /* If something has changed, sync the hierarchy */
    if (rwObjectTestPrivateFlags(frame->root, rwFRAMEPRIVATEHIERARCHYSYNCLTM))
    {
        /* Sync the whole hierarchy */
        _rwFrameSyncHierarchyLTM(frame->root);
    }

    RWRETURN(&frame->ltm);
}

/**
 * \ingroup rwframe
 * \ref RwFrameGetRoot retrieves the root frame of the
 * hierarchy that the specified frame belongs to. If the frame is not part of
 * any hierarchy the frame is its own root.
 *
 * \param frame  A pointer to the frame.
 *
 * \return Returns a pointer to the root frame.
 *
 * \see RwFrameCount
 * \see RwFrameDestroyHierarchy
 * \see RwFrameGetParent
 * \see RwFrameAddChild
 * \see RwFrameCreate
 *
 */
RwFrame *
RwFrameGetRoot(const RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameGetRoot"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    RWRETURN(frame->root);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Attaching Frames together

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifdef RWDEBUG

/****************************************************************************
 rwFrameIsAParentOf

 On entry   : Hierarchy root to test for presence
            : Frame to test for presence of as a parent of
 On exit    :
 */

static              RwBool
rwFrameIsAParentOf(RwFrame * frame, RwFrame * child)
{
    RwFrame            *parent;

    RWFUNCTION(RWSTRING("rwFrameIsAParentOf"));
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(child);
    RWASSERT(RWFRAMEALIGNMENT(child));
    RWASSERTISTYPE(child, rwFRAME);

    /* At the root, the parent of a frame is itself or NULL */
    parent = RwFrameGetParent(child);
    while (parent && (child != parent))
    {
        if (frame == parent)
        {
            /* 'frame' is a parent of 'child' */
            RWRETURN(TRUE);
        }

        child = parent;
        parent = RwFrameGetParent(child);
    }

    RWRETURN(FALSE);
}

#endif /* RWDEBUG */

/**
 * \ingroup rwframe
 * \ref RwFrameAddChildNoUpdate adds the specified child frame to
 * the hierarchy containing the given parent frame, yielding the
 * same result as \ref RwFrameAddChild, except the child frame is
 * \e not updated.
 *
 * \param parent  A pointer to the parent frame.
 * \param child  A pointer to the child frame.
 *
 * \return Returns a pointer to the parent frame.
 *
 * \see RwFrameAddChild
 * \see RwFrameRemoveChild
 * \see RwFrameCreate
 * \see RwFrameDestroy
 * \see RwFrameDestroyHierarchy
 *
 */
RwFrame *
RwFrameAddChildNoUpdate(RwFrame *parent, RwFrame *child)
{
    RWAPIFUNCTION(RWSTRING("RwFrameAddChildNoUpdate"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(parent);
    RWASSERT(RWFRAMEALIGNMENT(parent));
    RWASSERTISTYPE(parent, rwFRAME);
    RWASSERT(child);
    RWASSERT(RWFRAMEALIGNMENT(child));
    RWASSERTISTYPE(child, rwFRAME);
    RWASSERT(!rwFrameIsAParentOf(child, parent)); /* prevent loops */

    if (RwFrameGetParent(child))
    {
        RwFrameRemoveChild(child);
    }

    RWASSERT(child->root == child);

    /* add as child and re-parent */
    child->next = parent->child;
    parent->child = child;
    rwObjectSetParent(child, parent);
    rwSetHierarchyRoot(child, parent->root);

    RWRETURN(parent);
}

/**
 * \ingroup rwframe
 * \ref RwFrameAddChild adds the specified child frame to
 * the hierarchy containing the given parent frame. The child is attached to
 * the frame hierarchy directly beneath the parent and therefore inherits
 * movements resulting from changes made to its parent. Frame loops within
 * a given hierarchy are not permitted, therefore, the child must not already
 * exist within the hierarchy it is being added to.
 *
 * This function should be used to construct hierarchies of objects that can
 * be transformed via joints. Whenever the child's frame is processed its
 * Local Transform Matrix (LTM) is the concatenation of all the modeling
 * matrices up to the root frame.
 * (Note that the root frame has identical LT and modeling matrices.)
 *
 * \param parent  A pointer to the parent frame.
 * \param child  A pointer to the child frame.
 *
 * \return Returns a pointer to the parent frame.
 *
 * \see RwFrameRemoveChild
 * \see RwFrameCreate
 * \see RwFrameDestroy
 * \see RwFrameDestroyHierarchy
 *
 */
RwFrame *
RwFrameAddChild(RwFrame *parent, RwFrame *child)
{
    RWAPIFUNCTION(RWSTRING("RwFrameAddChild"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(parent);
    RWASSERT(RWFRAMEALIGNMENT(parent));
    RWASSERTISTYPE(parent, rwFRAME);
    RWASSERT(child);
    RWASSERT(RWFRAMEALIGNMENT(child));
    RWASSERTISTYPE(child, rwFRAME);
    RWASSERT(!rwFrameIsAParentOf(child, parent)); /* prevent loops */

    if (RwFrameGetParent(child))
    {
        RwFrameRemoveChild(child);
    }

    RWASSERT(child->root == child);

    /* add as child and re-parent */
    child->next = parent->child;
    parent->child = child;
    rwObjectSetParent(child, parent);
    rwSetHierarchyRoot(child, parent->root);

    /* Handle if its dirty */
    if (rwObjectTestPrivateFlags(child,
                                 rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                                 rwFRAMEPRIVATEHIERARCHYSYNCOBJ))
    {
        rwLinkListRemoveLLLink(&child->inDirtyListLink);
        /* clear flag */
        rwObjectSetPrivateFlags(child, rwObjectGetPrivateFlags(child) &
                                        ~(rwFRAMEPRIVATEHIERARCHYSYNCLTM |
                                          rwFRAMEPRIVATEHIERARCHYSYNCOBJ));
    }

    /* The child's local transformation matrix is definitely dirty */
    RwFrameUpdateObjects(child);

    RWRETURN(parent);
}

/**
 * \ingroup rwframe
 * \ref RwFrameRemoveChild removes the specified frame from
 * the hierarchy it is a member of. Once removed the frame becomes the root
 * of a new hierarchy comprising itself and all of its descendents.
 *
 * \param child  A pointer to the frame to detach from the hierarchy.
 *
 * \return Returns a pointer to the frame.
 *
 * \see RwFrameAddChild
 * \see RwFrameCreate
 * \see RwFrameDestroy
 * \see RwFrameDestroyHierarchy
 *
 */
RwFrame *
RwFrameRemoveChild(RwFrame *child)
{
    RwFrame *curFrame;

    RWAPIFUNCTION(RWSTRING("RwFrameRemoveChild"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(child);
    RWASSERT(RWFRAMEALIGNMENT(child));
    RWASSERTISTYPE(child, rwFRAME);
    RWASSERT(RwFrameGetParent(child));

    /* Need to do something */
    curFrame = ((RwFrame *) RwFrameGetParent(child))->child;

    /* Check if its the first child */
    if (curFrame == child)
    {
        RwFrameGetParent(child)->child = child->next;
    }
    else
    {
        while (curFrame->next != child)
        {
            curFrame = curFrame->next;
        }

        /* Remove it from the sibling list */
        curFrame->next = child->next;
    }

    /* Now its a root it also has no siblings */
    rwObjectSetParent(child, NULL);
    child->next = (RwFrame *)NULL;

    /* Set the hierarchy root */
    rwSetHierarchyRoot(child, child);

    /* Make it dirty */
    RwFrameUpdateObjects(child);

    RWRETURN(child);
}



/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Traversing the tree

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwframe
 * \ref RwFrameForAllChildren applies the given callback function
 * to a frame's immediate children (the next level down).  The format of the
 * callback function is:
 * \verbatim
   RwFrame * (*RwFrameCallBack)(RwFrame *frame, void *data)
   \endverbatim
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure status
 * the iteration is terminated. However, \ref RwFrameForAllChildren will still
 * return successfully.
 *
 * \param frame  A pointer to the frame with children to enumerate.
 * \param callBack  A pointer to the callback function to apply to each frame.
 * \param data  A pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the frame.
 *
 * \see RwFrameForAllChildren
 * \see RwFrameGetRoot
 * \see RwFrameGetParent
 * \see RwObjectGetType
 *
 */
RwFrame *
RwFrameForAllChildren(RwFrame *frame, RwFrameCallBack callBack, void *data)
{
    RwFrame *childFrame;

    RWAPIFUNCTION(RWSTRING("RwFrameForAllChildren"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(callBack);

    childFrame = frame->child;

    while (childFrame)
    {
        RwFrame *nextChildFrame = childFrame->next;

        if (!callBack(childFrame, data))
        {
            /* Early out */
            RWRETURN(frame);
        }

        childFrame = nextChildFrame;
    }

    RWRETURN(frame);
}

/**
 * \ingroup rwframe
 * \ref RwFrameCount determines the number of
 * frames in a hierarchy starting at the specified frame. The specified frame
 * is included in the count.
 *
 * \param frame  A pointer to the first frame in the hierarchy.
 *
 * \return Returns an \ref RwInt32 that equals the number of frames in the hierarchy.
 *
 * \see RwFrameGetRoot
 * \see RwFrameDestroyHierarchy
 *
 */
RwInt32
RwFrameCount(RwFrame *frame)
{
    RwInt32 numFrames = 1;

    RWAPIFUNCTION(RWSTRING("RwFrameCount"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    /* That's the hierarchy root frame counted!!!! - now count children */
    frame = frame->child;
    while (frame)
    {
        /* Count frames in child's subtree */
        numFrames += RwFrameCount(frame);

        /* Get child's sibling */
        frame = frame->next;
    }

    RWRETURN(numFrames);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Operations on a frame

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwframe
 * \ref RwFrameTranslate applies the given translation vector
 * to the modeling matrix of the specified frame. The combine operation may
 * be pre-concatenation, post-concatenation or replacement. This function
 * also marks the frame as dirty so that it's LTM will be recalculated at
 * the next synchronization stage.
 *
 * \param frame       A pointer to the frame to translate.
 * \param translation A pointer to a 3D vector that specifies the translation.
 * \param combineOp   A combination operator specifying how the translation
 *  is applied:
 *      \li rwCOMBINEPRECONCAT - Pre-concatenation.
 *      \li rwCOMBINEPOSTCONCAT - Post-concatenation.
 *      \li rwCOMBINEREPLACE - Replacement.
 *
 * \return Returns a pointer to the frame.
 *
 * \see RwFrameRotate
 * \see RwFrameSetIdentity
 * \see RwFrameOrthoNormalize
 * \see RwFrameUpdateObjects
 *
 */
RwFrame *
RwFrameTranslate(RwFrame *frame,
                 const RwV3d *translation,
                 RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwFrameTranslate"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(translation);

    /* Do the operation */
    RwMatrixTranslate(&frame->modelling, translation, combineOp);

    /* And mark the hierarchy as dirty */
    RwFrameUpdateObjects(frame);

    RWRETURN(frame);
}

/**
 * \ingroup rwframe
 * \ref RwFrameScale applies the given scale
 * to the modeling matrix of the specified frame. The combine operation may
 * be pre-concatenation, post-concatenation or replacement. This function
 * also marks the frame as dirty so that it's LTM will be recalculated at
 * the next synchronization stage.
 *
 * \param frame  A pointer to the frame to translate.
 * \param scale  Pointer to a 3D vector specifying the scale.
 * \param combineOp  A combination operator specifying how the scale is applied:
 *        \li rwCOMBINEPRECONCAT - Pre-concatenation.
 *        \li rwCOMBINEPOSTCONCAT - Post-concatenation.
 *        \li rwCOMBINEREPLACE - Replacement.
 *
 * \return Returns pointer to the frame.
 *
 * \see RwFrameRotate
 * \see RwFrameTranslate
 * \see RwFrameSetIdentity
 * \see RwFrameOrthoNormalize
 * \see RwFrameUpdateObjects
 * \see RwFrameTransform
 *
 */
RwFrame *
RwFrameScale(RwFrame *frame, const RwV3d *scale, RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwFrameScale"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(scale);

    /* Do the operation */
    RwMatrixScale(&frame->modelling, scale, combineOp);

    /* And mark the hierarchy as dirty */
    RwFrameUpdateObjects(frame);

    RWRETURN(frame);
}

/**
 * \ingroup rwframe
 * \ref RwFrameTransform applies the given general
 * transformation matrix to the modeling matrix of the specified frame. The
 * combine operation may be pre-concatenation, post-concatenation or
 * replacement. This function also marks the frame as dirty so that it's LTM
 * will be recalculated at the next synchronization stage.
 *
 * \param frame  A pointer to the frame to translate.
 * \param transform  A pointer to a matrix specifying the transformation.
 * \param combineOp  A combination operator specifying how the transformation
 *        is applied:
 *        \li rwCOMBINEPRECONCAT - Pre-concatenation.
 *        \li rwCOMBINEPOSTCONCAT - Post-concatenation.
 *        \li rwCOMBINEREPLACE - Replacement.
 *
 * \return Returns a pointer to the frame.
 *
 * \see RwFrameRotate
 * \see RwFrameTranslate
 * \see RwFrameSetIdentity
 * \see RwFrameOrthoNormalize
 * \see RwFrameUpdateObjects
 *
 */
RwFrame *
RwFrameTransform(RwFrame *frame,
                 const RwMatrix *transform,
                 RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwFrameTransform"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(transform);

    /* Do the operation */
    RwMatrixTransform(&frame->modelling, transform, combineOp);

    /* And mark the hierarchy as dirty */
    RwFrameUpdateObjects(frame);

    RWRETURN(frame);
}

/**
 * \ingroup rwframe
 * \ref RwFrameRotate is used to build a rotation matrix from the given
 * axis and angle of rotation and apply it to the modeling matrix of the
 * specified frame. The combine operation may be pre-concatenation,
 * post-concatenation or replacement. This function also marks the frame as
 * dirty so that it's LTM will be recalculated at the next synchronization
 * stage.
 *
 * \param frame  A pointer to the frame to rotate.
 * \param axis  A pointer to a 3D vector specifying the axis of rotation.
 * \param angle  An \ref RwReal value equal to the angle of rotation (in degrees).
 * \param combineOp  A combination operator specifying how the rotation is applied:
 *        \li rwCOMBINEPRECONCAT - Pre-concatenation.
 *        \li rwCOMBINEPOSTCONCAT - Post-concatenation.
 *        \li rwCOMBINEREPLACE - Replacement.
 *
 * \return Returns pointer to the frame.
 *
 * \see RwFrameTranslate
 * \see RwFrameSetIdentity
 * \see RwFrameOrthoNormalize
 * \see RwFrameUpdateObjects
 * \see RwFrameTransform
 * \see RwFrameScale
 * \see RwFrameTransform
 *
 */
RwFrame *
RwFrameRotate(RwFrame *frame, const RwV3d *axis,
              RwReal angle, RwOpCombineType combineOp)
{
    RWAPIFUNCTION(RWSTRING("RwFrameRotate"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(axis);

    /* Do the operation */
    RwMatrixRotate(&frame->modelling, axis, angle, combineOp);

    /* And mark the hierarchy as dirty */
    RwFrameUpdateObjects(frame);

    RWRETURN(frame);
}

/**
 * \ingroup rwframe
 * \ref RwFrameSetIdentity resets the modeling matrix of the
 * specified frame to the identity matrix. This will remove any
 * transformations previously applied to the frame. This function also marks
 * the frame as dirty so that it's LTM will be recalculated at the next
 * synchronization stage.
 *
 * \param frame  A pointer to the frame.
 *
 * \return Returns a pointer to the frame.
 *
 * \see RwFrameRotate
 * \see RwFrameTranslate
 * \see RwFrameOrthoNormalize
 * \see RwFrameUpdateObjects
 * \see RwFrameScale
 * \see RwFrameTransform
 *
 */
RwFrame *
RwFrameSetIdentity(RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameSetIdentity"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    /* Do the operation */
    RwMatrixSetIdentity(&frame->modelling);

    /* And mark the hierarchy as dirty */
    RwFrameUpdateObjects(frame);

    RWRETURN(frame);
}

/**
 * \ingroup rwframe
 * \ref RwFrameOrthoNormalize orthonormalizes the modeling
 * matrix of the specified frame. Orthonormalization is required if the frame
 * is rotated often, as small errors in the calculation will accumulate. This
 * will cause the frame to acquire unwanted scale or shear factors. To prevent
 * the significant build-up of such factors \ref RwFrameOrthoNormalize should
 * be periodically applied to ensure the frame's modeling matrix remains
 * orthonormal. The minimal satisfactory frequency of orthonormalzation will
 * depend on the nature of the application. Typically, a frequency of once
 * every 128 updates is adequate. This function also marks the frame as dirty
 * so that it's LTM will be recalculated at the next synchronization stage.
 *
 * \param frame  A pointer to the frame.
 *
 * \return Returns a pointer to the frame.
 *
 * \see RwFrameRotate
 * \see RwFrameTranslate
 * \see RwFrameSetIdentity
 * \see RwFrameUpdateObjects
 * \see RwFrameScale
 * \see RwFrameTransform
 *
 */
RwFrame *
RwFrameOrthoNormalize(RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwFrameOrthoNormalize"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

    /* Do the operation */
    RwMatrixOrthoNormalize(&frame->modelling, &frame->modelling);

    /* And mark the hierarchy as dirty */
    RwFrameUpdateObjects(frame);

    RWRETURN(frame);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Accessing objects connected to a frame

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwframe
 * \ref RwFrameForAllObjects is used applies the given callback function
 * to all objects attached to the specified frame.  The format of the callback
 * function is:
 * \verbatim
   RwObject * (*RwObjectCallBack)(RwObject *object, void *data)
   \endverbatim
 * where data is a user-supplied data pointer to pass to the callback function.
 *
 * Note that if any invocation of the callback function returns a failure
 * status the iteration is terminated. However, \ref RwFrameForAllObjects will
 * still return successfully.
 *
 * \param frame  A pointer to the frame with objects attached
 * \param callBack  A pointer to the callback function to apply to each object.
 * \param data  A pointer to user-supplied data to pass to callback function.
 *
 * \return Returns pointer to the frame.
 *
 */
RwFrame *
RwFrameForAllObjects(RwFrame *frame, RwObjectCallBack callBack, void *data)
{
    RwLLLink    *current, *next, *end;

    RWAPIFUNCTION(RWSTRING("RwFrameForAllObjects"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);
    RWASSERT(callBack);

    current = rwLinkListGetFirstLLLink(&frame->objectList);
    end = rwLinkListGetTerminator(&frame->objectList);
    while (current != end)
    {
        RwObject           *object;

        next = rwLLLinkGetNext(current);

        object = (RwObject *) rwLLLinkGetData(current, RwObjectHasFrame, lFrame);

        if (!callBack(object, data))
        {
            /* Early out */
            RWRETURN(frame);
        }

        current = next;
    }

    RWRETURN(frame);
}

/*
 * _rwFrameSetStaticPluginsSize tells RenderWare Graphics how much
 * memory is statically allocated for plugins. This must happen after the
 * engine has been initialized, but before the engine is opened.
 *
 * size  The size of the memory block to reserve in the frame
 *
 * Returns TRUE if successful, FALSE if there is an error
 */
RwBool
_rwFrameSetStaticPluginsSize(RwInt32 size)
{
    RwBool ret;

    RWFUNCTION(RWSTRING("_rwFrameSetStaticPluginsSize"));
    RWASSERT(!frameModule.numInstances);
    RWASSERT(size >= 0);

    /* Static structure size will be padded to alignement value. */

    size = (size + (rwFRAMEALIGNMENT - 1)) & (~(rwFRAMEALIGNMENT-1));

    /* Everything's cool, so pass it on */

    ret = _rwPluginRegistrySetStaticPluginsSize(&frameTKList, size);

    RWRETURN(ret);
}

/**
 * \ingroup rwframe
 * \ref RwFrameRegisterPlugin registers a plugin and reserve some space
 * within a frame. This must happen after the engine has been initialized, but
 * before the engine is opened.
 *
 * \param size  The size of the memory block to reserve in the frame
 * \param pluginID  The 32 bit unique ID for the plugin (used to identify binary chunks)
 * \param constructCB  The constructor for the plugin data block
 * \param destructCB  The destructor for the plugin data block
 * \param copyCB  The copy constructor for the plugin data block
 *
 * \return Returns one of the following values:
 *    \li positive - Byte offset within the frame of memory reserved for this plugin
 *    \li negative - On failure
 *
 * \see RwFrameRegisterPluginStream
 * \see RwFrameGetPluginOffset
 * \see RwFrameValidatePlugins
 */
RwInt32
RwFrameRegisterPlugin(RwInt32 size,
                      RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB)
{
    RwInt32 ret;

    RWAPIFUNCTION(RWSTRING("RwFrameRegisterPlugin"));
    RWASSERT(!frameModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */

    ret = _rwPluginRegistryAddPlugin(&frameTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(ret);
}

/**
 * \ingroup rwframe
 * \ref RwFrameGetPluginOffset is used to get the offset of a previously
 * registered plugin.
 *
 * \param pluginID  The plugin ID for which to get the data offset.
 *
 * \return Returns an \ref RwInt32 that equals the data block offset or -1 if
 * the plugin is not registered.
 *
 * \see RwFrameRegisterPlugin
 * \see RwFrameRegisterPluginStream
 * \see RwFrameValidatePlugins
 *
 */
RwInt32
RwFrameGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RwFrameGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&frameTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rwframe
 * \ref RwFrameValidatePlugins validates the plugin memory allocated
 * within the specified frame. This function is useful to help determine where
 * memory stomping may be occurring within an application. This function only
 * returns a meaningful response under a debug library.
 *
 * \param  frame   The frame for which to validate the plugin memory.
 *
 * \return Returns one of the following values:
 *    \li TRUE - Frame data is valid
 *    \li FALSE - Frame data has become corrupt, or an error has occurred.
 *
 * \see RwFrameRegisterPlugin
 * \see RwFrameRegisterPluginStream
 * \see RwFrameGetPluginOffset
 *
 */

RwBool
RwFrameValidatePlugins(const RwFrame * frame __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RwFrameValidatePlugins"));
    RWASSERT(frameModule.numInstances);
    RWASSERT(frame);
    RWASSERT(RWFRAMEALIGNMENT(frame));
    RWASSERTISTYPE(frame, rwFRAME);

#ifdef RWDEBUG
    RWRETURN(_rwPluginRegistryValidateObject(&frameTKList, frame));
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

#if defined (__MWERKS__)
#if (defined(RWVERBOSE))
#pragma message (__FILE__ "/" _SKY_EXPAND(__LINE__) ": __MWERKS__ == " _SKY_EXPAND(__MWERKS__))
#endif /* (defined (__MWERKS__)) */
#if (__option (global_optimizer))
#pragma always_inline on
#endif /* (__option (global_optimizer)) */
#endif /*  defined (__MWERKS__) */

