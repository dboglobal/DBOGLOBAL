#ifndef _DBO_TSCONT_SWITCH_H_
#define _DBO_TSCONT_SWITCH_H_


#include "DboTSCoreDefine.h"


/**
	Switch container
*/


class CDboTSContSwitch : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	enum { MAX_NEXT_LINK_COUNT = 10 };

	struct sBranchInfo
	{
		sBranchInfo( void )
		{
		}

		sBranchInfo( NTL_TS_TC_ID tcNextId, unsigned int uiBranchId )
		{
			this->tcNextId = tcNextId;
			this->uiBranchId = uiBranchId;
		}

		NTL_TS_TC_ID tcNextId;
		unsigned int uiBranchId;
	};

	typedef std::list<sBranchInfo> listdef_NEXT_LINK;
	typedef listdef_NEXT_LINK::const_iterator listdef_NEXT_LINK_CIT;

// Member variables
protected:
	listdef_NEXT_LINK					m_defNextLink;

// Constructions
public:
	CDboTSContSwitch( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_SWITCH; }

	// Next link container
	bool								IsNextLinkFromTCId( NTL_TS_TC_ID tcId ) const;
	bool								IsNextLinkFromBId( unsigned int uiBId ) const;

	NTL_TS_TC_ID						GetTCIdFromBId( unsigned int uiBId ) const;

	listdef_NEXT_LINK_CIT				GetNextLinkBegin( void ) const;
	listdef_NEXT_LINK_CIT				GetNextLinkEnd( void ) const;

	bool								AttachNextLink( NTL_TS_TC_ID tcId, unsigned int uiBranchID );
	void								DetachNextLink( NTL_TS_TC_ID tcId );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline bool CDboTSContSwitch::IsNextLinkFromTCId( NTL_TS_TC_ID tcId ) const
{
	listdef_NEXT_LINK::const_iterator it = m_defNextLink.begin();
	for ( ; it != m_defNextLink.end(); ++it )
	{
		const sBranchInfo& sInfo = *it;

		if ( sInfo.tcNextId == tcId )
		{
			return true;
		}
	}

	return false;
}


inline bool CDboTSContSwitch::IsNextLinkFromBId( unsigned int uiBId ) const
{
	listdef_NEXT_LINK::const_iterator it = m_defNextLink.begin();
	for ( ; it != m_defNextLink.end(); ++it )
	{
		const sBranchInfo& sInfo = *it;

		if ( sInfo.uiBranchId == uiBId )
		{
			return true;
		}
	}

	return false;
}


inline NTL_TS_TC_ID CDboTSContSwitch::GetTCIdFromBId( unsigned int uiBId ) const
{
	listdef_NEXT_LINK::const_iterator it = m_defNextLink.begin();
	for ( ; it != m_defNextLink.end(); ++it )
	{
		const sBranchInfo& sInfo = *it;

		if ( sInfo.uiBranchId == uiBId )
		{
			return sInfo.tcNextId;
		}
	}

	return NTL_TS_TC_ID_INVALID;
}


inline CDboTSContSwitch::listdef_NEXT_LINK_CIT CDboTSContSwitch::GetNextLinkBegin( void ) const
{
	return m_defNextLink.begin();
}


inline CDboTSContSwitch::listdef_NEXT_LINK_CIT CDboTSContSwitch::GetNextLinkEnd( void ) const
{
	return m_defNextLink.end();
}


#endif