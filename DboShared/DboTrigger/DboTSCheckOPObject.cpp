#include "precomp_trigger.h"
#include "DboTSCheckOPObject.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckOPObject, CNtlTSCond )


CDboTSCheckOPObject::CDboTSCheckOPObject( void )
{
}

void CDboTSCheckOPObject::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );
}

void CDboTSCheckOPObject::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );
}
