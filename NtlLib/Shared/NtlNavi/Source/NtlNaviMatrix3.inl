//////////////////////////////////////////////////////////////////////////
/**
 *	Matrix3 클래스
 *	왼손 좌표계, 행벡터를 기준으로 구현된 3 X 3 행렬.
 *
 *	\file		NtlNaviMatrix3.h
 *	\author		Jeong Ho, Rho
*/
//////////////////////////////////////////////////////////////////////////


CNtlNaviMatrix3::CNtlNaviMatrix3( bool bIdentity )
{
	if ( bIdentity ) MakeIdentity();
}


CNtlNaviMatrix3::CNtlNaviMatrix3( const CNtlNaviMatrix3& m3Val )
{
	memcpy( m_fSAElem, m3Val.m_fSAElem, sizeof(float)*9 );
}


CNtlNaviMatrix3::CNtlNaviMatrix3( const CNtlNaviVector3& v3ValX,
								  const CNtlNaviVector3& v3ValY,
								  const CNtlNaviVector3& v3ValZ )
{
	SetElem( v3ValX, v3ValY, v3ValZ );
}


CNtlNaviMatrix3::CNtlNaviMatrix3( float f00, float f01, float f02,
								  float f10, float f11, float f12,
								  float f20, float f21, float f22 )
{
	SetElem( f00, f01, f02, f10, f11, f12, f20, f21, f22 );
}


CNtlNaviMatrix3::CNtlNaviMatrix3( const float* pfVal )
{
	SetElem( pfVal );
}


float CNtlNaviMatrix3::GetElem( int nIdx ) const
{
	return m_fSAElem[nIdx];
}


float CNtlNaviMatrix3::GetElem( int nRow, int nCol ) const
{
	return m_fDAElem[nRow][nCol];
}


void CNtlNaviMatrix3::SetElem( int nRow, int nCol, float fVal )
{
	m_fDAElem[nRow][nCol] = fVal;
}


void CNtlNaviMatrix3::SetElem( float f00, float f01, float f02,
							   float f10, float f11, float f12,
							   float f20, float f21, float f22 )
{
	m_fDAElem[0][0] = f00;	m_fDAElem[0][1] = f01;	m_fDAElem[0][2] = f02;
	m_fDAElem[1][0] = f10;	m_fDAElem[1][1] = f11;	m_fDAElem[1][2] = f12;
	m_fDAElem[2][0] = f20;	m_fDAElem[2][1] = f21;	m_fDAElem[2][2] = f22;
}


void CNtlNaviMatrix3::SetElem( const CNtlNaviVector3& v3ValX,
							   const CNtlNaviVector3& v3ValY,
							   const CNtlNaviVector3& v3ValZ )
{
	v3ValX.GetXYZ( m_fDAElem[0][0], m_fDAElem[0][1], m_fDAElem[0][2] );
	v3ValY.GetXYZ( m_fDAElem[1][0], m_fDAElem[1][1], m_fDAElem[1][2] );
	v3ValZ.GetXYZ( m_fDAElem[2][0], m_fDAElem[2][1], m_fDAElem[2][2] );
}


void CNtlNaviMatrix3::SetElem( const float* pfVal )
{
	memcpy( m_fSAElem, pfVal, sizeof(float)*9 );
}


CNtlNaviVector3& CNtlNaviMatrix3::GetRow( int nRow )
{
	return *(CNtlNaviVector3*)&m_fDAElem[nRow][0];
}


const CNtlNaviVector3& CNtlNaviMatrix3::GetRow( int nRow ) const
{
	return *(const CNtlNaviVector3*)&m_fDAElem[nRow][0];
}


void CNtlNaviMatrix3::GetRow( int nRow, CNtlNaviVector3& v3Val ) const
{
	v3Val.SetElem( m_fDAElem[nRow][0], m_fDAElem[nRow][1], m_fDAElem[nRow][2] );
}


void CNtlNaviMatrix3::SetRow( int nRow, const CNtlNaviVector3& v3Val )
{
	v3Val.GetXYZ( m_fDAElem[nRow][0], m_fDAElem[nRow][1], m_fDAElem[nRow][2] );
}


const CNtlNaviVector3 CNtlNaviMatrix3::GetCol( int nRow ) const
{
	return CNtlNaviVector3( m_fDAElem[nRow][0], m_fDAElem[nRow][1], m_fDAElem[nRow][2] );
}


void CNtlNaviMatrix3::GetCol( int nCol, CNtlNaviVector3& v3Val ) const
{
	v3Val.SetElem( m_fDAElem[0][nCol], m_fDAElem[1][nCol], m_fDAElem[2][nCol] );
}


void CNtlNaviMatrix3::SetCol( int nCol, const CNtlNaviVector3& v3Val )
{
	v3Val.GetXYZ( m_fDAElem[0][nCol], m_fDAElem[1][nCol], m_fDAElem[2][nCol] );
}

void CNtlNaviMatrix3::Scale( float fX, float fY, float fZ )
{
	m_fDAElem[0][0] *= fX;	m_fDAElem[0][1] *= fX;	m_fDAElem[0][2] *= fX;
	m_fDAElem[1][0] *= fY;	m_fDAElem[1][1] *= fY;	m_fDAElem[1][2] *= fY;
	m_fDAElem[2][0] *= fZ;	m_fDAElem[2][1] *= fZ;	m_fDAElem[2][2] *= fZ;
}

void CNtlNaviMatrix3::MakeInvalid( void )
{
	m_fDAElem[0][0] = NAVI_FLT_MAX;	m_fDAElem[0][1] = NAVI_FLT_MAX;	m_fDAElem[0][2] = NAVI_FLT_MAX;
	m_fDAElem[1][0] = NAVI_FLT_MAX;	m_fDAElem[1][1] = NAVI_FLT_MAX;	m_fDAElem[1][2] = NAVI_FLT_MAX;
	m_fDAElem[2][0] = NAVI_FLT_MAX;	m_fDAElem[2][1] = NAVI_FLT_MAX;	m_fDAElem[2][2] = NAVI_FLT_MAX;
}


void CNtlNaviMatrix3::MakeZero( void )
{
	m_fDAElem[0][0] = 0.f;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = 0.f;	m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = 0.f;
}


void CNtlNaviMatrix3::MakeIdentity( void )
{
	m_fDAElem[0][0] = 1.f;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = 1.f;	m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = 1.f;
}


void CNtlNaviMatrix3::MakeDiagonal( float fVal )
{
	m_fDAElem[0][0] = fVal;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = fVal;	m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = fVal;
}


void CNtlNaviMatrix3::MakeDiagonal( float f00, float f11, float f22 )
{
	m_fDAElem[0][0] = f00;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = f11;	m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = f22;
}


void CNtlNaviMatrix3::MakeDiagonal( const CNtlNaviVector3& v3Val )
{
	v3Val.GetXYZ( m_fDAElem[0][0], m_fDAElem[1][1], m_fDAElem[2][2] );

									m_fDAElem[0][1] = 0.f;			m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = 0.f;											m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = 0.f;			m_fDAElem[2][1] = 0.f;
}


