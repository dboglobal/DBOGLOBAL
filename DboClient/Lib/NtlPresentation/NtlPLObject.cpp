#include "precomp_ntlpresentation.h"
#include "NtlPLObject.h"
#include "NtlPLEntityFreeList.h"
#include "NtlDebug.h"
#include "NtlProfiler.h"
#include "NtlPLResource.h"
#include "NtlPLResourceManager.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLHelpers.h"
#include "NtlPLGlobal.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlPLRenderState.h"
#include "NtlSoundMessage.h"
#include "NtlSoundManager.h"
#include "NtlEffectApi.h"
#include "NtlMath.h"
#include "NtlPLSceneManager.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlPLEventGenerator.h"
#include "NtlAtomic.h"
#include "NtlWorldShadow.h"
#include "NtlPLEntityBlend.h"
#include "NtlPLWorldState.h"
#include "NtlPLCharacter.h"
#include "NtlPLApi.h"
#include "NtlPLFog.h"
#include "NtlWorldInterface.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOptionManager.h"
#include "rtltmap.h"
#include "NtlWorldLTManager.h"
#include "NtlPLEmblemMaker.h"
#include "NtlPLLightObject.h"
#include "NtlPLWorldEntity.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"

#include "NtlPLCullingScheduling.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CNtlPLObject::CNtlPLObject()
{
	m_pProperty         = NULL;
	m_pClumpResource	= NULL;
	m_pUVAnim           = NULL;
	m_pBaseHierarchy    = NULL;
	m_pAnimResource     = NULL;
	m_bAnimPlay         = FALSE;	
	m_bHaveAnim			= FALSE;

	m_fAnimSpeed		= 1.f;

	ZeroMemory( &m_vWorldPosition, sizeof( RwV3d ) );
	ZeroMemory( &m_vModelAngle, sizeof( RwV3d ) );

	m_vModelScale.x = m_vModelScale.y = m_vModelScale.z = 1.0f;

	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_OBJECT);
	SetFlags(NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_NOTUPDATE | NTL_PLEFLAG_FADE | NTL_PLEFLAG_COLLISION);

	m_sColor.red        = 255;
	m_sColor.green		= 255;
	m_sColor.blue		= 255;
	m_sColor.alpha		= 255;

	m_sAddColor.red		= 0;
	m_sAddColor.green   = 0;
	m_sAddColor.blue	= 0;
	m_sAddColor.alpha	= 255;

	m_eFadeState        = FADE_VISIBLE;    
	m_fFadeTime         = 0.0f;    

	m_bActiveUpdate		= FALSE;
	m_bLoadMap			= TRUE;

	m_pAnimLayer		= NULL;
	m_pInstanceAnimTable = NULL;
	m_uiCurAnimKey		= INVALID_DWORD;

	m_uiMilepostTextID	= NULL;
	m_pFadeBlend		= NULL;
	m_pToonData			= NULL;

	m_uiObjectSerialID	= NULL;

	m_pSchedulingLMProp = NULL;

	m_pDojoEntity		= NULL;

	m_pObjectType		= NULL;

	ZeroMemory(&m_bbox, sizeof(RwBBox));

	m_vecNtlWorldShadow.clear();
	m_vecNtlWorldShadow.reserve(4);

#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->AddCulling(this);
#endif
}

CNtlPLObject::~CNtlPLObject()
{
#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->RemoveCulling(GetClassType(), this);
#endif

	m_vecNtlWorldShadow.clear();

	FreeSchedulingLTProp();
}

void* CNtlPLObject::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLObject::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_OBJECT));
}

void CNtlPLObject::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_OBJECT, pObj);
}

RwBool CNtlPLObject::CreateScheduling(const SPLEntityCreateParam * pParam)
{
	NTL_FUNCTION("CNtlPLObject::CreateScheduling");

	DBO_ASSERT(pParam != NULL, "null create param");
	if(pParam)
	{
		SPLObjectCreateParam* pObjectParam = (SPLObjectCreateParam*)pParam;
		m_bLoadMap = pObjectParam->bLoadMap;
		RwV3dAssign(&m_vWorldPosition, pParam->pPos);
	}

	std::string strDff = CNtlPLObjectProperty::m_strMeshPath + m_pProperty->m_strDffFile;
	if( m_pProperty->m_strDffFile.c_str() != NULL && rwstrlen( m_pProperty->m_strDffFile.c_str() ) > 0 )
		CNtlPLResourceManager::GetInstance()->LoadSchedulingClump( strDff.c_str(), CNtlPLObjectProperty::m_strTexPath.c_str(), this );   

	NTL_RETURN( TRUE );
}

RwBool CNtlPLObject::CreateThreadSafe(void)
{
	NTL_SPROFILE("CNtlPLObject::CreateThreadSafe");

	Helper_SetClumpAllAtomics(GetClump(), &m_vecAtomicList);		
	Helper_GetBoneList(GetClump(), &m_mapFrame);            // Bone List를 저장해 둔다.

	// Anim을 적용한다
	if(m_pProperty->m_strAnimFileName.size() > 0)
	{
		SetAnimation(m_pProperty->m_strAnimFileName.c_str());
	}

	// 트리거 애니메이션이 적용된게 있으면 적용한다.
	m_sScheduleInfo.bLoadComplete = TRUE;
	if(m_sScheduleInfo.uiAnimKey > 0)
	{
		SetTriggerAnimation(m_sScheduleInfo.uiAnimKey, m_sScheduleInfo.fAnimStartTime, m_sScheduleInfo.bAnimLoop);
	}


	// UVAnim을 Clump에 적용한다.
	if(m_pUVAnim)
	{
		m_pUVAnim->SetClump(GetClump());
		m_pUVAnim->SetUVAnimSpeed(m_pProperty->GetUVAnimSpeed());

		m_bHaveAnim = TRUE;
		m_bAnimPlay = TRUE;
	}

	// bbox Update
	m_bbox = GetTriggerAABBInfo();

	if(m_pProperty->GetCollision())
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_PECOLLISION);
	}

	if(m_pProperty->GetEnableShadow())
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_SHADOW);
	}

	// clump에 대한 atomic
	if( GetClump() != NULL )
	{
		// Toon
		CheckToonData();

		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
			RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );            
			RpAtomicSetRenderCallBack(pAtomic, CNtlPLObject::RenderCallBack);            			
			RpNtlAtomicSetData(pAtomic, this);

			// 폭포처럼 UV애니만 쓰는 오브젝트도 FX파이프라인, 환경맵을 가지고 있어도 FX파이프라인을 가지고 있으며
			// 밑에 if(!RpMaterialUVAnimExists(pMaterial))에서 또한번 체크하므로 다음 문장을 주석처리합니다.
			// 환경맵, UV애니등을 가진 오브젝트가 문제가 생긴다면 이부분이 다시 한번 고려가 되야 합니다.
			// 주석을 풀게 되면 UV와 환경맵을 가진 오브젝트를 선택할시 파이프라인이 틀리므로 addcolor되지 않습니다.
			// - 우택
			//if(pAtomic->pipeline == NULL && m_pUVAnim == NULL)
			if(pAtomic->pipeline == NULL)
			{
				RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
				if(pGeom != NULL)
				{
					RwInt32 nNumMat = RpGeometryGetNumMaterials(pGeom);
					for(int i = 0; i < nNumMat; i++)
					{
						RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
						if(pMaterial != NULL)
						{
							if(!RpMaterialUVAnimExists(pMaterial))
								NtlMatExtSetPipeline(pAtomic);
						}
					}

				}
			}
		}

		RwMatrixSetIdentity(RwFrameGetMatrix(RpClumpGetFrame(GetClump())));

		RpNtlClumpSetData( GetClump(), this );

		SetTransform();

		// BBox Update
		m_bbox = GetTriggerAABBInfo();

		CalcBoundingSphere();        
	}

	if(GetNtlResourceManager()->IsLoadScheduling())
	{		
		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			LoadLightmap(NULL);
		}

		AttachLightObject();
	}

	// 블렌드 객체를 생성하고 리스트에 추가한다.
	m_pFadeBlend = GetAlphaBlendController()->AddAlpha(1.0f);

	// Link Effect를 설정한다.
	for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
	{
		SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];        
		m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
	}
	if(!m_pProperty->m_vLinkEffect.empty())
	{
		Update(0.0f);       // 링크이펙트가 있으면 최초에 업데이트를 한번 해줘야 한다. (Effect가 Object보다 Update호출이 빠르기 때문)
	}

	// Sound를 재생한다.
	if(strlen(m_pProperty->GetSoundProp()->chSoundName) > 0)
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_OBJECT_MUSIC;
		tSoundParam.pcFileName		= m_pProperty->GetSoundProp()->chSoundName;
		tSoundParam.bLoop			= true;
		tSoundParam.fVolume			= m_pProperty->GetSoundProp()->fSoundVolume * 0.01f;
		tSoundParam.fPitch			= m_pProperty->GetSoundProp()->fSoundPitchMin;
		tSoundParam.fXPos			= m_vWorldPosition.x;
		tSoundParam.fYPos			= m_vWorldPosition.y;
		tSoundParam.fZPos			= m_vWorldPosition.z;
		tSoundParam.fMinDistance	= m_pProperty->GetSoundProp()->fSoundDist;
		tSoundParam.fMaxDistance	= m_pProperty->GetSoundProp()->fSoundDecayDist;

		GetSoundManager()->Play(&tSoundParam);

		AddLoopSound(tSoundParam.hHandle);
	}

	// 처음 맵에 진입시에 나타나는 오브젝트들은 Fade 효과를 적용하지 않는다.    
	if(m_bLoadMap || (!GetSceneManager()->GetDistanceFilterEnable()))
	{
		m_eFadeState = FADE_VISIBLE;	        
	}
	else
	{
		m_eFadeState = FADE_NOT_VISIBLE;       
		m_pFadeBlend->SetWeight(0.0f);
		SetWeightAlpha(0.0f);
	}

	CreateOccluderProxy();

	UpdateRpSectorOBBCheck();

	m_bActiveUpdate = TRUE;

	NTL_RPROFILE(TRUE);
	//return TRUE;
}

RwBool CNtlPLObject::CreateOccluderProxy()
{
#ifdef dNTL_WORLD_CULLING_NEW
	// +1 == Bounding Sphere Check를 위해서 사용한다.
	return COccluderProxy::CreateOccluderProxy(0.5f, m_vecAtomicList.size() + 1);
#else
	if (m_bHaveAnim)
	{
		return COccluderProxy::CreateOccluderProxy(0.5f, m_vecAtomicList.size());
	}
	else // Animetion 을 가지고 않을 경우는 Entity Bounding Sphere로 처리한다.
	{
		return COccluderProxy::CreateOccluderProxy(0.5f, 1);
	}
#endif

}

RwBool CNtlPLObject::SaveLightmap(FILE* pFile)
{
	RpAtomic*		pCurAtomic;
	RwTexture*		pCurTexture;
	RwInt32			UVCnt;
	RwTexCoords*	pTexCoords4Second;
	RwChar			szAtomName[64] = {0,};
	RwChar			szLMTexName[64] = {0,};
	RwInt32			TotAtomicCnt = m_vecAtomicList.size();
	RwBool			LitmapExisted;

	RwUInt32		uiLtMapObjectFlags = GetLightMapObjectFlags();
	RwUInt32		uiLtMapMaterialFlags = GetLightMapMaterialFlags();

	fwrite(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
	fwrite(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);

	fwrite(&TotAtomicCnt, sizeof(RwInt32), 1, pFile);
	for(RwInt32 i = 0; i < TotAtomicCnt; i++)
	{
		pCurAtomic	= m_vecAtomicList[i]->pData;
		pCurTexture = RpLtMapAtomicGetLightMap(pCurAtomic);

		if(pCurTexture)
			LitmapExisted = TRUE;
		else
			LitmapExisted = FALSE;

		fwrite(&LitmapExisted, sizeof(RwBool), 1, pFile);

		if(LitmapExisted)
		{
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = 0;
				strcpy_s(szAtomName, 64, Helper_AtomicName(pCurAtomic));
				uiLength = strlen(szAtomName);

				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				fwrite(szAtomName, sizeof(RwChar) * uiLength, 1, pFile);

				sprintf_s(szLMTexName, "%u%s", m_uiObjectSerialID, szAtomName);
				uiLength = strlen(szLMTexName);

				CNtlWorldLTManager::GetInstance()->SaveLightmap(pCurTexture, szLMTexName);

				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				fwrite(szLMTexName, sizeof(RwChar) * uiLength, 1, pFile);
			}

			UVCnt = pCurAtomic->geometry->numVertices;
			fwrite(&UVCnt, sizeof(RwInt32), 1, pFile);

			pTexCoords4Second = pCurAtomic->geometry->texCoords[1];
			fwrite(pTexCoords4Second, sizeof(RwTexCoords) * UVCnt, 1, pFile);
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::LoadSchedulingLightmapProp(FILE* pFile)
{
	AllocSchedulingLTProp();

	fread(&m_pSchedulingLMProp->_uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
	fread(&m_pSchedulingLMProp->_uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);

	RwInt32	TotAtomicCnt;
	fread(&TotAtomicCnt, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < TotAtomicCnt; i++)
	{
		RwBool LitmapExisted;
		fread(&LitmapExisted, sizeof(RwBool), 1, pFile);

		if(LitmapExisted)
		{
			sSCHEDULING_LM_ATOM_PROP* psSchedulingLMAtomProp = NTL_NEW sSCHEDULING_LM_ATOM_PROP;


			{
				RwUInt32 uiLength = 0;

				fread(&uiLength, sizeof(RwUInt32), 1, pFile);
				fread(psSchedulingLMAtomProp->_szAtomName, sizeof(RwChar) * uiLength, 1, pFile);
				psSchedulingLMAtomProp->_szAtomName[uiLength] = '\0';
			}	

			{
				RwUInt32 uiLength = 0;

				fread(&uiLength, sizeof(RwUInt32), 1, pFile);
				fread(psSchedulingLMAtomProp->_szTexName, sizeof(RwChar) * uiLength, 1, pFile);
				psSchedulingLMAtomProp->_szTexName[uiLength] = '\0';
			}

			fread(&psSchedulingLMAtomProp->_UVCnt, sizeof(RwInt32), 1, pFile);
			psSchedulingLMAtomProp->_pTexCoords = NTL_NEW RwTexCoords [psSchedulingLMAtomProp->_UVCnt];
			fread(psSchedulingLMAtomProp->_pTexCoords, sizeof(RwTexCoords) * psSchedulingLMAtomProp->_UVCnt, 1, pFile);

			m_pSchedulingLMProp->_vecSchedulingLMAtomProp.push_back(psSchedulingLMAtomProp);
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::LoadLightmap(FILE* pFile)
{
	RwChar			szTexName[64] = {0,};
	RwInt32			UVCnt = 0;
	RwChar			szAtomName[64] = {0,};
	RwInt32			TotAtomicCnt;
	RwBool			LitmapExisted = FALSE;
	RwChar*			pOldAtomicName = NULL;
	RwTexCoords*	pUVDat = NULL;
	RpAtomic*		pCurAtomic = NULL;
	RwTexture*		pCurLitmap = NULL;
	RwChar			szFullPathName[256];
	RwChar			szBPS[64];

	if(GetNtlResourceManager()->IsLoadScheduling())
	{
		if(!m_pSchedulingLMProp)
			return TRUE;

		SetLightMapObjectFlags(m_pSchedulingLMProp->_uiLtMapObjectFlags);
		SetLightMapMaterialFlags(m_pSchedulingLMProp->_uiLtMapMaterialFlags);

		for(RwUInt32 i = 0; i < m_pSchedulingLMProp->_vecSchedulingLMAtomProp.size(); ++i)
		{
			sSCHEDULING_LM_ATOM_PROP* pThe = m_pSchedulingLMProp->_vecSchedulingLMAtomProp[i];

			for(RwUInt32 k = 0; k < m_vecAtomicList.size(); ++k)
			{
				pOldAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
				if(!strcmp(pOldAtomicName, pThe->_szAtomName))
				{
					pCurAtomic = m_vecAtomicList[i]->pData;
					break;
				}
				else
					pCurAtomic = NULL;
			}

			if(pCurAtomic)
			{
				_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

				strcpy_s(szFullPathName, 256, ".\\Texture\\ntlwe\\ltmap\\");
				strcat_s(szFullPathName, 256, szBPS);
				strcat_s(szFullPathName, 256, "\\Objlm\\");

				pCurLitmap = GetNtlResourceManager()->LoadTexture(pThe->_szTexName, szFullPathName);
				DBO_ASSERT(pCurLitmap, "Texture load failed.");
				if (pCurLitmap)
				{
					RwTextureSetFilterMode(pCurLitmap, rwFILTERLINEAR);

					RpLtMapAtomicSetLightMap(pCurAtomic, pCurLitmap);
					RpAtomicSetPipeline(pCurAtomic, RpLtMapGetPlatformAtomicPipeline());

					RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKTEXCOORDS2);
					CopyMemory(pCurAtomic->geometry->texCoords[1], pThe->_pTexCoords, sizeof(RwTexCoords) * pThe->_UVCnt);
					RpGeometryUnlock(pCurAtomic->geometry);
				}
			}
		}
		FreeSchedulingLTProp();
	}
	else
	{
		RwUInt32 uiLtMapObjectFlags;
		RwUInt32 uiLtMapMaterialFlags;

		fread(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
		fread(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);

		SetLightMapObjectFlags(uiLtMapObjectFlags);
		SetLightMapMaterialFlags(uiLtMapMaterialFlags);

		fread(&TotAtomicCnt, sizeof(RwInt32), 1, pFile);

		for(RwInt32 i = 0; i < TotAtomicCnt; i++)
		{
			fread(&LitmapExisted, sizeof(RwBool), 1, pFile);
			if(LitmapExisted)
			{
				{
					RwUInt32 uiLength = 0;

					fread(&uiLength, sizeof(RwUInt32), 1, pFile);
					fread(szAtomName, sizeof(RwChar) * uiLength, 1, pFile);
					szAtomName[uiLength] = '\0';
				}				

				for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
				{
					pOldAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
					if(!strcmp(pOldAtomicName, szAtomName))
					{
						pCurAtomic = m_vecAtomicList[i]->pData;
						break;
					}
					else
						pCurAtomic = NULL;
				}

				NTL_ASSERT(pCurAtomic, "not exist lightmap atomic.");
				if(pCurAtomic)
				{
					//if (sub_417810(v81, (int)a2, v81) & 0x2000 || !(sub_1048890(v81) & 9))
					//	goto LABEL_52;

					{
						RwUInt32 uiLength = 0;
						fread(&uiLength, sizeof(RwUInt32), 1, pFile);
						fread(szTexName, sizeof(RwChar) * uiLength, 1, pFile);
						szTexName[uiLength] = '\0';
					}

					_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

					strcpy_s(szFullPathName, 256, ".\\Texture\\ntlwe\\ltmap\\");
					strcat_s(szFullPathName, 256, szBPS);
					strcat_s(szFullPathName, 256, "\\Objlm\\");

			//		DBO_WARNING_MESSAGE("RAW: " << dGET_WORLD_PARAM()->WorldRAW << ", : " << dGET_WORLD_PARAM()->WorldBSP << ", : " << dGET_WORLD_PARAM()->WorldProjectFolderName << ", : " << dGET_WORLD_PARAM()->CurWorkingFolderName << ", : " << dGET_WORLD_PARAM()->CurDefaultTexPath << ", : " << dGET_WORLD_PARAM()->WorldChar64Buf);

					pCurLitmap = GetNtlResourceManager()->LoadTexture(szTexName, szFullPathName);
					DBO_ASSERT(pCurLitmap, "Texture load failed.");
					RwTextureSetFilterMode(pCurLitmap, rwFILTERLINEAR);

					RpLtMapAtomicSetLightMap(pCurAtomic, pCurLitmap);
					RpAtomicSetPipeline(pCurAtomic, RpLtMapGetPlatformAtomicPipeline());

					fread(&UVCnt, sizeof(RwInt32), 1, pFile);

					RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKTEXCOORDS2);
					fread(pCurAtomic->geometry->texCoords[1], sizeof(RwTexCoords) * UVCnt, 1, pFile);
					RpGeometryUnlock(pCurAtomic->geometry);
				}
				else 
				{
					if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
					{
						fseek(pFile, sizeof(RwChar) * 64, SEEK_CUR);
					}
					else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
					{
						RwUInt32 uiLength = 0;

						fread(&uiLength, sizeof(RwUInt32), 1, pFile);
						fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);												
					}
					fread(&UVCnt, sizeof(RwInt32), 1, pFile);
					fseek(pFile, sizeof(RwTexCoords) * UVCnt, SEEK_CUR);
				}
			}
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::Create( const SPLEntityCreateParam * pParam )
{
	return CreateScheduling(pParam);
}

void CNtlPLObject::DeleteLightmapFile()
{
	if(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		string	strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
		string	strSubPath	= "\\texture\\ntlwe\\indoor\\lightmap\\";
		string	strFileName;
		string	strExt		= ".png";
		RwChar	szAtomName[64];

		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
		{
			strcpy_s(szAtomName, 64, Helper_AtomicName(m_vecAtomicList[i]->pData));

			strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;

			RwChar szIdxTmp[64];
			sprintf_s(szIdxTmp, "%u%s", m_uiObjectSerialID, szAtomName);
			strFileName = szIdxTmp;
			strFullPath = strFullPath + strSubPath + strFileName + strExt;

			if((_access(strFullPath.c_str(), 0)) != -1)
			{
				// The file exists, force it to do write permission
				SetFileAttributes(strFullPath.c_str(), FILE_ATTRIBUTE_ARCHIVE);
				::DeleteFile(strFullPath.c_str());
			}
		}
	}
}

void CNtlPLObject::CheckLightmapFlags()
{
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
		{
			if (!(RpNtlAtomicGetFlag(m_vecAtomicList.at(i)->pData) & NTL_NOT_VISIBLE)) 
			{
				return;
			}
		}
		SetLightMapObjectFlags(rtLTMAPOBJECTNOSHADOW);
	}
}

RwBool CNtlPLObject::LoadLightObjectAttachData(FILE* pFile)
{
	RwInt32 iLightObjectNum = 0;
	fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for (int i = 0; i < iLightObjectNum; i++)
	{
		SLightObjectAttachData* pLightObjectAttachData		= NTL_NEW SLightObjectAttachData;
		pLightObjectAttachData->pPLLightObjectCreateParam	= NTL_NEW SPLLightObjectCreateParam;

		SPLLightObjectCreateParam* pCreateParam = pLightObjectAttachData->pPLLightObjectCreateParam;

		{
			RwUInt32 uiLength = 0;
			fread(&uiLength, sizeof(RwUInt32), 1, pFile);

			fread(pLightObjectAttachData->chBoneName, sizeof(RwChar) * uiLength, 1, pFile);
			pLightObjectAttachData->chBoneName[uiLength] = '\0';
		}
		

		fread(&pCreateParam->rpLightType, sizeof(RpLightType), 1, pFile);

		fread(&pCreateParam->bRealLight, sizeof(RwBool), 1, pFile);

		fread(&pLightObjectAttachData->vPos, sizeof(RwV3d), 1, pFile);
		pCreateParam->pPos = &pLightObjectAttachData->vPos;

		fread(&pCreateParam->vRotate, sizeof(RwV3d), 1, pFile);

		{
			RwRGBA rgba;
			fread(&rgba, sizeof(RwRGBA), 1, pFile);
			RwRGBARealFromRwRGBA(&pCreateParam->rgbafColor, &rgba);
		}

		fread(&pCreateParam->fRadius, sizeof(RwReal), 1, pFile);

		fread(&pCreateParam->fAngle, sizeof(RwReal), 1, pFile);

		// CZ-LO
		fread(&pCreateParam->bDecal, sizeof(RwBool), 1, pFile);

		fread(&pCreateParam->bWorldLight, sizeof(RwBool), 1, pFile);

		m_vecAttachLightParam.push_back(pLightObjectAttachData);
	}

	if(!GetNtlResourceManager()->IsLoadScheduling())
	{
		AttachLightObject();
	}

	return TRUE;
}

void CNtlPLObject::AttachLightObject()
{
	for (RwInt32 i = 0; i < (RwInt32)m_vecAttachLightParam.size(); ++i)
	{
		SLightObjectAttachData* pLightObjectAttachData = m_vecAttachLightParam.at(i);

		CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", pLightObjectAttachData->pPLLightObjectCreateParam));
		DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

		RwBool bReturn = AttachBone(pNtlPLLightObject, pLightObjectAttachData->chBoneName);
		NTL_ASSERT(bReturn, "Attach Bone Failed.");
		if (!bReturn)
		{
#ifndef dNTL_WORLD_TOOL_MODE
			GetSceneManager()->DeleteEntity(pNtlPLLightObject);
#endif
		}

		NTL_DELETE(pLightObjectAttachData->pPLLightObjectCreateParam);
		NTL_DELETE(pLightObjectAttachData);
	}
	m_vecAttachLightParam.clear();
}

RwBool CNtlPLObject::SaveLightObjectAttachData(FILE* pFile)
{
	RwInt32 iLightObjectNum = 0;
	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
		{
			continue;
		}

		if(pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
		{
			++iLightObjectNum;
		}
	}
	fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
		{
			continue;
		}

		if(pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
		{
			CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pPLAttachAttr->pPLEntity;

			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
			{
				fwrite(pPLAttachAttr->chBoneName, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, pFile);
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = strlen(pPLAttachAttr->chBoneName);
				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				fwrite(pPLAttachAttr->chBoneName, sizeof(RwChar) * uiLength, 1, pFile);
			}

			RpLightType eType;
			eType = pLightObject->GetLightType();
			fwrite(&eType, sizeof(RpLightType), 1, pFile);

			RwBool bTemp;
			bTemp = pLightObject->IsRealLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			RwV3d vTemp;
			vTemp = pLightObject->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pLightObject->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			RwRGBAReal rgbafTemp;
			rgbafTemp = pLightObject->GetColor();
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
			{
				fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
			{
				RwRGBA rgba;
				RwRGBAFromRwRGBAReal(&rgba, &rgbafTemp);
				fwrite(&rgba, sizeof(RwRGBA), 1, pFile);
			}

			RwReal fTemp;
			fTemp = pLightObject->GetRadius();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pLightObject->GetAngle();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			bTemp = pLightObject->IsEnabledDecal();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			bTemp = pLightObject->IsEnabledWorldLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);			
		}
	}
	return TRUE;
}

void CNtlPLObject::Destroy(void) 
{
	NTL_FUNCTION("CNtlPLObject::Destroy");

	if (CNtlPLWorldState::GetActiveMiniIndoor() == this)
	{
		CNtlPLWorldState::InitSpace();
	}

	if (m_pObjectType)
	{
		m_pObjectType->Destroy();
		NTL_DELETE(m_pObjectType);
	}

	// delete lightobject 
	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
		{
			continue;
		}

		if(pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
		{
			pPLAttachAttr->pPLEntity->SetAttachParent(NULL);
			GetSceneManager()->DeleteEntity(pPLAttachAttr->pPLEntity);
			pPLAttachAttr->pPLEntity = NULL;
		}
	}

	for (RwInt32 i = 0; i < (RwInt32)m_vecAttachLightParam.size(); ++i)
	{
		SLightObjectAttachData* pLightObjectAttachData = m_vecAttachLightParam.at(i);

		NTL_DELETE(pLightObjectAttachData->pPLLightObjectCreateParam);
		NTL_DELETE(pLightObjectAttachData);
	}
	m_vecAttachLightParam.clear();

	// remove PS map before destroying clumps
	if(GetEnableGenShadowMap())
	{
		ENTITY_ATOMIC_VEC*		_pEAV	= GetAtomicList();
		ENTITY_ATOMIC_VEC_ITER	_The	= _pEAV->begin();

		while(_The != _pEAV->end())
		{
			CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(static_cast<RpAtomic*>((*_The)->pData)));
			NTL_DELETE(pUserDat);
			RpNtlAtomicSetUserDat(static_cast<RpAtomic*>((*_The)->pData), NULL);
			_The++;
		}
	}

	// remove PS map before destroying clumps
	DestroyEmblemMap();

	m_vecNtlWorldShadow.clear();

	ClearLoopEffect();
	ClearLoopSound();

	NTL_DELETE(m_pUVAnim);	

	if(m_pAnimResource)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pAnimResource);
		m_pAnimResource = NULL;
	}

	if(m_pBaseHierarchy)
	{
		RpHAnimHierarchyDestroy(m_pBaseHierarchy);
		m_pBaseHierarchy = NULL;
	}

	// Toon
	if(m_pToonData)
	{
		RpToonPaintDestroy(m_pToonData->pToonPaint);
		RpToonInkDestroy(m_pToonData->pToonInk);
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pToonData->pTexture);
		NTL_DELETE(m_pToonData);
		m_pToonData = NULL;
	}

	if( m_pClumpResource != NULL )
	{
		Helper_DestroyAllAtomics(&m_vecAtomicList);
		RemoveWorld();		
	}

	// 여기서는 m_pClumpResource의 null pointer를 검사하지 않는다(형석)
	CNtlPLResourceManager::GetInstance()->UnLoadScheduling(this, m_pClumpResource );
	m_pClumpResource = NULL;

	if(m_pInstanceAnimTable)
	{
		m_pInstanceAnimTable->Destroy();
		NTL_DELETE(m_pInstanceAnimTable);
	}

	if(m_pAnimLayer)
	{
		m_pAnimLayer->Destroy();
		NTL_DELETE(m_pAnimLayer);
	}

	DestroyOccluderProxy();

	NTL_RETURNVOID();
}

RwBool CNtlPLObject::CheckUVsSetUpForLightMapping()
{
	RwUInt32	axes[3];
	RwBool		outOfRange = FALSE;
	RwBool		unEqual = FALSE;

	for(RwUInt32 j = 0; j < m_vecAtomicList.size(); ++j)
	{
		RpGeometry*		geom		= m_vecAtomicList[j]->pData->geometry;

		if (RpGeometryGetNumTexCoordSets(geom) > 1)
		{
			RwTexCoords*	texCoords	= RpGeometryGetVertexTexCoords(geom, (RwTextureCoordinateIndex)2);
			RpTriangle*		triangles	= RpGeometryGetTriangles(geom);

			for(RwInt32 k = 0; k < RpGeometryGetNumTriangles(geom); ++k)
			{
				for(RwUInt32 n = 0; n < 3; n++)
				{
					axes[n] = RwFastRealToUInt32(10.0001f *	texCoords[triangles[k].vertIndex[n]].u);
					if(axes[n] > 5)
					{
						outOfRange = TRUE;
					}
				}

				// Make sure UVs are the same for all vertices in a triangle
				if((axes[0] != axes[1]) || (axes[1] != axes[2]))
				{
					unEqual = TRUE;
				}
			}
		}
	}

	if(outOfRange || unEqual)
		return FALSE;

	return TRUE;
}

void CNtlPLObject::ResetUV()
{
	for(RwUInt32 j = 0; j < m_vecAtomicList.size(); ++j)
	{
		RpGeometry* geom = m_vecAtomicList[j]->pData->geometry;

		RpGeometryLock(geom, rpGEOMETRYLOCKTEXCOORDS);
		RwTexCoords* texCoords = RpGeometryGetVertexTexCoords(geom, (RwTextureCoordinateIndex)2);

		for (RwUInt32 k = 0;k < (RwUInt32) RpGeometryGetNumVertices(geom);k++)
		{
			LtMapIntUV_  intUVs;

			/* NOTE: we use a union as when calculating the UVs
			* (see comment there for why) */
			intUVs.uv = texCoords[k];

			intUVs.uv.u = 0.1f * (intUVs.intUV[0] & 0x7);
			intUVs.uv.v = 0.1f * (intUVs.intUV[1] & 0x7);
			texCoords[k] = intUVs.uv;
		}

		RpGeometryUnlock(geom);
	}
}

RwBool CNtlPLObject::Update( RwReal fElapsed )
{
	NTL_SPROFILE("CNtlPLObject::Update");

	if (m_pObjectType)
	{
		m_pObjectType->Update(fElapsed);
	}

	// 트리거 애니메이션이 세팅되면, 생성될때까지 Time은 진행시킨다.
	if(!IsSchedulingLoadingComplete())
	{
		if(m_sScheduleInfo.uiAnimKey > 0)
		{
			m_sScheduleInfo.fAnimStartTime += fElapsed;
		}

		NTL_RPROFILE(TRUE);
	}

	if(!m_bActiveUpdate)
		NTL_RPROFILE(TRUE);

	if(m_pUVAnim)
	{
		m_pUVAnim->Update(fElapsed);
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OBJECT_ANIMATION))
#endif
	{
		if(m_bAnimPlay && m_pBaseHierarchy)
		{
			if(m_pAnimLayer)
			{
				m_pAnimLayer->Update(fElapsed * m_fAnimSpeed);
				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
			}
			else
			{
				RpHAnimHierarchyAddAnimTime(m_pBaseHierarchy, fElapsed * m_fAnimSpeed);        
				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
			}
		}
	}

	if(m_eFadeState != FADE_NOT_VISIBLE && m_eFadeState != FADE_VISIBLE)
	{
		UpdateFading(fElapsed);
	}

	CNtlPLAttach::Update(fElapsed);	

	NTL_RPROFILE(TRUE);
}


RwBool CNtlPLObject::SetThreadSafeProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLObject::SetThreadSafeProperty");

	m_pProperty = static_cast<CNtlPLObjectProperty*>(const_cast<CNtlPLProperty*>(pData));

	if(m_pProperty->m_strAnimFileName.size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
		{
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
		}
	}

	if(m_pProperty->m_strUVAnimFileName.size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
	}

	if(m_pProperty->m_vLinkEffect.size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
	}

	//  트리거 애니메이션이 있으면 Update Flag를 켠다
	if(m_pProperty->GetAnimTable()->GetTypeAnimMap()->size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
	}

	m_bbox = GetTriggerAABBInfo();

	// Anim Table 
	m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
	m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());

	NTL_RETURN(TRUE);
}

