#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include <atlbase.h>
#include "comutil.h"
#include "NtlPLHelpers.h"
#include "NtlPLObjectXMLScript.h"
#include "NtlPLItemParser.h"

CNtlPLObjectXMLScript::CNtlPLObjectXMLScript(void)
{
    m_pElemRoot = NULL;
}

CNtlPLObjectXMLScript::~CNtlPLObjectXMLScript(void)
{

}

RwBool CNtlPLObjectXMLScript::SaveObjectScript(const char* strFileName, CNtlPLObjectProperty* pProperty)
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

    m_pXMLDocument->createElement(L"OBJECT_SCRIPT", &m_pElemRoot);
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
    
    SetIndent(L"indent.xsl");

    HRESULT hr = m_pXMLDocument->save((_variant_t)strFileName);
    if(hr != S_OK)
        NTL_RETURN(FALSE);

    NTL_RETURN(TRUE);
}

RwBool CNtlPLObjectXMLScript::LoadObjectScript(const char* strFileName, CNtlPLObjectProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);

    Create();

    if(!Load((char*)strFileName))
        NTL_RETURN(FALSE);

    if(!LoadHeader(pProperty))
        NTL_RETURN(FALSE);

    if(!LoadBody(pProperty))
        NTL_RETURN(FALSE);

    if(!LoadLinkEffect(pProperty))
        NTL_RETURN(FALSE);

	if(!LoadAnimTable(pProperty))
		NTL_RETURN(FALSE);

    NTL_RETURN(TRUE);
}

RwBool CNtlPLObjectXMLScript::SaveHeader(CNtlPLObjectProperty* pProperty)
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    if(!m_pElemRoot || !pProperty)
        NTL_RETURN(FALSE);

    IXMLDOMElement* pElemHeader             = NULL;
    IXMLDOMElement* pElemVersion            = NULL;
    IXMLDOMElement* pElemName               = NULL;
    IXMLDOMElement* pElemMeshPath           = NULL;
    IXMLDOMElement* pElemTexturePath        = NULL;
    IXMLDOMElement* pElemAnimPath           = NULL;
    IXMLDOMElement* pElemMeshFileName       = NULL;

    IXMLDOMText*    pTextVersion            = NULL;
    IXMLDOMText*    pTextName               = NULL;
    IXMLDOMText*    pTextMeshPath           = NULL;
    IXMLDOMText*    pTextTexturePath        = NULL;
    IXMLDOMText*    pTextAnimPath           = NULL;
    IXMLDOMText*    pTextMeshFileName       = NULL;


    m_pXMLDocument->createElement(L"HEADER", &pElemHeader);
    m_pXMLDocument->createElement(L"VERSION", &pElemVersion);
    m_pXMLDocument->createElement(L"NAME",  &pElemName);
    m_pXMLDocument->createElement(L"MESH_PATH", &pElemMeshPath);
    m_pXMLDocument->createElement(L"TEXTURE_PATH", &pElemTexturePath);
    m_pXMLDocument->createElement(L"ANIM_PATH", &pElemAnimPath);
    m_pXMLDocument->createElement(L"MESH_FILE_NAME", &pElemMeshFileName);


    m_pXMLDocument->createTextNode(OBJECT_XML_SCRIPT_VER, &pTextVersion);
    m_pXMLDocument->createTextNode(A2W(pProperty->GetName()), &pTextName);
    m_pXMLDocument->createTextNode(A2W(pProperty->m_strMeshPath.c_str()), &pTextMeshPath);
    m_pXMLDocument->createTextNode(A2W(pProperty->m_strTexPath.c_str()), &pTextTexturePath);
    m_pXMLDocument->createTextNode(A2W(pProperty->GetAnimTable()->GetAnimPath().c_str()), &pTextAnimPath);        
    m_pXMLDocument->createTextNode(A2W(pProperty->GetDffName()), &pTextMeshFileName);    

    
    pElemVersion->appendChild(pTextVersion, NULL);
    pElemName->appendChild(pTextName, NULL);
    pElemMeshPath->appendChild(pTextMeshPath, NULL);
    pElemTexturePath->appendChild(pTextTexturePath, NULL);
    pElemAnimPath->appendChild(pTextAnimPath, NULL);
    pElemMeshFileName->appendChild(pTextMeshFileName, NULL);


    pElemHeader->appendChild(pElemVersion, NULL);
    pElemHeader->appendChild(pElemName, NULL);
    pElemHeader->appendChild(pElemMeshPath, NULL);
    pElemHeader->appendChild(pElemTexturePath, NULL);
    pElemHeader->appendChild(pElemAnimPath, NULL);
    pElemHeader->appendChild(pElemMeshFileName, NULL);


    m_pElemRoot->appendChild(pElemHeader, NULL);

    NTL_RETURN(TRUE);
}

