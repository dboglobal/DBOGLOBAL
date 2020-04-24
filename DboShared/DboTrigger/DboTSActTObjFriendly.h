#ifndef _DBO_TSACTTOBJFRIENDLY_H_
#define _DBO_TSACTTOBJFRIENDLY_H_


#include "DboTSCoreDefine.h"


/**
	Trigger object friendly
*/


class CDboTSActTObjFriendly : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiFriendlyFlags;		// eTOBJ_FRIENDLY

// Constructions
public:
	CDboTSActTObjFriendly( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TOBJ_FRIENDLY; }

	unsigned int						GetFriendlyFlags( void ) const;
	void								SetFriendlyFlags( unsigned int uiFlags /* eTOBJ_FRIENDLY */ );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActTObjFriendly::GetFriendlyFlags( void ) const
{
	return m_uiFriendlyFlags;
}


#endif