#include "precomp_ntlsimulation.h"
#include "NtlSobCharPerfController.h"

#include "NtlTimer.h"
#include "NtlSLGlobal.h"
#include "NtlSob.h"
#include "NtlSobAvatar.h"
#include "NtlSobProxy.h"


RwReal CNtlSobCharPerfController::s_fStandFrame				= 5.f;
RwReal CNtlSobCharPerfController::s_fRemainIntervalFrame	= 0.5f;
RwReal CNtlSobCharPerfController::s_fIncRenderCharPerSec	= 10.f;
RwReal CNtlSobCharPerfController::s_fDecRenderCharPerSec	= 10.f;
RwReal CNtlSobCharPerfController::s_fMinRenderChar			= 20.f;


CNtlSobCharPerfController::CNtlSobCharPerfController( void )
{
	m_fCurRenderCnt = s_fMinRenderChar;
}

CNtlSobCharPerfController::~CNtlSobCharPerfController( void )
{
}

void CNtlSobCharPerfController::Update( RwReal fElapsedTime )
{
	if ( NULL == GetNtlSLGlobal()->GetSobAvatar() )
	{
		return;
	}

	JudgementRenderCounter( fElapsedTime );

	UpdateVisibility( fElapsedTime );
}

void CNtlSobCharPerfController::SobCreate( CNtlSob* pSobObj )
{
	if ( pSobObj && pSobObj->GetClassID() == SLCLASS_PLAYER )
	{
		sPerfEntity sEntity;
		sEntity.pSobObj = pSobObj;
		sEntity.pEffect = NULL;
		m_defSobPCList[pSobObj] = sEntity;
	}
}

void CNtlSobCharPerfController::SobDelete( CNtlSob* pSobObj )
{
	if ( pSobObj )
	{
		mapdef_PerfList::iterator it = m_defSobPCList.find( pSobObj );
		if ( it != m_defSobPCList.end() )
		{
			sPerfEntity& sEntity = it->second;

			if ( sEntity.pEffect )
			{
				sEntity.pSobObj->GetSobProxy()->RemoveVisualSystemEffectAlpha( sEntity.pEffect );
				sEntity.pEffect = NULL;
			}

			m_defSobPCList.erase( it );
		}
	}
}

void CNtlSobCharPerfController::Destroy( void )
{
	mapdef_PerfList::iterator it = m_defSobPCList.begin();
	for ( ; it != m_defSobPCList.end(); ++it )
	{
		sPerfEntity& sEntity = it->second;

		if ( sEntity.pEffect )
		{
			sEntity.pSobObj->GetSobProxy()->RemoveVisualSystemEffectAlpha( sEntity.pEffect );
			sEntity.pEffect = NULL;
		}
	}

	m_defSobPCList.clear();
}

void CNtlSobCharPerfController::JudgementRenderCounter( RwReal fElapsedTime )
{
	RwReal fEverFPS = CNtlTimer::GetFps();

	if ( s_fStandFrame + s_fRemainIntervalFrame < fEverFPS )
	{
		m_fCurRenderCnt += s_fIncRenderCharPerSec * fElapsedTime;
	}
	else if ( s_fStandFrame - s_fRemainIntervalFrame > fEverFPS )
	{
		m_fCurRenderCnt -= s_fDecRenderCharPerSec * fElapsedTime;
	}

	m_fCurRenderCnt = ( m_fCurRenderCnt < s_fMinRenderChar ? s_fMinRenderChar : ( m_fCurRenderCnt > m_defSobPCList.size() ? m_defSobPCList.size() : m_fCurRenderCnt ) );
}

void CNtlSobCharPerfController::UpdateVisibility( RwReal fElapsedTime )
{
	m_fVisibleResetTime -= fElapsedTime;

	if ( m_fVisibleResetTime > 0.f )
	{
		return;
	}

	m_fVisibleResetTime = VISIBLE_RESET_TIME;

	RwV3d vAvatarPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition(); vAvatarPos.y = 0.f;

	std::multimap< RwReal, sPerfEntity* > mmapdef_SortList;

	for ( mapdef_PerfList::iterator it = m_defSobPCList.begin(); it != m_defSobPCList.end(); ++it )
	{
		sPerfEntity& sEntity = it->second;

		//if ( NULL == sEntity.pEffect )
		//{
		//	sEntity.pEffect = sEntity.pSobObj->GetSobProxy()->AddVisualSystemEffectAlphaBlend( 0.f, 0.f, TRUE, TRUE );
		//}

		RwV3d vDestPos = sEntity.pSobObj->GetPosition(); vDestPos.y = 0.f;

		RwReal fSDist = (vAvatarPos.x - vDestPos.x) * (vAvatarPos.x - vDestPos.x) + (vAvatarPos.z - vDestPos.z) * (vAvatarPos.z - vDestPos.z);

		mmapdef_SortList.insert( std::multimap< RwReal, sPerfEntity* >::value_type( fSDist, &it->second ) );
	}

	RwInt32 nRenderCnt = (RwInt32) m_fCurRenderCnt;

	std::multimap< RwReal, sPerfEntity* >::iterator itSort = mmapdef_SortList.begin();

	for ( ; itSort != mmapdef_SortList.end(); ++itSort, --nRenderCnt )
	{
		sPerfEntity* pEntity = itSort->second;

		if ( nRenderCnt > 0 )
		{
			if ( pEntity->pEffect )
			{
				pEntity->pSobObj->GetSobProxy()->RemoveVisualSystemEffectAlpha( pEntity->pEffect );
				pEntity->pEffect = NULL;
			}
		}
		else
		{
			if ( NULL == pEntity->pEffect )
			{
				pEntity->pEffect = pEntity->pSobObj->GetSobProxy()->AddVisualSystemEffectAlphaBlend( 0.f, 0.f, TRUE, TRUE );
			}
		}
	}
}
