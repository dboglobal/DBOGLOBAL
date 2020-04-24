//***********************************************************************************
//
//	File		:	NtlSessionFactory.h
//
//	Begin		:	2005-12-14
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Base Session Factory Abstract Class
//
//***********************************************************************************

#pragma once

class CNtlSession;

class CNtlSessionFactory
{
public:

	virtual CNtlSession *		CreateSession(SESSIONTYPE sessionType) = 0;
};

