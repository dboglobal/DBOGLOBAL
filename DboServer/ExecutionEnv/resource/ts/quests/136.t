CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 136;
	title = 13602;

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
				conv = 13609;
				ctype = 1;
				idx = 5062103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 13608;
				gtype = 1;
				area = 13601;
				goal = 13604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 13605;
				stype = 1;
				taid = 1;
				title = 13602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 13607;
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
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "5062103;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "137;";
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
			cont = 13608;
			gtype = 1;
			oklnk = 2;
			area = 13601;
			goal = 13604;
			sort = 13605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 13602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 13614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 13601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142203;";
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

