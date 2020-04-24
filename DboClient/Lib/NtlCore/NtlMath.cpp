#include "precomp_ntlcore.h"
#include ".\ntlmath.h"


#define dfnMath_fEpsilon			(0.001f)
#define dfnMath_fMatchFactor		(0.999f)


CNtlPlane::CNtlPlane()
{
}

CNtlPlane::CNtlPlane(RwReal A, RwReal B, RwReal C, RwReal D)
{
	a = A; b = B; c = C; d = D;
}

CNtlPlane::CNtlPlane (const RwV3d& rkNormal, RwReal fConstant)
{
	n = rkNormal;
	d = fConstant;
}

void CNtlPlane::Normalize()
{
	// Normalize
	RwReal len = (RwReal) sqrtf(a * a + b * b + c * c);
	a /= len; b /= len; c /= len; d /= len;
}

void CNtlPlane::PlaneFromPoints(const RwV3d* pV1, const RwV3d* pV2, const RwV3d* pV3 )
{
	RwV3d vA, vB;
	RwV3dSubMacro(&vA, pV2, pV1);
	RwV3dSubMacro(&vB, pV3, pV2);
    	
    RwV3dCrossProduct(&n, &vA, &vB);
	RwV3dNormalize(&n, &n); 
    	
    d = - a * pV1->x - b * pV1->y - c * pV1->z;
}

void CNtlPlane::PlaneFromPointNormal(const RwV3d* pPoint, const RwV3d* pNormal)
{
    n = *pNormal;
	
    d = - a * pPoint->x - b * pPoint->y - c * pPoint->z;
}

RwReal CNtlPlane::DotProduct(RwV3d* pV)
{
    return pV->x * a + pV->y * b + pV->z * c + d;
}

RwReal CNtlPlane::DistanceTo (const RwV3d *pPoint) const
{
	return RwV3dDotProduct(&n, pPoint) - d;
}


