CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11301;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100264;
				gtype = -1;
				area = 1100262;
				goal = 1100263;
				m0fz = "172.940002";
				m0widx = 100000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "852.960022";
				m0ttip = 1100265;
				rwd = 255;
				taid = 2;
				title = 1100261;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 1;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7111405;
				taid = 1;
				esctype = 0;
				idx2 = 3651102;
			}
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "4;";
			nolnk = 5;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 193000;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100264;
				gtype = -1;
				area = 1100262;
				goal = 1100263;
				m0fz = "172.940002";
				m0widx = 193000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "852.960022";
				m0ttip = 1100265;
				rwd = 255;
				taid = 2;
				title = 1100261;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 1;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7111405;
				taid = 1;
				esctype = 0;
				idx2 = 3651102;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "4;";

			CDboTSActRegQInfo
			{
				cont = 1100264;
				gtype = -1;
				area = 1100262;
				goal = 1100263;
				m0fz = "172.940002";
				m0widx = 80000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "852.960022";
				m0ttip = 1100265;
				rwd = 255;
				taid = 2;
				title = 1100261;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 8211200;
				cnt0 = 1;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7111405;
				taid = 1;
				esctype = 0;
				idx2 = 3651102;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "241;5;1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "0;";
			nolnk = 240;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckInWorld
			{
				widx = 80000;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 13020;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActSendSvrEvt
			{
				id = 13022;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
	}
}

