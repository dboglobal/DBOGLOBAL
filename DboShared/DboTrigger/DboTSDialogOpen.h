#ifndef _DBO_TSDIALOGOPEN_H_
#define _DBO_TSDIALOGOPEN_H_


#include "DboTSCoreDefine.h"


/**
	DIALOG OPEN
*/


class CDboTSDialogOpen : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI


// Member variables
protected:
	unsigned char						m_byDialogOpenType;


// Constructions
public:
	CDboTSDialogOpen( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_DIALOGOPEN; }

	unsigned char						GetDialogOpenType( void ) const;



// Implementations
protected:

	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

};


inline unsigned char CDboTSDialogOpen::GetDialogOpenType( void ) const
{
	return m_byDialogOpenType;
}




#endif