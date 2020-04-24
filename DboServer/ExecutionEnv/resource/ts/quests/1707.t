CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1707;
	title = 170702;

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
				eitype = 3;
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
			stdiag = 170707;
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
				npcidx = "6311201;";
			}
			CDboTSCheckClrQst
			{
				and = "1706;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 170709;
				ctype = 1;
				idx = 6311201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1015;
				cnt0 = 9;
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
				cont = 170708;
				m1fx = "-892.000000";
				m1fy = "-95.000000";
				sort = 170705;
				m0fy = "-82.000000";
				m0pat = 2;
				m1fz = "-755.000000";
				grade = 132203;
				m0fx = "-746.000000";
				m0ttip = 170715;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 170702;
				gtype = 2;
				area = 170701;
				goal = 170704;
				m0fz = "-654.000000";
				m0widx = 1;
				m1ttip = 170715;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 170714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 170701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "6311201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 170708;
			gtype = 2;
			oklnk = 2;
			area = 170701;
			goal = 170704;
			sort = 170705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 170702;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 881;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 722;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 170707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
	}
}

