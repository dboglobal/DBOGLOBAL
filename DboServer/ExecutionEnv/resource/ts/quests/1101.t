CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1101;
	title = 110102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 110109;
				ctype = 1;
				idx = 5061102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 110108;
				gtype = 1;
				area = 110101;
				goal = 110104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 110105;
				stype = 2;
				taid = 1;
				title = 110102;
			}
			CDboTSActSToCEvt
			{
				dt2 = 110126;
				otype2 = 7;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 7;
				widx2 = 1;
				dt0 = 110122;
				esctype = 0;
				iidx1 = -1;
				it1 = 110123;
				otype0 = 7;
				tidx2 = 376;
				dt1 = 110124;
				etype = 0;
				iidx2 = -1;
				it0 = 110121;
				taid = 3;
				tidx1 = 366;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 363;
				ectype = -1;
				it2 = 110125;
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 110108;
			gtype = 1;
			oklnk = 2;
			area = 110101;
			goal = 110104;
			sort = 110105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 110102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 110114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 110101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 110107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "5061102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

