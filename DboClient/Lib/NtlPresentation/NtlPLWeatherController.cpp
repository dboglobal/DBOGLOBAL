#include "precomp_ntlpresentation.h"

// core
#include "NtlAtomic.h"
#include "NtlMath.h"
#include "NtlProfiler.h"
#include "NtlPLGlobal.h"

#include "NtlSoundManager.h"

#include "NtlPLVisualManager.h"

#include "NtlCollis.h"

#include "NtlDebug.h"
#include "NtlWorldCommon.h"

#include "NtlPLRenderState.h"

#include "NtlPLSceneManager.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLBloom.h"

#include "NtlDNController.h"

#include "NtlPLCollision.h"

#include "NtlPLWeatherController.h"

RpCollisionTriangle* CollisionWeatherTriangle(RpIntersection* pInters, RpCollisionTriangle* pRpColTri, RwReal fRatio, void* pData)
{
	NTL_SPROFILE("CollisionWeatherTriangle")

		if (fRatio <= 0.0f)
		{
			NTL_RPROFILE(pRpColTri)
				//return pRpColTri;
		}

		SWorldIntersect* pWorldIntersect = (SWorldIntersect*)pData;

		// 연산을 줄인다. Dist 연산을 하지 않는다.
		if (fRatio < pWorldIntersect->fMinDist)
		{
			RwLine* pLine		= &pInters->t.line;
			RwV3d	vCollPos	= pLine->start + fRatio * (pLine->end - pLine->start);

			pWorldIntersect->fMinDist	= fRatio;
			pWorldIntersect->vCollPos	= vCollPos;
			pWorldIntersect->bCollision	= TRUE;
		}

		NTL_RPROFILE(pRpColTri)
			//return pRpColTri;
}


RpCollisionTriangle* CollisionWeatherTriangleAttrCheck(RpIntersection* pInters, RpCollisionTriangle* pRpColTri, RwReal fRatio, void* pData)
{
	NTL_SPROFILE("CollisionWeatherTriangle")

	if (fRatio <= 0.0f)
	{
		NTL_RPROFILE(pRpColTri)
		//return pRpColTri;
	}

	SWorldIntersect* pWorldIntersect = (SWorldIntersect*)pData;
	
	// 연산을 줄인다. Dist 연산을 하지 않는다.
	if (fRatio < pWorldIntersect->fMinDist)
	{
		RwLine* pLine		= &pInters->t.line;
		RwV3d	vCollPos	= pLine->start + fRatio * (pLine->end - pLine->start);

		if (GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
		{
			NTL_RPROFILE(pRpColTri);
		}

		pWorldIntersect->fMinDist	= fRatio;
		pWorldIntersect->vCollPos	= vCollPos;
		pWorldIntersect->bCollision	= TRUE;
	}

	NTL_RPROFILE(pRpColTri)
	//return pRpColTri;
}

RpWorldSector* CollisionWeatherTerrainOutdoor(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		CNtlWorldSector*	pNtlWorldSector = pNtlSector->pNtlWorldSector;
		RpAtomic*			pAtomic			= pNtlWorldSector->m_pAtomic;

		if (pAtomic)
		{
			if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
			{
				return pRpWorldSector;
			}
			RpAtomicForAllIntersections(pAtomic, pIntersection, CollisionWeatherTriangleAttrCheck, pData);
		}
	}

	return pRpWorldSector;	
}

RpWorldSector* CollisionWeatherWaterOutdoor(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pRpWorldSector, pNtlSector);
	if (pNtlSector && pNtlSector->pNtlWorldSector)
	{
		CNtlWorldSector*	pNtlWorldSector = pNtlSector->pNtlWorldSector;

		if (pNtlWorldSector->m_pWater)
		{
			RpAtomic*			pAtomic			= pNtlWorldSector->m_pWater->_pAtom;

			if (pAtomic)
			{
				if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
				{
					return pRpWorldSector;
				}
				RpAtomicForAllIntersections(pAtomic, pIntersection, CollisionWeatherTriangle, pData);
			}
		}
	}

	return pRpWorldSector;	
}

RpCollisionTriangle* CollisionWeatherTerrainIndoor( RpIntersection *pInters, RpWorldSector* pRpWorldSector, RpCollisionTriangle *pRpColTri, RwReal fRatio, void* pData )
{
	NTL_SPROFILE("CollisionWeatherTerrainIndoor")

	if (fRatio <= 0.0f)
	{
		NTL_RPROFILE(pRpColTri)
		//return pRpColTri;
	}

	SWorldIntersect* pWorldIntersect = (SWorldIntersect*)pData;
	if (fRatio < pWorldIntersect->fMinDist)
	{
		RwLine* pLine		= &pInters->t.line;
		RwV3d	vCollPos	= pLine->start + fRatio * (pLine->end - pLine->start);

		if (GetSceneManager()->GetWorldAttribute(vCollPos) & dNMAP_TRANSPARENCY_TILE_FLAG)
		{
			NTL_RPROFILE(pRpColTri);
		}

		pWorldIntersect->fMinDist	= fRatio;
		pWorldIntersect->vCollPos	= vCollPos;
		pWorldIntersect->bCollision	= TRUE;
	}
	return pRpColTri;
}

RpWorldSector* CollisionWeatherWaterIndoor(RpIntersection * pIntersection, RpWorldSector * pRpWorldSector, void *pData)
{
	std::vector<CNtlWorldSector*> vecNtlWorldSector;
	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetSectopMap(pRpWorldSector, vecNtlWorldSector);

	for (int i = 0; i < (int)vecNtlWorldSector.size(); ++i)
	{
		CNtlWorldSector*	pNtlWorldSector = vecNtlWorldSector.at(i);

		if (pNtlWorldSector->m_pWater)
		{
			RpAtomic*			pAtomic			= pNtlWorldSector->m_pWater->_pAtom;

			if (pAtomic)
			{
				if (pIntersection->type == rpINTERSECTLINE && !NtlTestLineSphere(&pIntersection->t.line.start, &pIntersection->t.line.end, &pAtomic->worldBoundingSphere))
				{
					return pRpWorldSector;
				}
				RpAtomicForAllIntersections(pAtomic, pIntersection, CollisionWeatherTriangle, pData);
			}
		}
	}
	return pRpWorldSector;
}

RpAtomic* CollisionWeatherAtomic( RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData )
{
	NTL_SPROFILE("CollisionWeatherAtomic")
	
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));
	if(!(RpNtlAtomicGetFlag(pAtomic) & NTL_NOT_VISIBLE) && pPLEntity && pPLEntity->GetClassType() == PLENTITY_OBJECT && pPLEntity->IsVisible(0))
	{
		SWorldIntersect* pWorldIntersect = (SWorldIntersect*)pData;
		RpAtomicForAllIntersections(pAtomic, &pWorldIntersect->Intersection, CollisionWeatherTriangle, pWorldIntersect);
	}

	NTL_RPROFILE(pAtomic)
	//return pAtomic;
}

RpAtomic* CollisionWeatherSectorAtomic(CNtlWorldSector* pSector, SWorldIntersect* pWorldIntersect)
{
	NTL_SPROFILE("CollisionWeatherSectorAtomic")

	if (!pSector->m_pAtomic && RpAtomicGetGeometry(pSector->m_pAtomic))
	{
		NTL_RPROFILE(NULL)
		//return NULL;
	}

	RpMorphTarget*			pMorphTarget	= RpGeometryGetMorphTarget(RpAtomicGetGeometry(pSector->m_pAtomic), 0);
	RwV3d*					pVertices		= RpMorphTargetGetVertices(pMorphTarget);

	WORD*					pIndices		= NULL;
	RwLine*					pLine			= &pWorldIntersect->Intersection.t.line;
	RwV3d					vDelta			= pLine->end - pLine->start;



	pIndices = const_cast<WORD*>(pSector->GetBodySystemIndexBuff(dGET_SECTOR_LOD()));
	if (pIndices)
	{
		for (int i = 0; i < pSector->GetBodyPrimitiveNum(dGET_SECTOR_LOD()); ++i)
		{
			RwReal	dist;
			RwV3d	*v0, *v1, *v2;

			v0 = &pVertices[pIndices[i * 3]];
			v1 = &pVertices[pIndices[i * 3 + 1]];
			v2 = &pVertices[pIndices[i * 3 + 2]];

			if (RtIntersectionLineTriangle(&pLine->start, &vDelta, v0, v1, v2, &dist))
			{
				RpCollisionTriangle rpColTri;
				rpColTri.point			= *(v0);
				rpColTri.index			= i;
				rpColTri.vertices[0]	= v0;
				rpColTri.vertices[1]	= v1;
				rpColTri.vertices[2]	= v2;

				if (!CollisionWeatherTriangle(&pWorldIntersect->Intersection, &rpColTri, dist, pWorldIntersect))
				{
					NTL_RPROFILE(NULL)
					//return NULL;
				}
			}
		}
	}

	for (int j = 0; j < 5; ++j)
	{
		pIndices = const_cast<WORD*>(pSector->GetLinkSystemIndexBuff(dGET_SECTOR_LOD(), j));
		if (pIndices)
		{
			for (int i = 0; i < pSector->GetLinkPrimitiveNum(dGET_SECTOR_LOD(), j); ++i)
			{
				RwReal	dist;
				RwV3d	*v0, *v1, *v2;

				v0 = &pVertices[pIndices[i * 3]];
				v1 = &pVertices[pIndices[i * 3 + 1]];
				v2 = &pVertices[pIndices[i * 3 + 2]];

				if (RtIntersectionLineTriangle(&pLine->start, &vDelta, v0, v1, v2, &dist))
				{
					RpCollisionTriangle rpColTri;
					rpColTri.point			= *(v0);
					rpColTri.index			= i;
					rpColTri.vertices[0]	= v0;
					rpColTri.vertices[1]	= v1;
					rpColTri.vertices[2]	= v2;

					if (!CollisionWeatherTriangle(&pWorldIntersect->Intersection, &rpColTri, dist, pWorldIntersect))
					{
						NTL_RPROFILE(NULL)
						//return NULL;
					}
				}
			}
		}
	}

	if (pWorldIntersect->bCollision)
	{
		pWorldIntersect->vCollPos.y = GetSceneManager()->GetWorld()->GetWorldSectorHeight(pWorldIntersect->vCollPos);		
	}	

	NTL_RPROFILE(pSector->m_pAtomic)
	//return pSector->m_pAtomic;
}

