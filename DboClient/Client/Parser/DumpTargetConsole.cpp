#include "precomp_dboclient.h"
#include "DumpTargetConsole.h"
#include "NtlDebug.h"



CDumpTargetConsole::CDumpTargetConsole()
{
	
}

CDumpTargetConsole::~CDumpTargetConsole()
{
	m_ConWnd.Close();
}

void CDumpTargetConsole::Dump(void)
{
	m_ConWnd.Output(GetStream());

	CNtlDumpTarget::Dump(); 
}

void CDumpTargetConsole::Active(RwBool bActive)
{
	CNtlDumpTarget::Active(bActive); 

	if(bActive)
	{
		m_ConWnd.Create("dump", true); 
	}
    else
	{
		m_ConWnd.Close(); 
	}
}

void CDumpTargetConsole::Color(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	WORD wColor = 0;
	if(byRed == 255)
		wColor |= FOREGROUND_RED;
	if(byGreen == 255)
		wColor |= FOREGROUND_GREEN;
	if(byBlue == 255)
		wColor |= FOREGROUND_BLUE;

	if(wColor == 0)
		wColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	m_ConWnd.Color(wColor);
}