RwBool CNtlPLObject::SetProperty(const CNtlPLProperty *pData)
{
	return SetThreadSafeProperty(pData);
}


void CNtlPLObject::AddWorld(void)
{
	if( CNtlPLGlobal::m_pRpWorld == NULL )
		return;

	RpClump *pClump = GetClump();
	if(pClump != NULL)
	{
		if(RpClumpGetWorld(pClump) == NULL)
			RpWorldAddClump( CNtlPLGlobal::m_pRpWorld, pClump );
	}
}

void CNtlPLObject::RemoveWorld(void)
{
	if( CNtlPLGlobal::m_pRpWorld == NULL )
		return;

	RpClump *pClump = GetClump();
	if( pClump != NULL )
	{
		if( RpClumpGetWorld(pClump) != NULL)
			RpWorldRemoveClump( CNtlPLGlobal::m_pRpWorld,pClump );
	}
}

void CNtlPLObject::CallPreSchedulingResource(void)
{
	// Applied if there is a set UVAnim (must be loaded before Clump is loaded)
	if(m_pProperty->m_strUVAnimFileName.size())
		SetUVAnim(m_pProperty->m_strUVAnimFileName.c_str());
}

void CNtlPLObject::CallSchedulingResource(CNtlPLResource *pResource)
{
	NTL_FUNCTION("CNtlPLObject::CallSchedulingResource");

	m_pClumpResource = pResource;
	if(m_pClumpResource == NULL)
		NTL_RETURNVOID();

	AddWorld();

	CreateThreadSafe();

	SetTransform();
	SetAlpha(m_sColor.alpha);

	// 로딩이 완료되었음을 이벤트로 쏜다.
	CNtlPLEventGenerator::CreateEventThreadLoading(GetSerialID());

	NTL_RETURNVOID();
}

void CNtlPLObject::CallSchedulingResourceOnly()
{
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		SetAtomicPSMap();
	}
}

void CNtlPLObject::SetPosition(const RwV3d *pPos)
{
	m_vWorldPosition = *pPos;
	if(m_pClumpResource == NULL)
		return;

	RwFrame *pFrame = RpClumpGetFrame( GetClump() );  

	RwMatrix *pMat = RwFrameGetMatrix( pFrame ); 
	RwV3d *pOriPos = RwMatrixGetPos ( pMat );
	RwV3dAssign(pOriPos, pPos); 
	RwFrameTranslate(pFrame, &CNtlPLGlobal::m_vZeroV3, rwCOMBINEPOSTCONCAT); 

	// BBox Update
	m_bbox = GetTriggerAABBInfo();

	UpdateLoopSoundPos();

	CalcBoundingSphere();

	UpdateRpSectorOBBCheck();
}	

void CNtlPLObject::SetRotate( RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
{
	m_vModelAngle.x = fXAngle;
	m_vModelAngle.y = fYAngle;
	m_vModelAngle.z = fZAngle;

	if(m_pClumpResource == NULL)
		return;

	SetTransform();

	// BBox Update
	m_bbox = GetTriggerAABBInfo();

	UpdateRpSectorOBBCheck();
}

RwV3d CNtlPLObject::GetPosition(void)
{
	if(m_pClumpResource == NULL)
		return m_vWorldPosition;

	RwFrame *pFrame = RpClumpGetFrame( GetClump() ); 
	return *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
}


void CNtlPLObject::SetScale( const RwV3d * pScale )
{
	memcpy( &m_vModelScale, pScale, sizeof( RwV3d ) );

	if(m_pClumpResource == NULL)
		return;

	SetTransform();

	// BBox Update
	m_bbox = GetTriggerAABBInfo();

	CalcBoundingSphere();

	UpdateRpSectorOBBCheck();
}


const RwV3d * CNtlPLObject::GetScale( void ) const
{
	return &m_vModelScale;
}

const RwV3d* CNtlPLObject::GetRotate(void) const 
{
	return &m_vModelAngle;
}

RwBool	CNtlPLObject::GetEnableShadow()
{
	return (GetFlags() & NTL_PLEFLAG_SHADOW);
}

VOID CNtlPLObject::DeleteDoodadShadow()
{
	if(m_vecAtomicList.size() == 0)
	{
		return;
	}

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			CNtlWorldShadow* pNtlWorldShadow = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(pAtomic));
			NTL_DELETE(pNtlWorldShadow);
			RpNtlAtomicSetUserDat(pAtomic, NULL);
		}
	}
}

RwBool CNtlPLObject::AreThereObjPSMap()
{
	if(m_vecAtomicList.size() == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(pAtomic));
			if(pUserDat && pUserDat->m_pTexShadow)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool CNtlPLObject::GetEnableGenShadowMap()
{
	if(m_vecAtomicList.size() == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			if( RpNtlAtomicGetFlag(pAtomic) & NTL_SHADOW_MAP )
				return TRUE;
		}
	}

	return FALSE;
}

RwBool CNtlPLObject::GetEmblemMarkEnabled()
{
	if(m_vecAtomicList.size() == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			if((RpNtlAtomicGetFlag(pAtomic) & NTL_EMBLEM_MARK) && RpNtlAtomicGetUserDat4RwTex(pAtomic))
				return TRUE;
		}
	}

	return FALSE;
}

ENTITY_ATOMIC_VEC *CNtlPLObject::GetAtomicList()
{
	return &m_vecAtomicList;
}

RwBool CNtlPLObject::SetAtomicEmblemMap(RwTexture* _pEmblemMap)
{
	DBO_ASSERT(_pEmblemMap, "CNtlPLObject::SetAtomicEmblemMap(RwTexture* _pEmblemMap), _pEmblemMap is NULL");

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
	{
		if(RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData) & NTL_EMBLEM_MARK)
		{
			RwTexture* pEmblemMarkTex = RpNtlAtomicGetUserDat4RwTex(m_vecAtomicList[i]->pData);
			if(pEmblemMarkTex)
				GetEmblemMaker()->DestoyEmblem(pEmblemMarkTex);

			RpNtlAtomicSetUserDat4RwTex(m_vecAtomicList[i]->pData, _pEmblemMap);
		}
	}

	return TRUE;
}

void CNtlPLObject::DestroyEmblemMap()
{
	if(GetEmblemMarkEnabled())
	{
		ENTITY_ATOMIC_VEC*		_pEAV	= GetAtomicList();
		ENTITY_ATOMIC_VEC_ITER	_The	= _pEAV->begin();

		while(_The != _pEAV->end())
		{
			RwTexture* pUserDat = static_cast<RwTexture*>(RpNtlAtomicGetUserDat(static_cast<RpAtomic*>((*_The)->pData)));
			if(pUserDat)
			{
				GetEmblemMaker()->DestoyEmblem(pUserDat);
				RpNtlAtomicSetUserDat(static_cast<RpAtomic*>((*_The)->pData), NULL);
			}

			_The++;
		}
	}
}

VOID CNtlPLObject::SetAtomicPSMap()
{
	for(RwUInt32 k = 0; k < m_vecNtlWorldShadow.size(); ++k)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
		{
			if(!strcmp(Helper_AtomicName(m_vecAtomicList[i]->pData), m_vecNtlWorldShadow[k]->m_pTexShadow->name))
			{
				RpNtlAtomicSetUserDat(m_vecAtomicList[i]->pData, m_vecNtlWorldShadow[k]);
				break;
			}
		}
	}

	m_vecNtlWorldShadow.clear();
}

void CNtlPLObject::SetTransform( void )
{
	NTL_FUNCTION("CNtlPLObject::SetTransform");

	RwFrame *		pFrame = RpClumpGetFrame( GetClump() ); 
	RwMatrix *		pMatrix = RwFrameGetMatrix( pFrame );

	RwMatrix		Matrix;
	RwV3d			Front, Left, Up;

	RwMatrixRotate( &Matrix, &CNtlPLGlobal::m_vYAxisV3, m_vModelAngle.y, rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &CNtlPLGlobal::m_vZAxisV3, &Matrix );
	RwV3dTransformVector( &Left, &CNtlPLGlobal::m_vXAxisV3, &Matrix );

	RwMatrixRotate( &Matrix, &Left, m_vModelAngle.x, rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &Front, &Matrix );
	RwV3dTransformVector( &Up, &CNtlPLGlobal::m_vYAxisV3, &Matrix );

	RwMatrixScale( pMatrix, &m_vModelScale, rwCOMBINEREPLACE );
	RwMatrixRotate( pMatrix, &CNtlPLGlobal::m_vYAxisV3, m_vModelAngle.y, rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Left, m_vModelAngle.x, rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Front, m_vModelAngle.z, rwCOMBINEPOSTCONCAT );
	RwMatrixTranslate( pMatrix, &m_vWorldPosition, rwCOMBINEPOSTCONCAT );

	RwFrameUpdateObjects( pFrame );

	NTL_RETURNVOID();
}

RwFrame * CNtlPLObject::GetFrame( void )
{
	if( GetClump() == NULL )
	{
		return NULL;
	}

	return RpClumpGetFrame( GetClump() );
}

void CNtlPLObject::SetMatrix( RwMatrix & matWorld )
{
	RwFrame *		pFrame = GetFrame();

	if( pFrame == NULL )
	{
		return;
	}

	if( RwFrameGetMatrix( pFrame ) == NULL )
	{
		return;
	}

	RwMatrixCopy( RwFrameGetMatrix( pFrame ), &matWorld );

	// 위치값을 저장한다.
	m_vWorldPosition = *RwMatrixGetPos(&matWorld);

	RwFrameUpdateObjects( pFrame );

	UpdateLoopSoundPos();

	return;
}


RwMatrix & CNtlPLObject::GetMatrix( void )
{
	RwFrame *		pFrame = GetFrame();

	if( pFrame == NULL )
	{
		return CNtlPLEntity::GetMatrix();
	}

	return *RwFrameGetMatrix( pFrame );
}


RwBool CNtlPLObject::SetUVAnim( const RwChar* szUVAnimFileName ) 
{
	if(!szUVAnimFileName)
		return FALSE;

	if(!m_pUVAnim)
	{
		m_pUVAnim = NTL_NEW CNtlPLUVAnim();
	}

	// NOTE: UVAnim은 Clump가 Load 되기전에 호출되어야만 적용된다.
	return m_pUVAnim->Create(szUVAnimFileName);    
}

/**
* 애니메이션 리소스및 객체를 생성하고, 오브젝트에 적용한다.
* \param szAnimFileName 적용할 AnimFileName (*.anm)
* return 성공 유무
*/
RwBool CNtlPLObject::SetAnimation( const RwChar* szAnimFileName ) 
{
	NTL_FUNCTION(__FUNCTION__);

	if(!GetClump())
		NTL_RETURN(FALSE);

	if(m_pAnimResource == NULL)
	{
		if(!CreateAnim())
			NTL_RETURN(FALSE);
	}
	else
	{
		// 로딩된 Anim을 메모리에서 제거한다.
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pAnimResource);
		m_pAnimResource = NULL;
	}

	if(szAnimFileName == NULL)
	{
		RpHAnimHierarchyDetach(m_pBaseHierarchy);
		m_bAnimPlay = FALSE;
	}
	else
	{
		m_pAnimResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(szAnimFileName);
		if(!m_pAnimResource)
			return FALSE;

		RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, m_pAnimResource->GetAnimation());
		m_bAnimPlay = TRUE;

		RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLObject::SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime /* = 0.0f */, RwBool bLoop /* = TRUE */)
{
	if (m_uiCurAnimKey == uiAnimKey)
		return FALSE;

    m_uiCurAnimKey = uiAnimKey;

	if(!IsSchedulingLoadingComplete())
	{
		m_sScheduleInfo.uiAnimKey = uiAnimKey;
		m_sScheduleInfo.fAnimStartTime = fStartTime;
		m_sScheduleInfo.bAnimLoop = bLoop;

		return FALSE;
	}

	// Create an AnimTable if it is not already created.
	if(!m_pInstanceAnimTable)
	{
		m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
		m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());
	}

	SInstanceAnimData* pInstanceAnimData = m_pInstanceAnimTable->Get(uiAnimKey);
	if (!pInstanceAnimData)
	{
		DBO_WARNING_MESSAGE("pInstanceAnimData == NULL uiAnimKey: " << uiAnimKey);
		NTL_RETURN(FALSE);
	}

	// Instance Anim Table 생성
	if(!m_pAnimLayer)
	{
		CreateAnim();

		// AnimLayer 객체를 만들기 위해서는 우선 애니메이션이 적용되어 있어야 한다.
		RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, pInstanceAnimData->pResource->GetAnimation());
		RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);

		m_pAnimLayer = NTL_NEW CNtlAnimLayer();
		if (!m_pAnimLayer->Create(m_pBaseHierarchy))
			NTL_RETURN(FALSE);

		m_pAnimLayer->SetCallBack(this, &CNtlPLObject::CallBackBaseAnim);
	}

	m_bAnimPlay = TRUE;

	ClearLoopEffect();
	ClearLoopSound();
	

	// Play time을 0 ~ 1 사이 값으로 조정한다
	RwReal fPlayTime = fStartTime;
	fPlayTime = (FLT_MAX == fPlayTime ? 0.f : fPlayTime);
	fPlayTime = fabs(fPlayTime);

	if(fPlayTime > 0.0f)
	{
		fPlayTime = fPlayTime / m_pAnimLayer->GetDurationAnimTime();
		fPlayTime -= floor(fPlayTime);
	}

	return m_pAnimLayer->SetAnimation(pInstanceAnimData, fPlayTime, bLoop, FALSE);
}

RwBool CNtlPLObject::CreateAnim() 
{
	m_pBaseHierarchy = Helper_GetHierarchyClump(GetClump());
	if(!m_pBaseHierarchy)
		return FALSE;

	Helper_SetHierarchyClumpSkin(GetClump(), m_pBaseHierarchy);

	RpHAnimHierarchySetFlags( m_pBaseHierarchy,
		(RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pBaseHierarchy) | 
		rpHANIMHIERARCHYUPDATELTMS | 
		rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));

	RpHAnimHierarchyAttach(m_pBaseHierarchy);		

	return TRUE;
}

RwSphere* CNtlPLObject::GetBoundingSphere()
{
	m_BSphereCur.center = GetPosition() + m_BSphere.center;
	m_BSphereCur.radius	= m_BSphere.radius;

	return &m_BSphereCur;
}

void CNtlPLObject::CalcBoundingSphere()
{
	m_BSphere.center = GetPosition();
	m_BSphere.radius = 0.0f;

	for (RwInt32 iAtomicCnt = 0; iAtomicCnt < (RwInt32)m_vecAtomicList.size(); ++iAtomicCnt)
	{
		RpAtomic*	pAtomic		= m_vecAtomicList.at(iAtomicCnt)->pData;
		RwSphere*	pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pAtomic));

		m_BSphere.center = (m_BSphere.center + pCurSphere->center) * 0.5f;
		m_BSphere.radius = RwV3dLength(&((m_BSphere.center - pCurSphere->center) * 0.5f)) + (m_BSphere.radius < pCurSphere->radius ? pCurSphere->radius : m_BSphere.radius);
	}

	m_BSphere.center = m_BSphere.center - GetPosition();
}

// GetPosition으로 업데이트가 힘든 문제점으로 변경함.
// 문제가 될 경우 다시 복원한다.
// RwSphere* CNtlPLObject::GetBoundingSphere()
// {
// 	return &m_BSphere;
// }
// 
// void CNtlPLObject::CalcBoundingSphere()
// {
// 	if (GetClump()
// 	if(m_pClumpResource == NULL)
// 		return;
// 
// 	RwSphere*	pCurSphere;
// 	RpClump*	pClump			= GetClump();
// 	RwLLLink*	pCur;
// 	RwLLLink*	pEnd;
// 	RwLLLink*	pNext;
// 	RpAtomic*	pCurAtomic;
// 	RwInt32		NumSphere = 0;
// 	RpAtomic*	pAtomicOfMaxRadius;
// 	RwReal		LenMaxRadius;
// 
// 	m_BSphere.center.x	= 0.0f;
// 	m_BSphere.center.y	= 0.0f;
// 	m_BSphere.center.z	= 0.0f;
// 	m_BSphere.radius	= 0.0f;
// 
// 	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
// 	pEnd = rwLinkListGetTerminator(&pClump->atomicList);
// 
// 	while(pCur != pEnd)
// 	{
// 		pCurAtomic	= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
// 		pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pCurAtomic));
// 
// 		if(!NumSphere)
// 		{
// 			LenMaxRadius		= pCurSphere->radius;
// 			pAtomicOfMaxRadius	= pCurAtomic;
// 
// 			CNtlMath::MathRwV3dAssign(&m_BSphere.center, pCurSphere->center.x, pCurSphere->center.y, pCurSphere->center.z);
// 		}
// 		else
// 		{
// 			if(LenMaxRadius < pCurSphere->radius)
// 			{
// 				LenMaxRadius		= pCurSphere->radius;
// 				pAtomicOfMaxRadius	= pCurAtomic;
// 			}
// 
// 			m_BSphere.center.x += pCurSphere->center.x;
// 			m_BSphere.center.y += pCurSphere->center.y;
// 			m_BSphere.center.z += pCurSphere->center.z;
// 		}
// 
// 		pNext	= rwLLLinkGetNext(pCur);
// 		pCur	= pNext;
// 
// 		NumSphere++;
// 	}
// 
// 	m_BSphere.center.x /= NumSphere;
// 	m_BSphere.center.y /= NumSphere;
// 	m_BSphere.center.z /= NumSphere;
// 	m_BSphere.radius	= LenMaxRadius + sqrtf((m_BSphere.center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) * (m_BSphere.center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) + (m_BSphere.center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) * (m_BSphere.center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) + (m_BSphere.center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z) * (m_BSphere.center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z));
// }

RwBool CNtlPLObject::RenderToTexture()
{
	if(m_pClumpResource == NULL)
		return TRUE;

	RpClump*	pClump = GetClump();
	RwLLLink*	pCur;
	RwLLLink*	pEnd;
	RwLLLink*	pNext;
	RpAtomic*	pCurAtomicInClump;

	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pClump->atomicList);

	BeginFogState(FALSE);

	while(pCur != pEnd)
	{	
		pCurAtomicInClump = rwLLLinkGetData(pCur, RpAtomic, inClumpLink);

		RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pCurAtomicInClump);
		if((uiRenderFlags & NTL_ALPHATEST) || (uiRenderFlags & NTL_ALPHA))
		{
			if(uiRenderFlags & NTL_ALPHATEST)
				BeginNtlAotmicAlphaTestStateShadow();
			if(uiRenderFlags & NTL_TWOSIDE)
				BeginNtlAtomicTwoSideState();

			pCurAtomicInClump->renderCallBack(pCurAtomicInClump);

			if(uiRenderFlags & NTL_TWOSIDE)
				EndNtlAtomicTwoSideState();
			if(uiRenderFlags & NTL_ALPHATEST)
				EndNtlAtomicAlphaTestState();
		}
		else
		{
			pCurAtomicInClump->renderCallBack(pCurAtomicInClump);
		}

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	CNtlPLAttach::RenderToTexture();

	EndFogState();

	return TRUE;
}

RwBool CNtlPLObject::Render()
{
	NTL_SPROFILE("CNtlPLObject::Render");

	if(m_pClumpResource == NULL)
		NTL_RPROFILE(TRUE);

	if(!GetEnableShadow() || !this->IsVisible())
		NTL_RPROFILE(FALSE);

	RpClump*					pClump = GetClump();
	RwLLLink*					pCur;
	RwLLLink*					pEnd;
	RwLLLink*					pNext;
	RpAtomic*					pCurAtomicInClump;

	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pClump->atomicList);

	while(pCur != pEnd)
	{	
		pCurAtomicInClump = rwLLLinkGetData(pCur, RpAtomic, inClumpLink);

		RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pCurAtomicInClump);

		if(!(uiRenderFlags & NTL_NOT_VISIBLE))
		{
			if((uiRenderFlags & NTL_ALPHATEST) || (uiRenderFlags & NTL_ALPHA))
			{
				if(uiRenderFlags & NTL_ALPHATEST)
					BeginNtlAotmicAlphaTestStateShadow();
				if(uiRenderFlags & NTL_TWOSIDE)
					BeginNtlAtomicTwoSideState();

				pCurAtomicInClump->renderCallBack(pCurAtomicInClump);

				if(uiRenderFlags & NTL_TWOSIDE)
					EndNtlAtomicTwoSideState();
				if(uiRenderFlags & NTL_ALPHATEST)
					EndNtlAtomicAlphaTestState();
			}
			else
			{
				pCurAtomicInClump->renderCallBack(pCurAtomicInClump);
			}
		}

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	NTL_RPROFILE(TRUE);
}

CNtlInstanceEffect* CNtlPLObject::AttachLinkEffect(SEventLinkEffect* pEventLinkEffect)
{
	if(!pEventLinkEffect)
		return FALSE;

	return m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
}

RwBool CNtlPLObject::DetachLinkEffect(CNtlInstanceEffect* pLinkEffect)
{
	if(!pLinkEffect)
		return FALSE;

	return m_LinkEffectInstance.DetachLinkEffect(this, pLinkEffect);


}

void CNtlPLObject::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sColor.red	= byRed;
	m_sColor.green	= byGreen;
	m_sColor.blue	= byBlue;
}

void CNtlPLObject::SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sAddColor.red		= byRed;
	m_sAddColor.green	= byGreen;
	m_sAddColor.blue	= byBlue;
}

RwBool CNtlPLObject::SetPlayAnimTime(RwReal fStartAnimTime)
{
	if(m_pClumpResource == NULL)
		return TRUE;

	if(!m_pBaseHierarchy)
		return FALSE;

	if(!m_pBaseHierarchy->currentAnim)
		return FALSE;

	if(fStartAnimTime < 0.f || fStartAnimTime > 1.f)
		return FALSE;

	RwReal fDurationTime = m_pBaseHierarchy->currentAnim->pCurrentAnim->duration;
	RpHAnimHierarchySetCurrentAnimTime(m_pBaseHierarchy, fDurationTime * fStartAnimTime);

	return TRUE;
}

RwBool CNtlPLObject::SetPlayAnimSpeed(RwReal fAnimSpeed)
{
	if(fAnimSpeed < 0.f)
		return FALSE;
	m_fAnimSpeed = fAnimSpeed;

	return TRUE;
}


RwBBox CNtlPLObject::GetTriggerAABBInfo() 
{
	return GetTriggerAABBInfo(GetPosition(), *GetRotate(), *GetScale()); 
}

RwBBox CNtlPLObject::GetDefaultAABB() 
{
	RwBBox bbox;
	if (m_pProperty->GetBBox())
	{
		bbox = *m_pProperty->GetBBox();
	}
	else
	{
		bbox = CreateDefaultAABB();
	}
	return bbox;
}

RwBBox CNtlPLObject::GetTriggerAABBInfo( const RwV3d& vPos, const RwV3d& vRotate, const RwV3d& vScale ) 
{
	RwBBox bboxOut;
	ZeroMemory(&bboxOut, sizeof(RwBBox));

	const RwBBox* pBBox = m_pProperty->GetBBox();
	if(!pBBox)
		return bboxOut;

	// 프로퍼티에 설정된 바운딩 박스가 없으면 계산해서 설정한다.
	if(RwV3dLength(&(pBBox->sup)) == 0)
	{
		m_pProperty->SetBBox(CreateDefaultAABB());        
		pBBox = m_pProperty->GetBBox();
		//return bboxOut;
	}

	RwV3d vertsIn[8], vertsOut[8];

	vertsIn[0] = pBBox->inf;
	vertsIn[1] = pBBox->inf;
	vertsIn[1].z = pBBox->sup.z;
	vertsIn[2] = vertsIn[1];
	vertsIn[2].x = pBBox->sup.x;
	vertsIn[3] = pBBox->inf;
	vertsIn[3].x = pBBox->sup .x;

	for(int i = 4; i < 8; ++i)
	{
		vertsIn[i] = vertsIn[i - 4];
		vertsIn[i].y = pBBox->sup.y;
	}

	RwMatrix matLtm;    
	RwMatrixSetIdentity(&matLtm);
	RwMatrixScale(&matLtm, &vScale, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &XAxis, vRotate.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &YAxis, vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &ZAxis, vRotate.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&matLtm, &vPos, rwCOMBINEPOSTCONCAT);

	RwV3dTransformPoints(vertsOut, vertsIn, 8, &matLtm);

	RwBBoxCalculate(&bboxOut, vertsOut, 8);

	return bboxOut;
}

static RpAtomic* GetAABB(RpAtomic* pAtomic, void* data)
{
	RwBBox* pBBox = (RwBBox*)data;

	RpGeometry* pGeom = RpAtomicGetGeometry(pAtomic);
	RwInt32 numVerts = RpGeometryGetNumVertices(pGeom);

	RwV3d* vertIn = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(pGeom, 0));

	for(int i = 0; i < numVerts; ++i)
	{
		RwBBoxAddPoint(pBBox, &vertIn[i]);
	}

	return pAtomic;
}

RwBBox CNtlPLObject::CreateDefaultAABB() 
{
	RwBBox bBox;

	if(GetClump() == NULL)
	{
		memset(&bBox, 0, sizeof(RwBBox));
		return bBox;
	}

	bBox.sup.x = bBox.sup.y = bBox.sup.z = RwRealMINVAL;
	bBox.inf.x = bBox.inf.y = bBox.inf.z = RwRealMAXVAL;

	RpClumpForAllAtomics(GetClump(), GetAABB, (void*)&bBox);

	return bBox;
}

RwBool CNtlPLObject::UpdateFadeSystem() 
{
	if(!CNtlPLGlobal::m_RwCamera)
		return FALSE;

	if(!(GetFlags() & NTL_PLEFLAG_FADE))
	{
		return FALSE;
	}

	if(m_eFadeState == FADE_NOT_VISIBLE || m_eFadeState == FADE_VISIBLE)
	{
		// 카메라와의 거리를 체크한다. (높이는 체크하지 않는다)
		RwFrame* pFrame = RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
		RwV3d vPosCamera = *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
		RwV3d vPosObject = GetPosition();
		vPosCamera.y = vPosObject.y = 0.0f;
		RwV3d vDistCamera = vPosCamera - vPosObject;
		RwReal fDistCamera = RwV3dLength(&vDistCamera);

		// Fade Out         
		if( m_eFadeState == FADE_VISIBLE && fDistCamera >= GetVisibleCullingDistance())
		{
			m_eFadeState = FADE_OUT_OBJECT;            // 사라질때는 object부터 사라진다 (이펙트는 남아있다)
			m_fFadeTime = 0.0f;

			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
				RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
			}

			AddSceneUpdate();

			// 	 		if(!m_bAnimPlay)            // 애니메이션이 적용안된 오브젝트는 update가 호출되지 않기때문에 Update를 호출할수있도록 추가한다.
			//   			{
			//   				GetSceneManager()->AddUpdate(this);
			// 	  		}
		}
		// Fade In
		else if(m_eFadeState == FADE_NOT_VISIBLE && fDistCamera <= GetVisibleCullingDistance())
		{
			//m_eFadeState = FADE_IN_EFFECT;             // 나타날때는 Effect부터 나타난다 (오브젝트는 차후에 나타난다)            
			/// -- Fade In 될때는 오브젝트와 이펙트가 같이 나타난다 (2007.7.31 by agebreak - 민석씨 요구사항)
			m_eFadeState = FADE_IN_OBJECT;
			m_fFadeTime = 0.0f;

			SetEffectVisible(TRUE);     			            

			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
				RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
			}

			AddSceneUpdate();

			// 			if(!m_bAnimPlay)
			// 			{
			// 				GetSceneManager()->AddUpdate(this);
			// 			}
		}

		m_fPrevCameraDistance = fDistCamera;
	}
	return TRUE;
}

