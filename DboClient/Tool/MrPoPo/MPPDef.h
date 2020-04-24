#pragma once

#define MPP_TITLE_VER                   L"Mr.PoPo - DBO Support Tool(v 8.10.28) by agebreak"

#define FILTER_CONFIG                   L"Config File(*.xml;*.edf)|*.xml;*.edf||"
#define FILTER_DST                      L"Display String File(*.dat)|*.dat||"

//#define ONLY_GRAPHIC_PANE               ///< 그래픽 설정만 보이게 컴파일하는 옵션

// 유틸 함수들
static CString GetFileNameFromPath(CString strFilePath)
{
    int nIndex = strFilePath.ReverseFind(L'\\');
    if(nIndex)
    {
        int nLength = strFilePath.GetLength() - nIndex - 1;
        strFilePath = strFilePath.Right(nLength);
    }

    return strFilePath;
}