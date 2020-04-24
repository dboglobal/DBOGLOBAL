//***********************************************************************************
//	File		:	CVenusPropertyContainer.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venuspropertycontainer.h"

#include ".\venusconfig.h"
#include "VenusDefine.h"
#include "Manipulator.h"
#include "VenusCamera.h"
#include "VenusVisualManager.h"
#include "EffectEditLayer.h"
#include "VenusFrame.h"

#include "NtlPLProperty.h"


CVenusPropertyContainer::CVenusPropertyContainer(void)
{
	m_lpDevice			= NULL;
	m_pEffectSplinePath	= NULL;

	m_bSave	= FALSE;

	RwIm3DVertexSetPos(&m_SplinePointVertex[0], 1.0f,-1.0f, 1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[1], 1.0f,-1.0f,-1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[2],-1.0f,-1.0f,-1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[3],-1.0f,-1.0f, 1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[4], 1.0f, 1.0f, 1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[5], 1.0f, 1.0f,-1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[6],-1.0f, 1.0f,-1.0f);
	RwIm3DVertexSetPos(&m_SplinePointVertex[7],-1.0f, 1.0f, 1.0f);

	m_SplinePointIndices[ 0] = 00;	m_SplinePointIndices[ 1] = 04;	m_SplinePointIndices[ 2] = 07;
	m_SplinePointIndices[ 3] = 00;	m_SplinePointIndices[ 4] = 07;	m_SplinePointIndices[ 5] = 03;
	m_SplinePointIndices[ 6] = 03;	m_SplinePointIndices[ 7] = 07;	m_SplinePointIndices[ 8] = 06;
	m_SplinePointIndices[ 9] = 03;	m_SplinePointIndices[10] = 06;	m_SplinePointIndices[11] = 02;
	m_SplinePointIndices[12] = 01;	m_SplinePointIndices[13] = 05;	m_SplinePointIndices[14] = 06;
	m_SplinePointIndices[15] = 01;	m_SplinePointIndices[16] = 06;	m_SplinePointIndices[17] = 02;
	m_SplinePointIndices[18] = 00;	m_SplinePointIndices[19] = 04;	m_SplinePointIndices[20] = 05;
	m_SplinePointIndices[21] = 00;	m_SplinePointIndices[22] = 05;	m_SplinePointIndices[23] = 01;
	m_SplinePointIndices[24] = 04;	m_SplinePointIndices[25] = 05;	m_SplinePointIndices[26] = 06;
	m_SplinePointIndices[27] = 04;	m_SplinePointIndices[28] = 06;	m_SplinePointIndices[29] = 07;
	m_SplinePointIndices[30] = 00;	m_SplinePointIndices[31] = 01;	m_SplinePointIndices[32] = 02;
	m_SplinePointIndices[33] = 00;	m_SplinePointIndices[34] = 02;	m_SplinePointIndices[35] = 03;

	m_fControlPointRadius = 0.1f;


	m_bCopyResourceData		= TRUE;
	m_pCopyResourceEffect	= NULL;
	m_pCopyResourceSystem	= NULL;
	m_nCopySystemDataFlag	= 0;

	m_bCopySplinePath		= TRUE;
	m_pCopySplinePath		= NULL;

    m_svEffectProperty.reserve(10000);
}

CVenusPropertyContainer::~CVenusPropertyContainer(void)
{
}

