#ifndef _DBO_TSCHECKCLRQST_H_
#define _DBO_TSCHECKCLRQST_H_


#include "DboTSCoreDefine.h"


/**
	Check cleared quest condition
	Purpose : Client quest, client trigger, server quest, server trigger pc
	Description: Inspect prior to completion, the Quest
*/


class CDboTSCheckClrQst : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

public:

	typedef std::list<NTL_TS_T_ID> listdef_NextQidList;


// Member variables
protected:
	std::vector<NTL_TS_T_ID>			m_vecAndIdList;
	std::vector<NTL_TS_T_ID>			m_vecOrIdList;

	BOOL								m_tFlink;//NEW
	BOOL								m_tNOT;//NEW
	listdef_NextQidList					m_defNextQidLink;
	listdef_NextQidList::iterator		m_itNextQidLink;

// Constructions and Destructions
public:
	CDboTSCheckClrQst( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_CLRQST; }

	//
	const std::vector<NTL_TS_T_ID>&		GetAndIdList( void ) const;
	bool								SetAndIdList( const std::string& strIDList );
	void								ClearAndIdList( void );

	//
	const std::vector<NTL_TS_T_ID>&		GetOrIdList( void ) const;
	bool								SetOrIdList( const std::string& strIDList );
	void								ClearOrIdList( void );


	//new unknown
	int								GetFlink( void ) const;
	void							SetFlink( int tFlink );
	int								GetNOT( void ) const;
	void							SetNOT( int tNOT );
	
	int								GetNumOfNextQidLink( void ) const;
	NTL_TS_T_ID						BeginNextQidLink( void );
	NTL_TS_T_ID						NextNextQidLink( void );
	bool							AttachNextQidLink( NTL_TS_T_ID tId );
	void							DetachNextQidLink( NTL_TS_T_ID tId );
	bool							ParsingNextQidLink( const std::string& strContents );


// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline const std::vector<NTL_TS_T_ID>& CDboTSCheckClrQst::GetAndIdList( void ) const
{
	return m_vecAndIdList;
}

inline const std::vector<NTL_TS_T_ID>& CDboTSCheckClrQst::GetOrIdList( void ) const
{
	return m_vecOrIdList;
}

inline int CDboTSCheckClrQst::GetFlink( void ) const
{
	return m_tFlink;
}

inline int CDboTSCheckClrQst::GetNOT( void ) const
{
	return m_tNOT;
}

inline int CDboTSCheckClrQst::GetNumOfNextQidLink( void ) const
{
	return (int)m_defNextQidLink.size();
}


#endif