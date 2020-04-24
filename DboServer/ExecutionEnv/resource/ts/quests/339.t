CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 339;
	title = 33902;

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
			stdiag = 33907;
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
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "4752201;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 33914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 33901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "60;";
				widx = 1;
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
				conv = 33909;
				ctype = 1;
				idx = 4752201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 33908;
				gtype = 1;
				area = 33901;
				goal = 33904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 33905;
				stype = 1;
				taid = 1;
				title = 33902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 33908;
			gtype = 1;
			oklnk = 2;
			area = 33901;
			goal = 33904;
			sort = 33905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 33902;
		}
	}
}

