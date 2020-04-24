//////////////////////////////////////////////////////////////////////////
/**
*	Vector3 Å¬·¡½º
*
*	\file		NtlNaviVector3.inl
*	\author		Jeong Ho, Rho
*/
//////////////////////////////////////////////////////////////////////////



CNtlNaviVector3::CNtlNaviVector3( bool bZero )
{
	if ( bZero ) MakeZero();
}


CNtlNaviVector3::CNtlNaviVector3( const CNtlNaviVector3& v3Val )
{
	SetElem( v3Val );
}


CNtlNaviVector3::CNtlNaviVector3( float fX, float fY, float fZ )
{
	SetElem( fX, fY, fZ );
}


CNtlNaviVector3::CNtlNaviVector3( const float* pfVal )
{
	SetElem( pfVal );
}


float& CNtlNaviVector3::GetX( void )
{
	return m_fElem[0];
}


const float& CNtlNaviVector3::GetX( void ) const
{
	return m_fElem[0];
}


float& CNtlNaviVector3::GetY( void )
{
	return m_fElem[1];
}


const float& CNtlNaviVector3::GetY( void ) const
{
	return m_fElem[1];
}


float& CNtlNaviVector3::GetZ( void )
{
	return m_fElem[2];
}


const float& CNtlNaviVector3::GetZ( void ) const
{
	return m_fElem[2];
}


void CNtlNaviVector3::GetXYZ( float& fX, float& fY, float& fZ ) const
{
	fX = m_fElem[0];
	fY = m_fElem[1];
	fZ = m_fElem[2];
}


float& CNtlNaviVector3::GetElem( int nIdx )
{
	return m_fElem[nIdx];
}


const float& CNtlNaviVector3::GetElem( int nIdx ) const
{
	return m_fElem[nIdx];
}


void CNtlNaviVector3::SetElem( float fVal )
{
	m_fElem[0] = m_fElem[1] = m_fElem[2] = fVal;
}


void CNtlNaviVector3::SetElem( int nIdx, float fVal )
{
	m_fElem[nIdx] = fVal;
}


void CNtlNaviVector3::SetElem( const CNtlNaviVector3& v3Val )
{
	m_fElem[0] = v3Val.m_fElem[0];	m_fElem[1] = v3Val.m_fElem[1];	m_fElem[2] = v3Val.m_fElem[2];
}


void CNtlNaviVector3::SetElem( float fX, float fY, float fZ )
{
	m_fElem[0] = fX;	m_fElem[1] = fY;	m_fElem[2] = fZ;
}


void CNtlNaviVector3::SetElem( float* pfVal )
{
	m_fElem[0] = pfVal[0];	m_fElem[1] = pfVal[1];	m_fElem[2] = pfVal[2];
}


void CNtlNaviVector3::MakeInvalid( void )
{
	m_fElem[0] = NAVI_FLT_MAX;	m_fElem[1] = NAVI_FLT_MAX;	m_fElem[2] = NAVI_FLT_MAX;
}


void CNtlNaviVector3::MakeUnitX( void )
{
	m_fElem[0] = 1.f;	m_fElem[1] = 0.f;	m_fElem[2] = 0.f;
}


void CNtlNaviVector3::MakeUnitY( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 1.f;	m_fElem[2] = 0.f;
}


void CNtlNaviVector3::MakeUnitZ( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 0.f;	m_fElem[2] = 1.f;
}


void CNtlNaviVector3::MakeZero( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 0.f;	m_fElem[2] = 0.f;
}


CNtlNaviVector3 CNtlNaviVector3::GetNormalize( void ) const
{
	float fLength = GetLength();

	if ( fLength <= NAVI_ZERO_THRESHOLD )
	{
		return CNtlNaviVector3( NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX );
	}

	fLength = 1.f / fLength;

	return CNtlNaviVector3( m_fElem[0] * fLength, m_fElem[1] * fLength, m_fElem[2] * fLength );
}


void CNtlNaviVector3::MakeNormalize( void )
{
	*this = GetNormalize();
}


void CNtlNaviVector3::MakeLerp( const CNtlNaviVector3& v3Val1, const CNtlNaviVector3& v3Val2, float t )
{
	m_fElem[0] = v3Val1.m_fElem[0] + t * ( v3Val2.m_fElem[0] - v3Val1.m_fElem[0] );
	m_fElem[1] = v3Val1.m_fElem[1] + t * ( v3Val2.m_fElem[1] - v3Val1.m_fElem[1] );
	m_fElem[2] = v3Val1.m_fElem[2] + t * ( v3Val2.m_fElem[2] - v3Val1.m_fElem[2] );
}


float CNtlNaviVector3::GetLength( void ) const
{
	return sqrtf( m_fElem[0] * m_fElem[0] + m_fElem[1] * m_fElem[1] + m_fElem[2] * m_fElem[2] );
}


