#pragma once

#include "NtlDebug.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"

class CTextTable;

// 말풍선을 이루는 구조체
struct SDragonDialog
{
	gui::CPanel*		m_pPanel;
	gui::CStaticBox*	m_pStatic;
	RwReal				m_fLifeTime;

	SDragonDialog()
	{
		m_pPanel = NULL;
		m_pStatic = NULL;
		m_fLifeTime = 0.0f;
	}

	void Show(RwBool bShow)
	{
		m_pPanel->Show(B2b(bShow));
		m_pStatic->Show(B2b(bShow));
	}

	void SetAlpha(RwReal fAlpha)
	{
		BYTE byAlpha = (BYTE)(fAlpha * 255.0f);
		m_pPanel->SetAlpha(byAlpha);
		m_pStatic->SetAlpha((UCHAR)byAlpha);
	}

	void SetPosition(RwInt32 x, RwInt32 y)
	{
		m_pPanel->SetPosition(x, y);
		m_pStatic->SetPosition(x, y);
	}
};

/**
* \ingroup Client
* \brief 용신의 대화(말풍선)을 표시하는 GUI 클래스
* \date 2007-07-6
* \author agebreak
*/
class CDBCDragonDialogGUI : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum EDialogType			///< 다이얼로그의 타입
	{
		E_DIALOG_NORMAL,
		E_DIALOG_WARN,

		DIALOG_COUNT,
	};

public:
	CDBCDragonDialogGUI(void);
	CDBCDragonDialogGUI(const RwChar* pName);
	virtual ~CDBCDragonDialogGUI(void);

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);	

	// Event
	VOID		HandleEvents(RWS::CMsg &pMsg);

	VOID		SetText(RwInt32 nTblIdx, EDialogType eDialogType = E_DIALOG_NORMAL);					///< 용신 대화 테이블 인덱스의 대화를 표시한다.
	VOID		SetText(std::wstring& strWString, EDialogType eDialogType = E_DIALOG_NORMAL);		///< 텍스트를 표시한다.	
	VOID		ShowNormalDialog(RwBool bShow);				///< 일반 대사창 표시 유무를 설정한다.

	static		CDBCDragonDialogGUI* GetInstance();			///< 싱글톤 Instance를 반환한다.
	static		void				 DeleteInstance();		///< 싱글톤 Instance를 삭제한다.

protected:	
	void		AdjustDialog();								///< UI의 위치를 결정한다.    

protected:
	static		CDBCDragonDialogGUI* m_pInstance;			///< 싱글톤 인스턴스

	SDragonDialog	m_SDragonDialog[DIALOG_COUNT];			///< 용신의 대화를 표시할 말풍선 구조체
	CTextTable*		m_pDragonTextTable;						///< 용신의 대화 테이블		
	RwBool			m_bDelete;
    
};	
