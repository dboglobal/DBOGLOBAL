#include "precomp_ntlpresentation.h"
#include "NtlWorldCommon.h"

#include "NtlMath.h"
#include "NtlPLDef.h"

#include "NtlPLOptionManager.h"
#include "NtlWorldInterface.h"
#include "NtlPLRenderState.h"


// 형석
typedef void (*fnRenderState)(void);

fnRenderState g_BeginRS[PLENTITY_LAYER_MAX] = {	NULL,
												NULL,
												NULL,
												NULL,
												BeginCharacterDecalState,
												BeginNameRenderState,
												BeginDamageBoxRenderState,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,};

fnRenderState g_EndRS[PLENTITY_LAYER_MAX] = {	NULL,
												NULL, 
												NULL,
												NULL,
												EndCharacterDecalState,
												EndNameRenderState,
												EndDamageBoxRenderState,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,
												NULL,};

void BeginRenderGroupLayer(RwUInt32 uiRenderLayer)
{
	if(g_BeginRS[uiRenderLayer])
		(*g_BeginRS[uiRenderLayer])();
}

void EndRenderGroupLayer(RwUInt32 uiRenderLayer)
{
	if(g_EndRS[uiRenderLayer])
		(*g_EndRS[uiRenderLayer])();
}


void BeginTreeRenderState(void)
{
	RwRenderStateSet( rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0xd0 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );
}

void EndTreeRenderState(void)
{
	RwRenderStateSet( rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATER );
}

void BeginNameRenderState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
}

void EndNameRenderState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND,(void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND,(void*)rwBLENDINVSRCALPHA);
}

void BeginDamageBoxRenderState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE); 
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
}

void EndDamageBoxRenderState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND,(void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND,(void*)rwBLENDINVSRCALPHA);
}

void BeginGuiRenderState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);
	
	RwD3D9SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); 

	// stage 0 alpha : nada 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); 

	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) NULL);
	
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
}

void EndGuiRenderState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);

	RwD3D9SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_CURRENT); 

	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); 

	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *) rwSHADEMODEGOURAUD);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) NULL);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
}

void BeginCharacterDecalState(void)
{
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE); 
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

}

void EndCharacterDecalState(void)
{
	RwD3D9SetTexture(NULL, 0);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);
}

void BegTerrainLayerCommonState(void)
{
	BeginLightState(FALSE);

	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, dGET_WORLD_PARAM()->ClrDayAndNight);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
}
void EndTerrainLayerCommonState(void)
{	
	EndLightState();
	
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}

void BegTerrainBaseLayerState(RwBool IsTileTransparencyUse)
{
	if(IsTileTransparencyUse)
	{
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}
	else
	{
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}
}

void EndTerrainBaseLayerState(RwBool IsTileTransparencyUse)
{
	if(IsTileTransparencyUse)
	{
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	}

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void BegTerrainDetailLayerState(RwBool IsTileTransparencyUse)
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	if (IsTileTransparencyUse)
	{
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}
	else
	{
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	}
}

void EndTerrainDetailLayerState(RwBool IsTileTransparencyUse)
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void BeginTerrrainShadowState()
{
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
}

void EndTerrrainShadowState()
{
	RwD3D9SetTexture(NULL, 2);

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &mIdentity);
	
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void BeginTerrainWireFrameState()
{
	BeginFogState(FALSE);
	BeginLightState(FALSE);

 	RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xff808080);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
}

void EndTerrainWireFrameState()
{
	EndFogState();
	EndLightState();

	RwD3D9SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}


void BegBaseSkyRenderState(void)
{
	BeginFogState(FALSE);
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	
	static RwReal fDistMax = 512.0f;
	RpNtlActiveCameraSetFarDistance(fDistMax);

	return;
}


void EndBaseSkyRenderState(void)
{
	EndFogState();
	EndLightState();

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	RpNtlActiveCameraSetFarDistance(GetNtlPLOptionManager()->GetInstance()->GetTerrainFar());

	return;
}


void BegDummySkyRenderState(void)
{
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

	static RwReal fDistMax = 512.0f;
	RpNtlActiveCameraSetFarDistance(fDistMax);

	return;
}


void EndDummySkyRenderState(void)
{
	EndLightState();

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);

	RpNtlActiveCameraSetFarDistance(GetNtlPLOptionManager()->GetInstance()->GetTerrainFar());

	return;
}


void BegLinearFogRenderState(RwUInt32 iFogColor, RwReal Start, RwReal End)
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEFOGCOLOR, (void*)iFogColor);
	RwD3D9SetRenderState(D3DRS_FOGSTART, CNtlMath::FtoDW(Start));
	RwD3D9SetRenderState(D3DRS_FOGEND, CNtlMath::FtoDW(End));

	return;
}

void EndLinearFogRenderState( void )
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void *)FALSE);

	return;
}

void BeginEffectParticleSystemState(void)
{
	BeginLightState(FALSE);
	
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);    
}
void EndEffectParticleSystemState(void)
{
	EndLightState();
	
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);    
}

