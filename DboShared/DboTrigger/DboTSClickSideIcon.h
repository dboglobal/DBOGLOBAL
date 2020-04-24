#ifndef _DBO_TSCLICKSIDEICON_H_
#define _DBO_TSCLICKSIDEICON_H_


#include "DboTSCoreDefine.h"


/**
	Click side icon
*/


class CDboTSClickSideIcon : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	BYTE						m_bySideIconType;


// Constructions
public:
	CDboTSClickSideIcon( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_CLICK_SIDEICON; }

	BYTE								GetSideIconType() const;



// Implementations
protected:

	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

};


inline BYTE CDboTSClickSideIcon::GetSideIconType() const
{
	return m_bySideIconType;
}




#endif