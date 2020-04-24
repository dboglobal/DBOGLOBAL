CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2596;
	title = 259602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				dt2 = 259625;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 259621;
				esctype = 0;
				iidx1 = -1;
				it1 = 259622;
				otype0 = 1;
				tidx2 = 3172303;
				dt1 = 259623;
				etype = 0;
				iidx2 = -1;
				it0 = 259620;
				taid = 3;
				tidx1 = 3172206;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3172108;
				ectype = -1;
				it2 = 259624;
			}
			CDboTSActRegQInfo
			{
				cont = 259608;
				gtype = 2;
				area = 259601;
				goal = 259604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 259605;
				stype = 1;
				taid = 1;
				title = 259602;
			}
			CDboTSActNPCConv
			{
				conv = 259609;
				ctype = 1;
				idx = 8531101;
				taid = 2;
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
				itype = 6;
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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
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
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 259608;
			gtype = 2;
			oklnk = 2;
			area = 259601;
			goal = 259604;
			sort = 259605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 259602;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 259607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2597;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "8531101;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 259614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 259601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8531101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

