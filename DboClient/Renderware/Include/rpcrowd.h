#ifndef RPCROWD_H
#define RPCROWD_H

/**
 * \defgroup rpcrowd RpCrowd
 * \ingroup fxpack
 *
 * Crowd Plugin for RenderWare Graphics. 
 */

/*===========================================================================*
 *--- Include files ---------------------------------------------------------*
 *===========================================================================*/

#include "rwcore.h"
#include "rpworld.h"

/*===========================================================================*
 *--- Defines ---------------------------------------------------------------*
 *===========================================================================*/

/**
 * \ingroup rpcrowd
 * \ref rpCROWDANIMATIONNUMSEQ defines the number of animation
 * sequences in an \ref RpCrowdAnimation.
 */
#define rpCROWDANIMATIONNUMSEQ      (4)

/**
 * \ingroup rpcrowd
 * \ref rpCROWDANIMATIONMAXSTEPS defines the maximum number of steps in an 
 * \ref RpCrowdAnimation sequence.
 */
#define rpCROWDANIMATIONMAXSTEPS    (16)

/*===========================================================================*
 *--- Global Types ----------------------------------------------------------*
 *===========================================================================*/

/**
 * \ingroup rpcrowd
 * An \ref RpCrowdTexture is a normal \ref RwTexture organised into
 * a grid consisting of a row of spectators (columns A to X) in N different
 * poses (rows 0 to N-1) for animation. The number of columns (X) should be
 * a power of 2.
 * 
 * \verbatim
      +----+----+----+----+----+----+---...----+
      | A0 | B0 | C0 | D0 | E0 | F0 |   ... X0 |
      +----+----+----+----+----+----+---...----+
      | A1 | B1 | C1 | D1 | E1 | F1 |   ... X1 |
      +----+----+----+----+----+----+---...----+
      | A2 | B2 | C2 | D2 | E2 | F2 |   ... X2 |
      +----+----+----+----+----+----+---...----+
      | A3 | B3 | C3 | D3 | E3 | F3 |   ... X3 |
      +----+----+----+----+----+----+---...----+
      | A4 | B4 | C4 | D4 | E4 | F4 |   ... X4 |
      +----+----+----+----+----+----+---...----+
      | A5 | B5 | C5 | D5 | E5 | F5 |   ... X5 |
      +----+----+----+----+----+----+---...----+
      | A6 | B6 | C6 | D6 | E6 | F6 |   ... X6 |
      ..........................................
    
      |AN-1|BN-1|CN-1|CN-1|EN-1|FN-1|   ...XN-1|
      +----+----+----+----+----+----+---...----+
 \endverbatim
 * 
 * The row of spectators is used over and over again to generate the whole
 * crowd. A single crowd billboard will start at a random spectator position
 * along a row and can include any number of spectators using texture
 * wrapping. For instance, if 'H' is the last column, an actual billboard
 * might contain 'EFGHABCDEFGHAB'.
 *
 * Crowd billboards are animated according to an \ref RpCrowdAnimation
 * which specifies which texture rows to use as time advances. Different
 * billboards start off at different points in the animation to add to the
 * randomness.
 *
 * When an \ref RpCrowd is created, any number of crowd textures can be 
 * registered, and the application may switch between them using 
 * \ref RpCrowdSetCurrentTexture. This might be used to make the crowd
 * react to certain events, for example.
 *
 * \see \ref rpcrowdoverview
 */
typedef RwTexture RpCrowdTexture;

/**
 * \ingroup rpcrowd
 * \struct RpCrowdAnimation
 * An RpCrowdAnimation specifies which rows should be selected from an
 * \ref RpCrowdTexture as time advances (each row of the texture
 * corresponds to the same spectators but in a different pose).
 *
 * Four independent looping animation sequences are available, with a maximum
 * of 16 steps in each. A particular billboard of the crowd will be randomly
 * assigned to one of the sequences and started off at a random point in that
 * sequence.
 *
 * When a crowd is created with \ref RpCrowdCreate, any number of animations
 * can be registered, and the application may switch between them as 
 * required using \ref RpCrowdSetCurrentAnim. This might happen
 * simultaneously with a switch to another texture when certain events take
 * place, such as goal scores.
 *
 * The crowd animation state is updated using \ref RpCrowdAddAnimTime.
 *
 * An example of an animation is shown below. The 1st sequence (first
 * column) shows a ping-pong animation loop. The 4th sequence uses the
 * same loop but at a different speed. The 2nd and 3rd sequences represent
 * a cycling animation at different speeds.
 *
 * \verbatim
    RpCrowdAnimation CrowdAnim1 =
        { 2.0f,
          16,    
          { {0, 4, 4, 0},   
            {1, 4, 4, 0},
            {2, 5, 4, 1},
            {3, 5, 5, 1},
            {3, 6, 5, 2},
            {2, 6, 5, 2},
            {1, 7, 5, 3},
            {0, 7, 5, 3},
            {0, 7, 6, 3},
            {1, 4, 6, 3},
            {2, 5, 6, 2},
            {3, 6, 6, 1},
            {3, 6, 6, 1},
            {2, 6, 7, 1},
            {1, 7, 7, 0},
            {0, 7, 7, 0} } }; 
   \endverbatim
 *
 * \see \ref rpcrowdoverview
 */
