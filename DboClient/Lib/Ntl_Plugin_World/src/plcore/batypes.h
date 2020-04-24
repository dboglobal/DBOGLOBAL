/***************************************************************************
 *                                                                         *
 * Module  : batypes.h                                                     *
 *                                                                         *
 * Purpose : Types                                                         *
 *                                                                         *
 **************************************************************************/

#ifndef RWTYPES_H
#define RWTYPES_H


/****************************************************************************
 Includes
 */

#include <stdlib.h>
#include <stddef.h>
#include "ostypes.h"

/* RWPUBLIC */

#include "rwversion.h"

/* IMPORTANT:
 * The following Doxygen comment MUST be copied into RwCore.h,
 * so don't move it from here. */

#if (!defined(RWFORCEENUMSIZEINT))
#define RWFORCEENUMSIZEINT ((RwInt32)((~((RwUInt32)0))>>1))
#endif /* (!defined(RWFORCEENUMSIZEINT)) */

/*
 * See
 * http://www.eskimo.com/~scs/C-faq/q11.17.html
 */

#define RW_STRINGIFY(X) #X
#define RW_STRINGIFY_EXPANDED(X) RW_STRINGIFY(X)

/****************************************************************************
 Attributes
 */

#if (defined(__GNUC__))

/* See http://www.gnu.org/software/gcc/onlinedocs/gcc_4.html#SEC91 */

#if (!(defined(__cplusplus) || defined(__MWERKS__) || defined(__RWUNUSED__)))
#define __RWUNUSED__ __attribute__ ((unused))
#endif /* (!(defined(__cplusplus) || defined(__MWERKS__) || defined(__RWUNUSED__))) */

#if (!(defined(__RWUNUSEDRELEASE__) || defined(RWVALIDATEPARAM)))
#if (!( defined(__cplusplus) || defined(__MWERKS__) || defined(RWDEBUG)))
#define __RWUNUSEDRELEASE__ __attribute__ ((unused))
#endif /* (!(defined(__cplusplus) || defined(__MWERKS__) || defined(RWDEBUG))) */
#endif /* (!(defined(__RWUNUSEDRELEASE__) || defined(RWVALIDATEPARAM))) */

#if (!defined(__RWFORMAT__))
#define __RWFORMAT__(_archetype, _string_index, _first_to_check)        \
    __attribute__ ((format (_archetype, _string_index, _first_to_check)))
#endif /* (!defined(__RWFORMAT__)) */

#endif /* (defined(__GNUC__)) */

#if (!defined(__RWUNUSED__))
#define __RWUNUSED__           /* No op */
#endif /* (!defined(__RWUNUSED__)) */

#if (!defined(__RWUNUSEDRELEASE__))
#define __RWUNUSEDRELEASE__           /* No op */
#endif /* (!defined(__RWUNUSEDRELEASE__)) */

#if (!defined(__RWFORMAT__))
#define __RWFORMAT__(_archetype, _string_index, _first_to_check)  /* No op */
#endif /* (!defined(__RWFORMAT__)) */

/****************************************************************************
 Calling conventions
 */

#if (defined(WIN32))
#define RWASMCALL   __cdecl
#define RWASMAPI(TYPE) TYPE RWASMCALL
#endif /* (defined(WIN32)) */

#if (!defined(RWASMCALL))
#define RWASMCALL              /* No op */
#endif /* (!defined(RWASMCALL)) */

#if (!defined(RWASMAPI))
#define RWASMAPI(TYPE) TYPE
#endif /* (!defined(RWASMAPI)) */

/* RWPUBLICEND */

/****************************************************************************
 Switches
 */

/* RWPUBLIC */

/* Maximum number of nested contexts */
#define rwMAXPIPECONTEXT 10

/* RWPUBLICEND */

/***************************************************************************
 Includes
 */

/* RWPUBLIC */

/****************************************************************************
 Macro wrappers. These are needed everywhere.
 */