//////////////////////////////////////////////////////////////////////////
// CNtlPLWeatherController
//////////////////////////////////////////////////////////////////////////
CNtlPLWeatherController::CNtlPLWeatherController()
:m_pWeatherEmitter(NULL)
,m_eWeatherUpdateFlag(NULL)
,m_hGlobalSound(INVALID_SOUND_HANDLE)
,m_iParticleSoundNum(0)
,m_CBParticleCollisionXRay(NULL)
,m_CBParticleCollisionTime(NULL)
,m_fCollisionLineScale(1.0f)
{
}

CNtlPLWeatherController::~CNtlPLWeatherController()
{
}

RwBool CNtlPLWeatherController::Create(CNtlPLWeatherProperty* pWeatherProperty, SPLWeatherEmitter* pWeatherEmitter)
{
	NTL_SPROFILE("CNtlPLWeatherController::Create")

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		m_CBParticleCollisionTime = &CNtlPLWeatherController::CalcParticleCollisionTimeIndoor;
		m_CBParticleCollisionXRay = &CNtlPLWeatherController::CalcParticleCollisionXRayIndoor;
	}
	else
	{
		m_CBParticleCollisionTime = &CNtlPLWeatherController::CalcParticleCollisionTimeOutdoor;
		m_CBParticleCollisionXRay = &CNtlPLWeatherController::CalcParticleCollisionXRayOutdoor;
	}

	if (pWeatherProperty && pWeatherEmitter)
	{
		m_pWeatherEmitter = pWeatherEmitter;

		if (m_pWeatherEmitter->eParticleBillboardType == EPLWEATHER_PARTICLE_BILLBOARD_Y)
		{
			m_eWeatherUpdateFlag |= EPLWEATHER_UPDATE_MATRIX;
		}

		if (!(pWeatherEmitter->clrParticleStart.alpha == pWeatherEmitter->clrParticleCenter.alpha &&
			pWeatherEmitter->clrParticleStart.alpha == pWeatherEmitter->clrParticleEnd.alpha &&
			pWeatherEmitter->clrParticleStart.red == pWeatherEmitter->clrParticleCenter.red &&
			pWeatherEmitter->clrParticleStart.red == pWeatherEmitter->clrParticleEnd.red &&
			pWeatherEmitter->clrParticleStart.blue == pWeatherEmitter->clrParticleCenter.blue &&
			pWeatherEmitter->clrParticleStart.blue == pWeatherEmitter->clrParticleEnd.blue &&
			pWeatherEmitter->clrParticleStart.green == pWeatherEmitter->clrParticleCenter.green &&
			pWeatherEmitter->clrParticleStart.green == pWeatherEmitter->clrParticleEnd.green))
		{
			m_eWeatherUpdateFlag |= EPLWEATHER_UPDATE_COLOR;
		}

		m_fParticleSoundTime	= m_pWeatherEmitter->fParticleSoundTime;
		m_fCollisionLineScale	= m_pWeatherEmitter->fCollisionLineScale;

		if ((m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_SOUND_GLOBAL))
		{
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_WEATHER_MUSIC;
			tSoundParam.pcFileName		= (char*)m_pWeatherEmitter->strGlobalSound.c_str();

			// SOUNDRESULT_OK CHECK 해야 한다. eSoundResultType가 추가 되면 그 때 같이 넣자.
			if (SOUNDRESULT_OK != GetSoundManager()->Play(&tSoundParam) )
			{
			}

			m_hGlobalSound = tSoundParam.hHandle;
		}

		m_vEmitterLerp.x	= 0.0f;
		m_vEmitterLerp.y	= 0.0f;
		m_vEmitterLerp.z	= 0.0f;
		m_vErrorPos			= *RwMatrixGetPos(RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

		NTL_RPROFILE(TRUE)
		//return TRUE;
	}

	NTL_RPROFILE(FALSE)
	//return FALSE;
}

void CNtlPLWeatherController::Destroy()
{
	NTL_SPROFILE("CNtlPLWeatherController::Destroy")

	for (PARTICLE_LIST_IT it = m_listParticle.begin(); it != m_listParticle.end(); ++it)
	{
		*it = DeleteParticle(*it);
	}
	
	if (m_hGlobalSound != INVALID_SOUND_HANDLE)
	{
		GetSoundManager()->Stop(m_hGlobalSound);
	}

	m_listParticle.clear();

	NTL_EPROFILE()
}

RwBool CNtlPLWeatherController::Update(RwReal fElapsed, RwReal fFade)
{
	NTL_SPROFILE("CNtlPLWeatherController::Update")

 	UpdateEmitterPos();

	UpdateParticleCreate(fElapsed, fFade);

	if (m_listParticle.size())
	{
		PARTICLE_LIST_IT	it					= m_listParticle.begin();
		PARTICLE_LIST_IT	itEnd				= m_listParticle.end();

		RwInt32				iCollisionCalcMax	= 100/*static_cast<RwInt32>(static_cast<RwReal>(m_pWeatherEmitter->iParticleCreateNum) * (1 / m_pWeatherEmitter->fParticleCreateTime) * fElapsed) + 1*/;
		RwInt32				iCollisionCalcCount	= 0;

		do 
		{
			SPLWeatherParticle*& pParticle = *it;

			// UpdateParticleFrustom에서는 보이지 않는 객체에 대하여 임의적으로 가중치를 주어 업데이트를 시킨다.
			UpdateParticleFrustom(pParticle, fElapsed);
			UpdateParticleTime(pParticle, fElapsed);
			UpdateParticlePos(pParticle, fElapsed);

			if (pParticle->fDist2dSquared < m_pWeatherEmitter->fParticleFrustumLengthSquared && m_pWeatherEmitter->eParticleCollisionFlag)
			{
				switch (m_pWeatherEmitter->eParticleMotionType)
				{
				case EPLWEATHER_PARTICLE_MOTION_STATIC:
					{
						// Static은 움직이지 않으므로 XRay 방식으로 딱 1번 충돌 체크 한다.
						if (!(pParticle->eCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_COMPLETE) && iCollisionCalcCount < iCollisionCalcMax)
						{
							(this->*m_CBParticleCollisionXRay)(pParticle);
							++iCollisionCalcCount;
						}
					}
					break;
				case EPLWEATHER_PARTICLE_MOTION_TSNAP:
					{
						// Terrain Snap으로 동작하는 Particle은 충돌처리를 달리 해 준다.
						// TSNAP은 계속 움직이므로 EPLWEATHER_PARTICLE_COLLISION_COMPLETE 비교하지 않는다.
						(this->*m_CBParticleCollisionXRay)(pParticle);
					}
					break;
				case EPLWEATHER_PARTICLE_MOTION_LINE:
				case EPLWEATHER_PARTICLE_MOTION_SPLINE:
					{
						// Line과 Spline은 Collision DeltaTime을 계산해 낸다. 딱 1번 충돌 체크 한다.
						if (!(pParticle->eCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_COMPLETE) && iCollisionCalcCount < iCollisionCalcMax)
						{
							(this->*m_CBParticleCollisionTime)(pParticle);
							++iCollisionCalcCount;
						}
					}
					break;
				}
			}

			// CollisionTime이나 ParticleTime을 연산한 후 지워야 하는 Particle일 경우 Effect나 Sound 처리를 같이 해 준다.
			if (UpdateParticleDelete(pParticle, it))
			{
				continue;
			}

			// Particle의 기본적인 정보를을 업데이트 한다.
			if (m_eWeatherUpdateFlag & EPLWEATHER_UPDATE_MATRIX)
			{
				UpdateParticleMatrix(pParticle, fElapsed);
			}
			if (m_eWeatherUpdateFlag & EPLWEATHER_UPDATE_COLOR)
			{
				UpdateParticleColor(pParticle, fElapsed);
			}

			++it;
		} while(it != itEnd);
	}

	NTL_RPROFILE(TRUE)
}

