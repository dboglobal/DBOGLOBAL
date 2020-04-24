/*****************************************************************************
 *
 * File			: NtlSobFeelingActor.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 04. 09	
 * Abstract		: Simulation object에서 감정을 느끼는 object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_FEELING_ACTOR_H__
#define __NTL_SOB_FEELING_ACTOR_H__

#include "NtlSobActor.h"

class CNtlInventoryBase;
class CNtlBuffContainer;
class CNtlPetBuffer;

#define NTL_MAX_HAVE_PET		5

class CNtlPetBuffer
{
private:

	SERIAL_HANDLE m_hPet[NTL_MAX_HAVE_PET];

public:

	CNtlPetBuffer();
	~CNtlPetBuffer();

	RwBool	Create(void);
	void	Destroy(void);

	void			SetSerialId(RwInt8 byIdx, SERIAL_HANDLE hSerialId);
	SERIAL_HANDLE	GetSerialId(RwInt8 byIdx);

	void			AddPet(SERIAL_HANDLE hSerialId);
	void			RemovePet(SERIAL_HANDLE hSerialId);

	SERIAL_HANDLE	FindPetFromSkillTable(RwUInt32 uiTblIdx);
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CNtlSobFeelingActor : public CNtlSobActor
{
protected:

	CNtlBuffContainer	*m_pBuffContainer;		/** buff container */
	CNtlPetBuffer		*m_pPetBuffer;			/** pet buffer */	
		
protected:

	CNtlSobFeelingActor();

public:

	virtual ~CNtlSobFeelingActor();

	virtual RwBool				Create(void);
	virtual void				Destroy(void);
	virtual void				ServerChangeOut(void);
	virtual void				ServerChangeIn(void);
	virtual void				Update(RwReal fElapsed);
	virtual CNtlSobInfluence*	CreateSobInfluence(void);
	virtual void				HandleEvents(RWS::CMsg &pMsg);
    virtual void				SetPosition(const RwV3d *pPos);
    virtual CNtlBuffContainer*	GetBuffContainer(void);
    virtual CNtlPetBuffer*		GetPetBuffer(void);

///////////////////////////////////////////////////////
// event handler
///////////////////////////////////////////////////////
	void						SobFaintingEventHandler(RWS::CMsg &pMsg);
	void						SobEquipChangeEventHandler(RWS::CMsg &pMsg);
	void						SobSubWeaponActiveEventHandler(RWS::CMsg &pMsg);
	void						SobSubWeaponDeActiveEventHandler(RWS::CMsg &pMsg);
    void                        SobPostEffectEventHandler(RWS::CMsg& pMsg);             ///< Post Effect Event를 처리한다.
};


inline CNtlBuffContainer* CNtlSobFeelingActor::GetBuffContainer(void)
{
	return m_pBuffContainer;
}

inline CNtlPetBuffer* CNtlSobFeelingActor::GetPetBuffer(void)
{
	return m_pPetBuffer;
}



#endif
