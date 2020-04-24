/*****************************************************************************
 *
 * File			: NtlPLItemFileData.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 2.	
 * Abstract		: NTL CNtlPLItemFileData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _CNTL_PL_ITEM_FILE_PARSER_
#define _CNTL_PL_ITEM_FILE_PARSER_

#include "NtlPLItemProperty.h"
#include "NtlPLCharacterParser.h"

class CNtlPLItemParser
{
public:
    CNtlPLItemParser();

	static	CNtlPLItemParser &GetInstance();

	const std::string &GetLastError() { return m_strError; }

    CNtlMatchingTable* GetItemMatchTable() {return &m_ItemMatchTable;}          ///< Item Animation Table을 반환한다	

protected:    
    void    CreateKeyword();                                ///< Animation Table 정보를 만든다.

protected:
    std::string				m_strError;
    
    CNtlMatchingTable       m_ItemMatchTable;               ///< Item Animation Table	

};

#endif