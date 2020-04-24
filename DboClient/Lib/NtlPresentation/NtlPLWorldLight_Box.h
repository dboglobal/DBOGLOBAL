#pragma once

#include "NtlPLWorldLight_Base.h"

struct SPLWorldLightCreateParam_Box : public SPLWorldLightCreateParam
{
	RwV3d vScale;
	RwV3d vRotate;
};

class CNtlPLWorldLight_Box : public CNtlPLWorldLight_Base
{
protected:
	CNtlPlane			m_Plane[6]; // 0:Front 1:Back 2:Left 3:Right 4:Up 5:Down
	RwV3d				m_vScale;
	RwV3d				m_vRotate;

public:
	CNtlPLWorldLight_Box();
	virtual ~CNtlPLWorldLight_Box();

	virtual RwBool				Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void				Destroy();

	virtual	void				SetRadius(RwReal fRadius) { return; }

	virtual EPLWorldLightType	GetWorldLightType()	{ return EPLWORLDLIGHT_BOX; }
	virtual RwBool				GetWorldLightColorf(const RwV3d* pvPos, RwRGBAReal* pclrfOut, void* pUserData);

	virtual void				SetRotate(const RwV3d* pvRot);
	virtual RwV3d*				GetRotate();

	virtual void				SetScaleVector(const RwV3d* pvScale);
	virtual RwV3d*				GetScaleVector();

	virtual void				SetPosition(const RwV3d* pvPos);

	virtual RwBool				IsIn(const RwV3d* pvPos, RwReal* pfOutDistance = NULL);

	virtual void				SetVision(RwBool _Flag);

protected:
	virtual void				CalcWorldLightBoxData();
};