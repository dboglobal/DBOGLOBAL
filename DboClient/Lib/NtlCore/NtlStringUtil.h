//***********************************************************************************
//	File		:	NtlStringUtil.h
//	Desc		:	
//	Begin		:	2007. 4. 4
//	Author		:	avooo
//	Update		:	
//***********************************************************************************

#pragma once


// 입력받은 스트링의 char형 바이트 수를 반환한다
// wchar_t 형은 모든 문자가 2바이트이지만 언어별 char 형은 1~3 바이트로 제각각이다.
// 서버에서 저장하는 문자열의 길이(byte)에 맞추기 위해 문자열이 몇 바이트인가 알아보기 위해 제작

// 인자를 char 형으로 받는다면 일부 문자를 제대로 인식하지 못한다
// 예) 夢 : 실제로는 2바이트이나 인식을 못해 ?로 변환되고 1바이트로 인식된다
RwUInt32 GetByte(const wchar_t* pwcText);


// 입력받은 숫자가 몇자리 숫자인지 알아본다
RwInt32 GetNumberLength(RwInt32 iNumber);
RwInt32 GetNumberLength(RwUInt32 uiNumber);


// 입력받은 데이터형에 맞춰 스트링을 만들어 반환한다
// 예) MakeStr("코스피가 %f%s 오른 %f포인트로 마감했습니다", 23.3, "포인트", 2003.23);
char* MakeStr(char* pcText, ...);

void		StringTokenize(std::vector<std::string>& strOutToken, const std::string& strText, const std::string& strToken);
std::string StringReplace(const std::string& strText, const std::string& strSrc, const std::string& strReplace);