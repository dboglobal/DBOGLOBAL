#include "stdafx.h"
#include "NtlCCBD.h"



bool IsCCBDBossStage(BYTE byStage)
{
	switch (byStage)
	{
		case 5:
		case 10:
		case 15:
		case 20:
		case 25:
		case 30:
		case 35:
		case 40:
		case 45:
		case 50:
		case 55:
		case 60:
		case 65:
		case 70:
		case 75:
		case 80:
		case 85:
		case 90:
		case 95:
		case 100:
		case 105:
		case 110:
		case 115:
		case 120:
		case 125:
		case 130:
		case 135:
		case 140:
		case 145:
		case 150:
			return true; break;

		default: return false; break;
	}
}
