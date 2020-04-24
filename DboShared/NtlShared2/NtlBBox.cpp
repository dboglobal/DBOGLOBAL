//***********************************************************************************
//
//	File		:	NtlBBox.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	BBox Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlBBox.h"


CNtlBBox::CNtlBBox()
{
	// 초기화를 하지 않는다
}


CNtlBBox::CNtlBBox(const CNtlBBox& rVal )
{
	m_rCenter = rVal.m_rCenter;
	m_rXAxis = rVal.m_rXAxis;
	m_rYAxis = rVal.m_rYAxis;
	m_rZAxis = rVal.m_rZAxis;
	m_fXExtents = rVal.m_fXExtents;
	m_fYExtents = rVal.m_fYExtents;
	m_fZExtents = rVal.m_fZExtents;
}


CNtlBBox::CNtlBBox(const CNtlVector& rCenter, const CNtlVector& rXAxis, const CNtlVector& rYAxis, const CNtlVector& rZAxis, float fXExtent, float fYExtent, float fZExtent)
{
	m_rCenter = rCenter;
	m_rXAxis = rXAxis;
	m_rYAxis = rYAxis;
	m_rZAxis = rZAxis;
	m_fXExtents = fXExtent;
	m_fYExtents = fYExtent;
	m_fZExtents = fZExtent;
}
