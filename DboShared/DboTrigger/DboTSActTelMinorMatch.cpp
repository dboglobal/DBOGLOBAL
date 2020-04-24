#include "precomp_trigger.h"
#include "DboTSActTelMinorMatch.h"


/**
	Teleport Minor-match
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActTelMinorMatch, CNtlTSAction )


CDboTSActTelMinorMatch::CDboTSActTelMinorMatch( void )
{
}

void CDboTSActTelMinorMatch::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );
}

void CDboTSActTelMinorMatch::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );
}
