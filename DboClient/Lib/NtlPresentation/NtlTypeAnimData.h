/*****************************************************************************
 *
 * File			: NtlTypeAnimData.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2006. 4. 11	
 * Abstract		: NTL NtlTypeAnimData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_TYPE_ANIM_DATA_H__
#define __NTL_TYPE_ANIM_DATA_H__

#include "NtlAnimEventData.h"
#include "NtlDebug.h"
#include "NtlCharacterData.h"
#include "NtlSerializer.h"

/// 플래그 관련 설정들
#define ANIM_FLAG_CULL_TEST_ALL_ATOMIC  0x00000001

/*!
 * \Animation
 * STypeAnimData: 변하지 않는 Data(Read만 가능)
 * 
 */
class AnimEvent_CompareFunc : public std::binary_function<SEventAnim *, SEventAnim *, bool> 
{
public:
	bool operator() (const SEventAnim* lhs, const SEventAnim* rhs) const
	{
		//if(lhs->fTime != ANIM_EVENT_NONE_TIME && rhs->fTime != ANIM_EVENT_NONE_TIME)
			return lhs->fTime < rhs->fTime;

		//return false;
	}
};

struct STypeAnimData
{
	std::string			strAnimName;							///< Animation File 이름
	RwUInt32			uiAnimKey;								///< Animation Key
	RwReal              fPlayTime;                              ///< Animation의 PlayTime
    RwUInt32            flagAnim;                               ///< 애니메이션 관련된 설정 플래그

	std::vector<SEventAnim *> vecAnimEvent;						///< Animation Event들 (fTime에 정렬이 되어 있어야 한다.)
	RwBool						m_bReLoad;						///< Resource를 새로 읽어야 할 경우
	
	STypeAnimData() : uiAnimKey(INVALID_GRAPHIC_ANIMATION_ID), m_bReLoad(FALSE), fPlayTime(0.0f), flagAnim(0)
	{
		vecAnimEvent.reserve(15);
	}

	~STypeAnimData()
	{
		if(vecAnimEvent.size())
		{
			for(RwUInt32 i = 0; i < vecAnimEvent.size(); i++)
			{
				if(vecAnimEvent[i] != NULL)
				{
                    // Hit Event에는 Multi Hissidan 데이터가 동적으로 할당되어 있기 때문에
                    // 형변환을 한후에 지워줘야 한다.
                    if(vecAnimEvent[i]->eEventID == EVENT_ANIM_HIT)
                    {
                        SEventAnimHit* pEventAnimHit = (SEventAnimHit*)vecAnimEvent[i];
                        NTL_DELETE(pEventAnimHit);
                    }
                    else
                    {
					    NTL_DELETE (vecAnimEvent[i]);
                    }
				}
			}
		}

		vecAnimEvent.clear();
	}

	void AnimEventSort()
	{
		AnimEvent_CompareFunc cf;
		sort(vecAnimEvent.begin(), vecAnimEvent.end(), cf);
	}

    //--------- 플래그 관련
    void    SetCullTestAllAtomic(RwBool bFlag) {flagAnim |= ANIM_FLAG_CULL_TEST_ALL_ATOMIC;}
    RwBool  IsCullTestAllAtomic() {return flagAnim & ANIM_FLAG_CULL_TEST_ALL_ATOMIC;}
};


typedef	std::map<RwUInt32, STypeAnimData *>		      TYPE_ANIM_MAP;
typedef	std::map<RwUInt32, STypeAnimData *>::iterator TYPE_ANIM_MAP_ITER;

class CNtlTypeAnimTable
{
public:
	CNtlTypeAnimTable() {};
	~CNtlTypeAnimTable();

protected:
	TYPE_ANIM_MAP	m_mapTypeAnim;	
    std::string     m_strAnimPath;                                          ///< Animation Folder Path

public:
	void	Create();
	void	Destroy();
	
	STypeAnimData	*Get(RwUInt32 uiKey);									///< Get STypeAnimData
	STypeAnimData	*Add(RwUInt32 uiKey, const std::string &strAnimName);	///< Add STypeAnimData
    STypeAnimData	*Add(RwUInt32 uiKey, STypeAnimData *pTypeAnimData);     ///< Add STypeAnimData 
    void            Remove(RwUInt32 uiKey);                                 ///< 맵에서 데이터를 제거한다. (by Agebreak. 2006.05.01)

    TYPE_ANIM_MAP	*GetTypeAnimMap();

	const std::string	*GetFileName(RwUInt32 uiKey);								///< Get AnimName

    void SetAnimPath(std::string& strAnimPath) { m_strAnimPath = strAnimPath;}
    std::string GetAnimPath() {return m_strAnimPath;}

	void	SaveSerialize( CNtlSerializer& sOut);						///< Serialize 객체에 데이터를 저장한다.
	void	LoadSerialize( CNtlSerializer& sIn);						///< Serailize 객체에서 데이터를 로드한다.
};

#endif