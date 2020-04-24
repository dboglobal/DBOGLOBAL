/***************************************************************************
 *                                                                         *
 * Module  : rwmath.h                                                      *
 *                                                                         *
 * Purpose : Abstraction API for math functions                            *
 *                                                                         *
 **************************************************************************/

#ifndef RWMATH_H
#define RWMATH_H

/****************************************************************************
 Includes
 */

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

#if (!defined(RwInt32FromRealMacro))
#define RwInt32FromRealMacro(x)                 \
    ((RwInt32)(x))
#endif /* (!defined(RwInt32FromRealMacro)) */

#if (!defined(RwFastRealToUInt32))
#define RwFastRealToUInt32(_x)                  \
   ((RwUInt32)RwInt32FromRealMacro(((RwReal)(_x))))
#endif /* (!defined(RwFastRealToUInt32)) */

/*
 * Ensure inclusion of prototypes for single precison maths functions
 * e.g. from
 * /usr/local/sce/ee/gcc/ee/include/math.h
 * /Program Files/Intel/Compiler4.0/include/mathf.h
 */

#if (defined(__ICL))

#if (defined(RWVERBOSE))

/*
 * See
 * http://www.eskimo.com/~scs/C-faq/q11.17.html
 */

#define _STRINGIFY(X) #X
#define _STRINGIFY_EXP(X) _STRINGIFY(X)

#pragma message ("Intel Compiler Version " _STRINGIFY_EXP(__ICL) ":" __FILE__ "(" _STRINGIFY_EXP(__LINE__) ")\n")
#pragma comment ( user, "comment:" "Intel Compiler Version " _STRINGIFY_EXP(__ICL) ":" __FILE__ "(" _STRINGIFY_EXP(__LINE__) ")\n")

#endif /* (defined(RWVERBOSE)) */

#if (400 <= __ICL)
#if (defined(__cplusplus))
#define _INC_MATH
#endif /* (defined(__cplusplus)) */
#include <mathf.h>
#else /* (400 < __ICL) */
#undef RW_USE_SPF
#endif /* (400 < __ICL) */

#endif /* (defined(__ICL)) */

#include <math.h>

#define   _RW_C1      ( (float) 4.1666667908e-02 )
#define   _RW_C2      ( (float)-1.3888889225e-03 )
#define   _RW_C3      ( (float) 2.4801587642e-05 )
#define   _RW_C4      ( (float)-2.7557314297e-07 )
#define   _RW_C5      ( (float) 2.0875723372e-09 )
#define   _RW_C6      ( (float)-1.1359647598e-11 )
#define   _RW_S1      ( (float)-1.6666667163e-01 )
#define   _RW_S2      ( (float) 8.3333337680e-03 )
#define   _RW_S3      ( (float)-1.9841270114e-04 )
#define   _RW_S4      ( (float) 2.7557314297e-06 )
#define   _RW_S5      ( (float)-2.5050759689e-08 )
#define   _RW_S6      ( (float) 1.5896910177e-10 )
#define   _RW_one     ( (float) 1.0000000000e+00 )
#define   _RW_pS0     ( (float) 1.6666667163e-01 )
#define   _RW_pS1     ( (float)-3.2556581497e-01 )
#define   _RW_pS2     ( (float) 2.0121252537e-01 )
#define   _RW_pS3     ( (float)-4.0055535734e-02 )
#define   _RW_pS4     ( (float) 7.9153501429e-04 )
#define   _RW_pS5     ( (float) 3.4793309169e-05 )
#define   _RW_pi      ( (float) 3.1415925026e+00 )
#define   _RW_pi_tol  ( (float) 0.0312500000e+00 )
#define   _RW_pio2_hi ( (float) 1.5707962513e+00 )
#define   _RW_pio2_lo ( (float) 7.5497894159e-08 )
#define   _RW_qS1     ( (float)-2.4033949375e+00 )
#define   _RW_qS2     ( (float) 2.0209457874e+00 )
#define   _RW_qS3     ( (float)-6.8828397989e-01 )
#define   _RW_qS4     ( (float) 7.7038154006e-02 )

