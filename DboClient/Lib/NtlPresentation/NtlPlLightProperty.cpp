#include "precomp_ntlpresentation.h"
#include "NtlPLLightProperty.h"
#include "NtlXMLDoc.h"
#include "NtlDebug.h"

CNtlPLLightProperty::CNtlPLLightProperty()
{
	SetClassID( "CNtlPLLightProperty" );
}


CNtlPLLightProperty::~CNtlPLLightProperty()
{
}


RwBool CNtlPLLightProperty::Load( CNtlXMLDoc * pDoc, IXMLDOMNode * pNode )
{
	NTL_FUNCTION( "CNtlPLLightProperty::Load" );

//	NTL_PRE( pDoc );
//	NTL_PRE( pNode );

	char		chBuffer[1024];

	if( !pDoc->GetTextWithAttributeName( pNode, "id", chBuffer, 1024 ) )
	{
		NTL_RETURN( FALSE );
	}

	SetId( atoi( chBuffer ) );
	
	if(!pDoc->GetTextWithAttributeName( pNode, "name", chBuffer, 1024 ) )
	{
		NTL_RETURN( FALSE );
	}

	SetName( chBuffer );
	
	NTL_RETURN( TRUE );
}


RwBool CNtlPLLightProperty::Save( CNtlXMLDoc * pDoc, IXMLDOMNode * pNode )
{
	return TRUE;
}