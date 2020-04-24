//////////////////////////////////////////////////////////////////////////
/**
 *	Matrix4 클래스
 *	왼손 좌표계, 행벡터를 기준으로 구현된 4 X 4 매트릭.
 *
 *	\file		CNtlNaviMatrix4.h
 *	\author		Jeong Ho, Rho
*/
//////////////////////////////////////////////////////////////////////////


CNtlNaviMatrix4::CNtlNaviMatrix4( bool bIdentity )
{
	if ( bIdentity ) MakeIdentity();
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviMatrix3& m3Val )
{
	SetElem( m3Val );

	m_fDAElem[0][3] = 0.f;	m_fDAElem[1][3] = 0.f;	m_fDAElem[2][3] = 0.f;

	m_fDAElem[3][0] = 0.f;	m_fDAElem[3][1] = 0.f;	m_fDAElem[3][2] = 0.f;	m_fDAElem[3][3] = 1.f;
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviMatrix3& m3Rot, const CNtlNaviVector3& v3Trans )
{
	SetElem( m3Rot, v3Trans );

	m_fDAElem[0][3] = 0.f;	m_fDAElem[1][3] = 0.f;	m_fDAElem[2][3] = 0.f;

	m_fDAElem[3][3] = 1.f;
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviMatrix4& m4Val )
{
	SetElem( m4Val );
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviVector3& v3ValX,
								  const CNtlNaviVector3& v3ValY,
								  const CNtlNaviVector3& v3ValZ )
{
	SetElem( v3ValX, v3ValY, v3ValZ );

	m_fDAElem[0][3] = 0.f;	m_fDAElem[1][3] = 0.f;	m_fDAElem[2][3] = 0.f;

	m_fDAElem[3][0] = 0.f;	m_fDAElem[3][1] = 0.f;	m_fDAElem[3][2] = 0.f;	m_fDAElem[3][3] = 1.f;
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviVector3& v3ValX,
								  const CNtlNaviVector3& v3ValY,
								  const CNtlNaviVector3& v3ValZ,
								  const CNtlNaviVector3& v3ValT )
{
	SetElem( v3ValX, v3ValY, v3ValZ, v3ValT );

	m_fDAElem[0][3] = 0.f;	m_fDAElem[1][3] = 0.f;	m_fDAElem[2][3] = 0.f;	m_fDAElem[3][3] = 1.f;
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviVector4& v4ValX,
								  const CNtlNaviVector4& v4ValY,
								  const CNtlNaviVector4& v4ValZ )
{
	SetElem( v4ValX, v4ValY, v4ValZ );

	m_fDAElem[3][0] = 0.f;	m_fDAElem[3][1] = 0.f;	m_fDAElem[3][2] = 0.f;	m_fDAElem[3][3] = 1.f;
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const CNtlNaviVector4& v4ValX,
								  const CNtlNaviVector4& v4ValY,
								  const CNtlNaviVector4& v4ValZ,
								  const CNtlNaviVector4& v4ValT )
{
	SetElem( v4ValX, v4ValY, v4ValZ, v4ValT );
}


CNtlNaviMatrix4::CNtlNaviMatrix4( float f00, float f01, float f02, float f03,
								  float f10, float f11, float f12, float f13,
								  float f20, float f21, float f22, float f23,
								  float f30, float f31, float f32, float f33 )
{
	m_fDAElem[0][0] = f00;	m_fDAElem[0][1] = f01;	m_fDAElem[0][2] = f02;	m_fDAElem[0][3] = f03;
	m_fDAElem[1][0] = f10;	m_fDAElem[1][1] = f11;	m_fDAElem[1][2] = f12;	m_fDAElem[1][3] = f13;
	m_fDAElem[2][0] = f20;	m_fDAElem[2][1] = f21;	m_fDAElem[2][2] = f22;	m_fDAElem[2][3] = f23;
	m_fDAElem[3][0] = f30;	m_fDAElem[3][1] = f31;	m_fDAElem[3][2] = f32;	m_fDAElem[3][3] = f33;
}


CNtlNaviMatrix4::CNtlNaviMatrix4( const float* pfVal )
{
	SetElem( pfVal );
}


float CNtlNaviMatrix4::GetElem( int nIdx ) const
{
	return m_fSAElem[nIdx];
}


float CNtlNaviMatrix4::GetElem( int nRow, int nCol ) const
{
	return m_fDAElem[nRow][nCol];
}


void CNtlNaviMatrix4::SetElem( int nRow, int nCol, float fVal )
{
	m_fDAElem[nRow][nCol] = fVal;
}


void CNtlNaviMatrix4::SetElem( const CNtlNaviMatrix3& m3Val )
{
	const float* pVal = m3Val;

	m_fDAElem[0][0] = pVal[0];	m_fDAElem[0][1] = pVal[1];	m_fDAElem[0][2] = pVal[2];
	m_fDAElem[1][0] = pVal[3];	m_fDAElem[1][1] = pVal[4];	m_fDAElem[1][2] = pVal[5];
	m_fDAElem[2][0] = pVal[6];	m_fDAElem[2][1] = pVal[7];	m_fDAElem[2][2] = pVal[8];
}


