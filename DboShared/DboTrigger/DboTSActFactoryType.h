#ifndef _DBO_TSACTFACTORYTYPE_H_
#define _DBO_TSACTFACTORYTYPE_H_


#include "DboTSActQItem.h"
#include "DboTSActItem.h"
#include "DboTSActETimerS.h"
#include "DboTSActETimerE.h"
#include "DboTSActSToCEvt.h"
#include "DboTSActNPCConv.h"
#include "DboTSActRegQInfo.h"
#include "DboTSActDir.h"
#include "DboTSActOPCam.h"
#include "DboTSActTWaitTS.h"
#include "DboTSActInSSM.h"
#include "DboTSActTSState.h"
#include "DboTSActPortal.h"
#include "DboTSActObjState.h"
#include "DboTSActConcCheck.h"
#include "DboTSActObjConv.h"
#include "DboTSActHint.h"
#include "DboTSActSendSvrEvt.h"
#include "DboTSActTMQStage.h"
#include "DboTSActOpenWindow.h"
#include "DboTSActTMQTimeBonus.h"
#include "DboTSActTelecast.h"
#include "DboTSActDirIndicator.h"
#include "DboTSActOPObject.h"
#include "DboTSActDrop.h"
#include "DboTSActRmvCoupon.h"
#include "DboTSActEscort.h"
#include "DboTSActTMQInfoShow.h"
#include "DboTSActWorldPlayScript.h"
#include "DboTSActSWProbSF.h"
#include "DboTSActPCConv.h"
#include "DboTSActCustomEvt.h"
#include "DboTSActTelMudosa.h"
#include "DboTSActTelMinorMatch.h"
#include "DboTSActPIDgn.h"
#include "DboTSActTLQ.h"
#include "DboTSActDoSkill.h"
#include "DboTSActTObjFriendly.h"
#include "DboTSActBroadMsg.h"
#include "DboTSActOutMsg.h"
#include "DboTSActWPSFD.h"
#include "DboTSActMail.h"
#include "DboTSActAvatarDead.h"
#include "DboTSActMiniNarration.h"
#include "DboTSActRegCTiming.h"
#include "DboTSActExcCGroup.h"
#include "DboTSActSkipCont.h"
#include "DboTSActObjWPS.h"
#include "DboTSActDojo.h"
#include "DboTSActSkyDgn.h"


/** 
	Action 관련 객체를 Runtime 시에 생성하기 위한 action entity type class 들
*/


class CDboTSEntityFactoryType_CDboTSActQItem : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActQItem )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActQItem( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActItem : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActItem )

		// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActItem( void );

	// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActETimerS : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActETimerS )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActETimerS( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActETimerE : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActETimerE )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActETimerE( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActSToCEvt : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActSToCEvt )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActSToCEvt( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActNPCConv : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActNPCConv )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActNPCConv( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActRegQInfo : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActRegQInfo )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActRegQInfo( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActDir : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActDir )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActDir( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActOPCam : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActOPCam )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActOPCam( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActTWaitTS : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTWaitTS )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTWaitTS( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActInSSM : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActInSSM )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActInSSM( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActTSState : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTSState )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTSState( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActPortal : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActPortal )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActPortal( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActObjState : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActObjState )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActObjState( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActConcCheck : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActConcCheck )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActConcCheck( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActObjConv : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActObjConv )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActObjConv( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActHint : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActHint )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActHint( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActSendSvrEvt : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActSendSvrEvt )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActSendSvrEvt( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTMQStage : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTMQStage )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTMQStage( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActOpenWindow : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActOpenWindow )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActOpenWindow( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTMQTimeBonus : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTMQTimeBonus )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTMQTimeBonus( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTelecast : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTelecast )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTelecast( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActDirIndicator : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActDirIndicator )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActDirIndicator( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActOPObject : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActOPObject )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActOPObject( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActDrop : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActDrop )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActDrop( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActRmvCoupon : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActRmvCoupon )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActRmvCoupon( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActEscort : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActEscort )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActEscort( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTMQInfoShow : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTMQInfoShow )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTMQInfoShow( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActWorldPlayScript : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActWorldPlayScript )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActWorldPlayScript( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActSWProbSF : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActSWProbSF )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActSWProbSF( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActPCConv : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActPCConv )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActPCConv( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActCustomEvt : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActCustomEvt )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActCustomEvt( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTelMudosa : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTelMudosa )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTelMudosa( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTelMinorMatch : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTelMinorMatch )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTelMinorMatch( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActPIDgn : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActPIDgn )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActPIDgn( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTLQ : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTLQ )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTLQ( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActDoSkill : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActDoSkill )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActDoSkill( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActTObjFriendly : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActTObjFriendly )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActTObjFriendly( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActBroadMsg : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActBroadMsg )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActBroadMsg( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActMiniNarration : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActMiniNarration )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActMiniNarration( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActRegCTiming : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActRegCTiming )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActRegCTiming( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActExcCGroup : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActExcCGroup )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActExcCGroup( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActSkipCont : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActSkipCont )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActSkipCont( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActObjWPS : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActObjWPS )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActObjWPS( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActDojo : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActDojo )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActDojo( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};


class CDboTSEntityFactoryType_CDboTSActOutMsg : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActOutMsg )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActOutMsg( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActWPSFD : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActWPSFD )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActWPSFD( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActMail : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActMail )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActMail( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActAvatarDead : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActAvatarDead )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActAvatarDead( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};

class CDboTSEntityFactoryType_CDboTSActSkyDgn : public CNtlTSEntityFactoryType_CNtlTSAction
{
	NTL_TS_DECLARE_REGIST_SCRIPT_RUNTIME_CREATE( CDboTSActSkyDgn )

// Constructions and Destructions
public:
	CDboTSEntityFactoryType_CDboTSActSkyDgn( void );

// Methods
public:
	virtual	CNtlTSScrObject*			CreateObj( void );
	virtual	void						DeleteObj( CNtlTSScrObject*& pObj );
};






#endif