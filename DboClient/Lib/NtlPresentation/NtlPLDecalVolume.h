#pragma once

#include "NtlDebug.h"

#define dPL_DECAL_VOLUME_BUFFER_SIZE	(1024)
#define dPL_DECAL_TEXTURE_NAME_LEN		(128)

enum EPL_DECAL_VOLUME_FLAG
{	
	EPL_DECAL_VOLUME_FLAG_TERRAIN	= 0x00000001,
	EPL_DECAL_VOLUME_FLAG_OBJECT	= 0x00000002,
};

struct SPLDecalVolumeCreateParam : public SPLEntityCreateParam
{
	RwChar			acTextureName[dPL_DECAL_TEXTURE_NAME_LEN];
	RwV3d			vRotation;
	RwV3d			vScale;
	RwRGBA			clrDiffuse;
	RwReal			fAppendOffset;
	RwReal			fVisibleDist;
	RwUInt32		uiDecalVolumeFlags;
};

class CNtlPLDecalVolume :public CNtlPLEntity
{
	enum
	{	
		EDECAL_FADE_IN_ING,
		EDECAL_FADE_IN,
		EDECAL_FADE_OUT_ING,		
		EDECAL_FADE_OUT,
	};
public:
	CNtlPLDecalVolume();
	virtual ~CNtlPLDecalVolume();

	virtual RwBool				Create(const SPLEntityCreateParam * pParam = NULL);
	virtual void				Destroy();

	virtual RwBool				SetProperty(const CNtlPLProperty *pData);
	
	virtual RwBool				Update(RwReal fElapsed);
	virtual RwBool				Render();

	virtual void				SetPosition(const RwV3d* pPos);
	virtual RwV3d				GetPosition();
	
	virtual void				SetScale(const RwV3d* pScale);
	virtual const RwV3d*		GetScale() const;

	virtual void				SetRotate(RwReal fXAngle, RwReal fYAngle, RwReal fZAngle);
	virtual const RwV3d*		GetRotate() const; 

	virtual void				SetColor(RwRGBA* pClr);
	virtual const RwRGBA*		GetColor() const;

	virtual void				SetDecalVolumeFlags(RwUInt32 uiFlags);
	virtual RwUInt32			GetDecalVolumeFlags();

	virtual void				SetAppendOffset(RwReal fAppendOffset);
	virtual	RwReal				GetAppendOffset();

	virtual void				SetVisibleDist(RwReal fVisibleDist);
	virtual RwReal				GetVisibleDist();

	virtual const RwChar*		GetTextureName() const { return m_acTextureName; }
	virtual const RwTexture*	GetTexture() const { return m_pTexture; }
	
	virtual void				UpdateDecal();
	virtual void				UpdateDecalVertice();
	virtual void				UpdateDecalColor(RwRGBA* pRwRGBA);

	virtual RwBool				Pick(RwReal& fDist);
	virtual void				RotatePickPlane();
	virtual void				GetDecalBBox(RwBBox* pBBox);

public:
	static CNtlPLDecalVolume*	s_pNtlPLDecalVolumeTool;
	static void					SetToolUI(CNtlPLDecalVolume* pNtlPLDecalVolume) { s_pNtlPLDecalVolumeTool = pNtlPLDecalVolume; }

protected:
	void						RunableFade(RwUInt32 eFadeState, RwReal fTime);
	void						UpdateFade(RwReal fElapsed);

	RwUInt32					m_eFadeState;
	RwReal						m_fTimeFadeCur;
	RwReal						m_fTimeFade;

protected:
	RwV3d						m_vPos;
	RwV3d						m_vRot;
	RwV3d						m_vScale;
	RwRGBA						m_clrDiffuse;

	RwUInt32					m_uiDecalVolumeFlags;

	RwReal						m_fAppendOffset;
	RwReal						m_fVisibleDist;
	
	RwIm3DVertex*				m_pVertices;
	RwUInt32					m_uiVerticesCnt;
	RwReal						m_fRadius;

	RwTexture*					m_pTexture;
	RwChar						m_acTextureName[dPL_DECAL_TEXTURE_NAME_LEN];
};