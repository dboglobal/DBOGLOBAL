#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlPLWaterProp.h"
#include "NtlXMLDoc.h"


CNtlPLWaterProp::CNtlPLWaterProp(void)
{
	NTL_FUNCTION("CNtlPLWaterProp::Init");

	NTL_RETURNVOID();
}


CNtlPLWaterProp::~CNtlPLWaterProp(void)
{
	NTL_FUNCTION("CNtlPLWaterProp::Init");

	m_vecWaterProp.clear();

	NTL_RETURNVOID();
}

RwBool CNtlPLWaterProp::Load( CNtlXMLDoc *pDoc, IXMLDOMNode * pNode )
{
	NTL_FUNCTION("CNtlPLWaterProp::Load");
	NTL_PRE(pDoc);
	NTL_PRE(pNode);


	sWATER_PROP WaterProp;
	char		chBuffer[dWATER_PROP_MAX_STRING_SIZE];

	if(!pDoc->GetTextWithAttributeName(pNode, "name", chBuffer, dWATER_PROP_MAX_STRING_SIZE))
	{
		NTL_RETURN(FALSE);
	}
	else
	{
		WaterProp._Name = chBuffer;
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "fps", chBuffer, dWATER_PROP_MAX_STRING_SIZE))
	{
		NTL_RETURN(FALSE);
	}
	else
	{
		WaterProp._FPS = static_cast<RwReal>(::atof(chBuffer));
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "frame", chBuffer, dWATER_PROP_MAX_STRING_SIZE))
	{
		NTL_RETURN(FALSE);
	}
	else
	{
		WaterProp._NumFrame = ::atoi(chBuffer);
	}

	if(!pDoc->GetTextWithAttributeName(pNode, "specular", chBuffer, dWATER_PROP_MAX_STRING_SIZE))
	{
		NTL_RETURN(FALSE);
	}
	else
	{
		WaterProp._Specular = (::atoi(chBuffer) != 0);
	}

	m_vecWaterProp.push_back(WaterProp);

	NTL_RETURN( TRUE );
}


RwBool CNtlPLWaterProp::Save( CNtlXMLDoc *pDoc, IXMLDOMNode * pNode )
{
	NTL_FUNCTION("CNtlPLWaterProp::Save");

	

	NTL_RETURN( TRUE );
}