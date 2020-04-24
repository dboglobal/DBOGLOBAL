#ifndef __GUI_COMPONENTOPTIONS_H__
#define __GUI_COMPONENTOPTIONS_H__

#include "gui_define.h"
#include "gui_componenttype.h"
#include "gui_frame.h"
#include "gui_panel.h"
#include "gui_anipanel.h"
#include "gui_button.h"
#include "gui_sliderbar.h"
#include "gui_scrollbar.h"
#include "gui_listbox.h"
#include "gui_inputbox.h"
#include "gui_outputbox.h"
#include "gui_staticbox.h"
#include "gui_htmlbox.h"
#include "gui_dialog.h"
//#include "gui_particleemitter.h"
#include "gui_particlebox.h"

START_GUI

class CComponentType_Component : public CComponentType
{
public:

	CComponentType_Component() : CComponentType()
	{
		m_stlOption["x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["visible"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["enabled"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["tooltip"].type = CComponentType::SOptionType::STRING;
		m_stlOption["description"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_file"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["render_top"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["priority"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["clipping"].type = CComponentType::SOptionType::BOOL;
	}
};

class CComponentType_Frame : public CComponentType_Component
{
public:
	CComponentType_Frame() : CComponentType_Component()
	{
		m_stlOption["mode"].type = CComponentType::SOptionType::NUMBER;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CFrame(parent,pSurfaceManager);
	}
};

class CComponentType_Panel : public CComponentType_Component
{
public:
	CComponentType_Panel() : CComponentType_Component()
	{
		m_stlOption["mode"].type = CComponentType::SOptionType::NUMBER;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CPanel(parent,pSurfaceManager);
	}
};

class CComponentType_AniPanel : public CComponentType_Component
{
public:
	CComponentType_AniPanel() : CComponentType_Component()
	{
		m_stlOption["mode"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["delta"].type = CComponentType::SOptionType::NUMBER;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CAniPanel(parent,pSurfaceManager);
	}
};

class CComponentType_Button : public CComponentType_Component
{
public:

	CComponentType_Button() : CComponentType_Component()
	{
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["togglemode"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["toggled"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_mask"].type = CComponentType::SOptionType::STRING;

		// button text 속성.
		m_stlOption["text_x"].type = CComponentType::SOptionType::NUMBER;						// 버튼내부의 텍스트 위치.
		m_stlOption["text_y"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["textcolor_up_red"].type = CComponentType::SOptionType::NUMBER;				// 각상태의 글자색.
		m_stlOption["textcolor_up_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_up_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha_up"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_down_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_down_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_down_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha_down"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_focus_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_focus_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_focus_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha_focus"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_disable_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_disable_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_disable_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha_disable"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;				// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_downcoord_x"].type = CComponentType::SOptionType::NUMBER;				// 눌렸을때 이동되는 값.
		m_stlOption["text_downcoord_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;					// 그외 스타일들.
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["enablemousecapture"].type = CComponentType::SOptionType::BOOL;				// 클릭시 마우스캡쳐 여부
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CButton(parent,pSurfaceManager);
	}
};

class CComponentType_ScrollBar : public CComponentType_Component
{
public:

	CComponentType_ScrollBar() : CComponentType_Component()
	{
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["orientation"].type = CComponentType::SOptionType::STRING;

		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CScrollBar(parent,pSurfaceManager);
	}
};


class CComponentType_SliderBar : public CComponentType_Component
{
public:

	CComponentType_SliderBar() : CComponentType_Component()
	{
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["orientation"].type = CComponentType::SOptionType::STRING;

		m_stlOption["surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CSliderBar(parent,pSurfaceManager);
	}
};

class CComponentType_InputBox : public CComponentType_Component
{
public:

	CComponentType_InputBox() : CComponentType_Component()
	{
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["maxlength"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["readonly"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["password_mode"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["numberonly"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;

		// ScrollBar 속성.
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["orientation"].type = CComponentType::SOptionType::STRING;
		
		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CInputBox(parent,pSurfaceManager);
	}
};


class CComponentType_OutputBox : public CComponentType_Component
{
public:

	CComponentType_OutputBox() : CComponentType_Component()
	{
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;			// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["maxline"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["interval"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["vertical_scroll"].type = CComponentType::SOptionType::BOOL;
		
		m_stlOption["scrollbar_dynamicshow"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["scrollbar_leftline"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["scrollbar_margin_top"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["scrollbar_margin_bottom"].type = CComponentType::SOptionType::NUMBER;
		
		m_stlOption["scrollbar_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_height"].type = CComponentType::SOptionType::NUMBER;
		
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW COutputBox(parent,pSurfaceManager);
	}
};


class CComponentType_HyperTextBox : public CComponentType_Component
{
public:

	CComponentType_HyperTextBox() : CComponentType_Component()
	{
		m_stlOption["row"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["column"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["interval"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["vertical_scroll"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CHyperTextBox(parent,pSurfaceManager);
	}
};

class CComponentType_StaticBox : public CComponentType_Component
{
public:

	CComponentType_StaticBox() : CComponentType_Component()
	{
		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bk_mode"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;			// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["linespace"].type = CComponentType::SOptionType::NUMBER;
		
		// dynamic attribute
		m_stlOption["dynamic"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["max_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 컴포넌트의 크기
		m_stlOption["max_cy"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 0
		m_stlOption["min_cy"].type = CComponentType::SOptionType::NUMBER;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CStaticBox(parent,pSurfaceManager);
	}
};

class CComponentType_Dialog : public CComponentType_Component
{
public:
	CComponentType_Dialog() : CComponentType_Component()
	{
		m_stlOption["mode"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["titlebar"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["titlebar_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["titlebar_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["titlebar_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["titlebar_height"].type = CComponentType::SOptionType::NUMBER;
		
		m_stlOption["dynamic"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["surface_corner_lefttop"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_righttop"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_leftbottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_rightbottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_left"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_top"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_right"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_bottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_center"].type = CComponentType::SOptionType::STRING;
		m_stlOption["left_cx"].type = CComponentType::SOptionType::STRING;
		m_stlOption["right_cx"].type = CComponentType::SOptionType::STRING;
		m_stlOption["top_cy"].type = CComponentType::SOptionType::STRING;
		m_stlOption["bottom_cy"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CDialog(parent,pSurfaceManager);
	}
};

class CComponentType_ProgressBar : public CComponentType_Component
{
public:
	CComponentType_ProgressBar() : CComponentType_Component()
	{
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["vertical"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["reverse_order"].type = CComponentType::SOptionType::BOOL;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CProgressBar(parent,pSurfaceManager);
	}
};


class CComponentType_ListBox : public CComponentType_Component
{
public:

	CComponentType_ListBox() : CComponentType_Component()
	{
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_selcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_selcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_selcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_alpha"].type = CComponentType::SOptionType::NUMBER;		
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;			// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["scrollbar_margin_x"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["multi_selection"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["visiblecount"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["sort"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["item"].type = CComponentType::SOptionType::STRING;	
		m_stlOption["masksurface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["scrollbar_dynamicshow"].type = CComponentType::SOptionType::BOOL;
		
		m_stlOption["scroll_button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["scroll_button_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["orientation"].type = CComponentType::SOptionType::STRING;

		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent *CreateComponent(CComponent *parent,CSurfaceManager *pSurfaceManager)
	{
		return NTL_NEW CListBox(parent,pSurfaceManager);
	}
};

//class CComponentType_DynamicOutline : public CComponentType_Component
//{
//public:
//	static VOID Outline_SetOption( stlOPTION_MAP& Option )
//	{
//		m_stlOption["balloontail"].type = CComponentType::SOptionType::BOOL;
//		m_stlOption["surface_corner_lefttop"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_corner_righttop"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_corner_leftbottom"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_corner_rightbottom"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_line_left"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_line_top"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_line_right"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_line_bottom"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_balloontail"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["surface_center"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["left_cx"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["right_cx"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["top_cy"].type = CComponentType::SOptionType::STRING;
//		m_stlOption["bottom_cy"].type = CComponentType::SOptionType::STRING;
//	} 
//};

class CComponentType_Balloon : public CComponentType_Component
{
public:

	CComponentType_Balloon() : CComponentType_Component()
	{
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bk_mode"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;			// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["linespace"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["dynamic"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["max_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 컴포넌트의 크기
		m_stlOption["max_cy"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 0
		m_stlOption["min_cy"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["balloontail"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["surface_corner_lefttop"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_righttop"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_leftbottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_rightbottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_left"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_top"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_right"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_bottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_balloontail"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_center"].type = CComponentType::SOptionType::STRING;
		m_stlOption["left_cx"].type = CComponentType::SOptionType::STRING;
		m_stlOption["right_cx"].type = CComponentType::SOptionType::STRING;
		m_stlOption["top_cy"].type = CComponentType::SOptionType::STRING;
		m_stlOption["bottom_cy"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CBalloon( parent, pSurfaceManager );
	}
};

class CComponentType_ComboBox : public CComponentType_Component
{
public:

	CComponentType_ComboBox() : CComponentType_Component()
	{
		// 추가 
		m_stlOption["list_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["static_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["button_text"].type = CComponentType::SOptionType::STRING;

		m_stlOption["list_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["combo_button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["combo_button_height"].type = CComponentType::SOptionType::NUMBER;

		// StaticBox Attribute
		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bk_mode"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;			// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		
		m_stlOption["disable_staticbox"].type = CComponentType::SOptionType::BOOL;

		m_stlOption["togglemode"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["toggled"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_mask"].type = CComponentType::SOptionType::STRING;

		m_stlOption["list_text_style"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_selcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_selcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_selcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mask_alpha"].type = CComponentType::SOptionType::NUMBER;		
		m_stlOption["masksurface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;
		
		m_stlOption["text_margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["scrollbar_margin_x"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["visiblecount"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["sort"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["item"].type = CComponentType::SOptionType::STRING;	

		// Scroll property of List box
		m_stlOption["scroll_button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["scroll_button_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;		
		m_stlOption["scrollbar_dynamicshow"].type = CComponentType::SOptionType::BOOL;

		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent* CreateComponent( CComponent *pParent, CSurfaceManager *pSurfaceManager )
	{
		return NTL_NEW CComboBox( pParent, pSurfaceManager );
	}
};

class CComponentType_ToolTip : public CComponentType_Component
{
public:

	CComponentType_ToolTip() : CComponentType_Component()
	{
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mousemargin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["mousemargin_y"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text"].type = CComponentType::SOptionType::STRING;
		m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bk_mode"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["text_bkcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_bkcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;			// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["dynamic"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["max_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 컴포넌트의 크기
		m_stlOption["max_cy"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 0
		m_stlOption["min_cy"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["surface_corner_lefttop"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_righttop"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_leftbottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_corner_rightbottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_left"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_top"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_right"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line_bottom"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_center"].type = CComponentType::SOptionType::STRING;
		m_stlOption["left_cx"].type = CComponentType::SOptionType::STRING;
		m_stlOption["right_cx"].type = CComponentType::SOptionType::STRING;
		m_stlOption["top_cy"].type = CComponentType::SOptionType::STRING;
		m_stlOption["bottom_cy"].type = CComponentType::SOptionType::STRING;
	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CToolTip( parent, pSurfaceManager );
	}
};

class CComponentType_TabButton : public CComponentType_Component
{
public:

	CComponentType_TabButton() : CComponentType_Component()
	{
		m_stlOption["initindex"].type = CComponentType::SOptionType::NUMBER;	
		
		m_stlOption["textcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_sel_red"].type = CComponentType::SOptionType::NUMBER;		// 상태의 글자색.
		m_stlOption["textcolor_sel_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_sel_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha_sel"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_foc_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_foc_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_foc_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textalpha_foc"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_disable_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_disable_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["textcolor_disable_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_mode"].type = CComponentType::SOptionType::STRING;				// shadow : 그림자 ouline : 외곽선,글로우 none : 없음
		m_stlOption["text_effectcolor_red"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_green"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effectcolor_blue"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["text_effect_value"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["textalpha_disable"].type = CComponentType::SOptionType::NUMBER;
		
		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;					// 그외 스타일들.
		m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
		m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;

		m_stlOption["surface_left"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_center"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_right"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_sel_left"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_sel_center"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_sel_right"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_foc_left"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_foc_center"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_foc_right"].type = CComponentType::SOptionType::STRING;
		m_stlOption["surface_line"].type = CComponentType::SOptionType::STRING;

		m_stlOption["tabtext_margin_x"].type = CComponentType::SOptionType::NUMBER;				// left surface의 left, top 기준 좌표.
		m_stlOption["tabtext_margin_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["tab_unsel_width"].type = CComponentType::SOptionType::NUMBER;				// 셀렉트 되지 탭의 최소 폭.
		m_stlOption["left_oriented"].type = CComponentType::SOptionType::BOOL;					
						
		m_stlOption["tabtext"].type = CComponentType::SOptionType::STRING;						// 넣는 순서대로 인덱스화.
	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CTabButton( parent, pSurfaceManager );
	}
};

class CComponentType_MDStaticBox : public CComponentType_Component
{
public:
	CComponentType_MDStaticBox() : CComponentType_Component()
	{
		// Font 단위. 생략될 수 없다. 
		{
			m_stlOption["font_name"].type = CComponentType::SOptionType::STRING;
			m_stlOption["font_height"].type = CComponentType::SOptionType::NUMBER;
			m_stlOption["font_attribute"].type = CComponentType::SOptionType::NUMBER;
		}

		// Text 단위. 생략될 수 없다. text
		{
			m_stlOption["text"].type = CComponentType::SOptionType::STRING;
			m_stlOption["text_name"].type = CComponentType::SOptionType::STRING;
			m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;
			m_stlOption["font_index"].type = CComponentType::SOptionType::NUMBER;
			m_stlOption["text_x"].type = CComponentType::SOptionType::NUMBER;
			m_stlOption["text_y"].type = CComponentType::SOptionType::NUMBER;
			m_stlOption["text_attachprevline"].type = CComponentType::SOptionType::BOOL;
			m_stlOption["text_forcexy"].type = CComponentType::SOptionType::BOOL;
			m_stlOption["text_color_red"].type = CComponentType::SOptionType::NUMBER;
			m_stlOption["text_color_green"].type = CComponentType::SOptionType::NUMBER;
			m_stlOption["text_color_blue"].type = CComponentType::SOptionType::NUMBER;
		}
		
		m_stlOption["min_cx"].type = CComponentType::SOptionType::NUMBER;	// 미니멈 사이즈
		m_stlOption["min_cy"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_x"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["margin_y"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["interval"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["defaultfont_index"].type = CComponentType::SOptionType::NUMBER;
	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CMDStaticBox( parent, pSurfaceManager );
	}
};

class CComponentType_HtmlBox : public CComponentType_Component
{
public:
	CComponentType_HtmlBox() : CComponentType_Component()
	{
		// ScrollBar 속성.
		m_stlOption["slider_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["slider_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_width"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["button_height"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["max"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["value"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["orientation"].type = CComponentType::SOptionType::STRING;
		
		m_stlOption["decrease_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["decrease_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_up"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_down"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_focus"].type = CComponentType::SOptionType::STRING;
		m_stlOption["increase_surface_disable"].type = CComponentType::SOptionType::STRING;
		m_stlOption["slider_surface"].type = CComponentType::SOptionType::STRING;
		m_stlOption["layout_surface"].type = CComponentType::SOptionType::STRING;

		// Text Style ( 세로 만 적용이 된다. 가로는 Html Tag의 가로 정렬로 적용한다. )
		m_stlOption["text_style"].type = CComponentType::SOptionType::STRING;

		// 폰트 크기 가중치
		m_stlOption["font_ratio"].type = CComponentType::SOptionType::NUMBER;
		
		// Dynamic
		m_stlOption["dynamic"].type = CComponentType::SOptionType::BOOL;
		m_stlOption["max_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 컴포넌트의 크기
		m_stlOption["max_cy"].type = CComponentType::SOptionType::NUMBER;
		m_stlOption["min_cx"].type = CComponentType::SOptionType::NUMBER;					// 기본값은 0
		m_stlOption["min_cy"].type = CComponentType::SOptionType::NUMBER;
				
	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CHtmlBox( parent, pSurfaceManager );
	}
};

class CComponentType_Flash : public CComponentType_Component
{
public:
	CComponentType_Flash() : CComponentType_Component()
	{
		m_stlOption["filename"].type = CComponentType::SOptionType::STRING;		// Flash Filename 경로없이 Filename만 써준다.
		m_stlOption["initplay"].type = CComponentType::SOptionType::BOOL;		// Play immediately after Load. Default = FLASE
		m_stlOption["autorewind"].type = CComponentType::SOptionType::BOOL;		// Auto replay after the movie ends. Default = FALSE
		m_stlOption["framecallback"].type = CComponentType::SOptionType::BOOL;	// It releases the callback when the frame ends. Default = FALSE
		m_stlOption["usefont"].type = CComponentType::SOptionType::BOOL;		// 폰트 사용시 꼭 켜주어야 한다. Default = FALSE
		m_stlOption["originalsize"].type = CComponentType::SOptionType::BOOL;	// 생성시 설정한 넓이 높이 값이 아닌 리소스자체의 작업영역으로 
		m_stlOption["resizetype"].type = CComponentType::SOptionType::STRING;	// ( Default = noscale ) showall(종횡비유지), exactfit(종횡비무시)		
		m_stlOption["scissor"].type = CComponentType::SOptionType::BOOL;		// 부모,자신 영역밖으로 나간 부분의 클리핑유무,
	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CFlash( parent, pSurfaceManager );
	}
};

class CComponentType_ParticleBox : public CComponentType_Component
{
public:
	CComponentType_ParticleBox() : CComponentType_Component()
	{

	}

	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
	{
		return NTL_NEW CParticleBox( parent, pSurfaceManager );
	}
};

//class CComponentType_ParticleEmitter : public CComponentType_Component
//{
//public:
//
//	CComponentType_ParticleEmitter() : CComponentType_Component()
//	{
//
//	}
//
//	virtual CComponent* CreateComponent( CComponent* parent, CSurfaceManager* pSurfaceManager )
//	{
//		return NTL_NEW CParticleEmitter( parent, pSurfaceManager );
//	}
//};

END_GUI

#endif