#define RwCosMinusPiToPiMacro(result, x)                          \
MACRO_START                                                       \
{                                                                 \
    const float z = x * x;                                        \
    const float r = ( z * (_RW_C1  +                              \
                           z * (_RW_C2  +                         \
                                z * (_RW_C3  +                    \
                                     z * (_RW_C4  +               \
                                          z * (_RW_C5  +          \
                                               z * _RW_C6))))));  \
    result = (_RW_one - ((float) 0.5 * z - (z * r )));            \
}                                                                 \
MACRO_STOP

#define RwSinMinusPiToPiMacro(result, x)                          \
do                                                                \
{                                                                 \
    const float z = x * x;                                        \
    const float v = z * x;                                        \
    const float r = ( _RW_S2 +                                    \
                      z * (_RW_S3 +                               \
                           z * (_RW_S4 +                          \
                                z * (_RW_S5 +                     \
                                     z * _RW_S6))) );             \
    result = x + v * (_RW_S1 + z * r);                            \
}                                                                 \
while(0)                                                                  

typedef union _rwIEEEFloatShapeType _rwIEEEFloatShapeType;

#if (!defined(DOXYGEN))
union _rwIEEEFloatShapeType
{
    float               value;
    unsigned int        word;
};
#endif /* (!defined(DOXYGEN)) */

#define _RW_GET_FLOAT_WORD(i,d)                 \
do {                                            \
  _rwIEEEFloatShapeType gf_u;                   \
  gf_u.value = (d);                             \
  (i) = gf_u.word;                              \
} while (0)

/* Set a float from a 32 bit int.  */

#define _RW_SET_FLOAT_WORD(d,i)                 \
do {                                            \
  _rwIEEEFloatShapeType sf_u;                   \
  sf_u.word = (i);                              \
  (d) = sf_u.value;                             \
} while (0)

#define RwIEEEACosfMacro(result, x)                                      \
do                                                                       \
{                                                                        \
    float               z, p, q, r, w, s, c, df;                         \
    int                 hx, ix;                                          \
                                                                         \
    _RW_GET_FLOAT_WORD(hx, x);                                           \
    ix = hx & 0x7fffffff;                                                \
    if (ix >= 0x3f800000)                                                \
    {                          /* |x|>=1 */                              \
        if (hx > 0)                                                      \
        {                                                                \
            /* acos(1) = 0  */                                           \
            result = (0.0);                                              \
        }                                                                \
        else                                                             \
        {                                                                \
            /* acos(-1)= _RW_pi */                                       \
            result = (_RW_pi + (float) 2.0 * _RW_pio2_lo);               \
        }                                                                \
                                                                         \
    }                                                                    \
    else if (ix < 0x3f000000)                                            \
    {                          /* |x| < 0.5 */                           \
        if (ix <= 0x23000000)                                            \
        {                                                                \
            /*if |x|<2**-57 */                                           \
            result = (_RW_pio2_hi + _RW_pio2_lo);                        \
        }                                                                \
        else                                                             \
        {                                                                \
            z = x * x;                                                   \
            p = z * (_RW_pS0 +                                           \
                     z * (_RW_pS1 +                                      \
                          z * (_RW_pS2 +                                 \
                               z * (_RW_pS3 +                            \
                                    z * (_RW_pS4 + z * _RW_pS5)))));     \
            q = _RW_one + z * (_RW_qS1 +                                 \
                               z * (_RW_qS2 +                            \
                                    z * (_RW_qS3 + z * _RW_qS4)));       \
            r = p / q;                                                   \
            result = (_RW_pio2_hi - (x - (_RW_pio2_lo - x * r)));        \
        }                                                                \
                                                                         \
    }                                                                    \
    else if (hx < 0)                                                     \
    {                          /* x < -0.5 */                            \
        z = (_RW_one + x) * (float) 0.5;                                 \
        p = z * (_RW_pS0 +                                               \
                 z * (_RW_pS1 +                                          \
                      z * (_RW_pS2 +                                     \
                           z * (_RW_pS3 +                                \
                                z * (_RW_pS4 + z * _RW_pS5)))));         \
        q = _RW_one + z * (_RW_qS1 +                                     \
                           z * (_RW_qS2 + z * (_RW_qS3 + z * _RW_qS4))); \
        rwSqrtMacro(&s, z);                                              \
        r = p / q;                                                       \
        w = r * s - _RW_pio2_lo;                                         \
        result = (_RW_pi - (float) 2.0 * (s + w));                       \
    }                                                                    \
    else                                                                 \
    {                          /* x > 0.5 */                             \
        int                 idf;                                         \
                                                                         \
        z = (_RW_one - x) * (float) 0.5;                                 \
        rwSqrtMacro(&s, z);                                              \
        df = s;                                                          \
        _RW_GET_FLOAT_WORD(idf, df);                                     \
        _RW_SET_FLOAT_WORD(df, idf & 0xfffff000);                        \
        c = (z - df * df) / (s + df);                                    \
        p = z * (_RW_pS0 +                                               \
                 z * (_RW_pS1 +                                          \
                      z * (_RW_pS2 +                                     \
                           z * (_RW_pS3 +                                \
                                z * (_RW_pS4 + z * _RW_pS5)))));         \
        q = _RW_one + z * (_RW_qS1 +                                     \
                           z * (_RW_qS2 + z * (_RW_qS3 + z * _RW_qS4))); \
        r = p / q;                                                       \
        w = r * s + c;                                                   \
        result = ((float) 2.0 * (df + w));                               \
    }                                                                    \
}                                                                        \
while(0)

