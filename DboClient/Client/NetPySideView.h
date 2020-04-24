#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"
#include "SideIconGui.h"
#include "DBOEvent.h"
    
/**
 * \ingroup Client
 * \brief NetPy 정보를 표시하는 Side View
 *
 * \date 2009-04-20
 * \author agebreak
 */
class CNetPySideViewGui : public CSideViewBase, public RWS::CEventHandler
{
public:
    enum ENetPySideViewHeight
    {
        NETPY_SIDEVIEW_HEIGHT_NORMAL = 60,
        NETPY_SIDEVIEW_HEIGHT_PCBANG = 140,
        NETPY_SIDEVIEW_HEIGHT_NFY    = 60,
    };

public:
    CNetPySideViewGui(const RwChar* pName);
    virtual ~CNetPySideViewGui(void);

    RwBool		Create();
    VOID		Destroy();
    VOID		Update(RwReal fElapsed);

    virtual VOID	OnPressESC();
    virtual VOID	OnSideViewOpen(const void* pData);
    virtual VOID	OnSideViewClose();
    virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	

protected:
    virtual VOID	HandleEvents( RWS::CMsg &msg );
    VOID			LocateComponent();
    VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
    VOID			OnResize( RwInt32 iOldW, RwInt32 iOldH );
    VOID			OnPaint();

    VOID            SetState(eNetPySideViewType type);
    VOID            OnShowInfo();           ///< 현재 NetPy 정보를 출력한다
    VOID            OnShowNotify();         ///< NetPy 습득 정보를 출력한다. 

protected:
    CWindowby3			m_BackPanel;		///< 배경
    eNetPySideViewType  m_type;             ///< 현재 보여지는 타입

    gui::CSlot			m_slotPaint;
    gui::CSlot			m_slotMove;
    gui::CSlot			m_slotResize;

    // 현재 NetPy 정보
    gui::CStaticBox*    m_pstbRemainTimeTitle;
    gui::CStaticBox*    m_pstbRemainTimeData;    
    gui::CStaticBox*    m_pstbGetTitle;
    gui::CStaticBox*    m_pstbGetData;    
    gui::CStaticBox*    m_pstbCurrentTitle;
    gui::CStaticBox*    m_pstbCurrentData;   

    // NetPy 획득 정보
    gui::CStaticBox*    m_pstbGetPoint;

    SDboEventUpdateNetPy m_UpdateNetPyInfo;                 ///< 서버로 부터 받은 NetPy 업데이트 정보
    DWORD                m_dwUpdateNetPyPoint;              ///< 획득 포인트
    RwReal               m_fRemainTime;                     ///< 다음 NetPy 획득까지의 남은 시간
    RwReal               m_fNotifyShowTime;                 ///< Notify 표시 시간    
    RwBool               m_bIsPCBang;                       ///< PC방인지를 판단한다.
};
