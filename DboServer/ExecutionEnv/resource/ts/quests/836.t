CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 836;
	title = 83602;

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
				conv = 83609;
				ctype = 1;
				idx = 7132102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 26;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 83602;
				taid = 3;
				widx0 = 1;
				dt0 = 83620;
				esctype = 0;
				otype0 = 7;
			}
			CDboTSActRegQInfo
			{
				cont = 83608;
				gtype = 1;
				area = 83601;
				goal = 83604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 83605;
				stype = 1;
				taid = 1;
				title = 83602;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 83607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "7132102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 83608;
			gtype = 1;
			oklnk = 2;
			area = 83601;
			goal = 83604;
			sort = 83605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 83602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 83614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 83601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7132102;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
	}
}

