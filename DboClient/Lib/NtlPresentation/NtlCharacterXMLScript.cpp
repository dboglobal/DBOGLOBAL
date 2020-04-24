#include "precomp_ntlpresentation.h"
#include "NtlCharacterXMLScript.h"
#include "NtlPLCharacterParser.h"
#include <atlbase.h>
#include "comutil.h"
#include "NtlPLHelpers.h"
#include "NtlPLXMLScriptHelper.h"

CNtlCharacterXMLScript::CNtlCharacterXMLScript(void)
{
    m_pElemRoot = NULL;
}

CNtlCharacterXMLScript::~CNtlCharacterXMLScript(void)
{
}

RwBool CNtlCharacterXMLScript::SaveCharacterScript(const char* strFileName, CNtlPLCharacterProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);

    Create();

    IXMLDOMProcessingInstruction* pPI    = NULL;        

    m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    if(!pPI)
        NTL_RETURN(FALSE);
    m_pXMLDocument->appendChild(pPI, NULL);
    
    m_pXMLDocument->createElement(L"MODEL_SCRIPT", &m_pElemRoot);
    if(!m_pElemRoot)
        NTL_RETURN(FALSE);

    m_pXMLDocument->appendChild(m_pElemRoot, NULL);

    if(!SaveHeader(pProperty))
        NTL_RETURN(FALSE);

    if(!SaveBoneData(pProperty))
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

RwBool CNtlCharacterXMLScript::LoadCharacterScript(const char* strFileName, CNtlPLCharacterProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);
    
    if(!pProperty)
        NTL_RETURN(FALSE);

    Create();

    if(!Load((char*)strFileName))
        NTL_RETURN(FALSE);

    if(!LoadHeader(pProperty))
        NTL_RETURN(FALSE);

    if(!LoadLinkEffect(pProperty))
        NTL_RETURN(FALSE);

    if(!LoadBoneData(pProperty))
        NTL_RETURN(FALSE);

    if(!LoadAnimTable(pProperty))
        NTL_RETURN(FALSE);

    NTL_RETURN(TRUE);
}

RwBool CNtlCharacterXMLScript::SaveHeader(CNtlPLCharacterProperty* pProperty)
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    if(!m_pElemRoot || !pProperty)
        NTL_RETURN(FALSE);

    IXMLDOMElement* pElemHeader         = NULL;
    IXMLDOMElement* pElemVersion        = NULL;
    IXMLDOMElement* pElemKeyName        = NULL;
    IXMLDOMElement* pElemMeshFilePath   = NULL;
    IXMLDOMElement* pElemMeshFileName   = NULL;        
    IXMLDOMElement* pElemTexturePath    = NULL;
    IXMLDOMElement* pElemAnimPath       = NULL;
    IXMLDOMElement* pElemSoundPath      = NULL;    
    IXMLDOMElement* pElemAnimBBox       = NULL;
	IXMLDOMElement* pElemUnknown = NULL;
    IXMLDOMElement* pElemFaceCamera     = NULL;
    IXMLDOMElement* pElemFlag           = NULL;
    IXMLDOMElement* pElemAlphaDist      = NULL;

    m_pXMLDocument->createElement(L"HEADER", &pElemHeader);    
    m_pXMLDocument->createElement(L"VERSION", &pElemVersion);
    m_pXMLDocument->createElement(L"NAME", &pElemKeyName);
    m_pXMLDocument->createElement(L"MESH_FILE_PATH", &pElemMeshFilePath);
    m_pXMLDocument->createElement(L"MESH_FILE_NAME", &pElemMeshFileName);        
    m_pXMLDocument->createElement(L"TEXTURE_PATH", &pElemTexturePath);
    m_pXMLDocument->createElement(L"ANIM_PATH", &pElemAnimPath);
    m_pXMLDocument->createElement(L"SOUND_PATH", &pElemSoundPath);
    m_pXMLDocument->createElement(L"ANIM_BBOX", &pElemAnimBBox);
	m_pXMLDocument->createElement(L"UNKNOWN", &pElemUnknown);
    m_pXMLDocument->createElement(L"FACE_CAMERA", &pElemFaceCamera);
    m_pXMLDocument->createElement(L"FLAG", &pElemFlag);
    m_pXMLDocument->createElement(L"ALPHA_DIST", &pElemAlphaDist);
    
    pElemVersion->put_text(CHARACTER_XML_SCRIPT_VER);    
    pElemKeyName->put_text(A2W(pProperty->GetName()));
    pElemMeshFilePath->put_text(A2W(pProperty->GetBaseMeshFilePath().c_str()));
    pElemMeshFileName->put_text(A2W(pProperty->GetBaseMeshFileName().c_str()));
    pElemTexturePath->put_text(A2W(pProperty->GetTexturePath().c_str()));
    pElemAnimPath->put_text(A2W(pProperty->GetAnimTable()->GetAnimPath().c_str()));
    pElemSoundPath->put_text(A2W(pProperty->GetSoundPath().c_str()));

    // Flag    
    WCHAR buf[32] = {0,};
    swprintf_s(buf, L"%d", pProperty->GetFlag());
    pElemFlag->put_text(buf);
    
    // Anim BBox
    
    RwBBox* bboxAnim = pProperty->GetAnimBBox();
    RwReal fAnimBBoxX = bboxAnim->sup.x - bboxAnim->inf.x;
    RwReal fAnimBBoxY = bboxAnim->sup.y - bboxAnim->inf.y;
    RwReal fAnimBBoxZ = bboxAnim->sup.z - bboxAnim->inf.z;    
    swprintf(buf, L"%.2f;%.2f;%.2f", fAnimBBoxX, fAnimBBoxY, fAnimBBoxZ);    
    pElemAnimBBox->put_text(buf);    

	// unknown data
	for (UINT i = 0; i < pProperty->m_vLinkUnknownProp.size(); ++i)
	{
		sUNKNOWN_CHAR_PROP* pUnknown = pProperty->m_vLinkUnknownProp[i];
		if (!pUnknown)
			continue;

		IXMLDOMElement* pSubElem = NULL;

		WCHAR buf[64] = { 0, };

		m_pXMLDocument->createElement(L"IDK", &pSubElem);
		m_XMLScriptHelper.SaveAttribute(pSubElem, L"COUNT", pUnknown->nUnknown);
		m_XMLScriptHelper.SaveAttribute(pSubElem, L"POS", pUnknown->m_vUnknownPos);
		m_XMLScriptHelper.SaveAttribute(pSubElem, L"POS_2", pUnknown->m_vUnknownPos2);

		pElemUnknown->appendChild(pSubElem, NULL);
	}

	m_pElemRoot->appendChild(pElemUnknown, NULL);

    // Face Camera    
    m_XMLScriptHelper.SaveAttribute(pElemFaceCamera, L"POS", pProperty->GetFaceCameraPos());
    m_XMLScriptHelper.SaveAttribute(pElemFaceCamera, L"LOOKAT", pProperty->GetFaceCameraLookAt());

    // Alpha Distance
    swprintf_s(buf, L"%.2f", pProperty->GetAlphaDistance());
    pElemAlphaDist->put_text(buf);

    pElemHeader->appendChild(pElemVersion, NULL);
    pElemHeader->appendChild(pElemKeyName, NULL);
    pElemHeader->appendChild(pElemMeshFilePath, NULL);
    pElemHeader->appendChild(pElemMeshFileName, NULL);    
    pElemHeader->appendChild(pElemTexturePath, NULL);
    pElemHeader->appendChild(pElemAnimPath, NULL);
    pElemHeader->appendChild(pElemSoundPath, NULL);
    pElemHeader->appendChild(pElemAnimBBox, NULL);
    pElemHeader->appendChild(pElemFaceCamera, NULL);
    pElemHeader->appendChild(pElemFlag, NULL);
    pElemHeader->appendChild(pElemAlphaDist, NULL);

    m_pElemRoot->appendChild(pElemHeader, NULL);

    NTL_RETURN(TRUE);
}

