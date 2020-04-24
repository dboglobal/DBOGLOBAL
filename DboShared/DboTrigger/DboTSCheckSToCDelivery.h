#ifndef _DBO_TSCHECKSTOCDELIVERY_H_
#define _DBO_TSCHECKSTOCDELIVERY_H_


#include "DboTSCoreDefine.h"


/**
	Check SToC delivery condition
*/


class CDboTSCheckSToCDelivery : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	enum
	{
		MAX_CHECK_STOC_DELIVERTY_ITEM	= 3
	};

// Member variables
protected:
	unsigned int						m_uiItemIdx[MAX_CHECK_STOC_DELIVERTY_ITEM];
	int									m_nItemCnt[MAX_CHECK_STOC_DELIVERTY_ITEM];

// Constructions and Destructions
public:
	CDboTSCheckSToCDelivery( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_STOC_DELIVERTY; }

	unsigned int						GetItemIdx( int nIdx );
	void								SetItemIdx( int nIdx, unsigned int uiItemIdx );

	int									GetItemCnt( int nIdx );
	int									GetItemCntFromItemIdx( unsigned int uiItemIdx );
	void								SetItemCnt( int nIdx, int nItemCnt );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSCheckSToCDelivery::GetItemIdx( int nIdx )
{
	if ( nIdx < 0 || nIdx >= MAX_CHECK_STOC_DELIVERTY_ITEM )
	{
		return 0xffffffff;
	}

	return m_uiItemIdx[nIdx];
}


inline int CDboTSCheckSToCDelivery::GetItemCnt( int nIdx )
{
	if ( nIdx < 0 || nIdx >= MAX_CHECK_STOC_DELIVERTY_ITEM )
	{
		return 0xffffffff;
	}

	return m_nItemCnt[nIdx];
}


inline int CDboTSCheckSToCDelivery::GetItemCntFromItemIdx( unsigned int uiItemIdx )
{
	for ( int i = 0; i < MAX_CHECK_STOC_DELIVERTY_ITEM; ++i )
	{
		if ( m_uiItemIdx[i] == uiItemIdx )
		{
			return m_nItemCnt[i];
		}
	}

	return -1;
}


#endif