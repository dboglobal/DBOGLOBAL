////////////////////////////////////////////////////////////////////////////////
// File: CinematicGui.h
// Desc: CinematicGui
//
// 2007.8.3 Peessi@hitel.net   
//
// To Be Desired:
////////////////////////////////////////////////////////////////////////////////

#ifndef __CINEMATIC_GUI_H__
#define __CINEMATIC_GUI_H__

#include "ceventhandler.h"
#include "gui_precomp.h"
#include "NtlPLCinematicGui.h"

class CCinematicGui : public CNtlPLCinematicGui
{

protected:
	//! Implement
	VOID	SetCinematicBalloon( void* pData );
	//VOID	SetCinematicFlash( void* pData );

	VOID	SetCinematicState( CINEMATICSTATE eState );
	VOID	SetBalloonState( BALLOONSTATE eState );

	virtual void	SkipCinematics(UINT32 hPc);

private:

	void	SetSkipAble(bool bFlag);


};

#endif//__QUEST_NARRATION_GUI_H__