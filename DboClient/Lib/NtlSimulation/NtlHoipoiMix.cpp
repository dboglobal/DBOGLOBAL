#include "precomp_ntlsimulation.h"
#include "NtlHoipoiMix.h"

// shared
#include "TableContainer.h"
#include "ItemRecipeTable.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLApi.h"


CNtlHoipoiMix::CNtlHoipoiMix()
{
	memset( m_asRecipeData, 0xFF, sizeof( sRECIPE_DATA ) * NTL_ITEM_RECIPE_MAX_COUNT );
	::ZeroMemory( &m_sHoipoiMixData, sizeof( sHOIPOIMIX_DATA ) );
	m_byRecipeCount = 0;
}

CNtlHoipoiMix::~CNtlHoipoiMix()
{
	
}

RwBool CNtlHoipoiMix::Create( void )
{
	LinkMsg( g_EventHoipoiMixRecipeRegNfy, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventHoipoiMixItemRecipeInfo, (const RwChar*)0, 0x9000 );

	SAvatarInfo* pInfo = GetNtlSLGlobal()->GetAvatarInfo();
	memcpy( &m_sHoipoiMixData, &pInfo->sCharPf.sMixData, sizeof( sHOIPOIMIX_DATA ) );

	SAvatarRecipeInfo* pRecipeInfo = GetNtlSLGlobal()->GetAvatarRecipeInfo();

	memset( m_asRecipeData, 0xFF, sizeof( sRECIPE_DATA ) * NTL_ITEM_RECIPE_MAX_COUNT );
	memcpy( m_asRecipeData, pRecipeInfo->asRecipeData, sizeof( sRECIPE_DATA ) * pRecipeInfo->byCount );

	return TRUE;
}

void CNtlHoipoiMix::Destroy()
{
	UnLinkMsg( g_EventHoipoiMixRecipeRegNfy );
	UnLinkMsg( g_EventHoipoiMixItemRecipeInfo );
}

void CNtlHoipoiMix::HandleEvents( RWS::CMsg &msg)
{
	if( msg.Id == g_EventHoipoiMixRecipeRegNfy )
	{
		SNtlEventHoipoiMixRecipeRegNfy* pNotify = reinterpret_cast<SNtlEventHoipoiMixRecipeRegNfy*>(msg.pData);

		/*sITEM_RECIPE_TBLDAT* pTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( pNotify->idxRecipeTbl );
		NTL_ASSERT( pTblDat, "CNtlHoipoiMix::HandleEvents - Cannot find ITEM RECIPE TABLE DATA" << pNotify->idxRecipeTbl );
		
		if( m_byRecipeCount+1 < NTL_ITEM_RECIPE_MAX_COUNT )
		{
			m_asRecipeData[m_byRecipeCount].byRecipeType = pTblDat->byRecipeType;
			m_asRecipeData[m_byRecipeCount].recipeTblidx = pNotify->idxRecipeTbl;
			++m_byRecipeCount;
		}*/
	}
	else if( msg.Id == g_EventHoipoiMixItemRecipeInfo )
	{
		SNtlEventHoipoiMixItemRecipeInfo* pInfo = reinterpret_cast<SNtlEventHoipoiMixItemRecipeInfo*>(msg.pData);

		if( pInfo->byCount > 0 )
		{
			memset( m_asRecipeData, 0xFF, sizeof( sRECIPE_DATA ) * NTL_ITEM_RECIPE_MAX_COUNT );
			memcpy( m_asRecipeData, pInfo->pRecipeData, sizeof( sRECIPE_DATA ) * pInfo->byCount );
		}
	}
}

RwInt32 CNtlHoipoiMix::GetRecipeCount()
{
	return m_byRecipeCount;
}

sRECIPE_DATA* CNtlHoipoiMix::GetRecipeData()
{
	return m_asRecipeData;
}

sHOIPOIMIX_DATA* CNtlHoipoiMix::GetHoipoiMixData()
{
	return &m_sHoipoiMixData;
}