RwBool CNtlCharacterXMLScript::SaveBoneData(CNtlPLCharacterProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);
    
    if(!m_pElemRoot || !pProperty)
        NTL_RETURN(FALSE);
    
    IXMLDOMElement* pElemBoneData           = NULL;
    IXMLDOMElement* pElemBoneCount          = NULL;
    IXMLDOMElement* pElemEnableBoneScale    = NULL;
    IXMLDOMElement* pElemEnableBaseScale    = NULL;
    IXMLDOMElement* pElemPosOffset          = NULL;
    IXMLDOMElement* pElemBaseScale          = NULL;    
    IXMLDOMElement* pElemBoneScaleData      = NULL;

    m_pXMLDocument->createElement(L"BONE_DATA", &pElemBoneData);
    m_pXMLDocument->createElement(L"BONE_COUNT", &pElemBoneCount);
    m_pXMLDocument->createElement(L"ENABLE_BONE_SCALE", &pElemEnableBoneScale);
    m_pXMLDocument->createElement(L"ENABLE_BASE_SCALE", &pElemEnableBaseScale);
    m_pXMLDocument->createElement(L"POS_OFFSET", &pElemPosOffset);
    m_pXMLDocument->createElement(L"BASE_SCALE", &pElemBaseScale);
    m_pXMLDocument->createElement(L"BONE_SCALE_DATA", &pElemBoneScaleData);
    
    WCHAR buf[256] = {0,};    
    swprintf(buf, L"%d", pProperty->GetBoneScaleData()->nBoneCount);
    pElemBoneCount->put_text(buf);    

    swprintf(buf, L"%d", pProperty->GetBoneScaleData()->bBoneScale);
    pElemEnableBoneScale->put_text(buf);    

    swprintf(buf, L"%d", pProperty->GetBoneScaleData()->bBaseScale);
    pElemEnableBaseScale->put_text(buf);    

    swprintf(buf, L"%.2f", pProperty->GetBoneScaleData()->vPosOffset.y);
    pElemPosOffset->put_text(buf);    

    swprintf(buf, L"%.2f", pProperty->GetBoneScaleData()->vBaseScale.x);
    pElemBaseScale->put_text(buf);    

	// Bone Scale이 활성화 되어있을때만 저장한다.
	if(pProperty->GetBoneScaleData()->bBoneScale)
	{
		for(int i = 0; i < pProperty->GetBoneScaleData()->nBoneCount; ++i)
		{
			IXMLDOMElement* pElemBoneScale = NULL;
			m_pXMLDocument->createElement(L"BONE_SCALE", &pElemBoneScale);			
	        
            m_XMLScriptHelper.SaveAttribute(pElemBoneScale, L"LENGTH", pProperty->GetBoneScaleData()->BoneScale[i].fLength);
            m_XMLScriptHelper.SaveAttribute(pElemBoneScale, L"WIDTH", pProperty->GetBoneScaleData()->BoneScale[i].fWidth);
			
			pElemBoneScaleData->appendChild(pElemBoneScale, NULL);
		}
	}
    
    pElemBoneData->appendChild(pElemBoneCount, NULL);
    pElemBoneData->appendChild(pElemEnableBoneScale, NULL);
    pElemBoneData->appendChild(pElemEnableBaseScale, NULL);
    pElemBoneData->appendChild(pElemPosOffset, NULL);
    pElemBoneData->appendChild(pElemBaseScale, NULL);
    pElemBoneData->appendChild(pElemBoneScaleData, NULL);

    m_pElemRoot->appendChild(pElemBoneData, NULL);

    NTL_RETURN(TRUE);
}

RwBool CNtlCharacterXMLScript::SaveLinkEffect( CNtlPLCharacterProperty* pProperty ) 
{
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

        m_XMLScriptHelper.SaveAttribute(pElemEffect, L"NAME", pLinkEffect->chEffectName);
        m_XMLScriptHelper.SaveAttribute(pElemEffect, L"BONE", pLinkEffect->chBoneName);
        m_XMLScriptHelper.SaveAttribute(pElemEffect, L"ATTACH_BONE", pLinkEffect->bAttachBone);
        m_XMLScriptHelper.SaveAttribute(pElemEffect, L"OFFSET_POS", pLinkEffect->vOffsetPos);

        pElemLinkEffect->appendChild(pElemEffect, NULL);
    }

    m_pElemRoot->appendChild(pElemLinkEffect, NULL);

    return TRUE;
}

