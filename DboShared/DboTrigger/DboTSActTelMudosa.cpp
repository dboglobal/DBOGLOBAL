#include "precomp_trigger.h"
#include "DboTSActTelMudosa.h"


/**
	Teleport Mudosa
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSActTelMudosa, CNtlTSAction )


CDboTSActTelMudosa::CDboTSActTelMudosa( void )
{
}

void CDboTSActTelMudosa::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::ApplyScriptDataForScript( clProperty );
}

void CDboTSActTelMudosa::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSAction::TakeScriptDataForScript( clProperty );
}
