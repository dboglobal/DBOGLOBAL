#include "precomp_ntlpresentation.h"
#include "NtlPLPlantProperty.h"
#include "NtlXMLDoc.h"
#include "NtlDebug.h"

CNtlPLPlantProperty::CNtlPLPlantProperty()
:m_fClipDistance(100.0f)
{
}

CNtlPLPlantProperty::~CNtlPLPlantProperty()
{
}

RwBool CNtlPLPlantProperty::Load( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode )
{
	NTL_FUNCTION("CNtlPLPlantProperty::Load");

	NTL_PRE(pDoc);
	NTL_PRE(pNode);

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNodeList*	pList						= NULL;
	IXMLDOMNode*		pChildNode					= NULL;

	pNode->get_childNodes(&pList);
	if (!pList)
	{
		NTL_RETURN(FALSE);
	}

	sPLANT_PROP			sPlantProp;
	RwInt32				iIdx;

	if(!pDoc->GetTextWithAttributeName(pNode, "index", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	iIdx = ::atoi(chBuffer);

	if(!pDoc->GetTextWithAttributeName(pNode, "name", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sPlantProp.strName = chBuffer;

	if(!pDoc->GetTextWithAttributeName(pNode, "nummin", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sPlantProp.iNumMin = ::atoi(chBuffer);

	if(!pDoc->GetTextWithAttributeName(pNode, "nummax", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sPlantProp.iNumMax = ::atoi(chBuffer);

	if(!pDoc->GetTextWithAttributeName(pNode, "scalemin", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sPlantProp.fScaleMin = static_cast<RwReal>(::atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNode, "scalemax", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sPlantProp.fScaleMax = static_cast<RwReal>(::atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNode, "rotationterrain", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sPlantProp.bIsRotationTerrain = static_cast<RwBool>(::atoi(chBuffer));

	long lNum;
	pList->get_length(&lNum);
	for (long i = 0; i < lNum; ++i)
	{
		pList->get_item(i, &pChildNode);

		BSTR bstr = NULL;
		if (FAILED(pChildNode->get_nodeName(&bstr)) || wcscmp(bstr, L"set_res_element"))
		{
			SysFreeString(bstr);
			pChildNode->Release();
			pChildNode = NULL;
			continue;
		}
		SysFreeString(bstr);

		if(!pDoc->GetTextWithAttributeName(pChildNode, "name", chBuffer, NTL_MAX_DIR_PATH))
		{
			NTL_RETURN(FALSE);
		}

		sPlantProp.vecstrResourceName.push_back(chBuffer);

		if (pChildNode)
		{
			pChildNode->Release();
			pChildNode = NULL;
		}
	}

	if (pList)
	{
		pList->Release();
		pList = NULL;
	}

	m_mapProperty[iIdx] = sPlantProp;

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPlantProperty::Save( CNtlXMLDoc *pDoc, IXMLDOMNode *pNode )
{
	return TRUE;
}