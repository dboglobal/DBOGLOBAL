#pragma once
#include "rtnormmap.h"
#include "rpnormmap.h"

#define  FAST_MODE

/**
 * \ingroup ModelTool2
 * \brief Normal Map과 Env Map을 생성, 적용하는 클래스
 * \date 2006-06-28
 * \author agebreak
 */
class CNormEnvMap
{
public:
    CNormEnvMap(void);
    virtual ~CNormEnvMap(void);

    static void SetEnableNormMap(RpClump* pClump, RwBool bEnable);                    ///< 노말맵, EnvMap을 적용한다.
    static void SetNormMap(RpMaterial* pMaterial, RwBool bEnable);                    ///< 노말맵을 적용한다.
    static void SetEnvMap(RpMaterial* pMaterial, RwTexture* pTexture);                ///< EnvMap을 적용한다.
    static void SetEnvMapShininess(RpMaterial* pMaterial, RwReal fShininess);         ///< Shininess를 설정한다.

protected:
    static RwTexture* NormalMapTextureSpaceCreateFromTexture(RwTexture *texture, RwReal bumpness);
    static RwRaster*  NormalMapTextureSpaceCreateFromImage(RwImage *image, RwUInt32 rasterFlags, RwBool clamp, RwReal bumpness);
    static RwBool IsAnim(RpClump* pClump);                                            ///< Anim이 적용됐는지 안됐는지를 반환한다.
};
