// peessi : SCViewer로 빌드할때는 밑의 precomp include를 주석처리하고, SCVIEWER_MODE 정의를 활성화 한다.
#include "precomp_dboclient.h"
//#define SCVIEWER_MODE

#include "SkillCustomizeParser.h"

#include "NtlDebug.h"

#include "gui_componenttokenizer.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillCustoizeLineItem

#define PRIORITY_LINE	0x7FFF
#define PRIORITY_SKILL	0x8000

#define UPGRADELINE_WIDTH	47
#define UPGRADELINE_HEIGHT	29

#define UPGRADELINE_ARROW	"srfUpgradeLineArrow"
#define UPGRADELINE_BAR		"srfUpgradeLineBar"
#define UPGRADELINE_ARROW_ACTIVE	"srfUpgradeLineArrowActive"
#define UPGRADELINE_BAR_ACTIVE		"srfUpgradeLineBarActive"

#define OPTIONLINE_SIZE		26
#define OPTIONLINE_HORIZONTAL	"srfOptionLineHorizontal"
#define OPTIONLINE_VERTICAL		"srfOptionLineVertical"
#define OPTIONLINE_LEFTUP		"srfOptionLineLeftUp"
#define OPTIONLINE_RIGHTUP		"srfOptionLineRightUp"
#define OPTIONLINE_LEFTDOWN		"srfOptionLineLeftDown"
#define OPTIONLINE_RIGHTDOWN	"srfOptionLineRightDown"
#define OPTIONLINE_HORIZONTAL_ACTIVE	"srfOptionLineHorizontalActive"
#define OPTIONLINE_VERTICAL_ACTIVE		"srfOptionLineVerticalActive"
#define OPTIONLINE_LEFTUP_ACTIVE		"srfOptionLineLeftUpActive"
#define OPTIONLINE_RIGHTUP_ACTIVE		"srfOptionLineRightUpActive"
#define OPTIONLINE_LEFTDOWN_ACTIVE		"srfOptionLineLeftDownActive"
#define OPTIONLINE_RIGHTDOWN_ACTIVE		"srfOptionLineRightDownActive"

#define SKILLCUSTOMIZE_SUFACEPAGE	"gui\\SkillCustomize.srf"

#define LINE_OFFSET			4		// 마스터리 스킬과, 일반스킬의 리소스 차에서 오는 크기.


CSkillCustomizeLineItem::CSkillCustomizeLineItem( gui::CComponent* pParent, RwUInt8 byLineType, RwUInt8 byBeginPos, RwUInt8 byEndPos,
												 CSkillCustomizeSkillItem* pBeginSkill, CSkillCustomizeSkillItem* pEndSkill, const std::string& strLineName, std::string& strReport )
												 : m_byLineType( byLineType )
{
	CRectangle	rtBegin = pBeginSkill->GetSkillPosition();
	CRectangle	rtEnd = pEndSkill->GetSkillPosition();

	if( byLineType == LINE_TYPE_UPGRADE )
	{
		CRectangle rtLine;
		CRectangle rtArrow;

		// Begin Down, End Up으로 고정.
		RwInt32 nXPos = rtBegin.left + ( rtEnd.right - rtBegin.left ) /  2 - UPGRADELINE_WIDTH / 2;
		rtLine.SetRectWH( nXPos, rtBegin.bottom, UPGRADELINE_WIDTH, rtEnd.top - rtBegin.bottom - UPGRADELINE_HEIGHT );
		SetLineSurface( GetLineSurface( UPGRADELINE_BAR ), GetLineSurface( UPGRADELINE_BAR_ACTIVE ), rtLine );

		rtArrow.SetRectWH( nXPos, rtEnd.top - UPGRADELINE_HEIGHT, UPGRADELINE_WIDTH, UPGRADELINE_HEIGHT );
		SetLineSurface( GetLineSurface( UPGRADELINE_ARROW ), GetLineSurface( UPGRADELINE_ARROW_ACTIVE ), rtArrow );				
	}
	else 
	{
		stLINE_PATH stOptionLineBeginPath;
		stLINE_PATH stOptionLineEndPath;

		GetLinePath( &stOptionLineBeginPath, byBeginPos, pBeginSkill, OPTIONLINE_SIZE );
		GetLinePath( &stOptionLineEndPath, byEndPos, pEndSkill, OPTIONLINE_SIZE );		

		while( TRUE )
		{
			if( CalcLinePath( &stOptionLineBeginPath, &stOptionLineEndPath, pBeginSkill, pEndSkill, OPTIONLINE_SIZE, strLineName, strReport ) )
				break;
		}
	}

	CRectangle rtLine;
	gui::CSurfaceManager* pSurfaceManager = CSkillCustomizeParser::m_pSurfaceManager;
	rtLine.SetRectWH( 0, 0, pParent->GetWidth(), GetBottomPos() );	
	m_ppnlLine = NTL_NEW gui::CPanel( rtLine, pParent, pSurfaceManager );	
	m_ppnlLine->SetClippingMode( true );
	m_ppnlLine->Enable( false );
	m_ppnlLine->SetPriority( PRIORITY_LINE );
	SetActive( FALSE );
}

CSkillCustomizeLineItem::~CSkillCustomizeLineItem(VOID)
{
	m_ppnlLine->ClearSurface();
	m_listLineSurface.clear();
	m_listActiveLineSurface.clear();

	NTL_DELETE( m_ppnlLine );
}

VOID CSkillCustomizeLineItem::Show( bool bShow )
{
	m_ppnlLine->Show( bShow );
}

VOID CSkillCustomizeLineItem::SetAlpha( RwUInt8 byAlpha )
{
	m_ppnlLine->SetAlpha( byAlpha );
}

VOID CSkillCustomizeLineItem::SetActive( RwBool bActive )
{
	LIST_SURFACE_ITER iter;

	m_ppnlLine->GetSurface()->clear();

	if( bActive )
	{
		for( iter = m_listActiveLineSurface.begin() ; iter != m_listActiveLineSurface.end() ; ++iter )
		{
			m_ppnlLine->AddSurface( (*iter) );
		}
	}
	else
	{
		for( iter = m_listLineSurface.begin() ; iter != m_listLineSurface.end() ; ++iter )
		{
			m_ppnlLine->AddSurface( (*iter) );
		}
	}
}

RwInt32 CSkillCustomizeLineItem::GetBottomPos(VOID)
{
	RwInt32 nBottomY = 0;
	LIST_SURFACE_ITER iter;

	for( iter = m_listActiveLineSurface.begin() ; iter != m_listActiveLineSurface.end() ; ++iter )
	{
		gui::CSurface& surface = (*iter);
		RwInt32 nBottomPos = surface.m_Original.rtRect.bottom;
		if( nBottomPos > nBottomY )
			nBottomY = nBottomPos;
	}

	return nBottomY;
}

gui::CSurface CSkillCustomizeLineItem::GetLineSurface( RwChar* surfacename )
{
	gui::CSurfaceManager* pSurfaceManager = CSkillCustomizeParser::m_pSurfaceManager;
	return pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, surfacename );
}

VOID CSkillCustomizeLineItem::SetLineSurface( gui::CSurface& Surface, gui::CSurface& ActiveSurface, CRectangle& rect )
{
	gui::CSurface surDeactive;
	gui::CSurface surActive;

	surDeactive = Surface;
	surActive = ActiveSurface;

	surDeactive.m_SnapShot.rtRect = rect;
	surDeactive.m_Original.rtRect = rect;
	surActive.m_SnapShot.rtRect = rect;
	surActive.m_Original.rtRect = rect;

	m_listLineSurface.push_back( surDeactive );
	m_listActiveLineSurface.push_back( surActive );
}

