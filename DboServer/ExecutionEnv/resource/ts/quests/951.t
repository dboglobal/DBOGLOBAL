CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 951;
	title = 95102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 95125;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 95121;
				esctype = 0;
				iidx1 = -1;
				it1 = 95122;
				otype0 = 1;
				tidx2 = 5592101;
				dt1 = 95123;
				etype = 0;
				iidx2 = -1;
				it0 = 95120;
				taid = 1;
				tidx1 = 4072106;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3331207;
				ectype = -1;
				it2 = 95124;
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
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 95109;
				ctype = 1;
				idx = 3612101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 95108;
				gtype = 1;
				area = 95101;
				goal = 95104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 95105;
				stype = 1;
				taid = 1;
				title = 95102;
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
			cont = 95108;
			gtype = 1;
			oklnk = 2;
			area = 95101;
			goal = 95104;
			sort = 95105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 95102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 95114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 95101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3612101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 95107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 34;
			}
			CDboTSClickNPC
			{
				npcidx = "3612101;";
			}
		}
	}
}

