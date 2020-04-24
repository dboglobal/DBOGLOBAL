#ifndef _DBO_TSACTCONTUSRSEL_H_
#define _DBO_TSACTCONTUSRSEL_H_


#include "DboTSCoreDefine.h"


/**
	User selection container
*/


class CDboTSContUsrSel : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	enum { MAX_NEXT_LINK_COUNT = 10 };
	typedef std::map<NTL_TS_TC_ID, unsigned int> mapdef_NEXT_LINK;
	typedef std::map<NTL_TS_TC_ID, unsigned int>::const_iterator mapdef_NEXT_LINK_CIT;

// Member variables
protected:
	eUSER_SEL_TYPE						m_eUSType;
	eUSER_SEL_PROGRESS_STATE			m_eUSProgState;		// eUSER_SEL_TYPE_NARRATION 인 경우만 유효함
	eUSER_SEL_TARGET_TYPE				m_eTargetType;		// eUSER_SEL_TYPE_GENERAL 인 경우만 유효함
	unsigned int						m_uiTargetTblIdx;	// eUSER_SEL_TYPE_GENERAL 인 경우만 유효함
	unsigned int						m_uiDesc;			// eUSER_SEL_TYPE_NARRATION / eUSER_SEL_TYPE_GENERAL 인 경우 유효함
	mapdef_NEXT_LINK					m_defNextLink;
	NTL_TS_TC_ID						m_tcCancelLink;
	NTL_TS_TC_ID						m_tcLogInLink;

// Constructions and Destructions
public:
	CDboTSContUsrSel( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_USERSEL; }

	eUSER_SEL_TYPE						GetUserSelType( void ) const;
	void								SetUserSelType( eUSER_SEL_TYPE eUSType );

	eUSER_SEL_PROGRESS_STATE			GetUserSelProgState( void ) const;
	void								SetUserSelProgState( eUSER_SEL_PROGRESS_STATE eUSProgState );

	eUSER_SEL_TARGET_TYPE				GetTargetType( void ) const;
	void								SetTargetType( eUSER_SEL_TARGET_TYPE eTargetType );

	unsigned int						GetTargetTblIdx( void ) const;
	void								SetTargetTblIdx( unsigned int uiTblIdx );

	unsigned int						GetDesc( void ) const;
	void								SetDesc( unsigned int uiDesc );

	bool								IsNextLink( NTL_TS_TC_ID tcId ) const;

	unsigned int						GetNextLink( NTL_TS_TC_ID tcId ) const;

	mapdef_NEXT_LINK_CIT				GetNextLinkBegin( void ) const;
	mapdef_NEXT_LINK_CIT				GetNextLinkEnd( void ) const;

	bool								AttachNextLink( NTL_TS_TC_ID tcId, unsigned int uiName );
	void								DetachNextLink( NTL_TS_TC_ID tcId );

	NTL_TS_TC_ID						GetCancelLink( void ) const;
	void								SetCancelLink( NTL_TS_TC_ID tcId );

	NTL_TS_TC_ID						GetLogInLink( void ) const;
	void								SetLogInLink( NTL_TS_TC_ID tcId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eUSER_SEL_TYPE CDboTSContUsrSel::GetUserSelType( void ) const
{
	return m_eUSType;
}

inline eUSER_SEL_PROGRESS_STATE CDboTSContUsrSel::GetUserSelProgState( void ) const
{
	return m_eUSProgState;
}

inline eUSER_SEL_TARGET_TYPE CDboTSContUsrSel::GetTargetType( void ) const
{
	return m_eTargetType;
}

inline unsigned int CDboTSContUsrSel::GetTargetTblIdx( void ) const
{
	return m_uiTargetTblIdx;
}

inline unsigned int CDboTSContUsrSel::GetDesc( void ) const
{
	return m_uiDesc;
}

inline NTL_TS_TC_ID CDboTSContUsrSel::GetCancelLink( void ) const
{
	return m_tcCancelLink;
}

inline NTL_TS_TC_ID CDboTSContUsrSel::GetLogInLink( void ) const
{
	return m_tcLogInLink;
}


#endif