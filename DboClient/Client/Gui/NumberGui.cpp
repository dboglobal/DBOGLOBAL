#include "precomp_dboclient.h"
#include "NumberGui.h"

// core
#include "NtlDebug.h"

namespace
{
	#define dMAX_NUMBER_ABS			0xffffffff
}


CNumberGui::CNumberGui()
:m_byHoriDirection(ND_LEFT)
,m_byVertDirection(NVD_TOP)
,m_iNumGap(0)
,m_byCurCipher(0)
,m_i64Number(dMAX_NUMBER_ABS)
{
	m_ParentPos.x = 0;
	m_ParentPos.y = 0;
	m_EffectRate.pCallback = NULL;
}

CNumberGui::~CNumberGui()
{
	Destroy();
}

RwBool CNumberGui::Create(RwUInt8 byHoriAlign /* = NCA_LEFT */, RwUInt8 byVertAlign /* = NVD_TOP */, RwInt32 iNumGap /* = 0 */,
						  RwUInt8 byFlag /* = dNCFLAG_NONE */, RwUInt8 byRemainCipher /* = 1 */)
{
	NTL_FUNCTION( "CNumberGui::Create" );

	NTL_ASSERT(byRemainCipher > 0, "CNumberGui::Create, invalid remain cipher : " << byRemainCipher);

	m_byHoriDirection	= byHoriAlign;
	m_byVertDirection	= byVertAlign;
	m_iNumGap			= iNumGap;
	m_byFlag			= byFlag;
	m_byRemainCipher	= byRemainCipher;
	m_i64Number			= dMAX_NUMBER_ABS;	

	NTL_RETURN(TRUE);
}

VOID CNumberGui::Destroy()
{
	NTL_FUNCTION( "CNumberGui::Destroy" );

	UnsetAll();

	if( m_EffectRate.pCallback )
		NTL_DELETE(m_EffectRate.pCallback);

	NTL_RETURNVOID();
}

VOID CNumberGui::SetNumber(RwInt64 iNumber)
{
	if( m_i64Number == iNumber )
		return;

	m_i64Number = iNumber;

	// 자릿수를 알아보기 위해
	iNumber = abs((long)iNumber);

	if( m_i64Number < dMAX_NUMBER_ABS )
	{
		for( RwUInt8 i = 0 ; i < dMAX_CHPHER ; ++i )
		{	
			RwUInt64 iCurNum = (RwUInt64)(iNumber - (iNumber/10*10));

			m_byCipher[i] = (RwUInt8)iCurNum;

			iNumber = iNumber / 10;

			// 몇자리 숫자인가
			if( iNumber == 0 )
			{
				m_byCurCipher = i + 1;
				break;
			}
		}
	}
	else
	{
		m_byCurCipher = 0;
	}
	

	CalcWidth();
}

VOID CNumberGui::Update(RwReal fElapsed)
{
	if( m_EffectRate.bEnable )
	{
		m_EffectRate.fElapsed += fElapsed;

		if( m_EffectRate.fElapsed >= m_EffectRate.fRemainTime )
			m_EffectRate.fElapsed	= m_EffectRate.fRemainTime;


		RwReal fRate = m_EffectRate.fStartRate + (m_EffectRate.fEndRate - m_EffectRate.fStartRate) * (m_EffectRate.fElapsed / m_EffectRate.fRemainTime);
		SetRate(fRate);


		if( m_EffectRate.fElapsed == m_EffectRate.fRemainTime )
		{
			m_EffectRate.bEnable = FALSE;
			if( m_EffectRate.pCallback )
				m_EffectRate.pCallback->Call();
		}
	}	
}

VOID CNumberGui::SetPosition(RwInt32 iPosX, RwInt32 iPosY)
{
	if( m_iX_fromParent == iPosX && m_iY_fromParent == iPosY )
		return;

	m_iX_fromParent	= iPosX;
	m_iY_fromParent	= iPosY;

	CalcWidth();
}

VOID CNumberGui::SetRate(RwReal fRate)
{
	if( m_fRate == fRate )
		return;

	m_fRate = fRate;

	for( RwUInt8 i = 0 ; i < NUM_NC ; ++i )
	{
		gui::CSurface* pSurface = m_srfNumber[i].GetSurface();

		pSurface->m_SnapShot.rtRect.SetRectWH(pSurface->m_Original.rtRect.left,
											  pSurface->m_Original.rtRect.top,
											  (RwInt32)((RwReal)pSurface->m_Original.rtRect.GetWidth() * fRate),
											  (RwInt32)((RwReal)pSurface->m_Original.rtRect.GetHeight() * fRate));
	}

	CalcWidth();
}

VOID CNumberGui::EffectRate(RwReal fStartRate, RwReal fEndRate, RwReal fRemainTime)
{
	if( fStartRate == fEndRate )
		return;

	if( m_EffectRate.fStartRate < 0.f )
		return;

	if( m_EffectRate.fEndRate < 0.f )
		return;

	m_EffectRate.bEnable		= TRUE;
	m_EffectRate.fStartRate		= fStartRate;
	m_EffectRate.fEndRate		= fEndRate;
	m_EffectRate.fRemainTime	= fRemainTime;
	m_EffectRate.fElapsed		= 0.f;
}

