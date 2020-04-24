#include "precomp_ntlpresentation.h"

#include "NtlDebug.h"
#include "NtlXMLDoc.h"

#include "NtlPLSoundProp.h"

CNtlPLSoundProp::CNtlPLSoundProp(void)
{
	SetClassID("CNtlPLSoundProp");
}

CNtlPLSoundProp::~CNtlPLSoundProp(void)
{
}

RwBool CNtlPLSoundProp::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLSoundProp::Load");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLSoundProp::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLSoundProp::Save");

	NTL_RETURN(TRUE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLSoundFieldProp::CNtlPLSoundFieldProp(void)
{
	SetClassID("CNtlPLSoundFieldProp");
}

CNtlPLSoundFieldProp::~CNtlPLSoundFieldProp(void)
{
}

RwBool CNtlPLSoundFieldProp::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLSoundFieldProp::Load");

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNodeList*	pNodeList					= NULL;
	IXMLDOMNode*		pNodeChild					= NULL;

	RwInt32				iIdx;
	std::string			strName;

	// INDEX
	if(!pDoc->GetTextWithAttributeName(pNode, "index", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}	
	iIdx = ::atoi(chBuffer);

	// NAME
	if(!pDoc->GetTextWithAttributeName(pNode, "name", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	strName = chBuffer;

	// INSERT
	m_mapSound[iIdx] = strName;

	NTL_RETURN(TRUE);
}

RwBool CNtlPLSoundFieldProp::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLSoundFieldProp::Save");

	NTL_RETURN(TRUE);
}