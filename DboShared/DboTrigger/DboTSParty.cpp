#include "precomp_trigger.h"
#include "DboTSParty.h"


/**
	Party event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSParty, CNtlTSEvent )


CDboTSParty::CDboTSParty( void )
{
}

void CDboTSParty::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSParty::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
