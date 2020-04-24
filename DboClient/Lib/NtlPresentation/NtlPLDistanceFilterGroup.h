/*****************************************************************************
*
* File			: NtlPLDistanceFilterGroup.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2006. 12. 12	
* Abstract		: Presentation distance filtering group 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_PLDISTANCEFILTER_GROUP_H__
#define __NTL_PLDISTANCEFILTER_GROUP_H__

class CNtlPLEntity;

/**
* \ingroup NtlPresentation
* 같은 종류의 entity를 group 관리하는 객체.
* stl 연관 배열 map을 사용한다.
*
*/

class CNtlPLDistanceFilterGroup
{
public:

	enum { MAX_DIST_FILTER_GROUP = 200 };

	typedef std::map<CNtlPLEntity*, CNtlPLEntity*> MapSet;
	typedef std::list<MapSet*> ListGroup;


private:

	RwBool					m_bFirst;
	ListGroup::iterator		m_itCurr;
	ListGroup				m_listGroup;

	MapSet*		AllocateSet(void);
	void		DeAllocateSet(MapSet *pSet);
	MapSet*		FindInsert(void);

public:

	/**
	*  constructor for Presentation distance filtering group
	*
	*/

	CNtlPLDistanceFilterGroup();

	/**
	*  destructor for Presentation distance filtering group
	*
	*/

	~CNtlPLDistanceFilterGroup();

	/**
	*  distance filtering group 객체가 생성된 다음 호출하는 함수.
	*  \see Destroy
	*/
	RwBool Create(void);

	/**
	*  distance filtering group 객체가 삭제되기 전에 호출하는 함수.
	*  \see Create
	*/
	void Destroy(void);

	/**
	*  distance filtering group에 entity를 추가한다.
	*  \see RemoveEntity
	*/
	void AddEntity(CNtlPLEntity *pPLEntity);

	/**
	*  distance filtering group에 entity를 제거한다.
	*  \see RemoveEntity
	*/
	void RemoveEntity(CNtlPLEntity *pPLEntity);

	/**
	*  next update filter을 얻어온다.
	*/
	const MapSet* GetNextUpdateSet(void);

	/**
	*  distance filtering group을 얻어온다.
	*/
	const ListGroup& GetEntities(void) const;

	/**
	*  distance filtering set 갯수.
	*/
	RwInt32 GetFilterSetCount(void);

	/**
	*  distance filtering group에 포함된 총 entity의 갯수.
	*/
	RwInt32 GetEntityCount(void);
};

#endif
