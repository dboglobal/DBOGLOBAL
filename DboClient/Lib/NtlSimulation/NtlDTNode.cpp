#include "precomp_ntlsimulation.h"
#include "NtlDTNode.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLGlobal.h"

// simulation
#include "NtlSob.h"
#include "NtlSLLogic.h"

CNtlDTNodeBase::CNtlDTNodeBase()
{
	m_bFinish	= FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

CNtlDTSplineNode::CNtlDTSplineNode()
{
	m_pProp			= NULL;
	m_pSobObj		= NULL;
	m_fCurrTime		= 0.0f;
	m_fPtTime		= 0.0f;	
	m_iPtIdx		= 0;

	m_eLookAtType	= DIRECT_LOOKAT_SELF;
}

CNtlDTSplineNode::~CNtlDTSplineNode()
{
}

void CNtlDTSplineNode::SetLookAtSelf(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos)
{
	RwMatrixTranslate(&m_matTrans, &vStartPos, rwCOMBINEPOSTCONCAT);	
}

void CNtlDTSplineNode::SetLookAtSelfAllAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos)
{
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vLookAtPos, &vStartPos);

	RwV3d vDir;
	RwV3dNormalize(&vDir, &vSub);

	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir);

	CNtlMath::MathRwMatrixAssign(&m_matTransRot, &CNtlPLGlobal::m_vZeroV3, &vDir);
}

void CNtlDTSplineNode::SetLookAtSelfYAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos)
{
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vLookAtPos, &vStartPos);

	vSub.y = 0.0f;

	RwV3d vDir;
	RwV3dNormalize(&vDir, &vSub);

	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir);
	CNtlMath::MathRwMatrixAssign(&m_matTransRot, &CNtlPLGlobal::m_vZeroV3, &vDir);
}

void CNtlDTSplineNode::SetLookAtSelfScaleAllAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos)
{
	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();
		
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vEndPos, &vStartPos);
	RwReal fDist = RwV3dLength(&vSub);

	RwV3dSubMacro(&vSub, &pCurvePt[iTotCurveCnt-1].vPos, &pCurvePt[0].vPos);
	RwReal fLen = RwV3dLength(&vSub);

	RwReal fScale = fDist/fLen;
	RwV3d vScale;
	CNtlMath::MathRwV3dAssign(&vScale, fScale, fScale, fScale);

	RwV3d vDir;
	RwV3dNormalize(&vDir, &vSub);
	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir, &vScale);
	
	CNtlMath::MathRwMatrixAssign(&m_matTransRot, &CNtlPLGlobal::m_vZeroV3, &vDir);
}

void CNtlDTSplineNode::SetLookAtSelfScaleYAxis(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos)
{
	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();
		
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vEndPos, &vStartPos);
	RwReal fDist = RwV3dLength(&vSub);

	RwV3dSubMacro(&vSub, &pCurvePt[iTotCurveCnt-1].vPos, &pCurvePt[0].vPos);
	RwReal fLen = RwV3dLength(&vSub);

	RwReal fScale = fDist/fLen;
	RwV3d vScale;
	CNtlMath::MathRwV3dAssign(&vScale, fScale, fScale, fScale);

	RwV3d vDir;
	RwV3dNormalize(&vDir, &vSub);
	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir, &vScale);

	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	CNtlMath::MathRwMatrixAssign(&m_matTransRot, &CNtlPLGlobal::m_vZeroV3, &vDir);
}

void CNtlDTSplineNode::SetLookAtDynamic(RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos)
{
	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();
		
	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vEndPos, &vStartPos);
	RwReal fDist = RwV3dLength(&vSub);

	RwV3dSubMacro(&vSub, &pCurvePt[iTotCurveCnt-1].vPos, &pCurvePt[0].vPos);
	RwReal fLen = RwV3dLength(&vSub);

	RwReal fScale = fDist/fLen;
	RwV3d vScale;
	CNtlMath::MathRwV3dAssign(&vScale, fScale, fScale, fScale);

	RwV3d vDir;
	RwV3dNormalize(&vDir, &vSub);
	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir, &vScale);

	m_vLookAtPos = vLookAtPos;
}


