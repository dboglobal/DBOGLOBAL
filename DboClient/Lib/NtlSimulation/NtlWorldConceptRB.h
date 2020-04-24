#pragma once

#include "NtlWorldConceptController.h"

/**
* \ingroup NtlSimulation
* \brief Control class in normal state (do not control anything)
* \date 2007-06-14
* \author peessi
*/

class CNtlWorldConceptRB : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptRB(void);
	~CNtlWorldConceptRB(void);

	virtual void	Update( RwReal fElapsed );	
	virtual void	ChangeState( RwInt32 eState );	
	void			ChangeRBState( RwInt32 eRBState );
	
	virtual RwBool	IsEnableAction( EAvatarAction eAction );

	virtual RwBool	IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos);

	virtual RwBool	CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode);

	RwBool			IsRingOut( RwUInt32 hSerialID );

	RwInt32			GetRBState(VOID) { return m_eRBState; }

	void			SetFirstMatchStart(bool bFlag) { m_bFirstMatchStart = bFlag; }
	bool			IsFirstMatchStart() { return m_bFirstMatchStart; }


private:	
	void	ResetCamera(VOID);

	void	UpdateState_None( RwReal fElapsed );
	void	UpdateState_Wait( RwReal fElapsed );
	void	UpdateState_Direction( RwReal fElapsed );
	void	UpdateState_Stage_Ready( RwReal fElapsed );
	void	UpdateState_Stage_Run( RwReal fElapsed );
	void	UpdateState_Stage_Finish( RwReal fElapsed );
	void	UpdateState_Boss_Direction( RwReal fElapsed );
	void	UpdateState_Boss_Ready( RwReal fElapsed );
	void	UpdateState_Boss_Run( RwReal fElapsed );
	void	UpdateState_Boss_Finish( RwReal fElapsed );
	void	UpdateState_End( RwReal fElapsed );

	void	(CNtlWorldConceptRB::*StateUpdate)( RwReal fElapsed );	

	struct stMemberInfo
	{
		SERIAL_HANDLE	hSerial;
	};

	RwInt32	m_eRBState;
	bool	m_bFirstMatchStart;
};
