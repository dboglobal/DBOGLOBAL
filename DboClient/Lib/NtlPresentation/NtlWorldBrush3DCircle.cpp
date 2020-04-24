#include "precomp_ntlpresentation.h"
#include "NtlWorldBrush3DCircle.h"
#include "NtlPLGlobal.h"
#include "NtlCamera.h"


CNtlWorldBrush3DCircle::CNtlWorldBrush3DCircle()
{

}

CNtlWorldBrush3DCircle::~CNtlWorldBrush3DCircle()
{

}

void CNtlWorldBrush3DCircle::Render(RwSphere* _pSphere)
{
	RwIm3DVertex	circle[dPOINT_NUM4CIRCLE + 1];
	RwV3d			point;
	RwInt32			i;

	RwMatrix matRender = *RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	*RwMatrixGetPos(&matRender) = _pSphere->center;

	for(i = 0; i < dPOINT_NUM4CIRCLE + 1; i++)
	{
		point.x = (RwReal)RwCos(i / (dPOINT_NUM4CIRCLE / 2.0f) * rwPI) * _pSphere->radius;
		point.y = (RwReal)RwSin(i / (dPOINT_NUM4CIRCLE / 2.0f) * rwPI) * _pSphere->radius;
		point.z = 0.0;

		RwIm3DVertexSetPos(&circle[i], point.x, point.y, point.z);
		RwIm3DVertexSetRGBA(&circle[i], 255, 0, 0, 255);
	}


	RwReal fOldFarPlane = RpNtlActiveCameraGetFarDistance();
	if (RpNtlActiveCameraSetFarDistance(10000.0f))
	{
		RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEFLAT);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

		if(RwIm3DTransform(circle, dPOINT_NUM4CIRCLE + 1, &matRender, rwIM3D_ALLOPAQUE))
		{
			RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);
			RwIm3DEnd();
		}

		RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);
	}
	RpNtlActiveCameraSetFarDistance(fOldFarPlane);
}