CRectangle CNumberGui::GetRect()
{
	CRectangle rtRect;

	// 좌우
	if( m_byHoriDirection == ND_LEFT )
	{
		rtRect.left		= m_iX_fromParent - m_iWidth;
		rtRect.right	= m_iX_fromParent;
	}
	if( m_byHoriDirection == ND_CENTER )
	{
		rtRect.left		= m_iX_fromParent - m_iWidth/2;
		rtRect.right	= m_iX_fromParent + m_iWidth/2;
	}
	else if( m_byHoriDirection == ND_RIGHT )
	{
		rtRect.left		= m_iX_fromParent;
		rtRect.right	= m_iX_fromParent + m_iWidth;
	}

	// 상하
	if( m_byVertDirection == NVD_CENTER )
	{
		rtRect.top		= m_iY_fromParent - m_iHeight/2;
		rtRect.bottom	= m_iY_fromParent + m_iHeight/2;
	}
	else
	{
		rtRect.top		= m_iY_fromParent;
		rtRect.bottom	= m_iY_fromParent + m_iHeight;
	}

	return rtRect;
}

CRectangle CNumberGui::GetScreenRect()
{
	CRectangle rtScreen;

	rtScreen = GetRect();

	rtScreen.left		+= m_ParentPos.x;
	rtScreen.top		+= m_ParentPos.y;
	rtScreen.right		+= m_ParentPos.x;
	rtScreen.bottom		+= m_ParentPos.y;

	return rtScreen;
}

VOID CNumberGui::CalcWidth()
{
	m_iWidth = 0;
	m_iHeight = 0;

	if( m_i64Number == dMAX_NUMBER_ABS )
		return;

	m_iHeight = m_srfNumber[NC_0].GetHeight();

	for( RwUInt32 i = 0 ; i < m_byCurCipher ; ++i )
	{
		m_iWidth += m_srfNumber[NC_0].GetWidth();
		m_iWidth += m_iNumGap;
	}

	// 유지되야할 최소 자릿수보다 실제 자릿수가 작다면
	if( m_byCurCipher < m_byRemainCipher )
	{
		RwInt8 byExpendCipher = m_byRemainCipher - m_byCurCipher;

		for(RwUInt8 i = 0 ; i < byExpendCipher ; ++i)
		{
			m_iWidth += m_srfNumber[NC_0].GetWidth();
			m_iWidth += m_iNumGap;
		}		
	}


	if( m_i64Number > 0 && m_byFlag & dNCFLAG_PLUS )
	{
		m_iWidth += m_srfNumber[ NC_PLUS ].GetWidth();
	}
	else if( m_i64Number < 0 && m_byFlag & dNCFLAG_MINUS )
	{
		m_iWidth += m_srfNumber[ NC_MINUS ].GetWidth();
	}
	else
		m_iWidth -= m_iNumGap;
}

VOID CNumberGui::Render(bool bRenderTop /* = false */)
{
	if( IsShow() == FALSE )
		return;

	if( m_i64Number == dMAX_NUMBER_ABS )
		return;


	RwInt32 iPosX = m_ParentPos.x + m_iX_fromParent;
	RwInt32 iPosY = m_ParentPos.y + m_iY_fromParent;

	// 수평 위치
	if( m_byHoriDirection == ND_LEFT )
	{
		iPosX -= m_iWidth;
	}
	else if( m_byHoriDirection == ND_CENTER )
	{
		iPosX -= m_iWidth/2;
	}

	// 상하 위치
	if( m_byVertDirection == NVD_CENTER )
	{
		iPosY -= m_iHeight/2;
	}
	else if( m_byVertDirection == NVD_BOTTOM )
	{
		iPosY -= m_iHeight;
	}


	if( m_i64Number > 0 && m_byFlag & dNCFLAG_PLUS )
	{
		m_srfNumber[ NC_PLUS ].SetPosition(iPosX, iPosY);
		m_srfNumber[ NC_PLUS ].Render();

		iPosX += m_srfNumber[ NC_PLUS ].GetWidth();
		iPosX += m_iNumGap;
	}
	else if( m_i64Number < 0 && m_byFlag & dNCFLAG_MINUS )
	{
		m_srfNumber[ NC_MINUS ].SetPosition(iPosX, iPosY);
		m_srfNumber[ NC_MINUS ].Render();

		iPosX += m_srfNumber[ NC_MINUS ].GetWidth();
		iPosX += m_iNumGap;
	}

	// 유지되야할 최소 자릿수보다 실제 자릿수가 작다면 0으로 먼저 채운다
	if( m_byCurCipher < m_byRemainCipher )
	{
		RwInt8 byExpendCipher = m_byRemainCipher - m_byCurCipher;

		for( RwUInt8 i = 0 ; i < byExpendCipher ; ++i )
		{
			m_srfNumber[NC_0].SetPosition(iPosX, iPosY);
			m_srfNumber[NC_0].Render();

			iPosX += m_srfNumber[NC_0].GetWidth();
			iPosX += m_iNumGap;
		}
	}

	for( RwUInt32 i = 1 ; i <= m_byCurCipher ; ++i )
	{
        if(m_byCurCipher < dMAX_CHPHER)
        {
		    m_srfNumber[ m_byCipher[m_byCurCipher - i] ].SetPosition(iPosX, iPosY);
		    m_srfNumber[ m_byCipher[m_byCurCipher - i] ].Render(bRenderTop);
        }

		iPosX += m_srfNumber[NC_0].GetWidth();
		iPosX += m_iNumGap;
	}
}

