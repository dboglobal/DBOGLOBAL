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
#include "SCSGui.h"

SCSGui::SCSGui()
{

}

SCSGui::~SCSGui()
{

}

RwBool SCSGui::Create( )
{
	return true;
}

void SCSGui::Destroy( )
{

}

void SCSGui::HandleEvents( RWS::CMsg& msg )
{

}

RwInt32	SCSGui::SwitchDialog(bool bOpen)
{
	return 0;
}