//------------------------------------------------------------------
//	FuncName	: GetInstance
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CVenusPropertyContainer& CVenusPropertyContainer::GetInstance()
{
	static CVenusPropertyContainer container;
	return container;
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Create(void)
{
	m_lpDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();
	return CNtlPLPropertyContainer::Create();
}
//------------------------------------------------------------------
//	FuncName	: Destroy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::Destroy(void)
{
	m_bCopyResourceData		= TRUE;
	m_pCopyResourceEffect	= NULL;
	m_pCopyResourceSystem	= NULL;
	m_nCopySystemDataFlag	= 0;

	m_svEffectProperty.clear();
	m_svSplinePathProperty.clear();
	m_svSelectSplineControlPoint.clear();
	m_pEffectSplinePath	= NULL;

	CNtlPLPropertyContainer::Destroy();
}

void CVenusPropertyContainer::Clear() 
{
	m_bCopyResourceData		= TRUE;
	m_pCopyResourceEffect	= NULL;
	m_pCopyResourceSystem	= NULL;
	m_nCopySystemDataFlag	= 0;

	m_svEffectProperty.clear();

	m_svSplinePathProperty.clear();

	m_svSelectSplineControlPoint.clear();
	m_pEffectSplinePath	= NULL;

	CNtlPLProperty *pProp;
	MapProp::iterator it;

	for(it = m_mapEffect.begin(); it != m_mapEffect.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapEffect.clear();

	// by agebreak
	for(it = m_mapSpline.begin(); it != m_mapSpline.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}
	m_mapSpline.clear();
}

void CVenusPropertyContainer::Render(void)
{
	if (CVenusConfig::GetInstance().m_nCurrentLayer == EDIT_LAYER_PATH) 
	{
		if (m_pEffectSplinePath != NULL)
		{
			RwMatrix matWorld;
			RwMatrixSetIdentity(&matWorld);
			DrawSpline(m_pEffectSplinePath, &matWorld);

			if (!m_svSelectSplineControlPoint.empty())
			{
				RwV3d vTotalPosition = { 0, 0, 0 };
				svdef_Int32::iterator it;
				for (it = m_svSelectSplineControlPoint.begin(); it != m_svSelectSplineControlPoint.end(); ++ it)
				{
					RwInt32 index = (*it);
					RwV3d vPosition = m_pEffectSplinePath->m_pControlPoint[index].pos;
					RwV3dAdd(&vTotalPosition, &vTotalPosition, &vPosition);
				}
				vTotalPosition.x = vTotalPosition.x / (RwReal)m_svSelectSplineControlPoint.size();
				vTotalPosition.y = vTotalPosition.y / (RwReal)m_svSelectSplineControlPoint.size();
				vTotalPosition.z = vTotalPosition.z / (RwReal)m_svSelectSplineControlPoint.size();

				RwMatrix vViewMatrix = *CVenusCamera::GetInstance().GetViewMatrix();
				CManipulator::GetInstance().SetPosition(vTotalPosition.x, vTotalPosition.y, vTotalPosition.z);
				CManipulator::GetInstance().Render(&vViewMatrix);
			}
		}
	}
}

//------------------------------------------------------------------
//	FuncName	: AddProp
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::AddProp(ENtlPLEntityType eType, CNtlPLProperty *pProp)
{
	if (eType == PLENTITY_EFFECT)
	{
		m_svEffectProperty.push_back(pProp);        
	}
	else if (eType == PLENTITY_SPLINE)
	{
		m_svSplinePathProperty.push_back(pProp);
	}

	return CNtlPLPropertyContainer::AddProp(eType, pProp);
}


//------------------------------------------------------------------
//	FuncName	: CreateResourceEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CNtlPLProperty* CVenusPropertyContainer::CreateProperty(ENtlPLEntityType eType, const RwChar* strName, CNtlResourceEffect* pSrcResourceEffect)
{
	m_bSave = TRUE;

	CNtlPLProperty* pProperty = NULL;
	switch(eType)
	{
	case PLENTITY_EFFECT:
		{
			pProperty = NTL_NEW CNtlResourceEffect(strName);
		}
		break;
	case PLENTITY_SPLINE:
		{
			pProperty = NTL_NEW CNtlEffectSplinePath(strName);
		}
		break;
	}

	AddProp(eType, pProperty);

	if (pSrcResourceEffect != NULL)
	{
		CNtlResourceEffect* pDestResourceEffect = (CNtlResourceEffect*)pProperty;

		CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
		for (it = pSrcResourceEffect->m_svResourceSystem.begin(); it != pSrcResourceEffect->m_svResourceSystem.end(); ++ it)
		{
			CreateResourceSystem(pDestResourceEffect, (*it)->GetSystemType(),  (*it)->GetSystemName(), (*it));
		}
	}

	return pProperty;
}

//------------------------------------------------------------------
//	FuncName	: DeleteProperty
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::DeleteProperty(ENtlPLEntityType eType, const RwChar* strName)
{
	m_bSave = TRUE;

	if (eType == PLENTITY_EFFECT)
	{
		if (!m_svEffectProperty.empty())
		{
			svdef_Property::iterator it;
			for (it = m_svEffectProperty.begin(); it != m_svEffectProperty.end(); ++ it)            
			{
				if (_stricmp((*it)->GetName(), strName) == 0)
				{
					m_svEffectProperty.erase(it);                    
					break;
				}                
            }
		}

		if (m_pCopyResourceEffect != NULL)
		{
			if (_stricmp(m_pCopyResourceEffect->GetName(), strName) == 0)
			{
				SetCopyResourceData(TRUE);
			}
		}
	}
	else if (eType == PLENTITY_SPLINE)
	{
		NTL_ASSERTE(m_mapSpline.size() == m_svSplinePathProperty.size());

		if (!m_svSplinePathProperty.empty())
		{
			svdef_Property::iterator it;
			for (it = m_svSplinePathProperty.begin(); it != m_svSplinePathProperty.end(); ++ it)
			{
				if (_stricmp((*it)->GetName(), strName) == 0)
				{
					m_svSplinePathProperty.erase(it);
					break;
				}
			}
		}
		if (m_pCopySplinePath != NULL)
		{
			if (_stricmp(m_pCopySplinePath->GetName(), strName) == 0)
			{
				SetCopySplinePath(TRUE);
			}
		}
	}

	MapProp& PropertyList = Gets(eType);
	MapProp::iterator it;
	if (!PropertyList.empty())
	{
		MapProp::iterator it = PropertyList.find(strName);
		if (it != PropertyList.end())
		{
			NTL_DELETE((*it).second);
			PropertyList.erase(it);                   
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------
//	FuncName	: IsResourceSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::IsResourceSystem(CNtlResourceEffect* pResourceEffect, const RwChar* strSystemName)
{
	CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
	for (it = pResourceEffect->m_svResourceSystem.begin(); it != pResourceEffect->m_svResourceSystem.end(); ++ it)
	{
		if (_stricmp((*it)->GetSystemName(), strSystemName) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}
//------------------------------------------------------------------
//	FuncName	: CreateResourceSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CNtlResourceComponentSystem* CVenusPropertyContainer::CreateResourceSystem(CNtlResourceEffect* pResourceEffect, RwInt8 nType, const RwChar* strName, CNtlResourceComponentSystem* pSrcSystem)
{
	CNtlResourceComponentSystem* pDestSystem = CNtlResourceEffect::CreateResourceSystem(nType, strName);

	if (pSrcSystem != NULL)
	{
		NTL_ASSERTE(pDestSystem->GetSystemType() == pSrcSystem->GetSystemType());
	}

	switch(nType)
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		{
			CNtlResourceParticleSystem* pDestParticleSystem = (CNtlResourceParticleSystem*)pDestSystem;
			if (pSrcSystem == NULL)
				InitialEmitterStandard(&pDestParticleSystem->m_EmitterStandard);
			else
				CopySystem_Particle(pDestParticleSystem, (CNtlResourceParticleSystem*)pSrcSystem);
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		{
			CNtlResourceMeshSystem* pResourceMeshSystem = (CNtlResourceMeshSystem*)pDestSystem;
			if (pSrcSystem == NULL)
				InitialEmitterStandard(&pResourceMeshSystem->m_EmitterStandard);
			else
				CopySystem_Mesh(pResourceMeshSystem, (CNtlResourceMeshSystem*)pSrcSystem);
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		{
			CNtlResourceBeamSystem* pResourceBeamSystem = (CNtlResourceBeamSystem*)pDestSystem;
			if (pSrcSystem == NULL)
			{
				InitialEmitterStandard(&pResourceBeamSystem->m_EmitterStandard);
				pResourceBeamSystem->m_bEternity = FALSE;
			}
			else
				CopySystem_Beam(pResourceBeamSystem, (CNtlResourceBeamSystem*)pSrcSystem);
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		{
			CNtlResourceHurricaneSystem* pResourceHurricaneSystem = (CNtlResourceHurricaneSystem*)pDestSystem;
			if (pSrcSystem == NULL)
			{
				InitialEmitterStandard(&pResourceHurricaneSystem->m_EmitterStandard);
				pResourceHurricaneSystem->m_bEternity = FALSE;
			}
			else
				CopySystem_Hurricane(pResourceHurricaneSystem, (CNtlResourceHurricaneSystem*)pSrcSystem);
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		{
			CNtlResourceLightningSystem* pResourceLightningSystem = (CNtlResourceLightningSystem*)pDestSystem;
			if (pSrcSystem == NULL)
			{
				InitialEmitterStandard(&pResourceLightningSystem->m_EmitterStandard);
				pResourceLightningSystem->m_bEternity = FALSE;
			}
			else
				CopySystem_Lightning(pResourceLightningSystem, (CNtlResourceLightningSystem*)pSrcSystem);
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		{
			CNtlResourceDecalSystem* pResourceDecalSystem = (CNtlResourceDecalSystem*)pDestSystem;
			if (pSrcSystem == NULL)
			{
				InitialEmitterStandard(&pResourceDecalSystem->m_EmitterStandard);
				pResourceDecalSystem->m_bEternity = FALSE;
			}
			else
				CopySystem_Decal(pResourceDecalSystem, (CNtlResourceDecalSystem*)pSrcSystem);
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		{
			CNtlResourcePostEffectSystem* pResourcePostEffectSystem = (CNtlResourcePostEffectSystem*)pDestSystem;
			if (pSrcSystem == NULL)
			{
				InitialEmitterStandard(&pResourcePostEffectSystem->m_EmitterStandard);
				pResourcePostEffectSystem->m_bEternity = FALSE;
			}
			else
				CopySystem_PostEffect(pResourcePostEffectSystem, (CNtlResourcePostEffectSystem*)pSrcSystem);
		}
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        {
            CNtlResourceLineSystem* pResourceLineSystem = (CNtlResourceLineSystem*)pDestSystem;
            if(pSrcSystem == NULL)
            {
                pResourceLineSystem->m_bEternity = FALSE;
            }
            else
                CopySystem_Line(pResourceLineSystem, (CNtlResourceLineSystem*)pSrcSystem);
        }
        break;
	default:
		{
			NTL_ASSERTE(!"default");
		}
		break;
	}
	pResourceEffect->m_svResourceSystem.push_back(pDestSystem);

	m_bSave = TRUE;

	return pDestSystem;
}


//------------------------------------------------------------------
//	FuncName	: DeleteResourceSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::DeleteResourceSystem(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceSystem)
{
	if (pResourceEffect->m_svResourceSystem.empty()) return FALSE;

	m_bSave = TRUE;

	if (m_pCopyResourceSystem != NULL)
	{
		if (m_pCopyResourceSystem == pResourceSystem)
		{
			SetCopyResourceData(TRUE);
		}
	}

	CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
	it = std::find(pResourceEffect->m_svResourceSystem.begin(), pResourceEffect->m_svResourceSystem.end(), pResourceSystem);
	if (it != pResourceEffect->m_svResourceSystem.end())
	{
		// 시스템을 삭제하기전에 이 시스템을 서브 시스템으로 사용하는곳이 있는지 체크해서 지워준다.
		CNtlResourceEffect::svdef_ResourceComponentSystem::iterator kt;
		for (kt = pResourceEffect->m_svResourceSystem.begin(); kt != pResourceEffect->m_svResourceSystem.end(); ++kt)
		{
			if ((*kt)->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE)
			{
				CNtlResourceParticleSystem* pResourceParticleSystem = (CNtlResourceParticleSystem*)(*kt);
				if (pResourceParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER) && !pResourceParticleSystem->m_strSubSystem.empty())
				{
					pResourceParticleSystem->m_strSubSystem.clear();
				}
			}
		}

		// resource List 에서 삭제하기
		pResourceEffect->m_svResourceSystem.erase(it);
		pResourceEffect->ResetSubSystem();

		NTL_DELETE(pResourceSystem);
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------
//	FuncName	: ChangePropertyName
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::ChangePropertyName(ENtlPLEntityType eType, const RwChar* strSrcName, const RwChar* strDestName)
{
	m_bSave = TRUE;

	MapProp& PropertyList = Gets(eType);
	MapProp::iterator it;
	if (!PropertyList.empty())
	{
		MapProp::iterator it = PropertyList.find(strSrcName);
		if (it != PropertyList.end())
		{
			CNtlPLProperty* pPlProperty = (*it).second;
			PropertyList.erase(it);

			pPlProperty->SetName(strDestName);

			PropertyList[strDestName] = pPlProperty;
			return TRUE;
		}
	}

	return FALSE;
}

void CVenusPropertyContainer::SetResourceEffectSize(CNtlResourceEffect* pResourceEffect, RwReal fScale)
{
	pResourceEffect->m_fScaleForTool = fScale;

	CVenusVisualManager::GetInstance().ResetInstanceEffect(pResourceEffect->GetName());
}


//------------------------------------------------------------------
//	FuncName	: SetEmitterDataFlag
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetEmitterDataFlag(CNtlResourceComponentSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceComponentSystem* pSrcSystem)
{
	m_bSave = TRUE;

	if (pSrcSystem != NULL)
	{
		NTL_ASSERTE(pDestSystem->GetSystemType() == pSrcSystem->GetSystemType());
	}

	switch(pDestSystem->GetSystemType())
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		SetEmitterDataFlag_ParticleSystem((CNtlResourceParticleSystem*)pDestSystem, dataFlag, (CNtlResourceParticleSystem*)pSrcSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		SetEmitterDataFlag_MeshSystem((CNtlResourceMeshSystem*)pDestSystem, dataFlag, (CNtlResourceMeshSystem*)pSrcSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		SetEmitterDataFlag_BeamSystem((CNtlResourceBeamSystem*)pDestSystem, dataFlag, (CNtlResourceBeamSystem*)pSrcSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		SetEmitterDataFlag_HurricaneSystem((CNtlResourceHurricaneSystem*)pDestSystem, dataFlag, (CNtlResourceHurricaneSystem*)pSrcSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		SetEmitterDataFlag_LightningSystem((CNtlResourceLightningSystem*)pDestSystem, dataFlag, (CNtlResourceLightningSystem*)pSrcSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		SetEmitterDataFlag_DecalSystem((CNtlResourceDecalSystem*)pDestSystem, dataFlag, (CNtlResourceDecalSystem*)pSrcSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		SetEmitterDataFlag_PostEffectSystem((CNtlResourcePostEffectSystem*)pDestSystem, dataFlag, (CNtlResourcePostEffectSystem*)pSrcSystem);
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        SetEmitterDataFlag_LineSystem((CNtlResourceLineSystem*)pDestSystem, dataFlag, (CNtlResourceLineSystem*)pSrcSystem);
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: SetEmitterDataFlag_ComponentSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetEmitterDataFlag_ComponentSystem(CNtlResourceComponentSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceComponentSystem* pSrcSystem)
{
	switch(dataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
		NTL_ASSERTE(pDestSystem->m_nPathEmitterDataFlag == 0);
		pDestSystem->m_nPathEmitterDataFlag = rpPRTADVEMITTERDATAFLAGSPLINEPATH;
		if (pSrcSystem == NULL)
			InitialEmitterSplinePath(&pDestSystem->m_EmitterPrtSplinePath);
		else
			memcpy(&pDestSystem->m_EmitterPrtSplinePath, &pSrcSystem->m_EmitterPrtSplinePath, sizeof(SNtlPrtEmitterSplinePath));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTFADEOUT:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtFadeOut(&pDestSystem->m_EmitterPrtFadeOut);
		else
			memcpy(&pDestSystem->m_EmitterPrtFadeOut, &pSrcSystem->m_EmitterPrtFadeOut, sizeof(SNtlPrtStdEmitterPrtFadeOut));
		break;

	case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiTexturesEmitter(&pDestSystem->m_EmitterMultiTextures);
		else
		{
			memcpy(&pDestSystem->m_EmitterMultiTextures, &pSrcSystem->m_EmitterMultiTextures, sizeof(RpPrtAdvEmtPrtMultiTextures));
			if (pDestSystem->m_EmitterMultiTextures.numTextures > 0)
			{
				pDestSystem->m_EmitterMultiTextures.list = NTL_NEW RpPrtAdvEmtPrtTextureItem[pDestSystem->m_EmitterMultiTextures.numTextures];
				for (RwUInt32 i = 0;i < pDestSystem->m_EmitterMultiTextures.numTextures; ++ i)
				{
					pDestSystem->m_EmitterMultiTextures.list[i].time = pSrcSystem->m_EmitterMultiTextures.list[i].time;
					pDestSystem->m_EmitterMultiTextures.list[i].timeBias = pSrcSystem->m_EmitterMultiTextures.list[i].timeBias;
					pDestSystem->m_EmitterMultiTextures.list[i].strTexture = pSrcSystem->m_EmitterMultiTextures.list[i].strTexture.c_str();
				}
			}
		}
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: SetEmitterDataFlag_ParticleSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetEmitterDataFlag_ParticleSystem(CNtlResourceParticleSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceParticleSystem* pSrcSystem)
{
	SetEmitterDataFlag_ComponentSystem(pDestSystem, dataFlag, pSrcSystem);
	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtColor(&pDestSystem->m_EmitterPrtCol);
		else
			memcpy(&pDestSystem->m_EmitterPrtCol, &pSrcSystem->m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtTexCoords(&pDestSystem->m_EmitterPrtTexCoords);
		else
			memcpy(&pDestSystem->m_EmitterPrtTexCoords, &pSrcSystem->m_EmitterPrtTexCoords, sizeof(RpPrtStdEmitterPrtTexCoords));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrt2DRotate(&pDestSystem->m_EmitterPrtRotate);
		else
			memcpy(&pDestSystem->m_EmitterPrtRotate, &pSrcSystem->m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtSize(&pDestSystem->m_EmitterPrtSize);
		else
			memcpy(&pDestSystem->m_EmitterPrtSize, &pSrcSystem->m_EmitterPrtSize, sizeof(RpPrtStdEmitterPrtSize));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTMATRIX:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtMatrix(&pDestSystem->m_EmitterPrtMatrix);
		else
			memcpy(&pDestSystem->m_EmitterPrtMatrix, &pSrcSystem->m_EmitterPrtMatrix, sizeof(RpPrtStdEmitterPrtMatrix));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTHURRICANE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtHurricane(&pDestSystem->m_EmitterPrtHurricane);
		else
			memcpy(&pDestSystem->m_EmitterPrtHurricane, &pSrcSystem->m_EmitterPrtHurricane, sizeof(SNtlPrtStdEmitterPrtHurricane));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtOrbitPoint(&pDestSystem->m_EmitterPrtOrbitPoint);
		else
			memcpy(&pDestSystem->m_EmitterPrtOrbitPoint, &pSrcSystem->m_EmitterPrtOrbitPoint, sizeof(SNtlPrtStdEmitterPrtOrbitPoint));
		break;

	case rpPRTADVEMITTERDATAFLAGPRTEMITTER:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialPrtEmitterEmitter(&pDestSystem->m_PrtEmitterEmitter);
		else
			memcpy(&pDestSystem->m_PrtEmitterEmitter, &pSrcSystem->m_PrtEmitterEmitter, sizeof(RpPrtAdvPrtEmitterEmitter));
		break;
	case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiColorEmitter(&pDestSystem->m_EmitterPrtMultiCol);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiCol, &pSrcSystem->m_EmitterPrtMultiCol, sizeof(RpPrtAdvPrtMultiColorEmitter));
			pDestSystem->m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[pDestSystem->m_EmitterPrtMultiCol.numCol];
			memcpy(pDestSystem->m_EmitterPrtMultiCol.list, pSrcSystem->m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem) * pDestSystem->m_EmitterPrtMultiCol.numCol);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiTexCoordsEmitter(&pDestSystem->m_EmitterPrtMultiTexCoords);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiTexCoords, &pSrcSystem->m_EmitterPrtMultiTexCoords, sizeof(RpPrtAdvEmtPrtMultiTexCoords));
			pDestSystem->m_EmitterPrtMultiTexCoords.list = NTL_NEW RpPrtAdvEmtPrtTexCoordsItem[pDestSystem->m_EmitterPrtMultiTexCoords.numTex];
			memcpy(pDestSystem->m_EmitterPrtMultiTexCoords.list, pSrcSystem->m_EmitterPrtMultiTexCoords.list, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * pDestSystem->m_EmitterPrtMultiTexCoords.numTex);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGMULTISIZE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiSizeEmitter(&pDestSystem->m_EmitterPrtMultiSize);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiSize, &pSrcSystem->m_EmitterPrtMultiSize, sizeof(RpPrtAdvPrtMultiSizeEmitter));
			pDestSystem->m_EmitterPrtMultiSize.list = NTL_NEW RpPrtAdvEmtPrtSizeItem[pDestSystem->m_EmitterPrtMultiSize.numSize];
			memcpy(pDestSystem->m_EmitterPrtMultiSize.list, pSrcSystem->m_EmitterPrtMultiSize.list, sizeof(RpPrtAdvEmtPrtSizeItem) * pDestSystem->m_EmitterPrtMultiSize.numSize);
		}
		break;
    case rpPRTSTDEMITTERDATAFLAGTRAIL:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if(pSrcSystem)
        {
            pDestSystem->m_EmitterTrail = pSrcSystem->m_EmitterTrail;
        }
        break;
	case rpPRTADVEMITTERDATAFLAGPOINTLIST:
		NTL_ASSERTE(pDestSystem->m_nPointEmitterDataFlag == 0);
		pDestSystem->m_nPointEmitterDataFlag = rpPRTADVEMITTERDATAFLAGPOINTLIST;
		if (pSrcSystem == NULL)
			InitialEmitterPointList(&pDestSystem->m_EmitterPtList);
		else
		{
			memcpy(&pDestSystem->m_EmitterPtList, &pSrcSystem->m_EmitterPtList, sizeof(RpPrtAdvPointListEmitter));
			pDestSystem->m_EmitterPtList.pointList = NTL_NEW RwV3d[pDestSystem->m_EmitterPtList.numPoint];
			memcpy(pDestSystem->m_EmitterPtList.pointList, pSrcSystem->m_EmitterPtList.pointList, sizeof(RwV3d) * pDestSystem->m_EmitterPtList.numPoint);
			pDestSystem->m_EmitterPtList.directionList = NTL_NEW RwV3d[pDestSystem->m_EmitterPtList.numPoint];
			memcpy(pDestSystem->m_EmitterPtList.directionList, pSrcSystem->m_EmitterPtList.directionList, sizeof(RwV3d) * pDestSystem->m_EmitterPtList.numPoint);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGCIRCLE:
		NTL_ASSERTE(pDestSystem->m_nPointEmitterDataFlag == 0);
		pDestSystem->m_nPointEmitterDataFlag = rpPRTADVEMITTERDATAFLAGCIRCLE;
		if (pSrcSystem == NULL)
			InitialCircleEmitter(&pDestSystem->m_EmitterPtrCircle);
		else
			memcpy(&pDestSystem->m_EmitterPtrCircle, &pSrcSystem->m_EmitterPtrCircle, sizeof(RpPrtAdvCircleEmitter));
		break;
	case rpPRTADVEMITTERDATAFLAGSPHERE:
		NTL_ASSERTE(pDestSystem->m_nPointEmitterDataFlag == 0);
		pDestSystem->m_nPointEmitterDataFlag = rpPRTADVEMITTERDATAFLAGSPHERE;
		if (pSrcSystem == NULL)
			InitialSphereEmitter(&pDestSystem->m_EmitterPtrSphere);
		else
			memcpy(&pDestSystem->m_EmitterPtrSphere, &pSrcSystem->m_EmitterPtrSphere, sizeof(RpPrtAdvSphereEmitter));
		break;
    
	}
}

//------------------------------------------------------------------
//	FuncName	: SetEmitterDataFlag_MeshSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetEmitterDataFlag_MeshSystem(CNtlResourceMeshSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceMeshSystem* pSrcSystem)
{
	SetEmitterDataFlag_ComponentSystem(pDestSystem, dataFlag, pSrcSystem);

	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtColor(&pDestSystem->m_EmitterPrtCol);
		else
			memcpy(&pDestSystem->m_EmitterPrtCol, &pSrcSystem->m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrt2DRotate(&pDestSystem->m_EmitterPrtRotate);
		else
			memcpy(&pDestSystem->m_EmitterPrtRotate, &pSrcSystem->m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtSize(&pDestSystem->m_EmitterPrtSize);
		else
			memcpy(&pDestSystem->m_EmitterPrtSize, &pSrcSystem->m_EmitterPrtSize, sizeof(SNtlPrtStdEmitterPrtSize));
		break;
    case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if(pSrcSystem)
        {
            memcpy(&pDestSystem->m_EmitterPrtFinish, &pSrcSystem->m_EmitterPrtFinish, sizeof(SNtlPrtStdEmitterPrtFinish));
        }
        break;
    case rpPRTSTDEMITTERDATAFLAGTRAIL:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if(pSrcSystem)
        {
            pDestSystem->m_EmitterTrail = pSrcSystem->m_EmitterTrail;
        }
        break;
	case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiColorEmitter(&pDestSystem->m_EmitterPrtMultiCol);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiCol, &pSrcSystem->m_EmitterPrtMultiCol, sizeof(RpPrtAdvPrtMultiColorEmitter));
			pDestSystem->m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[pDestSystem->m_EmitterPrtMultiCol.numCol];
			memcpy(pDestSystem->m_EmitterPrtMultiCol.list, pSrcSystem->m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem) * pDestSystem->m_EmitterPrtMultiCol.numCol);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGMULTISIZE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiSizeEmitter(&pDestSystem->m_EmitterPrtMultiSize);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiSize, &pSrcSystem->m_EmitterPrtMultiSize, sizeof(SNtlAdvMultiSizeEmitter));
			pDestSystem->m_EmitterPrtMultiSize.list = NTL_NEW SNtlAdvEmtPrtSizeItem[pDestSystem->m_EmitterPrtMultiSize.numSize];
			memcpy(pDestSystem->m_EmitterPrtMultiSize.list, pSrcSystem->m_EmitterPrtMultiSize.list, sizeof(SNtlAdvEmtPrtSizeItem) * pDestSystem->m_EmitterPrtMultiSize.numSize);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiRotateEmitter(&pDestSystem->m_EmitterPrtMultiRotate);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiRotate, &pSrcSystem->m_EmitterPrtMultiRotate, sizeof(SNtlAdvMultiRotateEmitter));
			pDestSystem->m_EmitterPrtMultiRotate.list = NTL_NEW SNtlAdvEmtPrtRotateItem[pDestSystem->m_EmitterPrtMultiRotate.numRotate];
			memcpy(pDestSystem->m_EmitterPrtMultiRotate.list, pSrcSystem->m_EmitterPrtMultiRotate.list, sizeof(SNtlAdvEmtPrtRotateItem) * pDestSystem->m_EmitterPrtMultiRotate.numRotate);
		}
		break;    
	}
}

//------------------------------------------------------------------
//	FuncName	: SetEmitterDataFlag_ImVertexSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::SetEmitterDataFlag_ImVertexSystem(CNtlResourceImVertexSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceImVertexSystem* pSrcSystem)
{
	SetEmitterDataFlag_ComponentSystem(pDestSystem, dataFlag, pSrcSystem);

	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtColor(&pDestSystem->m_EmitterPrtCol);
		else
			memcpy(&pDestSystem->m_EmitterPrtCol, &pSrcSystem->m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));
		break;
	case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialEmitterPrtSize(&pDestSystem->m_EmitterPrtSize);
		else
			memcpy(&pDestSystem->m_EmitterPrtSize, &pSrcSystem->m_EmitterPrtSize, sizeof(SNtlPrtStdEmitterPrtSize));
		break;
    case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if(pSrcSystem)
        {
            memcpy(&pDestSystem->m_EmitterPrtFinish, &pSrcSystem->m_EmitterPrtFinish, sizeof(SNtlPrtStdEmitterPrtFinish));
        }
        break;
	case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiColorEmitter(&pDestSystem->m_EmitterPrtMultiCol);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiCol, &pSrcSystem->m_EmitterPrtMultiCol, sizeof(RpPrtAdvPrtMultiColorEmitter));
			pDestSystem->m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[pDestSystem->m_EmitterPrtMultiCol.numCol];
			memcpy(pDestSystem->m_EmitterPrtMultiCol.list, pSrcSystem->m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem) * pDestSystem->m_EmitterPrtMultiCol.numCol);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGMULTISIZE:
		NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
		pDestSystem->m_nEmitterDataFlag |= dataFlag;
		if (pSrcSystem == NULL)
			InitialMultiSizeEmitter(&pDestSystem->m_EmitterPrtMultiSize);
		else
		{
			memcpy(&pDestSystem->m_EmitterPrtMultiSize, &pSrcSystem->m_EmitterPrtMultiSize, sizeof(SNtlAdvMultiSizeEmitter));
			pDestSystem->m_EmitterPrtMultiSize.list = NTL_NEW SNtlAdvEmtPrtSizeItem[pDestSystem->m_EmitterPrtMultiSize.numSize];
			memcpy(pDestSystem->m_EmitterPrtMultiSize.list, pSrcSystem->m_EmitterPrtMultiSize.list, sizeof(SNtlAdvEmtPrtSizeItem) * pDestSystem->m_EmitterPrtMultiSize.numSize);
		}
		break;
	}
}

void CVenusPropertyContainer::SetEmitterDataFlag_BeamSystem(CNtlResourceBeamSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceBeamSystem* pSrcSystem)
{
	SetEmitterDataFlag_ImVertexSystem(pDestSystem, dataFlag, pSrcSystem);
}

void CVenusPropertyContainer::SetEmitterDataFlag_HurricaneSystem(CNtlResourceHurricaneSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceHurricaneSystem* pSrcSystem)
{
	SetEmitterDataFlag_ImVertexSystem(pDestSystem, dataFlag, pSrcSystem);
}

void CVenusPropertyContainer::SetEmitterDataFlag_LightningSystem(CNtlResourceLightningSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceLightningSystem* pSrcSystem)
{
	SetEmitterDataFlag_ImVertexSystem(pDestSystem, dataFlag, pSrcSystem);
}

void CVenusPropertyContainer::SetEmitterDataFlag_DecalSystem(CNtlResourceDecalSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceDecalSystem* pSrcSystem)
{
	SetEmitterDataFlag_ImVertexSystem(pDestSystem, dataFlag, pSrcSystem);
	switch(dataFlag)
	{
		case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
			NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
			pDestSystem->m_nEmitterDataFlag |= dataFlag;
			if (pSrcSystem == NULL)
				InitialEmitterPrt2DRotate(&pDestSystem->m_EmitterPrtRotate);
			else
				memcpy(&pDestSystem->m_EmitterPrtRotate, &pSrcSystem->m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate));
			break;
		case rpPRTSTDEMITTERDATAFLAGUVANIM:
			NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
			pDestSystem->m_nEmitterDataFlag |= dataFlag;
			if(pSrcSystem)
			{
				pDestSystem->m_EmitterUVAnim = pSrcSystem->m_EmitterUVAnim;
			}
			break;
	}
}

void CVenusPropertyContainer::SetEmitterDataFlag_PostEffectSystem(CNtlResourcePostEffectSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourcePostEffectSystem* pSrcSystem)
{
	SetEmitterDataFlag_ComponentSystem(pDestSystem, dataFlag, pSrcSystem);

    switch(dataFlag)
    {
    case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if (pSrcSystem == NULL)
            InitialEmitterPrtColor(&pDestSystem->m_EmitterPrtCol);
        else
            memcpy(&pDestSystem->m_EmitterPrtCol, &pSrcSystem->m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));
        break;        
    case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if (pSrcSystem == NULL)
            InitialMultiColorEmitter(&pDestSystem->m_EmitterPrtMultiCol);
        else
        {
            memcpy(&pDestSystem->m_EmitterPrtMultiCol, &pSrcSystem->m_EmitterPrtMultiCol, sizeof(RpPrtAdvPrtMultiColorEmitter));
            pDestSystem->m_EmitterPrtMultiCol.list = NTL_NEW RpPrtAdvEmtPrtColorItem[pDestSystem->m_EmitterPrtMultiCol.numCol];
            memcpy(pDestSystem->m_EmitterPrtMultiCol.list, pSrcSystem->m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem) * pDestSystem->m_EmitterPrtMultiCol.numCol);
        }
        break;
    }
}

void CVenusPropertyContainer::SetEmitterDataFlag_LineSystem( CNtlResourceLineSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceLineSystem* pSrcSystem /*= NULL*/ )
{
    SetEmitterDataFlag_ImVertexSystem(pDestSystem, dataFlag, pSrcSystem);

    switch(dataFlag)
    {
    case rpPRTADVEMITTERDATAFLAGSPHERE:
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if(pSrcSystem)
        {
            memcpy(&pDestSystem->m_EmitterSphere, &pSrcSystem->m_EmitterSphere, sizeof(SNtlPrtStdEmitterPrtSphere));
        }
        break;
    case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if (pSrcSystem == NULL)
            InitialEmitterPrt2DRotate(&pDestSystem->m_EmitterPrtRotate);
        else
            memcpy(&pDestSystem->m_EmitterPrtRotate, &pSrcSystem->m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate));
        break;
    case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
        NTL_ASSERTE(!pDestSystem->IsEmitterDataFlag(dataFlag));
        pDestSystem->m_nEmitterDataFlag |= dataFlag;
        if (pSrcSystem == NULL)
            InitialMultiRotateEmitter(&pDestSystem->m_EmitterPrtMultiRotate);
        else
        {
            memcpy(&pDestSystem->m_EmitterPrtMultiRotate, &pSrcSystem->m_EmitterPrtMultiRotate, sizeof(SNtlAdvMultiRotateEmitter));
            pDestSystem->m_EmitterPrtMultiRotate.list = NTL_NEW SNtlAdvEmtPrtRotateItem[pDestSystem->m_EmitterPrtMultiRotate.numRotate];
            memcpy(pDestSystem->m_EmitterPrtMultiRotate.list, pSrcSystem->m_EmitterPrtMultiRotate.list, sizeof(SNtlAdvEmtPrtRotateItem) * pDestSystem->m_EmitterPrtMultiRotate.numRotate);
        }
        break;
    }
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag(CNtlResourceComponentSystem* pResourceComponentSystem, RwUInt32 dataFlag)
{
	m_bSave = TRUE;

	switch(pResourceComponentSystem->GetSystemType())
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		DeleteEmitterDataFlag_ParticleSystem((CNtlResourceParticleSystem*)pResourceComponentSystem, dataFlag);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		DeleteEmitterDataFlag_MeshSystem((CNtlResourceMeshSystem*)pResourceComponentSystem, dataFlag);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		DeleteEmitterDataFlag_BeamSystem((CNtlResourceBeamSystem*)pResourceComponentSystem, dataFlag);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		DeleteEmitterDataFlag_HurricaneSystem((CNtlResourceHurricaneSystem*)pResourceComponentSystem, dataFlag);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		DeleteEmitterDataFlag_LightningSystem((CNtlResourceLightningSystem*)pResourceComponentSystem, dataFlag);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		DeleteEmitterDataFlag_DecalSystem((CNtlResourceDecalSystem*)pResourceComponentSystem, dataFlag);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		DeleteEmitterDataFlag_PostEffectSystem((CNtlResourcePostEffectSystem*)pResourceComponentSystem, dataFlag);
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        DeleteEmitterDataFlag_LineSystem((CNtlResourceLineSystem*)pResourceComponentSystem, dataFlag);
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_ParticleSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_ParticleSystem(CNtlResourceParticleSystem* pResourceParticleSystem, RwUInt32 dataFlag)
{
	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
	case rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS:
	case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
	case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
	case rpPRTSTDEMITTERDATAFLAGPRTMATRIX:
	case rpPRTSTDEMITTERDATAFLAGPRTHURRICANE:
	case rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT:
	case rpPRTSTDEMITTERDATAFLAGPRTFADEOUT:
		NTL_ASSERTE(pResourceParticleSystem->IsEmitterDataFlag(dataFlag));
		pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;

	case rpPRTADVEMITTERDATAFLAGPRTEMITTER:
		NTL_ASSERTE(pResourceParticleSystem->IsEmitterDataFlag(dataFlag));
		pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
		pResourceParticleSystem->m_strSubSystem = "";
		break;

	case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
		NTL_ASSERTE(pResourceParticleSystem->IsEmitterDataFlag(dataFlag));
		pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceParticleSystem->m_EmitterPrtMultiCol.list);
		pResourceParticleSystem->m_EmitterPrtMultiCol.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS:
		NTL_ASSERTE(pResourceParticleSystem->IsEmitterDataFlag(dataFlag));
		pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceParticleSystem->m_EmitterPrtMultiTexCoords.list);
		pResourceParticleSystem->m_EmitterPrtMultiTexCoords.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTISIZE:
		NTL_ASSERTE(pResourceParticleSystem->IsEmitterDataFlag(dataFlag));
		pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceParticleSystem->m_EmitterPrtMultiSize.list);
		pResourceParticleSystem->m_EmitterPrtMultiSize.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
		NTL_ASSERTE(pResourceParticleSystem->IsEmitterDataFlag(dataFlag));
		NTL_ARRAY_DELETE(pResourceParticleSystem->m_EmitterMultiTextures.list);
		pResourceParticleSystem->m_EmitterMultiTextures.list = NULL;
		pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;
    case rpPRTSTDEMITTERDATAFLAGTRAIL:
        pResourceParticleSystem->m_nEmitterDataFlag &= ~dataFlag;
        break;
	case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
		NTL_ASSERTE(pResourceParticleSystem->m_nPathEmitterDataFlag != 0);
		pResourceParticleSystem->m_nPathEmitterDataFlag = 0;
		break;

	case rpPRTADVEMITTERDATAFLAGPOINTLIST:
		NTL_ASSERTE(pResourceParticleSystem->m_nPointEmitterDataFlag != 0);
		NTL_ARRAY_DELETE(pResourceParticleSystem->m_EmitterPtList.pointList);
		pResourceParticleSystem->m_EmitterPtList.pointList = NULL;
		pResourceParticleSystem->m_nPointEmitterDataFlag = 0;
		break;
	case rpPRTADVEMITTERDATAFLAGCIRCLE:
		NTL_ASSERTE(pResourceParticleSystem->m_nPointEmitterDataFlag != 0);
		pResourceParticleSystem->m_nPointEmitterDataFlag = 0;
		break;
	case rpPRTADVEMITTERDATAFLAGSPHERE:
		NTL_ASSERTE(pResourceParticleSystem->m_nPointEmitterDataFlag != 0);
		pResourceParticleSystem->m_nPointEmitterDataFlag = 0;
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_MeshSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_MeshSystem(CNtlResourceMeshSystem* pResourceMeshSystem, RwUInt32 dataFlag)
{
	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
	case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
	case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
		NTL_ASSERTE(pResourceMeshSystem->IsEmitterDataFlag(dataFlag));
		pResourceMeshSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
		NTL_ASSERTE(pResourceMeshSystem->IsEmitterDataFlag(dataFlag));
		pResourceMeshSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceMeshSystem->m_EmitterPrtMultiCol.list);
		pResourceMeshSystem->m_EmitterPrtMultiCol.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTISIZE:
		NTL_ASSERTE(pResourceMeshSystem->IsEmitterDataFlag(dataFlag));
		pResourceMeshSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceMeshSystem->m_EmitterPrtMultiSize.list);
		pResourceMeshSystem->m_EmitterPrtMultiSize.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
		NTL_ASSERTE(pResourceMeshSystem->IsEmitterDataFlag(dataFlag));
		pResourceMeshSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceMeshSystem->m_EmitterPrtMultiRotate.list);
		pResourceMeshSystem->m_EmitterPrtMultiRotate.list = NULL;
		break;

	case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
		NTL_ASSERTE(pResourceMeshSystem->m_nPathEmitterDataFlag != 0);
		pResourceMeshSystem->m_nPathEmitterDataFlag = 0;
		break;
    case rpPRTSTDEMITTERDATAFLAGTRAIL:
        pResourceMeshSystem->m_nEmitterDataFlag &= ~dataFlag;
        break;
	case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
		pResourceMeshSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_BeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_ImVertexSystem(CNtlResourceImVertexSystem* pResourceImVertexSystem, RwUInt32 dataFlag)
{
	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
	case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
	case rpPRTSTDEMITTERDATAFLAGPRTFADEOUT:
	case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
		NTL_ASSERTE(pResourceImVertexSystem->IsEmitterDataFlag(dataFlag));
		pResourceImVertexSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
		NTL_ASSERTE(pResourceImVertexSystem->IsEmitterDataFlag(dataFlag));
		pResourceImVertexSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceImVertexSystem->m_EmitterPrtMultiCol.list);
		pResourceImVertexSystem->m_EmitterPrtMultiCol.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTISIZE:
		NTL_ASSERTE(pResourceImVertexSystem->IsEmitterDataFlag(dataFlag));
		pResourceImVertexSystem->m_nEmitterDataFlag &= ~dataFlag;
		NTL_ARRAY_DELETE(pResourceImVertexSystem->m_EmitterPrtMultiSize.list);
		pResourceImVertexSystem->m_EmitterPrtMultiSize.list = NULL;
		break;
	case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
		NTL_ASSERTE(pResourceImVertexSystem->IsEmitterDataFlag(dataFlag));
		NTL_ARRAY_DELETE(pResourceImVertexSystem->m_EmitterMultiTextures.list);
		pResourceImVertexSystem->m_EmitterMultiTextures.list = NULL;
		pResourceImVertexSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_BeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_BeamSystem(CNtlResourceBeamSystem* pResourceBeamSystem, RwUInt32 dataFlag)
{
	DeleteEmitterDataFlag_ImVertexSystem(pResourceBeamSystem, dataFlag);
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_HurricaneSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_HurricaneSystem(CNtlResourceHurricaneSystem* pResourceHurricaneSystem, RwUInt32 dataFlag)
{
	DeleteEmitterDataFlag_ImVertexSystem(pResourceHurricaneSystem, dataFlag);
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_HurricaneSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_LightningSystem(CNtlResourceLightningSystem* pResourceLightningSystem, RwUInt32 dataFlag)
{
	DeleteEmitterDataFlag_ImVertexSystem(pResourceLightningSystem, dataFlag);
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_HurricaneSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_DecalSystem(CNtlResourceDecalSystem* pResourceDecalSystem, RwUInt32 dataFlag)
{
	DeleteEmitterDataFlag_ImVertexSystem(pResourceDecalSystem, dataFlag);
	switch(dataFlag)
	{
	case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
	case rpPRTSTDEMITTERDATAFLAGUVANIM:
		NTL_ASSERTE(pResourceDecalSystem->IsEmitterDataFlag(dataFlag));
		pResourceDecalSystem->m_nEmitterDataFlag &= ~dataFlag;
		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEmitterDataFlag_HurricaneSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::DeleteEmitterDataFlag_PostEffectSystem(CNtlResourcePostEffectSystem* pResourcePostEffectSystem, RwUInt32 dataFlag)
{
    switch(dataFlag)
    {
    case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
        NTL_ASSERTE(pResourcePostEffectSystem->IsEmitterDataFlag(dataFlag));
        pResourcePostEffectSystem->m_nEmitterDataFlag &= ~dataFlag;
        break;
    case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
        NTL_ASSERTE(pResourcePostEffectSystem->IsEmitterDataFlag(dataFlag));
        pResourcePostEffectSystem->m_nEmitterDataFlag &= ~dataFlag;
        NTL_ARRAY_DELETE(pResourcePostEffectSystem->m_EmitterPrtMultiCol.list);
        pResourcePostEffectSystem->m_EmitterPrtMultiCol.list = NULL;
        break;    
    case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
        NTL_ASSERTE(pResourcePostEffectSystem->IsEmitterDataFlag(dataFlag));
        NTL_ARRAY_DELETE(pResourcePostEffectSystem->m_EmitterMultiTextures.list);
        pResourcePostEffectSystem->m_EmitterMultiTextures.list = NULL;
        pResourcePostEffectSystem->m_nEmitterDataFlag &= ~dataFlag;
        break;
    }
}


void CVenusPropertyContainer::DeleteEmitterDataFlag_LineSystem( CNtlResourceLineSystem* pResourceLineSystem, RwUInt32 dataFlag )
{
    DeleteEmitterDataFlag_ImVertexSystem(pResourceLineSystem, dataFlag);
    
    switch(dataFlag)
    {
    case rpPRTADVEMITTERDATAFLAGSPHERE:
    case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
        pResourceLineSystem->m_nEmitterDataFlag &= ~dataFlag;
        break;
    case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
        NTL_ASSERTE(pResourceLineSystem->IsEmitterDataFlag(dataFlag));
        pResourceLineSystem->m_nEmitterDataFlag &= ~dataFlag;
        NTL_ARRAY_DELETE(pResourceLineSystem->m_EmitterPrtMultiRotate.list);
        pResourceLineSystem->m_EmitterPrtMultiRotate.list = NULL;
        break;
    }
}


//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(RpPrtStdEmitterStandard *pEmitterStandard)
{
	memset(pEmitterStandard, 0, sizeof(RpPrtStdEmitterStandard));
    /* Set the emitter's maximum particles */
    pEmitterStandard->maxPrt = 1000;

	pEmitterStandard->prtDelta2DRotate = 0.f;

    /* Set the emitter's emission volume */
    pEmitterStandard->emtSize.x = 0.0f;
    pEmitterStandard->emtSize.y = 0.0f;
    pEmitterStandard->emtSize.z = 0.0f;

    /* Set the default particle's size */
    pEmitterStandard->prtSize.x = 0.100000f;
    pEmitterStandard->prtSize.y = 0.100000f;

    /* Set the particle emission gap : should not be bigger
     *  than the batch size setted during the creation code
     */
    pEmitterStandard->emtPrtEmit = 1;
    pEmitterStandard->emtPrtEmitBias = 0;
    pEmitterStandard->emtEmitGap = 0.2f;
    pEmitterStandard->emtEmitGapBias = 0.f;

    /* Set the particle's life span */
    pEmitterStandard->prtLife = 5.000000f;
    pEmitterStandard->prtLifeBias = 0.000000f;

    /* Set the particles emission speed */
    pEmitterStandard->prtInitVel = 1.0f;
    pEmitterStandard->prtInitVelBias = 0.000000f;

    /* Set the particles emission Direction */
    pEmitterStandard->prtInitDir.x = 0.000000f;
    pEmitterStandard->prtInitDir.y = 1.000000f;
    pEmitterStandard->prtInitDir.z = 0.000000f;
    pEmitterStandard->prtInitDirBias.x = 0.000000f;
    pEmitterStandard->prtInitDirBias.y = 0.000000f;
    pEmitterStandard->prtInitDirBias.z = 0.000000f;

    /* Set the force emission Direction */
    pEmitterStandard->force.x = 0.000000f;
    pEmitterStandard->force.y = 0.000000f;
    pEmitterStandard->force.z = 0.000000f;

    /* Set the default Color */
    pEmitterStandard->prtColor.red = 255;
    pEmitterStandard->prtColor.green = 255;
    pEmitterStandard->prtColor.blue = 255;
    pEmitterStandard->prtColor.alpha = 255;

    /* Set the default Texture coordinate */
    pEmitterStandard->prtUV[0].u = 0.000000f;
    pEmitterStandard->prtUV[0].v = 0.000000f;
    pEmitterStandard->prtUV[1].u = 1.000000f;
    pEmitterStandard->prtUV[1].v = 1.000000f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(SNtlMeshEmitterStandard *pEmitterStandard)
{
	pEmitterStandard->bWriteEnable = FALSE;

	pEmitterStandard->prtColor.red = pEmitterStandard->prtColor.green = pEmitterStandard->prtColor.blue = pEmitterStandard->prtColor.alpha = 255;

	pEmitterStandard->prtSize.x = pEmitterStandard->prtSize.y = pEmitterStandard->prtSize.z = 1.f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(SNtlBeamEmitterStandard *pEmitterStandard)
{
	memset(pEmitterStandard, 0, sizeof(SNtlBeamEmitterStandard));

	pEmitterStandard->fSpeed = 10.f;

	pEmitterStandard->prtColor.red = pEmitterStandard->prtColor.green = pEmitterStandard->prtColor.blue = pEmitterStandard->prtColor.alpha = 255;

	pEmitterStandard->prtSize.x = pEmitterStandard->prtSize.y = pEmitterStandard->prtSize.z = 0.2f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(SNtlHurricaneEmitterStandard *pEmitterStandard)
{
	memset(pEmitterStandard, 0, sizeof(SNtlHurricaneEmitterStandard));

	pEmitterStandard->fSpeed = 10.f;

	pEmitterStandard->prtColor.red = pEmitterStandard->prtColor.green = pEmitterStandard->prtColor.blue = pEmitterStandard->prtColor.alpha = 255;

	pEmitterStandard->prtSize.x = pEmitterStandard->prtSize.y = pEmitterStandard->prtSize.z = 0.3f;

	pEmitterStandard->bEternity			= TRUE;
	pEmitterStandard->lifeTime			= 10.f;
	pEmitterStandard->maxPolygonCount	= 10000;
	pEmitterStandard->radius			= 0.3f;
	pEmitterStandard->radiusGap			= 0.4f;
	pEmitterStandard->revolutionAngle	= 10.f;

	pEmitterStandard->revolutionCount	= 5;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(SNtlLightningEmitterStandard *pEmitterStandard)
{
	memset(pEmitterStandard, 0, sizeof(SNtlLightningEmitterStandard));

	pEmitterStandard->prtSize.x = 0.2f;
	pEmitterStandard->prtSize.y = 0.2f;
	pEmitterStandard->prtSize.z = 0.2f;

	pEmitterStandard->prtColor.red = pEmitterStandard->prtColor.green = pEmitterStandard->prtColor.blue = pEmitterStandard->prtColor.alpha = 255;

	pEmitterStandard->fAmp = 1.f;
	pEmitterStandard->fRough = 1.f;

	pEmitterStandard->fUpdateTime = 0.1f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(SNtlDecalEmitterStandard *pEmitterStandard)
{
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterStandard(SNtlPostEffectEmitterStandard *pEmitterStandard)
{
	
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPointList
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPointList(RpPrtAdvPointListEmitter* pPointListEmitter)
{
	NTL_ASSERTE(pPointListEmitter->pointList == NULL);
	memset(pPointListEmitter, 0, sizeof(RpPrtAdvPointListEmitter));
    pPointListEmitter->useDirection		= TRUE;
    pPointListEmitter->random			= TRUE;
    pPointListEmitter->numPoint			= 2;

	pPointListEmitter->pointList = NTL_NEW RwV3d[pPointListEmitter->numPoint];
	memset(pPointListEmitter->pointList, 0, sizeof(RwV3d) * pPointListEmitter->numPoint);

    pPointListEmitter->pointList[0].x= -1.000000f;
    pPointListEmitter->pointList[0].y= 0.000000f;
    pPointListEmitter->pointList[0].z= 0.000000f;

    pPointListEmitter->pointList[1].x= 1.000000f;
    pPointListEmitter->pointList[1].y= 0.000000f;
    pPointListEmitter->pointList[1].z= 0.000000f;

	pPointListEmitter->directionList = NTL_NEW RwV3d[pPointListEmitter->numPoint];
	memset(pPointListEmitter->directionList, 0, sizeof(RwV3d) * pPointListEmitter->numPoint);

    pPointListEmitter->directionList[0].x= 0.000000f;
    pPointListEmitter->directionList[0].y= 1.000000f;
    pPointListEmitter->directionList[0].z= 0.000000f;

    pPointListEmitter->directionList[1].x= 0.000000f;
    pPointListEmitter->directionList[1].y= -1.000000f;
    pPointListEmitter->directionList[1].z= 0.000000f;
}

//------------------------------------------------------------------
//	FuncName	: InitialCircleEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialCircleEmitter(RpPrtAdvCircleEmitter* pCircleEmitter)
{
	memset(pCircleEmitter, 0, sizeof(RpPrtAdvCircleEmitter));

    pCircleEmitter->radius				= 1.0f;
    pCircleEmitter->radiusGap			= 0.0f;
    pCircleEmitter->height				= 0.0f;
    pCircleEmitter->useCircleEmission	= TRUE;
    pCircleEmitter->dirRotation			= 0.45f;
}

//------------------------------------------------------------------
//	FuncName	: InitialCircleEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialSphereEmitter(RpPrtAdvSphereEmitter* pSphereEmitter)
{
	memset(pSphereEmitter, 0, sizeof(RpPrtAdvSphereEmitter));
    pSphereEmitter->radius				= 1.f;
    pSphereEmitter->radiusGap			= 0.0f;
    pSphereEmitter->useSphereEmission	= TRUE;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtColor(RpPrtStdEmitterPrtColor* pEmitterPrtColor)
{
	memset(pEmitterPrtColor, 0, sizeof(RpPrtStdEmitterPrtColor));
    pEmitterPrtColor->prtStartCol.red = 255.000000f;
    pEmitterPrtColor->prtStartCol.green = 0.000000f;
    pEmitterPrtColor->prtStartCol.blue = 0.000000f;
    pEmitterPrtColor->prtStartCol.alpha = 255.000000f;

	pEmitterPrtColor->prtEndCol.red = 0.000000f;
    pEmitterPrtColor->prtEndCol.green = 0.000000f;
    pEmitterPrtColor->prtEndCol.blue = 255.000000f;
    pEmitterPrtColor->prtEndCol.alpha = 0.000000f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtTexCoords
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtTexCoords(RpPrtStdEmitterPrtTexCoords* pEmitterPrtTexCoords)
{
	memset(pEmitterPrtTexCoords, 0, sizeof(RpPrtStdEmitterPrtTexCoords));
	pEmitterPrtTexCoords->prtStartUV0.u = 0.f;
	pEmitterPrtTexCoords->prtStartUV0.v = 0.f;
	pEmitterPrtTexCoords->prtStartUV0Bias.u = 0.f;
	pEmitterPrtTexCoords->prtStartUV0Bias.v = 0.f;

	pEmitterPrtTexCoords->prtStartUV1.u = 0.5f;
	pEmitterPrtTexCoords->prtStartUV1.v = 1.0f;
	pEmitterPrtTexCoords->prtStartUV1Bias.u = 0.f;
	pEmitterPrtTexCoords->prtStartUV1Bias.v = 0.f;

	pEmitterPrtTexCoords->prtEndUV0.u = 0.5f;
	pEmitterPrtTexCoords->prtEndUV0.v = 0.f;
	pEmitterPrtTexCoords->prtEndUV0Bias.u = 0.f;
	pEmitterPrtTexCoords->prtEndUV0Bias.v = 0.f;

	pEmitterPrtTexCoords->prtEndUV1.u = 1.f;
	pEmitterPrtTexCoords->prtEndUV1.v = 1.f;
	pEmitterPrtTexCoords->prtEndUV1Bias.u = 0.f;
	pEmitterPrtTexCoords->prtEndUV1Bias.v = 0.f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrt2DRotate
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrt2DRotate(RpPrtStdEmitterPrt2DRotate* pEmitterPrt2DRotate)
{
	memset(pEmitterPrt2DRotate, 0, sizeof(RpPrtStdEmitterPrt2DRotate));
	pEmitterPrt2DRotate->prtStart2DRotate = 0.f;
	pEmitterPrt2DRotate->prtStart2DRotateBias = 0.f;

	pEmitterPrt2DRotate->prtEnd2DRotate = 3.14f;
	pEmitterPrt2DRotate->prtEnd2DRotateBias = 0.f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrt2DRotate
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrt2DRotate(SNtlPrtStdEmitterPrtRotate* pEmitterPrt2DRotate)
{
	memset(pEmitterPrt2DRotate, 0, sizeof(SNtlPrtStdEmitterPrtRotate));
	pEmitterPrt2DRotate->prtStartRotate.x = 0.f;
	pEmitterPrt2DRotate->prtStartRotate.y = 0.f;
	pEmitterPrt2DRotate->prtStartRotate.z = 0.f;

	pEmitterPrt2DRotate->prtEndRotate.x = 0.f;
	pEmitterPrt2DRotate->prtEndRotate.y = 360.0f;
	pEmitterPrt2DRotate->prtEndRotate.z = 0.f;
}


//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtSize(RpPrtStdEmitterPrtSize* pEmitterPrtSize)
{
	memset(pEmitterPrtSize, 0, sizeof(RpPrtStdEmitterPrtSize));
	pEmitterPrtSize->prtStartSize.x = 0.1f;
	pEmitterPrtSize->prtStartSize.y = 0.1f;

	pEmitterPrtSize->prtEndSize.x = 0.5f;
	pEmitterPrtSize->prtEndSize.y = 0.5f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtSize(SNtlPrtStdEmitterPrtSize* pEmitterPrtSize)
{
	memset(pEmitterPrtSize, 0, sizeof(SNtlPrtStdEmitterPrtSize));
	pEmitterPrtSize->prtStartSize.x = 1.0f;
	pEmitterPrtSize->prtStartSize.y = 1.0f;
	pEmitterPrtSize->prtStartSize.z = 1.0f;

	pEmitterPrtSize->prtEndSize.x = 0.2f;
	pEmitterPrtSize->prtEndSize.y = 0.2f;
	pEmitterPrtSize->prtEndSize.z = 0.2f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtMatrix(RpPrtStdEmitterPrtMatrix* pEmitterPrtMatrix)
{
	memset(pEmitterPrtMatrix, 0, sizeof(RpPrtStdEmitterPrtMatrix));
	RwMatrixSetIdentity(&pEmitterPrtMatrix->prtCnsMtx);

	pEmitterPrtMatrix->prtPosMtxAt.x = 0.f;
	pEmitterPrtMatrix->prtPosMtxAt.y = 0.f;
	pEmitterPrtMatrix->prtPosMtxAt.z = -1.f;

	pEmitterPrtMatrix->prtPosMtxUp.x = 0.f;
	pEmitterPrtMatrix->prtPosMtxUp.y = 1.f;
	pEmitterPrtMatrix->prtPosMtxUp.z = 0.f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtHurricane
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtHurricane(SNtlPrtStdEmitterPrtHurricane* pEmitterPrtHurricane)
{
	memset(pEmitterPrtHurricane, 0, sizeof(SNtlPrtStdEmitterPrtHurricane));

	pEmitterPrtHurricane->fSpeed = 10.f;
	pEmitterPrtHurricane->radius = 0.2f;
	pEmitterPrtHurricane->revolutionCount = 5.f;
	pEmitterPrtHurricane->revolutionAngle = 2.f;

}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtOrbitPoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtOrbitPoint(SNtlPrtStdEmitterPrtOrbitPoint* pEmitterPrtOrbitPoint)
{
	memset(pEmitterPrtOrbitPoint, 0, sizeof(SNtlPrtStdEmitterPrtOrbitPoint));
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterPrtFadeOut
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterPrtFadeOut(SNtlPrtStdEmitterPrtFadeOut* pEmitterPrtFadeOut)
{
	memset(pEmitterPrtFadeOut, 0, sizeof(SNtlPrtStdEmitterPrtFadeOut));

	pEmitterPrtFadeOut->time = 1.f;
	pEmitterPrtFadeOut->follow = TRUE;
}

//------------------------------------------------------------------
//	FuncName	: InitialPrtEmitterEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialPrtEmitterEmitter(RpPrtAdvPrtEmitterEmitter* pPrtEmitterEmitter)
{
	memset(pPrtEmitterEmitter, 0, sizeof(RpPrtAdvPrtEmitterEmitter));

    pPrtEmitterEmitter->time = 1.0f;
    pPrtEmitterEmitter->timeBias = 0.0f;
    pPrtEmitterEmitter->timeGap = 1.0f;
    pPrtEmitterEmitter->timeGapBias = 0.0;
}

//------------------------------------------------------------------
//	FuncName	: InitialMultiColorEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialMultiColorEmitter(RpPrtAdvPrtMultiColorEmitter* pMultiColorEmitter)
{
	NTL_ASSERTE(pMultiColorEmitter->list == NULL);
	memset(pMultiColorEmitter, 0, sizeof(RpPrtAdvPrtMultiColorEmitter));
	pMultiColorEmitter->numCol = 2;

	pMultiColorEmitter->list = NTL_NEW RpPrtAdvEmtPrtColorItem[pMultiColorEmitter->numCol];
	memset(pMultiColorEmitter->list, 0, sizeof(RpPrtAdvEmtPrtColorItem) * pMultiColorEmitter->numCol);

    pMultiColorEmitter->list[0].time = 0.100000f;
    pMultiColorEmitter->list[0].timeBias = 0.000000f;
    pMultiColorEmitter->list[0].midCol.red = 255.000000f;
    pMultiColorEmitter->list[0].midCol.green = 200.000000f;
    pMultiColorEmitter->list[0].midCol.blue = 200.000000f;
    pMultiColorEmitter->list[0].midCol.alpha = 255.000000f;
    pMultiColorEmitter->list[0].midColBias.red = 0.000000f;
    pMultiColorEmitter->list[0].midColBias.green = 0.000000f;
    pMultiColorEmitter->list[0].midColBias.blue = 0.000000f;
    pMultiColorEmitter->list[0].midColBias.alpha = 0.000000f;

    pMultiColorEmitter->list[1].time = 0.200000f;
    pMultiColorEmitter->list[1].timeBias = 0.000000f;
    pMultiColorEmitter->list[1].midCol.red = 0.000000f;
    pMultiColorEmitter->list[1].midCol.green = 200.000000f;
    pMultiColorEmitter->list[1].midCol.blue = 200.000000f;
    pMultiColorEmitter->list[1].midCol.alpha = 255.000000f;
    pMultiColorEmitter->list[1].midColBias.red = 0.000000f;
    pMultiColorEmitter->list[1].midColBias.green = 0.000000f;
    pMultiColorEmitter->list[1].midColBias.blue = 0.000000f;
    pMultiColorEmitter->list[1].midColBias.alpha = 0.000000f;
}

//------------------------------------------------------------------
//	FuncName	: InitialMultiSizeEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialMultiSizeEmitter(RpPrtAdvPrtMultiSizeEmitter* pMultiSizeEmitter)
{
	NTL_ASSERTE(pMultiSizeEmitter->list == NULL);
	memset(pMultiSizeEmitter, 0, sizeof(RpPrtAdvPrtMultiSizeEmitter));
	pMultiSizeEmitter->numSize = 2;

	pMultiSizeEmitter->list = NTL_NEW RpPrtAdvEmtPrtSizeItem[pMultiSizeEmitter->numSize];
	memset(pMultiSizeEmitter->list, 0, sizeof(RpPrtAdvEmtPrtSizeItem) * pMultiSizeEmitter->numSize);

	pMultiSizeEmitter->list[0].time = 0.1f;
	pMultiSizeEmitter->list[0].timeBias = 0.1f;
	pMultiSizeEmitter->list[0].midSize.x = 0.3f;
	pMultiSizeEmitter->list[0].midSize.y = 0.3f;
	pMultiSizeEmitter->list[0].midSizeBias.x = 0.0f;
	pMultiSizeEmitter->list[0].midSizeBias.y = 0.0f;

	pMultiSizeEmitter->list[1].time = 0.2f;
	pMultiSizeEmitter->list[1].timeBias = 0.1f;
	pMultiSizeEmitter->list[1].midSize.x = 0.6f;
	pMultiSizeEmitter->list[1].midSize.y = 0.6f;
	pMultiSizeEmitter->list[1].midSizeBias.x = 0.0f;
	pMultiSizeEmitter->list[1].midSizeBias.y = 0.0f;
}

//------------------------------------------------------------------
//	FuncName	: InitialMultiSizeEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialMultiSizeEmitter(SNtlAdvMultiSizeEmitter* pMultiSizeEmitter)
{
	NTL_ASSERTE(pMultiSizeEmitter->list == NULL);
	memset(pMultiSizeEmitter, 0, sizeof(SNtlAdvMultiSizeEmitter));
	pMultiSizeEmitter->numSize = 2;

	pMultiSizeEmitter->list = NTL_NEW SNtlAdvEmtPrtSizeItem[pMultiSizeEmitter->numSize];
	memset(pMultiSizeEmitter->list, 0, sizeof(SNtlAdvEmtPrtSizeItem) * pMultiSizeEmitter->numSize);

	pMultiSizeEmitter->list[0].time = 0.1f;
	pMultiSizeEmitter->list[0].timeBias = 0.1f;
	pMultiSizeEmitter->list[0].midSize.x = 0.3f;
	pMultiSizeEmitter->list[0].midSize.y = 0.3f;
	pMultiSizeEmitter->list[0].midSize.z = 0.3f;
	pMultiSizeEmitter->list[0].midSizeBias.x = 0.0f;
	pMultiSizeEmitter->list[0].midSizeBias.y = 0.0f;
	pMultiSizeEmitter->list[0].midSizeBias.z = 0.0f;

	pMultiSizeEmitter->list[1].time = 0.2f;
	pMultiSizeEmitter->list[1].timeBias = 0.1f;
	pMultiSizeEmitter->list[1].midSize.x = 0.6f;
	pMultiSizeEmitter->list[1].midSize.y = 0.6f;
	pMultiSizeEmitter->list[1].midSize.z = 0.6f;
	pMultiSizeEmitter->list[1].midSizeBias.x = 0.0f;
	pMultiSizeEmitter->list[1].midSizeBias.y = 0.0f;
	pMultiSizeEmitter->list[1].midSizeBias.z = 0.0f;
}

//------------------------------------------------------------------
//	FuncName	: InitialMultiSizeEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialMultiRotateEmitter(SNtlAdvMultiRotateEmitter* pMultiRotateEmitter)
{
	NTL_ASSERTE(pMultiRotateEmitter->list == NULL);
	memset(pMultiRotateEmitter, 0, sizeof(SNtlAdvMultiRotateEmitter));
	pMultiRotateEmitter->numRotate = 2;

	pMultiRotateEmitter->list = NTL_NEW SNtlAdvEmtPrtRotateItem[pMultiRotateEmitter->numRotate];
	memset(pMultiRotateEmitter->list, 0, sizeof(SNtlAdvEmtPrtRotateItem) * pMultiRotateEmitter->numRotate);

	pMultiRotateEmitter->list[0].time = 0.3f;
	pMultiRotateEmitter->list[0].timeBias = 0.0f;
	pMultiRotateEmitter->list[0].midRotate.x = 0.1f;
	pMultiRotateEmitter->list[0].midRotate.y = 0.1f;
	pMultiRotateEmitter->list[0].midRotate.z = 0.1f;
	pMultiRotateEmitter->list[0].midRotateBias.x = 0.0f;
	pMultiRotateEmitter->list[0].midRotateBias.y = 0.0f;
	pMultiRotateEmitter->list[0].midRotateBias.z = 0.0f;

	pMultiRotateEmitter->list[1].time = 0.6f;
	pMultiRotateEmitter->list[1].timeBias = 0.0f;
	pMultiRotateEmitter->list[1].midRotate.x = 0.2f;
	pMultiRotateEmitter->list[1].midRotate.y = 0.2f;
	pMultiRotateEmitter->list[1].midRotate.z = 0.2f;
	pMultiRotateEmitter->list[1].midRotateBias.x = 0.0f;
	pMultiRotateEmitter->list[1].midRotateBias.y = 0.0f;
	pMultiRotateEmitter->list[1].midRotateBias.z = 0.0f;
}

//------------------------------------------------------------------
//	FuncName	: InitialMultiTexCoordsEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialMultiTexCoordsEmitter(RpPrtAdvEmtPrtMultiTexCoords* pMultiTexCoordsEmitter)
{
	NTL_ASSERTE(pMultiTexCoordsEmitter->list == NULL);
	memset(pMultiTexCoordsEmitter, 0, sizeof(RpPrtAdvEmtPrtMultiTexCoords));
	pMultiTexCoordsEmitter->numTex = 4;

	pMultiTexCoordsEmitter->list = NTL_NEW RpPrtAdvEmtPrtTexCoordsItem[pMultiTexCoordsEmitter->numTex];
	memset(pMultiTexCoordsEmitter->list, 0, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * pMultiTexCoordsEmitter->numTex);

	pMultiTexCoordsEmitter->list[0].time = 0.1f;
	pMultiTexCoordsEmitter->list[0].timeBias = 0.0f;
	pMultiTexCoordsEmitter->list[0].midUV0.u = 0.0f;
	pMultiTexCoordsEmitter->list[0].midUV0.v = 0.0f;
	pMultiTexCoordsEmitter->list[0].midUV1.u = 0.5f;
	pMultiTexCoordsEmitter->list[0].midUV1.v = 0.5f;

	pMultiTexCoordsEmitter->list[1].time = 0.2f;
	pMultiTexCoordsEmitter->list[1].timeBias = 0.0f;
	pMultiTexCoordsEmitter->list[1].midUV0.u = 0.5f;
	pMultiTexCoordsEmitter->list[1].midUV0.v = 0.0f;
	pMultiTexCoordsEmitter->list[1].midUV1.u = 1.0f;
	pMultiTexCoordsEmitter->list[1].midUV1.v = 0.5f;

	pMultiTexCoordsEmitter->list[2].time = 0.3f;
	pMultiTexCoordsEmitter->list[2].timeBias = 0.0f;
	pMultiTexCoordsEmitter->list[2].midUV0.u = 0.0f;
	pMultiTexCoordsEmitter->list[2].midUV0.v = 0.5f;
	pMultiTexCoordsEmitter->list[2].midUV1.u = 0.5f;
	pMultiTexCoordsEmitter->list[2].midUV1.v = 1.0f;

	pMultiTexCoordsEmitter->list[3].time = 0.4f;
	pMultiTexCoordsEmitter->list[3].timeBias = 0.0f;
	pMultiTexCoordsEmitter->list[3].midUV0.u = 0.5f;
	pMultiTexCoordsEmitter->list[3].midUV0.v = 0.5f;
	pMultiTexCoordsEmitter->list[3].midUV1.u = 1.0f;
	pMultiTexCoordsEmitter->list[3].midUV1.v = 1.0f;
}

//------------------------------------------------------------------
//	FuncName	: InitialMultiTexturesEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialMultiTexturesEmitter(RpPrtAdvEmtPrtMultiTextures* pMultiTexturesEmitter)
{
	NTL_ASSERTE(pMultiTexturesEmitter->list == NULL);
	memset(pMultiTexturesEmitter, 0, sizeof(RpPrtAdvEmtPrtMultiTextures));

	pMultiTexturesEmitter->time = 0.f;
	pMultiTexturesEmitter->numTextures = 3;
	pMultiTexturesEmitter->bLoop = TRUE;

	pMultiTexturesEmitter->list = NTL_NEW RpPrtAdvEmtPrtTextureItem[pMultiTexturesEmitter->numTextures];
	// std::string 가 있어서 memset 는 할수 없다
//	memset(pMultiTexturesEmitter->list, 0, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * pMultiTexCoordsEmitter->numTex);

	pMultiTexturesEmitter->list[0].time = 1.0f;
	pMultiTexturesEmitter->list[0].timeBias = 0.0f;

	pMultiTexturesEmitter->list[1].time = 1.0f;
	pMultiTexturesEmitter->list[1].timeBias = 0.0f;

	pMultiTexturesEmitter->list[2].time = 1.0f;
	pMultiTexturesEmitter->list[2].timeBias = 0.0f;
}

//------------------------------------------------------------------
//	FuncName	: InitialEmitterSplinePath
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::InitialEmitterSplinePath(SNtlPrtEmitterSplinePath* pEmitterSplinePath)
{
	memset(pEmitterSplinePath, 0, sizeof(SNtlPrtEmitterSplinePath));
	pEmitterSplinePath->loop		= FALSE;
	pEmitterSplinePath->useRotate	= FALSE;
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Component
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Component(CNtlResourceComponentSystem* pDestSystem, CNtlResourceComponentSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_NONE;
/*	RwInt32				m_nSystemType;						// 복사 필요 없음
	std::string			m_strName;							// 복사 필요 없음

	RwUInt32			m_nEmitterDataFlag;

	RwBool				m_bSubSystem;						// 새로 결정 하므로 필요 없다

	RwBool				m_bEternity;
	RwReal				m_fStartTime;
	RwReal				m_fLifeTime;

	RwUInt32					m_nPathEmitterDataFlag;
	SNtlPrtEmitterSplinePath	m_EmitterPrtSplinePath;

	RpPrtAdvEmtPrtMultiTextures		m_EmitterMultiTextures;

	RwBool							m_bAlphaBlend;
	RwBlendFunction					m_nSrcBlend;
	RwBlendFunction					m_nDestBlend;

	RwV3d				m_vPosition;
	RwV3d				m_vRotate;

	RwMatrix			m_matWorld;
	RwMatrix			m_matPathWorld;
*/
	pDestSystem->m_nEmitterDataFlag				= pSrcSystem->m_nEmitterDataFlag;

	pDestSystem->m_bEternity					= pSrcSystem->m_bEternity;
	pDestSystem->m_fStartTime					= pSrcSystem->m_fStartTime;
	pDestSystem->m_fLifeTime					= pSrcSystem->m_fLifeTime;

	pDestSystem->m_nPathEmitterDataFlag			= pSrcSystem->m_nPathEmitterDataFlag;
	memcpy(&pDestSystem->m_EmitterPrtSplinePath, &pSrcSystem->m_EmitterPrtSplinePath, sizeof(SNtlPrtEmitterSplinePath));

	// SetEmitterDataFlag_ParticleSystem 에서 nEmitterDataFlag 가 assert 가 걸려 있기 때문에 
	// 여기서 초기화를 하고 액션들을 다 복사한다음 다시 원래대로 세팅한다.
	RwUInt32 nEmitterDataFlag = pDestSystem->m_nEmitterDataFlag;
	pDestSystem->m_nEmitterDataFlag = 0;

	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))		SetEmitterDataFlag_ComponentSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTFADEOUT,	pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))	SetEmitterDataFlag_ComponentSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTITEXTURES,	pSrcSystem);

	pDestSystem->m_nEmitterDataFlag = nEmitterDataFlag;


	pDestSystem->m_bAlphaBlend					= pSrcSystem->m_bAlphaBlend;
	pDestSystem->m_nSrcBlend					= pSrcSystem->m_nSrcBlend;
	pDestSystem->m_nDestBlend					= pSrcSystem->m_nDestBlend;

	pDestSystem->m_vPosition					= pSrcSystem->m_vPosition;
	pDestSystem->m_vRotate						= pSrcSystem->m_vRotate;

	pDestSystem->m_matWorld						= pSrcSystem->m_matWorld;
	pDestSystem->m_matPathWorld					= pSrcSystem->m_matPathWorld;
    pDestSystem->m_sGroundFlag                  = pSrcSystem->m_sGroundFlag;
}


//------------------------------------------------------------------
//	FuncName	: CopySystem_Particle
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Particle(CNtlResourceParticleSystem* pDestSystem, CNtlResourceParticleSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_PARTICLE;
	CopySystem_Component(pDestSystem, pSrcSystem);
/*	
	RwUInt32						m_nPointEmitterDataFlag;

	RwInt32							m_nParticleBatchMaxCount;


	RpPrtStdEmitterStandard			m_EmitterStandard;
	std::string						m_strTextureName;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	RpPrtStdEmitterPrtTexCoords		m_EmitterPrtTexCoords;
	RpPrtStdEmitterPrt2DRotate		m_EmitterPrtRotate;
	RpPrtStdEmitterPrtSize			m_EmitterPrtSize;
	RpPrtStdEmitterPrtMatrix		m_EmitterPrtMatrix;

	RpPrtAdvPrtEmitterEmitter		m_PrtEmitterEmitter;
	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;
	RpPrtAdvEmtPrtMultiTexCoords	m_EmitterPrtMultiTexCoords;
	RpPrtAdvPrtMultiSizeEmitter		m_EmitterPrtMultiSize;

	RpPrtAdvPointListEmitter		m_EmitterPtList;
	RpPrtAdvSphereEmitter			m_EmitterPtrSphere;
    RpPrtAdvCircleEmitter			m_EmitterPtrCircle;

	std::string						m_strSubSystem;					// sub system 은 없으므로 복사하지 않는다

	RwBool							m_bAlphaBlend;
	RwBlendFunction					m_nSrcBlend;
	RwBlendFunction					m_nDestBlend;
*/

	pDestSystem->m_nPointEmitterDataFlag		= pSrcSystem->m_nPointEmitterDataFlag;
	pDestSystem->m_nParticleBatchMaxCount		= pSrcSystem->m_nParticleBatchMaxCount;

	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(RpPrtStdEmitterStandard));
	pDestSystem->m_strTextureName				= pSrcSystem->m_strTextureName.c_str();

	// SetEmitterDataFlag_ParticleSystem 에서 nEmitterDataFlag 가 assert 가 걸려 있기 때문에 
	// 여기서 초기화를 하고 액션들을 다 복사한다음 다시 원래대로 세팅한다.
	RwUInt32 nEmitterDataFlag = pDestSystem->m_nEmitterDataFlag;
	pDestSystem->m_nEmitterDataFlag = 0;

	RwUInt32 nPointEmitterDataFlag = pDestSystem->m_nPointEmitterDataFlag;
	pDestSystem->m_nPointEmitterDataFlag = 0;

	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))				SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTCOLOR,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS,	pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRT2DROTATE,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))				SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTSIZE,			pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTMATRIX,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTHURRICANE,	pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))		SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT,	pSrcSystem);

	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGPRTEMITTER,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTICOLOR,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))		SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS,	pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))			SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTISIZE,		pSrcSystem);
    if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))			    SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGTRAIL,		    pSrcSystem);

	if (pSrcSystem->m_nPointEmitterDataFlag == rpPRTADVEMITTERDATAFLAGPOINTLIST)	SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGPOINTLIST,		pSrcSystem);
	if (pSrcSystem->m_nPointEmitterDataFlag == rpPRTADVEMITTERDATAFLAGCIRCLE)		SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGCIRCLE,			pSrcSystem);
	if (pSrcSystem->m_nPointEmitterDataFlag == rpPRTADVEMITTERDATAFLAGSPHERE)		SetEmitterDataFlag_ParticleSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGSPHERE,			pSrcSystem);
    


	pDestSystem->m_nEmitterDataFlag = nEmitterDataFlag;
	pDestSystem->m_nPointEmitterDataFlag = nPointEmitterDataFlag;
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Mesh
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Mesh(CNtlResourceMeshSystem* pDestSystem, CNtlResourceMeshSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_MESH;
	CopySystem_Component(pDestSystem, pSrcSystem);

/*	std::string						m_strMeshFileName;
	std::string						m_strAniFileName;
	std::string						m_strUvFileName;

	SNtlMeshEmitterStandard			m_EmitterStandard;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	SNtlPrtStdEmitterPrtSize		m_EmitterPrtSize;
	SNtlPrtStdEmitterPrtRotate		m_EmitterPrtRotate;
*/
	pDestSystem->m_strMeshFileName				= pSrcSystem->m_strMeshFileName.c_str();
	pDestSystem->m_strAniFileName				= pSrcSystem->m_strAniFileName.c_str();
	pDestSystem->m_strUvFileName				= pSrcSystem->m_strUvFileName.c_str();
	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlMeshEmitterStandard));

	// SetEmitterDataFlag_ParticleSystem 에서 nEmitterDataFlag 가 assert 가 걸려 있기 때문에 
	// 여기서 초기화를 하고 액션들을 다 복사한다음 다시 원래대로 세팅한다.
	RwUInt32 nEmitterDataFlag = pDestSystem->m_nEmitterDataFlag;
	pDestSystem->m_nEmitterDataFlag = 0;

	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))			SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTCOLOR,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))		SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRT2DROTATE,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))			SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTSIZE,			pSrcSystem);

	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))		SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTICOLOR,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))		SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTISIZE,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))		SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTIROTATE,		pSrcSystem);
    if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))        SetEmitterDataFlag_MeshSystem(pDestSystem,  rpPRTSTDEMITTERDATAFLAGPRTFINISH,       pSrcSystem);
    if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))		    SetEmitterDataFlag_MeshSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGTRAIL,		    pSrcSystem);

	pDestSystem->m_nEmitterDataFlag = nEmitterDataFlag;
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Particle
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_ImVertex(CNtlResourceImVertexSystem* pDestSystem, CNtlResourceImVertexSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_NONE;
	CopySystem_Component(pDestSystem, pSrcSystem);
/*

	std::string						m_strTextureName;

	RpPrtStdEmitterPrtColor			m_EmitterPrtCol;
	SNtlPrtStdEmitterPrtSize		m_EmitterPrtSize;

*/
	pDestSystem->m_strTextureName			= pSrcSystem->m_strTextureName.c_str();

	// SetEmitterDataFlag_ParticleSystem 에서 nEmitterDataFlag 가 assert 가 걸려 있기 때문에 
	// 여기서 초기화를 하고 액션들을 다 복사한다음 다시 원래대로 세팅한다.
	RwUInt32 nEmitterDataFlag = pDestSystem->m_nEmitterDataFlag;
	pDestSystem->m_nEmitterDataFlag = 0;

	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))			SetEmitterDataFlag_ImVertexSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTCOLOR,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))			SetEmitterDataFlag_ImVertexSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTSIZE,			pSrcSystem);

	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))		SetEmitterDataFlag_ImVertexSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTICOLOR,		pSrcSystem);
	if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))		SetEmitterDataFlag_ImVertexSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTISIZE,		pSrcSystem);
    if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))        SetEmitterDataFlag_ImVertexSystem(pDestSystem, rpPRTSTDEMITTERDATAFLAGPRTFINISH,        pSrcSystem);

	pDestSystem->m_nEmitterDataFlag = nEmitterDataFlag;
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Beam
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Beam(CNtlResourceBeamSystem* pDestSystem, CNtlResourceBeamSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_BEAM;
	CopySystem_ImVertex(pDestSystem, pSrcSystem);

	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlBeamEmitterStandard));
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Hurricane
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Hurricane(CNtlResourceHurricaneSystem* pDestSystem, CNtlResourceHurricaneSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_HURRICANE;
	CopySystem_ImVertex(pDestSystem, pSrcSystem);

	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlHurricaneEmitterStandard));
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Lightning
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Lightning(CNtlResourceLightningSystem* pDestSystem, CNtlResourceLightningSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_LIGHTNING;
	CopySystem_ImVertex(pDestSystem, pSrcSystem);

	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlLightningEmitterStandard));
}


