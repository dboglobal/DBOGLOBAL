CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 438;
	title = 43802;

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
				conv = 43809;
				ctype = 1;
				idx = 4261101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 43808;
				gtype = 1;
				area = 43801;
				goal = 43804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 43805;
				stype = 2;
				taid = 1;
				title = 43802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 43807;
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
				minlvl = 8;
			}
			CDboTSClickNPC
			{
				npcidx = "4261101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 43808;
			gtype = 1;
			oklnk = 2;
			area = 43801;
			goal = 43804;
			sort = 43805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 43802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 43814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 43801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151201;";
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

