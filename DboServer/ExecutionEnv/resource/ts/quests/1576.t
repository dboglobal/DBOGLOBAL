CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1576;
	title = 157602;

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
				conv = 157609;
				ctype = 1;
				idx = 1111116;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 157608;
				gtype = 1;
				area = 157601;
				goal = 157604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 157605;
				stype = 1;
				taid = 1;
				title = 157602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 157607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "1111116;";
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
			cont = 157608;
			gtype = 1;
			oklnk = 2;
			area = 157601;
			goal = 157604;
			sort = 157605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 157602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 157614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 157601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3222102;";
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

