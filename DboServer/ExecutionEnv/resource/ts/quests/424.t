CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 424;
	title = 42402;

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
			stdiag = 42407;
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
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "4191117;";
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
				conv = 42409;
				ctype = 1;
				idx = 4191117;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 42408;
				gtype = 1;
				area = 42401;
				goal = 42404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 42405;
				stype = 1;
				taid = 1;
				title = 42402;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 42414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 42401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "569;";
				widx = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 42408;
			gtype = 1;
			oklnk = 2;
			area = 42401;
			goal = 42404;
			sort = 42405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 42402;
		}
	}
}

