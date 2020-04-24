CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 421;
	title = 42102;

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
				conv = 42109;
				ctype = 1;
				idx = 7711101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 42108;
				gtype = 1;
				area = 42101;
				goal = 42104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 42105;
				stype = 1;
				taid = 1;
				title = 42102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 42114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 42101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2413108;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 42107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "7711101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 42108;
			gtype = 1;
			oklnk = 2;
			area = 42101;
			goal = 42104;
			sort = 42105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 42102;
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
	}
}