RwBool CNtlPLWeatherController::UpdateParticleCreate(RwReal fElapsed, RwReal fFade)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticleCreate");

	RwInt32 iCreateNum = 0;
	m_fCreateTime -= fElapsed;
	if (m_fCreateTime < 0.0f)
	{
		if (static_cast<RwInt32>(m_listParticle.size()) < m_pWeatherEmitter->iParticleCreateMax)
		{
			iCreateNum = static_cast<RwInt32>(fFade * (m_pWeatherEmitter->iParticleCreateNum + dPL_WEATHER_RANDOM(m_pWeatherEmitter->iParticleCreateNumGap)));

			for (RwInt32 i = 0; i < iCreateNum; ++i)
			{
				SPLWeatherParticle* pParticle = CreateParticle(i, iCreateNum);
				
				if (pParticle)
				{
					if ((m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_EFFECT_CREATE))
					{
						std::string& strParticleCreateEffect = m_pWeatherEmitter->vecParticleCreateEffect.at(NtlRandomNumber(0, m_pWeatherEmitter->vecParticleCreateEffect.size() - 1));
						CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, strParticleCreateEffect.c_str());
						DBO_ASSERT(pPLEntity, "Entity create failed.");
					
						pPLEntity->SetPosition(&pParticle->vPos);
					}

					m_listParticle.push_back(pParticle);				

					SendParticleEvent(pParticle, m_pWeatherEmitter->eParticleCreateEventType);
				}
			}
		}
 		m_fCreateTime = m_pWeatherEmitter->fParticleCreateTime + dPL_WEATHER_RANDOM(m_pWeatherEmitter->fParticleCreateTimeGap);
	}

	if (m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_SOUND_PARTICLE)
	{
		m_iParticleSoundNum += iCreateNum;
		m_fParticleSoundTime -= fElapsed;
		if (m_fParticleSoundTime < 0.0f)
		{
			if (m_iParticleSoundNum > 0)
			{
				RwInt32 iIdx = RwInt32MAXVAL;
				RwInt32 iMin = RwInt32MAXVAL;
				for (RwInt32 i = 0; i < static_cast<RwInt32>(m_pWeatherEmitter->vecParticleSound.size()); ++i)
				{
					SPLWeatherEmitter::PAIR_SOUND& pairSound = m_pWeatherEmitter->vecParticleSound[i];

					RwInt32 iDist = abs(m_iParticleSoundNum - pairSound.first);
					if (iDist < iMin)
					{
						iMin = iDist;
						iIdx = i;
					}
				}
				if (iIdx != RwInt32MAXVAL && (m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_SOUND_PARTICLE))
				{
					sNtlSoundPlayParameta tSoundParam;
					tSoundParam.iChannelGroup	= CHANNEL_GROUP_WEATHER_EFFECT_SOUND;
					tSoundParam.pcFileName		= (char*)m_pWeatherEmitter->vecParticleSound[iIdx].second.c_str();

					GetSoundManager()->Play(&tSoundParam);
				}
			}

			m_iParticleSoundNum		= 0;
			m_fParticleSoundTime	= m_pWeatherEmitter->fParticleSoundTime;
 		}
	}

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::SendParticleEvent(SPLWeatherParticle* pParticle, RwUInt32 eEvent)
{
	NTL_SPROFILE("CNtlPLWeatherController::SendParticleEvent");
	
	switch (eEvent)
	{
	case EPLWATHER_PARTICLE_EVENT_NONE:
		{
		}
		break;
	case EPLWATHER_PARTICLE_EVENT_LIGHTNING:
		{
			GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNtlPLBloom()->RunableLightning(0.1f, 2.0f);
		}
		break;
	case EPLWATHER_PARTICLE_EVENT_LIGHTNING2:
		{
			//GetDnController()->AddDNNode(0.1f, 2.0f, 64, 0);
	
 			GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNtlPLBloom()->RunableLightning(0.1f, 2.0f);

			GetSceneManager()->GetSky()->RunableLightning(0.1f, 2.0f);
		}
		break;
	}

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::UpdateEmitterPos()
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateEmitterPos");

	RwMatrix* pmatCamera = RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
	RwMatrix  matTemp;

	switch (m_pWeatherEmitter->eEmitterPlaceType)
	{
	case EPLWEATHER_EMITTER_PLACE_CAMERA_POS:
		{
			RwV3d vTarget;
			vTarget.x = RwMatrixGetPos(pmatCamera)->x + (RwMatrixGetAt(pmatCamera)->x * m_pWeatherEmitter->vEmitterSize.x);
			vTarget.y = RwMatrixGetPos(pmatCamera)->y + (RwMatrixGetAt(pmatCamera)->y * m_pWeatherEmitter->vEmitterSize.y);
			vTarget.z = RwMatrixGetPos(pmatCamera)->z + (RwMatrixGetAt(pmatCamera)->z * m_pWeatherEmitter->vEmitterSize.z);

			RwMatrixTranslate(&matTemp, &vTarget, rwCOMBINEREPLACE);
		}
		break;
	case EPLWEATHER_EMITTER_PLACE_CAMERA_AT:
		{
			RwMatrixRotate(&matTemp, &CNtlPLGlobal::m_vYAxisV3, CNtlMath::LineToAngleY(RwMatrixGetAt(pmatCamera)), rwCOMBINEREPLACE);
			RwMatrixTranslate(&matTemp, RwMatrixGetPos(pmatCamera), rwCOMBINEPOSTCONCAT);
		}
		break;
	}
	
	RwV3dTransformPoint(&m_vWeatherEmitter, &m_pWeatherEmitter->vEmitterPos, &matTemp);

	if (m_vErrorPos != *RwMatrixGetPos(pmatCamera))
	{
		RwV3d vTemp	= *RwMatrixGetPos(pmatCamera) - m_vErrorPos;

		m_vEmitterLerp.x	= vTemp.x * m_pWeatherEmitter->vEmitterSize.x + m_vEmitterLerp.x;
		m_vEmitterLerp.y	= vTemp.y * m_pWeatherEmitter->vEmitterSize.y + m_vEmitterLerp.y;
		m_vEmitterLerp.z	= vTemp.z * m_pWeatherEmitter->vEmitterSize.z + m_vEmitterLerp.z;
		m_vErrorPos			= *RwMatrixGetPos(pmatCamera);
	}
	m_vEmitterLerp		= m_vEmitterLerp * 0.9f;
	m_vWeatherEmitter	= m_vEmitterLerp + m_vWeatherEmitter;

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::UpdateParticleTime(SPLWeatherParticle* pParticle, RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticleTime");

	pParticle->fTimeCur		+= fElapsed;
	pParticle->fTimeDelta	 = pParticle->fTimeCur / pParticle->fTimeEnd;

	/*if (pParticle->fTimeDelta > 1.0f)
	{
		pParticle->fTimeDelta = 1.0f;
	}*/

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::UpdateParticleMatrix(SPLWeatherParticle* pParticle, RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticleMatrix");

	if (m_pWeatherEmitter->eParticleBillboardType == EPLWEATHER_PARTICLE_BILLBOARD_Y)
	{
		RwMatrix*	pmatCamera		= RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
		RwV3d*		pParticleUp		= RwMatrixGetUp(&pParticle->mat);
		RwV3d*		pParticleAt		= RwMatrixGetAt(&pParticle->mat);
		RwV3d*		pParticleRight	= RwMatrixGetRight(&pParticle->mat);
		RwV3d*		pParticlePos	= RwMatrixGetPos(&pParticle->mat);

		*pParticleUp = m_pWeatherEmitter->vParticleBillboard;

		RwV3dSubMacro(pParticleAt, RwMatrixGetPos(pmatCamera), &pParticle->vPos);
		RwV3dNormalize(pParticleAt, pParticleAt);

		RwV3dCrossProduct(pParticleRight, pParticleAt, pParticleUp);
		RwV3dNormalize(pParticleRight, pParticleRight);

		// SCALE
		RwV3dScale(pParticleRight, pParticleRight, m_pWeatherEmitter->vParticleSize.x);
		RwV3dScale(pParticleUp, pParticleUp, m_pWeatherEmitter->vParticleSize.y);

		// POS
		*pParticlePos = pParticle->vPos;

		// UPDATE
		RwMatrixTolerance matTol;
		RwEngineGetMatrixTolerances(&matTol);
		RwMatrixOptimize(&pParticle->mat, &matTol);
	}

	NTL_RPROFILE(TRUE)
	//return TRUE;
}


RwBool CNtlPLWeatherController::UpdateParticleFrustom(SPLWeatherParticle* pParticle, RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticleFrustom");

//	pParticle->fTimeFrustum -= fElapsed;
// 	if (pParticle->fTimeFrustum < 0.0f)
// 	{
// 		RwMatrix* pmatCamera		= RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
// 
// 		pParticle->vDist2d.x		= pParticle->vPos.x - RwMatrixGetPos(pmatCamera)->x;
// 		pParticle->vDist2d.y		= pParticle->vPos.z - RwMatrixGetPos(pmatCamera)->z;
// 
// 		pParticle->fDist2dSquared	= CNtlMath::MathRwV2dSquaredLength(&pParticle->vDist2d);
// 
// 		RwSphere sphere;
// 		sphere.center			= pParticle->vPos;
// 		sphere.radius			= m_pWeatherEmitter->fParticleRadius;
// 
// 		if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared || !RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere))
// 		{
// 			pParticle->fDist2dSquared = m_pWeatherEmitter->fParticleFrustumLengthSquared;
// 		}
// 
// 		pParticle->fTimeFrustum = dPL_WEATHER_FRUSTUM_UPDATE_TIME;
// 	}
// 
// 	if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared)
// 	{
// 		RwReal fFakeTime			 = fElapsed * m_pWeatherEmitter->fParticleFrustumFast;
// 		pParticle->fTimeCur			+= fFakeTime;
// 
// 		if ((m_pWeatherEmitter->eParticleMotionType & EPLWEATHER_PARTICLE_MOTION_STATIC))
// 		{
// 			NTL_RPROFILE(TRUE)
// 			//return TRUE;
// 		}
// 		RwV3dIncrementScaled(&pParticle->vPos, &pParticle->vVel, fFakeTime);
// 	}

	RwMatrix* pmatCamera	= RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));

	RwSphere sphere;
	sphere.center			= pParticle->vPos;
	sphere.radius			= m_pWeatherEmitter->fParticleRadius;

	if (RwCameraFrustumTestSphere(CNtlPLGlobal::m_RwCamera, &sphere))
	{
		pParticle->vDist2d.x	= pParticle->vPos.x - RwMatrixGetPos(pmatCamera)->x;
		pParticle->vDist2d.y	= pParticle->vPos.z - RwMatrixGetPos(pmatCamera)->z;

		pParticle->fDist2dSquared	= CNtlMath::MathRwV2dSquaredLength(&pParticle->vDist2d);
	}
	else
	{
		pParticle->fDist2dSquared	= m_pWeatherEmitter->fParticleFrustumLengthSquared;
	}

	if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared)
	{
		RwReal fFakeTime			 = fElapsed * m_pWeatherEmitter->fParticleFrustumFast;
		pParticle->fTimeCur			+= fFakeTime;

		if ((m_pWeatherEmitter->eParticleMotionType == EPLWEATHER_PARTICLE_MOTION_STATIC))
		{
			NTL_RPROFILE(TRUE)
			//return TRUE;
		}
		RwV3dIncrementScaled(&pParticle->vPos, &pParticle->vVel, fFakeTime);
	}

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::UpdateParticlePos(SPLWeatherParticle* pParticle, RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticlePos");

	if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared)
	{
		// UpdateParticleFrustom 먼저 위치가 이동되었다.
		NTL_RPROFILE(TRUE)
	}

	CNtlWorldFieldManager* pFieldMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	switch (m_pWeatherEmitter->eParticleMotionType)
	{
// 	case EPLWEATHER_PARTICLE_MOTION_STATIC:
// 		{
// 		}
// 		break;
	case EPLWEATHER_PARTICLE_MOTION_TSNAP:
		{
			/*
			if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared)
			{
				pParticle->fTimeMoveCur = 0.0f;
				pParticle->fTimeMoveEnd = 0.0f;
				RwV3dIncrementScaled(&pParticle->vPos, &pParticle->vVel, fElapsed);
				return TRUE;
			}
			else if (pParticle->fTimeMoveCur <= 0.0f)
			{				
				RwReal fHeight;

				if (pParticle->fTimeMoveEnd <= 0.0f)
				{
					pParticle->fTimeMoveEnd = 0.25f;

					fHeight	= pFieldMgr->GetWorldSectorHeight(pParticle->vPos);
					if (fHeight != -999.0f)
					{
						pParticle->vPos.y = fHeight;
					}

					fHeight = pFieldMgr->GetWaterHeight(pParticle->vPos);
					if (fHeight != -9999.0f && pParticle->vPos.y < fHeight)
					{
						pParticle->vPos.y = fHeight;
					}	

					pParticle->vPos.y += m_pWeatherEmitter->vParticleHalfSize.y;
				}

				pParticle->vPosPrev = pParticle->vPos;
				pParticle->vPosNext = pParticle->vPos;

				pParticle->fTimeMoveCur	+= pParticle->fTimeMoveEnd;

				RwV3dIncrementScaled(&pParticle->vPosNext, &pParticle->vVel, pParticle->fTimeMoveCur);

				fHeight	= pFieldMgr->GetWorldSectorHeight(pParticle->vPosNext);
				if (fHeight != -999.0f)
				{
					pParticle->vPosNext.y = fHeight;
				}

				fHeight = pFieldMgr->GetWaterHeight(pParticle->vPosNext);
				if (fHeight != -9999.0f && pParticle->vPosNext.y < fHeight)
				{
					pParticle->vPosNext.y = fHeight;
				}	

				pParticle->vPosNext.y += m_pWeatherEmitter->vParticleHalfSize.y;
			}

			pParticle->fTimeMoveCur -= fElapsed;
			pParticle->vPos			= CNtlMath::Interpolation(pParticle->vPosNext, pParticle->vPosPrev, pParticle->fTimeMoveCur / pParticle->fTimeMoveEnd);
			*/

			RwV3dIncrementScaled(&pParticle->vPos, &pParticle->vVel, fElapsed);

			RwReal fHeight;

			fHeight	= pFieldMgr->GetWorldSectorHeight(pParticle->vPos);
			if (fHeight != -999.0f)
			{
				pParticle->vPos.y = fHeight;
			}

			fHeight = pFieldMgr->GetWaterHeight(pParticle->vPos);
			if (fHeight != -9999.0f && pParticle->vPos.y < fHeight)
			{
				pParticle->vPos.y = fHeight;
			}	

			pParticle->vPos.y += m_pWeatherEmitter->vParticleHalfSize.y + m_pWeatherEmitter->vEmitterPos.y;

			if (pParticle->vPos.y > RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))->y + m_pWeatherEmitter->vEmitterSize.y)
			{
				pParticle->fDist2dSquared = m_pWeatherEmitter->fParticleFrustumLengthSquared;
			}
		}
		break;
	case EPLWEATHER_PARTICLE_MOTION_LINE:
		{
			RwV3dIncrementScaled(&pParticle->vPos, &pParticle->vVel, fElapsed);
		}
		break;
	case EPLWEATHER_PARTICLE_MOTION_SPLINE:
		{
			if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared)
			{
				pParticle->fTimeMoveCur = 0.0f;
				pParticle->fTimeMoveEnd = 0.0f;
				RwV3dIncrementScaled(&pParticle->vPos, &pParticle->vVel, fElapsed);
				NTL_RPROFILE(TRUE)
				//return TRUE;
			}
			else if (pParticle->fTimeMoveCur <= 0.0f)
			{
				pParticle->vPosPrev = pParticle->vPos;
				if ((pParticle->eCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_COMPLETE) && pParticle->fCollisionDelta < 1.0f)
				{					 
					RwReal fTime = ((pParticle->fCollisionDelta - pParticle->fTimeDelta) * pParticle->fTimeEnd);
					if (fTime > m_pWeatherEmitter->fParticleSplineTime * 2.0f)
					{
						pParticle->fTimeMoveEnd		= fElapsed + pParticle->fTimeMoveCur + m_pWeatherEmitter->fParticleSplineTime;
						pParticle->vPosNext			= pParticle->vPos + (pParticle->vVel * pParticle->fTimeMoveEnd);
						pParticle->fSplinePower		= -pParticle->fSplinePower;
					}
					else
					{
						pParticle->fTimeMoveEnd		= fElapsed + pParticle->fTimeMoveCur + fTime;
						pParticle->vPosNext			= pParticle->vCollisionPoint;
						pParticle->fSplinePower		= -pParticle->fSplinePower * (fTime / (m_pWeatherEmitter->fParticleSplineTime * 2.0f));
					}
				}
				else
				{
					pParticle->fTimeMoveEnd		= fElapsed + pParticle->fTimeMoveCur + m_pWeatherEmitter->fParticleSplineTime;
					pParticle->vPosNext			= pParticle->vPos + (pParticle->vVel * pParticle->fTimeMoveEnd);
					pParticle->fSplinePower		= -pParticle->fSplinePower;
				}


				pParticle->vPosCtrl			= ((pParticle->vPosNext - pParticle->vPosPrev) * 0.5f + pParticle->vPosPrev) + ((*RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))) * pParticle->fSplinePower);
				pParticle->fTimeMoveCur		= pParticle->fTimeMoveEnd;
			}

			pParticle->fTimeMoveCur -= fElapsed;

			RwReal U	= 1.0f - (pParticle->fTimeMoveCur / pParticle->fTimeMoveEnd);
			RwReal UM	= 1.0f - U;
			RwReal U2	= U * U;
			RwReal UM2	= UM * UM;

			pParticle->vPos.x = pParticle->vPosPrev.x * UM2 + 2.0f * pParticle->vPosCtrl.x * UM * U + pParticle->vPosNext.x * U2;
			pParticle->vPos.y = pParticle->vPosPrev.y * UM2 + 2.0f * pParticle->vPosCtrl.y * UM * U + pParticle->vPosNext.y * U2;
			pParticle->vPos.z = pParticle->vPosPrev.z * UM2 + 2.0f * pParticle->vPosCtrl.z * UM * U + pParticle->vPosNext.z * U2;
		}
		break;
	}

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::UpdateParticleColor(SPLWeatherParticle* pParticle, RwReal fElapsed)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticleColor");

	RwReal fTimeDelta	= pParticle->fTimeDelta; 
	RwReal fTimeEnd		= pParticle->fTimeEnd;
	if (pParticle->eCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_COMPLETE && pParticle->fCollisionDelta != 1.0f)
	{
		fTimeDelta	= fTimeDelta / pParticle->fCollisionDelta;
		fTimeEnd	= fTimeEnd * pParticle->fCollisionDelta;
	}
	CLAMP(fTimeDelta, 0.0f, 1.0f);

	switch (m_pWeatherEmitter->eParticleBlendingType)
	{
	case EPLWEATHER_PARTICLE_BLENDING_N:
		{		
			if (fTimeDelta <= m_pWeatherEmitter->fParticleSColorTime)
			{
			pParticle->clr[0] = m_pWeatherEmitter->clrParticleStart;
			}
			else if (fTimeDelta >= m_pWeatherEmitter->fParticleEColorTime)
			{
			pParticle->clr[0] = m_pWeatherEmitter->clrParticleEnd;
			}
			else if (fTimeDelta <= m_pWeatherEmitter->fParticleCColorTime)
			{
			pParticle->clr[0] = CNtlMath::Interpolation(m_pWeatherEmitter->clrParticleStart, m_pWeatherEmitter->clrParticleCenter,
			(fTimeDelta - m_pWeatherEmitter->fParticleSColorTime) / (m_pWeatherEmitter->fParticleCColorTime - m_pWeatherEmitter->fParticleSColorTime));
			}
			else
			{
			pParticle->clr[0] = CNtlMath::Interpolation(m_pWeatherEmitter->clrParticleCenter, m_pWeatherEmitter->clrParticleEnd,
			(fTimeDelta - m_pWeatherEmitter->fParticleCColorTime) / (m_pWeatherEmitter->fParticleEColorTime - m_pWeatherEmitter->fParticleCColorTime));
			}
		}
		break;
	case EPLWEATHER_PARTICLE_BLENDING_Y:
		{
			if (fTimeDelta <= m_pWeatherEmitter->fParticleSColorTime)
			{
				pParticle->clr[2] = pParticle->clr[3] = m_pWeatherEmitter->clrParticleStart;
			}
			else if (fTimeDelta >= m_pWeatherEmitter->fParticleEColorTime)
			{
				pParticle->clr[2] = pParticle->clr[3] = m_pWeatherEmitter->clrParticleEnd;
			}
			else if (fTimeDelta <= m_pWeatherEmitter->fParticleCColorTime)
			{
				pParticle->clr[2] = pParticle->clr[3] = CNtlMath::Interpolation(m_pWeatherEmitter->clrParticleStart, m_pWeatherEmitter->clrParticleCenter,
					(fTimeDelta - m_pWeatherEmitter->fParticleSColorTime) / (m_pWeatherEmitter->fParticleCColorTime - m_pWeatherEmitter->fParticleSColorTime));
			}
			else
			{
				pParticle->clr[2] = pParticle->clr[3] = CNtlMath::Interpolation(m_pWeatherEmitter->clrParticleCenter, m_pWeatherEmitter->clrParticleEnd,
					(fTimeDelta - m_pWeatherEmitter->fParticleCColorTime) / (m_pWeatherEmitter->fParticleEColorTime - m_pWeatherEmitter->fParticleCColorTime));
			}

			fTimeDelta += (m_pWeatherEmitter->vParticleHalfSize.y / fabsf(pParticle->vVel.y)) / fTimeEnd;
			CLAMP(fTimeDelta, 0.0f, 1.0f);

			if (fTimeDelta <= m_pWeatherEmitter->fParticleSColorTime)
			{
				pParticle->clr[0] = pParticle->clr[1] = m_pWeatherEmitter->clrParticleStart;
			}
			else if (fTimeDelta >= m_pWeatherEmitter->fParticleEColorTime)
			{
				pParticle->clr[0] = pParticle->clr[1] = m_pWeatherEmitter->clrParticleEnd;
			}
			else if (fTimeDelta <= m_pWeatherEmitter->fParticleCColorTime)
			{
				pParticle->clr[0] = pParticle->clr[1] = CNtlMath::Interpolation(m_pWeatherEmitter->clrParticleStart, m_pWeatherEmitter->clrParticleCenter,
					(fTimeDelta - m_pWeatherEmitter->fParticleSColorTime) / (m_pWeatherEmitter->fParticleCColorTime - m_pWeatherEmitter->fParticleSColorTime));
			}
			else
			{
				pParticle->clr[0] = pParticle->clr[1] = CNtlMath::Interpolation(m_pWeatherEmitter->clrParticleCenter, m_pWeatherEmitter->clrParticleEnd,
					(fTimeDelta - m_pWeatherEmitter->fParticleCColorTime) / (m_pWeatherEmitter->fParticleEColorTime - m_pWeatherEmitter->fParticleCColorTime));
			}
		}
		break;
	}

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::UpdateParticleDelete(SPLWeatherParticle* pParticle, PARTICLE_LIST_IT& itParticleList)
{
	NTL_SPROFILE("CNtlPLWeatherController::UpdateParticleDelete");

	if (pParticle->eCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_COMPLETE)
	{
		if (pParticle->fTimeDelta >= pParticle->fCollisionDelta)
		{
			if (pParticle->fDist2dSquared < m_pWeatherEmitter->fEffectFrustumLengthSquared && (m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_EFFECT_COLLISION))
			{
				CNtlPLEntity *pPLEntity = NULL;
				if ((m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_EFFECT_COLLISION_WATER) && (pParticle->eCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
				{
					pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pWeatherEmitter->strParticleCollisionEffectWater.c_str());
				}
				else
				{
					pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pWeatherEmitter->strParticleCollisionEffect.c_str());
				}
				DBO_ASSERT(pPLEntity, "Entity create failed.");

				pPLEntity->SetPosition(&pParticle->vCollisionPoint);
			}
			if (m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_DESTORY)
			{
				pParticle		= DeleteParticle(pParticle);
				itParticleList	= m_listParticle.erase(itParticleList);

				NTL_RPROFILE(TRUE)
			}
		}
		else if (pParticle->fCollisionDelta <= 0.0f)
		{
			pParticle		= DeleteParticle(pParticle);
			itParticleList	= m_listParticle.erase(itParticleList);

			NTL_RPROFILE(TRUE)
		}
	}

	if (pParticle->fTimeDelta >= 1.0f)
	{
		if (pParticle->fDist2dSquared < m_pWeatherEmitter->fEffectFrustumLengthSquared && (m_pWeatherEmitter->eParticleUseFlag & EPLWEATHER_PARTICLE_USE_EFFECT_DESTROY))
		{
			CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pWeatherEmitter->strParticleDestroyEffect.c_str());
			DBO_ASSERT(pPLEntity, "Entity create failed.");
			
			pPLEntity->SetPosition(&pParticle->vCollisionPoint);
		}

		pParticle		= DeleteParticle(pParticle);
		itParticleList	= m_listParticle.erase(itParticleList);

		NTL_RPROFILE(TRUE)
	}

	NTL_RPROFILE(FALSE)
}

