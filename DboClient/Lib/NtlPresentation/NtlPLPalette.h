/*****************************************************************************
*
* File			: NtlPLPalette.h
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2008. 3. 13
* Abstract		: Presentation layer palette info
*****************************************************************************
* Desc          : Dbo에서 쓰이는 팔레트 정보를 가지고 있다
*****************************************************************************/

#ifndef __NTL_PLPALETTE_H__
#define __NTL_PLPALETTE_H__

#define dEMBLEM_PALETTE_COUNT			32
#define	dPALETTE_EMBLEM_ELEMENT_NAME	"Emblem/element"


#define dINVALID_EMBLEM_PALETTE_INDEX			(0xff)
#define dINVALID_DOGI_PALETTE_INDEX				(0xff)


enum ePaletteType
{
	PALETTETYPE_EMBLEM,
	PALETTETYPE_DOGI,

	NUM_PALETTE
};

enum eLOAD_RESULT
{
	LOAD_RESULT_OK,

	LOAD_RESULT_FAIL_LOAD_FILE,						///< 파일을 읽지 못했다
	LOAD_RESULT_NOT_EXIST_PALETTE,					///< 해당 팔레트가 없다
	LOAD_RESULT_LESS_ELEMENT_COUNT,					///< 팔레트의 구성 갯수가 dEMBLEM_PALETTE_COUNT보다 작다
	LOAD_RESULT_MORE_ELEMENT_COUNT,					///< 팔레트의 구성 갯수가 dEMBLEM_PALETTE_COUNT보다 많다
};

class CNtlPLPalette
{
public:
	static CNtlPLPalette* GetInstance() { return m_pInstance; }

	CNtlPLPalette();
	virtual ~CNtlPLPalette();

	static VOID	CreateInstance();
	static VOID	DestoryInstance();

	COLORREF		GetColor(ePaletteType eType, RwUInt32 uiColorIndex);
	RwUInt8			GetIndex(ePaletteType eType, const COLORREF* pColor);

	///< 팔레트를 다시 읽어들이는 개발용 함수
	///< 절대로! 실제 게임에서 외부 데이터를 읽어 사용하지 말자.
	///< 팔레트 관련 담당자가 팔레트만 바꾸고 관련 GUI 등이 바뀌지 않을 위험이 있다
	eLOAD_RESULT	LoadPalette_forDeveloper();

protected:
	VOID	InitEmblemPalette();

protected:
	static CNtlPLPalette* m_pInstance;

	COLORREF		m_aEmblemColorPalette[dEMBLEM_PALETTE_COUNT];
};

static CNtlPLPalette* GetNtlPLPalette(VOID)
{
	return CNtlPLPalette::GetInstance();
}

#endif