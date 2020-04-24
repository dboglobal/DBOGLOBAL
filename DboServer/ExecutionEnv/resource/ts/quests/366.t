CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 366;
	title = 36602;

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
				conv = 36609;
				ctype = 1;
				idx = 4651201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 36608;
				gtype = 1;
				area = 36601;
				goal = 36604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 36605;
				stype = 2;
				taid = 1;
				title = 36602;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 36607;
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
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "4651201;";
			}
			CDboTSCheckClrQst
			{
				and = "365;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 36608;
			gtype = 1;
			oklnk = 2;
			area = 36601;
			goal = 36604;
			sort = 36605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 36602;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 36614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 36601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
		}
	}
}

