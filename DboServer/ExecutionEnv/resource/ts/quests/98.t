CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 98;
	title = 9802;

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
				conv = 9809;
				ctype = 1;
				idx = 6312102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 9808;
				gtype = 1;
				area = 9801;
				goal = 9804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 9805;
				stype = 2;
				taid = 1;
				title = 9802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 9807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 14;
			}
			CDboTSClickNPC
			{
				npcidx = "6312102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 9808;
			gtype = 1;
			oklnk = 2;
			area = 9801;
			goal = 9804;
			sort = 9805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 9802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 9814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 9801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072122;";
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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