float CNtlNaviVector3::GetLengthSquared( void ) const
{
	return m_fElem[0] * m_fElem[0] + m_fElem[1] * m_fElem[1] + m_fElem[2] * m_fElem[2];
}


float CNtlNaviVector3::GetDot( const CNtlNaviVector3& v3Val ) const
{
	return m_fElem[0] * v3Val.m_fElem[0] + m_fElem[1] * v3Val.m_fElem[1] + m_fElem[2] * v3Val.m_fElem[2];
}


CNtlNaviVector3 CNtlNaviVector3::GetCross( const CNtlNaviVector3& v3Val ) const
{
	return CNtlNaviVector3(
		m_fElem[1] * v3Val.m_fElem[2] - m_fElem[2] * v3Val.m_fElem[1],
		m_fElem[2] * v3Val.m_fElem[0] - m_fElem[0] * v3Val.m_fElem[2],
		m_fElem[0] * v3Val.m_fElem[1] - m_fElem[1] * v3Val.m_fElem[0] );
}


void CNtlNaviVector3::MakeCross( const CNtlNaviVector3& v3Val )
{
	*this = GetCross( v3Val );
}


float CNtlNaviVector3::GetVolume( void ) const
{
	return m_fElem[0] * m_fElem[1] * m_fElem[2];
}


float CNtlNaviVector3::GetDistance( const CNtlNaviVector3& v3Val ) const
{
	return sqrtf(
		(m_fElem[0] - v3Val.m_fElem[0]) * (m_fElem[0] - v3Val.m_fElem[0]) +
		(m_fElem[1] - v3Val.m_fElem[1]) * (m_fElem[1] - v3Val.m_fElem[1]) +
		(m_fElem[2] - v3Val.m_fElem[2]) * (m_fElem[2] - v3Val.m_fElem[2]) );
}


float CNtlNaviVector3::GetDistanceSquared( const CNtlNaviVector3& v3Val ) const
{
	return
		(m_fElem[0] - v3Val.m_fElem[0]) * (m_fElem[0] - v3Val.m_fElem[0]) +
		(m_fElem[1] - v3Val.m_fElem[1]) * (m_fElem[1] - v3Val.m_fElem[1]) +
		(m_fElem[2] - v3Val.m_fElem[2]) * (m_fElem[2] - v3Val.m_fElem[2]);
}


CNtlNaviVector3& CNtlNaviVector3::operator = ( const CNtlNaviVector3& v3Val )
{
	m_fElem[0] = v3Val.m_fElem[0];	m_fElem[1] = v3Val.m_fElem[1];	m_fElem[2] = v3Val.m_fElem[2];

	return *this;
}


float& CNtlNaviVector3::operator [] ( int nIdx )
{
	return m_fElem[nIdx];
}


const float& CNtlNaviVector3::operator [] ( int nIdx ) const
{
	return m_fElem[nIdx];
}


float& CNtlNaviVector3::operator () ( int nIdx )
{
	return m_fElem[nIdx];
}


const float& CNtlNaviVector3::operator () ( int nIdx ) const
{
	return m_fElem[nIdx];
}


bool CNtlNaviVector3::operator == ( const CNtlNaviVector3& v3Val ) const
{
	return CompareArrays( v3Val ) == 0;
}


bool CNtlNaviVector3::operator != ( const CNtlNaviVector3& v3Val ) const
{
	return CompareArrays( v3Val ) != 0;
}


CNtlNaviVector3 CNtlNaviVector3::operator - ( void ) const
{
	return CNtlNaviVector3( -m_fElem[0], -m_fElem[1], -m_fElem[2] );
}


CNtlNaviVector3 CNtlNaviVector3::operator * ( float fVal ) const
{
	return CNtlNaviVector3( m_fElem[0] * fVal, m_fElem[1] * fVal, m_fElem[2] * fVal );
}


CNtlNaviVector3 CNtlNaviVector3::operator * ( const CNtlNaviVector3& v3Val ) const
{
	return CNtlNaviVector3( m_fElem[0] * v3Val.m_fElem[0], m_fElem[1] * v3Val.m_fElem[1], m_fElem[2] * v3Val.m_fElem[2] );
}


CNtlNaviVector3 CNtlNaviVector3::operator / ( float fVal ) const
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		return CNtlNaviVector3( NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX );
	}

	fVal = 1.f / fVal;

	return CNtlNaviVector3( m_fElem[0] * fVal, m_fElem[1] * fVal, m_fElem[2] * fVal );
}


CNtlNaviVector3 CNtlNaviVector3::operator / ( const CNtlNaviVector3& v3Val ) const
{
	return CNtlNaviVector3( m_fElem[0] / v3Val.m_fElem[0], m_fElem[1] / v3Val.m_fElem[1], m_fElem[2] / v3Val.m_fElem[2] );
}


