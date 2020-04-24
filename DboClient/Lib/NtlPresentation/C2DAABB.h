#pragma once


class C2DAABB
{
public:
	C2DAABB(void);
	virtual ~C2DAABB(void);

protected:
	RwV2d m_Min;
	RwV2d m_Max;

public:
	RwBool	CollisionTest(C2DAABB& _CompareAABB);
	RwBool	CollisionTest(RwV2d& _Pos);
	VOID	SetNewMinMax(RwV2d& _Min, RwV2d& _Max);
	VOID	SetNewMinMax(RwV2d& _Pos);
};