VOID CSkillCustomizeLineItem::GetLinePath( stLINE_PATH* pLinePath, RwUInt8 byLinePos, CSkillCustomizeSkillItem* pSkill, RwUInt8 byLineSize )
{
	CRectangle rect = pSkill->GetSkillPosition();

	// begin 값 지정.
	pLinePath->byDirection = byLinePos;

	switch( byLinePos )
	{
	case LINE_POS_LEFT:
		pLinePath->nX = rect.left;
		pLinePath->nY = rect.top + ( rect.GetHeight() - byLineSize ) / 2;
		break;
	case LINE_POS_RIGHT:
		pLinePath->nX = rect.right;
		pLinePath->nY = rect.top + ( rect.GetHeight() - byLineSize ) / 2;
		break;
	case LINE_POS_UP:
		pLinePath->nX = rect.left + ( rect.GetWidth() - byLineSize ) / 2;
		pLinePath->nY = rect.top;
		break;
	case LINE_POS_DOWN:
		pLinePath->nX = rect.left + ( rect.GetWidth() - byLineSize ) / 2;
		pLinePath->nY = rect.bottom;
		break;
	}
}

RwBool CSkillCustomizeLineItem::CalcLinePath( stLINE_PATH* pCurrentPath, stLINE_PATH* pDestPath, CSkillCustomizeSkillItem* pBeginSkill, CSkillCustomizeSkillItem* pEndSkill, RwUInt8 byLineSize, const std::string& strLineName, std::string& strReport )
{
	RwBool bEnd = FALSE;
	gui::CSurface surface;
	CRectangle rtLine;	

	// 방향 결정.
	switch( pCurrentPath->byDirection )
	{
	case LINE_POS_LEFT:
		switch( pDestPath->byDirection )
		{
		case LINE_POS_RIGHT:			
			if( pDestPath->nX <= pCurrentPath->nX )
			{
				if( abs( pDestPath->nY - pCurrentPath->nY ) < byLineSize )
				{
					if( pDestPath->nX != pCurrentPath->nX )
					{
						rtLine.SetRect( pDestPath->nX, pCurrentPath->nY, pCurrentPath->nX, pCurrentPath->nY + byLineSize );
						SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					}

					bEnd = TRUE;					
				}
				else
				{
					if( pCurrentPath->nX > pDestPath->nX + byLineSize )
					{
						rtLine.SetRect( pDestPath->nX + byLineSize, pCurrentPath->nY, pCurrentPath->nX, pCurrentPath->nY + byLineSize );
						SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					}

					rtLine.SetRectWH( pDestPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					if( pDestPath->nY - pCurrentPath->nY > 0 )
					{// Dest가 더 밑에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTDOWN ), GetLineSurface( OPTIONLINE_RIGHTDOWN_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_DOWN, pDestPath->nX, pCurrentPath->nY + byLineSize );
					}
					else
					{// Dest가 더 위에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTUP ), GetLineSurface( OPTIONLINE_RIGHTUP_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_UP, pDestPath->nX, pCurrentPath->nY );
					}
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}			
			break;
		case LINE_POS_UP:
			if( pDestPath->nY - byLineSize >= pCurrentPath->nY )
			{
				if( pDestPath->nX + byLineSize < pCurrentPath->nX )
				{
					rtLine.SetRect( pDestPath->nX + byLineSize, pCurrentPath->nY, pCurrentPath->nX, pCurrentPath->nY + byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pDestPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTDOWN ), GetLineSurface( OPTIONLINE_RIGHTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_DOWN, pDestPath->nX, pCurrentPath->nY + byLineSize );
				}
				else
				{
					rtLine.SetRect( pCurrentPath->nX - byLineSize, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTDOWN ), GetLineSurface( OPTIONLINE_RIGHTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_DOWN, pCurrentPath->nX - byLineSize, pCurrentPath->nY + byLineSize );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );
			}		
			break;
		case LINE_POS_DOWN:
			if( pDestPath->nY <= pCurrentPath->nY )
			{
				if( pDestPath->nX + byLineSize < pCurrentPath->nX )
				{
					rtLine.SetRect( pDestPath->nX + byLineSize, pCurrentPath->nY, pCurrentPath->nX, pCurrentPath->nY + byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pDestPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTUP ), GetLineSurface( OPTIONLINE_RIGHTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_UP, pDestPath->nX, pCurrentPath->nY );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX - byLineSize, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTUP ), GetLineSurface( OPTIONLINE_RIGHTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_UP, pCurrentPath->nX - byLineSize, pCurrentPath->nY );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );
			}
			break;
		case LINE_POS_LEFT:
			// 역위치
			bEnd = TRUE;
			LineError( strLineName, strReport );
			break;
		}
		break;
	case LINE_POS_RIGHT:
		switch( pDestPath->byDirection )
		{
		case LINE_POS_LEFT:
			if( pDestPath->nX >= pCurrentPath->nX )
			{
				if( abs( pDestPath->nY - pCurrentPath->nY ) < byLineSize )
				{
					if( pDestPath->nX != pCurrentPath->nX )
					{
						rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pDestPath->nX, pCurrentPath->nY + byLineSize );
						SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					}

					bEnd = TRUE;
				}
				else
				{
					if( pDestPath->nX > pCurrentPath->nX + byLineSize )
					{
						rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pDestPath->nX - byLineSize, pCurrentPath->nY + byLineSize );
						SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					}					

					rtLine.SetRectWH( pDestPath->nX - byLineSize, pCurrentPath->nY, byLineSize, byLineSize );
					if( pDestPath->nY - pCurrentPath->nY > 0 )
					{// Dest가 더 밑에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_LEFTDOWN ), GetLineSurface( OPTIONLINE_LEFTDOWN_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_DOWN, pDestPath->nX - byLineSize, pCurrentPath->nY + byLineSize );
					}
					else
					{// Dest가 더 위에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_LEFTUP ), GetLineSurface( OPTIONLINE_LEFTUP_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_UP, pDestPath->nX - byLineSize, pCurrentPath->nY );
					}
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );
			}
			break;
		case LINE_POS_UP:
			if( pDestPath->nY - byLineSize >= pCurrentPath->nY )			
			{
				if( pCurrentPath->nX + byLineSize < pDestPath->nX )
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pDestPath->nX, pCurrentPath->nY + byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pDestPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTDOWN ), GetLineSurface( OPTIONLINE_LEFTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_DOWN, pDestPath->nX, pCurrentPath->nY + byLineSize );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTDOWN ), GetLineSurface( OPTIONLINE_LEFTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_DOWN, pCurrentPath->nX, pCurrentPath->nY + byLineSize );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );
			}
			break;
		case LINE_POS_DOWN:
			if( pDestPath->nY <= pCurrentPath->nY )
			{
				if( pCurrentPath->nX + byLineSize < pDestPath->nX )
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pDestPath->nX, pCurrentPath->nY + byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_HORIZONTAL ), GetLineSurface( OPTIONLINE_HORIZONTAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pDestPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTUP ), GetLineSurface( OPTIONLINE_LEFTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_UP, pDestPath->nX, pCurrentPath->nY );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTUP ), GetLineSurface( OPTIONLINE_LEFTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_UP, pCurrentPath->nX, pCurrentPath->nY );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;					 
				LineError( strLineName, strReport );
			}
			break;
		case LINE_POS_RIGHT:
			// 역위치
			bEnd = TRUE;
			LineError( strLineName, strReport );
			break;
		}
		break;
	case LINE_POS_UP:
		switch( pDestPath->byDirection )
		{
		case LINE_POS_DOWN:		
			if( pDestPath->nY <= pCurrentPath->nY )
			{
				if( abs( pDestPath->nX - pCurrentPath->nX ) < byLineSize )
				{
					if( pDestPath->nY != pCurrentPath->nY )
					{
						rtLine.SetRect( pCurrentPath->nX, pDestPath->nY, pCurrentPath->nX + byLineSize, pCurrentPath->nY );
						SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					}

					bEnd = TRUE;
				}
				else
				{
					if( pCurrentPath->nY > pDestPath->nX + byLineSize )
					{
						rtLine.SetRect( pCurrentPath->nX, pDestPath->nY + byLineSize, pCurrentPath->nX + byLineSize, pCurrentPath->nY );
						SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					}

					rtLine.SetRectWH( pCurrentPath->nX, pDestPath->nY, byLineSize, byLineSize );
					if( pDestPath->nX - pCurrentPath->nX > 0 )
					{// Dest가 더 오른쪽에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTDOWN ), GetLineSurface( OPTIONLINE_RIGHTDOWN_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX + byLineSize, pDestPath->nY );
					}
					else
					{// Dest가 더 왼쪽에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_LEFTDOWN ), GetLineSurface( OPTIONLINE_LEFTDOWN_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_LEFT, pCurrentPath->nX, pDestPath->nY );
					}
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}
			break;
		case LINE_POS_LEFT:
			if( pDestPath->nX >= pCurrentPath->nX + byLineSize )
			{
				if( pDestPath->nY < pCurrentPath->nY - byLineSize )
				{
					rtLine.SetRect( pCurrentPath->nX, pDestPath->nY + byLineSize, pCurrentPath->nX + byLineSize, pCurrentPath->nY );
					SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pCurrentPath->nX, pDestPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTDOWN ), GetLineSurface( OPTIONLINE_RIGHTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX + byLineSize, pDestPath->nY );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY - byLineSize, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTDOWN ), GetLineSurface( OPTIONLINE_RIGHTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX + byLineSize, pCurrentPath->nY - byLineSize );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}
			break;
		case LINE_POS_RIGHT:
			if( pDestPath->nX <= pCurrentPath->nX )
			{
				if( pDestPath->nY < pCurrentPath->nY - byLineSize )
				{
					rtLine.SetRect( pCurrentPath->nX, pDestPath->nY + byLineSize, pCurrentPath->nX + byLineSize, pCurrentPath->nY );
					SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pCurrentPath->nX, pDestPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTDOWN ), GetLineSurface( OPTIONLINE_LEFTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_LEFT, pCurrentPath->nX, pDestPath->nY );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY - byLineSize, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTDOWN ), GetLineSurface( OPTIONLINE_LEFTDOWN_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX, pCurrentPath->nY - byLineSize );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}
			break;
		case LINE_POS_UP:
			// 역위치
			bEnd = TRUE;
			LineError( strLineName, strReport );				
			break;		
		}
		break;
	case LINE_POS_DOWN:
		switch( pDestPath->byDirection )
		{
		case LINE_POS_UP:
			if( pDestPath->nY >= pCurrentPath->nY )
			{
				if( abs( pDestPath->nX - pCurrentPath->nX ) < byLineSize )
				{
					if( pDestPath->nY != pCurrentPath->nY )
					{
						rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pCurrentPath->nX + byLineSize, pDestPath->nY );
						SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					}

					bEnd = TRUE;
				}
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pCurrentPath->nX + byLineSize, pDestPath->nY - byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );

					rtLine.SetRectWH( pCurrentPath->nX, pDestPath->nY - byLineSize, byLineSize, byLineSize );
					if( pDestPath->nX - pCurrentPath->nX > 0 )
					{// Dest가 더 오른쪽에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTUP ), GetLineSurface( OPTIONLINE_RIGHTUP_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX + byLineSize, pDestPath->nY - byLineSize );
					}
					else
					{// Dest가 더 왼쪽에 있다.
						SetLineSurface( GetLineSurface( OPTIONLINE_LEFTUP ), GetLineSurface( OPTIONLINE_LEFTUP_ACTIVE ), rtLine );
						pCurrentPath->SetLinePath( LINE_POS_LEFT, pCurrentPath->nX, pDestPath->nY - byLineSize );
					}
				}
			}
			else
			{
				// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}
			break;
		case LINE_POS_LEFT:
			if( pDestPath->nX >= pCurrentPath->nX + byLineSize )
			{
				if( pDestPath->nY > pCurrentPath->nY )
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pCurrentPath->nX + byLineSize, pDestPath->nY );
					SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pCurrentPath->nX, pDestPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTUP ), GetLineSurface( OPTIONLINE_RIGHTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX + byLineSize, pDestPath->nY );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_RIGHTUP ), GetLineSurface( OPTIONLINE_RIGHTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_RIGHT, pCurrentPath->nX + byLineSize, pCurrentPath->nY );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}
			break;
		case LINE_POS_RIGHT:
			if( pDestPath->nX <= pCurrentPath->nX )
			{
				if( pDestPath->nY > pCurrentPath->nY )
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, pCurrentPath->nX + byLineSize, pDestPath->nY );
					SetLineSurface( GetLineSurface( OPTIONLINE_VERTICAL ), GetLineSurface( OPTIONLINE_VERTICAL_ACTIVE ), rtLine );
					rtLine.SetRectWH( pCurrentPath->nX, pDestPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTUP ), GetLineSurface( OPTIONLINE_LEFTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_LEFT, pCurrentPath->nX, pDestPath->nY );
				}				
				else
				{
					rtLine.SetRect( pCurrentPath->nX, pCurrentPath->nY, byLineSize, byLineSize );
					SetLineSurface( GetLineSurface( OPTIONLINE_LEFTUP ), GetLineSurface( OPTIONLINE_LEFTUP_ACTIVE ), rtLine );
					pCurrentPath->SetLinePath( LINE_POS_LEFT, pCurrentPath->nX, pCurrentPath->nY );
				}
			}
			else
			{// 역위치
				bEnd = TRUE;
				LineError( strLineName, strReport );				
			}
			break;		
		case LINE_POS_DOWN:
			bEnd = TRUE;
			break;
		}
		break;
	}

	return bEnd;
}

