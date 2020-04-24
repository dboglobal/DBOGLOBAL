/*****************************************************************************
 *
 * File			: NtlSobBuff.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 3. 14	
 * Abstract		: Simulation buff object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_BUFF_H__
#define __NTL_SOB_BUFF_H__

#include "NtlSob.h"

class CNtlSobBuffIcon;

class CNtlSobBuff : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobBuff, NTL_DEFAULT_MEMORY_POOL)

private:

	CNtlSobBuffIcon *m_pIcon;
	RwInt32 m_iVisualId;

	std::list<CNtlPLEntity*> m_listPLEntity;

public:

	CNtlSobBuff();
	virtual ~CNtlSobBuff();

	RwBool Create(void);

	void Destroy(void);

	void Update(RwReal fElapsed);

	void HandleEvents(RWS::CMsg &pMsg);

	CNtlSobIcon* GetIcon(void) const;

	void AddPLEntity(CNtlPLEntity *pPLEntity);

	void SetVisualId(RwInt32 iVisualId);

	RwInt32 GetVisualId(void) const;
};



inline void CNtlSobBuff::SetVisualId(RwInt32 iVisualId)
{
	m_iVisualId = iVisualId;
}

inline RwInt32 CNtlSobBuff::GetVisualId(void) const
{
	return m_iVisualId;
}

#endif