void CNtlNaviMatrix4::SetElem( const CNtlNaviMatrix3& m3Rot, const CNtlNaviVector3& v3Trans )
{
	const float* pVal = m3Rot;

	m_fDAElem[0][0] = pVal[0];	m_fDAElem[0][1] = pVal[1];	m_fDAElem[0][2] = pVal[2];
	m_fDAElem[1][0] = pVal[3];	m_fDAElem[1][1] = pVal[4];	m_fDAElem[1][2] = pVal[5];
	m_fDAElem[2][0] = pVal[6];	m_fDAElem[2][1] = pVal[7];	m_fDAElem[2][2] = pVal[8];

	pVal = v3Trans;

	m_fDAElem[3][0] = pVal[0];	m_fDAElem[3][1] = pVal[1];	m_fDAElem[3][2] = pVal[2];
}


void CNtlNaviMatrix4::SetElem( const CNtlNaviVector3& v3ValX,
							   const CNtlNaviVector3& v3ValY,
							   const CNtlNaviVector3& v3ValZ )
{
	v3ValX.GetXYZ( m_fDAElem[0][0], m_fDAElem[0][1], m_fDAElem[0][2] );
	v3ValY.GetXYZ( m_fDAElem[1][0], m_fDAElem[1][1], m_fDAElem[1][2] );
	v3ValZ.GetXYZ( m_fDAElem[2][0], m_fDAElem[2][1], m_fDAElem[2][2] );
}


void CNtlNaviMatrix4::SetElem( const CNtlNaviVector3& v3ValX,
							   const CNtlNaviVector3& v3ValY,
							   const CNtlNaviVector3& v3ValZ,
							   const CNtlNaviVector3& v3ValT )
{
	v3ValX.GetXYZ( m_fDAElem[0][0], m_fDAElem[0][1], m_fDAElem[0][2] );
	v3ValY.GetXYZ( m_fDAElem[1][0], m_fDAElem[1][1], m_fDAElem[1][2] );
	v3ValZ.GetXYZ( m_fDAElem[2][0], m_fDAElem[2][1], m_fDAElem[2][2] );
	v3ValT.GetXYZ( m_fDAElem[3][0], m_fDAElem[3][1], m_fDAElem[3][2] );
}


void CNtlNaviMatrix4::SetElem( const CNtlNaviVector4& v4ValX,
							   const CNtlNaviVector4& v4ValY,
							   const CNtlNaviVector4& v4ValZ )
{
	v4ValX.GetXYZW( m_fDAElem[0][0], m_fDAElem[0][1], m_fDAElem[0][2], m_fDAElem[0][3] );
	v4ValY.GetXYZW( m_fDAElem[1][0], m_fDAElem[1][1], m_fDAElem[1][2], m_fDAElem[1][3] );
	v4ValZ.GetXYZW( m_fDAElem[2][0], m_fDAElem[2][1], m_fDAElem[2][2], m_fDAElem[2][3] );
}


void CNtlNaviMatrix4::SetElem( const CNtlNaviVector4& v4ValX,
							   const CNtlNaviVector4& v4ValY,
							   const CNtlNaviVector4& v4ValZ,
							   const CNtlNaviVector4& v4ValT )
{
	v4ValX.GetXYZW( m_fDAElem[0][0], m_fDAElem[0][1], m_fDAElem[0][2], m_fDAElem[0][3] );
	v4ValY.GetXYZW( m_fDAElem[1][0], m_fDAElem[1][1], m_fDAElem[1][2], m_fDAElem[1][3] );
	v4ValZ.GetXYZW( m_fDAElem[2][0], m_fDAElem[2][1], m_fDAElem[2][2], m_fDAElem[2][3] );
	v4ValT.GetXYZW( m_fDAElem[3][0], m_fDAElem[3][1], m_fDAElem[3][2], m_fDAElem[3][3] );
}


void CNtlNaviMatrix4::SetElem( float f00, float f01, float f02, float f03,
							   float f10, float f11, float f12, float f13,
							   float f20, float f21, float f22, float f23,
							   float f30, float f31, float f32, float f33 )
{
	m_fDAElem[0][0] = f00;	m_fDAElem[0][1] = f01;	m_fDAElem[0][2] = f02;	m_fDAElem[0][3] = f03;
	m_fDAElem[1][0] = f10;	m_fDAElem[1][1] = f11;	m_fDAElem[1][2] = f12;	m_fDAElem[1][3] = f13;
	m_fDAElem[2][0] = f20;	m_fDAElem[2][1] = f21;	m_fDAElem[2][2] = f22;	m_fDAElem[2][3] = f23;
	m_fDAElem[3][0] = f30;	m_fDAElem[3][1] = f31;	m_fDAElem[3][2] = f32;	m_fDAElem[3][3] = f33;
}


void CNtlNaviMatrix4::SetElem( const float* pfVal )
{
	memcpy( m_fSAElem, pfVal, sizeof(float)*16 );
}


CNtlNaviVector3& CNtlNaviMatrix4::GetRow( int nRow )
{
	return *(CNtlNaviVector3*)&m_fDAElem[nRow][0];
}


const CNtlNaviVector3& CNtlNaviMatrix4::GetRow( int nRow ) const
{
	return *(const CNtlNaviVector3*)&m_fDAElem[nRow][0];
}


