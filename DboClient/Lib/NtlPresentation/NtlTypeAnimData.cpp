#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlTypeAnimData.h"

CNtlTypeAnimTable::~CNtlTypeAnimTable() 
{
    Destroy();
}

void CNtlTypeAnimTable::Create()
{
	//m_strFilePath = strFilePath;
	//Add(COMMON_NORMAL_IDLE_LOOP, "A_HFI_F_Idle1.anm");
}

/**
 * .anm Resource 해제
 */
void CNtlTypeAnimTable::Destroy()
{
	TYPE_ANIM_MAP_ITER stAnimMapIT  = m_mapTypeAnim.begin();
	TYPE_ANIM_MAP_ITER endAnimMapIT = m_mapTypeAnim.end();
	for(;stAnimMapIT != endAnimMapIT;)
	{
		STypeAnimData *pTypeAnimData = stAnimMapIT->second;
		if(pTypeAnimData)
		{
			NTL_DELETE(pTypeAnimData);
		}
		
		++stAnimMapIT;
	}
	
	m_mapTypeAnim.clear();
}


/**
 * Key값으로 vaule를 생성을 한다. 초기 생성시에만 사용
 * \param strAnimName Animation 파일 이름
 * \return STypeAnimData
 */
STypeAnimData *CNtlTypeAnimTable::Add(RwUInt32 uiKey, const std::string &strAnimName)
{
    // 중복 데이터가 들어오는 부분을 체크한다 (by agebreak. 2006.4.20)
    STypeAnimData *pTypeAnimData = NULL;

    if(m_mapTypeAnim.find(uiKey) != m_mapTypeAnim.end())
    {
        pTypeAnimData = m_mapTypeAnim[uiKey];    
    }
    else
    {
	    pTypeAnimData = NTL_NEW STypeAnimData;	    
    }

    pTypeAnimData->strAnimName = strAnimName;
    pTypeAnimData->uiAnimKey = uiKey;
    m_mapTypeAnim[uiKey] = pTypeAnimData;
	
	return m_mapTypeAnim[uiKey];
}


STypeAnimData *CNtlTypeAnimTable::Add(RwUInt32 uiKey, STypeAnimData *pTypeAnimData)
{
    // 기존의 코드로는 새로운 데이터를 추가할수가 없다.=_=; (By agebreak. 2006.4.25)	
	pTypeAnimData->uiAnimKey = uiKey;
	m_mapTypeAnim[uiKey] = pTypeAnimData;
	return m_mapTypeAnim[uiKey];
}

/**
 * 내부적으로 사용하는 Map을 Return 한다.
 * \return TYPE_ANIM_MAP *
 */
TYPE_ANIM_MAP *CNtlTypeAnimTable::GetTypeAnimMap()
{
	return &m_mapTypeAnim;
}

/**
 * Key값으로 STypeAnimData를 얻는다.
 * \param uiKey 고유 Key 값
 * \return STypeAnimData *
 */
STypeAnimData *CNtlTypeAnimTable::Get(RwUInt32 uiKey)
{
	if(m_mapTypeAnim.find( uiKey ) == m_mapTypeAnim.end())
		return NULL;
	
	STypeAnimData *pTypeAnimData = m_mapTypeAnim[uiKey];
	if(pTypeAnimData == NULL)
		return NULL;
		
	return pTypeAnimData;
}

/**
 * Key값으로 AnimFile을 얻는다.
 * \param uiKey 고유 Key 값
 * \return Animation File Name
 */
const std::string *CNtlTypeAnimTable::GetFileName(RwUInt32 uiKey)
{
	if(m_mapTypeAnim.find( uiKey ) == m_mapTypeAnim.end())
		return NULL;
	
	STypeAnimData *pTypeAnimData = m_mapTypeAnim[uiKey];
	if(pTypeAnimData == NULL)
		return NULL;
	
	return (&pTypeAnimData->strAnimName);
}

void CNtlTypeAnimTable::Remove(RwUInt32 uiKey)
{
    if(m_mapTypeAnim.find(uiKey) == m_mapTypeAnim.end())
        return;

    STypeAnimData* pTypeAnimData = m_mapTypeAnim[uiKey];
    if(pTypeAnimData)
    {
        NTL_DELETE(pTypeAnimData);
        pTypeAnimData = NULL;
    }

    m_mapTypeAnim.erase(uiKey);
}

