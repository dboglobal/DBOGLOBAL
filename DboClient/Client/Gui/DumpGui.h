/*****************************************************************************
 *
 * File			: DumpGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 27	
 * Abstract		: DBO dump gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DUMP_GUI_H__
#define __DUMP_GUI_H__

#include "NtlPLGui.h"

class CDumpGui : public CNtlPLGui
{
private:

	gui::COutputBox *m_poutDisplay;
		
public:

	CDumpGui();
	CDumpGui(const RwChar *pName);
	~CDumpGui();

	RwBool Create(void);
	void Destroy(void);

public:

	gui::CDialog* GetDialog(void);
	gui::COutputBox* GetOutputBox(void);

// signal function
public:

};

#endif