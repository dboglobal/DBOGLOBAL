#include "StdAfx.h"
#include "PropertyListXML.h"

CPropertyListXML::CPropertyListXML(void)
{
    m_pRootElem = NULL;
    m_pListElem = NULL;
}

CPropertyListXML::~CPropertyListXML(void)
{
}

RwBool CPropertyListXML::Save(const CHAR* szFileName, EProperyListMode eMode, CTreeCtrl* pTreeCtrl, TREEITEM_VECTOR vTreeItem)
{
    if(!szFileName || vTreeItem.size() == 0 || !pTreeCtrl)
        return FALSE;

    if(!Create())
        return FALSE;

    IXMLDOMProcessingInstruction* pPI    = NULL;        

    m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    m_pXMLDocument->appendChild(pPI, NULL);

    switch(eMode)
    {   
    case PROPERTY_CHARACTER:
        m_strPropetyName = "CHARACTER";
        m_strDataPath = PROPERTY_LIST_CHAR_PATH;
        break;
    case PROPERTY_OBJECT:
        m_strPropetyName = "OBJECT";
        m_strDataPath = PROPERTY_LIST_OBJECT_PATH;
        break;
    case PROPERTY_ITEM:
        m_strPropetyName = "ITEM";
        m_strDataPath = PROPERTY_LIST_ITEM;
        break;
    }

    if(!CreateHeader())
        return FALSE;

    if(!CreateElemList(pTreeCtrl, vTreeItem))
        return FALSE;

    SetIndent();

    if(FAILED(m_pXMLDocument->save((_variant_t)szFileName)))
        return FALSE;

    return TRUE;        
}

RwBool CPropertyListXML::CreateHeader()
{
    CString strRootElem;
    CString strListElem;
    CString strPathAttr;

    strRootElem = m_strPropetyName + "_PROPERTY";
    strListElem = m_strPropetyName + "_ELEMENT";
    strPathAttr = m_strPropetyName + "DATAPATH";

	WCHAR swRootElem[128];
	swprintf(swRootElem, 128, L"%S", strRootElem);

	WCHAR swListElem[128];
	swprintf(swListElem, 128, L"%S", strListElem);

    m_pXMLDocument->createElement(swRootElem, &m_pRootElem);
    m_pXMLDocument->createElement(swListElem, &m_pListElem);

    IXMLDOMElement*     pElemHeader = NULL;
    IXMLDOMAttribute*   pAttrVersion = NULL;
    IXMLDOMAttribute*   pAttrCharDataPath = NULL;

    m_pXMLDocument->createElement(L"HEADER", &pElemHeader);

    m_pXMLDocument->createAttribute(L"VER", &pAttrVersion);
    pAttrVersion->put_text(PROPERTY_LIST_VER);

	WCHAR swPathAttr[128];
	swprintf(swPathAttr, 128, L"%S", strPathAttr);

	WCHAR swDataPath[128];
	swprintf(swDataPath, 128, L"%S", m_strDataPath);

    m_pXMLDocument->createAttribute(swPathAttr, &pAttrCharDataPath);
    pAttrCharDataPath->put_text(swDataPath);

    pElemHeader->setAttributeNode(pAttrVersion, NULL);
    pElemHeader->setAttributeNode(pAttrCharDataPath, NULL);
    
    m_pXMLDocument->appendChild(m_pRootElem, NULL);
    m_pRootElem->appendChild(pElemHeader, NULL);
    m_pRootElem->appendChild(m_pListElem, NULL);

    return TRUE;
}

RwBool CPropertyListXML::CreateElemList(CTreeCtrl* pTreeCtrl, TREEITEM_VECTOR vTreeItem)
{
    if(!pTreeCtrl)
        return FALSE;

    for(UINT i = 0; i < vTreeItem.size(); ++i)
    {
        HTREEITEM hItem = vTreeItem[i];
        int nImage, nSelectImage;
        pTreeCtrl->GetItemImage(hItem, nImage, nSelectImage);
        if(nImage != 0)
        {
            CString strScriptName = pTreeCtrl->GetItemText(hItem);
            strScriptName.MakeUpper();
            if(strScriptName.Find(".XML") < 0)
            {
                strScriptName += ".XML";
            }

            IXMLDOMElement* pElem = NULL;
            IXMLDOMAttribute* pAttrName = NULL;

            m_pXMLDocument->createElement(L"ELEMENT", &pElem);
            m_pXMLDocument->createAttribute(L"NAME", &pAttrName);

			WCHAR swScriptName[128];
			swprintf(swScriptName, 128, L"%S", strScriptName);

            pAttrName->put_text(swScriptName);

            pElem->setAttributeNode(pAttrName, NULL);
            m_pListElem->appendChild(pElem, NULL);
        }
    }

    return TRUE;
}

RwBool CPropertyListXML::SetIndent()
{
    IXMLDOMDocument* pXSL = NULL;
    CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pXSL);
    if(!pXSL)
        return false;

    VARIANT_BOOL vBool;
    pXSL->put_async(VARIANT_FALSE);
    pXSL->load((_variant_t)"indent.xsl", &vBool);

    VARIANT vObject;
    VariantInit(&vObject);
    vObject.vt = VT_DISPATCH;
    vObject.pdispVal = m_pXMLDocument;

    m_pXMLDocument->transformNodeToObject(pXSL, vObject);

    if(pXSL)
    {
        pXSL->Release();
        pXSL = NULL;
    }

    return TRUE;
}