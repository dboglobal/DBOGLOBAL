#include "gui_precomp.h"
#include "GuiUtil.h"
#include "gui_html_doc.h"
#include "gui_htmlbox_item.h"

START_GUI

unsigned char RandomNuber( unsigned char uMin, unsigned char uMax )
{
	if( uMin == uMax )
		return uMin;

	return( ( rand() % ( uMax - uMin ) + 1 ) + uMin );
}

INT RandomNumber( INT nMin, INT nMax )
{
	if( nMin == nMax )
		return nMin;

	return( ( rand() % ( abs( nMax - nMin ) + 1 ) ) + nMin );
}

DWORD RandomNumber( DWORD dwMin, DWORD dwMax )
{
	if( dwMin == dwMax )
		return dwMin;

	return( ( rand() % ( dwMax - dwMin ) + 1 ) + dwMin );
}

FLOAT RandomNumber( FLOAT fMin, FLOAT fMax )
{
	if( fMin == fMax )
		return fMin;

	FLOAT fRandom = (FLOAT)rand() / (FLOAT)0x7fff;
	return ( ( fRandom * (FLOAT)fabs( fMax - fMin ) ) + fMin );
}

UINT  _ttoh(const CHAR *String)
{
	UINT Value = 0, Digit;
	CHAR c;

	while ((c = *String++) != 0) {

		if (c >= '0' && c <= '9')
			Digit = (UINT) (c - '0');
		else if (c >= 'a' && c <= 'f')
			Digit = (UINT) (c - 'a') + 10;
		else if (c >= 'A' && c <= 'F')
			Digit = (UINT) (c - 'A') + 10;
		else
			break;

		Value = (Value << 4) + Digit;
	}

	return Value;
}

UINT gui::_ttoh( const WCHAR *String ) 
{
	UINT Value = 0, Digit;
	WCHAR c;

	while ((c = *String++) != 0) {

		if (c >= '0' && c <= '9')
			Digit = (UINT) (c - '0');
		else if (c >= 'a' && c <= 'f')
			Digit = (UINT) (c - 'a') + 10;
		else if (c >= 'A' && c <= 'F')
			Digit = (UINT) (c - 'A') + 10;
		else
			break;

		Value = (Value << 4) + Digit;
	}

	return Value;
}

std::wstring GetHtmlFromFileString(const CHAR *pFileName)
{
	CHtmlDoc doc;
	doc.SetHtmlFromFile(NULL, pFileName);

	CHtmlDoc::ListHtmlItem::iterator it;
	CHtmlDoc::ListHtmlItem listItems = doc.GetItems();

	std::wstring wstr;
	const WCHAR *pText;
	CHtml_Item *pItem;

	for(it = listItems.begin(); it != listItems.end(); it++)
	{
		pItem = (*it);
		if(pItem->GetType() == HTML_ITEM_STRING)
		{
			pText = ((CHtml_StringItem*)pItem)->GetText();
			if(pText)
			{
				wstr += pText;
			}
		}
	}

	return wstr;
}

std::wstring GetHtmlFromMemoryString(const CHAR *pMemory, INT nSize, BOOL bNewLine /* = FALSE */)
{
	WCHAR awcBuffer[1024] = L" ";
	MultiByteToWideChar( GetACP(), 0, pMemory, -1, awcBuffer, 1024 );

	return gui::GetHtmlFromMemoryString( awcBuffer, (INT)wcslen(awcBuffer) );
}
 
std::wstring GetHtmlFromMemoryString(const WCHAR *pMemory, INT nSize, BOOL bNewLine /* = FALSE */)
{
	CHtmlDoc doc;
	doc.SetHtmlFromMemory(NULL, pMemory, nSize);

	CHtmlDoc::ListHtmlItem::iterator it;
	CHtmlDoc::ListHtmlItem listItems = doc.GetItems();

	std::wstring wstr;
	const WCHAR *pText;
	CHtml_Item *pItem;

	for(it = listItems.begin(); it != listItems.end(); it++)
	{
		pItem = (*it);
		if(pItem->GetType() == HTML_ITEM_STRING)
		{
			pText = ((CHtml_StringItem*)pItem)->GetText();
			if(pText)
			{
				wstr += pText;
			}
		}
		else if( bNewLine && pItem->GetType() == HTML_ITEM_NEWLINE )
		{
			wstr += L"\\n";
		}
	}

	return wstr;
}

FLOAT GetResultLineInterpolation( RwReal fTime, RwReal fGoalTime, RwReal fStartValue, RwReal fEndValue )
{
	FLOAT fFactor = fTime / fGoalTime;

	if( fFactor > 1.0f || fGoalTime == 0.0f )
		return fEndValue;
	else if( fFactor < 0.0f )
		return fStartValue;
	else
		return fFactor * fEndValue + 
			   ( 1 - fFactor ) * fStartValue;
//B1(t) t
//B2(t) (1-t)
}