void CNtlDTSplineNode::SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos, EDirectLookAtType eLookAtType)
{
	CNtlDTProp *pDTProp = const_cast<CNtlDTProp*>( pProp ); 
	m_pProp				= reinterpret_cast<CNtlDTSpline*>( pDTProp );
	m_pSobObj			= const_cast<CNtlSob*>( pSobObj );

	RwMatrixSetIdentity(&m_matTrans);
	RwMatrixSetIdentity(&m_matTransRot);

	m_eLookAtType		= eLookAtType;

	switch(m_eLookAtType)
	{
	case DIRECT_LOOKAT_NOT_USE:
		break;
	case DIRECT_LOOKAT_SELF:
		SetLookAtSelf(vStartPos, vEndPos, vLookAtPos);
		break;
	case DIRECT_LOOKAT_SELF_ALLAXIS:
		SetLookAtSelfAllAxis(vStartPos, vEndPos, vLookAtPos);
		break;
	case DIRECT_LOOKAT_SELF_YAXIS:
		SetLookAtSelfYAxis(vStartPos, vEndPos, vLookAtPos);
		break;
	case DIRECT_LOOKAT_DYNAMIC:
		SetLookAtDynamic(vStartPos, vEndPos, vLookAtPos);
		break;
	}

	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();
	m_vPtPos = pCurvePt[0].vPos;

	SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(0);
	if(pCustomData && pCustomData->chSoundFile[0])
	{
		DBO_WARNING_MESSAGE("Play sound: " << pCustomData->chSoundFile);
		Logic_PlayGUISound(pCustomData->chSoundFile);
	}
}

void CNtlDTSplineNode::SetData(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos, RwV3d vTransDir)
{
	CNtlDTProp *pDTProp = const_cast<CNtlDTProp*>( pProp ); 
	m_pProp				= reinterpret_cast<CNtlDTSpline*>( pDTProp );
	m_pSobObj			= const_cast<CNtlSob*>( pSobObj );

	RwMatrixSetIdentity(&m_matTrans);
	RwMatrixSetIdentity(&m_matTransRot);

	m_eLookAtType = DIRECT_LOOKAT_NOT_USE;

	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vTransDir);

	SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(0);
	if(pCustomData && pCustomData->chSoundFile[0])
	{
		Logic_PlayGUISound(pCustomData->chSoundFile);
	}
}

void CNtlDTSplineNode::SetDataGroundMove(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos)
{
	CNtlDTProp *pDTProp = const_cast<CNtlDTProp*>( pProp ); 
	m_pProp				= reinterpret_cast<CNtlDTSpline*>( pDTProp );
	m_pSobObj			= const_cast<CNtlSob*>( pSobObj );

	RwMatrixSetIdentity(&m_matTrans);
	RwMatrixSetIdentity(&m_matTransRot);

	RwV3d vDir;
	CNtlMath::MathRwV3dAssign(&vDir, 0.0f, 0.0f, 1.0f);
	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir);

	m_eLookAtType = DIRECT_GROUND_MOVE;

	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();
	m_vPtPos = pCurvePt[0].vPos;

	SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(0);
	if(pCustomData && pCustomData->chSoundFile[0])
	{
		Logic_PlayGUISound(pCustomData->chSoundFile);
	}
}

void CNtlDTSplineNode::SetDataFlyMove(const CNtlDTProp *pProp, const CNtlSob *pSobObj, RwV3d vStartPos)
{
	CNtlDTProp *pDTProp = const_cast<CNtlDTProp*>( pProp ); 
	m_pProp				= reinterpret_cast<CNtlDTSpline*>( pDTProp );
	m_pSobObj			= const_cast<CNtlSob*>( pSobObj );

	RwMatrixSetIdentity(&m_matTrans); 
	RwMatrixSetIdentity(&m_matTransRot);

	RwV3d vDir;
	CNtlMath::MathRwV3dAssign(&vDir, 0.0f, 0.0f, 1.0f);
	CNtlMath::MathRwMatrixAssign(&m_matTrans, &vStartPos, &vDir);

	m_eLookAtType = DIRECT_FLY_MOVE;

	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();
	m_vPtPos = pCurvePt[0].vPos;

	SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(0);
	if(pCustomData && pCustomData->chSoundFile[0])
	{
		Logic_PlayGUISound(pCustomData->chSoundFile);
	}
}

