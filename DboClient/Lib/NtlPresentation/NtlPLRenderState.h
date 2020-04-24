/*****************************************************************************
 *
 * File			: NtlPLRenderState.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 06	
 * Abstract		: Presentation render state 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLRENDERSTATE_H__
#define __NTL_PLRENDERSTATE_H__

#include <rwcore.h>
#include <rpworld.h>

void BeginRenderGroupLayer(RwUInt32 uiRenderLayer);
void EndRenderGroupLayer(RwUInt32 uiRenderLayer);

// tree
void BeginTreeRenderState(void);
void EndTreeRenderState(void);

// name box
void BeginNameRenderState(void);
void EndNameRenderState(void);

// damage box
void BeginDamageBoxRenderState(void);
void EndDamageBoxRenderState(void);

// gui
void BeginGuiRenderState(void);
void EndGuiRenderState(void);

// decal
void BeginCharacterDecalState(void);
void EndCharacterDecalState(void);

// terrain
void BegTerrainLayerCommonState(void);
void EndTerrainLayerCommonState(void);

void BegTerrainBaseLayerState(RwBool IsTileTransparencyUse);
void EndTerrainBaseLayerState(RwBool IsTileTransparencyUse);

void BegTerrainDetailLayerState(RwBool IsTileTransparencyUse);
void EndTerrainDetailLayerState(RwBool IsTileTransparencyUse);

void BeginTerrrainShadowState();
void EndTerrrainShadowState();

void BeginTerrainWireFrameState();
void EndTerrainWireFrameState();

// sky

void BegBaseSkyRenderState( void );
void EndBaseSkyRenderState( void );

void BegDummySkyRenderState( void );
void EndDummySkyRenderState( void );

void BeginSkyLightningRenderState();
void EndSkyLightningRenderState();

// fog
void BegLinearFogRenderState(RwUInt32 iFogColor, RwReal Start, RwReal End);
void EndLinearFogRenderState( void );


// effect
void BeginEffectParticleSystemState(void);
void EndEffectParticleSystemState(void);

void BeginEffectMeshSystemState(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend, RwBool bZwrite);
void EndEffectMeshSystemState(RwBool bBlend);

void BeginEffectImVertexSystemState(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend);
void EndEffectImVertexSystemState(RwBool bBlend);;

void BeginEffectTraceSystem(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend);
void EndEffectTraceSystem(RwBool bBlend);

void BeginEffectDecalSystemState(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend);
void EndEffectDecalSystemState(RwBool bBlend);

void BeginEffectPostSystemState(RwBlendFunction srcBlend, RwBlendFunction destBlend);
void EndEffectPostSystemState();

void BeginEffectHurricaneSystem(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend);
void EndEffectHurricaneSystem(RwBool bBlend);

void BeginBlurEffect();
void EndBlurEffect();

void BeginPostEffectState(RwBool bEnable);
void EndPostEffectState(RwBool bEnable);

void BeginSunState();
void EndSunState();

void BeginLensFlareState();
void EndLensFlareState();


// Ntl Atomic
void BeginNtlAtomicTwoSideState();
void EndNtlAtomicTwoSideState();

void BeginNtlAotmicAlphaTestStateShadow();
void BeginNtlAotmicAlphaTestState();
void EndNtlAtomicAlphaTestState();

// Only UpdateAlphaState() can handle alpha blending.
// However, vertex and texture alpha blending will be applied at the same time so be fully aware of this.
void UpdateAlphaState(RwBool bEnable);

// water
void SetWaterRenderStateBegin(/*DWORD SrcAlpha, DWORD DstAlpha, RwBool IsDepthMapEnable,*/ DWORD DayAndNight);
void SetWaterRenderStateEnd();

// shoreline
void SetShoreLineRenderStateBegin(RwInt32 ShoreLineType, DWORD DayAndNight);
void SetShoreLineRenderStateEnd();

// d3d line state
void BeginD3DLineState();
void EndD3DLineState();

// shadow property state
void BeginShadowPropState();
void EndShadowPropState();

// weather ptank
void BeginWeatherPTankRenderState(RwBool bUseZTest, RwBool bUseFog, RwBool bUseLight);
void EndWeatherPTankRenderState(RwBool bUseZTest, RwBool bUseFog, RwBool bUseLight);

// fog restore
void BeginFogState(RwBool bFog);
void EndFogState();

// light restore
void BeginLightState(RwBool bLight);
void EndLightState();

#endif