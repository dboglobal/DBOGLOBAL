#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlTokenizer.h"
#include "NtlPLCharacterParser.h"
#include "NtlTypeAnimData.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlCharacterXMLScript.h"



void CNtlMatchingTable::Add(const std::string& strString, unsigned int uiKey)
{
	m_mapStringToKey[strString] = uiKey;
	m_mapKeyToString[uiKey]     = strString;
}

unsigned int CNtlMatchingTable::GetKey(const std::string& strString)
{
	if(m_mapStringToKey.find( strString ) == m_mapStringToKey.end())
		return INVALID_GRAPHIC_ANIMATION_ID;
	
	return m_mapStringToKey[strString];
}

std::string* CNtlMatchingTable::GetString(unsigned int uiKey)
{
	if(m_mapKeyToString.find( uiKey ) == m_mapKeyToString.end())
		return NULL;
	
	return &m_mapKeyToString[uiKey];
}

void CNtlMatchingTable::Destroy()
{
	m_mapStringToKey.clear();
	m_mapKeyToString.clear();
}


CNtlPLCharacterParser::CNtlPLCharacterParser()
{
	CreateKeyword();
}

CNtlPLCharacterParser &CNtlPLCharacterParser::GetInstance()
{
	static CNtlPLCharacterParser	s_PLCharacterFileData;
	return s_PLCharacterFileData;
}

RwBool CNtlPLCharacterParser::HandleError(const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);
	int nBuf;
	char szBuffer[512];
	if( pszFormat )
	{
		nBuf = _vsntprintf_s(szBuffer, 512, pszFormat, args);
		NTL_ASSERTE(nBuf >= 0);
	}
	else
		szBuffer[0] = NULL;
	OutputDebugString(szBuffer);

	m_strError = szBuffer;
	va_end(args);

	return FALSE;
}

void CNtlPLCharacterParser::HandleSucess(const char* pszFormat, ...)
{
	va_list args;
	va_start(args, pszFormat);
	int nBuf;
	char szBuffer[512];
	if( pszFormat )
	{
		nBuf = _vsntprintf_s(szBuffer, 512, pszFormat, args);
		NTL_ASSERTE(nBuf >= 0);
	}
	else
		szBuffer[0] = NULL;
	OutputDebugString(szBuffer);
	va_end(args);
}

