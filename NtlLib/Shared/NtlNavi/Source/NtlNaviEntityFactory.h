#ifndef _NTL_NAVI_ENTITY_FACTORY_H_
#define _NTL_NAVI_ENTITY_FACTORY_H_


class CNtlNaviEntity;

class CNtlNaviWorld_WE;
class CNtlNaviWorldProp_WE;
class CNtlNaviFieldProp_WE;
class CNtlNaviFieldGroup_WE;
class CNtlNaviFieldHeight_WE;

class CNtlNaviWorld_PE;
class CNtlNaviWorldProp_PE;
class CNtlNaviWorldGroup_PE;
class CNtlNaviFieldGroup_PE;


class CNtlNaviEntityFactory
{
public:
	typedef std::map< void*, CNtlNaviEntity* > mapdef_EntityList;


protected:
	static mapdef_EntityList		s_defWEEntityList;
	static mapdef_EntityList		s_defPEEntityList;


public:
	static CNtlNaviWorld_WE*		CreateEntity_World_WE( void );
	static CNtlNaviWorldProp_WE*	CreateEntity_WorldProp_WE( void );
	static CNtlNaviFieldProp_WE*	CreateEntity_FieldProp_WE( void );
	static CNtlNaviFieldGroup_WE*	CreateEntity_FieldGroup_WE( void );
	static CNtlNaviFieldHeight_WE*	CreateEntity_FieldHeight_WE( void );
	static void						DeleteEntity_WE( CNtlNaviEntity* pEntity );
	static void						DeleteAllEntity_WE( void );

	static CNtlNaviWorld_PE*		CreateEntity_World_PE( void );
	static CNtlNaviWorldProp_PE*	CreateEntity_WorldProp_PE( void );
	static CNtlNaviWorldGroup_PE*	CreateEntity_WorldGroup_PE( void );
	static CNtlNaviFieldGroup_PE*	CreateEntity_FieldGroup_PE( void );
	static void						DeleteEntity_PE( CNtlNaviEntity* pEntity );
	static void						DeleteAllEntity_PE( void );

	static void						DeleteAllEntity( void );
};


#endif