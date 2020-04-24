/*****************************************************************************
*
* File			: NtlSobVehicle.h
* Author		: JeongHo, Rho
* Copyright		: (аж)NTL
* Date			: 2008. 11. 4
* Abstract		: Simulation vehicle object
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#ifndef __NTL_SOB_VEHICLE_H__
#define __NTL_SOB_VEHICLE_H__


#include "ceventhandler.h"
#include "NtlSobActor.h"


class CNtlBeCharData;
class CNtlSobProxySystemEffect;

class CNtlSobVehicle : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobVehicle, NTL_DEFAULT_MEMORY_POOL )

protected:

	CNtlBeCharData*			m_pBeData; 

	RwBool					m_bAttachDriver;

	RwReal					m_fRideOnOffAccel;
	RwReal					m_fRideOnOffMaxSpeed;
	RwReal					m_fRideOnOffCurSpeed;
	RwReal					m_fTargetRideOnHeight;
	RwReal					m_fCurRideOnHeight;

	CNtlSobProxySystemEffect* m_pSobProxySystemEffect; // required to make driver invisible

public:

	CNtlSobVehicle( void );
	virtual ~CNtlSobVehicle( void );


	virtual RwBool			Create( void );

	virtual void			Destroy( void );

	virtual void			Update( RwReal fElapsed );

	virtual void			HandleEvents( RWS::CMsg& pMsg );

	virtual void*			GetBehaviorData( void );


public:
	void					AttachDriver( RwBool bAttachDriver );

	RwReal					GetMaxRideOnHeight( void );

	RwReal					GetRideOnHeight( void );
	void					SetRideOnHeight( RwReal fRideOnHeight );


public:
	void					UpdateRideOnHeight( RwReal fElapsed );
	void					UpdateAttachDriverAndVehicle( RwReal fElapsed );
};


inline void* CNtlSobVehicle::GetBehaviorData( void )
{
	return m_pBeData;
}

inline RwReal CNtlSobVehicle::GetRideOnHeight( void )
{
	return m_fCurRideOnHeight;
}


#endif