FLOAT GetResult2BezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fControlValue, FLOAT fEndValue )
{
	FLOAT fFactor = fTime / fGoalTime;

	if( fFactor > 1.0f || fGoalTime == 0.0f )
		return fEndValue;
	else if( fFactor < 0.0f )
		return fStartValue;
	else
		return fFactor * fFactor * fEndValue + 
			   2 * fFactor * ( 1 - fFactor ) * fControlValue + 
			   ( 1 - fFactor ) * ( 1 - fFactor ) * fStartValue;
					
//B1(t) (t*t) 
//B2(t) (2*t*(1-t)) 
//B3(t) ((1-t)*(1-t))
}

FLOAT GetResult3BezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fControlValue1, FLOAT fControlValue2, FLOAT fEndValue )
{
	FLOAT fFactor = fTime / fGoalTime;

	if( fFactor > 1.0f || fGoalTime == 0.0f )
		return fEndValue;
	else if( fFactor < 0.0f )
		return fStartValue;
	else
		return fFactor * fFactor * fFactor * fEndValue +
			   3 * fFactor * fFactor * ( 1 - fFactor ) * fControlValue2 +
			   3 * fFactor * ( 1 - fFactor ) * ( 1 - fFactor ) * fControlValue1 +
			   ( 1 - fFactor ) * ( 1 - fFactor ) * ( 1 - fFactor ) * fStartValue;

//B1(t) = t * t * t 
//B2(t) = 3 * t * t * (1 - t) 
//B3(t) = 3 * t * (1 - t) * (1 - t) 
//B4(t) = (1 - t) * (1 - t) * (1 - t)
}

FLOAT GetResult4BezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT fStartValue, FLOAT fControlValue1, FLOAT fControlValue2, FLOAT fControlValue3, FLOAT fEndValue )
{
	FLOAT fFactor = fTime / fGoalTime;

	if( fFactor > 1.0f || fGoalTime == 0.0f )
		return fEndValue;
	else if( fFactor < 0.0f )
		return fStartValue;
	else
		return fFactor * fFactor * fFactor * fFactor * fEndValue +
			   4 * fFactor * fFactor * fFactor * ( 1 - fFactor ) * fControlValue3 +
			   6 * fFactor * fFactor * ( 1 - fFactor ) * ( 1 - fFactor ) * fControlValue2 +
			   4 * fFactor * ( 1 - fFactor ) * ( 1 - fFactor ) * ( 1 - fFactor ) * fControlValue1 +
			   ( 1 - fFactor ) * ( 1 - fFactor ) * ( 1 - fFactor ) * ( 1 - fFactor ) * fStartValue;

//	B1(t) = t * t * t * t 
//	B2(t) = 4 * t * t * t * (1 - t) 
//	B3(t) = 6 * t * t * (1 - t) * (1 - t) 
//	B4(t) = 4 * t * (1 - x) * (1 - t) * (1 - t) 
//	B5(t) = (1 - t) * (1 - t) * (1 - t) * (1 - t)
}

FLOAT GetResultBezierInterpolation( FLOAT fTime, FLOAT fGoalTime, FLOAT* fValue, INT nCount )
{
	FLOAT fFactor = fTime / fGoalTime;
		
	if( fFactor > 1.0f || fGoalTime == 0.0f )
		return fValue[nCount - 1];
	else if( fFactor < 0.0f )
		return fValue[0];
	else
	{
		FLOAT fResult = 0.0f;

		FLOAT fMuk, fMunk, fBlend;
		INT	nN, nK, nKN;

		--nCount;
		fMuk = 1;
		fMunk = pow( 1 - fFactor, (FLOAT)nCount );

		for( INT i = 0 ; i <= nCount ; ++i )
		{
			nN = nCount;
			nK = i;
			nKN = nCount - i;
			fBlend = fMuk * fMunk;
			fMuk *= fFactor;
			fMunk /= ( 1 - fFactor );

			while( nN >= 1 )
			{
				fBlend *= nN;
				nN--;

				if( nK > 1 )
				{
					fBlend /= (FLOAT)nK;
					nK--;
				}

				if( nKN > 1)
				{
					fBlend /= (FLOAT)nKN;
					nKN--;
				}
			}

			fResult += fValue[i] * fBlend;
		}

		return fResult;
	}
}

FLOAT GetValueByPercentOfStart( FLOAT fStartValue, FLOAT fEndValue, INT nPercent )
{
	return ( ( fEndValue - fStartValue ) * nPercent * 0.01f ) + fStartValue;
}

