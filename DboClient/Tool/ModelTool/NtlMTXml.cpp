#include "StdAfx.h"
#include "NtlMTXml.h"
#include "comutil.h"

CNtlMTXml::CNtlMTXml(void)
{
}

CNtlMTXml::~CNtlMTXml(void)
{
    
}

BOOL CNtlMTXml::CreateXML(WCHAR* szRootNodeName)
{
    Create();

    IXMLDOMProcessingInstruction* pPI    = NULL;        
    

    m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    m_pXMLDocument->appendChild(pPI, NULL);

    m_pXMLDocument->createElement(L"FOLDER", &m_pRootElem);
    m_pXMLDocument->appendChild(m_pRootElem, NULL);
    
    IXMLDOMAttribute* pAttr;
    m_pXMLDocument->createAttribute(L"NAME", &pAttr);
    m_pRootElem->setAttributeNode(pAttr, NULL);
    m_pRootElem->setAttribute(L"NAME", (_variant_t)szRootNodeName);

    return TRUE;
}

bool CNtlMTXml::Save(WCHAR* szFileName)
{
    if(!m_pXMLDocument || !szFileName)
        return false;

    // 스타일 시트를 적용하여 세이브 한다.
    IXMLDOMDocument* pXSL = NULL;
    CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pXSL);
    if(!pXSL)
        return false;

    VARIANT_BOOL vBool;
    pXSL->put_async(VARIANT_FALSE);
    pXSL->load((_variant_t)L"indent.xsl", &vBool);

    VARIANT vObject;
    VariantInit(&vObject);
    vObject.vt = VT_DISPATCH;
    vObject.pdispVal = m_pXMLDocument;
    
    m_pXMLDocument->transformNodeToObject(pXSL, vObject);

    HRESULT hr = m_pXMLDocument->save((_variant_t)szFileName);
   
    if(pXSL)
    {
        pXSL->Release();
        pXSL = NULL;
    }
    if(hr == S_OK)
    {
        return true;
    }
    else
    {
        return false;
    }        
}

ITEM_VECTOR CNtlMTXml::LoadTreeXML(WCHAR* szFileName)
{
    ITEM_VECTOR vItem;

    if(!Create())
        return vItem;

    if(!Load(szFileName))
        return vItem;

    // 1. 우선 모든 폴더를 로드한다.
    IXMLDOMNodeList* pNodeList = NULL;
    m_pXMLDocument->selectNodes(L"//FOLDER", &pNodeList);
    if(pNodeList)
    {
        long listLen = 0;
        pNodeList->get_length(&listLen);
        for(long i = 0; i < listLen; ++i)
        {
            IXMLDOMNode* pNode = NULL;
            pNodeList->get_item(i, &pNode);
            if(pNode)
            {
                SItemNode itemNode;
                ZeroMemory(&itemNode, sizeof(itemNode));
                itemNode.nImageKind = 0;

                // 자기 이름                                
                IXMLDOMNamedNodeMap* pMap;
                IXMLDOMNode* pNameNode = NULL;
                VARIANT vt;
                pNode->get_attributes(&pMap);                
                pMap->getNamedItem(L"NAME", &pNameNode);                
                pNameNode->get_nodeValue(&vt);                
                
                swprintf(itemNode.szNodeName, L"%s", vt.bstrVal);                

                // 부모 노드 이름
                IXMLDOMNode* pParentNode = NULL;
                pNode->get_parentNode(&pParentNode);
                if(pParentNode)
                {
                    pParentNode->get_attributes(&pMap);
                    if(pMap)
                    {
                        pMap->getNamedItem(L"NAME", &pNameNode);                
                        pNameNode->get_nodeValue(&vt);                

                        swprintf(itemNode.szParenetName, L"%s", vt.bstrVal);
                    }                    
                }

                // 리스트에 추가
                vItem.push_back(itemNode);
            }
        }
    }

    // 2. 스크립트 파일 정보를 로드한다.
    m_pXMLDocument->selectNodes(L"//SCRIPT", &pNodeList);
    if(pNodeList)
    {
        long listLen = 0;
        pNodeList->get_length(&listLen);
        for(long i = 0; i < listLen; ++i)
        {
            IXMLDOMNode* pNode = NULL;
            pNodeList->get_item(i, &pNode);
            if(pNode)
            {
                SItemNode itemNode;
                ZeroMemory(&itemNode, sizeof(itemNode));
                itemNode.nImageKind = 3;

                // 자기이름
                BSTR strText;
                pNode->get_text(&strText);
                swprintf(itemNode.szNodeName, L"%s", strText);

                // 부모노드 이름
                IXMLDOMNamedNodeMap* pMap;
                IXMLDOMNode* pNameNode = NULL;
                VARIANT vt;
                IXMLDOMNode* pParentNode = NULL;
                pNode->get_parentNode(&pParentNode);
                if(pParentNode)
                {
                    pParentNode->get_attributes(&pMap);
                    if(pMap)
                    {
                        pMap->getNamedItem(L"NAME", &pNameNode);                
                        pNameNode->get_nodeValue(&vt);                

                        swprintf(itemNode.szParenetName, L"%s", vt.bstrVal);
                    }                    
                }

                vItem.push_back(itemNode);
            }
        }
    }

    return vItem;
}