void CNtlNaviMatrix4::GetRow( int nRow, CNtlNaviVector3& v3Val ) const
{
	v3Val.SetElem( m_fDAElem[nRow][0], m_fDAElem[nRow][1], m_fDAElem[nRow][2] );
}


void CNtlNaviMatrix4::SetRow( int nRow, const CNtlNaviVector3& v3Val )
{
	v3Val.GetXYZ( m_fDAElem[nRow][0], m_fDAElem[nRow][1], m_fDAElem[nRow][2] );
}


const CNtlNaviVector3 CNtlNaviMatrix4::GetCol( int nCol ) const
{
	return CNtlNaviVector3( m_fDAElem[0][nCol], m_fDAElem[1][nCol], m_fDAElem[2][nCol] );
}


void CNtlNaviMatrix4::GetCol( int nCol, CNtlNaviVector3& v3Val ) const
{
	v3Val.SetElem( m_fDAElem[0][nCol], m_fDAElem[1][nCol], m_fDAElem[2][nCol] );
}


void CNtlNaviMatrix4::SetCol( int nCol, const CNtlNaviVector3& v3Val )
{
	v3Val.GetXYZ( m_fDAElem[0][nCol], m_fDAElem[1][nCol], m_fDAElem[2][nCol] );
}


void CNtlNaviMatrix4::MakeInvalid( void )
{
	m_fDAElem[0][0] = NAVI_FLT_MAX;	m_fDAElem[0][1] = NAVI_FLT_MAX;	m_fDAElem[0][2] = NAVI_FLT_MAX;	m_fDAElem[0][3] = NAVI_FLT_MAX;
	m_fDAElem[1][0] = NAVI_FLT_MAX;	m_fDAElem[1][1] = NAVI_FLT_MAX;	m_fDAElem[1][2] = NAVI_FLT_MAX;	m_fDAElem[1][3] = NAVI_FLT_MAX;
	m_fDAElem[2][0] = NAVI_FLT_MAX;	m_fDAElem[2][1] = NAVI_FLT_MAX;	m_fDAElem[2][2] = NAVI_FLT_MAX;	m_fDAElem[2][3] = NAVI_FLT_MAX;
	m_fDAElem[3][0] = NAVI_FLT_MAX;	m_fDAElem[3][1] = NAVI_FLT_MAX;	m_fDAElem[3][2] = NAVI_FLT_MAX;	m_fDAElem[3][3] = NAVI_FLT_MAX;
}


void CNtlNaviMatrix4::MakeZero( void )
{
	m_fDAElem[0][0] = 0.f;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;	m_fDAElem[0][3] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = 0.f;	m_fDAElem[1][2] = 0.f;	m_fDAElem[1][3] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = 0.f;	m_fDAElem[2][3] = 0.f;
	m_fDAElem[3][0] = 0.f;	m_fDAElem[3][1] = 0.f;	m_fDAElem[3][2] = 0.f;	m_fDAElem[3][3] = 0.f;
}


void CNtlNaviMatrix4::MakeIdentity( void )
{
	m_fDAElem[0][0] = 1.f;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;	m_fDAElem[0][3] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = 1.f;	m_fDAElem[1][2] = 0.f;	m_fDAElem[1][3] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = 1.f;	m_fDAElem[2][3] = 0.f;
	m_fDAElem[3][0] = 0.f;	m_fDAElem[3][1] = 0.f;	m_fDAElem[3][2] = 0.f;	m_fDAElem[3][3] = 1.f;
}


float CNtlNaviMatrix4::GetDet( void ) const
{
	float fA0 = m_fSAElem[ 0]*m_fSAElem[ 5] - m_fSAElem[ 1]*m_fSAElem[ 4];
    float fA1 = m_fSAElem[ 0]*m_fSAElem[ 6] - m_fSAElem[ 2]*m_fSAElem[ 4];
    float fA2 = m_fSAElem[ 0]*m_fSAElem[ 7] - m_fSAElem[ 3]*m_fSAElem[ 4];
    float fA3 = m_fSAElem[ 1]*m_fSAElem[ 6] - m_fSAElem[ 2]*m_fSAElem[ 5];
    float fA4 = m_fSAElem[ 1]*m_fSAElem[ 7] - m_fSAElem[ 3]*m_fSAElem[ 5];
    float fA5 = m_fSAElem[ 2]*m_fSAElem[ 7] - m_fSAElem[ 3]*m_fSAElem[ 6];
    float fB0 = m_fSAElem[ 8]*m_fSAElem[13] - m_fSAElem[ 9]*m_fSAElem[12];
    float fB1 = m_fSAElem[ 8]*m_fSAElem[14] - m_fSAElem[10]*m_fSAElem[12];
    float fB2 = m_fSAElem[ 8]*m_fSAElem[15] - m_fSAElem[11]*m_fSAElem[12];
    float fB3 = m_fSAElem[ 9]*m_fSAElem[14] - m_fSAElem[10]*m_fSAElem[13];
    float fB4 = m_fSAElem[ 9]*m_fSAElem[15] - m_fSAElem[11]*m_fSAElem[13];
    float fB5 = m_fSAElem[10]*m_fSAElem[15] - m_fSAElem[11]*m_fSAElem[14];

    return fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 *  fB0;
}


bool CNtlNaviMatrix4::CanInverse( void ) const
{
	return fabsf( GetDet() ) > NAVI_ZERO_THRESHOLD;
}