void CNtlDTSplineNode::UniformTypeUpdate(RwReal fElapsed)
{
	RwReal fUniformSpeed = m_pProp->GetUniformSpeed();
	RwReal fULen = fUniformSpeed * fElapsed;

	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &pCurvePt[m_iPtIdx+1].vPos, &m_vPtPos);
	RwReal fCurrPtLen = RwV3dLength(&vSub);

	while(fULen > fCurrPtLen)
	{
		fULen -= fCurrPtLen;
		m_iPtIdx++;
		m_vPtPos = pCurvePt[m_iPtIdx].vPos;

		if(m_iPtIdx >= iTotCurveCnt - 1)
		{
			m_bFinish = TRUE;
			return;
		}

		RwV3dSubMacro(&vSub, &pCurvePt[m_iPtIdx+1].vPos, &m_vPtPos);
		fCurrPtLen = RwV3dLength(&vSub);

		SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(m_iPtIdx);
		if(pCustomData && pCustomData->chSoundFile[0])
		{
			Logic_PlayGUISound(pCustomData->chSoundFile);
		}
	}

	// position을 구한다.

	RwV3dSubMacro(&vSub, &m_vPtPos, &pCurvePt[m_iPtIdx].vPos);
	RwReal fNextLen = RwV3dLength(&vSub) + fULen;

	RwV3dSubMacro(&vSub, &pCurvePt[m_iPtIdx+1].vPos, &pCurvePt[m_iPtIdx].vPos);
	RwReal fDivLen = RwV3dLength(&vSub);

	RwV3d vPos;
	RwReal fLerp = 1.0f;

	if(fDivLen > 0.0f)
		fLerp = fNextLen  / fDivLen;
	
	RwInt32 iNextPtIdx = m_iPtIdx+1;

	vPos.x = pCurvePt[m_iPtIdx].vPos.x + fLerp * (pCurvePt[iNextPtIdx].vPos.x - pCurvePt[m_iPtIdx].vPos.x);
	vPos.y = pCurvePt[m_iPtIdx].vPos.y + fLerp * (pCurvePt[iNextPtIdx].vPos.y - pCurvePt[m_iPtIdx].vPos.y);
	vPos.z = pCurvePt[m_iPtIdx].vPos.z + fLerp * (pCurvePt[iNextPtIdx].vPos.z - pCurvePt[m_iPtIdx].vPos.z);

	m_vPtPos = vPos;

	// 방향을 구한다.
	RtQuat quat;
	D3DXQuaternionSlerp((D3DXQUATERNION*)&quat, (D3DXQUATERNION*)&pCurvePt[m_iPtIdx].qRot, (D3DXQUATERNION*)&pCurvePt[iNextPtIdx].qRot, fLerp);
	RtQuatConvertToMatrix(&quat, &m_matRot);

	const RwMatrix *pMat = m_pProp->GetTransMatrix();

	RwV3dTransformPoints(&vPos, &vPos, 1, pMat);
	RwV3dTransformPoints(&vPos, &vPos, 1, &m_matTrans);

	RwV3d vDir;
	RwV3dAssignMacro(&vDir, &m_matRot.at);

	if(m_eLookAtType == DIRECT_LOOKAT_SELF_ALLAXIS || m_eLookAtType == DIRECT_LOOKAT_SELF_YAXIS)
	{
		RwV3dTransformPoints(&vDir, &m_matRot.at, 1, &m_matTransRot);
	}
	else if(m_eLookAtType == DIRECT_LOOKAT_DYNAMIC)
	{
		RwV3dSubMacro(&vDir, &m_vLookAtPos, &vPos);
		RwV3dNormalize(&vDir, &vDir);
	}

	m_pSobObj->SetPosition(&vPos);

	if(m_eLookAtType != DIRECT_LOOKAT_NOT_USE)
		m_pSobObj->SetDirection(&vDir);
}

