CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 404;
	title = 40402;

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
				conv = 40409;
				ctype = 1;
				idx = 7711101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 40408;
				gtype = 1;
				area = 40401;
				goal = 40404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 40405;
				stype = 64;
				taid = 1;
				title = 40402;
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
			stdiag = 40407;
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
				minlvl = 2;
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
			cont = 40408;
			gtype = 1;
			oklnk = 2;
			area = 40401;
			goal = 40404;
			sort = 40405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 40401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031301;";
			}
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