#if (defined(PSP_OSTYPES_H))
/*
 * PSP versions of these SPF macros are defined
 * in the PSP version of ostypes.h (above)...
 */
#define RwACos(_x)              _rwPspACos(_x)           
#define RwACosh(_x)             _rwPspACosh(_x)          
#define RwASin(_x)              _rwPspASin(_x)           
#define RwASinh(_x)             _rwPspASinh(_x)          
#define RwATan2(_x, _y)         _rwPspATan2(_x, _y)      
#define RwATan(_x)              _rwPspATan(_x)           
#define RwATanh(_x)             _rwPspATanh(_x)          
#define RwCabs()                _rwPspCabs()             
#define RwCbrt(_x)              _rwPspCbrt(_x)           
#define RwCeil(_x)              _rwPspCeil(_x)           
#define RwCopysign(_x, _y)      _rwPspCopysign(_x, _y)   
#define RwCos(_x)               _rwPspCos(_x)            
#define RwCosh(_x)              _rwPspCosh(_x)           
#define RwDrem(_x, _y)          _rwPspDrem(_x, _y)       
#define RwErfc(_x)              _rwPspErfc(_x)           
#define RwErf(_x)               _rwPspErf(_x)            
#define RwExp(_x)               _rwPspExp(_x)            
#define RwExpm1(_x)             _rwPspExpm1(_x)          
#define RwFinite(_x)            _rwPspFinite(_x)         
#define RwIlogb(_x)             _rwPspIlogb(_x)          
#define RwIsinf(_x)             _rwPspIsinf(_x)          
#define RwIsnan(_x)             _rwPspIsnan(_x)          
#define RwFabs(_x)              _rwPspFabs(_x)           
#define RwFloor(_x)             _rwPspFloor(_x)          
#define RwFmod(_x, _y)          _rwPspFmod(_x, _y)       
#define RwFrexp(_x, _iptr)      _rwPspFrexp(_x, _iptr)   
#define RwGamma(_x)             _rwPspGamma(_x)          
#define RwGammaf_(_x, _iptr)    _rwPspGammaf_(_x, _iptr) 
#define RwHypot(_x, _y)         _rwPspHypot(_x, _y)      
#define RwInfinity()            _rwPspInfinity()         
#define RwJ0(_x)                _rwPspJ0(_x)             
#define RwJ1(_x)                _rwPspJ1(_x)             
#define RwJn(_i, _x)            _rwPspJn(_i, _x)         
#define RwLdexp(_x, _i)         _rwPspLdexp(_x, _i)      
#define RwLgamma(_x)            _rwPspLgamma(_x)         
#define RwLgammaf_(_x, _iptr)   _rwPspLgammaf_(_x, _iptr)
#define RwLog10(_x)             _rwPspLog10(_x)          
#define RwLog1p(_x)             _rwPspLog1p(_x)          
#define RwLog(_x)               _rwPspLog(_x)            
#define RwModf(_x, _y)          _rwPspModf(_x, _y)       
#define RwNan()                 _rwPspNan()              
#define RwNextafter(_x, _y)     _rwPspNextafter(_x, _y)  
#define RwPow(_x, _y)           _rwPspPow(_x, _y)        
#define RwRemainder(_x, _y)     _rwPspRemainder(_x, _y)  
#define RwRint(_x)              _rwPspRint(_x)           
#define RwScalbn(_x, _i)        _rwPspScalbn(_x, _i)     
#define RwSin(_x)               _rwPspSin(_x)            
#define RwSinh(_x)              _rwPspSinh(_x)           
#define RwTan(_x)               _rwPspTan(_x)            
#define RwTanh(_x)              _rwPspTanh(_x)           
#define RwY0(_x)                _rwPspY0(_x)             
#define RwY1(_x)                _rwPspY1(_x)             
#define RwYn(_i, _x)            _rwPspYn(_i, _x)         

