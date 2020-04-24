/*****************************************************************************
*
* File			: NtlSobVehicleAttr.h
 * Author		: JeongHo, Rho
* Copyright		: (аж)NTL
* Date			: 2008. 11. 4	
* Abstract		: Simulation vehicle attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOB_VEHICLE_ATTR_H__
#define __NTL_SOB_VEHICLE_ATTR_H__


#include "NtlSobAttr.h"
#include "VehicleTable.h"


class CNtlSobVehicleAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL( CNtlSobVehicleAttr, NTL_DEFAULT_MEMORY_POOL )

protected:
	sVEHICLE_TBLDAT*			m_pVehicleTable;

public:

	CNtlSobVehicleAttr(); 

	virtual ~CNtlSobVehicleAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_VEHICLE); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:
	sVEHICLE_TBLDAT* GetVehicleTable( void ) const;
};


inline sVEHICLE_TBLDAT* CNtlSobVehicleAttr::GetVehicleTable( void ) const
{
	return m_pVehicleTable;
}


#endif