void CNtlDTSplineNode::TimeSequencetypeUpdate(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;
	m_fPtTime += fElapsed;
	
	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();

	while(m_fPtTime > pCurvePt[m_iPtIdx].fTime)
	{
		m_fPtTime -= pCurvePt[m_iPtIdx].fTime;
		m_iPtIdx++;

		if(m_iPtIdx >= iTotCurveCnt-1)
		{
			m_bFinish = TRUE;
			return;
		}

		SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(m_iPtIdx);
		if(pCustomData && pCustomData->chSoundFile[0])
		{
			Logic_PlayGUISound(pCustomData->chSoundFile);
		}
	};

	// position을 구한다.
	RwInt32 iNextPtIdx = m_iPtIdx+1;

	RwV3d vPos;
	RwReal fLerp = m_fPtTime  / pCurvePt[m_iPtIdx].fTime;

	if(fLerp < 0.0f)
		fLerp = 0.0f;

	if(fLerp > 1.0f)
		fLerp = 1.0f;

	vPos.x = pCurvePt[m_iPtIdx].vPos.x + fLerp * (pCurvePt[iNextPtIdx].vPos.x - pCurvePt[m_iPtIdx].vPos.x);
	vPos.y = pCurvePt[m_iPtIdx].vPos.y + fLerp * (pCurvePt[iNextPtIdx].vPos.y - pCurvePt[m_iPtIdx].vPos.y);
	vPos.z = pCurvePt[m_iPtIdx].vPos.z + fLerp * (pCurvePt[iNextPtIdx].vPos.z - pCurvePt[m_iPtIdx].vPos.z);

	// 방향을 구한다.
	RtQuat quat;
	D3DXQuaternionSlerp((D3DXQUATERNION*)&quat, (D3DXQUATERNION*)&pCurvePt[m_iPtIdx].qRot, (D3DXQUATERNION*)&pCurvePt[iNextPtIdx].qRot, fLerp);
	RtQuatConvertToMatrix(&quat, &m_matRot);

	const RwMatrix *pMat = m_pProp->GetTransMatrix();

	RwV3dTransformPoints(&vPos, &vPos, 1, pMat);
	RwV3dTransformPoints(&vPos, &vPos, 1, &m_matTrans);

	RwV3d vDir;
	RwV3dAssignMacro(&vDir, &m_matRot.at);

	if(m_eLookAtType == DIRECT_LOOKAT_SELF_ALLAXIS || m_eLookAtType == DIRECT_LOOKAT_SELF_YAXIS)
	{
		RwV3dTransformPoints(&vDir, &m_matRot.at, 1, &m_matTransRot);
	}
	else if(m_eLookAtType == DIRECT_LOOKAT_DYNAMIC)
	{
		RwV3dSubMacro(&vDir, &m_vLookAtPos, &vPos);
		RwV3dNormalize(&vDir, &vDir);
	}

	m_pSobObj->SetPosition(&vPos);

	if(m_eLookAtType != DIRECT_LOOKAT_NOT_USE)
		m_pSobObj->SetDirection(&vDir);
}

void CNtlDTSplineNode::GroundMoveUniformUpdate(RwReal fElapsed)
{
	RwReal fUniformSpeed = m_pProp->GetUniformSpeed();
	RwReal fULen = fUniformSpeed * fElapsed;

	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &pCurvePt[m_iPtIdx+1].vPos, &m_vPtPos);
	RwReal fCurrPtLen = RwV3dLength(&vSub);

	while(fULen > fCurrPtLen)
	{
		fULen -= fCurrPtLen;
		m_iPtIdx++;
		m_vPtPos = pCurvePt[m_iPtIdx].vPos;

		if(m_iPtIdx >= iTotCurveCnt - 1)
		{
			m_bFinish = TRUE;
			return;
		}

		RwV3dSubMacro(&vSub, &pCurvePt[m_iPtIdx+1].vPos, &m_vPtPos);
		fCurrPtLen = RwV3dLength(&vSub);

		SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(m_iPtIdx);
		if(pCustomData && pCustomData->chSoundFile[0])
		{
			Logic_PlayGUISound(pCustomData->chSoundFile);
		}
	}

	// position을 구한다.

	RwV3dSubMacro(&vSub, &m_vPtPos, &pCurvePt[m_iPtIdx].vPos);
	RwReal fNextLen = RwV3dLength(&vSub) + fULen;

	RwV3dSubMacro(&vSub, &pCurvePt[m_iPtIdx+1].vPos, &pCurvePt[m_iPtIdx].vPos);
	RwReal fDivLen = RwV3dLength(&vSub);

	RwV3d vPos;
	RwReal fLerp = 1.0f;

	if(fDivLen > 0.0f)
		fLerp = fNextLen  / fDivLen;
	
	RwInt32 iNextPtIdx = m_iPtIdx+1;

	vPos.x = pCurvePt[m_iPtIdx].vPos.x + fLerp * (pCurvePt[iNextPtIdx].vPos.x - pCurvePt[m_iPtIdx].vPos.x);
	vPos.y = pCurvePt[m_iPtIdx].vPos.y + fLerp * (pCurvePt[iNextPtIdx].vPos.y - pCurvePt[m_iPtIdx].vPos.y);
	vPos.z = pCurvePt[m_iPtIdx].vPos.z + fLerp * (pCurvePt[iNextPtIdx].vPos.z - pCurvePt[m_iPtIdx].vPos.z);

	m_vPtPos = vPos;

	// 방향을 구한다.
	RtQuat quat;
	D3DXQuaternionSlerp((D3DXQUATERNION*)&quat, (D3DXQUATERNION*)&pCurvePt[m_iPtIdx].qRot, (D3DXQUATERNION*)&pCurvePt[iNextPtIdx].qRot, fLerp);
	RtQuatConvertToMatrix(&quat, &m_matRot);

	const RwMatrix *pMat = m_pProp->GetTransMatrix();

	RwV3dTransformPoints(&vPos, &vPos, 1, pMat);
	RwV3dTransformPoints(&vPos, &vPos, 1, &m_matTrans);

	RwV3d vDir;
	RwV3dAssignMacro(&vDir, &m_matRot.at);

	RwV3dTransformPoints(&vDir, &m_matRot.at, 1, &m_matTransRot);

	if(m_eLookAtType == DIRECT_GROUND_MOVE)
	{
		SWorldHeightStuff sStuff;
		Logic_GetWorldHeight(&vPos, sStuff);

		vPos.y = sStuff.fFinialHeight;
	}

	m_pSobObj->SetPosition(&vPos);
	m_pSobObj->SetDirection(&vDir);
}

