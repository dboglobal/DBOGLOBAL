/*****************************************************************************
 *
 * File			: NtlSobDynamicObject.h
 * Author		: Haesung, Cho
 * Copyright	: (аж)NTL
 * Date			: 2008. 12. 15	
 * Abstract		: Simulation dynamic object
 *****************************************************************************
 * Desc         :
 *****************************************************************************/

#ifndef __NTL_SOB_DYNAMICOBJECT_H__
#define __NTL_SOB_DYNAMICOBJECT_H__

#include "NtlSobActor.h"

class CNtlSobDynamicObject : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobDynamicObject, NTL_DEFAULT_MEMORY_POOL )

public:
	CNtlSobDynamicObject(VOID);
	virtual ~CNtlSobDynamicObject(VOID);

	virtual RwBool	Create(VOID);
	virtual void	Destroy(VOID);
	
	virtual void	Update( RwReal fElapsed );	
	virtual void	HandleEvents( RWS::CMsg& pMsg );	
	virtual void	SetDirection(const RwV3d *pDir);

	void			InitState( RwUInt8 byState );
	void			UpdateState( RwUInt8 byState );

public:
	RwUInt8 GetCurrentDynamicObject(void) { return m_byCurState; }

	// Dynamic object's state
	RwUInt8			m_byCurState;
	RwUInt8			m_byPreState;	
};

#endif