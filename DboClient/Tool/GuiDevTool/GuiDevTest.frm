// --------------------------------------------------------------------------------------
// Dialog Setting

dialog dlgMain
{
	x	= 0;
 	y 	= 0;
	width	= 1024;
	height	= 768;

	surface_file = "GuiDevTest.srf";

	htmlbox testhtml
	{
		x = 1;
		y = 1;
		width = 480;
		height = 640;
		surface_file = "GuiDevTest.srf";
		surface = "testhtml";
		
		button_height	= 19;
		button_width	= 12;
		slider_height 	= 39;
		slider_width 	= 12;
		
		increase_surface_up			= "srfScrollNextBtnUp";
		increase_surface_down		= "srfScrollNextBtnDown";
		increase_surface_focus		= "srfScrollNextBtnFoc";
		increase_surface_disable	= "srfScrollNextBtnDown";
		decrease_surface_up			= "srfScrollPrevBtnUp";
		decrease_surface_down		= "srfScrollPrevBtnDown";
		decrease_surface_focus		= "srfScrollPrevBtnFoc";
		decrease_surface_disable	= "srfScrollPrevBtnDown";
		slider_surface				= "srfScrollMedium";
		layout_surface				= "srfScrollLayout";
	}

	panel pnlParent
	{
		x = 512;
		y = 368;
		width = 320;
		height = 240;

		surface_file = "GuiDevTest.srf";
		surface = "srfParent";
		clipping = true;

		staticbox stbChild
		{
			x = 200;
			y = 100;
			width = 200;
			height = 100;
			clipping = true;
			margin_x = 10;
			margin_y = 10;


			surface_file = "GuiDevTest.srf";
			surface = "balloonLTC";
			surface = "balloonRTC";
			surface = "balloonLBC";
			surface = "balloonRBC";
			surface = "balloonLL";
			surface = "balloonTL";
			surface = "balloonBL";
			surface = "balloonRL";
			font_height = 120;
			text = "DragonBall Online";
			text_style = center;

			button btnGrandChild
			{
				x = -30;
				y = 70;
				width = 56;
				height = 55;
				clipping = true;
				text = "업무시간엔 업무를 열심히";

				surface_file = "GuiDevTest.srf";
				surface_up 	= "srfButtonUp";
				surface_down 	= "srfButtonMask";
				surface_focus 	= "srfButtonFocus";
				surface_disable = "srfButtonMask";
				surface_mask	= "srfButtonMask";
				text_style = "up";
			}
		}
	}
}