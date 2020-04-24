#ifndef __GUI_DEFINE_H__
#define __GUI_DEFINE_H__

#pragma warning (disable:4995)
#pragma warning (disable:4786)

#define START_GUI  	namespace gui {
#define END_GUI     }
#define USING_GUI	using namespace gui

/**
* \brief 컴포넌트의 유형을 정의하는 열거형
*/
enum COMPONENT_STYLE
{
	COMP_HER_SCROLL				= 0x00000001,		///< 수평 스크롤 ( Horizontal scroll )
	COMP_VER_SCROLL				= 0x00000002,		///< 수직 스크롤 ( Vertical scroll )
	COMP_TEXT_LEFT				= 0x00000004,		///< Text : 왼쪽 정렬
	COMP_TEXT_CENTER			= 0x00000008,		///< Text : 중앙 정렬
	COMP_TEXT_RIGHT				= 0x00000010,		///< Text : 오른쪽 정렬
	COMP_TEXT_UP				= 0x00000020,		///< Text : 상단 정렬
	COMP_TEXT_VERTICAL_CENTER	= 0x00000040,		///< Text : 중단 정렬
	COMP_TEXT_DOWN				= 0x00000080		///< Text : 하단 정렬
};

/**
* \brief Text의 배경 처리
*/
enum TEXT_BK
{
	BK_TRANSPARENT,			///< 투명
	BK_OPAQUE				///< 불투명
};

/**
* \brief Text의 효과
*/
enum TEXT_EFFECT
{
	TE_NONE,
	TE_SHADOW,				///< 그림자
	TE_OUTLINE				///< outline
};

#define GUI_MAX_PATH			512
#define GUI_TEXT_BUFFER_SIZE	1024
#define GUI_NUMBER_BUFFER_SIZE	32

#define DEFAULT_FONT			"Default"
#define DEFAULT_FONTW			L"Default"
#define DEFAULT_FONT_SIZE		90
#define DEFAULT_FONT_ATTR		0

#define DETAIL_FONT				"detail"
#define DETAIL_FONTW			L"detail"

#define DEFAULT_SHADOW_EFFECT_VALUE		1
#define DEFAULT_OUTLINE_EFFECT_VALUE	0	

#define DEFAULT_BKMODE			BK_TRANSPARENT
#define DEFAULT_TEXTEFFECTMODE	TE_NONE

#define DEFAULT_TEXTCOLOR		RGB(255,255,255)
#define DEFAULT_BKCOLOR			RGB(0,0,0)
#define DEFAULT_TEXTEFFECTCOLOR	RGB(0,0,0)
#define DEFAULT_SELECTCOLOR		RGB(255,32,32)
#define DEFAULT_DISABLECOLOR	RGB(150,150,150)

#define DEFAULT_BUTTON_UP_COLOR				RGB(255, 255, 255)
#define DEFAULT_BUTTON_DOWN_COLOR			RGB(255, 210, 29)
#define DEFAULT_BUTTON_FOCUS_COLOR			RGB(255, 255, 255)
#define DEFAULT_BUTTON_DISABLED_COLOR		RGB(150, 150, 150)

#define DEFAULT_TABCOLOR		RGB(183,191,195)
#define DEFAULT_TABSELCOLOR		RGB(244,215,98)
#define DEFAULT_TABFOCUSCOLOR	RGB(255,255,255)

#define DEFAULT_TEXTCOLOR_RED		255
#define DEFAULT_TEXTCOLOR_GREEN		255
#define DEFAULT_TEXTCOLOR_BLUE		255

#define DEFAULT_BKCOLOR_RED		0
#define DEFAULT_BKCOLOR_GREEN	0
#define DEFAULT_BKCOLOR_BLUE	0

#define DEFAULT_TEXTEFFECTCOLOR_RED		0
#define DEFAULT_TEXTEFFECTCOLOR_GREEN	0
#define DEFAULT_TEXTEFFECTCOLOR_BLUE	0

#define DEFAULT_DISABLECOLOR_RED	173
#define DEFAULT_DISABLECOLOR_GREEN	173
#define DEFAULT_DISABLECOLOR_BLUE	173

#define DEFAULT_SELECTCOLOR_RED		255
#define DEFAULT_SELECTCOLOR_GREEN	32
#define DEFAULT_SELECTCOLOR_BLUE	32

#define DEFAULT_TABSELCOLOR_RED		244
#define DEFAULT_TABSELCOLOR_GREEN	215
#define DEFAULT_TABSELCOLOR_BLUE	98

#define DEFAULT_TABCOLOR_RED		183
#define DEFAULT_TABCOLOR_GREEN		191
#define DEFAULT_TABCOLOR_BLUE		195

#define DEFAULT_TABFOCUSCOLOR_RED	255
#define DEFAULT_TABFOCUSCOLOR_GREEN	255
#define DEFAULT_TABFOCUSCOLOR_BLUE	255

