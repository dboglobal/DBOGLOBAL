#include "precomp_ntlpresentation.h"
#include "LinkItemTrasformList.h"
#include "NtlDebug.h"
#include "NtlLogic.h"

#include "NtlPLResourcePack.h"
#include "NtlPLResourceManager.h"

CLinkItemTrasformChunk::CLinkItemTrasformChunk(eRACE_GENDER_CLASS_TYPE& _EGCT, sLINK_ITEM_TRASFORM& _LIT)
{
	m_LIT[_EGCT] = _LIT;
}

CLinkItemTrasformChunk::~CLinkItemTrasformChunk(void)
{

}

VOID CLinkItemTrasformChunk::Set(eRACE_GENDER_CLASS_TYPE& _EGCT, sLINK_ITEM_TRASFORM& _LIT)
{
	m_LIT[_EGCT] = _LIT;
}

sLINK_ITEM_TRASFORM* CLinkItemTrasformChunk::Get(eRACE_GENDER_CLASS_TYPE _EGCT)
{
	return &m_LIT[_EGCT];
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CLinkItemTrasformList& CLinkItemTrasformList::GetInstance()
{
	static CLinkItemTrasformList s_LinkItemTransformList;
	return s_LinkItemTransformList;
}

CLinkItemTrasformList::CLinkItemTrasformList(void)
{
	Create();
}

CLinkItemTrasformList::~CLinkItemTrasformList(void)
{
	Destroy();
}

void CLinkItemTrasformList::Refresh()
{
	Destroy();
	Create();
}

void CLinkItemTrasformList::Create()
{
	CNtlXMLDoc XMLDoc;              

	if(!XMLDoc.Create())
	{
		NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.Create()\n");
		return;
	}

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(dLINK_ITEM_TRANSFORM_LIST_XML_FILE_PATH, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!XMLDoc.LoadXML((char*)pTempBuffer))
			{			
				XMLDoc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.Load(dLINK_ITEM_TRANSFORM_LIST_XML_FILE_PATH)\n");
				return;
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(!XMLDoc.Load(dLINK_ITEM_TRANSFORM_LIST_XML_FILE_PATH))
		{
			XMLDoc.Destroy();
			NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.Load(dLINK_ITEM_TRANSFORM_LIST_XML_FILE_PATH)\n");
			return;
		}
	}

	IXMLDOMNodeList* pItemPropertyNodeList = XMLDoc.SelectNodeList((char*)"ITEM_TRANSFORM");
	if(pItemPropertyNodeList == NULL)
	{
		NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), pItemPropertyNodeList == NULL");
		return;
	}

	long itemPropertyCount = 0;
	pItemPropertyNodeList->get_length(&itemPropertyCount);
	for(long i = 0; i < itemPropertyCount; ++i)
	{
		IXMLDOMNode* pItemPropertyNode = NULL;
		pItemPropertyNodeList->get_item(i, &pItemPropertyNode);
		if(pItemPropertyNode)
		{
			IXMLDOMNodeList* pLMPNodeList = NULL;
			pItemPropertyNode->selectNodes(L"LINK_MESH_PREFIX", &pLMPNodeList);
			if(pLMPNodeList == NULL)
			{
				NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), pLMPNodeList == NULL");
				return;
			}

			if(pLMPNodeList)
			{
				long LMPTypeCount = 0;
				pLMPNodeList->get_length(&LMPTypeCount);
				for(long m = 0; m < LMPTypeCount; ++m)
				{
					IXMLDOMNode* pLMPTypeNode = NULL;
					pLMPNodeList->get_item(m, &pLMPTypeNode);
					if(pLMPTypeNode)
					{
						char sLMPName[64] = {0,};
						if(!XMLDoc.GetTextWithAttributeName(pLMPTypeNode,"TYPE", sLMPName, sizeof(sLMPName)))
						{
							NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pLMPTypeNode, ""TYPE"", sLMPName, sizeof(sLMPName))");
							return;
						}

						IXMLDOMNode*		pTypeNode		= NULL;
						IXMLDOMNodeList*	pTypeNodeList	= NULL;

						pLMPTypeNode->selectNodes(L"DATA", &pTypeNodeList);

						if(pTypeNodeList)
						{
							long dataCount = 0;
							pTypeNodeList->get_length(&dataCount);
							for(long t = 0; t < dataCount; t++)
							{
								IXMLDOMNode* pDataNode = NULL;
								pTypeNodeList->get_item(t, &pDataNode);
								char szType[64] = {0,};
								char szSCL[3][32] = {0,};
								char szROT[3][32] = {0,};
								char szPOS[3][32] = {0,};

								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""TYPE"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"CHAR_TYPE", szType, sizeof(szType)))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""CHAR_TYPE"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"SCL_X", szSCL[0], sizeof(szSCL[0])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""SCL_X"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"SCL_Y", szSCL[1], sizeof(szSCL[1])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""SCL_Y"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"SCL_Z", szSCL[2], sizeof(szSCL[2])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""SCL_Z"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"ROT_X", szROT[0], sizeof(szROT[0])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""ROT_X"", szType, sizeof(szType))");
									return;
								}

								strcpy_s(szROT[1], 32, "1.0");
								strcpy_s(szROT[2], 32, "1.0");

								/*
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"ROT_Y", szROT[1], sizeof(szROT[1])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""ROT_Y"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"ROT_Z", szROT[2], sizeof(szROT[2])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""ROT_Z"", szType, sizeof(szType))");
									return;
								}
								*/
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"POS_X", szPOS[0], sizeof(szPOS[0])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""POS_X"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"POS_Y", szPOS[1], sizeof(szPOS[1])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""POS_Y"", szType, sizeof(szType))");
									return;
								}
								if(!XMLDoc.GetTextWithAttributeName(pDataNode,"POS_Z", szPOS[2], sizeof(szPOS[2])))
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), !XMLDoc.GetTextWithAttributeName(pDataNode,""POS_Z"", szType, sizeof(szType))");
									return;
								}

								// get a char type id
								eRACE_GENDER_CLASS_TYPE eRGCT = RACE_GENDER_CLASS_TYPE_INVALID;

								if(!_stricmp(szType, "HUMAN_ADULT_MALE_FIGHTER"))			eRGCT = HUMAN_ADULT_MALE_FIGHTER;
								else if(!_stricmp(szType, "HUMAN_ADULT_MALE_MYSTIC"))		eRGCT = HUMAN_ADULT_MALE_MYSTIC;
								else if(!_stricmp(szType, "HUMAN_ADULT_MALE_ENGINEER"))		eRGCT = HUMAN_ADULT_MALE_ENGINEER;
								else if(!_stricmp(szType, "HUMAN_CHILD_MALE_FIGHTER"))		eRGCT = HUMAN_CHILD_MALE_FIGHTER;
								else if(!_stricmp(szType, "HUMAN_CHILD_MALE_MYSTIC"))		eRGCT = HUMAN_CHILD_MALE_MYSTIC;
								else if(!_stricmp(szType, "HUMAN_CHILD_MALE_ENGINEER"))		eRGCT = HUMAN_CHILD_MALE_ENGINEER;
								else if(!_stricmp(szType, "HUMAN_ADULT_FEMALE_FIGHTER"))	eRGCT = HUMAN_ADULT_FEMALE_FIGHTER;
								else if(!_stricmp(szType, "HUMAN_ADULT_FEMALE_MYSTIC"))		eRGCT = HUMAN_ADULT_FEMALE_MYSTIC;
								else if(!_stricmp(szType, "HUMAN_ADULT_FEMALE_ENGINEER"))	eRGCT = HUMAN_ADULT_FEMALE_ENGINEER;
								else if(!_stricmp(szType, "HUMAN_CHILD_FEMALE_FIGHTER"))	eRGCT = HUMAN_CHILD_FEMALE_FIGHTER;
								else if(!_stricmp(szType, "HUMAN_CHILD_FEMALE_MYSTIC"))		eRGCT = HUMAN_CHILD_FEMALE_MYSTIC;
								else if(!_stricmp(szType, "HUMAN_CHILD_FEMALE_ENGINEER"))	eRGCT = HUMAN_CHILD_FEMALE_ENGINEER;
								else if(!_stricmp(szType, "NAMEK_ADULT_FIGHTER"))			eRGCT = NAMEK_ADULT_FIGHTER;
								else if(!_stricmp(szType, "NAMEK_ADULT_MYSTIC"))			eRGCT = NAMEK_ADULT_MYSTIC;
								else if(!_stricmp(szType, "NAMEK_CHILD_FIGHTER"))			eRGCT = NAMEK_CHILD_FIGHTER;
								else if(!_stricmp(szType, "NAMEK_CHILD_MYSTIC"))			eRGCT = NAMEK_CHILD_MYSTIC;
								else if(!_stricmp(szType, "MAJIN_ADULT_MALE_MIGHTY"))		eRGCT = MAJIN_ADULT_MALE_MIGHTY;
								else if(!_stricmp(szType, "MAJIN_ADULT_MALE_WONDER"))		eRGCT = MAJIN_ADULT_MALE_WONDER;
								else if(!_stricmp(szType, "MAJIN_CHILD_MALE_MIGHTY"))		eRGCT = MAJIN_CHILD_MALE_MIGHTY;
								else if(!_stricmp(szType, "MAJIN_CHILD_MALE_WONDER"))		eRGCT = MAJIN_CHILD_MALE_WONDER;
								else if(!_stricmp(szType, "MAJIN_ADULT_FEMALE_MIGHTY"))		eRGCT = MAJIN_ADULT_FEMALE_MIGHTY;
								else if(!_stricmp(szType, "MAJIN_ADULT_FEMALE_WONDER"))		eRGCT = MAJIN_ADULT_FEMALE_WONDER;
								else if(!_stricmp(szType, "MAJIN_CHILD_FEMALE_MIGHTY"))		eRGCT = MAJIN_CHILD_FEMALE_MIGHTY;
								else if(!_stricmp(szType, "MAJIN_CHILD_FEMALE_WONDER"))		eRGCT = MAJIN_CHILD_FEMALE_WONDER;
								else
								{
									NTL_ASSERTFAIL("CLinkItemTrasformList::Create(), eRACE_GENDER_CLASS_TYPE eRGCT = RACE_GENDER_CLASS_TYPE_INVALID;");
									return;
								}

								sLINK_ITEM_TRASFORM	LinkItemTrasform;
								for(RwInt32 v = 0; v < 3; ++v)
								{
									LinkItemTrasform._SCL[v] = (RwReal)atof(szSCL[v]);
									LinkItemTrasform._ROT[v] = (RwReal)atof(szROT[v]);
									LinkItemTrasform._POS[v] = (RwReal)atof(szPOS[v]);
								}

								string strLMPName = sLMPName;
								Set(strLMPName, eRGCT, LinkItemTrasform);
							}
						}
					}
				}
			}
		}
	}

	XMLDoc.Destroy();
}

