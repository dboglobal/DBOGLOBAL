#pragma once

#include "NtlPLEntity.h"

enum ENTL_PL_GAME_PROPERTY_SHAPE
{
	ENTL_PL_GAME_PROPERTY_SHAPE_NONE	= 0,
	ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE,
	ENTL_PL_GAME_PROPERTY_SHAPE_PLANE,

	ENTL_PL_GAME_PROPERTY_SHAPE_NUM,
};

struct SPLGamePropertyCreateParam : public SPLEntityCreateParam
{
	RwUInt32		eShapeType;
	RwV3d			vSize;
	RwUInt32		uiProperty;
};

class CNtlPLGameProperty : public CNtlPLEntity
{
public:
	CNtlPLGameProperty();
	virtual ~CNtlPLGameProperty();

	virtual RwBool		SetProperty(const CNtlPLProperty *pData);

	virtual RwBool		Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void		Destroy();
	virtual RwBool		Update(RwReal fElapsed);
	virtual RwBool		Render();

	virtual void		SetShapeType(RwUInt32 eShapeType)	{ m_eShapeType = eShapeType; }
	virtual void		SetPosition(const RwV3d *pPos)		{ m_vPos = *pPos; }
	virtual void		SetSize(const RwV3d *pSize)			{ m_vSize = *pSize; }	
	virtual void		SetGameProperty(RwUInt32 uiProperty){ m_uiProperty = uiProperty; }

	virtual RwUInt32	GetShapeType()						{ return m_eShapeType; }
	virtual RwV3d		GetPosition()						{ return m_vPos; }
	virtual RwV3d		GetSize()							{ return m_vSize; }
	virtual RwUInt32	GetGameProperty()					{ return m_uiProperty; }

protected:
	RwUInt32		m_eShapeType;
	RwV3d			m_vPos;
	RwV3d			m_vSize;			// CIRCLE로 사용 시 m_vSize.x가 Radius를 뜻한다.
	RwUInt32		m_uiProperty;

#ifdef dNTL_WORLD_TOOL_MODE
	RpAtomic*		m_pAtomicCtlPt;
#endif

#ifdef dNTL_WORLD_TOOL_MODE
	static CNtlPLGameProperty* s_pPLGamePropertySelected;
#endif

#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void					SetSelectGameProperty(CNtlPLGameProperty* pPLGameProperty)	{ s_pPLGamePropertySelected = pPLGameProperty; }
	static CNtlPLGameProperty*	GetSelectGameProperty()										{ return s_pPLGamePropertySelected; }

	RwBool						Pick(RwReal& fDist);
#endif


};