float CNtlNaviMatrix4::GetInverse( CNtlNaviMatrix4& m4Val ) const
{
	float fA0 = m_fSAElem[ 0]*m_fSAElem[ 5] - m_fSAElem[ 1]*m_fSAElem[ 4];
    float fA1 = m_fSAElem[ 0]*m_fSAElem[ 6] - m_fSAElem[ 2]*m_fSAElem[ 4];
    float fA2 = m_fSAElem[ 0]*m_fSAElem[ 7] - m_fSAElem[ 3]*m_fSAElem[ 4];
    float fA3 = m_fSAElem[ 1]*m_fSAElem[ 6] - m_fSAElem[ 2]*m_fSAElem[ 5];
    float fA4 = m_fSAElem[ 1]*m_fSAElem[ 7] - m_fSAElem[ 3]*m_fSAElem[ 5];
    float fA5 = m_fSAElem[ 2]*m_fSAElem[ 7] - m_fSAElem[ 3]*m_fSAElem[ 6];
    float fB0 = m_fSAElem[ 8]*m_fSAElem[13] - m_fSAElem[ 9]*m_fSAElem[12];
    float fB1 = m_fSAElem[ 8]*m_fSAElem[14] - m_fSAElem[10]*m_fSAElem[12];
    float fB2 = m_fSAElem[ 8]*m_fSAElem[15] - m_fSAElem[11]*m_fSAElem[12];
    float fB3 = m_fSAElem[ 9]*m_fSAElem[14] - m_fSAElem[10]*m_fSAElem[13];
    float fB4 = m_fSAElem[ 9]*m_fSAElem[15] - m_fSAElem[11]*m_fSAElem[13];
    float fB5 = m_fSAElem[10]*m_fSAElem[15] - m_fSAElem[11]*m_fSAElem[14];

    float fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 *  fB0;

	if ( fabsf( fDet ) <= NAVI_ZERO_THRESHOLD )
	{
		m4Val.MakeInvalid();
		return fDet;
	}

	float fIDet = 1.f / fDet;

	m4Val.m_fDAElem[0][0] = (+ m_fSAElem[ 5]*fB5 - m_fSAElem[ 6]*fB4 + m_fSAElem[ 7]*fB3) * fIDet;
    m4Val.m_fDAElem[1][0] = (- m_fSAElem[ 4]*fB5 + m_fSAElem[ 6]*fB2 - m_fSAElem[ 7]*fB1) * fIDet;
    m4Val.m_fDAElem[2][0] = (+ m_fSAElem[ 4]*fB4 - m_fSAElem[ 5]*fB2 + m_fSAElem[ 7]*fB0) * fIDet;
    m4Val.m_fDAElem[3][0] = (- m_fSAElem[ 4]*fB3 + m_fSAElem[ 5]*fB1 - m_fSAElem[ 6]*fB0) * fIDet;
    m4Val.m_fDAElem[0][1] = (- m_fSAElem[ 1]*fB5 + m_fSAElem[ 2]*fB4 - m_fSAElem[ 3]*fB3) * fIDet;
    m4Val.m_fDAElem[1][1] = (+ m_fSAElem[ 0]*fB5 - m_fSAElem[ 2]*fB2 + m_fSAElem[ 3]*fB1) * fIDet;
    m4Val.m_fDAElem[2][1] = (- m_fSAElem[ 0]*fB4 + m_fSAElem[ 1]*fB2 - m_fSAElem[ 3]*fB0) * fIDet;
    m4Val.m_fDAElem[3][1] = (+ m_fSAElem[ 0]*fB3 - m_fSAElem[ 1]*fB1 + m_fSAElem[ 2]*fB0) * fIDet;
    m4Val.m_fDAElem[0][2] = (+ m_fSAElem[13]*fA5 - m_fSAElem[14]*fA4 + m_fSAElem[15]*fA3) * fIDet;
    m4Val.m_fDAElem[1][2] = (- m_fSAElem[12]*fA5 + m_fSAElem[14]*fA2 - m_fSAElem[15]*fA1) * fIDet;
    m4Val.m_fDAElem[2][2] = (+ m_fSAElem[12]*fA4 - m_fSAElem[13]*fA2 + m_fSAElem[15]*fA0) * fIDet;
    m4Val.m_fDAElem[3][2] = (- m_fSAElem[12]*fA3 + m_fSAElem[13]*fA1 - m_fSAElem[14]*fA0) * fIDet;
    m4Val.m_fDAElem[0][3] = (- m_fSAElem[ 9]*fA5 + m_fSAElem[10]*fA4 - m_fSAElem[11]*fA3) * fIDet;
    m4Val.m_fDAElem[1][3] = (+ m_fSAElem[ 8]*fA5 - m_fSAElem[10]*fA2 + m_fSAElem[11]*fA1) * fIDet;
    m4Val.m_fDAElem[2][3] = (- m_fSAElem[ 8]*fA4 + m_fSAElem[ 9]*fA2 - m_fSAElem[11]*fA0) * fIDet;
    m4Val.m_fDAElem[3][3] = (+ m_fSAElem[ 8]*fA3 - m_fSAElem[ 9]*fA1 + m_fSAElem[10]*fA0) * fIDet;

    return fDet;
}


float CNtlNaviMatrix4::MakeInverse( void )
{
	return GetInverse( *this );
}


