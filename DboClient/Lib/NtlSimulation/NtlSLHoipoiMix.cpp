#include "precomp_ntlsimulation.h"
#include "NtlSLHoipoiMix.h"

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


CNtlSLHoipoiMix::CNtlSLHoipoiMix()
{
}

CNtlSLHoipoiMix::~CNtlSLHoipoiMix()
{
	
}

RwBool CNtlSLHoipoiMix::Create( void )
{
	LinkMsg( g_EventHoipoiMixRecipeRegNfy, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventHoipoiMixItemRecipeInfo, (const RwChar*)0, 0x9000 );
	LinkMsg( g_EventHoipoiMixItemMakeExpNfy, (const RwChar*)0, 0x9000 );

	SAvatarInfo* pInfo = GetNtlSLGlobal()->GetAvatarInfo();
	memcpy( &m_sHoipoiMixData, &pInfo->sCharPf.sMixData, sizeof( sHOIPOIMIX_DATA ) );

	SAvatarRecipeInfo* pRecipeInfo = GetNtlSLGlobal()->GetAvatarRecipeInfo();

	for( WORD i=0; i < pRecipeInfo->wCount; ++i )
		AddRecipeData( pRecipeInfo->asRecipeData[i].recipeTblidx, pRecipeInfo->asRecipeData[i].byRecipeType );

	return TRUE;
}

void CNtlSLHoipoiMix::Destroy()
{
	ClearAllRecipeData();

	UnLinkMsg( g_EventHoipoiMixRecipeRegNfy );
	UnLinkMsg( g_EventHoipoiMixItemRecipeInfo );
	UnLinkMsg( g_EventHoipoiMixItemMakeExpNfy );
}

void CNtlSLHoipoiMix::HandleEvents( RWS::CMsg &msg)
{
	if( msg.Id == g_EventHoipoiMixRecipeRegNfy )
	{
		SNtlEventHoipoiMixRecipeRegNfy* pNotify = reinterpret_cast<SNtlEventHoipoiMixRecipeRegNfy*>(msg.pData);

		sITEM_RECIPE_TBLDAT* pTblDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( pNotify->idxRecipeTbl );
		NTL_ASSERT( pTblDat, "CNtlHoipoiMix::HandleEvents - Cannot find ITEM RECIPE TABLE DATA" << pNotify->idxRecipeTbl );

		AddRecipeData( pNotify->idxRecipeTbl, pTblDat->byRecipeType );
	}
	else if( msg.Id == g_EventHoipoiMixItemRecipeInfo )
	{
		SNtlEventHoipoiMixItemRecipeInfo* pInfo = reinterpret_cast<SNtlEventHoipoiMixItemRecipeInfo*>(msg.pData);

		ClearAllRecipeData();

		for( WORD i=0; i < pInfo->wCount; ++i )
		{
			sRECIPE_DATA* pRecipe = reinterpret_cast<sRECIPE_DATA*>( pInfo->pRecipeData );
			AddRecipeData( pRecipe[i].recipeTblidx, pRecipe[i].byRecipeType );
		}
	}
	else if( msg.Id == g_EventHoipoiMixItemMakeExpNfy )
	{
		SNtlEventHoipoiMixItemMakeExpNfy* pNotify = reinterpret_cast< SNtlEventHoipoiMixItemMakeExpNfy* >( msg.pData );

		m_sHoipoiMixData.dwExp = pNotify->dwCurExp;

		if (m_sHoipoiMixData.byLevel < pNotify->byCurLevel)
		{
			CNtlSLEventGenerator::HoipoiMixInfoUpdate(eHOIPOIMIX_INFO_LEVELUP, (RwUInt32)pNotify->byCurLevel);

			m_sHoipoiMixData.byLevel = pNotify->byCurLevel;
		}
	}
}

RwBool CNtlSLHoipoiMix::AddRecipeData( RwUInt32 idxTbl, RwUInt8 byType )
{
	MAPDEF_RECIPEDATA::iterator it = m_mapRecipeData.find( idxTbl );
	if( it == m_mapRecipeData.end() )
	{
		sRECIPE_DATA* pData = NTL_NEW sRECIPE_DATA;
		pData->recipeTblidx = idxTbl;
		pData->byRecipeType = byType;

		m_mapRecipeData.insert( std::make_pair( idxTbl, pData ) );

		return TRUE;
	}
	
	return FALSE;
}

RwInt32 CNtlSLHoipoiMix::GetRecipeCount()
{
	return (RwInt32)m_mapRecipeData.size();
}

sHOIPOIMIX_DATA* CNtlSLHoipoiMix::GetHoipoiMixData()
{
	return &m_sHoipoiMixData;
}

void CNtlSLHoipoiMix::ClearRecipeData(BYTE byRecipeType)
{
	MAPDEF_RECIPEDATA::iterator it = m_mapRecipeData.begin();
	while( it != m_mapRecipeData.end() )
	{
		if( (*it).second->byRecipeType == byRecipeType)
		{
			NTL_DELETE( (*it).second );
			it = m_mapRecipeData.erase( it );
		}
		else
		{
			it++;
		}
	}
}

void CNtlSLHoipoiMix::ClearAllRecipeData()
{
	MAPDEF_RECIPEDATA::iterator it = m_mapRecipeData.begin();
	while (it != m_mapRecipeData.end())
	{
		NTL_DELETE((*it).second);
		it = m_mapRecipeData.erase(it);
	}
}
