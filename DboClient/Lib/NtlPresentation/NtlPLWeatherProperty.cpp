#include "precomp_ntlpresentation.h"

#include "NtlStringUtil.h"

#include "NtlDebug.h"
#include "NtlXMLDoc.h"

#include "NtlPLWeatherProperty.h" 


CNtlPLWeatherProperty::CNtlPLWeatherProperty()
{
}

CNtlPLWeatherProperty::~CNtlPLWeatherProperty()
{
}

RwBool CNtlPLWeatherProperty::Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLWeatherProperty::Save");

	NTL_RETURN(TRUE);	
}

RwBool CNtlPLWeatherProperty::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLWeatherProperty::Load");

	NTL_PRE(pDoc);
	NTL_PRE(pNode);

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNodeList*	pNodeList					= NULL;
	IXMLDOMNode*		pNodeChild					= NULL;

	SPLWeatherEmitter	sEmitter;
	RwInt32				iIdx;

	// INIT : TYPE & FLAG
	sEmitter.eParticleRenderStateFlag	= sEmitter.eParticleBillboardType
										= sEmitter.eParticleCreateEventType
										= sEmitter.eParticleCollisionFlag
										= sEmitter.eParticleUseFlag
										= sEmitter.eParticleMotionType
										= sEmitter.eEmitterPlaceType
										= sEmitter.eParticleUseFlag
										= sEmitter.eParticleBlendingType
										= NULL;

	// INDEX
	if(!pDoc->GetTextWithAttributeName(pNode, "index", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	iIdx = ::atoi(chBuffer);

	// NAME
	if(!pDoc->GetTextWithAttributeName(pNode, "name", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.strName = chBuffer;

	// EMITTER
	if (FAILED(pNode->selectSingleNode(L"emitter", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "place_type", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (!strcmp(chBuffer, "camera_pos"))
	{
		sEmitter.eEmitterPlaceType = EPLWEATHER_EMITTER_PLACE_CAMERA_POS;
	}
	else if (!strcmp(chBuffer, "camera_at"))
	{
		sEmitter.eEmitterPlaceType = EPLWEATHER_EMITTER_PLACE_CAMERA_AT;
	}
	else
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "x", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vEmitterPos.x = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "y", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vEmitterPos.y = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "z", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vEmitterPos.z = static_cast<RwReal>(atof(chBuffer));


	if(!pDoc->GetTextWithAttributeName(pNodeChild, "cx", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vEmitterSize.x = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "cy", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vEmitterSize.y = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "cz", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vEmitterSize.z = static_cast<RwReal>(atof(chBuffer));

	sEmitter.fEmitterRadius = RwV3dLength(&sEmitter.vEmitterSize);

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// PARTICLE
	if (FAILED(pNode->selectSingleNode(L"particle", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "texture", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.strParticleTextureName = chBuffer;

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "cx", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleSize.x = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "cy", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleSize.y = static_cast<RwReal>(atof(chBuffer));
	
	sEmitter.vParticleHalfSize.x = sEmitter.vParticleSize.x * 0.5f;
	sEmitter.vParticleHalfSize.y = sEmitter.vParticleSize.y * 0.5f;

	sEmitter.fParticleRadius = RwV2dLength(&sEmitter.vParticleSize);

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}
	
	// MOTION
	if (FAILED(pNode->selectSingleNode(L"motion", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "motion_type", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (!strcmp(chBuffer, "line"))
	{
		sEmitter.eParticleMotionType = EPLWEATHER_PARTICLE_MOTION_LINE;
	}
	else if (!strcmp(chBuffer, "spline"))
	{
		sEmitter.eParticleMotionType = EPLWEATHER_PARTICLE_MOTION_SPLINE;
	}
	else if (!strcmp(chBuffer, "tsnap"))
	{
		sEmitter.eParticleMotionType = EPLWEATHER_PARTICLE_MOTION_TSNAP;
	}
	else if (!strcmp(chBuffer, "static"))
	{
		sEmitter.eParticleMotionType = EPLWEATHER_PARTICLE_MOTION_STATIC;
	}
	else
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "dir_x", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleMotionDir.x = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "dir_y", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleMotionDir.y = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "dir_z", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleMotionDir.z = static_cast<RwReal>(atof(chBuffer));

	RwV3dNormalize(&sEmitter.vParticleMotionDir, &sEmitter.vParticleMotionDir);

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "velocity", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleMotionVel = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "spline_power", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleSplinePower = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "spline_time", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleSplineTime = static_cast<RwReal>(atof(chBuffer));

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// COLOR
	if (FAILED(pNode->selectSingleNode(L"color", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "blend_type", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (!strcmp(chBuffer, "n"))
	{
		sEmitter.eParticleBlendingType = EPLWEATHER_PARTICLE_BLENDING_N;
	}
	else if (!strcmp(chBuffer, "y"))
	{
		sEmitter.eParticleBlendingType = EPLWEATHER_PARTICLE_BLENDING_Y;
	}
	else
	{
		NTL_RETURN(FALSE);
	}


	if(!pDoc->GetTextWithAttributeName(pNodeChild, "s_a", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleStart.alpha = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "s_r", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleStart.red = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "s_g", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleStart.green = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "s_b", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleStart.blue = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "c_a", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleCenter.alpha = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "c_r", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleCenter.red = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "c_g", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleCenter.green = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "c_b", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleCenter.blue = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "e_a", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleEnd.alpha = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "e_r", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleEnd.red = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "e_g", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleEnd.green = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "e_b", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.clrParticleEnd.blue = static_cast<RwUInt8>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "s_time", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleSColorTime = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "c_time", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleCColorTime = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "e_time", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleEColorTime = static_cast<RwReal>(atof(chBuffer));


	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}


	// CREATE
	if (FAILED(pNode->selectSingleNode(L"create", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "time", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleCreateTime = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "time_gap", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleCreateTimeGap = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "num", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.iParticleCreateNum = static_cast<RwInt32>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "num_gap", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.iParticleCreateNumGap = static_cast<RwInt32>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "max", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.iParticleCreateMax = static_cast<RwInt32>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "effect", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	std::string strParticleCreateEffect = chBuffer;
	if (strParticleCreateEffect.length())
	{
		StringTokenize(sEmitter.vecParticleCreateEffect, strParticleCreateEffect, std::string(","));
		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_EFFECT_CREATE;
	}

// 	if(!pDoc->GetTextWithAttributeName(pNodeChild, "sound", chBuffer, NTL_MAX_DIR_PATH))
// 	{
// 		NTL_RETURN(FALSE);
// 	}
// 
// 	strcpy_s(sEmitter.acParticleCreateSound, dPL_WEATHER_CHAR_BUFFER_SMALL, chBuffer);
// 	if (strlen(sEmitter.acParticleCreateSound))
// 	{
// 		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_CREATE_SOUND;
// 	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "event", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (!strcmp(chBuffer, "lightning"))
	{
		sEmitter.eParticleCreateEventType = EPLWATHER_PARTICLE_EVENT_LIGHTNING;
	}
	else if (!strcmp(chBuffer, "lightning2"))
	{
		sEmitter.eParticleCreateEventType = EPLWATHER_PARTICLE_EVENT_LIGHTNING2;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "terrain_dist", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleCreateTerrainDist = static_cast<RwReal>(atof(chBuffer));

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// DESTROY
	if (FAILED(pNode->selectSingleNode(L"destroy", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "effect", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.strParticleDestroyEffect = chBuffer;
	if (sEmitter.strParticleDestroyEffect.length())
	{
		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_EFFECT_DESTROY;
	}

// 	if(!pDoc->GetTextWithAttributeName(pNodeChild, "sound", chBuffer, NTL_MAX_DIR_PATH))
// 	{
// 		NTL_RETURN(FALSE);
// 	}
// 
// 	strcpy_s(sEmitter.acParticleDestroySound, dPL_WEATHER_CHAR_BUFFER_SMALL, chBuffer);
// 	if (strlen(sEmitter.acParticleDestroySound))
// 	{
// 		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_DESTROY_SOUND;
// 	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "lifetime", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleLifeTime = static_cast<RwReal>(atof(chBuffer));

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// COLLISION
	if (FAILED(pNode->selectSingleNode(L"collision", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "linescale", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fCollisionLineScale = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "effect", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.strParticleCollisionEffect	= chBuffer;

	if (sEmitter.strParticleCollisionEffect.length())
	{
		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_EFFECT_COLLISION;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "effect_water", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.strParticleCollisionEffectWater = chBuffer;

	if (sEmitter.strParticleCollisionEffectWater.length())
	{
		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_EFFECT_COLLISION_WATER;
	}

// 	if(!pDoc->GetTextWithAttributeName(pNodeChild, "sound", chBuffer, NTL_MAX_DIR_PATH))
// 	{
// 		NTL_RETURN(FALSE);
// 	}
// 
// 	strcpy_s(sEmitter.acParticleCollisionSound, dPL_WEATHER_CHAR_BUFFER_SMALL, chBuffer);
// 	if (strlen(sEmitter.acParticleCollisionSound))
// 	{
// 		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_COLLISION_SOUND;
// 	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "terrain", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_TERRAIN;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "water", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_WATER;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "object", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_OBJECT;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "destroy", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleCollisionFlag |= EPLWEATHER_PARTICLE_COLLISION_DESTORY;
	}

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// FRUSTUM
	if (FAILED(pNode->selectSingleNode(L"frustum", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "render_length", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	
	sEmitter.fParticleFrustumLengthSquared = static_cast<RwReal>(atof(chBuffer));
	sEmitter.fParticleFrustumLengthSquared *= sEmitter.fParticleFrustumLengthSquared;

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "effect_render_length", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fEffectFrustumLengthSquared = static_cast<RwReal>(atof(chBuffer));
	sEmitter.fEffectFrustumLengthSquared *= sEmitter.fEffectFrustumLengthSquared;

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "render_max", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.iParticleRenderMax = static_cast<RwInt32>(atoi(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "update_fast", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleFrustumFast = static_cast<RwReal>(atof(chBuffer));

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// BILLBOARD
	if (FAILED(pNode->selectSingleNode(L"billboard", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "billboard_type", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (!strcmp(chBuffer, "n"))
	{
		sEmitter.eParticleBillboardType = EPLWEATHER_PARTICLE_BILLBOARD_N;
	}
	else if (!strcmp(chBuffer, "y"))
	{
		sEmitter.eParticleBillboardType = EPLWEATHER_PARTICLE_BILLBOARD_Y;
	}
	else
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "v_x", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleBillboard.x = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "v_y", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleBillboard.y = static_cast<RwReal>(atof(chBuffer));

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "v_z", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.vParticleBillboard.z = static_cast<RwReal>(atof(chBuffer));
	
	RwV3dNormalize(&sEmitter.vParticleBillboard, &sEmitter.vParticleBillboard);

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// RENDERSTATE
	if (FAILED(pNode->selectSingleNode(L"renderstate", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "fog", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleRenderStateFlag |= EPLWEATHER_PARTICLE_RENDER_STATE_FOG;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "light", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleRenderStateFlag |= EPLWEATHER_PARTICLE_RENDER_STATE_LIGHT;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "ztest", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	if (atoi(chBuffer))
	{
		sEmitter.eParticleRenderStateFlag |= EPLWEATHER_PARTICLE_RENDER_STATE_ZTEST;
	}

	if (pNodeChild)
	{
		pNodeChild->Release();
		pNodeChild = NULL;
	}

	// SOUND
	if (FAILED(pNode->selectSingleNode(L"sound", &pNodeChild)))
	{
		NTL_RETURN(FALSE);
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "global_sound", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	
	sEmitter.strGlobalSound = chBuffer;
	if (sEmitter.strGlobalSound.length())
	{
		sEmitter.strGlobalSound = m_strSoundPath + sEmitter.strGlobalSound;
		sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_SOUND_GLOBAL;
	}

	if(!pDoc->GetTextWithAttributeName(pNodeChild, "particle_sound_time", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	sEmitter.fParticleSoundTime = static_cast<RwReal>(atof(chBuffer));

	if (pNodeChild)
	{
		pNodeChild->get_childNodes(&pNodeList);

		long lNum;
		pNodeList->get_length(&lNum);
		for (long i = 0; i < lNum; ++i)
		{
			IXMLDOMNode* pNodeTemp = NULL;
			pNodeList->get_item(i, &pNodeTemp);

			BSTR bstr = NULL;
			if (FAILED(pNodeTemp->get_nodeName(&bstr)) || wcscmp(bstr, L"particle_sound"))
			{
				SysFreeString(bstr);
				pNodeTemp->Release();
				pNodeTemp = NULL;
				continue;
			}
			SysFreeString(bstr);

			SPLWeatherEmitter::PAIR_SOUND pairSound;
			if(!pDoc->GetTextWithAttributeName(pNodeTemp, "name", chBuffer, NTL_MAX_DIR_PATH))
			{
				NTL_RETURN(FALSE);
			}

			pairSound.second = chBuffer;
			if (pairSound.second.length())
			{
				pairSound.second = m_strSoundPath + pairSound.second;

				if(!pDoc->GetTextWithAttributeName(pNodeTemp, "particle", chBuffer, NTL_MAX_DIR_PATH))
				{
					NTL_RETURN(FALSE);
				}

				pairSound.first = atoi(chBuffer);

				sEmitter.vecParticleSound.push_back(pairSound);

				sEmitter.eParticleUseFlag |= EPLWEATHER_PARTICLE_USE_SOUND_PARTICLE;
			}

			if (pNodeTemp)
			{
				pNodeTemp->Release();
				pNodeTemp = NULL;
			}
		}

		if (pNodeList)
		{
			pNodeList->Release();
			pNodeList = NULL;
		}

		if (pNodeChild)
		{
			pNodeChild->Release();
			pNodeChild = NULL;
		}
	}

	m_mapEmitter[iIdx] = sEmitter;

	NTL_RETURN(TRUE);
}

/*RwBool CNtlPLWeatherProperty::Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode)
{
	NTL_FUNCTION("CNtlPLWeatherProperty::Load");

	NTL_PRE(pDoc);
	NTL_PRE(pNode);

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNodeList*	pNodeList					= NULL;
	IXMLDOMNode*		pNodeChild					= NULL;
	
	SPLWeatherEmitter	sEmitter;
	RwInt32				iIdx;

	// INDEX
	if(!pDoc->GetTextWithAttributeName(pNode, "index", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	iIdx = ::atoi(chBuffer);

	// NAME
	if(!pDoc->GetTextWithAttributeName(pNode, "name", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	strcpy_s(sEmitter.acName, dPL_WEATHER_CHAR_BUFFER_SMALL, chBuffer);
	
	if (FAILED(pNode->selectNodes(L"emitter", &pNodeList)))
	{
		NTL_RETURN(FALSE);
	}

	long lNum;
	pNodeList->get_length(&lNum);
	for (long i = 0; i < lNum; ++i)
	{
		if (SUCCEEDED(pNodeList->get_item(i, &pNodeChild)))
		{
			if (LoadEmitter(pDoc, pNodeChild, &sEmitter))
			{
				if (pNodeChild)
				{
					pNodeChild->Release();
					pNodeChild = NULL;
				}
				continue;
			}
		}	

		if (pNodeList)
		{
			pNodeList->Release();
			pNodeList = NULL;
		}

		NTL_RETURN(FALSE);
	}

	if (FAILED(pNode->selectNodes(L"particle", &pNodeList)))
	{
		NTL_RETURN(FALSE);
	}

	pNodeList->get_length(&lNum);
	for (long i = 0; i < lNum; ++i)
	{
		if (SUCCEEDED(pNodeList->get_item(i, &pNodeChild)))
		{
			if (LoadParticle(pDoc, pNodeChild, &sEmitter))
			{
				if (pNodeChild)
				{
					pNodeChild->Release();
					pNodeChild = NULL;
				}
				continue;
			}
		}	

		if (pNodeList)
		{
			pNodeList->Release();
			pNodeList = NULL;
		}

		NTL_RETURN(FALSE);
	}

	if (pNodeList)
	{
		pNodeList->Release();
		pNodeList = NULL;
	}

	m_mapEmitter[iIdx] = sEmitter;

	NTL_RETURN(TRUE);
}

RwBool CNtlPLWeatherProperty::LoadEmitter(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode, SPLWeatherEmitter* pEmitter)
{
	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNode*		pNodeChild					= NULL;
	IXMLDOMNodeList*	pNodeChildList				= NULL;

	// POSITION
	if (SUCCEEDED(pNode->selectSingleNode(L"position", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "type", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		if (strlen(chBuffer)  >= 3 && (chBuffer[0] == 'p' || chBuffer[0] == 'P') && (chBuffer[1] == 'o' || chBuffer[1] == 'O') && (chBuffer[2] == 's' || chBuffer[2] == 'S'))
		{
			pEmitter->eEmitterPlaceType = EPLWEATHER_EMITTER_PLACE_POS;
		}
		else// if (strlen(chBuffer)  >= 2 && (chBuffer[0] == 'a' || chBuffer[0] == 'A') && (chBuffer[1] == 't' || chBuffer[1] == 'T'))
		{
			pEmitter->eEmitterPlaceType = EPLWEATHER_EMITTER_PLACE_AT;
		}

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "x", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vEmitterPos.x = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "y", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vEmitterPos.y = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "z", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vEmitterPos.z = static_cast<RwReal>(::atof(chBuffer));
	}

	// SIZE
	if (SUCCEEDED(pNode->selectSingleNode(L"size", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "x", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vEmitterSize.x = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "y", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vEmitterSize.y = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "z", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vEmitterSize.z	= static_cast<RwReal>(::atof(chBuffer));
	}

	return TRUE;
}

RwBool CNtlPLWeatherProperty::LoadParticle(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode, SPLWeatherEmitter* pEmitter)
{
	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNode*		pNodeChild					= NULL;
	IXMLDOMNodeList*	pNodeChildList				= NULL;	

	// CREATE
	if (SUCCEEDED(pNode->selectSingleNode(L"create", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "num", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->iParticleCrtNum = static_cast<RwInt32>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "time", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->fParticleCrtTime = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "max", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->iParticleMax = static_cast<RwInt32>(::atoi(chBuffer));		
	}

	// TEXTURE
	if (SUCCEEDED(pNode->selectSingleNode(L"texture", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "name", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		strcpy_s(pEmitter->acTextureName, dPL_WEATHER_CHAR_BUFFER_SMALL, chBuffer);
	}

	// SIZE
	if (SUCCEEDED(pNode->selectSingleNode(L"size", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "x", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleSize.x = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "y", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleSize.y = static_cast<RwReal>(::atof(chBuffer));
	}

	// BILLBOARD
	if (SUCCEEDED(pNode->selectSingleNode(L"billboard", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "type", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		if (strlen(chBuffer) >= 1 && (chBuffer[0] == 'y' || chBuffer[0] == 'Y'))
		{
			pEmitter->eParticleBillBoardType = EPLWEATHER_BILLBOARD_Y;
		}
		else
		{
			pEmitter->eParticleBillBoardType = EPLWEATHER_BILLBOARD;
		}

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "x", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleBillBoard.x = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "y", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleBillBoard.y = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "z", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleBillBoard.z = static_cast<RwReal>(::atof(chBuffer));

		RwV3dNormalize(&pEmitter->vParticleBillBoard, &pEmitter->vParticleBillBoard);
	}

	// DIRECTION
	if (SUCCEEDED(pNode->selectSingleNode(L"direction", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "x", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleDirection.x = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "y", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleDirection.y = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "z", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->vParticleDirection.z = static_cast<RwReal>(::atof(chBuffer));

		RwV3dNormalize(&pEmitter->vParticleDirection, &pEmitter->vParticleDirection);
	}

	// VECLOCITY
	if (SUCCEEDED(pNode->selectSingleNode(L"velocity", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "value", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->fParticleVelocity = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "gap", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->fParticleVelocityGap = static_cast<RwReal>(::atof(chBuffer));
	}

	// LIFETIME
	if (SUCCEEDED(pNode->selectSingleNode(L"lifetime", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "value", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->fParticleLifeTime = static_cast<RwReal>(::atof(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "gap", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->fParticleLifeTimeGap = static_cast<RwReal>(::atof(chBuffer));
	}

	// COLOR
	if (SUCCEEDED(pNode->selectSingleNode(L"color", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "start_alpha", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleStart.alpha = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "start_red", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleStart.red = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "start_green", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleStart.green = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "start_blue", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleStart.blue = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "end_alpha", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleEnd.alpha = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "end_red", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleEnd.red = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "end_green", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleEnd.green = static_cast<RwUInt8>(::atoi(chBuffer));

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "end_blue", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->clrParticleEnd.blue = static_cast<RwUInt8>(::atoi(chBuffer));
	}

	// COLLISION
	if (SUCCEEDED(pNode->selectSingleNode(L"collision", &pNodeChild)))
	{
		pEmitter->eParticleCollisionType = 0x00000000;

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "effect", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		strcpy_s(pEmitter->acEffectName, dPL_WEATHER_CHAR_BUFFER_SMALL, chBuffer);		

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "terrain", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		if (::atoi(chBuffer))
		{
			pEmitter->eParticleCollisionType |= EPLWEATHER_COLLISION_TERRAIN;
		}

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "water", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		if (::atoi(chBuffer))
		{
			pEmitter->eParticleCollisionType |= EPLWEATHER_COLLISION_WATER;
		}

		if (!pDoc->GetTextWithAttributeName(pNodeChild, "destroy", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		if (::atoi(chBuffer))
		{
			pEmitter->eParticleCollisionType |= EPLWEATHER_COLLISION_DESTORY;
		}
	}

	// REDNER
	if (SUCCEEDED(pNode->selectSingleNode(L"render", &pNodeChild)))
	{
		if (!pDoc->GetTextWithAttributeName(pNodeChild, "length", chBuffer, NTL_MAX_DIR_PATH))
		{
			return FALSE;
		}

		pEmitter->fParticleRenderLength = static_cast<RwReal>(::atof(chBuffer));
	}

	return TRUE;
}
*/
