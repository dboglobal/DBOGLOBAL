#pragma once

#include "NtlPLObject.h"
#include "MTCharacterInfo.h"

#define OBJECT_MESH_PATH    ".\\Object\\Mesh\\"
#define OBJECT_TEXTURE_PATH ".\\Texture\\Object\\;"

class CMTObject : public CNtlPLObject
{
public:
    CMTObject(void);
    virtual ~CMTObject(void);

    void* operator new(size_t size);
    void operator delete(void *pObj);

    virtual RwBool LoadClump(RwChar* szFileName);
    virtual RwBool Create(const SPLEntityCreateParam * pParam  = NULL);
    virtual RwBool Update(RwReal fElapsed);
    virtual void   SetVisible(BOOL bVisible);
    virtual int	   CallBackBaseAnim(void* pEventData);	            	///< Animation Event의 CallBack 함수    
    virtual RwBool SetTriggerAnimation(RwUInt32 uiAnimKey, RwReal fStartTime = 0.0f, RwBool bLoop = TRUE);				///< Trigger Animation을 재생한다.
    virtual void   SetWeightAlpha(RwReal fWeightValue);							///< Weight Alpha


    CMTClump*      GetClumpInfo() {return &m_MTClump;}                      ///< CMTClump 객체를 반환한다.
    ENTITY_ATOMIC_VEC   GetAtomicList() {return m_vecAtomicList;}           ///< Atomic List를 반환한다.      (Alpha Event에서 Atomic Index 순서를 맞추기 위해서 추가)
	
	virtual const RwBBox* GetBoundingBox(void);
	void	RenderBBox();						                            ///< 설정된 BBox를 렌더링한다.
	void	RenderWireFrame();					                            ///< Object의 Wireframe을 렌더링 한다.

    // Link Effect 관련
    CNtlInstanceEffect* GetLinkEffectFromName(RwChar* szName);              ///< Name에 해당하는 LinkEffect를 반환한다.
    void    AddLoopSound(SOUND_HANDLE hSound) {m_listLoopSound.push_back(hSound);}             ///< LoopSound List에 SoundHandle을 추가한다.
    void    ClearLoopSound();                                                                  ///< LoopSound List에 들어있는 LoopSound들을 소멸한다.

    // 모델툴에서는 컬링을 하지 않는다.
    virtual RwBool CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame) {return FALSE;}
    virtual RwBool CullingTest(RwCamera* pRwCamera) {return FALSE;}
	
protected:
    void    CalcBBox();                                                     ///< 기본 BBox를 계산해서, Property에 세팅한다.    

    virtual void OnEventAlphaFade(SEventAlpha* pEventAlpha);                ///< Model Tool에서 Alpha Fade는 자기 객체가 처리한다.    

protected:
    CMTClump m_MTClump;                         ///< Clump 관련 객체   
};

