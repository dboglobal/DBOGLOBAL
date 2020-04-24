#ifndef __NTL_PLSKY_H__
#define __NTL_PLSKY_H__


#include <RwCore.h>
#include "NtlDebug.h"
#include "NtlPLAttach.h"
#include "NtlPLApi.h"
#include "ceventhandler.h"


#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT0	(300)
#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT1	(280)
#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT2	(210)
#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT3	(140)
#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT4	(40)
#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT5	(0)
#define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT6	(-180)


#define dSKY_INTERPOLATION_RGB_2_RGB	0x00000001 
#define dSKY_INTERPOLATION_RGB_2_TEX	0x00000002
#define dSKY_INTERPOLATION_TEX_2_RGB	0x00000004
#define dSKY_INTERPOLATION_TEX_2_TEX	0x00000008
#define	dSKY_INTERPOLATION_OPA_PASS1	0x00000010
#define	dSKY_INTERPOLATION_OPA_PASS2	0x00000020
#define	dSKY_INTERPOLATION_ANG_PASS0	0x00000100
#define	dSKY_INTERPOLATION_ANG_PASS1	0x00000200
#define	dSKY_INTERPOLATION_ANG_PASS2	0x00000400

#define dSKY_RGB_EFFECT_SWITCHING_TIME (2.0f)
#define dSKY_OPA_EFFECT_SWITCHING_TIME (3.0f)
#define dSKY_ROT_EFFECT_SWITCHING_TIME (4.0f)

struct sNTL_FIELD_PROP;

enum eBASE_SKY_MODE
{
	eBSM_NOTHING = 0,
	eBSM_RGB_MODE,
	eBSM_TEX_MODE,
};

enum eSKY_TYPE_INDEX
{
	eSTI_HEMISPHERE = 0,
	eSTI_SPHERE,
	eSTI_DRAGONE,
};

class CNtlPLSky : public CNtlPLEntity, public RWS::CEventHandler
{
protected:

	struct sSKY_LAYER
	{
	public:
		sSKY_LAYER::sSKY_LAYER()
		{
			_AutoDelete	= FALSE;
			_pRender	= FALSE;
			_pAtom		= NULL;
			//_pTex		= NULL;

			for(int i = 0; i < 3; ++i)
			{
				_RGBA[i].red	= 255;
				_RGBA[i].green	= 255;
				_RGBA[i].blue	= 255;
				_RGBA[i].alpha	= 255;
			}

			_RotSpeed		= 0.0f;
			_RotSpeedMPS	= 0.0f;
		}

		sSKY_LAYER::~sSKY_LAYER()
		{
			if (_pAtom)
			{
 				if (API_PL_AtomicGetTexture(_pAtom))
				{
					API_PL_AtomicSetTexture(_pAtom, NULL);
				}
				API_PL_DeleteAtomic(_pAtom);
			}

			//if(_pTex)
			//{
			//	RwTextureDestroy(_pTex);
			//}
		}

	public:
		bool		_AutoDelete;
		RwBool		_pRender;
		RpAtomic*	_pAtom;
		//RwTexture*	_pTex;
		RwRGBA		_RGBA[3];
		RwRGBA		_RGBAFrom[3], _RGBATo[3];
		RwReal		_RotSpeed;
		RwReal		_RotSpeedMPS;
		RwReal		_RotSpeedFrom, _RotSpeedTo;
		RwMatrix	_Mat;

	public:
		void Rotate(RwReal Ang)
		{
			if(_RotSpeed > 360.0f)
			{
				_RotSpeed -= 360.0f;
			}

			_RotSpeed += Ang;
		}
	};

public:
	CNtlPLSky(void);
	virtual	~CNtlPLSky(void);

protected:
	RwBool		m_bPassedSky;

	DWORD		m_InterpolationMode;
	sSKY_LAYER*	m_pDummySkyLayer;
	sSKY_LAYER*	m_pBaseSkyLayer[2];
	sSKY_LAYER*	m_pBlendedSkyLayer[2];
	sSKY_LAYER*	m_pBlendedTmpLayer[2];
	RwV3d		m_WorldDatumPos;
	RwReal		m_RestTime4Change[3];

	// Lightning
	RwRGBA		m_clrLightning;

	RwReal		m_fTimeFadeIn;
	RwReal		m_fTimeFadeOut;
	RwReal		m_fTimeLightning;
	
public:
	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void	Destroy();
	virtual void	HandleEvents(RWS::CMsg &pMsg);

