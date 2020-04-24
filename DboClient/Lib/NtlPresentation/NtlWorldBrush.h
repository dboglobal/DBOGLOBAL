#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlWorldBrushController;


////////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlWorldBrush
{
public:
	CNtlWorldBrush();
	~CNtlWorldBrush();

protected:
	CNtlWorldBrushController*	m_pController;
	VOID*						m_pUP;

public:
	CNtlWorldBrushController*	GetController() { return m_pController; }
	void						SetController(CNtlWorldBrushController *pController);
	VOID*						GetUP()				{ return m_pUP; }
	VOID						SetUP(VOID* _pUP)	{ m_pUP = _pUP; }

public:
	void Render();
};