VOID CSkillCustomizeLineItem::LineError( const std::string& strLineName, std::string& strReport )
{
	strReport += "[ " + strLineName + " ] line has wrong position";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillCustoizeSkillItem
#define SKILLCUSTOMIZE_BTN_SPUSE_UP		"srfSPUseBtnUp"
#define SKILLCUSTOMIZE_BTN_SPUSE_DOWN	"srfSPUseBtnDown"
#define SKILLCUSTOMIZE_BTN_SPUSE_FOCUS	"srfSPUseBtnFocus"

#define SKILLCUSTOMIZE_BTN_RPUSE_UP		"srfRPUseBtnUp"
#define SKILLCUSTOMIZE_BTN_RPUSE_DOWN	"srfRPUseBtnDown"
#define SKILLCUSTOMIZE_BTN_RPUSE_FOCUS	"srfRPUseBtnFocus"

#define SKILLCUSTOMIZE_ICON_SIZE	32
#define SKILLCUSTOMIZE_BTN_SIZE		19

#define SKILLCUSTOMIZE_MASTERY_WIDTH		139//152
#define SKILLCUSTOMIZE_MASTERY_HEIGHT		45//49

#define SKILLCUSTOMIZE_MASTERY_OFFSET_X		4
#define SKILLCUSTOMIZE_MASTERY_OFFSET_Y		4

#define SKILLCUSTOMIZE_MASTERY_ICON_X		6//6
#define SKILLCUSTOMIZE_MASTERY_ICON_Y		7//9

#define SKILLCUSTOMIZE_MASTERY_NAME_X		40
#define SKILLCUSTOMIZE_MASTERY_NAME_Y		8//4
#define SKILLCUSTOMIZE_MASTERY_NAME_WIDTH	90
#define SKILLCUSTOMIZE_MASTERY_NAME_HEIGHT	33//37

#define SKILLCUSTOMIZE_MASTERY_SPUSE_X		113//124
#define SKILLCUSTOMIZE_MASTERY_SPUSE_Y		22//14

#define SKILLCUSTOMIZE_SKILL_WIDTH			73
#define SKILLCUSTOMIZE_SKILL_HEIGHT			55

#define SKILLCUSTOMIZE_SKILL_ICON_X			10
#define SKILLCUSTOMIZE_SKILL_ICON_Y			11

#define SKILLCUSTOMIZE_SKILL_RPUSE_X		47
#define SKILLCUSTOMIZE_SKILL_RPUSE_Y		7

#define SKILLCUSTOMIZE_SKILL_SPUSE_X		47
#define SKILLCUSTOMIZE_SKILL_SPUSE_Y		28

CSkillCustomizeSkillItem::CSkillCustomizeSkillItem( gui::CComponent* pParent, RwUInt8 bySkillType, sTBLDAT* pTblData, RwBool bMastery, RwInt32 nX, RwInt32 nY, gui::CSurfaceManager* pSurfaceManager, std::string& strReport )
: m_bySkillType( bySkillType ), m_pTblData( pTblData ), m_pstbMasteryName( NULL ), m_ppnlBack( NULL ), m_ppnlIcon( NULL ), m_pbtnSPUse( NULL ), m_pbtnRPUse( NULL ), m_bIconTextureCreated( FALSE ), m_bMastery( bMastery ), m_uiBaseSkillIndex( pTblData->tblidx )
{
	RwBool bRPUseBtn = FALSE;
	RwBool bSPUseBtn = FALSE;

	gui::CSurface surIcon;
	surIcon.m_Original.rtRect.bottom = SKILLCUSTOMIZE_ICON_SIZE;
	surIcon.m_Original.rtRect.right = SKILLCUSTOMIZE_ICON_SIZE;
	surIcon.m_SnapShot.rtRect.bottom = SKILLCUSTOMIZE_ICON_SIZE;
	surIcon.m_SnapShot.rtRect.right = SKILLCUSTOMIZE_ICON_SIZE;	

	CRectangle rtPos;
	CRectangle rtIconPos;
	CRectangle rtSPUse;
	CRectangle rtRPUse;

	if( bMastery )
	{
		rtPos.SetRectWH( nX + SKILLCUSTOMIZE_MASTERY_OFFSET_X, nY + SKILLCUSTOMIZE_MASTERY_OFFSET_Y, SKILLCUSTOMIZE_MASTERY_WIDTH, SKILLCUSTOMIZE_MASTERY_HEIGHT );
		rtIconPos.SetRectWH( SKILLCUSTOMIZE_MASTERY_ICON_X, SKILLCUSTOMIZE_MASTERY_ICON_Y, SKILLCUSTOMIZE_ICON_SIZE, SKILLCUSTOMIZE_ICON_SIZE );
		rtSPUse.SetRectWH( SKILLCUSTOMIZE_MASTERY_SPUSE_X, SKILLCUSTOMIZE_MASTERY_SPUSE_Y, SKILLCUSTOMIZE_BTN_SIZE, SKILLCUSTOMIZE_BTN_SIZE );
	}
	else
	{
		rtPos.SetRectWH( nX, nY, SKILLCUSTOMIZE_SKILL_WIDTH, SKILLCUSTOMIZE_SKILL_HEIGHT );
		rtIconPos.SetRectWH( SKILLCUSTOMIZE_SKILL_ICON_X, SKILLCUSTOMIZE_SKILL_ICON_Y, SKILLCUSTOMIZE_ICON_SIZE, SKILLCUSTOMIZE_ICON_SIZE );
		rtSPUse.SetRectWH( SKILLCUSTOMIZE_SKILL_SPUSE_X, SKILLCUSTOMIZE_SKILL_SPUSE_Y, SKILLCUSTOMIZE_BTN_SIZE, SKILLCUSTOMIZE_BTN_SIZE );		
		rtRPUse.SetRectWH( SKILLCUSTOMIZE_SKILL_RPUSE_X, SKILLCUSTOMIZE_SKILL_RPUSE_Y, SKILLCUSTOMIZE_BTN_SIZE, SKILLCUSTOMIZE_BTN_SIZE );
	}

	m_ppnlBack = NTL_NEW gui::CPanel( rtPos, pParent, pSurfaceManager );
	m_ppnlBack->SetClippingMode( true );
	m_ppnlBack->SetPriority( PRIORITY_SKILL );
	m_ppnlIcon = NTL_NEW gui::CPanel( rtIconPos, m_ppnlBack, pSurfaceManager, surIcon );	
	m_ppnlIcon->SetClippingMode( true );

	if( bMastery )
	{
		CRectangle rtMasteryName;
		rtMasteryName.SetRectWH( SKILLCUSTOMIZE_MASTERY_NAME_X, SKILLCUSTOMIZE_MASTERY_NAME_Y, SKILLCUSTOMIZE_MASTERY_NAME_WIDTH, SKILLCUSTOMIZE_MASTERY_NAME_HEIGHT );
		m_pstbMasteryName = NTL_NEW gui::CStaticBox( rtMasteryName, m_ppnlBack, pSurfaceManager, COMP_TEXT_UP );
		m_pstbMasteryName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
		m_pstbMasteryName->SetClippingMode( true );
	}

	SetActive( FALSE, &strReport, &bSPUseBtn, &bRPUseBtn );
	SetInitSkill();

	if( bSPUseBtn )
	{
		m_pbtnSPUse = NTL_NEW gui::CButton( rtSPUse, m_ppnlBack, pSurfaceManager );
		m_pbtnSPUse->SetClippingMode( true );
		m_pbtnSPUse->AddSurfaceUp( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_SPUSE_UP ) );
		m_pbtnSPUse->AddSurfaceDown( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_SPUSE_DOWN ) );
		m_pbtnSPUse->AddSurfaceFocus( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_SPUSE_FOCUS ) );
		m_pbtnSPUse->AddSurfaceDisabled( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_SPUSE_DOWN ) );
	}

	if( bRPUseBtn )
	{
		m_pbtnRPUse = NTL_NEW gui::CButton( rtRPUse, m_ppnlBack, pSurfaceManager );
		m_pbtnRPUse->SetClippingMode( true );
		m_pbtnRPUse->AddSurfaceUp( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_RPUSE_UP ) );
		m_pbtnRPUse->AddSurfaceDown( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_RPUSE_DOWN ) );
		m_pbtnRPUse->AddSurfaceFocus( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_RPUSE_FOCUS ) );
		m_pbtnRPUse->AddSurfaceDisabled( pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, SKILLCUSTOMIZE_BTN_RPUSE_DOWN ) );
	}

