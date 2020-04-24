#ifndef C_EDITOR_MAIN_CONTROL_BAR
#define C_EDITOR_MAIN_CONTROL_BAR

#include "CSizingTabControlBar.h"

class CEditorMainControlBar : public CSizingTabControlBar
{
	public:

										CEditorMainControlBar();
		virtual							~CEditorMainControlBar();

	public:

		virtual int						AddViewes( void );
};


#endif //C_EDITOR_MAIN_CONTROL_BAR