#define rwSqrt(_result, _x)     rwSqrtMacro(_result, _x)

#else /* (defined(PSP_OSTYPES_H)) */

#if (defined(RW_USE_SPF))

#define RwACos(_x)              acosf(_x)
#define RwACosh(_x)             acoshf(_x)
#define RwASin(_x)              asinf(_x)
#define RwASinh(_x)             asinhf(_x)

#if (!defined(__ICL))
/*
 * No SPF version in
 * Program Files/Intel/compilerXXX/include/mathf.h
 * of atan2()
 */
#define RwATan2(_x, _y)         atan2f(_x, _y)
#endif /* (!defined(__ICL)) */

#define RwATan(_x)              atanf(_x)
#define RwATanh(_x)             atanhf(_x)
#define RwCabs()                cabsf()
#define RwCbrt(_x)              cbrtf(_x)
#define RwCeil(_x)              ceilf(_x)
#define RwCopysign(_x, _y)      copysignf(_x, _y)
#define RwCos(_x)               cosf(_x)
#define RwCosh(_x)              coshf(_x)
#define RwDrem(_x, _y)          dremf(_x, _y)
#define RwErfc(_x)              erfcf(_x)
#define RwErf(_x)               erff(_x)
#define RwExp(_x)               expf(_x)
#define RwExpm1(_x)             expm1f(_x)
#define RwFinite(_x)            finitef(_x)
#define RwIlogb(_x)             ilogbf(_x)
#define RwIsinf(_x)             isinff(_x)
#define RwIsnan(_x)             isnanf(_x)
#define RwFabs(_x)              fabsf(_x)
#define RwFloor(_x)             floorf(_x)
#define RwFmod(_x, _y)          fmodf(_x, _y)
#define RwFrexp(_x, _iptr)      frexpf(_x, _iptr)
#define RwGamma(_x)             gammaf(_x)
#define RwGammaf_(_x, _iptr)    gammaf_r(_x, _iptr)
#define RwHypot(_x, _y)         hypotf(_x, _y)
#define RwInfinity()            infinityf()
#define RwJ0(_x)                j0f(_x)
#define RwJ1(_x)                j1f(_x)
#define RwJn(_i, _x)            jnf(_i, _x)
#define RwLdexp(_x, _i)         ldexpf(_x, _i)
#define RwLgamma(_x)            lgammaf(_x)
#define RwLgammaf_(_x, _iptr)   lgammaf_r(_x, _iptr)
#define RwLog10(_x)             log10f(_x)
#define RwLog1p(_x)             log1pf(_x)
#define RwLog(_x)               logf(_x)
#define RwModf(_x, _y)          modff(_x, _y)
#define RwNan()                 nanf()
#define RwNextafter(_x, _y)     nextafterf(_x, _y)
#define RwPow(_x, _y)           powf(_x, _y)
#define RwRemainder(_x, _y)     remainderf(_x, _y)
#define RwRint(_x)              rintf(_x)
#define RwScalbn(_x, _i)        scalbnf(_x, _i)
#define RwSin(_x)               sinf(_x)
#define RwSinh(_x)              sinhf(_x)
/* rwSqrtMacro/rwInvSqrtMacro are overloaded in drvmodel.h
 * (if they are at all) and wrapped as func/macro below */
