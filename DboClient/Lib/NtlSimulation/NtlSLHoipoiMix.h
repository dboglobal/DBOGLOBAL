/*****************************************************************************
 *
 * File			: NtlHoipoiMix.h
 * Author		: HaeSung, Cho
 * Copyright	: (аж)NTL
 * Date			: 2009. 1. 12
 * Abstract		: Simulation HoipoiMix Recipe & Level data container
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_HOIPOI_MIX_H__
#define __NTL_HOIPOI_MIX_H__

// shared
#include "NtlItem.h"

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLServerDeclear.h"

class CNtlSLHoipoiMix : public RWS::CEventHandler
{
public:
	typedef std::map< RwUInt32, sRECIPE_DATA* > MAPDEF_RECIPEDATA;

	CNtlSLHoipoiMix();
	virtual ~CNtlSLHoipoiMix();

	virtual RwBool		Create(void);
	void				Destroy();

	virtual void		HandleEvents(RWS::CMsg &msg);

	RwBool				AddRecipeData( RwUInt32 idxTbl, RwUInt8 byType );

	RwInt32				GetRecipeCount();
	MAPDEF_RECIPEDATA*	GetRecipeData()			{ return &m_mapRecipeData; }	
	sHOIPOIMIX_DATA*	GetHoipoiMixData();
	
	RwUInt8				GetMixLevel()	{ return m_sHoipoiMixData.byLevel; }
	RwUInt32			GetMixExp()		{ return m_sHoipoiMixData.dwExp; }
	
protected:

	void				ClearRecipeData(BYTE byRecipeType);
	void				ClearAllRecipeData();
	
protected:
	MAPDEF_RECIPEDATA	m_mapRecipeData;

	sHOIPOIMIX_DATA		m_sHoipoiMixData;
};

#endif//__NTL_HOIPOI_MIX_H__