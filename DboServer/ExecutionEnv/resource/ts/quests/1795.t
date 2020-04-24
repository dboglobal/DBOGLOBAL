CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1795;
	title = 179502;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 179507;
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
				npcidx = "3143108;";
			}
			CDboTSCheckClrQst
			{
				and = "1794;";
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
				conv = 179509;
				ctype = 1;
				idx = 3143108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 10;
				eitype = 0;
				idx0 = 7111109;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7112116;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 179508;
				m1fx = "-2702.000000";
				m1fy = "0.000000";
				sort = 179505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-21.780001";
				grade = 132203;
				m0fx = "-2702.000000";
				m0ttip = 179515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 179502;
				gtype = 3;
				area = 179501;
				goal = 179504;
				m0fz = "-21.780001";
				m0widx = 1;
				m1ttip = 179516;
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
			desc = 179514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 179501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7091105;";
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
			cont = 179508;
			gtype = 3;
			oklnk = 2;
			area = 179501;
			goal = 179504;
			sort = 179505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 179502;
		}
	}
}