#define RwTan(_x)               tanf(_x)
#define RwTanh(_x)              tanhf(_x)
#define RwY0(_x)                y0f(_x)
#define RwY1(_x)                y1f(_x)
#define RwYn(_i, _x)            ynf(_i, _x)

#endif /* (defined(RW_USE_SPF)) */

#if (!defined(RwACos))
#define RwACos(_x)              acos(_x)
#endif /* (!defined(RwACos)) */
#if (!defined(RwACosh))
#define RwACosh(_x)             acosh(_x)
#endif /* (!defined(RwACosh)) */
#if (!defined(RwASin))
#define RwASin(_x)              asin(_x)
#endif /* (!defined(RwASin)) */
#if (!defined(RwASinh))
#define RwASinh(_x)             asinh(_x)
#endif /* (!defined(RwASinh)) */
#if (!defined(RwATan2))
#define RwATan2(_x, _y)         atan2(_x, _y)
#endif /* (!defined(RwATan2)) */
#if (!defined(RwATan))
#define RwATan(_x)              atan(_x)
#endif /* (!defined(RwATan)) */
#if (!defined(RwATanh))
#define RwATanh(_x)             atanh(_x)
#endif /* (!defined(RwATanh)) */
#if (!defined(RwCabs))
#define RwCabs()                cabs()
#endif /* (!defined(RwCabs)) */
#if (!defined(RwCbrt))
#define RwCbrt(_x)              cbrt(_x)
#endif /* (!defined(RwCbrt)) */
#if (!defined(RwCeil))
#define RwCeil(_x)              ceil(_x)
#endif /* (!defined(RwCeil)) */
#if (!defined(RwCopysign))
#define RwCopysign(_x, _y)      copysign(_x, _y)
#endif /* (!defined(RwCopysign)) */
#if (!defined(RwCos))
#define RwCos(_x)               cos(_x)
#endif /* (!defined(RwCos)) */
#if (!defined(RwCosh))
#define RwCosh(_x)              cosh(_x)
#endif /* (!defined(RwCosh)) */
#if (!defined(RwDrem))
#define RwDrem(_x, _y)          drem(_x, _y)
#endif /* (!defined(RwDrem)) */
#if (!defined(RwErfc))
#define RwErfc(_x)              erfc(_x)
#endif /* (!defined(RwErfc)) */
#if (!defined(RwEr))
#define RwEr(_x)               erf(_x)
#endif /* (!defined(RwEr)) */
#if (!defined(RwExp))
#define RwExp(_x)               exp(_x)
#endif /* (!defined(RwExp)) */
#if (!defined(RwExpm1))
#define RwExpm1(_x)             expm1(_x)
#endif /* (!defined(RwExpm1)) */
#if (!defined(RwFinite))
#define RwFinite(_x)            finite(_x)
#endif /* (!defined(RwFinite)) */
#if (!defined(RwIlogb))
#define RwIlogb(_x)             ilogb(_x)
#endif /* (!defined(RwIlogb)) */
#if (!defined(RwIsin))
#define RwIsin(_x)             isinf(_x)
#endif /* (!defined(RwIsin)) */
#if (!defined(RwIsnan))
#define RwIsnan(_x)             isnan(_x)
#endif /* (!defined(RwIsnan)) */
#if (!defined(RwFabs))
#define RwFabs(_x)              fabs(_x)
#endif /* (!defined(RwFabs)) */
#if (!defined(RwFloor))
#define RwFloor(_x)             floor(_x)
#endif /* (!defined(RwFloor)) */
#if (!defined(RwFmod))
#define RwFmod(_x, _y)          fmod(_x, _y)
#endif /* (!defined(RwFmod)) */
#if (!defined(RwFrexp))
#define RwFrexp(_x, _iptr)      frexp(_x, _iptr)
#endif /* (!defined(RwFrexp)) */
#if (!defined(RwGamma))
#define RwGamma(_x)             gamma(_x)
#endif /* (!defined(RwGamma)) */
#if (!defined(RwGammaf_))
#define RwGammaf_(_x, _iptr)    gammaf_r(_x, _iptr)
#endif /* (!defined(RwGammaf_)) */
#if (!defined(RwHypot))
#define RwHypot(_x, _y)         hypot(_x, _y)
#endif /* (!defined(RwHypot)) */
#if (!defined(RwInfinity))
#define RwInfinity()            infinity()
#endif /* (!defined(RwInfinity)) */
#if (!defined(RwJ0))
#define RwJ0(_x)                j0(_x)
#endif /* (!defined(RwJ0)) */
#if (!defined(RwJ1))
#define RwJ1(_x)                j1(_x)
#endif /* (!defined(RwJ1)) */
#if (!defined(RwJn))
#define RwJn(_i, _x)            jn(_i, _x)
#endif /* (!defined(RwJn)) */
#if (!defined(RwLdexp))
#define RwLdexp(_x, _i)         ldexp(_x, _i)
#endif /* (!defined(RwLdexp)) */
#if (!defined(RwLgamma))
#define RwLgamma(_x)            lgamma(_x)
#endif /* (!defined(RwLgamma)) */
#if (!defined(RwLgammaf_))
#define RwLgammaf_(_x, _iptr)   lgammaf_r(_x, _iptr)
#endif /* (!defined(RwLgammaf_)) */
#if (!defined(RwLog10))
#define RwLog10(_x)             log10(_x)
#endif /* (!defined(RwLog10)) */
#if (!defined(RwLog1p))
#define RwLog1p(_x)             log1p(_x)
#endif /* (!defined(RwLog1p)) */
#if (!defined(RwLog))
#define RwLog(_x)               log(_x)
#endif /* (!defined(RwLog)) */
#if (!defined(RwMod))
#define RwMod(_x, _y)           mod(_x, _y )
#endif /* (!defined(RwMod)) */
#if (!defined(RwNan))
#define RwNan()                 nan()
#endif /* (!defined(RwNan)) */
#if (!defined(RwNextafter))
#define RwNextafter(_x, _y)     nextafter(_x, _y)
#endif /* (!defined(RwNextafter)) */
#if (!defined(RwPow))
#define RwPow(_x, _y)           pow(_x, _y)
#endif /* (!defined(RwPow)) */
#if (!defined(RwRemainder))
#define RwRemainder(_x, _y)     remainder(_x, _y)
#endif /* (!defined(RwRemainder)) */
#if (!defined(RwRint))
#define RwRint(_x)              rint(_x)
#endif /* (!defined(RwRint)) */
#if (!defined(RwScalbn))
#define RwScalbn(_x, _i)        scalbn(_x, _i)
#endif /* (!defined(RwScalbn)) */
#if (!defined(RwSin))
#define RwSin(_x)               sin(_x)
#endif /* (!defined(RwSin)) */
#if (!defined(RwSinh))
#define RwSinh(_x)              sinh(_x)
#endif /* (!defined(RwSinh)) */
#if (!defined(rwSqrt))
/* NOTE: this is overloaded in drvmodel.h for some targets (SKY2 and XBOX atm)
 * [we do in fact do overload w/ sqrtf there, if RW_USE_SPF,
 *  for D3D8, OpenGL and SoftRas] */
