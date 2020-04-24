CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 346;
	title = 34602;

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
				conv = 34609;
				ctype = 1;
				idx = 4751308;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 34608;
				gtype = 1;
				area = 34601;
				goal = 34604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 34605;
				stype = 1;
				taid = 1;
				title = 34602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 34607;
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
				minlvl = 19;
			}
			CDboTSClickNPC
			{
				npcidx = "4751308;";
			}
			CDboTSCheckClrQst
			{
				and = "338;";
				flink = 1;
				flinknextqid = "347;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 34608;
			gtype = 1;
			oklnk = 2;
			area = 34601;
			goal = 34604;
			sort = 34605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 34602;
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
			desc = 34614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 34601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511503;";
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
	}
}

