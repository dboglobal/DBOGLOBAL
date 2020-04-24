#ifndef RPTEAM_H
#define RPTEAM_H

/**
 * \defgroup rpteam RpTeam
 * \ingroup fxpack
 *
 * Team Plugin for RenderWare.
 */

/*===========================================================================*
 *--- Include files ---------------------------------------------------------*
 *===========================================================================*/
#include "rwcore.h"
#include "rpworld.h"
#include "rpskin.h"

/*===========================================================================*
 *--- Defines ---------------------------------------------------------------*
 *===========================================================================*/

/**
 * \ingroup rpteam
 * \ref rpTEAMELEMENTUNUSED should be used for the dictionary index when
 * disabling an element that a player is constructed.
 * A player is constructed using \ref RpTeamPlayerSetElement
 */
#define rpTEAMELEMENTUNUSED (~((RwUInt32)0))

/**
 * \ingroup rpteam
 * \ref rpTEAMELEMENTMAXLOD defines the maximum number of level of details
 * that can be added to the dictionary for a single element.
 *
 * \see RpTeamDictionarySetLODElement
 * \see RpTeamDictionaryGetLODElement
 * \see RpTeamPlayerSetCurrentLOD
 * \see RpTeamPlayerGetCurrentLOD
 */
#define rpTEAMELEMENTMAXLOD (5)

/**
 * \ingroup rpteam
 * \ref rpTEAMMAXNUMSHADOWS defines the maximum number of shadows the Team
 * plugin can render.
 *
 * \see RpTeamShadows
 * \see RpTeamSetShadows
 * \see RpTeamGetShadows
 */
#define rpTEAMMAXNUMSHADOWS (4)

/**
 * \ingroup rpteam
 * \ref rpTEAMMAXNUMBONES defines the maximum number of bones the Team
 * plugin can render for a single skinned character.
 */
#define rpTEAMMAXNUMBONES (32)

/*===========================================================================*
 *--- Global Types ----------------------------------------------------------*
 *===========================================================================*/

/**
 * \ingroup rpteam
 * \ref RpTeamElementType defines the different type of atomic which the
 * Team can control. The type is used when setting elements into the team's
 * dictionary.
 *
 * \see RpTeamDictionarySetElement
 * \see RpTeamDictionaryGetElement
 */
