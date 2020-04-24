CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2215;
	title = 221502;

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
			stdiag = 221507;
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
				npcidx = "4651209;";
			}
			CDboTSCheckClrQst
			{
				and = "2214;";
				flink = 1;
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
				conv = 221509;
				ctype = 1;
				idx = 4651209;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 221508;
				m1fx = "-2138.000000";
				m1fy = "0.000000";
				sort = 221505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2592.000000";
				grade = 132203;
				m0fx = "-1964.000000";
				m0ttip = 221515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 221502;
				gtype = 2;
				area = 221501;
				goal = 221504;
				m0fz = "-2538.000000";
				m0widx = 1;
				m1ttip = 221515;
				scitem = -1;
				stype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8912204;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 221514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 221501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 221508;
			gtype = 2;
			oklnk = 2;
			area = 221501;
			goal = 221504;
			sort = 221505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 221502;
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

