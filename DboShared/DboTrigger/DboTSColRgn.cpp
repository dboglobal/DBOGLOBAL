#include "precomp_trigger.h"
#include "DboTSColRgn.h"
#include <float.h>
#include <math.h>


/**
	collision region event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSColRgn, CNtlTSEvent )


CDboTSColRgn::CDboTSColRgn( void )
{
	m_uiWorldTblIdx = 0xffffffff;
	m_uiColCheckType = eCOL_RESULT_TYPE_NOT_COLLISION;

	m_eColRgnType = eEVENT_COL_RGN_TYPE_INVALID;

	m_bAABBMaked = false;
	m_fAABBBeginX = FLT_MAX;
	m_fAABBBeginZ = FLT_MAX;
	m_fAABBEndX = FLT_MAX;
	m_fAABBEndZ = FLT_MAX;
}

void CDboTSColRgn::SetWorldIdx( unsigned int uiWorldIdx )
{
	m_uiWorldTblIdx = uiWorldIdx;
}

void CDboTSColRgn::SetColCheckType( unsigned int uiColCheckType )
{
	m_uiColCheckType = uiColCheckType;
};

void CDboTSColRgn::SetRadiusData( float fX, float fZ, float fRadius )
{
	m_eColRgnType = eEVENT_COL_RGN_TYPE_RADIUS;

	m_uColRngData.sRadius.x = fX;
	m_uColRngData.sRadius.z = fZ;

	m_uColRngData.sRadius.r = fRadius;
}

void CDboTSColRgn::SetRectangleData( float fX0, float fZ0,
									 float fX1, float fZ1,
									 float fX2, float fZ2,
									 float fX3, float fZ3 )
{
	m_eColRgnType = eEVENT_COL_RGN_TYPE_RECTANGLE;

	m_uColRngData.sRectangle.x[0] = fX0;
	m_uColRngData.sRectangle.z[0] = fZ0;

	m_uColRngData.sRectangle.x[1] = fX1;
	m_uColRngData.sRectangle.z[1] = fZ1;

	m_uColRngData.sRectangle.x[2] = fX2;
	m_uColRngData.sRectangle.z[2] = fZ2;

	m_uColRngData.sRectangle.x[3] = fX3;
	m_uColRngData.sRectangle.z[3] = fZ3;
}

bool CDboTSColRgn::ColCheck( float fBeginX, float fBeginZ, float fEndX, float fEndZ )
{
	if ( IsInvalid() )
	{
		return false;
	}

	switch ( m_eColRgnType )
	{
	case eEVENT_COL_RGN_TYPE_RADIUS:
		return ( m_uiColCheckType & ColCheck_Radius( fBeginX, fBeginZ, fEndX, fEndZ ) ) ? true : false;

	case eEVENT_COL_RGN_TYPE_RECTANGLE:
		return ( m_uiColCheckType & ColCheck_Retangle( fBeginX, fBeginZ, fEndX, fEndZ ) ) ? true : false;
	}

	return false;
}

bool CDboTSColRgn::InCheck( float fPosX, float fPosZ )
{
	if ( IsInvalid() )
	{
		return false;
	}

	switch ( m_eColRgnType )
	{
	case eEVENT_COL_RGN_TYPE_RADIUS:
		return IsInRadius( fPosX, fPosZ, m_uColRngData.sRadius ) ? true : false;

	case eEVENT_COL_RGN_TYPE_RECTANGLE:
		return IsInRectangle( fPosX, fPosZ, m_uColRngData.sRectangle ) ? true : false;
	}

	return false;
}

bool CDboTSColRgn::IsInAABB( float fPosX, float fPosZ, float fPlusOffset /*= 0.f*/ )
{
	if ( !m_bAABBMaked )
	{
		switch ( m_eColRgnType )
		{
		case eEVENT_COL_RGN_TYPE_RADIUS:
			{
				m_fAABBBeginX = m_uColRngData.sRadius.x - m_uColRngData.sRadius.r;
				m_fAABBBeginZ = m_uColRngData.sRadius.z - m_uColRngData.sRadius.r;

				m_fAABBEndX = m_uColRngData.sRadius.x + m_uColRngData.sRadius.r;
				m_fAABBEndZ = m_uColRngData.sRadius.z + m_uColRngData.sRadius.r;

				m_bAABBMaked = true;
			}
			break;

		case eEVENT_COL_RGN_TYPE_RECTANGLE:
			{
				m_fAABBBeginX = m_uColRngData.sRectangle.x[0];
				m_fAABBBeginZ = m_uColRngData.sRectangle.z[0];

				m_fAABBEndX = m_uColRngData.sRectangle.x[0];
				m_fAABBEndZ = m_uColRngData.sRectangle.z[0];

				for ( int i = 1; i < 4; ++i )
				{
					m_fAABBBeginX = m_uColRngData.sRectangle.x[i] < m_fAABBBeginX ? m_uColRngData.sRectangle.x[i] : m_fAABBBeginX;
					m_fAABBBeginZ = m_uColRngData.sRectangle.z[i] < m_fAABBBeginZ ? m_uColRngData.sRectangle.z[i] : m_fAABBBeginZ;

					m_fAABBEndX = m_uColRngData.sRectangle.x[i] > m_fAABBEndX ? m_uColRngData.sRectangle.x[i] : m_fAABBEndX;
					m_fAABBEndZ = m_uColRngData.sRectangle.z[i] > m_fAABBEndZ ? m_uColRngData.sRectangle.z[i] : m_fAABBEndZ;
				}

				m_bAABBMaked = true;
			}
			break;
		}
	}

	if ( !m_bAABBMaked )
	{
		return false;
	}

	if ( m_fAABBBeginX - fPlusOffset < fPosX && fPosX < m_fAABBEndX + fPlusOffset &&
		 m_fAABBBeginZ - fPlusOffset < fPosZ && fPosZ < m_fAABBEndZ + fPlusOffset )
	{
		return true;
	}

	return false;
}

