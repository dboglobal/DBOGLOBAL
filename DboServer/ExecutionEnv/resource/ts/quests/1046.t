CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1046;
	title = 104602;

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
				conv = 104609;
				ctype = 1;
				idx = 3131201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				widx1 = 1;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 104621;
				esctype = 0;
				iidx1 = -1;
				it1 = 104622;
				otype0 = 1;
				dt1 = 104623;
				etype = 0;
				it0 = 104620;
				taid = 3;
				tidx1 = 1752101;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3172301;
			}
			CDboTSActRegQInfo
			{
				cont = 104608;
				gtype = 2;
				area = 104601;
				goal = 104604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 104605;
				stype = 1;
				taid = 1;
				title = 104602;
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
			cont = 104608;
			gtype = 2;
			oklnk = 2;
			area = 104601;
			goal = 104604;
			sort = 104605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 104602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 104614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 104601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3131201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 104607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "3131201;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
	}
}

