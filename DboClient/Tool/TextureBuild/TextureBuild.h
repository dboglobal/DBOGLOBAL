#pragma once

#ifndef __AFXWIN_H__
#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"		// 주 기호입니다.
#include "ConverterDirectX.h"

// CTextureBuildApp:
// 이 클래스의 구현에 대해서는 TextureBuild.cpp을 참조하십시오.
//

class CTextureBuildApp : public CWinApp
{
protected:
	CConverterDirectX m_Converter;

public:
	CConverterDirectX* GetConverter() { return &m_Converter; }


public:
	CTextureBuildApp();

// 재정의입니다.
	public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CTextureBuildApp theApp;