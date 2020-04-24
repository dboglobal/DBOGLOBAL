#ifndef _DBO_UNIFIED_QUEST_NARRATION_H_
#define _DBO_UNIFIED_QUEST_NARRATION_H_


#include "ceventhandler.h"
#include "QuestNarrationTable.h"
#include "DboTSCoreDefine.h"


class CDboUnifiedQuestNarration
{
// Declarations
public:
	enum eUQN_STATE
	{
		eUQN_STATE_IDLE,

		eUQN_STATE_BEGIN,
		eUQN_STATE_PROGRESS,
		eUQN_STATE_END,

		eUQN_STATE_SINGLE,

		eUQN_STATE_MAX
	};

	struct sNARRATION_ENTITY
	{
		eNARRATION_PROGRESS_STATE			eNarrationProgType;
		sQUEST_NARRATION_DATASET			sData;
	};

	typedef std::vector< sQUEST_NARRATION_DATASET > vecdef_QuestNarrationList;

	typedef std::vector< sNARRATION_ENTITY > vecdef_NarrationEntityList;

	typedef void (CDboUnifiedQuestNarration::* CALL_BACK_FUNC)( void );


// Member variables
protected:
	CALL_BACK_FUNC							m_arStateFunc[eUQN_STATE_MAX][eUQN_STATE_MAX];

	eUQN_STATE								m_eOldState, m_eCurState;

	sTS_KEY									m_sTSKey;
	unsigned int							m_uiTableIdx;
	unsigned int							m_uiRemainTime;
	vecdef_QuestNarrationList				m_defQuestNarrationList;

	vecdef_NarrationEntityList				m_defNarrationEntityQueue;


// Constructions and Destructions
public:
	CDboUnifiedQuestNarration( void );
	~CDboUnifiedQuestNarration( void );


// Operations
public:
	bool									IsUsing( void );

	const sTS_KEY&							GetTSKey( void );

	bool									Build( sTS_KEY& sKey, sQUEST_NARRATION_TBLDAT* pData );

	eUQN_STATE								NextState( sTS_KEY& sKey );

	void									Update( void );


// Implementations
protected:
	void									ChangeState( eUQN_STATE eState );

	void									OnIdle2Single( void );
	void									OnIdle2Begin( void );

	void									OnBegin2Progress( void );
	void									OnBegin2End( void );

	void									OnProgress2Process( void );
	void									OnProgress2End( void );

	void									OnEnd2Idle( void );

	void									OnSingle2Idle( void );
};


inline const sTS_KEY& CDboUnifiedQuestNarration::GetTSKey( void )
{
	return m_sTSKey;
}


#endif