void CNtlDTSplineNode::GroundMoveTimeSequencetypeUpdate(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;
	m_fPtTime += fElapsed;
	
	RwInt32 iTotCurveCnt			= m_pProp->GetTotCurveCount();	
	const SSplineCurvePt *pCurvePt	= m_pProp->GetSplineCurvePoint();

	while(m_fPtTime > pCurvePt[m_iPtIdx].fTime)
	{
		m_fPtTime -= pCurvePt[m_iPtIdx].fTime;
		m_iPtIdx++;

		if(m_iPtIdx >= iTotCurveCnt - 1)
		{
			m_bFinish = TRUE;
			return;
		}

		SSplineCustomData *pCustomData = m_pProp->GetSplineCustomData(m_iPtIdx);
		if(pCustomData && pCustomData->chSoundFile[0])
		{
			Logic_PlayGUISound(pCustomData->chSoundFile);
		}
	};

	// position을 구한다.
	RwInt32 iNextPtIdx = m_iPtIdx+1;

	RwV3d vPos;
	RwReal fLerp = m_fPtTime  / pCurvePt[m_iPtIdx].fTime;

	if(fLerp < 0.0f)
		fLerp = 0.0f;

	if(fLerp > 1.0f)
		fLerp = 1.0f;

	vPos.x = pCurvePt[m_iPtIdx].vPos.x + fLerp * (pCurvePt[iNextPtIdx].vPos.x - pCurvePt[m_iPtIdx].vPos.x);
	vPos.y = pCurvePt[m_iPtIdx].vPos.y + fLerp * (pCurvePt[iNextPtIdx].vPos.y - pCurvePt[m_iPtIdx].vPos.y);
	vPos.z = pCurvePt[m_iPtIdx].vPos.z + fLerp * (pCurvePt[iNextPtIdx].vPos.z - pCurvePt[m_iPtIdx].vPos.z);

	// 방향을 구한다.
	RtQuat quat;
	D3DXQuaternionSlerp((D3DXQUATERNION*)&quat, (D3DXQUATERNION*)&pCurvePt[m_iPtIdx].qRot, (D3DXQUATERNION*)&pCurvePt[iNextPtIdx].qRot, fLerp);
	RtQuatConvertToMatrix(&quat, &m_matRot);

	const RwMatrix *pMat = m_pProp->GetTransMatrix();

	RwV3dTransformPoints(&vPos, &vPos, 1, pMat);
	RwV3dTransformPoints(&vPos, &vPos, 1, &m_matTrans);

	RwV3d vDir;
	RwV3dAssignMacro(&vDir, &m_matRot.at);

	RwV3dTransformPoints(&vDir, &m_matRot.at, 1, &m_matTransRot);

	if(m_eLookAtType == DIRECT_GROUND_MOVE)
	{
		SWorldHeightStuff sStuff;
		Logic_GetWorldHeight(&vPos, sStuff);

		vPos.y = sStuff.fFinialHeight;
	}

	m_pSobObj->SetPosition(&vPos);
	m_pSobObj->SetDirection(&vDir);
}


void CNtlDTSplineNode::Update(RwReal fElapsed)
{
	if(m_bFinish)
		return;

	if(m_eLookAtType == DIRECT_GROUND_MOVE || m_eLookAtType == DIRECT_FLY_MOVE)
	{
		RwBool bUniformLerp = m_pProp->IsUniformLerp();	
		if(bUniformLerp)
			GroundMoveUniformUpdate(fElapsed);
		else
			GroundMoveTimeSequencetypeUpdate(fElapsed);
	}
	else
	{
		RwBool bUniformLerp = m_pProp->IsUniformLerp();	
		if(bUniformLerp)
			UniformTypeUpdate(fElapsed);
		else
			TimeSequencetypeUpdate(fElapsed);
	}
}