#ifndef MACRO_START
#define MACRO_START do
#endif /* MACRO_START */

#ifndef MACRO_STOP
#define MACRO_STOP while(0)
#endif /* MACRO_STOP */

/****************************************************************************
 Types needed everywhere
 */

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE !FALSE

/****************************************************************************
 MS VC/C++ Specific
 */

#if (defined(_MSC_VER))
#if (_MSC_VER>=1000)


/*
 * Check for correct compiler version
 */
#define RW_MSC_VER 1200

#if (0 && !defined(RW_NO_COMPILER_CHECK))
#if (_MSC_VER != RW_MSC_VER )
# pragma message (__FILE__ "(" RW_STRINGIFY_EXPANDED(__LINE__) "):" "\n  This compiler is a different version (" RW_STRINGIFY_EXPANDED(_MSC_VER) ")\n  to the compiler used to build the RenderWare product libraries (" RW_STRINGIFY_EXPANDED(RW_MSC_VER) ") \n  To turn off this warning please define RW_NO_COMPILER_CHECK " )
# pragma comment ( user, "comment:" __FILE__ "(" RW_STRINGIFY_EXPANDED(__LINE__) "):" "\n  This compiler is a different version (" RW_STRINGIFY_EXPANDED(_MSC_VER) ")\n  to the compiler used to build the RenderWare product libraries (" RW_STRINGIFY_EXPANDED(RW_MSC_VER) ") \n  To turn off this warning please define RW_NO_COMPILER_CHECK " )
#endif /* (_MSC_VER != RW_MSC_VER ) */
#endif /* (0 && !defined(RW_NO_COMPILER_CHECK)) */

/*
 * Output some compiler messages and object file comments
 */

#pragma comment ( compiler )

#pragma comment ( user, "comment:" __DATE__" "  __TIME__ " - " __FILE__ "(" RW_STRINGIFY_EXPANDED(__LINE__) ")")
#pragma comment ( user, "comment:" " _MSC_VER==" RW_STRINGIFY_EXPANDED(_MSC_VER) "; _M_IX86==" RW_STRINGIFY_EXPANDED(_M_IX86))
#if (defined(rwLIBRARYCURRENTVERSION))
#pragma comment ( user, "comment:" "rwLIBRARYCURRENTVERSION:" RW_STRINGIFY_EXPANDED(rwLIBRARYCURRENTVERSION) )
#endif /* (defined(rwLIBRARYCURRENTVERSION)) */

#if (defined(RWDEBUG) && defined(RWVERBOSE))

#if (defined(RWMEMDEBUG) && !defined(_CRTDBG_MAP_ALLOC))
#define _CRTDBG_MAP_ALLOC
#endif /* defined(RWMEMDEBUG) && !defined(_CRTDBG_MAP_ALLOC)) */
#include <crtdbg.h>

#pragma message (__DATE__" "  __TIME__ " - " __FILE__ "(" RW_STRINGIFY_EXPANDED(__LINE__) ")" )
#pragma message ("_MSC_VER==" RW_STRINGIFY_EXPANDED(_MSC_VER) "; _M_IX86==" RW_STRINGIFY_EXPANDED(_M_IX86))

#if (defined(rwLIBRARYCURRENTVERSION))
#pragma message ( "rwLIBRARYCURRENTVERSION:" RW_STRINGIFY_EXPANDED(rwLIBRARYCURRENTVERSION) )
#endif /* (defined(rwLIBRARYCURRENTVERSION)) */

#endif /* (defined(RWDEBUG) && defined(RWVERBOSE) ) */

#endif /* (_MSC_VER>=1000) */
#endif /* (defined(_MSC_VER)) */

/*******************/
/* Primitive types */
/*******************/

/* String construction stuff (gets us UNICODE support) */
#ifdef RWUNICODE
#define _RWSTRING(x) L ## x
#else /* RWUNICODE */
#define _RWSTRING(x) x
#endif /* RWUNICODE */
#define RWSTRING(x) _RWSTRING(x)