RwBool CNtlPLObject::UpdateFading(RwReal fElapsedTime)
{
	NTL_SPROFILE("CNtlPLObject::UpdateFading");

	m_fFadeTime += fElapsedTime;

	switch(m_eFadeState)
	{
	case FADE_IN_EFFECT:
		if(m_fFadeTime >= FADE_EFFECT_GAP_TIME)
		{            
			m_eFadeState = FADE_IN_OBJECT;
			m_fFadeTime = 0.0f;
		}
		break;
	case FADE_IN_OBJECT:
		if(m_fFadeTime >= CNtlPLGlobal::m_fDistFiterObjFrequency)
		{
			m_eFadeState = FADE_VISIBLE;
			m_fFadeTime = 0.0f;            
			m_pFadeBlend->SetWeight(1.0f);            

			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
				RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag &(~NTL_RUNTIME_ALPHA));
			}

			RemoveSceneUpdate();

			// 			if(!m_bAnimPlay)
			// 			{
			// 				GetSceneManager()->RemoveUpdate(this);
			// 			}
		}
		else
		{
			// Alpha를 0%->100%로 변화시킨다. 
			m_pFadeBlend->SetWeight(m_fFadeTime / CNtlPLGlobal::m_fDistFiterObjFrequency);            
		}
		break;
	case FADE_OUT_EFFECT:
		if(m_fFadeTime >= FADE_EFFECT_GAP_TIME)
		{
			SetEffectVisible(FALSE);
			m_eFadeState = FADE_NOT_VISIBLE;
			m_fFadeTime = 0.0f;

			RemoveSceneUpdate();

			// 			if(!m_bAnimPlay)
			// 			{
			// 				GetSceneManager()->RemoveUpdate(this);
			// 			}
		}            
		break;
	case FADE_OUT_OBJECT:
		if(m_fFadeTime >= CNtlPLGlobal::m_fDistFiterObjFrequency)
		{
			m_pFadeBlend->SetWeight(0.0f);            
			m_eFadeState = FADE_OUT_EFFECT;
			m_fFadeTime = 0.0f;
		}            
		else
		{
			// Alpha를 100%->0%로 변환시칸다.           
			m_pFadeBlend->SetWeight((CNtlPLGlobal::m_fDistFiterObjFrequency - m_fFadeTime) / CNtlPLGlobal::m_fDistFiterObjFrequency);            
		}   
		break;
	}

	NTL_RPROFILE(TRUE);
}

void CNtlPLObject::SetVisible( RwBool bVisible ) 
{
	CNtlPLAttach::SetVisible(bVisible);
	SetEffectVisible(bVisible);
}

void CNtlPLObject::SetEffectVisible( RwBool bVisible ) 
{
	m_LinkEffectInstance.SetVisible(bVisible);
	if(!bVisible)
	{
		ClearLoopEffect();
		ClearLoopSound();
	}
}

#define	RENDER_MAX_COLOR_TEMP	50
RwRGBA g_sColorTemp[RENDER_MAX_COLOR_TEMP];
RpAtomic *CNtlPLObject::RenderCallBack(RpAtomic *pAtomic)
{
	NTL_SPROFILE("CNtlPLObject::RenderCallBack");

	CNtlPLEntity *pEntity = (CNtlPLEntity*)RpNtlAtomicGetData(pAtomic);
	if(pEntity)
	{
		if(pEntity->GetClassType() == PLENTITY_OBJECT)
		{			
			CNtlPLObject *pObjEntity = (CNtlPLObject *)pEntity;

			RwRGBAReal	sColorRatioReal;
			RwRGBARealFromRwRGBA(&sColorRatioReal, pObjEntity->GetColor());

			RwRGBA	sColor;
			RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);

			bool	bSetMaterialColor = false;
			if(pGeom)
			{   
				RwUInt32 uFlags = RpGeometryGetFlags(pGeom); 
				if( !(uFlags & rpGEOMETRYMODULATEMATERIALCOLOR) )
				{
					bSetMaterialColor = true;
					uFlags |= rpGEOMETRYMODULATEMATERIALCOLOR;
					RpGeometrySetFlags(pGeom, uFlags);
				}

				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				if(nMatCount > RENDER_MAX_COLOR_TEMP)
				{
					DBO_WARNING(nMatCount <= RENDER_MAX_COLOR_TEMP && nMatCount >= 0, "out of range.")
						nMatCount = RENDER_MAX_COLOR_TEMP;
				}

				// Atomic에 설정된 Alpha 값과의 보정                
				sColorRatioReal.alpha *= RpNtlAtomicGetAlpha(pAtomic) / 255.0f;

				// Fade를 위한 Weight Alpha값과의 보정
#ifdef dNTL_WORLD_TOOL_MODE	// 툴에서는 무조건 Fade Alpha 적용
				sColorRatioReal.alpha *= pEntity->GetWeightAlpha();
#else
				if(!(pEntity->GetFlags() & NTL_PLEFLAG_OBJECT_FORCE_VISIBLE)) // 플래그가 켜져있다면, Fade alpha를 적용하지 않는다.
				{
					sColorRatioReal.alpha *= pEntity->GetWeightAlpha();
				}
#endif                

				if(sColorRatioReal.alpha == 0.0f)       // 알파가 0.0이면 렌더링하지 않는다.
					NTL_RPROFILE(pAtomic);

				// Alpha Test ATomic의 Alpha 보간 보정( Alpha Test일 경우 208 이상인 경우 통과인 관계로)
				if( RpNtlAtomicGetFlag(pAtomic) & NTL_ALPHATEST &&
					RpNtlAtomicGetFlag(pAtomic) & NTL_RUNTIME_ALPHA)
				{
					sColorRatioReal.alpha	= sColorRatioReal.alpha * 0.18823f + 0.81176f;
				}

				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);

					// get through the proper renderpipe
					if(pObjEntity->GetToonData())
					{
						// Toon이 적용되어 있는 경우에만 Material Render CallBack을 따른다. 안되어있으면 안나온다. (by agebreak)
						RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);
					}

					const RwRGBA *pMatColor = RpMaterialGetColor(pMaterial);

					g_sColorTemp[i].red	  = pMatColor->red;
					g_sColorTemp[i].green = pMatColor->green;
					g_sColorTemp[i].blue  = pMatColor->green;
					g_sColorTemp[i].alpha = pMatColor->alpha;


					sColor.red		= RwUInt8(g_sColorTemp[i].red * sColorRatioReal.red);
					sColor.green	= RwUInt8(g_sColorTemp[i].green * sColorRatioReal.green);
					sColor.blue		= RwUInt8(g_sColorTemp[i].blue * sColorRatioReal.blue);
					sColor.alpha	= RwUInt8(g_sColorTemp[i].alpha * sColorRatioReal.alpha);                    

					RpMaterialSetColor(pMaterial, &sColor);
					RpNtlMaterialExtSetAddColor(pMaterial, pObjEntity->GetAddColor());
				}
			}

			// Toon 
			SToonData* pToonData = pObjEntity->GetToonData();
			if(pToonData)
			{
				Helper_SetToonAtomicRender(pAtomic, pToonData);
			}     

			if(RpNtlAtomicGetRenderCallBack(pAtomic) != NULL)
			{
#ifdef dNTL_WORLD_TOOL_MODE
				if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE_OBJECT))
				{
					API_PL_RenderWireMesh(pAtomic, NULL);
				}
				else
				{
#endif
					RpNtlAtomicGetRenderCallBack(pAtomic)(pAtomic);
#ifdef dNTL_WORLD_TOOL_MODE
				}
#endif

			}

			if(pGeom)
			{
				if(bSetMaterialColor)
				{
					RwUInt32 uiFlags = RpGeometryGetFlags(pGeom); 
					uiFlags = uiFlags & (~rpGEOMETRYMODULATEMATERIALCOLOR);
					RpGeometrySetFlags(pGeom, uiFlags);
				}

				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					RpMaterialSetColor(pMaterial, &g_sColorTemp[i]);
				}
			}                  
		}
		else
		{
			if(RpNtlAtomicGetRenderCallBack(pAtomic) != NULL)
			{
				RpNtlAtomicGetRenderCallBack(pAtomic)(pAtomic);
			}
		}
	}
	NTL_RPROFILE(pAtomic);
}


RpClump * CNtlPLObject::GetClump( void ) const
{
	if( m_pClumpResource != NULL ) 
		return (RpClump *)m_pClumpResource->GetData(); 
	else return NULL; 
}

/**
* Fade 효과 유뮤를 적용한다.
* \param bEnable Fade 효과 유무
* return 
*/
void CNtlPLObject::SetFadeEnable(RwBool bEnable)
{
	RwInt32 uiFlags = GetFlags();
	if(bEnable)
	{
		SetFlags(uiFlags | NTL_PLEFLAG_FADE);

		if(m_eFadeState == FADE_NOT_VISIBLE)
		{
			m_pFadeBlend->SetWeight(0.0f);
		}
		else if(m_eFadeState == FADE_VISIBLE)
		{
			m_pFadeBlend->SetWeight(1.0f);
		}
		else
		{
			AddSceneUpdate();
		}

		// 		else if(!m_bAnimPlay)
		// 		{
		// 			GetSceneManager()->AddUpdate(this);
		// 		}
	}
	else 
	{
		SetFlags(uiFlags & (~NTL_PLEFLAG_FADE));

		// Visual Manager의 Update List에 포함되어 있으면 제거한다
		if(m_eFadeState != FADE_VISIBLE && m_eFadeState != FADE_NOT_VISIBLE && !m_bHaveAnim)
		{
			RemoveSceneUpdate();
			//GetSceneManager()->RemoveUpdate(this);
		}

		// 오브젝트를 화면에 표시한다.
		m_eFadeState = FADE_VISIBLE;        
		m_pFadeBlend->SetWeight(1.0f);    
		SetWeightAlpha(1.0f);
	}
}

RwReal CNtlPLObject::GetBaseDurationAnimTime() 
{
	if(!m_pAnimLayer)
		return 0.0f;

	return m_pAnimLayer->GetDurationAnimTime();

}
RwReal CNtlPLObject::GetBaseCurrentAnimTime() 
{
	return m_pAnimLayer->GetCurrentAnimTime();
}
void CNtlPLObject::SetBaseCurrentAnimTime( RwReal fCurrTime ) 
{
	return m_pAnimLayer->SetCurrentAnimTime(fCurrTime);
}

int CNtlPLObject::CallBackBaseAnim(void* pEventData ) 
{
	SEventAnim* pEventAnim = (SEventAnim*)pEventData;

	switch(pEventAnim->eEventID)
	{
	case EVENT_ANIM_END:
		OnEventAnimEnd((SEventAnimEnd*)pEventData);
		break;
	case EVENT_ANIM_VISUAL_EFFECT:
		OnEventVisualEffect((SEventVisualEffect*)pEventData);
		break;
	case EVENT_ANIM_VISUAL_SOUND:
		OnEventVisualSound((SEventSound*)pEventData);
		break;
	case EVENT_ANIM_ALPHA:
		OnEventAlphaFade((SEventAlpha*)pEventData);
		break;
	case EVENT_ANIM_TMQ:
		OnEventTMQ((SEventAnimCinematic*)pEventData);
		break;
	case EVENT_ANIM_EXPLOSION:
		OnEventExplosion((SEventExplosion*)pEventData);
		break;
	}

	return TRUE;
}

void CNtlPLObject::OnEventAnimEnd( SEventAnimEnd* pEventAnimEnd ) 
{
	CNtlPLEventGenerator::AnimEnd(GetSerialID(), pEventAnimEnd->uiAnimKey, INVALID_GRAPHIC_ANIMATION_ID);			
}

void CNtlPLObject::OnEventVisualSound( SEventSound* pEventSound ) 
{
	// Sound를 Play한다.    

	if(strlen(pEventSound->chSoundName) <= 1)
		return ;

	// LoopSound이고 이미 기존에 Play되고 있다면 Play하지 않는다.
	if(pEventSound->bLoop && IsExistLoopSound(pEventSound->chSoundName))
		return ;

	// 파일이 여러개 세팅되어 있는경우에는 랜덤으로 플레이된다.
	std::string soundFileName;
	int nMax = 1;
	if(strlen(pEventSound->chSoundName4) > 0)
		nMax = 4;
	else if(strlen(pEventSound->chSoundName3) > 0)
		nMax = 3;
	else if(strlen(pEventSound->chSoundName2) > 0)
		nMax = 2;

	if(nMax > 1)
	{
		int nRand = NtlRandomNumber(1, nMax);
		switch(nRand)
		{
		case 1:
			soundFileName = pEventSound->chSoundName;
			break;
		case 2:            
			soundFileName = pEventSound->chSoundName2;
			break;
		case 3:            
			soundFileName = pEventSound->chSoundName3;
			break;
		case 4:
			soundFileName = pEventSound->chSoundName4;
			break;
		}
	}
	else
	{
		soundFileName = pEventSound->chSoundName;
	}

	// 피치를 랜덤으로 선택한다
	RwReal fSoundPitch = NtlRandomNumber(pEventSound->fSoundPitchMin, pEventSound->fSoundPitchMax);

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= pEventSound->eSoundType;
	tSoundParam.pcFileName		= pEventSound->chSoundName;
	tSoundParam.bLoop			= B2b(pEventSound->bLoop);
	tSoundParam.fVolume			= pEventSound->fSoundVolume * 0.01f;
	tSoundParam.fPitch			= fSoundPitch;
	tSoundParam.fXPos			= m_vWorldPosition.x;
	tSoundParam.fYPos			= m_vWorldPosition.y;
	tSoundParam.fZPos			= m_vWorldPosition.z;
	tSoundParam.fMinDistance	= pEventSound->fSoundDist;
	tSoundParam.fMaxDistance	= pEventSound->fSoundDecayDist;

	int iRet = GetSoundManager()->Play(&tSoundParam);

	// Loop Sound면 리스트에 추가한다.
	if(iRet == SOUNDRESULT_OK && pEventSound->bLoop && tSoundParam.hHandle != INVALID_SOUND_HANDLE)
	{
		AddLoopSound(tSoundParam.hHandle);
	}
}

void CNtlPLObject::OnEventVisualEffect( SEventVisualEffect* pEventVisualEffect ) 
{
	//Effect Name이 없는 경우는 무조건 Return을 한다.
	if( strlen(pEventVisualEffect->chEffectName) <= 0)
		return;

	if( strlen(pEventVisualEffect->chBoneName) <= 0 && pEventVisualEffect->bAttachBone)
		return;

	// 만약 LoopEffect 리스트에 같은 이름,Bone이 있으면 새로 생성하지 않는다.
	if (IsExistLoopEffect(pEventVisualEffect->chEffectName, pEventVisualEffect->chBoneName))
	{
		return;
	}
//	DBO_WARNING_MESSAGE("pEventVisualEffect->chEffectName: " << pEventVisualEffect->chEffectName);
	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventVisualEffect->chEffectName);
	DBO_ASSERT(pPLEntity, "Entity create failed.");

	//Insert the Serial ID of the Object into the Effect (for use in the Client).
	pPLEntity->SetSerialID(GetSerialID());

	pPLEntity->SetVisible(IsVisible(0));

	CNtlInstanceEffect *pInstanceEffect = (CNtlInstanceEffect *)pPLEntity;

	// AutoDelete가 아니면 LoopEffect라고 간주하고 리스트에 추가한다   
	if(!pPLEntity->IsAutoDelete())
	{
		SLoopEffect* pLoopEffect = NTL_NEW SLoopEffect();
		pLoopEffect->pLoopEffectIntance = pInstanceEffect;
		pLoopEffect->szBoneName = pEventVisualEffect->chBoneName;

		AddLoopEffect(pLoopEffect);
	}

	if(!pEventVisualEffect->bAttach)
	{
		RwV3d vPos = GetPosition() + pEventVisualEffect->vOffSetPos;
		pInstanceEffect->SetPosition(&vPos);
	}
	else if( pEventVisualEffect->bAttachBone)				
	{
		SPLAttachAttr attachAttr;
		attachAttr.vOffsetPos = pEventVisualEffect->vOffSetPos;        

		AttachBone((CNtlPLAttach *)pPLEntity, pEventVisualEffect->chBoneName, &attachAttr);
	}
	else
	{
		AttachWorldPosAndRotate((CNtlPLAttach *)pPLEntity, pEventVisualEffect->vOffSetPos);
	}
}

void CNtlPLObject::OnEventAlphaFade( SEventAlpha* pEventAlpha ) 
{
	// Atomic Alpha에 관해서만 PL단에서 처리한다.
	if(pEventAlpha->eAlphaEventType != SEventAlpha::E_ALPHA_EVENT_ATOMIC)
	{
		CNtlPLEventGenerator::AnimEventAlpha(GetSerialID(), (void*)pEventAlpha);
		return;
	}

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(Helper_GetBitFlag(pEventAlpha->bfAtomicList, i))
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);

			std::string strAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
			GetAlphaBlendController()->AddAtomicAlpha(strAtomicName.c_str(), 
				pEventAlpha->nStartAlpha / 255.0f, 
				pEventAlpha->nDestAlpha / 255.0f, 
				pEventAlpha->fFadeTime, 
				pEventAlpha->fLifeTime, FALSE);
		}        
	}
}

void CNtlPLObject::OnEventTMQ( SEventAnimCinematic* pEventTMQ ) 
{
	RwBool bIn;
	if(pEventTMQ->eAnimCinematicEventType == E_ANIM_CINEMATIC_TMQ_IN)
	{
		bIn = TRUE;
	}
	else
	{
		bIn = FALSE;
	}

	CNtlPLEventGenerator::AnimEventTMQ(GetSerialID(), bIn);
}

void CNtlPLObject::OnEventExplosion( SEventExplosion* pEventExplosion ) 
{
	CNtlPLEventGenerator::AnimEventExplosion(GetSerialID(), (void*)pEventExplosion);
}

void CNtlPLObject::ClearLoopSound() 
{
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hSound = *it;
		GetSoundManager()->Stop(hSound);        
	}

	m_listLoopSound.clear();
}

RwBool CNtlPLObject::IsExistLoopSound( RwChar* szSoundName ) 
{
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hListSound = *it;
		if(strlen(GetSoundManager()->GetSoundName(hListSound)) > 0)
		{
			if(_strcmpi(GetSoundManager()->GetSoundName(hListSound),szSoundName) == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwReal CNtlPLObject::GetAnimPlayTime( RwUInt32 uiAnimKey ) 
{
	// AnimTable이 아직 생성되어 있지 않다면 생성해준다. 
	if(!m_pInstanceAnimTable)
	{
		m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
		m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());
	}

	SInstanceAnimData* pAnimData = m_pInstanceAnimTable->Get(uiAnimKey);
	if(!pAnimData)
		return 0.0f;

	return pAnimData->pTypeAnimData->fPlayTime;
}

void CNtlPLObject::ClearLoopEffect() 
{
	ListLoopEffect::iterator it = m_listLoopEffect.begin();
	for(;it != m_listLoopEffect.end(); ++it)
	{
		SLoopEffect* pLoopEffect = *it;
		if(pLoopEffect && pLoopEffect->pLoopEffectIntance)
		{
			pLoopEffect->pLoopEffectIntance->Finish();
		}        

		NTL_DELETE(pLoopEffect);
	}

	m_listLoopEffect.clear();
}

RwBool CNtlPLObject::IsExistLoopEffect( const RwChar* szEffectName, const RwChar* szBoneName ) 
{
	if(!szEffectName || !szBoneName)
		return FALSE;

	ListLoopEffect::iterator it = m_listLoopEffect.begin();
	for(;it != m_listLoopEffect.end(); ++it)
	{
		SLoopEffect* pLoopEffect = *it;
		if(pLoopEffect && pLoopEffect->pLoopEffectIntance)
		{
			if(strcmp(pLoopEffect->pLoopEffectIntance->GetName(), szEffectName) == 0 &&
				strcmp(pLoopEffect->szBoneName, szBoneName) == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CNtlPLObject::SetAlpha(RwUInt8 byValue)		
{
	m_sColor.alpha = byValue;  

	if(m_pClumpResource == NULL)
		return;

	if(byValue >= 255)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag &(~NTL_RUNTIME_ALPHA));
		}
	}
	else
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
		}
	}	
}

void CNtlPLObject::SetAtomicWeightAlpha( const RwChar *pAtomicName, RwReal fWeightValue ) 
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(strcmp(Helper_AtomicName(m_vecAtomicList[i]->pData), pAtomicName) == 0)
		{
			RwUInt8 byAlpha = (RwUInt8)(fWeightValue * 255.0f);
			RpNtlAtomicSetAlpha(m_vecAtomicList[i]->pData, byAlpha);            
			break;
		}
	}

}

RwMatrix* CNtlPLObject::GetBoneMatrix( const RwChar *pBoneName ) 
{
	if(!GetClump())
		return NULL;

	if(m_mapFrame.find( pBoneName ) == m_mapFrame.end())
		return NULL;

	RwFrame *pFrame = m_mapFrame[pBoneName];

	if(pFrame != NULL)
		return &pFrame->ltm;

	return NULL;
}

void CNtlPLObject::GetBoneName(std::vector<std::string>& vecBoneName)
{
	for (FRAME_MAP_ITER it = m_mapFrame.begin(); it != m_mapFrame.end(); ++it)
	{
		vecBoneName.push_back(it->first);
	}
}

RwV3d CNtlPLObject::GetDirection() 
{
	return *RwMatrixGetAt(&GetMatrix());
}

void CNtlPLObject::UpdateLoopSoundPos() 
{
	for each(SOUND_HANDLE hSound in m_listLoopSound)
	{
		GetSoundManager()->SetSoundPosition(hSound, m_vWorldPosition.x, m_vWorldPosition.y, m_vWorldPosition.z);
	}
}

void CNtlPLObject::CheckToonData() 
{
	RpAtomic* pAtomic = NULL;
	RpToonGeo *pToonGeo = NULL;
	for each(SEntityAtomicData* pAtomicData in m_vecAtomicList)
	{
		pAtomic = pAtomicData->pData;         
		if(pAtomic)
		{
			pToonGeo = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pAtomic) );
			if(pToonGeo)
				break;
		}         
	}

	if(!pToonGeo || !pAtomic)
		return;

	// Toon Apply (test)
	m_pToonData = NTL_NEW SToonData();
	m_pToonData->pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture("smooth.png", "texture/toon/");
	DBO_ASSERT(m_pToonData->pTexture, "Texture load failed.");

	m_pToonData->pToonInk = RpToonInkCreate();
	RpToonInkSetOverallThickness(m_pToonData->pToonInk, DEFAULT_INK_THICKNESS);
	RwRGBA colInk;
	colInk.red = colInk.green = colInk.blue = 0;
	colInk.alpha = 255;
	RpToonInkSetColor(m_pToonData->pToonInk, colInk);
	RpToonInkSetName(m_pToonData->pToonInk, "silhouette");
	m_pToonData->pToonPaint = RpToonPaintCreate();
	RpToonPaintSetGradientTexture(m_pToonData->pToonPaint, m_pToonData->pTexture);
	Helper_SetToonClump(GetClump(), m_pToonData);
}

#ifdef dNTL_WORLD_CULLING_NEW
//#include "NtlPLRenderGeometry.h"
RwBool CNtlPLObject::CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame)
{
	if (m_uiRenderFrame != uiRenderFrame)
	{
		if (!CNtlPLEntity::CullingTest(pRwCamera, uiRenderFrame))
		{
			RwInt32 iNumAtomic		= static_cast<RwInt32>(m_vecAtomicList.size());
			RwInt32	iFrustumCheck	= 0;
			RwInt32	iOccluderCheck	= 0;

			if (m_bActiveUpdate && iNumAtomic > 0)
			{	
				RwReal fVisibleDistance = GetVisibleCullingDistance();

#ifdef dNTL_WORLD_TOOL_MODE
				if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
				{							
					fVisibleDistance = RwCameraGetFarClipPlane(pRwCamera);
				}
#endif				

				if (IsCullingTestAllAtomic())
				{
					// [m_vecAtomicList.size()]번 Occluder Proxy는 Bounding Sphere다.
					for (RwInt32 i = 0; i < iNumAtomic; ++i)
					{
#ifdef _DEBUG
						++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif
						RpAtomic*		pAtomic = static_cast<RpAtomic*>(m_vecAtomicList.at(i)->pData);
						const RwSphere* pSphere = RpAtomicGetWorldBoundingSphere(pAtomic);

						if (!CNtlPLGlobal::m_bCollObjVisible && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE))
						{
							++iFrustumCheck;							
						}
						else if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
						{
							++iFrustumCheck;
						}
						else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, i))
						{ 
							++iOccluderCheck;
						}						
						else
						{
							break;
						}
					}
				}
				else
				{
					RwSphere* pSphere = GetBoundingSphere();

					// 					static RwRGBA	clr = { 255, 255, 255 ,255 };
					// 					static RwMatrix	mat;
					// 
					// 					RwMatrixTranslate(&mat, &pSphere->center, rwCOMBINEREPLACE);
					// 					RenderGeometryBillboardSphere(&mat, &clr, pSphere->radius, pRwCamera, TRUE);

#ifdef _DEBUG
					++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

					if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
					{
						iFrustumCheck = iNumAtomic;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, iNumAtomic))
					{	
						iOccluderCheck = iNumAtomic;
					}
				}
			}

			if (iFrustumCheck >= iNumAtomic)
			{
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
			}
			else if (iFrustumCheck + iOccluderCheck >= iNumAtomic) 
			{
				// Frustum + Occluder 갯수가 Atomic 갯수보다 많다면
				// OCCLUDER Flag를 셋팅 한다. 단 OccluderCheck 갯수가 존재해야 하므로, iFrustumCheck >= iNumAtomic를 통과해야만 가능하다.
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
			}
		}
		SetCullFlags(m_uiCullFlags);
	}
	return !IsVisible();
}
#else
RwBool CNtlPLObject::CullingTest(RwCamera* pRwCamera)
{
	if (!CNtlPLEntity::CullingTest(pRwCamera))
	{
		RwInt32 iNumAtomic = static_cast<RwInt32>(m_vecAtomicList.size());

		if (m_bActiveUpdate && iNumAtomic > 0)
		{
			RwInt32				 iFrustumCheck = 0;
			RwInt32				iOccluderCheck = 0;

			RwReal				fVisibleDistance = GetVisibleCullingDistance();

			if (m_bHaveAnim)
			{
				for (RwInt32 i = 0; i < iNumAtomic; ++i)
				{
#ifdef _DEBUG
					++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

					CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
					if(pCullScheduling)
					{
						pCullScheduling->IncAtomicOperator(PLENTITY_OBJECT);
					}

					RpAtomic* pAtomic = static_cast<RpAtomic*>(m_vecAtomicList.at(i)->pData);
					RwSphere* pSphere = &pAtomic->worldBoundingSphere;

#ifdef dNTL_WORLD_TOOL_MODE
					if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
					{
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) | rpATOMICRENDER);
						continue;
					}
#endif				
					if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
					{
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) & (~rpATOMICRENDER));
						++iFrustumCheck;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, i))
					{ 
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) & (~rpATOMICRENDER));
						++iOccluderCheck;
					}
					else
					{					
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) | rpATOMICRENDER);
					}
				}

				if (iFrustumCheck >= iNumAtomic)
				{
					m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
				}
				else if (iFrustumCheck + iOccluderCheck >= iNumAtomic) 
				{
					// Frustum + Occluder 갯수가 Atomic 갯수보다 많다면
					// OCCLUDER Flag를 셋팅 한다. 단 OccluderCheck 갯수가 존재해야 하므로, iFrustumCheck >= iNumAtomic를 통과해야만 가능하다.
					m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
				}
			}
			else
			{
#ifdef _DEBUG
				++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

#ifdef dNTL_WORLD_TOOL_MODE
				if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
#endif
				{
					CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
					if(pCullScheduling)
					{
						pCullScheduling->IncAtomicOperator(PLENTITY_OBJECT);
					}

					RwSphere* pSphere = GetBoundingSphere();
					if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
					{
						m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, 0))
					{
						m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
					}
				}				
			}		
		}
	}

	SetCullFlags(m_uiCullFlags);
	return static_cast<RwBool>(m_uiCullFlags);
}
#endif

RwReal CNtlPLObject::GetVisibleCullingDistance()
{
#ifdef dNTL_WORLD_TOOL_MODE
	return m_fVisibleCullingDistance;
#else
	if (m_fVisibleCullingDistance > 512 || m_fVisibleCullingDistance < GetNtlPLOptionManager()->GetObjectFar())
	{
		return m_fVisibleCullingDistance;
	}
	return GetNtlPLOptionManager()->GetObjectFar();
#endif
}

// RwPlane CNtlPLObject::g_planeFar;
// 
// void CNtlPLObject::SetFarPlane(RwReal fFarDist)
// {
// 	RwMatrix* pmatCamera	= RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
// 	g_planeFar.normal		= pmatCamera->at;
// 	g_planeFar.distance		= RwV3dDotProduct(&(pmatCamera->at * fFarDist + pmatCamera->pos), &pmatCamera->at);
// }

void CNtlPLObject::SetLightMapMaterialFlags(RwUInt32 uiFlags)
{
	// comment here  for the mini indoor
	// if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

			RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
			if(pGeom != NULL)
			{
				RwInt32 nNumMat = RpGeometryGetNumMaterials(pGeom);
				for(int i = 0; i < nNumMat; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					if(pMaterial != NULL)
					{
						RtLtMapMaterialSetFlags(pMaterial, uiFlags);
					}
				}
			}
		}
	}
}

void CNtlPLObject::SetLightMapObjectFlags(RwUInt32 uiFlags)
{
	// comment here  for the mini indoor
	// if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

			RtLtMapAtomicSetFlags(pAtomic, uiFlags);
		}
	}
}

RwUInt32 CNtlPLObject::GetLightMapMaterialFlags()
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
		if(pGeom != NULL)
		{
			RwInt32 nNumMat = RpGeometryGetNumMaterials(pGeom);
			for(int i = 0; i < nNumMat; i++)
			{
				RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
				if(pMaterial != NULL)
				{
					return RtLtMapMaterialGetFlags(pMaterial);
				}
			}
		}
	}
	return 0;
}

RwUInt32 CNtlPLObject::GetLightMapObjectFlags()
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

		return RtLtMapAtomicGetFlags(pAtomic);
	}
	return 0;
}

void CNtlPLObject::AllocSchedulingLTProp()
{
	FreeSchedulingLTProp();

	m_pSchedulingLMProp = NTL_NEW sSCHEDULING_LM_PROP;
}

void CNtlPLObject::FreeSchedulingLTProp()
{
	if (m_pSchedulingLMProp)
	{
		for (int i = 0; i < (int)m_pSchedulingLMProp->_vecSchedulingLMAtomProp.size(); ++i)
		{
			sSCHEDULING_LM_ATOM_PROP* pSchedulingLMAtomProp = m_pSchedulingLMProp->_vecSchedulingLMAtomProp.at(i);

			NTL_DELETE(pSchedulingLMAtomProp);
		}
		m_pSchedulingLMProp->_vecSchedulingLMAtomProp.clear();

		NTL_DELETE(m_pSchedulingLMProp);
	}
}

struct SNtlRpWorldSectorIntersectionOBB
{
	CNtlOBB*					 pOBB;
	std::vector<RpWorldSector*>* pvecRpWorldSector;
};

