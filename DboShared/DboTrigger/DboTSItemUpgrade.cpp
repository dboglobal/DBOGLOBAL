#include "precomp_trigger.h"
#include "DboTSItemUpgrade.h"


/**
	Item upgrade quest event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSItemUpgrade, CNtlTSEvent )


CDboTSItemUpgrade::CDboTSItemUpgrade( void )
{
}

void CDboTSItemUpgrade::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSItemUpgrade::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