/**
 * 새로운 폴더 노드를 추가한다
 * \param szParentFolder 붙일 부모 폴더의 이름, NULL인 경우에는 루트에 붙인다.
 * \param szFolderName 추가할 폴더노드의 이름 
 */
void CNtlMTXml::AddFolderNode( WCHAR* szParentFolder,  WCHAR* szFolderName)
{
    if(!szFolderName)
        return;

    IXMLDOMElement* pElem = NULL;
    m_pXMLDocument->createElement(L"FOLDER", &pElem);
    if(!pElem)
        return;


    IXMLDOMAttribute* pAttr;
    m_pXMLDocument->createAttribute(L"NAME", &pAttr);
    pElem->setAttributeNode(pAttr, NULL);
    pElem->setAttribute(L"NAME", (_variant_t)szFolderName);

    if(szParentFolder == NULL)
    {
        // 루트 노드에 붙인다.        
        m_pRootElem->appendChild(pElem, NULL);        
    }
    else
    {
        // 부모노드를 찾아서 그 밑에 붙인다.
        WCHAR szNodeName[128] = {0,};
        swprintf(szNodeName, L"//FOLDER[@NAME=\"%s\"]", szParentFolder);
        IXMLDOMNode* pNode = NULL;
        m_pXMLDocument->selectSingleNode(szNodeName, &pNode);
        if(pNode)
        {
            pNode->appendChild(pElem, NULL);
        }
    }
    if(pElem)
    {
        pElem->Release();
        pElem = NULL;
    }
}

void CNtlMTXml::AddScriptNode( WCHAR* szParentFolder,  WCHAR* szScriptName)
{
    if(!szParentFolder || !szScriptName)
        return;

    WCHAR szParentName[128] = {0,};
     swprintf(szParentName, L"//FOLDER[@NAME=\"%s\"]", szParentFolder);
    IXMLDOMNode* pNode = NULL;
    m_pXMLDocument->selectSingleNode(szParentName, &pNode);
    if(pNode)
    {
        IXMLDOMElement* pElem = NULL;
        m_pXMLDocument->createElement(L"SCRIPT", &pElem);        

        IXMLDOMText* pText = NULL;
        m_pXMLDocument->createTextNode(szScriptName, &pText);

        pElem->appendChild(pText, NULL);
        pNode->appendChild(pElem, NULL);

        if(pText)
        {
            pText->Release();
            pText = NULL;
        }

        if(pElem)
        {
            pElem->Release();
            pElem = NULL;
        }
    }
}


