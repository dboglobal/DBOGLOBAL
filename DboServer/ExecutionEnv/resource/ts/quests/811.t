CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 811;
	title = 81102;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 81109;
				ctype = 1;
				idx = 3031207;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 9411100;
				cnt0 = 7;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 9411300;
				taid = 3;
				esctype = 0;
				idx2 = 9411200;
			}
			CDboTSActRegQInfo
			{
				cont = 81108;
				m1fx = "7493.000000";
				m2fz = "2182.000000";
				m1fy = "0.000000";
				sort = 81105;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 81101;
				goal = 81104;
				m0fz = "2182.000000";
				m0widx = 1;
				m1ttip = 81116;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "2182.000000";
				m2fx = "7493.000000";
				m2ttip = 81117;
				m0fx = "7493.000000";
				m0ttip = 81115;
				title = 81102;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 81108;
			gtype = 2;
			oklnk = 2;
			area = 81101;
			goal = 81104;
			sort = 81105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 81102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 81114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 81101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653101;";
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
			stdiag = 81107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 28;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "812;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3031207;";
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

