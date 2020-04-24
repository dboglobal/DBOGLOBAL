/*****************************************************************************
 *
 * File			: NtlPLEmblem.h
 * Author		: Hong SungBock
 * Copyright	: (аж)NTL
 * Date			: 20057. 5. 3
 * Abstract		: Presentation layer emblem
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLEMBLEM_H__
#define __NTL_PLEMBLEM_H__

#include "NtlPLAttach.h"
#include "NtlPLGlobal.h"

#include "gui_texture.h"

/*
struct SPLEmblemCreateParam : public SPLEntityCreateParam
{
	RwReal fCullDist;
};


class CNtlPLEmblem : public CNtlPLAttach
{
private:
	
	RwBool m_bCameraCullIn;
	RwReal m_fCullDist;
	gui::CTexture *m_pTexture;
	
	RwV3d m_v3Pos[4];
	RwIm3DVertex m_3dVertices[4];
	RwMatrix m_matWorld;

private:

	void VertexAssin(void);

public:

	CNtlPLEmblem();
	virtual ~CNtlPLEmblem();

	RwBool Create( const SPLEntityCreateParam * pParam = NULL );
	void Destroy(void);
	RwBool Update(RwReal fElapsed);
	RwBool Render(void);
	RwBool SetProperty(const CNtlPLProperty *pData) { return TRUE;}

	void SetAlpha(RwUInt8 byValue);

	
	void* operator new(size_t size);
	void operator delete(void *pObj);

public:

	void SetPosition(const RwV3d *pPos);
	RwV3d GetPosition(void);
};
*/
#endif