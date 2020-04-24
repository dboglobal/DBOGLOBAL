#ifndef _DBO_TSITEMUSE_H_
#define _DBO_TSITEMUSE_H_


#include "DboTSCoreDefine.h"


class CDboTSItemUse : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiItemIdx;
	eEVENT_ITEM_TARGET_TYPE				m_eItemTargetType;
	unsigned int						m_uiItemTargetIdx;

// Constructions
public:
	CDboTSItemUse( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_ITEM_USE; }

	unsigned int						GetItemIdx( void ) const;
	void								SetItemIdx( unsigned int uiItemIdx );

	eEVENT_ITEM_TARGET_TYPE				GetItemTargetType( void ) const;
	void								SetItemTargetType( eEVENT_ITEM_TARGET_TYPE eItemTargetType );

	unsigned int						GetItemTargetIdx( void ) const;
	void								SetItemTargetIdx( unsigned int uiItemTargetIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSItemUse::GetItemIdx( void ) const
{
	return m_uiItemIdx;
}

inline eEVENT_ITEM_TARGET_TYPE CDboTSItemUse::GetItemTargetType( void ) const
{
	return m_eItemTargetType;
}

inline unsigned int CDboTSItemUse::GetItemTargetIdx( void ) const
{
	return m_uiItemTargetIdx;
}


#endif