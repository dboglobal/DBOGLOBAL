
#ifndef RPPRTADV_H
#define RPPRTADV_H

/*===========================================================================*
 *--- Include files ---------------------------------------------------------*
 *===========================================================================*/

#include "rwcore.h"
#include "rpworld.h"


/*---- start: ./prtadv.h----*/

#include "rwcore.h"
#include "rpworld.h"
#include "rpprtstd.h"

#include "rpprtadv.rpe"

/************************************************************************
 *
 * doxygen defgroups
 *
 ************************************************************************/

/**
 * \defgroup rpprtadv RpPrtAdv
 * \ingroup fxpack
 *
 * Advanced Particle Animation Plugin for RenderWare Graphics.
 */


/**
 * \defgroup rpprtadvmanage RpPrtAdv Tools and Management
 * \ingroup rpprtadv
 */


/**
 * \defgroup prtadvcircle Circular and Cylindrical Shaped Emitters
 * \ingroup rpprtadv
 */


/**
 * \defgroup prtadvsphere Spherical Shaped Emitters
 * \ingroup rpprtadv
 */


/**
 * \defgroup prtadvpointlist Point List Based Emitters
 * \ingroup rpprtadv
 */


/**
 * \defgroup prtadvmultiprop Multi-Property Animations
 * \ingroup rpprtadv
 */


/**
 * \defgroup prtadvprtemitter Particle-Emitter Animations
 * \ingroup rpprtadv
 */


/**
 * \defgroup prtadvtexstep Stepped Texture Coordinate Animations (non-interpolated)
 * \ingroup rpprtadv
 */


/************************************************************************
 *
 *
 *
 ************************************************************************/


/************************************************************************
 *
 *
 *
 ************************************************************************/

#define RPPRTADVPROPERTYCODE(_code)                   ((0x01 << 24) | (_code))

#define rpPRTADVPROPERTYCODEEMITTERPRTCHAIN              RPPRTADVPROPERTYCODE(0)
#define rpPRTADVPROPERTYCODEEMITTERPRTEMITTER            RPPRTADVPROPERTYCODE(1)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTICOLOR         RPPRTADVPROPERTYCODE(2)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTITEXCOORDS     RPPRTADVPROPERTYCODE(3)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTITEXCOORDSSTEP RPPRTADVPROPERTYCODE(4)
#define rpPRTADVPROPERTYCODEEMITTERPRTMULTISIZE          RPPRTADVPROPERTYCODE(5)
#define rpPRTADVPROPERTYCODEEMITTERPOINTLIST             RPPRTADVPROPERTYCODE(6)
#define rpPRTADVPROPERTYCODEEMITTERCIRCLE                RPPRTADVPROPERTYCODE(7)
#define rpPRTADVPROPERTYCODEEMITTERSPHERE                RPPRTADVPROPERTYCODE(8)

#define rpPRTADVPROPERTYCODEPARTICLECHAIN                RPPRTADVPROPERTYCODE(0)
#define rpPRTADVPROPERTYCODEPARTICLEEMITTER              RPPRTADVPROPERTYCODE(1)
#define rpPRTADVPROPERTYCODEPARTICLEMULTICOLOR           RPPRTADVPROPERTYCODE(2)
#define rpPRTADVPROPERTYCODEPARTICLEMULTITEXCOORDS       RPPRTADVPROPERTYCODE(3)
#define rpPRTADVPROPERTYCODEPARTICLEMULTITEXCOORDSSTEP   RPPRTADVPROPERTYCODE(4)
#define rpPRTADVPROPERTYCODEPARTICLEMULTISIZE            RPPRTADVPROPERTYCODE(5)

/* Emitter type Flags */
#define rpPRTADVEMITTERPROPERTYCOUNT     (rpPRTSTDEMITTERPROPERTYCOUNT + 8)
/* Standard properties                                    0x000000FF */

/* stackable properties                                   0x0000FF00 */

#define rpPRTADVEMITTERDATAFLAGPRTEMITTER                 0x00000100
#define rpPRTADVEMITTERDATAFLAGMULTICOLOR                 0x00000200
#define rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS             0x00000400
#define rpPRTADVEMITTERDATAFLAGMULTITEXCOORDSSTEP         0x00000800
#define rpPRTADVEMITTERDATAFLAGMULTISIZE                  0x00001000
/*
#define rpPRTADVEMITTERDATAFLAGx                          0x00002000
#define rpPRTADVEMITTERDATAFLAGx                          0x00004000
#define rpPRTADVEMITTERDATAFLAGx                          0x00008000
*/
/* Emitter Type                                           0x00FF0000 */


