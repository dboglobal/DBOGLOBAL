#ifndef _DBO_TSACTITEM_H_
#define _DBO_TSACTITEM_H_


#include "DboTSCoreDefine.h"


/**
	Item action
	용도 : Server quest, server trigger pc
	설명 : 일반 아이템을 생성 및 삭제 한다
*/


struct stTS_ITEM_INFO
{
	eTSITEM_SLOT_TYPE					eItemSlotType;	// 아이템이 들어갈 위치
	unsigned int						uiItemIdx;		// 생성 및 삭제할 아이템의 인덱스
};


class CDboTSActItem : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	enum { eMAX_TS_ITEM_COUNT = 3 };

// Member variables
protected:
	eTSITEM_TYPE						m_eItemType;
	stTS_ITEM_INFO						m_stItemInfo[eMAX_TS_ITEM_COUNT];

// Constructions
public:
	CDboTSActItem( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_ITEM; }

	eTSITEM_TYPE						GetItemType( void ) const;
	void								SetItemType( eTSITEM_TYPE eItemType );

	const stTS_ITEM_INFO&				GetItemInfo( int nIdx ) const;
	void								SetItemInfo( int nIdx, const stTS_ITEM_INFO& stItemInfo );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eTSITEM_TYPE CDboTSActItem::GetItemType( void ) const
{
	return m_eItemType;
}

inline const stTS_ITEM_INFO& CDboTSActItem::GetItemInfo( int nIdx ) const
{
	return m_stItemInfo[nIdx];
}


#endif