RwBool CNtlPLWeatherController::CalcParticleCollisionTimeOutdoor(SPLWeatherParticle* pParticle)
{
	NTL_SPROFILE("CNtlPLWeatherController::CalcParticleCollisionTimeOutdoor");

	SWorldIntersect	intersection;
	intersection.Intersection.type			= rpINTERSECTLINE;    
	intersection.Intersection.t.line.start	= pParticle->vPos + (pParticle->vVel * (-pParticle->fTimeCur + (-pParticle->fTimeEnd * (m_fCollisionLineScale - 1.0f))));	// Start Pos
	intersection.Intersection.t.line.end	= pParticle->vPos + (pParticle->vVel * -pParticle->fTimeCur) + (pParticle->vVel * pParticle->fTimeEnd);						// End Pos

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
	{
		intersection.fMinDist	= RwRealMAXVAL;
		intersection.bCollision = FALSE;
		if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherTerrainOutdoor, &intersection))
		{					
			if (intersection.bCollision)
			{
				intersection.fMinDist = intersection.fMinDist * m_fCollisionLineScale - (m_fCollisionLineScale - 1.0f);
				if (pParticle->fCollisionDelta > intersection.fMinDist)
				{
					pParticle->fCollisionDelta	= intersection.fMinDist;
					pParticle->vCollisionPoint	= intersection.vCollPos;
					pParticle->eCollisionFlag	|= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
				}
			}
		}				
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
	{
		intersection.fMinDist	= RwRealMAXVAL;
		intersection.bCollision = FALSE;
		if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherWaterOutdoor, &intersection))
		{					
			if (intersection.bCollision)
			{
				intersection.fMinDist = intersection.fMinDist * m_fCollisionLineScale - (m_fCollisionLineScale - 1.0f);
				if (pParticle->fCollisionDelta > intersection.fMinDist)
				{
					pParticle->fCollisionDelta	= intersection.fMinDist;
					pParticle->vCollisionPoint	= intersection.vCollPos;
					pParticle->eCollisionFlag	|= EPLWEATHER_PARTICLE_COLLISION_WATER;
				}
			}
		}				
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
	{
		intersection.fMinDist	= RwRealMAXVAL;
		intersection.bCollision = FALSE;
		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherAtomic, &intersection))
		{					
			if (intersection.bCollision)
			{
				intersection.fMinDist = intersection.fMinDist * m_fCollisionLineScale - (m_fCollisionLineScale - 1.0f);
				if (pParticle->fCollisionDelta > intersection.fMinDist)
				{
					pParticle->fCollisionDelta	= intersection.fMinDist;
					pParticle->vCollisionPoint	= intersection.vCollPos;
					pParticle->eCollisionFlag	|= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
				}
			}
		}				
	}

	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

	


