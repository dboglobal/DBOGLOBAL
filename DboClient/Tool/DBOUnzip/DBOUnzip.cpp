// DBOUnzip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NtlUnzip.h"
#include <string>
#include <stdlib.h>


// 첫번째 인자 : 압축파일명 (*.zip)
// 두번째 인자 : 압축을 풀 폴더

int main(int argc, char* argv[])
{
	if(argc < 3)
		return 0;

	std::string strFileName;
	std::string strFolderName;
	std::string strFileNameNoZip;
	bool isFolderName = false;

	for(int i = 1; i < argc; ++i)
	{
		if(!isFolderName)
		{
			strFileName += argv[i];

			int nIndex = strFileName.find(".zip");
			if(nIndex > 0)
			{
				isFolderName = true;

				char szDrive[_MAX_DRIVE], szFolder[MAX_PATH], szFName[_MAX_FNAME];
				_splitpath( argv[i], szDrive, szFolder, szFName, 0 );
				strFileNameNoZip = szFName;				
			}
			else
			{
				strFileName += " ";
			}
		}
		else
		{
			strFolderName += argv[i];
			if(i+1 < argc)
			{
				strFolderName += " ";
			}		
			else
			{
				strFolderName += strFileNameNoZip;
			}
		}
	}

	printf("UnPacking World Map...[%s]\n", strFileNameNoZip.c_str());

	if(!CNtlUnzip::Unzip(strFileName.c_str(), strFolderName.c_str()))
	{
		printf("UnPacking Fail...\n");
		printf("%s\n", strFileName.c_str());
		printf("%s\n", strFolderName.c_str());
		getchar();
	}	

	return 0;
}

