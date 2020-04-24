/*****************************************************************************
 *
 * File			: SkillHTBRpResult.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2008. 11. 26	
 * Abstract		: HTB Skill RP Result
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_HTBSKILL_RPRESULT_H__
#define __NTL_HTBSKILL_RPRESULT_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

class CSkillHTBRpResult : public CNtlPLGui, public RWS::CEventHandler
{
private:

	gui::CFlash*	m_pflaResult;
	RwBool			m_bAddUpdate;

protected:

	gui::CSlot		m_slotMovieEnd;

private:

	void			LoadFlash(const RwChar *pFlashName);
	void			DestroyFlash(void);

public:

	CSkillHTBRpResult(const RwChar* pName);
	~CSkillHTBRpResult(void);

	RwBool			Create(void);

	void			Destroy(void);

	//! Operation
	virtual void	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	virtual void	HandleEvents( RWS::CMsg& msg );

public:

	void			OnMovieEnd(gui::CComponent* pComponent);
};

#endif
