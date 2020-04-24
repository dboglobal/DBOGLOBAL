#include "precomp_ntlpresentation.h"
#include "NtlPLCharacterParser.h"
#include <atlbase.h>
#include "comutil.h"
#include "NtlPLHelpers.h"
#include "NtlPLItemXMLScript.h"
#include "NtlPLItemParser.h"
#include "NtlPLXMLScriptHelper.h"

// Header
#define NODE_ITEM_HEADER             L"ITEM_SCRIPT/HEADER/"
#define NODE_ITEM_HEADER_VER         L"ITEM_SCRIPT/HEADER/VERSION"
#define NODE_ITEM_HEADER_NAME        L"ITEM_SCRIPT/HEADER/NAME"
#define NODE_ITEM_HEADER_MESH_PATH   L"ITEM_SCRIPT/HEADER/MESH_PATH"
#define NODE_ITEM_HEADER_MESH        L"ITEM_SCRIPT/HEADER/MESH_FILE_NAME"
#define NODE_ITEM_HEADER_FLAG        L"ITEM_SCRIPT/HEADER/FLAG"

CNtlPLItemXMLScript::CNtlPLItemXMLScript(void)
{
    m_pElemRoot = NULL;
    m_fScriptVer = 1.0f;
}

CNtlPLItemXMLScript::~CNtlPLItemXMLScript(void)
{    
}

RwBool CNtlPLItemXMLScript::SaveItemScript( const char* strFileName, CNtlPLItemProperty* pProperty ) 
{
    NTL_FUNCTION(__FUNCTION__);

    if(!strFileName || !pProperty)
        NTL_RETURN(FALSE);

    Create();

    IXMLDOMProcessingInstruction* pPI    = NULL;        

    m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    if(!pPI)
        NTL_RETURN(FALSE);
    m_pXMLDocument->appendChild(pPI, NULL);

    m_pXMLDocument->createElement(L"ITEM_SCRIPT", &m_pElemRoot);
    if(!m_pElemRoot)
        NTL_RETURN(FALSE);

    m_pXMLDocument->appendChild(m_pElemRoot, NULL);

    if(!SaveHeader(pProperty))
        NTL_RETURN(FALSE);

    if(!SaveBody(pProperty))
        NTL_RETURN(FALSE);

    if(!SaveLinkEffect(pProperty))
        NTL_RETURN(FALSE);

    if(!SaveAnimTable(pProperty))
        NTL_RETURN(FALSE);

    SaveTraceEvent(&pProperty->m_eventTrace);

    if(!SaveUpgradeEffect(pProperty))
        NTL_RETURN(FALSE);

    SetIndent(L"indent.xsl");

    HRESULT hr = m_pXMLDocument->save((_variant_t)strFileName);
    if(hr != S_OK)
        NTL_RETURN(FALSE);

    NTL_RETURN(TRUE);
}

RwBool CNtlPLItemXMLScript::LoadItemScript( const char* strFileName, CNtlPLItemProperty* pProperty ) 
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);

    Create();

    if(!Load((char*)strFileName))
    {
        Destroy();
        NTL_RETURN(FALSE);
    }

    if(!LoadHeader(pProperty))
    {
        Destroy();
        NTL_RETURN(FALSE);
    }

    if(!LoadBody(pProperty))
    {
        Destroy();
        NTL_RETURN(FALSE);
    }

    if(!LoadLinkEffect(pProperty))
    {
        Destroy();
        NTL_RETURN(FALSE);
    }

    if(!LoadAnimTable(pProperty))
    {
        Destroy();
        NTL_RETURN(FALSE);
    }

    LoadTraceEvent(&pProperty->m_eventTrace);

    if(!LoadUpgradeEffect(pProperty))
    {
        Destroy();
        NTL_RETURN(FALSE);
    }

    NTL_RETURN(TRUE);
}

