CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1898;
	title = 189802;

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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 189814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 189801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1501103;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 189809;
				ctype = 1;
				idx = 1501103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 2;
				eitype = 0;
				idx0 = 7114128;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7114209;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 189808;
				m3widx = 4;
				m1fy = "-74.029999";
				m2widx = 4;
				grade = 132203;
				m1pat = 2;
				m1widx = 4;
				m3fx = "-303.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "199.000000";
				m4ttip = 189816;
				area = 189801;
				goal = 189804;
				m0fz = "-132.000000";
				m0widx = 4;
				m1ttip = 189816;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "153.000000";
				m3ttip = 189815;
				scitem = -1;
				stype = 2;
				m0fy = "-73.730003";
				m0pat = 2;
				m1fz = "421.000000";
				m2fx = "189.000000";
				m2ttip = 189815;
				m4widx = 4;
				m0fx = "245.000000";
				m0ttip = 189815;
				m3pat = 2;
				m4pat = 2;
				title = 189802;
				m1fx = "-90.000000";
				m2fz = "47.000000";
				sort = 189805;
				m4fz = "-243.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 189808;
			gtype = 3;
			oklnk = 2;
			area = 189801;
			goal = 189804;
			sort = 189805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 189802;
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 189807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSCheckClrQst
			{
				and = "1897;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1501103;";
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