enum RpTeamElementType
{
    rpNATEAMELEMENTTYPE  = 0,
    rpTEAMELEMENTSTATIC  = 0x01, /**<The atomic contains static geometry.  */
    rpTEAMELEMENTSKINNED = 0x02, /**<The atomic contains skinned geometry. */
    rpTEAMELEMENTPATCH   = 0x04, /**<The atomic contains patched geometry. */
    rpTEAMELEMENTALL     = 0xff,
    rpTEAMELEMENTFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpTeamElementType RpTeamElementType;

/**
 * \ingroup rpteam
 * \ref RpTeamLockMode defines the different areas of the team which can be
 * locked and then altered.
 *
 * \see RpTeamLock
 * \see RpTeamUnlock
 */
enum RpTeamLockMode
{
    rpNATEAMLOCKMODE         = 0,
    rpTEAMLOCKDICTIONARY     = 0x01, /**<Lock the team's dictionary.         */
    rpTEAMLOCKPLAYERS        = 0x02, /**<Lock the team's players.            */
    rpTEAMLOCKPLAYERLODS     = 0x04, /**<Lock the team's player's LODS.      */
    rpTEAMLOCKTEAMLOD        = 0x08, /**<Lock the team LOD setup.            */
    rpTEAMLOCKTEXDICTIONARYS = 0x10, /**<Lock the team's texture dictionary. */
    rpTEAMLOCKSHADOWS        = 0x20, /**<Lock the team shadow data.          */
    rpTEAMLOCKPLAYERSHADOWS  = 0x40, /**<Lock the team's player shadows.     */
    rpTEAMLOCKLIGHTS         = 0x80, /**<Lock the team's persistent lights.  */
    rpTEAMLOCKALL            = 0xff, /**<Lock the entire team.               */
    rpTEAMLOCKFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpTeamLockMode RpTeamLockMode;

/**
 * \ingroup rpteam
 * \ref RpTeamElementLOD defines the special LOD elements. The values
 * rpTEAMELEMENTLODSHADOW1 and rpTEAMELEMENTLODSHADOW4 are used whilst
 * rendering the player shadows and should be heavily reduced LOD.
 *
 * \see RpTeamShadows
 * \see RpTeamDictionarySetLODElement
 * \see RpTeamDictionaryGetLODElement
 */
enum RpTeamElementLOD
{
    rpTEAMELEMENTLODFIRST   = 0,                   /**<Highest LOD.     */
    rpTEAMELEMENTLODMAX     = rpTEAMELEMENTMAXLOD, /**<Lowest LOD.      */
    rpTEAMELEMENTLODSHADOW1 = rpTEAMELEMENTMAXLOD, /**<One shadow LOD.  */
    rpTEAMELEMENTLODSHADOW4,                       /**<Four shadow LOD. */
    rpTEAMELEMENTLODSUP,
    rpTEAMELEMENTLODFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpTeamElementLOD RpTeamElementLOD;

/**
 * \ingroup rpteam
 * \ref RpTeamRenderMode defines the rendering features to enable. These
 * flags can be ORed together to control the \ref RpTeamRender functions.
 *
 * \see RpTeamRender
 */
enum RpTeamRenderMode
{
    rpNATEAMRENDERMODE           = 0x00,
    rpTEAMRENDERUPDATELODS       = 0x01, /**<Update player LOD.           */
    rpTEAMRENDERUPDATEVISIBILITY = 0x02, /**<Update player visibility.    */
    rpTEAMRENDERUPDATEHIERARCHYS = 0x04, /**<Update player hierarchys.    */
    rpTEAMRENDERTEAM             = 0x08, /**<Render team players.         */
    rpTEAMRENDERALL              = 0x0F, /**<Enable all render functions. */
    rpTEAMRENDERFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpTeamRenderMode RpTeamRenderMode;

typedef struct RpTeamShadows RpTeamShadows;
/**
 * \ingroup rpteam
 * \struct RpTeamShadows
 * contains the data for rendering a teams shadows.
 * Each team has it's own shadow data.
 *
 * \see RpTeamSetShadows
 * \see RpTeamGetShadows
 */
struct RpTeamShadows
{
    RwFrame  *shadowFrames[rpTEAMMAXNUMSHADOWS]; /**<Frames of projection. */
    RwReal    shadowValues[rpTEAMMAXNUMSHADOWS]; /**<Shadow darkness.      */
    RwFrame  *shadowPlane;       /**<Plane the shadows are projected onto. */
    RwUInt32  number;            /**<Number of shadows to render. */
    RpTeamElementLOD shadowLOD;  /**<LOD to render.        */
};

/**
 * \ingroup rpteam
 * \ref RpTeamPlayer represents a player in a team. They are retrieved by
 * \ref RpTeamGetPlayer. This should be considered an opaque type. Use the
 * RpTeamPlayer API functions to access it.
 *
 * \see RpTeamPlayerSetElement
 * \see RpTeamPlayerGetElement
 * \see RpTeamPlayerSetHierarchyFrame
 * \see RpTeamPlayerGetHierarchyFrame
 * \see RpTeamPlayerSetActivity
 * \see RpTeamPlayerGetActivity
 * \see RpTeamPlayerSetRenderCallBack
 * \see RpTeamPlayerGetRenderCallBack
 * \see RpTeamPlayerSetCurrentLOD
 * \see RpTeamPlayerGetCurrentLOD
 * \see RpTeamPlayerSetSyncLODPerFrame
 * \see RpTeamPlayerGetSyncLODPerFrame
 * \see RpTeamPlayerGetBoundingSphere
 */
typedef struct RpTeamPlayer RpTeamPlayer;

/**
 * \ingroup rpteam
 * \ref RpTeamDictionary represents the part dictionary of a team. It can be
 * retrieved by \ref RpTeamGetDictionary. This should be considered an opaque
 * type. Use the RpTeamDictionary API functions to access it.
 *
 * \see RpTeamDictionaryGetNumOfElements
 * \see RpTeamDictionarySetElement
 * \see RpTeamDictionaryGetElement
 * \see RpTeamDictionarySetLODElement
 * \see RpTeamDictionaryGetLODElement
 */
typedef struct RpTeamDictionary RpTeamDictionary;

/**
 * \ingroup rpteam
 * \ref RpTeam represents a single team. This should be considered an opaque
 * type. Use the RpTeam API functions to access it.
 *
 * \see RpTeamCreate
 * \see RpTeamDestroy
 * \see RpTeamGetNumberOfPlayers
 * \see RpTeamGetNumberOfElements
 * \see RpTeamGetDictionary
 * \see RpTeamGetPlayer
 * \see RpTeamRender
 * \see RpTeamLock
 * \see RpTeamUnlock
 * \see RpTeamSetShadows
 * \see RpTeamGetShadows
 */
typedef struct RpTeam RpTeam;

/**
 * \ingroup rpteam
 * \ref RpTeamLODCamera represents a camera within the team used to
 * calculate the LOD for the players.
 *
 * \see RpTeamSetLODCamera
 * \see RpTeamGetLODCamera
 */
typedef RwCamera *RpTeamLODCamera;

/**
 * \ingroup rpteam
 * \ref RpTeamLODData represents the user data to be passed to the
 * \ref RpTeamLODCallBack function.
 *
 * \see RpTeamSetLODCallBack
 * \see RpTeamGetLODCallBack
 */
typedef void *RpTeamLODData;

/**
 * \ingroup rpteam
 * \ref RpTeamRenderData represents the user data to be passeds to the
 * \ref RpTeamPlayerRenderCallBack function.
 *
 * \see RpTeamPlayerSetRenderCallBack
 * \see RpTeamPlayerGetRenderCallBack
 */
typedef void *RpTeamRenderData;

/**
 * \ingroup rpteam
 * \ref RpTeamLODCallBack represents the function called from
 * \ref RpTeamRender to calculate the correct LOD for each player. Only
 * players which are active in the team will be processed,
 * \ref RpTeamPlayerSetActivity. A player will also be processed if it
 * has been set to synchronize per frame, \ref RpTeamPlayerSetSyncLODPerFrame.
 *
 * \param team         Team which is being rendered.
 * \param playerIndex  Index of player to calculate LOD.
 * \param data         User callback data.
 *
 * \return The present LOD of the player.
 *
 * \see RpTeamPlayerSetRenderCallBack
 * \see RpTeamPlayerGetRenderCallBack
 * \see RpTeamPlayerSetActivity
 * \see RpTeamPlayerGetActivity
 * \see RpTeamPlayerSetSyncLODPerFrame
 * \see RpTeamPlayerGetSyncLODPerFrame
 */
typedef RwUInt32(*RpTeamLODCallBack) (const RpTeam * team,
                                      RwUInt32 playerIndex,
                                      RpTeamLODData data);

/**
 * \ingroup rpteam
 * \ref RpTeamPlayerRenderCallBack represents the function called from
 * \ref RpTeamRender to allow player specific alteration to atomics. The
 * callback is set per player by \ref RpTeamPlayerSetRenderCallBack.
 *
 * \param team        Team which the player is a member of.
 * \param playerIndex Index of player being rendered.
 * \param atomic      Atomic which is being rendered as a part of the player.
 * \param data        User callback data.
 *
 * \return Pointer to the current atomic.
 */
typedef RpAtomic *(*RpTeamPlayerRenderCallBack) (const RpTeam * team,
                                                 RwUInt32 playerIndex,
                                                 RpAtomic * atomic,
                                                 RpTeamRenderData data);

/**
 * \ingroup rpteam
 * \ref RpTeamCameraTestSphereData represents the user data to be
 * passed to the \ref RpTeamCameraTestSphere function.
 *
 * \see RpTeamSetCameraTestSphereCallBack
 * \see RpTeamGetCameraTestSphereCallBack
 */
typedef void *RpTeamCameraTestSphereData;

/**
 * \ingroup rpteam
 * \ref RpTeamCameraTestSphere represents the function called from
 * \ref RpTeamRender to allow custom player culling. The callback is
 * set per team by \ref RpTeamSetCameraTestSphereCallBack.
 *
 * The function should return:
 * \li \ref rwSPHEREOUTSIDE  Don't render this player.
 * \li \ref rwSPHEREBOUNDARY Render the player clipped.
 * \li \ref rwSPHEREINSIDE   Render the player unclipped.
 *
 * Even if \ref rwSPHEREOUTSIDE is returned, \ref RpTeam may still
 * decide to render the player's shadows if they're visible.
 *
 * To stop a player from being rendered completely unselect the
 * player with \ref RpTeamPlayerSetActivity
 *
 * \param team        Team which the player is a member of.
 * \param playerIndex Index of player being camera tested.
 * \param camera      Camera to test the player against.
 * \param sphere      World space transformed player bounding sphere.
 * \param data        User callback data.
 *
 * \return A \ref RwFrustumTestResult value indicating the
 * result. Results can be: outside the frustum (\ref rwSPHEREOUTSIDE), on
 * the boundary of the frustum (\ref rwSPHEREBOUNDARY), or totally within
 * the frustum (\ref rwSPHEREINSIDE). Returns (\ref rwSPHEREOUTSIDE)
 * if an error occurred.

 *
 * \see RpTeamSetCameraTestSphereCallBack
 * \see RpTeamGetCameraTestSphereCallBack
 */
typedef RwFrustumTestResult (* RpTeamCameraTestSphere)
    (const RpTeam * team,
     RwUInt32 playerIndex,
     const RwCamera * camera,
     const RwSphere * sphere,
     RpTeamCameraTestSphereData data);

/*===========================================================================*
 *--- Plugin API Functions --------------------------------------------------*
 *===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*---------------------------------------------------------------------------*
 *-   Plugin functions                                                      -*
 *---------------------------------------------------------------------------*/
extern RwBool
RpTeamPluginAttach(void);

/*---------------------------------------------------------------------------*
 *-   Team functions                                                        -*
 *---------------------------------------------------------------------------*/
extern RpTeam *
RpTeamCreate(RwUInt32 numOfPlayers,
             RwUInt32 numOfPlayerElements);

extern RwBool
RpTeamDestroy(RpTeam * team);

extern RwUInt32
RpTeamGetNumberOfPlayers(const RpTeam * team);

extern RwUInt32
RpTeamGetNumberOfElements(const RpTeam * team);

extern RpTeamDictionary *
RpTeamGetDictionary(const RpTeam * team);

extern RpTeam *
RpTeamSetDictionary(RpTeam * team, RpTeamDictionary * dictionary);

extern RpTeamPlayer *
RpTeamGetPlayer(const RpTeam * team,
                RwUInt32 index);

extern const RpTeam *
RpTeamRender(const RpTeam * team, RwUInt32 flags);

extern RpTeam *
RpTeamLock(RpTeam * team, RwUInt32 lockMode);

extern RpTeam *
RpTeamUnlock(RpTeam * team);

extern RpTeam *
RpTeamSetCameraTestSphereCallBack(RpTeam * team,
                                  RpTeamCameraTestSphere callBack,
                                  RpTeamCameraTestSphereData data);

extern const RpTeam *
RpTeamGetCameraTestSphereCallBack(const RpTeam * team,
                                  RpTeamCameraTestSphere * callBack,
                                  RpTeamCameraTestSphereData * data);

/*---------------------------------------------------------------------------*
 *-   TeamDictionary functions                                              -*
 *---------------------------------------------------------------------------*/
extern RpTeamDictionary *
RpTeamDictionaryCreate(RwUInt32 numOfDictionaryElements);

extern RwBool
RpTeamDictionaryDestroy(RpTeamDictionary * dictionary);

extern RwUInt32
RpTeamDictionaryGetNumOfElements(const RpTeamDictionary * dictionary);

extern RpTeamDictionary *
RpTeamDictionarySetElement(RpTeamDictionary * dictionary,
                           RpAtomic * atomic,
                           RpTeamElementType type,
                           RwUInt32 dictionaryIndex);

extern RpAtomic *
RpTeamDictionaryGetElement(const RpTeamDictionary * dictionary,
                           RwUInt32 dictionayIndex);

extern RpTeamDictionary *
RpTeamDictionarySetLODElement(RpTeamDictionary * dictionary,
                              RpAtomic * atomic,
                              RpTeamElementType type,
                              RwUInt32 lodIndex,
                              RwUInt32 dictionaryIndex);

extern RpAtomic *
RpTeamDictionaryGetLODElement(const RpTeamDictionary * dictionary,
                              RwUInt32 lodIndex,
                              RwUInt32 dictionaryIndex);

extern RpTeamDictionary *
RpTeamDictionaryInstance(RpTeamDictionary *dictionary);

/*---------------------------------------------------------------------------*
 *-   TeamPlayer functions                                                  -*
 *---------------------------------------------------------------------------*/
extern RpTeamPlayer *
RpTeamPlayerSetElement(RpTeamPlayer * player,
                       RwUInt32 index,
                       RwUInt32 dictionaryIndex);

extern RwUInt32
RpTeamPlayerGetElement(const RpTeamPlayer * player,
                       RwUInt32 index);

extern RpTeamPlayer *
RpTeamPlayerSetHierarchyFrame(RpTeamPlayer * player,
                              RwFrame * frame);

extern RwFrame *
RpTeamPlayerGetHierarchyFrame(const RpTeamPlayer * player);

extern RpTeamPlayer *
RpTeamPlayerSetActivity(RpTeamPlayer * player,
                        RwBool active);

extern RwBool

RpTeamPlayerGetActivity(const RpTeamPlayer * player);

extern RpTeamPlayer *
RpTeamPlayerSetRenderCallBack(RpTeamPlayer *player,
                              RpTeamPlayerRenderCallBack preCallBack,
                              RpTeamPlayerRenderCallBack postCallBack,
                              RpTeamRenderData data);

extern const RpTeamPlayer *
RpTeamPlayerGetRenderCallBack(const RpTeamPlayer * player,
                              RpTeamPlayerRenderCallBack * preCallBack,
                              RpTeamPlayerRenderCallBack * postCallBack,
                              RpTeamRenderData * data);

extern RwSphere *
RpTeamPlayerGetBoundingSphere(RpTeamPlayer * player);

extern RpTeamPlayer *
RpTeamPlayerSetShadows(RpTeamPlayer *player,
                       RwBool active);

extern RwBool
RpTeamPlayerGetShadows(RpTeamPlayer *player);

/*---------------------------------------------------------------------------*
 *-   TeamLOD functions                                                     -*
 *---------------------------------------------------------------------------*/
extern RpTeam *
RpTeamSetLODCamera(RpTeam * team,
                   RpTeamLODCamera camera);

extern RpTeam *
RpTeamSetLODFarRange(RpTeam * team,
                     RwReal farRange);

extern RpTeam *
RpTeamSetLODCallBack(RpTeam * team,
                     RpTeamLODCallBack callBack,
                     RpTeamLODData data);

extern RpTeamLODCamera
RpTeamGetLODCamera(const RpTeam * team);

extern RwReal
RpTeamGetLODFarRange(const RpTeam * team);

extern const RpTeam *
RpTeamGetLODCallBack(const RpTeam * team,
                     RpTeamLODCallBack * callBack,
                     RpTeamLODData * data);

/*---------------------------------------------------------------------------*
 *-   TeamPlayerLOD functions                                               -*
 *---------------------------------------------------------------------------*/
extern RpTeamPlayer *
RpTeamPlayerSetCurrentLOD(RpTeamPlayer * player,
                          RwUInt32 lod);

extern RwUInt32
RpTeamPlayerGetCurrentLOD(const RpTeamPlayer * player);

extern RpTeamPlayer *
RpTeamPlayerSetSyncLODPerFrame(RpTeamPlayer * player,
                               RwBool syncLODPerFrame);

extern RwBool
RpTeamPlayerGetSyncLODPerFrame(RpTeamPlayer * player);

/*---------------------------------------------------------------------------*
 *-   TeamShadows functions                                                 -*
 *---------------------------------------------------------------------------*/
extern RpTeam *
RpTeamSetShadows(RpTeam * team,
                 RpTeamShadows * shadows);

extern RpTeamShadows *
RpTeamGetShadows(const RpTeam * team);

/*---------------------------------------------------------------------------*
 *-   TeamMaterial functions                                                -*
 *---------------------------------------------------------------------------*/
extern RpMaterial *
RpTeamMaterialSetTeamTexture(RpMaterial * material,
                             const RpTeam * team,
                             RwTexture * texture);

extern RwTexture *
RpTeamMaterialGetTeamTexture(const RpMaterial * material,
                             const RpTeam * team);

extern RpMaterial *
RpTeamMaterialSetPlayerTexture(RpMaterial * material,
                               const RpTeamPlayer * player,
                               RwTexture * texture);

extern RwTexture *
RpTeamMaterialGetPlayerTexture(const RpMaterial * material,
                               const RpTeamPlayer * player);

extern RpMaterial *
RpTeamMaterialSetPlayerDualTexture(RpMaterial * material,
                                   const RpTeamPlayer * player,
                                   RwTexture * texture);

extern RwTexture *
RpTeamMaterialGetPlayerDualTexture(const RpMaterial * material,
                                   const RpTeamPlayer * player);

/*---------------------------------------------------------------------------*/
extern RpMaterial *
RpTeamMaterialSetPlayerColor(RpMaterial * material,
                             const RpTeamPlayer * player,
                             const RwRGBA * color);

extern const RwRGBA *
RpTeamMaterialGetPlayerColor(const RpMaterial * material,
                             const RpTeamPlayer * player);

/*---------------------------------------------------------------------------*
 *-   TeamStream functions                                                  -*
 *---------------------------------------------------------------------------*/
extern RwUInt32
RpTeamStreamGetSize(const RpTeam * team);

extern RpTeam *
RpTeamStreamRead(RwStream * stream,
                 RpTeamDictionary * dictionary);

extern const RpTeam *
RpTeamStreamWrite(const RpTeam * team,
                  RwStream * stream);

extern RwUInt32
RpTeamDictionaryStreamGetSize(const RpTeamDictionary * dictionary);

extern RpTeamDictionary *
RpTeamDictionaryStreamRead(RwStream * stream);

extern const RpTeamDictionary *
RpTeamDictionaryStreamWrite(const RpTeamDictionary * dictionary,
                            RwStream * stream);

/*---------------------------------------------------------------------------*
 *-   Team light functions                                                  -*
 *---------------------------------------------------------------------------*/
extern RpTeam *
RpTeamSetPersistentLights(RpTeam * team,
                          RpLight * directional,
                          RpLight * ambient);

extern const RpTeam *
RpTeamGetPersistentLights(const RpTeam * team,
                          RpLight ** directional,
                          RpLight ** ambient);

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RPTEAM_H */
