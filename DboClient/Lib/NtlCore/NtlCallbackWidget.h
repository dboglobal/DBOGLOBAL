/*****************************************************************************
 *
 * File			: NtlCallbackWidget.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 22	
 * Abstract		: callback widget
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_CALLBACKWIDGET_H__
#define __NTL_CALLBACKWIDGET_H__

class CNtlCallbackParam0
{
public:

	virtual ~CNtlCallbackParam0() {}
	virtual int Call(void)=0;
};

template <class Callbackclass>
class CNtlCallbackWidget0 : public CNtlCallbackParam0
{
public:

	typedef int (Callbackclass::*Callback)(void);
	
	//: Method Slot V0 Constructor
	CNtlCallbackWidget0(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(void)
	{
		return (m_cbclass->*m_callback)();
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};


class CNtlCallbackParam1
{
public:

	virtual ~CNtlCallbackParam1() {}
	virtual int Call(unsigned int param1)=0;
};

template <class Callbackclass>
class CNtlCallbackWidget1 : public CNtlCallbackParam1
{
public:

	typedef int (Callbackclass::*Callback)(unsigned int param1);
	
	//: Method Slot V0 Constructor
	CNtlCallbackWidget1(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(unsigned int param1)
	{
		return (m_cbclass->*m_callback)(param1);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

class CNtlCallbackParam2
{
public:

	virtual ~CNtlCallbackParam2() {}
	virtual int Call(unsigned int param1,unsigned int param2)=0;
};

template <class Callbackclass>
class CNtlCallbackWidget2 : public CNtlCallbackParam2
{
public:

	typedef int (Callbackclass::*Callback)(unsigned int param1,unsigned int param2);
	
	CNtlCallbackWidget2(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(unsigned int param1,unsigned int param2)
	{
		return (m_cbclass->*m_callback)(param1,param2);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

class CNtlCallbackParam3
{
public:

	virtual ~CNtlCallbackParam3() {}
	virtual int Call(unsigned int param1, unsigned int param2, unsigned int param3) = 0;
};

template <class Callbackclass>
class CNtlCallbackWidget3 : public CNtlCallbackParam3
{
public:

	typedef int (Callbackclass::*Callback)(unsigned int param1, unsigned int param2, unsigned int param3);

	//: Method Slot V0 Constructor
	CNtlCallbackWidget3(Callbackclass *cbclass, Callback callback)
		: m_cbclass(cbclass), m_callback(callback) {
		;
	}

	virtual int Call(unsigned int param1, unsigned int param2, unsigned int param3)
	{
		return (m_cbclass->*m_callback)(param1, param2, param3);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};


class CNtlCallbackParam4
{
public:

	virtual ~CNtlCallbackParam4() {}
	virtual int Call(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4) = 0;
};

template <class Callbackclass>
class CNtlCallbackWidget4 : public CNtlCallbackParam4
{
public:

	typedef int (Callbackclass::*Callback)(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4);

	//: Method Slot V0 Constructor
	CNtlCallbackWidget4(Callbackclass *cbclass, Callback callback)
		: m_cbclass(cbclass), m_callback(callback) {
		;
	}

	virtual int Call(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4)
	{
		return (m_cbclass->*m_callback)(param1, param2, param3, param4);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};


class CNtlCallbackVPParam1
{
public:

	virtual ~CNtlCallbackVPParam1() {}
	virtual int Call(void* param1) = 0;
};

template <class Callbackclass>
class CNtlCallbackVPWidget1 : public CNtlCallbackVPParam1
{
public:

	typedef int (Callbackclass::*Callback)(void* param1);

	//: Method Slot V0 Constructor
	CNtlCallbackVPWidget1(Callbackclass *cbclass, Callback callback)
		: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(void* param1)
	{
		return (m_cbclass->*m_callback)(param1);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

class CNtlCallbackVPParam2
{
public:

	virtual ~CNtlCallbackVPParam2() {}
	virtual int Call( void* param1, void* param2 ) = 0;
};

template <class Callbackclass>
class CNtlCallbackVPWidget2 : public CNtlCallbackVPParam2
{
public:

	typedef int (Callbackclass::*Callback)( void* param1, void* param2 );

	CNtlCallbackVPWidget2( Callbackclass* cbclass, Callback callback )
		: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call( void* param1, void* param2 )
	{
		return (m_cbclass->*m_callback)( param1, param2 );
	}

public:

	Callbackclass* m_cbclass;
	Callback m_callback;
};


class CNtlCallbackVPParam3
{
public:

	virtual ~CNtlCallbackVPParam3() {}
	virtual int Call(void* param1, void* param2, void* param3) = 0;
};

template <class Callbackclass>
class CNtlCallbackVPWidget3 : public CNtlCallbackVPParam3
{
public:

	typedef int (Callbackclass::* Callback)(void* param1, void* param2, void* param3);

	CNtlCallbackVPWidget3(Callbackclass* cbclass, Callback callback)
		: m_cbclass(cbclass), m_callback(callback) {
		;
	}

	virtual int Call(void* param1, void* param2, void* param3)
	{
		return (m_cbclass->*m_callback)(param1, param2, param3);
	}

public:

	Callbackclass* m_cbclass;
	Callback m_callback;
};


class CNtlCallbackParam1bool
{
public:

	virtual ~CNtlCallbackParam1bool() {}
	virtual int Call(bool param1)=0;
};

template <class Callbackclass>
class CNtlCallbackWidget1bool : public CNtlCallbackParam1bool
{
public:

	typedef int (Callbackclass::*Callback)(bool param1);

	//: Method Slot V0 Constructor
	CNtlCallbackWidget1bool(Callbackclass *cbclass, Callback callback)
		: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(bool param1)
	{
		return (m_cbclass->*m_callback)(param1);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

class CNtlCallbackParam1float
{
public:

	virtual ~CNtlCallbackParam1float() {}
	virtual int Call(float param1)=0;
};

template <class Callbackclass>
class CNtlCallbackWidget1float : public CNtlCallbackParam1float
{
public:

	typedef int (Callbackclass::*Callback)(float param1);

	//: Method Slot V0 Constructor
	CNtlCallbackWidget1float(Callbackclass *cbclass, Callback callback)
		: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(float param1)
	{
		return (m_cbclass->*m_callback)(param1);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

#endif