#ifndef _DBO_TSCHECKQITEM_H_
#define _DBO_TSCHECKQITEM_H_


#include "DboTSCoreDefine.h"


/**
	Check quest item condition
	용도 : Client quest, client trigger, server quest, server trigger pc
*/


class CDboTSCheckQItem : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eQUEST_ITEM_CHECK_TYPE				m_eCheckType;
	unsigned int						m_uiItemIdx;
	int									m_nItemCnt;

// Constructions
public:
	CDboTSCheckQItem( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_QITEM; }

	eQUEST_ITEM_CHECK_TYPE				GetCheckType( void );
	void								SetCheckType( eQUEST_ITEM_CHECK_TYPE eCheckType );

	unsigned int						GetItemIdx( void ) const;
	void								SetItemIdx( unsigned int uiItemIdx );

	int									GetItemCnt( void ) const;
	void								SetItemCnt( int nItemCnt );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eQUEST_ITEM_CHECK_TYPE CDboTSCheckQItem::GetCheckType( void )
{
	return m_eCheckType;
}

inline unsigned int CDboTSCheckQItem::GetItemIdx( void ) const
{
	return m_uiItemIdx;
}

inline int CDboTSCheckQItem::GetItemCnt( void ) const
{
	return m_nItemCnt;
}


#endif