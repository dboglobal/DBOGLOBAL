/*****************************************************************************
 *
 * File			: NtlDecalManager.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 12. 12 (Prototype 형석씨가 만든거 복사)	
 * Abstract		: Decal Shadow Class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _NTL_DECAL_MANAGER_
#define _NTL_DECAL_MANAGER_

#include "rwcore.h"
#include "rpworld.h"
#include "rpcollis.h"

#define MAX_DECAL_IM3D	5000

class CNtlDecalManager
{	
private:

	RwMatrix m_matTrans;
	RwUInt32 m_nIm3DBufferCnt;
	RwIm3DVertex m_pIm3DBuffer[MAX_DECAL_IM3D];

public:

	CNtlDecalManager();
	~CNtlDecalManager();

	RwBool Create(void);
	void Destroy(void);
	void RwRender(RpWorld *pWorld, RwCamera *pCamera, RwV3d *pPos,
					RwReal fIntersectionRadius, RwReal fScale, RwReal fVisibleSquaredDist, RwTexture *pTexture);
	void TerrainRender(RpWorld *pWorld, RwCamera *pCamara, const RwV3d& pPos, const RwV3d& size, RwReal fVisibleSquaredDist, RwTexture *pTexture);

	void SetAlpha(RwUInt8 byAlpha);
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
};

CNtlDecalManager &GetDecalManager();

#endif