#define DEFAULT_BUTTON_UP_COLOR_RED			255
#define DEFAULT_BUTTON_UP_COLOR_GREEN		255	
#define DEFAULT_BUTTON_UP_COLOR_BLUE		255	

#define DEFAULT_BUTTON_DOWN_COLOR_RED		255
#define DEFAULT_BUTTON_DOWN_COLOR_GREEN		210
#define DEFAULT_BUTTON_DOWN_COLOR_BLUE		29

#define DEFAULT_BUTTON_FOCUS_COLOR_RED		255
#define DEFAULT_BUTTON_FOCUS_COLOR_GREEN	255
#define DEFAULT_BUTTON_FOCUS_COLOR_BLUE		255

#define DEFAULT_BUTTON_DISABLED_COLOR_RED	150
#define DEFAULT_BUTTON_DISABLED_COLOR_GREEN	150
#define DEFAULT_BUTTON_DISABLED_COLOR_BLUE	150

#define DEFAULT_ALPHA				255

#define GUI_BUTTON_DOWN_COORD_X		2
#define GUI_BUTTON_DOWN_COORD_Y		2

#define DEFAULT_SLIDER_WIDTH		16
#define DEFAULT_SLIDER_HEIGHT		16

#define GUI_MOUSE_WHEEL_DELTA		120

typedef unsigned int	COMPPAGE_HANDLE;
typedef unsigned int	RESOURCE_HANDLE;

#define UNISCRIBE_DLLNAME L"\\usp10.dll"

#define GETPROCADDRESS( Module, APIName, Temp ) \
    Temp = GetProcAddress( Module, #APIName ); \
    if( Temp ) \
        *(FARPROC*)&_##APIName = Temp

#define PLACEHOLDERPROC( APIName ) \
    _##APIName = Dummy_##APIName

#define IMM32_DLLNAME L"\\imm32.dll"
#define VER_DLLNAME L"\\version.dll"

#define GUI_PI	3.1415926535897932384626433832795028841971693993751f
#define GUI_DEG2RAD(_x) ((_x) * ( (GUI_PI) / (180.0f) ))
#define GUI_RAD2DEG(_x) ((_x) * ( (180.0f) / (GUI_PI) ))

//////////////////////////////////////////////////////////////////////////////////////
// staticbox symbol macro
// desc : 마침표, 글줄임표 등.
//////////////////////////////////////////////////////////////////////////////////////
#define STATICBOX_SYMBOL_CHARACTER_PERIOD	L'.'
#define STATICBOX_SYMBOL_STRING_ELLIPSIS	L"..."
#define STATICBOX_NULL_STRING				L'\0'

//////////////////////////////////////////////////////////////////////////////////////
// FONT
// desc : Period, writing ellipsis, and so on.
//////////////////////////////////////////////////////////////////////////////////////

//#define FONT_USE_32BIT

//////////////////////////////////////////////////////////////////////////////////////
// SoundLib
// desc : Sound library
//////////////////////////////////////////////////////////////////////////////////////

#define FMOD_SOUND_ATTACHED_TO_GUI

//////////////////////////////////////////////////////////////////////////////////////
// memory pool macro
// desc : Once you use the pool of Renderware.
//////////////////////////////////////////////////////////////////////////////////////

//#define RENDERWARE
//
//#ifdef RENDERWARE
//
//#define GUI_DECLEAR_MEMORY_POOL(class_name, blocksize) \
//public: \
//	static RwInt32 m_i##class_name##AddRef; \
//	static RwFreeList *m_p##class_name##FreeList; \
//	void* operator new(size_t size)  \
//{ \
//	if(m_p##class_name##FreeList == NULL) \
//{ \
//	m_p##class_name##FreeList = RwFreeListCreate(sizeof(class_name), blocksize, 16, rwMEMHINTDUR_GLOBAL); \
//} \
//	m_i##class_name##AddRef++; \
//	return RwFreeListAlloc(m_p##class_name##FreeList, rwMEMHINTDUR_GLOBAL); \
//} \
//	void operator delete(void *pObj) \
//{ \
//	if(m_p##class_name##FreeList) \
//{ \
//	RwFreeListFree(m_p##class_name##FreeList, pObj); \
//	m_i##class_name##AddRef--; \
//	if(m_i##class_name##AddRef <= 0) \
//{ \
//	RwFreeListDestroy(m_p##class_name##FreeList); \
//	m_p##class_name##FreeList = NULL; \
//} \
//} \
//} 
//#define GUI_DEFINITION_MEMORY_POOL(class_name) \
//	RwInt32 class_name::m_i##class_name##AddRef = 0; \
//	RwFreeList* class_name::m_p##class_name##FreeList = 0;

//#endif//RENDERWARE

#endif