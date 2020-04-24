#ifndef __NTLPLWORLDPROPERTY_H__
#define __NTLPLWORLDPROPERTY_H__

#include "NtlPLProperty.h"

class CNtlPLWorldProp : public CNtlPLProperty
{
public:
	CNtlPLWorldProp(void) { Init();; }
	virtual ~CNtlPLWorldProp(void) {;}

protected:
	std::string	m_strBspFileName;
	std::string	m_strResourceFilePath;

public:
	void			SetBspFileName( const RwChar * pName ) { m_strBspFileName = pName; }
	void			SetResourceFilePath( const RwChar * pName ) { m_strResourceFilePath = pName; }

	const RwChar *	GetBspFileName( void ) const { return m_strBspFileName.c_str(); }
	const RwChar *	GetResourceFilePath( void ) const { return m_strResourceFilePath.c_str(); }

public:

	virtual RwBool	Load( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );
	virtual RwBool	Save( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode );

protected:
	virtual void	Init( void );
};

#endif