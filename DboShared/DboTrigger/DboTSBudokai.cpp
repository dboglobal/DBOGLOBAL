#include "precomp_trigger.h"
#include "DboTSBudokai.h"


/**
	budokai
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSBudokai, CNtlTSEvent )


CDboTSBudokai::CDboTSBudokai( void )
{
}

void CDboTSBudokai::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSBudokai::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