VOID SetComponentColor(CComponent* pComopnent, DWORD dwColor )
{
	pComopnent->SetColor( GetRValue( dwColor ), GetGValue( dwColor ), GetBValue( dwColor ) );
}

VOID RemoveLineFeed( std::wstring& wstr )
{
	while( TRUE )
	{
		std::wstring::size_type nPos = wstr.find( L"\\n" );

		if( nPos == std::wstring::npos )
			break;

		wstr.erase( nPos, 2 );
	}
}

VOID RemoveCStyleLineFeed( std::wstring& wstr )
{
	while( TRUE )
	{
		std::wstring::size_type nPos = wstr.find( L"\n" );

		if( nPos == std::wstring::npos )
			break;

		wstr.erase( nPos, 1 );		
	}
}

VOID ChangeLineFeedToCStyle( std::wstring& wstr )
{
	while( TRUE )
	{
		std::wstring::size_type nPos = wstr.find( L"\\n" );

		if( nPos == std::wstring::npos )
			break;

		wstr.erase( nPos, 2 );
		wstr.insert( nPos, L"\n" );
	}
}

/**
* \brief 2d Vector의 선형 보간 함수
* \param v2dOutput		(RwV2d*) 결과값이 나올 벡터의 포인터
* \param v2dStart		(RwV2d*) 선형보간의 첫번째 벡터
* \param v2dEnd			(RwV2d*) 선형보간의 두번째 벡터
* \param fTime			(float)	보간될 위치
* \param fGoalTime		(float) 보간되는 위치의 최대값
* \return 보간된 벡터의 길이
*/
float RwV2dLineInterpolation( RwV2d* v2dOutput, RwV2d* v2dStart, RwV2d* v2dEnd, float fTime, float fGoalTime )
{
	RwV2d v2dResultA;
	RwV2d v2dResultB;

	// V = ( ( fGoalTime - fTime ) * v2dStart ) + ( fTime * v2dEnd )
	RwV2dScale( &v2dResultA, v2dStart, (fGoalTime - fTime) );
	RwV2dScale( &v2dResultB, v2dEnd, fTime );
	RwV2dAdd( v2dOutput, &v2dResultA, &v2dResultB );

	return RwV2dLength( v2dOutput );
}

/**
* \brief 문자열에서 주어진 캐릭터까지의 토큰을 추출한다.
* \param pStr			(char*) 문자열
* \param pBuffer		(char*) 추출된 토큰이 보관될 버퍼
* \param nLen			(char*) 버퍼의 길이
* \param pMark			(char*) 캐릭터
*/
void ExtractTokenByLastChar( char* pStr, char* pBuffer, int nLen, const char *pMark )
{
	char *endpos;
	int p;

	if( !strlen( pMark ) )
	{
		strcpy_s( pBuffer, nLen, pStr );
		return;
	}

	endpos = strstr( pStr, pMark );
	if( !endpos )
	{
		strcpy_s( pBuffer, nLen, pStr );
		return;
	}

	p = (int)(endpos - pStr);

	memset( pBuffer, 0, nLen );
	memcpy( pBuffer, pStr, p );

	strcpy_s( pStr, strlen(pStr), &pStr[p + strlen(pMark) ] );
}

/**
* \brief 원본 버퍼에서 토큰을 찾아 버퍼에 담는다. 성공 여부를 리턴 값으로 확인
* \param pBuffer		(char*) 버퍼
* \param nBufferSize	(int) 버퍼 사이즈
* \param pSrcBuffer		(char*) 대상 버퍼
* \param pToken			(char*) 찾을 토큰
* \return 성공 여부
*/
BOOL ExtractTokenByFind( char* pOutBuffer, int nBufferSize, char* pSrcBuffer, char* pToken, int nTokenSize )
{
	char* startpos = strstr( pSrcBuffer, pToken );
	if( startpos == NULL )
	{
		// 찾지 못함
		return FALSE;
	}
	
	memset( pOutBuffer, 0, sizeof(char) * nBufferSize);
	memcpy( pOutBuffer, startpos, nTokenSize );
	
	return TRUE;
}

/**
* \brief 주어진 버퍼에서 공백과 탭 문자, 라인 피드 문자를 제거한다.
*/
void RemoveSpaceForSelfBuffer( char* pStrBuffer )
{
	char* pTempStr = pStrBuffer;
	while( (*pStrBuffer) != '\0' )
	{
		if( ( (*pStrBuffer) != ' ' ) &&
			( (*pStrBuffer) != '\t' ) &&
			( (*pStrBuffer) != '\n' ) &&
			( (*pStrBuffer) != '\r' ) )
		{
			(*pTempStr) = (*pStrBuffer);
			pTempStr++;
		}

		pStrBuffer++;
	}

	(*pTempStr) = '\0';
}

END_GUI