#ifdef SCVIEWER_MODE
	std::wstring wstrName;

	switch( bySkillType )
	{
	case SKILL:
		{
			sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pTblData );	
			CSkillCustomizeParser::m_pTextTable->GetSkillTbl()->GetText( pSkillData->Skill_Name, &wstrName );
		}
		break;
	case HTB:
		{
			sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pTblData );
			CSkillCustomizeParser::m_pTextTable->GetHTBSetTbl()->GetText( pHTBData->HTB_Skill_Name, &wstrName );
		}
		break;
	case ACTION:
		{
			sACTION_TBLDAT* pActionData = reinterpret_cast<sACTION_TBLDAT*>( pTblData );
			CSkillCustomizeParser::m_pTextTable->GetActionTbl()->GetText( pActionData->Action_Name, &wstrName );
		}
		break;
	}
	m_ppnlIcon->SetToolTip( wstrName );
#endif
}

CSkillCustomizeSkillItem::~CSkillCustomizeSkillItem(VOID)
{
	LIST_LINEITEM_ITER iter;

	for( iter = m_listLineItem.begin() ; iter != m_listLineItem.end() ; ++iter )
	{
		CSkillCustomizeLineItem* pLineItem = (*iter);
		NTL_DELETE( pLineItem );
	}

	m_listLineItem.clear();	

	UnsetIconTexture();

	NTL_DELETE( m_pstbMasteryName );
	NTL_DELETE( m_pbtnRPUse );	
	NTL_DELETE( m_pbtnSPUse );
	NTL_DELETE( m_ppnlIcon );
	NTL_DELETE( m_ppnlBack );
}

