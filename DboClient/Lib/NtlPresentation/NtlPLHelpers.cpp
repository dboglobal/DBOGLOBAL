#include "precomp_ntlpresentation.h"
#include "NtlPLHelpers.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlCoreUtil.h"

// presentation
#include "NtlPLEntity.h"
#include "NtlPLAttach.h"
#include "NtlPLPlayerName.h"
#include "NtlPLSceneManager.h"

ENtlPLEntityType Helper_PLEntityTypeToName(const RwChar* pName)
{
	std::string strName = pName;

	if(strName == NTL_PLEN_SKY)
		return PLENTITY_SKY;
	else if(strName == NTL_PLEN_WORLD)
		return PLENTITY_WORLD;
	else if(strName == NTL_PLEN_DUMMY_WORLD)
		return PLENTITY_DUMMY_WORLD;
	else if(strName == NTL_PLEN_WATER)
		return PLENTITY_WATER;
	else if(strName == NTL_PLEN_OBJECT)
		return PLENTITY_OBJECT;
	else if(strName == NTL_PLEN_CHARACTER)
		return PLENTITY_CHARACTER;
	else if(strName == NTL_PLEN_ITEM)
		return PLENTITY_ITEM;
	else if(strName == NTL_PLEN_EFFECT)
		return PLENTITY_EFFECT;
	else if(strName == NTL_PLEN_SPLINE)
		return PLENTITY_SPLINE;
	else if(strName == NTL_PLEN_DECAL)
		return PLENTITY_DECAL;
	else if(strName == NTL_PLEN_PLAYER_NAME)
		return PLENTITY_PLAYER_NAME;
	else if(strName == NTL_PLEN_DAMAGE_BOX)
		return PLENTITY_DAMAGE_BOX;
	else if(strName == NTL_PLEN_LIGHT)
		return PLENTITY_LIGHT;
	else if(strName == NTL_PLEN_FOG)
		return PLENTITY_FOG;
	else if(strName == NTL_PLEN_GUI)
		return PLENTITY_GUI;
	else if(strName == NTL_PLEN_SOUND)
		return PLENTITY_SOUND;
	else if(strName == NTL_PLEN_SOUND_BGM)
		return PLENTITY_SOUND_BGM;
	else if (strName == NTL_PLEN_PLANT)
		return PLENTITY_PLANT;
	else if (strName == NTL_PLEN_WEATHER)
		return PLENTITY_WEATHER;
	else if (strName == NTL_PLEN_HEATHAZE)
		return PLENTITY_HEATHAZE;
	else if (strName == NTL_PLEN_LIGHT_OBJECT)
		return PLENTITY_LIGHT_OBJECT;
	else if (strName == NTL_PLEN_GAME_PROPERTY)
		return PLENTITY_GAME_PROPERTY;
	else if (strName == NTL_PLEN_DECAL_VOLUME)
		return PLENTITY_DECAL_VOLUME;
	else if (strName == NTL_PLEN_DOJO)
		return PLENTITY_DOJO;
	else
		return PLENTITY_INVALID_TYPE;
}