void CNtlTypeAnimTable::SaveSerialize(  CNtlSerializer& sOut ) 
{
	sOut<<m_strAnimPath;
	sOut<<m_mapTypeAnim.size();				// Size
	TYPE_ANIM_MAP_ITER it = m_mapTypeAnim.begin();
	for(; it != m_mapTypeAnim.end(); ++it)
	{
		STypeAnimData* pTypeAnimData = it->second;
		if(!pTypeAnimData)
			continue;

		sOut<<pTypeAnimData->strAnimName;			// Anim Name
		sOut<<pTypeAnimData->uiAnimKey;				// Anim Key
		sOut<<pTypeAnimData->fPlayTime;				// Play Time
        sOut<<pTypeAnimData->flagAnim;              // Anim Flag

		sOut<<pTypeAnimData->vecAnimEvent.size();	// Event Size		
		for each(SEventAnim* pEventAnim in pTypeAnimData->vecAnimEvent)
		{
			if(pEventAnim)
			{
				sOut<<pEventAnim->eEventID;			// 이벤트 타입을 헤더로 넣는다.
				switch(pEventAnim->eEventID)
				{
				case EVENT_ANIM_HIT:	
                    {
                        SEventAnimHit* pEventAnimHit = (SEventAnimHit*)pEventAnim;
                        pEventAnimHit->SaveSerialize(sOut);
                    }                    
					break;
				case EVENT_ANIM_VISUAL_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventVisualEffect));
					break;
				case EVENT_ANIM_VISUAL_SOUND:					
					sOut.In(pEventAnim, sizeof(SEventSound));
					break;
				case EVENT_ANIM_FOOT_STEP:					
					sOut.In(pEventAnim, sizeof(SEventFootStep));
					break;
				case EVENT_ANIM_LINK_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventLinkEffect));
					break;
				case EVENT_ANIM_WEIGHT_TIME:					
					sOut.In(pEventAnim, sizeof(SEventWeightTime));
					break;
				case EVENT_ANIM_TRACE_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventTrace));
					break;
				case EVENT_ANIM_SUB_WEAPON:					
					sOut.In(pEventAnim, sizeof(SEventSubWeapon));
					break;
				case EVENT_ANIM_POST_EFFECT:					
					sOut.In(pEventAnim, sizeof(SEventPostEffect));					
					break;		
				case EVENT_ANIM_SUMMON_PET:
					sOut.In(pEventAnim, sizeof(SEventSummonPet));
					break;
                case EVENT_ANIM_TMQ:
                    sOut.In(pEventAnim, sizeof(SEventAnimCinematic));
                    break;
                case EVENT_ANIM_ALPHA:
                    sOut.In(pEventAnim, sizeof(SEventAlpha));
                    break;
                case EVENT_ANIM_EXPLOSION:
                    sOut.In(pEventAnim, sizeof(SEventExplosion));
                    break;
                case EVENT_ANIM_DIRECT:
                    sOut.In(pEventAnim, sizeof(SEventDirect));
                    break;
                case EVENT_ANIM_COLOR_CHANGE:
                    sOut.In(pEventAnim, sizeof(SEventColorChange));
                    break;
                case EVENT_ANIM_STRETCH:
                    sOut.In(pEventAnim, sizeof(SEventStretch));
                    break;
                case EVENT_ANIM_TRIGGER:
                    sOut.In(pEventAnim, sizeof(SEventTrigger));
                    break;
                case EVENT_ANIM_SKILL_CANCEL:
                    sOut.In(pEventAnim, sizeof(SEventSkillCancel));
                    break;
				default:
					assert(!"Data is Wrong");
					break;
				}				
			}
		}
	}
}

