CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2638;
	title = 263802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 263809;
				ctype = 1;
				idx = 4753104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 263808;
				gtype = 1;
				area = 263801;
				goal = 263804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 263805;
				stype = 2;
				taid = 1;
				title = 263802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 263814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 263801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511518;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 263807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4753104;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
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
			prelnk = "100;";
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
			cont = 263808;
			gtype = 1;
			oklnk = 2;
			area = 263801;
			goal = 263804;
			sort = 263805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 263802;
		}
	}
}