void CNtlNaviMatrix3::GetEulerRotXYZ( float& fX, float& fY, float& fZ ) const
{
	//
	//	P' = P * RMX(fX) * RMY(fY) * RMZ(fZ)
	//
	//	|	Cy * Cz						Cy * Sz						-Sy			|
	//	|	Cz * Sx * Sy - Cx * Sz		Sx * Sy * Sz + Cx * Cz		Cy * Sx		|
	//	|	Cx * Cz * Sy + Sx * Sz		Cx * Sy * Sz - Cz * Sx		Cy * Cx		|
	//

	if ( m_fDAElem[0][2] < 1.f )
	{
		if ( m_fDAElem[0][2] > -1.f )
		{
			//	-1 < Sy < 1 <=> 0 < y < NAVI_PI / 2 || NAVI_PI / 2 < y < NAVI_PI <=> Cy != 0
			//	따라서, Cy로 나누기가 가능하다

			//	(Cy * Sx) / (Cy * Cx)
			fX = atan2f( m_fDAElem[1][2], m_fDAElem[2][2] );

			//	-sinf(y) = R[0][2] <=> sinf(y) = -R[0][2] <=> y = asinf( -R[0][2] )
			fY = asinf( -m_fDAElem[0][2] );

			//	(Cy * Sz) / (Cy * Cz)
			fZ = atan2f( m_fDAElem[0][1], m_fDAElem[0][0] );
		}
		//	정규직교 행렬을 대상으로 하기 때문에 -Sy 는 -1 보다 작을 수 없다
		//	여기서는 -Sy가 -1보다 작은 경우 -1로 처리한다
		else
		{
			fY = NAVI_PI / 2.f;

			//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
			//
			//	|	0					0			-1	|
			//	|	S(x-z)				C(x-z)		0	|
			//	|	C(x-z)				-S(x-z)		0	|
			//
			//	여기서, x, z는 무수히 많은 해를 가진다.
			//	따라서, X -> Y -> Z의 순서로 회전 행렬이 구성되므로
			//	편의상 x = 0 으로 놓고 z의 회전값을 결정한다

			fX = 0.f;
			fZ = -atan2f( m_fDAElem[1][0], m_fDAElem[2][0] );
		}
	}
	//	정규직교 행렬을 대상으로 하기 때문에 -Sy 는 1 보다 클 수 없다
	//	여기서는 -Sy가 1보다 큰 경우 1로 처리한다
	else
	{
		fY = -NAVI_PI / 2.f;

		//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
		//
		//	|	0					0			1	|
		//	|	-S(x+z)				C(x+z)		0	|
		//	|	-C(x+z)				-S(x+z)		0	|
		//
		//	여기서, x, z는 무수히 많은 해를 가진다.
		//	따라서, X -> Y -> Z의 순서로 회전 행렬이 구성되므로
		//	편의상 x = 0 으로 놓고 z의 회전값을 결정한다

		fX = 0.f;
		fZ = atan2f( m_fDAElem[1][0], m_fDAElem[2][0] );
	}
}


void CNtlNaviMatrix3::GetEulerRotXZY( float& fX, float& fY, float& fZ ) const
{
	//
	//	임의의 점 P를 RMX(fX) -> RMZ(fZ) -> RMY(fY)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMX(fX) * RMZ(fZ) * RMY(fY)
	//
	//	|	Cz * Cy						Sz			-Cz * Sy				|
    //	|	Sx * Sy - Cx * Cy * Sz		Cz * Cx		Cy * Sx + Cx * Sy * Sz	|
    //	|	Cx * Sy + Cy * Sx * Sz		-Cz * Sx	Cx * Cy - Sx * Sy * Sz	|
	//

	if ( m_fDAElem[0][1] < 1.f )
	{
		if ( m_fDAElem[0][1] > -1.f )
		{
			//	-1 < Sz < 1 <=> 0 < z < NAVI_PI / 2 || NAVI_PI / 2 < z < NAVI_PI <=> Cz != 0
			//	따라서, Cz로 나누기가 가능하다

			//	(-Cz * Sx) / (Cz * Cx)
			fX = -atan2f( m_fDAElem[2][1], m_fDAElem[1][1] );

			//	(-Cz * Sy) / (Cz * Cy) = -Tan(y) = (m_fDAElem[0][2]/m_fDAElem[0][0])
			fY = -atan2f( m_fDAElem[0][2], m_fDAElem[0][0] );

			//	sinf(z) = R[0][1]
			fZ = asinf( m_fDAElem[0][1] );
		}
		//	정규직교 행렬을 대상으로 하기 때문에 Sz 는 -1 보다 작을 수 없다
		//	여기서는 Sz가 -1보다 작은 경우 -1로 처리한다
		else
		{
			fZ = -NAVI_PI / 2.f;

			//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
			//
			//	|	0			-1		0		|
			//	|	C(x-y)		0		S(x-y)	|
			//	|	-S(x-y)		0		C(x-y)	|
			//
			//	여기서, x, y는 무수히 많은 해를 가진다.
			//	따라서, X -> Z -> Y의 순서로 회전 행렬이 구성되므로
			//	편의상 x = 0 으로 놓고 y의 회전값을 결정한다

			fX = 0.f;
			fY = -atan2f( m_fDAElem[1][2], m_fDAElem[2][2] );
		}
	}
	//	정규직교 행렬을 대상으로 하기 때문에 Sz 는 1 보다 클 수 없다
	//	여기서는 Sz가 1보다 큰 경우 1로 처리한다
	else
	{
		fZ = NAVI_PI / 2.f;

		//	|	Cz * Cy						Sz			-Cz * Sy				|
		//	|	Sx * Sy - Cx * Cy * Sz		Cz * Cx		Cy * Sx + Cx * Sy * Sz	|
		//	|	Cx * Sy + Cy * Sx * Sz		-Cz * Sx	Cx * Cy - Sx * Sy * Sz	|

		//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
		//
		//	|	0			1		0		|
		//	|	-C(x+y)		0		S(x+y)	|
		//	|	S(x+y)		0		C(x+y)	|
		//
		//	여기서, x, y는 무수히 많은 해를 가진다.
		//	따라서, X -> Z -> Y의 순서로 회전 행렬이 구성되므로
		//	편의상 x = 0 으로 놓고 y의 회전값을 결정한다

		fX = 0.f;
		fY = atan2f( m_fDAElem[1][2], m_fDAElem[2][2] );
	}
}


void CNtlNaviMatrix3::GetEulerRotYXZ( float& fX, float& fY, float& fZ ) const
{
	//
	//	임의의 점 P를 RMY(fY) -> RMX(fX) -> RMZ(fZ)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMY(fY) * RMX(fX) * RMZ(fZ)
	//
	//	|	Cy * Cz - Sx * Sy * Sz		Cz * Sx * Sy + Cy * Sz		-Cx * Sy	|
    //	|	-Cx * Sz					Cx * Cz						Sx			|
    //	|	Cz * Sy + Cy * Sx * Sz		-Cy * Cz * Sx + Sy * Sz		Cx * Cy		|
	//

	if ( m_fDAElem[1][2] < 1.f )
	{
		if ( m_fDAElem[1][2] > -1.f )
		{
			//	-1 < Sx < 1 <=> 0 < x < NAVI_PI / 2 || NAVI_PI / 2 < x < NAVI_PI <=> Cx != 0
			//	따라서, Cx로 나누기가 가능하다

			fX = asinf( m_fDAElem[1][2] );

			//	(-Cx * Sy) / (Cx * Cy) = -Tan(y) = (m_fDAElem[0][2]/m_fDAElem[2][2])
			fY = -atan2f( m_fDAElem[0][2], m_fDAElem[2][2] );

			//	(-Cx * Sz) / (Cx * Cz) = -Tan(z) = (m_fDAElem[1][0]/m_fDAElem[1][1])
			fZ = -atan2f( m_fDAElem[1][0], m_fDAElem[1][1] );
		}
		//	정규직교 행렬을 대상으로 하기 때문에 Sx 는 -1 보다 작을 수 없다
		//	여기서는 Sx가 -1보다 작은 경우 -1로 처리한다
		else
		{
			fX = -NAVI_PI / 2.f;

			//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
			//
			//	|	C(y-z)		-S(y-z)		0	|
			//	|	0			0			-1	|
			//	|	S(y-z)		C(y-z)		0	|
			//
			//	여기서, y, z는 무수히 많은 해를 가진다.
			//	따라서, Y -> X -> Z의 순서로 회전 행렬이 구성되므로
			//	편의상 y = 0 으로 놓고 z의 회전값을 결정한다

			fY = 0.f;
			fZ = -atan2f( m_fDAElem[2][0], m_fDAElem[0][0] );
		}
	}
	//	정규직교 행렬을 대상으로 하기 때문에 Sx 는 1 보다 클 수 없다
	//	여기서는 Sx가 1보다 큰 경우 1로 처리한다
	else
	{
		fX = NAVI_PI / 2.f;

		//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
		//
		//	|	C(y+z)		S(y+z)		0	|
		//	|	0			0			1	|
		//	|	S(y+z)		-C(y+z)		0	|
		//
		//	여기서, y, z는 무수히 많은 해를 가진다.
		//	따라서, Y -> X -> Z의 순서로 회전 행렬이 구성되므로
		//	편의상 y = 0 으로 놓고 z의 회전값을 결정한다

		fY = 0.f;
		fZ = atan2f( m_fDAElem[2][0], m_fDAElem[0][0] );
	}
}


