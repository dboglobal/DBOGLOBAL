/*
 *
 * Color space functionality.
 * Functions for manipulating RGB color space.
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/**
 * \ingroup rwrgba
 * \page rwrgbaoverview RwRGBA Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection rgbaoverview Overview
 *
 * This object represents color and alpha components.
 *
 * The data is usually stored combined into a single 32-bit integer, with 8 bits per component,
 * although this should not be assumed.
 *
 * Functions are provided to convert to and from the RwRGBAReal format, which uses individual RwReals
 * to store each component. (Other RenderWare objects usually use the RwRGBA form, but a real
 * number variant is useful for some color processing tasks.)
 *
 * Further functionality is provided for tasks like gamma correction, storing and retrieving RGBA data
 * directly from a pixel.
 *
*/


/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "balibtyp.h"
#include "osintf.h"
#include "bamath.h"

#include "bacolor.h"

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

/****************************************************************************
 Globals (across program)
 */

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo colorModule;

/****************************************************************************
 _rwColorOpen

 On entry   : Instance, offset, size
 On exit    : Instance on success
 */

void           *
_rwColorOpen(void *instance,
             RwInt32 __RWUNUSED__ offset ,
             RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwColorOpen"));
    RWASSERT(instance);

    /* One more instance */
    colorModule.numInstances++;

    /* All done */
    RWRETURN(instance);
}

/****************************************************************************
 _rwColorClose

 On entry   : Instance, offset, size
 On exit    : Instance on success
 */

void           *
_rwColorClose(void *instance,
              RwInt32 __RWUNUSED__ offset,
              RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwColorClose"));
    RWASSERT(instance);

    /* One less instance */
    colorModule.numInstances--;

    /* All done */
    RWRETURN(instance);
}

#if ( ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) )

/**
 * \ingroup rwrgba
 * \ref RwRGBARealAdd is used to add together the specified real colors.
 * The result of the addition is placed in a third real color.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param result  A pointer to the real color that will receive the result of
 * the addition
 * \param source1  A pointer to the first real color
 * \param source2  A pointer to the second real color.
 *
 * \return None.
 *
 * \see RwRGBARealSub
 * \see RwRGBARealScale
 *
 */
void
RwRGBARealAdd(RwRGBAReal *result,
              const RwRGBAReal *source1,
              const RwRGBAReal *source2)
{
    RWAPIFUNCTION(RWSTRING("RwRGBARealAdd"));
    RWASSERT(colorModule.numInstances);
    RWASSERT(result);
    RWASSERT(source1);
    RWASSERT(source2);

    RwRGBARealAddMacro(result, source1, source2);

    RWRETURNVOID();
}

/**
 * \ingroup rwrgba
 * \ref RwRGBARealSub is used to subtract the specified real colors. The
 * second color is subtracted from the first. The result of the subtraction
 * is placed in a third real color.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param result  A pointer to the real color that will receive the result of the
 * subtraction
 * \param source1  A pointer to the first real color
 * \param source2  A pointer to the second real color.
 *
 * \return None.
 *
 * \see RwRGBARealAdd
 * \see RwRGBARealScale
 *
 */
void
RwRGBARealSub(RwRGBAReal *result,
              const RwRGBAReal *source1,
              const RwRGBAReal *source2)
{
    RWAPIFUNCTION(RWSTRING("RwRGBARealSub"));
    RWASSERT(colorModule.numInstances);
    RWASSERT(result);
    RWASSERT(source1);
    RWASSERT(source2);

    RwRGBARealSubMacro(result, source1, source2);

    RWRETURNVOID();
}

/**
 * \ingroup rwrgba
 * \ref RwRGBARealScale is used to multiply the specified real color
 * by the given scalar. The result is placed in a second real color.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param result  A pointer to the real color that will receive the result
 * \param source  A pointer to the real color that will be scaled
 * \param scalar  An \ref RwReal value equal to the scale factor.
 *
 * \return None.
 *
 * \see RwRGBARealAdd
 * \see RwRGBARealSub
 *
 */
void
RwRGBARealScale(RwRGBAReal *result, const RwRGBAReal *source, RwReal scalar)
{
    RWAPIFUNCTION(RWSTRING("RwRGBARealScale"));
    RWASSERT(colorModule.numInstances);
    RWASSERT(result);
    RWASSERT(source);

    RwRGBARealScaleMacro(result, source, scalar);

    RWRETURNVOID();
}

/**
 * \ingroup rwrgba
 * \ref RwRGBAFromRwRGBAReal is used to convert the specified real color
 * to an RwRGBA. RwRGBA is an integer representation of the color.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param result  A pointer to the \ref RwRGBA color which will receive the result
 * \param source  A pointer to the real color that will be converted.
 *
 * \return None.
 *
 * \see RwRGBARealFromRwRGBA
 * \see RwRGBARealScale
 * \see RwRGBARealAdd
 * \see RwRGBARealSub
 */
void
RwRGBAFromRwRGBAReal(RwRGBA *result, const RwRGBAReal *source)
{
    RWAPIFUNCTION(RWSTRING("RwRGBAFromRwRGBAReal"));
    RWASSERT(result);
    RWASSERT(source);

    RwRGBAFromRwRGBARealMacro(result, source);

    RWRETURNVOID();
}

/**
 * \ingroup rwrgba
 * \ref RwRGBARealFromRwRGBA is used to convert the specified RwRGBA
 * color to a real color. RwRGBA is an integer representation of the color.
 *
 * \note Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param result  A pointer to the real color that will receive the result
 * \param source  A pointer to the \ref RwRGBA color that will be converted.
 *
 * \return None.
 *
 * \see RwRGBAFromRwRGBAReal
 * \see RwRGBARealScale
 * \see RwRGBARealAdd
 * \see RwRGBARealSub
 */
void
RwRGBARealFromRwRGBA(RwRGBAReal *result, RwRGBA *source)
{
    RWAPIFUNCTION(RWSTRING("RwRGBARealFromRwRGBA"));
    RWASSERT(result);
    RWASSERT(source);

    RwRGBARealFromRwRGBAMacro(result, source);

    RWRETURNVOID();
}

#endif /* ( ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) ) */

