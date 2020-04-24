/*****************************************************************************
*
* File			: NtlPLDecal.h
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2006. 6. 15	
* Abstract		: Presentation layer decal entity class
*****************************************************************************
* Desc         : Effect및 World에서 사용되는 Decal 클래스. (그림자 표현에도 사용된다)
*
*****************************************************************************/

#ifndef __NTL_PLDECAL_H__
#define __NTL_PLDECAL_H__

#include "NtlPLAttach.h"
#include "NtlEffectDefine.h"

/// Decal의 종류
enum EDecalType
{
	DECAL_EFFECT,					///< Effect용 Decal
	DECAL_TERRAIN,					///< 지형용 Decal
};

// Decal Effect에서만 쓰는 플래그
enum EDecalEffectFlag
{
	E_FLAG_DECAL_RENDER_OBJECT = 0x00100000,					///< 오브젝트에 Decal을 랜더
	E_FLAG_DECAL_RENDER_WORLD  = 0x00200000,					///< World에 Decal을 렌더
	E_FLAG_DECAL_WATER		   = 0x00400000,					///< 물 위에만 나타나는 Decal이라면 이 Flag를 켜준다 (기본값 False)
	E_FLAG_DECAL_DAYNIGHT 	   = 0x00800000						///< Day&Night의 영향을 받는 Decal이라면 이 Flag를 켜준다 (기본값 False)
};


/// Object에 Decal을 그리기 위해 콜백함수에게 넘겨주는 파라미터 구조체
struct DecalCallbackParam
{
	RwV3d vPos;
	RwMatrix *matTrans;
	RwMatrix *matAtomicTrans;
	RwInt32 *pBufferCnt;
	RwIm3DVertex *pVertices;

	float fIntersectionRadius;
	float fScale;
	RwInt32 nMaxVertextCnt;				///< 생성될 Decal의 최대 Vertex 개수
	RwReal fYOffset;
};

class CNtlPLWorldEntity;

struct SPLDecalCreateParam : public SPLEntityCreateParam
{
	RwInt32	nTexNameLen;
	const RwChar *pTexName;
	const RwChar *pTexPath;
	RwReal fIntersectionRadius;
	RwV3d  vSize;								///< Decal's Size (use only X, Z values)
	RwReal fScale;
	RwReal fVisibleSquaredDist;					///< The square of the visual distance the decal is visible
	RwReal fYOffset;							///< Y Offset value (Decal height from ground)
	RwTextureAddressMode rwWrapType;			///< Texture Adress Mode
	RwInt32	nFlagEffect;						///< Effect flags
	EDecalType eDecalType;						///< Decal Type

	SPLDecalCreateParam()
	: rwWrapType(rwTEXTUREADDRESSCLAMP), fScale(1.0f), fIntersectionRadius(3.0f), 
	  fVisibleSquaredDist(1600.0f), fYOffset(0.1f)
	{ 
		vSize.x = vSize.y = vSize.z = 1.0f;
		pPos = NULL;
		nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_RENDER_WORLD;		
		eDecalType = DECAL_EFFECT;
	}
};

class CNtlPLDecal : public CNtlPLAttach
{
public:

	CNtlPLDecal();
	virtual ~CNtlPLDecal();

	void* operator new(size_t size);
	void operator delete(void *pObj);

	virtual RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	virtual RwBool SetProperty(const CNtlPLProperty *pData);
	virtual void Destroy(void);
	virtual RwBool Update(RwReal fElapsed);
	virtual RwBool Render(void);
	
	virtual void SetNameLen(RwInt32 len) { m_iNameLen = len; }
	virtual void SetPosition(const RwV3d *pPos);	
	virtual void SetAlpha(RwUInt8 byAlpha);
	virtual void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	virtual void SetScale(RwReal fScale);
    virtual void SetScaleSize(RwReal fScale);
	virtual void SetSize(RwReal fXSize, RwReal fZSize);
	virtual void SetRotate(RwReal fDegree);															///< Decal을 회전시킨다. Y축을 기준으로만 회전한다.Rotate Action이 적용되어 있는 경우엔 적용되지 않는다.
	virtual void SetMatrix(RwMatrix& matWorld);														///< World Matrix를 적용한다. 
	virtual void SetYOffset(RwReal fYOffset) {m_fYOffset = fYOffset;}								///< Decal의 YOffset 값(지면으로부터 얼마 위에 그릴것인가)을 설정한다

	virtual RwInt32	GetNameLen() { return m_iNameLen; }