void CVenusPropertyContainer::CopySystem_Line( CNtlResourceLineSystem* pDestSystem, CNtlResourceLineSystem* pSrcSystem )
{
    //RESOURCE_SYSTEMTYPE_LINE
    CopySystem_ImVertex(pDestSystem, pSrcSystem);

    // Emitter Standard
    memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlLineEmitterStandard));

    // Emitter Sphere
    memcpy(&pDestSystem ->m_EmitterSphere, &pSrcSystem->m_EmitterSphere, sizeof(SNtlPrtStdEmitterPrtSphere));

    // Rotate Action
    memcpy(&pDestSystem->m_EmitterPrtRotate, &pSrcSystem->m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate));
    memcpy(&pDestSystem->m_EmitterPrtMultiRotate, &pSrcSystem->m_EmitterPrtMultiRotate, sizeof(SNtlAdvMultiRotateEmitter));
    

}


//------------------------------------------------------------------
//	FuncName	: CopySystem_Lightning
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_Decal(CNtlResourceDecalSystem* pDestSystem, CNtlResourceDecalSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_DECAL
	CopySystem_ImVertex(pDestSystem, pSrcSystem);

	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlDecalEmitterStandard));
	memcpy(&pDestSystem->m_EmitterPrtRotate, &pSrcSystem->m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate));
	memcpy(&pDestSystem->m_EmitterUVAnim, &pSrcSystem->m_EmitterUVAnim, sizeof(SNtlPrtStdEmitterPrtUVAnim));
}