#define rpPRTADVEMITTERDATAFLAGPOINTLIST                 0x00010000
#define rpPRTADVEMITTERDATAFLAGCIRCLE                    0x00020000
#define rpPRTADVEMITTERDATAFLAGSPHERE                    0x00030000

/*
#define rpPRTADVEMITTERDATAFLAGx                          0x00030000
#define rpPRTADVEMITTERDATAFLAGx                          0x00040000
#define rpPRTADVEMITTERDATAFLAGx                          0x00050000
#define rpPRTADVEMITTERDATAFLAGx                          0x00060000
#define rpPRTADVEMITTERDATAFLAGx                          0x00070000
#define rpPRTADVEMITTERDATAFLAGx                          0x00090000
#define rpPRTADVEMITTERDATAFLAGx                          0x000a0000
#define rpPRTADVEMITTERDATAFLAGx                          0x000b0000
#define rpPRTADVEMITTERDATAFLAGx                          0x000c0000
#define rpPRTADVEMITTERDATAFLAGx                          0x000d0000
#define rpPRTADVEMITTERDATAFLAGx                          0x000e0000
*/


/* Particle type Flags */

#define rpPRTADVPARTICLEPROPERTYCOUNT     (rpPRTSTDPARTICLEPROPERTYCOUNT + 5)
/* Standard properties                                    0x000000FF */

/* stackable properties                                   0x0000FF00 */

#define rpPRTADVPARTICLEDATAFLAGPRTEMITTER                0x00000100
#define rpPRTADVPARTICLEDATAFLAGMULTICOLOR                0x00000200
#define rpPRTADVPARTICLEDATAFLAGMULTITEXCOORDS            0x00000400
#define rpPRTADVPARTICLEDATAFLAGMULTITEXCOORDSSTEP        0x00000800
#define rpPRTADVPARTICLEDATAFLAGMULTISIZE                 0x00001000
/*
#define rpPRTADVPARTICLEDATAFLAGx                          0x00000400
#define rpPRTADVPARTICLEDATAFLAGx                          0x00000800
#define rpPRTADVPARTICLEDATAFLAGx                          0x00001000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00002000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00004000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00008000
*/

/* Particle Type                                           0x00FF0000 */

/*
#define rpPRTADVPARTICLEDATAFLAGx                          0x00010000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00020000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00030000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00040000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00050000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00060000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00070000
#define rpPRTADVPARTICLEDATAFLAGx                          0x00090000
#define rpPRTADVPARTICLEDATAFLAGx                          0x000a0000
#define rpPRTADVPARTICLEDATAFLAGx                          0x000b0000
#define rpPRTADVPARTICLEDATAFLAGx                          0x000c0000
#define rpPRTADVPARTICLEDATAFLAGx                          0x000d0000
#define rpPRTADVPARTICLEDATAFLAGx                          0x000e0000
*/

/************************************************************************
 *
 *
 *
 ************************************************************************/

#define RPPRTADVPROPERTYSIZEPARTICLEMULTICOLOR(_numFrame) \
    (sizeof(RpPrtAdvPrtMultiColor) + \
     ((_numFrame) * sizeof(RwReal)) + \
     ((_numFrame) * sizeof(RwRGBAReal) * 2))

#define RPPRTADVPROPERTYSIZEPARTICLEMULTITEXCOORDS(_numFrame) \
    (sizeof(RpPrtAdvPrtMultiTexCoords) + \
     ((_numFrame) * sizeof(RwReal)) + \
     ((_numFrame) * sizeof(RwTexCoords) * 4))

#define RPPRTADVPROPERTYSIZEPARTICLEMULTITEXSTEP(_numFrame) \
    (sizeof(RpPrtAdvPrtTexStep) + \
     ((_numFrame) * sizeof(RwReal)) + \
     ((_numFrame) * sizeof(RwTexCoords) * 2))

#define RPPRTADVPROPERTYSIZEPARTICLEMULTISIZE(_numFrame) \
    (sizeof(RpPrtAdvPrtMultiSize) + \
     ((_numFrame) * sizeof(RwReal)) + \
     ((_numFrame) * sizeof(RwV2d) * 2))

/************************************************************************
 *
 *
 *
 ************************************************************************/