	virtual RwV3d	GetPosition() {return m_vPos;}
	virtual RwRGBA	GetColor() {return m_color;}
	virtual RwReal	GetScale() {return m_fScale;}
	virtual RwReal	GetBaseScale() { return m_fStartScale; }
	virtual RwV3d	GetSize() {return m_vSize;}
	virtual RwReal	GetRotate() {return m_fDegree;}
	virtual RwMatrix& GetMatrix() {return m_matWorld;}
	virtual RwReal	GetYOffset() {return m_fYOffset;}
	virtual RwReal  GetVisibleDist() { return m_fVisibleSquaredDist; }
	virtual SNtlPrtStdEmitterPrtUVAnim& GetUVChunk() { return m_EmitterPrtUVAnim; }
    virtual RwUInt32 GetMemoryUseSize() {return m_uiMemoryUseSize;}                                 ///< 메모리 사용량을 반환한다.

	void			GetRotateActionValue(RwReal& fAngle, RwReal& fTimeGap);
	void			GetScaleActionValue(RwReal& _StartScale, RwReal& _EndScale, RwReal& _TimeGap);
	
	void SetLifeTime(RwReal fLifeTime) {m_fLifeTime = fLifeTime;}									///< Decal의 LifeTime을 설정한다. (영구 Decal의 경우 0을 설정)
	
	void SetAlphaBlend(RwBool bBlend, RwBlendFunction srcBlend, RwBlendFunction destBlend);			///< 알파 블렌드를 설정한다.
	RwInt32	GetVerexCount() {return m_nRenderVertexCnt;}											///< 현재 그려지고 있는 버텍스의 개수를 반환한다.

	// Effect Flag
	RwInt32	GetEffectFlag() {return m_nFlagEffect;}
	void	SetEffectFlag(RwInt32 nFlag) {m_nFlagEffect = nFlag;}

	// Action
	void	Set2DRoateAction(RwReal fAngle, RwReal fTimeGap = 1.0f);						///< 회전 액션을 적용한다. (1초에 걸쳐 회전하는 각도를 설정한다 (각도는 Degree이다.))
	void	SetColorAction(const RwRGBA& colStart, const RwRGBA& colEnd, RwReal fTimeGap);	///< Color Action을 적용한다.
	void	SetMultiColorAction(const RpPrtAdvPrtMultiColorEmitter& emitterPrtMulticol);		///< Multi Color Action을 적용한다.
	void	SetSizeAction(RwReal fStartScale, RwReal fEndScale, RwReal fTimeGap);			///< Size Action을 적용한다.
	void	SetMultiSizeAction(const SNtlAdvMultiSizeEmitter& emitterPrtMultiSize);			///< Multi Size Action을 적용한다.
	void	SetMultiTextureAction(const RpPrtAdvEmtPrtMultiTextures& emitterMultiTexture);	///< Multi Texture Action을 적용한다.
	void	SetUVAnimAction(const SNtlPrtStdEmitterPrtUVAnim& emitterUVAnim);				///< UV Animation을 적용한다.
    void    SetUpdateVertex(RwBool bUpdate) {m_bVertexUpdate = bUpdate;}                    ///< 버텍스를 강제 업데이트할때 플래그를 바꾸어준다.

protected:
	RwBool	UpdateVertices();						///< Decal Vertex의 정보를 갱신한다.
	void	SetVertexColor(const RwRGBA& color);	///< Vertex들의 Color를 설정한다.	
	void	UpdateUV();								///< Vertex들의 UV값을 Update한다
	
	void	UpdateRoate(RwReal fElapsedTime);		///< Rotate Action 값을 적용한다.
	void	UpdateColor(RwReal fElapsedTime);		///< Color Action 값을 적용한다.
	void	UpdateScale(RwReal fElapsedTime);		///< Scale Action 값을 적용한다.
	void	UpdateMultiTexture(RwReal fElapsedTime);///< Multi Texture Action 값을 적용한다.
	void	UpdateMultiColor(RwReal fElapsedTime);	///< Multi Color Action 값을 적용한다.
	void	UpdateMultiSize(RwReal fElapsedTime);	///< Multi Size Action 값을 적용한다. (size는 X값만 적용한다)
	void	UpdateUVAnim(RwReal fElapsedTime);		///< UV Anim Action 값을 적용한다.
	void	UpdateFade(RwReal fElapsedTime);		///< Fade In/Out을 Update한다.

	void	GetObjectDecalVertex();					///< Object에 그려지는 Decal을 계산한다.		
	RwBool	GetWaterDecalVertex();					///< 물위에 그려지는 Decal을 계산한다.
	RwBool	IsVisble();								///< 카메라 컬링과 Visible Distance를 계산해서 보여질지 유무를 반환한다.
    void    ChangeBigVB();                          ///< 버텍스 버퍼가 부족한 경우에 큰 버퍼로 변경한다.

	void	SetTextureMatrix(RwIm3DVertex* pVertices, RwInt32 nVertCnt, RwMatrix* pMatrix);	///< Vertex들의 UV에 Texture Matrix를 적용한다.

protected:

	RwInt32	m_iNameLen;

