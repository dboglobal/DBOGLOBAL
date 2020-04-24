/******************************************************************************
* File			: StringLocalConverter_Korean.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc			: 한국어는 앞 글자 종성에 따라 변경이 되는 조사가 5가지 있다
*
*				  예 ) 을/를, 이/가, 은/는, 와/과, 
*					   종성이 없거나 및 종성이 'ㄹ'이면 로/으로
*****************************************************************************/

#ifndef __GUI_STRING_LOCAL_CONVERTER_KOREAN_H__
#define __GUI_STRING_LOCAL_CONVERTER_KOREAN_H__

#include "StringLocalConverter_LocalBase.h"


START_GUI

class CStringLocalConverter_Korean : public CStringLocalConverter_LocalBase
{
public:
	virtual void	LocalStringCovert(WCHAR* pwcOutput, const WCHAR* pwcText);

protected:
	///< 유니코드 한글 음절의 종성 순서값을 반환한다
	RwInt32			GetKoreanThirdSound(const WCHAR* pwcText);
};

END_GUI

#endif // __GUI_STRING_LOCAL_CONVERTER_KOREAN_H__