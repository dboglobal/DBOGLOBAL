#ifndef __INPUTDEVICE_H__
#define __INPUTDEVICE_H__


class CInputDevice
{
public:
//! Enums:
	enum INPUTDEVICETYPE
	{
		TYPE_JOYSTICK,
		TYPE_MOUSE,
		TYPE_KEYBOARD
	};

	INPUTDEVICETYPE m_Type;
};


#endif