    RwInt32 m_nMaxVertexCnt;
	RwV3d m_vPos;
	RwV3d m_vSize;
	RwReal m_fIntersectionRadius;
	RwReal m_fScale;
	RwReal m_fVisibleSquaredDist;
	RwTextureAddressMode m_rwWrapType;				///< 텍스쳐의 Wrap Type
	RwMatrix m_matWorld;							///< World Matrix
	RwReal m_fYOffset;								///< Y Offset 값
	RwBool m_bVertexUpdate;							///< Vertex를 Update할지 유무 플래그
	RwV3d  m_vOriginPos;							///< 현재 생성된 World Vertex의 중점 위치
	RwReal m_fRadiusWorldVertex;					///< 현재 생성된 World Vertex의 반지름
	RwReal m_fDegree;								///< Rotation값 (Degree)
	EDecalType m_eDecalType;						///< DecalType

	RwTexture* m_pTexture;
	RwTexture* m_pCurrentTexture;
	RwTexture** m_ppMultiTextures;
	RwRGBA m_color;	

	// Render State
	RwBool				m_bBlend;			///< 알파 블렌딩 유무
	RwBlendFunction		m_blendSrc;			///< 소스 블렌드
	RwBlendFunction		m_blendDest;		///< 타겟 블렌드

	RwIm3DVertex*		m_pIm3DBuffer;		///< Vertex Buffer 
	RwInt32				m_nRenderVertexCnt;	///< 렌더링될 Vertex의 개수	

	// Emitter Action
	RwInt32				m_nFlagEffect;		///< 이펙트 속성 Flag
	RwReal				m_fTotalElapsedTime;///< 생성되서 지금까지의 시간
	RwReal				m_fElapsedTime;		///< 한 프레임의 타임
	RwReal				m_fLifeTime;		///< Decal의 Life Time

	RwReal				m_f2DRoateAngle;	///< 2D Roate Action시 회전하는 각도
	RwBool				m_bFirstRotate;		///< 로테이트시 텍스쳐의 중점을 변경을 체크하기 위해서 사용하는 변수
	RwReal				m_fOrgRotateAngle;	///< 원래 설정된 Rotate Angle값
	RwReal				m_fRotateTimeGap;	///< 설정된 Rotate Time Gap

	RwMatrix			m_matTexture;		///< UV 좌표 변경에 사용되는 텍스쳐 매트릭스

	RwRGBA				m_colStart;			///< Color Action에 사용되는 Start Color
	RwRGBA				m_colEnd;			///< Color Action에 사용되는 End Color
	RwReal				m_fColorActionTime;	///< Color Action에 사용되는 Time

	RwReal				m_fStartScale;		///< Size Aciton에 사용되는 Start Scale
	RwReal				m_fEndScale;		///< Scale Action에 사용되는 End Scale;
	RwReal				m_fScaleActionTimeGap; ///< Scale Action 사용되는 Time

	RwUInt32			m_nCurrentTexture;		  ///< Multi Texture Action에서 현재 렌더링 되는 텍스쳐의 인덱스 번호
	RwReal				m_fMultiTextureDeltaTime; ///< Multi Texture Acion에 사용되는 Time

	RwRGBA				m_colPrev;				  ///< Multo Color Action에 사용되는 이전의 Color
	RwUInt32			m_nNextColorIndex;		  ///< Multi Color Action에 사용되는 Index
	RwReal				m_fMultiColorDeltaTime;	  ///< Multi Color Action에 사용되는 Time

	RwReal				m_fPrevSize;			  ///< Multi Size Action에 사용되는 이전의 Size
	RwUInt32			m_nNextSizeIndex;		  ///< Multi Size Action에 사용되는 Index
	RwReal				m_fMultiSizeDeltaTime;	  ///< Multi Size Action에 사용되는 Time

	RpPrtAdvPrtMultiColorEmitter	m_EmitterPrtMultiCol;		///< Multi Color Action 정보를 담고 있는 구조체
	RpPrtAdvEmtPrtMultiTextures		m_EmitterMultiTextures;		///< Multi Texture Action 정보 구조체
	SNtlAdvMultiSizeEmitter			m_EmitterPrtMultiSize;		///< Multi Size Action 정보 구조체
	SNtlPrtStdEmitterPrtUVAnim		m_EmitterPrtUVAnim;			///< UV Anim Action 정보 구조체

	EFadeStatus			m_eFadeStatus;			  ///< Fade 상태 변수 (지형 Decal인 경우에만 사용된다)
	RwInt32 			m_uiFadeAlpha;			  ///< Fade시 사용되는 Alpha 
	RwReal				m_fFadeGap;

    RwUInt32            m_uiMemoryUseSize;        ///< 메모리 사용량

	static const RwReal		m_cfFadeTime;
};

#endif
