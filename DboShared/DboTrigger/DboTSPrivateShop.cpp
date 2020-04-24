#include "precomp_trigger.h"
#include "DboTSPrivateShop.h"


/**
	Private shop
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSPrivateShop, CNtlTSEvent )


CDboTSPrivateShop::CDboTSPrivateShop( void )
{
}

void CDboTSPrivateShop::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSPrivateShop::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
