CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3775;
	title = 377502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
				conv = 377509;
				ctype = 1;
				idx = 5062201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1588;
				cnt0 = 15;
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
				cont = 377508;
				m1fx = "-4958.549805";
				m2fz = "-5574.100098";
				m1fy = "0.000000";
				sort = 377505;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 377501;
				goal = 377504;
				m0fz = "-5515.339844";
				m0widx = 15;
				m1ttip = 377515;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5549.390137";
				m2fx = "-5067.180176";
				m2ttip = 377515;
				m0fx = "-5117.819824";
				m0ttip = 377515;
				title = 377502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377508;
			gtype = 2;
			oklnk = 2;
			area = 377501;
			goal = 377504;
			sort = 377505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 377502;
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
			desc = 377514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 377501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5062201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "5062201;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3837;";
				not = 0;
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
				eitype = 3;
				idx0 = 1587;
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
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1588;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377507;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
	}
}

