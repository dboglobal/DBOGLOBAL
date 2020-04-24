/// 클라이언트 서포트 툴(Mr.PoPo)와 클라이언트 간에 전송하는 윈도우 메시지

#pragma once

#define MSG_FROM_MRPOPO                 (WM_USER + 2374)                  ///< 서포트 툴로부터의 메시지

enum EMrPoPoMsg
{
    MSG_MRPOPO_CREATE_ITEM = (WM_USER + 101),
    MSG_MRPOPO_UPGRADE_ITEM,
    MSG_MRPOPO_SET_ZENNY,
    MSG_MRPOPO_ADD_EXP,    
    MSG_MRPOPO_SET_LEVEL,
    MSG_MRPOPO_COLLECT_DRAGONBALL,
    MSG_MRPOPO_SETSPEED,
    MSG_MRPOPO_SETOFFENCE,
    MSG_MRPOPO_SETDEFNECE,
    MSG_MRPOPO_TEST_CROWD,                          ///< 관중 연출 테스트
    MSG_MRPOPO_TELEPORT,
    MSG_MRPOPO_TELE_DIRECT_X,
    MSG_MRPOPO_TELE_DIRECT_Z,
    MSG_MRPOPO_OBSERVER_STATIC,
    MSG_MRPOPO_OBSERVER_LUA,
    MSG_MRPOPO_FREECAMERA,
    MSG_MRPOPO_SCOUTER_RENDER,
    MSG_MRPOPO_RENDER_MESHSYSTEM,
    MSG_MRPOPO_RENDER_DECALSYSTEM,
    MSG_MRPOPO_LEARN_ALL_SKILL,
    MSG_MRPOPO_UPGRADE_ALL_SKILL,
    MSG_MRPOPO_CREATE_LEVEL_ITEMS,
    MSG_MRPOPO_SUPER_MODE,    

    // 클라이언트 치트
    MSG_MRPOPO_ELAPSEDTIME_WEIGHT = 1000,                  ///< 시간 조절
    MSG_MRPOPO_DUMP_TO_CONSOLE,                     ///< 콘솔창 덤프
    MSG_MRPOPO_DUMP_TO_GUI,                         ///< GUI 덤프
    MSG_MRPOPO_DUMP_TO_FILE,                        ///< 파일 덤프
    MSG_MRPOPO_DUMP,                                ///< 덤프를 뜬다    
    MSG_MRPOPO_DUMP_REG,                            ///< 타겟을 덤프 대상에 등록
    MSG_MRPOPO_DUMP_UNREG,                          ///< 타겟을 덤프 대상에 제거
    MSG_MRPOPO_LOWSPEC_CHAR,                        ///< 저사양 캐릭터
    MSG_MRPOPO_LOWSPEC_EFFECT,                      ///< 저사양 이펙트
    MSG_MRPOPO_TEST1,                               ///< 테스트값 조절
    MSG_MRPOPO_TEST2,                               ///< 테스트값 조절
    MSG_MRPOPO_TEST3,                               ///< 테스트값 조절
    MSG_MRPOPO_TENKAICHI_MARK,                      ///< 천하제일 무도회 마크 테스트
    MSG_MRPOPO_TEST_CHANGECOLOR,                    ///< 컬러 변경 이벤트 테스트
    MSG_MRPOPO_TRANSLATE_STATE,                     ///< 상태 변경
    MSG_MRPOPO_TRANSFORM,                           ///< 변신 
    MSG_MRPOPO_STUN,                                ///< 스턴 상태들 테스트
    MSG_MRPOPO_TARGET_MARKING,                      ///< 스킬의 타겟으로 타게팅 테스트
    MSG_MRPOPO_PUSHING,                             ///< 회전 공격에 의한 Push 테스트

    // 그래픽 설정
    MSG_MRPOPO_SPEC_TERRAIN_DIST = 2000,             ///< 지형 거리
    MSG_MRPOPO_SPEC_TERRAIN_SHADOW,                 ///< 지형 그림자
    MSG_MRPOPO_SPEC_WATER_SPECULAR,                 ///< 물의 스펙큘라
	MSG_MRPOPO_SPEC_CHAR_DIST,                      ///< 캐릭터 Dist
    MSG_MRPOPO_SPEC_CHAR_EDGE,                      ///< 캐릭터 EDGE
	MSG_MRPOPO_SPEC_CHAR_GRAYCOLOR,                 ///< 캐릭터 Gray Color
    MSG_MRPOPO_SPEC_EFFECT,                         ///< 로우 스펙 이펙트
	MSG_MRPOPO_SPEC_EFFECT_PARTICLE_RATIO,          ///< Particle Ratio
	MSG_MRPOPO_SPEC_EFFECT_MESH,					///< Mesh Effect
	MSG_MRPOPO_SPEC_EFFECT_DECAL,			        ///< Decal Effect
};

enum EMPPTeleport
{
    MPP_TELE_YAHOI,             // 야호이 마을
    MPP_TELE_YUREKA,            // 유레카 공원
    MPP_TELE_DALPANG,           // 달팽이 마을
    MPP_TELE_DRAGON,            // 용의 코골이
    MPP_TELE_BAEE,              // 배에 빌리지
    MPP_TELE_AJIRANG,           // 아지랑이 샘
    MPP_TELE_KARINGA_1,         // 카린가의 마을 1
    MPP_TELE_KARINGA_2,         // 카린가의 마을 2
    MPP_TELE_GREAT_TREE,        // 그레이트 트리
    MPP_TELE_KARINGA_3,         // 카린가의 어촌
    MPP_TELE_MERMAID,           // 인어상
    MPP_TELE_GANNET,            // 가넷 천문대
    MPP_TELE_EMERALD,           // 에메랄드 비치
    MPP_TELE_TEMBARIN,          // 템버린 캠프    
    MPP_TELE_CELL,              // 셸링
    MPP_TELE_BUU,               // 부우 기념관
    MPP_TELE_CC,                // CC 역사 박물관
    MPP_TELE_MUSHROOM,          // 버섯촌
	MPP_TELE_PAPAYA,
};

/// 덤프 타겟
enum EMPPDumpTarget
{
    MPP_DUMP_SELF,
    MPP_DUMP_TARGET,    
};
