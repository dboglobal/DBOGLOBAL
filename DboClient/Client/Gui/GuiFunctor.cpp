#include "precomp_dboclient.h"
#include "GuiFunctor.h"

// presentation
#include "NtlPLGlobal.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlCameraManager.h"

// gui
//#include "GuiUtil.h"
#include "position.h"

// dbo client
#include "DboGlobal.h"

RwBool CGuiFunctor::Calc3DPosTo2D(const RwV3d& v3DPos, RwReal fHeight, CPos* vOffset, CPos* pResPos )
{
	RwSphere sphere;
	sphere.center.x = v3DPos.x;
	sphere.center.y = v3DPos.y + fHeight;
	sphere.center.z = v3DPos.z;
	sphere.radius = 1.0f;
	
	if( RwCameraFrustumTestSphere( CNtlPLGlobal::m_RwCamera, &sphere ) == rwSPHEREOUTSIDE )
		return FALSE;

	RwV3d v2dPoint;
	RwMatrix* pViewMatrix = RwCameraGetViewMatrix( CNtlPLGlobal::m_RwCamera );
	RwV3dTransformPoints( &v2dPoint, &sphere.center, 1, pViewMatrix );
	v2dPoint.x = ( v2dPoint.x / v2dPoint.z ) * GetDboGlobal()->GetScreenWidth();
	v2dPoint.y = ( v2dPoint.y / v2dPoint.z ) * GetDboGlobal()->GetScreenHeight();

	pResPos->x = (RwInt32)( v2dPoint.x + 0.5f );
	pResPos->y = (RwInt32)( v2dPoint.y + 0.5f );

	if( vOffset )
	{
		// 후에 재정리.
		//RwV3d*	pvCamera;
		//RwV3d	vResult;
		//pvCamera = RwMatrixGetPos( RwFrameGetMatrix( RwCameraGetFrame( CNtlPLGlobal::m_RwCamera ) ) );			
		//RwV3dSubMacro( &vResult, &sphere.center, pvCamera );
		//RwReal	fDist = RwV3dLength( &vResult );		

		pResPos->x += vOffset->x;
		pResPos->y += vOffset->y;
	}

	return TRUE;
}

RwReal CGuiFunctor::GetDistanceFromAvatar(const RwV3d& vDestPos )
{
    return CNtlMath::GetLength(GetNtlSLGlobal()->GetSobAvatar()->GetPosition(), vDestPos);
}

RwReal CGuiFunctor::GetDistanceFromCamera(const RwV3d& vDestPos )
{
    return CNtlMath::GetLength(*GetNtlGameCameraManager()->GetCameraPos(), vDestPos);
}

RwUInt32 CGuiFunctor::ColorDecision( RwUInt32 uiBase, RwUInt32 uiLast, DECISION eDecision /* = BIG_IS_GOOD */, RwUInt32 uiBasicColor /* = BASIC_STAT_COLOR */, RwUInt32 uiGoodColor /* = GOOD_STAT_COLOR */, RwUInt32 uiBadColor /* = BAD_STAT_COLOR  */ )
{
	if( uiBase == uiLast ) 
		return uiBasicColor;
	else if( uiBase > uiLast ) 
	{
		if( eDecision == BIG_IS_GOOD )
			return uiBadColor;
		else
			return uiGoodColor;
	}
	else
	{
		if( eDecision == BIG_IS_GOOD )
			return uiGoodColor;
		else
			return uiBadColor;
	}
}

RwUInt32 CGuiFunctor::ColorDecisionF( RwReal fBase, RwReal fLast, DECISION eDecision /* = BIG_IS_GOOD */, RwUInt32 uiBasicColor /* = BASIC_STAT_COLOR */, RwUInt32 uiGoodColor /* = GOOD_STAT_COLOR */, RwUInt32 uiBadColor /* = BAD_STAT_COLOR  */ )
{
	if( fBase == fLast ) 
		return uiBasicColor;
	else if( fBase > fLast ) 
	{
		if( eDecision == BIG_IS_GOOD )
			return uiBadColor;
		else
			return uiGoodColor;
	}
	else
	{
		if( eDecision == BIG_IS_GOOD )
			return uiGoodColor;
		else
			return uiBadColor;
	}
}