#ifndef _DBO_TSACTCONTSTART_H_
#define _DBO_TSACTCONTSTART_H_


#include "DboTSContGCond.h"


/**
	Start container
*/


class CDboTSContStart : public CDboTSContGCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiSTDiag;

// Constructions and Destructions
public:
	CDboTSContStart( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_START; }

	unsigned int						GetSTDiag( void ) const;
	void								SetSTDiag( unsigned int uiSTDiag );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSContStart::GetSTDiag( void ) const
{
	return m_uiSTDiag;
}


#endif