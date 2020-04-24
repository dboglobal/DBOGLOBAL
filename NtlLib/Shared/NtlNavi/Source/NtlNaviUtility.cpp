#include "precomp_navi.h"
#include "NtlNaviUtility.h"
#include "NtlNaviSync.h"


static CNtlNaviCS g_clUtilityCS;


//--------------------------------------------------------------------
//	File chunk
//--------------------------------------------------------------------


sFILE_CHUNK_INFO LoadFileChunk( const char* pFileName )
{
	CNtlNaviAutoCS clAuto( &g_clUtilityCS );

	FILE* pFile;

	sFILE_CHUNK_INFO sFileChunkInfo( 0, NULL );

	fopen_s( &pFile, pFileName, "rb" );

	if ( NULL == pFile )
	{
		return sFILE_CHUNK_INFO( 0xffffffff, NULL );
	}

	fseek( pFile, 0, SEEK_END );
	sFileChunkInfo.lChunkSize = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );

	if ( 0 != sFileChunkInfo.lChunkSize )
	{
		sFileChunkInfo.pChunk = new char[sFileChunkInfo.lChunkSize];
		fread( sFileChunkInfo.pChunk, sFileChunkInfo.lChunkSize, 1, pFile );
	}

	fclose( pFile );

	return sFileChunkInfo;
}

void UnloadFileChunk( sFILE_CHUNK_INFO& sFileChunkInfo )
{
	CNtlNaviAutoCS clAuto( &g_clUtilityCS );

	sFileChunkInfo.lChunkSize = 0;

	if ( sFileChunkInfo.pChunk )
	{
		delete [] sFileChunkInfo.pChunk;
		sFileChunkInfo.pChunk = NULL;
	}
}


//--------------------------------------------------------------------
//	Attach back slash
//--------------------------------------------------------------------


void AttachBackSlash( std::string& strString )
{
	CNtlNaviAutoCS clAuto( &g_clUtilityCS );

	if ( strString.empty() )
	{
		strString += "\\";
	}
	else
	{
		if ( strString[strString.size() - 1] != '\\' )
		{
			strString += "\\";
		}
	}
}


//--------------------------------------------------------------------
//	Upper case
//--------------------------------------------------------------------


std::string MakeUpperCase( std::string& strString )
{
	std::string strRet;

	std::string::iterator it = strString.begin();
	for ( ; it != strString.end(); ++it )
	{
		strRet += islower( *it ) ? _toupper( *it ) : *it;
	}

	return strRet;
}

std::string MakeUpperCase( const char* pString )
{
	std::string strRet;

	int nStrLen = (int)strlen( pString );
	for ( int i = 0; i < nStrLen; ++i )
	{
		strRet += islower( pString[i] ) ? _toupper( pString[i] ) : pString[i];
	}

	return strRet;
}


//--------------------------------------------------------------------
//	Radian <=> Degree
//--------------------------------------------------------------------


float RadianToDegree( float fRadian )
{
	return fRadian * 180.f / NAVI_PI;
}

float DegreeToRadian( float fDegree )
{
	return fDegree * NAVI_PI / 180.f;
}

//--------------------------------------------------------------------
// Line to Line collision test
//--------------------------------------------------------------------

int LineToLineIntersect2D( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float *x, float *y )
{
	float Ax, Bx, Cx, Ay, By, Cy, d, e, f, num, offset;
	float x1lo, x1hi, y1lo, y1hi;

	Ax = x2 - x1;
	Bx = x3 - x4;

	if( Ax < 0 )
	{
		x1lo = x2;
		x1hi = x1;
	}
	else
	{
		x1hi = x2;
		x1lo = x1;
	}

	if( Bx > 0 )
	{
		if( x1hi < x4 || x3 < x1lo )
			return LCTR_NO_COLLISION;
	}
	else
	{
		if( x1hi < x3 || x4 < x1lo )
			return LCTR_NO_COLLISION;
	}

	Ay = y2 - y1;
	By = y3 - y4;

	if( Ay < 0 )
	{
		y1lo = y2;
		y1hi = y1;
	}
	else
	{
		y1hi = y2;
		y1lo = y1;
	}

	if( By > 0 )
	{
		if( y1hi < y4 || y3 < y1lo )
			return LCTR_NO_COLLISION;
	}
	else
	{
		if( y1hi < y3 || y4 < y1lo )
			return LCTR_NO_COLLISION;
	}

	Cx = x1 - x3;
	Cy = y1 - y3;
	d = By*Cx - Bx*Cy;
	f = Ay*Bx - Ax*By;

	if( f > 0 )
	{
		if( d< 0 || d > f )
			return LCTR_NO_COLLISION;
	}
	else
	{
		if( d > 0 || d < f )
			return LCTR_NO_COLLISION;
	}

	e = Ax*Cy - Ay*Cx;

	if( f > 0 )
	{
		if( e < 0 || e > f )
			return LCTR_NO_COLLISION;
	}
	else
	{
		if( e > 0 || e < f )
			return LCTR_NO_COLLISION;
	}

	if( f == 0 )
		return LCTR_PARALLEL;

	num = d*Ax;
	if( (num >= 0.0f && f >= 0.0f) || (num < 0.0f && f < 0.0f) )
		offset = f / 2.0f;
	else
		offset = -f / 2.0f;

	if( ( ( num < 0.0f ) && ( f < 0.0f ) ) || ( ( num >= 0.0f ) && (f >= 0.0f) ) )
		*x = x1 + ((num+offset) / f ) - 0.5f;
	else
		*x = x1 + ((num+offset) / f ) + 0.5f;

	num = d * Ay;
	if( ( num >= 0.0f && f >= 0.0f ) || ( num < 0.0f && f < 0.0f ) )
		offset = f / 2.0f;
	else
		offset = -f / 2.0f;

	if( ( ( num < 0.0f ) && ( f < 0.0f ) ) || ( ( num >= 0.0f ) && ( f >= 0.0f ) ) )
		*y = y1 + ((num+offset) / f) - 0.5f;
	else
		*y = y1 + ((num+offset) / f ) + 0.5f;

	return LCTR_INTERSECT;
}