typedef RpPrtStdEmitter *
        (*RpPrtAdvParticleEmitterCallBack)
            (RpAtomic *atomic, RpPrtStdEmitter *emt, void *prt, void *data);


/************************************************************************
 *
 *
 *
 ************************************************************************/

#if (!defined(DOXYGEN))
typedef struct RpPrtAdvEmitterPrtChain RpPrtAdvEmitterPrtChain;

/**
 * \ingroup rpprtadv
 * \struct RpPrtAdvEmitterPrtChain
 *
 * Might be dropped
 */
struct RpPrtAdvEmitterPrtChain
{
    RwInt32                             numPrtChain;    /**<stuff */
    RwInt32                             numPrtChainBias;/**<stuff */
};
#endif

#define RpPrtAdvPrtEmitterEmitter RpPrtAdvEmtPrtEmt

typedef struct RpPrtAdvEmtPrtEmt RpPrtAdvEmtPrtEmt;

/**
 * \ingroup prtadvprtemitter
 * \struct RpPrtAdvEmtPrtEmt
 *
 * Properties for the PrtEmitter emitter type, that allow each particle to
 * control a sub-emitter.
 */
struct RpPrtAdvEmtPrtEmt
{
    RwReal                              time;       /**<Time before first
                                                     * emission of the
                                                     * sub-emitter.*/
    RwReal                              timeBias;   /**<Time bias.*/
    RwReal                              timeGap;    /**<Time gap between each
                                                     * emission of the
                                                     * sub-emitter.*/
    RwReal                              timeGapBias;/**<Time gap bias.*/
    RpPrtStdEmitter                     *emitter;   /**<Pointer to the
                                                     * sub-emitter.*/
};


#define RpPrtAdvEmitterPrtColorItem RpPrtAdvEmtPrtColorItem

typedef struct RpPrtAdvEmtPrtColorItem RpPrtAdvEmtPrtColorItem;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvEmtPrtColorItem
 *
 * Used to store each color keyframe when using the MultiColors particles.
 */
struct RpPrtAdvEmtPrtColorItem
{
    RwReal                              time;       /**<Time.*/
    RwReal                              timeBias;   /**<Time bias.*/
    RwRGBAReal                          midCol;     /**<Color.*/
    RwRGBAReal                          midColBias; /**<Color bias.*/
};

#define RpPrtAdvPrtMultiColorEmitter RpPrtAdvEmtPrtMultiColor

typedef struct RpPrtAdvEmtPrtMultiColor RpPrtAdvEmtPrtMultiColor;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvEmtPrtMultiColor
 *
 * Properties for the MultiColor emitter type, that allow particles' colors to
 * be interpolated through several colors.
 *
 */
struct RpPrtAdvEmtPrtMultiColor
{
    RwUInt32                            numCol; /**<Number of steps.*/
    RpPrtAdvEmtPrtColorItem             *list;  /**<Pointer to the
                                                 * \ref RpPrtAdvEmtPrtColorItem
                                                 * describing the different step
                                                 *  of the color animation.*/

    RwUInt32                            offsetTime;         /**< Internal usage */
    RwUInt32                            offsetStartCol;     /**< Internal usage */
    RwUInt32                            offsetDeltaCol;     /**< Internal usage */
};

#define RpPrtAdvEmitterPrtTexCoordsItem RpPrtAdvEmtPrtTexCoordsItem

typedef struct RpPrtAdvEmtPrtTexCoordsItem RpPrtAdvEmtPrtTexCoordsItem;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvEmtPrtTexCoordsItem
 *
 * Used to store each Texture Coordinates keyframe when using the MultiTexCoords
 * particles.
 */
struct RpPrtAdvEmtPrtTexCoordsItem
{
    RwReal                              time;       /**<Time.*/
    RwReal                              timeBias;   /**<Time bias.*/
    RwTexCoords                         midUV0;     /**<Top-Left UV set.*/
    RwTexCoords                         midUV0Bias; /**<Top-Left UV set bias.*/
    RwTexCoords                         midUV1;     /**<Bottom-Right UV set.*/
    RwTexCoords                         midUV1Bias; /**<Bottom-Right UV set
                                                     * bias.*/
};

#define RpPrtAdvPrtMultiTexCoordsEmitter RpPrtAdvEmtPrtMultiTexCoords

typedef struct RpPrtAdvEmtPrtMultiTexCoords RpPrtAdvEmtPrtMultiTexCoords;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvEmtPrtMultiTexCoords
 *
 * Properties for the MultiTexCoords emitter type, that allow particles' texture
 * coordinates to be interpolated through several texture coordinates.
 */
