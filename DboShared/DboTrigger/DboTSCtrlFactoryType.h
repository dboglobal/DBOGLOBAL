#ifndef _DBO_TSCTRLFACTORYTYPE_H_
#define _DBO_TSCTRLFACTORYTYPE_H_


/** 
	Control 객체를 Runtime 시에 생성하기 위한 control type class 들
*/


class CDboTSControlFactoryType_CDboTSEMObject : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMObject( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMNPC : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMNPC( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMItem : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMItem( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMSvrEvt : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMSvrEvt( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMSkill : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMSkill( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMColRgn : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMColRgn( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMRB : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMRB( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMMob : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMMob( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMBindStone : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMBindStone( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMSearchQuest : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMSearchQuest( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMItemUpgrade : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMItemUpgrade( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMTeleport : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMTeleport( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMBudokai : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMBudokai( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMSlotMachine : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMSlotMachine( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMHoipoiMix : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMHoipoiMix( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMPrivateShop : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMPrivateShop( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMFreeBattle : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMFreeBattle( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMItemIdentity : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMItemIdentity( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMUseMail : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMUseMail( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};

class CDboTSControlFactoryType_CDboTSEMParty : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMParty( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSEMFLink : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
	// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMFLink(void);

	// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj(void);
	virtual void						DeleteObj(CNtlTSControlObject*& pObj);
	virtual const char*					GetKeyword(void) const;
};

class CDboTSControlFactoryType_CDboTSEMSideIcon : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
	// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMSideIcon(void);

	// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj(void);
	virtual void						DeleteObj(CNtlTSControlObject*& pObj);
	virtual const char*					GetKeyword(void) const;
};

class CDboTSControlFactoryType_CDboTSEMLevelCheck : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
	// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMLevelCheck(void);

	// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj(void);
	virtual void						DeleteObj(CNtlTSControlObject*& pObj);
	virtual const char*					GetKeyword(void) const;
};

class CDboTSControlFactoryType_CDboTSEMQuest : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
	// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMQuest(void);

	// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj(void);
	virtual void						DeleteObj(CNtlTSControlObject*& pObj);
	virtual const char*					GetKeyword(void) const;
};

class CDboTSControlFactoryType_CDboTSEMDialogOpen : public CNtlTSControlFactoryType_CNtlTSEvtMapper
{
	// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSEMDialogOpen(void);

	// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj(void);
	virtual void						DeleteObj(CNtlTSControlObject*& pObj);
	virtual const char*					GetKeyword(void) const;
};





class CDboTSControlFactoryType_CDboTSTCtrl : public CNtlTSControlFactoryType_CNtlTSController
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSTCtrl( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


class CDboTSControlFactoryType_CDboTSQCtrl : public CDboTSControlFactoryType_CDboTSTCtrl
{
// Constructions and Destructions
public:
	CDboTSControlFactoryType_CDboTSQCtrl( void );

// Methods
public:
	virtual	CNtlTSControlObject*		CreateObj( void );
	virtual void						DeleteObj( CNtlTSControlObject*& pObj );
	virtual const char*					GetKeyword( void ) const;
};


#endif