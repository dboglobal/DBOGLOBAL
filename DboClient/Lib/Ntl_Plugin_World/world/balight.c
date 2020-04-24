
/**
 * \ingroup rplight
 * \page rplightoverview RpLight Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h, rpworld.h
 * \li \b Libraries: rwcore, rpworld
 * \li \b Plugin \b attachments: \ref RpWorldPluginAttach
 *
 * \subsection lightoverview Overview
 *
 * This object represents dynamic lighting in a RenderWare Graphics Retained
 * Mode scene.
 * Lighting models available are:
 *
 * \li Ambient
 * \li Directional
 * \li Point
 * \li Spotlight
 * \li Soft Spotlight
 *
 * Directional, Point, Spotlight and Soft Spotlight types all require that a Frame
 * \ref RwFrame be attached to the Light. An ambient type Light does not require a
 * Frame as positioning is not needed. It is possible to specify whether a particular
 * Light illuminates dynamic models, static models or both by setting the appropriate
 * flags.
 *
 * Further information is available in the Dynamic Models chapter of the User Guide.
 *
*/


/****************************************************************************
 Includes
 */

#include <stdlib.h>

/* Pick up memset prototype */
#include <string.h>
#if (defined(_MSC_VER))
#if (_MSC_VER>=1000)
#include <memory.h>
#endif /* (_MSC_VER>=1000) */
#endif /* (defined(_MSC_VER)) */

