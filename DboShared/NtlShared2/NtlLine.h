//***********************************************************************************
//
//	File		:	NtlLine.h
//
//	Begin		:	2008-01-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Jeong Ho, Rho   ( lleo52@ntl-inc.com )
//
//	Desc		:	Line Class
//
//***********************************************************************************

#pragma once

#pragma warning(disable : 4201)

#include "NtlSharedType.h"
#include "NtlVector.h"

#include <float.h>
#include <math.h>
#include <crtdbg.h>


class CNtlLine
{
public:
	CNtlLine();

	CNtlLine(const CNtlVector& rBegin, const CNtlVector& rEnd);


public:
	CNtlVector m_rBegin;

	CNtlVector m_rEnd;

};