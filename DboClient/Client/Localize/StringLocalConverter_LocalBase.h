/******************************************************************************
* File			: StringLocalConverter_LocalBase.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2008. 12. 19
* Abstract		: 
*****************************************************************************
* Desc			:
*****************************************************************************/

#pragma once

class CStringLocalConverter_LocalBase
{
public:
	CStringLocalConverter_LocalBase() {}
	virtual ~CStringLocalConverter_LocalBase() {}

	virtual void	LocalStringCovert(WCHAR* pwcOutput, const WCHAR* pwcText) {}
};