RwBool CNtlPLItemXMLScript::SaveHeader( CNtlPLItemProperty* pProperty ) 
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    if(!m_pElemRoot || !pProperty)
        NTL_RETURN(FALSE);

    IXMLDOMElement* pElemHeader             = NULL;
    IXMLDOMElement* pElemVersion            = NULL;
    IXMLDOMElement* pElemName               = NULL;
    IXMLDOMElement* pElemMeshPath           = NULL;    
    IXMLDOMElement* pElemMeshFileName       = NULL;
    IXMLDOMElement* pElemFlag               = NULL;
    IXMLDOMText*    pTextVersion            = NULL;
    IXMLDOMText*    pTextName               = NULL;
    IXMLDOMText*    pTextMeshPath           = NULL;    
    IXMLDOMText*    pTextMeshFileName       = NULL;    
    WCHAR           wstrFlag[32]            = {0,};

    m_pXMLDocument->createElement(L"HEADER", &pElemHeader);
    m_pXMLDocument->createElement(L"VERSION", &pElemVersion);
    m_pXMLDocument->createElement(L"NAME",  &pElemName);
    m_pXMLDocument->createElement(L"MESH_PATH", &pElemMeshPath);    
    m_pXMLDocument->createElement(L"MESH_FILE_NAME", &pElemMeshFileName);
    m_pXMLDocument->createElement(L"FLAG", &pElemFlag);

    m_pXMLDocument->createTextNode(ITEM_XML_SCRIPT_VER, &pTextVersion);
    m_pXMLDocument->createTextNode(A2W(pProperty->GetName()), &pTextName);
    m_pXMLDocument->createTextNode(A2W(pProperty->GetMeshPath().c_str()), &pTextMeshPath);    
    m_pXMLDocument->createTextNode(A2W(pProperty->GetMeshFileName().c_str()), &pTextMeshFileName);
    
    swprintf_s(wstrFlag, L"%d", pProperty->GetFlag());
    pElemFlag->put_text(wstrFlag);


    pElemVersion->appendChild(pTextVersion, NULL);
    pElemName->appendChild(pTextName, NULL);
    pElemMeshPath->appendChild(pTextMeshPath, NULL);    
    pElemMeshFileName->appendChild(pTextMeshFileName, NULL);

    pElemHeader->appendChild(pElemVersion, NULL);
    pElemHeader->appendChild(pElemName, NULL);
    pElemHeader->appendChild(pElemMeshPath, NULL);    
    pElemHeader->appendChild(pElemMeshFileName, NULL);
    pElemHeader->appendChild(pElemFlag, NULL);

    m_pElemRoot->appendChild(pElemHeader, NULL);

    NTL_RETURN(TRUE);
}

RwBool CNtlPLItemXMLScript::LoadHeader( CNtlPLItemProperty* pProperty ) 
{
    USES_CONVERSION;    

    if(!pProperty)
        NTL_RETURN(FALSE);
    
    WCHAR szVersion[SIZE_PROP_BUF]      = {0,};
    WCHAR szName[SIZE_PROP_BUF]         = {0,};
    WCHAR szMeshPath[SIZE_PROP_BUF]     = {0,};    
    WCHAR szMeshFileName[SIZE_PROP_BUF] = {0,};
    WCHAR szFlag[SIZE_PROP_BUF]         = {0,};

    GetDataWithXPath(NODE_ITEM_HEADER_VER, szVersion, SIZE_PROP_BUF);
    GetDataWithXPath(NODE_ITEM_HEADER_NAME, szName, SIZE_PROP_BUF);
    GetDataWithXPath(NODE_ITEM_HEADER_MESH_PATH, szMeshPath, SIZE_PROP_BUF);    
    GetDataWithXPath(NODE_ITEM_HEADER_MESH, szMeshFileName, SIZE_PROP_BUF);
    GetDataWithXPath(NODE_ITEM_HEADER_FLAG, szFlag, SIZE_PROP_BUF);

    m_fScriptVer = (RwReal)_wtof(szVersion);
    pProperty->SetName(W2A(szName));        
    pProperty->SetMeshPath(W2A(szMeshPath));
    pProperty->SetMeshFileName(W2A(szMeshFileName));
    pProperty->SetFlag(_wtoi(szFlag));

    return TRUE;
}

