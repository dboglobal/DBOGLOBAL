CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 468;
	title = 46802;

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
				conv = 46809;
				ctype = 1;
				idx = 7153103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 46808;
				gtype = 1;
				area = 46801;
				goal = 46804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 46805;
				stype = 2;
				taid = 1;
				title = 46802;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 46807;
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
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "7153103;";
			}
			CDboTSCheckClrQst
			{
				and = "463;";
				flink = 1;
				flinknextqid = "469;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 46808;
			gtype = 1;
			oklnk = 2;
			area = 46801;
			goal = 46804;
			sort = 46805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 46802;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 46814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 46801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372207;";
			}
		}
	}
}

