#include "precomp_trigger.h"
#include "DboTSHoipoiMix.h"


/**
	Hoipoi mix
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSHoipoiMix, CNtlTSEvent )


CDboTSHoipoiMix::CDboTSHoipoiMix( void )
{
}

void CDboTSHoipoiMix::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSHoipoiMix::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