struct RpPrtAdvEmtPrtMultiTexCoords
{
    RwUInt32                            numTex; /**<Number of steps.*/
    RpPrtAdvEmtPrtTexCoordsItem         *list;  /**<Pointer to the
                                                 * \ref RpPrtAdvEmtPrtTexCoordsItem
                                                 * describing the different step
                                                 *  of the texture coordinates
                                                 * animation.*/

    RwUInt32                            offsetTime;         /**< Internal usage */
    RwUInt32                            offsetStartUV0;     /**< Internal usage */
    RwUInt32                            offsetDeltaUV0;     /**< Internal usage */
    RwUInt32                            offsetStartUV1;     /**< Internal usage */
    RwUInt32                            offsetDeltaUV1;     /**< Internal usage */
};

#define RpPrtAdvEmitterPrtSizeItem RpPrtAdvEmtPrtSizeItem

typedef struct RpPrtAdvEmtPrtSizeItem RpPrtAdvEmtPrtSizeItem;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvEmtPrtSizeItem
 *
 * Used to store each size keyframe when using the MultiSize particles.
 */
struct RpPrtAdvEmtPrtSizeItem
{
    RwReal                              time;           /**<Time.*/
    RwReal                              timeBias;       /**<Time bias.*/
    RwV2d                               midSize;        /**<Size.*/
    RwV2d                               midSizeBias;    /**<Size bias.*/
};

#define RpPrtAdvPrtMultiSizeEmitter RpPrtAdvEmtPrtMultiSize

typedef struct RpPrtAdvEmtPrtMultiSize RpPrtAdvEmtPrtMultiSize;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvEmtPrtMultiSize
 *
 * Properties for the MultiSize emitter type, that allow particles' size to be
 * interpolated through several sizes.
 */
struct RpPrtAdvEmtPrtMultiSize
{
    RwUInt32                            numSize;    /**<Number of steps.*/
    RpPrtAdvEmtPrtSizeItem              *list;      /**<Pointer to the
                                                     * \ref RpPrtAdvEmtPrtSizeItem
                                                     * describing the different
                                                     * step of the size
                                                     * animation.*/

    RwUInt32                            offsetTime;         /**< Internal usage */
    RwUInt32                            offsetStartSize;    /**< Internal usage */
    RwUInt32                            offsetDeltaSize;    /**< Internal usage */
};

#define RpPrtAdvPointListEmitter RpPrtAdvEmtPointList

typedef struct RpPrtAdvEmtPointList RpPrtAdvEmtPointList;

/**
 * \ingroup prtadvpointlist
 * \struct RpPrtAdvEmtPointList
 *
 * Properties for the PointList emitter type, that allow to use a list of point
 * (and an optional direction) to vary the particle's position
 * (and optionally it's initial velocity).
 */
struct RpPrtAdvEmtPointList
{
    RwUInt32                            numPoint;       /**<Number of points
                                                         * in the list.*/
    RwUInt32                            lastPoint;      /**<Index of the
                                                         * last point used to
                                                         * emit particles.*/
    RwBool                              useDirection;   /**<Use or not the
                                                         * direction list to
                                                         * generate the initial
                                                         * velocity of the
                                                         * particles.*/
    RwBool                              random;         /**<Set if the points
                                                         * are to be used
                                                         * randomly.*/
    RwV3d                               *pointList;     /**<Pointer to the
                                                         * list of point.*/
    RwV3d                               *directionList; /**<Pointer to the
                                                         * list of directions.*/
};

#define RpPrtAdvCircleEmitter RpPrtAdvEmtCircle

typedef struct RpPrtAdvEmtCircle RpPrtAdvEmtCircle;

/**
 * \ingroup prtadvcircle
 * \struct RpPrtAdvEmtCircle
 *
 * Properties for the circular emitter type, that allow to use a circular, disc or
 * cylindrical shaped emitter.
 */
struct RpPrtAdvEmtCircle
{
    RwReal                              radius;             /**<Radius.*/
    RwReal                              radiusGap;          /**<Width of the
                                                             * disc.*/
    RwReal                              height;             /**<Height of the
                                                             * shape.*/

