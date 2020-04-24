#include "precomp_ntlpresentation.h"
#include "NtlPLCharacterProperty.h"
#include "NtlDebug.h"
#include "NtlXMLDoc.h"
#include "NtlCharacterXMLScript.h"

unsigned int CNtlPLCharacterProperty::m_strVer;
std::string CNtlPLCharacterProperty::m_strCharacterDataPath = "./devdata/character/";

CNtlPLCharacterProperty::CNtlPLCharacterProperty()
{
	SetClassID("CNtlPLCharacterProperty");	
    ZeroMemory(&m_bboxAnim, sizeof(m_bboxAnim));
    ZeroMemory(&m_vFaceCameraPos, sizeof(m_vFaceCameraPos));
    ZeroMemory(&m_vFaceCameraLookAt, sizeof(m_vFaceCameraLookAt));

    m_vFaceCameraPos.y = 1.0f;
    m_vFaceCameraPos.z =  0.5f;
    m_vFaceCameraLookAt.y = 1.0f;
	m_fInitWalkFrontAnimSpeed = 1.0f;
	m_fInitRunFrontAnimSpeed = 1.0f; 
    m_fAlphaDistance = 0.0f;

	m_vLinkEffect.reserve(10);
}

CNtlPLCharacterProperty::~CNtlPLCharacterProperty()
{
	m_TypeAnimTable.Destroy();

    for(UINT i = 0; i < m_vLinkEffect.size(); ++i)
    {
        NTL_DELETE(m_vLinkEffect[i]);
    }
}

/**
 *  xml file로 object property data를 save하는 함수
 *  \param pDoc는 xml wrapping class pointer.
 *  \param pNod는 유효한 xml node.
 *
 */
RwBool CNtlPLCharacterProperty::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	return TRUE;
}

/**
  *  xml file로부터 object property data를 loading하는 함수
  *  \param pDoc는 xml wrapping class pointer.
  *  \param pNod는 유효한 xml node.
  *
  */
RwBool CNtlPLCharacterProperty::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLCharacterProperty::Load");
	
	NTL_PRE(pDoc);
	NTL_PRE(pNode);
	
	char chScriptFileName[256];
	char chBuffer[256];
	
	if(!pDoc->GetTextWithAttributeName(pNode, "NAME", chBuffer, sizeof(chBuffer)))
	{
		NTL_RETURN(FALSE);
	}
	strcpy_s(chScriptFileName, 256, m_strCharacterDataPath.c_str());
	strcat_s(chScriptFileName, 256, chBuffer);
	
	if(!LoadScript(chScriptFileName))
	{
		NtlLogFilePrintArgs("[ModelTool] %s Load Fail", chScriptFileName);
		NTL_ASSERTFAIL("");
		NTL_RETURN(FALSE);
	}
	NTL_RETURN(TRUE);
}

RwBool CNtlPLCharacterProperty::LoadScript(const std::string &strFileName)
{
	NTL_FUNCTION("CNtlPLCharacterProperty::LoadScript");
	
	m_TypeAnimTable.Destroy();	

	CNtlCharacterXMLScript xmlScript;
	RwBool bReturn = xmlScript.LoadCharacterScript(strFileName.c_str(), this);

	return bReturn;
}

RwBool CNtlPLCharacterProperty::SaveScript(const std::string &strFileName)
{
    NTL_FUNCTION(__FUNCTION__);
    
	CNtlCharacterXMLScript xmlScript;
	RwBool bReturn = xmlScript.SaveCharacterScript(strFileName.c_str(), this);

    return bReturn;
}

void CNtlPLCharacterProperty::SaveSerialize( CNtlSerializer& sOut ) 
{
	sOut<<m_strName;
    sOut<<m_uiFlag;
	sOut<<m_strTexturePath;
	sOut<<m_strSoundPath;
	sOut<<m_TypeMeshFileName;
	sOut<<m_strMeshPath;    
	sOut<<m_fAlphaDistance;

	// Link Effect List
	sOut<<m_vLinkEffect.size();
	for(UINT i = 0; i < m_vLinkEffect.size(); ++i)
	{
		if(m_vLinkEffect[i])
		{
            sOut.In(m_vLinkEffect[i], sizeof(SEventLinkEffect));
		}
	}

	// Anim Table
	m_TypeAnimTable.SaveSerialize(sOut);

	// Bone Data    
	m_TypeBoneData.SaveSerialize(sOut);

	// BBox
    sOut.In(&m_bboxAnim, sizeof(RwBBox));	

	// Unknown
	sOut << m_vLinkUnknownProp.size();
	for (UINT i = 0; i < m_vLinkUnknownProp.size(); ++i)
	{
		if (m_vLinkUnknownProp[i])
		{
			sOut.In(m_vLinkUnknownProp[i], sizeof(sUNKNOWN_CHAR_PROP));
		}
	}

	//Face Camera
    sOut.In(&m_vFaceCameraPos, sizeof(RwV3d));
    sOut.In(&m_vFaceCameraLookAt, sizeof(RwV3d));	

	// Walk Speed
	sOut<<m_fInitRunFrontAnimSpeed;
	sOut<<m_fInitWalkFrontAnimSpeed;
}

void CNtlPLCharacterProperty::LoadSerialize( CNtlSerializer& sIn ) 
{
	sIn>>m_strName;
    sIn>>m_uiFlag;
	sIn>>m_strTexturePath;
	sIn>>m_strSoundPath;
	sIn>>m_TypeMeshFileName;
	sIn>>m_strMeshPath;
    sIn>>m_fAlphaDistance;

	// Link Effect List
	UINT uiLinkEffectSize = 0;
	sIn>>uiLinkEffectSize;

	for(UINT i = 0; i < uiLinkEffectSize; ++i)
	{
		SEventLinkEffect* pEventLinkEffect = NTL_NEW SEventLinkEffect();
        sIn.Out(pEventLinkEffect, sizeof(SEventLinkEffect));		
		m_vLinkEffect.push_back(pEventLinkEffect);
	}

	// Anim Table
	m_TypeAnimTable.LoadSerialize(sIn);

	// Bone Data    
	m_TypeBoneData.LoadSerialize(sIn);

	// BBox
    sIn.Out(&m_bboxAnim, sizeof(RwBBox));

	// Unknown
	UINT uiUnknownCount = 0;
	sIn >> uiUnknownCount;

	for (int i = 0; i < uiUnknownCount; i++)
	{
		sUNKNOWN_CHAR_PROP* pProp = NTL_NEW sUNKNOWN_CHAR_PROP();
		sIn.Out(pProp, sizeof(sUNKNOWN_CHAR_PROP));
		m_vLinkUnknownProp.push_back(pProp);
	}

	// Face Camera
    sIn.Out(&m_vFaceCameraPos, sizeof(RwV3d));
    sIn.Out(&m_vFaceCameraLookAt, sizeof(RwV3d));	

	// Walk Speed
	sIn>>m_fInitWalkFrontAnimSpeed;
	sIn>>m_fInitRunFrontAnimSpeed;
}