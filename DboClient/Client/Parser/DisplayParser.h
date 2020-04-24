/*****************************************************************************
 *
 * File			: DisplayPaser.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 20	
 * Abstract		: DBO gui displayer paser
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __DISPLAY_PARSER__
#define __DISPLAY_PARSER__

#include "NtlTokenOption.h"

class CDisplayParser
{
public:

	typedef std::map<std::string, std::wstring> MapDataOption;

private:

	MapDataOption m_DataOption;

	RwBool ParseUnicodeData(const std::string &strFilename);

public:

	CDisplayParser(const std::string &strDataFileName);

	const MapDataOption& GetDataOptions(void) const { return m_DataOption; }
};

#endif