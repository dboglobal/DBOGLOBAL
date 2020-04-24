CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1435;
	title = 143502;

	CNtlTSGroup
	{
		gid = 0;

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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 143507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "5531101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 143509;
				ctype = 1;
				idx = 5531101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 143508;
				gtype = 1;
				area = 143501;
				goal = 143504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 143505;
				stype = 1;
				taid = 1;
				title = 143502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 143514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 143501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "57;";
				widx = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 143508;
			gtype = 1;
			oklnk = 2;
			area = 143501;
			goal = 143504;
			sort = 143505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 143502;
		}
	}
}

