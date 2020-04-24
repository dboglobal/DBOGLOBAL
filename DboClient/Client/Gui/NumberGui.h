/******************************************************************************
* File			: CNumberGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 9. 4
* Abstract		: 
*****************************************************************************
* Desc			: GUI단에서 이미지로 구성된 숫자로 표시하기 위한 클래스
*				  m_rtRect의 영역이 실제로 표시되는 공간을 가리키는 것이 아니다
*				  m_rtRect의 구성요소중 left, top(이미지의 최상단)은 숫자가
*				  표시되는 부모로부터의 정렬에 따른 기준 좌표 right, bottom은
*				  정렬방법에 상관없이 left, top에서부터 무조건 넓이와 높이를 가리킨다
*
*				  이미지의 넓이, 높이는 모두 같다고 가정한다
*****************************************************************************/

#pragma once

// core
#include "NtlDebug.h"
#include "NtlCallbackWidget.h"

// dbo
#include "SurfaceGui.h"


#define dMAX_CHPHER			10		///< 숫자를 표시할 수 있는 최대 자릿수

#define dNCFLAG_NONE		0x00
#define dNCFLAG_MINUS		0x01
#define dNCFLAG_PLUS		0x02

enum eNumberContent
{
	NC_0,
	NC_1,
	NC_2,
	NC_3,
	NC_4,
	NC_5,
	NC_6,
	NC_7,
	NC_8,
	NC_9,
	NC_PLUS,
	NC_MINUS,

	NUM_NC
};

enum eNumberHoriDirection
{
	ND_LEFT,
	ND_CENTER,
	ND_RIGHT
};

enum eNumberVertDirection
{
	NVD_TOP,
	NVD_CENTER,
	NVD_BOTTOM,
};


class CNumberGui
{
public:
	struct sEffectRate
	{
		RwBool			bEnable;
		RwReal			fStartRate;
		RwReal			fEndRate;
		RwReal			fRemainTime;
		RwReal			fElapsed;

		CNtlCallbackParam0* pCallback;

		sEffectRate()
		:bEnable(false)
		,fStartRate(0.f)
		,fEndRate(0.f)
		,fRemainTime(0.f)
		,fElapsed(0.f)
		,pCallback(NULL)
		{
		}
	};

	typedef VOID* EndEffectRateCallBack();

	CNumberGui();
	virtual ~CNumberGui();
	
	/*	byHoriAlign		: 주어진 좌표에서 숫자 및 표식이 정렬되는 좌우 방향
		byVertAlign		: 주어진 좌표에서 숫자 및 표식이 정렬되는 상하 방향
		byNumGap		: 숫자 및 표식간의 간격
		byFlag			: 숫자 외의 사용할 표식의 플래그를 등록
		byRemainCipher	: 반드시 표시해야 할 숫자의 자릿수
						  예) byRemainCipher = 2 일 때 SetNumber(1) 이면 01을 표시
							  byRemainCipher = 4 일 때 SetNumber(1) 이면 0001을 표시 */	
	RwBool			Create(RwUInt8 byHoriAlign = ND_LEFT, RwUInt8 byVertAlign = NVD_TOP, RwInt32 iNumGap = 0, RwUInt8 byFlag = dNCFLAG_NONE, RwUInt8 byRemainCipher = 1);
	VOID			Update(RwReal fElapsed);
	VOID			Destroy();