const RwChar* Helper_PLEntityNameToType(const ENtlPLEntityType eType)
{
	switch(eType)
	{
	case PLENTITY_SKY:
		return NTL_PLEN_SKY;
	case PLENTITY_WORLD:
		return NTL_PLEN_WORLD;
	case PLENTITY_DUMMY_WORLD:
		return NTL_PLEN_DUMMY_WORLD;
	case PLENTITY_WATER:
		return NTL_PLEN_WATER;
	case PLENTITY_OBJECT:
		return NTL_PLEN_OBJECT;
	case PLENTITY_CHARACTER:
		return NTL_PLEN_CHARACTER;
	case PLENTITY_ITEM:
		return NTL_PLEN_ITEM;
	case PLENTITY_EFFECT:
		return NTL_PLEN_EFFECT;
	case PLENTITY_SPLINE:
		return NTL_PLEN_SPLINE;
	case PLENTITY_DECAL:
		return NTL_PLEN_DECAL;
	case PLENTITY_PLAYER_NAME:
		return NTL_PLEN_PLAYER_NAME;
	case PLENTITY_DAMAGE_BOX:
		return NTL_PLEN_DAMAGE_BOX;
	case PLENTITY_LIGHT:
		return NTL_PLEN_LIGHT;
	case PLENTITY_FOG:
		return NTL_PLEN_FOG;
	case PLENTITY_GUI:
		return NTL_PLEN_GUI;
	case PLENTITY_SOUND:
		return NTL_PLEN_SOUND;
	case PLENTITY_SOUND_BGM:
		return NTL_PLEN_SOUND_BGM;
	case PLENTITY_PLANT:
		return NTL_PLEN_PLANT;
	case PLENTITY_WORLD_LIGHT:
		return NTL_PLEN_WORLD_LIGHT;
	case PLENTITY_WEATHER:
		return NTL_PLEN_WEATHER;
	case PLENTITY_HEATHAZE:
		return NTL_PLEN_HEATHAZE;
	case PLENTITY_LIGHT_OBJECT:
		return NTL_PLEN_LIGHT_OBJECT;
	case PLENTITY_GAME_PROPERTY:
		return NTL_PLEN_GAME_PROPERTY;
	case PLENTITY_DECAL_VOLUME:
		return NTL_PLEN_DECAL_VOLUME;
	case PLENTITY_DOJO:
		return NTL_PLEN_DOJO;
	}

	return 0;
}

RwChar* Helper_AbstractFileName(RwChar *pPathName)
{
	RwInt32 nLen = (RwInt32)strlen(pPathName);
	nLen -= 1;

	while(nLen >= 0)
	{
		if(pPathName[nLen] == '\\' || pPathName[nLen] == '/')
			return (pPathName+nLen+1);
		nLen--;		
	};

	return pPathName;
}

CNtlPLPlayerName* Helper_CreatePlayerName( const RwV3d *pPos, COLORREF Color, const WCHAR *pName, COLORREF ColorGuild, const WCHAR *pGuildName, sEmblemFactor *pEmblemFactor,
	COLORREF ColorTitle, const WCHAR *pTitleName)
{
	struct SPLPlayerNameCreateParam sParam;
	sParam.pPos			= pPos;
	sParam.pName		= pName;
	sParam.Color		= Color;
	sParam.fCullDist	= 100.0f; // changed from 40 to 100 by daneos. To make names visible
	sParam.pGuildName	= pGuildName;
	sParam.ColorGuild	= ColorGuild;
	sParam.pTitleName = pTitleName;
	sParam.ColorTitle = ColorTitle;
	sParam.pEmblemFactor= pEmblemFactor;

	return reinterpret_cast<CNtlPLPlayerName*>(GetSceneManager()->CreateEntity(PLENTITY_PLAYER_NAME, NULL, &sParam));
}

void Helper_AttachWorldPos(CNtlPLEntity *pSource, CNtlPLEntity *pTarget, RwV3d vOffset, RwBool bApplyRotate)
{
	NTL_PRE(pSource);
	NTL_PRE(pTarget);

	if( !(pSource->GetFlags() & NTL_PLEFLAG_ATTACH) )
	{
		DBO_TRACE(pSource->GetName(), "not attach presentation object");
		DBO_TRACE(!pSource->GetName(), pSource->GetName() << " - not attach presentation object");
		return;
	}

	if( !(pTarget->GetFlags() & NTL_PLEFLAG_ATTACH) )
	{
		DBO_TRACE(pTarget->GetName(), "not attach presentation object");
		DBO_TRACE(!pTarget->GetName(), pTarget->GetName() << " - not attach presentation object");
		return;
	}

	CNtlPLAttach *pSourceAttach = reinterpret_cast<CNtlPLAttach*>(pSource);
	CNtlPLAttach *pTargetAttach = reinterpret_cast<CNtlPLAttach*>(pTarget);

	if(bApplyRotate)
	{
		pSourceAttach->AttachWorldPosAndRotate(pTargetAttach, vOffset);

	}
	else
	{
		pSourceAttach->AttachWorldPos(pTargetAttach, vOffset);
	}    
}