RwBool CNtlPLItemXMLScript::SaveBody( CNtlPLItemProperty* pProperty ) 
{
	NTL_FUNCTION( "CNTlPLItemXMLScript::SaveBody" );

    USES_CONVERSION;

    IXMLDOMElement* pElemBody               = NULL;
    IXMLDOMElement* pElemAttachType         = NULL;
    IXMLDOMElement* pElemAttachSlotType     = NULL;
    IXMLDOMElement* pElemAnimPath           = NULL;
    IXMLDOMElement* pElemAnimFileName       = NULL;
    IXMLDOMElement* pElemAttachOffset       = NULL;
    IXMLDOMElement* pElemTraceEnableDefault = NULL;

    m_pXMLDocument->createElement(L"BODY", &pElemBody);
    m_pXMLDocument->createElement(L"ATTACH_TYPE", &pElemAttachType);
    m_pXMLDocument->createElement(L"SLOT_TYPE", &pElemAttachSlotType);
    m_pXMLDocument->createElement(L"ATTACH_OFFSET", &pElemAttachOffset);
    m_pXMLDocument->createElement(L"TRACE_ENABLE", &pElemTraceEnableDefault);

    WCHAR buf[SIZE_PROP_BUF] = {0,};

    swprintf_s(buf, SIZE_PROP_BUF, L"%d", pProperty->GetResType());    
    pElemAttachType->put_text(buf);

    swprintf_s(buf, SIZE_PROP_BUF, L"%d", pProperty->GetEquipSlotType());    
    pElemAttachSlotType->put_text(buf);

    Helper_V3D2StringU(buf, pProperty->GetAttachOffset());
    pElemAttachOffset->put_text(buf);

    swprintf_s(buf, SIZE_PROP_BUF, L"%d", pProperty->GetTraceEnableDefault());
    pElemTraceEnableDefault->put_text(buf);

    pElemBody->appendChild(pElemAttachType, NULL);
    pElemBody->appendChild(pElemAttachSlotType, NULL);
    pElemBody->appendChild(pElemAttachOffset, NULL);
    pElemBody->appendChild(pElemTraceEnableDefault, NULL);

    m_pElemRoot->appendChild(pElemBody, NULL);

    NTL_RETURN(TRUE);
}

RwBool CNtlPLItemXMLScript::LoadBody( CNtlPLItemProperty* pProperty ) 
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);

    std::string strBody         = "ITEM_SCRIPT/BODY/";
    std::string strAttachType   = strBody + "ATTACH_TYPE";
    std::string strSlotType     = strBody + "SLOT_TYPE";
    std::string strAttachOffset = strBody + "ATTACH_OFFSET";
    std::string strTraceEnableDefault = strBody + "TRACE_ENABLE";
    char szAttachType[4] = {0,};
    char szSlotType[4]   = {0,};
    char szAttachOffset[64] = {0,};
    char szTraceEnableDefault[SIZE_PROP_BUF] = {0,};

    GetDataWithXPath((char*)strAttachType.c_str(), szAttachType, sizeof(szAttachType));
    GetDataWithXPath((char*)strSlotType.c_str(), szSlotType, sizeof(szSlotType));
    GetDataWithXPath((char*)strAttachOffset.c_str(), szAttachOffset, sizeof(szAttachOffset));
    GetDataWithXPath((char*)strTraceEnableDefault.c_str(), szTraceEnableDefault, sizeof(szTraceEnableDefault));

    pProperty->SetResType((EItemResType)atoi(szAttachType));
    pProperty->SetEquipSlotType((EItemEquipSlotType)atoi(szSlotType));
    pProperty->SetAttachOffset(Helper_String2V3D(szAttachOffset));
    pProperty->SetTraceEnableDefault((RwBool)atoi(szTraceEnableDefault));

    NTL_RETURN(TRUE);
}