#define rwSqrt(_result, _x)     rwSqrtMacro(_result, _x)
#endif /* (!defined(rwSqrt)) */
#if (!defined(rwInvSqrt))
/* NOTE: this is overloaded in drvmodel.h for some targets (SKY2 and XBOX atm)
 * [we do in fact do overload w/ (1 / sqrtf) there, if RW_USE_SPF,
 *  for D3D8, OpenGL and SoftRas] */
#define rwInvSqrt(_recip, _x)   rwInvSqrtMacro(_recip, _x)
#endif /* (!defined(rwInvSqrt)) */
#if (!defined(RwTan))
#define RwTan(_x)               tan(_x)
#endif /* (!defined(RwTan)) */
#if (!defined(RwTanh))
#define RwTanh(_x)              tanh(_x)
#endif /* (!defined(RwTanh)) */
#if (!defined(RwY0))
#define RwY0(_x)                y0(_x)
#endif /* (!defined(RwY0)) */
#if (!defined(RwY1))
#define RwY1(_x)                y1(_x)
#endif /* (!defined(RwY1)) */
#if (!defined(RwYn))
#define RwYn(_i, _x)            yn(_i, _x)
#endif /* (!defined(RwYn)) */

#endif /* (defined(PSP_OSTYPES_H)) */

/* RWPUBLICEND */
#endif /* RWMATH_H */
