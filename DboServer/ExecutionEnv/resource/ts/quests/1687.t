CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1687;
	title = 168702;

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
				conv = 168709;
				ctype = 1;
				idx = 7135201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 168708;
				gtype = 1;
				area = 168701;
				goal = 168704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 168705;
				stype = 2;
				taid = 1;
				title = 168702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 168714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 168701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5311101;";
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
			cont = 168708;
			gtype = 1;
			oklnk = 2;
			area = 168701;
			goal = 168704;
			sort = 168705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 168702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 168707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "7135201;";
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
	}
}

