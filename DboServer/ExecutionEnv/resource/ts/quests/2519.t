CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2519;
	title = 251902;

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
			stdiag = 251907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
			CDboTSCheckClrQst
			{
				and = "2518;";
				flink = 1;
				flinknextqid = "2520;";
				not = 0;
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
				conv = 251909;
				ctype = 1;
				idx = 3181108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 9;
				eitype = 0;
				idx0 = 1832104;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1831202;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 251908;
				m3widx = 14;
				m1fy = "0.000000";
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				m3fx = "-395.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-755.000000";
				m4ttip = 251915;
				area = 251901;
				goal = 251904;
				m0fz = "-1072.000000";
				m0widx = 14;
				m1ttip = 251916;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-588.000000";
				m3ttip = 251915;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-770.000000";
				m2fx = "-448.000000";
				m2ttip = 251916;
				m4widx = 14;
				m0fx = "-746.000000";
				m0ttip = 251916;
				m3pat = 2;
				m4pat = 2;
				title = 251902;
				m1fx = "-549.000000";
				m2fz = "-917.000000";
				sort = 251905;
				m4fz = "-780.000000";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 251914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 251901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 251908;
			gtype = 2;
			oklnk = 2;
			area = 251901;
			goal = 251904;
			sort = 251905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 251902;
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

