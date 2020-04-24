#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"

/**
 * \ingroup Client
 * \brief 커뮤니티 타겟 윈도우 GUI
 * \date 2007-12-26
 * \author agebreak
 */
class CCommuTargetGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CCommuTargetGui(void);
	CCommuTargetGui(const RwChar* pName);
	virtual ~CCommuTargetGui(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();	
	RwInt32		SwitchDialog(bool bOpen);

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

protected:
    VOID        OnEventTargetSelect(SNtlEventCommuTargetSelect* data);
    VOID        OnEventTargetRelease();

protected:
    gui::CStaticBox*    m_pSttName;                     ///< 타겟 이름 표시 
    gui::CStaticBox*    m_pSttType;                     ///< 타겟 종류 표시 (친구,길드...)

    SNtlEventCommuTargetSelect     m_currTarget;

};	
