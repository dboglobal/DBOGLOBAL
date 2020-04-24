CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 471;
	title = 47102;

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
				conv = 47109;
				ctype = 1;
				idx = 4371311;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 47108;
				gtype = 1;
				area = 47101;
				goal = 47104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 47105;
				stype = 2;
				taid = 1;
				title = 47102;
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
			desc = 47114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 47101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5651101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 47107;
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
				npcidx = "4371311;";
			}
			CDboTSCheckClrQst
			{
				and = "466;";
				flink = 1;
				flinknextqid = "472;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 47108;
			gtype = 1;
			oklnk = 2;
			area = 47101;
			goal = 47104;
			sort = 47105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 47102;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

