CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11302;
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
				m0fz = "400.000000";
				m0widx = 100000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "939.000000";
				m0ttip = 1100265;
				rwd = 255;
				taid = 2;
				title = 1100261;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 3;
				eitype = 0;
				idx0 = 4031105;
				cnt0 = 6;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7111404;
				taid = 1;
				esctype = 0;
				idx2 = 7112400;
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
				m0fz = "400.000000";
				m0widx = 193000;
				sort = 1100024;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "939.000000";
				m0ttip = 1100265;
				rwd = 255;
				taid = 2;
				title = 1100261;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 3;
				eitype = 0;
				idx0 = 4031105;
				cnt0 = 6;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7111404;
				taid = 1;
				esctype = 0;
				idx2 = 7112407;
			}
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
				id = 13021;
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
				m1fx = "974.909973";
				m2fz = "399.470001";
				m1fy = "0.000000";
				sort = 1100024;
				m2widx = 80000;
				grade = -1;
				m0fx = "905.409973";
				m0ttip = 1100265;
				m1pat = 2;
				m1widx = 80000;
				rwd = 255;
				taid = 2;
				title = 1100261;
				gtype = -1;
				area = 1100262;
				goal = 1100263;
				m0fz = "770.080017";
				m0widx = 80000;
				m1ttip = 1100265;
				m2fy = "0.000000";
				m2pat = 2;
				stype = 4;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "550.450012";
				m2fx = "947.200012";
				m2ttip = 1100265;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 3;
				eitype = 0;
				idx0 = 4031105;
				cnt0 = 6;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7111404;
				taid = 1;
				esctype = 0;
				idx2 = 7112400;
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
				id = 13023;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
	}
}

