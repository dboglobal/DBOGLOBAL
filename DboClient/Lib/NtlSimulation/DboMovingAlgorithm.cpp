#include "precomp_ntlsimulation.h"
#include "DboMovingAlgorithm.h"

#include "NtlMath.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


RwBool CDboDeadReckonig::Refresh( RwV3d& vClientDir,
								  RwV3d& vClientPos,
								  RwReal fClientSpeed,
								  RwV3d& vServerDir,
								  RwV3d& vServerPos,
								  RwReal fServerSpeed,
								  RwReal fAccelator,
								  RwReal fLatencyTime )
{
	if ( !m_defPosList.empty() )
	{
		m_defPosList.clear();
	}
	else
	{
		m_defPosList.reserve( 4 );
	}

	m_defPosList.push_back( vClientPos );
	m_defPosList.push_back( vClientPos + vClientDir * fClientSpeed );
	m_defPosList.push_back( vServerPos + vServerDir * fServerSpeed * fLatencyTime + ( vServerDir * fAccelator * fLatencyTime * fLatencyTime ) / 2.f );
	m_defPosList.push_back( m_defPosList[2] - ( vServerDir * fServerSpeed + vServerDir * fAccelator * fLatencyTime ) );

	return TRUE;
}

RwBool CDboDeadReckonig::Refresh( RwV3d& vPos0,
								  RwV3d& vPos1,
								  RwV3d& vPos2,
								  RwV3d& vPos3 )
{
	if ( !m_defPosList.empty() )
	{
		m_defPosList.clear();
	}
	else
	{
		m_defPosList.reserve( 4 );
	}

	m_defPosList.push_back( vPos0 );
	m_defPosList.push_back( vPos1 );
	m_defPosList.push_back( vPos2 );
	m_defPosList.push_back( vPos3 );

	return TRUE;
}

RwReal CDboDeadReckonig::GetMaxDistance( void )
{
	RwReal fA1 = m_defPosList[3].x	-	3 * m_defPosList[2].x	+	3 * m_defPosList[1].x	-	m_defPosList[0].x		;
	RwReal fB1 =						3 * m_defPosList[2].x	-	6 * m_defPosList[1].x	+	3 * m_defPosList[0].x	;
	RwReal fC1 =												+	3 * m_defPosList[1].x	-	3 * m_defPosList[0].x	;
	RwReal fD1 =																				m_defPosList[0].x		;

	RwReal fA2 = m_defPosList[3].z	-	3 * m_defPosList[2].z	+	3 * m_defPosList[1].z	-	m_defPosList[0].z		;
	RwReal fB2 =						3 * m_defPosList[2].z	-	6 * m_defPosList[1].z	+	3 * m_defPosList[0].z	;
	RwReal fC2 =												+	3 * m_defPosList[1].z	-	3 * m_defPosList[0].z	;
	RwReal fD2 =																				m_defPosList[0].z		;

	RwReal fMaxDist = sqrtf( (9.f / 5.f) * (fA1 * fA1 + fA2 * fA2) +
							 (12.f / 4.f) * (fA1 * fB1 + fA2 * fB2) +
							 (1.f / 3.f) * (6.f * fA1 * fC1 + 4.f * fB1 * fB1 + 6.f * fA2 * fC2 + 4.f * fB2 * fB2) +
							 (4.f / 2.f) * (fB1 * fC1 + fB2 * fC2) +
							 (fC1 * fC1 + fC2 * fC2) );

	return fMaxDist;
}

void CDboDeadReckonig::GetDirAndPos( RwReal fTimeRatio, RwV3d& vDir, RwV3d& vPos )
{
	vDir.x = GetDirX( fTimeRatio );
	vDir.z = GetDirZ( fTimeRatio );
	vDir.y = 0.f;

	RwV3dNormalize( &vDir, &vDir );

	vPos.x = GetPosX( fTimeRatio );
	vPos.z = GetPosZ( fTimeRatio );
}

