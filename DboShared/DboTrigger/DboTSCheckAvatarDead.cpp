#include "precomp_trigger.h"
#include "DboTSCheckAvatarDead.h"


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckAvatarDead, CNtlTSCond )


CDboTSCheckAvatarDead::CDboTSCheckAvatarDead( void )
{
}

void CDboTSCheckAvatarDead::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );
}

void CDboTSCheckAvatarDead::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );
}