void CNtlNaviMatrix3::GetEulerRotYZX( float& fX, float& fY, float& fZ ) const
{
	//
	//	임의의 점 P를 RMY(fY) -> RMZ(fZ) -> RMX(fX)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMY(fY) * RMZ(fZ) * RMX(fX)
	//
	//	|	Cy * Cz		Sx * Sy + Cx * Cy * Sz		-Cx * Sy + Cy * Sx * Sz		|
    //	|	-Sz			Cx * Cz						Cz * Sx						|
    //	|	Cz * Sy		-Cy * Sx + Cx * Sy * Sz		Cx * Cy + Sx * Sy * Sz		|
	//

	if ( m_fDAElem[1][0] < 1.f )
	{
		if ( m_fDAElem[1][0] > -1.f )
		{
			//	-1 < -Sz < 1 <=> 0 < z < NAVI_PI / 2 || NAVI_PI / 2 < z < NAVI_PI <=> Cz != 0
			//	따라서, Cz로 나누기가 가능하다

			//	(Cz * Sx) / (Cz * Cx)
			fX = -atan2f( m_fDAElem[1][2], m_fDAElem[1][1] );

			//	(Cz * Sy) / (Cz * Cy)
			fY = atan2f( m_fDAElem[2][0], m_fDAElem[0][0] );

			//	-sinf(z) = m_fDAElem[1][0]
			fZ = -asinf( m_fDAElem[1][0] );
		}
		//	정규직교 행렬을 대상으로 하기 때문에 -Sz 는 -1 보다 작을 수 없다
		//	여기서는 -Sz가 -1보다 작은 경우 -1로 처리한다
		else
		{
			fZ = NAVI_PI / 2.f;

			//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
			//
			//	|	0		C(x-y)		S(x-y)	|
			//	|	-1		0			0		|
			//	|	0		-S(x-y)		C(x-y)	|
			//
			//	여기서, x, y는 무수히 많은 해를 가진다.
			//	따라서, Y -> Z -> X의 순서로 회전 행렬이 구성되므로
			//	편의상 y = 0 으로 놓고 x의 회전값을 결정한다

			fY = 0.f;
			fX = atan2f( m_fDAElem[0][2], m_fDAElem[2][2] );
		}
	}
	//	정규직교 행렬을 대상으로 하기 때문에 -Sz 는 1 보다 클 수 없다
	//	여기서는 -Sz가 1보다 큰 경우 1로 처리한다
	else
	{
		fZ = -NAVI_PI / 2.f;

		//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
		//
		//	|	0		-C(x+y)		-S(x+y)	|
		//	|	1		0			0		|
		//	|	0		-S(x+y)		C(x+y)	|
		//
		//	여기서, x, y는 무수히 많은 해를 가진다.
		//	따라서, Y -> Z -> X의 순서로 회전 행렬이 구성되므로
		//	편의상 y = 0 으로 놓고 x의 회전값을 결정한다

		fY = 0.f;
		fX = atan2f( m_fDAElem[2][1], m_fDAElem[0][1] );
	}
}


void CNtlNaviMatrix3::GetEulerRotZXY( float& fX, float& fY, float& fZ ) const
{
	//
	//	임의의 점 P를 RMZ(fZ) -> RMX(fX) -> RMY(fY)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMZ(fZ) * RMX(fX) * RMY(fY)
	//
    //	|	Cy * Cz + Sx * Sy * Sz		Cx * Sz		-Cz * Sy + Cy * Sx * Sz		|
    //	|	Cz * Sx * Sy - Cy * Sz		Cx * Cz		Cy * Cz * Sx + Sy * Sz		|
    //	|	Cx * Sy						-Sx			Cx * Cy						|
	//

	if ( m_fDAElem[2][1] < 1.f )
	{
		if ( m_fDAElem[2][1] > -1.f )
		{
			//	-1 < -Sx < 1 <=> 0 < x < NAVI_PI / 2 || NAVI_PI / 2 < x < NAVI_PI <=> Cx != 0
			//	따라서, Cx로 나누기가 가능하다

			//	-sinf(x) = m_fDAElem[2][1]
			fX = -asinf( m_fDAElem[2][1] );

			//	(Cx * Sy) / (Cx * Cy)
			fY = atan2f( m_fDAElem[2][0], m_fDAElem[2][2] );

			//	(Cx * Sz) / (Cx * Cz)
			fZ = atan2f( m_fDAElem[0][1], m_fDAElem[1][1] );
		}
		//	정규직교 행렬을 대상으로 하기 때문에 -Sx 는 -1 보다 작을 수 없다
		//	여기서는 -Sx가 -1보다 작은 경우 -1로 처리한다
		else
		{
			fX = NAVI_PI / 2.f;

			//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
			//
			//	|	C(y-z)		0		-S(y-z)	|
			//	|	S(y-z)		0		C(y-z)	|
			//	|	0			-1		0		|
			//
			//	여기서, y, z는 무수히 많은 해를 가진다.
			//	따라서, Z -> X -> Y의 순서로 회전 행렬이 구성되므로
			//	편의상 z = 0 으로 놓고 y의 회전값을 결정한다

			fZ = 0.f;
			fY = atan2f( m_fDAElem[1][0], m_fDAElem[0][0] );
		}
	}
	//	정규직교 행렬을 대상으로 하기 때문에 -Sx 는 1 보다 클 수 없다
	//	여기서는 -Sx가 1보다 큰 경우 1로 처리한다
	else
	{
		fX = -NAVI_PI / 2.f;

		//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
		//
		//	|	C(y+z)		0		-S(y+z)	|
		//	|	-S(y+z)		0		-C(y+z)	|
		//	|	0			1		0		|
		//
		//	여기서, y, z는 무수히 많은 해를 가진다.
		//	따라서, Z -> X -> Y의 순서로 회전 행렬이 구성되므로
		//	편의상 z = 0 으로 놓고 y의 회전값을 결정한다

		fZ = 0.f;
		fY = atan2f( m_fDAElem[0][2], m_fDAElem[1][2] );
	}
}