void Helper_AttachWorldPosHeight( CNtlPLEntity* pSource, CNtlPLEntity* pTarget ) 
{
	NTL_PRE(pSource);
	NTL_PRE(pTarget);

	if( !(pSource->GetFlags() & NTL_PLEFLAG_ATTACH) )
	{
		DBO_TRACE(pSource->GetName(), "not attach presentation object");
		DBO_TRACE(!pSource->GetName(), pSource->GetName() << " - not attach presentation object");
		return;
	}

	if( !(pTarget->GetFlags() & NTL_PLEFLAG_ATTACH) )
	{
		DBO_TRACE(pTarget->GetName(), "not attach presentation object");
		DBO_TRACE(!pTarget->GetName(), pTarget->GetName() << " - not attach presentation object");
		return;
	}

	CNtlPLAttach *pSourceAttach = reinterpret_cast<CNtlPLAttach*>(pSource);
	CNtlPLAttach *pTargetAttach = reinterpret_cast<CNtlPLAttach*>(pTarget);

	pSourceAttach->AttachWorldPosHeight(pTargetAttach);
}

void Helper_AttachBone(CNtlPLEntity *pSource, CNtlPLEntity *pTarget, const RwChar *pBoneName, RwV3d vOffset)
{
	NTL_PRE(pSource);
	NTL_PRE(pTarget);
	NTL_PRE(pBoneName);

	if( !(pSource->GetFlags() & NTL_PLEFLAG_ATTACH) )
	{
		DBO_TRACE(pSource->GetName(), "not attach presentation object");
		DBO_TRACE(!pSource->GetName(), pSource->GetName() << " - not attach presentation object");
		return;
	}

	if( !(pTarget->GetFlags() & NTL_PLEFLAG_ATTACH) )
	{
		DBO_TRACE(pTarget->GetName(), "not attach presentation object");
		DBO_TRACE(!pTarget->GetName(), pTarget->GetName() << " - not attach presentation object");
		return;
	}

	CNtlPLAttach *pSourceAttach = reinterpret_cast<CNtlPLAttach*>(pSource);
	CNtlPLAttach *pTargetAttach = reinterpret_cast<CNtlPLAttach*>(pTarget);

	SPLAttachAttr sAttachAttr;
	sAttachAttr.vOffsetPos = vOffset;
	pSourceAttach->AttachBone(pTargetAttach, pBoneName, &sAttachAttr);
}


void Helper_DetachPLEntity(CNtlPLEntity *pSource, CNtlPLEntity *pTarget)
{
	NTL_PRE(pSource);
	NTL_PRE(pTarget);

	if( !(pSource->GetFlags() & NTL_PLEFLAG_ATTACH) )
		return;

	if( !(pTarget->GetFlags() & NTL_PLEFLAG_ATTACH) )
		return;

	CNtlPLAttach *pSourceAttach = reinterpret_cast<CNtlPLAttach*>(pSource);
	CNtlPLAttach *pTargetAttach = reinterpret_cast<CNtlPLAttach*>(pTarget);

	pSourceAttach->Detach(pTargetAttach);
}

/**
* 문자열을 파싱하여 반환한다.
* \param szInput 입력 문자열
* \param szOutput 출력 문자열
* \param szSplit 파싱할 문자열 
* \param nCount 파싱후, 가져오고 싶은 문자열의 Index, 0부터 시작한다. (Ex. "r;g;b"에서 g를 가져오고싶다면 1) 
*/
void Helper_StringSplit( const RwChar* szInput, RwChar* szOutput, size_t sizeOutput, const RwChar* szSplit, RwInt32 nCount ) 
{
	if(!szInput || !szSplit ||!szOutput)
		return;

	int nMaxBuffer = 64;

	int nCnt = 0;
	RwChar* tempIndex = (char*)szInput;

	RwChar* pIndex = strstr(tempIndex, szSplit);
	while(pIndex)
	{
		if(nCnt == nCount)
		{
			strncpy_s(szOutput, sizeOutput, tempIndex, pIndex - tempIndex);
			return;                
		}

		tempIndex = pIndex + 1;
		pIndex = strstr(pIndex + 1, szSplit);
		++nCnt;
	}

	// 마지막 문자열을 찾을때 필요     
	sprintf_s(szOutput, sizeOutput, "%s", tempIndex);       
}