void CDboTSColRgn::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "widx" ) )
	{
		m_uiWorldTblIdx = clProperty.GetValueAsInt( "widx" );
	}

	if ( clProperty.IsExist( "cct" ) )
	{
		m_uiColCheckType = clProperty.GetValueAsInt( "cct" );
	}

	if ( clProperty.IsExist( "crt" ) )
	{
		m_eColRgnType = (eEVENT_COL_RGN_TYPE)clProperty.GetValueAsInt( "crt" );
	}

	switch ( m_eColRgnType )
	{
	case eEVENT_COL_RGN_TYPE_RADIUS:
		{
			if ( clProperty.IsExist( "x" ) )
			{
				m_uColRngData.sRadius.x = (float)atof( clProperty.GetValue( "x" ).c_str() );
			}
			else
			{
				m_uColRngData.sRadius.x = FLT_MAX;
			}

			if ( clProperty.IsExist( "z" ) )
			{
				m_uColRngData.sRadius.z = (float)atof( clProperty.GetValue( "z" ).c_str() );
			}
			else
			{
				m_uColRngData.sRadius.z = FLT_MAX;
			}

			if ( clProperty.IsExist( "r" ) )
			{
				m_uColRngData.sRadius.r = (float)atof( clProperty.GetValue( "r" ).c_str() );
			}
			else
			{
				m_uColRngData.sRadius.r = FLT_MAX;
			}
		}
		break;

	case eEVENT_COL_RGN_TYPE_RECTANGLE:
		{
			if ( clProperty.IsExist( "x0" ) )
			{
				m_uColRngData.sRectangle.x[0] = (float)atof( clProperty.GetValue( "x0" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.x[0] = FLT_MAX;
			}

			if ( clProperty.IsExist( "z0" ) )
			{
				m_uColRngData.sRectangle.z[0] = (float)atof( clProperty.GetValue( "z0" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.z[0] = FLT_MAX;
			}

			if ( clProperty.IsExist( "x1" ) )
			{
				m_uColRngData.sRectangle.x[1] = (float)atof( clProperty.GetValue( "x1" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.x[1] = FLT_MAX;
			}

			if ( clProperty.IsExist( "z1" ) )
			{
				m_uColRngData.sRectangle.z[1] = (float)atof( clProperty.GetValue( "z1" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.z[1] = FLT_MAX;
			}

			if ( clProperty.IsExist( "x2" ) )
			{
				m_uColRngData.sRectangle.x[2] = (float)atof( clProperty.GetValue( "x2" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.x[2] = FLT_MAX;
			}

			if ( clProperty.IsExist( "z2" ) )
			{
				m_uColRngData.sRectangle.z[2] = (float)atof( clProperty.GetValue( "z2" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.z[2] = FLT_MAX;
			}

			if ( clProperty.IsExist( "x3" ) )
			{
				m_uColRngData.sRectangle.x[3] = (float)atof( clProperty.GetValue( "x3" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.x[3] = FLT_MAX;
			}

			if ( clProperty.IsExist( "z3" ) )
			{
				m_uColRngData.sRectangle.z[3] = (float)atof( clProperty.GetValue( "z3" ).c_str() );
			}
			else
			{
				m_uColRngData.sRectangle.z[3] = FLT_MAX;
			}
		}
		break;
	}
}

void CDboTSColRgn::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", m_uiWorldTblIdx );
	clProperty.m_defProperty["widx"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_uiColCheckType );
	clProperty.m_defProperty["cct"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", m_eColRgnType );
	clProperty.m_defProperty["crt"] = g_NtlTSString;

	switch ( m_eColRgnType )
	{
	case eEVENT_COL_RGN_TYPE_RADIUS:
		{
			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRadius.x );
			clProperty.m_defProperty["x"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRadius.z );
			clProperty.m_defProperty["z"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRadius.r );
			clProperty.m_defProperty["r"] = g_NtlTSString;
		}
		break;

	case eEVENT_COL_RGN_TYPE_RECTANGLE:
		{
			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.x[0] );
			clProperty.m_defProperty["x0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.z[0] );
			clProperty.m_defProperty["z0"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.x[1] );
			clProperty.m_defProperty["x1"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.z[1] );
			clProperty.m_defProperty["z1"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.x[2] );
			clProperty.m_defProperty["x2"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.z[2] );
			clProperty.m_defProperty["z2"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.x[3] );
			clProperty.m_defProperty["x3"] = g_NtlTSString;

			sprintf_s( g_NtlTSString, "%f", m_uColRngData.sRectangle.z[3] );
			clProperty.m_defProperty["z3"] = g_NtlTSString;
		}
		break;
	}
}


bool CDboTSColRgn::IsInvalid( void )
{
	switch ( m_eColRgnType )
	{
	case eEVENT_COL_RGN_TYPE_RADIUS:
		{
			if ( FLT_MAX == m_uColRngData.sRadius.x ||
				 FLT_MAX == m_uColRngData.sRadius.z ||
				 FLT_MAX == m_uColRngData.sRadius.r )
			{
				return true;
			}
		}
		return false;

	case eEVENT_COL_RGN_TYPE_RECTANGLE:
		{
			if ( FLT_MAX == m_uColRngData.sRectangle.x[0] ||
				 FLT_MAX == m_uColRngData.sRectangle.z[0] ||
				 FLT_MAX == m_uColRngData.sRectangle.x[1] ||
				 FLT_MAX == m_uColRngData.sRectangle.z[1] ||
				 FLT_MAX == m_uColRngData.sRectangle.x[2] ||
				 FLT_MAX == m_uColRngData.sRectangle.z[2] ||
				 FLT_MAX == m_uColRngData.sRectangle.x[3] ||
				 FLT_MAX == m_uColRngData.sRectangle.z[3] )
			{
				return true;
			}
		}
		return false;
	}

	return true;
}

eCOL_RESULT_TYPE CDboTSColRgn::ColCheck_Radius( float fBeginX, float fBeginZ, float fEndX, float fEndZ )
{
	bool bBeginIn = IsInRadius( fBeginX, fBeginZ, m_uColRngData.sRadius );
	bool bEndIn = IsInRadius( fEndX, fEndZ, m_uColRngData.sRadius );

	// 시작점[In], 종료점[In]
	if ( bBeginIn && bEndIn )
	{
		// 시작점 및 종료점이 모두 안에 존재하는 경우는
		// 충돌하지 않은 것으로 간주한다
		return eCOL_RESULT_TYPE_NOT_COLLISION;
	}
	// 시작점[Out], 종료점[In]
	else if ( !bBeginIn && bEndIn )
	{
		// 밖에서 안으로 들어온것으로 간주한다
		return eCOL_RESULT_TYPE_ENTER;
	}
	// 시작점[In], 종료점[Out]
	else if ( bBeginIn && !bEndIn )
	{
		// 안에서 밖으로 나간것으로 간주한다
		return eCOL_RESULT_TYPE_LEAVE;
	}
	// 시작점[Out], 종료점[Out]
	else if ( !bBeginIn && !bEndIn )
	{
		// 통과 여부를 검사한다
		float fDirX = fEndX - fBeginX;	float fDirZ = fEndZ - fBeginZ;
		float fInvLength = 1.f / sqrtf( fDirX * fDirX + fDirZ * fDirZ );
		fDirX *= fInvLength; fDirZ *= fInvLength;

		float fProjX = (m_uColRngData.sRadius.x - fBeginX) * fDirX;
		float fProjZ = (m_uColRngData.sRadius.z - fBeginZ) * fDirZ;

		float fVerticalProjSDist = (fProjX - (m_uColRngData.sRadius.x - fBeginX)) * fProjX - (m_uColRngData.sRadius.x - fBeginX) +
								   (fProjZ - (m_uColRngData.sRadius.z - fBeginZ)) * fProjZ - (m_uColRngData.sRadius.z - fBeginZ);

		if ( fVerticalProjSDist <= m_uColRngData.sRadius.r * m_uColRngData.sRadius.r )
		{
			return eCOL_RESULT_TYPE_THROUGH;
		}
		else
		{
			return eCOL_RESULT_TYPE_NOT_COLLISION;
		}
	}

	return eCOL_RESULT_TYPE_NOT_COLLISION;
}

eCOL_RESULT_TYPE CDboTSColRgn::ColCheck_Retangle( float fBeginX, float fBeginZ, float fEndX, float fEndZ )
{
	bool bBeginIn = IsInRectangle( fBeginX, fBeginZ, m_uColRngData.sRectangle );
	bool bEndIn = IsInRectangle( fEndX, fEndZ, m_uColRngData.sRectangle );

	// 시작점[In], 종료점[In]
	if ( bBeginIn && bEndIn )
	{
		// 시작점 및 종료점이 모두 안에 존재하는 경우는
		// 충돌하지 않은 것으로 간주한다
		return eCOL_RESULT_TYPE_NOT_COLLISION;
	}
	// 시작점[Out], 종료점[In]
	else if ( !bBeginIn && bEndIn )
	{
		// 밖에서 안으로 들어온것으로 간주한다
		return eCOL_RESULT_TYPE_ENTER;
	}
	// 시작점[In], 종료점[Out]
	else if ( bBeginIn && !bEndIn )
	{
		// 안에서 밖으로 나간것으로 간주한다
		return eCOL_RESULT_TYPE_LEAVE;
	}
	// 시작점[Out], 종료점[Out]
	else if ( !bBeginIn && !bEndIn )
	{
		// 통과 여부를 검사한다

		if ( IsIntersectLine( fBeginX, fBeginZ, fEndX, fEndZ, m_uColRngData.sRectangle.x[3], m_uColRngData.sRectangle.z[3], m_uColRngData.sRectangle.x[0], m_uColRngData.sRectangle.z[0] ) )
		{
			return eCOL_RESULT_TYPE_THROUGH;
		}

		for ( int i = 0; i < 3; ++i )
		{
			if ( IsIntersectLine( fBeginX, fBeginZ, fEndX, fEndZ, m_uColRngData.sRectangle.x[i], m_uColRngData.sRectangle.z[i], m_uColRngData.sRectangle.x[i+1], m_uColRngData.sRectangle.z[i+1] ) )
			{
				return eCOL_RESULT_TYPE_THROUGH;
			}
		}

		return eCOL_RESULT_TYPE_NOT_COLLISION;
	}

	return eCOL_RESULT_TYPE_NOT_COLLISION;
}

bool CDboTSColRgn::IsInRadius( float fX, float fZ, sRadiusData& sRadData )
{
	float fSRadius = sRadData.r * sRadData.r;
	float fSDist = (fX - sRadData.x) * (fX - sRadData.x) + (fZ - sRadData.z) * (fZ - sRadData.z);

	return fSRadius < fSDist ? false : true;
}

bool CDboTSColRgn::IsInRectangle( float fX, float fZ, sRectangleData& sRectData )
{
	float fInputLineX = fX - sRectData.x[3];
	float fInputLineZ = fZ - sRectData.z[3];

	float fRectLineX = sRectData.x[0] - sRectData.x[3];
	float fRectLineZ = sRectData.z[0] - sRectData.z[3];

	bool bNormPlusDir = fInputLineZ * fRectLineX - fInputLineX * fRectLineZ > 0.f ? true : false;

	for ( int i = 0; i < 3; ++i )
	{
		fInputLineX = fX - sRectData.x[i];
		fInputLineZ = fZ - sRectData.z[i];

		fRectLineX = sRectData.x[i+1] - sRectData.x[i];
		fRectLineZ = sRectData.z[i+1] - sRectData.z[i];

		if ( ( bNormPlusDir && (fInputLineZ * fRectLineX - fInputLineX * fRectLineZ < 0.f) ) ||
			 ( !bNormPlusDir && (fInputLineZ * fRectLineX - fInputLineX * fRectLineZ > 0.f) ) )
		{
			return false;
		}
	}

	return true;
}

bool CDboTSColRgn::IsIntersectLine( float fAx, float fAz, float fBx, float fBz, float fCx, float fCz, float fDx, float fDz )
{
	float fDistAB, fTheCos, fTheSin, fNewX, fABPos;

	// Intersection 할 Line의 길이가 0 인 경우는 충돌 하지 않은 것으로 간주
	if ( fAx == fBx && fAz == fBz || fCx == fDx && fCz == fDz )
	{
		return false;
	}

	fBx -= fAx; fBz -= fAz;
	fCx -= fAx; fCz -= fAz;
	fDx -= fAx; fDz -= fAz;

	fDistAB = sqrtf( fBx * fBx + fBz * fBz );

	fTheCos = fBx / fDistAB;
	fTheSin = fBz / fDistAB;
	fNewX = fCx * fTheCos + fCz * fTheSin;
	fCz = fCz * fTheCos - fCx * fTheSin;
	fCx = fNewX;
	fNewX = fDx * fTheCos + fDz * fTheSin;
	fDz = fDz * fTheCos - fDx * fTheSin;
	fDx = fNewX;

	if  ( fCz < 0.f && fDz < 0.f || fCz >= 0.f && fDz >= 0.f )
	{
		return false;
	}

	fABPos = fDx + ( fCx - fDx ) * fDz / ( fDz - fCz );

	if ( fABPos < 0.f || fABPos > fDistAB )
	{
		return false;
	}

	return true;
}
