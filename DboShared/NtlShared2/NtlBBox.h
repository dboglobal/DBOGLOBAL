//***********************************************************************************
//
//	File		:	NtlBBox.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	BBox Class
//
//***********************************************************************************

#pragma once

#pragma warning(disable : 4201)

#include "NtlSharedType.h"
#include "NtlVector.h"

#include <float.h>
#include <math.h>
#include <crtdbg.h>


class CNtlBBox
{
public:
	CNtlBBox();

	CNtlBBox(const CNtlBBox& rVal );

	CNtlBBox(const CNtlVector& rCenter, const CNtlVector& rXAxis, const CNtlVector& rYAxis, const CNtlVector& rZAxis, float fXExtent, float fYExtent, float fZExtent);


public:
	CNtlVector m_rCenter;
	CNtlVector m_rXAxis;
	CNtlVector m_rYAxis;
	CNtlVector m_rZAxis;
	float m_fXExtents;
	float m_fYExtents;
	float m_fZExtents;

};