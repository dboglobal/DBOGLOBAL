CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 390;
	title = 39002;

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
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 39020;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 39009;
				ctype = 1;
				idx = 8512103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 39008;
				gtype = 1;
				area = 39001;
				goal = 39004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 39005;
				stype = 64;
				taid = 1;
				title = 39002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 39008;
			gtype = 1;
			oklnk = 2;
			area = 39001;
			goal = 39004;
			sort = 39005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 39002;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSTeleport
			{
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 39014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 39001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8512103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 39007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "8512103;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
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

