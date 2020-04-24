#pragma once

#include "NtlPLWorldLight_Base.h"

struct SPLWorldLightCreateParam_Sphere : public SPLWorldLightCreateParam
{
	RwReal fRadius;
};

class CNtlPLWorldLight_Sphere : public CNtlPLWorldLight_Base
{
public:
	CNtlPLWorldLight_Sphere();
	virtual ~CNtlPLWorldLight_Sphere();

	virtual RwBool				Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void				Destroy();

	virtual	void				SetRadius(RwReal fRadius);

	virtual EPLWorldLightType	GetWorldLightType()	{ return EPLWORLDLIGHT_SPHERE; }
	virtual RwBool				GetWorldLightColorf(const RwV3d* pvPos, RwRGBAReal* pclrfOut, void* pUserData);

	virtual void				SetVision(RwBool _Flag);
};