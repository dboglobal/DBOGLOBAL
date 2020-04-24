/*****************************************************************************
 *
 * File			: NtlSobGroup.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation entity group class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOB_GROUP_H__
#define __NTL_SOB_GROUP_H__

class CNtlSob;

class CNtlSobGroup
{
public:

	typedef std::map<RwUInt32, CNtlSob*>				MapObject;
	typedef std::map<RwUInt32, CNtlSob*>::iterator		MapObject_Iter;

private:

	MapObject m_mapObject;

	CNtlSob* FindEntity(RwUInt32 uiKey);

public:

	~CNtlSobGroup();

	RwBool Create(void);
	void Destroy(void);

	RwBool AddEntity(CNtlSob *pObj);
	void RemoveEntity(CNtlSob *pObj);
	MapObject& GetObjects(void);
	RwUInt32 GetCount(void);

	MapObject_Iter Begin( void );
	MapObject_Iter End( void );
};


#endif