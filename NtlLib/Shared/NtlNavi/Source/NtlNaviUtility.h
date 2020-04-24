#ifndef _NTL_NAVI_UTILITY_H_
#define _NTL_NAVI_UTILITY_H_


//--------------------------------------------------------------------
//	File chunk
//--------------------------------------------------------------------


struct sFILE_CHUNK_INFO
{
	sFILE_CHUNK_INFO( void ) { return; }

	sFILE_CHUNK_INFO( long lChunkSize, char* pChunk )
	{
		this->lChunkSize = lChunkSize;
		this->pChunk = pChunk;
	}

	long						lChunkSize;
	char*						pChunk;
};


extern sFILE_CHUNK_INFO	LoadFileChunk( const char* pFileName );

extern void UnloadFileChunk( sFILE_CHUNK_INFO& sFileChunkInfo );


//--------------------------------------------------------------------
//	Attach back slash
//--------------------------------------------------------------------


extern void AttachBackSlash( std::string& strString );


//--------------------------------------------------------------------
//	Upper case
//--------------------------------------------------------------------


extern std::string MakeUpperCase( std::string& strString );

extern std::string MakeUpperCase( const char* pString );


//--------------------------------------------------------------------
//	Radian <=> Degree
//--------------------------------------------------------------------

extern float RadianToDegree( float fRadian );

extern float DegreeToRadian( float fDegree );

//--------------------------------------------------------------------
// Line to Line collision test
//--------------------------------------------------------------------

enum eLineCollisionTestResult
{
	LCTR_NO_COLLISION = 0,
	LCTR_INTERSECT,
	LCTR_PARALLEL
};

extern int LineToLineIntersect2D( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float *x, float *y );


#endif