/* NB volatile keyword required for VC5.0 to ensure a reload - AMB */
typedef union RwSplitBits RwSplitBits;

#if (!defined(DOXYGEN))
union RwSplitBits
{
    RwReal nReal;
    volatile RwInt32 nInt;
    volatile RwUInt32 nUInt;
};

typedef struct RwSplitFixed RwSplitFixed;

#ifdef rwBIGENDIAN
struct RwSplitFixed
{
    RwInt16 integral;
    RwUInt16 fractional;
};

#else /* rwBIGENDIAN */
#ifdef rwLITTLEENDIAN
struct RwSplitFixed
{
    RwUInt16 fractional;
    RwInt16 integral;
};

#else /* rwLITTLEENDIAN */
#error "ENDIAN-ness undefined!"
#endif /* rwLITTLEENDIAN */
#endif /* rwBIGENDIAN */

typedef union RwUnionReal RwUnionReal;

union RwUnionReal /* MSB is sign bit in any circumstance */
{
    RwReal real; /* 4 bytes interpreted as RwReal */
    float floating; /* 4 bytes interpreted as float */
    RwFixed fixed; /* 4 bytes interpreted as 16:16 fixed */
    RwSplitFixed splitfixed; /* 4 bytes interpreted as 16:16 fixed */
};
#endif /* (!defined(DOXYGEN)) */

/*****************/

/* Complex types */

/*****************/

typedef struct RwV2d RwV2d;
/**
 * \ingroup rwv2d
 * \struct RwV2d
 * This type represents points in a 2D space, such as device
 * space, specified by the (x, y) coordinates of the point.
 */
struct RwV2d
{
    RwReal x;   /**< X value*/
    RwReal y;   /**< Y value */
};

typedef struct RwV3d RwV3d;
/**
 * \ingroup rwv3d
 * \struct RwV3d
 *  This type represents 3D points and vectors specified by
 * the (x, y, z) coordinates of a 3D point or the (x, y, z) components of a
 * 3D vector.
 */
struct RwV3d
{
    RwReal x;   /**< X value */
    RwReal y;   /**< Y value */
    RwReal z;   /**< Z value */
};

#define RWV4DALIGNMENT(_v4d) \
   (! (((rwV4DALIGNMENT)-1) & ((RwUInt32)(_v4d))))


#if (!defined(DOXYGEN))

struct RwV4dTag
{
    RwReal x;   /**< X value */
    RwReal y;   /**< Y value */
    RwReal z;   /**< Z value */
    RwReal w;   /**< W value */
};

/*
 * RwV4d & RwV4dTag must be different otherwise the alignment
 * directive is ignored when compiling under C++ on Visual C
 */
typedef struct RwV4dTag RWALIGN(RwV4d, rwV4DALIGNMENT);

#else /* (!defined(DOXYGEN)) */

#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup rwv4d
 * \struct RwV4d
 *  This type represents 4D points and vectors specified by
 * the (x, y, z, w) coordinates of a 4D point or the (x, y, z, w) components of a
 * 4D vector.
 */
#endif /* RWADOXYGENEXTERNAL */

typedef struct RwV4d RWALIGN(RwV4d, rwV4DALIGNMENT);

#endif /* (!defined(DOXYGEN)) */


/* RWPUBLICEND */
typedef struct RwV3dFixed RwV3dFixed;
struct RwV3dFixed
{
    RwFixed x, y, z;
};

typedef struct RwV3dSplitBits RwV3dSplitBits;
struct RwV3dSplitBits
{
    RwSplitBits x, y, z;
};

/* RWPUBLIC */

typedef struct RwRect RwRect;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup geometricaltypes
 * \struct RwRect
 * This type represents a 2D device space rectangle specified
 * by the position of the top-left corner (the offset x, y) and its width (w)
 * and height (h).
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwRect
{
    RwInt32 x;  /**< X value of the top-left corner */
    RwInt32 y;  /**< Y value of the top-left corner */
    RwInt32 w;  /**< Width of the rectangle */
    RwInt32 h;  /**< Height of the rectangle */
};