RpWorldSector* NtlRpWorldSectorIntersectionOBB(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	static CNtlOBB						OBBRpWorldSector;
	SNtlRpWorldSectorIntersectionOBB*	pNtlRpWorldSectorIntersectionOBB = (SNtlRpWorldSectorIntersectionOBB*)pData;
	const RwBBox*						pBBoxRpWorldSector = RpWorldSectorGetBBox(pRpWorldSector);

	//내부 연산을줄이기 위해 직접 입력.
	OBBRpWorldSector.fAxisLen[0] = (pBBoxRpWorldSector->sup.x - pBBoxRpWorldSector->inf.x) * 0.5f;
	OBBRpWorldSector.fAxisLen[1] = (pBBoxRpWorldSector->sup.y - pBBoxRpWorldSector->inf.y) * 0.5f;
	OBBRpWorldSector.fAxisLen[2] = (pBBoxRpWorldSector->sup.z - pBBoxRpWorldSector->inf.z) * 0.5f;
	OBBRpWorldSector.vAxisDir[0] = CNtlPLGlobal::m_vXAxisV3;
	OBBRpWorldSector.vAxisDir[1] = CNtlPLGlobal::m_vYAxisV3;
	OBBRpWorldSector.vAxisDir[2] = CNtlPLGlobal::m_vZAxisV3;
	OBBRpWorldSector.vCenterPos.x= (pBBoxRpWorldSector->sup.x + pBBoxRpWorldSector->inf.x) * 0.5f;
	OBBRpWorldSector.vCenterPos.y= (pBBoxRpWorldSector->sup.y + pBBoxRpWorldSector->inf.y) * 0.5f;
	OBBRpWorldSector.vCenterPos.z= (pBBoxRpWorldSector->sup.z + pBBoxRpWorldSector->inf.z) * 0.5f;

	if (CNtlOBB::IntersectionOBBvsOBB(pNtlRpWorldSectorIntersectionOBB->pOBB, &OBBRpWorldSector))
	{
		pNtlRpWorldSectorIntersectionOBB->pvecRpWorldSector->push_back(pRpWorldSector);
	}	
	return pRpWorldSector;
}

void CNtlPLObject::UpdateRpSectorOBBCheck()
{
	if (GetSceneManager()->GetActiveWorldType() != AW_RWWORLD)
	{
		return;
	}

	RwBBox bboxAABB = GetDefaultAABB();
	CNtlOBB::CalcOBBData(&m_OBB, &GetMatrix(), &bboxAABB);
	m_vecRpWorldSector.clear();

	RpIntersection intersection;
	intersection.type		= rpINTERSECTSPHERE; 
	intersection.t.sphere	= *GetBoundingSphere();

	SNtlRpWorldSectorIntersectionOBB sNtlRpWorldSectorIntersectionOBB;
	sNtlRpWorldSectorIntersectionOBB.pOBB				= &m_OBB;
	sNtlRpWorldSectorIntersectionOBB.pvecRpWorldSector	= &m_vecRpWorldSector;

	RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, NtlRpWorldSectorIntersectionOBB, &sNtlRpWorldSectorIntersectionOBB);
}

RwBool CNtlPLObject::IsRpWorldSectorList(RpWorldSector* pRpWorldSector)
{
	if (!m_bHaveAnim && m_vecRpWorldSector.end() == find(m_vecRpWorldSector.begin(), m_vecRpWorldSector.end(), pRpWorldSector))
	{
		return FALSE;
	}
	return TRUE;
}

RwUInt32 CNtlPLObject::GetObjectType()
{
	if (m_pObjectType)
	{
		return m_pObjectType->GetType();
	}
	return EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
}

void CNtlPLObject::SetObjectType(RwUInt32 uiObjectType)
{
	if (m_pObjectType)
	{
		NTL_DELETE(m_pObjectType);
	}

	switch (uiObjectType)
	{
	case EPL_OBJECT_TYPE_OUTDOOR_OBJECT:		
		break;
	case EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE:
		m_pObjectType = NTL_NEW CNtlPLObjectType_MiniIndoor_Close;
		break;
	case EPL_OBJECT_TYPE_MINI_INDOOR_OPEN:
		m_pObjectType = NTL_NEW CNtlPLObjectType_MiniIndoor_Open;
		break;
	case EPL_OBJECT_TYPE_MINI_INDOOR_OBJECT:
		m_pObjectType = NTL_NEW CNtlPLObjectType_MiniIndoor_Object;
		break;
	}

	if (m_pObjectType)
	{		
		m_pObjectType->Create(this);

		AddSceneUpdate();
	}
	else
	{
		RemoveSceneUpdate();
	}
}

void CNtlPLObject::AddSceneUpdate()
{
	// Animation이 없거나 ObjectType이 MINI_INDOOR_CLOSE 또는 EPL_OBJECT_TYPE_MINI_INDOOR_OPEN이면
	// UpdateList에 포함한다. Animation을 가지고 있는 Object는 SetProperty에서 Update Flag를 제거 하기 때문에
	// 여기서는 관여해서는 안된다.
	if (GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE || 
		GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN ||
		!m_bHaveAnim)
	{
		GetSceneManager()->AddUpdate(this);
	}
}

void CNtlPLObject::RemoveSceneUpdate()
{
	// Animation이 없거나 ObjectType이 MINI_INDOOR_CLOSE 또는 EPL_OBJECT_TYPE_MINI_INDOOR_OPEN이면
	// UpdateList에 포함한다. Animation을 가지고 있는 Object는 SetProperty에서 Update Flag를 제거 하기 때문에
	// 여기서는 관여해서는 안된다.
	if (GetObjectType() != EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE && 
		GetObjectType() != EPL_OBJECT_TYPE_MINI_INDOOR_OPEN &&
		!m_bHaveAnim)
	{
		GetSceneManager()->RemoveUpdate(this);
	}
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlPLObject::SaveSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector*	pSector			= &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];
		RwInt32				iNumTrigger		= 0;
		RwInt32				iNumNonTrigger	= 0;

		RwInt32 iNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament.size());
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i)); 

			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				++iNumTrigger;
			}
			else
			{
				++iNumNonTrigger;
			}
		}

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		if(_chdir("swap") == -1)
		{
			_mkdir("swap");
			_chdir("swap");
		}		

		if(_chdir("obj") == -1)
		{
			_mkdir("obj");
			_chdir("obj");
		}

		// trigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.objt", iSectorIdx);

		FILE* pFileTrigger;
		if(fopen_s(&pFileTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		// nontrigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.obj", iSectorIdx);

		FILE* pFileNonTrigger;
		if(fopen_s(&pFileNonTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			fclose(pFileTrigger);

			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		fwrite(&iNumTrigger, sizeof(RwInt32), 1, pFileTrigger);
		fwrite(&iNumNonTrigger, sizeof(RwInt32), 1, pFileNonTrigger);

		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i)); 

			RwUInt32 uiMinimapLayer = pObject->GetMinimapLayer();
			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				fwrite(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileTrigger);
			}
			else
			{
				fwrite(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileNonTrigger);
			}
		}

		fclose(pFileTrigger);
		fclose(pFileNonTrigger);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLObject::LoadSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector* pSector = &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("swap");
		_chdir("obj");

		// trigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.objt", iSectorIdx);

		FILE* pFileTrigger;
		if(fopen_s(&pFileTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		// nontrigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.obj", iSectorIdx);

		FILE* pFileNonTrigger;
		if(fopen_s(&pFileNonTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			fclose(pFileTrigger);

			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		RwInt32 iNum;
		RwInt32 iNumTrigger;
		RwInt32 iNumNonTirgger;

		fread(&iNumTrigger, sizeof(RwInt32), 1, pFileTrigger);
		fread(&iNumNonTirgger, sizeof(RwInt32), 1, pFileNonTrigger);

		iNum = iNumTrigger + iNumNonTirgger;
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i)); 

			RwUInt32 uiMinimapLayer = pObject->GetMinimapLayer();
			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				fread(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileTrigger);
			}
			else
			{
				fread(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileNonTrigger);
			}
			pObject->SetMinimapLayer(uiMinimapLayer);
		}

		fclose(pFileTrigger);
		fclose(pFileNonTrigger);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

#endif

RwBool CNtlPLObject::IsCullingTestAllAtomic() 
{
	if(m_uiCurAnimKey != INVALID_DWORD)  // 이 키값은 트리거 애니메이션이 적용될때만 유효하다.
	{
		STypeAnimData *pTypeAnimData = m_pProperty->GetAnimTable()->Get(m_uiCurAnimKey);
		if(!pTypeAnimData)
			return TRUE;

		return pTypeAnimData->IsCullTestAllAtomic();
	}
	else
	{
		return m_pProperty->IsCullTestAllAtomic();
	}
}

RwBool CNtlPLObject::LoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{
	if (GetObjectTypePtr() && (GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN))
	{
		GetObjectTypePtr()->LoadFromFile(pFile);
	}

	// serial id
	RwUInt32 uiSID;	
	fread(&uiSID, sizeof(RwUInt32), 1, pFile);
	SetSID(uiSID);

	// object id
	RwUInt32 uiObjectID;
	fread(&uiObjectID, sizeof(RwUInt32), 1, pFile);
	SetObjectSerialID(uiObjectID);

	// object name index
	DWORD dwNameIdx;
	fread(&dwNameIdx, sizeof(DWORD), 1, pFile);
	SetObjNameIdx(dwNameIdx);

	// visibility distance
	RwReal fVisibilityDistance;
	fread(&fVisibilityDistance, sizeof(RwReal), 1, pFile);
	SetVisibleCullingDistance(fVisibilityDistance);

	RwUInt32 uiFlags = 0;
	fread(&uiFlags, sizeof(RwUInt32), 1, pFile);
	SetFlags(GetFlags() | uiFlags);

	// object PS map
	LoadPSMap(pFile);

	if(!GetNtlResourceManager()->IsLoadScheduling())
	{
		SetAtomicPSMap();
	}

	// milepost GUID
	RwBool bEnableFlag;
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		RwUInt32 uiMilePostGUID;
		fread(&uiMilePostGUID, sizeof(RwUInt32), 1, pFile);
		SetMilepostID(uiMilePostGUID);
	}

	// attach lightojbect
	LoadLightObjectAttachData(pFile);

	if (eActiveWorldType == AW_HEGITHFIELD)
	{
		BOOL bUnknown = FALSE;
		fread(&bUnknown, sizeof(BOOL), 1, pFile);

		if (bUnknown == FALSE)
			return TRUE;
	}
	else if (eActiveWorldType != AW_RWWORLD)
	{
		return TRUE;
	}

	// Illumination
	if(GetNtlResourceManager()->IsLoadScheduling())
	{
		LoadSchedulingLightmapProp(pFile);
	}
	else
	{
		LoadLightmap(pFile);
	}

	return TRUE;
}

RwBool CNtlPLObject::SaveIntoFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{
	if (GetObjectTypePtr() && (GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN))
	{
		GetObjectTypePtr()->SaveIntoFile(pFile);
	}

	// serial id
	RwUInt32 uiSID = GetSID();	
	fwrite(&uiSID, sizeof(RwUInt32), 1, pFile);

	// object id
	if (eActiveWorldType == AW_RWWORLD)
	{
		RwUInt32 uiObjectID = GetObjectSerialID();
		if (uiObjectID == 0)
		{
			uiObjectID = dGET_WORLD_PARAM()->WorldObjectSerialID;
			++dGET_WORLD_PARAM()->WorldObjectSerialID;
		}
		fwrite(&uiObjectID, sizeof(RwUInt32), 1, pFile);

	}

	// object name index
	DWORD dwNameIdx = GetObjNameIdx();
	fwrite(&dwNameIdx, sizeof(DWORD), 1, pFile);

	// visibility distance
	RwReal fVisibilityDistance = GetVisibleCullingDistance();
	fwrite(&fVisibilityDistance, sizeof(RwReal), 1, pFile);

	RwBool bEnableFlag;
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		// enable trigger
		bEnableFlag = GetFlags() & NTL_PLEFLAG_TRIGGER;
		fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

		// enable selection
		bEnableFlag = GetFlags() & NTL_PLEFLAG_SELECTION;
		fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

		// enable alterable; dynamic obj.s.
		bEnableFlag = GetFlags() & NTL_PLEFLAG_ALTERABLE;
		fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

		// enable portal
		bEnableFlag = GetFlags() & NTL_PLEFLAG_PORTAL;
		fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

		// enable path engine collision
		bEnableFlag = GetFlags() & NTL_PLEFLAG_PECOLLISION;
		fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

		// enable shadow prop
		bEnableFlag = GetFlags() & NTL_PLEFLAG_SHADOW_PROP;
		fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiFlags = GetFlags() & (NTL_PLEFLAG_TRIGGER | NTL_PLEFLAG_SELECTION | NTL_PLEFLAG_ALTERABLE | NTL_PLEFLAG_PORTAL | NTL_PLEFLAG_PECOLLISION | NTL_PLEFLAG_SHADOW_PROP);

		fwrite(&uiFlags, sizeof(RwUInt32), 1, pFile);
	}

	// object PS map
	SavePSMap(pFile);

	// milepost GUID
	RwUInt32 uiMilePostGUID = GetMilepostID();

	bEnableFlag = (GetProperty()->GetObjectType() == E_OBJECT_MILEPOST && uiMilePostGUID);
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{		
		fwrite(&uiMilePostGUID, sizeof(RwUInt32), 1, pFile);
	}

	// attach lightojbect
	SaveLightObjectAttachData(pFile);

	// Illumination
	if (eActiveWorldType == AW_RWWORLD)
	{
		SaveLightmap(pFile);
	}

	return TRUE;
}

RwInt32	CNtlPLObject::SkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		CNtlPLObjectTypeHasProperty::SkipToFile(pFile);
	}

	::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // trigger obj serial id

	::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // object serial id

	::fseek(pFile, (sizeof(DWORD)), SEEK_CUR); // object name index
	::fseek(pFile, (sizeof(RwReal)), SEEK_CUR); // visibility distance

	::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // flags

	SkipPSMap(pFile);

	// milepost
	RwBool bMilepost;
	::fread(&bMilepost, sizeof(RwBool), 1, pFile);
	if(bMilepost)
	{
		::fseek(pFile, sizeof(RwUInt32), SEEK_CUR);
	}

	// attach light object skip
	SkipLightObjectAttachData(pFile);	


	if (eActiveWorldType == AW_HEGITHFIELD)
	{
		BOOL bUnknown = FALSE;
		fread(&bUnknown, sizeof(BOOL), 1, pFile);

		if (bUnknown == FALSE)
			return TRUE;
	}
	else if (eActiveWorldType != AW_RWWORLD)
	{
		return TRUE;
	}

	// Illumination
	SkipLightmap(pFile);

	return ftell(pFile);
}

BYTE* CNtlPLObject::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(pFile, pFileMem);
	}

	// trigger obj serial id
	::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	::fwrite(pFileMem, sizeof(DWORD), 1, pFile);
	pFileMem += sizeof(DWORD);

	// visibility distance
	::fwrite(pFileMem, sizeof(RwReal), 1, pFile);
	pFileMem += sizeof(RwReal);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// enable trigger
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable selection
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable alterable
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable portal
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable path engine collision
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable ShadowProp
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SavePSMapFromFileMem(pFile, pFileMem);	

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	fwrite(&EnableFlg, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SaveLightObjectAttachDataFromFileMem(pFile, pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SaveLightmapFromFileMem(pFile, pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SaveLightObjectAttachDataFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 iLightObjectNum;
	CopyMemory(&iLightObjectNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for (int j = 0; j < iLightObjectNum; ++j)
	{
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			fwrite(pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, pFile);
			pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;

			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			pFileMem += sizeof(RwUInt32);

			fwrite(pFileMem, sizeof(RwChar) * uiLength, 1, pFile);
			pFileMem += sizeof(RwChar) * uiLength;
		}

		fwrite(pFileMem, sizeof(RpLightType), 1, pFile);
		pFileMem += sizeof(RpLightType);
		fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
		pFileMem += sizeof(RwV3d);

		fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
		pFileMem += sizeof(RwV3d);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			fwrite(pFileMem, sizeof(RwRGBAReal), 1, pFile);
			pFileMem += sizeof(RwRGBAReal);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			fwrite(pFileMem, sizeof(RwRGBA), 1, pFile);
			pFileMem += sizeof(RwRGBA);
		}

		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);
		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);

		fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);
		fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);
	}
	return pFileMem;
}

BYTE* CNtlPLObject::SaveLightmapFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwUInt32 uiLtMapObjectFlags;
	CopyMemory(&uiLtMapObjectFlags, pFileMem, sizeof(RwUInt32));
	fwrite(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	RwUInt32 uiLtMapMaterialFlags;
	CopyMemory(&uiLtMapMaterialFlags, pFileMem, sizeof(RwUInt32));
	fwrite(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	RwInt32 iAtomicCnt;
	CopyMemory(&iAtomicCnt, pFileMem, sizeof(RwInt32));
	fwrite(&iAtomicCnt, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	for (int j = 0; j < iAtomicCnt; ++j)
	{
		RwBool bLitmapExisted;
		CopyMemory(&bLitmapExisted, pFileMem, sizeof(RwBool));
		fwrite(&bLitmapExisted, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		if (bLitmapExisted)
		{
			RwChar szAtomName[64];
			RwChar szTexName[64];
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
			{
				CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * 64);
				fwrite(&szAtomName, sizeof(RwChar) * 64, 1, pFile);
				pFileMem += sizeof(RwChar) * 64;

				CopyMemory(szTexName, pFileMem, sizeof(RwChar) * 64);
				fwrite(&szTexName, sizeof(RwChar) * 64, 1, pFile);
				pFileMem += sizeof(RwChar) * 64;
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = 0;

				CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				pFileMem += sizeof(RwUInt32);

				CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * uiLength);
				szAtomName[uiLength] = '\0';
				fwrite(&szAtomName, sizeof(RwChar) * uiLength, 1, pFile);
				pFileMem += sizeof(RwChar) * uiLength;

				CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				pFileMem += sizeof(RwUInt32);

				CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * uiLength);
				szAtomName[uiLength] = '\0';
				fwrite(&szAtomName, sizeof(RwChar) * uiLength, 1, pFile);
				pFileMem += sizeof(RwChar) * uiLength;
			}

			RwInt32 iUVCnt;
			CopyMemory(&iUVCnt, pFileMem, sizeof(RwInt32));
			fwrite(&iUVCnt, sizeof(RwInt32), 1, pFile);
			pFileMem += sizeof(RwInt32);

			RwTexCoords* pTexCoords = NTL_NEW RwTexCoords [ iUVCnt ];
			CopyMemory(pTexCoords, pFileMem, sizeof(RwTexCoords) * iUVCnt);
			fwrite(pTexCoords, sizeof(RwTexCoords) * iUVCnt, 1, pFile);
			pFileMem += sizeof(RwTexCoords) * iUVCnt;
			NTL_ARRAY_DELETE(pTexCoords);
		}
	}

	return pFileMem;
}

RwInt32 CNtlPLObject::SkipLightObjectAttachData(FILE* pFile)
{
	RwInt32 iLightObjectNum;
	fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for (int j = 0; j < iLightObjectNum; ++j)
	{
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;
			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);
		}

		fseek(pFile, sizeof(RpLightType), SEEK_CUR);
		fseek(pFile, sizeof(RwBool), SEEK_CUR);

		fseek(pFile, sizeof(RwV3d), SEEK_CUR);

		fseek(pFile, sizeof(RwV3d), SEEK_CUR);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
		}		

		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);

		fseek(pFile, sizeof(RwBool), SEEK_CUR);
		fseek(pFile, sizeof(RwBool), SEEK_CUR);
	}

	return ftell(pFile);
}

RwInt32 CNtlPLObject::SkipLightmap(FILE* pFile)
{
	::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // uiLtMapObjectFlag
	::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // uiLtMapMaterialFlags

	RwInt32 iAtomicCnt;
	::fread(&iAtomicCnt, sizeof(RwInt32), 1, pFile);

	for (int j = 0; j < iAtomicCnt; ++j)
	{
		RwBool bLitmapExisted;
		::fread(&bLitmapExisted, sizeof(RwBool), 1, pFile);

		if (bLitmapExisted)
		{
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = 0;

				fread(&uiLength, sizeof(RwUInt32), 1, pFile);
				fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);												

				fread(&uiLength, sizeof(RwUInt32), 1, pFile);
				fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);												
			}

			RwInt32 iUVCnt;
			::fread(&iUVCnt, sizeof(RwInt32), 1, pFile);
			::fseek(pFile, sizeof(RwTexCoords) * iUVCnt, SEEK_CUR);
		}
	}

	return ftell(pFile);
}

BYTE* CNtlPLObject::SkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMem);
	}

	// trigger obj serial id
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	pFileMem += sizeof(DWORD);

	// visibility distance
	pFileMem += sizeof(RwReal);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// enable trigger
		pFileMem += sizeof(RwBool);

		// enable selection
		pFileMem += sizeof(RwBool);

		// enable alterable
		pFileMem += sizeof(RwBool);

		// enable portal
		pFileMem += sizeof(RwBool);

		// enable path engine collision
		pFileMem += sizeof(RwBool);

		// enable ShadowProp
		pFileMem += sizeof(RwBool);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipPSMapToFileMem(pFileMem);

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipLightObjectAttachDataToFileMem(pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SkipLightmapToFileMem(pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SkipLightObjectAttachDataToFileMem(BYTE* pFileMem)
{
	RwInt32 iLightObjectNum;
	CopyMemory(&iLightObjectNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);

	for (int j = 0; j < iLightObjectNum; ++j)
	{
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;

			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			pFileMem += sizeof(RwUInt32);

			pFileMem += sizeof(RwChar) * uiLength;
		}

		pFileMem += sizeof(RpLightType);
		pFileMem += sizeof(RwBool);

		pFileMem += sizeof(RwV3d);

		pFileMem += sizeof(RwV3d);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			pFileMem += sizeof(RwRGBAReal);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			pFileMem += sizeof(RwRGBA);
		}

		pFileMem += sizeof(RwReal);
		pFileMem += sizeof(RwReal);

		pFileMem += sizeof(RwBool);
		pFileMem += sizeof(RwBool);
	}
	return pFileMem;
}

BYTE* CNtlPLObject::SkipLightmapToFileMem(BYTE* pFileMem)
{
	RwUInt32 uiLtMapObjectFlags;
	CopyMemory(&uiLtMapObjectFlags, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	RwUInt32 uiLtMapMaterialFlags;
	CopyMemory(&uiLtMapMaterialFlags, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	RwInt32 iAtomicCnt;
	CopyMemory(&iAtomicCnt, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);

	for (int j = 0; j < iAtomicCnt; ++j)
	{
		RwBool bLitmapExisted;
		CopyMemory(&bLitmapExisted, pFileMem, sizeof(RwBool));
		pFileMem += sizeof(RwBool);

		if (bLitmapExisted)
		{
			RwChar szAtomName[64];
			RwChar szTexName[64];
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
			{
				CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * 64);
				pFileMem += sizeof(RwChar) * 64;

				CopyMemory(szTexName, pFileMem, sizeof(RwChar) * 64);
				pFileMem += sizeof(RwChar) * 64;
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = 0;

				CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
				pFileMem += sizeof(RwUInt32);

				CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * uiLength);
				szAtomName[uiLength] = '\0';
				pFileMem += sizeof(RwChar) * uiLength;

				CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
				pFileMem += sizeof(RwUInt32);

				CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * uiLength);
				szAtomName[uiLength] = '\0';
				pFileMem += sizeof(RwChar) * uiLength;
			}

			RwInt32 iUVCnt;
			CopyMemory(&iUVCnt, pFileMem, sizeof(RwInt32));
			pFileMem += sizeof(RwInt32);

			pFileMem += sizeof(RwTexCoords) * iUVCnt;
		}
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMem);
	}

	// trigger obj serial id
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	pFileMem += sizeof(DWORD);

	// visibility distance
	pFileMem += sizeof(RwReal);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// enable trigger
		CopyMemory(pResult, pFileMem, sizeof(RwBool));
		pFileMem += sizeof(RwBool);

		// enable selection
		pFileMem += sizeof(RwBool);

		// enable alterable
		pFileMem += sizeof(RwBool);

		// enable portal
		pFileMem += sizeof(RwBool);

		// enable path engine collision
		pFileMem += sizeof(RwBool);

		// enable ShadowProp
		pFileMem += sizeof(RwBool);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiFlags = 0;
		CopyMemory(&uiFlags, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		*pResult = ((uiFlags & NTL_PLEFLAG_TRIGGER) != 0);
	}

	pFileMem = SkipPSMapToFileMem(pFileMem);

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipLightObjectAttachDataToFileMem(pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SkipLightmapToFileMem(pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMem);
	}

	// trigger obj serial id
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	pFileMem += sizeof(DWORD);

	// visibility distance
	pFileMem += sizeof(RwReal);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// enable trigger	
		pFileMem += sizeof(RwBool);

		// enable selection
		pFileMem += sizeof(RwBool);

		// enable alterable
		pFileMem += sizeof(RwBool);

		// enable portal
		pFileMem += sizeof(RwBool);

		// enable path engine collision
		CopyMemory(pResult, pFileMem, sizeof(RwBool));
		pFileMem += sizeof(RwBool);

		// enable ShadowProp
		pFileMem += sizeof(RwBool);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiFlags = 0;
		CopyMemory(&uiFlags, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		*pResult = ((uiFlags & NTL_PLEFLAG_PECOLLISION) != 0);
	}


	pFileMem = SkipPSMapToFileMem(pFileMem);

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipLightObjectAttachDataToFileMem(pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SkipLightmapToFileMem(pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(pFile, pFileMem);
	}

	// trigger obj serial id
	::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	::fwrite(pFileMem, sizeof(DWORD), 1, pFile);
	pFileMem += sizeof(DWORD);

	// visibility distance
	CLAMP(fVD, 70, 600);
	CLAMP(fMinVD, 70, 600);
	CLAMP(fMaxVD, 70, 600);

	RwReal fCurVD;
	CopyMemory(&fCurVD, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	if(fCurVD > fMinVD && fCurVD < fMaxVD)
	{
		::fwrite(&fVD, sizeof(RwReal), 1, pFile);
	}
	else
	{
		::fwrite(&fCurVD, sizeof(RwReal), 1, pFile);
	}

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// enable trigger
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable selection
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable alterable
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable portal
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable path engine collision
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		// enable ShadowProp
		::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		::fwrite(pFileMem,  sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SavePSMapFromFileMem(pFile, pFileMem);

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	fwrite(&EnableFlg, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SaveLightObjectAttachDataFromFileMem(pFile, pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SaveLightmapFromFileMem(pFile, pFileMem);
	}

	return pFileMem;
}

RwBool CNtlPLObject::SavePSMap(FILE* pFile)
{
	RwInt32 PSAtomicCnt = 0;
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(m_vecAtomicList[i]->pData));
		if(pUserDat && pUserDat->m_pTexShadow)
		{
			++PSAtomicCnt;
		}
	}

	fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(m_vecAtomicList[i]->pData));
		if(pUserDat && pUserDat->m_pTexShadow)
		{
			// atomic name to match for loading
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
			{
				fwrite(pUserDat->m_pTexShadow->name, rwTEXTUREBASENAMELENGTH, 1, pFile);
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = strlen(pUserDat->m_pTexShadow->name);

				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				fwrite(pUserDat->m_pTexShadow->name, uiLength, 1, pFile);
			}

			// frustum view window
			fwrite(&pUserDat->m_CamViewWindowX, sizeof(RwReal), 1, pFile);

			// light matrices
			fwrite(&pUserDat->m_matLight, sizeof(RwMatrix), 1, pFile);

// 			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
// 			{			
// 			}
// 			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
// 			{
// 				// Save 될 때 섹터의 그림자 색상을 저장한다. 이 후 로드 때 PSMap 색상이 변할 수 있다.
// 				// Load 시 하기 위해서는 Shadow 정보가 Object보다 먼저 Load되야 하지만 현재 Shadow는 Object보다 나중에 읽기 때문에
// 				// 지금은 이렇게 해결한다. 나중에 순서를 바꾸어 저장하는 방법을 검토해 본다.
// 
// 				CNtlWorldFieldManager* pMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 
// 				RwInt32 iSectorIdx = pMgr->GetSectorIdx(GetPosition());
// 				if (iSectorIdx != -1)
// 				{
// 					pUserDat->m_rgbaShadow = pMgr->m_pSectors[iSectorIdx].m_pNtlWorldShadow->m_rgbaShadow;
// 				}			
// 
// 				fwrite(&pUserDat->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
// 			}

			// size
			RwInt32 Size = RwRasterGetWidth(pUserDat->m_pTexShadow->raster);
			fwrite(&Size, sizeof(RwInt32), 1, pFile);

			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
			{
				// texture data
				CNtlPLResourceManager::GetInstance()->SaveTexture(pUserDat->m_pTexShadow, pFile);
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
			{
				CNtlPLResourceManager::GetInstance()->SaveTextureShadowMap(pUserDat->m_pTexShadow, pFile);
				//CNtlPLResourceManager::GetInstance()->SaveAlphaBitsShadowMap(pUserDat->m_pTexShadow, pFile);
			}
		}
	}
	return TRUE;
}

RwBool CNtlPLObject::LoadPSMap(FILE* pFile)
{
	RwInt32 PSAtomicCnt = 0;
	fread(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < PSAtomicCnt; ++i)
	{
		CNtlWorldShadow* pLoadNWS = NTL_NEW CNtlWorldShadow;

		// name
		RwChar ReadBuff[rwTEXTUREBASENAMELENGTH] = {0,};

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fread(ReadBuff, sizeof(RwChar) * uiLength, 1, pFile);
			ReadBuff[uiLength] = '\0';
		}

		// frustum view window
		fread(&pLoadNWS->m_CamViewWindowX, sizeof(RwReal), 1, pFile);

		// light matrices
		fread(&pLoadNWS->m_matLight, sizeof(RwMatrix), 1, pFile);

// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
// 		{			
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
// 		{			
// 			fread(&pLoadNWS->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
// 		}

		RwInt32 Size = 0;
		fread(&Size, sizeof(RwInt32), 1, pFile);
		pLoadNWS->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture(ReadBuff, Size, Size, 32, rwRASTERFORMAT8888);
		DBO_ASSERT(pLoadNWS->m_pTexShadow, "Texture load failed.");

		RwTextureSetFilterMode(pLoadNWS->m_pTexShadow, rwFILTERLINEAR);
		RwTextureSetAddressing(pLoadNWS->m_pTexShadow, rwTEXTUREADDRESSCLAMP);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
		{
			CNtlPLResourceManager::GetInstance()->LoadTexture(pLoadNWS->m_pTexShadow, pFile);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{			
			CNtlPLResourceManager::GetInstance()->LoadTextureShadowMap(pLoadNWS->m_pTexShadow, pFile);
			//CNtlPLResourceManager::GetInstance()->LoadAlphaBitsShadowMap(pLoadNWS->m_pTexShadow, pFile, &pLoadNWS->m_rgbaShadow);
		}


		m_vecNtlWorldShadow.push_back(pLoadNWS);
	}
	return TRUE;
}

RwInt32 CNtlPLObject::SkipPSMap(FILE* pFile)
{
	// PS Map Size
	RwInt32 PSAtomicCnt = 0;
	::fread(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
		{
			::fseek(pFile, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, SEEK_CUR);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;

			::fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			::fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);
		}


		// Frustum view window extent
		::fseek(pFile, sizeof(RwReal), SEEK_CUR);

		// Light matrices
		::fseek(pFile, sizeof(RwMatrix), SEEK_CUR);

// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
// 		{			
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
// 		{
// 			::fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
// 		}

		// Map size
		RwInt32 Size = 0;
		::fread(&Size, sizeof(RwInt32), 1, pFile);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
		{			
			::fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			::fseek(pFile, sizeof(RwUInt8) * 3 * Size * Size, SEEK_CUR);
			//::fseek(pFile, sizeof(RwUInt8) * Size * Size, SEEK_CUR);
		}

	}

	return ::ftell(pFile);
}

