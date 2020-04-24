/*****************************************************************************
 *
 * File :     IOCoercion.h
 *
 * Abstract : Macros to provide 'format coercion' for ostream insertions. Use
 *            them 'in place' e.g...
 *
 *               cout << RWS_HEX(42) << endl;           
 *
 *****************************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd. or
 * Canon Inc. to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Canon Inc.
 *
 *****************************************************************************/

#ifndef __IOCOERCION_H__
#define __IOCOERCION_H__

/**
*
*  \ingroup DebugMacros
*
*  Outputs 'a' in hexadecimal format, with a leading '0x' (NB: uses a homegrown
*  '0x' because pointer output neglects 'showbase'). Assumes 'a' is an integer
*  type.
*
*/

#define RWS_HEX(a) "0x" << std::hex << (a) << std::dec

/**
*
*  \ingroup DebugMacros
*  Outputs 'a' in decimal format. Assumes 'a' is an integer type.
*
*/

#define RWS_DEC(a) std::dec << (a)

/**
*
*  \ingroup DebugMacros
*
*  Outputs 'a' as a fixed precision decimal, to 'b' decimal places. Assumes 'a'
*  is a floating-point type. Client needs to #include <iomanip> to use this one.
*
*/

#define RWS_FIX(a, b) std::fixed << std::setprecision(b) << (a) << std::scientific

/**
*
*  \ingroup DebugMacros
*
*  Outputs 'a' lexicographically, (i.e. as 'true' or 'false', instead of as '0' or '1')
*  Assumes 'a' is a 'bool'.
*/

#define RWS_LEX(a) std::boolalpha << (a) << std::noboolalpha

/**
*
*  \ingroup DebugMacros
*
*  Checks strings for null, if they are null returns "null" otherwise returns the string
*  useful in stream expressions where the string may be invalid.
*
*/
#define RWS_VALIDSTRING(a) (a?a:"null")

#endif