RwBool CNtlPLObjectXMLScript::SaveBody( CNtlPLObjectProperty* pProperty ) 
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    IXMLDOMElement* pElemBody               = NULL;
    IXMLDOMElement* pElemShadow             = NULL;    
    IXMLDOMElement* pElemBBox               = NULL;
    IXMLDOMElement* pElemUVAnim             = NULL;
    IXMLDOMElement* pElemUVAnimSpeed        = NULL;
    IXMLDOMElement* pElemCullingDistance    = NULL;
    IXMLDOMElement* pElemAnimFile           = NULL;
    IXMLDOMElement* pElemCollision          = NULL;    
    IXMLDOMElement* pElemCollisonMeshName   = NULL;
    IXMLDOMElement* pElemObjectType         = NULL;
    IXMLDOMElement* pElemApplyPVS           = NULL;
    IXMLDOMElement* pElemObjSound           = NULL;
    IXMLDOMElement* pElemFlag               = NULL;

    m_pXMLDocument->createElement(L"BODY", &pElemBody);
    m_pXMLDocument->createElement(L"SHADOW", &pElemShadow);    
    m_pXMLDocument->createElement(L"BBOX", &pElemBBox);
    m_pXMLDocument->createElement(L"UVANIM", &pElemUVAnim);
    m_pXMLDocument->createElement(L"UVANIMSPEED", &pElemUVAnimSpeed);    
    m_pXMLDocument->createElement(L"CULLDIST", &pElemCullingDistance);
    m_pXMLDocument->createElement(L"ANIM", &pElemAnimFile);
    m_pXMLDocument->createElement(L"PATH_COLLISION", &pElemCollision);    
    m_pXMLDocument->createElement(L"COLLISON_MESH", &pElemCollisonMeshName);
    m_pXMLDocument->createElement(L"OBJECT_TYPE", &pElemObjectType);   
    m_pXMLDocument->createElement(L"APPLY_PVS", &pElemApplyPVS);
    m_pXMLDocument->createElement(L"SOUND", &pElemObjSound);
    m_pXMLDocument->createElement(L"FLAG", &pElemFlag);

    WCHAR buf[64] = {0,};

    swprintf_s(buf, L"%d", pProperty->GetEnableShadow());
    pElemShadow->put_text(buf);

    Helper_V3D2StringU(buf, pProperty->GetBBox()->inf);    
    pElemBBox->setAttribute(L"MIN", (_variant_t)buf);
    Helper_V3D2StringU(buf, pProperty->GetBBox()->sup);
    pElemBBox->setAttribute(L"MAX", (_variant_t)buf);

    pElemUVAnim->put_text(A2W(pProperty->m_strUVAnimFileName.c_str()));

    swprintf_s(buf, L"%.2f", pProperty->GetUVAnimSpeed());
    pElemUVAnimSpeed->put_text(buf);    

    swprintf_s(buf, L"%.2f", pProperty->GetCullingDistance());    
    pElemCullingDistance->put_text(buf);

    pElemAnimFile->put_text(A2W(pProperty->m_strAnimFileName.c_str()));

    swprintf_s(buf, L"%d", pProperty->GetCollision());
    pElemCollision->put_text(buf);    

    pElemCollisonMeshName->put_text(A2W(pProperty->m_strCollisionMeshName.c_str()));

    swprintf_s(buf, L"%d", pProperty->GetObjectType());
    pElemObjectType->put_text(buf);

    swprintf_s(buf, L"%d", pProperty->GetApplyPVS());
    pElemApplyPVS->put_text(buf);

    swprintf_s(buf, L"%d", pProperty->GetFlag());
    pElemFlag->put_text(buf);

    // 사운드 설정
    m_XMLScriptHelper.SaveSoundEvent(pElemObjSound, pProperty->GetSoundProp());
    
    pElemBody->appendChild(pElemShadow, NULL);    
    pElemBody->appendChild(pElemBBox, NULL);
    pElemBody->appendChild(pElemUVAnim, NULL);
    pElemBody->appendChild(pElemUVAnimSpeed, NULL);    
    pElemBody->appendChild(pElemCullingDistance, NULL);
    pElemBody->appendChild(pElemAnimFile, NULL);
    pElemBody->appendChild(pElemCollision, NULL);    
    pElemBody->appendChild(pElemCollisonMeshName, NULL);
    pElemBody->appendChild(pElemObjectType, NULL);
    pElemBody->appendChild(pElemApplyPVS, NULL);
    pElemBody->appendChild(pElemFlag, NULL);
    pElemBody->appendChild(pElemObjSound, NULL);

    m_pElemRoot->appendChild(pElemBody, NULL);

    NTL_RETURN(TRUE);    
}

