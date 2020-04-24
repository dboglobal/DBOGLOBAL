//////////////////////////////////////////////////////////////////////////
/**
 *	Vector4 Å¬·¡½º.
 *
 *	\file		CNtlNaviVector4.inl
 *	\author		Jeong Ho, Rho
*/
//////////////////////////////////////////////////////////////////////////


CNtlNaviVector4::CNtlNaviVector4( bool bZero )
{
	if ( bZero ) MakeZero();
}


CNtlNaviVector4::CNtlNaviVector4( const CNtlNaviVector4& v4Val )
{
	SetElem( v4Val );
}


CNtlNaviVector4::CNtlNaviVector4( float fX, float fY, float fZ, float fW )
{
	SetElem( fX, fY, fZ, fW );
}


CNtlNaviVector4::CNtlNaviVector4( const float* pfVal )
{
	SetElem( pfVal );
}


float& CNtlNaviVector4::GetX( void )
{
	return m_fElem[0];
}


const float& CNtlNaviVector4::GetX( void ) const
{
	return m_fElem[0];
}


float& CNtlNaviVector4::GetY( void )
{
	return m_fElem[1];
}


const float& CNtlNaviVector4::GetY( void ) const
{
	return m_fElem[1];
}


float& CNtlNaviVector4::GetZ( void )
{
	return m_fElem[2];
}


const float& CNtlNaviVector4::GetZ( void ) const
{
	return m_fElem[2];
}


float& CNtlNaviVector4::GetW( void )
{
	return m_fElem[3];
}


const float& CNtlNaviVector4::GetW( void ) const
{
	return m_fElem[3];
}


void CNtlNaviVector4::GetXYZW( float& fX, float& fY, float& fZ, float& fW ) const
{
	fX = m_fElem[0];	fY = m_fElem[1];	fZ = m_fElem[2];	fW = m_fElem[3];
}


float& CNtlNaviVector4::GetElem( int nIdx )
{
	return m_fElem[nIdx];
}


const float& CNtlNaviVector4::GetElem( int nIdx ) const
{
	return m_fElem[nIdx];
}


void CNtlNaviVector4::SetElem( float fVal )
{
	m_fElem[0] = m_fElem[1] = m_fElem[2] = m_fElem[3] = fVal;
}


void CNtlNaviVector4::SetElem( const CNtlNaviVector4& v4Val )
{
	m_fElem[0] = v4Val.m_fElem[0];	m_fElem[1] = v4Val.m_fElem[1];	m_fElem[2] = v4Val.m_fElem[2];	m_fElem[3] = v4Val.m_fElem[3];
}


void CNtlNaviVector4::SetElem( float fX, float fY, float fZ, float fW )
{
	m_fElem[0] = fX;	m_fElem[1] = fY;	m_fElem[2] = fZ;	m_fElem[3] = fW;
}


void CNtlNaviVector4::SetElem( float* pfVal )
{
	m_fElem[0] = pfVal[0];	m_fElem[1] = pfVal[1];	m_fElem[2] = pfVal[2];	m_fElem[3] = pfVal[3];
}


void CNtlNaviVector4::MakeInvalid( void )
{
	m_fElem[0] = NAVI_FLT_MAX;	m_fElem[1] = NAVI_FLT_MAX;	m_fElem[2] = NAVI_FLT_MAX;	m_fElem[3] = NAVI_FLT_MAX;
}


void CNtlNaviVector4::MakeUnitX( void )
{
	m_fElem[0] = 1.f;	m_fElem[1] = 0.f;	m_fElem[2] = 0.f;	m_fElem[3] = 0.f;
}


void CNtlNaviVector4::MakeUnitY( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 1.f;	m_fElem[2] = 0.f;	m_fElem[3] = 0.f;
}


void CNtlNaviVector4::MakeUnitZ( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 0.f;	m_fElem[2] = 1.f;	m_fElem[3] = 0.f;
}


void CNtlNaviVector4::MakeUnitW( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 0.f;	m_fElem[2] = 0.f;	m_fElem[3] = 1.f;
}


void CNtlNaviVector4::MakeZero( void )
{
	m_fElem[0] = 0.f;	m_fElem[1] = 0.f;	m_fElem[2] = 0.f;	m_fElem[3] = 0.f;
}


CNtlNaviVector4 CNtlNaviVector4::GetNormalize( void ) const
{
	CNtlNaviVector4 v4Temp;

	float fLength = GetLength();

	if ( fLength <= NAVI_ZERO_THRESHOLD )
	{
		v4Temp.MakeInvalid();
		return v4Temp;
	}
	else
	{
		fLength = 1.f / fLength;

		v4Temp.m_fElem[0] *= fLength;	v4Temp.m_fElem[1] *= fLength;	v4Temp.m_fElem[2] *= fLength;	v4Temp.m_fElem[3] *= fLength;

		return v4Temp;
	}
}


void CNtlNaviVector4::MakeNormalize( void )
{
	*this = GetNormalize();
}


float CNtlNaviVector4::GetLength( void ) const
{
	return sqrtf( m_fElem[0] * m_fElem[0] + m_fElem[1] * m_fElem[1] + m_fElem[2] * m_fElem[2] + m_fElem[3] * m_fElem[3] );
}


float CNtlNaviVector4::GetLengthSquared( void ) const
{
	return m_fElem[0] * m_fElem[0] + m_fElem[1] * m_fElem[1] + m_fElem[2] * m_fElem[2] + m_fElem[3] * m_fElem[3];
}


CNtlNaviVector4& CNtlNaviVector4::operator = ( const CNtlNaviVector4& v4Val )
{
	m_fElem[0] = v4Val.m_fElem[0];	m_fElem[1] = v4Val.m_fElem[1];	m_fElem[2] = v4Val.m_fElem[2];	m_fElem[3] = v4Val.m_fElem[3];

	return *this;
}