void CNtlPLCharacterParser::CreateKeyword()
{
#define PC_TABLE_ADD(id) m_PCMatchTable.Add(#id, id)

        PC_TABLE_ADD(BTL_DEF_FP_LOOP);
        PC_TABLE_ADD(BTL_DEF_FP_HURT);
        PC_TABLE_ADD(BTL_DEF_FP_DOT_HURT);
        PC_TABLE_ADD(BTL_DEF_FP_IDLE);
        PC_TABLE_ADD(BTL_DEF_BLOCK_DEF);
        PC_TABLE_ADD(BTL_DEF_BLOCK_START);
        PC_TABLE_ADD(BTL_DEF_BLOCK_END);
        PC_TABLE_ADD(BTL_DEF_DODGE);
        PC_TABLE_ADD(BTL_DEF_KD_FLYING);
        PC_TABLE_ADD(BTL_DEF_KD_LANDING);
        PC_TABLE_ADD(BTL_DEF_KD_HURT);
        PC_TABLE_ADD(BTL_DEF_KD_STAND_UP);
        PC_TABLE_ADD(BTL_DEF_STUN);
        PC_TABLE_ADD(BTL_DEF_COUNTER);
        PC_TABLE_ADD(BTL_DEF_GUARD_CRUSH);

		PC_TABLE_ADD(BTL_DEF_HTB_HOMING_UP);
		PC_TABLE_ADD(BTL_DEF_HTB_HOMING_IDLE);
		PC_TABLE_ADD(BTL_DEF_HTB_HOMING_DOWN);
		PC_TABLE_ADD(BTL_DEF_HTB_HOMING_LANDING);
		PC_TABLE_ADD(BTL_DEF_HTB_TOSS_UP);
		PC_TABLE_ADD(BTL_DEF_HTB_TOSS_IDLE);
		PC_TABLE_ADD(BTL_DEF_HTB_TOSS_HURT);
		PC_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN);
		PC_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN_LANDING);
		PC_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN_HURT);

        PC_TABLE_ADD(BTL_GUN_FP_LOOP);
        PC_TABLE_ADD(BTL_GUN_FP_HURT);
        PC_TABLE_ADD(BTL_GUN_FP_DOT_HURT);
        PC_TABLE_ADD(BTL_GUN_FP_IDLE);
        PC_TABLE_ADD(BTL_GUN_BLOCK_DEF);
        PC_TABLE_ADD(BTL_GUN_BLOCK_START);
        PC_TABLE_ADD(BTL_GUN_BLOCK_END);
        PC_TABLE_ADD(BTL_GUN_DODGE);
        PC_TABLE_ADD(BTL_GUN_KD_FLYING);
        PC_TABLE_ADD(BTL_GUN_KD_LANDING);
        PC_TABLE_ADD(BTL_GUN_KD_HURT);
        PC_TABLE_ADD(BTL_GUN_KD_STAND_UP);
        PC_TABLE_ADD(BTL_GUN_COUNTER);
        PC_TABLE_ADD(BTL_GUN_GUARD_CRUSH);

        PC_TABLE_ADD(BTL_STAFF_FP_LOOP);
        PC_TABLE_ADD(BTL_STAFF_FP_HURT);
        PC_TABLE_ADD(BTL_STAFF_FP_DOT_HURT);
        PC_TABLE_ADD(BTL_STAFF_FP_IDLE);
        PC_TABLE_ADD(BTL_STAFF_BLOCK_DEF);
        PC_TABLE_ADD(BTL_STAFF_BLOCK_START);
        PC_TABLE_ADD(BTL_STAFF_BLOCK_END);
        PC_TABLE_ADD(BTL_STAFF_DODGE);
        PC_TABLE_ADD(BTL_STAFF_KD_FLYING);
        PC_TABLE_ADD(BTL_STAFF_KD_LANDING);
        PC_TABLE_ADD(BTL_STAFF_KD_HURT);
        PC_TABLE_ADD(BTL_STAFF_KD_STAND_UP);
        PC_TABLE_ADD(BTL_STAFF_COUNTER);
        PC_TABLE_ADD(BTL_STAFF_GUARD_CRUSH);

        PC_TABLE_ADD(BTL_DGUN_FP_LOOP);
        PC_TABLE_ADD(BTL_DGUN_FP_HURT);
        PC_TABLE_ADD(BTL_DGUN_FP_DOT_HURT);
        PC_TABLE_ADD(BTL_DGUN_FP_IDLE);
        PC_TABLE_ADD(BTL_DGUN_BLOCK_DEF);
        PC_TABLE_ADD(BTL_DGUN_BLOCK_START);
        PC_TABLE_ADD(BTL_DGUN_BLOCK_END);
        PC_TABLE_ADD(BTL_DGUN_DODGE);
        PC_TABLE_ADD(BTL_DGUN_KD_FLYING);
        PC_TABLE_ADD(BTL_DGUN_KD_LANDING);
        PC_TABLE_ADD(BTL_DGUN_KD_HURT);
        PC_TABLE_ADD(BTL_DGUN_KD_STAND_UP);
        PC_TABLE_ADD(BTL_DGUN_COUNTER);
        PC_TABLE_ADD(BTL_DGUN_GUARD_CRUSH);

        PC_TABLE_ADD(ATK_DEF_1);
        PC_TABLE_ADD(ATK_DEF_2);
        PC_TABLE_ADD(ATK_DEF_3);
        PC_TABLE_ADD(ATK_DEF_4);
        PC_TABLE_ADD(ATK_DEF_5);
        PC_TABLE_ADD(ATK_DEF_6);
        PC_TABLE_ADD(ATK_GLOVE_1);
        PC_TABLE_ADD(ATK_GLOVE_2);
        PC_TABLE_ADD(ATK_GLOVE_3);
        PC_TABLE_ADD(ATK_GLOVE_4);
        PC_TABLE_ADD(ATK_GLOVE_5);
        PC_TABLE_ADD(ATK_GLOVE_6);
        PC_TABLE_ADD(ATK_GUN_1);
        PC_TABLE_ADD(ATK_GUN_2);
        PC_TABLE_ADD(ATK_GUN_3);
        PC_TABLE_ADD(ATK_GUN_4);
        PC_TABLE_ADD(ATK_GUN_5);
        PC_TABLE_ADD(ATK_GUN_6);
        PC_TABLE_ADD(ATK_STAFF_1);
        PC_TABLE_ADD(ATK_STAFF_2);
        PC_TABLE_ADD(ATK_STAFF_3);
        PC_TABLE_ADD(ATK_STAFF_4);
        PC_TABLE_ADD(ATK_STAFF_5);
        PC_TABLE_ADD(ATK_STAFF_6);
        PC_TABLE_ADD(ATK_DGUN_1);
        PC_TABLE_ADD(ATK_DGUN_2);
        PC_TABLE_ADD(ATK_DGUN_3);
        PC_TABLE_ADD(ATK_DGUN_4);
        PC_TABLE_ADD(ATK_DGUN_5);
        PC_TABLE_ADD(ATK_DGUN_6);

        PC_TABLE_ADD(NML_IDLE_LOOP);
        PC_TABLE_ADD(NML_IDLE_01);
        PC_TABLE_ADD(NML_IDLE_02);
        PC_TABLE_ADD(NML_IDLE_HURT);
        PC_TABLE_ADD(NML_IDLE_DOT_HURT);
        PC_TABLE_ADD(SIT_DOWN);
        PC_TABLE_ADD(SIT_UP);
        PC_TABLE_ADD(SIT_IDLE);
        PC_TABLE_ADD(SIT_HURT);
        PC_TABLE_ADD(JUMP_START);
        PC_TABLE_ADD(JUMP_HOVER);
        PC_TABLE_ADD(JUMP_LANDING_SHORT);
        PC_TABLE_ADD(JUMP_LANDING_LONG);
        PC_TABLE_ADD(RUN_FRONT);
		PC_TABLE_ADD(WALK_FRONT); //by HongHoDong(2006. 5. 19) 기획서상에는 없지만 Mob과의 통일성을 위해서 넣어줘야 한다.
        PC_TABLE_ADD(IDLE_RUN_FRONT);
        PC_TABLE_ADD(RUN_BACK);
        PC_TABLE_ADD(IDLE_RUN_BACK);
        PC_TABLE_ADD(TURN_LEFT);
        PC_TABLE_ADD(TURN_RIGHT);
        PC_TABLE_ADD(SWIM_IDLE);
        PC_TABLE_ADD(SWIM_FRONT);
		PC_TABLE_ADD(SWIM_BACK);
        PC_TABLE_ADD(SWIM_IDLE_HURT);
        PC_TABLE_ADD(SWIM_FAINTING);
        PC_TABLE_ADD(HOVER_IDLE);
        PC_TABLE_ADD(HOVER_FRONT);
        PC_TABLE_ADD(HOVER_IDLE_HURT);
        PC_TABLE_ADD(HOVER_UP);
        PC_TABLE_ADD(HOVER_DOWN);
        PC_TABLE_ADD(HOVER_LANDING);
        PC_TABLE_ADD(FAINTING);
        PC_TABLE_ADD(FAINTING_STAND_UP);        
		PC_TABLE_ADD(NML_DASH_FRONT);
		PC_TABLE_ADD(NML_DASH_FRONT_LANDING);
		PC_TABLE_ADD(NML_DASH_BACK);
		PC_TABLE_ADD(NML_DASH_BACK_LANDING);
		PC_TABLE_ADD(NML_DASH_LEFT);
		PC_TABLE_ADD(NML_DASH_LEFT_LANDING);
		PC_TABLE_ADD(NML_DASH_RIGHT);
		PC_TABLE_ADD(NML_DASH_RIGHT_LANDING);
        PC_TABLE_ADD(NML_CHARGE_START);
        PC_TABLE_ADD(NML_CHARGE_LOOP);
        PC_TABLE_ADD(NML_CHARGE_END);

        PC_TABLE_ADD(NML_RIDE);
        PC_TABLE_ADD(NML_SCOUTER_ON_OFF);
        PC_TABLE_ADD(NML_SCOUTER_BURST);
        PC_TABLE_ADD(NML_SKILL_ABILITY);
        PC_TABLE_ADD(NML_TRANSFORM_SEQUELA);
        PC_TABLE_ADD(NML_USE_HOIPOI_CAPSULE);
        PC_TABLE_ADD(NML_STAFF_IDLE_LOOP);
        PC_TABLE_ADD(NML_STAFF_IDLE_01);
        PC_TABLE_ADD(NML_STAFF_IDLE_02);
        PC_TABLE_ADD(NML_STAFF_FRONT_RUN);
        PC_TABLE_ADD(NML_STAFF_IDLE_RUN_FRONT);
		PC_TABLE_ADD(NML_STAFF_BACK_RUN);
		PC_TABLE_ADD(NML_STAFF_LEFT_TURN);
		PC_TABLE_ADD(NML_STAFF_RIGHT_TURN);
		PC_TABLE_ADD(NML_STAFF_SIT_DOWN);
        PC_TABLE_ADD(NML_STAFF_SIT_IDLE);
        PC_TABLE_ADD(NML_STAFF_SIT_UP);
        
		PC_TABLE_ADD(NML_DGUN_IDLE_LOOP);
        PC_TABLE_ADD(NML_DGUN_IDLE_01);
        PC_TABLE_ADD(NML_DGUN_IDLE_02);
		PC_TABLE_ADD(NML_DGUN_FRONT_RUN);
        PC_TABLE_ADD(NML_DGUN_IDLE_RUN_FRONT);
		PC_TABLE_ADD(NML_DGUN_SIT_DOWN);
		PC_TABLE_ADD(NML_DGUN_SIT_IDLE);
		PC_TABLE_ADD(NML_DGUN_SIT_UP);

        PC_TABLE_ADD(SOC_AGREE);
        PC_TABLE_ADD(SOC_NO);
        PC_TABLE_ADD(SOC_BYE);
        PC_TABLE_ADD(SOC_HAPPY);
        PC_TABLE_ADD(SOC_LAUGH);
        PC_TABLE_ADD(SOC_SAD);
        PC_TABLE_ADD(SOC_CLAP_HANDS);
        PC_TABLE_ADD(SOC_POINT);
        PC_TABLE_ADD(SOC_RUSH);
        PC_TABLE_ADD(SOC_YEAH);
        PC_TABLE_ADD(SOC_AMUSE);
        PC_TABLE_ADD(SOC_HI);
        PC_TABLE_ADD(SOC_PAFUPAFU);
        PC_TABLE_ADD(SOC_COURTESY);
        PC_TABLE_ADD(SOC_POKE);

        // Transform
        PC_TABLE_ADD(TRANS_IDLE);
        PC_TABLE_ADD(TRANS_RUN_FRONT);
        PC_TABLE_ADD(TRANS_RUN_BACK);
        PC_TABLE_ADD(TRANS_ATK_DEF_1);
        PC_TABLE_ADD(TRANS_ATK_DEF_2);
        PC_TABLE_ADD(TRANS_SPAWN);

        // Vehicle
        PC_TABLE_ADD(VEHICLE_SRP1_IDLE);
        PC_TABLE_ADD(VEHICLE_SRP1_RUN);
        PC_TABLE_ADD(VEHICLE_SRP1_START);
        PC_TABLE_ADD(VEHICLE_SRP1_STOP);
        PC_TABLE_ADD(VEHICLE_SRP1_JUMP);
        PC_TABLE_ADD(VEHICLE_SRP1_TURN_LEFT);        
        PC_TABLE_ADD(VEHICLE_SRP1_TURN_RIGHT);        
        PC_TABLE_ADD(VEHICLE_SRP1_LANDING);

        PC_TABLE_ADD(VEHICLE_SRP2_IDLE);
        PC_TABLE_ADD(VEHICLE_SRP2_RUN);
        PC_TABLE_ADD(VEHICLE_SRP2_START);
        PC_TABLE_ADD(VEHICLE_SRP2_STOP);
        PC_TABLE_ADD(VEHICLE_SRP2_JUMP);
        PC_TABLE_ADD(VEHICLE_SRP2_TURN_LEFT);
        PC_TABLE_ADD(VEHICLE_SRP2_TURN_RIGHT);        
        PC_TABLE_ADD(VEHICLE_SRP2_LANDING);


