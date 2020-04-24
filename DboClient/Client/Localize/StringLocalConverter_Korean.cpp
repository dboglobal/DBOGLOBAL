#include "precomp_dboclient.h"
#include "StringLocalConverter_Korean.h"

// core
#include "NtlDebug.h"

namespace
{
	#define dUNICODE_HANGULE_SYLLABLES_FIRST			(44032)
};


void CStringLocalConverter_Korean::LocalStringCovert(WCHAR* pwcOutput, const WCHAR* pwcText)
{
	RwInt32 iCount = 0;
	RwInt32 iTextLength = wcslen(pwcText);

	// 조사가 붙으려면 최소한 2글자 이상이여야 한다
	if( iTextLength < 2 )
	{
		memcpy(pwcOutput, pwcText, sizeof(WCHAR) * wcslen(pwcText) );
		return;
	}

	memcpy(pwcOutput, &(pwcText[0]), sizeof(WCHAR));
	++iCount;

	while( iCount < iTextLength )
	{
		if( wcscmp( L"잃", &(pwcText[iCount]) ) == 0 )
		{
			int iThirdSound = GetKoreanThirdSound( &(pwcText[iCount - 1]) );

			if( iThirdSound == 0 )	// 받침이 없다
				memcpy(pwcOutput + iCount, L"가", sizeof(WCHAR));
			else
				memcpy(pwcOutput + iCount, L"이", sizeof(WCHAR));

			++iCount;
			continue;
		}
		else if( wcscmp( L"읋", &(pwcText[iCount]) ) == 0 )
		{
			int iThirdSound = GetKoreanThirdSound( &(pwcText[iCount - 1]) );

			if( iThirdSound == 0 )	// 받침이 없다
				memcpy(pwcOutput + iCount, L"를", sizeof(WCHAR));
			else
				memcpy(pwcOutput + iCount, L"을", sizeof(WCHAR));

			++iCount;
			continue;
		}
		else if( wcscmp( L"읂", &(pwcText[iCount]) ) == 0 )
		{
			int iThirdSound = GetKoreanThirdSound( &(pwcText[iCount - 1]) );

			if( iThirdSound == 0 )	// 받침이 없다
				memcpy(pwcOutput + iCount, L"는", sizeof(WCHAR));
			else
				memcpy(pwcOutput + iCount, L"은", sizeof(WCHAR));

			++iCount;
			continue;
		}
		else if( wcscmp( L"왆", &(pwcText[iCount]) ) == 0 )
		{
			int iThirdSound = GetKoreanThirdSound( &(pwcText[iCount - 1]) );

			if( iThirdSound == 0 )	// 받침이 없다
				memcpy(pwcOutput + iCount, L"와", sizeof(WCHAR));
			else
				memcpy(pwcOutput + iCount, L"과", sizeof(WCHAR));

			++iCount;
			continue;
		}
		else if( wcscmp( L"롫", &(pwcText[iCount]) ) == 0 )
		{
			int iThirdSound = GetKoreanThirdSound( &(pwcText[iCount - 1]) );

			if( iThirdSound == 0 && iThirdSound == 8 )	// 받침이 없거나 'ㄹ' 이다
			{
				memcpy(pwcOutput + iCount, L"로", sizeof(WCHAR));
				++iCount;
			}
			else
			{
				memcpy(pwcOutput + iCount, L"으로", sizeof(WCHAR) * 2);
				iCount += 2;
			}

			continue;
		}

		memcpy(pwcOutput + iCount, &(pwcText[iCount]), sizeof(WCHAR));
		++iCount;
	}

	return;
}

RwInt32 CStringLocalConverter_Korean::GetKoreanThirdSound(const WCHAR* pwcText)
{
	if( !pwcText )
		return 0;

	int iIndex_from_HangulFirst = pwcText[0] - dUNICODE_HANGULE_SYLLABLES_FIRST;

	// 유니코드상 한글 음절의 범위 검사
	if( iIndex_from_HangulFirst < 0 || iIndex_from_HangulFirst >= 11172 )
		return 0;

	//int iFirstSound	= iIndex_from_HangulFirst / (21*28);				///< 초성 순서값
	//int iSecondSound	= iIndex_from_HangulFirst % (21*28) / 28;			///< 중성 순서값
	int iThirdSound		= iIndex_from_HangulFirst % (21*28) % 28;			///< 종성 순서값

	return iThirdSound;
}