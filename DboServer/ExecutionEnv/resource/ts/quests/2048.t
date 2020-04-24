CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 2048;
	title = 204802;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 204807;
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
				raceflg = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "7571111;";
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
			cont = 204808;
			gtype = 2;
			oklnk = 2;
			area = 204801;
			goal = 204804;
			sort = 204805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 204802;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 204814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 204801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7571111;";
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
				conv = 204809;
				ctype = 1;
				idx = 7571111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 204808;
				gtype = 2;
				area = 204801;
				goal = 204804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 204805;
				stype = 64;
				taid = 1;
				title = 204802;
			}
		}
	}
}