RwV3d CNtlPlane::SplitPoint(const RwV3d *pFrom,const RwV3d *pAt)
{
	RwReal fFromDot = RwV3dDotProduct(pFrom, &n);
	RwReal fAtDot = RwV3dDotProduct(pAt, &n);

	RwReal fScale = (-d-fFromDot)/(fAtDot-fFromDot);

	RwV3d vDir;
	RwV3dSubMacro(&vDir, pAt, pFrom);
	RwV3dScale(&vDir, &vDir, fScale);

	RwV3d vSplitPos;
	RwV3dAdd(&vSplitPos, pFrom, &vDir);

	return vSplitPos;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// math

void CNtlMath::MathRwV2dAssign(RwV2d *pOut, RwReal fX, RwReal fY)
{
	pOut->x = fX;
	pOut->y = fY;
}

void CNtlMath::MathRwV2dAdd(RwV2d *pOut, RwReal fX, RwReal fY)
{
	pOut->x += fX;
	pOut->y += fY;
}

RwReal CNtlMath::MathRwV2dSquaredLength(const RwV2d *pIn)
{
	return pIn->x*pIn->x + pIn->y*pIn->y;
}

void CNtlMath::MathRwV3dAssign(RwV3d *pOut, RwReal fX, RwReal fY, RwReal fZ)
{
	pOut->x = fX;
	pOut->y = fY;
	pOut->z = fZ;
}

void CNtlMath::MathRwV3dAdd(RwV3d *pOut, RwReal fX, RwReal fY, RwReal fZ)
{
	pOut->x += fX;
	pOut->y += fY;
	pOut->z += fZ;
}

void CNtlMath::MathRwMatrixAssign(RwMatrix *pOut, const RwV3d *pPos, const RwV3d *pDir, const RwV3d *pScale)
{
	RwMatrixSetIdentity(pOut);
	
	RwV3d vYAxis;
	MathRwV3dAssign(&vYAxis, 0.0f, -1.0f, 0.0f);
	
	RwV3dAssign(&pOut->at, pDir);
	RwV3dCrossProduct(&pOut->right, pDir, &vYAxis);  
	RwV3dNormalize(&pOut->right, &pOut->right);
	RwV3dCrossProduct(&pOut->up, pDir, &pOut->right);
	RwV3dNormalize(&pOut->up, &pOut->up);
	
	if(pScale)
		RwMatrixScale(pOut, pScale, rwCOMBINEPOSTCONCAT);

	RwMatrixTranslate(pOut, pPos, rwCOMBINEPOSTCONCAT);
}

void CNtlMath::MathRwV3dAddMul(RwV3d *pOut, const RwV3d *pIn, RwReal fValue)
{
	pOut->x += pIn->x * fValue;
	pOut->y += pIn->y * fValue;
	pOut->z += pIn->z * fValue;
}

void CNtlMath::MathRwV3dSubMul(RwV3d *pOut, const RwV3d *pIn, RwReal fValue)
{
	pOut->x -= pIn->x * fValue;
	pOut->y -= pIn->y * fValue;
	pOut->z -= pIn->z * fValue;
}

RwReal CNtlMath::MathRwV3dSquaredLength(const RwV3d *pIn)
{
	return pIn->x*pIn->x + pIn->y*pIn->y + pIn->z*pIn->z;
}

/**
 * curPos와 desPos간의 각도를 구한다(degree)임
 */
RwReal CNtlMath::GetLineAngle(const RwV3d &curPos, const RwV3d &desPos)
{
	RwV2d tempSub = {0.f, 0.f};
	RwV2d tempMoveTargetPos;
	RwV2d tempMoveCurrentPos;
	RwV2d tempMoveDirection = {0.f, 1.f};

	tempMoveTargetPos.x  = desPos.x;
	tempMoveTargetPos.y  = desPos.z;
	tempMoveCurrentPos.x = curPos.x;
	tempMoveCurrentPos.y = curPos.z;
	RwV2dSub(&tempSub, &tempMoveTargetPos, &tempMoveCurrentPos);
	RwReal tempSubLength = RwV2dLength(&tempSub);
	
	RwReal radian = RwATan2(tempMoveDirection.x, tempMoveDirection.y);
	RwReal degree = (radian * 180.f)/ DBO_PI;

	return degree;
}

RwReal CNtlMath::LineToAngleY(const RwV3d *pLine)
{
	RwV2d vDir;
	CNtlMath::MathRwV2dAssign(&vDir, pLine->x, pLine->z);
	RwV2dNormalize(&vDir, &vDir);

	RwReal fRadian = atan2(vDir.x, vDir.y);
	return NTL_RAD2DEG(fRadian);
}

RwReal CNtlMath::LineToAngleX(const RwV3d *pLine)
{
	RwMatrix *pMat = RwMatrixCreate();
	RwMatrixSetIdentity(pMat);

	RwReal fAngleY = CNtlMath::LineToAngleY(pLine);
	if(fAngleY < 0.0f)
		fAngleY = fabs(fAngleY);
	else
		fAngleY = -fAngleY;

	RwMatrixRotate (pMat, &YAxis, fAngleY, rwCOMBINEPOSTCONCAT);
	RwMatrixUpdate(pMat);

	RwV3d v0;
	RwV3dTransformPoint(&v0, pLine, pMat);

	RwMatrixDestroy(pMat);
	
	return NTL_RAD2DEG(RwASin(-v0.y));
}

RwReal CNtlMath::LineToAngleX(const RwV3d *pLine, RwReal fAxisAngleY)
{
	RwMatrix *pMat = RwMatrixCreate();
	RwMatrixSetIdentity(pMat);

	RwReal fAngleY = fAxisAngleY;
	if(fAngleY < 0.0f)
		fAngleY = fabs(fAngleY);
	else
		fAngleY = -fAngleY;

	RwMatrixRotate (pMat, &YAxis, fAngleY, rwCOMBINEPOSTCONCAT);
	RwMatrixUpdate(pMat);

	RwV3d v0;
	RwV3dTransformPoint(&v0, pLine, pMat);

	RwMatrixDestroy(pMat);
	
	return NTL_RAD2DEG(RwASin(-v0.y));
}

void CNtlMath::LineToAngleXY(const RwV3d *pLine, RwReal& fAxisAngleX, RwReal& fAxisAngleY)
{
	fAxisAngleY = LineToAngleY(pLine);
	fAxisAngleX = LineToAngleX(pLine, fAxisAngleY);
}

/**
 * Matrix에서 Euler Angle 값을 얻어 오는 함수
 * 주의 사항: x축 각도는 -90 - 90 사이만 정상적으로 얻을 수 있다. (-90, 90 은 안됨)
 * 그리고 Rotation과 Translation만 적용된 Matrix에서만 현재 Test를 해봄
 */
void CNtlMath::MathGetMatrixEulerAngle(const RwMatrix *pMatrix, RwV3d *pvAngle)
{
	RwReal fEulerAngleX = 0.f;
	RwReal fEulerAngleY = 0.f;
	RwReal fEulerAngleZ = 0.f;
	
	//fEulerAngleX = (RwASin(pMatrix->up.z) * 180.f / DBO_PI); 
	fEulerAngleX = NTL_RAD2DEG(RwASin(pMatrix->up.z));

	if(fEulerAngleX < 90.f)
	{
		if(fEulerAngleX > -90.f)
		{
			fEulerAngleZ = RwATan2( -pMatrix->up.x, pMatrix->up.y);
			fEulerAngleY = RwATan2( -pMatrix->right.z, pMatrix->at.z);
		}
		else
		{
			fEulerAngleZ = -RwATan2( pMatrix->at.x, pMatrix->right.x);
			fEulerAngleY = 0.f;
		}
	}
	else
	{
		fEulerAngleZ = RwATan2(pMatrix->at.x, pMatrix->right.x);
		fEulerAngleY = 0.f;
	}
	
	
	fEulerAngleY = NTL_RAD2DEG(fEulerAngleY);
	fEulerAngleZ = NTL_RAD2DEG(fEulerAngleZ);

	//fEulerAngleY = fEulerAngleY * 180.f / DBO_PI;
	//fEulerAngleZ = fEulerAngleZ * 180.f / DBO_PI;
	
	pvAngle->x = fEulerAngleX;
	pvAngle->y = fEulerAngleY;
	pvAngle->z = fEulerAngleZ;
}
void CNtlMath::MathGetRotationMatrix(RwMatrix* pMatrix, const RwV3d* pvUnit_A, const RwV3d* pvUnit_B)
{
	RtQuat	quat;
	RwV3d	vCross;
	RwReal	f = (1.0f + RwV3dDotProduct(pvUnit_A, pvUnit_B)) * 2.0f; 
	
 	if (f <= 0.0f) // f가 0이 될 수 있으므로... 오류가 나올 수 있다.
	{
		f = 0.0001f;
	}

	f = sqrtf(f);
	RwV3dCrossProduct(&vCross, pvUnit_A, pvUnit_B);
	
	quat.imag.x	= vCross.x / f;
	quat.imag.y	= vCross.y / f;
	quat.imag.z	= vCross.z / f;
	quat.real	= f / 2.0f;

	RtQuatConvertToMatrix(&quat, pMatrix);

	/*RtQuat	quat;
	RwV3d	vCross;
	float	fRadian = acosf(RwV3dDotProduct(pvUnit_A, pvUnit_B));

	RwV3d vTemp = *pvUnit_A;
	if (DBO_PI - fRadian < 0.0005f)
	{
		vTemp.x += 0.0005f;
		vTemp.y += 0.0005f;
		vTemp.z += 0.0005f;
	}

	RwV3dCrossProduct(&vCross, &vTemp, pvUnit_B);
	RwV3dNormalize(&vCross, &vCross);

	float fTheta = sinf(fRadian / 2.0f);
	quat.imag.x	= vCross.x * fTheta;
	quat.imag.y	= vCross.y * fTheta;
	quat.imag.z	= vCross.z * fTheta;
	quat.real	= cosf(fRadian / 2.0f);

	RtQuatConvertToMatrix(&quat, pMatrix);*/
}

void CNtlMath::MathGetMatrix(RwMatrix *pMatrix, const RwV3d* pvScale, const RwV3d* pvRot, const RwV3d* pvPos)
{
	RwMatrix		matTemp;
	RwV3d			vFront, vLeft, vUp;

	static RwV3d	vYAxis = { 0.0f, 1.0f, 0.0f };
	static RwV3d	vXAxis = { 1.0f, 0.0f, 0.0f };
	static RwV3d	vZAxis = { 0.0f, 0.0f, 1.0f };

	RwMatrixRotate(&matTemp, &vYAxis, pvRot->y, rwCOMBINEREPLACE);

	RwV3dTransformVector(&vFront, &vZAxis, &matTemp);
	RwV3dTransformVector(&vLeft, &vXAxis, &matTemp);

	RwMatrixRotate(&matTemp, &vLeft, pvRot->x, rwCOMBINEREPLACE);

	RwV3dTransformVector(&vFront, &vFront, &matTemp);
	RwV3dTransformVector(&vUp, &vYAxis, &matTemp);

	RwMatrixScale(pMatrix, pvScale, rwCOMBINEREPLACE);
	RwMatrixRotate(pMatrix, &vYAxis, pvRot->y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pMatrix, &vLeft, pvRot->x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(pMatrix, &vFront, pvRot->z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(pMatrix, pvPos, rwCOMBINEPOSTCONCAT);
}

RwRGBA CNtlMath::Interpolation( const RwRGBA& startColor, const RwRGBA& endColor, const RwReal fDelta ) 
{
    RwRGBA color;

    color.red   = RwInt32FromRealMacro((RwReal)startColor.red + (fDelta * (RwReal)(endColor.red - startColor.red)));
    color.green = RwInt32FromRealMacro((RwReal)startColor.green + (fDelta * (RwReal)(endColor.green - startColor.green)));
    color.blue  = RwInt32FromRealMacro((RwReal)startColor.blue + (fDelta * (RwReal)(endColor.blue - startColor.blue)));
    color.alpha = RwInt32FromRealMacro((RwReal)startColor.alpha + (fDelta * (RwReal)(endColor.alpha - startColor.alpha)));

    return color;
}

RwReal CNtlMath::Interpolation(const RwReal fStart, const RwReal fEnd, const RwReal fDelta)
{
    return fStart + (fEnd - fStart) * fDelta;
}

RwV3d CNtlMath::Interpolation(const RwV3d& vStart, const RwV3d& vEnd, const RwReal fDelta)
{
    return vStart + (vEnd - vStart) * fDelta;
}

RwBool CNtlMath::LineSphereCollisionTestPixelRay(RwCamera *pCamera, RwSphere *pSphere, RwInt32 iPixelX, RwInt32 iPixelY, RwReal fRayDist, RwReal& fDistance)
{
	RwV2d vPixel = { (RwReal)iPixelX, (RwReal)iPixelY };
	RwLine Line;
	RwCameraCalcPixelRay( pCamera, &Line, &vPixel );

	RwV3d vRayDir;
	RwV3dSubMacro(&vRayDir, &Line.end, &Line.start);
	RwV3dNormalize(&vRayDir, &vRayDir);
	
	Line.end.x = Line.start.x + vRayDir.x*fRayDist;
	Line.end.y = Line.start.y + vRayDir.y*fRayDist;
	Line.end.z = Line.start.z + vRayDir.z*fRayDist;

	RwBool bResult = RtLineSphereIntersectionTest(&Line, pSphere, &fDistance);

	fDistance = fRayDist * fDistance;

	return bResult;
}

RwBool CNtlMath::BBoxBBoxCollision(const RwBBox *pBox1, const RwBBox *pBox2)
{
	if( ((pBox1)->inf.x <= (pBox2)->sup.x) && ((pBox1)->inf.y <= (pBox2)->sup.y) && ((pBox1)->inf.z <= (pBox2)->sup.z) && 
		((pBox1)->sup.x >= (pBox2)->inf.x) && ((pBox1)->sup.y >= (pBox2)->inf.y) && ((pBox1)->sup.z >= (pBox2)->inf.z) )
		return TRUE;

	return FALSE;
}

RwReal CNtlMath::Range( const RwReal fValue, const RwReal fMin, const RwReal fMax ) 
{
    RwReal fMinVal = min(fMin, fMax);
    RwReal fMaxVal = max(fMin, fMax);

    if(fValue > fMaxVal)
        return fMax;

    if(fValue < fMinVal)
        return fMin;

    return fValue;
}

RwReal CNtlMath::ScaleFromRwMatrix( const RwMatrix& matrix ) 
{
    return (matrix.right.x + matrix.up.y + matrix.at.z) / 3.0f;
}

RwV3d CNtlMath::RwV3dInverse( const RwV3d& v3d ) 
{
    RwV3d vInverse;
    vInverse.x = 1 / v3d.x;
    vInverse.y = 1 / v3d.y;
    vInverse.z = 1 / v3d.z;

    return vInverse;
}

RwReal CNtlMath::GetLength( const RwV3d& v1, const RwV3d& v2 ) 
{
    RwV3d vTemp = v1 - v2;
    return RwV3dLength(&vTemp);
}

RwReal CNtlMath::GetLength( const RwV2d& v1, const RwV2d& v2 ) 
{
    RwV2d vTemp = v1 - v2;
    return RwV2dLength(&vTemp);
}

/// 3D 벡터간의 더하기
const RwV3d operator+(const RwV3d& lhs, const RwV3d& rhs)
{
    RwV3d v3Return;
    v3Return.x = lhs.x + rhs.x;
    v3Return.y = lhs.y + rhs.y;
    v3Return.z = lhs.z + rhs.z;

    return v3Return;
}

/// 3D 벡터간의 뺴기
const RwV3d operator-(const RwV3d& lhs, const RwV3d& rhs)
{
    RwV3d v3Return;
    v3Return.x = lhs.x - rhs.x;
    v3Return.y = lhs.y - rhs.y;
    v3Return.z = lhs.z - rhs.z;

    return v3Return;
}

/// 3D 벡터 * 스칼라값
const RwV3d operator*(const RwV3d& lhs, const RwReal& rhs)
{
    RwV3d v3Return;
    v3Return.x = lhs.x * rhs;
    v3Return.y = lhs.y * rhs;
    v3Return.z = lhs.z * rhs;

    return v3Return;
}
/// 3D 벡터 *= 스칼라값
const void operator*=( RwV3d& lhs, const RwReal& rhs ) 
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
}

const RwV3d operator*( const RwReal& lhs, const RwV3d& rhs ) 
{
    return operator*(rhs, lhs);
}

const RwMatrix operator*( const RwMatrix& lhs, const RwMatrix& rhs ) 
{
	RwMatrix matResult;
	RwMatrixMultiply(&matResult, &lhs, &rhs);
	return matResult;
}

/// 3D 벡터 / 스칼라값
const RwV3d operator/(const RwV3d& lhs, const RwReal& rhs)
{
    assert(rhs != 0);

    RwV3d v3Return;
    v3Return.x = lhs.x / rhs;
    v3Return.y = lhs.y / rhs;
    v3Return.z = lhs.z / rhs;

    return v3Return;
}

const RwV3d operator/(const RwReal& lhs, const RwV3d& rhs)
{
    return operator/(rhs, lhs);
}


/// 3D 벡터 += 연산
const void operator+=(RwV3d& lhs, const RwV3d& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
}

/// 3D벡터 -= 연산
const void operator-=(RwV3d& lhs, const RwV3d& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
}

/// 3D벡터 == 연산
const RwBool operator==(const RwV3d& lhs, const RwV3d& rhs)
{
	if( lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z )
		return true;

	return false;
}

/// 3D벡터 != 연산
const RwBool operator!=(const RwV3d& lhs, const RwV3d& rhs)
{
	if( lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z )
		return false;

	return true;
}

const RwV2d operator+( const RwV2d& lhs, const RwV2d& rhs ) 
{
    RwV2d vTemp;
    RwV2dAdd(&vTemp, &lhs, &rhs);
    return vTemp;
}

const RwV2d operator-(const RwV2d& lhs, const RwV2d& rhs)
{
    RwV2d vTemp;
    RwV2dSub(&vTemp, &lhs, &rhs);
    return vTemp;
}

const RwV2d operator*(const RwV2d& lhs, const RwReal& rhs)
{
	RwV2d v2dReturn;	
    RwV2dScale(&v2dReturn, &lhs, rhs);
	return v2dReturn;
}

const RwV2d operator/( const RwV2d& lhs, const RwReal& rhs ) 
{
    RwV2d v2dReturn;
    RwV2dScale(&v2dReturn, &lhs, 1 / rhs);
    return v2dReturn;
}

const void operator*=(RwV2d& lhs, const RwReal& rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
}

const RwV3d operator-( const RwV3d& rhs ) 
{
    return ZeroAxis - rhs;
}

CNtlOBB::CNtlOBB()
{
}

CNtlOBB::~CNtlOBB()
{
}

CNtlOBB::CNtlOBB(RwMatrix* pMatrix, RwBBox* pAABB)
{
	CalcOBBData(this, pMatrix, pAABB);
// 	RwV3d vPos	= *RwMatrixGetPos(pMatrix);
// 
// 	RwV3d vSup = vPos + pAABB->sup;
// 	RwV3d vInf = vPos + pAABB->inf;
// 
// 	vPos.x = (vSup.x + vInf.x) * 0.5f;
// 	vPos.y = (vSup.y + vInf.y) * 0.5f;
// 	vPos.z = (vSup.z + vInf.z) * 0.5f;
// 
// 	vSup.x = vSup.x - vPos.x;
// 	vSup.y = vSup.y - vPos.y;
// 	vSup.z = vSup.z - vPos.z;
// 
// 	vInf.x = vInf.x - vPos.x;
// 	vInf.y = vInf.y - vPos.y;
// 	vInf.z = vInf.z - vPos.z;
// 
// 	vCenterPos	= vPos;
// 
// 	vAxisDir[0] = *RwMatrixGetRight(pMatrix);
// 	vAxisDir[1] = *RwMatrixGetUp(pMatrix);
// 	vAxisDir[2] = *RwMatrixGetAt(pMatrix);
// 
// 	fAxisLen[0] = vSup.x * RwV3dLength(&vAxisDir[0]);
// 	fAxisLen[1] = vSup.y * RwV3dLength(&vAxisDir[1]);
// 	fAxisLen[2] = vSup.z * RwV3dLength(&vAxisDir[2]);
// 
// 	RwV3dNormalize(&vAxisDir[0], &vAxisDir[0]);
// 	RwV3dNormalize(&vAxisDir[1], &vAxisDir[1]);
// 	RwV3dNormalize(&vAxisDir[2], &vAxisDir[2]);	
}

void CNtlOBB::CalcOBBData(CNtlOBB* pOBB, RwMatrix* pMatrix, RwBBox* pAABB)
{
	RwV3d vPos	= *RwMatrixGetPos(pMatrix);

	RwV3d vSup = vPos + pAABB->sup;
	RwV3d vInf = vPos + pAABB->inf;

	vPos.x = (vSup.x + vInf.x) * 0.5f;
	vPos.y = (vSup.y + vInf.y) * 0.5f;
	vPos.z = (vSup.z + vInf.z) * 0.5f;

	vSup.x = vSup.x - vPos.x;
	vSup.y = vSup.y - vPos.y;
	vSup.z = vSup.z - vPos.z;

	vInf.x = vInf.x - vPos.x;
	vInf.y = vInf.y - vPos.y;
	vInf.z = vInf.z - vPos.z;

	pOBB->vCenterPos	= vPos;

	pOBB->vAxisDir[0] = *RwMatrixGetRight(pMatrix);
	pOBB->vAxisDir[1] = *RwMatrixGetUp(pMatrix);
	pOBB->vAxisDir[2] = *RwMatrixGetAt(pMatrix);

	pOBB->fAxisLen[0] = vSup.x * RwV3dLength(&pOBB->vAxisDir[0]);
	pOBB->fAxisLen[1] = vSup.y * RwV3dLength(&pOBB->vAxisDir[1]);
	pOBB->fAxisLen[2] = vSup.z * RwV3dLength(&pOBB->vAxisDir[2]);

	RwV3dNormalize(&pOBB->vAxisDir[0], &pOBB->vAxisDir[0]);
	RwV3dNormalize(&pOBB->vAxisDir[1], &pOBB->vAxisDir[1]);
	RwV3dNormalize(&pOBB->vAxisDir[2], &pOBB->vAxisDir[2]);	
}

RwBool CNtlOBB::IntersectionOBBvsOBB(CNtlOBB* pA, CNtlOBB* pB) // Game Physics P.342
{
	double c[3][3];
	double absC[3][3];
	double d[3];

	double r0, r1, r;
	int i;

	const double cutoff = 0.999999;
	bool existsParallelPair = false;

	RwV3d diff = pA->vCenterPos - pB->vCenterPos;



	for( i = 0 ; i < 3 ; ++i )
	{
		c[0][i] = RwV3dDotProduct( &pA->vAxisDir[0], &pB->vAxisDir[i] );
		absC[0][i] = abs( c[0][i] );
		if( absC[0][i] > cutoff )
			existsParallelPair = true;
	}
	d[0] = RwV3dDotProduct( &diff, &pA->vAxisDir[0] );
	r = abs( d[0] );
	r0 = pA->fAxisLen[0];
	r1 = pB->fAxisLen[0] * absC[0][0] + pB->fAxisLen[1] * absC[0][1] + pB->fAxisLen[2] * absC[0][2];

	if( r > r0 + r1 )
		return FALSE;



	for( i = 0 ; i < 3 ; ++i )
	{
		c[1][i] = RwV3dDotProduct( &pA->vAxisDir[1], &pB->vAxisDir[i] );
		absC[1][i] = abs( c[1][i] );
		if( absC[1][i] > cutoff )
			existsParallelPair = true;
	}
	d[1] = RwV3dDotProduct( &diff, &pA->vAxisDir[1] );
	r = abs( d[1] );
	r0 = pA->fAxisLen[1];
	r1 = pB->fAxisLen[0] * absC[1][0] + pB->fAxisLen[1] * absC[1][1] + pB->fAxisLen[2] * absC[1][2];

	if( r > r0 + r1 )
		return FALSE;



	for( i = 0 ; i < 3 ; ++i )
	{
		c[2][i] = RwV3dDotProduct( &pA->vAxisDir[2], &pB->vAxisDir[i] );
		absC[2][i] = abs( c[2][i] );
		if( absC[2][i] > cutoff )
			existsParallelPair = true;
	}
	d[2] = RwV3dDotProduct( &diff, &pA->vAxisDir[2] );
	r = abs( d[2] );
	r0 = pA->fAxisLen[2];
	r1 = pB->fAxisLen[0] * absC[2][0] + pB->fAxisLen[1] * absC[2][1] + pB->fAxisLen[2] * absC[2][2];

	if( r > r0 + r1 )
		return FALSE;



	r = abs( RwV3dDotProduct( &diff, &pB->vAxisDir[0] ) );
	r0 = pA->fAxisLen[0] * absC[0][0] + pA->fAxisLen[1] * absC[1][0] + pA->fAxisLen[2] * absC[2][0];
	r1 = pB->fAxisLen[0];

	if( r > r0 + r1 )
		return FALSE;



	r = abs( RwV3dDotProduct( &diff, &pB->vAxisDir[1] ) );
	r0 = pA->fAxisLen[0] * absC[0][1] + pA->fAxisLen[1] * absC[1][1] + pA->fAxisLen[2] * absC[2][1];
	r1 = pB->fAxisLen[1];

	if( r > r0 + r1 )
		return FALSE;



	r = abs( RwV3dDotProduct( &diff, &pB->vAxisDir[2] ) );
	r0 = pA->fAxisLen[0] * absC[0][2] + pA->fAxisLen[1] * absC[1][2] + pA->fAxisLen[2] * absC[2][2];
	r1 = pB->fAxisLen[2];

	if( r > r0 + r1 )
		return FALSE;



	if( existsParallelPair == true )
		return TRUE;



	r = abs( d[2]*c[1][0] - d[1]*c[2][0] );
	r0 = pA->fAxisLen[1] * absC[2][0] + pA->fAxisLen[2] * absC[1][0];
	r1 = pB->fAxisLen[1] * absC[0][2] + pB->fAxisLen[2] * absC[0][1];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[2]*c[1][1] - d[1]*c[2][1] );
	r0 = pA->fAxisLen[1] * absC[2][1] + pA->fAxisLen[2] * absC[1][1];
	r1 = pB->fAxisLen[0] * absC[0][2] + pB->fAxisLen[2] * absC[0][0];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[2]*c[1][2] - d[1]*c[2][2] );
	r0 = pA->fAxisLen[1] * absC[2][2] + pA->fAxisLen[2] * absC[1][2];
	r1 = pB->fAxisLen[0] * absC[0][1] + pB->fAxisLen[1] * absC[0][0];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[0]*c[2][0] - d[2]*c[0][0] );
	r0 = pA->fAxisLen[0] * absC[2][0] + pA->fAxisLen[2] * absC[0][0];
	r1 = pB->fAxisLen[1] * absC[1][2] + pB->fAxisLen[2] * absC[1][1];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[0]*c[2][1] - d[2]*c[0][1] );
	r0 = pA->fAxisLen[0] * absC[2][1] + pA->fAxisLen[2] * absC[0][1];
	r1 = pB->fAxisLen[0] * absC[1][2] + pB->fAxisLen[2] * absC[1][0];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[0]*c[2][2] - d[2]*c[0][2] );
	r0 = pA->fAxisLen[0] * absC[2][2] + pA->fAxisLen[2] * absC[0][2];
	r1 = pB->fAxisLen[0] * absC[1][1] + pB->fAxisLen[1] * absC[1][0];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[1]*c[0][0] - d[0]*c[1][0] );
	r0 = pA->fAxisLen[0] * absC[1][0] + pA->fAxisLen[1] * absC[0][0];
	r1 = pB->fAxisLen[1] * absC[2][2] + pB->fAxisLen[2] * absC[2][1];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[1]*c[0][1] - d[0]*c[1][1] );
	r0 = pA->fAxisLen[0] * absC[1][1] + pA->fAxisLen[1] * absC[0][1];
	r1 = pB->fAxisLen[0] * absC[2][2] + pB->fAxisLen[2] * absC[2][0];
	if( r > r0 + r1 )
		return FALSE;



	r = abs( d[1]*c[0][2] - d[0]*c[1][2] );
	r0 = pA->fAxisLen[0] * absC[1][2] + pA->fAxisLen[1] * absC[0][2];
	r1 = pB->fAxisLen[0] * absC[2][1] + pB->fAxisLen[1] * absC[2][0];
	if( r > r0 + r1 )
		return FALSE;

	return TRUE;
}

RwInt32 CNtlMath::GetSafeIdx3D(RwV3d& vPos, RwInt32 iHalfSize, RwInt32 iChunkSize, RwInt32 iChunkNum)
{	
	// float 유효수의 문제로 0.9995 같은 경우 올림으로 1이 되어 버리는 현상으로 Idx가 잘못 계산 되었다.
	// 이런 문제를 해결하기 위해서 floor를 사용하여 소수부를 내림하여 해결하였다.
	RwReal fX = floor(vPos.x);
	RwReal fZ = floor(vPos.z);

	RwReal	fXBasedOn0	= fX + (RwReal)iHalfSize;
	RwReal	fZBasedOn0	= fZ + (RwReal)iHalfSize;
	RwInt32 iWidthNum	= (RwInt32)(fXBasedOn0 / (RwReal)iChunkSize);
	RwInt32 iDepthNum	= (RwInt32)(fZBasedOn0 / (RwReal)iChunkSize);

	return (iDepthNum * iChunkNum) + iWidthNum;
}