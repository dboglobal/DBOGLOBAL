#include "precomp_ntlpresentation.h"
#include "NtlPLPalette.h"

// core
#include "NtlDebug.h"

// util
#include "NtlXMLDoc.h"


CNtlPLPalette* CNtlPLPalette::m_pInstance = NULL;

CNtlPLPalette::CNtlPLPalette()
{	
}

CNtlPLPalette::~CNtlPLPalette()
{	
}

VOID CNtlPLPalette::CreateInstance()
{
	NTL_FUNCTION("CNtlPLPalette::CreateInstance");
	NTL_ASSERT(m_pInstance == NULL, "CNtlPLPalette::CreateInstance, Already created instance");

	m_pInstance = NTL_NEW CNtlPLPalette;

	m_pInstance->InitEmblemPalette();

	NTL_RETURNVOID();	
}

VOID CNtlPLPalette::DestoryInstance()
{
	NTL_FUNCTION("CNtlPLPalette::~DestoryInstance");

	NTL_DELETE(m_pInstance);
	
	NTL_RETURNVOID();
}

COLORREF CNtlPLPalette::GetColor(ePaletteType eType, RwUInt32 uiColorIndex)
{
	switch( eType )
	{
	case PALETTETYPE_EMBLEM:
		{
			if( dINVALID_EMBLEM_PALETTE_INDEX == uiColorIndex )
				return RGB(255, 255, 255);

			if( uiColorIndex >= dEMBLEM_PALETTE_COUNT )
			{
				DBO_FAIL("CNtlPLPalette::GetColor, PALETTETYPE_EMBLEM, too big color index : " << uiColorIndex);
				return RGB(255, 255, 255);
			}
			return m_aEmblemColorPalette[uiColorIndex];
		}
	case PALETTETYPE_DOGI:
		{
			if( dINVALID_DOGI_PALETTE_INDEX == uiColorIndex )
				return RGB(255, 255, 255);

			if( uiColorIndex >= dEMBLEM_PALETTE_COUNT )
			{
				DBO_FAIL("CNtlPLPalette::GetColor, PALETTETYPE_EMBLEM, too big color index : " << uiColorIndex);
				return RGB(255, 255, 255);
			}
			return m_aEmblemColorPalette[uiColorIndex];
		}
	}

	NTL_ASSERT(false, "CNtlPLPalette::GetColor, Invalid palette type : " << eType);
	return RGB(255, 255, 255);
}

RwUInt8 CNtlPLPalette::GetIndex(ePaletteType eType, const COLORREF* pColor)
{
	switch( eType )
	{
	case PALETTETYPE_EMBLEM:
	case PALETTETYPE_DOGI:
		{
			for(RwUInt8 i = 0 ; i < dEMBLEM_PALETTE_COUNT ; ++i )
			{
				if( *pColor == m_aEmblemColorPalette[i] )
				{
					return i;
				}
			}

			return 0xff;
		}
	}

	NTL_ASSERT(false, "CNtlPLPalette::GetColor, Invalid palette type : " << eType);
	return 0xff;
}

VOID CNtlPLPalette::InitEmblemPalette()
{
	m_aEmblemColorPalette[0]	= RGB(158,		11,		15);
	m_aEmblemColorPalette[1]	= RGB(238,		28,		36);
	m_aEmblemColorPalette[2]	= RGB(255,		70,		70);
	m_aEmblemColorPalette[3]	= RGB(255,		102,	0);
	m_aEmblemColorPalette[4]	= RGB(255,		186,	0);
	m_aEmblemColorPalette[5]	= RGB(255,		245,	118);
	m_aEmblemColorPalette[6]	= RGB(159,		220,	71);
	m_aEmblemColorPalette[7]	= RGB(88,		255,	152);

	m_aEmblemColorPalette[8]	= RGB(75,		134,	9);
	m_aEmblemColorPalette[9]	= RGB(0,		108,	33);
	m_aEmblemColorPalette[10]	= RGB(60,		186,	146);
	m_aEmblemColorPalette[11]	= RGB(0,		115,	106);
	m_aEmblemColorPalette[12]	= RGB(1,		255,	255);
	m_aEmblemColorPalette[13]	= RGB(0,		174,	239);
	m_aEmblemColorPalette[14]	= RGB(0,		84,		165);
	m_aEmblemColorPalette[15]	= RGB(0,		0,		190);

	m_aEmblemColorPalette[16]	= RGB(133,		95,		168);
	m_aEmblemColorPalette[17]	= RGB(145,		39,		143);
	m_aEmblemColorPalette[18]	= RGB(143,		35,		244);
	m_aEmblemColorPalette[19]	= RGB(237,		0,		140);
	m_aEmblemColorPalette[20]	= RGB(158,		0,		92);
	m_aEmblemColorPalette[21]	= RGB(48,		0,		74);
	m_aEmblemColorPalette[22]	= RGB(149,		149,	149);
	m_aEmblemColorPalette[23]	= RGB(85,		85,		85);

	m_aEmblemColorPalette[24]	= RGB(140,		98,		58);
	m_aEmblemColorPalette[25]	= RGB(96,		56,		17);
	m_aEmblemColorPalette[26]	= RGB(172,		66,		0);
	m_aEmblemColorPalette[27]	= RGB(255,		146,	187);
	m_aEmblemColorPalette[28]	= RGB(128,		89,		113);
	m_aEmblemColorPalette[29]	= RGB(58,		66,		52);
	m_aEmblemColorPalette[30]	= RGB(255,		255,	255);
	m_aEmblemColorPalette[31]	= RGB(0,		0,		0);
}

eLOAD_RESULT CNtlPLPalette::LoadPalette_forDeveloper()
{
	CNtlXMLDoc XMLDoc;

	XMLDoc.Create();

	// NtlPresentation 레이어에 Palette.xml 을 추가해 두었다
	if( !XMLDoc.Load("Palette.xml") )
		return LOAD_RESULT_FAIL_LOAD_FILE;

	IXMLDOMNodeList* pElementNodeList = XMLDoc.SelectNodeList(dPALETTE_EMBLEM_ELEMENT_NAME);
	if( !pElementNodeList )
		return LOAD_RESULT_NOT_EXIST_PALETTE;	


	RwUInt8 byRValue, byGValue, byBValue;
	char acBuffer_R[32], acBuffer_G[32], acBuffer_B[32];

	long listLen = 0;
	pElementNodeList->get_length(&listLen);

	if( dEMBLEM_PALETTE_COUNT > listLen )
		return LOAD_RESULT_LESS_ELEMENT_COUNT;

	if( dEMBLEM_PALETTE_COUNT < listLen )
		return LOAD_RESULT_MORE_ELEMENT_COUNT;


	for(long i = 0; i < listLen; ++i)
	{
		IXMLDOMNode* pNode = NULL;
		pElementNodeList->get_item(i, &pNode);
		if(!pNode)
			continue;

		XMLDoc.GetTextWithAttributeName(pNode, "R", acBuffer_R, sizeof(acBuffer_R));
		XMLDoc.GetTextWithAttributeName(pNode, "G", acBuffer_G, sizeof(acBuffer_G));
		XMLDoc.GetTextWithAttributeName(pNode, "B", acBuffer_B, sizeof(acBuffer_B));

		byRValue = (RwUInt8)atoi(acBuffer_R);
		byGValue = (RwUInt8)atoi(acBuffer_G);
		byBValue = (RwUInt8)atoi(acBuffer_B);

		m_aEmblemColorPalette[i] = RGB(byRValue, byGValue, byBValue);
	}

	return LOAD_RESULT_OK;
}