#include "precomp_ntlpresentation.h"

#include "NtlPLRenderGeometry.h"

#define CONE_NUM_VERTICES           (10)
#define SPHERE_NUM_VERTICES		    (32)
#define ARROW_NUM_VERTICES			(20)
#define ARROW_CYLINDER_DIAMETER		(1.5f)
#define ARROW_CYLINDER_LENGTH		(5.0f)
#define ARROW_CONE_SIZE				(4.0f)
#define ARROW_CONE_ANGLE			(0.25f*rwPI)

void RenderGeometryCone(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fConeAngle, RwReal fConeSize, RwBool bWire)
{
	static RwIm3DVertex    aImVertices[CONE_NUM_VERTICES + 1];
	static RwImVertexIndex aImIndices[3 * (CONE_NUM_VERTICES * 2)];
	
	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);
	
	RwV3d	vPoint;
	RwReal  fCosValue, fSinValue, fConeAngleD;
	RwV3d   vdRight, vdUp, vdAt;
	
	fConeAngleD = (RwReal)RwCos(fConeAngle);

	for (RwInt32 i = 0; i < CONE_NUM_VERTICES; ++i)
	{
		fCosValue = (RwReal)(RwCos(i * 2 * rwPI / CONE_NUM_VERTICES) * RwSin(fConeAngle));
		fSinValue = (RwReal)(RwSin(i * 2 * rwPI / CONE_NUM_VERTICES) * RwSin(fConeAngle));

		RwV3dScale(&vdUp, pvUp, fSinValue * fConeSize);
		RwV3dScale(&vdRight, pvRight, fCosValue * fConeSize);
		RwV3dScale(&vdAt, pvAt, fConeAngleD * fConeSize);

		vPoint.x = pvPos->x + vdAt.x + vdUp.x + vdRight.x;
		vPoint.y = pvPos->y + vdAt.y + vdUp.y + vdRight.y;
		vPoint.z = pvPos->z + vdAt.z + vdUp.z + vdRight.z;

		RwIm3DVertexSetRGBA(&(aImVertices[i + 1]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
		RwIm3DVertexSetPos(&(aImVertices[i + 1]), vPoint.x, vPoint.y, vPoint.z);
	}
	RwIm3DVertexSetRGBA(&(aImVertices[0]), prgbaColor->red,	prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[0]),  pvPos->x, pvPos->y, pvPos->z);

	RwInt32 iIdx = 0;
	for (RwInt32 i = 0; i < CONE_NUM_VERTICES; ++i)
	{
		aImIndices[iIdx++] = 0;
		aImIndices[iIdx++] = i + 2;
		aImIndices[iIdx++] = i + 1;
	}
	aImIndices[iIdx - 2] = 1;
	for (RwInt32 i = 0; i < CONE_NUM_VERTICES; ++i)
	{
		aImIndices[iIdx++] = 0;
		aImIndices[iIdx++] = i + 1;
		aImIndices[iIdx++] = i + 2;
	}
	aImIndices[iIdx - 1] = 1;

	if (bWire)
	{
		RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	RwD3D9SetTexture(NULL, 0);
	if (RwIm3DTransform(aImVertices, (CONE_NUM_VERTICES + 1), NULL, 0))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, aImIndices, 3 * (CONE_NUM_VERTICES * 2));
		RwIm3DEnd();
	}
	if (bWire)
	{
		RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void RenderGeometryArrow(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fArrowSize, RwBool bWire)
{
	static RwIm3DVertex    aImVertices[ARROW_NUM_VERTICES * 2];
	static RwImVertexIndex aImIndices[3 * (2 * ARROW_NUM_VERTICES +	2 * (ARROW_NUM_VERTICES - 2))];

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwV3d     vPoint;
	RwReal    fCosValue, fSinValue, fConeOffset;
	RwV3d     vdRight, vdUp, vdAt;
	RwV3d     vtRight, vtUp, vtAt;
		
 	RwV3dScale(&vtRight, pvRight, fArrowSize);
 	RwV3dScale(&vtUp,    pvUp,    fArrowSize);
 	RwV3dScale(&vtAt,    pvAt,    fArrowSize);

	fConeOffset = ARROW_CONE_SIZE * (RwReal)RwCos(ARROW_CONE_ANGLE);

	for (RwInt32 i = 0; i < (2 * ARROW_NUM_VERTICES); i += 2)
	{
		fCosValue = (RwReal)RwCos(rwPI*i / (RwReal)ARROW_NUM_VERTICES);
		fSinValue = (RwReal)RwSin(rwPI*i / (RwReal)ARROW_NUM_VERTICES);

		RwV3dScale(&vdUp,    &vtRight,  fSinValue * ARROW_CYLINDER_DIAMETER);
		RwV3dScale(&vdRight, &vtUp,		fCosValue * ARROW_CYLINDER_DIAMETER);
		RwV3dScale(&vdAt,    &vtAt,		 -fConeOffset);

		/* Cylinder base vertices... */
		vPoint.x = pvPos->x + vdRight.x + vdUp.x + vdAt.x;
		vPoint.y = pvPos->y + vdRight.y + vdUp.y + vdAt.y;
		vPoint.z = pvPos->z + vdRight.z + vdUp.z + vdAt.z;
		RwIm3DVertexSetPos(&(aImVertices[i]), vPoint.x, vPoint.y, vPoint.z);

		RwV3dScale(&vdAt, &vtAt, -(ARROW_CYLINDER_LENGTH + ARROW_CONE_SIZE));
		vPoint.x = pvPos->x + vdAt.x + vdUp.x + vdRight.x;
		vPoint.y = pvPos->y + vdAt.y + vdUp.y + vdRight.y;
		vPoint.z = pvPos->z + vdAt.z + vdUp.z + vdRight.z;
		RwIm3DVertexSetPos(&(aImVertices[i + 1]), vPoint.x, vPoint.y, vPoint.z);
	}
	
	for (RwInt32 i = 0; i < (2 * ARROW_NUM_VERTICES); ++i)
	{
		RwIm3DVertexSetRGBA(&(aImVertices[i]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	}

	RwInt32 iIdx = 0;
	for (RwInt32 i = 0; i < 2 * ARROW_NUM_VERTICES;)
	{
		aImIndices[iIdx++] = i++;
		aImIndices[iIdx++] = i++;
		aImIndices[iIdx++] = (i % (2*ARROW_NUM_VERTICES));

		aImIndices[iIdx++] = (i % (2*ARROW_NUM_VERTICES));
		aImIndices[iIdx++] = i - 1;
		aImIndices[iIdx++] = ((i + 1) % (2*ARROW_NUM_VERTICES));
	}
	for (RwInt32 i = 0; i < (ARROW_NUM_VERTICES - 2); ++i)
	{
		aImIndices[iIdx++] = 0;
		aImIndices[iIdx++] = 2*(i + 1);
		aImIndices[iIdx++] = 2*(i + 2);
	}
	for (RwInt32 i = 0; i < (ARROW_NUM_VERTICES - 2); ++i)
	{
		aImIndices[iIdx++] = 1;
		aImIndices[iIdx++] = 1 + 2*(i + 2);
		aImIndices[iIdx++] = 1 + 2*(i + 1);
	}

	if (bWire)
	{
		RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	RwD3D9SetTexture(NULL, 0);
	if(RwIm3DTransform(aImVertices, (2 * ARROW_NUM_VERTICES), NULL, 0))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, aImIndices, 3 * (2 * ARROW_NUM_VERTICES + 2 * (ARROW_NUM_VERTICES - 2)));
		RwIm3DEnd();
	}
	if (bWire)
	{
		RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	RenderGeometryCone(pmat, prgbaColor, ARROW_CONE_ANGLE, -fArrowSize * ARROW_CONE_SIZE, bWire);
}

void RenderGeometryBillboardSpherePlane(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fRadius, RwCamera* pCamera)
{
	static RwIm3DVertex    aImVertices[SPHERE_NUM_VERTICES + 1];
	static RwImVertexIndex aImIndices[SPHERE_NUM_VERTICES * 3];

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwV3d     vPoint;
	
	RwMatrix* pmatCamera	= RwFrameGetLTM(RwCameraGetFrame(pCamera));
	RwMatrix* pmatAlign		= RwMatrixCreate();

	for (RwInt32 i = 0; i < SPHERE_NUM_VERTICES; ++i)
	{
		RwV3dScale(&vPoint, RwMatrixGetRight(pmatCamera), fRadius);
		RwMatrixRotate(pmatAlign, RwMatrixGetAt(pmatCamera), (360 * i / (RwReal)SPHERE_NUM_VERTICES), rwCOMBINEREPLACE);
		RwMatrixTranslate(pmatAlign, pvPos, rwCOMBINEPOSTCONCAT);
		RwV3dTransformPoint(&vPoint, &vPoint, pmatAlign);

		RwIm3DVertexSetRGBA(&(aImVertices[i]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
		RwIm3DVertexSetPos(&(aImVertices[i]), vPoint.x, vPoint.y, vPoint.z);
	}
	vPoint.x = vPoint.y = vPoint.z = 0.0f;
	RwMatrixTranslate(pmatAlign, pvPos, rwCOMBINEREPLACE);
	RwV3dTransformPoint(&vPoint, &vPoint, pmatAlign);
	RwIm3DVertexSetRGBA(&(aImVertices[SPHERE_NUM_VERTICES]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[SPHERE_NUM_VERTICES]), vPoint.x, vPoint.y, vPoint.z);

	RwInt32 iIdx = 0;
	for (RwInt32 i = 0; i < SPHERE_NUM_VERTICES - 1; ++i)
	{
		aImIndices[iIdx++] = SPHERE_NUM_VERTICES;
		aImIndices[iIdx++] = i + 1;
		aImIndices[iIdx++] = i;
	}
	aImIndices[iIdx++] = SPHERE_NUM_VERTICES;
	aImIndices[iIdx++] = 0;
	aImIndices[iIdx++] = SPHERE_NUM_VERTICES - 1;

	RwD3D9SetTexture(NULL, 0);
	if (RwIm3DTransform(aImVertices, SPHERE_NUM_VERTICES + 1, NULL, 0))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, aImIndices, SPHERE_NUM_VERTICES * 3);
		RwIm3DEnd();
	}

	RwMatrixDestroy(pmatAlign);
}

void RenderGeometryBillboardSphereLine(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fRadius, RwCamera* pCamera)
{
	static RwIm3DVertex    aImVertices[SPHERE_NUM_VERTICES];
	static RwImVertexIndex aImIndices[SPHERE_NUM_VERTICES * 2 + 4];

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwV3d     vPoint;

	RwMatrix* pmatCamera	= RwFrameGetLTM(RwCameraGetFrame(pCamera));
	RwMatrix* pmatAlign		= RwMatrixCreate();

	for (RwInt32 i = 0; i < SPHERE_NUM_VERTICES; ++i)
	{
		RwV3dScale(&vPoint, RwMatrixGetRight(pmatCamera), fRadius);
		RwMatrixRotate(pmatAlign, RwMatrixGetAt(pmatCamera), (360 * i / (RwReal)SPHERE_NUM_VERTICES), rwCOMBINEREPLACE);
		RwMatrixTranslate(pmatAlign, pvPos, rwCOMBINEPOSTCONCAT);
		RwV3dTransformPoint(&vPoint, &vPoint, pmatAlign);

		RwIm3DVertexSetRGBA(&(aImVertices[i]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
		RwIm3DVertexSetPos(&(aImVertices[i]), vPoint.x, vPoint.y, vPoint.z);
	}

	RwInt32 iIdx = 0;
	for (RwInt32 i = 0; i < SPHERE_NUM_VERTICES;i++)
	{
		aImIndices[iIdx++] = i;
		aImIndices[iIdx++] = i + 1;
	}
	aImIndices[iIdx - 1] = 0;

	aImIndices[iIdx++] = 0;
	aImIndices[iIdx++] = SPHERE_NUM_VERTICES >> 1;
	aImIndices[iIdx++] = SPHERE_NUM_VERTICES >> 2;
	aImIndices[iIdx++] = 3 * (SPHERE_NUM_VERTICES >> 2);

	RwD3D9SetTexture(NULL, 0);
	if(RwIm3DTransform(aImVertices, SPHERE_NUM_VERTICES, NULL, rwIM3D_ALLOPAQUE))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, aImIndices, 4 + 2 * SPHERE_NUM_VERTICES);
		RwIm3DEnd();
	}
	RwMatrixDestroy(pmatAlign);
}

void RenderGeometryBillboardSphere(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fRadius, RwCamera* pCamera, RwBool bWire)
{
	if (bWire)
	{
		RenderGeometryBillboardSphereLine(pmat, prgbaColor, fRadius, pCamera);
	}
	else
	{
		RenderGeometryBillboardSpherePlane(pmat, prgbaColor, fRadius, pCamera);
	}
}

void RenderGeometrySpherePlane(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fRadius)
{
	static RwIm3DVertex    aImVertices[SPHERE_NUM_VERTICES + 1];
	static RwImVertexIndex aImIndices[SPHERE_NUM_VERTICES * 3];

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwV3d     vPoint;

	RwMatrix* pmatAlign		= RwMatrixCreate();

	for (RwInt32 i = 0; i < SPHERE_NUM_VERTICES; ++i)
	{
		RwV3dScale(&vPoint, pvRight, fRadius);
		RwMatrixRotate(pmatAlign, pvUp, (360 * i / (RwReal)SPHERE_NUM_VERTICES), rwCOMBINEREPLACE);
		RwMatrixTranslate(pmatAlign, pvPos, rwCOMBINEPOSTCONCAT);
		RwV3dTransformPoint(&vPoint, &vPoint, pmatAlign);

		RwIm3DVertexSetRGBA(&(aImVertices[i]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
		RwIm3DVertexSetPos(&(aImVertices[i]), vPoint.x, vPoint.y, vPoint.z);
	}
	vPoint.x = vPoint.y = vPoint.z = 0.0f;
	RwMatrixTranslate(pmatAlign, pvPos, rwCOMBINEREPLACE);
	RwV3dTransformPoint(&vPoint, &vPoint, pmatAlign);
	RwIm3DVertexSetRGBA(&(aImVertices[SPHERE_NUM_VERTICES]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[SPHERE_NUM_VERTICES]), vPoint.x, vPoint.y, vPoint.z);

	RwInt32 iIdx = 0;
	for (RwInt32 i = 0; i < SPHERE_NUM_VERTICES - 1; ++i)
	{
		aImIndices[iIdx++] = SPHERE_NUM_VERTICES;
		aImIndices[iIdx++] = i + 1;
		aImIndices[iIdx++] = i;
	}
	aImIndices[iIdx++] = SPHERE_NUM_VERTICES;
	aImIndices[iIdx++] = 0;
	aImIndices[iIdx++] = SPHERE_NUM_VERTICES - 1;

	RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

	RwD3D9SetTexture(NULL, 0);
	if (RwIm3DTransform(aImVertices, SPHERE_NUM_VERTICES + 1, NULL, 0))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, aImIndices, SPHERE_NUM_VERTICES * 3);
		RwIm3DEnd();
	}

	RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);

	RwMatrixDestroy(pmatAlign);
}

void RenderGeometryQuadPlane(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fSizeHalfX, RwReal fSizeHalfZ)
{
	static RwIm3DVertex    aImVertices[4];

	RwV3d* pvRight	= RwMatrixGetRight(pmat);
	RwV3d* pvUp		= RwMatrixGetUp(pmat);
	RwV3d* pvAt		= RwMatrixGetAt(pmat);
	RwV3d* pvPos	= RwMatrixGetPos(pmat);

	RwV3d     vPoint;

	vPoint.x = pvPos->x + (pvRight->x * fSizeHalfX) + (pvAt->x * fSizeHalfZ);
	vPoint.y = pvPos->y + (pvRight->y * fSizeHalfX) + (pvAt->y * fSizeHalfZ);
	vPoint.z = pvPos->z + (pvRight->z * fSizeHalfX) + (pvAt->z * fSizeHalfZ);
	RwIm3DVertexSetRGBA(&(aImVertices[0]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[0]), vPoint.x, vPoint.y, vPoint.z);

	vPoint.x = pvPos->x + (pvRight->x * fSizeHalfX) - (pvAt->x * fSizeHalfZ);
	vPoint.y = pvPos->y + (pvRight->y * fSizeHalfX) - (pvAt->y * fSizeHalfZ);
	vPoint.z = pvPos->z + (pvRight->z * fSizeHalfX) - (pvAt->z * fSizeHalfZ);
	RwIm3DVertexSetRGBA(&(aImVertices[1]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[1]), vPoint.x, vPoint.y, vPoint.z);

	vPoint.x = pvPos->x - (pvRight->x * fSizeHalfX) + (pvAt->x * fSizeHalfZ);
	vPoint.y = pvPos->y - (pvRight->y * fSizeHalfX) + (pvAt->y * fSizeHalfZ);
	vPoint.z = pvPos->z - (pvRight->z * fSizeHalfX) + (pvAt->z * fSizeHalfZ);
	RwIm3DVertexSetRGBA(&(aImVertices[2]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[2]), vPoint.x, vPoint.y, vPoint.z);

	vPoint.x = pvPos->x - (pvRight->x * fSizeHalfX) - (pvAt->x * fSizeHalfZ);
	vPoint.y = pvPos->y - (pvRight->y * fSizeHalfX) - (pvAt->y * fSizeHalfZ);
	vPoint.z = pvPos->z - (pvRight->z * fSizeHalfX) - (pvAt->z * fSizeHalfZ);
	RwIm3DVertexSetRGBA(&(aImVertices[3]), prgbaColor->red, prgbaColor->green, prgbaColor->blue, prgbaColor->alpha);
	RwIm3DVertexSetPos(&(aImVertices[3]), vPoint.x, vPoint.y, vPoint.z);

	RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLNONE);

	RwD3D9SetTexture(NULL, 0);
	if (RwIm3DTransform(aImVertices, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
	{
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
		RwIm3DEnd(); 
	}

	RwRenderStateSet(rwRENDERSTATECULLMODE,	(void *)rwCULLMODECULLBACK);
}

void RenderGeometryBox(RwMatrix* pmat, const RwBBox* pBBox, RwRGBA* prgbaColorLine, RwRGBA* prgbaColorPlane, RwReal fOffset, RwBool bWire)
{
	static RwImVertexIndex indicesLine[24] =
	{
		0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
		7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
	};

	static RwImVertexIndex indicesTriangle[36] =
	{
		0, 2, 1, 1, 2, 3, 4, 6, 0, 0, 6, 2, 5, 7, 4, 4, 7, 6,
		1, 3, 5, 5, 3, 7, 4, 0, 5, 5, 0, 1, 2, 6, 3, 3, 6, 7
	};

	static RwIm3DVertex	im3DVertices[8];


	RwIm3DVertex* pVertex = im3DVertices;
	for(int i = 0; i < 8; ++i)
	{
		RwIm3DVertexSetPos(pVertex, 
			i & 1 ? pBBox->sup.x - fOffset: pBBox->inf.x + fOffset,
			i & 2 ? pBBox->sup.y - fOffset: pBBox->inf.y + fOffset,
			i & 4 ? pBBox->sup.z - fOffset: pBBox->inf.z + fOffset);

		RwIm3DVertexSetRGBA(pVertex, prgbaColorLine->red, prgbaColorLine->green, prgbaColorLine->blue, prgbaColorLine->alpha);
		pVertex++;
	}

	RwD3D9SetTexture(NULL, 0);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	if(RwIm3DTransform(im3DVertices, 8, pmat, rwIM3D_ALLOPAQUE))
	{
		RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indicesLine, 24);

		RwIm3DEnd();
	}
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);

	if (!bWire)
	{
		pVertex = im3DVertices;
		for(int i = 0; i < 8; ++i)
		{
			RwIm3DVertexSetRGBA(pVertex, prgbaColorPlane->red, prgbaColorPlane->green, prgbaColorPlane->blue, prgbaColorPlane->alpha);
			pVertex++;
		}
		RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
		if(RwIm3DTransform(im3DVertices, 8, pmat, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA))
		{
			RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, indicesTriangle, 36);
			RwIm3DEnd();
		}
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
		RwRenderStateSet(rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
	}
}