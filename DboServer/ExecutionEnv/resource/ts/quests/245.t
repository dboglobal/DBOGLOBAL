CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 245;
	title = 24502;

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
			stdiag = 24507;
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
				minlvl = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "4651102;";
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
				conv = 24509;
				ctype = 1;
				idx = 4651102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 24508;
				gtype = 1;
				area = 24501;
				goal = 24504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 24505;
				stype = 1;
				taid = 1;
				title = 24502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 24514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 24501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "59;";
				widx = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 24508;
			gtype = 1;
			oklnk = 2;
			area = 24501;
			goal = 24504;
			sort = 24505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 24502;
		}
	}
}

