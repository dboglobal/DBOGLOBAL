/******************************************************************************
* File			: StringLocalConverter.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc			: Change words or letters according to language and country.
*				  For example, Korean /, and / etc ...
*
*				  Japanese is not applicable.
*****************************************************************************/

#ifndef __GUI_STRING_LOCAL_CONVERTER_H__
#define __GUI_STRING_LOCAL_CONVERTER_H__

START_GUI


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

	///< Returns the modified string depending on the language-specific situation
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

END_GUI

#endif //__GUI_STRING_LOCAL_CONVERTER_H__