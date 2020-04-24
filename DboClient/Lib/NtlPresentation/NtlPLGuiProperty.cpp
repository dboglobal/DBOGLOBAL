#include "precomp_ntlpresentation.h"
#include "NtlPLGuiProperty.h"
#include "NtlXMLDoc.h"
#include "NtlDebug.h"


unsigned int CNtlPLGuiProperty::m_strVer;
std::string CNtlPLGuiProperty::m_strScriptPath;
std::string CNtlPLGuiProperty::m_strTexPath;


CNtlPLGuiProperty::CNtlPLGuiProperty()
{
	SetClassID("CNtlPLObjectProperty");
}


CNtlPLGuiProperty::~CNtlPLGuiProperty()
{
}


RwBool CNtlPLGuiProperty::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLObjectProperty::Load");

	NTL_PRE(pDoc);
	NTL_PRE(pNode);

	char chBuffer[1024];
	
	if(!pDoc->GetTextWithAttributeName(pNode, "name", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	SetName(chBuffer);

	if(!pDoc->GetTextWithAttributeName(pNode, "res", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	m_strResFile = chBuffer;

	if(!pDoc->GetTextWithAttributeName(pNode, "sur", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	m_strSurFile = chBuffer;

	if(!pDoc->GetTextWithAttributeName(pNode, "comp", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	m_strCompFile = chBuffer;

	
	NTL_RETURN(TRUE);
}

RwBool CNtlPLGuiProperty::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	return TRUE;
}