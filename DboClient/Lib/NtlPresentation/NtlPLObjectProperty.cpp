#include "precomp_ntlpresentation.h"
#include "NtlPLObjectProperty.h"
#include "NtlXMLDoc.h"
#include "NtlDebug.h"
#include "NtlPLObjectXMLScript.h"
#include "NtLPlApi.h"

unsigned int CNtlPLObjectProperty::m_strVer;
std::string CNtlPLObjectProperty::m_strObjectDataPath = "devdata/Object/";
std::string CNtlPLObjectProperty::m_strMeshPath;
std::string CNtlPLObjectProperty::m_strAniPath;
std::string CNtlPLObjectProperty::m_strTexPath = "texture/object/";


CNtlPLObjectProperty::CNtlPLObjectProperty()
{
	SetClassID("CNtlPLObjectProperty");
	
    ZeroMemory(&m_bbox, sizeof(m_bbox));

    m_bShadow = TRUE;
    m_fUvAnimSpeed = 1.0f;    
    m_fCullingDistance = 300;
    m_bCollision = FALSE;  
    m_eObjectType = E_OBJECT_NORMAL;
    m_bApplyPVS = TRUE;

    m_SoundProp.bLoop = TRUE;       // 기본 사운드는 항상 Loop이다.
}


CNtlPLObjectProperty::~CNtlPLObjectProperty()
{
	m_TypeAnimTable.Destroy();

    for(UINT i = 0; i < m_vLinkEffect.size(); ++i)
    {
        NTL_DELETE(m_vLinkEffect[i]);
    }
}


RwBool CNtlPLObjectProperty::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLObjecProperty::Load");

	NTL_PRE(pDoc);
	NTL_PRE(pNode);

	char chBuffer[XML_PARSING_BUFFER_SIZE];

	// id
	if(!pDoc->GetTextWithAttributeName(pNode, "NAME", chBuffer, XML_PARSING_BUFFER_SIZE))
	{
		NTL_RETURN(FALSE);
	}
	std::string strFileName = m_strObjectDataPath;
	strFileName += chBuffer;
	
	if(!LoadScript(strFileName.c_str()))
	{
		NTL_ASSERTFAIL("Not Find Object File: " << strFileName.c_str());
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLObjectProperty::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	return TRUE;
}

RwBool CNtlPLObjectProperty::SaveScript(const std::string &strFileName)
{
    CNtlPLObjectXMLScript xmlScript;
    return xmlScript.SaveObjectScript(strFileName.c_str(), this);
}

RwBool CNtlPLObjectProperty::LoadScript(const RwChar* strFileName)
{
	m_TypeAnimTable.Destroy();

    CNtlPLObjectXMLScript xmlScript;
    return xmlScript.LoadObjectScript(strFileName, this);
}

void CNtlPLObjectProperty::SaveSerialize( CNtlSerializer& sOut ) 
{
	sOut<<m_strName;
    sOut<<m_uiFlag;
	sOut<<m_strDffFile;
	sOut<<m_strUVAnimFileName;
	sOut<<m_strAnimFileName;
	sOut<<m_strCollisionMeshName;

	// Link Effect List
	sOut<<m_vLinkEffect.size();
	for(UINT i = 0; i < m_vLinkEffect.size(); ++i)
	{
		if(m_vLinkEffect[i])
		{
			sOut.In(m_vLinkEffect[i], sizeof(SEventLinkEffect));			
		}
	}

	// BBox
    sOut.In(&m_bbox, sizeof(RwBBox));	

	sOut<<m_bCollision;
	sOut<<m_bShadow;
	sOut<<m_fUvAnimSpeed;
	sOut<<m_fCullingDistance;
    sOut<<m_eObjectType;
    sOut<<m_bApplyPVS;
    sOut.In(&m_SoundProp, sizeof(m_SoundProp));

	m_TypeAnimTable.SaveSerialize(sOut);
}

void CNtlPLObjectProperty::LoadSerialize( CNtlSerializer& sIn ) 
{
	m_TypeAnimTable.Destroy();

	sIn>>m_strName;
    sIn>>m_uiFlag;
	sIn>>m_strDffFile;
	sIn>>m_strUVAnimFileName;
	sIn>>m_strAnimFileName;
	sIn>>m_strCollisionMeshName;

	// Link Effect List
	UINT uiLinkEffectSize = 0;
	sIn>>uiLinkEffectSize;
	for(UINT i = 0; i < uiLinkEffectSize; ++i)
	{
		SEventLinkEffect* pEventLinkEffect = NTL_NEW SEventLinkEffect();
		sIn.Out(pEventLinkEffect, sizeof(SEventLinkEffect));
		m_vLinkEffect.push_back(pEventLinkEffect);
	}

	// BBox
	sIn.Out(&m_bbox, sizeof(RwBBox));

	sIn>>m_bCollision;
	sIn>>m_bShadow;
	sIn>>m_fUvAnimSpeed;
	sIn>>m_fCullingDistance;
    sIn>>E2I(m_eObjectType);
    sIn>>m_bApplyPVS;
    sIn.Out(&m_SoundProp, sizeof(m_SoundProp));

	m_TypeAnimTable.LoadSerialize(sIn);
}