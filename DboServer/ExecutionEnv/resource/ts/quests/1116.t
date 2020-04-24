CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1116;
	title = 111602;

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
			stdiag = 111607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "3612103;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 111614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 111601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3612103;";
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
				conv = 111609;
				ctype = 1;
				idx = 3612103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 111608;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "-520.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-223.000000";
				m4ttip = 111615;
				area = 111601;
				goal = 111604;
				m0fz = "1561.000000";
				m0widx = 1;
				m1ttip = 111616;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "2352.000000";
				m3ttip = 111616;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1561.000000";
				m2fx = "-520.000000";
				m2ttip = 111615;
				m4widx = 1;
				m0fx = "235.000000";
				m0ttip = 111615;
				m3pat = 2;
				m4pat = 2;
				title = 111602;
				m1fx = "235.000000";
				m2fz = "2352.000000";
				sort = 111605;
				m4fz = "2313.000000";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 5813202;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5813302;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 111608;
			gtype = 2;
			oklnk = 2;
			area = 111601;
			goal = 111604;
			sort = 111605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 111602;
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