void BeginEffectMeshSystemState(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend, RwBool bZwrite)
{
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,				(void *)bZwrite);

	if (bBlend)
	{
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)TRUE); 
		RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)srcBlend);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)destBlend);
	}
}
void EndEffectMeshSystemState(RwBool bBlend)
{
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,				(void *)TRUE);
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);

	if (bBlend)
	{
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE); 
		RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);
	}
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void BeginEffectImVertexSystemState(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend)
{
	if (bBlend)
	{
        
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);                

		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)TRUE);         
		RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)srcBlend);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)destBlend);
	}
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLNONE);
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void EndEffectImVertexSystemState(RwBool bBlend)
{
	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);

	if (bBlend)
	{
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)TRUE);

		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE); 
		RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);
	}
}

void BeginEffectTraceSystem( RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend ) 
{
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)bBlend);         

    if (bBlend)
    {
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);                       
        
        RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)srcBlend);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)destBlend);
    }
    RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLNONE);

    //RwRenderStateSet(rwRENDERSTATESTENCILENABLE, (void*)TRUE);
    //RwRenderStateSet(rwRENDERSTATESTENCILFUNCTION, (void*)rwSTENCILFUNCTIONGREATER);
    //RwRenderStateSet(rwRENDERSTATESTENCILFUNCTIONREF, (void*)0x00000001);
    //RwRenderStateSet(rwRENDERSTATESTENCILFUNCTIONMASK, (void*)0xFFFFFFFF);
    //RwRenderStateSet(rwRENDERSTATESTENCILFUNCTIONWRITEMASK, (void*)0xFFFFFFFF);
    //RwRenderStateSet(rwRENDERSTATESTENCILFAIL, (void*)rwSTENCILOPERATIONKEEP);
    //RwRenderStateSet(rwRENDERSTATESTENCILZFAIL, (void*)rwSTENCILOPERATIONKEEP);
    //RwRenderStateSet(rwRENDERSTATESTENCILPASS, (void*)rwSTENCILOPERATIONINCRSAT);
}

void EndEffectTraceSystem( RwBool bBlend ) 
{
    RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);

    if (bBlend)
    {
        //RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);        
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)TRUE);        

        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE); 
        RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);
    }

    RwRenderStateSet(rwRENDERSTATESTENCILENABLE, (void*)FALSE);
}

void BeginEffectHurricaneSystem( RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend ) 
{
    if (bBlend)
    {

        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);                

        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)TRUE);         
        RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)srcBlend);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)destBlend);
    }
    RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLNONE);
}

void EndEffectHurricaneSystem( RwBool bBlend ) 
{
    RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);

    if (bBlend)
    {
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)TRUE);

        RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE); 
        RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
        RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);
    }
}

void BeginEffectPostSystemState( RwBlendFunction srcBlend, RwBlendFunction destBlend ) 
{
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);                

    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)TRUE);         
    RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)srcBlend);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)destBlend);
}

void EndEffectPostSystemState() 
{
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)TRUE);                

    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE);         
    RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void BeginEffectDecalSystemState(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend)
{
	BeginEffectImVertexSystemState(bBlend, srcBlend, destBlend);

	RwRenderStateSet(rwRENDERSTATECULLMODE,					(void *)rwCULLMODECULLBACK);		

	RwReal fDepthBias = -0.00005f;
	RwD3D9SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)&fDepthBias));	
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void EndEffectDecalSystemState(RwBool bBlend)
{
	RwD3D9SetRenderState(D3DRS_DEPTHBIAS, 0x00000000);	
	EndEffectImVertexSystemState(bBlend);	
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void BeginPostEffectState(RwBool bEnable)
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	if (bEnable)
	{
		RwD3D9SetTextureStageState (0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);

		RwD3D9SetTextureStageState (1, D3DTSS_COLOROP,		D3DTOP_ADD);
		RwD3D9SetTextureStageState (1, D3DTSS_COLORARG1,	D3DTA_CURRENT); 
		RwD3D9SetTextureStageState (1, D3DTSS_COLORARG2,	D3DTA_TEXTURE);
	}
}

//------------------------------------------------------------------
//	FuncName	: BeginRenderState
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void EndPostEffectState(RwBool bEnable)
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	if (bEnable)
	{
		RwD3D9SetTextureStageState (0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);

		RwD3D9SetTextureStageState(1, D3DTSS_COLOROP,	D3DTOP_DISABLE);	
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);
	}

	// CzStateChange : Start
	// CzStateChange : New
	RwD3D9SetTextureStageState (1, D3DTSS_TEXCOORDINDEX, 1);
	// CzStateChange : End	
}


void BeginBlurEffect() 
{
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);         
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

    RwD3D9SetTextureStageState (0, D3DTSS_COLOROP,		D3DTOP_MODULATE);
    RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
    RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);

    RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE);
    RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
    RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE);

}
void EndBlurEffect() 
{
    RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
    
    RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
    
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
}


