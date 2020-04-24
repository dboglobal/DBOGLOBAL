/******************************************************************************
* File			: CharMakeDisplayPart.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 2. 5
* Abstract		: 
*****************************************************************************
* Desc			: 캐릭터를 만들기 위해 캐릭터의 각 부분을 표시하는 구조체 모음
*****************************************************************************/

#pragma once

// core
#include "NtlDebug.h"

// shared
#include "NtlCharacter.h"

// gui
#include "gui_button.h"

enum eDefaultClass
{
	DEFAULT_CLASS_HUMAN_FIGHTER,			// 무도가
	DEFAULT_CLASS_HUMAN_MYSTIC,				// 기공사
	DEFAULT_CLASS_HUMAN_ENGINEER,				// 

	DEFAULT_CLASS_NAMEK_FIGHTER,			// 나메크 전사
	DEFAULT_CLASS_NAMEK_MYSTIC,				// 용족

	DEFAULT_CLASS_MAJIN_MIGHTY,				// 대마인
	DEFAULT_CLASS_MAJIN_WONDER,				// 의마인

	NUM_DEFAULT_CLASS
};

#define dNUM_DEFAULT_CLASS					(6)
#define dNUM_HUMAN_DEFAULT_CLASS			(2)
#define dNUM_NAMEK_DEFAULT_CLASS			(2)
#define dNUM_MAJIN_DEFAULT_CLASS			(2)

#define dNUM_SEX							(2)


struct sDISPLAY_PARTS
{
    virtual ~sDISPLAY_PARTS() {Destroy();}
	virtual VOID Destroy() {}
	virtual VOID Enable(bool bEnable) {}	
	virtual VOID OnMove(RwInt32 iParentX, RwInt32 iParentY) {}
	virtual VOID Render() {}
	virtual VOID PostRender(RwInt32 iSelectIndex) {}
};


struct sDISPLAY_RACE_BUTTONS : public sDISPLAY_PARTS
{
	gui::CStaticBox*	pAttributeName;
	gui::CButton*		pRaceButton[RACE_COUNT];
	gui::CSlot			slotButton[RACE_COUNT];
	CSurfaceGui			srfSelect;

	virtual VOID Destroy()
	{
		NTL_DELETE(pAttributeName);

		for( RwInt32 i = 0 ; i < RACE_COUNT ; ++i )
			NTL_DELETE(pRaceButton[i]);
	}

	virtual VOID Enable(bool bEnable)
	{
		for( RwInt32 i = 0 ; i < RACE_COUNT ; ++i )
			pRaceButton[i]->ClickEnable(bEnable);
	}

	virtual VOID PostRender(RwInt32 iSelectIndex)
	{
		if( RACE_COUNT > iSelectIndex )
		{
			CRectangle rtButton = pRaceButton[iSelectIndex]->GetScreenRect();
			srfSelect.SetPosition(rtButton.left, rtButton.top);
			srfSelect.Render();
		}
	}
};


struct sDISPLAY_CLASS_BUTTONS : public sDISPLAY_PARTS
{
	typedef std::list<gui::CButton*>			LIST_ACTIVE_CLASS;

	gui::CStaticBox*	pAttributeName;
	gui::CButton*		pClassButton[NUM_DEFAULT_CLASS];
	gui::CSlot			slotButton[NUM_DEFAULT_CLASS];
	CSurfaceGui			srfSelect;
	LIST_ACTIVE_CLASS	listActiveClass;	

	virtual VOID Destroy()
	{
		NTL_DELETE(pAttributeName);

		for( RwUInt8 i = 0 ; i < NUM_DEFAULT_CLASS ; ++i )
			NTL_DELETE(pClassButton[i]);

		listActiveClass.clear();
	}

	virtual VOID Enable(bool bEnable)
	{
		for( RwInt32 i = 0 ; i < NUM_DEFAULT_CLASS ; ++i )
			pClassButton[i]->ClickEnable(bEnable);
	}

	virtual VOID PostRender(RwInt32 iSelectIndex)
	{
		LIST_ACTIVE_CLASS::iterator it = listActiveClass.begin();
		for( RwUInt8 i = 0 ; i < listActiveClass.size() ; ++i, ++it )
		{
			if( i == iSelectIndex )
			{
				CRectangle rtButton = (*it)->GetScreenRect();
				srfSelect.SetPosition(rtButton.left, rtButton.top);
				srfSelect.Render();
				break;
			}
		}
	}

