#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include ".\ntlplworldprop.h"


void CNtlPLWorldProp::Init( void )
{
	NTL_FUNCTION("CNtlPLWorldProp::Init");

	//	CNtlPLProperty::Init() )

	// To Do : Add Initialize Sequence for Terrain Property

	SetClassID("CNtlPLWorldProp");

	//m_strBspFileName.clear();
	//m_strResourceFilePath.clear();

	NTL_RETURNVOID();
}


RwBool CNtlPLWorldProp::Load( CNtlXMLDoc *pDoc, IXMLDOMNode * pNode )
{
	NTL_FUNCTION("CNtlPLWorldProp::Load");

//	m_strBspFileName.assign( "world\\field_1km.bsp" );
//	m_strResourceFilePath.assign( "texture\\world\\" );

	NTL_RETURN( TRUE );
}


RwBool CNtlPLWorldProp::Save( CNtlXMLDoc *pDoc, IXMLDOMNode * pNode )
{
	NTL_FUNCTION("CNtlPLWorldProp::Save");

	NTL_RETURN( TRUE );
}