void CNtlTypeAnimTable::LoadSerialize(  CNtlSerializer& sIn ) 
{
	sIn>>m_strAnimPath;
	UINT nAnimDataCount = 0;
	sIn>>nAnimDataCount;

	for(UINT i = 0; i < nAnimDataCount; ++i)
	{
		STypeAnimData* pTypeAnimData = NTL_NEW STypeAnimData();
		sIn>>pTypeAnimData->strAnimName;
		sIn>>pTypeAnimData->uiAnimKey;
		sIn>>pTypeAnimData->fPlayTime;
        sIn>>pTypeAnimData->flagAnim;
        Add(pTypeAnimData->uiAnimKey, pTypeAnimData);

		UINT nAnimEventSize = 0;
		sIn>>nAnimEventSize;
		for(UINT j = 0; j < nAnimEventSize; ++j)
		{
			int nEventID = 0;
			sIn>>nEventID;

			SEventAnim* pEventAnim = NULL;
			switch(nEventID)
			{
			case EVENT_ANIM_HIT:
                {
                    pEventAnim = NTL_NEW SEventAnimHit();				
                    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)pEventAnim;
                    pEventAnimHit->LoadSerialize(sIn);
                }				
				break;
			case EVENT_ANIM_VISUAL_EFFECT:
				pEventAnim = NTL_NEW SEventVisualEffect();
				sIn.Out(pEventAnim, sizeof(SEventVisualEffect));
				break;
			case EVENT_ANIM_VISUAL_SOUND:
				pEventAnim = NTL_NEW SEventSound();
				sIn.Out(pEventAnim, sizeof(SEventSound));
				break;
			case EVENT_ANIM_FOOT_STEP:
				pEventAnim = NTL_NEW SEventFootStep();
				sIn.Out(pEventAnim, sizeof(SEventFootStep));
				break;
			case EVENT_ANIM_LINK_EFFECT:
				pEventAnim = NTL_NEW SEventLinkEffect();
				sIn.Out(pEventAnim, sizeof(SEventLinkEffect));
				break;
			case EVENT_ANIM_WEIGHT_TIME:
				pEventAnim = NTL_NEW SEventWeightTime();
				sIn.Out(pEventAnim, sizeof(SEventWeightTime));
				break;
			case EVENT_ANIM_TRACE_EFFECT:
				pEventAnim = NTL_NEW SEventTrace();
				sIn.Out(pEventAnim, sizeof(SEventTrace));
				break;
			case EVENT_ANIM_SUB_WEAPON:
				pEventAnim = NTL_NEW SEventSubWeapon();
				sIn.Out(pEventAnim, sizeof(SEventSubWeapon));
				break;
			case EVENT_ANIM_POST_EFFECT:
				pEventAnim = NTL_NEW SEventPostEffect();
				sIn.Out(pEventAnim, sizeof(SEventPostEffect));
				break;		
			case EVENT_ANIM_SUMMON_PET:
				pEventAnim = NTL_NEW SEventSummonPet();
				sIn.Out(pEventAnim, sizeof(SEventSummonPet));
				break;
            case EVENT_ANIM_TMQ:
				pEventAnim = NTL_NEW SEventAnimCinematic;
				sIn.Out(pEventAnim, sizeof(SEventAnimCinematic));
                break;
            case EVENT_ANIM_ALPHA:
                pEventAnim = NTL_NEW SEventAlpha();
                sIn.Out(pEventAnim, sizeof(SEventAlpha));
                break;
            case EVENT_ANIM_EXPLOSION:
                pEventAnim = NTL_NEW SEventExplosion();
                sIn.Out(pEventAnim, sizeof(SEventExplosion));
                break;
            case EVENT_ANIM_DIRECT:
                pEventAnim = NTL_NEW SEventDirect();
                sIn.Out(pEventAnim, sizeof(SEventDirect));
                break;
            case EVENT_ANIM_COLOR_CHANGE:
                pEventAnim = NTL_NEW SEventColorChange();
                sIn.Out(pEventAnim, sizeof(SEventColorChange));
                break;
            case EVENT_ANIM_STRETCH:
                pEventAnim = NTL_NEW SEventStretch();
                sIn.Out(pEventAnim, sizeof(SEventStretch));
                break;
            case EVENT_ANIM_TRIGGER:
                pEventAnim = NTL_NEW SEventTrigger();
                sIn.Out(pEventAnim, sizeof(SEventTrigger));
                break;
            case EVENT_ANIM_SKILL_CANCEL:
                pEventAnim = NTL_NEW SEventSkillCancel();
                sIn.Out(pEventAnim, sizeof(SEventSkillCancel));
                break;
            default:
                assert(!"Data is Wrong");
                break;
			}

			DBO_ASSERT(pEventAnim, "CNtlTypeAnimTable::LoadSerialize::pEventAnim, NULL");
			if(pEventAnim)
			{
				pTypeAnimData->vecAnimEvent.push_back(pEventAnim);
			}			
		}		
	}
}