	virtual	void	UpdateFieldVariation(RwReal fElapsed);
	virtual RwBool	Update(RwReal fElapsed);

	virtual void	SkyLayerRender(sSKY_LAYER* pSkyLayer, RwInt32 iTFactor);
	virtual RwBool	Render(void);

	virtual eBASE_SKY_MODE	GetBaseSkyMode();
	virtual	void			SetColorOfDiffuseSkyAtomic(RwRGBA m_DiffuseSkyAtomicRGBA[3]);
	virtual RwBool			SetProperty(const CNtlPLProperty *pData);
	virtual void			SetPosition(const RwV3d *pPosition) { if(pPosition) memcpy(&m_WorldDatumPos, pPosition, sizeof( RwV3d ) ); }
	virtual RpAtomic*		GetAtomic();

	virtual void			RunableLightning(RwReal fTimeFadeIn, RwReal fTimeFadeOut);
	virtual void			RunableVariation(sNTL_FIELD_PROP* pNtlFieldProp, RwBool NoVariation);

public:
	RwBool m_VisibilityFlag;
};


#endif //__NTL_PLSKY_H__

// Sky Performance 향상을 위해서 Mesh를 분할하였으나 향상되지 않았다.
// #ifndef __NTL_PLSKY_H__
// #define __NTL_PLSKY_H__
// 
// 
// #include <RwCore.h>
// 
// #include "CEventHandler.h"
// 
// #include "NtlDebug.h"
// #include "NtlPLAttach.h"
// #include "NtlPLApi.h"
// #include "NtlPLEntityRenderHelpers.h"
// #include "NtlPLResourceManager.h"
// 
// 
// 
// #define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT0	(300)
// #define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT1	(280)
// #define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT2	(210)
// #define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT3	(140)
// #define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT4	(40)
// #define dNTL_PL_DIFFUSE_SKY_RGB_HEIGHT5	(0)
// 
// 
// #define dSKY_INTERPOLATION_RGB_2_RGB	0x00000001 
// #define dSKY_INTERPOLATION_RGB_2_TEX	0x00000002
// #define dSKY_INTERPOLATION_TEX_2_RGB	0x00000004
// #define dSKY_INTERPOLATION_TEX_2_TEX	0x00000008
// #define	dSKY_INTERPOLATION_OPA_PASS1	0x00000010
// #define	dSKY_INTERPOLATION_OPA_PASS2	0x00000020
// #define	dSKY_INTERPOLATION_ANG_PASS0	0x00000100
// #define	dSKY_INTERPOLATION_ANG_PASS1	0x00000200
// #define	dSKY_INTERPOLATION_ANG_PASS2	0x00000400
// 
// #define dSKY_RGB_EFFECT_SWITCHING_TIME (2.0f)
// #define dSKY_OPA_EFFECT_SWITCHING_TIME (3.0f)
// #define dSKY_ROT_EFFECT_SWITCHING_TIME (4.0f)
// 
// 
// enum eBASE_SKY_MODE
// {
// 	eBSM_NOTHING = 0,
// 	eBSM_RGB_MODE,
// 	eBSM_TEX_MODE,
// };
// 
// enum eSKY_TYPE_INDEX
// {
// 	eSTI_HEMISPHERE = 0,
// 	eSTI_SPHERE,
// 	eSTI_DRAGONE,
// };
// 
// class CNtlPLSky : public CNtlPLEntity, public RWS::CEventHandler
// {
// protected:
// 
// 	struct sSKY_LAYER
// 	{
// 	public:
// 		sSKY_LAYER::sSKY_LAYER() : _AutoDelete(FALSE), _pRender(FALSE), _pClumpRes(NULL), _pTexture(NULL), _RotSpeed(0.0f), _RotSpeedMPS(0.0f)
// 		{
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				_RGBA[i].red	= 255;
// 				_RGBA[i].green	= 255;
// 				_RGBA[i].blue	= 255;
// 				_RGBA[i].alpha	= 255;
// 			}
// 		}
// 
// 		sSKY_LAYER::~sSKY_LAYER()
// 		{
// 			for (int i = 0; i < (int)_vecAtomic.size(); ++i)
// 			{
// 				NTL_DELETE(_vecAtomic.at(i));
// 			}
// 			_vecAtomic.clear();
// 
// 			if (_pClumpRes)
// 			{
// 				CNtlPLResourceManager::GetInstance()->UnLoad(_pClumpRes);
// 			}
// 
// 			if (_pTexture)
// 			{
// 				CNtlPLResourceManager::GetInstance()->UnLoadTexture(_pTexture);
// 			}
// 		}
// 
// 	public:
// 		CNtlPLResource*		_pClumpRes;
// 		RwTexture*			_pTexture;
// 		ENTITY_ATOMIC_VEC	_vecAtomic;
// 
// 		RwBool				_AutoDelete;
// 		RwBool				_pRender;
// 
// 		RwRGBA				_RGBA[3];
// 		RwRGBA				_RGBAFrom[3], _RGBATo[3];
// 		RwReal				_RotSpeed;
// 		RwReal				_RotSpeedMPS;
// 		RwReal				_RotSpeedFrom, _RotSpeedTo;
// 		RwMatrix			_Mat;
// 
// 	public:
// 		void Rotate(RwReal Ang)
// 		{
// 			if(_RotSpeed > 360.0f)
// 			{
// 				_RotSpeed -= 360.0f;
// 			}
// 
// 			_RotSpeed += Ang;
// 		}
// 	};
// 
// public:
// 	CNtlPLSky(void);
// 	virtual	~CNtlPLSky(void);
// 
// protected:
// 	DWORD		m_InterpolationMode;
// 	sSKY_LAYER*	m_pDummySkyLayer;
// 	sSKY_LAYER*	m_pBaseSkyLayer[2];
// 	sSKY_LAYER*	m_pBlendedSkyLayer[2];
// 	sSKY_LAYER*	m_pBlendedTmpLayer[2];
// 	RwV3d		m_WorldDatumPos;
// 	RwReal		m_RestTime4Change[3];
// 
// 	// Lightning
// 	RwRGBA		m_clrLightning;
// 
// 	RwReal		m_fTimeFadeIn;
// 	RwReal		m_fTimeFadeOut;
// 	RwReal		m_fTimeLightning;
// 
// public:
// 	virtual RwBool	Create(const SPLEntityCreateParam* pParam = NULL);
// 	virtual void	Destroy();
// 	virtual void	HandleEvents(RWS::CMsg &pMsg);
// 
// 	virtual	void	UpdateFieldVariation(RwReal fElapsed);
// 	virtual RwBool	Update(RwReal fElapsed);
// 	virtual RwBool	Render(void);
// 
// 
// 	virtual RwBool				SetProperty(const CNtlPLProperty *pData);
// 	virtual void				SetPosition(const RwV3d *pPosition) { if(pPosition) memcpy(&m_WorldDatumPos, pPosition, sizeof( RwV3d ) ); }
// 	//virtual RpAtomic*			GetAtomic();
// 	virtual ENTITY_ATOMIC_VEC	GetAtomicList();
// 	virtual eBASE_SKY_MODE		GetBaseSkyMode();
// 	virtual void				RunableLightning(RwReal fTimeFadeIn, RwReal fTimeFadeOut);
// 
// public:
// 	RwBool m_VisibilityFlag;
// 
// protected:
// 	virtual void			CreateSkyLayer(sSKY_LAYER** ppSkyLayer, const char *pStrName, const char * pResourcePath, RwUInt32 eGeoFlag, RwBool bRender, RwBool bClearTextrue);
// 	virtual void			DestroySkyLayer(sSKY_LAYER** ppSkyLayer);
// 
// 	virtual void			SetSkyLayerDiffuseColor(sSKY_LAYER* pSkyLayer, RwRGBA rgba[3]);
// 	virtual void			SetSkyLayerTextrue(sSKY_LAYER* pSkyLayer, RwTexture* pTexture);
// 	virtual void			SetSkyLayerAlpha(sSKY_LAYER* pSkyLayer, RwUInt8 uiAlpha);
// 
// 	virtual void			SkyLayerTranslation(sSKY_LAYER* pSkyLayer, RwV3d* pvPos, RwOpCombineType rwOpCombineType);
// 	virtual void			SkyLayerRotation(sSKY_LAYER* pSkyLayer, RwReal fRot, RwOpCombineType rwOpCombineType);
// 	virtual void			SkyLayerRender(sSKY_LAYER* pSkyLayer, RwInt32 iTFactor);
// 
// 
// 	RwBool					bRenderSky[30];
// 
// };