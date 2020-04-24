#ifndef _DBO_TSCHECKITEM_H_
#define _DBO_TSCHECKITEM_H_


#include "DboTSCoreDefine.h"


/**
	Check item condition
	용도 : Client quest, client trigger, server quest, server trigger pc
	설명 : 아이템을 가지고 있는지 검사한다
*/


class CDboTSCheckItem : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiItemIdx;
	int									m_nItemCnt;

// Constructions
public:
	CDboTSCheckItem( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_ITEM; }

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


inline unsigned int CDboTSCheckItem::GetItemIdx( void ) const
{
	return m_uiItemIdx;
}

inline int CDboTSCheckItem::GetItemCnt( void ) const
{
	return m_nItemCnt;
}


#endif