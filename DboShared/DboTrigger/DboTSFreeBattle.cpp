#include "precomp_trigger.h"
#include "DboTSFreeBattle.h"


/**
	free battle
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSFreeBattle, CNtlTSEvent )


CDboTSFreeBattle::CDboTSFreeBattle( void )
{
}

void CDboTSFreeBattle::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSFreeBattle::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
