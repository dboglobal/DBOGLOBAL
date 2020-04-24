#include "Precomp_NtlPathEngine.h"
#include "NtlPathEngineHelper.h"

#include "NtlPathEngineLog.h"

bool NTLPE_RangesOverlap( const cHorizontalRange& r1, const cHorizontalRange& r2 )
{
	if(r1.minX > r2.maxX)
	{
		return false;
	}
	if(r1.maxX < r2.minX)
	{
		return false;
	}
	if(r1.minY > r2.maxY)
	{
		return false;
	}
	if(r1.maxY < r2.minY)
	{
		return false;
	}
	return true;
}

// srcRange에 destRange가 포함되나 안되나 확인
extern bool NTLPE_RangesInclusion( const cHorizontalRange& srcRange, const cHorizontalRange& destRange )
{
	if( srcRange.maxX < destRange.maxX )
		return false;

	if( srcRange.maxY < destRange.maxY )
		return false;

	if( srcRange.minX > destRange.minX )
		return false;

	if( srcRange.minY > destRange.minY )
		return false;

	return true;
}

/**
* \brief 각 Min Max의 중앙 의 거리를 구한다.
*/
extern int NTLPE_RangesLengthTo( const cHorizontalRange& r1, const cHorizontalRange& r2 )
{
	//int nCenterX = (r2.minX + ((r2.maxX - r2.minX)/2)) - (r1.minX + ((r1.maxX - r1.minX)/2));
	//int nCenterY = (r2.minY + ((r2.maxY - r2.minY)/2)) - (r1.minY + ((r1.maxY - r1.minY)/2));

	float fCenterR1X = ( r1.minX + ((r1.maxX - r1.minX)/2) ) * 0.001f;
	float fCenterR1Y = ( r1.minY + ((r1.maxY - r1.minY)/2) ) * 0.001f;

	float fCenterR2X = ( r2.minX + ((r2.maxX - r2.minX)/2) ) * 0.001f;
	float fCenterR2Y = ( r2.minY + ((r2.maxY - r2.minY)/2) ) * 0.001f;

	float fCenterX = fCenterR1X - fCenterR2X;
	float fCenterY = fCenterR1Y - fCenterR2Y;

	float fSqrtLength = powf( fCenterX, 2 ) + powf( fCenterY, 2 );

	return (int)sqrtf( fSqrtLength );
}

char* NTLPE_LoadBinary_AllocatedBuffer( const char* filename, unsigned long& size )
{
	size = 0;
	FILE* fp = NULL;
	if( fopen_s( &fp, filename, "rb") != 0 )
	{
		CNtlPathEngineLog::GetInstance()->Log( "CNtlPathEngine::LoadBinary_AllocatedBuffer: Failed to open %s", filename );		
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buffer = new char[size + 1];
	fread(buffer, size, 1, fp);
	buffer[size] = NULL;
	fclose(fp);

	return buffer;
}

void NTLPE_FreeBuffer( char* buffer )
{
	if( buffer )
	{
		delete buffer;
		buffer = NULL;
	}
}