typedef struct RwSphere RwSphere;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup geometricaltypes
 * \struct RwSphere
 * This type represents a sphere specified by the position
 * of its center and its radius.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwSphere
{
    RwV3d center;   /**< Sphere center */
    RwReal radius;  /**< Sphere radius */
};

#if (!defined(RwSphereAssign))
#define RwSphereAssign(_target, _source)             \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwSphereAssign)) */

typedef struct RwLine RwLine;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup geometricaltypes
 * \struct RwLine
 * This type represents a 3D line specified by the position
 * of its start and end points.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwLine
{
    RwV3d start;    /**< Line start */
    RwV3d end;      /**< Line end */
};

#if (!defined(RwLineAssign))
#define RwLineAssign(_target, _source)             \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwLineAssign)) */

/* The maximum number of texture coordinates */
#define rwMAXTEXTURECOORDS 8
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup fundtypesdatatypes
 * RwTextureCoordinateIndex
 *  This type represents the index for texture coordinates.
 */
#endif /* RWADOXYGENEXTERNAL */
enum RwTextureCoordinateIndex
{
    rwNARWTEXTURECOORDINATEINDEX = 0,
    rwTEXTURECOORDINATEINDEX0,
    rwTEXTURECOORDINATEINDEX1,
    rwTEXTURECOORDINATEINDEX2,
    rwTEXTURECOORDINATEINDEX3,
    rwTEXTURECOORDINATEINDEX4,
    rwTEXTURECOORDINATEINDEX5,
    rwTEXTURECOORDINATEINDEX6,
    rwTEXTURECOORDINATEINDEX7,
    rwTEXTURECOORDINATEINDEXFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwTextureCoordinateIndex RwTextureCoordinateIndex;

typedef struct RwTexCoords RwTexCoords;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup fundtypesdatatypes
 * \struct RwTexCoords
 * This type represents the u and v texture
 * coordinates of a particular vertex.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwTexCoords
{
    RwReal u;   /**< U value */
    RwReal v;   /**< V value */
};


/* Singley linked list macros. End marked as NULL */

typedef struct RwSLLink RwSLLink;    /*** RwSLLink ***/

#if (!defined(DOXYGEN))
struct RwSLLink
{
    RwSLLink  *next;
};
#endif /* (!defined(DOXYGEN)) */

#define rwSLLinkGetData(link,type,entry)                                \
    ((type *)(((RwUInt8 *)(link))-offsetof(type,entry)))

#define rwSLLinkGetConstData(link,type,entry)                           \
    ((const type *)(((const RwUInt8 *)(link))-offsetof(type,entry)))

#define rwSLLinkInitialize(linkvar)                                     \
    (linkvar)->next = NULL;

#define rwSLLinkGetNext(linkvar)                                        \
    ((linkvar)->next)

typedef struct RwSingleList RwSingleList;

#if (!defined(DOXYGEN))
struct RwSingleList
{
    RwSLLink link;
};
#endif /* (!defined(DOXYGEN)) */

#define rwSingleListInitialize(list)                                    \
    (list)->link.next= NULL;
#define rwSingleListEmpty(list)                                         \
    (((list)->link.next)==NULL)
#define rwSingleListAddSLLink(list,linkvar)                             \
    ( (linkvar)->next = (list)->link.next,                              \
      (list)->link.next = (linkvar) )
#define rwSingleListGetFirstSLLink(list)                                \
    ((list)->link.next)
#define rwSingleListGetTerminator(list) (NULL)

/* Doubly linked list. End marked as start (its a ring) */

typedef struct RwLLLink  RwLLLink;                     /*** RwLLLink ***/

#if (!defined(DOXYGEN))
/**
 * \ingroup fundtypesdatatypes
 * \struct RwLLLink
 * RwLLLink is an internal two way linked list pointer.  It contains 
 * links to the previous and next RwLLLink's.  It is usually used in a 
 * ring list fashion.
 */
struct RwLLLink
{
    RwLLLink *next;
    RwLLLink *prev;
};
#endif /* (!defined(DOXYGEN)) */

#define rwLLLinkGetData(linkvar,type,entry)                             \
    ((type *)(((RwUInt8 *)(linkvar))-offsetof(type,entry)))

#define rwLLLinkGetConstData(linkvar,type,entry)                        \
    ((const type *)(((const RwUInt8 *)(linkvar))-offsetof(type,entry)))

#define rwLLLinkGetNext(linkvar)                                        \
    ((linkvar)->next)

#define rwLLLinkGetPrevious(linkvar)                                    \
    ((linkvar)->prev)

#define rwLLLinkInitialize(linkvar)                                     \
    ( (linkvar)->prev = (RwLLLink *)NULL,                               \
      (linkvar)->next = (RwLLLink *)NULL )

#define rwLLLinkAttached(linkvar)                                       \
    ((linkvar)->next)

typedef struct RwLinkList RwLinkList;

#if (!defined(DOXYGEN))
/**
 * \ingroup fundtypesdatatypes
 * \struct RwLLLink
 * Both the next and previous directions of an RwLinkList are linked in a 
 * ring list.  That is, to iterate over all the elements of this structure 
 * store the starting link and iterate over the elements until 
 * returning back to this first element. 
 */
struct RwLinkList
{
    RwLLLink link;
};
#endif /* (!defined(DOXYGEN)) */

#define rwLinkListInitialize(list)                                      \
    ( (list)->link.next = ((RwLLLink *)(list)),                         \
      (list)->link.prev = ((RwLLLink *)(list)) )
#define rwLinkListEmpty(list)                                           \
    (((list)->link.next) == (&(list)->link))
#define rwLinkListAddLLLink(list, linkvar)                              \
    ( (linkvar)->next = (list)->link.next,                              \
      (linkvar)->prev = (&(list)->link),                                \
      ((list)->link.next)->prev = (linkvar),                            \
      (list)->link.next = (linkvar) )
#define rwLinkListRemoveLLLink(linkvar)                                 \
    ( ((linkvar)->prev)->next = (linkvar)->next,                        \
      ((linkvar)->next)->prev = (linkvar)->prev )
#define rwLinkListGetFirstLLLink(list)                                  \
    ((list)->link.next)
#define rwLinkListGetLastLLLink(list)                                   \
    ((list)->link.prev)
#define rwLinkListGetTerminator(list)                                   \
    (&((list)->link))

typedef struct RwSurfaceProperties RwSurfaceProperties;
#ifndef RWADOXYGENEXTERNAL
/**
 * \ingroup fundtypesdatatypes
 * \struct RwSurfaceProperties
 *  This type represents the ambient, diffuse and
 * specular reflection coefficients of a particular geometry. Each coefficient
 * is specified in the range 0.0 (no reflection) to 1.0 (maximum reflection). 
 * Note that currently the specular element is not used.
 */
#endif /* RWADOXYGENEXTERNAL */
struct RwSurfaceProperties
{
    RwReal ambient;   /**< ambient reflection coefficient */
    RwReal specular;  /**< specular reflection coefficient */
    RwReal diffuse;   /**< reflection coefficient */
};

#if (!defined(RwSurfacePropertiesAssign))
#define RwSurfacePropertiesAssign(_target, _source)             \
    ( *(_target) = *(_source) )
#endif /* (!defined(RwSurfacePropertiesAssign)) */

/**********
 * Macros *
 **********/

/* ANSI C defines the offsetof(type,member) macro; should be in <stddef.h> */

/* If not, fall back to this: */
#ifndef offsetof
#define offsetof(type, member)                                          \
    ((size_t)((RwUInt8 *)&((type *) 0)->member - (RwUInt8 *)((type *) 0)))
#endif /* offsetof */

/*
 *
 * Numeric Macros to handle Fixed/Floating point versions of RenderWare
 *
 */
#define RWFIX_MIN     (1)
#define RWFIX_MAX     (0x7fffffff)
#define RwFixedToInt(a)    ((a) >> 16)
#define RwFixedToFloat(a)  ((float)(((float)(a)) * (1.0f / 65536.0f)))
#define RwFixedToReal(a)   ((RwReal)(((RwReal)(a)) * (1.0f / 65536.0f)))
#define RwIntToFixed(a)    ((a) << 16)
#define RwRealToFixed(a)   (RwInt32FromRealMacro((a) * 65536.0f))
#define RwRealAbs(a)       ((RwReal)((a) >= (RwReal)(0.0) ? (a) : (-(a))))
#define RwRealMin2(a,b)    ((RwReal)( ((a) <= (b)) ?  (a) : (b)))
#define RwRealMax2(a,b)    ((RwReal)( ((a) >= (b)) ?  (a) : (b)))
#define RwRealMin3(a,b,c)  RwRealMin2(a,RwRealMin2(b,c))
#define RwRealMax3(a,b,c)  RwRealMax2(a,RwRealMax2(b,c))

#ifndef NORWREALSHORTCUT
#define RToFixed RwRealToFixed
#define RAbs     RwRealAbs
#define FxToInt   RwFixedToInt
#define FxToFloat RwFixedToFloat
#define FxToReal  RwFixedToFloat

#endif

/* RWPUBLICEND */

/* RWPUBLIC */
#ifndef rwPI
#define rwPI ((RwReal)(3.1415926535f))
#define rwPIOVER2 (rwPI / (RwReal)(2.0f))
#endif
#define RWRGBALONG(r,g,b,a)                                             \
    ((RwUInt32) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

                             RwPlane

 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/*
 * typedef for struct RwPlane
 */
typedef struct RwPlane RwPlane;

#if (!defined(DOXYGEN))
/*
 * This type represents a plane
 */
struct RwPlane
{
    RwV3d normal;    /**< Normal to the plane */
    RwReal distance; /**< Distance to plane from origin in normal direction*/
};
#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 Defines
 */

enum RwPlaneType
{
    rwXPLANE = 0, /* These are deliberately multiples of sizeof(RwReal) */
    rwYPLANE = 4,
    rwZPLANE = 8,
    rwPLANETYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwPlaneType RwPlaneType;

#define rwSECTORATOMIC -1
#define rwSECTORBUILD -2       /* Only used when building a world */

/* vect is a RwV3d, y is the component */
#define GETCOORD(vect,y)                                                \
    (*(RwReal *)(((RwUInt8 *)(&((vect).x)))+(RwInt32)(y)))
#define GETCONSTCOORD(vect,y)                                           \
    (*(const RwReal *)(((const RwUInt8 *)(&((vect).x)))+(RwInt32)(y)))
#define SETCOORD(vect,y,value)                                          \
    (((*(RwReal *)(((RwUInt8 *)(&((vect).x)))+(RwInt32)(y))))=(value))
#define SETCONSTCOORD(vect,y,value)                                      \
    (((*(const RwReal *)                                                \
       (((const RwUInt8 *)                                              \
         (&((vect).x)))+(RwInt32)(y))))=(value))
#define GETCOORDINT(vect,y)                                             \
    (*(RwInt32 *)(((RwUInt8 *)(&((vect).x)))+(y)))
#define GETCONSTCOORDINT(vect,y)                                        \
    (*(const RwInt32 *)(((const RwUInt8 *)(&((vect).x)))+(y)))


/**
 * \ingroup integertypes
 * \page inttypes Integer Types
 *
 * RenderWare supports a number of integer types:
 *
 * RwInt8 8-bit signed integer.
 *    \li RwUInt8 8-bit unsigned integer.
 *    \li RwChar Character type.
 *    \li RwInt16 16-bit signed integer.
 *    \li RwUInt16 16-bit unsigned integer.
 *    \li RwInt32 32-bit signed integer.
 *    \li RwUInt32 32-bit unsigned integer.
 *    \li RwInt64 64-bit signed integer.
 *    \li RwUInt64 64-bit unsigned integer.
 *    \li RwInt128 128-bit signed integer.
 *    \li RwUInt128 128-bit unsigned integer.
 *    \li RwBool Boolean type (in 32 bits).
 *
 * These types should be used in applications in preference to the underlying
 * native types.
 *
 * The following constants indicate the maximum and minimum values possible
 * for the various RenderWare integer types:
 *
 *    \li RwInt32MAXVAL   Maximum RwInt32 value.
 *    \li RwInt32MINVAL   Minimum RwInt32 value.
 *    \li RwUInt32MAXVAL   Maximum RwUInt32 value.
 *    \li RwUInt32MINVAL   Minimum RwUInt32 value.
 *    \li RwInt16MAXVAL   Maximum RwInt16 value.
 *    \li RwInt16MINVAL   Minimum RwInt16 value.
 *    \li RwUInt16MAXVAL   Maximum RwUInt16 value.
 *    \li RwUInt16MINVAL   Minimum RwUInt16 value.
 *
 * \see RwReal
 */

/**
 * \ingroup realtypes
 * \typedef RwReal
 *
 * RenderWare supports a single RwReal floating-point type to aid portability
 * across platforms. This type should be used in applications in preference to
 * the underlying native type.
 *
 * The constants RwRealMAXVAL and RwRealMINVAL are provided for determining
 * the maximum and minimum values possible using the RwReal type.
 *
 * In addition, the following macros are available for operations on RwReal
 * types:
 *      \li RwRealMin2(a, b)    Find the minimum of two RwReal values.
 *      \li RwRealMax2(a, b)    Find the maximum of two RwReal values.
 *      \li RwRealMin3(a, b, c)    Find the minimum of three RwReal values.
 *      \li RwRealMax3(a, b, c)    Find the maximum of three RwReal values.
 *      \li RwRealAbs(x)    Find the absolute value of a RwReal value.
 *
 * \see \ref inttypes
 */

/**
 * \ingroup realtypes
 * \typedef RwFixed
 *
 * RenderWare supports a single RwFixed fixed-point type.
 *
 * Although popular in the days when integer mathematics was much faster than
 * floating point mathematics, fixed-point math is now rarely used. It is
 * provided because it is still useful for some processes.
 *
 * The maximum and minimum size of an RwFixed value are defined by the constants
 * RWFIX_MAX and RWFIX_MIN respectively.
 *
 * The following macros are provided to help you work with RwFixed datatypes:
 *    \li RwFixedToInt(x)       Convert an RwFixed to an integer. (The fractional portion is lost.)
 *    \li RwFixedToFloat(x)     Convert an RwFixed to a float.
 *    \li RwFixedToReal(x)      Convert an RwFixed to an RwReal.
 *    \li RwRealToFixed(x)      Convert an RwReal to an RwFixed. (Some precision may be lost.)
 *    \li RwIntToFixed(x)       Convert an RwInt32 to an RwFixed. (Some precision may be lost.)
 */

/**
 * \ingroup integertypes
 * \typedef RwInt8
 *
 * Signed 8 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwUInt8
 *
 * Unsigned 8bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwChar
 *
 * Character type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwInt16
 *
 * Signed 16 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwUInt16
 *
 * Unsigned 16 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwInt32
 *
 * Signed 32 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwUInt32
 *
 * Unsigned 32 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwInt64
 *
 * Signed 64 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwUInt64
 *
 * Unsigned 64 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwInt128
 *
 * Signed 128 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwUInt128
 *
 * Unsigned 128 bit integer type.
 * \see \ref inttypes
 */

/**
 * \ingroup integertypes
 * \typedef RwBool
 *
 * Boolean type.
 * \see \ref inttypes
 */

/* RWPUBLICEND */

#endif /* RWTYPES_H */

