#pragma once

// 버튼리스트의 플래그들
#define BL_MULTI_SELECT       0x00000001                    ///< 토글버튼을 이용한 멀티 선택 가능

template <typename _TClass, typename _TData = void*>
class CButtonList
{
public:
	struct SItem
	{
		std::wstring	strText;
		_TData			TData;
	};

	struct SButtonItem
	{
		gui::CButton*	pButton;
		gui::CSlot		slotClicked;
        gui::CSlot      slotToggled;        ///< 토글이 눌러질때로 클릭으로 인식한다 (by agebreak)
		SItem*			pItem;
	};

	typedef gui::CButton*				(_TClass::*PCBCREATEITEM)();
	typedef void						(_TClass::*PCBDESTROYITEM)(gui::CButton* pButton);
	typedef void						(_TClass::*PCBCLICKEDITEM)(RwInt32 iIndex);
	typedef std::vector<SItem*>			ITEMVEC;
	typedef std::vector<SButtonItem*>	BUTTONITEMVEC;

protected:
	_TClass*		m_pOwnerClass;
	PCBCREATEITEM	m_pCreateItem;
	PCBDESTROYITEM	m_pDestroyItem;
	PCBCLICKEDITEM	m_pClickedItem;

	ITEMVEC			m_vecItem;
	BUTTONITEMVEC	m_vecButtonItem;

	RwInt32			m_iLeft;
	RwInt32			m_iTop;
	RwInt32			m_iMarginHeight;
	RwInt32			m_iMaxVisible;
	RwInt32			m_iValue;

	gui::CDialog*	m_pDialog;
	gui::CSlot		m_slotPaint;

	RwBool			m_bUpdate;
	RwBool			m_bLocate;
    RwInt32         m_nFlag;

public:
	CButtonList();
	virtual ~CButtonList();
	
	RwBool	Create(gui::CComponent* pParentComponent, _TClass* pOwnerClass, PCBCREATEITEM pCreateItem,
					PCBDESTROYITEM pDestroyItem, PCBCLICKEDITEM pClickedItem, RwInt32 iLeft, RwInt32 iTop,
					RwInt32 iMarginHeight, RwInt32 iMaxVisible, RwInt32 nFlag = NULL);	
	void	Destroy();
	void	Show(bool bShow);

	void	SetValue(RwInt32 iValue);
    RwInt32 GetValue() {return m_iValue;}
	void	SetMaxVisible(RwInt32 iMaxVisible);
	RwInt32	GetMaxVisible()			{ return m_iMaxVisible; }
    RwInt32 GetSize();
    gui::CButton* GetButton(RwInt32 nIndex);

	void	OnPaint();
	void	OnButtonItemClicked(gui::CComponent* pComponent);
    void    OnButtonToggled(gui::CComponent* pComponent, bool bToggle);

	void	AddItem(const std::wstring& strText, const _TData& tData);
	void	ClearItem();
	SItem	GetItem(RwInt32 iValue)	{ return *m_vecItem.at(iValue); }

    RwInt32 GetSelect();                            ///< 현재 선택된 버튼의 인덱스를 반환한다. 선택된게 없으면 -1을 반환. Select 상태에서 사용한다.

	void	LocateItem();
	void	UpdateItem();
};

template<typename _TClass, typename _TData>
CButtonList<_TClass, _TData>::CButtonList() : m_pOwnerClass(NULL), m_pCreateItem(NULL), m_pDestroyItem(NULL), m_pClickedItem(NULL),
m_iLeft(0), m_iTop(0), m_iMarginHeight(0), m_iMaxVisible(0), m_iValue(0), m_pDialog(NULL), m_bLocate(FALSE), m_bUpdate(FALSE), m_nFlag(NULL)
{
}

template<typename _TClass, typename _TData>
CButtonList<_TClass, _TData>::~CButtonList()
{
}