void CNtlNaviMatrix3::GetEulerRotZYX( float& fX, float& fY, float& fZ ) const
{
	//
	//	임의의 점 P를 RMZ(fZ) -> RMY(fY) -> RMX(fX)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMZ(fZ) * RMY(fY) * RMX(fX)
	//
	//	|	Cy * Cz		Cz * Sx * Sy + Cx * Sz		-Cx * Cz * Sy + Sx * Sz		|
    //	|	-Cy * Sz	Cx * Cz - Sx * Sy * Sz		Cz * Sx + Cx * Sy * Sz		|
    //	|	Sy			-Cy * Sx					Cy * Cx						|
	//

	if ( m_fDAElem[2][0] < 1.f )
	{
		if ( m_fDAElem[2][0] > -1.f )
		{
			//	-1 < Sy < 1 <=> 0 < y < NAVI_PI / 2 || NAVI_PI / 2 < y < NAVI_PI <=> Cy != 0
			//	따라서, Cy로 나누기가 가능하다

			//	(-Cy * Sx) / (Cy * Cx) = -Tan(x) = (m_fDAElem[2][1]/m_fDAElem[2][2])
			fX = -atan2f( m_fDAElem[2][1], m_fDAElem[2][2] );

			fY = asinf( m_fDAElem[2][0] );

			//	(-Cy * Sz) / (Cy * Cz) = -Tan(z) = (m_fDAElem[1][0]/m_fDAElem[0][0])
			fZ = atan2f( m_fDAElem[1][0], m_fDAElem[0][0] );
		}
		//	정규직교 행렬을 대상으로 하기 때문에 Sy 는 -1 보다 작을 수 없다
		//	여기서는 Sy가 -1보다 작은 경우 -1로 처리한다
		else
		{
			fY = -NAVI_PI / 2.f;

			//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
			//
			//	|	0		S(z-x)		C(z-x)	|
			//	|	0		C(z-x)		-S(z-x)	|
			//	|	-1		0			0		|
			//
			//	여기서, x, z는 무수히 많은 해를 가진다.
			//	따라서, Z -> Y -> X의 순서로 회전 행렬이 구성되므로
			//	편의상 z = 0 으로 놓고 x의 회전값을 결정한다

			fZ = 0.f;
			fX = -atan2f( m_fDAElem[0][1], m_fDAElem[1][1] );
		}
	}
	//	정규직교 행렬을 대상으로 하기 때문에 Sy 는 1 보다 클 수 없다
	//	여기서는 Sy가 1보다 큰 경우 1로 처리한다
	else
	{
		fY = NAVI_PI / 2.f;

		//	회전 행렬에 대한 공식을 정리하면 아래와 같다.
		//
		//	|	0		S(z+x)		-C(z+x)	|
		//	|	0		C(z+x)		S(z+x)	|
		//	|	-1		0			0		|
		//
		//	여기서, x, z는 무수히 많은 해를 가진다.
		//	따라서, Z -> Y -> X의 순서로 회전 행렬이 구성되므로
		//	편의상 z = 0 으로 놓고 x의 회전값을 결정한다

		fZ = 0.f;
		fX = atan2f( m_fDAElem[0][1], m_fDAElem[1][1] );
	}
}


void CNtlNaviMatrix3::MakeEulerRotX( float fVal )
{
	//
	//	C = cosf
	//	S = sinf
	//
	//	X축을 기준으로 주어진 각(fVal) 만큼 회전하는 행렬
	//
	//	|	1	0	0	|
	//	|	0	C	S	|
	//	|	0	-S	C	|
	//

	float fC = cosf( fVal );
	float fS = sinf( fVal );

	m_fDAElem[0][0] = 1.f;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = fC;	m_fDAElem[1][2] = fS;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = -fS;	m_fDAElem[2][2] = fC;
}


void CNtlNaviMatrix3::MakeEulerRotY( float fVal )
{
	//
	//	C = cosf
	//	S = sinf
	//
	//	Y축을 기준으로 주어진 각(fVal) 만큼 회전하는 행렬
	//
	//	|	C	0	-S	|
	//	|	0	1	0	|
	//	|	S	0	C	|
	//

	float fC = cosf( fVal );
	float fS = sinf( fVal );
	
	m_fDAElem[0][0] = fC;	m_fDAElem[0][1] = 0.f;	m_fDAElem[0][2] = -fS;
	m_fDAElem[1][0] = 0.f;	m_fDAElem[1][1] = 1.f;	m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = fS;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = fC;
}


void CNtlNaviMatrix3::MakeEulerRotZ( float fVal )
{
	//
	//	C = cosf
	//	S = sinf
	//
	//	Z축을 기준으로 주어진 각(fVal) 만큼 회전하는 행렬
	//
	//	|	C	S	0	|
	//	|	-S	C	0	|
	//	|	0	0	1	|
	//

	float fC = cosf( fVal );
	float fS = sinf( fVal );

	m_fDAElem[0][0] = fC;	m_fDAElem[0][1] = fS;	m_fDAElem[0][2] = 0.f;
	m_fDAElem[1][0] = -fS;	m_fDAElem[1][1] = fC;	m_fDAElem[1][2] = 0.f;
	m_fDAElem[2][0] = 0.f;	m_fDAElem[2][1] = 0.f;	m_fDAElem[2][2] = 1.f;
}


void CNtlNaviMatrix3::MakeEulerRotXYZ( float fX, float fY, float fZ )
{
	//
	//	임의의 점 P를 RMX(fX) -> RMY(fY) -> RMZ(fZ)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMX(fX) * RMY(fY) * RMZ(fZ)
	//
	//	|	Cy * Cz						Cy * Sz						-Sy			|
	//	|	Cz * Sx * Sy - Cx * Sz		Sx * Sy * Sz + Cx * Cz		Cy * Sx		|
	//	|	Cx * Cz * Sy + Sx * Sz		Cx * Sy * Sz - Cz * Sx		Cx * Cy		|
	//

	float fCx = cosf( fX );
	float fSx = sinf( fX );
	float fCy = cosf( fY );
	float fSy = sinf( fY );
	float fCz = cosf( fZ );
	float fSz = sinf( fZ );

	m_fDAElem[0][0] = fCy * fCz;
	m_fDAElem[0][1] = fCy * fSz;
	m_fDAElem[0][2] = -fSy;

	m_fDAElem[1][0] = fCz * fSx * fSy - fCx * fSz;
	m_fDAElem[1][1] = fSx * fSy * fSz + fCx * fCz;
	m_fDAElem[1][2] = fCy * fSx;

	m_fDAElem[2][0] = fCx * fCz * fSy + fSx * fSz;
	m_fDAElem[2][1] = fCx * fSy * fSz - fCz * fSx;
	m_fDAElem[2][2] = fCx * fCy;
}


void CNtlNaviMatrix3::MakeEulerRotXZY( float fX, float fY, float fZ )
{
	//
	//	임의의 점 P를 RMX(fX) -> RMZ(fZ) -> RMY(fY)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMX(fX) * RMZ(fZ) * RMY(fY)
	//
	//	|	Cy * Cz						Sz			-Cz * Sy				|
    //	|	Sx * Sy - Cx * Cy * Sz		Cx * Cz		Cy * Sx + Cx * Sy * Sz	|
    //	|	Cx * Sy + Cy * Sx * Sz		-Cz * Sx	Cx * Cy - Sx * Sy * Sz	|
	//

	float fCx = cosf( fX );
	float fSx = sinf( fX );
	float fCy = cosf( fY );
	float fSy = sinf( fY );
	float fCz = cosf( fZ );
	float fSz = sinf( fZ );

	m_fDAElem[0][0] = fCy * fCz;
	m_fDAElem[0][1] = fSz;
	m_fDAElem[0][2] = -fCz * fSy;

	m_fDAElem[1][0] = fSx * fSy - fCx * fCy * fSz;
	m_fDAElem[1][1] = fCx * fCz;
	m_fDAElem[1][2] = fCy * fSx + fCx * fSy * fSz;

	m_fDAElem[2][0] = fCx * fSy + fCy * fSx * fSz;
	m_fDAElem[2][1] = -fCz * fSx;
	m_fDAElem[2][2] = fCx * fCy - fSx * fSy * fSz;
}


