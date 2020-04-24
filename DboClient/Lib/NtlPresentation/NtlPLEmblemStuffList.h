/*****************************************************************************
*
* File			: NtlPLEmblemStuffList.h
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2008. 4. 8
* Abstract		: Presentation layer emblem stuff list
*****************************************************************************
* Desc          :
*****************************************************************************/

#ifndef __NTL_PLEMBLEM_STUFF_H__
#define __NTL_PLEMBLEM_STUFF_H__

#include "NtlPLDef.h"

typedef std::map<RwUInt32, RwUInt32>				MAP_STUFF;
typedef std::map<RwUInt32, RwUInt32>::iterator		MAP_STUFF_ITER;

struct sEmblemFactor;

typedef void (*CallbackLoadEmblemStuff)(const RwChar* pFileName, void** pData, int* iSize);
extern CallbackLoadEmblemStuff g_fnCallback_LoadEmblemStuff;

void LinkLoadEmblemStuff(CallbackLoadEmblemStuff fn);
void UnLinkLoadEmblemStuff();


class CNtlPLEmblemStuffList
{
public:
	typedef std::map<RwUInt8, std::string>				MAP_CONVERTNAME;
	typedef std::map<RwUInt8, std::string>::iterator	MAP_CONVERTNAME_ITER;


	CNtlPLEmblemStuffList();
	virtual ~CNtlPLEmblemStuffList();

	RwBool		Create();
	VOID		Destroy();

	const MAP_STUFF*	GetStuffList(eEmblemType eType);

protected:
	VOID		LoadScript();

protected:
	MAP_STUFF			m_mapStuff[NUM_EMBLEMTYPE];		///< 스크립트 데이터
};

#endif