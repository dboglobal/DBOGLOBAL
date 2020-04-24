//***********************************************************************************
//
//	File		:	NtlSphere.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	Sphere Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSphere.h"


CNtlSphere::CNtlSphere()
{
	// 초기화를 하지 않는다
}


CNtlSphere::CNtlSphere(const CNtlVector& rCenter, float fRadius)
{
	m_rCenter = rCenter;
	m_fRadius = fRadius;
}