// 	CNtlWorldFieldManager*	pFieldMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 	CNtlWorldSector*		pSectors	= pFieldMgr->m_pSectors;
// 
// 	RwInt32					iCheckSector= -1;				
// 	RwV3d					vCheckPos	= pParticle->vPos + (pParticle->vVel * -pParticle->fTimeCur);
// 	RwReal					fCheckTime	= pParticle->fTimeEnd;
// 
// 	SWorldIntersect	intersection;
// 	intersection.Intersection.type			= rpINTERSECTLINE;    
// 	intersection.Intersection.t.line.start	= vCheckPos;
// 	intersection.Intersection.t.line.end	= vCheckPos + pParticle->vVel * fCheckTime;
// 
// 	do
// 	{
// 		RwInt32 iCurSector = pFieldMgr->GetSectorIdx(vCheckPos);
// 		if (iCurSector != -1 && iCurSector != iCheckSector)
// 		{
// 			CNtlWorldSector* pSector = &pSectors[iCurSector];
// 
// 			if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
// 			{
// 				intersection.fMinDist	= RwRealMAXVAL;
// 				intersection.bCollision = FALSE;
// 				if (pSector->m_pAtomic && CollisionWeatherSectorAtomic(pSector, &intersection))
// 				{					
// 					if (intersection.bCollision)
// 					{
// 						if (pParticle->fCollisionDelta > intersection.fMinDist)
// 						{
// 							pParticle->fCollisionDelta	= intersection.fMinDist;
// 							pParticle->vCollisionPoint	= intersection.vCollPos;
// 							pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
// 						}
// 					}
// 				}
// 			}
// 
// 			if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
// 			{					
// 				intersection.fMinDist	= RwRealMAXVAL;
// 				intersection.bCollision = FALSE;
// 
// 				if (pSector->m_pWater && RpAtomicForAllIntersections(pSector->m_pWater->_pAtom, &intersection.Intersection, CollisionWeatherTriangle, &intersection))
// 				{					
// 					if (intersection.bCollision)
// 					{
// 						if (pParticle->fCollisionDelta > intersection.fMinDist)
// 						{
// 							pParticle->fCollisionDelta	= intersection.fMinDist;
// 							pParticle->vCollisionPoint	= intersection.vCollPos;
// 							pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		if (pParticle->eCollisionFlag)
// 		{
// 			break;
// 		}
// 
// 		fCheckTime	-= pParticle->fTimeEnd * 0.25f;
// 		vCheckPos	+= pParticle->vVel;
// 		iCheckSector = iCurSector;
// 	} while (fCheckTime > 0.0f);
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
// 	{
// 		intersection.fMinDist	= RwRealMAXVAL;
// 		intersection.bCollision = FALSE;
// 		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherAtomic, &intersection))
// 		{					
// 			if (intersection.bCollision)
// 			{
// 				if (pParticle->fCollisionDelta > intersection.fMinDist)
// 				{
// 					pParticle->fCollisionDelta	= intersection.fMinDist;
// 					pParticle->vCollisionPoint	= intersection.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
// 				}
// 			}
// 		}				
// 	}
// 
// 	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::CalcParticleCollisionTimeIndoor(SPLWeatherParticle* pParticle)
{
	NTL_SPROFILE("CNtlPLWeatherController::CalcParticleCollisionTimeIndoor");

	SWorldIntersect	intersection;
	intersection.Intersection.type			= rpINTERSECTLINE;    
	intersection.Intersection.t.line.start	= pParticle->vPos + (pParticle->vVel * (-pParticle->fTimeCur + (-pParticle->fTimeEnd * (m_fCollisionLineScale - 1.0f))));	// Start Pos
	intersection.Intersection.t.line.end	= pParticle->vPos + (pParticle->vVel * -pParticle->fTimeCur) + (pParticle->vVel * pParticle->fTimeEnd);						// End Pos

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
	{
		intersection.fMinDist	= RwRealMAXVAL;
		intersection.bCollision = FALSE;
		if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherTerrainIndoor, &intersection))
		{					
			if (intersection.bCollision)
			{
				intersection.fMinDist = intersection.fMinDist * m_fCollisionLineScale - (m_fCollisionLineScale - 1.0f);
				if (pParticle->fCollisionDelta > intersection.fMinDist)
				{
					pParticle->fCollisionDelta	= intersection.fMinDist;
					pParticle->vCollisionPoint	= intersection.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
				}
			}
		}
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
	{
		intersection.fMinDist	= RwRealMAXVAL;
		intersection.bCollision = FALSE;
		if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherWaterIndoor, &intersection))
		{					
			if (intersection.bCollision)
			{
				intersection.fMinDist = intersection.fMinDist * m_fCollisionLineScale - (m_fCollisionLineScale - 1.0f);
				if (pParticle->fCollisionDelta > intersection.fMinDist)
				{
					pParticle->fCollisionDelta	= intersection.fMinDist;
					pParticle->vCollisionPoint	= intersection.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
				}
			}
		}				
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
	{
		intersection.fMinDist	= RwRealMAXVAL;
		intersection.bCollision = FALSE;
		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherAtomic, &intersection))
		{					
			if (intersection.bCollision)
			{
				intersection.fMinDist = intersection.fMinDist * m_fCollisionLineScale - (m_fCollisionLineScale - 1.0f);
				if (pParticle->fCollisionDelta > intersection.fMinDist)
				{
					pParticle->fCollisionDelta	= intersection.fMinDist;
					pParticle->vCollisionPoint	= intersection.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
				}
			}
		}				
	}

	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

