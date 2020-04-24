#include "precomp_trigger.h"
#include "DboTSTeleport.h"


/**
	Teleport quest event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSTeleport, CNtlTSEvent )


CDboTSTeleport::CDboTSTeleport( void )
{
}

void CDboTSTeleport::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSTeleport::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
