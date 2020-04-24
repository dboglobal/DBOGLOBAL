/*****************************************************************************
 *
 * File			: NtlItem.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 10. 25	
 * Abstract		: Simulation item object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_ITEM_H__
#define __NTL_SOB_ITEM_H__

#include "NtlSob.h"
#include "NtlDebug.h"

class CNtlPLItem;
class CNtlSobItemIcon;

class CNtlSobItem : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobItem, NTL_DEFAULT_MEMORY_POOL)
	
public:

	enum SLOT_ITEM_POSITION
	{
		SIP_NONE,
		SIP_EQUIP,
		SIP_BAG,
		SIP_BAG_CHILD,
		SIP_SCOUT_CHILD,
		SIP_WAREHOUSE,
		SIP_WAREHOUSE_CHILD
	};

protected:

	SERIAL_HANDLE		m_hParentSerial;
	SLOT_ITEM_POSITION	m_eItemPos;
	RwInt32				m_uiSlotIdx;

	RwInt32				m_iChildNum;
	SERIAL_HANDLE		*m_pChildSerial;
	
	CNtlSobItemIcon		*m_pIcon;

// item event handler function
protected:

	void		CreateEventHandler(RWS::CMsg &pMsg);
	void		UpdateEventHandler(RWS::CMsg &pMsg);
	void		ItemMoveEventHandler(RWS::CMsg &pMsg);
	void		ItemStackMoveEventHandler(RWS::CMsg &pMsg);
	void		ItemRestrictStateUpdateEventHandler(RWS::CMsg &pMsg);
	void		ItemSocketInsertBeadEventHandler(RWS::CMsg &pMsg);
	void		ItemSocketDestroyBeadEventHandler(RWS::CMsg &pMsg);

public:

	CNtlSobItem();
	virtual ~CNtlSobItem();

	virtual RwBool	Create(void);
	virtual void	Destroy(void);
	virtual void	Update(RwReal fElapsed);
	virtual void	HandleEvents(RWS::CMsg &pMsg);

public:
    
	// current item
	void				SetItemPosition(SLOT_ITEM_POSITION ePos);
	SLOT_ITEM_POSITION	GetItemPosition(void) const;
	void				SetItemSlotIdx(RwInt32 iSlotIdx);
	RwInt32				GetItemSlotIdx(void) const; 

	// parent item 
	void				SetParentItemSerial(SERIAL_HANDLE hParentSeiral);
	SERIAL_HANDLE		GetParentItemSerial(void) const;
	CNtlSobItem*		GetParentItem(void) const;
	SLOT_ITEM_POSITION	GetParentItemPosition(void) const;
	RwUInt32			GetParentItemSlotIdx(void) const;

	// child item 
	void				AllocateChild(RwInt32 iChildNum);
	void				SetChildSerial(RwInt32 iIdx, SERIAL_HANDLE hChildSerial);
	RwUInt32			GetChildSerial(RwInt32 iIdx) const;
	CNtlSobItem*		GetChildItem(RwInt32 iIdx);
	RwInt32				GetChildNum(void) const;
	RwInt32				GetValidChildNum(void) const;
	RwBool				ExistChild(void);
	RwBool				EmptyChild(void);
	RwBool				FullChild(void);
	RwInt32				FindEmptyChildSlot(void);
	RwBool				IsValidChild(RwInt32 iIdx);

// item attribute function
public:

	RwBool				IsDragAndDropPossible(void);
	RwBool				IsUsePossible(void);
	RwBool				IsStackable(void);
	RwBool				IsEquipItem(void);
	RwBool				IsBagItem(void);
	RwBool				IsScouterItem(void);

	CNtlSobIcon*		GetIcon(void) const;
};

inline void CNtlSobItem::SetParentItemSerial(SERIAL_HANDLE hParentSeiral)
{
	m_hParentSerial = hParentSeiral;
}

inline SERIAL_HANDLE CNtlSobItem::GetParentItemSerial(void) const
{
	return m_hParentSerial;
}


inline void CNtlSobItem::SetChildSerial(RwInt32 iIdx, SERIAL_HANDLE hChildSerial)
{
	NTL_ASSERT(m_pChildSerial != NULL, "Child item not allocate");
	NTL_ASSERT(iIdx >= 0 && iIdx < m_iChildNum, "Child item index invalid");

	m_pChildSerial[iIdx] = hChildSerial;
}

inline SERIAL_HANDLE CNtlSobItem::GetChildSerial(RwInt32 iIdx) const
{
	if(m_pChildSerial == NULL)
		return INVALID_SERIAL_ID;

	if(iIdx < 0 || iIdx >= m_iChildNum)
		return INVALID_SERIAL_ID;

	return m_pChildSerial[iIdx];
}

inline RwInt32 CNtlSobItem::GetChildNum(void) const
{
	return m_iChildNum;
}

inline RwInt32 CNtlSobItem::GetValidChildNum(void) const
{
	if( !m_pChildSerial )
		return 0;

	RwInt32 nCount = 0;

	for( RwInt32 i = 0 ; i < m_iChildNum ; ++i )
	{
		if( m_pChildSerial[i] == INVALID_SERIAL_ID )
			continue;

		++nCount;
	}

	return nCount;
}

inline void CNtlSobItem::SetItemPosition(SLOT_ITEM_POSITION ePos)
{
	m_eItemPos = ePos;
}

inline CNtlSobItem::SLOT_ITEM_POSITION CNtlSobItem::GetItemPosition(void) const
{
	return m_eItemPos;
}

inline void CNtlSobItem::SetItemSlotIdx(RwInt32 iSlotIdx)
{
	m_uiSlotIdx = iSlotIdx;
}

inline RwInt32 CNtlSobItem::GetItemSlotIdx(void) const
{
	return m_uiSlotIdx;
}


inline RwBool CNtlSobItem::ExistChild(void)
{
	for( RwInt32 i = 0 ; i < m_iChildNum ; ++i )
	{
		if( GetChildSerial(i) != INVALID_SERIAL_ID )
			return TRUE;
	}	

	return FALSE;
}

inline RwBool CNtlSobItem::EmptyChild(void)
{
	if(m_pChildSerial == NULL || m_iChildNum == 0)
		return TRUE;

	for(RwInt32 i = 0; i < m_iChildNum; ++i)
	{
		if(m_pChildSerial[i] != INVALID_SERIAL_ID)
			return FALSE;
	}

	return TRUE;
}

inline RwBool CNtlSobItem::FullChild(void)
{
	if(m_pChildSerial == NULL || m_iChildNum == 0)
		return FALSE;

	for(RwInt32 i = 0; i < m_iChildNum; ++i)
	{
		if(m_pChildSerial[i] == INVALID_SERIAL_ID)
			return FALSE;
	}

	return TRUE;
}

inline RwBool CNtlSobItem::IsValidChild(RwInt32 iIdx)
{
	if(m_pChildSerial == NULL)
		return FALSE;

	if(iIdx < 0 || iIdx >= m_iChildNum)
		return FALSE;

	if(m_pChildSerial[iIdx] == INVALID_SERIAL_ID)
		return FALSE;

	return TRUE;
}


#endif