//by HongHoDong 2006. 5. 19일 MobMatching Table 수정 그래픽팀과 같은 이름으로 하기로 결정
//(아직 Aliena Brain의 문서와 다름)
#define MOB_TABLE_ADD(id) m_MobMatchTable.Add(#id, id)
	MOB_TABLE_ADD(NML_SPAWN);
	MOB_TABLE_ADD(NML_IDLE_LOOP);
	MOB_TABLE_ADD(NML_IDLE_01);
	MOB_TABLE_ADD(NML_IDLE_02);
	MOB_TABLE_ADD(NML_IDLE_HURT);
    MOB_TABLE_ADD(SIT_DOWN);
    MOB_TABLE_ADD(SIT_UP);
    MOB_TABLE_ADD(SIT_IDLE);
    MOB_TABLE_ADD(SIT_HURT);


	MOB_TABLE_ADD(BTL_DEF_FP_LOOP);
	MOB_TABLE_ADD(BTL_DEF_FP_HURT);
	MOB_TABLE_ADD(BTL_DEF_BLOCK_DEF);
	MOB_TABLE_ADD(BTL_DEF_BLOCK_START);
	MOB_TABLE_ADD(BTL_DEF_BLOCK_END);
	MOB_TABLE_ADD(BTL_DEF_DODGE);
	
	MOB_TABLE_ADD(BTL_DEF_KD_FLYING);
	MOB_TABLE_ADD(BTL_DEF_KD_LANDING);
	MOB_TABLE_ADD(BTL_DEF_KD_HURT);
	MOB_TABLE_ADD(BTL_DEF_KD_STAND_UP);
    MOB_TABLE_ADD(BTL_DEF_STUN);
	
	MOB_TABLE_ADD(BTL_DEF_HTB_TOSS_UP);
	MOB_TABLE_ADD(BTL_DEF_HTB_TOSS_IDLE);
	MOB_TABLE_ADD(BTL_DEF_HTB_TOSS_HURT);
	MOB_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN);
	MOB_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN_LANDING);
	MOB_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN_HURT);

	MOB_TABLE_ADD(WALK_FRONT);
	MOB_TABLE_ADD(RUN_FRONT);
	MOB_TABLE_ADD(FAINTING);

	MOB_TABLE_ADD(ATK_DEF_1);
	MOB_TABLE_ADD(ATK_DEF_2);
	MOB_TABLE_ADD(ATK_DEF_3);
	MOB_TABLE_ADD(ATK_DEF_4);
	MOB_TABLE_ADD(ATK_DEF_5);
	MOB_TABLE_ADD(ATK_DEF_6);

    MOB_TABLE_ADD(SOC_AGREE);
	MOB_TABLE_ADD(SOC_NO);
	MOB_TABLE_ADD(SOC_BYE);
	MOB_TABLE_ADD(SOC_HAPPY);
	MOB_TABLE_ADD(SOC_LAUGH);
	MOB_TABLE_ADD(SOC_SAD);
	MOB_TABLE_ADD(SOC_CLAP_HANDS);
	MOB_TABLE_ADD(SOC_POINT);
	MOB_TABLE_ADD(SOC_RUSH);
	MOB_TABLE_ADD(SOC_YEAH);
	MOB_TABLE_ADD(SOC_AMUSE);
	MOB_TABLE_ADD(SOC_HI);
	MOB_TABLE_ADD(SOC_PAFUPAFU);
	MOB_TABLE_ADD(SOC_COURTESY);
	MOB_TABLE_ADD(SOC_POKE);

    MOB_TABLE_ADD(TRANS_SPAWN);

    #define NPC_TABLE_ADD(id) m_NpcMatchTable.Add(#id, id)	
    NPC_TABLE_ADD(BTL_DEF_FP_LOOP);
    NPC_TABLE_ADD(BTL_DEF_FP_HURT);
    NPC_TABLE_ADD(BTL_DEF_BLOCK_DEF);
    NPC_TABLE_ADD(BTL_DEF_BLOCK_DEF);
    NPC_TABLE_ADD(BTL_DEF_BLOCK_START);
    NPC_TABLE_ADD(BTL_DEF_BLOCK_END);
    NPC_TABLE_ADD(BTL_DEF_DODGE);
    NPC_TABLE_ADD(BTL_DEF_KD_FLYING);
    NPC_TABLE_ADD(BTL_DEF_KD_LANDING);
    NPC_TABLE_ADD(BTL_DEF_KD_HURT);
    NPC_TABLE_ADD(BTL_DEF_KD_STAND_UP);
    NPC_TABLE_ADD(BTL_DEF_STUN);

    NPC_TABLE_ADD(BTL_DEF_HTB_TOSS_UP);
    NPC_TABLE_ADD(BTL_DEF_HTB_TOSS_IDLE);
    NPC_TABLE_ADD(BTL_DEF_HTB_TOSS_HURT);
    NPC_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN);
    NPC_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN_LANDING);
    NPC_TABLE_ADD(BTL_DEF_HTB_FALL_DOWN_HURT);

    NPC_TABLE_ADD(ATK_DEF_1);
    NPC_TABLE_ADD(ATK_DEF_2);
    NPC_TABLE_ADD(ATK_DEF_3);
    NPC_TABLE_ADD(ATK_DEF_4);
    NPC_TABLE_ADD(ATK_DEF_5);
    NPC_TABLE_ADD(ATK_DEF_6);

	NPC_TABLE_ADD(NML_SPAWN);
    NPC_TABLE_ADD(NML_IDLE_LOOP);
    NPC_TABLE_ADD(NML_IDLE_01);
    NPC_TABLE_ADD(NML_IDLE_02);
    NPC_TABLE_ADD(NML_IDLE_HURT);
    NPC_TABLE_ADD(WALK_FRONT);
    NPC_TABLE_ADD(RUN_FRONT);
    NPC_TABLE_ADD(FAINTING);
    NPC_TABLE_ADD(SIT_DOWN);
    NPC_TABLE_ADD(SIT_IDLE);
    NPC_TABLE_ADD(SIT_UP);
    NPC_TABLE_ADD(TURN_LEFT);

	NPC_TABLE_ADD(SOC_AGREE);
	NPC_TABLE_ADD(SOC_NO);
	NPC_TABLE_ADD(SOC_BYE);
	NPC_TABLE_ADD(SOC_HAPPY);
	NPC_TABLE_ADD(SOC_LAUGH);
	NPC_TABLE_ADD(SOC_SAD);
	NPC_TABLE_ADD(SOC_CLAP_HANDS);
	NPC_TABLE_ADD(SOC_POINT);
	NPC_TABLE_ADD(SOC_RUSH);
	NPC_TABLE_ADD(SOC_YEAH);
	NPC_TABLE_ADD(SOC_AMUSE);
	NPC_TABLE_ADD(SOC_HI);
	NPC_TABLE_ADD(SOC_PAFUPAFU);
	NPC_TABLE_ADD(SOC_COURTESY);
	NPC_TABLE_ADD(SOC_POKE);
	
    // Skill
    char chBuf[64] = {0,};
    for(int i = 0; i < SKILL_ANIMATION_END-SKILL_ANIMATION_START; i++)
    {
        sprintf_s(chBuf, "SKILL_%d", i);

        m_PCMatchTable.Add(chBuf, SKILL_ANIMATION_START + i);
        m_NpcMatchTable.Add(chBuf, SKILL_ANIMATION_START + i);
        m_MobMatchTable.Add(chBuf, SKILL_ANIMATION_START + i);
    }

    // HTB
    for(int i = 0; i  < HTB_ANIMATION_END - HTB_ANIMATION_START; ++i)
    {
        sprintf_s(chBuf, "HTB_%d", i);

        m_PCMatchTable.Add(chBuf, HTB_ANIMATION_START + i);
    }

    // Trigger Animation
    for(int i = 0; i < PC_TRIGGER_ANIMATION_END - PC_TRIGGER_ANIMATION_START; ++i)
    {
        sprintf_s(chBuf, "TRIGGER_%d", i);

        m_PCMatchTable.Add(chBuf, PC_TRIGGER_ANIMATION_START + i);
        m_NpcMatchTable.Add(chBuf, PC_TRIGGER_ANIMATION_START + i);
        m_MobMatchTable.Add(chBuf, PC_TRIGGER_ANIMATION_START + i);        
    }

    // Item Use Animation
    for(int i = 0; i < ITEM_USE_ANIMATION_END - ITEM_USE_ANIMATION_START; ++i)
    {
        sprintf_s(chBuf, "ITEM_USE_%d", i);
        m_PCMatchTable.Add(chBuf, ITEM_USE_ANIMATION_START + i);
    }

    #define VEHICLE_TABLE_ADD(id) m_VehicleTable.Add(#id, id)	
    VEHICLE_TABLE_ADD(VEHICLE_IDLE);
    VEHICLE_TABLE_ADD(VEHICLE_RUN);
    VEHICLE_TABLE_ADD(VEHICLE_START);
    VEHICLE_TABLE_ADD(VEHICLE_STOP);
    VEHICLE_TABLE_ADD(VEHICLE_JUMP);    
    VEHICLE_TABLE_ADD(VEHICLE_TURN_LEFT);
    VEHICLE_TABLE_ADD(VEHICLE_TURN_RIGHT);
    VEHICLE_TABLE_ADD(VEHICLE_LANDING);

    LoadLuaProjectileEffectType("script\\sldef.lua");    
}

