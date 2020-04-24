#ifndef _DBOTSDEFINE_H_
#define _DBOTSDEFINE_H_


#include "NtlConsole.h"
#include "NtlSLDef.h"
#include "DboTSCore.h"


#ifdef RWDEBUG
//#define __OUT_QUEST_TRACE__
#endif


extern RwBool ConvertSkillUseTargetHandleToInfo( SERIAL_HANDLE hTarget, SSkillUseTargetInfo& sTargetInfo );

extern RwBool ConvertItemUseTargetHandleToInfo( SERIAL_HANDLE hTarget, SItemUseTargetInfo& sTargetInfo );


class CDummyConsole
{
public:
	CNtlConsole m_clConsole;

public:
	CDummyConsole( void )	{ m_clConsole.Create( "Quest" ); m_clConsole.Show(); m_clConsole.Color( FOREGROUND_GREEN|FOREGROUND_BLUE ); }
	~CDummyConsole( void )	{ m_clConsole.Close(); }
};


#ifdef __OUT_QUEST_TRACE__
	extern CDummyConsole g_clConsole;

	#define OUT_QMSG_0(s)					g_clConsole.m_clConsole.Output(s"\n");
	#define OUT_QMSG_1(s,p1)				g_clConsole.m_clConsole.Output(s"\n",p1)
	#define OUT_QMSG_2(s,p1,p2)				g_clConsole.m_clConsole.Output(s"\n",p1,p2)
	#define OUT_QMSG_3(s,p1,p2,p3)			g_clConsole.m_clConsole.Output(s"\n",p1,p2,p3)
	#define OUT_QMSG_4(s,p1,p2,p3,p4)		g_clConsole.m_clConsole.Output(s"\n",p1,p2,p3,p4)
	#define OUT_QMSG_5(s,p1,p2,p3,p4,p5)	g_clConsole.m_clConsole.Output(s"\n",p1,p2,p3,p4,p5)
	#define OUT_QMSG_6(s,p1,p2,p3,p4,p5,p6)	g_clConsole.m_clConsole.Output(s"\n",p1,p2,p3,p4,p5,p6)
	#define OUT_QMSG_9(s,p1,p2,p3,p4,p5,p6,p7,p8,p9)	g_clConsole.m_clConsole.Output(s"\n",p1,p2,p3,p4,p5,p6,p7,p8,p9)
#else
	#define OUT_QMSG_0(s)
	#define OUT_QMSG_1(s,p1)
	#define OUT_QMSG_2(s,p1,p2)
	#define OUT_QMSG_3(s,p1,p2,p3)
	#define OUT_QMSG_4(s,p1,p2,p3,p4)
	#define OUT_QMSG_5(s,p1,p2,p3,p4,p5)
	#define OUT_QMSG_6(s,p1,p2,p3,p4,p5,p6)
	#define OUT_QMSG_9(s,p1,p2,p3,p4,p5,p6,p7,p8,p9)
#endif


#define SYNC_WITH_MIDDLE_BEGIN( tg, tc, ta )						\
	if ( NTL_TS_MAIN_GROUP_ID == tg ) AddContSyncQueue( tc, ta );	\
	else AddUpdateExceptionGroupActRef( tg );


#define SYNC_WITH_MIDDLE_END( tg, tc, ta )							\
	if ( NTL_TS_MAIN_GROUP_ID == tg ) DelContSyncQueue( tc, ta );	\
	else ReleaseUpdateExceptionGroupActRef( tg );
	

#endif