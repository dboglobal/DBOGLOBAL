CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 2046;
	title = 204602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 204607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "7571105;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 10010000;
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
			cont = 204608;
			gtype = 2;
			oklnk = 2;
			area = 204601;
			goal = 204604;
			sort = 204605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 204602;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 204614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 204601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7571105;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 204609;
				ctype = 1;
				idx = 7571105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 204608;
				gtype = 2;
				area = 204601;
				goal = 204604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 204605;
				stype = 64;
				taid = 1;
				title = 204602;
			}
		}
	}
}