	VOID ActivateClass(RwUInt8 byRace)
	{
		listActiveClass.clear();

		switch(byRace)
		{
		case RACE_HUMAN:
			{
				for( RwUInt8 i = 0 ; i < NUM_DEFAULT_CLASS ; ++i )
				{
					if( i == DEFAULT_CLASS_HUMAN_FIGHTER ||
						i == DEFAULT_CLASS_HUMAN_MYSTIC ||
						DEFAULT_CLASS_HUMAN_ENGINEER == i)
					{
						listActiveClass.push_back( pClassButton[i] );
						pClassButton[i]->Show(true);
					}
					else
					{
						pClassButton[i]->Show(false);
					}
				}
				break;
			}
		case RACE_NAMEK:
			{
				for( RwUInt8 i = 0 ; i < NUM_DEFAULT_CLASS ; ++i )
				{
					if( i == DEFAULT_CLASS_NAMEK_FIGHTER ||
						i == DEFAULT_CLASS_NAMEK_MYSTIC )
					{
						listActiveClass.push_back( pClassButton[i] );
						pClassButton[i]->Show(true);
					}
					else
					{
						pClassButton[i]->Show(false);
					}
				}
				break;
			}
		case RACE_MAJIN:
			{
				for( RwUInt8 i = 0 ; i < NUM_DEFAULT_CLASS ; ++i )
				{
					if( i == DEFAULT_CLASS_MAJIN_MIGHTY ||
						i == DEFAULT_CLASS_MAJIN_WONDER )
					{
						listActiveClass.push_back( pClassButton[i] );
						pClassButton[i]->Show(true);
					}
					else
					{
						pClassButton[i]->Show(false);
					}
				}
				break;
			}
		default:
			{
				NTL_ASSERT(false, "sDISPLAY_CLASS_BUTTONS, ActivateClass, Invalid race : " << byRace);
				break;
			}
		}
	}
};


struct sDISPLAY_SEX_BUTTONS : public sDISPLAY_PARTS
{
	gui::CStaticBox*	pAttributeName;
	gui::CButton*		pSexButton[dNUM_SEX];
	gui::CSlot			slotButton[dNUM_SEX];
	CSurfaceGui			srfSelect;

	virtual VOID Destroy()
	{
		NTL_DELETE(pAttributeName);

		for( RwInt32 i = 0 ; i < dNUM_SEX ; ++i )
			NTL_DELETE(pSexButton[i]);
	}

	virtual VOID Enable(bool bEnable)
	{
		for( RwInt32 i = 0 ; i < dNUM_SEX ; ++i )
			pSexButton[i]->ClickEnable(bEnable);
	}

	virtual VOID PostRender(RwInt32 iSelectIndex)
	{
		if( false == pSexButton[0]->IsEnabled() )
			return;

		if( dNUM_SEX > iSelectIndex )
		{
			CRectangle rtButton = pSexButton[iSelectIndex]->GetScreenRect();
			srfSelect.SetPosition(rtButton.left, rtButton.top);
			srfSelect.Render();
		}
	}
};


struct sDISPLAY_LEFT_RIGHT_BUTTONS : public sDISPLAY_PARTS
{
	gui::CStaticBox*	pAttributeName;
	gui::CButton*		pLeftButton;
	gui::CButton*		pRightButton;
	CSurfaceGui			srfBackground;

	gui::CSlot			m_slotLeftButton;
	gui::CSlot			m_slotRightButton;

	virtual VOID Destroy()
	{
		NTL_DELETE(pAttributeName);
		NTL_DELETE(pLeftButton);
		NTL_DELETE(pRightButton);
	}

	virtual VOID Enable(bool bEnable)
	{
		pLeftButton->ClickEnable(bEnable);
		pRightButton->ClickEnable(bEnable);
	}

	virtual VOID OnMove(RwInt32 iParentX, RwInt32 iParentY)
	{
		srfBackground.SetPositionbyParent(iParentX, iParentY);
	}

	virtual VOID Render()
	{
		srfBackground.Render();
	}
};