RwBool CNtlPLObjectXMLScript::LoadHeader(CNtlPLObjectProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);

    std::string strHeader        = "OBJECT_SCRIPT/HEADER/";
    std::string strVersion       = strHeader + "VERSION";
    std::string strName          = strHeader + "NAME";
    std::string strMeshPath      = strHeader + "MESH_PATH";
    std::string strTexturePath   = strHeader + "TEXTURE_PATH";
    std::string strAnimPath      = strHeader + "ANIM_PATH";
    std::string strMeshFileName  = strHeader + "MESH_FILE_NAME";
    

    char szVersion[256]     = {0,};
    char szName[256]        = {0,};
    char szMeshPath[256]    = {0,};
    char szTexturePath[256] = {0,};
    char szAnimPath[256]    = {0,};
    char szMeshFileName[256] = {0,};



    GetDataWithXPath((char*)strVersion.c_str(), szVersion, sizeof(szVersion));
    GetDataWithXPath((char*)strName.c_str(), szName, sizeof(szName));
    GetDataWithXPath((char*)strMeshPath.c_str(), szMeshPath, sizeof(szMeshPath));
    GetDataWithXPath((char*)strTexturePath.c_str(), szTexturePath, sizeof(szTexturePath));
    GetDataWithXPath((char*)strAnimPath.c_str(), szAnimPath, sizeof(szAnimPath));
    GetDataWithXPath((char*)strMeshFileName.c_str(), szMeshFileName, sizeof(szMeshFileName));
    


    pProperty->SetName(szName);
    pProperty->m_strMeshPath = szMeshPath;
    pProperty->m_strTexPath = szTexturePath;
    pProperty->m_strAniPath = szAnimPath;
	pProperty->GetAnimTable()->SetAnimPath(pProperty->m_strAniPath);
    pProperty->SetDffName(szMeshFileName);    
    
    NTL_RETURN(TRUE);
}

