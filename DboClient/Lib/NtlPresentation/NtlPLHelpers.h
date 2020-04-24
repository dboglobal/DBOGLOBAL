/*****************************************************************************
*
* File			: NtlPLHelpers.h
* Author		: All Author
* Copyright	: (주)NTL
* Date			: 2005. 8. 11	
* Abstract		: Presentation layer helper function.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLHELPER_H__
#define __NTL_PLHELPER_H__
/*
#include <windows.h>
#include <rwcore.h>
#include <rpworld.h>
*/

#include <tchar.h>
#include "NtlPLDef.h"
#include "NtlPLEmblemMaker.h"
#include "NtlMath.h"


class CNtlPLEntity;
class CNtlPLPlayerName;


ENtlPLEntityType Helper_PLEntityTypeToName(const RwChar* pName);

const RwChar* Helper_PLEntityNameToType(const ENtlPLEntityType eType);

/// 상대경로,절대경로에서 파일 이름만을 추출한다.
RwChar* Helper_AbstractFileName(RwChar *pPathName);

/// 문자열을 파싱한다. (by agebreak 2006.06.26)
void Helper_StringSplit(const RwChar* szInput, RwChar* szOutput, size_t sizeOutPut, const RwChar* szSplit, RwInt32 nCount);

//////////////////////////////////////////////////////////////////////////
// 기본 구조체들의 내용을 L"x;y;z;" 형태들의 문자열로 변경한다.
//////////////////////////////////////////////////////////////////////////

/// RwV3d 구조체 내용을 L"x;y;z" 형태의 문자열로 변경한다. (by agebreak 2006.08.16)
#define  Helper_V3D2StringU(buf, vPos)   swprintf_s(buf, L"%.2f;%.2f;%.2f", vPos.x, vPos.y, vPos.z);
RwV3d Helper_String2V3D(const RwChar* buf); // "x;y;z;"형태의 문자열을 RwV3d형태로 변환하여 반환한다.

// RwV2d 구조체 내용을 L"x;y"형태의 문자열로 변경한다. (by agebreak 2006.08.16)
#define  Helper_V2D2StringU(buf, vPos)   swprintf_s(buf, L"%.2f;%.2f", vPos.x, vPos.y);

// "x;y"형태의 문자열을 RwV2d형태로 변환하여 반환한다.
RwV2d Helper_String2V2D(const RwChar* buf);
RwV2d Helper_String2V2D(const WCHAR* buf);

// "r;g;b;a;" 형태의 문자열을 RwRGBA형태로 변환하여 반환한다.
RwRGBA Helper_String2RGBA(const RwChar* buf);
#define Helper_RGBA2String(buf, rgba) swprintf_s(buf, L"%d;%d;%d;%d", rgba.red, rgba.green, rgba.blue, rgba.alpha);

// RGB 정보를 Gray Color로 변환한다.
RwUInt8 Helper_RGB2Gray(RwUInt8& _R, RwUInt8& _G, RwUInt8& _B);

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// presentation entity 생성.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//CNtlPLPlayerName* Helper_CreatePlayerName(const RwV3d *pPos, COLORREF Color, const WCHAR *pName);
CNtlPLPlayerName* Helper_CreatePlayerName(const RwV3d *pPos, COLORREF Color, const WCHAR *pName,
										  COLORREF ColorGuild = RGB(255, 255, 255), const WCHAR *pGuildName = NULL,
										  sEmblemFactor *pEmblemFactor = NULL,
										  COLORREF ColorTitle = RGB(255, 255, 255), const WCHAR *pTitleName = NULL);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// attach function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Helper_AttachWorldPos(CNtlPLEntity *pSource, CNtlPLEntity *pTarget, RwV3d vOffset = ZeroAxis, RwBool bApplyRotate = FALSE);
void Helper_AttachWorldPosHeight(CNtlPLEntity* pSource, CNtlPLEntity* pTarget);           ///< 월드 위치에 붙이는데 높이는 그 위치의 World Height 값을 적용한다.
void Helper_AttachBone(CNtlPLEntity *pSource, CNtlPLEntity *pTarget, const RwChar *pBoneName, RwV3d vOffset = ZeroAxis);
void Helper_DetachPLEntity(CNtlPLEntity *pSource, CNtlPLEntity *pTarget);

//////////////////////////////////////////////////////////////////////////
// Bit Flag 설정 관련
//////////////////////////////////////////////////////////////////////////

__inline void Helper_SetBigFlag(BITFLAG* bitFlag, int index, BOOL bBool)
{
    if(index < 0 || index > 31)
        return;

    if(bBool)
    {
        *bitFlag |= 1 << index;
    }
    else
    {
        *bitFlag &= ~(1 << index);
    }    
}

__inline BOOL Helper_GetBitFlag(BITFLAG bitFlag, int index)
{
    if(index < 0 || index > 31)
        return FALSE;

    return bitFlag &= 1 << index;
}


//////////////////////////////////////////////////////////////////////////
// Callback 함수
//////////////////////////////////////////////////////////////////////////

typedef RwUInt8 (*Callback_GetDogiColorIndex)(RwUInt32 uiSerialID);
extern Callback_GetDogiColorIndex g_fnGetDogiColorIndex;

void LinkCallback_GetDogiColorIndex(Callback_GetDogiColorIndex fn);
RwUInt8 Logic_GetDogiColorIndex(RwUInt32 uiSerialID);


#endif