BYTE* CNtlPLObject::SavePSMapFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			fwrite(pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
			pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;

			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			pFileMem += sizeof(RwUInt32);

			fwrite(pFileMem, sizeof(RwChar) * uiLength, 1, pFile);
			pFileMem += sizeof(RwChar) * uiLength;
		}		

		// Frustum view window extent
		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);

		// Light matrices
		fwrite(pFileMem, sizeof(RwMatrix), 1, pFile);
		pFileMem += (sizeof(RwMatrix));

// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
// 		{
// 			fwrite(pFileMem, sizeof(RwRGBA), 1, pFile);
// 			pFileMem += sizeof(RwRGBA);
// 		}

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		fwrite(&Size, sizeof(RwInt32), 1, pFile);
		pFileMem += (sizeof(RwInt32));

		// Map
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			fwrite(pFileMem, sizeof(DWORD) * Size * Size, 1, pFile);
			pFileMem += (sizeof(DWORD) * Size * Size);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			fwrite(pFileMem, sizeof(RwUInt8) * 3 * Size * Size, 1, pFile);
			pFileMem += (sizeof(RwUInt8) * 3 * Size * Size);
		}


	}
	return pFileMem;
}

BYTE* CNtlPLObject::SkipPSMapToFileMem(BYTE* pFileMem)
{
	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;

			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));			
			pFileMem += sizeof(RwUInt32);
			pFileMem += sizeof(RwChar) * uiLength;
		}			

		// Frustum view window extent
		pFileMem += sizeof(RwReal);

		// Light matrices
		pFileMem += (sizeof(RwMatrix));

// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
// 		{
// 			pFileMem += sizeof(RwRGBA);
// 		}

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		pFileMem += (sizeof(RwInt32));

		// Map
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			pFileMem += (sizeof(DWORD) * Size * Size);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			pFileMem += (sizeof(RwUInt8) * 3 * Size * Size);
		}
	}

	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CNtlPLObject::CNtlPLObject()
{
	m_pProperty         = NULL;
	m_pClumpResource	= NULL;
	m_pUVAnim           = NULL;
	m_pBaseHierarchy    = NULL;
	m_pAnimResource     = NULL;
	m_bAnimPlay         = FALSE;	
	m_bHaveAnim			= FALSE;

	m_fAnimSpeed		= 1.f;

	ZeroMemory( &m_vWorldPosition, sizeof( RwV3d ) );
	ZeroMemory( &m_vModelAngle, sizeof( RwV3d ) );

	m_vModelScale.x = m_vModelScale.y = m_vModelScale.z = 1.0f;

	SetLayer(PLENTITY_LAYER_RWWORLD);
	SetMinimapLayer(NTL_PLEFLAG_MINIMAP_LAYER_ONE);
	SetClassType(PLENTITY_OBJECT);
	SetFlags(NTL_PLEFLAG_ATTACH | NTL_PLEFLAG_NOTUPDATE | NTL_PLEFLAG_FADE | NTL_PLEFLAG_COLLISION);

	m_sColor.red        = 255;
	m_sColor.green		= 255;
	m_sColor.blue		= 255;
	m_sColor.alpha		= 255;

	m_sAddColor.red		= 0;
	m_sAddColor.green   = 0;
	m_sAddColor.blue	= 0;
	m_sAddColor.alpha	= 255;

	m_eFadeState        = FADE_VISIBLE;    
	m_fFadeTime         = 0.0f;    

	m_bActiveUpdate		= FALSE;
	m_bLoadMap			= TRUE;

	m_pAnimLayer		= NULL;
	m_pInstanceAnimTable = NULL;
	m_uiCurAnimKey		= INVALID_DWORD;

	m_uiMilepostTextID	= NULL;
	m_pFadeBlend		= NULL;
	m_pToonData			= NULL;

	m_uiObjectSerialID	= NULL;

	m_pSchedulingLMProp = NULL;

	m_pDojoEntity		= NULL;

	m_pObjectType		= NULL;

	ZeroMemory(&m_bbox, sizeof(RwBBox));

	m_vecNtlWorldShadow.clear();
	m_vecNtlWorldShadow.reserve(4);

#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->AddCulling(this);
#endif
}

CNtlPLObject::~CNtlPLObject()
{
#ifndef dNTL_WORLD_CULLING_NEW
	GetNtlPLCullingScheduler()->RemoveCulling(GetClassType(), this);
#endif

	m_vecNtlWorldShadow.clear();

	FreeSchedulingLTProp();
}

void* CNtlPLObject::operator new(size_t size)
{
	NTL_FUNCTION("CNtlPLObject::operator new");

	NTL_RETURN(CNtlPLEntityFreeList::Alloc(PLENTITY_OBJECT));
}

void CNtlPLObject::operator delete(void *pObj)
{
	CNtlPLEntityFreeList::Free(PLENTITY_OBJECT, pObj);
}

RwBool CNtlPLObject::CreateScheduling(const SPLEntityCreateParam * pParam)
{
	NTL_FUNCTION("CNtlPLObject::CreateScheduling");

	DBO_ASSERT(pParam != NULL, "null create param");
	if(pParam)
	{
		SPLObjectCreateParam* pObjectParam = (SPLObjectCreateParam*)pParam;
		m_bLoadMap = pObjectParam->bLoadMap;
		RwV3dAssign(&m_vWorldPosition, pParam->pPos);
	}

	std::string strDff = CNtlPLObjectProperty::m_strMeshPath + m_pProperty->m_strDffFile;
	if( m_pProperty->m_strDffFile.c_str() != NULL && rwstrlen( m_pProperty->m_strDffFile.c_str() ) > 0 )
		CNtlPLResourceManager::GetInstance()->LoadSchedulingClump( strDff.c_str(), CNtlPLObjectProperty::m_strTexPath.c_str(), this );   

	NTL_RETURN( TRUE );
}

RwBool CNtlPLObject::CreateThreadSafe(void)
{
	NTL_SPROFILE("CNtlPLObject::CreateThreadSafe");

	Helper_SetClumpAllAtomics(GetClump(), &m_vecAtomicList);		
	Helper_GetBoneList(GetClump(), &m_mapFrame);            // Bone List를 저장해 둔다.

	// Anim을 적용한다
	if(m_pProperty->m_strAnimFileName.size() > 0)
	{
		SetAnimation(m_pProperty->m_strAnimFileName.c_str());
	}

    // 트리거 애니메이션이 적용된게 있으면 적용한다.
    m_sScheduleInfo.bLoadComplete = TRUE;
    if(m_sScheduleInfo.uiAnimKey > 0)
    {
        SetTriggerAnimation(m_sScheduleInfo.uiAnimKey, m_sScheduleInfo.fAnimStartTime, m_sScheduleInfo.bAnimLoop);
    }
    

	// UVAnim을 Clump에 적용한다.
	if(m_pUVAnim)
	{
		m_pUVAnim->SetClump(GetClump());
		m_pUVAnim->SetUVAnimSpeed(m_pProperty->GetUVAnimSpeed());

		m_bHaveAnim = TRUE;
		m_bAnimPlay = TRUE;
	}

	// bbox Update
	m_bbox = GetTriggerAABBInfo();

	if(m_pProperty->GetCollision())
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_PECOLLISION);
	}

	if(m_pProperty->GetEnableShadow())
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_SHADOW);
	}

	// clump에 대한 atomic
	if( GetClump() != NULL )
	{
		// Toon
		CheckToonData();

		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
			RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );            
			RpAtomicSetRenderCallBack(pAtomic, CNtlPLObject::RenderCallBack);            			
			RpNtlAtomicSetData(pAtomic, this);

			// 폭포처럼 UV애니만 쓰는 오브젝트도 FX파이프라인, 환경맵을 가지고 있어도 FX파이프라인을 가지고 있으며
			// 밑에 if(!RpMaterialUVAnimExists(pMaterial))에서 또한번 체크하므로 다음 문장을 주석처리합니다.
			// 환경맵, UV애니등을 가진 오브젝트가 문제가 생긴다면 이부분이 다시 한번 고려가 되야 합니다.
			// 주석을 풀게 되면 UV와 환경맵을 가진 오브젝트를 선택할시 파이프라인이 틀리므로 addcolor되지 않습니다.
			// - 우택
			//if(pAtomic->pipeline == NULL && m_pUVAnim == NULL)
			if(pAtomic->pipeline == NULL)
			{
				RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
				if(pGeom != NULL)
				{
					RwInt32 nNumMat = RpGeometryGetNumMaterials(pGeom);
					for(int i = 0; i < nNumMat; i++)
					{
						RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
						if(pMaterial != NULL)
						{
							if(!RpMaterialUVAnimExists(pMaterial))
								NtlMatExtSetPipeline(pAtomic);
						}
					}

				}
			}
		}

		RwMatrixSetIdentity(RwFrameGetMatrix(RpClumpGetFrame(GetClump())));

		RpNtlClumpSetData( GetClump(), this );

		SetTransform();

		// BBox Update
		m_bbox = GetTriggerAABBInfo();

		CalcBoundingSphere();        
	}

	if(GetNtlResourceManager()->IsLoadScheduling())
	{		
		if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			LoadLightmap(NULL);
		}

		AttachLightObject();
	}

	// 블렌드 객체를 생성하고 리스트에 추가한다.
	m_pFadeBlend = GetAlphaBlendController()->AddAlpha(1.0f);

	// Link Effect를 설정한다.
	for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
	{
		SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];        
		m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
	}
	if(!m_pProperty->m_vLinkEffect.empty())
	{
		Update(0.0f);       // 링크이펙트가 있으면 최초에 업데이트를 한번 해줘야 한다. (Effect가 Object보다 Update호출이 빠르기 때문)
	}

	// Sound를 재생한다.
	if(strlen(m_pProperty->GetSoundProp()->chSoundName) > 0)
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_OBJECT_MUSIC;
		tSoundParam.pcFileName		= m_pProperty->GetSoundProp()->chSoundName;
		tSoundParam.bLoop			= true;
		tSoundParam.fVolume			= m_pProperty->GetSoundProp()->fSoundVolume * 0.01f;
		tSoundParam.fPitch			= m_pProperty->GetSoundProp()->fSoundPitchMin;
		tSoundParam.fXPos			= m_vWorldPosition.x;
		tSoundParam.fYPos			= m_vWorldPosition.y;
		tSoundParam.fZPos			= m_vWorldPosition.z;
		tSoundParam.fMinDistance	= m_pProperty->GetSoundProp()->fSoundDist;
		tSoundParam.fMaxDistance	= m_pProperty->GetSoundProp()->fSoundDecayDist;

		GetSoundManager()->Play(&tSoundParam);

		AddLoopSound(tSoundParam.hHandle);
	}

	// 처음 맵에 진입시에 나타나는 오브젝트들은 Fade 효과를 적용하지 않는다.    
	if(m_bLoadMap || (!GetSceneManager()->GetDistanceFilterEnable()))
	{
		m_eFadeState = FADE_VISIBLE;	        
	}
	else
	{
		m_eFadeState = FADE_NOT_VISIBLE;       
		m_pFadeBlend->SetWeight(0.0f);
		SetWeightAlpha(0.0f);
	}

	CreateOccluderProxy();
	
	UpdateRpSectorOBBCheck();

	m_bActiveUpdate = TRUE;

	NTL_RPROFILE(TRUE);
	//return TRUE;
}

RwBool CNtlPLObject::CreateOccluderProxy()
{
#ifdef dNTL_WORLD_CULLING_NEW
	// +1 == Bounding Sphere Check를 위해서 사용한다.
	return COccluderProxy::CreateOccluderProxy(0.5f, m_vecAtomicList.size() + 1);
#else
	if (m_bHaveAnim)
	{
		return COccluderProxy::CreateOccluderProxy(0.5f, m_vecAtomicList.size());
	}
	else // Animetion 을 가지고 않을 경우는 Entity Bounding Sphere로 처리한다.
	{
		return COccluderProxy::CreateOccluderProxy(0.5f, 1);
	}
#endif

}

RwBool CNtlPLObject::SaveLightmap(FILE* pFile)
{
	RpAtomic*		pCurAtomic;
	RwTexture*		pCurTexture;
	RwInt32			UVCnt;
	RwTexCoords*	pTexCoords4Second;
	RwChar			szAtomName[64] = {0,};
	RwChar			szLMTexName[64] = {0,};
	RwInt32			TotAtomicCnt = m_vecAtomicList.size();
	RwBool			LitmapExisted;

	RwUInt32		uiLtMapObjectFlags = GetLightMapObjectFlags();
	RwUInt32		uiLtMapMaterialFlags = GetLightMapMaterialFlags();
	
	fwrite(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
	fwrite(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);

	fwrite(&TotAtomicCnt, sizeof(RwInt32), 1, pFile);
	for(RwInt32 i = 0; i < TotAtomicCnt; i++)
	{
		pCurAtomic	= m_vecAtomicList[i]->pData;
		pCurTexture = RpLtMapAtomicGetLightMap(pCurAtomic);

		if(pCurTexture)
			LitmapExisted = TRUE;
		else
			LitmapExisted = FALSE;

		fwrite(&LitmapExisted, sizeof(RwBool), 1, pFile);

		if(LitmapExisted)
		{
			strcpy_s(szAtomName, 64, Helper_AtomicName(pCurAtomic));
			fwrite(szAtomName, sizeof(RwChar) * 64, 1, pFile);

			sprintf_s(szLMTexName, "%u%s", m_uiObjectSerialID, szAtomName);
			CNtlWorldLTManager::GetInstance()->SaveLightmap(pCurTexture, szLMTexName);
			fwrite(szLMTexName, sizeof(RwChar) * 64, 1, pFile);

			UVCnt = pCurAtomic->geometry->numVertices;
			fwrite(&UVCnt, sizeof(RwInt32), 1, pFile);

			pTexCoords4Second = pCurAtomic->geometry->texCoords[1];
			fwrite(pTexCoords4Second, sizeof(RwTexCoords) * UVCnt, 1, pFile);
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::LoadSchedulingLightmapProp(FILE* pFile)
{
	AllocSchedulingLTProp();

	fread(&m_pSchedulingLMProp->_uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
	fread(&m_pSchedulingLMProp->_uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);

	RwInt32	TotAtomicCnt;
	fread(&TotAtomicCnt, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < TotAtomicCnt; i++)
	{
		RwBool LitmapExisted;
		fread(&LitmapExisted, sizeof(RwBool), 1, pFile);

		if(LitmapExisted)
		{
			sSCHEDULING_LM_ATOM_PROP* psSchedulingLMAtomProp = NTL_NEW sSCHEDULING_LM_ATOM_PROP;

			fread(psSchedulingLMAtomProp->_szAtomName, sizeof(RwChar) * 64, 1, pFile);
			fread(psSchedulingLMAtomProp->_szTexName, sizeof(RwChar) * 64, 1, pFile);

			fread(&psSchedulingLMAtomProp->_UVCnt, sizeof(RwInt32), 1, pFile);
			psSchedulingLMAtomProp->_pTexCoords = NTL_NEW RwTexCoords [psSchedulingLMAtomProp->_UVCnt];
			fread(psSchedulingLMAtomProp->_pTexCoords, sizeof(RwTexCoords) * psSchedulingLMAtomProp->_UVCnt, 1, pFile);

			m_pSchedulingLMProp->_vecSchedulingLMAtomProp.push_back(psSchedulingLMAtomProp);
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::LoadLightmap(FILE* pFile)
{
	RwChar			szTexName[64] = {0,};
	RwInt32			UVCnt = 0;
	RwChar			szAtomName[64] = {0,};
	RwInt32			TotAtomicCnt;
	RwBool			LitmapExisted = FALSE;
	RwChar*			pOldAtomicName = NULL;
	RwTexCoords*	pUVDat = NULL;
	RpAtomic*		pCurAtomic = NULL;
	RwTexture*		pCurLitmap = NULL;
	RwChar			szFullPathName[256];
	RwChar			szBPS[64];

	if(GetNtlResourceManager()->IsLoadScheduling())
	{
		if(!m_pSchedulingLMProp)
			return TRUE;

		SetLightMapObjectFlags(m_pSchedulingLMProp->_uiLtMapObjectFlags);
		SetLightMapMaterialFlags(m_pSchedulingLMProp->_uiLtMapMaterialFlags);

		for(RwUInt32 i = 0; i < m_pSchedulingLMProp->_vecSchedulingLMAtomProp.size(); ++i)
		{
			sSCHEDULING_LM_ATOM_PROP* pThe = m_pSchedulingLMProp->_vecSchedulingLMAtomProp[i];

			for(RwUInt32 k = 0; k < m_vecAtomicList.size(); ++k)
			{
				pOldAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
				if(!strcmp(pOldAtomicName, pThe->_szAtomName))
				{
					pCurAtomic = m_vecAtomicList[i]->pData;
					break;
				}
				else
					pCurAtomic = NULL;
			}

			if(pCurAtomic)
			{
				_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

				strcpy_s(szFullPathName, 256, ".\\Texture\\ntlwe\\ltmap\\");
				strcat_s(szFullPathName, 256, szBPS);
				strcat_s(szFullPathName, 256, "\\Objlm\\");

				pCurLitmap = GetNtlResourceManager()->LoadTexture(pThe->_szTexName, szFullPathName);
				DBO_ASSERT(pCurLitmap, "Texture load failed.");
				RwTextureSetFilterMode(pCurLitmap, rwFILTERLINEAR);

				RpLtMapAtomicSetLightMap(pCurAtomic, pCurLitmap);
				RpAtomicSetPipeline(pCurAtomic, RpLtMapGetPlatformAtomicPipeline());

				RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKTEXCOORDS2);
				CopyMemory(pCurAtomic->geometry->texCoords[1], pThe->_pTexCoords, sizeof(RwTexCoords) * pThe->_UVCnt);
				RpGeometryUnlock(pCurAtomic->geometry);
			}
		}
		FreeSchedulingLTProp();
	}
	else
	{
		RwUInt32 uiLtMapObjectFlags;
		RwUInt32 uiLtMapMaterialFlags;

		fread(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
		fread(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);

		SetLightMapObjectFlags(uiLtMapObjectFlags);
		SetLightMapMaterialFlags(uiLtMapMaterialFlags);

		fread(&TotAtomicCnt, sizeof(RwInt32), 1, pFile);
		for(RwInt32 i = 0; i < TotAtomicCnt; i++)
		{
			fread(&LitmapExisted, sizeof(RwBool), 1, pFile);
			if(LitmapExisted)
			{
				fread(szAtomName, sizeof(RwChar) * 64, 1, pFile);

				for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
				{
					pOldAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
					if(!strcmp(pOldAtomicName, szAtomName))
					{
						pCurAtomic = m_vecAtomicList[i]->pData;
						break;
					}
					else
						pCurAtomic = NULL;
				}

				if(pCurAtomic)
				{
					fread(szTexName, sizeof(RwChar) * 64, 1, pFile);

					_itoa_s(dGET_WORLD_PARAM()->WorldResourceID, szBPS, sizeof(szBPS), 10);

					strcpy_s(szFullPathName, 256, ".\\Texture\\ntlwe\\ltmap\\");
					strcat_s(szFullPathName, 256, szBPS);
					strcat_s(szFullPathName, 256, "\\Objlm\\");

					pCurLitmap = GetNtlResourceManager()->LoadTexture(szTexName, szFullPathName);
					DBO_ASSERT(pCurLitmap, "Texture load failed.");
					RwTextureSetFilterMode(pCurLitmap, rwFILTERLINEAR);

					RpLtMapAtomicSetLightMap(pCurAtomic, pCurLitmap);
					RpAtomicSetPipeline(pCurAtomic, RpLtMapGetPlatformAtomicPipeline());

					fread(&UVCnt, sizeof(RwInt32), 1, pFile);

					RpGeometryLock(pCurAtomic->geometry, rpGEOMETRYLOCKTEXCOORDS2);
					fread(pCurAtomic->geometry->texCoords[1], sizeof(RwTexCoords) * UVCnt, 1, pFile);
					RpGeometryUnlock(pCurAtomic->geometry);
				}
			}
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::Create( const SPLEntityCreateParam * pParam )
{
	return CreateScheduling(pParam);
}

void CNtlPLObject::DeleteLightmapFile()
{
	if(GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		string	strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;
		string	strSubPath	= "\\texture\\ntlwe\\indoor\\lightmap\\";
		string	strFileName;
		string	strExt		= ".png";
		RwChar	szAtomName[64];

		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
		{
			strcpy_s(szAtomName, 64, Helper_AtomicName(m_vecAtomicList[i]->pData));

			strFullPath	= dGET_WORLD_PARAM()->CurWorkingFolderName;

			RwChar szIdxTmp[64];
			sprintf_s(szIdxTmp, "%u%s", m_uiObjectSerialID, szAtomName);
			strFileName = szIdxTmp;
			strFullPath = strFullPath + strSubPath + strFileName + strExt;

			if((_access(strFullPath.c_str(), 0)) != -1)
			{
				// The file exists, force it to do write permission
				SetFileAttributes(strFullPath.c_str(), FILE_ATTRIBUTE_ARCHIVE);
				::DeleteFile(strFullPath.c_str());
			}
		}
	}
}

void CNtlPLObject::CheckLightmapFlags()
{
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
		{
			if (!(RpNtlAtomicGetFlag(m_vecAtomicList.at(i)->pData) & NTL_NOT_VISIBLE)) 
			{
				return;
			}
		}
		SetLightMapObjectFlags(rtLTMAPOBJECTNOSHADOW);
	}
}

RwBool CNtlPLObject::LoadLightObjectAttachData(FILE* pFile)
{
	RwInt32 iLightObjectNum;
	fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for (int i = 0; i < iLightObjectNum; ++i)
	{
		SLightObjectAttachData* pLightObjectAttachData		= NTL_NEW SLightObjectAttachData;
		pLightObjectAttachData->pPLLightObjectCreateParam	= NTL_NEW SPLLightObjectCreateParam;

		SPLLightObjectCreateParam* pCreateParam = pLightObjectAttachData->pPLLightObjectCreateParam;

		fread(pLightObjectAttachData->chBoneName, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, pFile);

		fread(&pCreateParam->rpLightType, sizeof(RpLightType), 1, pFile);
		fread(&pCreateParam->bRealLight, sizeof(RwBool), 1, pFile);
		
		fread(&pLightObjectAttachData->vPos, sizeof(RwV3d), 1, pFile);
		pCreateParam->pPos = &pLightObjectAttachData->vPos;

		fread(&pCreateParam->vRotate, sizeof(RwV3d), 1, pFile);
		fread(&pCreateParam->rgbafColor, sizeof(RwRGBAReal), 1, pFile);
		fread(&pCreateParam->fRadius, sizeof(RwReal), 1, pFile);
		fread(&pCreateParam->fAngle, sizeof(RwReal), 1, pFile);

		// CZ-LO
		fread(&pCreateParam->bDecal, sizeof(RwBool), 1, pFile);
		fread(&pCreateParam->bWorldLight, sizeof(RwBool), 1, pFile);

		m_vecAttachLightParam.push_back(pLightObjectAttachData);
	}

	if(!GetNtlResourceManager()->IsLoadScheduling())
	{
		AttachLightObject();
	}

	return TRUE;
}

void CNtlPLObject::AttachLightObject()
{
	for (RwInt32 i = 0; i < (RwInt32)m_vecAttachLightParam.size(); ++i)
	{
		SLightObjectAttachData* pLightObjectAttachData = m_vecAttachLightParam.at(i);

		CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", pLightObjectAttachData->pPLLightObjectCreateParam));
		DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

		RwBool bReturn = AttachBone(pNtlPLLightObject, pLightObjectAttachData->chBoneName);
		NTL_ASSERT(bReturn, "Attach Bone Failed.");
		if (!bReturn)
		{
#ifndef dNTL_WORLD_TOOL_MODE
			GetSceneManager()->DeleteEntity(pNtlPLLightObject);
#endif
		}

		NTL_DELETE(pLightObjectAttachData->pPLLightObjectCreateParam);
		NTL_DELETE(pLightObjectAttachData);
	}
	m_vecAttachLightParam.clear();
}

RwBool CNtlPLObject::SaveLightObjectAttachData(FILE* pFile)
{
	RwInt32 iLightObjectNum = 0;
	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
		{
			continue;
		}

		if(pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
		{
			++iLightObjectNum;
		}
	}
	fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
		{
			continue;
		}

		if(pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
		{
			CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pPLAttachAttr->pPLEntity;

			fwrite(pPLAttachAttr->chBoneName, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, pFile);

			RpLightType eType;
			eType = pLightObject->GetLightType();
			fwrite(&eType, sizeof(RpLightType), 1, pFile);

			RwBool bTemp;
			bTemp = pLightObject->IsRealLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			RwV3d vTemp;
			vTemp = pLightObject->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pLightObject->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			RwRGBAReal rgbafTemp;
			rgbafTemp = pLightObject->GetColor();
			fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);

			RwReal fTemp;
			fTemp = pLightObject->GetRadius();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pLightObject->GetAngle();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			bTemp = pLightObject->IsEnabledDecal();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			bTemp = pLightObject->IsEnabledWorldLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);			
		}
	}
	return TRUE;
}

void CNtlPLObject::Destroy(void) 
{
	NTL_FUNCTION("CNtlPLObject::Destroy");

	if (CNtlPLWorldState::GetActiveMiniIndoor() == this)
	{
		CNtlPLWorldState::InitSpace();
	}

	if (m_pObjectType)
	{
		m_pObjectType->Destroy();
		NTL_DELETE(m_pObjectType);
	}

	// delete lightobject 
	for each(SPLAttachAttr* pPLAttachAttr in m_listAttachAttr)
	{
		if(!pPLAttachAttr->pPLEntity)
		{
			continue;
		}

		if(pPLAttachAttr->pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
		{
			pPLAttachAttr->pPLEntity->SetAttachParent(NULL);
			GetSceneManager()->DeleteEntity(pPLAttachAttr->pPLEntity);
			pPLAttachAttr->pPLEntity = NULL;
		}
	}

	for (RwInt32 i = 0; i < (RwInt32)m_vecAttachLightParam.size(); ++i)
	{
		SLightObjectAttachData* pLightObjectAttachData = m_vecAttachLightParam.at(i);

		NTL_DELETE(pLightObjectAttachData->pPLLightObjectCreateParam);
		NTL_DELETE(pLightObjectAttachData);
	}
	m_vecAttachLightParam.clear();

	// remove PS map before destroying clumps
	if(GetEnableGenShadowMap())
	{
		ENTITY_ATOMIC_VEC*		_pEAV	= GetAtomicList();
		ENTITY_ATOMIC_VEC_ITER	_The	= _pEAV->begin();

		while(_The != _pEAV->end())
		{
			CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(static_cast<RpAtomic*>((*_The)->pData)));
			NTL_DELETE(pUserDat);
			RpNtlAtomicSetUserDat(static_cast<RpAtomic*>((*_The)->pData), NULL);
			_The++;
		}
	}
	
	// remove PS map before destroying clumps
	DestroyEmblemMap();
	
	m_vecNtlWorldShadow.clear();

	ClearLoopEffect();
	ClearLoopSound();

	NTL_DELETE(m_pUVAnim);	

	if(m_pAnimResource)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pAnimResource);
		m_pAnimResource = NULL;
	}

	if(m_pBaseHierarchy)
	{
		RpHAnimHierarchyDestroy(m_pBaseHierarchy);
		m_pBaseHierarchy = NULL;
	}

	// Toon
	if(m_pToonData)
	{
		RpToonPaintDestroy(m_pToonData->pToonPaint);
		RpToonInkDestroy(m_pToonData->pToonInk);
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pToonData->pTexture);
		NTL_DELETE(m_pToonData);
		m_pToonData = NULL;
	}

	if( m_pClumpResource != NULL )
	{
		Helper_DestroyAllAtomics(&m_vecAtomicList);
		RemoveWorld();		
	}

	// 여기서는 m_pClumpResource의 null pointer를 검사하지 않는다(형석)
	CNtlPLResourceManager::GetInstance()->UnLoadScheduling(this, m_pClumpResource );
	m_pClumpResource = NULL;

	if(m_pInstanceAnimTable)
	{
		m_pInstanceAnimTable->Destroy();
		NTL_DELETE(m_pInstanceAnimTable);
	}

	if(m_pAnimLayer)
	{
		m_pAnimLayer->Destroy();
		NTL_DELETE(m_pAnimLayer);
	}

	DestroyOccluderProxy();

	NTL_RETURNVOID();
}

RwBool CNtlPLObject::CheckUVsSetUpForLightMapping()
{
	RwUInt32	axes[3];
	RwBool		outOfRange = FALSE;
	RwBool		unEqual = FALSE;

	for(RwUInt32 j = 0; j < m_vecAtomicList.size(); ++j)
	{
		RpGeometry*		geom		= m_vecAtomicList[j]->pData->geometry;

		if (RpGeometryGetNumTexCoordSets(geom) > 1)
		{
			RwTexCoords*	texCoords	= RpGeometryGetVertexTexCoords(geom, (RwTextureCoordinateIndex)2);
			RpTriangle*		triangles	= RpGeometryGetTriangles(geom);

			for(RwInt32 k = 0; k < RpGeometryGetNumTriangles(geom); ++k)
			{
				for(RwUInt32 n = 0; n < 3; n++)
				{
					axes[n] = RwFastRealToUInt32(10.0001f *	texCoords[triangles[k].vertIndex[n]].u);
					if(axes[n] > 5)
					{
						outOfRange = TRUE;
					}
				}

				// Make sure UVs are the same for all vertices in a triangle
				if((axes[0] != axes[1]) || (axes[1] != axes[2]))
				{
					unEqual = TRUE;
				}
			}
		}
	}

	if(outOfRange || unEqual)
		return FALSE;

	return TRUE;
}

void CNtlPLObject::ResetUV()
{
	for(RwUInt32 j = 0; j < m_vecAtomicList.size(); ++j)
	{
		RpGeometry* geom = m_vecAtomicList[j]->pData->geometry;

		RpGeometryLock(geom, rpGEOMETRYLOCKTEXCOORDS);
		RwTexCoords* texCoords = RpGeometryGetVertexTexCoords(geom, (RwTextureCoordinateIndex)2);

		for (RwUInt32 k = 0;k < (RwUInt32) RpGeometryGetNumVertices(geom);k++)
		{
			LtMapIntUV_  intUVs;

			/* NOTE: we use a union as when calculating the UVs
			* (see comment there for why) */
			intUVs.uv = texCoords[k];

			intUVs.uv.u = 0.1f * (intUVs.intUV[0] & 0x7);
			intUVs.uv.v = 0.1f * (intUVs.intUV[1] & 0x7);
			texCoords[k] = intUVs.uv;
		}

		RpGeometryUnlock(geom);
	}
}

RwBool CNtlPLObject::Update( RwReal fElapsed )
{
	NTL_SPROFILE("CNtlPLObject::Update");

	if (m_pObjectType)
	{
		m_pObjectType->Update(fElapsed);
	}

	// 트리거 애니메이션이 세팅되면, 생성될때까지 Time은 진행시킨다.
    if(!IsSchedulingLoadingComplete())
    {
        if(m_sScheduleInfo.uiAnimKey > 0)
        {
            m_sScheduleInfo.fAnimStartTime += fElapsed;
        }

        NTL_RPROFILE(TRUE);
    }

	if(!m_bActiveUpdate)
		NTL_RPROFILE(TRUE);

	if(m_pUVAnim)
	{
		m_pUVAnim->Update(fElapsed);
	}

#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OBJECT_ANIMATION))
#endif
	{
		if(m_bAnimPlay && m_pBaseHierarchy)
		{
			if(m_pAnimLayer)
			{
				m_pAnimLayer->Update(fElapsed * m_fAnimSpeed);
				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
			}
			else
			{
				RpHAnimHierarchyAddAnimTime(m_pBaseHierarchy, fElapsed * m_fAnimSpeed);        
				RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
			}
		}
	}

	if(m_eFadeState != FADE_NOT_VISIBLE && m_eFadeState != FADE_VISIBLE)
	{
		UpdateFading(fElapsed);
	}

	CNtlPLAttach::Update(fElapsed);	

	NTL_RPROFILE(TRUE);
}


RwBool CNtlPLObject::SetThreadSafeProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLObject::SetThreadSafeProperty");

	m_pProperty = static_cast<CNtlPLObjectProperty*>(const_cast<CNtlPLProperty*>(pData));

	if(m_pProperty->m_strAnimFileName.size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
		{
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
		}
	}

	if(m_pProperty->m_strUVAnimFileName.size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
	}

	if(m_pProperty->m_vLinkEffect.size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
	}

	//  트리거 애니메이션이 있으면 Update Flag를 켠다
	if(m_pProperty->GetAnimTable()->GetTypeAnimMap()->size() > 0)
	{
		m_bHaveAnim = TRUE;
		if(GetFlags() & NTL_PLEFLAG_NOTUPDATE)
			SetFlags(GetFlags() & ~NTL_PLEFLAG_NOTUPDATE);
	}

	m_bbox = GetTriggerAABBInfo();

	// Anim Table 
	m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
	m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());

	NTL_RETURN(TRUE);
}

RwBool CNtlPLObject::SetProperty(const CNtlPLProperty *pData)
{
	return SetThreadSafeProperty(pData);
}


void CNtlPLObject::AddWorld(void)
{
	if( CNtlPLGlobal::m_pRpWorld == NULL )
		return;

	RpClump *pClump = GetClump();
	if(pClump != NULL)
	{
		if(RpClumpGetWorld(pClump) == NULL)
			RpWorldAddClump( CNtlPLGlobal::m_pRpWorld, pClump );
	}
}

void CNtlPLObject::RemoveWorld(void)
{
	if( CNtlPLGlobal::m_pRpWorld == NULL )
		return;

	RpClump *pClump = GetClump();
	if( pClump != NULL )
	{
		if( RpClumpGetWorld(pClump) != NULL)
			RpWorldRemoveClump( CNtlPLGlobal::m_pRpWorld,pClump );
	}
}

void CNtlPLObject::CallPreSchedulingResource(void)
{
	// 설정된 UVAnim이 있으면 적용한다.(Clump가 Load되기전에 Load 되어야 한다)
	if(m_pProperty->m_strUVAnimFileName.size())
		SetUVAnim(m_pProperty->m_strUVAnimFileName.c_str());
}

void CNtlPLObject::CallSchedulingResource(CNtlPLResource *pResource)
{
	NTL_FUNCTION("CNtlPLObject::CallSchedulingResource");

	m_pClumpResource = pResource;
	if(m_pClumpResource == NULL)
		NTL_RETURNVOID();

	AddWorld();

	CreateThreadSafe();

	SetTransform();
	SetAlpha(m_sColor.alpha);

	// 로딩이 완료되었음을 이벤트로 쏜다.
	CNtlPLEventGenerator::CreateEventThreadLoading(GetSerialID());

	NTL_RETURNVOID();
}

void CNtlPLObject::CallSchedulingResourceOnly()
{
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		SetAtomicPSMap();
	}
}

void CNtlPLObject::SetPosition(const RwV3d *pPos)
{
	m_vWorldPosition = *pPos;
	if(m_pClumpResource == NULL)
		return;

	RwFrame *pFrame = RpClumpGetFrame( GetClump() );  

	RwMatrix *pMat = RwFrameGetMatrix( pFrame ); 
	RwV3d *pOriPos = RwMatrixGetPos ( pMat );
	RwV3dAssign(pOriPos, pPos); 
	RwFrameTranslate(pFrame, &CNtlPLGlobal::m_vZeroV3, rwCOMBINEPOSTCONCAT); 

	// BBox Update
	m_bbox = GetTriggerAABBInfo();

	UpdateLoopSoundPos();

	CalcBoundingSphere();

	UpdateRpSectorOBBCheck();
}	

void CNtlPLObject::SetRotate( RwReal fXAngle, RwReal fYAngle, RwReal fZAngle)
{
	m_vModelAngle.x = fXAngle;
	m_vModelAngle.y = fYAngle;
	m_vModelAngle.z = fZAngle;

	if(m_pClumpResource == NULL)
		return;

	SetTransform();

	// BBox Update
	m_bbox = GetTriggerAABBInfo();

	UpdateRpSectorOBBCheck();
}

RwV3d CNtlPLObject::GetPosition(void)
{
	if(m_pClumpResource == NULL)
		return m_vWorldPosition;

	RwFrame *pFrame = RpClumpGetFrame( GetClump() ); 
	return *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
}


void CNtlPLObject::SetScale( const RwV3d * pScale )
{
	memcpy( &m_vModelScale, pScale, sizeof( RwV3d ) );

	if(m_pClumpResource == NULL)
		return;

	SetTransform();

	// BBox Update
	m_bbox = GetTriggerAABBInfo();

	CalcBoundingSphere();

	UpdateRpSectorOBBCheck();
}


const RwV3d * CNtlPLObject::GetScale( void ) const
{
	return &m_vModelScale;
}

const RwV3d* CNtlPLObject::GetRotate(void) const 
{
	return &m_vModelAngle;
}

RwBool	CNtlPLObject::GetEnableShadow()
{
	return (GetFlags() & NTL_PLEFLAG_SHADOW);
}

VOID CNtlPLObject::DeleteDoodadShadow()
{
	if(m_vecAtomicList.size() == 0)
	{
		return;
	}

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			CNtlWorldShadow* pNtlWorldShadow = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(pAtomic));
			NTL_DELETE(pNtlWorldShadow);
			RpNtlAtomicSetUserDat(pAtomic, NULL);
		}
	}
}

