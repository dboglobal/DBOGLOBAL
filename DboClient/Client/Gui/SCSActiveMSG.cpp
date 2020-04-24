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
#include "SCSActiveMSG.h"

SCSActiveMSG::SCSActiveMSG()
{

}

SCSActiveMSG::~SCSActiveMSG()
{

}

RwBool SCSActiveMSG::Create( )
{
	return true;
}

void SCSActiveMSG::Destroy( )
{

}

void SCSActiveMSG::HandleEvents( RWS::CMsg& msg )
{

}

RwInt32	SCSActiveMSG::SwitchDialog(bool bOpen)
{
	return 0;
}

