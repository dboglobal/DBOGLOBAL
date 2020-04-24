/******************************************************************************
* File			: RwTextureGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 5. 20
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: RwTexture의 전체 이미지를 랜더링한다
*				  SetTexture시 RwTexture를 직접 인자로 넘겨준 경우
*				  생성,삭제는 책임지지 않는다
*****************************************************************************/

#pragma once

// gui
#include "rectangle.h"

class CRwTextureGui
{
public:
	CRwTextureGui();
	virtual ~CRwTextureGui();

	VOID			SetTexture(RwTexture* pTexture);
	VOID			SetTexture(RwChar* pcFile);
	VOID			UnsetTexture();

	VOID			SetAlpha(RwUInt8 byAlpha);
	VOID			SetPositionfromParent(RwInt32 iPosX, RwInt32 iPosY);
	VOID			SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY);
	VOID			SetSize(RwInt32 iWidth, RwInt32 iHeight);

	RwInt32			GetWidth();
	RwInt32			GetHeight();
	RwInt32			GetPositionX();
	RwInt32			GetPositionY();
	CRectangle		GetPosition();
	RwTexture*		GetTexture();

	VOID			Show(bool bShow);
	VOID			Render();

	RwBool			PtInRect(int iScreenX, int iScreenY);
	RwBool			IsSelfCreatedTexture();

protected:
	VOID			CalcVertex();

protected:
	RwTexture*			m_pTexture;
	RwIm2DVertex		v2dVertices[4];

	RwInt32				m_iParentX;
	RwInt32				m_iParentY;
	RwInt32				m_iX_fromParent;
	RwInt32				m_iY_fromParent;
	RwInt32				m_iWidth;
	RwInt32				m_iHeight;

	RwUInt8				m_byAlpha;
	RwBool				m_bShow;
	RwBool				m_bSelfCreatedTexture;
};