VOID CSkillCustomizeSkillItem::Show( bool bShow )
{
	m_ppnlBack->Show( bShow );
}

VOID CSkillCustomizeSkillItem::SetAlpha( RwUInt8 byAlpha )
{
	m_ppnlBack->SetAlpha( byAlpha );
}

VOID CSkillCustomizeSkillItem::SetColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue )
{
	m_ppnlBack->SetColor( byRed, byGreen, byBlue );
}

VOID CSkillCustomizeSkillItem::SetActive( RwBool bActive, std::string* pstrReport /* = NULL */, RwBool* pbSPUseBtn /* = NULL */, RwBool* pbRPUseBtn /* = NULL  */ )
{
	const RwChar* szSurfaceNameArray[] = {	"srfBlessSkillBack",	"srfBlessSkillLearnBack",
		"srfCurseSkillBack",	"srfCurseSkillLearnBack",
		"srfPhysicalSkillBack",	"srfPhysicalSkillLearnBack",
		"srfEnergySkillBack",	"srfEnergySkillLearnBack",
		"srfHTBSkillBack",		"srfHTBSkillLearnBack",
		"srfPassiveSkillBack",	"srfPassiveSkillLearnBack",
		"srfMasterySkillBack",	"srfMasterySkillLearnBack",
		"srfEtcSkillBack",		"srfEtcSkillLearnBack" };

	enum SURFACENAME {	SKILLCUSTOMIZE_SURFACE_BLESS = 0,	
		SKILLCUSTOMIZE_SURFACE_CURSE = 2,	
		SKILLCUSTOMIZE_SURFACE_PHYSICAL = 4,
		SKILLCUSTOMIZE_SURFACE_ENERGY = 6,
		SKILLCUSTOMIZE_SURFACE_HTB = 8,		
		SKILLCUSTOMIZE_SURFACE_PASSIVE = 10,
		SKILLCUSTOMIZE_SURFACE_MASTERY = 12,
		SKILLCUSTOMIZE_SURFACE_ETC = 14 };

	RwChar buf[256];
	gui::CSurface surBack;
	gui::CSurfaceManager* pSurfaceManager = CSkillCustomizeParser::m_pSurfaceManager;

	switch( m_bySkillType )
	{
	case SKILL :
		{
			sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( m_pTblData );	

			if( m_bMastery )
				surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_MASTERY + bActive] );
			else
			{
				if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_PASSIVE )
					surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_PASSIVE + bActive] );
				else if( pSkillData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
				{
					if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_PHYSICAL )
						surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_PHYSICAL + bActive] );
					else if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_ENERGY )
						surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_ENERGY + bActive] );
					else if( pSkillData->bySkill_Type == NTL_SKILL_TYPE_STATE )
					{
						if( Dbo_IsForBlessBuff( pSkillData->bySkill_Active_Type ) )
							surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_BLESS + bActive] );
						else if( Dbo_IsForCurseBuff( pSkillData->bySkill_Active_Type ) )
							surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_CURSE + bActive] );						
						else
							surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_ETC + bActive] );						
					}
					else
					{
						if( pstrReport )
						{
							sprintf_s( buf, "!!! Mismatched Skill Type : [%u] SkillIdx !!!\\n", pSkillData->tblidx );
							*pstrReport += buf;
						}						
					}

					if( pbRPUseBtn )
						*pbRPUseBtn = TRUE;

				}
				else
				{
					if( pstrReport )
					{
						sprintf_s( buf, "!!!Mismatched Skill Class : [%u] SkillIdx !!!\\n", pSkillData->tblidx );
						*pstrReport += buf;
					}					
				}
			}	

			if( pbSPUseBtn )
			{
				if( pSkillData->dwNextSkillTblidx != INVALID_TBLIDX ||
					pSkillData->bSelfTrain )
					*pbSPUseBtn = TRUE;
			}
		}
		break;
	case HTB:
		{
			//sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( m_pTblData );
			surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_HTB + bActive] );
		}
		break;
	case ACTION:
		{
			//sACTION_TBLDAT* pActionData = reinterpret_cast<sACTION_TBLDAT*>( m_pTblData );
			surBack = pSurfaceManager->GetSurface( SKILLCUSTOMIZE_SUFACEPAGE, szSurfaceNameArray[SKILLCUSTOMIZE_SURFACE_PASSIVE + bActive] );
		}
		break;
	default:
		{
			if( pstrReport )
			{
				sprintf_s( buf, "!!! Invalid Skill Type : [%u] Idx !!!", m_pTblData->tblidx );
				*pstrReport += buf;
			}			
		}
	}

	m_ppnlBack->GetSurface()->clear();
	m_ppnlBack->AddSurface( surBack );
}

VOID CSkillCustomizeSkillItem::SetLineShow( bool bShow, RwUInt8 byLineType )
{
	LIST_LINEITEM_ITER iter;

	for( iter = m_listLineItem.begin() ; iter != m_listLineItem.end() ; ++iter )
	{
		CSkillCustomizeLineItem* pLine = (*iter);

		if( pLine->GetLineType() == byLineType )
			pLine->Show( bShow );
	}
}

VOID CSkillCustomizeSkillItem::SetLineAlpha( RwUInt8 byAlpha, RwUInt8 byLineType )
{
	LIST_LINEITEM_ITER iter;

	for( iter = m_listLineItem.begin() ; iter != m_listLineItem.end() ; ++iter )
	{
		CSkillCustomizeLineItem* pLine = (*iter);

		if( pLine->GetLineType() == byLineType )
			pLine->SetAlpha( byAlpha );
	}
}

VOID CSkillCustomizeSkillItem::SetLineActive( RwBool bActive, RwUInt8 byLineType )
{
	LIST_LINEITEM_ITER iter;

	for( iter = m_listLineItem.begin() ; iter != m_listLineItem.end() ; ++iter )
	{
		CSkillCustomizeLineItem* pLine = (*iter);

		if( pLine->GetLineType() == byLineType )
			pLine->SetActive( bActive );
	}
}

VOID CSkillCustomizeSkillItem::SetInitSkill(VOID)
{
	std::string strIconName;

	switch( GetSkillType() )
	{
	case SKILL: 
		{
			sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( CSkillCustomizeParser::m_pSkillTable->FindData( m_uiBaseSkillIndex ) );
			strIconName = pSkillData->szIcon_Name;
			SetSkillData( pSkillData );
		}
		break;
	case HTB:
		{
			sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( CSkillCustomizeParser::m_pHTBTable->FindData( m_uiBaseSkillIndex ) );
			strIconName = pHTBData->szIcon_Name;
			SetSkillData( pHTBData );
		}		
		break;
	case ACTION:
		{
			sACTION_TBLDAT* pActionData = reinterpret_cast<sACTION_TBLDAT*>( CSkillCustomizeParser::m_pActionTable->FindData( m_uiBaseSkillIndex ) );
			strIconName = pActionData->szIcon_Name;
			SetSkillData( pActionData );
		}		
		break;
	default:
		NTL_ASSERTFAIL( "CSkillCustomizeSkillItem::SetInitSkill : Invalid Skill Type" );
		return;
	}

	SetIconTexture( strIconName.c_str() );
}

