#pragma once

// macro bit flag
#define BIT_FLAG_CLEAR(b)		( (b) = 0 )
#define BIT_FLAG_FLIP(b)		( (b) ^= (b) )
#define BIT_FLAG_SET(b, f)		( (b) |= (f) )
#define BIT_FLAG_UNSET(b, f)	( (b) &= ~(f) )
#define BIT_FLAG_TEST(b, f)		( (b) & (f) ? true : false ) 
#define BIT_FLAG_TOGGLE(b, f)	( (b) ^= (f) )
#define MAKE_BIT_FLAG(x)		( 0x01 << (x) )
#define MAKE_BIT_FLAG64(x)		( DWORD64(0x01) << (x) )