RwBool CNtlPLItemXMLScript::SaveLinkEffect( CNtlPLItemProperty* pProperty ) 
{
    USES_CONVERSION;

    IXMLDOMElement* pElemLinkEffect;

    m_pXMLDocument->createElement(L"LINK_EFFECT", &pElemLinkEffect);

    for(UINT i = 0; i < pProperty->m_vLinkEffect.size(); ++i)
    {
        SEventLinkEffect* pLinkEffect = pProperty->m_vLinkEffect[i];
        if(!pLinkEffect)
            continue;

        IXMLDOMElement* pElemEffect = NULL;        

        WCHAR buf[64] = {0,};

        m_pXMLDocument->createElement(L"EFFECT", &pElemEffect);

        pElemEffect->setAttribute(L"NAME", (_variant_t)A2W(pLinkEffect->chEffectName));
        pElemEffect->setAttribute(L"BONE", (_variant_t)A2W(pLinkEffect->chBoneName));

        swprintf_s(buf, L"%d", pLinkEffect->bAttachBone);
        pElemEffect->setAttribute(L"ATTACH_BONE", (_variant_t)buf);

        swprintf_s(buf, L"%.2f;%.2f;%.2f", pLinkEffect->vOffsetPos.x, pLinkEffect->vOffsetPos.y, pLinkEffect->vOffsetPos.z);
        pElemEffect->setAttribute(L"OFFSET_POS", (_variant_t)buf);

        pElemLinkEffect->appendChild(pElemEffect, NULL);
    }

    m_pElemRoot->appendChild(pElemLinkEffect, NULL);

    return TRUE;

}

RwBool CNtlPLItemXMLScript::LoadLinkEffect( CNtlPLItemProperty* pProperty ) 
{
    std::string strLinkEffect = "ITEM_SCRIPT/LINK_EFFECT/";
    std::string strEffect = strLinkEffect + "EFFECT";

    IXMLDOMNodeList* pEffectNodeList = SelectNodeList((char*)strEffect.c_str());
    if(pEffectNodeList)
    {
        long listLen = 0;
        pEffectNodeList->get_length(&listLen);
        for(long i = 0; i < listLen; ++i)
        {
            IXMLDOMNode* pNodeLinkEffect = NULL;
            pEffectNodeList->get_item(i, &pNodeLinkEffect);
            if(pNodeLinkEffect)
            {
                char szEffectName[MAX_DEFAULT_NAME] = {0,};
                char szBoneName[MAX_DEFAULT_NAME] = {0,};
                char szAttachBone[4] = {0,};
                char szOffsetPos[MAX_DEFAULT_NAME] = {0,};                                

                GetTextWithAttributeName(pNodeLinkEffect, "NAME", szEffectName, sizeof(szEffectName));
                GetTextWithAttributeName(pNodeLinkEffect, "BONE", szBoneName, sizeof(szBoneName));
                GetTextWithAttributeName(pNodeLinkEffect, "ATTACH_BONE", szAttachBone, sizeof(szAttachBone));
                GetTextWithAttributeName(pNodeLinkEffect, "OFFSET_POS", szOffsetPos, sizeof(szOffsetPos));                

                SEventLinkEffect* pLinkEffect = NTL_NEW SEventLinkEffect();
                sprintf_s(pLinkEffect->chEffectName, "%s", szEffectName);
                sprintf_s(pLinkEffect->chBoneName, "%s", szBoneName);
                pLinkEffect->bAttachBone = (RwBool)atoi(szAttachBone);
                pLinkEffect->vOffsetPos = Helper_String2V3D(szOffsetPos);

                pProperty->m_vLinkEffect.push_back(pLinkEffect);
            }
        }

        pEffectNodeList->Release();
    }

    return TRUE;

}
void CNtlPLItemXMLScript::SaveTraceEvent(const SEventTrace* pEventTrace)
{
    USES_CONVERSION;

    IXMLDOMElement* pElemTraceEvent;

    m_pXMLDocument->createElement(L"TRACE_EVENT", &pElemTraceEvent);

    WCHAR buf[256] = {0,};

    pElemTraceEvent->setAttribute(L"START_BONE", (_variant_t)A2W(pEventTrace->strStartBoneName));
    pElemTraceEvent->setAttribute(L"END_BONE", (_variant_t)A2W(pEventTrace->strEndBoneName));
    pElemTraceEvent->setAttribute(L"TEXTURE", (_variant_t)A2W(pEventTrace->strTexture));

    Helper_V3D2StringU(buf, pEventTrace->v3dStartBoneOffset);
    pElemTraceEvent->setAttribute(L"START_OFFSET", (_variant_t)buf);

    Helper_V3D2StringU(buf, pEventTrace->v3dEndBoneOffset);
    pElemTraceEvent->setAttribute(L"END_OFFSET", (_variant_t)buf);

    swprintf(buf, L"%.2f", pEventTrace->fEdgeGap);
    pElemTraceEvent->setAttribute(L"EDGE_GAP", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->nSplinePointCount);
    pElemTraceEvent->setAttribute(L"SPLINE_COUNT", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->nMaxEdgeCount);
    pElemTraceEvent->setAttribute(L"MAX_EDGE", (_variant_t)buf);

    swprintf(buf, L"%.2f", pEventTrace->fMaxLength);
    pElemTraceEvent->setAttribute(L"MAX_LENGTH", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->eSrcBlend);
    pElemTraceEvent->setAttribute(L"SRC_BLEND", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->eDestBlend);
    pElemTraceEvent->setAttribute(L"DEST_BLEND", (_variant_t)buf);

    swprintf(buf, L"%d;%d;%d;%d", pEventTrace->colStartColor.red,
        pEventTrace->colStartColor.green,
        pEventTrace->colStartColor.blue,
        pEventTrace->colStartColor.alpha);
    pElemTraceEvent->setAttribute(L"START_COLOR", (_variant_t)buf);

    swprintf(buf, L"%d;%d;%d;%d", pEventTrace->colEndColor.red,
        pEventTrace->colEndColor.green,
        pEventTrace->colEndColor.blue,
        pEventTrace->colEndColor.alpha);
    pElemTraceEvent->setAttribute(L"END_COLOR", (_variant_t)buf);   

    m_pElemRoot->appendChild(pElemTraceEvent, NULL);
}

