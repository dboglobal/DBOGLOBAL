CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2176;
	title = 217602;

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
			stdiag = 217607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "3031406;";
			}
			CDboTSCheckClrQst
			{
				and = "2175;";
				flink = 1;
				flinknextqid = "2178;";
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
				conv = 217609;
				ctype = 1;
				idx = 3031406;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8913203;
				cnt0 = 13;
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
				cont = 217608;
				m1fx = "-1117.000000";
				m1fy = "0.000000";
				sort = 217605;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2647.000000";
				grade = 132203;
				m0fx = "-1079.000000";
				m0ttip = 217615;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 217602;
				gtype = 2;
				area = 217601;
				goal = 217604;
				m0fz = "-2502.000000";
				m0widx = 1;
				m1ttip = 217615;
				scitem = -1;
				stype = 1;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 217614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 217601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031406;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 217608;
			gtype = 2;
			oklnk = 2;
			area = 217601;
			goal = 217604;
			sort = 217605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 217602;
		}
	}
}

