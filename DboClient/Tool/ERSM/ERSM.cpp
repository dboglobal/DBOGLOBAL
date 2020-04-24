// ERSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ERSMCore.h"


int _tmain(int argc, _TCHAR* argv[])
{
	NCDefInitSGT(ERSMCore);

	if(NCGetSGT(ERSMCore)->Init())
	{
		std::cout<<"# Error Report Storage Manager Init Success..\n";		
	}
	else
	{
		std::cout<<"# Error Report Storage Manager Init Fail!\n";
		return 0;
	}

	NCGetSGT(ERSMCore)->Run();

	return 0;
}

