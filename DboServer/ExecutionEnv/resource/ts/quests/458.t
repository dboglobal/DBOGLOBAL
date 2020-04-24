CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 458;
	title = 45802;

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
			stdiag = 45807;
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
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "4151201;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 45814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 45801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "570;";
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
				conv = 45809;
				ctype = 1;
				idx = 4151201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 45808;
				gtype = 1;
				area = 45801;
				goal = 45804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 45805;
				stype = 1;
				taid = 1;
				title = 45802;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 45808;
			gtype = 1;
			oklnk = 2;
			area = 45801;
			goal = 45804;
			sort = 45805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 45802;
		}
	}
}