    RwBool                              useCircleEmission;  /**<Set to generate
                                                             * the particle
                                                             * initial velocity
                                                             * using it's
                                                             * position on the
                                                             * shape.*/
    RwReal                              dirRotation;        /**<Rotate the
                                                             * initial velocity
                                                             * around the circle
                                                             *  (used when
                                                             * \ref useCircleEmission
                                                             * is set to TRUE).*/
};

#define RpPrtAdvSphereEmitter RpPrtAdvEmtSphere

typedef struct RpPrtAdvEmtSphere RpPrtAdvEmtSphere;

/**
 * \ingroup prtadvsphere
 * \struct RpPrtAdvEmtSphere
 *
 * Properties for the spherical emitter type, that allow to use a spherical shaped emitter.
 */
struct RpPrtAdvEmtSphere
{
    RwReal                              radius;             /**<Radius.*/
    RwReal                              radiusGap;          /**<Width of the sphere.*/

    RwBool                              useSphereEmission;  /**<Set to set the
                                                             * particle initial
                                                             * velocity using
                                                             * it's position on
                                                             * the shape.*/
};

typedef struct RpPrtAdvParamPointList RpPrtAdvParamPointList;

/**
 * \ingroup rpprtadvmanage
 * \struct RpPrtAdvParamPointList
 *
 * Contains the parameters needed to create a PointList emitter class.
 *
 */
struct RpPrtAdvParamPointList
{
    RwUInt32                            numPoint;       /**<Number of point in
                                                         * the list.*/
    RwBool                              haveDirection;  /**<Set if you want a
                                                         * direction list to
                                                         * be created.*/
};

typedef struct RpPrtAdvMultiPropParams RpPrtAdvMultiPropParams;

/**
 * \ingroup rpprtadvmanage
 * \struct RpPrtAdvMultiPropParams
 *
 * Contains the parameters needed to create a multi-Colors/Size/Texture class.
 */
struct RpPrtAdvMultiPropParams
{
    RwUInt32                            numFrames;  /**<Desired number of
                                                     * elements.*/
};

typedef struct RpPrtAdvParamBlock RpPrtAdvParamBlock;

/**
 * \ingroup rpprtadvmanage
 * \struct RpPrtAdvParamBlock
 *
 * Parametre block passed to the Emitter and the Particle class creation.
 */
struct RpPrtAdvParamBlock
{
    RpPrtAdvParamPointList pointList;       /**<PointList Parameters.*/

    RpPrtAdvMultiPropParams colorFrames;    /**<MultiColor Parameters.*/

    RpPrtAdvMultiPropParams sizeFrames;     /**<MultiSize Parameters.*/

    RpPrtAdvMultiPropParams texFrames;      /**<MultiTexture Parameters.*/

};



/************************************************************************
 *
 *
 *
 ************************************************************************/

#define RpPrtAdvParticleEmitter RpPrtAdvPrtEmitter

typedef struct RpPrtAdvPrtEmitter RpPrtAdvPrtEmitter;

/**
 * \ingroup prtadvprtemitter
 * \struct RpPrtAdvPrtEmitter
 *
 * Properties for the PrtEmitter particle type, that allow each particle to
 * control a sub-emitter.
 */
struct RpPrtAdvPrtEmitter
{
    RwReal                              emtEmitTime;        /**<Time before
                                                             * next emission of
                                                             * the sub-emitter.*/
    RwReal                              emtEmitTimeGap;     /**<Time gap between
                                                             *  each emission of
                                                             *  the sub-emitter.*/
};

#if (!defined(DOXYGEN))

typedef struct RpPrtAdvParticleChain RpPrtAdvParticleChain;

/**
 * \ingroup rpprtadv
 * \struct RpPrtAdvParticleChain
 *
 * Might be dropped
 */
struct RpPrtAdvParticleChain
{
    RwInt32                             numPrtChain;    /**<stuff */
    RwV3d                               jitter;         /**<stuff */
    RwV3d                               pos;            /**<stuff */
};
#endif

#define RpPrtAdvParticleMultiColor RpPrtAdvPrtMultiColor

typedef struct RpPrtAdvPrtMultiColor RpPrtAdvPrtMultiColor;


/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvPrtMultiColor
 *
 * Properties for the MultiColor particle type, that allow particle's colors to
 * be interpolated through several colors.
 * These properties are generated by the MultiColor emitter.
 */
struct RpPrtAdvPrtMultiColor
{
    RwUInt32                            currIndex;  /**<Current position in the animation.*/
};

#define RpPrtAdvParticleMultiTexCoords RpPrtAdvPrtMultiTexCoords

