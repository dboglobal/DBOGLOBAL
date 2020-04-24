CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11619;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 35;
			prelnk = "2;";

			CDboTSActRegQInfo
			{
				cont = 1100633;
				gtype = -1;
				area = 1100636;
				goal = 1100635;
				m0fz = "86.400002";
				m0widx = 190000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "333.329987";
				m0ttip = 1100634;
				rwd = 255;
				taid = 1;
				title = 1100634;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 5;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckInWorld
			{
				widx = 180000;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 2;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckInWorld
			{
				widx = 170000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 35;
			prelnk = "2;";

			CDboTSActRegQInfo
			{
				cont = 1100633;
				gtype = -1;
				area = 1100636;
				goal = 1100635;
				m0fz = "86.400002";
				m0widx = 180000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "333.329987";
				m0ttip = 1100634;
				rwd = 255;
				taid = 1;
				title = 1100634;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "35;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 16600;
			}
		}
		CDboTSContGCond
		{
			cid = 35;
			prelnk = "5;4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 254;

			CDboTSRcvSvrEvt
			{
				id = 16581;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 35;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 1100633;
				gtype = -1;
				area = 1100636;
				goal = 1100635;
				m0fz = "86.400002";
				m0widx = 170000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "333.329987";
				m0ttip = 1100634;
				rwd = 255;
				taid = 1;
				title = 1100634;
			}
		}
	}
}

