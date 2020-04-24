#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptScramble.h"

// simulation
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSobGroup.h"
#include "NtlSobPet.h"
#include "NtlSLLogic.h"


CNtlWorldConceptScramble::CNtlWorldConceptScramble(void)
{
}

CNtlWorldConceptScramble::~CNtlWorldConceptScramble(void)
{
}

RwBool CNtlWorldConceptScramble::IsEnableAction(EAvatarAction eAction)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CNtlDojo*		pDojo			= pAvatar->GetDojo();
	sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();
	if( !pSCRAMBLE_INFO )
	{
		DBO_FAIL("Not exist scramble infomation");
		return FALSE;
	}

	switch(eAction)
	{	
	case E_ACTION_MOVE:
	case E_ACTION_PICK_WORLD:
	case E_ACTION_PICK_OBJECT:
	case E_ACTION_ATTACK:
		{
			if( eDBO_DOJO_STATUS_START > pSCRAMBLE_INFO->eDojoState )
				return FALSE;

			break;
		}
	}

	return TRUE;
}

void CNtlWorldConceptScramble::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);

	if( WORLD_STATE_ENTER == iState )
		ChangeState(WORLD_STATE_IDLE);
}

RwBool CNtlWorldConceptScramble::IsAttackableEnemy(RwUInt32 uiHandle)
{
	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(uiHandle);
	if( !pSob )
		return FALSE;

	RwUInt32		uiClassID	= pSob->GetClassID();
	CNtlSobPlayer*	pSobPlayer	= NULL;

	switch(uiClassID)
	{
	case SLCLASS_PLAYER:
		{
			pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( pSob );
			break;
		}
	case SLCLASS_PET:
		{
			CNtlSobPet*	pSobPet	= reinterpret_cast<CNtlSobPet*>( pSob );
			
			pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pSobPet->GetOwnerID()) );
			if( !pSobPlayer )
			{
				DBO_FAIL("Not exist owner player of pet of handle : " << pSobPet->GetOwnerID());
				return FALSE;
			}

			break;
		}
	default:
		{
			return FALSE;
		}
	}	

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return FALSE;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	return IsEnemy(pSobPlayer->GetGuildName(), pGuild->GetGuildName());
}

RwBool CNtlWorldConceptScramble::IsEnemy(const WCHAR* pwcAvatarGuildName, const WCHAR* pwcOtherGuildName)
{
	if( !pwcAvatarGuildName || !pwcOtherGuildName )
		return FALSE;

	if( wcscmp(pwcAvatarGuildName, pwcOtherGuildName) == 0 )
		return FALSE;

	return TRUE;
}

CNtlSobActor* CNtlWorldConceptScramble::GetNearAttackableEnemy( CNtlSobActor* pActor )
{
	if( !pActor )
		return NULL;	

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return NULL;
	}

	CNtlGuild*		pGuild					= pAvatar->GetGuild();
	const WCHAR*	pAvatarGuildName		= pGuild->GetGuildName();	
	CNtlSobActor*	pNearestTarget			= NULL;
	RwV3d			v3AvatarPos				= pAvatar->GetPosition();
	RwReal			fMinLength				= 100.f;
	RwV3d			v3ActorPos, v3Sub;

	enum eSEARCH_TYPE
	{
		SEARCH_TYPE_PLAYER,
		SEARCH_TYPE_PET,

		NUM_SEARCH_TYPE
	};

	CNtlSobGroup* pSobGroup = NULL;
	CNtlSobGroup::MapObject::iterator it;
	for( RwUInt8 i = 0 ; i < NUM_SEARCH_TYPE ; ++i )
	{
		if( NUM_SEARCH_TYPE <= i )
		{
			DBO_FAIL("Invalid index : " << i);
			continue;
		}

		switch( i )
		{
		case SEARCH_TYPE_PLAYER:
			{
				pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PLAYER );
				break;
			}
		case SEARCH_TYPE_PET:
			{
				pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_PET );
				break;
			}
		default:
			{
				DBO_FAIL("Invalid index : " << i);
				break;
			}
		}

		if( !pSobGroup )
			continue;

		for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
		{
			CNtlSobPlayer*	pSobPlayer	= NULL;

			switch( i )
			{
			case SEARCH_TYPE_PLAYER:
				{
					pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );
					break;
				}
			case SEARCH_TYPE_PET:
				{
					CNtlSobPet*	pSobPet	= reinterpret_cast<CNtlSobPet*>( it->second );
					pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pSobPet->GetOwnerID()) );
					if( !pSobPlayer )
						DBO_FAIL("Not exist owner player of pet of handle : " << pSobPet->GetOwnerID());

					break;
				}
			default:
				{
					DBO_FAIL("Invalid index : " << i);
					break;
				}
			}

			if( !pSobPlayer )
				continue;

			if( false == IsEnemy(pAvatarGuildName, pSobPlayer->GetGuildName()) )
				continue;

			CNtlSobActor* pTargetActor = reinterpret_cast<CNtlSobActor*>( pSobPlayer );
			if(!Logic_CanTargetted(pTargetActor) || !Logic_CanAttacked(pTargetActor))
				continue;

			v3ActorPos = pTargetActor->GetPosition();
			RwV3dSubMacro( &v3Sub, &v3ActorPos, &v3AvatarPos );
			RwReal fLength = RwV3dLength( &v3Sub );

			if( fLength > 50.0f )
				continue;

			if( fLength < fMinLength )
			{
				fMinLength = fLength;

				switch( i )
				{
				case SEARCH_TYPE_PLAYER:
					{
						pNearestTarget	= pTargetActor;
						break;
					}
				case SEARCH_TYPE_PET:
					{
						pNearestTarget	= pTargetActor;
						break;
					}
				default:
					{
						DBO_FAIL("Invalid index : " << i);
						break;
					}
				}
			}
		}
	}

	return pNearestTarget;
}