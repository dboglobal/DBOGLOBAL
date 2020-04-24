CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1049;
	title = 104902;

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
				conv = 104909;
				ctype = 1;
				idx = 4072114;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 104908;
				gtype = 1;
				area = 104901;
				goal = 104904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 104905;
				stype = 2;
				taid = 1;
				title = 104902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 104914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 104901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3143102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 104907;
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
				npcidx = "4072114;";
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
			cont = 104908;
			gtype = 1;
			oklnk = 2;
			area = 104901;
			goal = 104904;
			sort = 104905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 104902;
		}
	}
}