float& CNtlNaviVector4::operator [] ( int nIdx )
{
	return m_fElem[nIdx];
}


const float& CNtlNaviVector4::operator [] ( int nIdx ) const
{
	return m_fElem[nIdx];
}


float& CNtlNaviVector4::operator () ( int nIdx )
{
	return m_fElem[nIdx];
}


const float& CNtlNaviVector4::operator () ( int nIdx ) const
{
	return m_fElem[nIdx];
}


bool CNtlNaviVector4::operator == ( const CNtlNaviVector4& v4Val ) const
{
	return CompareArrays( v4Val ) == 0;
}


bool CNtlNaviVector4::operator != ( const CNtlNaviVector4& v4Val ) const
{
	return CompareArrays( v4Val ) != 0;
}


CNtlNaviVector4 CNtlNaviVector4::operator - ( void ) const
{
	return CNtlNaviVector4( -m_fElem[0], -m_fElem[1], -m_fElem[2], -m_fElem[3] );
}


CNtlNaviVector4 CNtlNaviVector4::operator * ( float fVal ) const
{
	return CNtlNaviVector4( m_fElem[0] * fVal, m_fElem[1] * fVal, m_fElem[2] * fVal, m_fElem[3] * fVal );
}


CNtlNaviVector4 CNtlNaviVector4::operator * ( const CNtlNaviVector4& v4Val ) const
{
	return CNtlNaviVector4( m_fElem[0] * v4Val.m_fElem[0], m_fElem[1] * v4Val.m_fElem[1], m_fElem[2] * v4Val.m_fElem[2], m_fElem[3] * v4Val.m_fElem[3] );
}


CNtlNaviVector4 CNtlNaviVector4::operator / ( float fVal ) const
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		return CNtlNaviVector4( NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX );
	}

	fVal = 1.f / fVal;

	return CNtlNaviVector4( m_fElem[0] * fVal, m_fElem[1] * fVal, m_fElem[2] * fVal, m_fElem[3] * fVal );
}


CNtlNaviVector4 CNtlNaviVector4::operator / ( const CNtlNaviVector4& v4Val ) const
{
	return CNtlNaviVector4( m_fElem[0] / v4Val.m_fElem[0], m_fElem[1] / v4Val.m_fElem[1], m_fElem[2] / v4Val.m_fElem[2], m_fElem[3] / v4Val.m_fElem[3] );
}


CNtlNaviVector4 CNtlNaviVector4::operator + ( const CNtlNaviVector4& v4Val ) const
{
	return CNtlNaviVector4( m_fElem[0] + v4Val.m_fElem[0], m_fElem[1] + v4Val.m_fElem[1], m_fElem[2] + v4Val.m_fElem[2], m_fElem[3] + v4Val.m_fElem[3] );
}


CNtlNaviVector4 CNtlNaviVector4::operator - ( const CNtlNaviVector4& v4Val ) const
{
	return CNtlNaviVector4( m_fElem[0] - v4Val.m_fElem[0], m_fElem[1] - v4Val.m_fElem[1], m_fElem[2] - v4Val.m_fElem[2], m_fElem[3] - v4Val.m_fElem[3] );
}


CNtlNaviVector4& CNtlNaviVector4::operator *= ( float fVal )
{
	m_fElem[0] *= fVal;	m_fElem[1] *= fVal;	m_fElem[2] *= fVal;	m_fElem[3] *= fVal;

	return *this;
}


CNtlNaviVector4& CNtlNaviVector4::operator *= ( const CNtlNaviVector4& v4Val )
{
	m_fElem[0] *= v4Val.m_fElem[0];	m_fElem[1] *= v4Val.m_fElem[1];	m_fElem[2] *= v4Val.m_fElem[2];	m_fElem[3] *= v4Val.m_fElem[3];

	return *this;
}


CNtlNaviVector4& CNtlNaviVector4::operator /= ( float fVal )
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		MakeInvalid();
	}
	else
	{
		fVal = 1.f / fVal;

		m_fElem[0] *= fVal;	m_fElem[1] *= fVal;	m_fElem[2] *= fVal;	m_fElem[3] *= fVal;
	}

	return *this;
}


CNtlNaviVector4& CNtlNaviVector4::operator /= ( const CNtlNaviVector4& v4Val )
{
	m_fElem[0] /= v4Val.m_fElem[0];	m_fElem[1] /= v4Val.m_fElem[1];	m_fElem[2] /= v4Val.m_fElem[2];	m_fElem[3] /= v4Val.m_fElem[3];

	return *this;
}


CNtlNaviVector4& CNtlNaviVector4::operator += ( const CNtlNaviVector4& v4Val )
{
	m_fElem[0] += v4Val.m_fElem[0];	m_fElem[1] += v4Val.m_fElem[1];	m_fElem[2] += v4Val.m_fElem[2];	m_fElem[3] += v4Val.m_fElem[3];

	return *this;
}


CNtlNaviVector4& CNtlNaviVector4::operator -= ( const CNtlNaviVector4& v4Val )
{
	m_fElem[0] -= v4Val.m_fElem[0];	m_fElem[1] -= v4Val.m_fElem[1];	m_fElem[2] -= v4Val.m_fElem[2];	m_fElem[3] -= v4Val.m_fElem[3];

	return *this;
}


CNtlNaviVector4::operator float* ( void )
{
	return m_fElem;
}


CNtlNaviVector4::operator const float* ( void ) const
{
	return m_fElem;
}


int CNtlNaviVector4::CompareArrays( const CNtlNaviVector4& v4Val ) const
{
	return memcmp( m_fElem, v4Val.m_fElem, sizeof(float)*4 );
}