void BeginNtlAtomicTwoSideState()
{
	RwRenderStateSet( rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLNONE);
}

void EndNtlAtomicTwoSideState()
{
	RwRenderStateSet( rwRENDERSTATECULLMODE,(void *)rwCULLMODECULLBACK);
}

void BeginNtlAotmicAlphaTestStateShadow()
{
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0x70 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );
}

void BeginNtlAotmicAlphaTestState()
{
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0xd0 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );
}

void EndNtlAtomicAlphaTestState()
{
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATER );
}

void UpdateAlphaState(RwBool bEnable)
{
	if(bEnable)
	{
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	}
	else
	{
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
	}
}

// woody1019 // Cz : 07.11.26
void SetWaterRenderStateBegin(/*DWORD SrcAlpha, DWORD DstAlpha, RwBool IsDepthMapEnable,*/ DWORD DayAndNight)
{
	//RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	//RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)SrcAlpha);
	//RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)DstAlpha);
	
	BeginLightState(FALSE);

	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, DayAndNight); 

 	// Specular Alpha Map
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// Specular Map
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);	
 	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// Base Map
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);	
	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT); 
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// DepthMap
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);	
	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_CURRENT); 
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	
	// CNtlPLWater::OnRender 안에서 결정
	//RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	//RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void SetWaterRenderStateEnd()
{
	EndLightState();

	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);
	RwD3D9SetTexture(NULL, 2);
	RwD3D9SetTexture(NULL, 3);
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE2, &mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE3, &mIdentity);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,	1);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 3);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(2, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	RwD3D9SetTextureStageState(3, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	
	//RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	//RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	//RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
}

void BeginLensFlareState() 
{
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);                
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);

	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
}

void EndLensFlareState() 
{
	EndLightState();
	
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)TRUE);                
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

}

void BeginSunState() 
{
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLNONE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)FALSE);                

	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//RwD3D9SetRenderState(D3DRS_COLORVERTEX, TRUE);
	//RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//RwD3D9SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//RwD3D9SetRenderState(D3DRS_ALPHAREF, 0x5);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);	
}

void EndSunState() 
{
	EndLightState();

	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLBACK);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE,			(void *)TRUE);                

	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//RwD3D9SetRenderState(D3DRS_COLORVERTEX, FALSE);
	//RwD3D9SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);	
}

void BeginD3DLineState()
{
	BeginFogState(FALSE);
	BeginLightState(FALSE);
	
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
}

void EndD3DLineState()
{
	EndFogState();
	EndLightState();

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void BeginShadowPropState()
{
	BeginLightState(TRUE);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void EndShadowPropState()
{
	EndLightState();

	RwD3D9SetTexture(NULL, 0);

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void BeginSkyLightningRenderState()
{
	BeginFogState(FALSE);
	BeginLightState(FALSE);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
}

void EndSkyLightningRenderState()
{
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);

	EndLightState();
	EndFogState();
}

void BeginWeatherPTankRenderState(RwBool bUseZTest, RwBool bUseFog, RwBool bUseLight)
{
	BeginFogState(bUseFog);
	BeginLightState(bUseLight);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)bUseZTest);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);	
}

void EndWeatherPTankRenderState(RwBool bUseZTest, RwBool bUseFog, RwBool bUseLight)
{
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	
	EndLightState();
	EndFogState();
}



/*
void BeginWaterRenderstate(DWORD SrcAlpha, DWORD DstAlpha)
{
	RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, dGET_WORLD_PARAM()->ClrDayAndNight);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);	
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	/*
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)SrcAlpha);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)DstAlpha);

	RwD3D9SetRenderState(D3DRS_LIGHTING, FALSE);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);

	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

}

void EndWaterRenderstate()
{
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);

	RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);
	RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);

	D3DXMATRIXA16 mIdentity;
	D3DXMatrixIdentity(&mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE0, &mIdentity);
	RwD3D9SetTransform(D3DTS_TEXTURE1, &mIdentity);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);

	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT); 
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); 
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);  
	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);	
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	/*
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwD3D9SetRenderState(D3DRS_LIGHTING, TRUE);

	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAARG2,	 D3DTA_CURRENT);
	
}	*/

static DWORD g_RenderStateFogBackup;
void BeginFogState(RwBool bFog)
{
	RwRenderStateGet(rwRENDERSTATEFOGENABLE, &g_RenderStateFogBackup);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)bFog);
}

void EndFogState()
{
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)g_RenderStateFogBackup);
}

static DWORD g_RenderStateLightBackup;
void BeginLightState(RwBool bLight)
{
	RwD3D9GetRenderState(D3DRS_LIGHTING, &g_RenderStateLightBackup);
	RwD3D9SetRenderState(D3DRS_LIGHTING, bLight);
}

void EndLightState()
{
	RwD3D9SetRenderState(D3DRS_LIGHTING, g_RenderStateLightBackup);
}