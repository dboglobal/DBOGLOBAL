#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptFreePvpZone.h"

// simulation
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSobGroup.h"
#include "NtlSobPet.h"
#include "NtlSLLogic.h"


CNtlWorldConceptFreePvpZone::CNtlWorldConceptFreePvpZone()
{
}

CNtlWorldConceptFreePvpZone::~CNtlWorldConceptFreePvpZone()
{
}

void CNtlWorldConceptFreePvpZone::Update(RwReal fElapsedTime)
{
	switch (m_iWorldState)
	{
		case WORLD_STATE_ENTER:
		{
			m_fStateTime += fElapsedTime;

			if (m_fStateTime >= 2.0f)
			{
				ChangeState(WORLD_STATE_IDLE);
			}
		}
		break;
		case WORLD_STATE_IDLE:
		break;
		case WORLD_STATE_EXIT:
		{
			m_fStateTime += fElapsedTime;
			if (m_fStateTime >= 2.0f)
			{
				ChangeState(WORLD_STATE_NONE);
			}
		}
		break;

		default: break;
	}
}

void CNtlWorldConceptFreePvpZone::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);
}

CNtlSobActor* CNtlWorldConceptFreePvpZone::GetNearAttackableEnemy(CNtlSobActor* pActor)
{
	if (!pActor)
		return NULL;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
	{
		DBO_FAIL("Not exist avatar instance");
		return NULL;
	}

	CNtlSobActor* pNearestTarget = NULL;
	RwV3d			v3AvatarPos = pAvatar->GetPosition();
	RwReal			fMinLength = 100.f;
	RwV3d			v3ActorPos, v3Sub;

	enum eSEARCH_TYPE
	{
		SEARCH_TYPE_PLAYER,
		SEARCH_TYPE_PET,

		NUM_SEARCH_TYPE
	};

	CNtlSobGroup* pSobGroup = NULL;
	CNtlSobGroup::MapObject::iterator it;
	for (RwUInt8 i = 0; i < NUM_SEARCH_TYPE; ++i)
	{
		if (NUM_SEARCH_TYPE <= i)
		{
			DBO_FAIL("Invalid index : " << i);
			continue;
		}

		switch (i)
		{
			case SEARCH_TYPE_PLAYER:
			{
				pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
				break;
			}
			case SEARCH_TYPE_PET:
			{
				pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PET);
				break;
			}
			default:
			{
				DBO_FAIL("Invalid index : " << i);
				break;
			}
		}

		if (!pSobGroup)
			continue;

		for (it = pSobGroup->GetObjects().begin(); it != pSobGroup->GetObjects().end(); ++it)
		{
			CNtlSobPlayer* pSobPlayer = NULL;

			switch (i)
			{
				case SEARCH_TYPE_PLAYER:
				{
					pSobPlayer = reinterpret_cast<CNtlSobPlayer*>(it->second);
					break;
				}
				case SEARCH_TYPE_PET:
				{
					CNtlSobPet* pSobPet = reinterpret_cast<CNtlSobPet*>(it->second);
					pSobPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject(pSobPet->GetOwnerID()));
					if (!pSobPlayer)
						DBO_FAIL("Not exist owner player of pet of handle : " << pSobPet->GetOwnerID());

					break;
				}
				default:
				{
					DBO_FAIL("Invalid index : " << i);
					break;
				}
			}

			if (!pSobPlayer || pSobPlayer->IsDie())
			{
				continue;
			}

			v3ActorPos = pSobPlayer->GetPosition();
			RwV3dSubMacro(&v3Sub, &v3ActorPos, &v3AvatarPos);
			RwReal fLength = RwV3dLength(&v3Sub);

			if (fLength > 50.0f)
				continue;

			if (fLength < fMinLength)
			{
				CNtlSobActor * pTargetActor = reinterpret_cast<CNtlSobActor*>(pSobPlayer);
				if (!Logic_CanTargetted(pTargetActor) || !Logic_CanAttacked(pTargetActor))
					continue;

				if (GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor(pAvatar, pTargetActor) == FALSE)
					continue;

			
				fMinLength = fLength;
				pNearestTarget = pTargetActor;
			}
		}
	}

	return pNearestTarget;
}

void CNtlWorldConceptFreePvpZone::UpdateNames()
{
}