//------------------------------------------------------------------
//	FuncName	: CopySystem_Lightning
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::CopySystem_PostEffect(CNtlResourcePostEffectSystem* pDestSystem, CNtlResourcePostEffectSystem* pSrcSystem)
{
	//RESOURCE_SYSTEMTYPE_POST_EFFECT
	CopySystem_Component(pDestSystem, pSrcSystem);

	memcpy(&pDestSystem->m_EmitterStandard, &pSrcSystem->m_EmitterStandard, sizeof(SNtlPostEffectEmitterStandard));

    // SetEmitterDataFlag_ParticleSystem 에서 nEmitterDataFlag 가 assert 가 걸려 있기 때문에 
    // 여기서 초기화를 하고 액션들을 다 복사한다음 다시 원래대로 세팅한다.
    RwUInt32 nEmitterDataFlag = pDestSystem->m_nEmitterDataFlag;
    pDestSystem->m_nEmitterDataFlag = 0;

    if (pSrcSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))			SetEmitterDataFlag_PostEffectSystem(pDestSystem,	rpPRTSTDEMITTERDATAFLAGPRTCOLOR,		pSrcSystem);
    if (pSrcSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))		SetEmitterDataFlag_PostEffectSystem(pDestSystem,	rpPRTADVEMITTERDATAFLAGMULTICOLOR,		pSrcSystem);

    pDestSystem->m_nEmitterDataFlag = nEmitterDataFlag;
}


