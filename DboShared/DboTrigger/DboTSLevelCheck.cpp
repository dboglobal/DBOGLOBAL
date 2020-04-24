#include "precomp_trigger.h"
#include "DboTSLevelCheck.h"


/**
	level check
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSLevelCheck, CNtlTSEvent )


CDboTSLevelCheck::CDboTSLevelCheck( void )
{
}

void CDboTSLevelCheck::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSLevelCheck::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}