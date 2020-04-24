//*****************************************************************************
// File       : CNtlPlFog.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_NTL_PL_FOG_H__
#define __C_NTL_PL_FOG_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NtlDebug.h"
#include "NtlPLEntity.h"
#include "ceventhandler.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

#define dFOG_EFFECT_NEAR				(50.0f)
#define dFOG_EFFECT_FAR					(CNtlPLGlobal::m_RwCamera->farPlane - 25.0f)
#define dFOG_EFFECT_SWITCHING_TIME		(3.0f)

//=============================================================================
// Name       : CNtlPlFog
// Desc       : 
// Author     : 
//=============================================================================

class CNtlPLFog : public CNtlPLEntity, public RWS::CEventHandler
{
public:
	CNtlPLFog( void ) { Initialize(); }
	virtual	~CNtlPLFog( void ) { Destroy(); }

protected:
	RwFogType	m_eType;
	RwRGBA		m_RGBA;
	RwBool		m_bSwitch;
	RwReal		m_Plane[2];
	RwRGBA		m_RGBA4DN;

	// Fog switching effect
	RwReal	m_RestTime4Change;
	RwRGBA	m_FogRGBA4Change;
	RwReal  m_FogPlanes[2];
	RwRGBA	m_FogRGBA4ChangeOld;
	RwReal  m_FogPlanesOld[2];

protected:
	void Initialize( void );

public:
	virtual RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	virtual void	Destroy( void );
	virtual void	HandleEvents(RWS::CMsg &pMsg);

	virtual RwBool	Update( RwReal fElapsed );
	virtual RwBool	Render( void );

	RwFogType		GetType( void ) const { return m_eType; }
	void			SetType( RwFogType eType ) { m_eType = eType; }
	virtual RwBool	SetProperty( const CNtlPLProperty * pData ) { return TRUE; }

	void	Switch();
	RwBool	IsWorking();
};


//=============================================================================
// End of Class : CNtlPlFog
//=============================================================================

#endif //__C_NTL_PL_FOG_H__

//*****************************************************************************
//
// End of File : CNtlPlFog.h
//
//*****************************************************************************