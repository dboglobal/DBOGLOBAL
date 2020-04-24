/*=======================================================================================
//  Copyright  : (주)NTL-INC  , www.ntl-inc.com			                              //
//  Version    : 0.1                                                                   // 
//  Author     : 장 형 석                                                              //  
//  E-mail     : jhssugi@hanmail.net                                                   //
//  -----------------------------------------------------------------------------------// 
//  Engine Name : 해당사항 없음.                                                       // 
//  Name       : Callback Widget.													                              //
//  Data       : 2002 . 12. 27                                                          //
//  File Des.  : callback widget																												//       
=======================================================================================*/

#ifndef __CALLBACKWIDGET_H__
#define __CALLBACKWIDGET_H__

typedef		int		CPARAM;

class CCallback_Param0
{
public:

	virtual ~CCallback_Param0() {}
	virtual int Call(VOID)=0;
};

template <class Callbackclass>
class CCallback_Widget0 : public CCallback_Param0
{
public:

	typedef int (Callbackclass::*Callback)(VOID);
	
	//: Method Slot V0 Constructor
	CCallback_Widget0(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(VOID)
	{
		return (m_cbclass->*m_callback)();
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};


class CCallback_Param1
{
public:

	virtual ~CCallback_Param1() {}
	virtual int Call(CPARAM param1)=0;
};

template <class Callbackclass>
class CCallback_Widget1 : public CCallback_Param1
{
public:

	typedef int (Callbackclass::*Callback)(CPARAM param1);
	
	//: Method Slot V0 Constructor
	CCallback_Widget1(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(CPARAM param1)
	{
		return (m_cbclass->*m_callback)(param1);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};


class CCallback_Param2
{
public:

	virtual ~CCallback_Param2() {}
	virtual int Call(CPARAM param1,CPARAM param2)=0;
};

template <class Callbackclass>
class CCallback_Widget2 : public CCallback_Param2
{
public:

	typedef int (Callbackclass::*Callback)(CPARAM param1,CPARAM param2);
	
	CCallback_Widget2(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(CPARAM param1,CPARAM param2)
	{
		return (m_cbclass->*m_callback)(param1,param2);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

class CCallback_Param3
{
public:

	virtual ~CCallback_Param3() {}
	virtual int Call(CPARAM param1,CPARAM param2,CPARAM param3)=0;
};

template <class Callbackclass>
class CCallback_Widget3 : public CCallback_Param3
{
public:

	typedef int (Callbackclass::*Callback)(CPARAM param1,CPARAM param2,CPARAM param3);
	
	//: Method Slot V0 Constructor
	CCallback_Widget3(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(CPARAM param1,CPARAM param2,CPARAM param3)
	{
		return (m_cbclass->*m_callback)(param1,param2,param3);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};


class CCallback_Param4
{
public:

	virtual ~CCallback_Param4() {}
	virtual int Call(CPARAM param1,CPARAM param2,CPARAM param3,CPARAM param4)=0;
};

template <class Callbackclass>
class CCallback_Widget4 : public CCallback_Param4
{
public:

	typedef int (Callbackclass::*Callback)(CPARAM param1,CPARAM param2,CPARAM param3,CPARAM param4);
	
	//: Method Slot V0 Constructor
	CCallback_Widget4(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) {;}

	virtual int Call(CPARAM param1,CPARAM param2,CPARAM param3,CPARAM param4)
	{
		return (m_cbclass->*m_callback)(param1,param2,param3,param4);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

#endif