// 	CNtlWorldFieldManager*	pFieldMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 	CNtlWorldSector*		pSectors	= pFieldMgr->m_pSectors;
// 
// 	RwInt32					iCheckSector= -1;				
//  	RwV3d					vCheckPos	= pParticle->vPos + (pParticle->vVel * -pParticle->fTimeCur);
// 	RwReal					fCheckTime	= pParticle->fTimeEnd;
// 
// 	SWorldIntersect	intersection;
// 	intersection.Intersection.type			= rpINTERSECTLINE;    
// 	intersection.Intersection.t.line.start	= vCheckPos;
// 	intersection.Intersection.t.line.end	= vCheckPos + pParticle->vVel * fCheckTime;
// 
// 	do
// 	{
// 		RwInt32 iCurSector = pFieldMgr->GetSectorIdx(vCheckPos);
// 		if (iCurSector != -1 && iCurSector != iCheckSector)
// 		{
// 			CNtlWorldSector* pSector = &pSectors[iCurSector];
// 
// 			if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
// 			{					
// 				intersection.fMinDist	= RwRealMAXVAL;
// 				intersection.bCollision = FALSE;
// 
// 				if (pSector->m_pWater && RpAtomicForAllIntersections(pSector->m_pWater->_pAtom, &intersection.Intersection, CollisionWeatherTriangle, &intersection))
// 				{					
// 					if (intersection.bCollision)
// 					{
// 						if (pParticle->fCollisionDelta > intersection.fMinDist)
// 						{
// 							pParticle->fCollisionDelta	= intersection.fMinDist;
// 							pParticle->vCollisionPoint	= intersection.vCollPos;
// 							pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
// 						}
// 					}
// 				}
// 			}
// 		}
// 
// 		if (pParticle->eCollisionFlag)
// 		{
// 			break;
// 		}
// 
// 		fCheckTime	-= pParticle->fTimeEnd * 0.25f;
// 		vCheckPos	+= pParticle->vVel;
// 		iCheckSector = iCurSector;
// 	} while (fCheckTime > 0.0f);
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
// 	{
// 		intersection.fMinDist	= RwRealMAXVAL;
// 		intersection.bCollision = FALSE;
// 		if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherTerrainIndoor, &intersection))
// 		{					
// 			if (intersection.bCollision)
// 			{
// 				if (pParticle->fCollisionDelta > intersection.fMinDist)
// 				{
// 					pParticle->fCollisionDelta	= intersection.fMinDist;
// 					pParticle->vCollisionPoint	= intersection.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
// 				}
// 			}
// 		}
// 	}
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
// 	{
// 		intersection.fMinDist	= RwRealMAXVAL;
// 		intersection.bCollision = FALSE;
// 		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection.Intersection, CollisionWeatherAtomic, &intersection))
// 		{					
// 			if (intersection.bCollision)
// 			{
// 				if (pParticle->fCollisionDelta > intersection.fMinDist)
// 				{
// 					pParticle->fCollisionDelta	= intersection.fMinDist;
// 					pParticle->vCollisionPoint	= intersection.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
// 				}
// 			}
// 		}				
// 	}
// 
// 	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

RwBool CNtlPLWeatherController::CalcParticleCollisionXRayIndoor(SPLWeatherParticle* pParticle)
{
	NTL_SPROFILE("CNtlPLWeatherController::CalcParticleCollisionXRayIndoor");

	SWorldIntersect	intersectionLTRB, intersectionRTLB;
	RwV3d			vRight, vUp;
	intersectionLTRB.Intersection.type 				= rpINTERSECTLINE;    
	intersectionLTRB.fMinDist						= RwRealMAXVAL;
	intersectionLTRB.bCollision						= FALSE;

	intersectionRTLB.Intersection.type 				= rpINTERSECTLINE;    
	intersectionRTLB.fMinDist						= RwRealMAXVAL;
	intersectionRTLB.bCollision						= FALSE;

	if (m_pWeatherEmitter->eParticleBillboardType == EPLWEATHER_PARTICLE_BILLBOARD_N)
	{
		vRight	= *RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) * m_pWeatherEmitter->vParticleHalfSize.x;
		vUp		= *RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) * m_pWeatherEmitter->vParticleHalfSize.y;
	}
	else if (m_pWeatherEmitter->eParticleBillboardType == EPLWEATHER_PARTICLE_BILLBOARD_Y)
	{
		vRight	= *RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) * m_pWeatherEmitter->vParticleHalfSize.x;
		vUp		= m_pWeatherEmitter->vParticleBillboard * m_pWeatherEmitter->vParticleHalfSize.y;
	}

	intersectionLTRB.Intersection.t.line.start		= pParticle->vPos;
	intersectionLTRB.Intersection.t.line.end		= pParticle->vPos;

	intersectionRTLB.Intersection.t.line.start		= pParticle->vPos;
	intersectionRTLB.Intersection.t.line.end		= pParticle->vPos;

	intersectionLTRB.Intersection.t.line.start		+= vRight;
	intersectionLTRB.Intersection.t.line.start		+= vUp;
	intersectionLTRB.Intersection.t.line.end		-= vRight;
	intersectionLTRB.Intersection.t.line.end		-= vUp;

	intersectionRTLB.Intersection.t.line.start		-= vRight;
	intersectionRTLB.Intersection.t.line.start		+= vUp;
	intersectionRTLB.Intersection.t.line.end		+= vRight;
	intersectionRTLB.Intersection.t.line.end		-= vUp;

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
	{
		intersectionLTRB.fMinDist	= RwRealMAXVAL;
		intersectionLTRB.bCollision = FALSE;
		intersectionRTLB.fMinDist	= RwRealMAXVAL;
		intersectionRTLB.bCollision = FALSE;
		if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherTerrainIndoor, &intersectionLTRB))
		{					
			if (intersectionLTRB.bCollision)
			{
				pParticle->fCollisionDelta	= 0.0f;
				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
			}
		}
		if (!intersectionLTRB.bCollision)
		{
			if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherTerrainIndoor, &intersectionRTLB))
			{					
				if (intersectionRTLB.bCollision)
				{
					pParticle->fCollisionDelta	= 0.0f;
					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
				}
			}
		}
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
	{					
		intersectionLTRB.fMinDist	= RwRealMAXVAL;
		intersectionLTRB.bCollision = FALSE;
		intersectionRTLB.fMinDist	= RwRealMAXVAL;
		intersectionRTLB.bCollision = FALSE;
		if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherWaterIndoor, &intersectionLTRB))
		{					
			if (intersectionLTRB.bCollision)
			{
				pParticle->fCollisionDelta	= 0.0f;
				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
			}
		}
		if (!intersectionLTRB.bCollision)
		{
			if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherWaterOutdoor, &intersectionRTLB))		
			{					
				if (intersectionRTLB.bCollision)
				{
					pParticle->fCollisionDelta	= 0.0f;
					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
				}
			}
		}
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
	{
		intersectionLTRB.fMinDist	= RwRealMAXVAL;
		intersectionLTRB.bCollision = FALSE;
		intersectionRTLB.fMinDist	= RwRealMAXVAL;
		intersectionRTLB.bCollision = FALSE;

		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherAtomic, &intersectionLTRB))
		{					
			if (intersectionLTRB.bCollision)
			{
				pParticle->fCollisionDelta	= 0.0f;
				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
			}
		}
		if (!intersectionLTRB.bCollision)
		{
			if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherAtomic, &intersectionRTLB))
			{					
				if (intersectionRTLB.bCollision)
				{
					pParticle->fCollisionDelta	= 0.0f;
					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
				}
			}
		}
	}

	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

// 	CNtlWorldFieldManager*	pFieldMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 	CNtlWorldSector*		pSectors	= pFieldMgr->m_pSectors;
// 	RwInt32					iCurSector	= pFieldMgr->GetSectorIdx(pParticle->vPos);
// 	CNtlWorldSector*		pSector		= &pSectors[iCurSector];
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
// 	{
// 		intersectionLTRB.fMinDist	= RwRealMAXVAL;
// 		intersectionLTRB.bCollision = FALSE;
// 		intersectionRTLB.fMinDist	= RwRealMAXVAL;
// 		intersectionRTLB.bCollision = FALSE;
// 		if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherTerrainIndoor, &intersectionLTRB))
// 		{					
// 			if (intersectionLTRB.bCollision)
// 			{
// 				pParticle->fCollisionDelta	= 0.0f;
// 				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
// 				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
// 			}
// 		}
// 		if (!intersectionLTRB.bCollision)
// 		{
// 			if (RpCollisionWorldForAllIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherTerrainIndoor, &intersectionRTLB))
// 			{					
// 				if (intersectionRTLB.bCollision)
// 				{
// 					pParticle->fCollisionDelta	= 0.0f;
// 					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
// 				}
// 			}
// 		}
// 	}
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
// 	{					
// 		intersectionLTRB.fMinDist	= RwRealMAXVAL;
// 		intersectionLTRB.bCollision = FALSE;
// 		intersectionRTLB.fMinDist	= RwRealMAXVAL;
// 		intersectionRTLB.bCollision = FALSE;
// 		if (pSector->m_pWater && RpAtomicForAllIntersections(pSector->m_pWater->_pAtom, &intersectionLTRB.Intersection, CollisionWeatherTriangle, &intersectionLTRB))
// 		{					
// 			if (intersectionLTRB.bCollision)
// 			{
// 				pParticle->fCollisionDelta	= 0.0f;
// 				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
// 				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
// 			}
// 		}
// 		if (!intersectionLTRB.bCollision)
// 		{
// 			if (pSector->m_pWater && RpAtomicForAllIntersections(pSector->m_pWater->_pAtom, &intersectionRTLB.Intersection, CollisionWeatherTriangle, &intersectionRTLB))
// 			{					
// 				if (intersectionRTLB.bCollision)
// 				{
// 					pParticle->fCollisionDelta	= 0.0f;
// 					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
// 				}
// 			}
// 		}
// 	}
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
// 	{
// 		intersectionLTRB.fMinDist	= RwRealMAXVAL;
// 		intersectionLTRB.bCollision = FALSE;
// 		intersectionRTLB.fMinDist	= RwRealMAXVAL;
// 		intersectionRTLB.bCollision = FALSE;
// 
// 		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherAtomic, &intersectionLTRB))
// 		{					
// 			if (intersectionLTRB.bCollision)
// 			{
// 				pParticle->fCollisionDelta	= 0.0f;
// 				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
// 				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
// 			}
// 		}
// 		if (!intersectionLTRB.bCollision)
// 		{
// 			if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherAtomic, &intersectionRTLB))
// 			{					
// 				if (intersectionRTLB.bCollision)
// 				{
// 					pParticle->fCollisionDelta	= 0.0f;
// 					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
// 				}
// 			}
// 		}
// 	}
// 
// 	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

	NTL_RPROFILE(TRUE)
		//return TRUE;
}