/**
* "x;y;z"형태의 문자열을 RwV3d로 변환하여 반환한다.
* \param buf 입력될 문자열
* return 반환된 RwV3d값
*/
RwV3d Helper_String2V3D(const RwChar* buf)
{
	RwV3d vReturn = {0.0f, 0.0f, 0.0f};    
	RwChar szOutput[3][64] = {0,};

	// NOTE: 속도를 위해서 For문을 사용하지 않는다.
	Helper_StringSplit(buf, szOutput[0], 64, ";", 0);
	Helper_StringSplit(buf, szOutput[1], 64, ";", 1);
	Helper_StringSplit(buf, szOutput[2], 64, ";", 2);

	vReturn.x = (RwReal)atof(szOutput[0]);
	vReturn.y = (RwReal)atof(szOutput[1]);
	vReturn.z = (RwReal)atof(szOutput[2]);

	return vReturn;
}

RwV2d Helper_String2V2D( const RwChar* buf ) 
{
	RwV2d vReturn = {0.0f,};
	RwChar szOutput[2][8] = {0,};

	Helper_StringSplit(buf, szOutput[0], 8, ";", 0);
	Helper_StringSplit(buf, szOutput[1], 8, ";", 1);

	vReturn.x = (RwReal)atof(szOutput[0]);
	vReturn.y = (RwReal)atof(szOutput[1]);

	return vReturn;
}

RwV2d Helper_String2V2D( const WCHAR* buf ) 
{
	char chBuffer[256] = {0,};
	::WideCharToMultiByte(GetACP(), 0, buf, -1, chBuffer, 256, NULL, NULL);
	return Helper_String2V2D(chBuffer);
}

RwRGBA Helper_String2RGBA( const RwChar* buf ) 
{
	RwRGBA colReturn;
	RwChar szOutput[4][8] = {0,};
	ZeroMemory(&colReturn, sizeof(RwRGBA));

	Helper_StringSplit(buf, szOutput[0], 8, ";", 0);
	Helper_StringSplit(buf, szOutput[1], 8, ";", 1);
	Helper_StringSplit(buf, szOutput[2], 8, ";", 2);
	Helper_StringSplit(buf, szOutput[3], 8, ";", 3);

	colReturn.red = (RwUInt8)atoi(szOutput[0]);
	colReturn.green = (RwUInt8)atoi(szOutput[1]);
	colReturn.blue = (RwUInt8)atoi(szOutput[2]);
	colReturn.alpha = (RwUInt8)atoi(szOutput[3]);

	return colReturn;    
}

RwUInt8 Helper_RGB2Gray(RwUInt8& _R, RwUInt8& _G, RwUInt8& _B)
{
	return static_cast<RwUInt8>(
		static_cast<RwReal>(_R) * 0.299f +
		static_cast<RwReal>(_G) * 0.587f +
		static_cast<RwReal>(_B) * 0.114f
		);
}



//////////////////////////////////////////////////////////////////////////
// Callback 함수
//////////////////////////////////////////////////////////////////////////


Callback_GetDogiColorIndex g_fnGetDogiColorIndex = NULL;

void LinkCallback_GetDogiColorIndex(Callback_GetDogiColorIndex fn)
{
	g_fnGetDogiColorIndex = fn;
}

RwUInt8 Logic_GetDogiColorIndex(RwUInt32 uiSerialID)
{
	if( !g_fnGetDogiColorIndex )
		return INVALID_BYTE;

	return g_fnGetDogiColorIndex(uiSerialID);
}