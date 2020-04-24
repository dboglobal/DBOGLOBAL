/*****************************************************************************
*
* File			: NtlSobWorldItem.h
* Author		: Peessi
* Copyright		: ( аж )NTL
* Date			: 2006. 7. 11	
* Abstract		: Simulation World Item object
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SOB_WORLD_ITEM_H__
#define __NTL_SOB_WORLD_ITEM_H__

#include "NtlSobActor.h"
#include "NtlDebug.h"

class CNtlDistTimeSequence;

class CNtlSobWorldItem : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobWorldItem, NTL_DEFAULT_MEMORY_POOL )

public:

	enum STATE { WI_NONE, WI_CREATE, WI_ACTDROP, WI_ACTEND, WI_NOCREATE };

protected:

	VOID CreateEventHandler( RWS::CMsg& pMsg );
	VOID DestroyEventHandler( RWS::CMsg& pMsg );

	VOID CreateDropEffect(VOID);
	VOID CreateDropVisualEffect(VOID);
	
	VOID SwitchState( RwInt32 eState );
	VOID State_CreateUpdate(VOID);
	void State_NoCreateUpdate();
	VOID State_ActDropUpdate(VOID);
		
public:

	CNtlSobWorldItem(VOID);
	virtual ~CNtlSobWorldItem(VOID);

	virtual RwBool Create(VOID);
	virtual VOID Destroy(VOID);

	virtual CNtlFSMLayer* CreateFSM(void);

	virtual CNtlSobInfluence* CreateSobInfluence(void);

	virtual VOID Update( RwReal fElapsed );

	virtual VOID HandleEvents( RWS::CMsg& pMsg );

	RwBool IsEnableLoot(VOID);

	VOID SetLootSend(RwBool bSend);
	RwBool IsLootSend(void);


protected:

	RwReal  m_fTime;
	RwReal	m_fDestTime;
	RwReal	m_fDestAngle;
	RwV3d   m_vDestPos;
	RwBool	m_bVisualEff;
	CNtlDistTimeSequence* m_pSeq;
	RwInt32	m_eState;
	RwBool	m_bLootSend;

	VOID	(CNtlSobWorldItem::*StateUpdate)();
};

#endif
