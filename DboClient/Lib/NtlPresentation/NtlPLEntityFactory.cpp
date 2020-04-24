#include "precomp_ntlpresentation.h"
#include "NtlPLEntityFactory.h"
#include "NtlPLObject.h"
#include "NtlPLSkySphere.h"
#include "NtlPLSkyDragon.h"
#include "NtlPLFog.h"
#include "NtlPLCharacter.h"
#include "NtlPLLight.h"
#include "NtlPLItem.h"
#include "NtlPLPlayerName.h"
#include "NtlPLDamageBox.h"
#include "NtlDebug.h"
#include "NtlPLHelpers.h"
#include "NtlPLWater.h"
#include "NtlPLDummyWorld.h"
#include "NtlPLDecal.h"
#include "NtlPLWorldDecal.h"
#include "NtlPLSound.h"
#include "Ntlinstanceeffect.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLSoftSpot.h"
#include "NtlPLBloom.h"
#include "NtlPLShoreLine.h"
#include "NtlPLSun.h"
#include "NtlPLPlanet.h"
#include "NtlPLPlant.h"
#include "NtlPLWorldLight_Base.h"
#include "NtlPLWorldLight_Sphere.h"
#include "NtlPLWorldLight_Box.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"
#include "NtlPLWeather.h"
#include "NtlPLHeatHaze.h"
#include "NtlPLLightObject.h"
#include "NtlPLGameProperty.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLDojo.h"

CNtlPLEntity* CNtlPLEntityFactory::CreateEntity(ENtlPLEntityType eType, const SPLEntityCreateParam* _pParam)
{
	NTL_FUNCTION("CNtlPLEntityFactory::CreateEntity");

	NTL_ASSERT(eType != PLENTITY_INVALID_TYPE, "entity type invalid.");

	CNtlPLEntity *pEntity = NULL;

	switch(eType)
	{
	case PLENTITY_OBJECT:
		pEntity = NTL_NEW CNtlPLObject;
		break;
	case PLENTITY_CHARACTER:
		pEntity = NTL_NEW CNtlPLCharacter;
		break;
	case PLENTITY_EFFECT:
		pEntity = NTL_NEW CNtlInstanceEffect;
		break;
	case PLENTITY_WORLD:
		pEntity = NTL_NEW CNtlPLWorldEntity;
		break;
	case PLENTITY_DUMMY_WORLD:
		pEntity = NTL_NEW CNtlPLDummyWorld;
		break;
	case PLENTITY_WATER:
		pEntity = NTL_NEW CNtlPLWater;
		break;
	case PLENTITY_SOUND:
		pEntity = NTL_NEW CNtlPLSound;
		break;
	case PLENTITY_SOUND_BGM:
		pEntity = NTL_NEW CNtlPLSoundBGM;
		break;
	case PLENTITY_SKY:
		{
			DBO_ASSERT(_pParam && _pParam->_pUserData, "createparam error.");

			eSKY_TYPE_INDEX* pSkyTypeIdx = static_cast<eSKY_TYPE_INDEX*>(_pParam->_pUserData);

			if(*pSkyTypeIdx == eSTI_HEMISPHERE)
			{
				pEntity = NTL_NEW CNtlPLSky;
			}
			else if(*pSkyTypeIdx == eSTI_SPHERE)
			{
				pEntity = NTL_NEW CNtlPLSkySphere;
			}
			else if(*pSkyTypeIdx == eSTI_DRAGONE)
			{
				pEntity = NTL_NEW CNtlPLSkyDragon;
			}
			else
			{
				DBO_WARNING_MESSAGE("Sky type: " << static_cast<int*>(_pParam->_pUserData) << "dont exist");
			}

			break;
		}
	case PLENTITY_FOG:
		pEntity = NTL_NEW CNtlPLFog;
		break;
	case PLENTITY_LIGHT:
		pEntity = NTL_NEW CNtlPLLight;
		break;
	case PLENTITY_SHORELINE:
		pEntity = NTL_NEW CNtlPLShoreLine;
		break;
		// CZ-SS
// 	case PLENTITY_SPOT_LIGHT:
// 		pEntity = NTL_NEW CNtlPLSpot;
// 		break;
	case PLENTITY_ITEM:
		pEntity = NTL_NEW CNtlPLItem;
		break;
	case PLENTITY_PLAYER_NAME:
		pEntity = NTL_NEW CNtlPLPlayerName;
		break;
	case PLENTITY_DAMAGE_BOX:
		pEntity = NTL_NEW CNtlPLDamageBox;
		break;
	case PLENTITY_DECAL:
		pEntity = NTL_NEW CNtlPLWorldDecal;
		break;
	case PLENTITY_BLOOM:
		pEntity = NTL_NEW CNtlPLBloom;
		break;
	case PLENTITY_SUN:
		{
			DBO_ASSERT(_pParam && _pParam->_pUserData, "createparam error.");

			eSKY_FLOATING_BILLBOARD_TYPE* pSkyFloatingBlillboardType = static_cast<eSKY_FLOATING_BILLBOARD_TYPE*>(_pParam->_pUserData);

			if(*pSkyFloatingBlillboardType == eSFBT_SUN)
			{
				pEntity = NTL_NEW CNtlPLSun;
			}
			else if(*pSkyFloatingBlillboardType == eSFBT_PLANET)
			{
				pEntity = NTL_NEW CNtlPLPlanet;
			}

			break;
		}
	case PLENTITY_PLANT:
		pEntity = NTL_NEW CNtlPLPlant;
		break;
	case PLENTITY_WORLD_LIGHT:
		{
			DBO_ASSERT(_pParam && _pParam->_pUserData, "createparam error.");

			EPLWorldLightType* pWorldLightType = static_cast<EPLWorldLightType*>(_pParam->_pUserData);

			if (*pWorldLightType == EPLWORLDLIGHT_SPHERE)
			{
				pEntity = NTL_NEW CNtlPLWorldLight_Sphere;
			}
			else if (*pWorldLightType == EPLWORLDLIGHT_BOX)
			{
				pEntity = NTL_NEW CNtlPLWorldLight_Box;
			}
		}		
		break;
	case PLENTITY_OCCLUDER:
		{
			DBO_ASSERT(_pParam && _pParam->_pUserData, "createparam error.");

			EPLOccluderType* pOccluderType = static_cast<EPLOccluderType*>(_pParam->_pUserData);

			if (*pOccluderType == EPLOCCLUDER_QUAD)
			{
				pEntity = NTL_NEW CNtlPLOccluder_Quad;
			}
		}
		break;
	case PLENTITY_WEATHER:
		pEntity = NTL_NEW CNtlPLWeather;
		break;
	case PLENTITY_HEATHAZE:
		pEntity = NTL_NEW CNtlPLHeatHazeObject;
		break;
	case PLENTITY_LIGHT_OBJECT:
		pEntity = NTL_NEW CNtlPLLightObject;
		break;
	case PLENTITY_GAME_PROPERTY:
		pEntity = NTL_NEW CNtlPLGameProperty;
		break;
	case PLENTITY_DECAL_VOLUME:
		pEntity = NTL_NEW CNtlPLDecalVolume;
		break;
	case PLENTITY_DOJO:
		pEntity = NTL_NEW CNtlPLDojo;
		break;
	}

	if(pEntity)
		pEntity->SetClassType(eType);

	NTL_RETURN(pEntity);
}

