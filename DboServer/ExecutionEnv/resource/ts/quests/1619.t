CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1619;
	title = 161902;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 161908;
			gtype = 3;
			oklnk = 2;
			area = 161901;
			goal = 161904;
			sort = 161905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 161902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 161914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 161901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4952101;";
			}
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
			stdiag = 161907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "4952101;";
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
				conv = 161909;
				ctype = 1;
				idx = 4952101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 6212112;
				cnt0 = 5;
				cnt2 = 5;
				ectype = -1;
				etype = 0;
				idx1 = 4261107;
				taid = 3;
				esctype = 0;
				idx2 = 1781104;
			}
			CDboTSActRegQInfo
			{
				cont = 161908;
				m3widx = 10;
				m1fy = "0.000000";
				m2widx = 10;
				grade = 132203;
				m1pat = 2;
				m1widx = 10;
				m3fx = "-404.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "-449.000000";
				m4ttip = 161916;
				area = 161901;
				goal = 161904;
				m0fz = "-515.000000";
				m0widx = 10;
				m1ttip = 161916;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-613.000000";
				m3ttip = 161915;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-515.000000";
				m2fx = "-225.000000";
				m2ttip = 161917;
				m4widx = 10;
				m0fx = "-254.000000";
				m0ttip = 161915;
				m3pat = 2;
				m4pat = 2;
				title = 161902;
				m1fx = "-254.000000";
				m2fz = "-646.000000";
				sort = 161905;
				m4fz = "-415.000000";
			}
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