template<typename _TClass, typename _TData>
RwBool CButtonList<_TClass, _TData>::Create(gui::CComponent* pParentComponent, _TClass* pOwnerClass, PCBCREATEITEM pCreateItem, PCBDESTROYITEM pDestroyItem, PCBCLICKEDITEM pClickedItem, RwInt32 iLeft, RwInt32 iTop, RwInt32 iMarginHeight, RwInt32 iMaxVisible, RwInt32 nFlag /* = NULL */)
{
	m_pOwnerClass	= pOwnerClass;
	m_pCreateItem	= pCreateItem;
	m_pDestroyItem	= pDestroyItem;
	m_pClickedItem	= pClickedItem;

	m_iLeft			= iLeft;
	m_iTop			= iTop;
	m_iMarginHeight	= iMarginHeight;
    m_nFlag         = nFlag;

	m_pDialog		= NTL_NEW gui::CDialog(pParentComponent, GetNtlGuiManager()->GetSurfaceManager());
	m_slotPaint		= m_pDialog->SigPaint().Connect(this, &CButtonList<_TClass, _TData>::OnPaint);

	SetMaxVisible(iMaxVisible);
	LocateItem();
	UpdateItem();
	return TRUE;
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::Destroy()
{
	for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
	{
		SButtonItem* pButtonItem = m_vecButtonItem.at(i);
		if (pButtonItem)
		{
			(m_pOwnerClass->*m_pDestroyItem)(pButtonItem->pButton);
			NTL_DELETE(pButtonItem);
		}
	}
	m_vecButtonItem.clear();

	for (int i = 0; i < (int)m_vecItem.size(); ++i)
	{
		SItem* pItem = m_vecItem.at(i);
		if (pItem)
		{
			NTL_DELETE(pItem);
		}
	}
	m_vecItem.clear();

	if (m_pDialog)
	{
		NTL_DELETE(m_pDialog);
		m_pDialog = NULL;
	}
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::SetValue(RwInt32 iValue)
{
	m_iValue = iValue;
	m_bUpdate = TRUE;
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::SetMaxVisible(RwInt32 iMaxVisible)
{
	while ((RwInt32)m_vecButtonItem.size() != iMaxVisible)
	{
		if ((RwInt32)m_vecButtonItem.size() < iMaxVisible)
		{
			SButtonItem* pButtonItem	= NTL_NEW SButtonItem;
			pButtonItem->pButton		= (m_pOwnerClass->*m_pCreateItem)();
			pButtonItem->slotClicked	= pButtonItem->pButton->SigClicked().Connect(this, &CButtonList<_TClass, _TData>::OnButtonItemClicked);            
            pButtonItem->slotToggled    = pButtonItem->pButton->SigToggled().Connect(this, &CButtonList<_TClass, _TData>::OnButtonToggled);            
			pButtonItem->pItem			= NULL;

			pButtonItem->pButton->SetParent(m_pDialog, true);
			m_vecButtonItem.push_back(pButtonItem);
		}
		else
		{
			SButtonItem* pButtonItem = m_vecButtonItem.back();
			if (pButtonItem)
			{
				(m_pOwnerClass->*m_pDestroyItem)(pButtonItem->pButton);
				NTL_DELETE(pButtonItem);
			}				
			m_vecButtonItem.pop_back();
		}
	}

	m_iMaxVisible = (RwInt32)m_vecButtonItem.size();
	m_bLocate = TRUE;
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::OnPaint()
{
	if (m_bLocate)
	{
		LocateItem();
	}
	if (m_bUpdate)
	{
		UpdateItem();
	}
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::OnButtonItemClicked(gui::CComponent* pComponent)
{
	for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
	{
		if (pComponent == m_vecButtonItem.at(i)->pButton)
		{
			(m_pOwnerClass->*m_pClickedItem)(i + m_iValue);
			break;
		}
	}
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::OnButtonToggled(gui::CComponent* pComponent, bool bToggle)
{
    if(!bToggle)
        return;

    if(!(m_nFlag & BL_MULTI_SELECT))    ///< 멀티 셀렉트가 아니면 하나만 선택 가능
    {
        for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
        {        
            if (pComponent != m_vecButtonItem[i]->pButton)
            {
                m_vecButtonItem[i]->pButton->SetDown(false);
            }
        }
    }

    for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
    {
        if (pComponent == m_vecButtonItem[i]->pButton)
        {
            (m_pOwnerClass->*m_pClickedItem)(i + m_iValue);
            break;
        }
    }
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::AddItem(const std::wstring& strText, const _TData& tData)
{
	SItem* pItem	= NTL_NEW SItem;
	pItem->strText	= strText;
	pItem->TData	= tData;

	m_vecItem.push_back(pItem);
	m_bUpdate = TRUE;
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::ClearItem()
{
    for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
    {
        if (m_vecButtonItem.at(i)->pButton->IsToggleButton())
        {
            // 토글된 버튼이 있으면 모두 해제한다.
            m_vecButtonItem[i]->pButton->SetDown(false);
        }
    }

	for (int i = 0; i < (int)m_vecItem.size(); ++i)
	{
		SItem* pItem = m_vecItem.at(i);
		if (pItem)
		{
			NTL_DELETE(pItem);
		}
	}
	m_vecItem.clear();
	m_bUpdate = TRUE;
    SetValue(0);
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::Show(bool bShow)
{
	m_pDialog->Show(bShow);
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::LocateItem()
{
	RwInt32		iLeft		= 0;
	RwInt32		iTop		= 0;
	RwInt32		iMaxWidth	= 0;
	for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
	{	
		gui::CButton* pButton = m_vecButtonItem.at(i)->pButton;

		pButton->SetPosition(iLeft, iTop);

		if (iMaxWidth < pButton->GetWidth())
		{
			iMaxWidth = pButton->GetWidth();
		}
		iTop += pButton->GetHeight() + m_iMarginHeight;
	}

	m_pDialog->SetPosition(CRectangle(m_iLeft, m_iTop, m_iLeft + iMaxWidth, m_iTop + iTop - m_iMarginHeight));
	m_bLocate = FALSE;
}

template<typename _TClass, typename _TData>
void CButtonList<_TClass, _TData>::UpdateItem()
{
	RwInt32 iValue = m_iValue;
	for (int i = 0; i < (int)m_vecButtonItem.size(); ++i)
	{
		SButtonItem* pButtonItem = m_vecButtonItem.at(i);

		if (iValue < (RwInt32)m_vecItem.size())
		{
			pButtonItem->pItem = m_vecItem.at(iValue++);
			pButtonItem->pButton->SetText(pButtonItem->pItem->strText);
			pButtonItem->pButton->Show(true);
			pButtonItem->pButton->Enable(true);
		}
		else
		{
			pButtonItem->pItem = NULL;
			pButtonItem->pButton->Show(false);
			pButtonItem->pButton->Enable(false);
		}			
	}
	m_bUpdate = FALSE;
}

template<typename _TClass, typename _TData>
RwInt32 CButtonList<_TClass, _TData>::GetSize()
{
    return (RwInt32)m_vecItem.size();
}

template<typename _TClass, typename _TData>
gui::CButton* CButtonList<_TClass, _TData>::GetButton(RwInt32 nIndex)
{
    NTL_ASSERT(nIndex < (RwInt32)m_vecButtonItem.size(), "Fail");
    if(nIndex >= (RwInt32)m_vecButtonItem.size())
        return NULL;

    return m_vecButtonItem[nIndex]->pButton;
}

template<typename _TClass, typename _TData>
RwInt32 CButtonList<_TClass, _TData>::GetSelect()
{
    RwInt32 nRet = -1;
    for(UINT i = 0; i < m_vecButtonItem.size(); ++i)
    {
        gui::CButton* pButton = m_vecButtonItem[i]->pButton;
        if(pButton->IsToggleButton() && pButton->IsDown())
        {
            nRet = i;
            break;
        }
    }

    return nRet;
}