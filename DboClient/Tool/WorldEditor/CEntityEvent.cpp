//*****************************************************************************
// File       : CEntityEvent.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CLightEntity.h"

#include "CEditorApplication.h"
#include "CSceneManager.h"

#include "CEntityEvent.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityEvent::Initialize( void )
{
	m_eType			= E_ENTITY_EVENT_TYPE_NONE;
	m_iUniqueID		= 0L;
	m_iNodeCount	= 0L;

	m_bActive		= FALSE;
	m_bLoop			= FALSE;

	m_pNodes		= NULL;
	m_pData			= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityEvent::Create( void )
{
	return FALSE;
}



RwBool CEntityEvent::CreateFadeInOut( RwUInt32 iUniqueID, RwReal fValue, RwReal fBeginTime, RwReal fInTime, RwReal fKeepTime, RwReal fOutTime )
{
	m_eType					= E_ENTITY_EVENT_TYPE_LIGHT_COLOR;
	m_iUniqueID				= iUniqueID;
	m_iNodeCount			= 4;
	m_pNodes				= NTL_NEW SEntityEventNode[m_iNodeCount];

	RwRGBAReal *			pColor;

	m_pNodes[0].fTime		= fBeginTime;
	m_pNodes[0].eApplyType	= E_ENTITY_EVENT_APPLY_TYPE_DIRECT;
	pColor					= NTL_NEW RwRGBAReal;
	pColor->red				= 1.0f;
	pColor->green			= 1.0f;
	pColor->blue			= 1.0f;
	pColor->alpha			= 1.0f;
	m_pNodes[0].pData		= pColor;

	m_pNodes[1].fTime		= fBeginTime + fInTime;
	m_pNodes[1].eApplyType	= E_ENTITY_EVENT_APPLY_TYPE_OVER_TIME;
	m_pNodes[1].pData		= NTL_NEW RwRGBAReal;
	pColor					= NTL_NEW RwRGBAReal;
	pColor->red				= fValue;
	pColor->green			= fValue;
	pColor->blue			= fValue;
	pColor->alpha			= 1.0f;
	m_pNodes[1].pData		= pColor;

	m_pNodes[2].fTime		= fBeginTime + fInTime + fKeepTime;
	m_pNodes[2].eApplyType	= E_ENTITY_EVENT_APPLY_TYPE_OVER_TIME;
	m_pNodes[2].pData		= NTL_NEW RwRGBAReal;
	pColor					= NTL_NEW RwRGBAReal;
	pColor->red				= fValue;
	pColor->green			= fValue;
	pColor->blue			= fValue;
	pColor->alpha			= 1.0f;
	m_pNodes[2].pData		= pColor;

	m_pNodes[3].fTime		= fBeginTime + fInTime + fKeepTime + fOutTime;
	m_pNodes[3].eApplyType	= E_ENTITY_EVENT_APPLY_TYPE_OVER_TIME;
	m_pNodes[3].pData		= NTL_NEW RwRGBAReal;
	pColor					= NTL_NEW RwRGBAReal;
	pColor->red				= 1.0f;
	pColor->green			= 1.0f;
	pColor->blue			= 1.0f;
	pColor->alpha			= 1.0f;
	m_pNodes[3].pData		= pColor;

	return TRUE;
}




//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityEvent::Destroy( void )
{
	for( RwUInt32 iNodeIndex = 0; iNodeIndex < m_iNodeCount; ++iNodeIndex )
	{
		if( m_pNodes[iNodeIndex].pData != NULL )
		{
			NTL_DELETE( m_pNodes[iNodeIndex].pData );
		}
	}

	NTL_ARRAY_DELETE( m_pNodes );

	return;
}


//-----------------------------------------------------------------------------
// Name			: IsBegin
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityEvent::IsBegin( RwReal fCurrenTime ) const
{
	if( m_iNodeCount > 0 && m_pNodes != NULL )
	{
		if( m_pNodes[0].fTime <= fCurrenTime )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: IsEnd
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityEvent::IsEnd( RwReal fCurrenTime ) const
{
	if( m_iNodeCount > 0 && m_pNodes != NULL )
	{
		if( m_pNodes[m_iNodeCount - 1].fTime >= fCurrenTime )
		{
			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Update
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityEvent::Update( RwReal fCurrentTime )
{
	if( IsBegin( fCurrentTime ) == FALSE )
	{
		return FALSE;
	}

	if( IsEnd( fCurrentTime ) != FALSE )
	{
		if( IsActive() != FALSE )
		{
			if( m_pData !=- NULL )
			{
				NTL_DELETE( m_pData );
				m_pData = NULL;
			}

			Activate( FALSE );
		}

		return FALSE;
	}

	CEntity *		pEntity = g_pSceneManager->GetEntity( m_iUniqueID );

	if( pEntity == NULL )
	{
		return FALSE;
	}

	RwBool			bResult = FALSE;
	RwUInt32		iNodeBegin = 0;
	RwUInt32		iNodeEnd = 0;

	for( RwUInt32 iNodeIndex = 0; iNodeIndex < m_iNodeCount; ++iNodeIndex )
	{
		if( m_pNodes[iNodeIndex].fTime >= fCurrentTime )
		{
			iNodeEnd = iNodeIndex;

			if( iNodeEnd > 0 )
			{
				iNodeBegin = iNodeEnd - 1;
			}
			else
			{
				iNodeBegin = 0;
			}

			break;
		}
	}

	switch( GetType() )
	{
		case E_ENTITY_EVENT_TYPE_LIGHT_COLOR:
		{
			CLightEntity *		pLightEntity = reinterpret_cast<CLightEntity *>(pEntity);
			SColorFloat *		pOriginalColor;

			if( IsActive() == FALSE )
			{
				pOriginalColor = NTL_NEW SColorFloat;

				pOriginalColor->Set( pLightEntity->GetColor() );

				m_pData = pOriginalColor;

				Activate( TRUE );
			}
			else
			{
				Assert( m_pData != NULL );

				pOriginalColor = reinterpret_cast<SColorFloat *>(m_pData);
			}

			SColorFloat		ColorParam;

			if( m_pNodes[iNodeEnd].eApplyType == E_ENTITY_EVENT_APPLY_TYPE_DIRECT ||
				m_pNodes[iNodeEnd].fTime == m_pNodes[iNodeBegin].fTime )
			{
				ColorParam.Set( *(reinterpret_cast<SColorFloat *>(m_pNodes[iNodeEnd].pData)) );
			}
			else if( m_pNodes[iNodeEnd].eApplyType == E_ENTITY_EVENT_APPLY_TYPE_OVER_TIME )
			{
				RwRGBAReal *	pBeginValue = reinterpret_cast<RwRGBAReal *>(m_pNodes[iNodeBegin].pData);
				RwRGBAReal *	pEndValue = reinterpret_cast<RwRGBAReal *>(m_pNodes[iNodeEnd].pData);

				ColorParam.red = pBeginValue->red + (pEndValue->red - pBeginValue->red ) / (m_pNodes[iNodeEnd].fTime - m_pNodes[iNodeBegin].fTime) * (fCurrentTime - m_pNodes[iNodeBegin].fTime);
				ColorParam.green = pBeginValue->green + (pEndValue->green - pBeginValue->green ) / (m_pNodes[iNodeEnd].fTime - m_pNodes[iNodeBegin].fTime) * (fCurrentTime - m_pNodes[iNodeBegin].fTime);
				ColorParam.blue = pBeginValue->blue + (pEndValue->blue - pBeginValue->blue ) / (m_pNodes[iNodeEnd].fTime - m_pNodes[iNodeBegin].fTime) * (fCurrentTime - m_pNodes[iNodeBegin].fTime);
				ColorParam.alpha = pBeginValue->alpha + (pEndValue->alpha - pBeginValue->alpha ) / (m_pNodes[iNodeEnd].fTime - m_pNodes[iNodeBegin].fTime) * (fCurrentTime - m_pNodes[iNodeBegin].fTime);
			}

			SColorFloat		NewColor( *pOriginalColor );
			NewColor.Scale( ColorParam );
			pLightEntity->SetColor( &NewColor );

			bResult = TRUE;

			break;
		}
	}

	return bResult;
}


//*****************************************************************************
//
// End of File : CEntityEvent.cpp
//
//*****************************************************************************