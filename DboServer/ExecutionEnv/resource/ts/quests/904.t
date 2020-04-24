CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 904;
	title = 90402;

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
				conv = 90409;
				ctype = 1;
				idx = 7131117;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 90408;
				gtype = 1;
				area = 90401;
				goal = 90404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 90405;
				stype = 1;
				taid = 1;
				title = 90402;
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 90408;
			gtype = 1;
			oklnk = 2;
			area = 90401;
			goal = 90404;
			sort = 90405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 90402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 90414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 90401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031210;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 90407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSClickNPC
			{
				npcidx = "7131117;";
			}
		}
	}
}