CNtlNaviVector3 CNtlNaviVector3::operator + ( const CNtlNaviVector3& v3Val ) const
{
	return CNtlNaviVector3( m_fElem[0] + v3Val.m_fElem[0], m_fElem[1] + v3Val.m_fElem[1], m_fElem[2] + v3Val.m_fElem[2] );
}


CNtlNaviVector3 CNtlNaviVector3::operator - ( const CNtlNaviVector3& v3Val ) const
{
	return CNtlNaviVector3( m_fElem[0] - v3Val.m_fElem[0], m_fElem[1] - v3Val.m_fElem[1], m_fElem[2] - v3Val.m_fElem[2] );
}


CNtlNaviVector3& CNtlNaviVector3::operator *= ( float fVal )
{
	m_fElem[0] *= fVal;	m_fElem[1] *= fVal;	m_fElem[2] *= fVal;

	return *this;
}


CNtlNaviVector3& CNtlNaviVector3::operator *= ( const CNtlNaviVector3& v3Val )
{
	m_fElem[0] *= v3Val.m_fElem[0];	m_fElem[1] *= v3Val.m_fElem[1];	m_fElem[2] *= v3Val.m_fElem[2];

	return *this;
}


CNtlNaviVector3& CNtlNaviVector3::operator /= ( float fVal )
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		m_fElem[0] = NAVI_FLT_MAX;	m_fElem[1] = NAVI_FLT_MAX;	m_fElem[2] = NAVI_FLT_MAX;
	}
	else
	{
		fVal = 1.f / fVal;

		m_fElem[0] *= fVal;	m_fElem[1] *= fVal;	m_fElem[2] *= fVal;
	}

	return *this;
}


CNtlNaviVector3& CNtlNaviVector3::operator /= ( const CNtlNaviVector3& v3Val )
{
	m_fElem[0] /= v3Val.m_fElem[0];	m_fElem[1] /= v3Val.m_fElem[1];	m_fElem[2] /= v3Val.m_fElem[2];

	return *this;
}


CNtlNaviVector3& CNtlNaviVector3::operator += ( const CNtlNaviVector3& v3Val )
{
	m_fElem[0] += v3Val.m_fElem[0];	m_fElem[1] += v3Val.m_fElem[1];	m_fElem[2] += v3Val.m_fElem[2];

	return *this;
}


CNtlNaviVector3& CNtlNaviVector3::operator -= ( const CNtlNaviVector3& v3Val )
{
	m_fElem[0] -= v3Val.m_fElem[0];	m_fElem[1] -= v3Val.m_fElem[1];	m_fElem[2] -= v3Val.m_fElem[2];

	return *this;
}


CNtlNaviVector3::operator float* ( void )
{
	return m_fElem;
}


CNtlNaviVector3::operator const float* ( void ) const
{
	return m_fElem;
}


float CNtlNaviVector3::operator | ( const CNtlNaviVector3& v3Val ) const
{
	return m_fElem[0] * v3Val.m_fElem[0] + m_fElem[1] * v3Val.m_fElem[1] + m_fElem[2] * v3Val.m_fElem[2];
}


CNtlNaviVector3 CNtlNaviVector3::operator ^ ( const CNtlNaviVector3& v3Val ) const
{
	return CNtlNaviVector3(
		m_fElem[1] * v3Val.m_fElem[2] - m_fElem[2] * v3Val.m_fElem[1],
		m_fElem[2] * v3Val.m_fElem[0] - m_fElem[0] * v3Val.m_fElem[2],
		m_fElem[0] * v3Val.m_fElem[1] - m_fElem[1] * v3Val.m_fElem[0] );
}


CNtlNaviVector3& CNtlNaviVector3::operator |= ( const CNtlNaviVector3& v3Val )
{
	*this = *this * (*this | v3Val);

	return *this;
}


CNtlNaviVector3& CNtlNaviVector3::operator ^= ( const CNtlNaviVector3& v3Val )
{
	*this = *this ^ v3Val;

	return *this;
}


CNtlNaviVector3 operator * ( float fVal1, const CNtlNaviVector3& v3Val2 )
{
	return CNtlNaviVector3( fVal1 * v3Val2.m_fElem[0], fVal1 * v3Val2.m_fElem[1], fVal1 * v3Val2.m_fElem[2] );
}


CNtlNaviVector3 operator / ( float fVal1, const CNtlNaviVector3& v3Val2 )
{
	if ( fabsf( fVal1 ) <= NAVI_ZERO_THRESHOLD )
	{
		return CNtlNaviVector3( NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX );
	}

	fVal1 = 1.f / fVal1;

	return CNtlNaviVector3( fVal1 * v3Val2.m_fElem[0], fVal1 * v3Val2.m_fElem[1], fVal1 * v3Val2.m_fElem[2] );
}


int CNtlNaviVector3::CompareArrays( const CNtlNaviVector3& v3Val ) const
{
	return memcmp( m_fElem, v3Val.m_fElem, sizeof(float)*3 );
}