typedef struct RpPrtAdvPrtMultiTexCoords RpPrtAdvPrtMultiTexCoords;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvPrtMultiTexCoords
 *
 * Properties for the MultiTexCoords particle type, that allow particle's
 * texture coordinates to be interpolated through several texture coordinates.
 * These properties are generated by the MultiTexCoords emitter.
 */
struct RpPrtAdvPrtMultiTexCoords
{
    RwUInt32                            currIndex;  /**<Current position in the animation.*/
};

#define RpPrtAdvParticleMultiTexCoordsStep RpPrtAdvPrtTexStep

typedef struct RpPrtAdvPrtTexStep RpPrtAdvPrtTexStep;

/**
 * \ingroup prtadvtexstep
 * \struct RpPrtAdvPrtTexStep
 *
 * Properties for the MultiTexCoordsStep particle type, that allow particle's
 * texture coordinates to be set one by one.
 * These properties are generated by the MultiTexCoordsStep emitter.
 */
struct RpPrtAdvPrtTexStep
{
    RwUInt32                            currIndex;  /**<Current position in the animation.*/
};

#define RpPrtAdvParticleMultiSize RpPrtAdvPrtMultiSize

typedef struct RpPrtAdvPrtMultiSize RpPrtAdvPrtMultiSize;

/**
 * \ingroup prtadvmultiprop
 * \struct RpPrtAdvPrtMultiSize
 *
 * Properties for the MultiSize particle type, that allow particle's sizes to be
 * interpolated through several sizes.
 * These properties are generated by the MultiSize emitter.
 */
struct RpPrtAdvPrtMultiSize
{
    RwUInt32                            currIndex;  /**<Current position in the animation.*/
};

/************************************************************************
 *
 *
 *
 ************************************************************************/


#ifdef    __cplusplus
extern "C"
{
#endif         /* __cplusplus */

/************************************************************************/

extern RwBool
RpPrtAdvPluginAttach( void );


/************************************************************************/
extern RpPrtStdEmitterClass *
RpPrtAdvEClassStdCreate( RwInt32 dataFlag, RpPrtAdvParamBlock *paramBlock);

extern RwInt32
RpPrtAdvEClassStdCreateID( RwInt32 dataFlag );

extern RpPrtStdParticleClass *
RpPrtAdvPClassStdCreate( RwInt32 dataFlag, RpPrtAdvParamBlock *paramBlock );

extern RwInt32
RpPrtAdvPClassStdCreateID( RwInt32 dataFlag );

extern RpPrtStdEmitterClass *
RpPrtAdvEClassStdSetupCB(RpPrtStdEmitterClass *eClass);

extern RpPrtStdParticleClass *
RpPrtAdvPClassStdSetupCB(RpPrtStdParticleClass *pClass);

/************************************************************************/

extern RpPrtStdEmitter *
RpPrtAdvEmitterChainEmitCB(RpAtomic *atomic,
                           RpPrtStdEmitter *emt,
                           void *data);

extern RpPrtStdParticleBatch *
RpPrtAdvParticleChainUpdateCB(RpPrtStdEmitter *emt,
                              RpPrtStdParticleBatch *prtBatch,
                              void *data);

/************************************************************************/
#define RpPrtAdvPrtEmitterUpdateCB                 RpPrtAdvPrtEmtUpdateCB
#define RpPrtAdvPrtEmitterEmitterEmitCB            RpPrtAdvEmtPrtEmtEmitCB
#define RpPrtAdvPrtEmitterEmitterCloneCB           RpPrtAdvEmtPrtEmtCloneCB
#define RpPrtAdvPrtEmitterEmitterStreamGetSizeCB   RpPrtAdvEmtPrtEmtStreamGetSizeCB
#define RpPrtAdvPrtEmitterEmitterStreamReadCB      RpPrtAdvEmtPrtEmtStreamReadCB
#define RpPrtAdvPrtEmitterEmitterStreamWriteCB     RpPrtAdvEmtPrtEmtStreamWriteCB

extern RpPrtStdEmitter *
RpPrtAdvEmtPrtEmtEmitCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                        void *data);

