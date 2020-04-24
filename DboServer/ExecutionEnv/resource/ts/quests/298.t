CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 298;
	title = 29802;

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
				conv = 29809;
				ctype = 1;
				idx = 4751102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 29808;
				gtype = 1;
				area = 29801;
				goal = 29804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 29805;
				stype = 1;
				taid = 1;
				title = 29802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 29814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 29801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751308;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 29807;
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
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4751102;";
			}
			CDboTSCheckClrQst
			{
				and = "289;294;";
				flink = 1;
				not = 0;
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
			cont = 29808;
			gtype = 1;
			oklnk = 2;
			area = 29801;
			goal = 29804;
			sort = 29805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 29802;
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