void CLinkItemTrasformList::Destroy()
{
	tLINK_ITEM_TRASFORM_MAP_ITER The = m_mapLinkItemTransformChunk.begin();
	
	while(The != m_mapLinkItemTransformChunk.end())
	{
		CLinkItemTrasformChunk* pDel = The->second;
		NTL_DELETE(pDel);

		The = m_mapLinkItemTransformChunk.erase(The);
	}

	m_mapLinkItemTransformChunk.clear();
}

VOID CLinkItemTrasformList::Set(string& _KeyStr, eRACE_GENDER_CLASS_TYPE& _EGCT, sLINK_ITEM_TRASFORM& _LIT)
{
	tLINK_ITEM_TRASFORM_MAP_ITER The = m_mapLinkItemTransformChunk.find(_KeyStr);

	if(The == m_mapLinkItemTransformChunk.end())
	{
		CLinkItemTrasformChunk* pAdd = NTL_NEW CLinkItemTrasformChunk(_EGCT, _LIT);

		m_mapLinkItemTransformChunk.insert(tLINK_ITEM_TRASFORM_MAP_VALUE(_KeyStr, pAdd));
	}
	else
	{
		return static_cast<CLinkItemTrasformChunk*>(The->second)->Set(_EGCT, _LIT);
	}
}

sLINK_ITEM_TRASFORM* CLinkItemTrasformList::Get(string& _KeyStr, eRACE_GENDER_CLASS_TYPE _EGCT)
{
    if(_EGCT == RACE_GENDER_CLASS_TYPE_INVALID)
        return NULL;

	tLINK_ITEM_TRASFORM_MAP_ITER The = m_mapLinkItemTransformChunk.find(_KeyStr);

	if(The != m_mapLinkItemTransformChunk.end())
	{
		return static_cast<CLinkItemTrasformChunk*>(The->second)->Get(_EGCT);
	}

	return NULL;
}