void CNtlNaviMatrix3::MakeEulerRotYXZ( float fX, float fY, float fZ )
{
	//
	//	임의의 점 P를 RMY(fY) -> RMX(fX) -> RMZ(fZ)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMY(fY) * RMX(fX) * RMZ(fZ)
	//
	//	|	Cy * Cz - Sx * Sy * Sz		Cz * Sx * Sy + Cy * Sz		-Cx * Sy	|
    //	|	-Cx * Sz					Cx * Cz						Sx			|
    //	|	Cz * Sy + Cy * Sx * Sz		-Cy * Cz * Sx + Sy * Sz		Cx * Cy		|
	//

	float fCx = cosf( fX );
	float fSx = sinf( fX );
	float fCy = cosf( fY );
	float fSy = sinf( fY );
	float fCz = cosf( fZ );
	float fSz = sinf( fZ );

	m_fDAElem[0][0] = fCy * fCz - fSx * fSy * fSz;
	m_fDAElem[0][1] = fCz * fSx * fSy + fCy * fSz;
	m_fDAElem[0][2] = -fCx * fSy;

	m_fDAElem[1][0] = -fCx * fSz;
	m_fDAElem[1][1] = fCx * fCz;
	m_fDAElem[1][2] = fSx;

	m_fDAElem[2][0] = fCz * fSy + fCy * fSx * fSz;
	m_fDAElem[2][1] = -fCy * fCz * fSx + fSy * fSz;
	m_fDAElem[2][2] = fCx * fCy;
}


void CNtlNaviMatrix3::MakeEulerRotYZX( float fX, float fY, float fZ )
{
	//
	//	임의의 점 P를 RMY(fY) -> RMZ(fZ) -> RMX(fX)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMY(fY) * RMZ(fZ) * RMX(fX)
	//
	//	|	Cy * Cz		Sx * Sy + Cx * Cy * Sz		-Cx * Sy + Cy * Sx * Sz		|
    //	|	-Sz			Cx * Cz						Cz * Sx						|
    //	|	Cz * Sy		-Cy * Sx + Cx * Sy * Sz		Cx * Cy + Sx * Sy * Sz		|
	//

	float fCx = cosf( fX );
	float fSx = sinf( fX );
	float fCy = cosf( fY );
	float fSy = sinf( fY );
	float fCz = cosf( fZ );
	float fSz = sinf( fZ );

	m_fDAElem[0][0] = fCy * fCz;
	m_fDAElem[0][1] = fSx * fSy + fCx * fCy * fSz;
	m_fDAElem[0][2] = -fCx * fSy + fCy * fSx * fSz;

	m_fDAElem[1][0] = -fSz;
	m_fDAElem[1][1] = fCx * fCz;
	m_fDAElem[1][2] = fCz * fSx;

	m_fDAElem[2][0] = fCz * fSy;
	m_fDAElem[2][1] = -fCy * fSx + fCx * fSy * fSz;
	m_fDAElem[2][2] = fCx * fCy + fSx * fSy * fSz;
}


void CNtlNaviMatrix3::MakeEulerRotZXY( float fX, float fY, float fZ )
{
	//
	//	임의의 점 P를 RMZ(fZ) -> RMX(fX) -> RMY(fY)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMZ(fZ) * RMX(fX) * RMY(fY)
	//
    //	|	Cy * Cz + Sx * Sy * Sz		Cx * Sz		-Cz * Sy + Cy * Sx * Sz		|
    //	|	Cz * Sx * Sy - Cy * Sz		Cx * Cz		Cy * Cz * Sx + Sy * Sz		|
    //	|	Cx * Sy						-Sx			Cx * Cy						|
	//

	float fCx = cosf( fX );
	float fSx = sinf( fX );
	float fCy = cosf( fY );
	float fSy = sinf( fY );
	float fCz = cosf( fZ );
	float fSz = sinf( fZ );

	m_fDAElem[0][0] = fCy * fCz + fSx * fSy * fSz;
	m_fDAElem[0][1] = fCx * fSz;
	m_fDAElem[0][2] = -fCz * fSy + fCy * fSx * fSz;

	m_fDAElem[1][0] = fCz * fSx * fSy - fCy * fSz;
	m_fDAElem[1][1] = fCx * fCz;
	m_fDAElem[1][2] = fCy * fCz * fSx + fSy * fSz;

	m_fDAElem[2][0] = fCx * fSy;
	m_fDAElem[2][1] = -fSx;
	m_fDAElem[2][2] = fCx * fCy;
}


void CNtlNaviMatrix3::MakeEulerRotZYX( float fX, float fY, float fZ )
{
	//
	//	임의의 점 P를 RMZ(fZ) -> RMY(fY) -> RMX(fX)을 따라 회전 시키는 행렬
	//
	//	P' = P * RMZ(fZ) * RMY(fY) * RMX(fX)
	//
	//	|	Cy * Cz		Cz * Sx * Sy + Cx * Sz		-Cx * Cz * Sy + Sx * Sz		|
    //	|	-Cy * Sz	Cx * Cz - Sx * Sy * Sz		Cz * Sx + Cx * Sy * Sz		|
    //	|	Sy			-Cy * Sx					Cx * Cy						|
	//

	float fCx = cosf( fX );
	float fSx = sinf( fX );
	float fCy = cosf( fY );
	float fSy = sinf( fY );
	float fCz = cosf( fZ );
	float fSz = sinf( fZ );

	m_fDAElem[0][0] = fCy * fCz;
	m_fDAElem[0][1] = fCz * fSx * fSy + fCx * fSz;
	m_fDAElem[0][2] = -fCx * fCz * fSy + fSx * fSz;

	m_fDAElem[1][0] = -fCy * fSz;
	m_fDAElem[1][1] = fCx * fCz - fSx * fSy * fSz;
	m_fDAElem[1][2] = fCz * fSx + fCx * fSy * fSz;

	m_fDAElem[2][0] = fSy;
	m_fDAElem[2][1] = -fCy * fSx;
	m_fDAElem[2][2] = fCx * fCy;
}

