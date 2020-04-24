#include "precomp_trigger.h"
#include "DboTSUseMail.h"


/**
	Use mail event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSUseMail, CNtlTSEvent )


CDboTSUseMail::CDboTSUseMail( void )
{
}

void CDboTSUseMail::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSUseMail::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
