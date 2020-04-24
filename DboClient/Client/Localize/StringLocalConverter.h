/******************************************************************************
* File			: StringLocalConverter.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc			: 언어별, 국가별 상황에 따른 단어 혹은 글자의 변경을 한다.
*				  예) 한국어의 을/를, 이/가 등등..
*
*				  일본어는 적용 대상이 없다.
*****************************************************************************/

#pragma once

class CStringLocalConverter_LocalBase;

class CStringLocalConverter
{
public:
	#define dSTRING_CONVERTER_BUFFER_SIZE		1024

	static CStringLocalConverter* GetInstance();
	static RwBool	CreateInstance();
	static void		DestroyInstance();

	enum eLocalType
	{
		LOCAL_TYPE_KOREA,

		INVALID_LOCAL_TYPE
	};

	typedef std::map<RwUInt8, CStringLocalConverter_LocalBase*>		MAP_LOCAL_CONVERTER;

public:
	CStringLocalConverter() {}
	virtual ~CStringLocalConverter() {}

	///< 언어별 상황에 따라 변경된 스트링을 반환한다
	const WCHAR*	LocalStringCovert(const WCHAR* pwcText);

protected:
	static CStringLocalConverter* m_pInstance;

	WCHAR						m_awcBuffer[dSTRING_CONVERTER_BUFFER_SIZE];
	MAP_LOCAL_CONVERTER			m_mapLocalConverter;
};

static CStringLocalConverter* GetStringConverter()
{
	return CStringLocalConverter::GetInstance();
}