	VOID			SetNumber(RwInt64 iNumber);
	VOID			SetSurface(RwInt8 byIndex, gui::CSurface& surface);
	VOID			SetPosition(RwInt32 iPosX, RwInt32 iPosY);
	VOID			SetParentPos(RwInt32 iParentX, RwInt32 iParentY);
	VOID			SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha);
	VOID			SetColorOnly(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	VOID			SetAlpha(RwUInt8 byAlpha);
	VOID			SetNumGap(RwInt32 iGap);
	VOID			Show(RwBool bShow);
	VOID			SetHoriDirection(RwUInt8 byHoriDirection);
	VOID			SetVertDirection(RwUInt8 byVertDirection);
	VOID			SetRate(RwReal fRate);	

	RwInt64			GetNumber();
	RwInt32			GetWidth();
	RwInt32			GetContentWidth(RwInt8 byIndex);
	RwInt32			GetHeight();	
	CRectangle		GetPosition();
	CPos			GetParentPos();
	CRectangle		GetRect();
	CRectangle		GetScreenRect();
	RwReal			GetRate();

	// CNumberGui_ZoomHelper 클래스를 이용하세요. 이 함수는 차후 지울 예정입니다
	VOID			EffectRate(RwReal fStartRate, RwReal fEndRate, RwReal fRemainTime);

	VOID			UnsetSurface(RwUInt8 byIndex);
	VOID			UnsetAll();

	RwBool			PtInRect(RwInt32 iPosX, RwInt32 iPosY);
	RwBool			IsShow();
	RwBool			IsEffectRate();

	VOID			Render(bool bRenderTop = false);


	// when finish rate effect
	template <class Callbackclass>	
	VOID			LinkRateEffect(Callbackclass *cbclass,RwInt32 (Callbackclass::*callback)())
	{	
		if(m_EffectRate.pCallback)
		{
			NTL_DELETE(m_EffectRate.pCallback);
		}

		m_EffectRate.pCallback = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
	}

protected:
	VOID			CalcWidth();

protected:
	CSurfaceGui			m_srfNumber[NUM_NC];			///< 전투력을 표시하는 숫자
	
	RwUInt8				m_byFlag;
	RwUInt8				m_byHoriDirection;
	RwUInt8				m_byVertDirection;

	RwInt32				m_iNumGap;		
	RwInt32				m_iX_fromParent, m_iY_fromParent;
	RwInt32				m_iWidth, m_iHeight;
	CPos				m_ParentPos;
	RwReal				m_fRate;

	RwUInt8				m_byCurCipher;					///< 현재 자릿수
	RwUInt8				m_byCipher[dMAX_CHPHER];		///< 각 자릿수별 숫자
	RwUInt8				m_byRemainCipher;				///< 유지되야할 최소 자릿수
	RwInt64				m_i64Number;					///< 입력받은 숫자

	sEffectRate			m_EffectRate;
};



inline VOID CNumberGui::SetSurface(RwInt8 byIndex, gui::CSurface& surface)
{
	m_srfNumber[byIndex].SetSurface(surface);
}

inline VOID CNumberGui::SetParentPos(RwInt32 iParentX, RwInt32 iParentY)
{
	m_ParentPos.x = iParentX;
	m_ParentPos.y = iParentY;
}

inline VOID CNumberGui::SetColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byAlpha )
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].SetColor(byRed, byGreen, byBlue, byAlpha);
}

inline VOID CNumberGui::SetColorOnly( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue )
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].SetColorOnly(byRed, byGreen, byBlue);
}

inline VOID CNumberGui::SetAlpha(RwUInt8 byAlpha)
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].SetAlpha(byAlpha);
}

inline VOID CNumberGui::SetNumGap(RwInt32 iGap)
{
	m_iNumGap = iGap;
}

inline VOID CNumberGui::Show(RwBool bShow)
{
	for( RwInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].Show(bShow);
}

inline VOID CNumberGui::SetHoriDirection(RwUInt8 byHoriDirection)
{
	m_byHoriDirection = byHoriDirection;
}

inline VOID CNumberGui::SetVertDirection(RwUInt8 byVertDirection)
{
	m_byVertDirection = byVertDirection;
}

inline RwInt64 CNumberGui::GetNumber()
{
	return m_i64Number;
}

inline RwInt32 CNumberGui::GetWidth()
{
	return m_iWidth;
}

inline RwInt32 CNumberGui::GetContentWidth(RwInt8 byIndex)
{
	return m_srfNumber[byIndex].GetWidth();
}

inline RwInt32 CNumberGui::GetHeight()
{
	return m_iHeight;
}

inline CRectangle CNumberGui::GetPosition()
{
	// 부모로부터의 기준 좌표와 넓이, 높이만을 참고하자
	CRectangle rtRect;

	rtRect.left		= m_iX_fromParent;
	rtRect.top		= m_iY_fromParent;
	rtRect.right	= rtRect.left + m_iX_fromParent;
	rtRect.bottom	= rtRect.top + m_iY_fromParent;

	return rtRect;
}

inline CPos CNumberGui::GetParentPos()
{
	return m_ParentPos;
}

inline VOID CNumberGui::UnsetSurface(RwUInt8 byIndex)
{
	m_srfNumber[byIndex].UnsetTexture();
}

inline VOID CNumberGui::UnsetAll()
{
	for( RwUInt8 i = 0 ; i < NUM_NC ; ++i )
		m_srfNumber[i].UnsetTexture();
}

inline RwBool CNumberGui::PtInRect(RwInt32 iPosX, RwInt32 iPosY)
{
	// 화면 좌표가 아닌 부모로부터의 좌표를 입력해야 한다
	CRectangle rtRect = GetRect();

	return rtRect.PtInRect(iPosX, iPosY);
}

inline RwBool CNumberGui::IsShow()
{
	return m_srfNumber[NC_0].IsShow();
}

inline RwBool CNumberGui::IsEffectRate()
{
	return m_EffectRate.bEnable;
}

inline RwReal CNumberGui::GetRate()
{
	return m_fRate;
}