void CNtlNaviMatrix3::GetAxisAngleFromRotMat( CNtlNaviVector3& v3Axis, float& fAngle ) const
{
	//
	//	임의의 회전축 A ( Ax, Ay, Az ),
	//	단위 행렬 E,
	//	CR = |	0		Az		-Ay	|	이라 했을때 
	//		 |	-Az		0		Ax	|
	//		 |	Ay		-Ax		0	|
	//	임의의 축 A(v3Axis) 를 기준으로 주어진 각(fAngle) 만큼 회전시키는 행렬 R은
	//	아래와 같은 공식으로 정리할 수 있다
	//
	//	R = E + sinf(t) * CR	+ (1 - cosf(t)) * CR^2	-- (ㄱ)
	//
	//	  = |	C + Ax * Ax * ( 1 - C )			Ax * Ay * ( 1 - C ) + Az * S		Ax * Az * ( 1 - C ) - Ay * S	|
	//		|	Ax * Ay * ( 1 - C ) - Az * S	C + Ay * Ay * ( 1 - C )				Ay * Az * ( 1 - C ) + Ax * S	|
	//		|	Ax * Az * ( 1 - C ) + Ay * S	Ay * Az * ( 1 - C ) - Ax * S		C + Az * Az * ( 1 - C )			|
	//
	//	여기서,
	//	(1) cosf(t) = (trace(R) - 1) / 2 을 이용하여 회전각을 계산한다
	//	(2) R - R^T = 2 * sinf(t) * CR 을 이용하여 회전축을 계산하면 되나
	//		t = 0, NAVI_PI 인 경우 sinf(t)로 양변을 나눌 수가 없으므로 다른 공식을 이용해야 한다
	//		여기서, t = 0 이면 회전이 없으니 임의의 축을 회전 축으로 정하면 되고
	//		t = NAVI_PI 인 경우 (ㄱ)식을 이용하여 회전축을 계산한다
	//

	float fTrace = m_fDAElem[0][0] + m_fDAElem[1][1] + m_fDAElem[2][2];
	fAngle = acosf( 0.5f * (fTrace - 1) );

	if ( fAngle > 0.f )
	{
		if ( fAngle < NAVI_PI )
		{
			//	(2) 식을 이용하여 회전축을 계산한다
			v3Axis.SetElem( m_fDAElem[1][2] - m_fDAElem[2][1],
							m_fDAElem[2][0] - m_fDAElem[0][2],
							m_fDAElem[0][1] - m_fDAElem[1][0] );
			v3Axis.MakeNormalize();
		}
		else
		{
			//	(ㄱ) 식을 풀어 대각 성분의 가감식을 이용하여 회전축을 계산한다
			//	R = |	1 - 2 ( Ay^2 + Az^2 )	2 * Ax * Ay				2 * Ax * Az				|
			//		|	2 * Ax * Ay				1 - 2 ( Ax^2 + Az^2 )	2 * Ay * Az				|
			//		|	2 * Ax * Az				2 * Ay * Az				1 - 2 ( Ax^2 + Ay^2 )	|

			if ( m_fDAElem[0][0] >= m_fDAElem[1][1] )
			{
				if ( m_fDAElem[0][0] >= m_fDAElem[2][2] )
				{
					//	m_fDAElem[0][0] 가 대각 성분중 최대값을 갖는다
					float fAx = 0.5f * sqrtf( m_fDAElem[0][0] - m_fDAElem[1][1] - m_fDAElem[2][2] + 1.f );

					if ( fAx <= NAVI_ZERO_THRESHOLD )
					{
						v3Axis.MakeInvalid();
					}
					else
					{
						v3Axis[0] = fAx;

						fAx = 0.5f * (1.f / fAx);
						v3Axis[1] = m_fDAElem[0][1] * fAx;
						v3Axis[2] = m_fDAElem[0][2] * fAx;
					}
				}
				else
				{
					//	m_fDAElem[2][2] 가 대각 성분중 최대값을 갖는다
					float fAz = 0.5f * sqrtf( m_fDAElem[2][2] - m_fDAElem[0][0] - m_fDAElem[1][1] + 1.f );

					if ( fAz <= NAVI_ZERO_THRESHOLD )
					{
						v3Axis.MakeInvalid();
					}
					else
					{
						v3Axis[2] = fAz;

						fAz = 0.5f * (1.f / fAz);
						v3Axis[0] = m_fDAElem[0][2] * fAz;
						v3Axis[1] = m_fDAElem[1][2] * fAz;
					}
				}
			}
			else
			{
				if ( m_fDAElem[1][1] >= m_fDAElem[2][2] )
				{
					//	m_fDAElem[1][1] 가 대각 성분중 최대값을 갖는다
					float fAy = 0.5f * sqrtf( m_fDAElem[1][1] - m_fDAElem[0][0] - m_fDAElem[2][2] + 1.f );

					if ( fAy <= NAVI_ZERO_THRESHOLD )
					{
						v3Axis.MakeInvalid();
					}
					else
					{
						v3Axis[1] = fAy;

						fAy = 0.5f * (1.f / fAy);
						v3Axis[0] = m_fDAElem[0][1] * fAy;
						v3Axis[2] = m_fDAElem[1][2] * fAy;
					}
				}
				else
				{
					//	m_fDAElem[2][2] 가 대각 성분중 최대값을 갖는다
					float fAz = 0.5f * sqrtf( m_fDAElem[2][2] - m_fDAElem[0][0] - m_fDAElem[1][1] + 1.f );

					if ( fAz <= NAVI_ZERO_THRESHOLD )
					{
						v3Axis.MakeInvalid();
					}
					else
					{
						v3Axis[2] = fAz;

						fAz = 0.5f * (1.f / fAz);
						v3Axis[0] = m_fDAElem[0][2] * fAz;
						v3Axis[1] = m_fDAElem[1][2] * fAz;
					}
				}
			}
		}
	}
	else
	{
		//	회전이 없는 경우
		//	회전축을 임의로 X축으로 정한다
		v3Axis.SetElem( 1.f, 0.f, 0.f );
	}
}


void CNtlNaviMatrix3::MakeRotMatFromAxisAngle( const CNtlNaviVector3& v3Axis, float fAngle )
{
	//
	//	C = cosf
	//	S = sinf
	//	임의의 회전축 = A ( Ax, Ay, Az )
	//
	//	임의의 축 A(v3Axis) 를 기준으로 주어진 각(fAngle) 만큼 회전시키는 행렬
	//
	//	|	C + Ax * Ax * ( 1 - C )			Ax * Ay * ( 1 - C ) + Az * S		Ax * Az * ( 1 - C ) - Ay * S	|
	//  |	Ax * Ay * ( 1 - C ) - Az * S	C + Ay * Ay * ( 1 - C )				Ay * Az * ( 1 - C ) + Ax * S	|
	//  |	Ax * Az * ( 1 - C ) + Ay * S	Ay * Az * ( 1 - C ) - Ax * S		C + Az * Az * ( 1 - C )			|
	//

	float fC = cosf( fAngle );
	float fS = sinf( fAngle );
	float fOMC = 1 - fC;

	float fX, fY, fZ;
	v3Axis.GetXYZ( fX, fY, fZ );

	float fXY = fX * fY;
	float fXZ = fX * fZ;
	float fYZ = fY * fZ;

	m_fDAElem[0][0] = fC + fX * fX * fOMC;
	m_fDAElem[0][1] = fXY * fOMC + fZ * fS;
	m_fDAElem[0][2] = fXZ * fOMC - fY * fS;

	m_fDAElem[1][0] = fXY * fOMC - fZ * fS;
	m_fDAElem[1][1] = fC + fY * fY * fOMC;
	m_fDAElem[1][2] = fYZ * fOMC + fX * fS;

	m_fDAElem[2][0] = fXZ * fOMC + fY * fS;
	m_fDAElem[2][1] = fYZ * fOMC - fX * fS;
	m_fDAElem[2][2] = fC + fZ * fZ * fOMC;
}


float CNtlNaviMatrix3::GetDet( void ) const
{
	return
		m_fDAElem[0][0] * ( m_fDAElem[1][1] * m_fDAElem[2][2] - m_fDAElem[1][2] * m_fDAElem[2][1] ) -
		m_fDAElem[0][1] * ( m_fDAElem[1][0] * m_fDAElem[2][2] - m_fDAElem[1][2] * m_fDAElem[2][0] ) +
		m_fDAElem[0][2] * ( m_fDAElem[1][0] * m_fDAElem[2][1] - m_fDAElem[1][1] * m_fDAElem[2][0] );
}


bool CNtlNaviMatrix3::CanInverse( void ) const
{
	return fabsf( GetDet() ) > NAVI_ZERO_THRESHOLD;
}