RwBool CNtlPLWeatherController::CalcParticleCollisionXRayOutdoor(SPLWeatherParticle* pParticle)
{
	NTL_SPROFILE("CNtlPLWeatherController::CalcParticleCollisionXRayOutdoor");

	SWorldIntersect	intersectionLTRB, intersectionRTLB;
	RwV3d			vRight, vUp;
	intersectionLTRB.Intersection.type 				= rpINTERSECTLINE;    
	intersectionLTRB.fMinDist						= RwRealMAXVAL;
	intersectionLTRB.bCollision						= FALSE;

	intersectionRTLB.Intersection.type 				= rpINTERSECTLINE;    
	intersectionRTLB.fMinDist						= RwRealMAXVAL;
	intersectionRTLB.bCollision						= FALSE;

	if (m_pWeatherEmitter->eParticleBillboardType == EPLWEATHER_PARTICLE_BILLBOARD_N)
	{
		vRight	= *RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) * m_pWeatherEmitter->vParticleHalfSize.x;
		vUp		= *RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) * m_pWeatherEmitter->vParticleHalfSize.y;
	}
	else if (m_pWeatherEmitter->eParticleBillboardType == EPLWEATHER_PARTICLE_BILLBOARD_Y)
	{
		vRight	= *RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) * m_pWeatherEmitter->vParticleHalfSize.x;
		vUp		= m_pWeatherEmitter->vParticleBillboard * m_pWeatherEmitter->vParticleHalfSize.y;
	}

	intersectionLTRB.Intersection.t.line.start		= pParticle->vPos;
	intersectionLTRB.Intersection.t.line.end		= pParticle->vPos;

	intersectionRTLB.Intersection.t.line.start		= pParticle->vPos;
	intersectionRTLB.Intersection.t.line.end		= pParticle->vPos;

	intersectionLTRB.Intersection.t.line.start		+= vRight;
	intersectionLTRB.Intersection.t.line.start		+= vUp;
	intersectionLTRB.Intersection.t.line.end		-= vRight;
	intersectionLTRB.Intersection.t.line.end		-= vUp;

	intersectionRTLB.Intersection.t.line.start		-= vRight;
	intersectionRTLB.Intersection.t.line.start		+= vUp;
	intersectionRTLB.Intersection.t.line.end		+= vRight;
	intersectionRTLB.Intersection.t.line.end		-= vUp;

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
	{
		intersectionLTRB.fMinDist	= RwRealMAXVAL;
		intersectionLTRB.bCollision = FALSE;
		intersectionRTLB.fMinDist	= RwRealMAXVAL;
		intersectionRTLB.bCollision = FALSE;
		if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherTerrainOutdoor, &intersectionLTRB))
		{					
			if (intersectionLTRB.bCollision)
			{
				pParticle->fCollisionDelta	= 0.0f;
				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
			}
		}
		if (intersectionLTRB.bCollision)		
		{
			if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherTerrainOutdoor, &intersectionRTLB))		
			{					
				if (intersectionRTLB.bCollision)
				{
					pParticle->fCollisionDelta	= 0.0f;
					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
				}
			}
		}
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
	{					
		intersectionLTRB.fMinDist	= RwRealMAXVAL;
		intersectionLTRB.bCollision = FALSE;
		intersectionRTLB.fMinDist	= RwRealMAXVAL;
		intersectionRTLB.bCollision = FALSE;
	if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherWaterOutdoor, &intersectionLTRB))
		{					
			if (intersectionLTRB.bCollision)
			{
				pParticle->fCollisionDelta	= 0.0f;
				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
			}
		}
		if (!intersectionLTRB.bCollision)
		{
			if (RpWorldForAllWorldSectorIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherWaterOutdoor, &intersectionRTLB))		
			{					
				if (intersectionRTLB.bCollision)
				{
					pParticle->fCollisionDelta	= 0.0f;
					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
				}
			}
		}
	}

	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
	{
		intersectionLTRB.fMinDist	= RwRealMAXVAL;
		intersectionLTRB.bCollision = FALSE;
		intersectionRTLB.fMinDist	= RwRealMAXVAL;
		intersectionRTLB.bCollision = FALSE;

		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherAtomic, &intersectionLTRB))
		{					
			if (intersectionLTRB.bCollision)
			{
				pParticle->fCollisionDelta	= 0.0f;
				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
			}
		}
		if (!intersectionLTRB.bCollision)
		{
			if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherAtomic, &intersectionRTLB))
			{					
				if (intersectionRTLB.bCollision)
				{
					pParticle->fCollisionDelta	= 0.0f;
					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
				}
			}
		}
	}

	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

// 	CNtlWorldFieldManager*	pFieldMgr	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 	CNtlWorldSector*		pSectors	= pFieldMgr->m_pSectors;
// 	RwInt32					iCurSector	= pFieldMgr->GetSectorIdx(pParticle->vPos);
// 	CNtlWorldSector*		pSector		= &pSectors[iCurSector];
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_TERRAIN))
// 	{
// 		intersectionLTRB.fMinDist	= RwRealMAXVAL;
// 		intersectionLTRB.bCollision = FALSE;
// 		intersectionRTLB.fMinDist	= RwRealMAXVAL;
// 		intersectionRTLB.bCollision = FALSE;
// 		if (pSector->m_pAtomic && CollisionWeatherSectorAtomic(pSector, &intersectionLTRB))
// 		{					
// 			if (intersectionLTRB.bCollision)
// 			{
// 				pParticle->fCollisionDelta	= 0.0f;
// 				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
// 				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
// 			}
// 		}
// 		if (!intersectionLTRB.bCollision)
// 		{
// 			if (pSector->m_pAtomic && CollisionWeatherSectorAtomic(pSector, &intersectionRTLB))
// 			{					
// 				if (intersectionRTLB.bCollision)
// 				{
// 					pParticle->fCollisionDelta	= 0.0f;
// 					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
// 				}
// 			}
// 		}
// 	}
// 
// 	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_WATER))
// 	{					
// 		intersectionLTRB.fMinDist	= RwRealMAXVAL;
// 		intersectionLTRB.bCollision = FALSE;
// 		intersectionRTLB.fMinDist	= RwRealMAXVAL;
// 		intersectionRTLB.bCollision = FALSE;
// 		if (pSector->m_pWater && RpAtomicForAllIntersections(pSector->m_pWater->_pAtom, &intersectionLTRB.Intersection, CollisionWeatherTriangle, &intersectionLTRB))
// 		{					
// 			if (intersectionLTRB.bCollision)
// 			{
// 				pParticle->fCollisionDelta	= 0.0f;
// 				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
// 				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
// 			}
// 		}
// 		if (!intersectionLTRB.bCollision)
// 		{
// 			if (pSector->m_pWater && RpAtomicForAllIntersections(pSector->m_pWater->_pAtom, &intersectionRTLB.Intersection, CollisionWeatherTriangle, &intersectionRTLB))
// 			{					
// 				if (intersectionRTLB.bCollision)
// 				{
// 					pParticle->fCollisionDelta	= 0.0f;
// 					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_WATER;
// 				}
// 			}
// 		}
// 	}
// 
//  	if ((m_pWeatherEmitter->eParticleCollisionFlag & EPLWEATHER_PARTICLE_COLLISION_OBJECT))
// 	{
// 		intersectionLTRB.fMinDist	= RwRealMAXVAL;
// 		intersectionLTRB.bCollision = FALSE;
// 		intersectionRTLB.fMinDist	= RwRealMAXVAL;
// 		intersectionRTLB.bCollision = FALSE;
// 
// 		if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionLTRB.Intersection, CollisionWeatherAtomic, &intersectionLTRB))
// 		{					
// 			if (intersectionLTRB.bCollision)
// 			{
// 				pParticle->fCollisionDelta	= 0.0f;
// 				pParticle->vCollisionPoint	= intersectionLTRB.vCollPos;
// 				pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
// 			}
// 		}
// 		if (!intersectionLTRB.bCollision)
// 		{
// 			if (RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersectionRTLB.Intersection, CollisionWeatherAtomic, &intersectionRTLB))
// 			{					
// 				if (intersectionRTLB.bCollision)
// 				{
// 					pParticle->fCollisionDelta	= 0.0f;
// 					pParticle->vCollisionPoint	= intersectionRTLB.vCollPos;
// 					pParticle->eCollisionFlag	= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
// 				}
// 			}
// 		}
// 	}
// 
// 	pParticle->eCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_COMPLETE;

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

SPLWeatherParticle* CNtlPLWeatherController::CreateParticle(RwInt32 iIndex, RwInt32 iNum)
{
	NTL_SPROFILE("CNtlPLWeatherController::CreateParticle");

	SPLWeatherParticle* pParticle = NTL_NEW SPLWeatherParticle;

	pParticle->fTimeCur				= 0.0f;
	pParticle->fTimeEnd				= m_pWeatherEmitter->fParticleLifeTime;
	pParticle->fTimeDelta			= 0.0f;

	pParticle->fTimeMoveCur			= 0.0f;
	pParticle->fTimeMoveEnd			= 0.0f;

	// Collision Data
	pParticle->eCollisionFlag		= EPLWEATHER_PARTICLE_COLLISION_NONE;
	pParticle->fCollisionDelta		= 1.0f;
	pParticle->vCollisionPoint		= CNtlPLGlobal::m_vZeroV3;

	RwReal fClose					= (static_cast<RwReal>((iIndex+1) / iNum) < 0.5f ? 1.0f : 2.0f);
	pParticle->vPos.x				= dPL_WEATHER_RANDOM(m_pWeatherEmitter->vEmitterSize.x * fClose);
	pParticle->vPos.y				= dPL_WEATHER_RANDOM(m_pWeatherEmitter->vEmitterSize.y * fClose);
	pParticle->vPos.z				= dPL_WEATHER_RANDOM(m_pWeatherEmitter->vEmitterSize.z * fClose);
	pParticle->vPos					+= m_vWeatherEmitter;

	pParticle->vVel					= m_pWeatherEmitter->vParticleMotionDir * m_pWeatherEmitter->fParticleMotionVel;

	pParticle->fSplinePower			= (dPL_WEATHER_RANDOM(m_pWeatherEmitter->fParticleSplinePower) > 0 ? m_pWeatherEmitter->fParticleSplinePower : -m_pWeatherEmitter->fParticleSplinePower);
	pParticle->vPosCtrl				= CNtlPLGlobal::m_vZeroV3;
	pParticle->vPosPrev				= CNtlPLGlobal::m_vZeroV3;
	pParticle->vPosNext				= CNtlPLGlobal::m_vZeroV3;
	pParticle->mat					= CNtlPLGlobal::m_matIden;

	if (m_pWeatherEmitter->eParticleMotionType != EPLWEATHER_PARTICLE_MOTION_TSNAP && m_pWeatherEmitter->fParticleCreateTerrainDist > 0.0f)
	{
		RwReal fY = pParticle->vPos.y - GetSceneManager()->GetWorld()->GetWorldSectorHeight(pParticle->vPos);
		if (m_pWeatherEmitter->fParticleCreateTerrainDist > fY)
		{
			//NTL_DELETE(pParticle);
			pParticle->vPos.y += m_pWeatherEmitter->fParticleCreateTerrainDist - fY;
		}
	}

	NTL_RPROFILE(pParticle)
	//return pParticle;
}

