/*
	$Id: precomp.h,v 1.10 2001/12/14 02:39:57 plasmoid Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/

#ifndef __GUI_PRECOMP_H__
#define __GUI_PRECOMP_H__

#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0500		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0500	// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

// window
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <tchar.h>
#include <imm.h>
#include <usp10.h>
#include <dimm.h>

// standard
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <fcntl.h>
#include <io.h>
#include <assert.h>
#include <mmsystem.h>
#include <strsafe.h>
#include <stddef.h>
#include <stdlib.h>

// stl
#include <string>
#include <map>
#include <list>
#include <unordered_map>
#include <vector>
#include <deque>
#include <stack>
#include <set>

// renderware
#include <rwcore.h>
#include <rwplcore.h>

#include <rpworld.h>
#include <rpusrdat.h>
#include <rphanim.h>
#include <rpskin.h>
#include <rpmatfx.h>
#include <rphanim.h>
#include <rpprtstd.h>
#include <rpprtadv.h>
#include <rpuvanim.h>
#include <rpusrdat.h>
#include <rptoon.h>
#include <rpcollis.h>
#include <rpid.h>

#include <rtanim.h>
#include <rtfsyst.h>
#include <rtanim.h>
#include <rtquat.h> 
#include <rtpick.h>
#include <rtray.h>
#include <rtdict.h>
#include <rtslerp.h>
#include <rtimport.h>
#include <rtintsec.h>
#include <rtpng.h>
#include <rtbmp.h>
#include <rtcharse.h>

// direct-x
#include <d3d9.h>
#include <d3dx9math.h>

//: gui frame work
#include "gui_guimanager.h"
#include "gui_setupgui.h"
#include "gui_componentmanager.h"
#include "gui_surfacemanager.h"
#include "gui_resourcemanager.h"
#include "gui_componentoption.h"

//: component
#include "gui_frame.h"
#include "gui_panel.h"
#include "gui_anipanel.h"
#include "gui_button.h"
#include "gui_scrollbar.h"
#include "gui_sliderbar.h"
#include "gui_inputbox.h"
#include "gui_outputbox.h"
#include "gui_hypertextbox.h"
#include "gui_dialog.h"
#include "gui_progressbar.h"
#include "gui_staticbox.h"
#include "gui_mdstaticbox.h"
#include "gui_htmlbox.h"
#include "gui_listbox.h"
#include "gui_balloon.h"
#include "gui_tooltip.h"
#include "gui_combobox.h"
#include "gui_tabbutton.h"
#include "gui_html_doc.h"
#include "gui_flash.h"
#include "gui_particle.h"
#include "gui_particlebox.h"

#include "gui_globalcomponent.h"

#include "gui_dynamicoutline.h"
#include "gui_effect_radar.h"
#include "gui_effect_flicker.h"
#include "gui_effect_path_chase.h"

// resource manager
#include "gui_font.h"
#include "gui_fontmanager.h"
#include "gui_texture.h"
#include "gui_texturemanager.h"

// Util Function
#include "GuiUtil.h"

#endif
