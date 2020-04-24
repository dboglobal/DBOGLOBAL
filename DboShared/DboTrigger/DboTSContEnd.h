#ifndef _DBO_TSACTCONTEND_H_
#define _DBO_TSACTCONTEND_H_


#include "DboTSContGAct.h"


/**
	End container
*/


enum eEND_TYPE
{
	eEND_TYPE_NOT_PROGRESS,
	eEND_TYPE_COMPLETE
};


class CDboTSContEnd : public CDboTSContGAct
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	eEND_TYPE							m_eEndType;

// Constructions and Destructions
public:
	CDboTSContEnd( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_END; }

	eEND_TYPE							GetEndType( void ) const;
	void								SetEndType( eEND_TYPE eEndType );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eEND_TYPE CDboTSContEnd::GetEndType( void ) const
{
	return m_eEndType;
}


#endif