float CNtlNaviMatrix3::GetInverse( CNtlNaviMatrix3& m3Val ) const
{
	float fDet = GetDet();

	if ( fabsf( fDet ) <= NAVI_ZERO_THRESHOLD )
	{
		m3Val.MakeInvalid();
		return fDet;
	}

	float fIDet = 1.f / fDet;

	m3Val.m_fDAElem[0][0] =  fIDet * ( m_fDAElem[1][1] * m_fDAElem[2][2] - m_fDAElem[1][2] * m_fDAElem[2][1] );
	m3Val.m_fDAElem[0][1] = -fIDet * ( m_fDAElem[0][1] * m_fDAElem[2][2] - m_fDAElem[0][2] * m_fDAElem[2][1] );
	m3Val.m_fDAElem[0][2] =  fIDet * ( m_fDAElem[0][1] * m_fDAElem[1][2] - m_fDAElem[0][2] * m_fDAElem[1][1] );

	m3Val.m_fDAElem[1][0] = -fIDet * ( m_fDAElem[1][0] * m_fDAElem[2][2] - m_fDAElem[1][2] * m_fDAElem[2][0] );
	m3Val.m_fDAElem[1][1] =  fIDet * ( m_fDAElem[0][0] * m_fDAElem[2][2] - m_fDAElem[0][2] * m_fDAElem[2][0] );
	m3Val.m_fDAElem[1][2] = -fIDet * ( m_fDAElem[0][0] * m_fDAElem[1][2] - m_fDAElem[0][2] * m_fDAElem[1][0] );

	m3Val.m_fDAElem[2][0] =  fIDet * ( m_fDAElem[1][0] * m_fDAElem[2][1] - m_fDAElem[1][1] * m_fDAElem[2][0] );
	m3Val.m_fDAElem[2][1] = -fIDet * ( m_fDAElem[0][0] * m_fDAElem[2][1] - m_fDAElem[0][1] * m_fDAElem[2][0] );
	m3Val.m_fDAElem[2][2] =  fIDet * ( m_fDAElem[0][0] * m_fDAElem[1][1] - m_fDAElem[0][1] * m_fDAElem[1][0] );

	return fDet;
}


float CNtlNaviMatrix3::MakeInverse( void )
{
	return GetInverse( *this );
}


CNtlNaviMatrix3 CNtlNaviMatrix3::GetTranspose( void ) const
{
	CNtlNaviMatrix3 matTranspose;

	for ( int i = 0; i < 3; ++i )
	{
		for ( int j = 0; j < 3; ++j )
		{
			matTranspose.m_fDAElem[j][i] = m_fDAElem[i][j];
		}
	}

	return matTranspose;
}


void CNtlNaviMatrix3::MakeTranspose( void )
{
	float fTemp;

	int i, j;
	for ( i = 0; i < 2; ++i )
	{
		for ( j = i + 1; j < 3; ++j )
		{
			fTemp = m_fDAElem[i][j];
			m_fDAElem[i][j] = m_fDAElem[j][i];
			m_fDAElem[j][i] = fTemp;
		}
	}
}


float CNtlNaviMatrix3::GetTrace( void ) const
{
	return m_fDAElem[0][0] + m_fDAElem[1][1] + m_fDAElem[2][2];
}


CNtlNaviMatrix3& CNtlNaviMatrix3::operator = ( const CNtlNaviMatrix3& m3Val )
{
	memcpy( m_fSAElem, m3Val.m_fSAElem, sizeof(float)*9 );

	return *this;
}


float* CNtlNaviMatrix3::operator [] ( int nIdx )
{
	return m_fDAElem[nIdx];
}


const float* CNtlNaviMatrix3::operator [] ( int nIdx ) const
{
	return m_fDAElem[nIdx];
}


float& CNtlNaviMatrix3::operator () ( int nRow, int nCol )
{
	return m_fDAElem[nRow][nCol];
}


const float& CNtlNaviMatrix3::operator () ( int nRow, int nCol ) const
{
	return m_fDAElem[nRow][nCol];
}


bool CNtlNaviMatrix3::operator == ( const CNtlNaviMatrix3& m3Val ) const
{
	return CompareArrays( m3Val ) == 0;
}


bool CNtlNaviMatrix3::operator != ( const CNtlNaviMatrix3& m3Val ) const
{
	return CompareArrays( m3Val ) != 0;
}


CNtlNaviMatrix3 CNtlNaviMatrix3::operator - ( void ) const
{
	return CNtlNaviMatrix3(
		-m_fDAElem[0][0], -m_fDAElem[0][1], -m_fDAElem[0][2],
		-m_fDAElem[1][0], -m_fDAElem[1][1], -m_fDAElem[1][2],
		-m_fDAElem[2][0], -m_fDAElem[2][1], -m_fDAElem[2][2] );
}


CNtlNaviMatrix3 CNtlNaviMatrix3::operator * ( float fVal ) const
{
	return CNtlNaviMatrix3(
		m_fDAElem[0][0] * fVal, m_fDAElem[0][1] * fVal, m_fDAElem[0][2] * fVal,
		m_fDAElem[1][0] * fVal, m_fDAElem[1][1] * fVal, m_fDAElem[1][2] * fVal,
		m_fDAElem[2][0] * fVal, m_fDAElem[2][1] * fVal, m_fDAElem[2][2] * fVal );
}


CNtlNaviMatrix3 CNtlNaviMatrix3::operator / ( float fVal ) const
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		return CNtlNaviMatrix3(
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX,
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX,
			NAVI_FLT_MAX, NAVI_FLT_MAX, NAVI_FLT_MAX );
	}

	fVal = 1.f / fVal;

	return CNtlNaviMatrix3(
		m_fDAElem[0][0] * fVal, m_fDAElem[0][1] * fVal, m_fDAElem[0][2] * fVal,
		m_fDAElem[1][0] * fVal, m_fDAElem[1][1] * fVal, m_fDAElem[1][2] * fVal,
		m_fDAElem[2][0] * fVal, m_fDAElem[2][1] * fVal, m_fDAElem[2][2] * fVal );
}


CNtlNaviMatrix3 CNtlNaviMatrix3::operator + ( const CNtlNaviMatrix3& m3Val ) const
{
	return CNtlNaviMatrix3(
		m_fDAElem[0][0] + m3Val.m_fDAElem[0][0], m_fDAElem[0][1] + m3Val.m_fDAElem[0][1], m_fDAElem[0][2] + m3Val.m_fDAElem[0][2],
		m_fDAElem[1][0] + m3Val.m_fDAElem[1][0], m_fDAElem[1][1] + m3Val.m_fDAElem[1][1], m_fDAElem[1][2] + m3Val.m_fDAElem[1][2],
		m_fDAElem[2][0] + m3Val.m_fDAElem[2][0], m_fDAElem[2][1] + m3Val.m_fDAElem[2][1], m_fDAElem[2][2] + m3Val.m_fDAElem[2][2] );
}


CNtlNaviMatrix3 CNtlNaviMatrix3::operator - ( const CNtlNaviMatrix3& m3Val ) const
{
	return CNtlNaviMatrix3(
		m_fDAElem[0][0] - m3Val.m_fDAElem[0][0], m_fDAElem[0][1] - m3Val.m_fDAElem[0][1], m_fDAElem[0][2] - m3Val.m_fDAElem[0][2],
		m_fDAElem[1][0] - m3Val.m_fDAElem[1][0], m_fDAElem[1][1] - m3Val.m_fDAElem[1][1], m_fDAElem[1][2] - m3Val.m_fDAElem[1][2],
		m_fDAElem[2][0] - m3Val.m_fDAElem[2][0], m_fDAElem[2][1] - m3Val.m_fDAElem[2][1], m_fDAElem[2][2] - m3Val.m_fDAElem[2][2] );
}