void CNtlPLItemXMLScript::LoadTraceEvent(SEventTrace* pEventTrace)
{
    std::string strLinkEffect = "ITEM_SCRIPT/TRACE_EVENT";    

    IXMLDOMNode* pNodeEvent = SelectSingleNode((char*)strLinkEffect.c_str());
    if(!pNodeEvent)
        return;

    char buf[256] = {0,};

    if(GetTextWithAttributeName(pNodeEvent, "START_BONE", buf, sizeof(buf)))
    {
        strncpy_s(pEventTrace->strStartBoneName, buf, MAX_DEFAULT_NAME);
    }

    if(GetTextWithAttributeName(pNodeEvent, "END_BONE", buf, sizeof(buf)))
    {
        strncpy_s(pEventTrace->strEndBoneName, buf, MAX_DEFAULT_NAME);
    }

    if(GetTextWithAttributeName(pNodeEvent, "TEXTURE", buf, sizeof(buf)))
    {
        strncpy_s(pEventTrace->strTexture, buf, MAX_DEFAULT_NAME);
    }

    if(GetTextWithAttributeName(pNodeEvent, "START_OFFSET", buf, sizeof(buf)))
    {
        pEventTrace->v3dStartBoneOffset = Helper_String2V3D(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "END_OFFSET", buf, sizeof(buf)))
    {
        pEventTrace->v3dEndBoneOffset = Helper_String2V3D(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "EDGE_GAP", buf, sizeof(buf)))
    {
        pEventTrace->fEdgeGap = (RwReal)atof(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "SPLINE_COUNT", buf, sizeof(buf)))
    {
        pEventTrace->nSplinePointCount = atoi(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "MAX_EDGE", buf, sizeof(buf)))
    {
        pEventTrace->nMaxEdgeCount = atoi(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "MAX_LENGTH", buf, sizeof(buf)))
    {
        pEventTrace->fMaxLength = (RwReal)atof(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "SRC_BLEND", buf, sizeof(buf)))
    {
        pEventTrace->eSrcBlend = (RwBlendFunction)atoi(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "DEST_BLEND", buf, sizeof(buf)))
    {
        pEventTrace->eDestBlend = (RwBlendFunction)atoi(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "START_COLOR", buf, sizeof(buf)))
    {
        pEventTrace->colStartColor = Helper_String2RGBA(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "END_COLOR", buf, sizeof(buf)))
    {
        pEventTrace->colEndColor = Helper_String2RGBA(buf);
    }

    pNodeEvent->Release();
}