RwBool CNtlPLObjectXMLScript::LoadBody( CNtlPLObjectProperty* pProperty ) 
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    std::string strBody          = "OBJECT_SCRIPT/BODY/";
    std::string strShadow        = strBody + "SHADOW";    
    std::string strBBox          = strBody + "BBOX";
    std::string strUVAnim        = strBody + "UVANIM";
    std::string strUVAnimSpeed   = strBody + "UVANIMSPEED";    
    std::string strCullDist      = strBody + "CULLDIST";
    std::string strAnimFileName  = strBody + "ANIM";
    std::string strCollision     = strBody + "PATH_COLLISION";    
    std::string strColiisonMeshName = strBody + "COLLISON_MESH";
    std::string strObjectType    = strBody + "OBJECT_TYPE";
    std::string strApplyPVS      = strBody + "APPLY_PVS";
    std::string strFlag          = strBody + "FLAG";
    std::string strSound         = strBody + "SOUND";

    char szShadow[4]        = "1";    
    char szBBoxMin[64]         = {0,};
    char szBBoxMax[64]         = {0,};
    char szBBoxSize[3][8]   = {0,};
    char szUVAnim[64]       = {0,};
    char szUVAnimSpeed[16]  = "1";    
    char szCullDist[16]     = "300";
    char szAnimFileName[64] = {0,};
    char szCollision[4]     = {0,};    
    char szCollisonMeshName[256] = {0,};
    char szObjectType[4]    = {0,};
    char szApplyPVS[4]      = "1";
    char szFlag[64]         = {0,};
           
    GetDataWithXPath((char*)strShadow.c_str(), szShadow, sizeof(szShadow));        
    GetDataWithXPath((char*)strUVAnim.c_str(), szUVAnim, sizeof(szUVAnim));
    GetDataWithXPath((char*)strUVAnimSpeed.c_str(), szUVAnimSpeed, sizeof(szUVAnimSpeed));    
    GetDataWithXPath((char*)strCullDist.c_str(), szCullDist, sizeof(szCullDist));
    GetDataWithXPath((char*)strAnimFileName.c_str(), szAnimFileName, sizeof(szAnimFileName));
    GetDataWithXPath((char*)strCollision.c_str(), szCollision, sizeof(szCollision));    
    GetDataWithXPath((char*)strColiisonMeshName.c_str(), szCollisonMeshName, sizeof(szCollisonMeshName));
    GetDataWithXPath((char*)strObjectType.c_str(), szObjectType, sizeof(szObjectType));
    GetDataWithXPath((char*)strApplyPVS.c_str(), szApplyPVS, sizeof(szApplyPVS));
    GetDataWithXPath((char*)strFlag.c_str(), szFlag, sizeof(szFlag));

    GetTextWithAttributeName(SelectSingleNode((char*)strBBox.c_str()), "MIN", szBBoxMin, sizeof(szBBoxMin));
    GetTextWithAttributeName(SelectSingleNode((char*)strBBox.c_str()), "MAX", szBBoxMax, sizeof(szBBoxMax));

    pProperty->SetEnableShadow((RwBool)atoi(szShadow));    
    pProperty->m_strUVAnimFileName = szUVAnim;
    pProperty->SetUVAnimSpeed((RwReal)atof(szUVAnimSpeed));    
    pProperty->SetCullingDistance((RwReal)atof(szCullDist));
    pProperty->m_strAnimFileName = szAnimFileName;
    pProperty->SetCollision((RwBool)atoi(szCollision));    
    pProperty->m_strCollisionMeshName = szCollisonMeshName;
    pProperty->SetObjectType((EPLObjectType)atoi(szObjectType));
    pProperty->SetApplyPVS((RwBool)atoi(szApplyPVS));
    pProperty->SetFlag(atoi(szFlag));

    RwBBox bbox;
    bbox.inf = Helper_String2V3D(szBBoxMin);
    bbox.sup = Helper_String2V3D(szBBoxMax);
    pProperty->SetBBox(bbox);

    // 사운드
    m_XMLScriptHelper.LoadSoundEvent(SelectSingleNode((char*)strSound.c_str()), pProperty->GetSoundProp());

    NTL_RETURN(TRUE);
}

RwBool CNtlPLObjectXMLScript::SaveLinkEffect( CNtlPLObjectProperty* pProperty ) 
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