RwReal CDboDeadReckonig::GetDirX( RwReal fTimeRatio )
{
	RwReal fA = m_defPosList[3].x	-	3 * m_defPosList[2].x	+	3 * m_defPosList[1].x	-	m_defPosList[0].x		;
	RwReal fB =							3 * m_defPosList[2].x	-	6 * m_defPosList[1].x	+	3 * m_defPosList[0].x	;
	RwReal fC =													+	3 * m_defPosList[1].x	-	3 * m_defPosList[0].x	;
	RwReal fD =																					m_defPosList[0].x		;

	return 3.f * fA * (fTimeRatio * fTimeRatio) + 2 * fB * (fTimeRatio) + fC;
}

RwReal CDboDeadReckonig::GetDirZ( RwReal fTimeRatio )
{
	RwReal fA = m_defPosList[3].z	-	3 * m_defPosList[2].z	+	3 * m_defPosList[1].z	-	m_defPosList[0].z		;
	RwReal fB =							3 * m_defPosList[2].z	-	6 * m_defPosList[1].z	+	3 * m_defPosList[0].z	;
	RwReal fC =													+	3 * m_defPosList[1].z	-	3 * m_defPosList[0].z	;
	RwReal fD =																					m_defPosList[0].z		;

	return 3.f * fA * (fTimeRatio * fTimeRatio) + 2 * fB * (fTimeRatio) + fC;
}

RwReal CDboDeadReckonig::GetPosX( RwReal fTimeRatio )
{
	RwReal fA = m_defPosList[3].x	-	3 * m_defPosList[2].x	+	3 * m_defPosList[1].x	-	m_defPosList[0].x		;
	RwReal fB =							3 * m_defPosList[2].x	-	6 * m_defPosList[1].x	+	3 * m_defPosList[0].x	;
	RwReal fC =													+	3 * m_defPosList[1].x	-	3 * m_defPosList[0].x	;
	RwReal fD =																					m_defPosList[0].x		;

	return ( fA * (fTimeRatio * fTimeRatio * fTimeRatio) + fB * (fTimeRatio * fTimeRatio) + fC * ( fTimeRatio ) + fD );
}

