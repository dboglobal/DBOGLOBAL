/******************************************************************************
* File			: StringLocalConverter_LocalBase.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc			:
*****************************************************************************/

#ifndef __GUI_STRING_LOCAL_CONVERTER_LOCALBASE_H__
#define __GUI_STRING_LOCAL_CONVERTER_LOCALBASE_H__

START_GUI

class CStringLocalConverter_LocalBase
{
public:
	CStringLocalConverter_LocalBase() {}
	virtual ~CStringLocalConverter_LocalBase() {}

	virtual void	LocalStringCovert(WCHAR* pwcOutput, const WCHAR* pwcText) { pwcOutput = (WCHAR*)pwcText; }
};

END_GUI

#endif // __GUI_STRING_LOCAL_CONVERTER_LOCALBASE_H__