RwBool CNtlPLObject::AreThereObjPSMap()
{
	if(m_vecAtomicList.size() == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(pAtomic));
			if(pUserDat && pUserDat->m_pTexShadow)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

RwBool CNtlPLObject::GetEnableGenShadowMap()
{
	if(m_vecAtomicList.size() == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			if( RpNtlAtomicGetFlag(pAtomic) & NTL_SHADOW_MAP )
				return TRUE;
		}
	}

	return FALSE;
}

RwBool CNtlPLObject::GetEmblemMarkEnabled()
{
	if(m_vecAtomicList.size() == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		if(pAtomic != NULL)
		{
			if((RpNtlAtomicGetFlag(pAtomic) & NTL_EMBLEM_MARK) && RpNtlAtomicGetUserDat4RwTex(pAtomic))
				return TRUE;
		}
	}

	return FALSE;
}

ENTITY_ATOMIC_VEC *CNtlPLObject::GetAtomicList()
{
	return &m_vecAtomicList;
}

VOID CNtlPLObject::SavePSMap(FILE* _pFile)
{
	RwInt32 PSAtomicCnt = 0;
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(m_vecAtomicList[i]->pData));
		if(pUserDat && pUserDat->m_pTexShadow)
		{
			++PSAtomicCnt;
		}
	}

	fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		CNtlWorldShadow* pUserDat = static_cast<CNtlWorldShadow*>(RpNtlAtomicGetUserDat(m_vecAtomicList[i]->pData));
		if(pUserDat && pUserDat->m_pTexShadow)
		{
			// atomic name to match for loading
			fwrite(pUserDat->m_pTexShadow->name, rwTEXTUREBASENAMELENGTH, 1, _pFile);

			// frustum view window
			fwrite(&pUserDat->m_CamViewWindowX, sizeof(RwReal), 1, _pFile);

			// light matrices
			fwrite(&pUserDat->m_matLight, sizeof(RwMatrix), 1, _pFile);

			// size
			RwInt32 Size = RwRasterGetWidth(pUserDat->m_pTexShadow->raster);
			fwrite(&Size, sizeof(RwInt32), 1, _pFile);

			// texture data
			CNtlPLResourceManager::GetInstance()->SaveTexture(pUserDat->m_pTexShadow, _pFile);
		}
	}
}

VOID CNtlPLObject::LoadPSMap(FILE* _pFile)
{
	RwInt32 PSAtomicCnt = 0;
	fread(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);

	for(RwInt32 i = 0; i < PSAtomicCnt; ++i)
	{
		CNtlWorldShadow* pLoadNWS = NTL_NEW CNtlWorldShadow;

		// name
		RwChar ReadBuff[rwTEXTUREBASENAMELENGTH] = {0,};
		fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);

		// frustum view window
		fread(&pLoadNWS->m_CamViewWindowX, sizeof(RwReal), 1, _pFile);

		// light matrices
		fread(&pLoadNWS->m_matLight, sizeof(RwMatrix), 1, _pFile);

		RwInt32 Size = 0;
		fread(&Size, sizeof(RwInt32), 1, _pFile);
		pLoadNWS->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture(ReadBuff, Size, Size, 32, rwRASTERFORMAT8888);
		RwTextureSetFilterMode(pLoadNWS->m_pTexShadow, rwFILTERLINEAR);
		RwTextureSetAddressing(pLoadNWS->m_pTexShadow, rwTEXTUREADDRESSCLAMP);
		CNtlPLResourceManager::GetInstance()->LoadTexture(pLoadNWS->m_pTexShadow, _pFile);
		DBO_ASSERT(pLoadNWS->m_pTexShadow, "Texture load failed.");
		m_vecNtlWorldShadow.push_back(pLoadNWS);
	}
}

RwBool CNtlPLObject::SetAtomicEmblemMap(RwTexture* _pEmblemMap)
{
	DBO_ASSERT(_pEmblemMap, "CNtlPLObject::SetAtomicEmblemMap(RwTexture* _pEmblemMap), _pEmblemMap is NULL");

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
	{
		if(RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData) & NTL_EMBLEM_MARK)
		{
			RwTexture* pEmblemMarkTex = RpNtlAtomicGetUserDat4RwTex(m_vecAtomicList[i]->pData);
			if(pEmblemMarkTex)
				GetEmblemMaker()->DestoyEmblem(pEmblemMarkTex);

			RpNtlAtomicSetUserDat4RwTex(m_vecAtomicList[i]->pData, _pEmblemMap);
		}
	}

	return TRUE;
}

void CNtlPLObject::DestroyEmblemMap()
{
	if(GetEmblemMarkEnabled())
	{
		ENTITY_ATOMIC_VEC*		_pEAV	= GetAtomicList();
		ENTITY_ATOMIC_VEC_ITER	_The	= _pEAV->begin();

		while(_The != _pEAV->end())
		{
			RwTexture* pUserDat = static_cast<RwTexture*>(RpNtlAtomicGetUserDat(static_cast<RpAtomic*>((*_The)->pData)));
			if(pUserDat)
			{
				GetEmblemMaker()->DestoyEmblem(pUserDat);
				RpNtlAtomicSetUserDat(static_cast<RpAtomic*>((*_The)->pData), NULL);
			}

			_The++;
		}
	}
}

VOID CNtlPLObject::SetAtomicPSMap()
{
	for(RwUInt32 k = 0; k < m_vecNtlWorldShadow.size(); ++k)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); ++i)
		{
			if(!strcmp(Helper_AtomicName(m_vecAtomicList[i]->pData), m_vecNtlWorldShadow[k]->m_pTexShadow->name))
			{
				RpNtlAtomicSetUserDat(m_vecAtomicList[i]->pData, m_vecNtlWorldShadow[k]);
				break;
			}
		}
	}

	m_vecNtlWorldShadow.clear();
}

void CNtlPLObject::SetTransform( void )
{
	NTL_FUNCTION("CNtlPLObject::SetTransform");

	RwFrame *		pFrame = RpClumpGetFrame( GetClump() ); 
	RwMatrix *		pMatrix = RwFrameGetMatrix( pFrame );

	RwMatrix		Matrix;
	RwV3d			Front, Left, Up;

	RwMatrixRotate( &Matrix, &CNtlPLGlobal::m_vYAxisV3, m_vModelAngle.y, rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &CNtlPLGlobal::m_vZAxisV3, &Matrix );
	RwV3dTransformVector( &Left, &CNtlPLGlobal::m_vXAxisV3, &Matrix );

	RwMatrixRotate( &Matrix, &Left, m_vModelAngle.x, rwCOMBINEREPLACE );

	RwV3dTransformVector( &Front, &Front, &Matrix );
	RwV3dTransformVector( &Up, &CNtlPLGlobal::m_vYAxisV3, &Matrix );

	RwMatrixScale( pMatrix, &m_vModelScale, rwCOMBINEREPLACE );
	RwMatrixRotate( pMatrix, &CNtlPLGlobal::m_vYAxisV3, m_vModelAngle.y, rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Left, m_vModelAngle.x, rwCOMBINEPOSTCONCAT );
	RwMatrixRotate( pMatrix, &Front, m_vModelAngle.z, rwCOMBINEPOSTCONCAT );
	RwMatrixTranslate( pMatrix, &m_vWorldPosition, rwCOMBINEPOSTCONCAT );

	RwFrameUpdateObjects( pFrame );

	NTL_RETURNVOID();
}

RwFrame * CNtlPLObject::GetFrame( void )
{
	if( GetClump() == NULL )
	{
		return NULL;
	}

	return RpClumpGetFrame( GetClump() );
}

void CNtlPLObject::SetMatrix( RwMatrix & matWorld )
{
	RwFrame *		pFrame = GetFrame();

	if( pFrame == NULL )
	{
		return;
	}

	if( RwFrameGetMatrix( pFrame ) == NULL )
	{
		return;
	}

	RwMatrixCopy( RwFrameGetMatrix( pFrame ), &matWorld );

	// 위치값을 저장한다.
	m_vWorldPosition = *RwMatrixGetPos(&matWorld);

	RwFrameUpdateObjects( pFrame );

	UpdateLoopSoundPos();

	return;
}


RwMatrix & CNtlPLObject::GetMatrix( void )
{
	RwFrame *		pFrame = GetFrame();

	if( pFrame == NULL )
	{
		return CNtlPLEntity::GetMatrix();
	}

	return *RwFrameGetMatrix( pFrame );
}


RwBool CNtlPLObject::SetUVAnim( const RwChar* szUVAnimFileName ) 
{
	if(!szUVAnimFileName)
		return FALSE;

	if(!m_pUVAnim)
	{
		m_pUVAnim = NTL_NEW CNtlPLUVAnim();
	}

	// NOTE: UVAnim은 Clump가 Load 되기전에 호출되어야만 적용된다.
	return m_pUVAnim->Create(szUVAnimFileName);    
}

/**
* 애니메이션 리소스및 객체를 생성하고, 오브젝트에 적용한다.
* \param szAnimFileName 적용할 AnimFileName (*.anm)
* return 성공 유무
*/
RwBool CNtlPLObject::SetAnimation( const RwChar* szAnimFileName ) 
{
	NTL_FUNCTION(__FUNCTION__);

	if(!GetClump())
		NTL_RETURN(FALSE);

	if(m_pAnimResource == NULL)
	{
		if(!CreateAnim())
			NTL_RETURN(FALSE);
	}
	else
	{
		// 로딩된 Anim을 메모리에서 제거한다.
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pAnimResource);
		m_pAnimResource = NULL;
	}

	if(szAnimFileName == NULL)
	{
		RpHAnimHierarchyDetach(m_pBaseHierarchy);
		m_bAnimPlay = FALSE;
	}
	else
	{
		m_pAnimResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(szAnimFileName);
		if(!m_pAnimResource)
			return FALSE;

		RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, m_pAnimResource->GetAnimation());
		m_bAnimPlay = TRUE;

		RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLObject::SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime /* = 0.0f */, RwBool bLoop /* = TRUE */)
{
    if(!IsSchedulingLoadingComplete())
    {
        m_sScheduleInfo.uiAnimKey = uiAnimKey;
        m_sScheduleInfo.fAnimStartTime = fStartTime;
        m_sScheduleInfo.bAnimLoop = bLoop;
        return FALSE;
    }

	// AnimTable이 아직 생성되어 있지 않다면 생성해준다. 
	if(!m_pInstanceAnimTable)
	{
		m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
		m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());
	}

	SInstanceAnimData* pInstanceAnimData = m_pInstanceAnimTable->Get(uiAnimKey);
	if(!pInstanceAnimData)
		NTL_RETURN(FALSE);

	// Instance Anim Table 생성
	if(!m_pAnimLayer)
	{
		CreateAnim();

		// AnimLayer 객체를 만들기 위해서는 우선 애니메이션이 적용되어 있어야 한다.
		RpHAnimHierarchySetCurrentAnim(m_pBaseHierarchy, pInstanceAnimData->pResource->GetAnimation());
		RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);

		m_pAnimLayer = NTL_NEW CNtlAnimLayer();
		if(!m_pAnimLayer->Create(m_pBaseHierarchy))
			NTL_RETURN(FALSE);

		m_pAnimLayer->SetCallBack(this, &CNtlPLObject::CallBackBaseAnim);
	}

	m_bAnimPlay = TRUE;

	ClearLoopEffect();
	ClearLoopSound();

	m_uiCurAnimKey = uiAnimKey;

	// Play time을 0 ~ 1 사이 값으로 조정한다
	RwReal fPlayTime = fStartTime;
	fPlayTime = (FLT_MAX == fPlayTime ? 0.f : fPlayTime);
	fPlayTime = fabs(fPlayTime);

	if(fPlayTime > 0.0f)
	{
		fPlayTime = fPlayTime / m_pAnimLayer->GetDurationAnimTime();
		fPlayTime -= floor(fPlayTime);
	}

	return m_pAnimLayer->SetAnimation(pInstanceAnimData, fPlayTime, bLoop, FALSE);
}

RwBool CNtlPLObject::CreateAnim() 
{
	m_pBaseHierarchy = Helper_GetHierarchyClump(GetClump());
	if(!m_pBaseHierarchy)
		return FALSE;

	Helper_SetHierarchyClumpSkin(GetClump(), m_pBaseHierarchy);

	RpHAnimHierarchySetFlags( m_pBaseHierarchy,
		(RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pBaseHierarchy) | 
		rpHANIMHIERARCHYUPDATELTMS | 
		rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));

	RpHAnimHierarchyAttach(m_pBaseHierarchy);		

	return TRUE;
}

RwSphere* CNtlPLObject::GetBoundingSphere()
{
	m_BSphereCur.center = GetPosition() + m_BSphere.center;
	m_BSphereCur.radius	= m_BSphere.radius;

	return &m_BSphereCur;
}

void CNtlPLObject::CalcBoundingSphere()
{
	m_BSphere.center = GetPosition();
	m_BSphere.radius = 0.0f;

	for (RwInt32 iAtomicCnt = 0; iAtomicCnt < (RwInt32)m_vecAtomicList.size(); ++iAtomicCnt)
	{
		RpAtomic*	pAtomic		= m_vecAtomicList.at(iAtomicCnt)->pData;
		RwSphere*	pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pAtomic));

		m_BSphere.center = (m_BSphere.center + pCurSphere->center) * 0.5f;
		m_BSphere.radius = RwV3dLength(&((m_BSphere.center - pCurSphere->center) * 0.5f)) + (m_BSphere.radius < pCurSphere->radius ? pCurSphere->radius : m_BSphere.radius);
	}

	m_BSphere.center = m_BSphere.center - GetPosition();
}

// GetPosition으로 업데이트가 힘든 문제점으로 변경함.
// 문제가 될 경우 다시 복원한다.
// RwSphere* CNtlPLObject::GetBoundingSphere()
// {
// 	return &m_BSphere;
// }
// 
// void CNtlPLObject::CalcBoundingSphere()
// {
// 	if (GetClump()
// 	if(m_pClumpResource == NULL)
// 		return;
// 
// 	RwSphere*	pCurSphere;
// 	RpClump*	pClump			= GetClump();
// 	RwLLLink*	pCur;
// 	RwLLLink*	pEnd;
// 	RwLLLink*	pNext;
// 	RpAtomic*	pCurAtomic;
// 	RwInt32		NumSphere = 0;
// 	RpAtomic*	pAtomicOfMaxRadius;
// 	RwReal		LenMaxRadius;
// 
// 	m_BSphere.center.x	= 0.0f;
// 	m_BSphere.center.y	= 0.0f;
// 	m_BSphere.center.z	= 0.0f;
// 	m_BSphere.radius	= 0.0f;
// 
// 	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
// 	pEnd = rwLinkListGetTerminator(&pClump->atomicList);
// 
// 	while(pCur != pEnd)
// 	{
// 		pCurAtomic	= rwLLLinkGetData(pCur, RpAtomic, inClumpLink);
// 		pCurSphere	= const_cast<RwSphere*>(RpAtomicGetWorldBoundingSphere(pCurAtomic));
// 
// 		if(!NumSphere)
// 		{
// 			LenMaxRadius		= pCurSphere->radius;
// 			pAtomicOfMaxRadius	= pCurAtomic;
// 
// 			CNtlMath::MathRwV3dAssign(&m_BSphere.center, pCurSphere->center.x, pCurSphere->center.y, pCurSphere->center.z);
// 		}
// 		else
// 		{
// 			if(LenMaxRadius < pCurSphere->radius)
// 			{
// 				LenMaxRadius		= pCurSphere->radius;
// 				pAtomicOfMaxRadius	= pCurAtomic;
// 			}
// 
// 			m_BSphere.center.x += pCurSphere->center.x;
// 			m_BSphere.center.y += pCurSphere->center.y;
// 			m_BSphere.center.z += pCurSphere->center.z;
// 		}
// 
// 		pNext	= rwLLLinkGetNext(pCur);
// 		pCur	= pNext;
// 
// 		NumSphere++;
// 	}
// 
// 	m_BSphere.center.x /= NumSphere;
// 	m_BSphere.center.y /= NumSphere;
// 	m_BSphere.center.z /= NumSphere;
// 	m_BSphere.radius	= LenMaxRadius + sqrtf((m_BSphere.center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) * (m_BSphere.center.x - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.x) + (m_BSphere.center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) * (m_BSphere.center.y - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.y) + (m_BSphere.center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z) * (m_BSphere.center.z - RpAtomicGetWorldBoundingSphere(pAtomicOfMaxRadius)->center.z));
// }

RwBool CNtlPLObject::RenderToTexture()
{
	if(m_pClumpResource == NULL)
		return TRUE;

	RpClump*	pClump = GetClump();
	RwLLLink*	pCur;
	RwLLLink*	pEnd;
	RwLLLink*	pNext;
	RpAtomic*	pCurAtomicInClump;

	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pClump->atomicList);

	BeginFogState(FALSE);

	while(pCur != pEnd)
	{	
		pCurAtomicInClump = rwLLLinkGetData(pCur, RpAtomic, inClumpLink);

		RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pCurAtomicInClump);
		if((uiRenderFlags & NTL_ALPHATEST) || (uiRenderFlags & NTL_ALPHA))
		{
			if(uiRenderFlags & NTL_ALPHATEST)
				BeginNtlAotmicAlphaTestStateShadow();
			if(uiRenderFlags & NTL_TWOSIDE)
				BeginNtlAtomicTwoSideState();

			pCurAtomicInClump->renderCallBack(pCurAtomicInClump);

			if(uiRenderFlags & NTL_TWOSIDE)
				EndNtlAtomicTwoSideState();
			if(uiRenderFlags & NTL_ALPHATEST)
				EndNtlAtomicAlphaTestState();
		}
		else
		{
			pCurAtomicInClump->renderCallBack(pCurAtomicInClump);
		}

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	CNtlPLAttach::RenderToTexture();

	EndFogState();

	return TRUE;
}

RwBool CNtlPLObject::Render()
{
	NTL_SPROFILE("CNtlPLObject::Render");

	if(m_pClumpResource == NULL)
		NTL_RPROFILE(TRUE);

	if(!GetEnableShadow() || !this->IsVisible())
		NTL_RPROFILE(FALSE);

	RpClump*					pClump = GetClump();
	RwLLLink*					pCur;
	RwLLLink*					pEnd;
	RwLLLink*					pNext;
	RpAtomic*					pCurAtomicInClump;

	pCur = rwLinkListGetFirstLLLink(&pClump->atomicList);
	pEnd = rwLinkListGetTerminator(&pClump->atomicList);

	while(pCur != pEnd)
	{	
		pCurAtomicInClump = rwLLLinkGetData(pCur, RpAtomic, inClumpLink);

		RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag(pCurAtomicInClump);

		if(!(uiRenderFlags & NTL_NOT_VISIBLE))
		{
			if((uiRenderFlags & NTL_ALPHATEST) || (uiRenderFlags & NTL_ALPHA))
			{
				if(uiRenderFlags & NTL_ALPHATEST)
					BeginNtlAotmicAlphaTestStateShadow();
				if(uiRenderFlags & NTL_TWOSIDE)
					BeginNtlAtomicTwoSideState();

				pCurAtomicInClump->renderCallBack(pCurAtomicInClump);

				if(uiRenderFlags & NTL_TWOSIDE)
					EndNtlAtomicTwoSideState();
				if(uiRenderFlags & NTL_ALPHATEST)
					EndNtlAtomicAlphaTestState();
			}
			else
			{
				pCurAtomicInClump->renderCallBack(pCurAtomicInClump);
			}
		}

		pNext	= rwLLLinkGetNext(pCur);
		pCur	= pNext;
	}

	NTL_RPROFILE(TRUE);
}

CNtlInstanceEffect* CNtlPLObject::AttachLinkEffect(SEventLinkEffect* pEventLinkEffect)
{
	if(!pEventLinkEffect)
		return FALSE;

	return m_LinkEffectInstance.AttachLinkEffect(this, pEventLinkEffect);
}

RwBool CNtlPLObject::DetachLinkEffect(CNtlInstanceEffect* pLinkEffect)
{
	if(!pLinkEffect)
		return FALSE;

	return m_LinkEffectInstance.DetachLinkEffect(this, pLinkEffect);


}

void CNtlPLObject::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sColor.red	= byRed;
	m_sColor.green	= byGreen;
	m_sColor.blue	= byBlue;
}

void CNtlPLObject::SetAddColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_sAddColor.red		= byRed;
	m_sAddColor.green	= byGreen;
	m_sAddColor.blue	= byBlue;
}

RwBool CNtlPLObject::SetPlayAnimTime(RwReal fStartAnimTime)
{
	if(m_pClumpResource == NULL)
		return TRUE;

	if(!m_pBaseHierarchy)
		return FALSE;

	if(!m_pBaseHierarchy->currentAnim)
		return FALSE;

	if(fStartAnimTime < 0.f || fStartAnimTime > 1.f)
		return FALSE;

	RwReal fDurationTime = m_pBaseHierarchy->currentAnim->pCurrentAnim->duration;
	RpHAnimHierarchySetCurrentAnimTime(m_pBaseHierarchy, fDurationTime * fStartAnimTime);

	return TRUE;
}

RwBool CNtlPLObject::SetPlayAnimSpeed(RwReal fAnimSpeed)
{
	if(fAnimSpeed < 0.f)
		return FALSE;
	m_fAnimSpeed = fAnimSpeed;

	return TRUE;
}


RwBBox CNtlPLObject::GetTriggerAABBInfo() 
{
	return GetTriggerAABBInfo(GetPosition(), *GetRotate(), *GetScale()); 
}

RwBBox CNtlPLObject::GetDefaultAABB() 
{
	RwBBox bbox;
	if (m_pProperty->GetBBox())
	{
		bbox = *m_pProperty->GetBBox();
	}
	else
	{
		bbox = CreateDefaultAABB();
	}
	return bbox;
}

RwBBox CNtlPLObject::GetTriggerAABBInfo( const RwV3d& vPos, const RwV3d& vRotate, const RwV3d& vScale ) 
{
	RwBBox bboxOut;
	ZeroMemory(&bboxOut, sizeof(RwBBox));

	const RwBBox* pBBox = m_pProperty->GetBBox();
	if(!pBBox)
		return bboxOut;

	// 프로퍼티에 설정된 바운딩 박스가 없으면 계산해서 설정한다.
	if(RwV3dLength(&(pBBox->sup)) == 0)
	{
		m_pProperty->SetBBox(CreateDefaultAABB());        
		pBBox = m_pProperty->GetBBox();
		//return bboxOut;
	}

	RwV3d vertsIn[8], vertsOut[8];

	vertsIn[0] = pBBox->inf;
	vertsIn[1] = pBBox->inf;
	vertsIn[1].z = pBBox->sup.z;
	vertsIn[2] = vertsIn[1];
	vertsIn[2].x = pBBox->sup.x;
	vertsIn[3] = pBBox->inf;
	vertsIn[3].x = pBBox->sup .x;

	for(int i = 4; i < 8; ++i)
	{
		vertsIn[i] = vertsIn[i - 4];
		vertsIn[i].y = pBBox->sup.y;
	}

	RwMatrix matLtm;    
	RwMatrixSetIdentity(&matLtm);
	RwMatrixScale(&matLtm, &vScale, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &XAxis, vRotate.x, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &YAxis, vRotate.y, rwCOMBINEPOSTCONCAT);
	RwMatrixRotate(&matLtm, &ZAxis, vRotate.z, rwCOMBINEPOSTCONCAT);
	RwMatrixTranslate(&matLtm, &vPos, rwCOMBINEPOSTCONCAT);

	RwV3dTransformPoints(vertsOut, vertsIn, 8, &matLtm);

	RwBBoxCalculate(&bboxOut, vertsOut, 8);

	return bboxOut;
}

static RpAtomic* GetAABB(RpAtomic* pAtomic, void* data)
{
	RwBBox* pBBox = (RwBBox*)data;

	RpGeometry* pGeom = RpAtomicGetGeometry(pAtomic);
	RwInt32 numVerts = RpGeometryGetNumVertices(pGeom);

	RwV3d* vertIn = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(pGeom, 0));

	for(int i = 0; i < numVerts; ++i)
	{
		RwBBoxAddPoint(pBBox, &vertIn[i]);
	}

	return pAtomic;
}

RwBBox CNtlPLObject::CreateDefaultAABB() 
{
	RwBBox bBox;

	if(GetClump() == NULL)
	{
		memset(&bBox, 0, sizeof(RwBBox));
		return bBox;
	}

	bBox.sup.x = bBox.sup.y = bBox.sup.z = RwRealMINVAL;
	bBox.inf.x = bBox.inf.y = bBox.inf.z = RwRealMAXVAL;

	RpClumpForAllAtomics(GetClump(), GetAABB, (void*)&bBox);

	return bBox;
}

