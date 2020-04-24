/*****************************************************************************
* File			: gui_particlebox_generic.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 9. 18
* Abstract		: 
*****************************************************************************
* Desc          : DBO 파티클 박스의 Generic 클래스
*****************************************************************************/

#ifndef __GUI_PARTICLE_BOX_GENERIC_H__
#define __GUI_PARTICLE_BOX_GENERIC_H__

#include "gui_surface.h"
#include "gui_particle_doc.h"

START_GUI

class CParticleSystem;

class CParticleBox_Generic
{
public:
	// Construction & Destruction
	CParticleBox_Generic( CParticleBox* pSelf );
	~CParticleBox_Generic(VOID);

	void Play(BOOL bRepeat);
	void Stop();

	VOID Update( FLOAT fElapsed );

	void SetDoc( CParticleDoc* pDoc );
	void Load( const char* pParticleName );
	void Load( CParticleDoc* doc );

protected:
	// CallBack:
	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnMove(INT nX,INT nY);
	
	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;

	typedef std::list<CParticleSystem*>				LISTPS;
	typedef std::list<CParticleSystem*>::iterator	LISTPSIT;
	LISTPS		m_listParticle;

	BOOL				m_bPlay;
	BOOL				m_bRepeat;
	float				m_fPlayTime;
	float				m_fLifeTime;

	CParticleDoc*		m_pDoc;
		
public:	
	CParticleBox*			m_pParticleBox;
};  

END_GUI

#endif//