RwBool CNtlPLItemXMLScript::SaveAnimTable( CNtlPLItemProperty* pProperty ) 
{
    USES_CONVERSION;

    if(!m_pElemRoot || !pProperty)
        return FALSE;

    IXMLDOMElement* pElemAnimTable;
    m_pXMLDocument->createElement(L"ANIM_TABLE", &pElemAnimTable);

    pElemAnimTable->setAttribute(L"ANIM_PATH", (_variant_t)A2W(pProperty->GetAnimTable()->GetAnimPath().c_str()));

    TYPE_ANIM_MAP* pMapAnim = pProperty->GetAnimTable()->GetTypeAnimMap();
    TYPE_ANIM_MAP_ITER it;
    for(it = pMapAnim->begin(); it != pMapAnim->end(); ++it)
    {
        STypeAnimData* pAnimData = it->second;
        if(!pAnimData)
            continue;

        IXMLDOMElement* pElemAnimData = NULL;
        m_pXMLDocument->createElement(L"ANIM_DATA", &pElemAnimData);

        WCHAR buf[256] = {0,};

        std::string* pStrAnimKey = CNtlPLItemParser::GetInstance().GetItemMatchTable()->GetString(pAnimData->uiAnimKey);
        if(pStrAnimKey)
        {
            pElemAnimData->setAttribute(L"KEY", (_variant_t)A2W(pStrAnimKey->c_str()));
        }

        m_XMLScriptHelper.SaveAnimData(pElemAnimData, pAnimData);        

        pElemAnimTable->appendChild(pElemAnimData, NULL);
    }

    m_pElemRoot->appendChild(pElemAnimTable, NULL);

    return TRUE;
}

RwBool CNtlPLItemXMLScript::LoadAnimTable( CNtlPLItemProperty* pProperty ) 
{
    USES_CONVERSION;

    if(!pProperty)
        return FALSE;

    
    std::string strAnimTable = "ITEM_SCRIPT/ANIM_TABLE";
    std::string strAnimData = strAnimTable + "/ANIM_DATA";
    IXMLDOMNode* pAnimTable = SelectSingleNode((char*)strAnimTable.c_str());
    if(pAnimTable)
    {
        char szAnimPath[256] = {0,};

        GetTextWithAttributeName(pAnimTable, "ANIM_PATH", szAnimPath, sizeof(szAnimPath));

        std::string strAnimPath = szAnimPath;
        pProperty->GetAnimTable()->SetAnimPath(strAnimPath);

        pAnimTable->Release();
    }

    IXMLDOMNodeList* pAnimDataList = SelectNodeList((char*)strAnimData.c_str());
    if(pAnimDataList)
    {
        long listLen = 0;
        pAnimDataList->get_length(&listLen);
        for(long i = 0; i < listLen; ++i)
        {
            IXMLDOMNode* pAnimData = NULL;
            pAnimDataList->get_item(i, &pAnimData);
            if(!pAnimData)
                continue;

            STypeAnimData* pTypeAnimData = NTL_NEW STypeAnimData();

            char szKey[128] = {0,};
            char szAnimFile[256] = {0,};
            char szPlayTime[8] = {0,};

            GetTextWithAttributeName(pAnimData, "KEY", szKey, sizeof(szKey));
            GetTextWithAttributeName(pAnimData, "FILE", szAnimFile, sizeof(szAnimFile));
            GetTextWithAttributeName(pAnimData, "PLAYTIME", szPlayTime, sizeof(szPlayTime));

            pTypeAnimData->uiAnimKey = CNtlPLItemParser::GetInstance().GetItemMatchTable()->GetKey(szKey);
            pTypeAnimData->strAnimName = szAnimFile;
            pTypeAnimData->fPlayTime = (RwReal)atof(szPlayTime);
            
            pProperty->GetAnimTable()->Add(pTypeAnimData->uiAnimKey, pTypeAnimData);
        }

        pAnimDataList->Release();
    }

    return TRUE;
}

