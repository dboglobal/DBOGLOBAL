CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 968;
	title = 96802;

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
				conv = 96809;
				ctype = 1;
				idx = 7131118;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 96808;
				gtype = 1;
				area = 96801;
				goal = 96804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 96805;
				stype = 2;
				taid = 1;
				title = 96802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 96807;
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
				npcidx = "7131118;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 96808;
			gtype = 1;
			oklnk = 2;
			area = 96801;
			goal = 96804;
			sort = 96805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 96802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 96814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 96801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1752101;";
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

