/*****************************************************************************
 *
 * File			: NtlPLGuiProperty.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 22	
 * Abstract		: Presentation layer gui property class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLGUI_PROPERTY_H__
#define __NTL_PLGUI_PROPERTY_H__

#include <string>
#include "NtlPLProperty.h"

/**
 * \ingroup NtlPresentation
 * CNtlPLGui에 해당하는 property class이다.
 * property data는 xml file로 작성되어져 있다.
 *
 */

class CNtlPLGuiProperty : public CNtlPLProperty
{
public:

	static unsigned int m_strVer;			/* gui property file version **/
	static std::string m_strScriptPath;		/* gui script path name **/ 
	static std::string m_strTexPath;		/* gui texture file의 path name **/ 
	
	std::string m_strResFile;				/* gui resource script file name **/ 
	std::string m_strSurFile;				/* gui surface script file name **/ 
	std::string m_strCompFile;				/* gui component script file name **/ 

public:

	/**
    *  Default constructor for Presentation object property
    *
    */
	CNtlPLGuiProperty();

	/**
    *  destructor for Presentation object property
    *
    */
	virtual ~CNtlPLGuiProperty();

	/**
    *  xml file로부터 object property data를 loading하는 함수
	*  \param pDoc는 xml wrapping class pointer.
	*  \param pNod는 유효한 xml node.
    *
    */
	virtual RwBool	Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode);

	/**
    *  xml file로 object property data를 save하는 함수
	*  \param pDoc는 xml wrapping class pointer.
	*  \param pNod는 유효한 xml node.
    *
    */
	virtual RwBool	Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );
};


#endif