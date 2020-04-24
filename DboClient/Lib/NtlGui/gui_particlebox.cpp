#include "gui_precomp.h"
#include "gui_particlebox.h"
#include "gui_particlebox_generic.h"
#include "gui_particle_doc.h" 
#include "gui_particlepath.h"

START_GUI

CParticleBox::CParticleBox( CComponent* pParent, CSurfaceManager* pSurfaceManager )
: CComponent( pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CParticleBox_Generic( this );
}

CParticleBox::CParticleBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager )
: CComponent( rtRect, pParent, pSurfaceManager ), m_pImpl( NULL )
{
	m_pImpl = NTL_NEW CParticleBox_Generic( this );
}

CParticleBox::~CParticleBox(VOID)
{
	NTL_DELETE( m_pImpl );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attribute
VOID CParticleBox::Update( FLOAT fElapsed )
{
	m_pImpl->Update( fElapsed );
}

VOID CParticleBox::Play( BOOL bRepeat )
{
	m_pImpl->Play( bRepeat );
}

VOID CParticleBox::Stop()
{
	m_pImpl->Stop();
}

VOID CParticleBox::SetParticleDoc( CParticleDoc* pDoc )
{
	m_pImpl->SetDoc( pDoc );
}

VOID CParticleBox::Load( const char* pParticleName )
{
	m_pImpl->Load( pParticleName );
}

END_GUI