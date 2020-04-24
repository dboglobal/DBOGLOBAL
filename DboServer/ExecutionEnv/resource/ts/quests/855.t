CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 855;
	title = 85502;

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
			stdiag = 85507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "1431108;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 85514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 85501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1431108;";
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
				conv = 85509;
				ctype = 1;
				idx = 1431108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 1711101;
				cnt0 = 5;
				cnt2 = 9;
				ectype = -1;
				etype = 0;
				idx1 = 2132101;
				taid = 3;
				esctype = 0;
				idx2 = 2133100;
			}
			CDboTSActRegQInfo
			{
				cont = 85508;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4905.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "4905.000000";
				m4ttip = 85517;
				area = 85501;
				goal = 85504;
				m0fz = "1650.000000";
				m0widx = 1;
				m1ttip = 85516;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "1665.000000";
				m3ttip = 85515;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1650.000000";
				m2fx = "4925.000000";
				m2ttip = 85517;
				m4widx = 1;
				m0fx = "4925.000000";
				m0ttip = 85515;
				m3pat = 2;
				m4pat = 2;
				title = 85502;
				m1fx = "4925.000000";
				m2fz = "1650.000000";
				sort = 85505;
				m4fz = "1665.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 85508;
			gtype = 2;
			oklnk = 2;
			area = 85501;
			goal = 85504;
			sort = 85505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 85502;
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

