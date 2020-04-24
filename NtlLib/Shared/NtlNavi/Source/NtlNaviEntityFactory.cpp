#include "precomp_navi.h"
#include "NtlNaviEntityFactory.h"
#include "NtlNaviEntity.h"
#include "NtlNaviWorld_WE.h"
#include "NtlNaviWorldProp_WE.h"
#include "NtlNaviFieldProp_WE.h"
#include "NtlNaviFieldGroup_WE.h"
#include "NtlNaviFieldHeight_WE.h"
#include "NtlNaviWorld_PE.h"
#include "NtlNaviWorldProp_PE.h"
#include "NtlNaviWorldGroup_PE.h"
#include "NtlNaviFieldGroup_PE.h"


CNtlNaviEntityFactory::mapdef_EntityList CNtlNaviEntityFactory::s_defWEEntityList;

CNtlNaviEntityFactory::mapdef_EntityList CNtlNaviEntityFactory::s_defPEEntityList;


CNtlNaviWorld_WE* CNtlNaviEntityFactory::CreateEntity_World_WE( void )
{
	CNtlNaviWorld_WE* pEntity = new CNtlNaviWorld_WE;

	s_defWEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviWorldProp_WE* CNtlNaviEntityFactory::CreateEntity_WorldProp_WE( void )
{
	CNtlNaviWorldProp_WE* pEntity = new CNtlNaviWorldProp_WE;

	s_defWEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviFieldProp_WE* CNtlNaviEntityFactory::CreateEntity_FieldProp_WE( void )
{
	CNtlNaviFieldProp_WE* pEntity = new CNtlNaviFieldProp_WE;

	s_defWEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviFieldGroup_WE* CNtlNaviEntityFactory::CreateEntity_FieldGroup_WE( void )
{
	CNtlNaviFieldGroup_WE* pEntity = new CNtlNaviFieldGroup_WE;

	s_defWEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviFieldHeight_WE* CNtlNaviEntityFactory::CreateEntity_FieldHeight_WE( void )
{
	CNtlNaviFieldHeight_WE* pEntity = new CNtlNaviFieldHeight_WE;

	s_defWEEntityList[pEntity] = pEntity;

	return pEntity;
}

void CNtlNaviEntityFactory::DeleteEntity_WE( CNtlNaviEntity* pEntity )
{
	mapdef_EntityList::iterator it = s_defWEEntityList.find( pEntity );
	if ( it != s_defWEEntityList.end() )
	{
		delete pEntity;
		s_defWEEntityList.erase( it );
	}
}

void CNtlNaviEntityFactory::DeleteAllEntity_WE( void )
{
	mapdef_EntityList::iterator it = s_defWEEntityList.begin();
	for ( ; it != s_defWEEntityList.end(); ++it )
	{
		delete it->second;
	}
	s_defWEEntityList.clear();
}

CNtlNaviWorld_PE* CNtlNaviEntityFactory::CreateEntity_World_PE( void )
{
	CNtlNaviWorld_PE* pEntity = new CNtlNaviWorld_PE;

	s_defPEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviWorldProp_PE* CNtlNaviEntityFactory::CreateEntity_WorldProp_PE( void )
{
	CNtlNaviWorldProp_PE* pEntity = new CNtlNaviWorldProp_PE;

	s_defPEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviWorldGroup_PE* CNtlNaviEntityFactory::CreateEntity_WorldGroup_PE( void )
{
	CNtlNaviWorldGroup_PE* pEntity = new CNtlNaviWorldGroup_PE;

	s_defPEEntityList[pEntity] = pEntity;

	return pEntity;
}

CNtlNaviFieldGroup_PE* CNtlNaviEntityFactory::CreateEntity_FieldGroup_PE( void )
{
	CNtlNaviFieldGroup_PE* pEntity = new CNtlNaviFieldGroup_PE;

	s_defPEEntityList[pEntity] = pEntity;

	return pEntity;
}

void CNtlNaviEntityFactory::DeleteEntity_PE( CNtlNaviEntity* pEntity )
{
	mapdef_EntityList::iterator it = s_defPEEntityList.find( pEntity );
	if ( it != s_defPEEntityList.end() )
	{
		delete pEntity;
		s_defPEEntityList.erase( it );
	}
}

void CNtlNaviEntityFactory::DeleteAllEntity_PE( void )
{
	mapdef_EntityList::iterator it = s_defPEEntityList.begin();
	for ( ; it != s_defPEEntityList.end(); ++it )
	{
		delete it->second;
	}
	s_defPEEntityList.clear();
}

void CNtlNaviEntityFactory::DeleteAllEntity( void )
{
	DeleteAllEntity_WE();
	DeleteAllEntity_PE();
}
