/*****************************************************************************
* File			: gui_particlebox.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 9. 18
* Abstract		: 
*****************************************************************************
* Desc          : DBO 파티클의 컴포넌트
*****************************************************************************/
#ifndef __GUI_PARTICLE_Box_H__
#define __GUI_PARTICLE_Box_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CParticleBox_Generic;
class CParticleDoc;

//: ParticleBox component
class CParticleBox : public CComponent
{
public:
	//: ParticleBox Constructor
	CParticleBox( CComponent* pParent, CSurfaceManager* pSurfaceManager );
	CParticleBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager );

	//: ParticleBox Destructor
	virtual ~CParticleBox(VOID);

	VOID Play( BOOL bRepeat );
	VOID Stop();

	//! Update;
	VOID Update( FLOAT fElapsed );

	VOID SetParticleDoc( CParticleDoc* pDoc );
	VOID Load( const char* pParticleName );
		
protected:
	//! Variables
	CParticleBox_Generic *m_pImpl;

private:
	//! Etc
	CParticleBox( const CPanel &copy ) : CComponent(NULL,NULL) { return; } // disallow copy construction.

	friend class CParticleBox_Generic;	
};  

END_GUI

#endif//__GUI_PARTICLE_Box_H__
