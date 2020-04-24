/******************************************************************************
* File			: NumberGui_Helper.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 21
* Abstract		: 
*****************************************************************************
* Desc			: CNumberGui에 여러가지 유요한 기능을 모아둔다
*****************************************************************************/

#pragma once

class CNumberGui;

// core
#include "NtlCallbackWidget.h"


//////////////////////////////////////////////////////////////////////////
//	입력된 CNumberGui가 차례되로 크기가 변한다
//  1. AddNumberGui() 함수를 적용할 CNumberGui만큼 호출
//  2. Play(true) 호출
//  3. Update() 함수를 직접 호출해 준다
//////////////////////////////////////////////////////////////////////////

class CNumberGui_ZoomHelper
{
public:
	struct sZoomData
	{
		RwBool				bFinish;
		RwReal				fStartRate;
		RwReal				fEndRate;
		RwReal				fRemainTime;
		RwReal				fElapsed;
		CNumberGui*			pNumberGui;
	};

	typedef std::list<sZoomData*>				LIST_ZOOM;


	CNumberGui_ZoomHelper();
	virtual ~CNumberGui_ZoomHelper();

	VOID		Update(RwReal fElapsed);
	VOID		Play(RwBool bPlay);
	VOID		Restart();

	// fStartRate 에서 fEndRate 까지 fRemainTime 시간동안 크기가 변경된다
	// Scale 효과를 줄 CNumberGui의 추가에 실패할 경우 false를 반환한다
	RwBool		AddNumberGui(CNumberGui* pNumberGui, RwReal fStartRate, RwReal fEndRate, RwReal fRemainTime);
	VOID		RemoveAll();

	RwBool		IsFinish();
	RwBool		IsPlay();


	template <class Callbackclass>	
	VOID LinkFinishCallback(Callbackclass *cbclass, RwInt32 (Callbackclass::*callback)())
	{	
		if(m_pCallback)
		{
			NTL_DELETE(m_pCallback);
		}

		m_pCallback = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
	}

protected:
	sZoomData*				GetCurData();

protected:
	RwBool					m_bFinish;
	RwBool					m_bPlay;
	CNtlCallbackParam0*		m_pCallback;
	LIST_ZOOM				m_listZoom;
	LIST_ZOOM::iterator		m_itCur;
};