VOID CSkillCustomizeSkillItem::SetSkillData( sTBLDAT* pData )
{
	m_pTblData = pData;

	if( m_bMastery && m_pstbMasteryName && GetSkillType() == SKILL )
	{	
		sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pData );
		std::wstring wstrMasteryName;
		CSkillCustomizeParser::m_pTextTable->GetSkillTbl()->GetText( pSkillData->Skill_Name, &wstrMasteryName );
		m_pstbMasteryName->SetText( wstrMasteryName.c_str() );
	}
}

VOID CSkillCustomizeSkillItem::SetLine( CSkillCustomizeLineItem* pLineItem )
{
	m_listLineItem.push_back( pLineItem );
}

VOID CSkillCustomizeSkillItem::SetIconTexture( gui::CTexture* pTexture )
{
	std::list<gui::CSurface>::iterator iter_surface;
	std::list<gui::CSurface>* surList = m_ppnlIcon->GetSurface();

	for( iter_surface = surList->begin() ; iter_surface != surList->end() ; ++iter_surface )
	{
		gui::CSurface* pSurface = &(*iter_surface);

		if( m_bIconTextureCreated )
		{
			DeleteTexture( &pSurface->m_pTexture, &m_bIconTextureCreated );
			pSurface->m_pTexture = pTexture;		
		}
		else
			pSurface->m_pTexture = pTexture;		
	}
}

VOID CSkillCustomizeSkillItem::SetIconTexture( const RwChar* szFileName )
{
	std::list<gui::CSurface>::iterator iter_surface;
	std::list<gui::CSurface>* surList = m_ppnlIcon->GetSurface();

	for( iter_surface = surList->begin() ; iter_surface != surList->end() ; ++iter_surface )
	{
		gui::CSurface* pSurface = &(*iter_surface);

		if( m_bIconTextureCreated )
		{
			DeleteTexture( &pSurface->m_pTexture, &m_bIconTextureCreated );
			pSurface->m_pTexture = CreateTexture( szFileName, &m_bIconTextureCreated );
		}
		else
			pSurface->m_pTexture = CreateTexture( szFileName, &m_bIconTextureCreated );
	}
}

VOID CSkillCustomizeSkillItem::UnsetIconTexture(VOID)
{
	std::list<gui::CSurface>::iterator iter_surface;
	std::list<gui::CSurface>* surList = m_ppnlIcon->GetSurface();

	for( iter_surface = surList->begin() ; iter_surface != surList->end() ; ++iter_surface )
	{
		gui::CSurface* pSurface = &(*iter_surface);

		if( m_bIconTextureCreated )
			DeleteTexture( &pSurface->m_pTexture, &m_bIconTextureCreated );
		else
			pSurface->m_pTexture = NULL;
	}
}

gui::CTexture* CSkillCustomizeSkillItem::CreateTexture( const RwChar* szFileName, RwBool* bCreateFlag )
{
	std::string buf = "Icon\\";
	buf += szFileName;

	gui::CTexture* pImage = NULL;
	pImage = g_TextureMgr.CreateTexture( buf.c_str() );

	if( !pImage )
		pImage = g_TextureMgr.CreateTexture( "icon\\DEF_NTL_ICO.png" );	

	if( pImage )
		*bCreateFlag = TRUE;

	return pImage;
}

VOID CSkillCustomizeSkillItem::DeleteTexture( gui::CTexture** ppTexture, RwBool* bCreateFlag )
{
	if( *ppTexture )
	{
		g_TextureMgr.DeleteTexture( *ppTexture );		
		*ppTexture = NULL;
		*bCreateFlag = FALSE;
	}
}

CRectangle CSkillCustomizeSkillItem::GetSkillPosition(VOID)
{
	CRectangle rect = m_ppnlBack->GetPosition();

	if( !m_bMastery )
	{
		rect.left += LINE_OFFSET;
		rect.top += LINE_OFFSET;
		rect.right -= LINE_OFFSET;
		rect.bottom -= LINE_OFFSET;
	}

	return rect;
}

RwInt32 CSkillCustomizeSkillItem::GetBottomPos(VOID)
{
	RwInt32 nBottomPos = GetSkillPosition().bottom;

	LIST_LINEITEM_ITER iter;

	for (iter = m_listLineItem.begin(); iter != m_listLineItem.end(); ++iter)
	{
		CSkillCustomizeLineItem* pLineItem = (*iter);
		RwInt32 nLineBottom = pLineItem->GetBottomPos();

		if (nBottomPos < nLineBottom)
			nBottomPos = nLineBottom;
	}

	return nBottomPos;
}

CRectangle CSkillCustomizeSkillItem::GetSkillScreenRect()
{
	return m_ppnlBack->GetScreenRect();
}

CRectangle* CSkillCustomizeSkillItem::GetSkillRectangle()
{
	return m_ppnlBack->GetClippingRect();
}

