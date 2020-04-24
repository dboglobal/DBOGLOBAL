#include "precomp_ntlsound.h"
#include "NtlSoundSubSystem.h"

// sound
#include "NtlFadeInOut.h"
#include "NtlSoundEventHandler.h"
#include "NtlFMODSoundPool.h"


CNtlSoundSubSystem::CNtlSoundSubSystem()
:m_pSoundEventHandler(NULL)
{}

CNtlSoundSubSystem::~CNtlSoundSubSystem()
{

}

RwBool CNtlSoundSubSystem::Create()
{
	NTL_FUNCTION("CNtlSoundSubSystem::Create");

	m_pSoundEventHandler = NTL_NEW CNtlSoundEventHandler;
	if( !m_pSoundEventHandler->Create() )
		NTL_RETURN(FALSE);

	if( !CNtlFadeInOut::CreateInstance() )
		NTL_RETURN(FALSE);

	if( !CNtlFMODSoundPool::CreateInstance() )
		NTL_RETURN(FALSE);

	NTL_RETURN(TRUE);
}

void CNtlSoundSubSystem::Destroy()
{
	NTL_FUNCTION("CNtlSoundSubSystem::Destroy");

	CNtlFMODSoundPool::DestroyInstance();

	CNtlFadeInOut::DestroyInstance();

	if( m_pSoundEventHandler )
	{
		m_pSoundEventHandler->Destroy();
		NTL_DELETE( m_pSoundEventHandler );
	}

	NTL_RETURNVOID();
}