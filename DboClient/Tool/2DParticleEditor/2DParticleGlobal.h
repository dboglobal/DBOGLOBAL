#ifndef _2DPARTICLE_GLOBAL_H_
#define _2DPARTICLE_GLOBAL_H_

///< Particle Defines
#define GUIFONT_SCRIPT_FILE		".\\gui\\GuiFont.def"
#define METATAG_SCRIPT_FILE		".\\script\\metatag.htm"
#define FLASHFONT_SCRIPT_FILE	".\\gui\\FlashFont.def"

#define PARTICLE_RESOURCE_FILE	"Game.rsr"
#define PARTICLE_SURFACE_FILE	"DBO_2DParticle.srf"

#include <string>

///< Global Data
extern TCHAR g_tcharWorkPath[256];

// 마우스의 상태를 정의한다.
enum eMouseMode
{
	MOUSE_MODE_MOVED,			///< 마우스로 무엇을 선택해서 이동을 하는 상태

	MOUSE_MODE_NUMS,
	MOUSE_MODE_INVALID = 0xFF
};

/// Global API's...

#endif