RwBool CNtlPLObject::UpdateFadeSystem() 
{
	if(!CNtlPLGlobal::m_RwCamera)
		return FALSE;

	if(!(GetFlags() & NTL_PLEFLAG_FADE))
	{
		return FALSE;
	}

	if(m_eFadeState == FADE_NOT_VISIBLE || m_eFadeState == FADE_VISIBLE)
	{
		// 카메라와의 거리를 체크한다. (높이는 체크하지 않는다)
		RwFrame* pFrame = RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
		RwV3d vPosCamera = *RwMatrixGetPos(RwFrameGetMatrix(pFrame));
		RwV3d vPosObject = GetPosition();
		vPosCamera.y = vPosObject.y = 0.0f;
		RwV3d vDistCamera = vPosCamera - vPosObject;
		RwReal fDistCamera = RwV3dLength(&vDistCamera);

		// Fade Out         
		if( m_eFadeState == FADE_VISIBLE && fDistCamera >= GetVisibleCullingDistance())
		{
			m_eFadeState = FADE_OUT_OBJECT;            // 사라질때는 object부터 사라진다 (이펙트는 남아있다)
			m_fFadeTime = 0.0f;

			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
				RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
			}

			AddSceneUpdate();

// 	 		if(!m_bAnimPlay)            // 애니메이션이 적용안된 오브젝트는 update가 호출되지 않기때문에 Update를 호출할수있도록 추가한다.
//   			{
//   				GetSceneManager()->AddUpdate(this);
// 	  		}
		}
		// Fade In
		else if(m_eFadeState == FADE_NOT_VISIBLE && fDistCamera <= GetVisibleCullingDistance())
		{
			//m_eFadeState = FADE_IN_EFFECT;             // 나타날때는 Effect부터 나타난다 (오브젝트는 차후에 나타난다)            
			/// -- Fade In 될때는 오브젝트와 이펙트가 같이 나타난다 (2007.7.31 by agebreak - 민석씨 요구사항)
			m_eFadeState = FADE_IN_OBJECT;
			m_fFadeTime = 0.0f;

			SetEffectVisible(TRUE);     			            

			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
				RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
			}

			AddSceneUpdate();

// 			if(!m_bAnimPlay)
// 			{
// 				GetSceneManager()->AddUpdate(this);
// 			}
		}

		m_fPrevCameraDistance = fDistCamera;
	}
	return TRUE;
}

RwBool CNtlPLObject::UpdateFading(RwReal fElapsedTime)
{
	NTL_SPROFILE("CNtlPLObject::UpdateFading");

	m_fFadeTime += fElapsedTime;

	switch(m_eFadeState)
	{
	case FADE_IN_EFFECT:
		if(m_fFadeTime >= FADE_EFFECT_GAP_TIME)
		{            
			m_eFadeState = FADE_IN_OBJECT;
			m_fFadeTime = 0.0f;
		}
		break;
	case FADE_IN_OBJECT:
		if(m_fFadeTime >= CNtlPLGlobal::m_fDistFiterObjFrequency)
		{
			m_eFadeState = FADE_VISIBLE;
			m_fFadeTime = 0.0f;            
			m_pFadeBlend->SetWeight(1.0f);            

			for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
			{
				RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
				RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag &(~NTL_RUNTIME_ALPHA));
			}

			RemoveSceneUpdate();

// 			if(!m_bAnimPlay)
// 			{
// 				GetSceneManager()->RemoveUpdate(this);
// 			}
		}
		else
		{
			// Alpha를 0%->100%로 변화시킨다. 
			m_pFadeBlend->SetWeight(m_fFadeTime / CNtlPLGlobal::m_fDistFiterObjFrequency);            
		}
		break;
	case FADE_OUT_EFFECT:
		if(m_fFadeTime >= FADE_EFFECT_GAP_TIME)
		{
			SetEffectVisible(FALSE);
			m_eFadeState = FADE_NOT_VISIBLE;
			m_fFadeTime = 0.0f;

			RemoveSceneUpdate();

// 			if(!m_bAnimPlay)
// 			{
// 				GetSceneManager()->RemoveUpdate(this);
// 			}
		}            
		break;
	case FADE_OUT_OBJECT:
		if(m_fFadeTime >= CNtlPLGlobal::m_fDistFiterObjFrequency)
		{
			m_pFadeBlend->SetWeight(0.0f);            
			m_eFadeState = FADE_OUT_EFFECT;
			m_fFadeTime = 0.0f;
		}            
		else
		{
			// Alpha를 100%->0%로 변환시칸다.           
			m_pFadeBlend->SetWeight((CNtlPLGlobal::m_fDistFiterObjFrequency - m_fFadeTime) / CNtlPLGlobal::m_fDistFiterObjFrequency);            
		}   
		break;
	}

	NTL_RPROFILE(TRUE);
}

void CNtlPLObject::SetVisible( RwBool bVisible ) 
{
	CNtlPLAttach::SetVisible(bVisible);
	SetEffectVisible(bVisible);
}

void CNtlPLObject::SetEffectVisible( RwBool bVisible ) 
{
	m_LinkEffectInstance.SetVisible(bVisible);
	if(!bVisible)
	{
		ClearLoopEffect();
		ClearLoopSound();
	}
}

#define	RENDER_MAX_COLOR_TEMP	50
RwRGBA g_sColorTemp[RENDER_MAX_COLOR_TEMP];
RpAtomic *CNtlPLObject::RenderCallBack(RpAtomic *pAtomic)
{
	NTL_SPROFILE("CNtlPLObject::RenderCallBack");

	CNtlPLEntity *pEntity = (CNtlPLEntity*)RpNtlAtomicGetData(pAtomic);
	if(pEntity)
	{
		if(pEntity->GetClassType() == PLENTITY_OBJECT)
		{			
			CNtlPLObject *pObjEntity = (CNtlPLObject *)pEntity;

			RwRGBAReal	sColorRatioReal;
			RwRGBARealFromRwRGBA(&sColorRatioReal, pObjEntity->GetColor());

			RwRGBA	sColor;
			RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);

			bool	bSetMaterialColor = false;
			if(pGeom)
			{   
				RwUInt32 uFlags = RpGeometryGetFlags(pGeom); 
				if( !(uFlags & rpGEOMETRYMODULATEMATERIALCOLOR) )
				{
					bSetMaterialColor = true;
					uFlags |= rpGEOMETRYMODULATEMATERIALCOLOR;
					RpGeometrySetFlags(pGeom, uFlags);
				}

				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				if(nMatCount > RENDER_MAX_COLOR_TEMP)
				{
					NTL_ASSERT(nMatCount <= RENDER_MAX_COLOR_TEMP && nMatCount >= 0, "out of range.")
					nMatCount = RENDER_MAX_COLOR_TEMP;
				}

				// Atomic에 설정된 Alpha 값과의 보정                
				sColorRatioReal.alpha *= RpNtlAtomicGetAlpha(pAtomic) / 255.0f;

				// Fade를 위한 Weight Alpha값과의 보정
#ifdef dNTL_WORLD_TOOL_MODE	// 툴에서는 무조건 Fade Alpha 적용
				sColorRatioReal.alpha *= pEntity->GetWeightAlpha();
#else
				if(!(pEntity->GetFlags() & NTL_PLEFLAG_OBJECT_FORCE_VISIBLE)) // 플래그가 켜져있다면, Fade alpha를 적용하지 않는다.
				{
					sColorRatioReal.alpha *= pEntity->GetWeightAlpha();
				}
#endif                

				if(sColorRatioReal.alpha == 0.0f)       // 알파가 0.0이면 렌더링하지 않는다.
					NTL_RPROFILE(pAtomic);

				// Alpha Test ATomic의 Alpha 보간 보정( Alpha Test일 경우 208 이상인 경우 통과인 관계로)
				if( RpNtlAtomicGetFlag(pAtomic) & NTL_ALPHATEST &&
					RpNtlAtomicGetFlag(pAtomic) & NTL_RUNTIME_ALPHA)
				{
					sColorRatioReal.alpha	= sColorRatioReal.alpha * 0.18823f + 0.81176f;
				}

				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);

					// get through the proper renderpipe
					if(pObjEntity->GetToonData())
					{
						// Toon이 적용되어 있는 경우에만 Material Render CallBack을 따른다. 안되어있으면 안나온다. (by agebreak)
						RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);
					}

					const RwRGBA *pMatColor = RpMaterialGetColor(pMaterial);

					g_sColorTemp[i].red	  = pMatColor->red;
					g_sColorTemp[i].green = pMatColor->green;
					g_sColorTemp[i].blue  = pMatColor->green;
					g_sColorTemp[i].alpha = pMatColor->alpha;


					sColor.red		= RwUInt8(g_sColorTemp[i].red * sColorRatioReal.red);
					sColor.green	= RwUInt8(g_sColorTemp[i].green * sColorRatioReal.green);
					sColor.blue		= RwUInt8(g_sColorTemp[i].blue * sColorRatioReal.blue);
					sColor.alpha	= RwUInt8(g_sColorTemp[i].alpha * sColorRatioReal.alpha);                    

					RpMaterialSetColor(pMaterial, &sColor);
					RpNtlMaterialExtSetAddColor(pMaterial, pObjEntity->GetAddColor());
				}
			}

			// Toon 
			SToonData* pToonData = pObjEntity->GetToonData();
			if(pToonData)
			{
				Helper_SetToonAtomicRender(pAtomic, pToonData);
			}     

			if(RpNtlAtomicGetRenderCallBack(pAtomic) != NULL)
			{
#ifdef dNTL_WORLD_TOOL_MODE
				if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_WIRE_OBJECT))
				{
					API_PL_RenderWireMesh(pAtomic, NULL);
				}
				else
				{
#endif
					RpNtlAtomicGetRenderCallBack(pAtomic)(pAtomic);
#ifdef dNTL_WORLD_TOOL_MODE
				}
#endif

			}

			if(pGeom)
			{
				if(bSetMaterialColor)
				{
					RwUInt32 uiFlags = RpGeometryGetFlags(pGeom); 
					uiFlags = uiFlags & (~rpGEOMETRYMODULATEMATERIALCOLOR);
					RpGeometrySetFlags(pGeom, uiFlags);
				}

				RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

				for(int i = 0; i < nMatCount; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					RpMaterialSetColor(pMaterial, &g_sColorTemp[i]);
				}
			}                  
		}
		else
		{
			if(RpNtlAtomicGetRenderCallBack(pAtomic) != NULL)
			{
				RpNtlAtomicGetRenderCallBack(pAtomic)(pAtomic);
			}
		}
	}
	NTL_RPROFILE(pAtomic);
}


RpClump * CNtlPLObject::GetClump( void ) const
{
	if( m_pClumpResource != NULL ) 
		return (RpClump *)m_pClumpResource->GetData(); 
	else return NULL; 
}

/**
* Fade 효과 유뮤를 적용한다.
* \param bEnable Fade 효과 유무
* return 
*/
void CNtlPLObject::SetFadeEnable(RwBool bEnable)
{
	RwInt32 uiFlags = GetFlags();
	if(bEnable)
	{
		SetFlags(uiFlags | NTL_PLEFLAG_FADE);

		if(m_eFadeState == FADE_NOT_VISIBLE)
		{
			m_pFadeBlend->SetWeight(0.0f);
		}
		else if(m_eFadeState == FADE_VISIBLE)
		{
			m_pFadeBlend->SetWeight(1.0f);
		}
		else
		{
			AddSceneUpdate();
		}

// 		else if(!m_bAnimPlay)
// 		{
// 			GetSceneManager()->AddUpdate(this);
// 		}
	}
	else 
	{
		SetFlags(uiFlags & (~NTL_PLEFLAG_FADE));

		// Visual Manager의 Update List에 포함되어 있으면 제거한다
 		if(m_eFadeState != FADE_VISIBLE && m_eFadeState != FADE_NOT_VISIBLE && !m_bHaveAnim)
 		{
			RemoveSceneUpdate();
 			//GetSceneManager()->RemoveUpdate(this);
 		}

		// 오브젝트를 화면에 표시한다.
		m_eFadeState = FADE_VISIBLE;        
		m_pFadeBlend->SetWeight(1.0f);    
		SetWeightAlpha(1.0f);
	}
}

RwReal CNtlPLObject::GetBaseDurationAnimTime() 
{
	if(!m_pAnimLayer)
		return 0.0f;

	return m_pAnimLayer->GetDurationAnimTime();

}
RwReal CNtlPLObject::GetBaseCurrentAnimTime() 
{
	return m_pAnimLayer->GetCurrentAnimTime();
}
void CNtlPLObject::SetBaseCurrentAnimTime( RwReal fCurrTime ) 
{
	return m_pAnimLayer->SetCurrentAnimTime(fCurrTime);
}

int CNtlPLObject::CallBackBaseAnim(void* pEventData ) 
{
	SEventAnim* pEventAnim = (SEventAnim*)pEventData;

	switch(pEventAnim->eEventID)
	{
	case EVENT_ANIM_END:
		OnEventAnimEnd((SEventAnimEnd*)pEventData);
		break;
	case EVENT_ANIM_VISUAL_EFFECT:
		OnEventVisualEffect((SEventVisualEffect*)pEventData);
		break;
	case EVENT_ANIM_VISUAL_SOUND:
		OnEventVisualSound((SEventSound*)pEventData);
		break;
	case EVENT_ANIM_ALPHA:
		OnEventAlphaFade((SEventAlpha*)pEventData);
		break;
	case EVENT_ANIM_TMQ:
		OnEventTMQ((SEventAnimCinematic*)pEventData);
		break;
	case EVENT_ANIM_EXPLOSION:
		OnEventExplosion((SEventExplosion*)pEventData);
		break;
	}

	return TRUE;
}

void CNtlPLObject::OnEventAnimEnd( SEventAnimEnd* pEventAnimEnd ) 
{
	CNtlPLEventGenerator::AnimEnd(GetSerialID(), pEventAnimEnd->uiAnimKey, INVALID_GRAPHIC_ANIMATION_ID);			
}

void CNtlPLObject::OnEventVisualSound( SEventSound* pEventSound ) 
{
	// Sound를 Play한다.    

	if(strlen(pEventSound->chSoundName) <= 1)
		return ;

	// LoopSound이고 이미 기존에 Play되고 있다면 Play하지 않는다.
	if(pEventSound->bLoop && IsExistLoopSound(pEventSound->chSoundName))
		return ;

	// 파일이 여러개 세팅되어 있는경우에는 랜덤으로 플레이된다.
	std::string soundFileName;
	int nMax = 1;
	if(strlen(pEventSound->chSoundName4) > 0)
		nMax = 4;
	else if(strlen(pEventSound->chSoundName3) > 0)
		nMax = 3;
	else if(strlen(pEventSound->chSoundName2) > 0)
		nMax = 2;

	if(nMax > 1)
	{
		int nRand = NtlRandomNumber(1, nMax);
		switch(nRand)
		{
		case 1:
			soundFileName = pEventSound->chSoundName;
			break;
		case 2:            
			soundFileName = pEventSound->chSoundName2;
			break;
		case 3:            
			soundFileName = pEventSound->chSoundName3;
			break;
		case 4:
			soundFileName = pEventSound->chSoundName4;
			break;
		}
	}
	else
	{
		soundFileName = pEventSound->chSoundName;
	}

	// 피치를 랜덤으로 선택한다
	RwReal fSoundPitch = NtlRandomNumber(pEventSound->fSoundPitchMin, pEventSound->fSoundPitchMax);

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= pEventSound->eSoundType;
	tSoundParam.pcFileName		= pEventSound->chSoundName;
	tSoundParam.bLoop			= B2b(pEventSound->bLoop);
	tSoundParam.fVolume			= pEventSound->fSoundVolume * 0.01f;
	tSoundParam.fPitch			= fSoundPitch;
	tSoundParam.fXPos			= m_vWorldPosition.x;
	tSoundParam.fYPos			= m_vWorldPosition.y;
	tSoundParam.fZPos			= m_vWorldPosition.z;
	tSoundParam.fMinDistance	= pEventSound->fSoundDist;
	tSoundParam.fMaxDistance	= pEventSound->fSoundDecayDist;

	int iRet = GetSoundManager()->Play(&tSoundParam);

	// Loop Sound면 리스트에 추가한다.
	if(iRet == SOUNDRESULT_OK && pEventSound->bLoop && tSoundParam.hHandle != INVALID_SOUND_HANDLE)
	{
		AddLoopSound(tSoundParam.hHandle);
	}
}

void CNtlPLObject::OnEventVisualEffect( SEventVisualEffect* pEventVisualEffect ) 
{
	//Effect Name이 없는 경우는 무조건 Return을 한다.
	if( strlen(pEventVisualEffect->chEffectName) <= 0)
		return;

	if( strlen(pEventVisualEffect->chBoneName) <= 0 &&
		pEventVisualEffect->bAttachBone)
		return;

	// 만약 LoopEffect 리스트에 같은 이름,Bone이 있으면 새로 생성하지 않는다.
	if(IsExistLoopEffect(pEventVisualEffect->chEffectName, pEventVisualEffect->chBoneName))
		return;

	CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventVisualEffect->chEffectName);
	DBO_ASSERT(pPLEntity, "Entity create failed.")
	if(pPLEntity == NULL)
		return;

	//Effect에 Object의 Serial ID를 넣는다.(Client에서 사용을 하기 위해서)
	pPLEntity->SetSerialID(GetSerialID());

	pPLEntity->SetVisible(IsVisible(0));

	CNtlInstanceEffect *pInstanceEffect = (CNtlInstanceEffect *)pPLEntity;

	// AutoDelete가 아니면 LoopEffect라고 간주하고 리스트에 추가한다              
	if(!pPLEntity->IsAutoDelete())
	{
		SLoopEffect* pLoopEffect = NTL_NEW SLoopEffect();
		pLoopEffect->pLoopEffectIntance = pInstanceEffect;
		pLoopEffect->szBoneName = pEventVisualEffect->chBoneName;

		AddLoopEffect(pLoopEffect);
	}

    if(!pEventVisualEffect->bAttach)
    {
        RwV3d vPos = GetPosition() + pEventVisualEffect->vOffSetPos;
        pInstanceEffect->SetPosition(&vPos);
    }
	else if( pEventVisualEffect->bAttachBone)				
	{
		SPLAttachAttr attachAttr;
		attachAttr.vOffsetPos = pEventVisualEffect->vOffSetPos;        

		AttachBone((CNtlPLAttach *)pPLEntity, pEventVisualEffect->chBoneName, &attachAttr);
	}
	else
	{
		AttachWorldPosAndRotate((CNtlPLAttach *)pPLEntity, pEventVisualEffect->vOffSetPos);
	}
}

void CNtlPLObject::OnEventAlphaFade( SEventAlpha* pEventAlpha ) 
{
	// Atomic Alpha에 관해서만 PL단에서 처리한다.
	if(pEventAlpha->eAlphaEventType != SEventAlpha::E_ALPHA_EVENT_ATOMIC)
	{
		CNtlPLEventGenerator::AnimEventAlpha(GetSerialID(), (void*)pEventAlpha);
		return;
	}

	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(Helper_GetBitFlag(pEventAlpha->bfAtomicList, i))
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);

			std::string strAtomicName = Helper_AtomicName(m_vecAtomicList[i]->pData);
			GetAlphaBlendController()->AddAtomicAlpha(strAtomicName.c_str(), 
				pEventAlpha->nStartAlpha / 255.0f, 
				pEventAlpha->nDestAlpha / 255.0f, 
				pEventAlpha->fFadeTime, 
				pEventAlpha->fLifeTime, FALSE);
		}        
	}
}

void CNtlPLObject::OnEventTMQ( SEventAnimCinematic* pEventTMQ ) 
{
	RwBool bIn;
	if(pEventTMQ->eTMQEventType == E_TMQ_EVENT_IN)
	{
		bIn = TRUE;
	}
	else
	{
		bIn = FALSE;
	}

	CNtlPLEventGenerator::AnimEventTMQ(GetSerialID(), bIn);
}

void CNtlPLObject::OnEventExplosion( SEventExplosion* pEventExplosion ) 
{
	CNtlPLEventGenerator::AnimEventExplosion(GetSerialID(), (void*)pEventExplosion);
}

void CNtlPLObject::ClearLoopSound() 
{
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hSound = *it;
		GetSoundManager()->Stop(hSound);        
	}

	m_listLoopSound.clear();
}

RwBool CNtlPLObject::IsExistLoopSound( RwChar* szSoundName ) 
{
	ListSoundHandle::iterator it = m_listLoopSound.begin();
	for(; it != m_listLoopSound.end(); ++it)
	{
		SOUND_HANDLE hListSound = *it;
        if(strlen(GetSoundManager()->GetSoundName(hListSound)) > 0)
        {
		    if(_strcmpi(GetSoundManager()->GetSoundName(hListSound),szSoundName) == 0)
		    {
			    return TRUE;
		    }
        }
	}

	return FALSE;
}

RwReal CNtlPLObject::GetAnimPlayTime( RwUInt32 uiAnimKey ) 
{
	// AnimTable이 아직 생성되어 있지 않다면 생성해준다. 
	if(!m_pInstanceAnimTable)
	{
		m_pInstanceAnimTable = NTL_NEW CNtlInstanceAnimTable();
		m_pInstanceAnimTable->Create(m_pProperty->GetAnimTable());
	}

	SInstanceAnimData* pAnimData = m_pInstanceAnimTable->Get(uiAnimKey);
	if(!pAnimData)
		return 0.0f;

	return pAnimData->pTypeAnimData->fPlayTime;
}

void CNtlPLObject::ClearLoopEffect() 
{
	ListLoopEffect::iterator it = m_listLoopEffect.begin();
	for(;it != m_listLoopEffect.end(); ++it)
	{
		SLoopEffect* pLoopEffect = *it;
		if(pLoopEffect && pLoopEffect->pLoopEffectIntance)
		{
			pLoopEffect->pLoopEffectIntance->Finish();
		}        

		NTL_DELETE(pLoopEffect);
	}

	m_listLoopEffect.clear();
}

RwBool CNtlPLObject::IsExistLoopEffect( const RwChar* szEffectName, const RwChar* szBoneName ) 
{
	if(!szEffectName || !szBoneName)
		return FALSE;

	ListLoopEffect::iterator it = m_listLoopEffect.begin();
	for(;it != m_listLoopEffect.end(); ++it)
	{
		SLoopEffect* pLoopEffect = *it;
		if(pLoopEffect && pLoopEffect->pLoopEffectIntance)
		{
			if(strcmp(pLoopEffect->pLoopEffectIntance->GetName(), szEffectName) == 0 &&
				strcmp(pLoopEffect->szBoneName, szBoneName) == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CNtlPLObject::SetAlpha(RwUInt8 byValue)		
{
	m_sColor.alpha = byValue;  

	if(m_pClumpResource == NULL)
		return;

	if(byValue >= 255)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag &(~NTL_RUNTIME_ALPHA));
		}
	}
	else
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RwUInt32 uiRenderFlag = RpNtlAtomicGetFlag(m_vecAtomicList[i]->pData);
			RpNtlAtomicSetFlag(m_vecAtomicList[i]->pData, uiRenderFlag | NTL_RUNTIME_ALPHA);
		}
	}	
}

void CNtlPLObject::SetAtomicWeightAlpha( const RwChar *pAtomicName, RwReal fWeightValue ) 
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		if(strcmp(Helper_AtomicName(m_vecAtomicList[i]->pData), pAtomicName) == 0)
		{
			RwUInt8 byAlpha = (RwUInt8)(fWeightValue * 255.0f);
			RpNtlAtomicSetAlpha(m_vecAtomicList[i]->pData, byAlpha);            
			break;
		}
	}

}

RwMatrix* CNtlPLObject::GetBoneMatrix( const RwChar *pBoneName ) 
{
	if(!GetClump())
		return NULL;

	if(m_mapFrame.find( pBoneName ) == m_mapFrame.end())
		return NULL;

	RwFrame *pFrame = m_mapFrame[pBoneName];

	if(pFrame != NULL)
		return &pFrame->ltm;

	return NULL;
}

void CNtlPLObject::GetBoneName(std::vector<std::string>& vecBoneName)
{
	for (FRAME_MAP_ITER it = m_mapFrame.begin(); it != m_mapFrame.end(); ++it)
	{
		vecBoneName.push_back(it->first);
	}
}

RwV3d CNtlPLObject::GetDirection() 
{
	return *RwMatrixGetAt(&GetMatrix());
}

void CNtlPLObject::UpdateLoopSoundPos() 
{
	for each(SOUND_HANDLE hSound in m_listLoopSound)
	{
		GetSoundManager()->SetSoundPosition(hSound, m_vWorldPosition.x, m_vWorldPosition.y, m_vWorldPosition.z);
	}
}

void CNtlPLObject::CheckToonData() 
{
	RpAtomic* pAtomic = NULL;
	RpToonGeo *pToonGeo = NULL;
	for each(SEntityAtomicData* pAtomicData in m_vecAtomicList)
	{
		pAtomic = pAtomicData->pData;         
		if(pAtomic)
		{
			pToonGeo = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pAtomic) );
			if(pToonGeo)
				break;
		}         
	}

	if(!pToonGeo || !pAtomic)
		return;

	// Toon 적용 (테스트)
	m_pToonData = NTL_NEW SToonData();
	m_pToonData->pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture("smooth.png", "texture/toon/");
	DBO_ASSERT(m_pToonData->pTexture, "Texture load failed.");

	m_pToonData->pToonInk = RpToonInkCreate();
	RpToonInkSetOverallThickness(m_pToonData->pToonInk, DEFAULT_INK_THICKNESS);
	RwRGBA colInk;
	colInk.red = colInk.green = colInk.blue = 0;
	colInk.alpha = 255;
	RpToonInkSetColor(m_pToonData->pToonInk, colInk);
	RpToonInkSetName(m_pToonData->pToonInk, "silhouette");
	m_pToonData->pToonPaint = RpToonPaintCreate();
	RpToonPaintSetGradientTexture(m_pToonData->pToonPaint, m_pToonData->pTexture);
	Helper_SetToonClump(GetClump(), m_pToonData);
}

#ifdef dNTL_WORLD_CULLING_NEW
//#include "NtlPLRenderGeometry.h"
RwBool CNtlPLObject::CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame)
{
	if (m_uiRenderFrame != uiRenderFrame)
	{
		if (!CNtlPLEntity::CullingTest(pRwCamera, uiRenderFrame))
		{
			RwInt32 iNumAtomic		= static_cast<RwInt32>(m_vecAtomicList.size());
			RwInt32	iFrustumCheck	= 0;
			RwInt32	iOccluderCheck	= 0;

			if (m_bActiveUpdate && iNumAtomic > 0)
			{	
				RwReal fVisibleDistance = GetVisibleCullingDistance();

#ifdef dNTL_WORLD_TOOL_MODE
				if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
				{							
					fVisibleDistance = RwCameraGetFarClipPlane(pRwCamera);
				}
#endif				

				if (IsCullingTestAllAtomic())
				{
					// [m_vecAtomicList.size()]번 Occluder Proxy는 Bounding Sphere다.
					for (RwInt32 i = 0; i < iNumAtomic; ++i)
					{
#ifdef _DEBUG
						++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif
						RpAtomic*		pAtomic = static_cast<RpAtomic*>(m_vecAtomicList.at(i)->pData);
						const RwSphere* pSphere = RpAtomicGetWorldBoundingSphere(pAtomic);

						if (!CNtlPLGlobal::m_bCollObjVisible && (RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE))
						{
							++iFrustumCheck;							
						}
						else if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
						{
							++iFrustumCheck;
						}
						else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, i))
						{ 
							++iOccluderCheck;
						}						
						else
						{
							break;
						}
					}
				}
				else
				{
					RwSphere* pSphere = GetBoundingSphere();

// 					static RwRGBA	clr = { 255, 255, 255 ,255 };
// 					static RwMatrix	mat;
// 
// 					RwMatrixTranslate(&mat, &pSphere->center, rwCOMBINEREPLACE);
// 					RenderGeometryBillboardSphere(&mat, &clr, pSphere->radius, pRwCamera, TRUE);

#ifdef _DEBUG
					++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

					if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
					{
						iFrustumCheck = iNumAtomic;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, iNumAtomic))
					{	
						iOccluderCheck = iNumAtomic;
					}
				}
			}

			if (iFrustumCheck >= iNumAtomic)
			{
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
			}
			else if (iFrustumCheck + iOccluderCheck >= iNumAtomic) 
			{
				// Frustum + Occluder 갯수가 Atomic 갯수보다 많다면
				// OCCLUDER Flag를 셋팅 한다. 단 OccluderCheck 갯수가 존재해야 하므로, iFrustumCheck >= iNumAtomic를 통과해야만 가능하다.
				m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
			}
		}
		SetCullFlags(m_uiCullFlags);
	}
	return !IsVisible();
}
#else
RwBool CNtlPLObject::CullingTest(RwCamera* pRwCamera)
{
	if (!CNtlPLEntity::CullingTest(pRwCamera))
	{
		RwInt32 iNumAtomic = static_cast<RwInt32>(m_vecAtomicList.size());

		if (m_bActiveUpdate && iNumAtomic > 0)
		{
			RwInt32				 iFrustumCheck = 0;
			RwInt32				iOccluderCheck = 0;

			RwReal				fVisibleDistance = GetVisibleCullingDistance();

			if (m_bHaveAnim)
			{
				for (RwInt32 i = 0; i < iNumAtomic; ++i)
				{
#ifdef _DEBUG
					++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

					CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
					if(pCullScheduling)
					{
						pCullScheduling->IncAtomicOperator(PLENTITY_OBJECT);
					}

					RpAtomic* pAtomic = static_cast<RpAtomic*>(m_vecAtomicList.at(i)->pData);
					RwSphere* pSphere = &pAtomic->worldBoundingSphere;

#ifdef dNTL_WORLD_TOOL_MODE
					if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
					{
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) | rpATOMICRENDER);
						continue;
					}
#endif				
					if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
					{
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) & (~rpATOMICRENDER));
						++iFrustumCheck;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, i))
					{ 
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) & (~rpATOMICRENDER));
						++iOccluderCheck;
					}
					else
					{					
						rwObjectSetFlags(pAtomic, rwObjectGetFlags(pAtomic) | rpATOMICRENDER);
					}
				}

				if (iFrustumCheck >= iNumAtomic)
				{
					m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
				}
				else if (iFrustumCheck + iOccluderCheck >= iNumAtomic) 
				{
					// Frustum + Occluder 갯수가 Atomic 갯수보다 많다면
					// OCCLUDER Flag를 셋팅 한다. 단 OccluderCheck 갯수가 존재해야 하므로, iFrustumCheck >= iNumAtomic를 통과해야만 가능하다.
					m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
				}
			}
			else
			{
#ifdef _DEBUG
				++CNtlPLGlobal::m_uiCullTestAtomicCnt;
#endif

#ifdef dNTL_WORLD_TOOL_MODE
				if (CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
#endif
				{
					CNtlPLCullingScheduling *pCullScheduling = GetNtlPLCullingScheduler();
					if(pCullScheduling)
					{
						pCullScheduling->IncAtomicOperator(PLENTITY_OBJECT);
					}

					RwSphere* pSphere = GetBoundingSphere();
					if (RpNtlCameraFrustumTestSphere(pRwCamera, pSphere, &fVisibleDistance) == rwSPHEREOUTSIDE)
					{
						m_uiCullFlags |= NTL_PLEFLAG_CULLED_CAMERA_FRUSTUM_OUT;
					}
					else if (OccluderQuery(EOCCLUDER_SPHERE, EPLOCCLUDER_FUNC_OBJECT, (void*)pSphere, 0))
					{
						m_uiCullFlags |= NTL_PLEFLAG_CULLED_OCCLUDER;
					}
				}				
			}		
		}
	}

	SetCullFlags(m_uiCullFlags);
	return static_cast<RwBool>(m_uiCullFlags);
}
#endif

