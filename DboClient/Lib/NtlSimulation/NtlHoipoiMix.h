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

class CNtlHoipoiMix : public RWS::CEventHandler
{
public:
	CNtlHoipoiMix();
	virtual ~CNtlHoipoiMix();

	virtual RwBool		Create(void);
	void				Destroy();

	virtual void		HandleEvents(RWS::CMsg &msg);

	RwInt32				GetRecipeCount();
	sRECIPE_DATA*		GetRecipeData();
	sHOIPOIMIX_DATA*	GetHoipoiMixData();

protected:
	sRECIPE_DATA		m_asRecipeData[NTL_ITEM_RECIPE_MAX_COUNT];
	RwUInt8				m_byRecipeCount;

	sHOIPOIMIX_DATA		m_sHoipoiMixData;
};

#endif//__NTL_HOIPOI_MIX_H__