RwReal CDboDeadReckonig::GetPosZ( RwReal fTimeRatio )
{
	RwReal fA = m_defPosList[3].z	-	3 * m_defPosList[2].z	+	3 * m_defPosList[1].z	-	m_defPosList[0].z		;
	RwReal fB =							3 * m_defPosList[2].z	-	6 * m_defPosList[1].z	+	3 * m_defPosList[0].z	;
	RwReal fC =													+	3 * m_defPosList[1].z	-	3 * m_defPosList[0].z	;
	RwReal fD =																					m_defPosList[0].z		;

	return ( fA * (fTimeRatio * fTimeRatio * fTimeRatio) + fB * (fTimeRatio * fTimeRatio) + fC * ( fTimeRatio ) + fD );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


CDboHermiteMoveSpline::CDboHermiteMoveSpline( void )
{
	m_matHermite._11 =  2.f;	m_matHermite._12 = -2.f;	m_matHermite._13 =  1.f;	m_matHermite._14 =  1.f;
	m_matHermite._21 = -3.f;	m_matHermite._22 =  3.f;	m_matHermite._23 = -2.f;	m_matHermite._24 = -1.f;
	m_matHermite._31 =  0.f;	m_matHermite._32 =  0.f;	m_matHermite._33 =  1.f;	m_matHermite._34 =  0.f;
	m_matHermite._41 =  1.f;	m_matHermite._42 =  0.f;	m_matHermite._43 =  0.f;	m_matHermite._44 =  0.f;

	m_defNodeList.reserve( 255 );

	m_fMaxDist = 0.f;
}

void CDboHermiteMoveSpline::Init( void )
{
	m_defNodeList.clear();

	m_fMaxDist = 0.f;
}

void CDboHermiteMoveSpline::AddNodePos( RwV3d* pvPos )
{
	sSPLINE_DATA sSlplineData;

	sSlplineData.vPos.x = pvPos->x;
	sSlplineData.vPos.y = pvPos->y;
	sSlplineData.vPos.z = pvPos->z;

	sSlplineData.vVel.x = 0.f;
	sSlplineData.vVel.y = 0.f;
	sSlplineData.vVel.z = 0.f;

	sSlplineData.fDist = 0.f;

	if ( m_defNodeList.empty() )
	{
		m_defNodeList.push_back( sSlplineData );

		m_fMaxDist = 0.f;
	}
	else
	{
		int nSize = (int)m_defNodeList.size();

		D3DXVECTOR3 vDistLen;
		vDistLen.x = m_defNodeList[nSize - 1].vPos.x - pvPos->x;
		vDistLen.y = m_defNodeList[nSize - 1].vPos.y - pvPos->y;
		vDistLen.z = m_defNodeList[nSize - 1].vPos.z - pvPos->z;

		RwReal fDistLen = D3DXVec3Length( &vDistLen );

		m_defNodeList[nSize - 1].fDist = fDistLen;
		m_fMaxDist += m_defNodeList[nSize - 1].fDist;

		m_defNodeList.push_back( sSlplineData );
	}
}

RwBool CDboHermiteMoveSpline::BuildSpline( void )
{
	int nSize = (int)m_defNodeList.size();

	if ( nSize < eBUILD_MIN_NODE_SIZE )
	{
		return FALSE;
	}

	for ( int i = 1; i < nSize - 1; ++i )
	{
		D3DXVECTOR3 vDistDir1 = m_defNodeList[i + 1].vPos - m_defNodeList[i].vPos;
		D3DXVec3Normalize( &vDistDir1, &vDistDir1 );

		D3DXVECTOR3 vDistDir2 = m_defNodeList[i - 1].vPos - m_defNodeList[i].vPos;
		D3DXVec3Normalize( &vDistDir2, &vDistDir2 );

		m_defNodeList[i].vVel = vDistDir1 - vDistDir2;
		D3DXVec3Normalize( &m_defNodeList[i].vVel, &m_defNodeList[i].vVel );
	}

	m_defNodeList[0].vVel = GetStartVelocity( 0 );
	m_defNodeList[nSize - 1].vVel = GetEndVelocity( nSize - 1 );

	return TRUE;
}

RwReal CDboHermiteMoveSpline::GetMaxDistance( void )
{
	return m_fMaxDist;
}

RwBool CDboHermiteMoveSpline::GetMoveDistRatio( RwReal fTime, RwReal& fMoveDistRatio )
{
	int nSize = (int)m_defNodeList.size();

	if ( nSize < eBUILD_MIN_NODE_SIZE )
	{
		fMoveDistRatio = 0.f;

		return FALSE;
	}

	RwReal fDist = fTime * m_fMaxDist;
	RwReal fCurDist = 0.f;

	RwInt32 i = 0;
	while ( fCurDist + m_defNodeList[i].fDist < fDist && i < nSize - 2 )
	{
		fCurDist += m_defNodeList[i].fDist;
		++i;
	}

	RwReal t = (fDist - fCurDist) / m_defNodeList[i].fDist;

	if ( t < 0.00001f ) t = 0.f;
	if ( t > 0.99999f ) t = 1.f;

	fMoveDistRatio = t;

	return TRUE;
}

RwBool CDboHermiteMoveSpline::GetPosAndDir( RwReal fTime, RwV3d& vDir, RwV3d& vPos )
{
	int nSize = (int)m_defNodeList.size();

	if ( nSize < eBUILD_MIN_NODE_SIZE )
	{
		return FALSE;
	}

	RwReal fDist = fTime * m_fMaxDist;
	RwReal fCurDist = 0.f;

	RwInt32 i = 0;
	while ( fCurDist + m_defNodeList[i].fDist < fDist && i < nSize - 2 )
	{
		fCurDist += m_defNodeList[i].fDist;
		++i;
	}

	RwReal t = (fDist - fCurDist) / m_defNodeList[i].fDist;

	if ( t < 0.00001f ) t = 0.f;
	if ( t > 0.99999f ) t = 1.f;

	D3DXVECTOR3 vStartVel = m_defNodeList[i].vVel * m_defNodeList[i].fDist;
	D3DXVECTOR3 vEndVel = m_defNodeList[i+1].vVel * m_defNodeList[i].fDist;

	vDir = GetDirectionOnCubic( m_defNodeList[i].vPos, vStartVel, m_defNodeList[i+1].vPos, vEndVel, t );
	RwV3dNormalize( &vDir, &vDir );

	vPos = GetPositionOnCubic( m_defNodeList[i].vPos, vStartVel, m_defNodeList[i+1].vPos, vEndVel, t );

	return TRUE;
}

D3DXVECTOR3 CDboHermiteMoveSpline::GetStartVelocity( int nIndex )
{
	D3DXVECTOR3 vTemp = 3.f * ( m_defNodeList[nIndex + 1].vPos - m_defNodeList[nIndex].vPos ) / m_defNodeList[nIndex].fDist;
	return ( vTemp - m_defNodeList[nIndex + 1].vVel ) * 0.5f;
}

D3DXVECTOR3 CDboHermiteMoveSpline::GetEndVelocity( int nIndex )
{
	D3DXVECTOR3 vTemp = 3.f * ( m_defNodeList[nIndex].vPos - m_defNodeList[nIndex - 1].vPos ) / m_defNodeList[nIndex - 1].fDist;
	return ( vTemp - m_defNodeList[nIndex - 1].vVel ) * 0.5f;
}

RwV3d CDboHermiteMoveSpline::GetDirectionOnCubic( const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vStartVel, const D3DXVECTOR3& vEndPos, const D3DXVECTOR3& vEndVel, RwReal fTime )
{
	D3DXMATRIX mat;

	mat._11 = vStartPos.x;	mat._12 = vStartPos.y;	mat._13 = vStartPos.z;	mat._14 = 0.f;
	mat._21 = vEndPos.x;	mat._22 = vEndPos.y;	mat._23 = vEndPos.z;	mat._24 = 0.f;
	mat._31 = vStartVel.x;	mat._32 = vStartVel.y;	mat._33 = vStartVel.z;	mat._34 = 0.f;
	mat._41 = vEndVel.x; 	mat._42 = vEndVel.y; 	mat._43 = vEndVel.z; 	mat._44 = 0.f;

	mat = m_matHermite * mat;

	D3DXVECTOR4 vTimeVector( 3.f * fTime * fTime , 2.f * fTime, 1.f, 0.f );

	D3DXVec4Transform( &vTimeVector, &vTimeVector, &mat );

	RwV3d vResultDir;

	vResultDir.x = vTimeVector.x;
	vResultDir.y = vTimeVector.y;
	vResultDir.z = vTimeVector.z;

	return vResultDir;
}

RwV3d CDboHermiteMoveSpline::GetPositionOnCubic( const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vStartVel, const D3DXVECTOR3& vEndPos, const D3DXVECTOR3& vEndVel, RwReal fTime )
{
	D3DXMATRIX mat;

	mat._11 = vStartPos.x;	mat._12 = vStartPos.y;	mat._13 = vStartPos.z;	mat._14 = 0.f;
	mat._21 = vEndPos.x;	mat._22 = vEndPos.y;	mat._23 = vEndPos.z;	mat._24 = 0.f;
	mat._31 = vStartVel.x;	mat._32 = vStartVel.y;	mat._33 = vStartVel.z;	mat._34 = 0.f;
	mat._41 = vEndVel.x; 	mat._42 = vEndVel.y; 	mat._43 = vEndVel.z; 	mat._44 = 0.f;

	mat = m_matHermite * mat;

	D3DXVECTOR4 vTimeVector( fTime * fTime * fTime, fTime * fTime, fTime, 1.f );

	D3DXVec4Transform( &vTimeVector, &vTimeVector, &mat );

	RwV3d vResultPos;

	vResultPos.x = vTimeVector.x;
	vResultPos.y = vTimeVector.y;
	vResultPos.z = vTimeVector.z;

	return vResultPos;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
