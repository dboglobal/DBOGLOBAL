#include "precomp_trigger.h"
#include "DboTSBindStone.h"


/**
	Bind stone event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSBindStone, CNtlTSEvent )


CDboTSBindStone::CDboTSBindStone( void )
{
}

void CDboTSBindStone::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSBindStone::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
