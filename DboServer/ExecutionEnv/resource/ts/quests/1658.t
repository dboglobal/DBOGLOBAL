CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1658;
	title = 165802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
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
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 165807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "7135201;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 165814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 165801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7135201;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 165809;
				ctype = 1;
				idx = 7135201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 3501109;
				cnt0 = 7;
				cnt2 = 5;
				ectype = -1;
				etype = 0;
				idx1 = 3503100;
				taid = 3;
				esctype = 0;
				idx2 = 3503105;
			}
			CDboTSActRegQInfo
			{
				cont = 165808;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "-1947.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-1789.000000";
				m4ttip = 165817;
				area = 165801;
				goal = 165804;
				m0fz = "-772.000000";
				m0widx = 1;
				m1ttip = 165816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-557.000000";
				m3ttip = 165815;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-458.000000";
				m2fx = "-1544.000000";
				m2ttip = 165817;
				m4widx = 1;
				m0fx = "-1729.000000";
				m0ttip = 165815;
				m3pat = 2;
				m4pat = 2;
				title = 165802;
				m1fx = "-2023.000000";
				m2fz = "-577.000000";
				sort = 165805;
				m4fz = "-603.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 165808;
			gtype = 2;
			oklnk = 2;
			area = 165801;
			goal = 165804;
			sort = 165805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 165802;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

