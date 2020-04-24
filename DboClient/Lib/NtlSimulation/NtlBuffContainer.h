/*****************************************************************************
*
* File			: NtlBuffContainer.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2006. 5. 11	
* Abstract		: Simulation buff container
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_BUFF_CONTAINER_H__
#define __NTL_BUFF_CONTAINER_H__

// shared
#include "NtlSkill.h"
#include "NtlObject.h"

// core
#include "ceventhandler.h"

// simulation
#include "NtlSLDef.h"

class CNtlSob;
class CNtlSobBuff;

struct sSKILL_TBLDAT;
struct sITEM_TBLDAT;

class CNtlBuffContainer : public RWS::CEventHandler
{
public:

	typedef std::list<CNtlSobBuff*> ListBuff;

private:

	CNtlSob *m_pSobObj;
	ListBuff m_listBuff;

	void CreateEventHandler(RWS::CMsg &pMsg);
	void AddEventHandler(RWS::CMsg &pMsg);
	void DropEventHandler(RWS::CMsg &pMsg);
	void RefreshAllEventHandler(RWS::CMsg &pMsg);
	void ActivateEventHandler(RWS::CMsg &pMsg);
	void AutoRecoverEventHandler(RWS::CMsg &pMsg);
	
public:

	CNtlBuffContainer();
	~CNtlBuffContainer();

	RwBool Create(void);
	void Destroy(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

	void SetObj(const CNtlSob *pSobObj);

	RwInt32 GetBuffCount(void);
	ListBuff::iterator GetBeginBuff(void);
	ListBuff::iterator GetEndBuff(void);
};


struct sNtlFakeBuff
{
	sNtlFakeBuff(void)
	{
		eType = DBO_OBJECT_SOURCE_UNKNOWN;
		fTime = 0.0f;
		fMaxTime = 0.0f;
		bBlessType = FALSE;
		pImage = NULL;
		bActive = false;
	}

	~sNtlFakeBuff(void)
	{
		DeleteImage();
	}

	void CreateImage(const RwChar* pFileName);


	void DeleteImage(void);

	BYTE						byBuffIndex;
	eDBO_OBJECT_SOURCE			eType;
	RwReal						fTime;
	RwReal						fMaxTime;
	RwBool						bBlessType;
	void*						pImage;
	bool						bActive;

	union
	{
		struct
		{
			sSKILL_TBLDAT*			pSkillTbl;
			sDBO_BUFF_PARAMETER		aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
		};

		struct
		{
			sITEM_TBLDAT* pItemTbl;
		};
	};
};

typedef std::list<sNtlFakeBuff*>				LIST_FAKE_BUFF;
typedef std::list<sNtlFakeBuff*>::iterator		LIST_FAKE_BUFF_ITER;

class CNtlFakeBuffContainer : public RWS::CEventHandler
{
public:
	CNtlFakeBuffContainer();
	virtual ~CNtlFakeBuffContainer();

	RwBool			Create(SERIAL_HANDLE hOwner);
	void			Update(RwReal fElapsed);
	void			Destroy();

	void			AddBuff(eDBO_OBJECT_SOURCE eType, BYTE byBuffIndex, RwUInt32 uiTableIndex, RwReal fRemainTime, bool bActive, sDBO_BUFF_PARAMETER* aBuffParameter = NULL);
	void			DropBuff(eDBO_OBJECT_SOURCE eType, BYTE byBuffIndex);

	RwInt32			GetBuffCount(void);

	LIST_FAKE_BUFF_ITER	GetBeginBuff(void);
	LIST_FAKE_BUFF_ITER	GetEndBuff(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

protected:
	LIST_FAKE_BUFF		m_listBuff;
	SERIAL_HANDLE		m_hOwner;
};

#endif
