/*****************************************************************************
 *
 * File			: NtlSobQuestItem.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 11. 06	
 * Abstract		: Simulation quest item object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_QUESTITEM_H__
#define __NTL_SOB_QUESTITEM_H__

#include "NtlSob.h"
#include "NtlDebug.h"

class CNtlSobQuestItemIcon;

class CNtlSobQuestItem : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobQuestItem, NTL_DEFAULT_MEMORY_POOL)

protected:
	
	RwInt32					m_uiSlotIdx;
	CNtlSobQuestItemIcon	*m_pIcon;

protected:

	void		CreateEventHandler(RWS::CMsg &pMsg);

public:

	CNtlSobQuestItem();
	virtual ~CNtlSobQuestItem();

	virtual RwBool	Create(void);
	virtual void	Destroy(void);
	virtual void	Update(RwReal fElapsed);
	virtual void	HandleEvents(RWS::CMsg &pMsg);

public:
    
	// current item
	void				SetItemSlotIdx(RwInt32 iSlotIdx);
	RwInt32				GetItemSlotIdx(void) const; 

// item attribute function
public:

	RwBool				IsDragAndDropPossible(void);
	RwBool				IsUsePossible(void);
	CNtlSobIcon*		GetIcon(void) const;

public:
	
};


#endif
