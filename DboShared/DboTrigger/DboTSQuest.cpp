#include "precomp_trigger.h"
#include "DboTSQuest.h"


/**
	Private shop
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSQuest, CNtlTSEvent )


CDboTSQuest::CDboTSQuest( void )
{
}

void CDboTSQuest::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSQuest::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}