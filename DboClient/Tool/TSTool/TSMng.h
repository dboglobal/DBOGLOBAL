#pragma once


#include "DboTSCore.h"


class CTSTrigger;


class CTSMng : public CDboTSMain
{
// Constructions and Destructions
public:
	CTSMng( void );
	virtual ~CTSMng( void );

// Methods
public:
	virtual bool						Create( void );
	virtual void						Delete( void );

	CNtlTSTrigger*						CreateTrigger( CTSTrigger* pTSTrigger );
	void								DeleteTrigger( CNtlTSTrigger* pNTLTrigger );

// Implementations
protected:
	virtual bool						LoadLog( void );
	virtual void						UnloadLog( void );

	virtual bool						LoadFactories( void );
	virtual void						UnloadFactories( void );

	virtual bool						LoadScripts( void )			{ return true; }
	virtual void						UnloadAllScripts( void )	{ return; }

	virtual bool						LoadEventMappers( void )	{ return true; }
	virtual void						UnloadEventMappers( void )	{ return; }

	virtual bool						LoadRecv( void )			{ return true; }
	virtual void						UnloadRecv( void )			{ return; }
};
