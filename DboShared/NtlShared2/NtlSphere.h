//***********************************************************************************
//
//	File		:	NtlSphere.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	Sphere Class
//
//***********************************************************************************

#pragma once

#pragma warning(disable : 4201)

#include "NtlSharedType.h"
#include "NtlVector.h"

#include <float.h>
#include <math.h>
#include <crtdbg.h>


class CNtlSphere
{
public:
	CNtlSphere();

	CNtlSphere(const CNtlVector& rCenter, float fRadius);


public:
	CNtlVector m_rCenter;

	float m_fRadius;

};