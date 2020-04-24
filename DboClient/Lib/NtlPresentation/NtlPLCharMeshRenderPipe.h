#pragma once


#include "NtlMaterialExt.h"


#define dRW_DXT1_ALPHA_CHECKER_ON	{g_DXT1AlphaChecker = RpNtlMaterialExtGetFlag(m_pParam->mesh->material) & rpNTL_MATERIAL_DXT1_ALPHA_CHECK;}
#define dRW_DXT1_ALPHA_CHECKER_OFF	{g_DXT1AlphaChecker = FALSE;}


class CNtlPLCharMeshRenderPipe
{
public:
	typedef std::map<D3DCOLOR, RwTexture* >				MAP_TINY_COLOR_TEX;
	typedef	std::map<D3DCOLOR, RwTexture* >::iterator	MAP_TINY_COLOR_TEX_ITER;

public:
	CNtlPLCharMeshRenderPipe(void);
	virtual ~CNtlPLCharMeshRenderPipe(void);

protected:
	RwMatrix			m_SEMTexMat;
	MAP_TINY_COLOR_TEX	m_mapTinyColorTex;
	D3DCOLOR			m_EnvColor, m_SkinColor, m_AllColor;
	RwBlendFunction		m_srcBlend, m_destBlend;
	RwBool				m_zWriteEnable;	
	RwBool				m_bAlphaEnable;
	RwTexture*			m_pEnvMap;
	RwMatrix*			m_pTexMat;
	RwBool				m_EMUVWorking;
	RwTexture*			m_pBaseTexture;
	RwTexture*			m_pMultiTexture;
	RwBool				m_EmblemWorking;
	RwBool				m_DogiWearingOnly;
	RwTexture*			m_pSkinTexture;
	RwTexture*			m_pDogiEditedColorTex;
	sMatExtInfo*		m_pParam;
	RwRGBA*				m_pCombinationColor;
	RwTexture*			m_pSEM;
	static RwTexture*	m_pPetrify;
	RwBool				m_PetrifySkillActivated;

protected:
	static RwBool m_ToggleEMUVAni;

public:
	virtual void		Execute(void* _pParam) = 0;
	virtual void		GetRenderCase(void* _pParam);
	virtual void		RenderCharTgtSelecting(RwTexture* basetexture);
	virtual	RwTexture*	GetTinyColorTex(RwRGBA* _pRGBA);

public:
	RwMatrix*		GetSEMTexMat() { return &m_SEMTexMat; }
	static	VOID	ToggleEMUVAni(RwBool _Visibility) { m_ToggleEMUVAni = _Visibility; }
	RwBool			GetToggleEMUVAni() { return m_ToggleEMUVAni; }
};