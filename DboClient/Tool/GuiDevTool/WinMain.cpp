#include "precomp_guidevtool.h"
#include "GuiDevApplication.h"
#include "NtlDebug.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	CGuiDevApplication app;
	
	app.Init();
	if(!app.Create(hInstance, 0, 0, 1024, 768, FALSE))
	{
		NTL_ASSERTE("Application create failed!!!");
		return 1;
	}

	app.MessagePump();
	app.Destroy();
	
	return 1;
}


