#include "precomp_dboclient.h"

//core
#include "NtlDebug.h"
#include "ceventhandler.h"

//gui
#include "gui_define.h"

//presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"


// table
#include "HelpTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "SCSActive.h"

SCSActive::SCSActive()
{

}

SCSActive::~SCSActive()
{

}

RwBool SCSActive::Create( )
{
	return true;
}

void SCSActive::Destroy( )
{

}

void SCSActive::HandleEvents( RWS::CMsg& msg )
{

}

RwInt32	SCSActive::SwitchDialog(bool bOpen)
{
	return 0;
}

