/*****************************************************************************
*
* File			: NtlPLSceneManager.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 8. 01	
* Abstract		: Presentation scene manager base class 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLRENDERGROUP_H__
#define __NTL_PLRENDERGROUP_H__

#include <rwcore.h>
#include <rpworld.h>
#include <map>
#include <list>

class CNtlPLEntity;

/**
* \ingroup NtlPresentation
* 같은 종류의 entity를 group 관리하는 객체.
* stl 연관 배열 map을 사용한다.
*
*/

class CNtlPLRenderGroup
{
public:

	typedef std::map<RwUInt32, CNtlPLEntity*> MapEntity;

private:

	RwUInt32 m_uiLayer;
	MapEntity m_mapEntity;		/* entity map container **/

public:

	/**
	*  destructor for Presentation render group
	*
	*/

	~CNtlPLRenderGroup();

	/**
	*  render group 객체가 생성된 다음 호출하는 함수.
	*  \see Destroy
	*/
	RwBool Create(void);

	/**
	*  render group 객체가 삭제되기 전에 호출하는 함수.
	*  \see Create
	*/
	void Destroy(void);

	/**
	*  render group layer setting.
	*  \see GetLayer
	*/
	void SetLayer(RwUInt32 uiLayer);

	/**
	*  render group layer 얻어오기.
	*  \see SetLayer
	*/
	RwUInt32 GetLayer(void) const;

	/**
	*  group에 entity를 add 한다.
	*  \param pEntity는 add할 entity pointer이다.
	*  \see RemoveEntity
	*/
	RwBool AddEntity(const CNtlPLEntity *pEntity);

	/**
	*  group에 entity를 remove 한다.
	*  \param pEntity는 remove할 entity pointer이다.
	*  \see AddEntity
	*/
	void RemoveEntity(CNtlPLEntity *pEntity);

	/**
	*  group에서 해당 entity를 찾는다.
	*  \param pEntity는 container에서 찾을 entity pointer
	*  \return 성공하면 TRUE, 실패하면 FALSE
	*/
	RwBool FindEntity(const CNtlPLEntity *pEntity);

	/**
	*  group에서 entity 갯수를 구한다.
	*  \return entity 갯수.
	*/
	RwInt32 GetEntityCount(void);

	/**
	*  group에 해당하는 entity container 참조를 리턴한다.
	*  \return entity container(m_mapEntity) 참조를 리턴.
	*/
	MapEntity* GetEntities(void);

	/**
	*  group에 해당하는 모든 entity를 담는다.
	*/
	void Gets(std::list<CNtlPLEntity*>& listEntities);
};

inline void CNtlPLRenderGroup::SetLayer(RwUInt32 uiLayer)
{
	m_uiLayer = uiLayer;
}

inline RwUInt32 CNtlPLRenderGroup::GetLayer(void) const
{
	return m_uiLayer;
}


inline CNtlPLRenderGroup::MapEntity* CNtlPLRenderGroup::GetEntities(void)
{
	return &m_mapEntity;
}


#endif