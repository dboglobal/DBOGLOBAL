/*****************************************************************************
*
* File			: CNtlSobVehicleProxy
* Author		: JeongHo, Rho
* Copyright		: (주)NTL
* Date			: 2008. 11. 4
* Abstract		: Simulation vehicle Proxy
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SOB_VEHICLE_PROXY_H__
#define __NTL_SOB_VEHICLE_PROXY_H__


#include "NtlSLDef.h"
#include "NtlSobProxy.h"


class CNtlPLCharacter;


class CNtlSobVehicleProxy : public CNtlSobProxy
{
	DECLEAR_MEMORY_POOL( CNtlSobVehicleProxy, NTL_DEFAULT_MEMORY_POOL )


private:

	CNtlPLCharacter*		m_pPLVehicle;


public:

	CNtlSobVehicleProxy(void);
	virtual ~CNtlSobVehicleProxy(void);

	virtual RwBool			Create( RwUInt32 uiCompType );

	virtual void			Destroy( void );

	virtual void			Update( RwReal fElapsed );

	virtual void			HandleEvents( RWS::CMsg& msg );

	virtual CNtlPLEntity*	GetPLMainEntity( void );


public:

	/**
	* transform에 관현된 함수.
	*/

	virtual void			SetPosition( const RwV3d* pPos );

	virtual void			SetDirection( const RwV3d* pDir );

	virtual RwV3d			GetDirection( void );

	virtual void			SetAngleY( RwReal fAngle );

	/**
	* animation에 관련된 함수.
	*/

	virtual void			SetBaseAnimation( RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f );

	virtual RwBool			IsExistBaseAnimation( RwUInt32 uiAnimKey );

	virtual RwUInt32		GetBaseAnimationKey( void );

	virtual RwBool			IsBaseAnimationEnd( void );

	virtual void			SetAnimSpeed( RwReal fAniSpeed );
	virtual RwReal          GetAnimSpeed( void );

	/**
	* entity에 관련된 함수.
	*/

	virtual RwReal			GetPLEntityHeight( void );
	virtual RwReal			GetPLEntityWidth( void );
	virtual RwReal			GetPLEntityDepth( void );
	virtual RwReal			GetPLEntityBaseScale( void );
	virtual RwReal			GetScale( void );

	/**
	* visible에 관련된 함수.
	*/

	virtual void			EnableVisible( RwBool bEnable );


protected:

	CNtlPLCharacter*		CreateVehicleEntity( const RwV3d& vLoc, const RwV3d& vDir, RwUInt32 uiVehicleTblIdx, RwBool bForChild );

	void					DeleteVehicleEntity( CNtlPLCharacter*& pPLVehicle );
};

inline CNtlPLEntity* CNtlSobVehicleProxy::GetPLMainEntity( void ) 
{
	return reinterpret_cast<CNtlPLEntity*>( m_pPLVehicle );
}


#endif