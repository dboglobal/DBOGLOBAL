#ifndef _DBO_TSITEMEQUIP_H_
#define _DBO_TSITEMEQUIP_H_


#include "DboTSCoreDefine.h"


class CDboTSItemEquip : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiItemIdx;

// Constructions
public:
	CDboTSItemEquip( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_ITEM_EQUIP; }

	unsigned int						GetItemIdx( void ) const;
	void								SetItemIdx( unsigned int uiItemIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSItemEquip::GetItemIdx( void ) const
{
	return m_uiItemIdx;
}


#endif