RwBool CNtlCharacterXMLScript::SaveAnimTable(CNtlPLCharacterProperty* pProperty)
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    if(!m_pElemRoot || !pProperty)
        NTL_RETURN(FALSE);

	WCHAR buf[256] = {0,};             

    IXMLDOMElement* pElemAnimTable;    
    m_pXMLDocument->createElement(L"ANIM_TABLE", &pElemAnimTable);

	// Walk Speed	
    m_XMLScriptHelper.SaveAttribute(pElemAnimTable, L"WALK", pProperty->GetInitWalkFrontAnimSpeed());
    m_XMLScriptHelper.SaveAttribute(pElemAnimTable, L"RUN", pProperty->GetInitRunFrontAnimSpeed());

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

            std::string* pstrAnimKey = CNtlPLCharacterParser::GetInstance().GetPCMatchTable()->GetString(pAnimData->uiAnimKey);

			if(!pstrAnimKey)
			{
				// PC 테이블에 없으면 NPC 테이블에서 찾아본다.			
				pstrAnimKey = CNtlPLCharacterParser::GetInstance().GetNPCMatchTable()->GetString(pAnimData->uiAnimKey);
                if(!pstrAnimKey)
                {
                    pstrAnimKey = CNtlPLCharacterParser::GetInstance().GetMobMatchTable()->GetString(pAnimData->uiAnimKey);
                }
			}

            if(pstrAnimKey)
            {
                m_XMLScriptHelper.SaveAttribute(pElemAnimData, L"KEY", pstrAnimKey->c_str());
            }
            else
            {
                m_XMLScriptHelper.SaveAttribute(pElemAnimData, L"KEY", "");
            }

            m_XMLScriptHelper.SaveAnimData(pElemAnimData, pAnimData);


            for(UINT i = 0; i < pAnimData->vecAnimEvent.size(); ++i)
            {
                if(!pAnimData->vecAnimEvent[i] || pAnimData->vecAnimEvent[i]->fTime < 0.0f)
                    continue;

                IXMLDOMElement* pElemEvent = NULL;

                m_pXMLDocument->createElement(L"ANIM_EVENT", &pElemEvent);                

                m_XMLScriptHelper.SaveAttribute(pElemEvent, L"ID", pAnimData->vecAnimEvent[i]->eEventID);
                m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TIME", pAnimData->vecAnimEvent[i]->fTime);

                switch(pAnimData->vecAnimEvent[i]->eEventID)
                {
                case EVENT_ANIM_HIT:
                    SaveHitEvent(pElemEvent, (SEventAnimHit*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_VISUAL_SOUND:                    
                    m_XMLScriptHelper.SaveSoundEvent(pElemEvent, (SEventSound*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_VISUAL_EFFECT:
                    m_XMLScriptHelper.SaveEffectEvent(pElemEvent, (SEventVisualEffect*)pAnimData->vecAnimEvent[i]);                    
                    break;
                case EVENT_ANIM_FOOT_STEP:
                    SaveFootStepEvent(pElemEvent, (SEventFootStep*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_WEIGHT_TIME:                    
                    {
                        SEventWeightTime* pEventSlowTime = (SEventWeightTime*)pAnimData->vecAnimEvent[i];
                        m_XMLScriptHelper.SaveAttribute(pElemEvent, L"LIFE", pEventSlowTime->fLifeTime);
                        m_XMLScriptHelper.SaveAttribute(pElemEvent, L"WEIGHT", pEventSlowTime->fWeightValue);                        
                    }
                    break;
                case EVENT_ANIM_TRACE_EFFECT:
                    SaveTraceEvent(pElemEvent, (SEventTrace*)pAnimData->vecAnimEvent[i]);                    
                    break;
                case EVENT_ANIM_SUB_WEAPON:
                    SaveSubWeaponEvent(pElemEvent, (SEventSubWeapon*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_POST_EFFECT:
					SavePostEffectEvent(pElemEvent, (SEventPostEffect*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_SUMMON_PET: // Summon Pet은 Time외에 저장 값이 없다.
                    break;
                case EVENT_ANIM_ALPHA:                    
                    m_XMLScriptHelper.SaveAlphaEvent(pElemEvent, (SEventAlpha*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_DIRECT:  
                    SaveDirectEvent(pElemEvent, (SEventDirect*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_COLOR_CHANGE: 
                    SaveColorChangeEvent(pElemEvent, (SEventColorChange*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_STRETCH:
                    SaveStretchEvent(pElemEvent, (SEventStretch*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_TRIGGER:
                    SaveTriggerEvent(pElemEvent, (SEventTrigger*)pAnimData->vecAnimEvent[i]);
                    break;
                case EVENT_ANIM_SKILL_CANCEL:
                    SaveSkillCancelEvent(pElemEvent, (SEventSkillCancel*)pAnimData->vecAnimEvent[i]);
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

RwBool CNtlCharacterXMLScript::LoadHeader(CNtlPLCharacterProperty* pProperty)
{
    USES_CONVERSION;

    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);

    std::string strHeader = "MODEL_SCRIPT/HEADER/";
    std::string strVersion = strHeader + "VERSION";
    std::string strMeshFilePath = strHeader + "MESH_FILE_PATH";
    std::string strMeshFileName = strHeader + "MESH_FILE_NAME";    
    std::string strTexturePath = strHeader + "TEXTURE_PATH";
    std::string strAnimPath = strHeader + "ANIM_PATH";
    std::string strSoundPath = strHeader + "SOUND_PATH";
    std::string strKeyName = strHeader + "NAME";
    std::string strAnimBBox = strHeader + "ANIM_BBOX";
    std::string strFaceCamera = strHeader + "FACE_CAMERA";
    std::string strFlag       = strHeader + "FLAG";
    std::string strAlphaDist  = strHeader + "ALPHA_DIST";

    char szVersion[256] = {0,};
    char szFilePath[256] = {0,};
    char szFileName[256] = {0,};    
    char szTexturePath[256] = {0,};
    char szAnimPath[256] = {0,};
    char szSoundPath[256] = {0,};
    char szKeyName[256] = {0,};
    char szAnimBBox[256] = {0,};
    char szFlag[32] = {0,};
    char szAlphaDist[32] = {0,};

    GetDataWithXPath((char*)strVersion.c_str(), szVersion, 256);
    GetDataWithXPath((char*)strKeyName.c_str(), szKeyName, sizeof(szKeyName));
    GetDataWithXPath((char*)strMeshFilePath.c_str(), szFilePath, sizeof(szFilePath));
    GetDataWithXPath((char*)strMeshFileName.c_str(), szFileName, 256);      
    GetDataWithXPath((char*)strTexturePath.c_str(), szTexturePath, 256);
    GetDataWithXPath((char*)strAnimPath.c_str(), szAnimPath, sizeof(szAnimPath));
    GetDataWithXPath((char*)strSoundPath.c_str(), szSoundPath, sizeof(szSoundPath));
    GetDataWithXPath((char*)strAnimBBox.c_str(), szAnimBBox, sizeof(szAnimBBox));
    GetDataWithXPath((char*)strFlag.c_str(), szFlag, sizeof(szFlag));
    GetDataWithXPath((char*)strAlphaDist.c_str(), szAlphaDist, sizeof(szAlphaDist));
    
	pProperty->m_strVer = atoi(szVersion);
    pProperty->SetName(szKeyName);
    pProperty->SetFlag(atoi(szFlag));
    pProperty->m_strMeshPath = szFilePath;
    pProperty->m_TypeMeshFileName = szFileName;    
    pProperty->m_strTexturePath = szTexturePath;
    std::string strLoadAnimPath = szAnimPath;
    pProperty->GetAnimTable()->SetAnimPath(strLoadAnimPath);
    pProperty->m_strSoundPath = szSoundPath;
    pProperty->SetAlphaDistance((RwReal)atof(szAlphaDist));
    

    // Anim BBox ("0.1;0.1;0.1" 형식이다)   
    char szAnimBBoxX[8] = {0,};
    char szAnimBBoxY[8] = {0,};
    char szAnimBBoxZ[8] = {0,};
    char* pIndex = strstr(szAnimBBox, ";");
    if(pIndex)
    {
        char* pIndex2 = strstr(pIndex + 1, ";");
        strncpy(szAnimBBoxX, szAnimBBox, pIndex - szAnimBBox);
        strncpy(szAnimBBoxY, pIndex + 1, pIndex2 - pIndex - 1);
        strcpy(szAnimBBoxZ, pIndex2 + 1);

        RwBBox animBBox;
        float fAnimBoxInfoX = (float)atof(szAnimBBoxX);
        float fAnimBoxInfoY = (float)atof(szAnimBBoxY);
        float fAnimBoxInfoZ = (float)atof(szAnimBBoxZ);

        animBBox.sup.x = fAnimBoxInfoX / 2.0f;
        animBBox.sup.y = fAnimBoxInfoY;
        animBBox.sup.z = fAnimBoxInfoZ / 2.0f;
        animBBox.inf.x = -(fAnimBoxInfoX / 2.0f);
        animBBox.inf.y = 0.0f;
        animBBox.inf.z = -(fAnimBoxInfoZ / 2.0f);

        pProperty->SetAnimBBox(animBBox);
    }

    // Face Camera
    IXMLDOMNode* pNodeFaceCamera = SelectSingleNode(A2W(strFaceCamera.c_str()));
    if(pNodeFaceCamera)
    {
        char szFaceCameaPos[32] = {0,};
        char szFaceCameraHeight[32] = {0,};
        
        GetTextWithAttributeName(pNodeFaceCamera, "POS", szFaceCameaPos, sizeof(szFaceCameaPos));
        GetTextWithAttributeName(pNodeFaceCamera, "LOOKAT", szFaceCameraHeight, sizeof(szFaceCameraHeight));

        pProperty->SetFaceCameraPos(Helper_String2V3D(szFaceCameaPos));
        pProperty->SetFaceCameraLookAt(Helper_String2V3D(szFaceCameraHeight));
    }

    NTL_RETURN(TRUE);
}

RwBool CNtlCharacterXMLScript::LoadBoneData(CNtlPLCharacterProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pProperty)
        NTL_RETURN(FALSE);
    
    std::string strBoneData = "MODEL_SCRIPT/BONE_DATA/";
    std::string strBoneCount = strBoneData + "BONE_COUNT";
    std::string strEnableBoneScale = strBoneData + "ENABLE_BONE_SCALE";
    std::string strEnableBaseScale = strBoneData + "ENABLE_BASE_SCALE";
    std::string strPosOffset = strBoneData + "POS_OFFSET";
    std::string strBaseScale = strBoneData + "BASE_SCALE";
    std::string strBoneScale = strBoneData + "BONE_SCALE_DATA/BONE_SCALE";
    char szBoneCount[256] = {0,};
    char szEnableBoneScale[256] = {0,};
    char szEnableBaseScale[256] = {0,};
    char szPosOffset[256] = {0,};
    char szBaseScale[256] = {0,};

    GetDataWithXPath((char*)strBoneCount.c_str(), szBoneCount, 256);
    GetDataWithXPath((char*)strEnableBoneScale.c_str(), szEnableBoneScale, 256);
    GetDataWithXPath((char*)strEnableBaseScale.c_str(), szEnableBaseScale, 256);
    GetDataWithXPath((char*)strPosOffset.c_str(), szPosOffset, 256);
    GetDataWithXPath((char*)strBaseScale.c_str(), szBaseScale, 256);

    pProperty->GetBoneScaleData()->nBoneCount = atoi(szBoneCount);
    pProperty->GetBoneScaleData()->bBoneScale = (RwBool)atoi(szEnableBoneScale);
    pProperty->GetBoneScaleData()->bBaseScale = (RwBool)atoi(szEnableBaseScale);
    pProperty->GetBoneScaleData()->vPosOffset.x = (RwReal)atof(szPosOffset);
    pProperty->GetBoneScaleData()->vPosOffset.y = (RwReal)atof(szPosOffset);
    pProperty->GetBoneScaleData()->vPosOffset.z = (RwReal)atof(szPosOffset);
    pProperty->GetBoneScaleData()->vBaseScale.x = (RwReal)atof(szBaseScale);
    pProperty->GetBoneScaleData()->vBaseScale.y = (RwReal)atof(szBaseScale);
    pProperty->GetBoneScaleData()->vBaseScale.z = (RwReal)atof(szBaseScale);

	if(pProperty->GetBoneScaleData()->bBoneScale)
	{
		IXMLDOMNodeList* pBoneScaleList = SelectNodeList((char*)strBoneScale.c_str());
		if(pBoneScaleList)
		{
			long listLen = 0;
			pBoneScaleList->get_length(&listLen);
			for(long i = 0; i < listLen; ++i)
			{
				IXMLDOMNode* pNode = NULL;
				pBoneScaleList->get_item(i, &pNode);
				if(pNode)
				{
					char szBoneLength[8] = {0,};
					char szBoneWidth[8] = {0,};
					GetTextWithAttributeName(pNode, "LENGTH", szBoneLength, 8);
					GetTextWithAttributeName(pNode, "WIDTH", szBoneWidth, 8);
					pProperty->GetBoneScaleData()->BoneScale[i].fLength = (RwReal)atof(szBoneLength);
					pProperty->GetBoneScaleData()->BoneScale[i].fWidth = (RwReal)atof(szBoneWidth);
				}
			}
		}
	}

    NTL_RETURN(TRUE);
}

RwBool CNtlCharacterXMLScript::LoadLinkEffect( CNtlPLCharacterProperty* pProperty ) 
{
    std::string strLinkEffect = "MODEL_SCRIPT/LINK_EFFECT/";
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

RwBool CNtlCharacterXMLScript::LoadAnimTable(CNtlPLCharacterProperty* pProperty)
{
    NTL_FUNCTION(__FUNCTION__);

    std::string strAnimTable = "MODEL_SCRIPT/ANIM_TABLE";
    std::string strAnimData = strAnimTable + "/ANIM_DATA";    

	// Walk Speed
	char szWalkAnimSpeed[8] = {0,};
	char szRunAnimSpeed[8] = {0,};

	IXMLDOMNode* pAnimTable = SelectSingleNode((char*)strAnimTable.c_str());
	if(GetTextWithAttributeName(pAnimTable, "WALK", szWalkAnimSpeed, sizeof(szWalkAnimSpeed)))
	{
		pProperty->SetInitWalkFrontAnimSpeed((RwReal)atof(szWalkAnimSpeed));
	}

	if(GetTextWithAttributeName(pAnimTable, "RUN", szRunAnimSpeed, sizeof(szRunAnimSpeed)))
	{
		pProperty->SetInitRunFrontAnimSpeed((RwReal)atof(szRunAnimSpeed));
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
            if(pAnimData)
            {
                STypeAnimData* pTypeAnimData = NTL_NEW STypeAnimData();       

                m_XMLScriptHelper.LoadAnimData(pAnimData, pTypeAnimData);

				//by HongHoDong (2006. 5. 19) Key값이 변경이 되거나 했을 경우 문제가 생기므로 MatchTable에서 KeyID를 가져오도록 한다.
				//저장된 KeyID는 Server용에서 사용을 한다.
                char szKey[128] = {0,};
                GetTextWithAttributeName(pAnimData, "KEY", szKey, sizeof(szKey));
                
				pTypeAnimData->uiAnimKey = CNtlPLCharacterParser::GetInstance().GetPCMatchTable()->GetKey(szKey);
				if(pTypeAnimData->uiAnimKey == INVALID_GRAPHIC_ANIMATION_ID)
				{
					pTypeAnimData->uiAnimKey = CNtlPLCharacterParser::GetInstance().GetNPCMatchTable()->GetKey(szKey);
                    if(pTypeAnimData->uiAnimKey == INVALID_GRAPHIC_ANIMATION_ID)
                    {
                        pTypeAnimData->uiAnimKey = CNtlPLCharacterParser::GetInstance().GetMobMatchTable()->GetKey(szKey);
                    }
				}

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
                            if(nEventID == EVENT_ANIM_HIT)
                            {
                                SEventAnimHit* pHitEvent = NTL_NEW SEventAnimHit();
                                pHitEvent->fTime = (RwReal)atof(szTime);

                                LoadHitEvent(pNodeEvent, pHitEvent);
                         
                                pTypeAnimData->vecAnimEvent.push_back(pHitEvent);
                            }
                            else if(nEventID == EVENT_ANIM_VISUAL_SOUND)
                            {
                                SEventSound* pSoundEvent = NTL_NEW SEventSound();        
                                pSoundEvent->fTime = (RwReal)atof(szTime);
                                m_XMLScriptHelper.LoadSoundEvent(pNodeEvent, pSoundEvent);                               

                                pTypeAnimData->vecAnimEvent.push_back(pSoundEvent);
                            }
                            else if(nEventID == EVENT_ANIM_VISUAL_EFFECT)
                            {
                                SEventVisualEffect* pEventEffect = NTL_NEW SEventVisualEffect();
                                pEventEffect->fTime = (RwReal)atof(szTime);
                                m_XMLScriptHelper.LoadEffectEvent(pNodeEvent, pEventEffect);

                                pTypeAnimData->vecAnimEvent.push_back(pEventEffect);
                            }
                            else if(nEventID == EVENT_ANIM_FOOT_STEP)
                            {
                                SEventFootStep* pEventFootStep = NTL_NEW SEventFootStep();
                                pEventFootStep->fTime = (RwReal)atof(szTime);

                                LoadFootStepEvent(pNodeEvent, pEventFootStep);

                                pTypeAnimData->vecAnimEvent.push_back(pEventFootStep);
                            }
                            else if(nEventID == EVENT_ANIM_WEIGHT_TIME)
                            {
                                SEventWeightTime* pEventSlowTime = NTL_NEW SEventWeightTime();
                                char szLifeTIme[8] = {0,};
                                char szWeightValue[8] = {0,};

                                GetTextWithAttributeName(pNodeEvent, "LIFE", szLifeTIme, sizeof(szLifeTIme));
                                GetTextWithAttributeName(pNodeEvent, "WEIGHT", szWeightValue, sizeof(szWeightValue));

                                pEventSlowTime->fTime = (RwReal)atof(szTime);
                                pEventSlowTime->fLifeTime = (RwReal)atof(szLifeTIme);
                                pEventSlowTime->fWeightValue = (RwReal)atof(szWeightValue);

                                pTypeAnimData->vecAnimEvent.push_back(pEventSlowTime);
                            }
                            else if(nEventID == EVENT_ANIM_TRACE_EFFECT)
                            {
                                SEventTrace* pEventTrace = NTL_NEW SEventTrace();
                                pEventTrace->fTime = (RwReal)atof(szTime);

                                LoadTraceEvent(pNodeEvent, pEventTrace);

                                pTypeAnimData->vecAnimEvent.push_back(pEventTrace);

                            }
                            else if(nEventID == EVENT_ANIM_SUB_WEAPON)
                            {
                                SEventSubWeapon* pEventSubWeapon = NTL_NEW SEventSubWeapon();
                                pEventSubWeapon->fTime = (RwReal)atof(szTime);

                                LoadSubWeaponEvent(pNodeEvent, pEventSubWeapon);

                                pTypeAnimData->vecAnimEvent.push_back(pEventSubWeapon);
                            }
							else if(nEventID == EVENT_ANIM_POST_EFFECT)
							{
								SEventPostEffect* pEventPostEffect = NTL_NEW SEventPostEffect();
								pEventPostEffect->fTime = (RwReal)atof(szTime);

								LoadPostEffectEvent(pNodeEvent, pEventPostEffect);

								pTypeAnimData->vecAnimEvent.push_back(pEventPostEffect);
							}
							else if(nEventID == EVENT_ANIM_SUMMON_PET)
							{
								SEventSummonPet* pEventSummonPet = NTL_NEW SEventSummonPet();
								pEventSummonPet->fTime = (RwReal)atof(szTime);

								pTypeAnimData->vecAnimEvent.push_back(pEventSummonPet);
							}
                            else if(nEventID == EVENT_ANIM_ALPHA)
                            {
                                SEventAlpha* pEventAlpha = NTL_NEW SEventAlpha();
                                pEventAlpha->fTime = (RwReal)atof(szTime);

                                m_XMLScriptHelper.LoadAlphaEvent(pNodeEvent, pEventAlpha);

                                pTypeAnimData->vecAnimEvent.push_back(pEventAlpha);
                            }
                            else if(nEventID == EVENT_ANIM_DIRECT)
                            {
                                SEventDirect* pEventDirect = NTL_NEW SEventDirect();
                                pEventDirect->fTime = (RwReal)atof(szTime);
                                LoadDirectEvent(pNodeEvent, pEventDirect);
                                pTypeAnimData->vecAnimEvent.push_back(pEventDirect);
                            }
                            else if(nEventID == EVENT_ANIM_COLOR_CHANGE)
                            {
                                SEventColorChange* pEvent = NTL_NEW SEventColorChange();
                                pEvent->fTime = (RwReal)atof(szTime);
                                LoadColorChangeEvent(pNodeEvent, pEvent);
                                pTypeAnimData->vecAnimEvent.push_back(pEvent);
                            }
                            else if(nEventID == EVENT_ANIM_STRETCH)
                            {
                                SEventStretch* pEvent = NTL_NEW SEventStretch();
                                pEvent->fTime = (RwReal)atof(szTime);
                                LoadStretchEvent(pNodeEvent, pEvent);
                                pTypeAnimData->vecAnimEvent.push_back(pEvent);
                            }
                            else if(nEventID == EVENT_ANIM_TRIGGER)
                            {
                                SEventTrigger* pEvent = NTL_NEW SEventTrigger();
                                pEvent->fTime = (RwReal)atof(szTime);
                                LoadTriggerEvent(pNodeEvent, pEvent);
                                pTypeAnimData->vecAnimEvent.push_back(pEvent);
                            }
                            else if(nEventID == EVENT_ANIM_SKILL_CANCEL)
                            {
                                SEventSkillCancel* pEvent = NTL_NEW SEventSkillCancel();
                                pEvent->fTime = (RwReal)atof(szTime);
                                LoadSkillCancelEvent(pNodeEvent, pEvent);
                                pTypeAnimData->vecAnimEvent.push_back(pEvent);
                            }
							else
							{
								NTL_ASSERTE(!"Not Event ID");
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

void CNtlCharacterXMLScript::SaveTraceEvent( IXMLDOMElement* pElemEvent, const SEventTrace* pEventTrace ) 
{
    USES_CONVERSION;

    if(!pElemEvent || !pEventTrace)
        return;

    WCHAR buf[256] = {0,};

    swprintf(buf, L"%.2f", pEventTrace->fLifeTime);
    pElemEvent->setAttribute(L"LIFE", (_variant_t)buf);

    swprintf(buf, L"%.2f", pEventTrace->fEdgeLifeTime);
    pElemEvent->setAttribute(L"EDGE_LIFE", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->eAttachType);
    pElemEvent->setAttribute(L"ATTACH_TYPE", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->eTraceKind);
    pElemEvent->setAttribute(L"TRACE_KIND", (_variant_t)buf);

    pElemEvent->setAttribute(L"START_BONE", (_variant_t)A2W(pEventTrace->strStartBoneName));
    pElemEvent->setAttribute(L"END_BONE", (_variant_t)A2W(pEventTrace->strEndBoneName));
    pElemEvent->setAttribute(L"TEXTURE", (_variant_t)A2W(pEventTrace->strTexture));

    Helper_V3D2StringU(buf, pEventTrace->v3dStartBoneOffset);
    pElemEvent->setAttribute(L"START_OFFSET", (_variant_t)buf);

    Helper_V3D2StringU(buf, pEventTrace->v3dEndBoneOffset);
    pElemEvent->setAttribute(L"END_OFFSET", (_variant_t)buf);

    swprintf(buf, L"%.2f", pEventTrace->fEdgeGap);
    pElemEvent->setAttribute(L"EDGE_GAP", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->nSplinePointCount);
    pElemEvent->setAttribute(L"SPLINE_COUNT", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->nMaxEdgeCount);
    pElemEvent->setAttribute(L"MAX_EDGE", (_variant_t)buf);

    swprintf(buf, L"%.2f", pEventTrace->fMaxLength);
    pElemEvent->setAttribute(L"MAX_LENGTH", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->eSrcBlend);
    pElemEvent->setAttribute(L"SRC_BLEND", (_variant_t)buf);

    swprintf(buf, L"%d", pEventTrace->eDestBlend);
    pElemEvent->setAttribute(L"DEST_BLEND", (_variant_t)buf);

    swprintf(buf, L"%d;%d;%d;%d", pEventTrace->colStartColor.red,
        pEventTrace->colStartColor.green,
        pEventTrace->colStartColor.blue,
        pEventTrace->colStartColor.alpha);
    pElemEvent->setAttribute(L"START_COLOR", (_variant_t)buf);

    swprintf(buf, L"%d;%d;%d;%d", pEventTrace->colEndColor.red,
        pEventTrace->colEndColor.green,
        pEventTrace->colEndColor.blue,
        pEventTrace->colEndColor.alpha);
    pElemEvent->setAttribute(L"END_COLOR", (_variant_t)buf);                    
}

void CNtlCharacterXMLScript::LoadTraceEvent(IXMLDOMNode* pNodeEvent, SEventTrace* pEventTrace)
{
    if(!pNodeEvent || !pEventTrace)
        return;

    char buf[256] = {0,};

    if(GetTextWithAttributeName(pNodeEvent, "LIFE", buf, sizeof(buf)))
    {
        pEventTrace->fLifeTime = (RwReal)atof(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "EDGE_LIFE", buf, sizeof(buf)))
    {
        pEventTrace->fEdgeLifeTime = (RwReal)atof(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "ATTACH_TYPE", buf, sizeof(buf)))
    {
        pEventTrace->eAttachType = (SEventTrace::EAttachType)atoi(buf);
    }

    if(GetTextWithAttributeName(pNodeEvent, "TRACE_KIND", buf, sizeof(buf)))
    {
        pEventTrace->eTraceKind = (SEventTrace::ETraceKind)atoi(buf);
    }

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
}

void CNtlCharacterXMLScript::SaveSubWeaponEvent( IXMLDOMElement* pElemEvent, const SEventSubWeapon* pEventSubWeapon ) 
{
    if(!pElemEvent || !pEventSubWeapon)
        return;

    WCHAR buf[256] = {0,};

    swprintf(buf, L"%d", pEventSubWeapon->eSubWeaponActiveFlag);
    pElemEvent->setAttribute(L"ACTIVE", (_variant_t)buf);
    
}

void CNtlCharacterXMLScript::LoadSubWeaponEvent( IXMLDOMNode* pNodeEvent, SEventSubWeapon* pEventSubWeapon ) 
{
    if(!pNodeEvent || !pEventSubWeapon)
        return;

    char buf[256] = {0,};

    if(GetTextWithAttributeName(pNodeEvent, "ACTIVE", buf, sizeof(buf)))
    {
        pEventSubWeapon->eSubWeaponActiveFlag = (ESubWeaponActiveFlag)atoi(buf);
    }

}

void CNtlCharacterXMLScript::SavePostEffectEvent( IXMLDOMElement* pElemEvent, const SEventPostEffect* pEventPostEffect ) 
{
	if(!pElemEvent || !pEventPostEffect)
		return;

    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"NAME", pEventPostEffect->szPostEffectName);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET", pEventPostEffect->eTarget);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"OFFSET", pEventPostEffect->v3dOffset);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"CENTER_FIX", pEventPostEffect->bCenterFixEnable);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET_HEIGHT", pEventPostEffect->fTargetHeight);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"PC_BONE", pEventPostEffect->szPCBoneName);
}

void CNtlCharacterXMLScript::LoadPostEffectEvent( IXMLDOMNode* pNodeEvent, SEventPostEffect* pPostEffect ) 
{
	if(!pNodeEvent || !pPostEffect)
		return;

	USES_CONVERSION;

	char buf[256] = {0,};

	if(GetTextWithAttributeName(pNodeEvent, "NAME", buf, sizeof(buf)))
	    strcpy(pPostEffect->szPostEffectName, buf);
	
	if(GetTextWithAttributeName(pNodeEvent, "TARGET", buf, sizeof(buf)))
	    pPostEffect->eTarget = (EPostEffectTypeFlag)atoi(buf);
	
	if(GetTextWithAttributeName(pNodeEvent, "OFFSET", buf, sizeof(buf)))
	    pPostEffect->v3dOffset = Helper_String2V3D(buf);

    if(GetTextWithAttributeName(pNodeEvent, "CENTER_FIX", buf, sizeof(buf)))
        pPostEffect->bCenterFixEnable = (RwBool)atoi(buf);

    if(GetTextWithAttributeName(pNodeEvent, "TARGET_HEIGHT", buf, sizeof(buf)))
        pPostEffect->fTargetHeight = (RwReal)atof(buf);

    if(GetTextWithAttributeName(pNodeEvent, "PC_BONE", buf, sizeof(buf)))
        strcpy_s(pPostEffect->szPCBoneName, buf);
}

void CNtlCharacterXMLScript::SaveHitEvent(IXMLDOMElement* pElemEvent, const SEventAnimHit* pEventHit)
{
    if(!pElemEvent || !pEventHit)
        return;

    WCHAR buf[256] = {0,};

    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"BEHAVIOR", pEventHit->eTargetBehavior);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"KB2PUSH", pEventHit->bKB2Push);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"DAMAGE", pEventHit->uiDamage);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"POWER", pEventHit->bPowerEffect);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TYPE", pEventHit->eAttackType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HAND", pEventHit->eHandType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"BONE", pEventHit->chBoneName);                       
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SUB_WEAPON_BONE", pEventHit->nSubWeaponFlag);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"EFFECT", pEventHit->uiProjectileEffectType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SHOT", pEventHit->eProjectileShotType);                       
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"PROJ_NAME", pEventHit->chProjectileEffectName);                        
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"PROJ_SPEED", pEventHit->fProjectileSpeed);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET_ATTACH_BONE", pEventHit->bTargetAttach);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET_EFFECT", pEventHit->chTargetEffectName);    					
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET_EFFECT_TYPE", pEventHit->eTargetEffectType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SUBTARGET_EFFECT", pEventHit->chSubTargetEffect);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET_SOUND", pEventHit->chTargetSoundName);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SOUND_TYPE", pEventHit->eSoundType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HIT_SOUND_ECHO", pEventHit->bHitSoundEcho);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TARGET_HEIGHT", pEventHit->fTargetHeight);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"WORD_EFFECT", pEventHit->chWordEffect);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SHAKE", pEventHit->bCameraShake);

    // 추가 데이터 저장
    if(pEventHit->eAttackType == ATTACK_TYPE_ENERGY)
    {
        switch(pEventHit->uiProjectileEffectType)
        {
        case BEID_PROJ_HISSIDAN:                                         
            m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HISSIDAN_APPLY_ANGLE", pEventHit->uEffectTypeExtraData.hissidanData.bApplyAngle);
            m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HISSIDIAN_DIR", pEventHit->uEffectTypeExtraData.hissidanData.v2dAngle);
            break;
		case BEID_PROJ_HELLZONE:
			m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HOIDAN_OFFSET", pEventHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset);
			m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HOIDAN_WAIT_TIME", pEventHit->uEffectTypeExtraData.hellZoneData.fTargetEffectStartWaitTime);
			m_XMLScriptHelper.SaveAttribute(pElemEvent, L"HOIDAN_SPEED", pEventHit->uEffectTypeExtraData.hellZoneData.fTargetEffectSpeed);
			break;
        case BEID_PROJ_MULTI_HISSIDAN:
            // Hissidan의 개수만큼 저장한다. (count는 저장하지 않아도 된다. 로드할때 읽은만큼 count를 설정한다)
            for(int cnt = 0; cnt < pEventHit->uEffectTypeExtraData.multiHissidanData.nCount; ++cnt)
            {
                IXMLDOMElement* pElemMultiHissidanDir = NULL;
                m_pXMLDocument->createElement(L"MULTI_HISSIDAN_DIR", &pElemMultiHissidanDir);
                Helper_V2D2StringU(buf, pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[cnt]);
                pElemMultiHissidanDir->put_text(buf);
                pElemEvent->appendChild(pElemMultiHissidanDir, NULL);
            }
            break;
        }
    }

    // 카메라 셰이크
    if(pEventHit->bCameraShake)
    {
        m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SHAKE_FACTOR", pEventHit->fCameraShakeFactor);
        m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SHAKE_MAX_HEIGHT", pEventHit->fCameraShakeMaxHeight);
    }
}

void CNtlCharacterXMLScript::LoadHitEvent(IXMLDOMNode* pNodeEvent, SEventAnimHit* pEventHit)
{
    char szBehavior[8] = {0,};
    char szKB2Push[8] = {0,};
    char szDamage[8] = {0,};
    char szPower[8] = {0,};
    char szType[8] = {0,};
    char szHand[8] = {0,};
    char szBone[64] = {0,};
    char szSubWeaponBone[8] = {0,};
    char szEffect[8] = {0,};                                
    char szProjShot[8] = {0,};
    char szProjName[32] = {0,};
    char szProjSpeed[8] = {0,};
    char szAttachBone[8] = {0,};
    char szTargetEffectName[32] = {0,};
    char szTargetEffectType[8] = {0,};
    char szSubTargetEffect[MAX_DEFAULT_NAME] = {0,};
    char szTargetSoundName[32] = {0,};
    char szTargetSoundType[8] = {0,};
    char szHitSoundEcho[8] = {0,};
    char szTargetHeight[8] = {0,};
    char szWordEffect[MAX_DEFAULT_NAME] = {0,};
    char szCameraShake[8] = {0,};
    char szShakeFactor[8] = {0,};
    char szShakeMaxHeight[8] = {0,};
    char szHoidanOffset[MAX_DEFAULT_NAME] = {0,};
    char szHoidanStartWaitTime[MAX_DEFAULT_NAME] = {0,};
    char szHoidanSpeed[MAX_DEFAULT_NAME] = {0,};
    char szHissiDianDir[MAX_DEFAULT_NAME] = {0,};
    char szHissidanApplyDir[8] = {0,};

    GetTextWithAttributeName(pNodeEvent, "BEHAVIOR", szBehavior, sizeof(szBehavior));
    GetTextWithAttributeName(pNodeEvent, "KB2PUSH", szKB2Push, sizeof(szKB2Push));
    GetTextWithAttributeName(pNodeEvent, "DAMAGE", szDamage, sizeof(szDamage));
    GetTextWithAttributeName(pNodeEvent, "POWER", szPower, sizeof(szPower));
    GetTextWithAttributeName(pNodeEvent, "TYPE", szType, sizeof(szType));
    GetTextWithAttributeName(pNodeEvent, "HAND", szHand, sizeof(szHand));
    GetTextWithAttributeName(pNodeEvent, "BONE", szBone, sizeof(szBone));
    GetTextWithAttributeName(pNodeEvent, "SUB_WEAPON_BONE", szSubWeaponBone, sizeof(szSubWeaponBone));
    GetTextWithAttributeName(pNodeEvent, "EFFECT", szEffect, sizeof(szEffect));                                
    GetTextWithAttributeName(pNodeEvent, "SHOT", szProjShot, sizeof(szProjShot));
    GetTextWithAttributeName(pNodeEvent, "PROJ_NAME", szProjName, sizeof(szProjName));
    GetTextWithAttributeName(pNodeEvent, "TARGET_ATTACH_BONE", szAttachBone, sizeof(szAttachBone));    
    GetTextWithAttributeName(pNodeEvent, "TARGET_EFFECT", szTargetEffectName, sizeof(szTargetEffectName));
    GetTextWithAttributeName(pNodeEvent, "TARGET_EFFECT_TYPE", szTargetEffectType, sizeof(szTargetEffectType));
    GetTextWithAttributeName(pNodeEvent, "SUBTARGET_EFFECT", szSubTargetEffect, sizeof(szSubTargetEffect));
    GetTextWithAttributeName(pNodeEvent, "TARGET_SOUND", szTargetSoundName, sizeof(szTargetSoundName));
    GetTextWithAttributeName(pNodeEvent, "SOUND_TYPE", szTargetSoundType, sizeof(szTargetSoundType));
    GetTextWithAttributeName(pNodeEvent, "HIT_SOUND_ECHO", szHitSoundEcho, sizeof(szHitSoundEcho));
    GetTextWithAttributeName(pNodeEvent, "TARGET_HEIGHT", szTargetHeight, sizeof(szTargetHeight));
    GetTextWithAttributeName(pNodeEvent, "WORD_EFFECT", szWordEffect, sizeof(szWordEffect));
    GetTextWithAttributeName(pNodeEvent, "SHAKE", szCameraShake, sizeof(szCameraShake));
    GetTextWithAttributeName(pNodeEvent, "HOIDAN_OFFSET", szHoidanOffset, sizeof(szHoidanOffset));
    GetTextWithAttributeName(pNodeEvent, "HOIDAN_WAIT_TIME", szHoidanStartWaitTime, sizeof(szHoidanStartWaitTime));
    GetTextWithAttributeName(pNodeEvent, "HOIDAN_SPEED", szHoidanSpeed, sizeof(szHoidanSpeed));
    GetTextWithAttributeName(pNodeEvent, "HISSIDIAN_DIR", szHissiDianDir, sizeof(szHissiDianDir));
    GetTextWithAttributeName(pNodeEvent, "HISSIDAN_APPLY_ANGLE", szHissidanApplyDir, sizeof(szHissidanApplyDir));    


    if(GetTextWithAttributeName(pNodeEvent, "PROJ_SPEED", szProjSpeed, sizeof(szProjSpeed)))
    {
        pEventHit->fProjectileSpeed = (RwReal)atof(szProjSpeed);
    }

    pEventHit->eTargetBehavior = (ETargetBehavior)atoi(szBehavior);
    pEventHit->bKB2Push = (RwBool)atoi(szKB2Push);
    pEventHit->uiDamage = (RwUInt32)atoi(szDamage);
    pEventHit->bPowerEffect = (RwBool)atoi(szPower);
    pEventHit->eAttackType = (EAttackType)atoi(szType);
    pEventHit->eHandType = (EHandType)atoi(szHand);
    sprintf(pEventHit->chBoneName, "%s", szBone);
    pEventHit->nSubWeaponFlag = atoi(szSubWeaponBone);
    pEventHit->uiProjectileEffectType = (EProjectileEffectType)atoi(szEffect);                                
    pEventHit->eProjectileShotType = (EBoneType)atoi(szProjShot);
    sprintf(pEventHit->chProjectileEffectName, "%s", szProjName);
    pEventHit->bTargetAttach = (RwBool)atoi(szAttachBone);
    sprintf(pEventHit->chTargetEffectName, "%s", szTargetEffectName);
    pEventHit->eTargetEffectType = (ETargetEffectType)atoi(szTargetEffectType);
    sprintf(pEventHit->chSubTargetEffect, "%s", szSubTargetEffect);
    sprintf(pEventHit->chTargetSoundName, "%s", szTargetSoundName);
    pEventHit->eSoundType = (eChannelGroupType)atoi(szTargetSoundType);
    pEventHit->bHitSoundEcho = (RwBool)atoi(szHitSoundEcho);
    pEventHit->fTargetHeight = (RwReal)atof(szTargetHeight);
    sprintf(pEventHit->chWordEffect, "%s", szWordEffect);
    pEventHit->bCameraShake = (RwBool)atoi(szCameraShake);

    // 추가 데이터를 읽어들인다.
    if(pEventHit->eAttackType == ATTACK_TYPE_ENERGY)
    {
        switch(pEventHit->uiProjectileEffectType)
        {
        case BEID_PROJ_HISSIDAN:
            pEventHit->uEffectTypeExtraData.hissidanData.bApplyAngle = (RwBool)atoi(szHissidanApplyDir);
            pEventHit->uEffectTypeExtraData.hissidanData.v2dAngle = Helper_String2V2D(szHissiDianDir);
            break;
        case BEID_PROJ_MULTI_HISSIDAN:
            IXMLDOMNodeList* pMultiHissidanList;
            pNodeEvent->selectNodes(L"MULTI_HISSIDAN_DIR", &pMultiHissidanList);
            if(pMultiHissidanList)
            {
                long lDirCount = 0;
                WCHAR szDir[32] = {0,};

                pMultiHissidanList->get_length(&lDirCount);
                pEventHit->uEffectTypeExtraData.multiHissidanData.nCount = lDirCount;           // count
                pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle = NTL_NEW RwV2d[lDirCount];   // 동적 할당한다.

                for(int k = 0; k < lDirCount; ++k)
                {
                    IXMLDOMNode* pNodeDir = NULL;
                    pMultiHissidanList->get_item(k, &pNodeDir);
                    if(pNodeDir)
                    {
                        BSTR bstr = NULL;
                        pNodeDir->get_text(&bstr);
                        wcscpy_s(szDir, 32, bstr);
                        pEventHit->uEffectTypeExtraData.multiHissidanData.pArrayAngle[k] = Helper_String2V2D(szDir);

                        ::SysFreeString(bstr);                                                    
                    }
                }
            }
            break;
        case BEID_PROJ_HELLZONE:
            pEventHit->uEffectTypeExtraData.hellZoneData.vTargetEffectOffset = Helper_String2V3D(szHoidanOffset);
            pEventHit->uEffectTypeExtraData.hellZoneData.fTargetEffectStartWaitTime = (RwReal)atof(szHoidanStartWaitTime);
            pEventHit->uEffectTypeExtraData.hellZoneData.fTargetEffectSpeed = (RwReal)atof(szHoidanSpeed);
            break;
        }
    }

    // 카메라 셰이크
    if(pEventHit->bCameraShake)
    {
        if(GetTextWithAttributeName(pNodeEvent, "SHAKE_FACTOR", szShakeFactor, sizeof(szShakeFactor)))
            pEventHit->fCameraShakeFactor = (RwReal)atof(szShakeFactor);

        if(GetTextWithAttributeName(pNodeEvent, "SHAKE_MAX_HEIGHT", szShakeMaxHeight, sizeof(szShakeMaxHeight)))
            pEventHit->fCameraShakeMaxHeight = (RwReal)atof(szShakeMaxHeight);
    }
}

void CNtlCharacterXMLScript::SaveFootStepEvent( IXMLDOMElement* pElemEvent, const SEventFootStep* pEventFootStep ) 
{
    if(!pElemEvent || !pEventFootStep)
        return;

    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TYPE", pEventFootStep->eFootStepType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"MOB_TYPE", pEventFootStep->eFootStepMobType);
}

void CNtlCharacterXMLScript::LoadFootStepEvent( IXMLDOMNode* pNodeEvent, SEventFootStep* pFootStep ) 
{
    if(!pNodeEvent || !pFootStep)
        return;

    char buf[8] = {0,};

    if(GetTextWithAttributeName(pNodeEvent, "TYPE", buf, sizeof(buf)))
        pFootStep->eFootStepType = (EFootStepType)atoi(buf);

    if(GetTextWithAttributeName(pNodeEvent, "MOB_TYPE", buf, sizeof(buf)))
        pFootStep->eFootStepMobType = (EFootStepMobType)atoi(buf);
}

void CNtlCharacterXMLScript::SaveDirectEvent( IXMLDOMElement* pElemEvent, const SEventDirect* pEventDirect ) 
{
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TYPE", pEventDirect->eType);
}

void CNtlCharacterXMLScript::LoadDirectEvent( IXMLDOMNode* pNodeEvent, SEventDirect* pEventDirect ) 
{
    char buf[8] = {0,};
    if(GetTextWithAttributeName(pNodeEvent, "TYPE", buf, sizeof(buf)))
        pEventDirect->eType = (ENtlPLDirectEventType)atoi(buf);
}

void CNtlCharacterXMLScript::SaveColorChangeEvent( IXMLDOMElement* pElemEvent, const SEventColorChange* pEvent ) 
{
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TYPE", pEvent->eType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"EDGE", pEvent->colorEdge);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"BODY", pEvent->colorBody);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"ADD", pEvent->colorAdd);
}

void CNtlCharacterXMLScript::LoadColorChangeEvent( IXMLDOMNode* pNodeEvent, SEventColorChange* pEvent ) 
{
    char buf[64] = {0,};
    if(GetTextWithAttributeName(pNodeEvent, "TYPE", buf, sizeof(buf)))
        pEvent->eType = (EColorChangeType)atoi(buf);
    if(GetTextWithAttributeName(pNodeEvent, "EDGE", buf, sizeof(buf)))
        pEvent->colorEdge = Helper_String2RGBA(buf);
    if(GetTextWithAttributeName(pNodeEvent, "BODY", buf, sizeof(buf)))
        pEvent->colorBody = Helper_String2RGBA(buf);
    if(GetTextWithAttributeName(pNodeEvent, "ADD", buf, sizeof(buf)))
        pEvent->colorAdd = Helper_String2RGBA(buf);
}

void CNtlCharacterXMLScript::SaveStretchEvent( IXMLDOMElement* pElemEvent, const SEventStretch* pEvent ) 
{
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"TYPE", pEvent->eType);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"BONE1", pEvent->szBoneName[0]);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"BONE2", pEvent->szBoneName[1]);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SPEED", pEvent->fStretchSpeed);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"WITDH", pEvent->fWidth);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"ACCEL", pEvent->fAccel);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SCALE_BONE", pEvent->szScaleBone);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"SCALE", pEvent->fScaleSize);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"AXIS", pEvent->szAxisBone);
    m_XMLScriptHelper.SaveAttribute(pElemEvent, L"EFFECT", pEvent->szTargetEffect);
}

void CNtlCharacterXMLScript::LoadStretchEvent( IXMLDOMNode* pNodeEvent, SEventStretch* pEvent ) 
{
    char buf[MAX_DEFAULT_NAME] = {0,};    
    if(GetTextWithAttributeName(pNodeEvent, "TYPE", buf, MAX_DEFAULT_NAME))
        pEvent->eType = (EStretchEventType)atoi(buf);

    GetTextWithAttributeName(pNodeEvent, "BONE1", pEvent->szBoneName[0], MAX_DEFAULT_NAME);
    GetTextWithAttributeName(pNodeEvent, "BONE2", pEvent->szBoneName[1], MAX_DEFAULT_NAME);
    
    if(GetTextWithAttributeName(pNodeEvent, "SPEED", buf, sizeof(buf)))
        pEvent->fStretchSpeed = (RwReal)atof(buf);    

    if(GetTextWithAttributeName(pNodeEvent, "WIDTH", buf, sizeof(buf)))
        pEvent->fWidth = (RwReal)atof(buf);    

    if(GetTextWithAttributeName(pNodeEvent, "ACCEL", buf, sizeof(buf)))
        pEvent->fAccel = (RwReal)atof(buf);

    GetTextWithAttributeName(pNodeEvent, "SCALE_BONE", pEvent->szScaleBone, MAX_DEFAULT_NAME);

    if(GetTextWithAttributeName(pNodeEvent, "SCALE", buf, sizeof(buf)))
        pEvent->fScaleSize = (RwReal)atof(buf);

    GetTextWithAttributeName(pNodeEvent, "AXIS", pEvent->szAxisBone, MAX_DEFAULT_NAME);

    GetTextWithAttributeName(pNodeEvent, "EFFECT", pEvent->szTargetEffect, MAX_DEFAULT_NAME);
}

void CNtlCharacterXMLScript::SaveTriggerEvent( IXMLDOMElement* pElemEvent, const SEventTrigger* pEvent ) 
{

}

void CNtlCharacterXMLScript::LoadTriggerEvent( IXMLDOMNode* pNodeEvent, SEventTrigger* pEvent ) 
{

}
void CNtlCharacterXMLScript::SaveSkillCancelEvent( IXMLDOMElement* pElemEvent, const SEventSkillCancel* pEvent ) 
{

}
void CNtlCharacterXMLScript::LoadSkillCancelEvent( IXMLDOMNode* pNodeEvent, SEventSkillCancel* pEvent ) 
{

}