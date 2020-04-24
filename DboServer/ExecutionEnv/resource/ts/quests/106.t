CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 106;
	title = 10602;

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
				conv = 10609;
				ctype = 1;
				idx = 3142102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 10608;
				gtype = 1;
				area = 10601;
				goal = 10604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 10605;
				stype = 1;
				taid = 1;
				title = 10602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 10614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 10601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7611101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 10607;
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
				minlvl = 15;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "107;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3142102;";
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 10608;
			gtype = 1;
			oklnk = 2;
			area = 10601;
			goal = 10604;
			sort = 10605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 10602;
		}
	}
}

