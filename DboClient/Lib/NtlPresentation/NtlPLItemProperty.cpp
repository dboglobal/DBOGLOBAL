#include "precomp_ntlpresentation.h"
#include "NtlPLItemProperty.h"
#include "NtlDebug.h"
#include "NtlPLItem.h"
#include "NtlXMLDoc.h"
#include "NtlPLItemXMLScript.h"
#include "NtlPLApi.h"

unsigned int CNtlPLItemProperty::m_strVer;			  /* property file version **/
std::string CNtlPLItemProperty::m_strItemDataPath;	  /* Item mesh file의 path name **/

CNtlPLItemProperty::CNtlPLItemProperty()
{
	SetClassID("CNtlPLItemProperty");
    m_TypeResType       = ITEM_RES_EQUIP_BODY;
    m_TypeEquipSlotType = ITEM_EQUIP_NONE;
    ZeroMemory(&m_v3dAttachOffset, sizeof(RwV3d));

	m_vLinkEffect.reserve(10);
    
    m_bTraceEnableDefault = FALSE;
}

CNtlPLItemProperty::~CNtlPLItemProperty()
{
    m_TypeAnimTable.Destroy();

    // Link Effect의 해제는 여기서 한다.
    for(UINT i = 0; i < m_vLinkEffect.size(); ++i)
    {
        NTL_DELETE(m_vLinkEffect[i]);
    }
}

/**
  *  xml file로부터 object property data를 loading하는 함수
  *  \param pDoc는 xml wrapping class pointer.
  *  \param pNod는 유효한 xml node.
  *
  */
RwBool CNtlPLItemProperty::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
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
	strcpy_s(chScriptFileName, 256, m_strItemDataPath.c_str());
	strcat_s(chScriptFileName, 256, chBuffer);
	
	if(!LoadScript(chScriptFileName))
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}



/**
    *  xml file로 object property data를 save하는 함수

	*  \param pDoc는 xml wrapping class pointer.
	*  \param pNod는 유효한 xml node.
    *
    */
RwBool CNtlPLItemProperty::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	return TRUE;
}

RwBool CNtlPLItemProperty::LoadScript( const std::string &strFileName ) 
{
    m_TypeAnimTable.Destroy();

    CNtlPLItemXMLScript xmlScript;
    RwBool bReturn = xmlScript.LoadItemScript(strFileName.c_str(), this);
    xmlScript.Destroy();    

    return bReturn;
}
RwBool CNtlPLItemProperty::SaveScript( const std::string &strFileName ) 
{
    CNtlPLItemXMLScript xmlScript;
    RwBool bReturn = xmlScript.SaveItemScript(strFileName.c_str(), this);
    xmlScript.Destroy();

    return bReturn;
   
}

void CNtlPLItemProperty::SaveSerialize( CNtlSerializer& sOut ) 
{
	sOut<<m_strName;

    // Flag
    sOut<<m_uiFlag;

	// Link Effect List
	sOut<<m_vLinkEffect.size();
	for(UINT i = 0; i < m_vLinkEffect.size(); ++i)
	{
		if(m_vLinkEffect[i])
		{
			sOut.In(m_vLinkEffect[i], sizeof(SEventLinkEffect));			
		}
	}

	// Trace Event
	sOut.In(&m_eventTrace, sizeof(SEventTrace));	

	sOut<<m_TypeMeshPath;
	sOut<<m_TypeMeshFileName;
	sOut<<m_TypeResType;
	sOut<<m_TypeEquipSlotType;	
	m_TypeAnimTable.SaveSerialize(sOut);		// Anim Table
	sOut<<m_v3dAttachOffset.x;
	sOut<<m_v3dAttachOffset.y;
	sOut<<m_v3dAttachOffset.z;	
	sOut<<m_bTraceEnableDefault;
	
	// Upgrade Effect 
    sOut.In(&m_UpgradeEffectProp, sizeof(SUpgradeEffectProperty));	
}

void CNtlPLItemProperty::LoadSerialize( CNtlSerializer& sIn ) 
{
	sIn>>m_strName;

    // Flag
    sIn>>m_uiFlag;

	// Link Effect List
	UINT uiLinkEffectSize = 0;
	sIn>>uiLinkEffectSize;
	for(UINT i = 0; i < uiLinkEffectSize; ++i)
	{
		SEventLinkEffect* pEventLinkEffect = NTL_NEW SEventLinkEffect();
		sIn.Out(pEventLinkEffect, sizeof(SEventLinkEffect));		
		m_vLinkEffect.push_back(pEventLinkEffect);
	}

	// Trace Event
	sIn.Out(&m_eventTrace, sizeof(SEventTrace));	

	sIn>>m_TypeMeshPath;
	sIn>>m_TypeMeshFileName;
	sIn>>E2I(m_TypeResType);
	sIn>>E2I(m_TypeEquipSlotType);
	m_TypeAnimTable.LoadSerialize(sIn);
	sIn>>m_v3dAttachOffset.x;
	sIn>>m_v3dAttachOffset.y;
	sIn>>m_v3dAttachOffset.z;
	sIn>>m_bTraceEnableDefault;

	// Upgrade Effect
    sIn.Out(&m_UpgradeEffectProp, sizeof(SUpgradeEffectProperty));	
}