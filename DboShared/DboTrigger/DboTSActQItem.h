#ifndef _DBO_TSACTQITEM_H_
#define _DBO_TSACTQITEM_H_


#include "DboTSCoreDefine.h"


/**
	Quest item action
	Purpose: Server quest, server trigger pc
	Description: Create and delete Quest item
*/


struct stTS_QITEM_INFO
{
	unsigned int						uiQItemIdx;		// The index of create and delete quest items
	int									nQItemCnt;		// The number of create and delete quest items
	float								fProbability;	// Generation probabilities
};


class CDboTSActQItem : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	enum { eMAX_TS_QITEM_COUNT = 3 };

// Member variables
protected:
	eQITEM_TYPE							m_eQItemType;
	stTS_QITEM_INFO						m_stQItemInfo[eMAX_TS_QITEM_COUNT];

// Constructions
public:
	CDboTSActQItem( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_QITEM; }

	eQITEM_TYPE							GetQItemType( void ) const;
	void								SetQItemType( eQITEM_TYPE eQItemType );

	const stTS_QITEM_INFO&				GetQItemInfo( int nIdx ) const;
	void								SetQItemInfo( int nIdx, const stTS_QITEM_INFO& stQItemInfo );

// Implementations
protected:
	// A function for loading and input from the script data of a component object
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline eQITEM_TYPE CDboTSActQItem::GetQItemType( void ) const
{
	return m_eQItemType;
}

inline const stTS_QITEM_INFO& CDboTSActQItem::GetQItemInfo( int nIdx ) const
{
	return m_stQItemInfo[nIdx];
}


#endif