//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(const RwChar* strPathName)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, strPathName, "wb");

	if (pFile == NULL)
	{
        RwChar szMsg[128] = {0,};
        sprintf_s(szMsg, "[%s] File Is Can't Open to Save", strPathName);
        AfxMessageBox(szMsg);
		return FALSE;
	}

	RwChar* strFileHeader = EFFECT_FILE_HEADER;
	fwrite(strFileHeader,				strlen(EFFECT_FILE_HEADER),					1,			pFile);

	RwUInt32 nVersion = EFFECT_FILE_VERSION;
	fwrite(&nVersion,					sizeof(RwUInt32),							1,			pFile);

	// ============================================================================
	// Spline Property
	// ============================================================================
	RwInt32 nSplineCount = (RwInt32)m_mapSpline.size();
	fwrite(&nSplineCount,				sizeof(RwInt32),							1,			pFile);
	if (nSplineCount > 0)
	{
        for each(CNtlEffectSplinePath* pEffectSplinePath in m_svSplinePathProperty)
		{
			RwInt32 length = (RwInt32)strlen(pEffectSplinePath->GetName()) + 1;
			fwrite(&length,					sizeof(RwInt32),							1,			pFile);
			fwrite(pEffectSplinePath->GetName(),	sizeof(RwChar),							length,		pFile);

			Save(pFile, pEffectSplinePath);
		}
	}

	// ============================================================================
	// Effect Property
	// ============================================================================
	NTL_ASSERTE(m_mapEffect.size() == m_svEffectProperty.size());
	RwInt32 nEffectCount = (RwInt32)m_svEffectProperty.size();
	fwrite(&nEffectCount,				sizeof(RwInt32),							1,			pFile);
	if (nEffectCount > 0)
	{
        for each(CNtlResourceEffect* pResourceEffect in m_svEffectProperty)
		{
			RwInt32 length = (RwInt32)strlen(pResourceEffect->GetName()) + 1;
			fwrite(&length,					sizeof(RwInt32),							1,			pFile);
			fwrite(pResourceEffect->GetName(),	sizeof(RwChar),							length,		pFile);

			Save(pFile, pResourceEffect);
		}
	}

	fclose(pFile);

    // ============================================================================
    // XML Save
    // ============================================================================
    CString strFileName = PATH_TREE_XML;
    strFileName += GetFileNameFromPath(strPathName);
    strFileName.Replace("eff", "XML");
    CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
    if(!pView->SaveXMLTree(strFileName.GetBuffer(0)))
    {
        AfxMessageBox("XML Tree File Save Fail!");
    }
    
	m_bSave = FALSE;
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlEffectSplinePath* pEffectSplinePath)
{
	fwrite(&pEffectSplinePath->m_Info,			sizeof(CNtlEffectSplinePath::INFO),	1,			pFile);

	RwInt32 nControlPointCount = pEffectSplinePath->m_Info.nControlCount;
	fwrite(pEffectSplinePath->m_pControlPoint,	sizeof(SNTL_SPLINE_CONTROL_POINT),	nControlPointCount,			pFile);

	RwInt32 nTotalCurveCount = pEffectSplinePath->m_nTotalCurveCount;
	fwrite(pEffectSplinePath->m_pCurveData,		sizeof(SNTL_SPLINE_CURVE_POINT),	nTotalCurveCount,			pFile);

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceEffect* pResourceEffect)
{
	RwInt32 nSystemCount = (RwInt32)pResourceEffect->m_svResourceSystem.size();
	fwrite(&nSystemCount,			sizeof(RwInt32),							1,			pFile);
	if (nSystemCount > 0)
	{
		CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
		for (it = pResourceEffect->m_svResourceSystem.begin(); it != pResourceEffect->m_svResourceSystem.end(); ++ it)
		{
			RwInt32 nSystemType = (*it)->GetSystemType();
			fwrite(&nSystemType,				sizeof(RwInt32),					1,			pFile);

			RwInt32 length = (RwInt32)strlen((*it)->GetSystemName()) + 1;
			fwrite(&length,						sizeof(RwInt32),					1,			pFile);

			fwrite((*it)->GetSystemName(),		sizeof(RwChar),						length,		pFile);

			switch(nSystemType)
			{
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
				Save(pFile, (CNtlResourceParticleSystem*)(*it));
				break;
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
				Save(pFile, (CNtlResourceMeshSystem*)(*it));
				break;
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
				Save(pFile, (CNtlResourceBeamSystem*)(*it));
				break;
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
				Save(pFile, (CNtlResourceHurricaneSystem*)(*it));
				break;
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
				Save(pFile, (CNtlResourceLightningSystem*)(*it));
				break;
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
				Save(pFile, (CNtlResourceDecalSystem*)(*it));
				break;
			case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
				Save(pFile, (CNtlResourcePostEffectSystem*)(*it));
				break;
            case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
                Save(pFile, (CNtlResourceLineSystem*)(*it));
                break;
			default:
				NTL_ASSERTE(!"default");
				break;
			}
		}
	}

    // Sound Save
    fwrite(&pResourceEffect->m_EffectSound, sizeof(pResourceEffect->m_EffectSound), 1, pFile);

    // Bounding Sphere
    fwrite(&pResourceEffect->m_EffectBoundingSphere, sizeof(pResourceEffect->m_EffectBoundingSphere), 1, pFile);

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceComponentSystem* pResourceSystem)
{
	fwrite(&pResourceSystem->m_nEmitterDataFlag,		sizeof(RwUInt32),						1,			pFile);

	fwrite(&pResourceSystem->m_bSubSystem,				sizeof(RwBool),							1,			pFile);

	fwrite(&pResourceSystem->m_bEternity,				sizeof(RwBool),							1,			pFile);
	fwrite(&pResourceSystem->m_fStartTime,				sizeof(RwReal),							1,			pFile);
	fwrite(&pResourceSystem->m_fLifeTime,				sizeof(RwReal),							1,			pFile);

	fwrite(&pResourceSystem->m_nPathEmitterDataFlag,	sizeof(RwUInt32),						1,			pFile);
	if (pResourceSystem->m_nPathEmitterDataFlag == rpPRTADVEMITTERDATAFLAGSPLINEPATH)
	{
		fwrite(&pResourceSystem->m_EmitterPrtSplinePath,	sizeof(SNtlPrtEmitterSplinePath),		1,			pFile);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))
	{
		fwrite(&pResourceSystem->m_EmitterPrtFadeOut,		sizeof(SNtlPrtStdEmitterPrtFadeOut),		1,			pFile);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
	{
		fwrite(&pResourceSystem->m_EmitterMultiTextures.time,					sizeof(RwReal),		1,			pFile);
		fwrite(&pResourceSystem->m_EmitterMultiTextures.bLoop,					sizeof(RwBool),		1,			pFile);
		fwrite(&pResourceSystem->m_EmitterMultiTextures.numTextures,			sizeof(RwUInt32),	1,			pFile);
		if (pResourceSystem->m_EmitterMultiTextures.numTextures > 0)
		{
			for (RwUInt32 i = 0; i < pResourceSystem->m_EmitterMultiTextures.numTextures; ++ i)
			{
				fwrite(&pResourceSystem->m_EmitterMultiTextures.list[i].time,		sizeof(RwReal),						1,			pFile);
				fwrite(&pResourceSystem->m_EmitterMultiTextures.list[i].timeBias,	sizeof(RwReal),						1,			pFile);

				Save(pFile, pResourceSystem->m_EmitterMultiTextures.list[i].strTexture);
			}
		}
	}

	fwrite(&pResourceSystem->m_bAlphaBlend,				sizeof(RwBool),							1,			pFile);
	fwrite(&pResourceSystem->m_nSrcBlend,				sizeof(RwBlendFunction),				1,			pFile);
	fwrite(&pResourceSystem->m_nDestBlend,				sizeof(RwBlendFunction),				1,			pFile);

	fwrite(&pResourceSystem->m_vPosition,				sizeof(RwV3d),							1,			pFile);
	fwrite(&pResourceSystem->m_vRotate,					sizeof(RwV3d),							1,			pFile);

    fwrite(&pResourceSystem->m_sGroundFlag, sizeof(SGroundFlag), 1, pFile);
    fwrite(&pResourceSystem->m_fBoundSphereRadius, sizeof(RwReal), 1, pFile);

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceParticleSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceComponentSystem*) pResourceSystem);

	fwrite(&pResourceSystem->m_nPointEmitterDataFlag,	sizeof(RwUInt32),						1,			pFile);

	fwrite(&pResourceSystem->m_nParticleBatchMaxCount,	sizeof(RwInt32),						1,			pFile);

	Save(pFile, pResourceSystem->m_strSubSystem);

	Save(pFile, pResourceSystem->m_strTextureName);

	fwrite(&pResourceSystem->m_EmitterStandard,				sizeof(RpPrtStdEmitterStandard),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
		fwrite(&pResourceSystem->m_EmitterPrtCol,			sizeof(RpPrtStdEmitterPrtColor),		1,			pFile);
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS))
		fwrite(&pResourceSystem->m_EmitterPrtTexCoords,		sizeof(RpPrtStdEmitterPrtTexCoords),	1,			pFile);
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
		fwrite(&pResourceSystem->m_EmitterPrtRotate,		sizeof(RpPrtStdEmitterPrt2DRotate),		1,			pFile);
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
		fwrite(&pResourceSystem->m_EmitterPrtSize,			sizeof(RpPrtStdEmitterPrtSize),			1,			pFile);
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
		fwrite(&pResourceSystem->m_EmitterPrtMatrix,		sizeof(RpPrtStdEmitterPrtMatrix),		1,			pFile);
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE))
		fwrite(&pResourceSystem->m_EmitterPrtHurricane,		sizeof(SNtlPrtStdEmitterPrtHurricane),	1,			pFile);
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))
		fwrite(&pResourceSystem->m_EmitterPrtOrbitPoint,	sizeof(SNtlPrtStdEmitterPrtOrbitPoint),	1,			pFile);


	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER))
		fwrite(&pResourceSystem->m_PrtEmitterEmitter,		sizeof(RpPrtAdvPrtEmitterEmitter),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiCol.numCol,			sizeof(RwUInt32),					1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiCol.numCol > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiCol.list,			sizeof(RpPrtAdvEmtPrtColorItem),		pResourceSystem->m_EmitterPrtMultiCol.numCol,	pFile);
		}
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiTexCoords.numTex,		sizeof(RwUInt32),				1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiTexCoords.numTex > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiTexCoords.list,	sizeof(RpPrtAdvEmtPrtTexCoordsItem),	pResourceSystem->m_EmitterPrtMultiTexCoords.numTex,	pFile);
		}
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiSize.numSize,			sizeof(RwUInt32),	1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiSize.numSize > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiSize.list,	sizeof(RpPrtAdvEmtPrtSizeItem),	pResourceSystem->m_EmitterPrtMultiSize.numSize,	pFile);
		}
	}

	switch(pResourceSystem->m_nPointEmitterDataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGPOINTLIST:
		{
			fwrite(&pResourceSystem->m_EmitterPtList.useDirection,		sizeof(RwBool),			1,			pFile);
			fwrite(&pResourceSystem->m_EmitterPtList.numPoint,			sizeof(RwUInt32),		1,			pFile);
			if (pResourceSystem->m_EmitterPtList.numPoint > 0)
			{
				fwrite(pResourceSystem->m_EmitterPtList.pointList,		sizeof(RwV3d),		pResourceSystem->m_EmitterPtList.numPoint,			pFile);
				fwrite(pResourceSystem->m_EmitterPtList.directionList,	sizeof(RwV3d),		pResourceSystem->m_EmitterPtList.numPoint,			pFile);
			}
		}
		break;
	case rpPRTADVEMITTERDATAFLAGCIRCLE:
		fwrite(&pResourceSystem->m_EmitterPtrCircle,		sizeof(RpPrtAdvCircleEmitter),			1,			pFile);
		break;
	case rpPRTADVEMITTERDATAFLAGSPHERE:
		fwrite(&pResourceSystem->m_EmitterPtrSphere,		sizeof(RpPrtAdvSphereEmitter),			1,			pFile);
		break;
	}

    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        fwrite(&pResourceSystem->m_EmitterTrail, sizeof(SNtlPrtStdEmitterTrail), 1, pFile);
    }

	if(pResourceSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE))
	{
		fwrite(&pResourceSystem->m_fDepthBias, sizeof(RwReal), 1, pFile);
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceMeshSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceComponentSystem*) pResourceSystem);

	Save(pFile, pResourceSystem->m_strMeshFileName);

	Save(pFile, pResourceSystem->m_strAniFileName);

	Save(pFile, pResourceSystem->m_strUvFileName);

	fwrite(&pResourceSystem->m_EmitterStandard,				sizeof(SNtlMeshEmitterStandard),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
		fwrite(&pResourceSystem->m_EmitterPrtCol,			sizeof(RpPrtStdEmitterPrtColor),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
		fwrite(&pResourceSystem->m_EmitterPrtRotate,		sizeof(SNtlPrtStdEmitterPrtRotate),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
		fwrite(&pResourceSystem->m_EmitterPrtSize,			sizeof(SNtlPrtStdEmitterPrtSize),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiCol.numCol,			sizeof(RwUInt32),					1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiCol.numCol > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiCol.list,			sizeof(RpPrtAdvEmtPrtColorItem),		pResourceSystem->m_EmitterPrtMultiCol.numCol,	pFile);
		}
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiSize.numSize,			sizeof(RwUInt32),	1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiSize.numSize > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiSize.list,	sizeof(SNtlAdvEmtPrtSizeItem),	pResourceSystem->m_EmitterPrtMultiSize.numSize,	pFile);
		}
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiRotate.numRotate,			sizeof(RwUInt32),	1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiRotate.numRotate > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiRotate.list,	sizeof(SNtlAdvEmtPrtRotateItem),	pResourceSystem->m_EmitterPrtMultiRotate.numRotate,	pFile);
		}
	}

    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
    {
        fwrite(&pResourceSystem->m_EmitterPrtFinish, sizeof(SNtlPrtStdEmitterPrtFinish), 1, pFile);
    }

    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        fwrite(&pResourceSystem->m_EmitterTrail, sizeof(SNtlPrtStdEmitterTrail), 1, pFile);
    }

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceImVertexSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceComponentSystem*) pResourceSystem);

	Save(pFile, pResourceSystem->m_strTextureName);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
		fwrite(&pResourceSystem->m_EmitterPrtCol,			sizeof(RpPrtStdEmitterPrtColor),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
		fwrite(&pResourceSystem->m_EmitterPrtSize,			sizeof(SNtlPrtStdEmitterPrtSize),		1,			pFile);

	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiCol.numCol,			sizeof(RwUInt32),					1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiCol.numCol > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiCol.list,			sizeof(RpPrtAdvEmtPrtColorItem),		pResourceSystem->m_EmitterPrtMultiCol.numCol,	pFile);
		}
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		fwrite(&pResourceSystem->m_EmitterPrtMultiSize.numSize,			sizeof(RwUInt32),	1,			pFile);
		if (pResourceSystem->m_EmitterPrtMultiSize.numSize > 0)
		{
			fwrite(pResourceSystem->m_EmitterPrtMultiSize.list,	sizeof(SNtlAdvEmtPrtSizeItem),	pResourceSystem->m_EmitterPrtMultiSize.numSize,	pFile);
		}
	}

    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
    {
        fwrite(&pResourceSystem->m_EmitterPrtFinish, sizeof(SNtlPrtStdEmitterPrtFinish), 1, pFile);
    }

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceBeamSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceImVertexSystem*)pResourceSystem);

	fwrite(&pResourceSystem->m_EmitterStandard,				sizeof(SNtlBeamEmitterStandard),		1,			pFile);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceHurricaneSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceImVertexSystem*)pResourceSystem);

	fwrite(&pResourceSystem->m_EmitterStandard,				sizeof(SNtlHurricaneEmitterStandard),		1,			pFile);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceLightningSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceImVertexSystem*)pResourceSystem);

	fwrite(&pResourceSystem->m_EmitterStandard,				sizeof(SNtlLightningEmitterStandard),		1,			pFile);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourceDecalSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceImVertexSystem*)pResourceSystem);

	fwrite(&pResourceSystem->m_EmitterStandard,	sizeof(SNtlDecalEmitterStandard), 1, pFile);
	fwrite(&pResourceSystem->m_EmitterPrtRotate, sizeof(RpPrtStdEmitterPrt2DRotate), 1, pFile);
	fwrite(&pResourceSystem->m_EmitterUVAnim, sizeof(SNtlPrtStdEmitterPrtUVAnim), 1, pFile);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, CNtlResourcePostEffectSystem* pResourceSystem)
{
	Save(pFile, (CNtlResourceComponentSystem*)pResourceSystem);

	Save(pFile, pResourceSystem->m_strTextureName);

	fwrite(&pResourceSystem->m_EmitterStandard,	sizeof(SNtlPostEffectEmitterStandard), 1, pFile);        
    
    // Color Emitter
    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
    {
        fwrite(&pResourceSystem->m_EmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor), 1, pFile);        
    }

    // Multi Color Emitter
    if(pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
    {
        fwrite(&pResourceSystem->m_EmitterPrtMultiCol.offsetTime, sizeof(RwUInt32), 1, pFile);
        fwrite(&pResourceSystem->m_EmitterPrtMultiCol.offsetStartCol, sizeof(RwUInt32), 1, pFile);
        fwrite(&pResourceSystem->m_EmitterPrtMultiCol.offsetDeltaCol, sizeof(RwUInt32), 1, pFile);
        fwrite(&pResourceSystem->m_EmitterPrtMultiCol.numCol, sizeof(RwUInt32), 1, pFile); 
        if(pResourceSystem->m_EmitterPrtMultiCol.numCol > 0)
        {
            fwrite(pResourceSystem->m_EmitterPrtMultiCol.list, sizeof(RpPrtAdvEmtPrtColorItem), pResourceSystem->m_EmitterPrtMultiCol.numCol, pFile);
        }        
    }

	return TRUE;
}