#include <rwcore.h>
#include "baworld.h"
#include "bapipew.h"
#include "balight.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWLIGHTGLOBAL(var)                              \
    (RWPLUGINOFFSET(rpLightGlobals,                     \
                    RwEngineInstance,                   \
                    lightModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwPluginRegistry lightTKList =
    { sizeof(RpLight),
      sizeof(RpLight),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

static RwModuleInfo lightModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                              Lighting

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

static void
LightTidyDestroyLight( void *object,
                       void * data __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("LightTidyDestroyLight"));
    RWASSERT(object);

    RpLightDestroy((RpLight *) object);

    RWRETURNVOID();
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                     Finding the lights position

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rpLightSync

 On entry   : Light
 On exit    : Light pointer on success
 */

static RwObjectHasFrame *
LightSync(RwObjectHasFrame * type)
{
    RWFUNCTION(RWSTRING("LightSync"));
    RWASSERT(type);
    RWASSERTISTYPE(type, rpLIGHT);

    /* This function doesn't do much - the work is done by the world sync
     * function attached by the plugin
     */
    RWRETURN(type);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                         Altering lights properties

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rplight
 * \ref RpLightGetRadius is used to retrieve the radius of a point or
 * spot light - the distance from the light in world units beyond which it
 * has no influence. Within the radius of the light the intensity falls
 * according to 1-(d/R), where d is the distance from the light and R is the
 * radius. Hence at d=0 the light has full intensity and at d=R the intensity
 * is zero.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns a RwReal value equal to the radius of the light
 * (in world units) if successful or zero if there is an error.
 *
 * \see RpLightSetRadius
 * \see RpLightGetColor
 * \see RpLightSetColor
 * \see RpLightGetConeAngle
 * \see RpLightSetConeAngle
 * \see RpWorldPluginAttach
 *
 */
RwReal
RpLightGetRadius(const RpLight *light)
{
    RWAPIFUNCTION(RWSTRING("RpLightGetRadius"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightGetRadiusMacro(light));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rplight
 * \ref RpLightSetRadius is used to specify the radius of a point or
 * spot light. The radius is the distance from the light in world units beyond
 * which it has no influence. Within the radius the intensity falls
 * according to 1-(d/R), where d is the distance from the light and R is the
 * radius. Hence at d=0 the light has full intensity and at d=R the intensity
 * is zero.
 *
 * The default radius is zero.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 * \param radius  A RwReal value equal to the radius.
 *
 * \return Returns pointer to the light if successful or NULL if there
 * is an error.
 *
 * \see RpLightGetRadius
 * \see RpLightSetColor
 * \see RpLightGetColor
 * \see RpLightSetConeAngle
 * \see RpLightGetConeAngle
 * \see RpWorldPluginAttach
 *
 */
RpLight            *
RpLightSetRadius(RpLight * light, RwReal radius)
{
    RwFrame *frame;

    RWAPIFUNCTION(RWSTRING("RpLightSetRadius"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);
    RWASSERT(radius >= (RwReal) (0.0));

    frame = RpLightGetFrame(light);
    light->radius = radius;
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    RWRETURN(light);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rplight
 * \ref RpLightGetColor is used to retrieve the color of the specified
 * light. Light colors are specified as real values within the range 0 to 1 in
 * each color channel.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns pointer to a RwRGBAReal value equal to the light's color
 * if successful or NULL if there is an error.
 *
 * \see RpLightSetColor
 * \see RpLightGetRadius
 * \see RpLightSetRadius
 * \see RpLightGetConeAngle
 * \see RpLightSetConeAngle
 * \see RpWorldPluginAttach
 *
 */
const RwRGBAReal *
RpLightGetColor(const RpLight *light)
{
    RWAPIFUNCTION(RWSTRING("RpLightGetColor"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightGetColorMacro(light));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rplight
 * \ref RpLightSetColor is used to specify the color of the given light.
 * Light colors are specified as real values within the range 0 to 1 in
 * each color channel.
 *
 * The default color is white (red = 1.0, green = 1.0, blue = 1.0).
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light
 * \param color  A pointer to a RwRGBAReal value equal to the light's color.
 *
 * \return Returns pointer to the light if successful or NULL if there
 * is an error.
 *
 * \see RpLightGetColor
 * \see RpLightSetRadius
 * \see RpLightGetRadius
 * \see RpLightSetConeAngle
 * \see RpLightGetConeAngle
 * \see RpWorldPluginAttach
 *
 */
RpLight            *
RpLightSetColor(RpLight * light, const RwRGBAReal * color)
{
    RWAPIFUNCTION(RWSTRING("RpLightSetColor"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);
    RWASSERT(color);

    RwRGBARealAssign(&light->color, color);

    if ((light->color.red == light->color.green) &&
        (light->color.red == light->color.blue))
    {
        /* It's a shade of grey, so set the 'grey' flag (no chroma) */
        rwObjectSetPrivateFlags(light, rpLIGHTPRIVATENOCHROMA);
    }
    else
    {
        /* It's got color, turn off the optimisation */
        rwObjectSetPrivateFlags(light, 0);
    }

    RWRETURN(light);
}

/**
 * \ingroup rplight
 * \ref RpLightGetConeAngle is used to retrieve the angle at which a
 * spot light illuminates objects (measured from the direction vector of
 * the light). The cone angle only applies to lights of type rpLIGHTTYPESPOT
 * and rpLIGHTTYPESPOTSOFT and this function will return an error if used
 * on any other type.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns a RwReal value equal to the cone angle in radians if
 * successful or zero if there is an error.
 *
 * \see RpLightSetConeAngle
 * \see RpLightGetRadius
 * \see RpLightSetRadius
 * \see RpLightGetColor
 * \see RpLightSetColor
 * \see RpWorldPluginAttach
 *
 */
RwReal
RpLightGetConeAngle(const RpLight * light)
{
    RwReal result = ((RwReal)0);

    RWAPIFUNCTION(RWSTRING("RpLightGetConeAngle"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    result = -light->minusCosAngle;
    RwIEEEACosfMacro(result, result);

    RWRETURN(result);
}

/**
 * \ingroup rplight
 * \ref RpLightSetConeAngle is used to specify the cone angle at which a
 * spot light illuminates objects (measured in radians from the direction
 * vector of the light). This function is only valid for lights of type
 * rpLIGHTTYPESPOT or rpLIGHTTYPESPOTSOFT.
 *
 * The default cone angle is zero. The maximum allowed cone angle (this
 * value being platform-dependent) is given by rpLIGHTMAXCONEANGLE.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 * \param angle  A RwReal value equal to the cone angle (in radians).
 *
 * \return Returns pointer to the light if successful or NULL if there
 * is an error.
 *
 * \see RpLightGetConeAngle
 * \see RpLightSetRadius
 * \see RpLightGetRadius
 * \see RpLightSetColor
 * \see RpLightGetColor
 * \see RpWorldPluginAttach
 *
 */
RpLight            *
RpLightSetConeAngle(RpLight * light, RwReal angle)
{
    RwReal minusCosAngle;

    RWAPIFUNCTION(RWSTRING("RpLightSetConeAngle"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    if ((angle < rpLIGHTMINCONEANGLE) ||
        (angle > rpLIGHTMAXCONEANGLE) )
    {
        /* Might this bloat use code w/ platform-predication?
         * NO: if behaviour needs to be the same on two platforms,
         * then they need to use the smallest valid cone angle for both.
         * If behaviour is different, behaviour is different :) */
        RwDebugSendMessage(rwDEBUGERROR,
                           "RpLightSetConeAngle",
                           "Light cone angle invalid (on this platform).");
        RWRETURN((RpLight *)NULL);
    }


    minusCosAngle = - (RwReal) RwCos(angle);
    light->minusCosAngle = minusCosAngle;

    RWRETURN(light);
}

/**
 * \ingroup rplight
 * \ref RpLightRegisterPlugin is used to register a plugin and
 * reserve some space within a light. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * The world plugin must be attached before using this function.
 *
 * \param size  A RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param constructCB  Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB  Copy constructor for the plugin data block.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the light
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpLightValidatePlugins
 * \see RpLightRegisterPluginStream
 * \see RpLightSetStreamAlwaysCallBack
 * \see RpLightGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpLightRegisterPlugin(RwInt32 size, RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpLightRegisterPlugin"));
    RWASSERT(!lightModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPlugin(&lightTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rplight
 * \ref RpLightRegisterPluginStream is used to associate a set of
 * binary stream functionality with a previously registered light plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param readCB  Callback used when a chunk is read that is identified as being for
 *       this plugin.
 * \param writeCB  Callback used when a chunk should be written out for this plugin.
 * \param getSizeCB  Callback used to determine the binary stream size required for this
 *       plugin (return negative to suppress chunk writing).
 *
 * \return Returns a RwInt32 value equal to the byte offset within the light
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpLightSetStreamAlwaysCallBack
 * \see RpLightValidatePlugins
 * \see RpLightRegisterPlugin
 * \see RpLightGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpLightRegisterPluginStream(RwUInt32 pluginID,
                            RwPluginDataChunkReadCallBack readCB,
                            RwPluginDataChunkWriteCallBack writeCB,
                            RwPluginDataChunkGetSizeCallBack getSizeCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpLightRegisterPluginStream"));
    RWASSERT(readCB);
    RWASSERT(writeCB);
    RWASSERT(getSizeCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStream(&lightTKList, pluginID,
                                           readCB, writeCB, getSizeCB);

    RWRETURN(plug);
}

/**
 * \ingroup rplight
 * \ref RpLightSetStreamAlwaysCallBack is used to associate a
 * binary stream functionality with a previously registered light plugin.
 * This callback is called for all plugins after stream data reading has
 * completed.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param alwaysCB  Callback used when object base and plugin data reading is complete.
 *
 * \return Returns a RwInt32 value equal to the byte offset within the light
 * of memory reserved for this plugin or a negative value if there is an error.
 *
 * \see RpLightRegisterPluginStream
 * \see RpLightValidatePlugins
 * \see RpLightRegisterPlugin
 * \see RpLightGetPluginOffset
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpLightSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                    RwPluginDataChunkAlwaysCallBack alwaysCB)
{
    RwInt32             plug;

    RWAPIFUNCTION(RWSTRING("RpLightSetStreamAlwaysCallBack"));
    RWASSERT(alwaysCB);

    /* Everything's cool, so pass it on */
    plug = rwPluginRegistryAddPluginStreamAlwaysCB(
               &lightTKList, pluginID, alwaysCB);

    RWRETURN(plug);
}

/**
 * \ingroup rplight
 * \ref RpLightGetPluginOffset is used to get the offset of a previously
 * registered light plugin.
 *
 * The world plugin must be attached before using this function.
 *
 * \param pluginID  A RwUInt32 value equal to the plugin ID.
 *
 * \return Returns the data block offset if successful or -1 if the plugin
 * is not registered.
 *
 * \see RpLightRegisterPlugin
 * \see RpLightRegisterPluginStream
 * \see RpLightSetStreamAlwaysCallBack
 * \see RpLightValidatePlugins
 * \see RpWorldPluginAttach
 *
 */
RwInt32
RpLightGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32             offset;

    RWAPIFUNCTION(RWSTRING("RpLightGetPluginOffset"));

    offset = rwPluginRegistryGetPluginOffset(&lightTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rplight
 * \ref RpLightValidatePlugins validates the plugin memory
 * allocated within the specified light. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * This function only returns a meaningful response under a debug library.
 *
 * The world plugin must be attached before using this function.
 *
 * \param  light   Pointer to the light.
 *
 * \return Returns TRUE if the light data is valid or FALSE if there is an
 * error or if the light data has become corrupt.
 *
 * \see RpLightRegisterPlugin
 * \see RpLightRegisterPluginStream
 * \see RpLightSetStreamAlwaysCallBack
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpLightValidatePlugins(const RpLight * light __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RpLightValidatePlugins"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

#ifdef RWDEBUG
    {
        RwBool              valid;

        valid = rwPluginRegistryValidateObject(&lightTKList, light);
        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

/****************************************************************************
 *
 * Binary Format Functions
 */

/**
 * \ingroup rplight
 * \ref RpLightStreamGetSize is used to determine the size in bytes
 * of the binary representation of the given light object. This is used in
 * the binary chunk header to indicate the size of the chunk. The size does
 * not include the size of the chunk header.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns a RwUInt32 value equal to the chunk size in bytes
 * if successful or zero if there is an error.
 *
 * \see RpLightStreamRead
 * \see RpLightStreamWrite
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpLightStreamGetSize(const RpLight * light)
{
    RwUInt32            size;

    RWAPIFUNCTION(RWSTRING("RpLightStreamGetSize"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    /* First the size without the extension chunks */
    size = sizeof(_rpLight) + rwCHUNKHEADERSIZE;

    /* Then add in the size of the extension chunks */
    size += rwPluginRegistryGetSize(&lightTKList, light) + rwCHUNKHEADERSIZE;

    RWRETURN(size);
}

/**
 * \ingroup rplight
 * \ref RpLightStreamWrite is used to write the specified light to a
 * binary stream. Note that the stream will have been opened prior to this
 * function call.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the light if successful or NULL if
 * there is an error.
 *
 * \see RpLightStreamRead
 * \see RpLightStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RpWorldPluginAttach
 *
 */
const RpLight      *
RpLightStreamWrite(const RpLight * light, RwStream * stream)
{
    _rpLight            lite;
    RpLightType         type;

    RWAPIFUNCTION(RWSTRING("RpLightStreamWrite"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);
    RWASSERT(stream);

    if (!RwStreamWriteChunkHeader(stream, rwID_LIGHT, RpLightStreamGetSize(light)))
    {
        RWRETURN((const RpLight *)NULL);
    }

    if (!RwStreamWriteChunkHeader(stream, rwID_STRUCT, sizeof(_rpLight)))
    {
        RWRETURN((const RpLight *)NULL);
    }

    /* Set up the structure
     * - don't forget to make it the right format */

    lite.radius = light->radius;
    lite.red = light->color.red;
    lite.green = light->color.green;
    lite.blue = light->color.blue;
    lite.minusCosAngle = light->minusCosAngle;
    type = RpLightGetType(light);
    lite.typeAndFlags = (((RwUInt32) type) << 16) | RpLightGetFlags(light);

    (void)RwMemRealToFloat32(&lite.radius, sizeof(lite.radius));
    (void)RwMemRealToFloat32(&lite.red, sizeof(lite.red));
    (void)RwMemRealToFloat32(&lite.green, sizeof(lite.green));
    (void)RwMemRealToFloat32(&lite.blue, sizeof(lite.blue));
    (void)RwMemRealToFloat32(&lite.minusCosAngle, sizeof(lite.minusCosAngle));
    (void)RwMemLittleEndian32(&lite, sizeof(lite));

    if (!RwStreamWrite(stream, &lite, sizeof(lite)))
    {
        RWRETURN((const RpLight *)NULL);
    }

    /* Now the extension chunks */
    if (!rwPluginRegistryWriteDataChunks(&lightTKList, stream, light))
    {
        /* Failed to write extension data */
        RWRETURN((const RpLight *)NULL);
    }

    /* Done */
    RWRETURN(light);
}

/**
 * \ingroup rplight
 * \ref RpLightStreamRead is used to read a light object from the
 * specified binary stream. Note that prior to this function call a binary
 * light chunk must be found in the stream using the
 * \ref RwStreamFindChunk API function.
 *
 * The world plugin must be attached before using this function.
 *
 * The sequence to locate and read a light from a binary stream is
 * as follows:
   \code
   RwStream *stream;
   RpLight *NewLight;

   stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, "mybinary.xxx");
   if (stream)
   {
       if( RwStreamFindChunk(stream, rwID_LIGHT, NULL, NULL) )
       {
           NewLight = RpLightStreamRead(stream);
       }

       RwStreamClose(stream, NULL);
   }
  \endcode
 *
 * \param stream  Pointer to the stream.
 *
 * \return Returns pointer to the light if successful or NULL if there is an error.
 *
 * \see RpLightStreamWrite
 * \see RpLightStreamGetSize
 * \see RwStreamOpen
 * \see RwStreamClose
 * \see RwStreamFindChunk
 * \see RpWorldPluginAttach
 */
RpLight            *
RpLightStreamRead(RwStream * stream)
{
    RwUInt32            size;
    RwUInt32            version;

    RWAPIFUNCTION(RWSTRING("RpLightStreamRead"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(stream);

    if (!RwStreamFindChunk(stream, rwID_STRUCT, &size, &version))
    {
        RWRETURN((RpLight *)NULL);
    }

    if ((version >= rwLIBRARYBASEVERSION) && (version <= rwLIBRARYCURRENTVERSION))
    {
        RpLight            *light;
        _rpLight            lite;

        RWASSERT(size <= sizeof(lite));
        memset(&lite, 0, sizeof(lite));
        if (RwStreamRead(stream, &lite, size) != size)
        {
            RWRETURN((RpLight *)NULL);
        }

        (void)RwMemNative32(&lite, sizeof(lite));

        light = RpLightCreate((RpLightType)((lite.typeAndFlags >> 16) & 0xFF));
        if (!light)
        {
            RWRETURN((RpLight *)NULL);
        }

        (void)RwMemFloat32ToReal(&lite.radius, sizeof(lite.radius));
        (void)RwMemFloat32ToReal(&lite.red, sizeof(lite.red));
        (void)RwMemFloat32ToReal(&lite.green, sizeof(lite.green));
        (void)RwMemFloat32ToReal(&lite.blue, sizeof(lite.blue));
        (void)RwMemFloat32ToReal(&lite.minusCosAngle, sizeof(lite.minusCosAngle));

        /* For versioning, see rwLIBRARYCURRENTVERSION in
         * rwsdk/src/batypes.h
         */

        light->radius = lite.radius;
        light->color.red = lite.red;
        light->color.green = lite.green;
        light->color.blue = lite.blue;

        if (version < 0x30300)
        {
            /*
             * This was stored with tanAngle rather than minusCosAngle
             */
            RWMINUSCOSFROMTAN(light->minusCosAngle, lite.minusCosAngle);
        }
        else
        {
            light->minusCosAngle = lite.minusCosAngle;
        }

        if ((light->color.red == light->color.green) &&
            (light->color.red == light->color.blue))
        {
            /* Its a shade of grey,
             * so set the 'grey' flag (no chroma) */
            rwObjectSetPrivateFlags(light, rpLIGHTPRIVATENOCHROMA);
        }
        else
        {
            /* It's got color, turn off the optimisation */
            rwObjectSetPrivateFlags(light, 0);
        }

        RpLightSetFlags(light, lite.typeAndFlags & 0xFF);

        /* LIGHT EXTENSION DATA */
        if (!rwPluginRegistryReadDataChunks(&lightTKList, stream, light))
        {
            RWRETURN((RpLight *)NULL);
        }

        /* AOK */
        RWRETURN(light);
    }

    RWERROR((E_RW_BADVERSION));
    RWRETURN((RpLight *)NULL);
}

/******************************************************************
 * ingroup rplight
 * ref _rpLightChunkInfoRead _rpLightChunkInfoRead extracts
 * Chunk Info data from a RenderWare stream. The data is converted
 * from its original format and inserted into an RpLightChunkInfo
 * structure. A pointer to this structure is returned on success.
 *
 * param stream  Pointer to the stream from which to read the chunk info.
 * param lightChunkInfo  Pointer to an RpLightChunkInfo object.
 * param bytesRead  Pointer to a RwInt32 which will be used to return the
 * number of bytes read.
 *
 * return Returns pointer to RpLightChunkInfo if successful or NULL
 * if there is an error
 *
 * see RpLightStreamRead
 *
 */
RpLightChunkInfo   *
_rpLightChunkInfoRead(RwStream * stream,
                      RpLightChunkInfo * lightChunkInfo,
                      RwInt32 * bytesRead)
{
    RwUInt32            size, readSize;

    RWFUNCTION(RWSTRING("_rpLightChunkInfoRead"));
    RWASSERT(stream);
    RWASSERT(lightChunkInfo);

    if (!RwStreamFindChunk(stream, (RwUInt32)rwID_STRUCT,
                           &size, (RwUInt32 *)NULL))
    {
        RWRETURN((RpLightChunkInfo *)NULL);
    }

    RWASSERT(size <= sizeof(RpLightChunkInfo));
    readSize = sizeof(RpLightChunkInfo);
    memset(lightChunkInfo, 0, readSize);
    if (RwStreamRead(stream, lightChunkInfo, readSize) != readSize)
    {
        RWRETURN((RpLightChunkInfo *)NULL);
    }

    *bytesRead = size + (sizeof(RwInt32) * 3);
    /* move on to known place */
    RwStreamSkip(stream, size - readSize);

    (void)RwMemNative32(lightChunkInfo, size);

    (void)RwMemFloat32ToReal(&lightChunkInfo->radius, sizeof(lightChunkInfo->radius));
    (void)RwMemFloat32ToReal(&lightChunkInfo->red, sizeof(lightChunkInfo->red));
    (void)RwMemFloat32ToReal(&lightChunkInfo->green, sizeof(lightChunkInfo->green));
    (void)RwMemFloat32ToReal(&lightChunkInfo->blue, sizeof(lightChunkInfo->blue));
    (void)RwMemFloat32ToReal(&lightChunkInfo->minusCosAngle, sizeof(lightChunkInfo->minusCosAngle));

    RWRETURN(lightChunkInfo);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rplight
 * \ref RpLightSetFrame is used to attach the specified light to the
 * given frame so that it can be positioned and oriented, as appropriate,
 * within a world.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 * \param frame  Pointer to the frame.
 *
 * \return Returns pointer to the light if successful or NULL if
 * there is an error.
 *
 * \see RpLightGetFrame
 * \see RwFrameCreate
 * \see RpWorldAddLight
 * \see RpWorldRemoveLight
 * \see RpWorldPluginAttach
 *
 */
RpLight *
RpLightSetFrame(RpLight *light, RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RpLightSetFrame"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightSetFrameMacro(light, frame));
}

/**
 * \ingroup rplight
 * \ref RpLightGetFrame is used to retrieve the frame the specified
 * light is attached to, if any.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns a pointer to the frame if successful or NULL if there
 * is an error.
 *
 * \see RpLightSetFrame
 * \see RpLightCreate
 * \see RpWorldPluginAttach
 *
 */
RwFrame *
RpLightGetFrame(const RpLight *light)
{
    RWAPIFUNCTION(RWSTRING("RpLightGetFrame"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightGetFrameMacro(light));
}

/**
 * \ingroup rplight
 * \ref RpLightGetType is used to determine the type of a light object,
 * if any.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return \ref RpLightType or rpNALIGHTTYPE
 * if the light is none of the supported types.
 *
 * \see RpLightCreate
 * \see RpWorldPluginAttach
 *
 */
RpLightType
RpLightGetType(const RpLight *light)
{
    RWAPIFUNCTION(RWSTRING("RpLightGetType"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightGetTypeMacro(light));
}

/**
 * \ingroup rplight
 * \ref RpLightSetFlags is used to specify what a particular light object
 * illuminates.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 * \param flags  A 32-bit unsigned integer bitfield
 * that specifies one (or both OR'd together) of the following values:
 *  rpLIGHTLIGHTATOMICS  Light illuminates the atomics.
 *  rpLIGHTLIGHTWORLD    Light illuminates the world (static geometry).
 *
 * \return Returns pointer to the light if successful or NULL if
 * there is an error.
 *
 * \see RpLightGetFlags
 * \see RpLightCreate
 * \see RpWorldPluginAttach
 *
 */
RpLight *
RpLightSetFlags(RpLight *light, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RpLightSetFlags"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightSetFlagsMacro(light, flags));
}

/**
 * \ingroup rplight
 * \ref RpLightGetFlags is used to retrieve the flags associated with
 * a light object.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light.
 *
 * \return Returns a 32-bit unsigned integer whose value is one of the following
 * constants (if both flags are set the return value is the bit-wise OR of the
 * two constants):
 *
 * \li rpLIGHTLIGHTATOMICS     The light illuminates atomics.
 * \li rpLIGHTLIGHTWORLD       The light illuminates static world geometry.
 *
 * \see RpLightSetFlags
 * \see RpWorldPluginAttach
 *
 */
RwUInt32
RpLightGetFlags(const RpLight *light)
{
    RWAPIFUNCTION(RWSTRING("RpLightGetFlags"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    RWRETURN(RpLightGetFlagsMacro(light));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                                Lights

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rplight
 * \ref RpLightDestroy is used to destroy the specified dynamic light.
 * The light must be removed from the worlds it belongs to (if any) before
 * destruction using \ref RpWorldRemoveLight. Any frame attached to the
 * light should be removed from the light using \ref RpLightSetFrame
 * passing NULL as the new frame.
 *
 * \note This does not destroy the frame. To destroy the frame, get the
 * pointer using \ref RpLightGetFrame, then remove the frame from the light,
 * then destroy it with \ref RwFrameDestroy.
 *
 * The following is an example, from the light example file
 * \c lights.c in \c LightsDestroy() (line 335). SpotSoftLight is a pointer
 * to an RpLight.
 * \code
    if( SpotSoftLight )
    {
        world = RpLightGetWorld(SpotSoftLight);
        if( world )
        {
            RpWorldRemoveLight(world, SpotSoftLight);
        }

        frame = RpLightGetFrame(SpotSoftLight);
        RpLightSetFrame(SpotSoftLight, NULL);
        RwFrameDestroy(frame);

        RpLightDestroy(SpotSoftLight);
    }
  \endcode
 *
 * The world plugin must be attached before using this function.
 *
 * \param light  Pointer to the light
 *
 * \return Returns TRUE if successful or FALSE if there is an error
 *
 * \see RpWorldRemoveLight
 * \see RpLightSetFrame
 * \see RwFrameDestroy
 * \see RpLightCreate
 * \see RpWorldPluginAttach
 *
 */
RwBool
RpLightDestroy(RpLight * light)
{
    RWAPIFUNCTION(RWSTRING("RpLightDestroy"));
    RWASSERT(lightModule.numInstances);
    RWASSERT(light);
    RWASSERTISTYPE(light, rpLIGHT);

    /* De-Init toolit memory */
    rwPluginRegistryDeInitObject(&lightTKList, light);

    rwObjectHasFrameReleaseFrame(light);

    RwFreeListFree(RWLIGHTGLOBAL(lightFreeList), light);

    RWRETURN(TRUE);
}

/**
 * \ingroup rplight
 * \ref RpLightCreate is used to create a new light of the type specified.
 * The light needs to be linked to a frame and added to a world before it can
 * be included in any rendering. The associated frame enables the light to be
 * positioned and oriented (i.e. positioned) within the world as required.
 *
 * When a frame is defined directional lighting (rpLIGHTDIRECTIONAL, rpLIGHTSPOT
 * and rpLIGHTSPOTSOFT) is oriented such that it illuminates in the direction
 * of the frame's look-at vector, while positionable lighting (rpLIGHTPOINT,
 * rpLIGHTSPOT and rpLIGHTSPOTSOFT) is located at the origin. Transformations
 * applied to the frame can then be used to move and re-orient the light as
 * required.
 *
 * The following defaults are defined:
 * \li  For lights with intensity fall-off
 *      with distance (rpLIGHTPOINT, rpLIGHTSPOT and rpLIGHTSPOTSOFT) the radius
 *      is zero,
 * \li  For spot lights (rpLIGHTSPOT and rpLIGHTSPOTSOFT) the cone
 *      angle is zero,
 * \li  For all light types the initial color is white
 *      (1.0, 1.0, 1.0),
 * \li  All light types are set up to illuminate both atomics
 *      and static world geometry.
 *
 * The world plugin must be attached before using this function.
 *
 * \param type  A RwInt32 value indicating the type of light to be created.
 *
 * Supported values are
 * \li     rpLIGHTDIRECTIONAL   Directional light.
 * \li     rpLIGHTAMBIENT       Ambient light.
 * \li     rpLIGHTPOINT         Point light, linear intensity fall-off
 *                              with distance from the source.
 * \li    rpLIGHTSPOT           Spot light, linear intensity fall-off
 *                              with distance from the source, no fall-off
 *                              across the cone (hard-edged).
 * \li    rpLIGHTSPOTSOFT       Spot light, linear intensity fall-off
 *                              with distance from the source,
 *                              quadratic fall-off across the cone (soft-edged).
 *
 * \return Returns pointer to the new light if successful or NULL if
 * there is an error.
 *
 * \see RpLightDestroy
 * \see RpLightSetFrame
 * \see RpWorldAddLight
 * \see RpWorldRemoveLight
 * \see RpLightSetColor
 * \see RpLightSetRadius
 * \see RpLightSetConeAngle
 * \see RpWorldPluginAttach
 *
 */
RpLight            *
RpLightCreate(RwInt32 type)
{
    RpLight            *light;

    RWAPIFUNCTION(RWSTRING("RpLightCreate"));
    RWASSERT(lightModule.numInstances);

    RWASSERT(RWLIGHTGLOBAL(lightFreeList));
    light = (RpLight *) RwFreeListAlloc(RWLIGHTGLOBAL(lightFreeList),
                                     rwMEMHINTDUR_EVENT | rwID_LIGHT);
    if (!light)
    {
        RWRETURN((RpLight *)NULL);
    }
    rwObjectHasFrameInitialize(light, rpLIGHT, type, LightSync);

    light->radius = (RwReal) (0);
    light->minusCosAngle = (RwReal) (0.0);

    light->color.red = (RwReal) (1.0);
    light->color.green = (RwReal) (1.0);
    light->color.blue = (RwReal) (1.0);
    light->color.alpha = (RwReal) (1.0);

    /* Its a shade of grey, so set the 'grey' flag (no chroma) */
    rwObjectSetPrivateFlags(light, rpLIGHTPRIVATENOCHROMA);

    rwLinkListInitialize(&light->WorldSectorsInLight);
    rwLLLinkInitialize(&light->inWorld);

    light->lightFrame = RWSRCGLOBAL(lightFrame) - 1;

    /* Set default to light everything */
    RpLightSetFlags(light, rpLIGHTLIGHTWORLD | rpLIGHTLIGHTATOMICS);

    /* Install an 'apply youself' function - the plugin can get in
     * and detect unknown types later.  First a default, then allow
     * for device specific one.
     */
#if (0 && defined(RWAPPLYUNKNOWNLIGHT))
    light->applyLight = _rwApplyUnknownLight;
    rpGetWorldPipeInterface()->pipeSetupLightApply(light);
#endif /* (0 && defined(RWAPPLYUNKNOWNLIGHT)) */

    /* Init toolit memory */
    rwPluginRegistryInitObject(&lightTKList, light);

    /* Done */
    RWRETURN(light);
}

/****************************************************************************
 _rpLightClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rpLightClose(void *instance,
              RwInt32 offset __RWUNUSED__,
              RwInt32 size __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("_rpLightClose"));

    /* Destroy any lights left in the freelist */
    RwFreeListForAllUsed(RWLIGHTGLOBAL(lightFreeList),
                         LightTidyDestroyLight, NULL);

    /* Then destroy the free list itself */
    RwFreeListDestroy(RWLIGHTGLOBAL(lightFreeList));
    RWLIGHTGLOBAL(lightFreeList) = (RwFreeList *)NULL;

    /* One less module instance */
    lightModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

static RwInt32 _rpLightFreeListBlockSize = 32,
               _rpLightFreeListPreallocBlocks = 1;
static RwFreeList _rpLightFreeList;

/**
 * \ingroup rplight
 * \ref RpLightSetFreeListCreateParams allows the developer to specify
 * how many \ref RpLight s to preallocate space for.
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
RpLightSetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc)
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RpLightSetFreeListCreateParams"));
#endif
    _rpLightFreeListBlockSize = blockSize;
    _rpLightFreeListPreallocBlocks = numBlocksToPrealloc;
#if 0
    RWRETURNVOID();
#endif
}

/****************************************************************************
 _rpLightOpen

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rpLightOpen( void *instance,
              RwInt32 offset,
              RwInt32 size __RWUNUSED__ )
{
    RWFUNCTION(RWSTRING("_rpLightOpen"));

    /* Grab the global data offset (same for all instances) */
    lightModule.globalsOffset = offset;

    /* And get on with the setup */
    RWLIGHTGLOBAL(lightFreeList) = RwFreeListCreateAndPreallocateSpace(
        lightTKList.sizeOfStruct, _rpLightFreeListBlockSize,
        rwMATRIXALIGNMENT,
        _rpLightFreeListPreallocBlocks, &_rpLightFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_LIGHT);
    if (RWLIGHTGLOBAL(lightFreeList))
    {
        /* One more module instance */
        lightModule.numInstances++;

        /* Success */
        RWRETURN(instance);
    }

    /* Failure */
    RWRETURN(NULL);
}

#if (0 && defined(RWAPPLYUNKNOWNLIGHT))

/***************************************************************************
 _rwApplyUnknownLight

 On entry   : Instanced data
            : Light
            : Optional inverse object matrix (to transform light to object space)
            : Inverse scale of object
            : Surface properties of the light
 On exit    :
 */

static void
ApplyUnknownLight(RwResEntry * repEntry __RWUNUSED__,
                  const void *voidLight __RWUNUSED__,
                  const RwMatrix * inverseMat __RWUNUSED__,
                  RwReal invScale __RWUNUSED__,
                  const RwSurfaceProperties * surfaceProps __RWUNUSED__)
{
    RWFUNCTION(RWSTRING("ApplyUnknownLight"));

    RWRETURNVOID();
}
#endif /* (0 && defined(RWAPPLYUNKNOWNLIGHT)) */
