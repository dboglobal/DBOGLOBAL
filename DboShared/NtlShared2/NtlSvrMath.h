//***********************************************************************************
//
//	File		:	NtlSvrMath.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	Mathmatics Class
//
//***********************************************************************************

#pragma once

#pragma warning(disable : 4201)

#include "NtlSharedType.h"
#include "NtlLine.h"
#include "NtlSphere.h"
#include "NtlBBox.h"

#include <float.h>
#include <math.h>


class CNtlSvrMath
{

public:

	static bool Line2DSphere2DTest( const CNtlLine& rLine, const CNtlSphere& rSphere );

	// rLine 에서 y 값은 무시된다.
	// rBBox 에서 y 값 및 y 축값은 무시된다.
	static bool Line2DBBox2DTest( const CNtlLine& rLine, const CNtlBBox& rBBox );
	static float CalculateObjectRadius( const CNtlVector vMax, const CNtlVector vMin, const float fObjectScale, const float fscale=1);
};