CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 46;
	title = 4602;

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
				cont = 4608;
				gtype = 1;
				area = 4601;
				goal = 4604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 4605;
				stype = 1;
				taid = 1;
				title = 4602;
			}
			CDboTSActObjConv
			{
				conv = 4609;
				idx = 15;
				widx = 1;
				ctype = 1;
				taid = 2;
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
			cont = 4608;
			gtype = 1;
			oklnk = 2;
			area = 4601;
			goal = 4604;
			sort = 4605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 4602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 4614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 4601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511504;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 4607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 8;
			}
			CDboTSClickObject
			{
				objidx = "15;";
				widx = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
		}
	}
}

