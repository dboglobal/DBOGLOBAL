#pragma once

class CMouseMode
{
public:
	CMouseMode(void);
	virtual ~CMouseMode(void);

	void		SetMode( RwUInt8 byMode );
	RwUInt8		GetMode();

	static CMouseMode*	GetInstance()
	{
		static CMouseMode instance;
		return &instance;
	}

protected:
	RwUInt8		m_byMode;
};

static CMouseMode* GetMouseMode()
{
	return CMouseMode::GetInstance();
}
