#include "precomp_trigger.h"
#include "DboTSSearchQuest.h"


/**
	Search quest event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSSearchQuest, CNtlTSEvent )


CDboTSSearchQuest::CDboTSSearchQuest( void )
{
}

void CDboTSSearchQuest::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSSearchQuest::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