RwBool CVenusPropertyContainer::Save( FILE* pFile, CNtlResourceLineSystem* pResourceLineSystem )
{
    Save(pFile, (CNtlResourceImVertexSystem*)pResourceLineSystem);

    fwrite(&pResourceLineSystem->m_EmitterStandard, sizeof(SNtlLineEmitterStandard), 1, pFile);

    if(pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGSPHERE))
    {
        fwrite(&pResourceLineSystem->m_EmitterSphere, sizeof(SNtlPrtStdEmitterPrtSphere), 1, pFile);
    }

    if(pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
    {
        fwrite(&pResourceLineSystem->m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate), 1, pFile);
    }

    if(pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
    {
        fwrite(&pResourceLineSystem->m_EmitterPrtMultiRotate.numRotate,			sizeof(RwUInt32),	1,			pFile);
        if (pResourceLineSystem->m_EmitterPrtMultiRotate.numRotate > 0)
        {
            fwrite(pResourceLineSystem->m_EmitterPrtMultiRotate.list,	sizeof(SNtlAdvEmtPrtRotateItem),	pResourceLineSystem->m_EmitterPrtMultiRotate.numRotate,	pFile);
        }        
    }

    return TRUE;
}
//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Save(FILE* pFile, std::string& string)
{
	RwInt32 length;
	if (string.empty())
	{
		length = 0;
		fwrite(&length,										sizeof(RwInt32),						1,			pFile);
	}
	else
	{
		length = (RwInt32)string.size() + 1;
		fwrite(&length,										sizeof(RwInt32),						1,			pFile);
		fwrite(string.c_str(),								sizeof(RwChar),							length,		pFile);
	}
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPropertyContainer::Load(const RwChar* strPathName)
{
	return CNtlPLPropertyContainer::LoadEffect(strPathName);

    // 이펙트 사용 메모리를 파일에 출력한다.
    //OutputUsedMemory();    
}

RwBool CVenusPropertyContainer::LoadSerialize( RwBool bEncrypt /*= FALSE*/, RwChar* szCryptPassword /*= NULL*/ ) 
{
	NTL_FUNCTION(__FUNCTION__);

	return CNtlPLPropertyContainer::LoadSerialize(bEncrypt, szCryptPassword); // by daneos

	if(!LoadNtlWorld(""))
	{
        NTL_ASSERTFAIL("Load World Fail");
		NTL_RETURN(FALSE);
	}

	if(!LoadWater(".\\script\\waterproperty.xml"))
	{
        NTL_ASSERTFAIL("Load Water Fail");
		NTL_RETURN(FALSE);
	}

	if(!LoadShoreLine(".\\script\\ShoreLineProp.xml"))
	{
        NTL_ASSERTFAIL("Load ShoreLine Fail");
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}


//------------------------------------------------------------------
//	FuncName	: Sort
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPropertyContainer::Sort(RwBool bSequence)
{
	m_bSave = TRUE;

	m_svEffectProperty.clear();

	MapProp::iterator it;
	for(it = m_mapEffect.begin(); it != m_mapEffect.end(); ++it)
	{
		CNtlPLProperty *pProp = (*it).second;

		svdef_Property::iterator jt;
		for (jt = m_svEffectProperty.begin(); jt != m_svEffectProperty.end(); ++ jt)
		{
			if (bSequence)
			{
				if (Sort((*jt)->GetName(), pProp->GetName()) == -1)
				{
					break;
				}
			}
			else
			{
				if (Sort((*jt)->GetName(), pProp->GetName()) == 1)
				{
					break;
				}
			}
		}
		m_svEffectProperty.insert(jt, pProp);
	}
}

RwInt32 CVenusPropertyContainer::Sort(const RwChar* strSrc_In, const RwChar* strDest_In)
{
	RwChar strSrc[MAX_PATH], strDest[MAX_PATH];
	strcpy(strSrc, strSrc_In);
	strcpy(strDest, strDest_In);

	strlwr(strSrc);
	strlwr(strDest);

	RwInt32 nLength = strlen(strSrc) < strlen(strDest) ? (RwInt32)strlen(strSrc) : (RwInt32)strlen(strDest);
	for (RwInt32 i = 0; i < nLength; ++ i)
	{
		if (strSrc[i] < strDest[i])
		{
			return 1;
		}
		else if (strSrc[i] > strDest[i])
		{
			return -1;
		}
	}
	if (strlen(strSrc) < strlen(strDest))
	{
		return 1;
	}
	else if (strlen(strSrc) > strlen(strDest))
	{
		return -1;
	}

	return 0;
}

CNtlResourceEffect* CVenusPropertyContainer::FindProperty( const RwChar* strName ) 
{
    for each(CNtlPLProperty* pProperty in m_svEffectProperty)
    {
        if(strcmp(pProperty->GetName(), strName) == 0)
        {
            return (CNtlResourceEffect*)pProperty;
        }
    }

    return NULL;
}

void CVenusPropertyContainer::OutputUsedMemory() 
{
    CFile file("EffectUsedMemoryList.txt", CFile::modeCreate | CFile::modeWrite);    
    char szBuf[1024] = {0,};
    int nCount = 0;    

    for each(CNtlPLProperty* pProperty in m_svEffectProperty)
    {
        ++nCount;
        CNtlInstanceEffect* pEffect = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pProperty->GetName());
        if(pEffect->GetMemoryUseSize() >= 1024 * 1024)
        {
            sprintf_s(szBuf, "%d. %s\t\t\t\t %dKb       --- is Big Memory Used!!\r\n", nCount, pProperty->GetName(), pEffect->GetMemoryUseSize() / 1024);
        }
        else
        {
            sprintf_s(szBuf, "%d. %s\t\t\t\t %dKb\r\n", nCount, pProperty->GetName(), pEffect->GetMemoryUseSize() / 1024);
        }
        
        GetSceneManager()->DeleteEntity(pEffect);

        file.Write(szBuf, strlen(szBuf));
    }

    file.Close();
}