RwBool CNtlPLObjectXMLScript::LoadLinkEffect( CNtlPLObjectProperty* pProperty ) 
{
    std::string strLinkEffect = "OBJECT_SCRIPT/LINK_EFFECT/";
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
                char szOffsetPosValue[3][MAX_DEFAULT_NAME / 3] = {0,};                

                GetTextWithAttributeName(pNodeLinkEffect, "NAME", szEffectName, sizeof(szEffectName));
                GetTextWithAttributeName(pNodeLinkEffect, "BONE", szBoneName, sizeof(szBoneName));
                GetTextWithAttributeName(pNodeLinkEffect, "ATTACH_BONE", szAttachBone, sizeof(szAttachBone));
                GetTextWithAttributeName(pNodeLinkEffect, "OFFSET_POS", szOffsetPos, sizeof(szOffsetPos));                

                SEventLinkEffect* pLinkEffect = NTL_NEW SEventLinkEffect();
                sprintf_s(pLinkEffect->chEffectName, "%s", szEffectName);
                sprintf_s(pLinkEffect->chBoneName, "%s", szBoneName);
                pLinkEffect->bAttachBone = (RwBool)atoi(szAttachBone);

                for(int i = 0; i < 3; ++i)
                {
                    Helper_StringSplit(szOffsetPos, szOffsetPosValue[i], sizeof(szOffsetPosValue[i]), ";", i);                    
                }

                pLinkEffect->vOffsetPos.x = (RwReal)atof(szOffsetPosValue[0]);
                pLinkEffect->vOffsetPos.y = (RwReal)atof(szOffsetPosValue[1]);
                pLinkEffect->vOffsetPos.z = (RwReal)atof(szOffsetPosValue[2]);

                pProperty->m_vLinkEffect.push_back(pLinkEffect);
            }
        }
    }
    return TRUE;
}

