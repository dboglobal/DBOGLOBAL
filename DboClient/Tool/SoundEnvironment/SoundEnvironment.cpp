#include "precomp_soundenvironment.h"

#include "conio.h"
#include "SoundInfo.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CSoundInfo soundInfo;

	if( !soundInfo.Create() )
		return 0;

	soundInfo.ReportSoundEnvironment();

	soundInfo.Destory();

	printf("\n아무키나 눌러서 프로그램을 종료하여 주십시요\n");
	getch();

	return 0;
}