extern RpPrtStdParticleBatch *
RpPrtAdvPrtEmtUpdateCB(RpPrtStdEmitter *emt,
                       RpPrtStdParticleBatch *prtBatch,
                       void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPrtEmtCloneCB(RpAtomic *atomic,
                           RpPrtStdEmitter *emt,
                           void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPrtEmtStreamGetSizeCB(RpAtomic * atomic __RWUNUSED__,
                                 RpPrtStdEmitter *emt,
                                 void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPrtEmtStreamReadCB(RpAtomic * atomic __RWUNUSED__,
                              RpPrtStdEmitter *emt,
                              void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPrtEmtStreamWriteCB(RpAtomic * atomic __RWUNUSED__,
                               RpPrtStdEmitter *emt,
                               void * data);

/************************************************************************/
#define RpPrtAdvMultiPropParticleUpdateCB         RpPrtAdvPrtMultiPropUpdateCB
#define RpPrtAdvMultiPropEmitterEmitCB            RpPrtAdvEmtMultiPropEmitCB
#define RpPrtAdvMultiPropEmitterCloneCB           RpPrtAdvEmtMultiPropCloneCB
#define RpPrtAdvMultiPropEmitterStreamGetSizeCB   RpPrtAdvEmtMultiPropStreamGetSizeCB
#define RpPrtAdvMultiPropEmitterStreamReadCB      RpPrtAdvEmtMultiPropStreamReadCB
#define RpPrtAdvMultiPropEmitterStreamWriteCB     RpPrtAdvEmtMultiPropStreamWriteCB
#define RpPrtAdvMultiPropEmitterCreateCB          RpPrtAdvEmtMultiPropCreateCB

extern RpPrtStdEmitter *
RpPrtAdvEmtMultiPropEmitCB(RpAtomic *atomic,
                           RpPrtStdEmitter *emt,
                           void *data);

extern RpPrtStdParticleBatch *
RpPrtAdvPrtMultiPropUpdateCB(RpPrtStdEmitter *emt,
                             RpPrtStdParticleBatch *prtBatch,
                             void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtMultiPropCloneCB(RpAtomic *atomic,
                           RpPrtStdEmitter *emt,
                           void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtMultiPropStreamGetSizeCB(RpAtomic * atomic __RWUNUSED__,
                                    RpPrtStdEmitter *emt,
                                    void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtMultiPropStreamReadCB(RpAtomic * atomic __RWUNUSED__,
                                 RpPrtStdEmitter *emt,
                                 void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtMultiPropStreamWriteCB(RpAtomic * atomic __RWUNUSED__,
                                  RpPrtStdEmitter *emt,
                                  void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtMultiPropCreateCB(RpAtomic * atomic __RWUNUSED__,
                             RpPrtStdEmitter *emt,
                             void * data __RWUNUSED__);

/************************************************************************/
#define RpPrtAdvTexStepParticleUpdateCB         RpPrtAdvPrtTexStepUpdateCB
#define RpPrtAdvTexStepEmitterEmitCB            RpPrtAdvEmtTexStepEmitCB
#define RpPrtAdvTexStepEmitterCloneCB           RpPrtAdvEmtTexStepCloneCB
#define RpPrtAdvTexStepEmitterStreamGetSizeCB   RpPrtAdvEmtTexStepStreamGetSizeCB
#define RpPrtAdvTexStepEmitterStreamReadCB      RpPrtAdvEmtTexStepStreamReadCB
#define RpPrtAdvTexStepEmitterStreamWriteCB     RpPrtAdvEmtTexStepStreamWriteCB
#define RpPrtAdvTexStepEmitterCreateCB          RpPrtAdvEmtTexStepCreateCB


extern RpPrtStdEmitter *
RpPrtAdvEmtTexStepEmitCB(RpAtomic *atomic,
                           RpPrtStdEmitter *emt,
                           void *data);

extern RpPrtStdParticleBatch *
RpPrtAdvPrtTexStepUpdateCB(RpPrtStdEmitter *emt,
                           RpPrtStdParticleBatch *prtBatch,
                           void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtTexStepCloneCB(RpAtomic *atomic,
                          RpPrtStdEmitter *emt,
                          void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtTexStepStreamGetSizeCB(RpAtomic * atomic __RWUNUSED__,
                                  RpPrtStdEmitter *emt,
                                  void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtTexStepStreamReadCB(RpAtomic * atomic __RWUNUSED__,
                               RpPrtStdEmitter *emt,
                               void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtTexStepStreamWriteCB(RpAtomic * atomic __RWUNUSED__,
                                RpPrtStdEmitter *emt,
                                void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtTexStepCreateCB(RpAtomic * atomic __RWUNUSED__,
                           RpPrtStdEmitter *emt,
                           void * data __RWUNUSED__);

/************************************************************************/
#define RpPrtAdvPointListEmitterEmitCB          RpPrtAdvEmtPtListEmitCB
#define RpPrtAdvPointListEmitterCloneCB         RpPrtAdvEmtPtListCloneCB
#define RpPrtAdvPointListEmitterStreamGetSizeCB RpPrtAdvEmtPtListStreamGetSizeCB
#define RpPrtAdvPointListEmitterStreamReadCB    RpPrtAdvEmtPtListStreamReadCB
#define RpPrtAdvPointListEmitterStreamWriteCB   RpPrtAdvEmtPtListStreamWriteCB
#define RpPrtAdvPointListEmitterCreateCB        RpPrtAdvEmtPtListCreateCB

extern RpPrtStdEmitter *
RpPrtAdvEmtPtListEmitCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                          void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPtListCloneCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                          void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPtListStreamGetSizeCB(RpAtomic * atomic __RWUNUSED__,
                                       RpPrtStdEmitter *emt,
                                       void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPtListStreamReadCB(RpAtomic * atomic __RWUNUSED__,
                                       RpPrtStdEmitter *emt,
                                       void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPtListStreamWriteCB(RpAtomic * atomic __RWUNUSED__,
                                       RpPrtStdEmitter *emt,
                                       void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtPtListCreateCB(RpAtomic * atomic __RWUNUSED__,
                                RpPrtStdEmitter *emt,
                                void * data __RWUNUSED__);

/************************************************************************/
#define RpPrtAdvCircleEmitterEmitCB          RpPrtAdvEmtCircleEmitCB
#define RpPrtAdvCircleEmitterCloneCB         RpPrtAdvEmtCircleCloneCB
#define RpPrtAdvCircleEmitterStreamGetSizeCB RpPrtAdvEmtCircleStreamGetSizeCB
#define RpPrtAdvCircleEmitterStreamReadCB    RpPrtAdvEmtCircleStreamReadCB
#define RpPrtAdvCircleEmitterStreamWriteCB   RpPrtAdvEmtCircleStreamWriteCB

extern RpPrtStdEmitter *
RpPrtAdvEmtCircleEmitCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                        void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtCircleCloneCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                        void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtCircleStreamGetSizeCB(RpAtomic * atomic __RWUNUSED__,
                                 RpPrtStdEmitter *emt,
                                 void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtCircleStreamReadCB(RpAtomic * atomic __RWUNUSED__,
                              RpPrtStdEmitter *emt,
                              void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtCircleStreamWriteCB(RpAtomic * atomic __RWUNUSED__,
                               RpPrtStdEmitter *emt,
                               void * data);

/************************************************************************/
#define RpPrtAdvSphereEmitterEmitCB          RpPrtAdvEmtSphereEmitCB
#define RpPrtAdvSphereEmitterCloneCB         RpPrtAdvEmtSphereCloneCB
#define RpPrtAdvSphereEmitterStreamGetSizeCB RpPrtAdvEmtSphereStreamGetSizeCB
#define RpPrtAdvSphereEmitterStreamReadCB    RpPrtAdvEmtSphereStreamReadCB
#define RpPrtAdvSphereEmitterStreamWriteCB   RpPrtAdvEmtSphereStreamWriteCB

extern RpPrtStdEmitter *
RpPrtAdvEmtSphereEmitCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                        void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtSphereCloneCB(RpAtomic *atomic,
                        RpPrtStdEmitter *emt,
                        void *data);

extern RpPrtStdEmitter *
RpPrtAdvEmtSphereStreamGetSizeCB(RpAtomic * atomic __RWUNUSED__,
                                 RpPrtStdEmitter *emt,
                                 void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtSphereStreamReadCB(RpAtomic * atomic __RWUNUSED__,
                              RpPrtStdEmitter *emt,
                              void * data);

extern RpPrtStdEmitter *
RpPrtAdvEmtSphereStreamWriteCB(RpAtomic * atomic __RWUNUSED__,
                               RpPrtStdEmitter *emt,
                               void * data);

/************************************************************************/

#ifdef    __cplusplus
}
#endif         /* __cplusplus */


/*---- end: ./prtadv.h----*/

/*---- start: c:/RW/Graphics/rwsdk/plugin/prtadv/d3d9/prtadvplatform.h----*/
/*---- end: c:/RW/Graphics/rwsdk/plugin/prtadv/d3d9/prtadvplatform.h----*/

#endif /* RPPRTADV_H */