struct RpCrowdAnimation
{
    RwReal  duration;   /**< Duration of the animation (arbitrary units) */
    RwInt32 numSteps;	/**< Number of steps in the animation sequences
                             (maximum 16). */
    RwInt32 sequences[rpCROWDANIMATIONMAXSTEPS][rpCROWDANIMATIONNUMSEQ]; 
                        /**< Table of 4 looping animation sequences. Each
                             sequence is an array of \e numSteps indices
                             specifying the rows of the crowd texture to use
                             in the animation. */
};
typedef struct RpCrowdAnimation RpCrowdAnimation;

/**
 * \ingroup rpcrowd
 * \struct RpCrowdParam
 * Crowd parameters for use with \ref RpCrowdCreate, specifying information
 * about the crowd geometry, textures, and animations. 
 *
 * An internal copy of this structure is stored, and may be accessed
 * (read only) at a later time using \ref RpCrowdGetParam. Note however,
 * that depending on the platform, the original base atomic and crowd
 * textures may no longer be referenced after creation (see \ref
 * RpCrowdCreate).
 *
 * \see \ref rpcrowdoverview
 */
struct RpCrowdParam
{
    RpAtomic    *base;          /**< Base atomic - triangle pairs form
                                     quads representing the crowd
                                     seating sections. See \ref
                                     rpcrowdoverview. */
    RwUInt32    densityU;       /**< Number of crowd spectators across a
                                     billboard per unit of a base quad's U
                                     texture coordinate */
    RwUInt32    densityV;       /**< Number of crowd rows per unit of
                                     a base quad's V texture coordinate. eg a
                                     base quad mapped to uv range [0,0],[1,1]
                                     has densityU number of people in 
                                     densityV number of rows. */
    RwV3d       up;             /**< World space up-vector (see platform
                                     notes) */
    RwReal      height;         /**< Height of a billboard in world units */

    RwUInt32    numTextures;    /**< Number of textures in the array */
    RwUInt32    numTexCols;     /**< Number of columns in an
                                     \ref RpCrowdTexture */
    RwUInt32    numTexRows;     /**< Number of rows in an
                                     \ref RpCrowdTexture */
    RpCrowdTexture **textures;  /**< \ref RpCrowdTexture array */

    RwUInt32    numAnims;       /**< Number of animations in the array */
    RpCrowdAnimation **anims;   /**< \ref RpCrowdAnimation array */
};
typedef struct RpCrowdParam RpCrowdParam;

/**
 * \ingroup rpcrowd
 * \struct RpCrowd
 * This should be considered an opaque type.
 *
 * \see \ref rpcrowdoverview
 */
typedef struct RpCrowd RpCrowd;

/*===========================================================================*
 *--- Plugin API Functions --------------------------------------------------*
 *===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * Plugin attach
 */
extern RwBool
RpCrowdPluginAttach(void);

/*
 * Creation and destruction
 */
extern RpCrowd *
RpCrowdCreate(RpCrowdParam *param);

extern RwBool
RpCrowdDestroy(RpCrowd *crowd);

/*
 * Animation update
 */
extern RpCrowd *
RpCrowdAddAnimTime(RpCrowd *crowd, RwReal time);

/*
 * Rendering
 */
extern RpCrowd *
RpCrowdRender(RpCrowd *crowd);

/*
 * Get/Set functions
 */
extern const RpCrowdParam *
RpCrowdGetParam(const RpCrowd *crowd);

extern RpCrowd *
RpCrowdSetCurrentTexture(RpCrowd *crowd, RwUInt32 index);

extern RwUInt32
RpCrowdGetCurrentTexture(RpCrowd *crowd);

extern RpCrowd *
RpCrowdSetCurrentAnim(RpCrowd *crowd, RwUInt32 index);

extern RwUInt32
RpCrowdGetCurrentAnim(RpCrowd *crowd);

extern RpCrowd *
RpCrowdSetCurrentAnimTime(RpCrowd *crowd, RwReal time);

extern RwReal
RpCrowdGetCurrentAnimTime(RpCrowd *crowd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPCROWD_H */