RwBool CNtlPLObjectXMLScript::SaveAnimTable( CNtlPLObjectProperty* pProperty ) 
{
	USES_CONVERSION;

	NTL_FUNCTION(__FUNCTION__);

	if(!m_pElemRoot || !pProperty)
		NTL_RETURN(FALSE);

	IXMLDOMElement* pElemAnimTable;

	m_pXMLDocument->createElement(L"ANIM_TABLE", &pElemAnimTable);

	TYPE_ANIM_MAP* pMapAnim = pProperty->GetAnimTable()->GetTypeAnimMap();
	TYPE_ANIM_MAP_ITER it;
	for(it = pMapAnim->begin(); it != pMapAnim->end(); ++it)
	{
		STypeAnimData* pAnimData = it->second;
		if(pAnimData)
		{
			pAnimData->AnimEventSort();

			IXMLDOMElement* pElemAnimData = NULL;

			m_pXMLDocument->createElement(L"ANIM_DATA", &pElemAnimData);

			WCHAR buf[256] = {0,};             

			pElemAnimData->setAttribute(L"KEY", (_variant_t)L"");                			

            m_XMLScriptHelper.SaveAnimData(pElemAnimData, pAnimData);			


			for(UINT i = 0; i < pAnimData->vecAnimEvent.size(); ++i)
			{
				if(!pAnimData->vecAnimEvent[i])
					continue;

				IXMLDOMElement* pElemEvent = NULL;

				m_pXMLDocument->createElement(L"ANIM_EVENT", &pElemEvent);                

				swprintf(buf, L"%d", pAnimData->vecAnimEvent[i]->eEventID);
				pElemEvent->setAttribute(L"ID", (_variant_t)buf);                

				swprintf(buf, L"%.2f", pAnimData->vecAnimEvent[i]->fTime);
				pElemEvent->setAttribute(L"TIME", (_variant_t)buf);                

                switch(pAnimData->vecAnimEvent[i]->eEventID)
                {
                case EVENT_ANIM_VISUAL_SOUND:                    
                    m_XMLScriptHelper.SaveSoundEvent(pElemEvent, (SEventSound*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_VISUAL_EFFECT:
                    m_XMLScriptHelper.SaveEffectEvent(pElemEvent, (SEventVisualEffect*)pAnimData->vecAnimEvent[i]);				    
                    break;
                case EVENT_ANIM_TMQ:
                    m_XMLScriptHelper.SaveTMQEvent(pElemEvent, (SEventAnimCinematic*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_ALPHA:
                    m_XMLScriptHelper.SaveAlphaEvent(pElemEvent, (SEventAlpha*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_EXPLOSION:
                    m_XMLScriptHelper.SaveExplosionEvent(pElemEvent, (SEventExplosion*)pAnimData->vecAnimEvent[i]);
                    break;
                default:
					NTL_ASSERTE(!"Not Event ID");
                    break;
                }
				pElemAnimData->appendChild(pElemEvent, NULL);
			}
			pElemAnimTable->appendChild(pElemAnimData, NULL);
		}
	}

	m_pElemRoot->appendChild(pElemAnimTable, NULL);

	NTL_RETURN(TRUE);

}
RwBool CNtlPLObjectXMLScript::LoadAnimTable( CNtlPLObjectProperty* pProperty ) 
{
	NTL_FUNCTION(__FUNCTION__);

	std::string strAnimTable = "OBJECT_SCRIPT/ANIM_TABLE/";
	std::string strAnimData = strAnimTable + "ANIM_DATA";    
	IXMLDOMNodeList* pAnimDataList = SelectNodeList((char*)strAnimData.c_str());
	if(pAnimDataList)
	{
		long listLen = 0;
		pAnimDataList->get_length(&listLen);
		for(long i = 0; i < listLen; ++i)
		{
			IXMLDOMNode* pAnimData = NULL;
			pAnimDataList->get_item(i, &pAnimData);
			if(pAnimData)
			{
				STypeAnimData* pTypeAnimData = NTL_NEW STypeAnimData();                
                m_XMLScriptHelper.LoadAnimData(pAnimData, pTypeAnimData);

				// 이벤트
				IXMLDOMNodeList* pEventList = NULL;
				pAnimData->selectNodes(L"ANIM_EVENT", &pEventList);
				if(pEventList)
				{
					long eventCount = 0;
					pEventList->get_length(&eventCount);
					for(long i = 0; i < eventCount; ++i)
					{
						IXMLDOMNode* pNodeEvent = NULL;
						pEventList->get_item(i, &pNodeEvent);
						if(pNodeEvent)
						{
							char szEventID[8] = {0,};
							char szTime[8] = {0,};

							GetTextWithAttributeName(pNodeEvent, "ID", szEventID, 8);
							GetTextWithAttributeName(pNodeEvent, "TIME", szTime, 8);
							int nEventID = atoi(szEventID);							
                            switch(nEventID)
                            {                          
                            case EVENT_ANIM_VISUAL_EFFECT:
                                {
                                    SEventVisualEffect* pEventEffect = NTL_NEW SEventVisualEffect();
                                    pEventEffect->fTime = (RwReal)atof(szTime);
                                    m_XMLScriptHelper.LoadEffectEvent(pNodeEvent, pEventEffect);
                                    pTypeAnimData->vecAnimEvent.push_back(pEventEffect);
                                }                                							    							
                                break;
                            case EVENT_ANIM_VISUAL_SOUND:
                                {
                                    SEventSound* pSoundEvent = NTL_NEW SEventSound();                                                                   
                                    pSoundEvent->fTime = (RwReal)atof(szTime);
                                    m_XMLScriptHelper.LoadSoundEvent(pNodeEvent, pSoundEvent);
                                    pTypeAnimData->vecAnimEvent.push_back(pSoundEvent);
                                }
                                break;
                            case EVENT_ANIM_TMQ:
                                {
                                    SEventAnimCinematic*  pEventTMQ = NTL_NEW SEventAnimCinematic();
                                    pEventTMQ->fTime = (RwReal)atof(szTime);
                                    m_XMLScriptHelper.LoadTMQEvent(pNodeEvent, pEventTMQ);
                                    pTypeAnimData->vecAnimEvent.push_back(pEventTMQ);
                                }
                                break;
                            case EVENT_ANIM_ALPHA:
                                {
                                    SEventAlpha* pEventAlpha = NTL_NEW SEventAlpha();
                                    pEventAlpha->fTime = (RwReal)atof(szTime);
                                    m_XMLScriptHelper.LoadAlphaEvent(pNodeEvent, pEventAlpha);
                                    pTypeAnimData->vecAnimEvent.push_back(pEventAlpha);                                    
                                }
                                break;
                            case EVENT_ANIM_EXPLOSION:
                                {
                                    SEventExplosion* pEventExplosion = NTL_NEW SEventExplosion();
                                    pEventExplosion->fTime = (RwReal)atof(szTime);
                                    m_XMLScriptHelper.LoadExplosionEvent(pNodeEvent, pEventExplosion);
                                    pTypeAnimData->vecAnimEvent.push_back(pEventExplosion);
                                }
                                break;
                            }
						}
					}
				}
				pProperty->GetAnimTable()->Add(pTypeAnimData->uiAnimKey, pTypeAnimData);
			}
		}
	}

	NTL_RETURN(TRUE);

}