RwBool CNtlPLItemXMLScript::SaveUpgradeEffect( CNtlPLItemProperty* pProperty ) 
{
    if(!pProperty)
        return FALSE;

    SUpgradeEffectProperty* pUpgradeProp = pProperty->GetUpgradeEffectProperty();
    if(!pUpgradeProp)
        return FALSE;

    USES_CONVERSION;

    IXMLDOMElement* pElemUpgradeEffect;
    m_pXMLDocument->createElement(L"UPGRADE_EFFECT", &pElemUpgradeEffect);

	CNtlPLXMLScriptHelper::SaveAttribute(pElemUpgradeEffect, L"BONE_1", pUpgradeProp->szBone1);
	CNtlPLXMLScriptHelper::SaveAttribute(pElemUpgradeEffect, L"BONE_2", pUpgradeProp->szBone2);
	CNtlPLXMLScriptHelper::SaveAttribute(pElemUpgradeEffect, L"OFFSET_1", pUpgradeProp->vOffset1);
	CNtlPLXMLScriptHelper::SaveAttribute(pElemUpgradeEffect, L"OFFSET_2", pUpgradeProp->vOffset2);

	for (int i = 0; i < MAX_UPGRADE_EFFECT_COUNT; i++)
	{
		WCHAR buf[64] = { 0, };
		swprintf_s(buf, 64, L"EFFECT_NAME_%u", i);
		CNtlPLXMLScriptHelper::SaveAttribute(pElemUpgradeEffect, buf, pUpgradeProp->szEffect[i]);
	}
    
    m_pElemRoot->appendChild(pElemUpgradeEffect, NULL);

    return TRUE;
}

RwBool CNtlPLItemXMLScript::LoadUpgradeEffect( CNtlPLItemProperty* pProperty ) 
{
    if(!pProperty || !pProperty->GetUpgradeEffectProperty())
        return FALSE;

    IXMLDOMNode* pNode = SelectSingleNode("ITEM_SCRIPT/UPGRADE_EFFECT");
    if(!pNode)
        return TRUE;

    char szBone1[64] = {0,};
    char szBone2[64] = {0,};
    char szOffset1[16] = {0,};
    char szOffset2[16] = {0,};

	char szEffect[MAX_UPGRADE_EFFECT_COUNT][MAX_DEFAULT_NAME*2];

    GetTextWithAttributeName(pNode, "BONE_1", szBone1, sizeof(szBone1));
    GetTextWithAttributeName(pNode, "BONE_2", szBone2, sizeof(szBone2));
	GetTextWithAttributeName(pNode, "OFFSET_1", szOffset1, sizeof(szOffset1));
	GetTextWithAttributeName(pNode, "OFFSET_2", szOffset2, sizeof(szOffset2));

	char szEffectTitle[MAX_DEFAULT_NAME] = { 0, };

	for (int i = 0; i < MAX_UPGRADE_EFFECT_COUNT; i++)
	{
		sprintf_s(szEffectTitle, "EFFECT_NAME_%u", i);
		GetTextWithAttributeName(pNode, szEffectTitle, szEffect[i], sizeof(szEffect[i]));
	}
	

    SUpgradeEffectProperty* pUpgradeProp = pProperty->GetUpgradeEffectProperty();

	for (int i = 0; i < MAX_UPGRADE_EFFECT_COUNT; i++)
	{
		sprintf_s(pUpgradeProp->szEffect[i], "%s", szEffect[i]);
	}

	sprintf_s(pUpgradeProp->szBone1, "%s", szBone1);
	sprintf_s(pUpgradeProp->szBone2, "%s", szBone2);
	pUpgradeProp->vOffset1 = Helper_String2V3D(szOffset1);
	pUpgradeProp->vOffset2 = Helper_String2V3D(szOffset2);

    pNode->Release();

    return TRUE;
}