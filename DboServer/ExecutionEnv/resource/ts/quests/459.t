CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 459;
	title = 45902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 45908;
				gtype = 1;
				area = 45901;
				goal = 45904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 45905;
				stype = 2;
				taid = 1;
				title = 45902;
			}
			CDboTSActNPCConv
			{
				conv = 45909;
				ctype = 1;
				idx = 4151201;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 45908;
			gtype = 1;
			oklnk = 2;
			area = 45901;
			goal = 45904;
			sort = 45905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 45902;
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
			desc = 45914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 45901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372207;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 45907;
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
				minlvl = 11;
			}
			CDboTSClickNPC
			{
				npcidx = "4151201;";
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
	}
}

