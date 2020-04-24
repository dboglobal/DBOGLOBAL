CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1631;
	title = 163102;

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
			cont = 163108;
			gtype = 2;
			oklnk = 2;
			area = 163101;
			goal = 163104;
			sort = 163105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 163102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 163114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 163101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242103;";
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
			stdiag = 163107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSCheckClrQst
			{
				and = "1630;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3242103;";
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
				conv = 163109;
				ctype = 1;
				idx = 3242103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7212103;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 163108;
				m1fx = "-2108.000000";
				m1fy = "0.000000";
				sort = 163105;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "337.000000";
				grade = 132203;
				m0fx = "-2132.000000";
				m0ttip = 163115;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 163102;
				gtype = 2;
				area = 163101;
				goal = 163104;
				m0fz = "134.000000";
				m0widx = 1;
				m1ttip = 163115;
				scitem = -1;
				stype = 2;
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

