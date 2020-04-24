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


#ifndef __NTL_SLSERVER_DECLEAR_H__
#define __NTL_SLSERVER_DECLEAR_H__

// table
struct sPC_TBLDAT;
struct sNPC_TBLDAT;
struct sMOB_TBLDAT;
struct sSKILL_TBLDAT;
struct sITEM_TBLDAT;
struct sITEM_OPTION_TBLDAT;
struct sQUESTITEM_TBLDAT;
struct sMERCHANT_TBLDAT;
struct sHTB_SET_TBLDAT;
struct sACTION_TBLDAT;
struct sCHARM_TBLDAT;
struct sCHAT_COMMAND_TBLDAT;
struct sQUEST_TEXT_DATA_TBLDAT;
struct sOBJECT_TBLDAT;
struct sTEXT_TBLDAT;
struct sMOVE_PATTERN_TBLDAT;

class CWorldTable;
class CPCTable;
class CMobTable;
class CNPCTable;
class CItemTable;
class CItemOptionTable;
class CQuestItemTable;
class CSkillTable;
class CSystemEffectTable;
class CNewbieTable;
class CMerchantTable;
class CHTBSetTable;
class CUseItemTable;
class CCharmTable;
class CActionTable;
class CChatCommandTable;
class CQuestTextDataTable;
class CObjectTable;
class CTextAllTable;

// character
struct sCHARSTATE;
struct sPC_BRIEF;
struct sMOB_BRIEF;
struct sNPC_BRIEF;
struct sSUMMON_PET_BRIEF;
struct sSUMMON_PET_PROFILE;
struct sCHARSTATE;
struct sSKILL_RESULT;

#include "NtlSharedType.h"

#endif