RwBool CNtlPLCharacterParser::LoadLuaProjectileEffectType(const std::string &strFileName)
{
	NTL_FUNCTION("CNtlPLCharacterParser::LoadLuaProjectileEffectType()");
	
	CNtlTokenizer lexer(strFileName);
	if(!lexer.IsSuccess())
	{
		NTL_RETURN(FALSE);
	}
	
	enum EDataParseState
	{
		PARSE_DATA_GLOBAL,
		PARSE_DATA_LUA,
	};
	
	EDataParseState eState = PARSE_DATA_GLOBAL;

	RwInt32 iOldLine = 0;
	RwInt32 iLine;
	
	std::string strEffectType;
	std::string strEffectID;

	while(1)
	{
		std::string strToken = lexer.PeekNextToken(NULL, &iLine);
		if(strToken == "") 
		{
			break;
		}
		switch (eState)
		{
			case PARSE_DATA_GLOBAL:
			{
				if(strToken == "BEGIN_ENERGY_TYPE")
				{
					strToken = lexer.PeekNextToken(NULL, &iLine);
					if(strToken == "=")
					{
						strToken = lexer.PeekNextToken(NULL, &iLine);
						
						eState = PARSE_DATA_LUA;
					}
					else
					{
						NTL_RETURN(HandleError("error syntax Line:%d \n", iLine));
					}
				}
				break;
			}

			case PARSE_DATA_LUA:
			{
				if(strToken == "END_ENERGY_TYPE")
				{
					eState = PARSE_DATA_GLOBAL;
					break;
				}

				strEffectType = strToken;
				strToken = lexer.PeekNextToken(NULL, &iLine);

				if(strToken == "=")
				{
					strEffectID = lexer.PeekNextToken(NULL, &iLine);
					m_LuaProjEffectTypeTable.Add(strEffectType.c_str(), atoi(strEffectID.c_str()));
				}
				else
				{
					NTL_RETURN(HandleError("error syntax Line:%d \n", iLine));
				}
				break;
			}
		}
		
		lexer.PopToPeek();
	}

	NTL_RETURN(TRUE);
}