void CNtlPLEntityFactory::DeleteEntity(CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLEntityFactory::DeleteEntity");

	NTL_PRE(pEntity);

	ENtlPLEntityType eType = pEntity->GetClassType(); 
	switch(eType)
	{
	case PLENTITY_OBJECT:
		{
			CNtlPLObject *pObject = static_cast<CNtlPLObject*>(pEntity);
			NTL_DELETE( pObject );
			break;
		}

	case PLENTITY_CHARACTER:
		{
			CNtlPLCharacter *pCharacter = static_cast<CNtlPLCharacter*>(pEntity);
			NTL_DELETE( pCharacter );
			break;
		}
		// by agebreak
	case PLENTITY_EFFECT:
		{
			CNtlInstanceEffect *pEffect = static_cast<CNtlInstanceEffect*>(pEntity);
			NTL_DELETE( pEffect );
			break;
		}

	case PLENTITY_LIGHT:
		{
			CNtlPLLight * pLight = static_cast<CNtlPLLight*>(pEntity);
			NTL_DELETE( pLight );
			break;
		}

		// CZ-SS
// 	case PLENTITY_SPOT_LIGHT:
// 		{
// 			CNtlPLSpot * pLight = static_cast<CNtlPLSpot*>(pEntity);
//			NTL_DELETE( pLight );
// 			break;
// 		}

		// woody1019
	case PLENTITY_SHORELINE:
		{
			CNtlPLShoreLine *pNtlPLShoreLine = static_cast<CNtlPLShoreLine*>(pEntity);
			NTL_DELETE( pNtlPLShoreLine );
			break;
		}
	case PLENTITY_WORLD:
		{
			CNtlPLWorldEntity *pWorld = static_cast<CNtlPLWorldEntity*>(pEntity);
			NTL_DELETE( pWorld );
			break;
		}
	case PLENTITY_DUMMY_WORLD:
		{
			CNtlPLDummyWorld *pDummyWorld = static_cast<CNtlPLDummyWorld*>(pEntity);
			NTL_DELETE( pDummyWorld );
			break;
		}

	case PLENTITY_WATER:
		{
			CNtlPLWater *pWater = static_cast<CNtlPLWater*>(pEntity);
			NTL_DELETE( pWater );
			break;
		}

	case PLENTITY_SKY:
		{
			CNtlPLSky *pSky = static_cast<CNtlPLSky*>(pEntity);
			NTL_DELETE( pSky );
			break;
		}

	case PLENTITY_FOG:
		{
			CNtlPLFog *pFog = static_cast<CNtlPLFog*>(pEntity);
			NTL_DELETE( pFog );
			break;
		}

	case PLENTITY_SOUND:
		{
			CNtlPLSound *pSound = static_cast<CNtlPLSound*>(pEntity);
			NTL_DELETE( pSound );
			break;
		}
	case PLENTITY_SOUND_BGM:
		{
			CNtlPLSoundBGM *pSoundBGM = static_cast<CNtlPLSoundBGM*>(pEntity);
			NTL_DELETE( pSoundBGM );
			break;
		}
	case PLENTITY_ITEM:
		{
			CNtlPLItem *pItem = static_cast<CNtlPLItem*>(pEntity);
			NTL_DELETE( pItem );
			break;
		}

	case PLENTITY_PLAYER_NAME:
		{
			CNtlPLPlayerName *pPlayerName = static_cast<CNtlPLPlayerName*>(pEntity);
			NTL_DELETE( pPlayerName );
			break;
		}

	case PLENTITY_DAMAGE_BOX:
		{
			CNtlPLDamageBox *pDamageBox = static_cast<CNtlPLDamageBox*>(pEntity);
			NTL_DELETE( pDamageBox );
			break;
		}
	case PLENTITY_DECAL:
		{
			CNtlPLWorldDecal *pDecal = static_cast<CNtlPLWorldDecal*>(pEntity);
			NTL_DELETE( pDecal );
			break;
		}
	case PLENTITY_BLOOM:
		{
			CNtlPLBloom *pBloom = static_cast<CNtlPLBloom*>(pEntity);
			NTL_DELETE( pBloom );
			break;
		}
	case PLENTITY_SUN:
		{
			CNtlPLSun* pSun = static_cast<CNtlPLSun*>(pEntity);
			NTL_DELETE( pSun );
			break;
		}
	case PLENTITY_PLANT:
		{
			CNtlPLPlant* pPlant = static_cast<CNtlPLPlant*>(pEntity);
			NTL_DELETE( pPlant );
			break;
		}
	case PLENTITY_WORLD_LIGHT:
		{
			CNtlPLWorldLight_Base* pWorldLight = static_cast<CNtlPLWorldLight_Base*>(pEntity);
			NTL_DELETE( pWorldLight );
			break;
		}
	case PLENTITY_OCCLUDER:
		{
			CNtlPLOccluder_Base* pOccluder = static_cast<CNtlPLOccluder_Base*>(pEntity);
			NTL_DELETE( pOccluder );
			break;
		}
	case PLENTITY_WEATHER:
		{
			CNtlPLWeather* pWeather = static_cast<CNtlPLWeather*>(pEntity);
			NTL_DELETE( pWeather );
			break;
		}
	case PLENTITY_HEATHAZE:
		{
			CNtlPLHeatHazeObject* pHeatHaze = static_cast<CNtlPLHeatHazeObject*>(pEntity);
			NTL_DELETE( pHeatHaze );
			break;
		}
	case PLENTITY_LIGHT_OBJECT:
		{
			CNtlPLLightObject* pLightObject = static_cast<CNtlPLLightObject*>(pEntity);
			NTL_DELETE( pLightObject );
			break;
		}
	case PLENTITY_GAME_PROPERTY:
		{
			CNtlPLGameProperty* pGameProperty = static_cast<CNtlPLGameProperty*>(pEntity);
			NTL_DELETE( pGameProperty );
			break;
		}
	case PLENTITY_DECAL_VOLUME:
		{
			CNtlPLDecalVolume* pDecalVolume = static_cast<CNtlPLDecalVolume*>(pEntity);
			NTL_DELETE( pDecalVolume );
			break;
		}
	case PLENTITY_DOJO:
		{
			CNtlPLDojo* pDojo = static_cast<CNtlPLDojo*>(pEntity);
			NTL_DELETE( pDojo );
			break;
		}
	}

	DBO_ASSERT(eType != PLENTITY_INVALID_TYPE, "entity type invalid.");

	NTL_RETURNVOID();
}
