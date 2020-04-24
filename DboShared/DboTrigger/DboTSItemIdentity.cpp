#include "precomp_trigger.h"
#include "DboTSItemIdentity.h"


/**
	Item identity event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSItemIdentity, CNtlTSEvent )


CDboTSItemIdentity::CDboTSItemIdentity( void )
{
}

void CDboTSItemIdentity::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSItemIdentity::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