CNtlNaviMatrix4 CNtlNaviMatrix4::GetTranspose( void ) const
{
	CNtlNaviMatrix4 matTranspose;
	
	for ( int i = 0; i < 4; ++i )
	{
		for ( int j = 0; j < 4; ++j )
		{
			matTranspose.m_fDAElem[j][i] = m_fDAElem[i][j];
		}
	}

	return matTranspose;
}


void CNtlNaviMatrix4::MakeTranspose( void )
{
	float fTemp;

	int i, j;
	for ( i = 0; i < 3; ++i )
	{
		for ( j = i + 1; j < 4; ++j )
		{
			fTemp = m_fDAElem[i][j];
			m_fDAElem[i][j] = m_fDAElem[j][i];
			m_fDAElem[j][i] = fTemp;
		}
	}
}


float CNtlNaviMatrix4::GetTrace( void ) const
{
	return m_fDAElem[0][0] + m_fDAElem[1][1] + m_fDAElem[2][2] + m_fDAElem[3][3];
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator = ( const CNtlNaviMatrix3& m3Val )
{
	SetElem( m3Val );

	return *this;
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator = ( const CNtlNaviMatrix4& m4Val )
{
	SetElem( m4Val );

	return *this;
}


float* CNtlNaviMatrix4::operator [] ( int nIdx )
{
	return m_fDAElem[nIdx];
}


const float* CNtlNaviMatrix4::operator [] ( int nIdx ) const
{
	return m_fDAElem[nIdx];
}


float& CNtlNaviMatrix4::operator () ( int nRow, int nCol )
{
	return m_fDAElem[nRow][nCol];
}


const float& CNtlNaviMatrix4::operator () ( int nRow, int nCol ) const
{
	return m_fDAElem[nRow][nCol];
}


bool CNtlNaviMatrix4::operator == ( const CNtlNaviMatrix4& m4Val ) const
{
	return CompareArrays( m4Val ) == 0;
}


bool CNtlNaviMatrix4::operator != ( const CNtlNaviMatrix4& m4Val ) const
{
	return CompareArrays( m4Val ) != 0;
}


CNtlNaviMatrix4 CNtlNaviMatrix4::operator - ( void ) const
{
	return CNtlNaviMatrix4(
		-m_fDAElem[0][0], -m_fDAElem[0][1], -m_fDAElem[0][2], -m_fDAElem[0][3],
		-m_fDAElem[1][0], -m_fDAElem[1][1], -m_fDAElem[1][2], -m_fDAElem[1][3],
		-m_fDAElem[2][0], -m_fDAElem[2][1], -m_fDAElem[2][2], -m_fDAElem[2][3],
		-m_fDAElem[3][0], -m_fDAElem[3][1], -m_fDAElem[3][2], -m_fDAElem[3][3] );
}


CNtlNaviMatrix4 CNtlNaviMatrix4::operator * ( float fVal ) const
{
	return CNtlNaviMatrix4(
		m_fDAElem[0][0] * fVal, m_fDAElem[0][1] * fVal, m_fDAElem[0][2] * fVal, m_fDAElem[0][3] * fVal,
		m_fDAElem[1][0] * fVal, m_fDAElem[1][1] * fVal, m_fDAElem[1][2] * fVal, m_fDAElem[1][3] * fVal,
		m_fDAElem[2][0] * fVal, m_fDAElem[2][1] * fVal, m_fDAElem[2][2] * fVal, m_fDAElem[2][3] * fVal,
		m_fDAElem[3][0] * fVal, m_fDAElem[3][1] * fVal, m_fDAElem[3][2] * fVal, m_fDAElem[3][3] * fVal );
}


CNtlNaviMatrix4 CNtlNaviMatrix4::operator / ( float fVal ) const
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		return CNtlNaviMatrix4(
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, 
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, 
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, 
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX );

	}

	fVal = 1.f / fVal;

	return CNtlNaviMatrix4(
		m_fDAElem[0][0] * fVal, m_fDAElem[0][1] * fVal, m_fDAElem[0][2] * fVal, m_fDAElem[0][3] * fVal,
		m_fDAElem[1][0] * fVal, m_fDAElem[1][1] * fVal, m_fDAElem[1][2] * fVal, m_fDAElem[1][3] * fVal,
		m_fDAElem[2][0] * fVal, m_fDAElem[2][1] * fVal, m_fDAElem[2][2] * fVal, m_fDAElem[2][3] * fVal,
		m_fDAElem[3][0] * fVal, m_fDAElem[3][1] * fVal, m_fDAElem[3][2] * fVal, m_fDAElem[3][3] * fVal );
}


