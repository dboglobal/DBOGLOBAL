CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2981;
	title = 298102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 298109;
				ctype = 1;
				idx = 4501108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 298108;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4948.640137";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "4736.580078";
				m4ttip = 298117;
				area = 298101;
				goal = 298104;
				m0fz = "-4013.989990";
				m0widx = 1;
				m1ttip = 298115;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4070.929932";
				m3ttip = 298117;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4248.819824";
				m2fx = "4765.410156";
				m2ttip = 298116;
				m4widx = 1;
				m0fx = "4910.890137";
				m0ttip = 298115;
				m3pat = 2;
				m4pat = 2;
				title = 298102;
				m1fx = "4721.799805";
				m2fz = "-4252.970215";
				sort = 298105;
				m4fz = "-4233.759766";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 1;
				idx0 = 1569;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 1570;
				taid = 3;
				esctype = 0;
				idx2 = 1543;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
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
				eitype = 1;
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
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 298108;
			gtype = 3;
			oklnk = 2;
			area = 298101;
			goal = 298104;
			sort = 298105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 298102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 298107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4501108;";
			}
			CDboTSCheckClrQst
			{
				and = "2980;";
				flink = 1;
				not = 0;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 298114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 298101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4501108;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 298107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 1569;
				iidx2 = 1543;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 1570;
				iprob0 = "1.000000";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

