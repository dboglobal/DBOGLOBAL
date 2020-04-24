//***********************************************************************************
//
//	File		:	NtlSvrMath.cpp
//
//	Begin		:	2008-01-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	Mathmatics Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSvrMath.h"


bool CNtlSvrMath::Line2DSphere2DTest( const CNtlLine& rLine, const CNtlSphere& rSphere )
{
	CNtlLine rLineVal = rLine;
	rLineVal.m_rBegin.y = 0.f;
	rLineVal.m_rEnd.y = 0.f;

	CNtlSphere rSphereVal = rSphere;
	rSphereVal.m_rCenter.y = 0.f;

	CNtlVector rLineNormal;
	float fRecipLineLength;
	float fAtomicRadius2;
	float fAtomicDistance;
	CNtlVector rE0;
	float fHyp2;

	/* Calculate line parameters */
	rLineNormal = rLineVal.m_rEnd - rLineVal.m_rBegin;
	fRecipLineLength = rLineNormal.Normalize();

	if ( 0.f == fRecipLineLength )
	{
		CNtlVector rDist = rSphereVal.m_rCenter - rLineVal.m_rBegin;

		if ( rDist.Length() < rSphereVal.m_fRadius )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		/* Get bounding sphere of atomic */
		fAtomicRadius2 = rSphereVal.m_fRadius * rSphereVal.m_fRadius;

		/* Offset atomic's center so that line is effectively starting at (0,0,0) */
		rE0 = rSphereVal.m_rCenter - rLineVal.m_rBegin;

		/* Project atomic's center distance along line */
		fAtomicDistance = rE0.Dot( rLineNormal );

		/* Distance to atomic center */
		fHyp2 = rE0.Dot( rE0 );

		if ( fHyp2 < fAtomicRadius2 )
		{
			return true;
		}
		else if ( fAtomicDistance > 0.f )
		{
			/* It's in front of the line */
			float fDisc;

			/* Do pythagoras to see if we intersect the sphere */
			fDisc = fAtomicRadius2 - (fHyp2 - (fAtomicDistance) * (fAtomicDistance));

			if ( fDisc >= 0.f )
			{
				return true;
			}
		}

		/* No collision */
		return false;
	}
}

bool CNtlSvrMath::Line2DBBox2DTest( const CNtlLine& rLine, const CNtlBBox& rBBox )
{
	CNtlLine rLineVal = rLine;
	rLineVal.m_rBegin.y = 0.f;
	rLineVal.m_rEnd.y = 0.f;

	CNtlBBox rBBoxVal = rBBox;
	rBBoxVal.m_rCenter.y = 0.f;
	rBBoxVal.m_rZAxis.y = 0.f;
	rBBoxVal.m_rXAxis.y = 0.f;

	CNtlVector rLineDir = rLineVal.m_rEnd - rLineVal.m_rBegin;
	if( rLineDir.Normalize() == 0.f )
	{
		if ( rLine.m_rBegin.x > rBBox.m_rCenter.x + rBBox.m_fXExtents ||
			 rLine.m_rBegin.z > rBBox.m_rCenter.z + rBBox.m_fZExtents ||
			 rLine.m_rBegin.x <= rBBox.m_rCenter.x - rBBox.m_fXExtents ||
			 rLine.m_rBegin.z <= rBBox.m_rCenter.z - rBBox.m_fZExtents )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		CNtlVector rDiff = rLineVal.m_rBegin - rBBoxVal.m_rCenter;
		CNtlVector rPerp;
		rPerp.x = rLineDir.z;
		rPerp.y = 0.f;
		rPerp.z = -rLineDir.x;

		float fLhs = (float)fabs( rPerp.Dot( rDiff ) );
		float fPart0 = (float)fabs( rPerp.Dot( rBBoxVal.m_rXAxis ) );
		float fPart1 = (float)fabs( rPerp.Dot( rBBoxVal.m_rZAxis ) );
		float fRhs = rBBoxVal.m_fXExtents * fPart0 + rBBoxVal.m_fZExtents * fPart1;

		return fLhs <= fRhs;
	}
}

float CNtlSvrMath::CalculateObjectRadius( const CNtlVector vMax, const CNtlVector vMin, const float fObjectScale, const float fscale/*=1*/ )
{
	const float fWidth = vMax.x - vMin.x;
	const float fDepth = vMax.z - vMin.x;

	return (((fWidth + fDepth) * fObjectScale) * fscale) / 4.0f;
}