CNtlNaviMatrix3 CNtlNaviMatrix3::operator * ( const CNtlNaviMatrix3& m3Val ) const
{
	return CNtlNaviMatrix3(
		m_fDAElem[0][0] * m3Val.m_fDAElem[0][0] + m_fDAElem[0][1] * m3Val.m_fDAElem[1][0] + m_fDAElem[0][2] * m3Val.m_fDAElem[2][0],
		m_fDAElem[0][0] * m3Val.m_fDAElem[0][1] + m_fDAElem[0][1] * m3Val.m_fDAElem[1][1] + m_fDAElem[0][2] * m3Val.m_fDAElem[2][1],
		m_fDAElem[0][0] * m3Val.m_fDAElem[0][2] + m_fDAElem[0][1] * m3Val.m_fDAElem[1][2] + m_fDAElem[0][2] * m3Val.m_fDAElem[2][2],

		m_fDAElem[1][0] * m3Val.m_fDAElem[0][0] + m_fDAElem[1][1] * m3Val.m_fDAElem[1][0] + m_fDAElem[1][2] * m3Val.m_fDAElem[2][0],
		m_fDAElem[1][0] * m3Val.m_fDAElem[0][1] + m_fDAElem[1][1] * m3Val.m_fDAElem[1][1] + m_fDAElem[1][2] * m3Val.m_fDAElem[2][1],
		m_fDAElem[1][0] * m3Val.m_fDAElem[0][2] + m_fDAElem[1][1] * m3Val.m_fDAElem[1][2] + m_fDAElem[1][2] * m3Val.m_fDAElem[2][2],

		m_fDAElem[2][0] * m3Val.m_fDAElem[0][0] + m_fDAElem[2][1] * m3Val.m_fDAElem[1][0] + m_fDAElem[2][2] * m3Val.m_fDAElem[2][0],
		m_fDAElem[2][0] * m3Val.m_fDAElem[0][1] + m_fDAElem[2][1] * m3Val.m_fDAElem[1][1] + m_fDAElem[2][2] * m3Val.m_fDAElem[2][1],
		m_fDAElem[2][0] * m3Val.m_fDAElem[0][2] + m_fDAElem[2][1] * m3Val.m_fDAElem[1][2] + m_fDAElem[2][2] * m3Val.m_fDAElem[2][2] );
}


CNtlNaviMatrix3& CNtlNaviMatrix3::operator *= ( float fVal )
{
	m_fDAElem[0][0] *= fVal; m_fDAElem[0][1] *= fVal; m_fDAElem[0][2] *= fVal;
	m_fDAElem[1][0] *= fVal; m_fDAElem[1][1] *= fVal; m_fDAElem[1][2] *= fVal;
	m_fDAElem[2][0] *= fVal; m_fDAElem[2][1] *= fVal; m_fDAElem[2][2] *= fVal;

	return *this;
}


CNtlNaviMatrix3& CNtlNaviMatrix3::operator /= ( float fVal )
{
	if ( fabsf( fVal ) <= NAVI_ZERO_THRESHOLD )
	{
		MakeInvalid();
	}
	else
	{
		fVal = 1.f / fVal;

		m_fDAElem[0][0] *= fVal; m_fDAElem[0][1] *= fVal; m_fDAElem[0][2] *= fVal;
		m_fDAElem[1][0] *= fVal; m_fDAElem[1][1] *= fVal; m_fDAElem[1][2] *= fVal;
		m_fDAElem[2][0] *= fVal; m_fDAElem[2][1] *= fVal; m_fDAElem[2][2] *= fVal;
	}

	return *this;
}


CNtlNaviMatrix3& CNtlNaviMatrix3::operator += ( const CNtlNaviMatrix3& m3Val )
{
	m_fDAElem[0][0] += m3Val.m_fDAElem[0][0]; m_fDAElem[0][1] += m3Val.m_fDAElem[0][1]; m_fDAElem[0][2] += m3Val.m_fDAElem[0][2];
	m_fDAElem[1][0] += m3Val.m_fDAElem[1][0]; m_fDAElem[1][1] += m3Val.m_fDAElem[1][1]; m_fDAElem[1][2] += m3Val.m_fDAElem[1][2];
	m_fDAElem[2][0] += m3Val.m_fDAElem[2][0]; m_fDAElem[2][1] += m3Val.m_fDAElem[2][1]; m_fDAElem[2][2] += m3Val.m_fDAElem[2][2];

	return *this;
}


CNtlNaviMatrix3& CNtlNaviMatrix3::operator -= ( const CNtlNaviMatrix3& m3Val )
{
	m_fDAElem[0][0] -= m3Val.m_fDAElem[0][0]; m_fDAElem[0][1] -= m3Val.m_fDAElem[0][1]; m_fDAElem[0][2] -= m3Val.m_fDAElem[0][2];
	m_fDAElem[1][0] -= m3Val.m_fDAElem[1][0]; m_fDAElem[1][1] -= m3Val.m_fDAElem[1][1]; m_fDAElem[1][2] -= m3Val.m_fDAElem[1][2];
	m_fDAElem[2][0] -= m3Val.m_fDAElem[2][0]; m_fDAElem[2][1] -= m3Val.m_fDAElem[2][1]; m_fDAElem[2][2] -= m3Val.m_fDAElem[2][2];

	return *this;
}


CNtlNaviMatrix3& CNtlNaviMatrix3::operator *= ( const CNtlNaviMatrix3& m3Val )
{
	*this = *this * m3Val;

	return *this;
}


CNtlNaviMatrix3::operator float* ( void )
{
	return m_fSAElem;
}


CNtlNaviMatrix3::operator const float* ( void ) const
{
	return m_fSAElem;
}


CNtlNaviMatrix3 operator * ( float fVal1, const CNtlNaviMatrix3& m3Val2 )
{
	return m3Val2 * fVal1;
}


CNtlNaviMatrix3 operator / ( float fVal1, const CNtlNaviMatrix3& m3Val2 )
{
	return m3Val2 / fVal1;
}


CNtlNaviVector3 operator * ( const CNtlNaviVector3& v3Val1, const CNtlNaviMatrix3& m3Val2 )
{
	const float* pVal = v3Val1;

	return CNtlNaviVector3(
		pVal[0] * m3Val2.m_fDAElem[0][0] + pVal[1] * m3Val2.m_fDAElem[1][0] + pVal[2] * m3Val2.m_fDAElem[2][0],
		pVal[0] * m3Val2.m_fDAElem[0][1] + pVal[1] * m3Val2.m_fDAElem[1][1] + pVal[2] * m3Val2.m_fDAElem[2][1],
		pVal[0] * m3Val2.m_fDAElem[0][2] + pVal[1] * m3Val2.m_fDAElem[1][2] + pVal[2] * m3Val2.m_fDAElem[2][2] );
}


CNtlNaviVector3 operator * ( const CNtlNaviMatrix3& m3Val1, const CNtlNaviVector3& v3Val2 )
{
	const float* pVal = v3Val2;

	return CNtlNaviVector3(
		m3Val1.m_fDAElem[0][0] * pVal[0] + m3Val1.m_fDAElem[0][1] * pVal[1] + m3Val1.m_fDAElem[0][2] * pVal[2],
		m3Val1.m_fDAElem[1][0] * pVal[0] + m3Val1.m_fDAElem[1][1] * pVal[1] + m3Val1.m_fDAElem[1][2] * pVal[2],
		m3Val1.m_fDAElem[2][0] * pVal[0] + m3Val1.m_fDAElem[2][1] * pVal[1] + m3Val1.m_fDAElem[2][2] * pVal[2] );
}


int CNtlNaviMatrix3::CompareArrays( const CNtlNaviMatrix3& m3Val ) const
{
	return memcmp( m_fDAElem, m3Val.m_fDAElem, sizeof(float)*9 );
}

