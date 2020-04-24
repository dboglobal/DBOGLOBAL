#ifndef _NTL_TSSCROBJECT_H_
#define _NTL_TSSCROBJECT_H_


#include "NtlTSRTTI.h"
#include "NtlTSScrProperty.h"


/** 
	Script object
	Class to be created, in order to automatically generate physical objects through the Script below the class 
	Inheritance received should be implemented

	(Note) 
	Use a class that inherits TSRTTI in order to support the generation of runtime objects in a script 
	Can be dynamically generated at runtime object
*/


class CNtlTSScrObject
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	virtual ~CNtlTSScrObject( void ) { return; }

// Implementations
protected:
	virtual	CNtlTSScrObject*			GetParentForScript( void ) = 0;
	virtual	void						SetParentForScript( CNtlTSScrObject* pParent ) = 0;

	virtual	CNtlTSScrObject*			BeginChildForScript( void ) = 0;
	virtual	CNtlTSScrObject*			NextChildForScript( void ) = 0;

	virtual	void						AttachChildForScript( CNtlTSScrObject* pChild ) = 0;

public:
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty ) = 0;
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty ) = 0;

public:
	virtual void						MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList ) = 0;

	friend class CNtlTSScrParser;
	friend class CNtlTSScrSaver;
};


#endif