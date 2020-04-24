#ifndef _DBO_TSACTMAIL_H_
#define _DBO_TSACTMAIL_H_


#include "DboTSCoreDefine.h"


/**
	mail action
*/


class CDboTSActMail : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:

	unsigned int			m_uiZeni;
	unsigned char			m_byItemCount;
	unsigned int			m_uiItemTblidx;
	NTL_TS_TA_ID			m_taId;
	unsigned int			m_dtIdx;
	unsigned int			m_stIdx;


// Constructions
public:
	CDboTSActMail( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_MAIL; }


	unsigned int						GetZeni( void ) const;
	unsigned char						GetItemCount( void ) const;
	unsigned int						GetItemTblidx( void ) const;
	NTL_TS_TA_ID						GetTaIdx( void ) const;
	unsigned int						GetDtIdx( void ) const;
	unsigned int						GetStIdx( void ) const;

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};



inline unsigned int CDboTSActMail::GetZeni( void ) const
{
	return m_uiZeni;
}

inline unsigned char CDboTSActMail::GetItemCount( void ) const
{
	return m_byItemCount;
}

inline unsigned int CDboTSActMail::GetItemTblidx( void ) const
{
	return m_uiItemTblidx;
}

inline NTL_TS_TA_ID CDboTSActMail::GetTaIdx( void ) const
{
	return m_taId;
}

inline unsigned int CDboTSActMail::GetDtIdx( void ) const
{
	return m_dtIdx;
}

inline unsigned int CDboTSActMail::GetStIdx( void ) const
{
	return m_stIdx;
}


#endif