CNtlNaviMatrix4 CNtlNaviMatrix4::operator + ( const CNtlNaviMatrix4& m4Val ) const
{
	return CNtlNaviMatrix4(
		m_fDAElem[0][0] + m4Val.m_fDAElem[0][0],
		m_fDAElem[0][1] + m4Val.m_fDAElem[0][1],
		m_fDAElem[0][2] + m4Val.m_fDAElem[0][2],
		m_fDAElem[0][3] + m4Val.m_fDAElem[0][3],

		m_fDAElem[1][0] + m4Val.m_fDAElem[1][0],
		m_fDAElem[1][1] + m4Val.m_fDAElem[1][1],
		m_fDAElem[1][2] + m4Val.m_fDAElem[1][2],
		m_fDAElem[1][3] + m4Val.m_fDAElem[1][3],

		m_fDAElem[2][0] + m4Val.m_fDAElem[2][0],
		m_fDAElem[2][1] + m4Val.m_fDAElem[2][1],
		m_fDAElem[2][2] + m4Val.m_fDAElem[2][2],
		m_fDAElem[2][3] + m4Val.m_fDAElem[2][3],

		m_fDAElem[3][0] + m4Val.m_fDAElem[3][0],
		m_fDAElem[3][1] + m4Val.m_fDAElem[3][1],
		m_fDAElem[3][2] + m4Val.m_fDAElem[3][2],
		m_fDAElem[3][3] + m4Val.m_fDAElem[3][3] );
}


CNtlNaviMatrix4 CNtlNaviMatrix4::operator - ( const CNtlNaviMatrix4& m4Val ) const
{
	return CNtlNaviMatrix4(
		m_fDAElem[0][0] - m4Val.m_fDAElem[0][0],
		m_fDAElem[0][1] - m4Val.m_fDAElem[0][1],
		m_fDAElem[0][2] - m4Val.m_fDAElem[0][2],
		m_fDAElem[0][3] - m4Val.m_fDAElem[0][3],
		
		m_fDAElem[1][0] - m4Val.m_fDAElem[1][0],
		m_fDAElem[1][1] - m4Val.m_fDAElem[1][1],
		m_fDAElem[1][2] - m4Val.m_fDAElem[1][2],
		m_fDAElem[1][3] - m4Val.m_fDAElem[1][3],
		
		m_fDAElem[2][0] - m4Val.m_fDAElem[2][0],
		m_fDAElem[2][1] - m4Val.m_fDAElem[2][1],
		m_fDAElem[2][2] - m4Val.m_fDAElem[2][2],
		m_fDAElem[2][3] - m4Val.m_fDAElem[2][3],
		
		m_fDAElem[3][0] - m4Val.m_fDAElem[3][0],
		m_fDAElem[3][1] - m4Val.m_fDAElem[3][1],
		m_fDAElem[3][2] - m4Val.m_fDAElem[3][2],
		m_fDAElem[3][3] - m4Val.m_fDAElem[3][3] );
}


