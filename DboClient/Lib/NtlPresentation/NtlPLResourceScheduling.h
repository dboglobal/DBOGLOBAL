/*****************************************************************************
 *
 * File			: NtlPLResourcePack.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 3. 20	
 * Abstract		: Presentation layer resource pack class
 *****************************************************************************
 * Desc         : 
 * Adjust		: 
 *
 *****************************************************************************/

#ifndef __NTL_PLRESOURCE_SCHEDULING_H__
#define __NTL_PLRESOURCE_SCHEDULING_H__

#include "PerformanceChecker.h"
#include "NtlCallbackWidget.h"
#include "NtlCharsetPrint.h"

class CNtlPLResource;
class CNtlPLEntity;

struct SResourceScheduleNode
{
	RwUInt32	uiResType;
	RwChar		chResourcePath[1024];
	RwChar		chFileName[64];
};

/**
* \brief Serial ID 로 하나씩 묶어서 사용하기 위한 CharUnit의 구조체
*/
struct SResourceScheduleCharUnit
{
	CNtlPLEntity*			pPLEntity;
	SResourceScheduleNode*	pNode;
};

/**
* \ingroup presentation
*/
class CNtlResourceScheduleUnit
{
protected:

	typedef std::map<CNtlPLEntity*, SResourceScheduleNode*> MapScheduling;
	typedef std::list<CNtlPLResource*> ListScheduling;

	RwUInt8			m_byEntityType;

	RwReal			m_fLoadSchedulingTime;
	RwReal			m_fLoadCurrTime;
	
	MapScheduling	m_mapClumpLoadSchedule;
	ListScheduling	m_listClumpDeleteSchedule;

	protected:
	void			UpdateObjectLoadScheduling(RwReal fElapsed);
	void			UpdateObjectLoadSeamlessScheduling(RwReal fElapsed);

	void			UpdateLoadScheduling(RwReal fElapsed);
	void			UpdateLoadSeamlessScheduling(RwReal fElapsed);
	virtual void	UpdateDeleteScheduling(RwReal fElapsed);

public:

	CNtlResourceScheduleUnit();
	virtual ~CNtlResourceScheduleUnit();

	void			SetEntityType(RwUInt8 byEntityType);

	virtual void	Update(RwReal fElapsed);

	virtual void	AddSchedulingNode(CNtlPLEntity *pPLEntity, SResourceScheduleNode *pNode);

	virtual void	UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource);

	virtual RwBool	IsEmptyLoadScheduling(void);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \ingroup client
* \brief 캐릭터를 위한 Schedule Unit
*/
class CNtlResourceScheduleCharUnit : public CNtlResourceScheduleUnit
{
public:
	CNtlResourceScheduleCharUnit();
	virtual ~CNtlResourceScheduleCharUnit();

	virtual void	Update(RwReal fElapsed);
	
	virtual void	AddSchedulingNode(CNtlPLEntity *pPLEntity, SResourceScheduleNode *pNode);

	virtual void	UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource);

	virtual RwBool	IsEmptyLoadScheduling(void);

protected:
	void			UpdateCharacterLoadScheduling(RwReal fElapsed);

	void			UpdateCharacterLoadSeamlessScheduling(RwReal fElapsed);

	// list
	typedef std::list< SResourceScheduleCharUnit > ListCharScheduling;
	ListCharScheduling	m_listCharClumpLoadSchedule;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlResourceScheduleManager
{
private:

	typedef std::map<RwUInt8, CNtlResourceScheduleUnit*> MapScheduleUnit;

	MapScheduleUnit m_mapUnit;

	static RwFreeList *m_pScheduleNodeFreeList;		/* scheduling node memory pool **/

private:

	CNtlResourceScheduleUnit*	FindUnit(RwUInt8 byEntityType);	

public:

	CNtlResourceScheduleManager() {}

	~CNtlResourceScheduleManager();

	
	static RwBool	AllocFreeList(void);

	static void		DeAllocFreeList(void);

	static void		FreeListFree(void *pData);

	void			Update(RwReal fElapsed);

	void			AddSchedulingClump(const char *pStrName, const char * pResourcePath, CNtlPLEntity *pEntity);

	void			UnLoadScheduling(CNtlPLEntity *pPLEntity, CNtlPLResource *pResource);

	RwBool			IsEmptyLoadScheduling(RwUInt8 byEntityType);

};

RwBool	GetLoadObjectSeamlessScheduling(void);
void	SetLoadObjectSeamlessScheduling(RwBool bEnable);

void	SetLoadObjectSchedulingOneTickTime(RwReal fTime);
void	SetLoadObjectSeamlessSchedulingOneTickTime(RwReal fTime);


#endif