bool CSkillCustomizeSkillItem::IsShow(VOID)
{
	return m_ppnlBack->IsVisible();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stSKILLCUSTOMIZEPARSEDDATA

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stSKILLCUSTOMIZEPARSEDSKILL

stSKILLCUSTOMIZEPARSEDSKILL::stSKILLCUSTOMIZEPARSEDSKILL( RwUInt8 byType )
: stSKILLCUSTOMIZEPARSEDDATA( SKILL ), bySkillType( byType )
{
	nX = 0;
	nY = 0;
	bMastery = FALSE;
	tblidx = INVALID_TBLIDX;	
}

RwBool stSKILLCUSTOMIZEPARSEDSKILL::SetData( const RwChar* szDataName, const RwChar* szDataValue, std::string& strReport )
{
	RwChar bufName[256];
	RwChar bufValue[256];
	strcpy_s( bufName, 256, szDataName );
	_strlwr_s( bufName, 256 );

	if( !strcmp( bufName, "x" ) )
	{
		nX = atoi( szDataValue );
	}
	else if( !strcmp( bufName, "y" ) )
	{
		nY = atoi( szDataValue );
	}
	else if( !strcmp( bufName, "mastery" ) )
	{
		strcpy_s( bufValue, 256, szDataValue );
		_strlwr_s( bufValue, 256 );

		if( !strcmp( bufValue, "true" ) )
			bMastery = TRUE;
		else if( !strcmp( bufValue, "false" ) )
			bMastery = FALSE;
		else
		{
			strReport += "[ " + m_strName + " ] : type Mastery is Wrong";
			return FALSE;
		}
	}
	else if( !strcmp( bufName, "base_skillindex" ) )
	{
		tblidx = atoi( szDataValue );
	}
	else
	{
		strReport += "[ " + m_strName + " ] : Value Name is Wrong";
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// stSKILLCUSTOMIZEPARSEDLINE

stSKILLCUSTOMIZEPARSEDLINE::stSKILLCUSTOMIZEPARSEDLINE( RwUInt8 byType )
: stSKILLCUSTOMIZEPARSEDDATA( LINE ), byLineType ( byType )
{
	byBeginLinePos = LINE_POS_INVALID;
	byEndLinePos = LINE_POS_INVALID;	
}

RwBool stSKILLCUSTOMIZEPARSEDLINE::SetData( const RwChar* szDataName, const RwChar* szDataValue, std::string& strReport )
{
	RwChar bufName[256];
	RwChar bufValue[256];
	strcpy_s( bufName, 256, szDataName );
	_strlwr_s( bufName, 256 );

	if( !strcmp( bufName, "begin_skill" ) )
	{
		strBeginSkillName = szDataValue;
	}
	else if( !strcmp( bufName, "end_skill" ) )
	{
		strEndSkillName = szDataValue;
	}
	else if( !strcmp( bufName, "begin_attachpos" ) )
	{
		strcpy_s( bufValue, 256, szDataValue );
		_strlwr_s( bufValue, 256 );

		byBeginLinePos = GetPosition( bufValue );

		if( byBeginLinePos == INVALID_BYTE )
		{
			strReport += "[ " + m_strName + " ] : Begin LinePos is Wrong";
			return FALSE;
		}
	}
	else if( !strcmp( bufName, "end_attachpos" ) )
	{
		strcpy_s( bufValue, 256, szDataValue );
		_strlwr_s( bufValue, 256 );

		byEndLinePos = GetPosition( bufValue );

		if( byEndLinePos == INVALID_BYTE )
		{
			strReport += "[ " + m_strName + " ] : End LinePos is Wrong";
			return FALSE;
		}
	}
	else
	{
		strReport += "[ " + m_strName + " ] : Value Name is Wrong";
		return FALSE;
	}

	return TRUE;
}

RwUInt8 stSKILLCUSTOMIZEPARSEDLINE::GetPosition( const RwChar* szValue )
{
	RwChar bufValue[256];
	strcpy_s( bufValue, 256, szValue );
	_strlwr_s( bufValue, 256 );

	if( !strcmp( bufValue, "up" ) )
		return LINE_POS_UP;
	else if( !strcmp( bufValue, "down" ) )
		return LINE_POS_DOWN;
	else if( !strcmp( bufValue, "right" ) )
		return LINE_POS_RIGHT;
	else if( !strcmp( bufValue, "left" ) )
		return LINE_POS_LEFT;

	return INVALID_BYTE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSkillCustoizeParser

CSkillTable* CSkillCustomizeParser::m_pSkillTable = NULL;
CHTBSetTable* CSkillCustomizeParser::m_pHTBTable = NULL;
CActionTable* CSkillCustomizeParser::m_pActionTable = NULL;
CTextAllTable* CSkillCustomizeParser::m_pTextTable = NULL;
gui::CSurfaceManager* CSkillCustomizeParser::m_pSurfaceManager = NULL;

CSkillCustomizeParser::CSkillCustomizeParser( const RwChar* szFileName )
{
	if( !CheckStaticValue() )
		return;

	std::string strFileName( szFileName );
	gui::CComponentTokenizer lexer( strFileName );

	if( !lexer.IsSuccess() )
	{
		m_strError = "[[ " + strFileName + " ]]" + " file does not exist\\n";
		return;
	}

	enum EParseState
	{
		PARSE_GLOBAL,			// 전체 frame
		PARSE_COMPONENT_TYPE,   // component type
		PARSE_COMPONENT_NAME,   // component name
		PARSE_COMPONENT,
		PARSE_VARIABLE_NAME,    // variable name
		PARSE_VARIABLE_VALUE    // variable value
	};

	EParseState State = PARSE_GLOBAL;
	stSKILLCUSTOMIZEPARSEDDATA* pParsedData = NULL;
	stSKILLCUSTOMIZEPARSEDDATA* pLastParsedData = NULL;
	std::string	strVariableName, strVariableValue;	

	while( TRUE )
	{
		const std::string& token = lexer.PeekNextToken();

		switch( State )
		{
		case PARSE_GLOBAL:
			{
				pParsedData = IsValidType(token.c_str());
				if(pParsedData)
				{
					// an existing component type is being declared.
					State = PARSE_COMPONENT_TYPE;			
				}
				else if( token == "" ) 
				{
					// end of Script.
					m_strError.clear();					
					return;
				}
				else
				{
					m_strError = "[" + token + "] Type Name is Wrong\\n";
					ErrorBeforeThis( pLastParsedData );
					return;
				}
			}
			break;
		case PARSE_COMPONENT_TYPE:
			{
				if( token == "{" )
				{
					m_strError = "Name is Invalid\\n";
					NTL_DELETE( pParsedData );
					ErrorBeforeThis( pLastParsedData );
					return;
				}
				else
				{
					if( pParsedData->GetType() == stSKILLCUSTOMIZEPARSEDDATA::SKILL )
					{
						if( m_mapParsedSkillData.find( token ) != m_mapParsedSkillData.end() )
						{
							m_strError = "Name Error. Same Skill Name Exist. [" + token + "]\\n";
							NTL_DELETE( pParsedData );
							ErrorBeforeThis( pLastParsedData );
							return;
						}

						pParsedData->SetName( token.c_str() );
					}
					else if( pParsedData->GetType() == stSKILLCUSTOMIZEPARSEDDATA::LINE )
					{
						if( m_mapParsedLineData.find( token ) != m_mapParsedLineData.end() )
						{
							m_strError = "Name Error. Same Line Name Exist. [" + token + "]\\n";
							NTL_DELETE( pParsedData );
							ErrorBeforeThis( pLastParsedData );
							return;
						}

						pParsedData->SetName( token.c_str() );
					}

					State = PARSE_COMPONENT_NAME;
				}
			}
			break;
		case PARSE_COMPONENT_NAME:
			{
				if( token != "{" )
				{
					m_strError = "Missing '{' following declaration of [ " + pParsedData->GetName() + " ]\\n";
					NTL_DELETE( pParsedData );
					return;
				}

				State = PARSE_COMPONENT;
			}
			break;
		case PARSE_COMPONENT:
			{
				if( token == "}" )
				{
					// declaration of current component is finished
					if( pParsedData->GetType() == stSKILLCUSTOMIZEPARSEDDATA::SKILL )
						m_mapParsedSkillData[pParsedData->GetName()] = reinterpret_cast<stSKILLCUSTOMIZEPARSEDSKILL*>( pParsedData );
					else if( pParsedData->GetType() == stSKILLCUSTOMIZEPARSEDDATA::LINE )
						m_mapParsedLineData[pParsedData->GetName()] = reinterpret_cast<stSKILLCUSTOMIZEPARSEDLINE*>( pParsedData );

					pParsedData = NULL;
					State = PARSE_GLOBAL;
				}
				else if( token.length() == 1 && lexer.IsOperator(token[0]) )
				{
					m_strError = "Expected variable-name of '}' of [ " + pParsedData->GetName() + " ]\\n";
					NTL_DELETE( pParsedData );
					return;
				}
				else
				{
					// we've entered the declaration of a VriableName
					State = PARSE_VARIABLE_NAME;
					strVariableName = token;		
					strVariableValue.clear();
				}
			}
			break;
		case PARSE_VARIABLE_NAME:
			{
				if( token == "=" )
				{
					// a component option with a value is being declared
					State = PARSE_VARIABLE_VALUE;
				}
				else if (token == ";")
				{
					// a component option without a value is being declared
					m_strError = "No Value in " + strVariableName + "of [ " + pParsedData->GetName() + " ]\\n";
					strVariableName.clear();
					NTL_DELETE( pParsedData );
					return;
				}
				else
				{
					m_strError = "Expected = Operator " + strVariableValue + "of [ " + pParsedData->GetName() + " ]\\n";
					NTL_DELETE( pParsedData );
					return;
				}
			}			
			break;
		case PARSE_VARIABLE_VALUE:
			{
				if( token == ";" )
				{
					if( !pParsedData->SetData( strVariableName.c_str(), strVariableValue.c_str(), m_strError ) )
					{
						NTL_DELETE( pParsedData );
						return;
					}

					strVariableName.clear();
					strVariableValue.clear();
					State = PARSE_COMPONENT;
				}
				else
				{
					strVariableValue += token;
				}
			}
			break;
		}
		lexer.PopToPeek();
	}	
}

CSkillCustomizeParser::~CSkillCustomizeParser(VOID)
{
	MAP_SKILLITEM_ITER iter;

	for( iter = m_mapSkillItem.begin() ; iter != m_mapSkillItem.end() ; ++iter )
	{
		CSkillCustomizeSkillItem* pItem = (*iter).second;
		NTL_DELETE( pItem );
	}

	m_mapSkillItem.clear();

	DeleteParsedItem();	
}

RwBool CSkillCustomizeParser::GenerateGuiItem( gui::CComponent* pParent )
{
	MAP_PARSED_SKILLDATA_ITER iter;

#ifdef SCVIEWER_MODE
	RwInt32 nYLimit = pParent->GetHeight();
#endif

	for( iter = m_mapParsedSkillData.begin() ; iter != m_mapParsedSkillData.end() ; ++iter )
	{
		stSKILLCUSTOMIZEPARSEDSKILL* pData = (*iter).second;
		sTBLDAT* pTableData = NULL;

		switch( pData->bySkillType )
		{
		case SKILL_TYPE_SKILL : pTableData = reinterpret_cast<sTBLDAT*>( m_pSkillTable->FindData( pData->tblidx ) ); break;
		case SKILL_TYPE_HTB : pTableData = reinterpret_cast<sTBLDAT*>( m_pHTBTable->FindData( pData->tblidx ) ); break;
		case SKILL_TYPE_ACTION : pTableData = reinterpret_cast<sTBLDAT*>( m_pActionTable->FindData( pData->tblidx ) ); break;	
		}

		if( !pTableData )
		{
			RwChar buf[256];
			sprintf_s( buf, "\n [ %s ] Skill Component has wrong TblIdx ( %u )\\n", pData->GetName().c_str(), pData->tblidx );
			m_strError += buf;
			continue;
		}

		CSkillCustomizeSkillItem* pSkillItem = NTL_NEW CSkillCustomizeSkillItem( pParent, pData->bySkillType, pTableData, pData->bMastery, pData->nX, pData->nY, m_pSurfaceManager, m_strError );
		m_mapSkillItem[pData->GetName()] = pSkillItem;

#ifdef SCVIEWER_MODE
		if( pSkillItem->GetSkillPosition().bottom > nYLimit )
			nYLimit = pSkillItem->GetSkillPosition().bottom;			
#endif
	}

	MAP_PARSED_LINEDATA_ITER iterline;

	for( iterline = m_mapParsedLineData.begin() ; iterline != m_mapParsedLineData.end() ; ++iterline )
	{
		stSKILLCUSTOMIZEPARSEDLINE* pData = (*iterline).second;

		CSkillCustomizeSkillItem* pBeginSkillItem = NULL;
		CSkillCustomizeSkillItem* pEndSkillItem = NULL;
		MAP_SKILLITEM_ITER iterItem;

		iterItem = m_mapSkillItem.find( pData->strBeginSkillName );
		if( iterItem != m_mapSkillItem.end() )
			pBeginSkillItem = (*iterItem).second;

		iterItem = m_mapSkillItem.find( pData->strEndSkillName );
		if( iterItem != m_mapSkillItem.end() )
			pEndSkillItem = (*iterItem).second;

		if( !pBeginSkillItem )
		{
			m_strError += "\n begin_skill [ " + pData->strBeginSkillName + " ] does not exists\\n";
			continue;
		}
		else if( !pEndSkillItem )
		{
			m_strError += "\n end_skill [ " + pData->strEndSkillName + " ] does not exists\\n";
			continue;
		}

		CSkillCustomizeLineItem* pLineItem = NTL_NEW CSkillCustomizeLineItem( pParent, pData->byLineType, pData->byBeginLinePos, pData->byEndLinePos, pBeginSkillItem, pEndSkillItem, iterline->first, m_strError );

		// peessi : 업그레이드 여부는 End쪽에서 검사. Option Line은 Begin쪽에서 관리.
		if( pData->byLineType == LINE_TYPE_UPGRADE )
			pEndSkillItem->SetLine( pLineItem );
		else
			pBeginSkillItem->SetLine( pLineItem );

#ifdef SCVIEWER_MODE
		if( pLineItem->GetBottomPos() > nYLimit )
			nYLimit = pLineItem->GetBottomPos();
#endif
	}

	DeleteParsedItem();

#ifdef SCVIEWER_MODE
	//	부모 높이 설정은 기본적으로 안보이는 스킬 때문에 하부로 이동... 뷰어모드로 빌드할때는 다 보이게 한다. 
	if( nYLimit > pParent->GetHeight() )
		pParent->SetHeight( nYLimit );
#endif

	return TRUE;
}

stSKILLCUSTOMIZEPARSEDDATA* CSkillCustomizeParser::IsValidType( const RwChar* szTypeName )
{
	stSKILLCUSTOMIZEPARSEDDATA* pData = NULL;
	RwChar bufType[256];
	strcpy_s( bufType, 256, szTypeName );
	_strlwr_s( bufType, 256 );

	if( !strcmp( bufType, "skill" ) )
		pData = NTL_NEW stSKILLCUSTOMIZEPARSEDSKILL( SKILL_TYPE_SKILL );		
	else if( !strcmp( bufType, "htb" ) )
		pData = NTL_NEW stSKILLCUSTOMIZEPARSEDSKILL( SKILL_TYPE_HTB );		
	else if( !strcmp( bufType, "action" ) )
		pData = NTL_NEW stSKILLCUSTOMIZEPARSEDSKILL( SKILL_TYPE_ACTION );		
	else if( !strcmp( bufType, "upgradeline" ) )
		pData = NTL_NEW stSKILLCUSTOMIZEPARSEDLINE( LINE_TYPE_UPGRADE );		
	else if( !strcmp( bufType, "optionline" ) )
		pData = NTL_NEW stSKILLCUSTOMIZEPARSEDLINE( LINE_TYPE_OPTION );		

	return pData;
}

VOID CSkillCustomizeParser::SetCommonData( CSkillTable* pSkillTable, CHTBSetTable* pHTBTable, CActionTable* pActionTable, CTextAllTable* pTextTable, gui::CSurfaceManager* pSurfaceManager )
{
	m_pSkillTable = pSkillTable;
	m_pHTBTable = pHTBTable;
	m_pActionTable = pActionTable;
	m_pTextTable = pTextTable;
	m_pSurfaceManager = pSurfaceManager;
}

RwBool CSkillCustomizeParser::CheckStaticValue(VOID)
{
	RwBool bRes = TRUE;

	if( !m_pSkillTable )
	{
		m_strError = "[[ SkillTable  ]] table does not exist\\n";
		bRes = FALSE;
	}

	if( !m_pHTBTable )
	{
		m_strError += "[[ HTBTable ]] table does not exist\\n";
		bRes = FALSE;
	}

	if( !m_pActionTable )
	{
		m_strError += "[[ ActionTable ]] table does not exist\\n";
		bRes = FALSE;
	}	

	if( !m_pSurfaceManager )
	{
		m_strError += "[[ SurfaceManager ]] does not exist\\n";
		bRes = FALSE;
	}	

	return bRes;
}

VOID CSkillCustomizeParser::DeleteParsedItem(VOID)
{
	MAP_PARSED_LINEDATA_ITER iter_line;

	for( iter_line = m_mapParsedLineData.begin() ; iter_line != m_mapParsedLineData.end() ; ++iter_line )
	{
		stSKILLCUSTOMIZEPARSEDLINE* pLine = (*iter_line).second;
		NTL_DELETE( pLine );
	}

	m_mapParsedLineData.clear();

	MAP_PARSED_SKILLDATA_ITER iter_skill;

	for( iter_skill = m_mapParsedSkillData.begin() ; iter_skill != m_mapParsedSkillData.end() ; ++iter_skill )
	{
		stSKILLCUSTOMIZEPARSEDSKILL* pSkill = (*iter_skill).second;
		NTL_DELETE( pSkill );
	}

	m_mapParsedSkillData.clear();
}

VOID CSkillCustomizeParser::ErrorBeforeThis( stSKILLCUSTOMIZEPARSEDDATA* pData )
{
	if( pData )
		m_strError += "!! Parse Error Before [" + pData->GetName() + " ] !!";
}