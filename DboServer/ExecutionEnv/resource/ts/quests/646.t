CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 646;
	title = 64602;

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
				cont = 64608;
				gtype = 1;
				area = 64601;
				goal = 64604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 64605;
				stype = 2;
				taid = 1;
				title = 64602;
			}
			CDboTSActNPCConv
			{
				conv = 64609;
				ctype = 1;
				idx = 7131109;
				taid = 2;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 64614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 64601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3032103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 64607;
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
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "7131109;";
			}
			CDboTSCheckClrQst
			{
				and = "610;";
				flink = 1;
				flinknextqid = "648;";
				not = 0;
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
			cont = 64608;
			gtype = 1;
			oklnk = 2;
			area = 64601;
			goal = 64604;
			sort = 64605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 64602;
		}
	}
}

