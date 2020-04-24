////////////////////////////////////////////////////////////////////////////////
// Name: 
// Desc: GUI 공용 유틸 함수들.
//
// 2006.09.26 Peessi@hitel.net   
//
// To Be Desired:
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_UTIL_H__
#define __GUI_UTIL_H__

START_GUI

unsigned char RandomNuber( unsigned char uMin, unsigned char uMax );
INT	  RandomNumber( INT nMin, INT nMax );
DWORD RandomNumber( DWORD dwMin, DWORD dwMax );
FLOAT RandomNumber( FLOAT fMin, FLOAT fMax );

UINT  _ttoh(const CHAR *String);
UINT  _ttoh(const WCHAR *String); 

std::wstring GetHtmlFromFileString(const CHAR *pFileName);
std::wstring GetHtmlFromMemoryString(const CHAR *pMemory, INT nSize, BOOL bNewLine = FALSE);
std::wstring GetHtmlFromMemoryString(const WCHAR *pMemory, INT nSize, BOOL bNewLine = FALSE);

FLOAT GetResultLineInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fEndValue );
FLOAT GetResult2BezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fControlValue, FLOAT fEndValue );
FLOAT GetResult3BezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fControlValue1, FLOAT fControlValue2, FLOAT fEndValue );
FLOAT GetResult4BezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fControlValue1, FLOAT fControlValue2, FLOAT fControlValue3, FLOAT fEndValue );
FLOAT GetResultBezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT* fValue, INT nCount ); 

FLOAT GetValueByPercentOfStart( FLOAT fStartValue, FLOAT fEndValue, INT nPercent );

VOID SetComponentColor( CComponent* pComopnent, DWORD dwColor );

VOID RemoveLineFeed( std::wstring& wstr );
VOID RemoveCStyleLineFeed( std::wstring& wstr );
VOID ChangeLineFeedToCStyle( std::wstring& wstr );

float RwV2dLineInterpolation( RwV2d* v2dOutput, RwV2d* v2dStart, RwV2d* v2dEnd, float fTime, float fGoalTime );

// 문자열 관련
void ExtractTokenByLastChar( char* pStr, char* pBuffer, int nLen, const char *pMark );
BOOL ExtractTokenByFind( char* pOutBuffer, int nBufferSize, char* pSrcBuffer, char* pToken, int nTokenSize );
void RemoveSpaceForSelfBuffer( char* pStrBuffer );

END_GUI

#endif//__GUI_UTIL_H__