RwReal CNtlPLObject::GetVisibleCullingDistance()
{
#ifdef dNTL_WORLD_TOOL_MODE
	return m_fVisibleCullingDistance;
#else
	if (m_fVisibleCullingDistance > 512 || m_fVisibleCullingDistance < GetNtlPLOptionManager()->GetObjectFar())
	{
		return m_fVisibleCullingDistance;
	}
	return GetNtlPLOptionManager()->GetObjectFar();
#endif
}

// RwPlane CNtlPLObject::g_planeFar;
// 
// void CNtlPLObject::SetFarPlane(RwReal fFarDist)
// {
// 	RwMatrix* pmatCamera	= RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
// 	g_planeFar.normal		= pmatCamera->at;
// 	g_planeFar.distance		= RwV3dDotProduct(&(pmatCamera->at * fFarDist + pmatCamera->pos), &pmatCamera->at);
// }

void CNtlPLObject::SetLightMapMaterialFlags(RwUInt32 uiFlags)
{
	// comment here  for the mini indoor
	// if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

			RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
			if(pGeom != NULL)
			{
				RwInt32 nNumMat = RpGeometryGetNumMaterials(pGeom);
				for(int i = 0; i < nNumMat; i++)
				{
					RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
					if(pMaterial != NULL)
					{
						RtLtMapMaterialSetFlags(pMaterial, uiFlags);
					}
				}
			}
		}
	}
}

void CNtlPLObject::SetLightMapObjectFlags(RwUInt32 uiFlags)
{
	// comment here  for the mini indoor
	// if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
		{
			RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

			RtLtMapAtomicSetFlags(pAtomic, uiFlags);
		}
	}
}

RwUInt32 CNtlPLObject::GetLightMapMaterialFlags()
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
		RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
		if(pGeom != NULL)
		{
			RwInt32 nNumMat = RpGeometryGetNumMaterials(pGeom);
			for(int i = 0; i < nNumMat; i++)
			{
				RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
				if(pMaterial != NULL)
				{
					return RtLtMapMaterialGetFlags(pMaterial);
				}
			}
		}
	}
	return 0;
}

RwUInt32 CNtlPLObject::GetLightMapObjectFlags()
{
	for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
	{
		RpAtomic *pAtomic = m_vecAtomicList[i]->pData;

		return RtLtMapAtomicGetFlags(pAtomic);
	}
	return 0;
}

void CNtlPLObject::AllocSchedulingLTProp()
{
	FreeSchedulingLTProp();
	
	m_pSchedulingLMProp = NTL_NEW sSCHEDULING_LM_PROP;
}

void CNtlPLObject::FreeSchedulingLTProp()
{
	if (m_pSchedulingLMProp)
	{
		for (int i = 0; i < (int)m_pSchedulingLMProp->_vecSchedulingLMAtomProp.size(); ++i)
		{
			sSCHEDULING_LM_ATOM_PROP* pSchedulingLMAtomProp = m_pSchedulingLMProp->_vecSchedulingLMAtomProp.at(i);
			
			NTL_DELETE(pSchedulingLMAtomProp);
		}
		m_pSchedulingLMProp->_vecSchedulingLMAtomProp.clear();

		NTL_DELETE(m_pSchedulingLMProp);
	}
}

struct SNtlRpWorldSectorIntersectionOBB
{
	CNtlOBB*					 pOBB;
	std::vector<RpWorldSector*>* pvecRpWorldSector;
};

RpWorldSector* NtlRpWorldSectorIntersectionOBB(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	static CNtlOBB						OBBRpWorldSector;
	SNtlRpWorldSectorIntersectionOBB*	pNtlRpWorldSectorIntersectionOBB = (SNtlRpWorldSectorIntersectionOBB*)pData;
	const RwBBox*						pBBoxRpWorldSector = RpWorldSectorGetBBox(pRpWorldSector);
	
	//내부 연산을줄이기 위해 직접 입력.
	OBBRpWorldSector.fAxisLen[0] = (pBBoxRpWorldSector->sup.x - pBBoxRpWorldSector->inf.x) * 0.5f;
	OBBRpWorldSector.fAxisLen[1] = (pBBoxRpWorldSector->sup.y - pBBoxRpWorldSector->inf.y) * 0.5f;
	OBBRpWorldSector.fAxisLen[2] = (pBBoxRpWorldSector->sup.z - pBBoxRpWorldSector->inf.z) * 0.5f;
	OBBRpWorldSector.vAxisDir[0] = CNtlPLGlobal::m_vXAxisV3;
	OBBRpWorldSector.vAxisDir[1] = CNtlPLGlobal::m_vYAxisV3;
	OBBRpWorldSector.vAxisDir[2] = CNtlPLGlobal::m_vZAxisV3;
	OBBRpWorldSector.vCenterPos.x= (pBBoxRpWorldSector->sup.x + pBBoxRpWorldSector->inf.x) * 0.5f;
	OBBRpWorldSector.vCenterPos.y= (pBBoxRpWorldSector->sup.y + pBBoxRpWorldSector->inf.y) * 0.5f;
	OBBRpWorldSector.vCenterPos.z= (pBBoxRpWorldSector->sup.z + pBBoxRpWorldSector->inf.z) * 0.5f;

	if (CNtlOBB::IntersectionOBBvsOBB(pNtlRpWorldSectorIntersectionOBB->pOBB, &OBBRpWorldSector))
	{
		pNtlRpWorldSectorIntersectionOBB->pvecRpWorldSector->push_back(pRpWorldSector);
	}	
	return pRpWorldSector;
}

void CNtlPLObject::UpdateRpSectorOBBCheck()
{
	if (GetSceneManager()->GetActiveWorldType() != AW_RWWORLD)
	{
		return;
	}

	RwBBox bboxAABB = GetDefaultAABB();
	CNtlOBB::CalcOBBData(&m_OBB, &GetMatrix(), &bboxAABB);
	m_vecRpWorldSector.clear();

	RpIntersection intersection;
	intersection.type		= rpINTERSECTSPHERE; 
	intersection.t.sphere	= *GetBoundingSphere();
	
	SNtlRpWorldSectorIntersectionOBB sNtlRpWorldSectorIntersectionOBB;
	sNtlRpWorldSectorIntersectionOBB.pOBB				= &m_OBB;
	sNtlRpWorldSectorIntersectionOBB.pvecRpWorldSector	= &m_vecRpWorldSector;

	RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, NtlRpWorldSectorIntersectionOBB, &sNtlRpWorldSectorIntersectionOBB);
}

RwBool CNtlPLObject::IsRpWorldSectorList(RpWorldSector* pRpWorldSector)
{
	if (!m_bHaveAnim && m_vecRpWorldSector.end() == find(m_vecRpWorldSector.begin(), m_vecRpWorldSector.end(), pRpWorldSector))
	{
		return FALSE;
	}
	return TRUE;
}

RwUInt32 CNtlPLObject::GetObjectType()
{
	if (m_pObjectType)
	{
		return m_pObjectType->GetType();
	}
	return EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
}

void CNtlPLObject::SetObjectType(RwUInt32 uiObjectType)
{
	if (m_pObjectType)
	{
		NTL_DELETE(m_pObjectType);
	}

	switch (uiObjectType)
	{
	case EPL_OBJECT_TYPE_OUTDOOR_OBJECT:		
		break;
	case EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE:
		m_pObjectType = NTL_NEW CNtlPLObjectType_MiniIndoor_Close;
		break;
	case EPL_OBJECT_TYPE_MINI_INDOOR_OPEN:
		m_pObjectType = NTL_NEW CNtlPLObjectType_MiniIndoor_Open;
		break;
	case EPL_OBJECT_TYPE_MINI_INDOOR_OBJECT:
		m_pObjectType = NTL_NEW CNtlPLObjectType_MiniIndoor_Object;
		break;
	}

	if (m_pObjectType)
	{		
		m_pObjectType->Create(this);

		AddSceneUpdate();
	}
	else
	{
		RemoveSceneUpdate();
	}
}

void CNtlPLObject::AddSceneUpdate()
{
	// Animation이 없거나 ObjectType이 MINI_INDOOR_CLOSE 또는 EPL_OBJECT_TYPE_MINI_INDOOR_OPEN이면
	// UpdateList에 포함한다. Animation을 가지고 있는 Object는 SetProperty에서 Update Flag를 제거 하기 때문에
	// 여기서는 관여해서는 안된다.
	if (GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE || 
		GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN ||
		!m_bHaveAnim)
	{
		GetSceneManager()->AddUpdate(this);
	}
}

void CNtlPLObject::RemoveSceneUpdate()
{
	// Animation이 없거나 ObjectType이 MINI_INDOOR_CLOSE 또는 EPL_OBJECT_TYPE_MINI_INDOOR_OPEN이면
	// UpdateList에 포함한다. Animation을 가지고 있는 Object는 SetProperty에서 Update Flag를 제거 하기 때문에
	// 여기서는 관여해서는 안된다.
	if (GetObjectType() != EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE && 
		GetObjectType() != EPL_OBJECT_TYPE_MINI_INDOOR_OPEN &&
		!m_bHaveAnim)
	{
		GetSceneManager()->RemoveUpdate(this);
	}
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlPLObject::SaveSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector*	pSector			= &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];
		RwInt32				iNumTrigger		= 0;
		RwInt32				iNumNonTrigger	= 0;

		RwInt32 iNum = static_cast<RwInt32>(pSector->m_vecNtlPLEntityOrnament.size());
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i)); 

			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				++iNumTrigger;
			}
			else
			{
				++iNumNonTrigger;
			}
		}
		
		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		if(_chdir("swap") == -1)
		{
			_mkdir("swap");
			_chdir("swap");
		}		

		if(_chdir("obj") == -1)
		{
			_mkdir("obj");
			_chdir("obj");
		}

		// trigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.objt", iSectorIdx);

		FILE* pFileTrigger;
		if(fopen_s(&pFileTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		// nontrigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.obj", iSectorIdx);
		
		FILE* pFileNonTrigger;
		if(fopen_s(&pFileNonTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			fclose(pFileTrigger);

			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		fwrite(&iNumTrigger, sizeof(RwInt32), 1, pFileTrigger);
		fwrite(&iNumNonTrigger, sizeof(RwInt32), 1, pFileNonTrigger);

		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i)); 

			RwUInt32 uiMinimapLayer = pObject->GetMinimapLayer();
			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				fwrite(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileTrigger);
			}
			else
			{
				fwrite(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileNonTrigger);
			}
		}

		fclose(pFileTrigger);
		fclose(pFileNonTrigger);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlPLObject::LoadSwapFile(RwReal x, RwReal y, RwReal z)
{
	RwV3d	vSectorPos;

	vSectorPos.x = x;
	vSectorPos.y = y;
	vSectorPos.z = z;

	RwInt32	iSectorIdx = GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectorIdx(vSectorPos);
	if (iSectorIdx != -1)
	{
		CNtlWorldSector* pSector = &GetSceneManager()->GetWorld()->GetWorldFieldMgr()->m_pSectors[iSectorIdx];

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("swap");
		_chdir("obj");

		// trigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.objt", iSectorIdx);

		FILE* pFileTrigger;
		if(fopen_s(&pFileTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}

		// nontrigger file
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.obj", iSectorIdx);

		FILE* pFileNonTrigger;
		if(fopen_s(&pFileNonTrigger, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
		{
			fclose(pFileTrigger);

			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return;
		}
		
		RwInt32 iNum;
		RwInt32 iNumTrigger;
		RwInt32 iNumNonTirgger;

		fread(&iNumTrigger, sizeof(RwInt32), 1, pFileTrigger);
		fread(&iNumNonTirgger, sizeof(RwInt32), 1, pFileNonTrigger);

		iNum = iNumTrigger + iNumNonTirgger;
		for (int i = 0; i < (int)iNum; ++i)
		{
			CNtlPLObject* pObject = static_cast<CNtlPLObject*>(pSector->m_vecNtlPLEntityOrnament.at(i)); 

			RwUInt32 uiMinimapLayer = pObject->GetMinimapLayer();
			if (pObject->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				fread(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileTrigger);
			}
			else
			{
				fread(&uiMinimapLayer, sizeof(RwUInt32), 1, pFileNonTrigger);
			}
			pObject->SetMinimapLayer(uiMinimapLayer);
		}

		fclose(pFileTrigger);
		fclose(pFileNonTrigger);
		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

#endif

RwBool CNtlPLObject::IsCullingTestAllAtomic() 
{
    if(m_uiCurAnimKey != INVALID_DWORD)  // 이 키값은 트리거 애니메이션이 적용될때만 유효하다.
    {
        STypeAnimData *pTypeAnimData = m_pProperty->GetAnimTable()->Get(m_uiCurAnimKey);
        if(!pTypeAnimData)
            return TRUE;

        return pTypeAnimData->IsCullTestAllAtomic();
    }
    else
    {
        return m_pProperty->IsCullTestAllAtomic();
    }
}

RwBool CNtlPLObject::LoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{
	if (GetObjectTypePtr() && (GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
								GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN))
	{
		GetObjectTypePtr()->LoadFromFile(pFile);
	}

	// serial id
	RwUInt32 uiSID;	
	fread(&uiSID, sizeof(RwUInt32), 1, pFile);
	SetSID(uiSID);

	// object id
	if (eActiveWorldType == AW_RWWORLD)
	{
		RwUInt32 uiObjectID;
		fread(&uiObjectID, sizeof(RwUInt32), 1, pFile);
		SetObjectSerialID(uiObjectID);
	}

	// object name index
	DWORD dwNameIdx;
	fread(&dwNameIdx, sizeof(DWORD), 1, pFile);
	SetObjNameIdx(dwNameIdx);

	// visibility distance
	RwReal fVisibilityDistance;
	fread(&fVisibilityDistance, sizeof(RwReal), 1, pFile);
	SetVisibleCullingDistance(fVisibilityDistance);

	RwBool bEnableFlag;

	// enable trigger
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_TRIGGER);
	}

	// enable selection
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_SELECTION);
	}

	// enable alterable; dynamic obj.s.
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_ALTERABLE);
	}

	// enable portal
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_PORTAL);
	}

	// enable path engine collision
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_PECOLLISION);
	}

	// enable shadow prop
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		SetFlags(GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
	}

	// object PS map
	LoadPSMap(pFile);

	if(!GetNtlResourceManager()->IsLoadScheduling())
	{
		SetAtomicPSMap();
	}

	// milepost GUID
	fread(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{
		RwUInt32 uiMilePostGUID;
		fread(&uiMilePostGUID, sizeof(RwUInt32), 1, pFile);
		SetMilepostID(uiMilePostGUID);
	}

	// attach lightojbect
	LoadLightObjectAttachData(pFile);

	// Illumination
	if (eActiveWorldType == AW_RWWORLD)
	{
		if(GetNtlResourceManager()->IsLoadScheduling())
		{
			LoadSchedulingLightmapProp(pFile);
		}
		else
		{
			LoadLightmap(pFile);
		}
	}

	return TRUE;
}

RwBool CNtlPLObject::SaveIntoFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{
	if (GetObjectTypePtr() && (GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
								GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN))
	{
		GetObjectTypePtr()->SaveIntoFile(pFile);
	}

	// serial id
	RwUInt32 uiSID = GetSID();	
	fwrite(&uiSID, sizeof(RwUInt32), 1, pFile);

	// object id
	if (eActiveWorldType == AW_RWWORLD)
	{
		RwUInt32 uiObjectID = GetObjectSerialID();
		if (uiObjectID == 0)
		{
			uiObjectID = dGET_WORLD_PARAM()->WorldObjectSerialID;
			++dGET_WORLD_PARAM()->WorldObjectSerialID;
		}
		fwrite(&uiObjectID, sizeof(RwUInt32), 1, pFile);
		
	}

	// object name index
	DWORD dwNameIdx = GetObjNameIdx();
	fwrite(&dwNameIdx, sizeof(DWORD), 1, pFile);

	// visibility distance
	RwReal fVisibilityDistance = GetVisibleCullingDistance();
	fwrite(&fVisibilityDistance, sizeof(RwReal), 1, pFile);

	RwBool bEnableFlag;

	// enable trigger
	bEnableFlag = GetFlags() & NTL_PLEFLAG_TRIGGER;
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

	// enable selection
	bEnableFlag = GetFlags() & NTL_PLEFLAG_SELECTION;
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

	// enable alterable; dynamic obj.s.
	bEnableFlag = GetFlags() & NTL_PLEFLAG_ALTERABLE;
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

	// enable portal
	bEnableFlag = GetFlags() & NTL_PLEFLAG_PORTAL;
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

	// enable path engine collision
	bEnableFlag = GetFlags() & NTL_PLEFLAG_PECOLLISION;
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

	// enable shadow prop
	bEnableFlag = GetFlags() & NTL_PLEFLAG_SHADOW_PROP;
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);

	// object PS map
	SavePSMap(pFile);

	// milepost GUID
	RwUInt32 uiMilePostGUID = GetMilepostID();

	bEnableFlag = (GetProperty()->GetObjectType() == E_OBJECT_MILEPOST && uiMilePostGUID);
	fwrite(&bEnableFlag, sizeof(RwBool), 1, pFile);
	if(bEnableFlag)
	{		
		fwrite(&uiMilePostGUID, sizeof(RwUInt32), 1, pFile);
	}

	// attach lightojbect
	SaveLightObjectAttachData(pFile);

	// Illumination
	if (eActiveWorldType == AW_RWWORLD)
	{
		SaveLightmap(pFile);
	}

	return TRUE;
}

RwInt32	CNtlPLObject::SkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		CNtlPLObjectTypeHasProperty::SkipToFile(pFile);
	}

	::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // trigger obj serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // object serial id
	}
	::fseek(pFile, (sizeof(DWORD)), SEEK_CUR); // object name index
	::fseek(pFile, (sizeof(RwReal)), SEEK_CUR); // visibility distance
	::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable trigger
	::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable selection
	::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable alterable
	::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable portal
	::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable p.e. collision
	::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // shadow prop

	// PS Map Size
	RwInt32 PSAtomicCnt = 0;
	::fread(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		::fseek(pFile, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, SEEK_CUR);

		// Frustum view window extent
		::fseek(pFile, sizeof(RwReal), SEEK_CUR);

		// Light matrices
		::fseek(pFile, sizeof(RwMatrix), SEEK_CUR);

		// Map size
		RwInt32 Size = 0;
		::fread(&Size, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
	}

	// milepost
	RwBool bMilepost;
	::fread(&bMilepost, sizeof(RwBool), 1, pFile);
	if(bMilepost)
	{
		::fseek(pFile, sizeof(RwUInt32), SEEK_CUR);
	}

	// attach light object skip
	SkipLightObjectAttachData(pFile);	

	if (eActiveWorldType == AW_RWWORLD)
	{
		SkipLightmap(pFile);		
	}

	return ftell(pFile);
}

BYTE* CNtlPLObject::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(pFile, pFileMem);
	}

	// trigger obj serial id
	::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	::fwrite(pFileMem, sizeof(DWORD), 1, pFile);
	pFileMem += sizeof(DWORD);

	// visibility distance
	::fwrite(pFileMem, sizeof(RwReal), 1, pFile);
	pFileMem += sizeof(RwReal);

	// enable trigger
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable selection
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable alterable
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable portal
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable path engine collision
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable ShadowProp
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		fwrite(pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
		pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

		// Frustum view window extent
		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);

		// Light matrices
		fwrite(pFileMem, sizeof(RwMatrix), 1, pFile);
		pFileMem += (sizeof(RwMatrix));

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		fwrite(&Size, sizeof(RwInt32), 1, pFile);
		pFileMem += (sizeof(RwInt32));

		// Map
		fwrite(pFileMem, sizeof(DWORD) * Size * Size, 1, pFile);
		pFileMem += (sizeof(DWORD) * Size * Size);
	}

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	fwrite(&EnableFlg, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SaveLightObjectAttachDataFromFileMem(pFile, pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SaveLightmapFromFileMem(pFile, pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SaveLightObjectAttachDataFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 iLightObjectNum;
	CopyMemory(&iLightObjectNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for (int j = 0; j < iLightObjectNum; ++j)
	{
		fwrite(pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, pFile);
		pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

		fwrite(pFileMem, sizeof(RpLightType), 1, pFile);
		pFileMem += sizeof(RpLightType);
		fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
		pFileMem += sizeof(RwV3d);

		fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
		pFileMem += sizeof(RwV3d);
		fwrite(pFileMem, sizeof(RwRGBAReal), 1, pFile);
		pFileMem += sizeof(RwRGBAReal);
		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);
		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);

		fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);
		fwrite(pFileMem, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);
	}
	return pFileMem;
}

BYTE* CNtlPLObject::SaveLightmapFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwUInt32 uiLtMapObjectFlags;
	CopyMemory(&uiLtMapObjectFlags, pFileMem, sizeof(RwUInt32));
	fwrite(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	RwUInt32 uiLtMapMaterialFlags;
	CopyMemory(&uiLtMapMaterialFlags, pFileMem, sizeof(RwUInt32));
	fwrite(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	RwInt32 iAtomicCnt;
	CopyMemory(&iAtomicCnt, pFileMem, sizeof(RwInt32));
	fwrite(&iAtomicCnt, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	for (int j = 0; j < iAtomicCnt; ++j)
	{
		RwBool bLitmapExisted;
		CopyMemory(&bLitmapExisted, pFileMem, sizeof(RwBool));
		fwrite(&bLitmapExisted, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		if (bLitmapExisted)
		{
			RwChar szAtomName[64];
			CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * 64);
			fwrite(&szAtomName, sizeof(RwChar) * 64, 1, pFile);
			pFileMem += sizeof(RwChar) * 64;

			RwChar szTexName[64];
			CopyMemory(szTexName, pFileMem, sizeof(RwChar) * 64);
			fwrite(&szTexName, sizeof(RwChar) * 64, 1, pFile);
			pFileMem += sizeof(RwChar) * 64;

			RwInt32 iUVCnt;
			CopyMemory(&iUVCnt, pFileMem, sizeof(RwInt32));
			fwrite(&iUVCnt, sizeof(RwInt32), 1, pFile);
			pFileMem += sizeof(RwInt32);

			RwTexCoords* pTexCoords = NTL_NEW RwTexCoords [ iUVCnt ];
			CopyMemory(pTexCoords, pFileMem, sizeof(RwTexCoords) * iUVCnt);
			fwrite(pTexCoords, sizeof(RwTexCoords) * iUVCnt, 1, pFile);
			pFileMem += sizeof(RwTexCoords) * iUVCnt;
			NTL_ARRAY_DELETE(pTexCoords);
		}
	}

	return pFileMem;
}

RwInt32 CNtlPLObject::SkipLightObjectAttachData(FILE* pFile)
{
	RwInt32 iLightObjectNum;
	fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

	for (int j = 0; j < iLightObjectNum; ++j)
	{
		fseek(pFile, sizeof(RwChar) * MAX_ATTR_BONE_NAME, SEEK_CUR);

		fseek(pFile, sizeof(RpLightType), SEEK_CUR);
		fseek(pFile, sizeof(RwBool), SEEK_CUR);

		fseek(pFile, sizeof(RwV3d), SEEK_CUR);

		fseek(pFile, sizeof(RwV3d), SEEK_CUR);
		fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);

		fseek(pFile, sizeof(RwBool), SEEK_CUR);
		fseek(pFile, sizeof(RwBool), SEEK_CUR);
	}

	return ftell(pFile);
}

RwInt32 CNtlPLObject::SkipLightmap(FILE* pFile)
{
	::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // uiLtMapObjectFlag
	::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // uiLtMapMaterialFlags

	RwInt32 iAtomicCnt;
	::fread(&iAtomicCnt, sizeof(RwInt32), 1, pFile);

	for (int j = 0; j < iAtomicCnt; ++j)
	{
		RwBool bLitmapExisted;
		::fread(&bLitmapExisted, sizeof(RwBool), 1, pFile);

		if (bLitmapExisted)
		{
			::fseek(pFile, sizeof(RwChar) * 64, SEEK_CUR);
			::fseek(pFile, sizeof(RwChar) * 64, SEEK_CUR);

			RwInt32 iUVCnt;
			::fread(&iUVCnt, sizeof(RwInt32), 1, pFile);
			::fseek(pFile, sizeof(RwTexCoords) * iUVCnt, SEEK_CUR);
		}
	}

	return ftell(pFile);
}

BYTE* CNtlPLObject::SkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMem);
	}

	// trigger obj serial id
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	pFileMem += sizeof(DWORD);

	// visibility distance
	pFileMem += sizeof(RwReal);

	// enable trigger
	pFileMem += sizeof(RwBool);

	// enable selection
	pFileMem += sizeof(RwBool);

	// enable alterable
	pFileMem += sizeof(RwBool);

	// enable portal
	pFileMem += sizeof(RwBool);

	// enable path engine collision
	pFileMem += sizeof(RwBool);

	// enable ShadowProp
	pFileMem += sizeof(RwBool);

	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

		// Frustum view window extent
		pFileMem += sizeof(RwReal);

		// Light matrices
		pFileMem += (sizeof(RwMatrix));

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		pFileMem += (sizeof(RwInt32));

		// Map
		pFileMem += (sizeof(DWORD) * Size * Size);
	}

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipLightObjectAttachDataToFileMem(pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SkipLightmapToFileMem(pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SkipLightObjectAttachDataToFileMem(BYTE* pFileMem)
{
	RwInt32 iLightObjectNum;
	CopyMemory(&iLightObjectNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);

	for (int j = 0; j < iLightObjectNum; ++j)
	{
		pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

		pFileMem += sizeof(RpLightType);
		pFileMem += sizeof(RwBool);

		pFileMem += sizeof(RwV3d);

		pFileMem += sizeof(RwV3d);
		pFileMem += sizeof(RwRGBAReal);
		pFileMem += sizeof(RwReal);
		pFileMem += sizeof(RwReal);

		pFileMem += sizeof(RwBool);
		pFileMem += sizeof(RwBool);
	}
	return pFileMem;
}

BYTE* CNtlPLObject::SkipLightmapToFileMem(BYTE* pFileMem)
{
	RwUInt32 uiLtMapObjectFlags;
	CopyMemory(&uiLtMapObjectFlags, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	RwUInt32 uiLtMapMaterialFlags;
	CopyMemory(&uiLtMapMaterialFlags, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	RwInt32 iAtomicCnt;
	CopyMemory(&iAtomicCnt, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);

	for (int j = 0; j < iAtomicCnt; ++j)
	{
		RwBool bLitmapExisted;
		CopyMemory(&bLitmapExisted, pFileMem, sizeof(RwBool));
		pFileMem += sizeof(RwBool);

		if (bLitmapExisted)
		{
			RwChar szAtomName[64];
			CopyMemory(szAtomName, pFileMem, sizeof(RwChar) * 64);
			pFileMem += sizeof(RwChar) * 64;

			RwChar szTexName[64];
			CopyMemory(szTexName, pFileMem, sizeof(RwChar) * 64);
			pFileMem += sizeof(RwChar) * 64;

			RwInt32 iUVCnt;
			CopyMemory(&iUVCnt, pFileMem, sizeof(RwInt32));
			pFileMem += sizeof(RwInt32);

			pFileMem += sizeof(RwTexCoords) * iUVCnt;
		}
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMem);
	}

	// trigger obj serial id
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	pFileMem += sizeof(DWORD);

	// visibility distance
	pFileMem += sizeof(RwReal);

	// enable trigger
	CopyMemory(pResult, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);

	// enable selection
	pFileMem += sizeof(RwBool);

	// enable alterable
	pFileMem += sizeof(RwBool);

	// enable portal
	pFileMem += sizeof(RwBool);

	// enable path engine collision
	pFileMem += sizeof(RwBool);

	// enable ShadowProp
	pFileMem += sizeof(RwBool);

	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

		// Frustum view window extent
		pFileMem += sizeof(RwReal);

		// Light matrices
		pFileMem += (sizeof(RwMatrix));

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		pFileMem += (sizeof(RwInt32));

		// Map
		pFileMem += (sizeof(DWORD) * Size * Size);
	}

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipLightObjectAttachDataToFileMem(pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SkipLightmapToFileMem(pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwBool* pResult)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMem);
	}

	// trigger obj serial id
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	pFileMem += sizeof(DWORD);

	// visibility distance
	pFileMem += sizeof(RwReal);

	// enable trigger	
	pFileMem += sizeof(RwBool);

	// enable selection
	pFileMem += sizeof(RwBool);

	// enable alterable
	pFileMem += sizeof(RwBool);

	// enable portal
	pFileMem += sizeof(RwBool);

	// enable path engine collision
	CopyMemory(pResult, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);

	// enable ShadowProp
	pFileMem += sizeof(RwBool);

	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

		// Frustum view window extent
		pFileMem += sizeof(RwReal);

		// Light matrices
		pFileMem += (sizeof(RwMatrix));

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		pFileMem += (sizeof(RwInt32));

		// Map
		pFileMem += (sizeof(DWORD) * Size * Size);
	}

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SkipLightObjectAttachDataToFileMem(pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SkipLightmapToFileMem(pFileMem);
	}

	return pFileMem;
}

BYTE* CNtlPLObject::SaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwUInt32 uiObjectType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD)
{
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		pFileMem = CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(pFile, pFileMem);
	}

	// trigger obj serial id
	::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	// object serial id
	if (eActiveWorldType == AW_RWWORLD)
	{
		::fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	// trigger obj object name index
	::fwrite(pFileMem, sizeof(DWORD), 1, pFile);
	pFileMem += sizeof(DWORD);

	// visibility distance
	CLAMP(fVD, 70, 600);
	CLAMP(fMinVD, 70, 600);
	CLAMP(fMaxVD, 70, 600);

	RwReal fCurVD;
	CopyMemory(&fCurVD, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	if(fCurVD > fMinVD && fCurVD < fMaxVD)
	{
		::fwrite(&fVD, sizeof(RwReal), 1, pFile);
	}
	else
	{
		::fwrite(&fCurVD, sizeof(RwReal), 1, pFile);
	}

	// enable trigger
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable selection
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable alterable
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable portal
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable path engine collision
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	// enable ShadowProp
	::fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	RwInt32 PSAtomicCnt = 0;
	CopyMemory(&PSAtomicCnt, pFileMem, sizeof(RwInt32));

	::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);

	for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
	{
		// Map name
		fwrite(pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
		pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

		// Frustum view window extent
		fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);

		// Light matrices
		fwrite(pFileMem, sizeof(RwMatrix), 1, pFile);
		pFileMem += (sizeof(RwMatrix));

		// Map size
		RwInt32 Size = 0;
		CopyMemory(&Size, pFileMem, sizeof(RwInt32));
		fwrite(&Size, sizeof(RwInt32), 1, pFile);
		pFileMem += (sizeof(RwInt32));

		// Map
		fwrite(pFileMem, sizeof(DWORD) * Size * Size, 1, pFile);
		pFileMem += (sizeof(DWORD) * Size * Size);
	}

	// milepost
	RwBool EnableFlg;
	CopyMemory(&EnableFlg, pFileMem, sizeof(RwBool));
	fwrite(&EnableFlg, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);
	if(EnableFlg)
	{
		fwrite(pFileMem, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
	}

	pFileMem = SaveLightObjectAttachDataFromFileMem(pFile, pFileMem);

	if (eActiveWorldType == AW_RWWORLD)
	{
		pFileMem = SaveLightmapFromFileMem(pFile, pFileMem);
	}

	return pFileMem;
}
//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif