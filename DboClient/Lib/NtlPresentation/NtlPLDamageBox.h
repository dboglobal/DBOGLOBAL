/*****************************************************************************
 *
 * File			: NtlPLDamageBox.h
 * Author		: HyungSuk Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 11. 28	
 * Abstract		: Presentation layer damage box entity class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLDAMAGEBOX_H__
#define __NTL_PLDAMAGEBOX_H__

#include "NtlPLEntity.h"

struct SPLDamageBoxCreateParam : public SPLEntityCreateParam
{
	RwUInt32 uiDamage;
	RwInt32 iClolorType;
};

enum
{
	DAMAGEBOX_COLOR_PC,
	DAMAGEBOX_COLOR_MOB,
	DAMAGEBOX_COLOR_PC_HEAL,
	DAMAGEBOX_COLOR_MOB_HEAL,
	MAX_DAMAGEBOX_COLOR
};

enum
{
	MAX_DAMAGEBOX_IMAGE_COUNT = 10	
};


class CNtlPLDamageBox : public CNtlPLEntity
{
private:

	static RwTexture *m_ppTexture[MAX_DAMAGEBOX_COLOR][MAX_DAMAGEBOX_IMAGE_COUNT];

	RwReal m_fLifeTime;
	RwInt32 m_iImageCount;
	RwUInt8 m_byColorType;
	RwUInt8 m_byImageIdx[10];

	RwV3d m_vCurrPos;
	RwV3d m_vPos[MAX_DAMAGEBOX_IMAGE_COUNT][4];
	RwIm3DVertex m_Vertices[MAX_DAMAGEBOX_IMAGE_COUNT][4];
	RwMatrix m_matWorld;

private:

	void VertexAssin(void);

public:

	CNtlPLDamageBox();
	virtual ~CNtlPLDamageBox();

	static void CreateResource(void);
	static void DestroyResource(void);

	RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	void Destroy(void);
	RwBool Update(RwReal fElapsed);
	RwBool Render(void);
	RwBool SetProperty(const CNtlPLProperty *pData);
	
	void* operator new(size_t size);
	void operator delete(void *pObj);
};



#endif