CNtlNaviMatrix4 CNtlNaviMatrix4::operator * ( const CNtlNaviMatrix4& m4Val ) const
{
	return CNtlNaviMatrix4(
		m_fDAElem[0][0] * m4Val.m_fDAElem[0][0] + m_fDAElem[0][1] * m4Val.m_fDAElem[1][0] + m_fDAElem[0][2] * m4Val.m_fDAElem[2][0] + m_fDAElem[0][3] * m4Val.m_fDAElem[3][0],
		m_fDAElem[0][0] * m4Val.m_fDAElem[0][1] + m_fDAElem[0][1] * m4Val.m_fDAElem[1][1] + m_fDAElem[0][2] * m4Val.m_fDAElem[2][1] + m_fDAElem[0][3] * m4Val.m_fDAElem[3][1],
		m_fDAElem[0][0] * m4Val.m_fDAElem[0][2] + m_fDAElem[0][1] * m4Val.m_fDAElem[1][2] + m_fDAElem[0][2] * m4Val.m_fDAElem[2][2] + m_fDAElem[0][3] * m4Val.m_fDAElem[3][2],
		m_fDAElem[0][0] * m4Val.m_fDAElem[0][3] + m_fDAElem[0][1] * m4Val.m_fDAElem[1][3] + m_fDAElem[0][2] * m4Val.m_fDAElem[2][3] + m_fDAElem[0][3] * m4Val.m_fDAElem[3][3],

		m_fDAElem[1][0] * m4Val.m_fDAElem[0][0] + m_fDAElem[1][1] * m4Val.m_fDAElem[1][0] + m_fDAElem[1][2] * m4Val.m_fDAElem[2][0] + m_fDAElem[1][3] * m4Val.m_fDAElem[3][0],
		m_fDAElem[1][0] * m4Val.m_fDAElem[0][1] + m_fDAElem[1][1] * m4Val.m_fDAElem[1][1] + m_fDAElem[1][2] * m4Val.m_fDAElem[2][1] + m_fDAElem[1][3] * m4Val.m_fDAElem[3][1],
		m_fDAElem[1][0] * m4Val.m_fDAElem[0][2] + m_fDAElem[1][1] * m4Val.m_fDAElem[1][2] + m_fDAElem[1][2] * m4Val.m_fDAElem[2][2] + m_fDAElem[1][3] * m4Val.m_fDAElem[3][2],
		m_fDAElem[1][0] * m4Val.m_fDAElem[0][3] + m_fDAElem[1][1] * m4Val.m_fDAElem[1][3] + m_fDAElem[1][2] * m4Val.m_fDAElem[2][3] + m_fDAElem[1][3] * m4Val.m_fDAElem[3][3],

		m_fDAElem[2][0] * m4Val.m_fDAElem[0][0] + m_fDAElem[2][1] * m4Val.m_fDAElem[1][0] + m_fDAElem[2][2] * m4Val.m_fDAElem[2][0] + m_fDAElem[2][3] * m4Val.m_fDAElem[3][0],
		m_fDAElem[2][0] * m4Val.m_fDAElem[0][1] + m_fDAElem[2][1] * m4Val.m_fDAElem[1][1] + m_fDAElem[2][2] * m4Val.m_fDAElem[2][1] + m_fDAElem[2][3] * m4Val.m_fDAElem[3][1],
		m_fDAElem[2][0] * m4Val.m_fDAElem[0][2] + m_fDAElem[2][1] * m4Val.m_fDAElem[1][2] + m_fDAElem[2][2] * m4Val.m_fDAElem[2][2] + m_fDAElem[2][3] * m4Val.m_fDAElem[3][2],
		m_fDAElem[2][0] * m4Val.m_fDAElem[0][3] + m_fDAElem[2][1] * m4Val.m_fDAElem[1][3] + m_fDAElem[2][2] * m4Val.m_fDAElem[2][3] + m_fDAElem[2][3] * m4Val.m_fDAElem[3][3],

		m_fDAElem[3][0] * m4Val.m_fDAElem[0][0] + m_fDAElem[3][1] * m4Val.m_fDAElem[1][0] + m_fDAElem[3][2] * m4Val.m_fDAElem[2][0] + m_fDAElem[3][3] * m4Val.m_fDAElem[3][0],
		m_fDAElem[3][0] * m4Val.m_fDAElem[0][1] + m_fDAElem[3][1] * m4Val.m_fDAElem[1][1] + m_fDAElem[3][2] * m4Val.m_fDAElem[2][1] + m_fDAElem[3][3] * m4Val.m_fDAElem[3][1],
		m_fDAElem[3][0] * m4Val.m_fDAElem[0][2] + m_fDAElem[3][1] * m4Val.m_fDAElem[1][2] + m_fDAElem[3][2] * m4Val.m_fDAElem[2][2] + m_fDAElem[3][3] * m4Val.m_fDAElem[3][2],
		m_fDAElem[3][0] * m4Val.m_fDAElem[0][3] + m_fDAElem[3][1] * m4Val.m_fDAElem[1][3] + m_fDAElem[3][2] * m4Val.m_fDAElem[2][3] + m_fDAElem[3][3] * m4Val.m_fDAElem[3][3] );
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator *= ( float fVal )
{
	m_fDAElem[0][0] *= fVal; m_fDAElem[0][1] *= fVal; m_fDAElem[0][2] *= fVal; m_fDAElem[0][3] *= fVal;
	m_fDAElem[1][0] *= fVal; m_fDAElem[1][1] *= fVal; m_fDAElem[1][2] *= fVal; m_fDAElem[1][3] *= fVal;
	m_fDAElem[2][0] *= fVal; m_fDAElem[2][1] *= fVal; m_fDAElem[2][2] *= fVal; m_fDAElem[2][3] *= fVal;
	m_fDAElem[3][0] *= fVal; m_fDAElem[3][1] *= fVal; m_fDAElem[3][2] *= fVal; m_fDAElem[3][3] *= fVal;

	return *this;
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator /= ( float fVal )
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		MakeInvalid();
	}
	else
	{
		fVal = 1.f / fVal;

		m_fDAElem[0][0] *= fVal; m_fDAElem[0][1] *= fVal; m_fDAElem[0][2] *= fVal; m_fDAElem[0][3] *= fVal;
		m_fDAElem[1][0] *= fVal; m_fDAElem[1][1] *= fVal; m_fDAElem[1][2] *= fVal; m_fDAElem[1][3] *= fVal;
		m_fDAElem[2][0] *= fVal; m_fDAElem[2][1] *= fVal; m_fDAElem[2][2] *= fVal; m_fDAElem[2][3] *= fVal;
		m_fDAElem[3][0] *= fVal; m_fDAElem[3][1] *= fVal; m_fDAElem[3][2] *= fVal; m_fDAElem[3][3] *= fVal;
	}

	return *this;
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator += ( const CNtlNaviMatrix4& m4Val )
{
	m_fDAElem[0][0] += m4Val.m_fDAElem[0][0];
	m_fDAElem[0][1] += m4Val.m_fDAElem[0][1];
	m_fDAElem[0][2] += m4Val.m_fDAElem[0][2];
	m_fDAElem[0][3] += m4Val.m_fDAElem[0][3];

	m_fDAElem[1][0] += m4Val.m_fDAElem[1][0];
	m_fDAElem[1][1] += m4Val.m_fDAElem[1][1];
	m_fDAElem[1][2] += m4Val.m_fDAElem[1][2];
	m_fDAElem[1][3] += m4Val.m_fDAElem[1][3];

	m_fDAElem[2][0] += m4Val.m_fDAElem[2][0];
	m_fDAElem[2][1] += m4Val.m_fDAElem[2][1];
	m_fDAElem[2][2] += m4Val.m_fDAElem[2][2];
	m_fDAElem[2][3] += m4Val.m_fDAElem[2][3];

	m_fDAElem[3][0] += m4Val.m_fDAElem[3][0];
	m_fDAElem[3][1] += m4Val.m_fDAElem[3][1];
	m_fDAElem[3][2] += m4Val.m_fDAElem[3][2];
	m_fDAElem[3][3] += m4Val.m_fDAElem[3][3];

	return *this;
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator -= ( const CNtlNaviMatrix4& m4Val )
{
	m_fDAElem[0][0] -= m4Val.m_fDAElem[0][0];
	m_fDAElem[0][1] -= m4Val.m_fDAElem[0][1];
	m_fDAElem[0][2] -= m4Val.m_fDAElem[0][2];
	m_fDAElem[0][3] -= m4Val.m_fDAElem[0][3];

	m_fDAElem[1][0] -= m4Val.m_fDAElem[1][0];
	m_fDAElem[1][1] -= m4Val.m_fDAElem[1][1];
	m_fDAElem[1][2] -= m4Val.m_fDAElem[1][2];
	m_fDAElem[1][3] -= m4Val.m_fDAElem[1][3];

	m_fDAElem[2][0] -= m4Val.m_fDAElem[2][0];
	m_fDAElem[2][1] -= m4Val.m_fDAElem[2][1];
	m_fDAElem[2][2] -= m4Val.m_fDAElem[2][2];
	m_fDAElem[2][3] -= m4Val.m_fDAElem[2][3];

	m_fDAElem[3][0] -= m4Val.m_fDAElem[3][0];
	m_fDAElem[3][1] -= m4Val.m_fDAElem[3][1];
	m_fDAElem[3][2] -= m4Val.m_fDAElem[3][2];
	m_fDAElem[3][3] -= m4Val.m_fDAElem[3][3];

	return *this;
}


CNtlNaviMatrix4& CNtlNaviMatrix4::operator *= ( const CNtlNaviMatrix4& m4Val )
{
	*this = *this * m4Val;

	return *this;
}


CNtlNaviMatrix4::operator CNtlNaviMatrix3 ( void )
{
	return CNtlNaviMatrix3(
		m_fDAElem[0][0], m_fDAElem[0][1], m_fDAElem[0][2],
		m_fDAElem[1][0], m_fDAElem[1][1], m_fDAElem[1][2],
		m_fDAElem[2][0], m_fDAElem[2][1], m_fDAElem[2][2] );
}


CNtlNaviMatrix4::operator float* ( void )
{
	return m_fSAElem;
}


CNtlNaviMatrix4::operator const float* ( void ) const
{
	return m_fSAElem;
}


CNtlNaviMatrix4 operator * ( float fVal1, const CNtlNaviMatrix4& m4Val2 )
{
	return m4Val2 * fVal1;
}


CNtlNaviMatrix4 operator / ( float fVal1, const CNtlNaviMatrix4& m4Val2 )
{
	return m4Val2 / fVal1;
}


CNtlNaviVector4 operator * ( const CNtlNaviVector4& v4Val1, const CNtlNaviMatrix4& m4Val2 )
{
	const float* pVal = v4Val1;

	return CNtlNaviVector4(
		pVal[0] * m4Val2.m_fDAElem[0][0] + pVal[1] * m4Val2.m_fDAElem[1][0] + pVal[2] * m4Val2.m_fDAElem[2][0] + pVal[3] * m4Val2.m_fDAElem[3][0],
		pVal[0] * m4Val2.m_fDAElem[0][1] + pVal[1] * m4Val2.m_fDAElem[1][1] + pVal[2] * m4Val2.m_fDAElem[2][1] + pVal[3] * m4Val2.m_fDAElem[3][1],
		pVal[0] * m4Val2.m_fDAElem[0][2] + pVal[1] * m4Val2.m_fDAElem[1][2] + pVal[2] * m4Val2.m_fDAElem[2][2] + pVal[3] * m4Val2.m_fDAElem[3][2],
		pVal[0] * m4Val2.m_fDAElem[0][3] + pVal[1] * m4Val2.m_fDAElem[1][3] + pVal[2] * m4Val2.m_fDAElem[2][3] + pVal[3] * m4Val2.m_fDAElem[3][3] );
}


CNtlNaviVector4 operator * ( const CNtlNaviMatrix4& m4Val1, const CNtlNaviVector4& v4Val2 )
{
	const float* pVal = v4Val2;

	return CNtlNaviVector4(
		m4Val1.m_fDAElem[0][0] * pVal[0] + m4Val1.m_fDAElem[0][1] * pVal[1] + m4Val1.m_fDAElem[0][2] * pVal[2] + m4Val1.m_fDAElem[0][3] * pVal[3],
		m4Val1.m_fDAElem[1][0] * pVal[0] + m4Val1.m_fDAElem[1][1] * pVal[1] + m4Val1.m_fDAElem[1][2] * pVal[2] + m4Val1.m_fDAElem[1][3] * pVal[3],
		m4Val1.m_fDAElem[2][0] * pVal[0] + m4Val1.m_fDAElem[2][1] * pVal[1] + m4Val1.m_fDAElem[2][2] * pVal[2] + m4Val1.m_fDAElem[2][3] * pVal[3],
		m4Val1.m_fDAElem[3][0] * pVal[0] + m4Val1.m_fDAElem[3][1] * pVal[1] + m4Val1.m_fDAElem[3][2] * pVal[2] + m4Val1.m_fDAElem[3][3] * pVal[3] );
}


int CNtlNaviMatrix4::CompareArrays( const CNtlNaviMatrix4& m4Val ) const
{
	return memcmp( m_fSAElem, m4Val.m_fSAElem, sizeof(float)*16 );
}