SPLWeatherParticle* CNtlPLWeatherController::DeleteParticle(SPLWeatherParticle* pParticle)
{
	NTL_SPROFILE("CNtlPLWeatherController::DeleteParticle");

	NTL_DELETE(pParticle);

	NTL_RPROFILE(pParticle)
	//return pParticle;
}

//////////////////////////////////////////////////////////////////////////
// CNtlPLWeatherControllerPTank
//////////////////////////////////////////////////////////////////////////
CNtlPLWeatherControllerPTank::CNtlPLWeatherControllerPTank()
:m_pAtmPTank(NULL)
,m_pFrmPTank(NULL)
,m_pTexPTank(NULL)
{
}

CNtlPLWeatherControllerPTank::~CNtlPLWeatherControllerPTank()
{
}

RwBool CNtlPLWeatherControllerPTank::Create(CNtlPLWeatherProperty* pWeatherProperty, SPLWeatherEmitter* pWeatherEmitter)
{
	NTL_SPROFILE("CNtlPLWeatherControllerPTank::Create");

 	if (!CNtlPLWeatherController::Create(pWeatherProperty, pWeatherEmitter))
	{		
		NTL_RPROFILE(FALSE)
		//return FALSE;		
	}

	RwInt32 iDataFlag = rpPTANKDFLAGCNSVTX2TEXCOORDS | rpPTANKDFLAGARRAY;

 	if (m_eWeatherUpdateFlag & EPLWEATHER_UPDATE_MATRIX)
	{
		iDataFlag |= rpPTANKDFLAGMATRIX;
	}
	else
	{
		iDataFlag |= rpPTANKDFLAGPOSITION;
	}

	if (m_eWeatherUpdateFlag & EPLWEATHER_UPDATE_COLOR)
	{
		switch (m_pWeatherEmitter->eParticleBlendingType)
		{
		case EPLWEATHER_PARTICLE_BLENDING_N:
			iDataFlag |= rpPTANKDFLAGCOLOR;
			break;
		case EPLWEATHER_PARTICLE_BLENDING_Y:
			iDataFlag |= rpPTANKDFLAGVTXCOLOR;
			break;
		}
	}	

	m_pAtmPTank = RpPTankAtomicCreate(pWeatherEmitter->iParticleRenderMax, iDataFlag, 0);
	m_pFrmPTank = RwFrameCreate(); RwMatrixSetIdentity(RwFrameGetMatrix(m_pFrmPTank));
	m_pTexPTank = CNtlPLResourceManager::GetInstance()->LoadTexture(m_pWeatherEmitter->strParticleTextureName.c_str(), pWeatherProperty->GetTexturePath());
	DBO_ASSERT(m_pTexPTank, "Texture load failed.");

	RpAtomicSetFrame(m_pAtmPTank, m_pFrmPTank);
	RpPTankAtomicSetActiveParticlesCount(m_pAtmPTank, 0);

	// UV
	RwTexCoords uv[2] = { { 0.0f, 0.0f } , { 1.0f, 1.0f } };
	RpPTankAtomicSetConstantVtx2TexCoords(m_pAtmPTank, uv);	

	// SIZE
	RpPTankAtomicSetConstantSize(m_pAtmPTank, &m_pWeatherEmitter->vParticleSize);

	// COLOR
	if (!(m_eWeatherUpdateFlag & EPLWEATHER_UPDATE_COLOR))
	{
		RpPTankAtomicSetConstantColor(m_pAtmPTank, &m_pWeatherEmitter->clrParticleStart);
	}

	RpPTankAtomicSetTexture(m_pAtmPTank, m_pTexPTank);
	RpPTankAtomicSetVertexAlpha(m_pAtmPTank, TRUE);
	RpPTankAtomicSetBlendModes(m_pAtmPTank, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);

	NTL_RPROFILE(TRUE)
	//return TRUE;
}

void CNtlPLWeatherControllerPTank::Destroy()
{
	NTL_SPROFILE("CNtlPLWeatherControllerPTank::Destroy");

	CNtlPLWeatherController::Destroy();

	if (m_pAtmPTank)
	{
		if (RpAtomicGetFrame(m_pAtmPTank))
		{
			RpAtomicSetFrame(m_pAtmPTank, NULL);
		}
		RpAtomicDestroy(m_pAtmPTank);
		m_pAtmPTank = NULL;
	}

	if (m_pFrmPTank)
	{
		RwFrameDestroy(m_pFrmPTank);
		m_pFrmPTank = NULL;
	}

	if (m_pTexPTank)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pTexPTank);
		m_pTexPTank = NULL;
	}

	NTL_EPROFILE();
}

RwBool CNtlPLWeatherControllerPTank::Update(RwReal fElapsed, RwReal fFade)
{
	NTL_SPROFILE("CNtlPLWeatherControllerPTank::Update");

	if (CNtlPLWeatherController::Update(fElapsed, fFade))
	{
		*RwMatrixGetPos(RwFrameGetMatrix(m_pFrmPTank)) = m_vWeatherEmitter;
		RwMatrixUpdate(RwFrameGetMatrix(m_pFrmPTank));
		RwFrameUpdateObjects(m_pFrmPTank);

		RwMatrix*	pmatCamera		= RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
		RwInt32		iNumParticle	= 0;
		if (m_listParticle.size())
		{
			PARTICLE_LIST_IT	it				= m_listParticle.begin();
			PARTICLE_LIST_IT	itEnd			= m_listParticle.end();

			RpPTankLockStruct sLockPos, sLockMat, sLockClr, sLockVClr;
			RpPTankAtomicLock(m_pAtmPTank, &sLockPos, rpPTANKLFLAGPOSITION, rpPTANKLOCKWRITE);
			RpPTankAtomicLock(m_pAtmPTank, &sLockMat, rpPTANKLFLAGMATRIX, rpPTANKLOCKWRITE);
			RpPTankAtomicLock(m_pAtmPTank, &sLockClr, rpPTANKLFLAGCOLOR, rpPTANKLOCKWRITE);
			RpPTankAtomicLock(m_pAtmPTank, &sLockVClr, rpPTANKLFLAGVTXCOLOR, rpPTANKLOCKWRITE);			

			do 
			{
				SPLWeatherParticle* pParticle = *it;
				if (pParticle->fDist2dSquared >= m_pWeatherEmitter->fParticleFrustumLengthSquared)
				{
					++it;
					continue;
				}

				// PTank Update
				// POSITION
				if (sLockPos.data)
				{
					*(RwV3d*)sLockPos.data	 = pParticle->vPos;
					sLockPos.data			+= sLockPos.stride;
				}

				// MATRIX
				if (sLockMat.data)
				{
					RwMatrixCopy((RwMatrix*)sLockMat.data, &pParticle->mat);
					sLockMat.data += sLockMat.stride;
				}

				// COLOR
				if (sLockClr.data)
				{
					*(RwRGBA*)sLockClr.data	 = pParticle->clr[0];
					sLockClr.data			+= sLockClr.stride;
				}
				else if (sLockVClr.data)
				{
					((RwRGBA*)sLockVClr.data)[0] = pParticle->clr[0];
					((RwRGBA*)sLockVClr.data)[1] = pParticle->clr[1];
					((RwRGBA*)sLockVClr.data)[2] = pParticle->clr[2];
					((RwRGBA*)sLockVClr.data)[3] = pParticle->clr[3];
					sLockVClr.data				+= sLockVClr.stride;
				}

				++iNumParticle;
				++it;
			} while (it != itEnd && iNumParticle < m_pWeatherEmitter->iParticleRenderMax);	

			RpPTankAtomicUnlock(m_pAtmPTank);
		}

 		RpPTankAtomicSetActiveParticlesCount(m_pAtmPTank, iNumParticle);

		NTL_RPROFILE(TRUE)
		//return TRUE;		
	}
	NTL_RPROFILE(FALSE)
	//return FALSE;
}


RwBool CNtlPLWeatherControllerPTank::Render()
{
	NTL_SPROFILE("CNtlPLWeatherControllerPTank::Render");

	BeginWeatherPTankRenderState((m_pWeatherEmitter->eParticleRenderStateFlag & EPLWEATHER_PARTICLE_RENDER_STATE_ZTEST),
								(m_pWeatherEmitter->eParticleRenderStateFlag & EPLWEATHER_PARTICLE_RENDER_STATE_FOG),
								(m_pWeatherEmitter->eParticleRenderStateFlag & EPLWEATHER_PARTICLE_RENDER_STATE_LIGHT));

	RpAtomicRender(m_pAtmPTank);

	EndWeatherPTankRenderState((m_pWeatherEmitter->eParticleRenderStateFlag & EPLWEATHER_PARTICLE_RENDER_STATE_ZTEST),
							(m_pWeatherEmitter->eParticleRenderStateFlag & EPLWEATHER_PARTICLE_RENDER_STATE_FOG),
							(m_pWeatherEmitter->eParticleRenderStateFlag & EPLWEATHER_PARTICLE_RENDER_STATE_LIGHT));

	NTL_RPROFILE(TRUE)
	//return TRUE;
}